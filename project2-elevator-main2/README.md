# Elevator

COP4610 Project 2. 

## Group Members
- **Willa Gutowski**: wgg22@fsu.edu
- **Carter Randolph**: cgr24@fsu.edu
- **Terryon Larkins**: tlarkins@fsu.edu
## Division of Labor

### Part 1: System Call Tracing
- **Responsibilities**: [Added the 5 system calls that were necessary for project]
- **Assigned to**: Willa Gutowski

### Part 2: Timer Kernel Module
- **Responsibilities**: [Created the kernel module.]
- **Assigned to**: Willa Gutowski

### Part 3a: Adding System Calls
- **Responsibilities**: [Added System Calls]
- **Assigned to**: Carter Randolph

### Part 3b: Kernel Compilation
- **Responsibilities**: [Compile and Installed the Kernel]
- **Assigned to**: Carter Randolph

### Part 3c: Threads
- **Responsibilities**: [Implemented the thread that deals with the elevator's actions]
- **Assigned to**: Terryon Larkins

### Part 3d: Linked List
- **Responsibilities**: [Implemented the Elevator with linked lists, kthread, mutex, and kmalloc]
- **Assigned to**: Carter Randoplh

### Part 3e: Mutexes
- **Responsibilities**: [Created a module that provides the proc entry.]
- **Assigned to**: Terryon Larkins

### Part 3f: Scheduling Algorithm
- **Responsibilities**: [Made the algorithm that determines the next place the elevator should visit.]
- **Assigned to**: Terryon Larkins

## File Listing
```
elevator/
├── Makefile
├── part1/
│   ├── empty.c
│   ├── empty.trace
│   ├── part1.c
│   ├── part1.trace
│   └── Makefile
├── part2/
│   ├── src/my_timer.c
│   └── Makefile
├── part3/
│   ├── src/
│   ├── tests/
│   ├── Makefile
│   └── sys_call.c
├── Makefile
└── README.md

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
For a C/C++ example:
```bash
make
```
This will build the executable in ...
### Execution
```bash
make run
```
This will run the program ...

## Development Log
Each member records their contributions here.

### Willa Gutowski

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-10-31 | Figure out virtual environment basics, review system call powerpoints, cognitive work for part 1  |
| 2026-11-01 | Generated code for both parts and tested in VM  |
| 2026-11-02 | Refinement of part2 and updates to README  |

### [Member 2]

| Date       | Work Completed / Notes |
|------------|------------------------|
| YYYY-MM-DD | [Description of task]  |
| YYYY-MM-DD | [Description of task]  |
| YYYY-MM-DD | [Description of task]  |


### [Terryon Larkins]

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-10-26 | [Generated the code for parts 3c, 3e, and 3f]  |
| 2026-10-28 | [Started testing the code with the tests that were provided]  |
| 2026-10-29 | [Fixed memory issue. Initially, memory ]  |
| 2026-10-31 | [Testing finalized. Project pushed to secondary repository, just in case] |


## Meetings
Document in-person meetings, their purpose, and what was discussed.

| Date       | Attendees            | Topics Discussed | Outcomes / Decisions |
|------------|----------------------|------------------|-----------------------|
| 2026-10-14 | [Willa, Terryon, Carter] | [First Division of Labor Meeting]   | [We decided who would perform which tasks. Decided the two individuals on each topic.]  |
| 2026-10-21 | [Terryon, Carter, Willa] | [Revisited the Division of Labor, VM usage, repository]   | [We realized that there was only one VM able to be used at one time. So we scheduled times and days to use the VM.]  |
| 2026-10-29 | [Carter, Willa, Terryon] | [General Check-In]   | [Asked about general progress. ]  |



## Bugs
- **Bug 1**: This is bug 1.
- **Bug 2**: This is bug 2.
- **Bug 3**: This is bug 3.

## Considerations
Used LLM tools for some code generation. 
