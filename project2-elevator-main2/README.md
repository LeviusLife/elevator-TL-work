# Elevator

COP4610 Project 2. 

## Group Members
- **Willa Gutowski**: wgg22@fsu.edu
- **Carter Rudolph**: cgr24@fsu.edu
- **Terryon Larkins**: tlarkins@fsu.edu
## Division of Labor

### Part 1: System Call Tracing
- **Responsibilities**: Added the 5 system calls that were necessary for project
- **Assigned to**: Willa Gutowski

### Part 2: Timer Kernel Module
- **Responsibilities**: Created the kernel module.
- **Assigned to**: Willa Gutowski

### Part 3a: Adding System Calls
- **Responsibilities**: Added System Calls
- **Assigned to**: Carter Rudolph

### Part 3b: Kernel Compilation
- **Responsibilities**: Compile and Installed the Kernel
- **Assigned to**: Carter Rudolph

### Part 3c: Threads
- **Responsibilities**: Implemented the thread that deals with the elevator's actions
- **Assigned to**: Terryon Larkins

### Part 3d: Linked List
- **Responsibilities**: Implemented the Elevator with linked lists, kthread, mutex, and kmalloc
- **Assigned to**: Carter Rudolph

### Part 3e: Mutexes
- **Responsibilities**: Created a module that provides the proc entry.
- **Assigned to**: Terryon Larkins

### Part 3f: Scheduling Algorithm
- **Responsibilities**: Made the algorithm that determines the next place the elevator should visit.
- **Assigned to**: Terryon Larkins

## File Listing
```
elevator/
├── Makefile                          # Builds all three parts
├── README.md                         # Project documentation and logs
├── part1/
│   ├── Makefile                      # Builds and straces programs
│   ├── empty.c                       # Baseline empty program
│   ├── empty.trace                   # Strace output for empty
│   ├── part1.c                       # Makes five system calls
│   └── part1.trace                   # Strace output for part1
├── part2/
│   ├── Makefile                      # Builds timer kernel module
│   └── src/
│       └── my_timer.c                # /proc/timer elapsed time module
└── part3/
    ├── Makefile                      # Builds elevator kernel module
    ├── syscalls.c                    # Elevator syscall stubs (548-550)
    ├── kernel/
    │   ├── setup.sh                  # Adds syscalls, builds kernel
    │   └── install.sh                # Installs kernel, sets GRUB
    ├── src/
    │   ├── elevator_main.c           # Elevator thread, scheduling, proc
    │   ├── pet_list.c                # Pet linked list operations
    │   └── pet_list.h                # Pet and building structs
    └── tests/
        ├── elevator-test/
        │   ├── Makefile              # Builds producer and consumer
        │   ├── README.md             # Elevator test usage notes
        │   ├── consumer.c            # Starts/stops the elevator
        │   ├── producer.c            # Issues random pet requests
        │   └── wrappers.h            # Syscall wrapper functions
        ├── pet-list-test/
        │   ├── Makefile              # Builds pet list test
        │   └── pet_list_test.c       # Standalone pet list test module
        └── system-calls-test/
            ├── Makefile              # Builds syscall test programs
            ├── README.md             # Syscall test usage notes
            ├── syscheck.c            # Module verifying syscalls exist
            ├── test-syscalls.c       # User-space syscall existence check
            └── test-syscalls.h       # Syscall numbers and wrappers

```
# How to Compile & Execute

### Requirements
- Compiler: gcc (tested with 13.3.0)
- OS: Ubuntu 24.04.5 LTS on Linux kernel 6.16.12
- Tools: make and strance
- sudo access and a build tree for running kernel 


## Part 1
### Compilation
From the part1 directory, run `make`.
That builds the empty and part1 executables. 

### Execution
Next, run `make run`, which runs both programs under strace,
and will write empty.trace and part1.trace. 


## Part 2
### Compilation
From part2 directory, run `make`.
This will build the kernel module .ko in part2/. 

