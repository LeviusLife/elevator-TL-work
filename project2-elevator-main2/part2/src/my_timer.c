#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/timekeeping.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("cop4610t");
MODULE_DESCRIPTION("Kernel module that reports current and elapsed time");

#define ENTRY_NAME "timer"
#define PERMS 0644
#define PARENT NULL
#define BUF_LEN 256

static struct proc_dir_entry* timer_entry;

static struct timespec64 last_time; // time of previous read
static bool has_last_time = false; // first read happens

static char msg[BUF_LEN];
static int msg_len;

static ssize_t timer_read(struct file  *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	struct timespec64 ts_now;
	struct timespec64 elapsed;

	if (*ppos == 0) {
		ktime_get_real_ts64(&ts_now);
		msg_len = snprintf(msg, sizeof(msg), "current time: %lld.%09ld\n",
				(long long)ts_now.tv_sec, ts_now.tv_nsec);
		if (has_last_time) {
			elapsed.tv_sec = ts_now.tv_sec - last_time.tv_sec;
			elapsed.tv_nsec = ts_now.tv_nsec - last_time.tv_nsec;

			// borrow a second if needed (when nanoseconds go negative)
			if (elapsed.tv_nsec < 0) {
				elapsed.tv_sec -= 1;
				elapsed.tv_nsec += NSEC_PER_SEC;
			}
			msg_len += snprintf(msg + msg_len, sizeof(msg) - msg_len,
				"elapsed time: %lld.%09ld\n",
				(long long)elapsed.tv_sec, elapsed.tv_nsec);
		}

		last_time = ts_now;
		has_last_time = true;


	}

	return simple_read_from_buffer(ubuf, count, ppos, msg, msg_len);
}

static const struct proc_ops
timer_fops = {
	.proc_read = timer_read,
};

static int __init timer_init(void)
{
	timer_entry = proc_create(ENTRY_NAME, PERMS, PARENT, &timer_fops);
	if (!timer_entry) {
	return -ENOMEM;
	}
	return 0;
}


static void __exit timer_exit(void)
{
	proc_remove(timer_entry);
}


module_init(timer_init);
module_exit(timer_exit);
