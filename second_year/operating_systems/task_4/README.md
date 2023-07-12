# Task 4

## Requirements
Open: Monday, May 15, 2023, 00:00
Due: Sunday, June 4, 2023, 23:59

The goal of this exercise is to implement the "max before deadline" scheduling strategy in MINIX and add a system call that allows processes to select this strategy.

Each process scheduled with this algorithm specifies a deadline, before which it wants to be executed (parameter deadline), an estimated total execution time (parameter estimate), and a behavior in case of exceeding the time (kill flag). The deadline is a non-negative integer representing the time in milliseconds since 00:00 on January 1, 1970, or -1 to indicate opting out of this scheduling strategy. The estimate is a positive integer representing the estimated time in milliseconds needed to complete the process. Processes are scheduled to maximize the number of processes executed before their respective deadlines.

## Scheduling

All processes scheduled with the "max before deadline" algorithm have the same priority, DEADLINE_Q, which means they are in the same queue of ready-to-run processes. Within this queue, the first waiting process is selected. The queue is modified according to the following rules:

- When the queue is empty and a new process requests scheduling, it is placed at the beginning of the queue.
- When there is already a process in the queue and a new process with an estimated total execution time arrives, it is inserted at position p such that:
  - If k processes could execute before their deadlines before its insertion, then after its insertion, k + 1 processes should be able to execute before their deadlines.
  - If there are multiple positions satisfying the previous condition, it is inserted at the last one.
  - If no such position exists, it is inserted at the last position where the same k processes can execute before their deadlines, the process at position p - 1 has an estimated total execution time less than or equal to estimate, and the process at position p + 1 has an estimated total execution time greater than or equal to estimate.
  - If no such positions exist, it is inserted at the end of the queue.
- When a process is inserted into the queue, if the current time exceeds its deadline, the process returns to default scheduling, to the queue it was in before being scheduled with the "max before deadline" algorithm.
- If the selected process has been executing longer than its estimated total execution time, the next process is chosen. The future of the considered process depends on the value of the kill flag:
  - If the kill flag is set to true, the process is killed.
  - If the kill flag is set to false, the process returns to default scheduling, placed at the end of the PENALTY_Q queue.

During the execution, processes scheduled with the new algorithm do not change their priority (queue) compared to regular processes. Care should be taken to prevent regular processes from being assigned the DEADLINE_Q priority.

Child processes of processes scheduled with the "max before deadline" algorithm inherit all the scheduling parameters (deadline, estimate, kill, the last queue before the scheduling change, execution time since the scheduling change).

## Implementation

The implementation should include the definition of the constants DEADLINE_Q = 8 and PENALTY_Q = 14.

The implementation should include a new system call:

```c
int sched_deadline(int64_t deadline, int64_t estimate, bool kill);
```

If the value of the deadline parameter is non-negative, the process scheduling will be changed to the "max before deadline" algorithm with the specified deadline and estimated total execution time. A value of -1 indicates that the process is opting out of the "max before deadline" scheduling and returning to default scheduling, to the queue it was in before being scheduled with this algorithm.

The function should return 0 if the scheduling method was changed successfully and -1 otherwise. If the parameter values are invalid (i.e., the deadline value is less than the current time plus the estimate and not equal to -1, or the estimate value is non-positive), errno should be set to EINVAL. If a process that wants to change the scheduling method to "max before deadline" is already scheduled with this algorithm, errno should be set to EPERM. The same should happen if a process that wants to opt out of the "max before deadline" scheduling is not scheduled with it.

Modifications can be made in the following directories:

- /usr/src/minix/servers/sched
- /usr/src/minix/servers/pm
- /usr/src/minix/kernel
- /usr/src/lib/libc/misc
- /usr/src/minix/lib/libsys

and in the header files:

- /usr/src/minix/include/minix/com.h, which will be copied to /usr/include/minix/com.h
- /usr/src/minix/include/minix/callnr.h, which will be copied to /usr/include/minix/callnr.h
- /usr/src/include/unistd.h, which will be copied to /usr/include/unistd.h
- /usr/src/minix/include/minix/syslib.h, which will be copied to /usr/include/minix/syslib.h
- /usr/src/minix/include/minix/ipc.h, which will be copied to /usr/include/minix/ipc.h
- /usr/src/minix/include/minix/config.h, which will be copied to /usr/include/minix/config.h

## Hints

To change the scheduling method, a new system call can be added to the microkernel. In this case, it is advisable to base it on the example of the `do_schedule()` function. It is also possible to modify the existing `do_schedule()` function.

Please note that the microkernel (/usr/src/minix/kernel/proc.c) is responsible for inserting processes into the ready queues. The sched server should ensure that regular processes are not assigned the DEADLINE_Q priority.

It is not necessary (and not recommended) to write a new scheduling server. The default sched server can be modified.

To make the new scheduling algorithm work, run `make` and `make install` in the `/usr/src/minix/servers/sched` directory and other directories with changes. Then, build a new kernel image by running `make hdboot` in the `/usr/src/releasetools` directory and reboot the system. If the image fails to load or a serious error (kernel panic) occurs, choose option 6 at system startup to load the original kernel.

The current time can be obtained, for example, using the `int getuptime(clock_t*, clock_t*, time_t*)` function.

To obtain the current time while in the kernel, you can refer to the implementation of `int do_times(struct proc*, message*)`.

## Solution

Below, we assume that ab123456 represents the identifier of the person solving the exercise. Prepare a patch in the `/usr` directory. Generate a patch file named `ab123456.patch` using the `diff` command, as in Exercise 3.

Submit the solution in the form of the `ab123456.patch` patch file.

Note: No points will be awarded for a solution where the patch does not apply correctly, does not compile, or causes kernel panic during boot.

To compile and run the system with the new scheduling version, execute the following commands:

```bash
cd /
patch -t -p1 < ab123456.patch
cd /usr/src; make includes
cd /usr/src/minix/kernel; make && make install
cd /usr/src/minix/lib/libsys; make && make install
cd /usr/src/minix/servers/sched; make && make install
cd /usr/src/minix/servers/pm; make && make install
cd /usr/src/lib/libc; make && make install
cd /usr/src/releasetools; make hdboot
```

The newly built system will then be tested using an automated test suite. The final score of the solution will depend on the stability of the system, correctness and efficiency on the automated tests, and coding style.