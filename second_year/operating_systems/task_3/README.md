# Task 3

## Requirements
Open: Wednesday, April 19, 2023, 00:00
Due: Sunday, May 14, 2023, 23:59

## Payment System in MINIX

The goal of this exercise is to enable processes in the MINIX system to have money and perform mutual transfers.

Each process is initially allocated INIT_BALANCE currency units. Then, processes can transfer money to each other. Process P can initiate a transfer of n currency units to process Q. For the transfer to succeed, process P must have at least n currency units on its account (the account balance of process P cannot be negative), and process Q must have no more than MAX_BALANCE - n currency units on its account. Furthermore, as a basic protection against money laundering, we require that processes P and Q are not in a parent-child relationship. If the transfer succeeds, the account balance of process P is reduced by n currency units, and the account balance of process Q is increased by n currency units.

The money of processes is not inherited - when a process terminates, the accumulated currency units disappear.

Note: granting each process new currency units at the start inevitably leads to money inflation, but let's leave that problem to economists.

## New System Call

The task is to add a system call PM_TRANSFER_MONEY and a library function `int transfermoney(pid_t recipient, int amount)`. The function should be declared in the file `unistd.h`. The constants INIT_BALANCE = 100 and MAX_BALANCE = 1000 should be defined in the file `minix/config.h`.

The function `int transfermoney(pid_t recipient, int amount)` should perform a transfer of `amount` currency units from the calling process's account to the account of the process with the given `recipient` identifier. If the transfer is successful, the function should return the account balance of the process that made the transfer after the transfer.

Note: A process can check its account balance by transferring 0 currency units to itself.

In case the transfer fails, the `transfermoney` function should return -1 and set `errno` to the appropriate error code:

- If `recipient` is not the identifier of the currently executing process, `errno` should be set to ESRCH.
- If `recipient` is the identifier of a process that is a child or ancestor of the calling process, `errno` should be set to EPERM.
- If the value of `amount` is negative, or the calling process has fewer than `amount` currency units on its account, or the process with the `recipient` identifier has more than MAX_BALANCE - `amount` currency units, `errno` should be set to EINVAL.

The behavior of the `transfermoney` function should be implemented using the new system call PM_TRANSFER_MONEY, which needs to be added to the PM server. A custom message type should be defined to pass the parameters.

## Solution Format

Below, we assume that ab123456 represents the identifier of the student submitting the solution. Prepare a patch in the `/usr` directory. Generate a patch file named `ab123456.patch` using the following command:

```bash
diff -rupNEZbB original-sources/usr/ my-solution/usr/ > ab123456.patch
```

Here, `original-sources` is the path to the unchanged MINIX sources, and `my-solution` is the path to the MINIX sources containing the solution. The `diff` command will recursively scan the files in the `original-sources/usr` path, compare them with the files in the `my-solution/usr` path, and generate the `ab123456.patch` file summarizing the differences. This file will be used to automatically apply the changes to a clean copy of MINIX, where the solution will be tested. More information about the `diff` command can be found in the manual (man diff).

After applying the patch to a clean copy of MINIX by placing the patch file in the root directory and executing the following command:

```bash
patch -p1 < ab123456.patch
```

All the required changes should be applied. Then, the following commands will be executed:

```bash
make && make install
```

in the directories `/usr/src/minix/fs/procfs`, `/usr/src/minix/servers/pm`, `/usr/src/minix/drivers/storage/ramdisk`, `/usr/src/minix/drivers/storage/memory`, and `/usr/src/lib/libc`, and

```bash
make do-hdboot
```

in the `/usr/src/releasetools` directory, followed by a reboot.