// Parts 3c, 3e, 3f: elevator worker, synchronization and scheduling.
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/errno.h>
#include <linux/compiler.h>
#include "pet_list.h"

extern int (*STUB_start_elevator)(void);
extern int (*STUB_issue_request)(int, int, int);
extern int (*STUB_stop_elevator)(void);

enum elevator_state { OFFLINE, IDLE, LOADING, UP, DOWN };
static const char * const state_names[] = {
    "OFFLINE", "IDLE", "LOADING", "UP", "DOWN"
};
static DEFINE_MUTEX(elevator_lock);
static DECLARE_WAIT_QUEUE_HEAD(elevator_wait);
static struct building building;
static struct task_struct *worker;
static struct proc_dir_entry *proc_entry;
static enum elevator_state state = OFFLINE;
static int current_floor = 1;
static bool stopping;
static bool active;

// Called with elevator_lock held. Onboard destinations take priority.
static int choose_target(void)
{
    struct pet *p;
    int floor, best = 0, distance = NUM_FLOORS + 1;

    list_for_each_entry(p, &building.elevator, list) {
        int d = abs(p->dest_floor - current_floor);
        if (d < distance) {
            best = p->dest_floor;
            distance = d;
        }
    }
    if (best || stopping)
        return best;

    for (floor = 1; floor <= NUM_FLOORS; floor++) {
        int d;
        if (!floor_count(&building, floor))
            continue;
        d = abs(floor - current_floor);
        if (d < distance) {
            best = floor;
            distance = d;
        }
    }
    return best;
}

static int elevator_thread(void *unused)
{
    while (!kthread_should_stop()) {
        int target;
        bool service;

        mutex_lock(&elevator_lock);
        if (!active) {
            mutex_unlock(&elevator_lock);
            wait_event_interruptible(elevator_wait,
                kthread_should_stop() || READ_ONCE(active));
            continue;
        }

        // Finish the current passengers even after a stop request.
        if (stopping && building.num_on_board == 0) {
            active = false;
            stopping = false;
            state = OFFLINE;
            mutex_unlock(&elevator_lock);
            continue;
        }

        service = has_dropoff(&building, current_floor) ||
            (!stopping && next_pet_fits(&building, current_floor));
        if (service) {
            state = LOADING;
            mutex_unlock(&elevator_lock);
            msleep(1000);
            mutex_lock(&elevator_lock);
            unload_pets(&building, current_floor);
            if (!stopping)
                load_pets(&building, current_floor);
            mutex_unlock(&elevator_lock);
            continue;
        }

        target = choose_target();
        if (!target) {
            state = IDLE;
            mutex_unlock(&elevator_lock);
            wait_event_interruptible(elevator_wait,
                kthread_should_stop() || READ_ONCE(stopping) ||
                READ_ONCE(building.num_waiting) > 0 || !READ_ONCE(active));
            continue;
        }
        if (target == current_floor) {
            // The first pet cannot fit: passengers must disembark first.
            state = IDLE;
            mutex_unlock(&elevator_lock);
            wait_event_interruptible_timeout(elevator_wait,
                kthread_should_stop() || READ_ONCE(stopping), HZ);
            continue;
        }
        state = target > current_floor ? UP : DOWN;
        mutex_unlock(&elevator_lock);
        msleep(2000);
        mutex_lock(&elevator_lock);
        current_floor += (state == UP) ? 1 : -1;
        mutex_unlock(&elevator_lock);
    }
    return 0;
}

static int elevator_start(void)
{
    mutex_lock(&elevator_lock);
    if (active) {
        mutex_unlock(&elevator_lock);
        return 1;
    }
    free_all_pets(&building);
    building_init(&building);
    current_floor = 1;
    stopping = false;
    active = true;
    state = IDLE;
    mutex_unlock(&elevator_lock);
    wake_up_interruptible(&elevator_wait);
    return 0;
}

static int elevator_request(int start, int destination, int type)
{
    int result;
    mutex_lock(&elevator_lock);
    result = add_pet(&building, start, destination, type);
    mutex_unlock(&elevator_lock);
    if (!result)
        wake_up_interruptible(&elevator_wait);
    return result;
}

static int elevator_stop(void)
{
    mutex_lock(&elevator_lock);
    if (!active || stopping) {
        mutex_unlock(&elevator_lock);
        return 1;
    }
    stopping = true;
    mutex_unlock(&elevator_lock);
    wake_up_interruptible(&elevator_wait);
    return 0;
}

static int elevator_show(struct seq_file *m, void *v)
{
    char pets[10000];
    int floor;
    mutex_lock(&elevator_lock);
    seq_printf(m, "Elevator state: %s\nCurrent floor: %d\nCurrent load: %d lbs\n",
               state_names[state], current_floor, building.load);
    print_elevator(&building, pets, sizeof(pets));
    seq_printf(m, "Elevator status:%s\n", pets);
    for (floor = NUM_FLOORS; floor >= 1; floor--) {
        print_floor(&building, floor, pets, sizeof(pets));
        seq_printf(m, "[%c] Floor %d: %d%s\n",
                   current_floor == floor ? '*' : ' ', floor,
                   floor_count(&building, floor), pets);
    }
    seq_printf(m, "Number of pets: %d\nNumber of pets waiting: %d\n"
               "Number of pets serviced: %d\n", building.num_waiting,
               building.num_waiting, building.num_serviced);
    mutex_unlock(&elevator_lock);
    return 0;
}

static int elevator_open(struct inode *inode, struct file *file)
{
    return single_open(file, elevator_show, NULL);
}

static const struct proc_ops elevator_proc_ops = {
    .proc_open = elevator_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init elevator_init(void)
{
    building_init(&building);
    proc_entry = proc_create("elevator", 0444, NULL, &elevator_proc_ops);
    if (!proc_entry)
        return -ENOMEM;
    worker = kthread_run(elevator_thread, NULL, "pet_elevator");
    if (IS_ERR(worker)) {
        int err = PTR_ERR(worker);
        proc_remove(proc_entry);
        return err;
    }
    WRITE_ONCE(STUB_start_elevator, elevator_start);
    WRITE_ONCE(STUB_issue_request, elevator_request);
    WRITE_ONCE(STUB_stop_elevator, elevator_stop);
    pr_info("pet elevator loaded\n");
    return 0;
}

static void __exit elevator_exit(void)
{
    WRITE_ONCE(STUB_start_elevator, NULL);
    WRITE_ONCE(STUB_issue_request, NULL);
    WRITE_ONCE(STUB_stop_elevator, NULL);
    wake_up_interruptible(&elevator_wait);
    kthread_stop(worker);
    proc_remove(proc_entry);
    mutex_lock(&elevator_lock);
    free_all_pets(&building);
    mutex_unlock(&elevator_lock);
    pr_info("pet elevator unloaded\n");
}

module_init(elevator_init);
module_exit(elevator_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Pet elevator kernel module");