### Execution
Then, run
```
sudo insmod my_timer.ko
cat /proc/timer
sudo rmmod my_timer

```

example output:
```
$ cat /proc/timer
current time:
1793651442.583394914
elapsed time: 1.006100749

```


## Part 3

### Compilation
```bash
make
```
This will build the `elevator.ko` kernel module in `part3/`.
### Execution
```bash
sudo insmod elevator.ko
watch -n 1 cat /proc/elevator
sudo rmmod elevator
```
This will load the module and show the elevator's state, current floor, load, and waiting pets,
updated every second. Pets are added and the elevator is started and stopped with the provided
`producer` and `consumer` programs.

## Development Log
Each member records their contributions here.

### Willa Gutowski

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-10-31 | Figure out virtual environment basics, review system call powerpoints, cognitive work for part 1  |
| 2026-11-01 | Generated code for both parts and tested in VM  |
| 2026-11-02 | Refinement of part2 and updates to README  |

### Carter Rudolph

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-10-28 | Reassigned the division of labor so no member depends on another's work. Reviewed the starter files. |
| 2026-10-28 | Part 3a: wrote `syscalls.c` and the kernel setup script that adds syscalls 548-550 to the table, `syscalls.h`, and the kernel Makefile. |
| 2026-10-28 | Part 3b: configured (`localmodconfig`, no debug info), compiled, and installed kernel 6.16.12 on the group VM. Set it as the GRUB default. `syscheck` test passed for all three syscalls. |
| 2026-10-29 | Part 3d: wrote `pet_list.h` and `pet_list.c` (pet creation and validation, FIFO loading, unloading, counts, print helpers, cleanup) and a standalone test module. All checks passed on the VM with no leaks after `rmmod`. |

**Kernel build notes**
- `part3/kernel/setup.sh` adds the system calls and builds the kernel. `install.sh` installs it
  and sets 6.16.12 as the GRUB default, since the VM's stock kernel (7.0) is newer and would
  boot first.
- In 6.16 the syscall table is at `arch/x86/entry/syscalls/syscall_64.tbl`, not the path in the
  spec.

**Use of AI**
- Used Claude to optimize `syscalls.c`, the kernel `setup.sh` and `install.sh` scripts,
  `pet_list.c`, and the pet list test module, and to debug kernel build errors. I wrote
  `pet_list.h`, went through `pet_list.c` function by function, ran and verified every step on
  the VM (`uname -r`, `syscheck`, the pet list test), and reviewed the code to make sure I
  understand it.


### Terryon Larkins

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-10-26 | Generated the code for parts 3c, 3e, and 3f  |
| 2026-10-28 | Started testing the code with the tests that were provided  |
| 2026-10-29 | Fixed memory issue. Initially, memory   |
| 2026-10-31 | Testing finalized. Project pushed to secondary repository, just in case |

**Use of AI**
-Used ChatGPT in order to help guide me through making the elevator_main.c file. I needed the
most help with the scheduling algorithm and also trying to write and understand code related to mutex.
There were some memory allocation issues that I also used to AI to help resolve. After those things were done,
I read each line so that I could understand and scrutinize everything that ChatGPT helped me with.


## Meetings
Document in-person meetings, their purpose, and what was discussed.

| Date       | Attendees            | Topics Discussed | Outcomes / Decisions |
|------------|----------------------|------------------|-----------------------|
| 2026-10-14 | Willa, Terryon, Carter | First Division of Labor Meeting   | We decided who would perform which tasks. Decided the two individuals on each topic.  |
| 2026-10-21 | Terryon, Carter, Willa | Revisited the Division of Labor, VM usage, repository   | We realized that there was only one VM able to be used at one time. So we scheduled times and days to use the VM.  |
| 2026-10-29 | Carter, Willa, Terryon | General Check-In   | Asked about general progress.   |



## Bugs
- No known bugs in.

## Considerations
Used LLM tools for some code generation. 
