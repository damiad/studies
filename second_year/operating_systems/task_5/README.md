# Task 5

## Completion Requirements
Open: Thursday, May 25, 2023, 00:00
Due by: Sunday, June 18, 2023, 23:59
Exclusive file locking by users.

The goal of this task is to extend the vfs server with a mechanism that allows users to exclusively lock access to selected files. Unlike standard flock locks, this mechanism will be mandatory and will operate at the user level rather than the process level. Unlike standard file access permissions, this mechanism will implement temporary locking without requiring changes to file attributes.

## VFS

VFS (Virtual File System) is an operating system subsystem that provides unified access to files located on different file systems. It acts as an intermediary layer between applications and subsystems implementing specific file systems (MFS, ext2, procfs, etc.). It processes system calls that perform file operations, implements actions common to different file systems, and forwards requests to the appropriate file systems. It also manages all files used in the system and all mounted file systems.

In MINIX, the virtual file system is implemented as the vfs server. More information about its structure and operation can be found in the MINIX Wiki: VFS internals.

## VFS_FEXCLUSIVE and VFS_EXCLUSIVE System Calls

The file locking mechanism is based on the new VFS_FEXCLUSIVE and VFS_EXCLUSIVE system calls supported by the vfs server. Using these system calls, a user can temporarily lock other users from performing the following actions on a specified file: file opening (VFS_OPEN and VFS_CREAT system calls), reading (VFS_READ), writing (VFS_WRITE), truncating (VFS_TRUNCATE and VFS_FTRUNCATE), renaming and moving (VFS_RENAME, both when the locked file is the first and second argument), and file deletion (VFS_UNLINK). The user who locks the file can perform these operations without restrictions from different processes. However, any attempt by another user to perform these operations should result in an EACCES error.

The VFS_FEXCLUSIVE system call takes a file descriptor and a flag indicating the action to be performed. The following actions are supported:

EXCL_LOCK: Locks the file indicated by the file descriptor exclusively for the user who invokes this system call. If the file is not explicitly unlocked by the user, it will be automatically unlocked when the descriptor used to block the file is closed.

EXCL_LOCK_NO_OTHERS: Works like EXCL_LOCK but locks the file only if it is not currently open by another user (the user locking the file can have the file open any number of times). Otherwise, the call fails with an EAGAIN error.

EXCL_UNLOCK: Unlocks the file indicated by the file descriptor. Only the user who locked the file can unlock it.

EXCL_UNLOCK_FORCE: Unlocks the file indicated by the file descriptor. The file can be unlocked by the user who locked it, the user who is the owner of the file, or the superuser (root user with UID = 0).

The VFS_EXCLUSIVE system call takes a file path and a flag indicating the action to be performed. The following actions are supported:

EXCL_LOCK: Locks the file indicated by the specified path exclusively for the user who invokes this system call. The file remains locked until explicitly unlocked by the user. An exception to this rule is when the locked file is deleted by the user (using the VFS_UNLINK system call) or replaced by another file (the locked file is the second argument of VFS_RENAME). In such cases, the file will be automatically unlocked when it is no longer in use by any user (no process has the file open).

EXCL_LOCK_NO_OTHERS: Works like EXCL_LOCK but locks the file only if it is not currently open by another user (the user locking the file can have the file open any number of times). Otherwise, the call fails with an EAGAIN error.

EXCL_UNLOCK: Unlocks the file indicated by the specified path. Only the user who locked the file can unlock it.

EXCL_UNLOCK_FORCE: Unlocks the file indicated by the specified path. The file can be unlocked by the user who locked it, the user who is the owner of the file, or the superuser (root user with UID = 0).

The file locking mechanism works according to the following specifications:

Only regular files can be locked. Attempts to lock directories, pseudo-devices, pipes (FIFOs), etc., result in an EFTYPE error.

A specific file indicated by the file descriptor or path is locked or unlocked at the time of locking or unlocking. Moving the file (within the same partition), renaming it, referencing it through a symbolic link, etc., does not cause the file to become unlocked. Technically, a specific vnode or inode is locked (v-node/i-node).

File locks are not preserved after unmounting a file system. The presence of locked files does not prevent the unmounting of the file system (if unmounting the partition without locked files would succeed, unmounting the partition with locked files will also succeed).

The lock applies from the moment the file is locked until it is unlocked. The aforementioned system calls check the file's accessibility in each call. Therefore, it is possible for a user to successfully open a file and then have another user lock it, causing the next operation by the first user (e.g., reading the file's contents) to result in an EACCES error. When the second user unlocks the file, the subsequent operation by the first user (e.g., another attempt to read the file's contents) will succeed.

The user is identified by their real UID, regardless of the effective UID value.

## Usage

Using VFS_FEXCLUSIVE, a file can only be locked if the specified descriptor is open for reading or writing. Otherwise, the system call fails with an EBADF error. Using VFS_EXCLUSIVE, a file can only be locked if the invoking user has read or write permissions for the file specified by the given path. Otherwise, the system call fails with an EACCES error.

If the VFS_FEXCLUSIVE and VFS_EXCLUSIVE system calls cannot succeed, they terminate with the appropriate error. For example: EINVAL - if a different flag is specified in the call than the supported actions, or the file specified for unlocking is not locked; EBADF - if the specified descriptor is invalid; EALREADY - if the file specified for locking is already locked; EPERM - if the user is not authorized to unlock the specified file, etc.

At most NR_EXCLUSIVE files can be locked simultaneously (this constant is defined in the attachment to the task). Calls that would exceed this limit should result in an ENOLCK error.

The VFS_FEXCLUSIVE and VFS_EXCLUSIVE system calls implement the same file locking mechanism. It is possible to lock a file using one system call and unlock it using the other system call.

## Task Attachments

The task includes sample programs: "test-fexclusive.c," "test-exclusive-lock.c," and "test-exclusive-unlock.c," which illustrate the usage of the VFS_FEXCLUSIVE and VFS_EXCLUSIVE system calls to lock access to a file specified as an argument to the program. 

## Example usage scenario for the "test-fexclusive.c" program:

```bash
make test-fexclusive
clang -O2 -o test-fexclusive test-fexclusive.c
touch /tmp/test.txt
./test-fexclusive /tmp/test.txt
Locking the file...
VFS_FEXCLUSIVE result: 0, errno: 0
Waiting... Press something
```
At this point, attempting to open the file /tmp/test.txt by another user will fail:

```bash
cat /tmp/test.txt
cat: /tmp/test.txt: Permission denied
```
Continuing the test-fexclusive program will close the file /tmp/test.txt, unlocking it, and allowing other users to work with it.

## Example usage scenario for the "test-exclusive-lock.c" program:

```bash
make test-exclusive-lock
clang -O2 -o test-exclusive-lock test-exclusive-lock.c
touch /tmp/test.txt
./test-exclusive-lock /tmp/test.txt
Locking the file...
VFS_EXCLUSIVE result: 0, errno: 0
```
At this point, attempting to open the file /tmp/test.txt by another user will fail:

```bash
# cat /tmp/test.txt
cat: /tmp/test.txt: Permission denied
```
The file can be unlocked using the test-exclusive-unlock.c program:

```bash
make test-exclusive-unlock
clang -O2 -o test-exclusive-unlock test-exclusive-unlock.c
./test-exclusive-unlock /tmp/test.txt
Unlocking the file...
VFS_EXCLUSIVE result: 0, errno: 0
```
Now other users can work with the file again.

Comments within the code of the sample programs also illustrate the usage of the other system call flags.

## Requirements and Non-requirements

All other operations performed by the vfs server, apart from those described above, should work without changes.

Modifications to the vfs server must not cause errors in the system (e.g., kernel panic) or errors in the file system (e.g., data inconsistencies).

Modifications must not change the contents of files, file attributes, file structure, or the format in which data is stored on the disk.

Modifications can only affect the vfs server (i.e., they can only affect files in the /usr/src/minix/servers/vfs/ directory), except for modifications added by the zadanie5-szablon.patch patch. The definitions added by this patch must not be changed.

While the modified vfs server is running, it should not output any additional information to the console or event log.

It can be assumed that the solutions will not be tested using hard links. Locking programs that will be executed later will not be tested either.

The solution does not have to be optimized for speed. Solutions that work without noticeable delay for the user will be accepted.

## Tips

The implementation of the vfs server is not discussed in class because one of the goals of this task is to independently analyze the relevant MINIX source code fragments. While solving this task, you should read more code than you write or modify.

Examples of how to implement a particular functionality in the vfs server can be found in the code implementing existing system calls.

Many resources on the Internet discuss the operation of the vfs server, such as Prashant Shenoy's lecture slides. However, when using such materials, pay attention to whether they refer to the same version of MINIX and vfs server as the one being modified in this task.

Additional hard disks can be attached to a MINIX running on QEMU. To do this:

A. Create a file on the host computer that will act as the new disk, e.g.: qemu-img create -f raw extra.img 1M.

B. Attach this disk to the virtual machine by adding the following parameters to the QEMU startup parameters: -drive file=extra.img,format=raw,index=1,media=disk, where the index parameter specifies the disk's sequential number (0 is the main disk - the image of our machine).

C. The first time, create an MFS file system on the new disk: /dev/c0d<number of the added disk>, e.g.: /sbin/mkfs.mfs /dev/c0d1.

D. Create an empty directory (e.g., mkdir /root/nowy) and mount the attached disk to it: mount /dev/c0d1 /root/nowy.

E. All operations within this directory will be performed on the mounted disk.

F. To unmount the disk, use the umount /root/nowy command.

## Solution

Below, we assume that ab123456 represents the identifier of the student solving the task. Prepare a patch (patch file) with the changes as described in the task. Submit the solution as a patch named ab123456.patch in Moodle. The patch will be applied to a fresh copy of MINIX. Note: The solution patch should also include the changes added by the zadanie5-szablon.patch patch. Ensure that the patch contains only necessary differences.

To compile and run the system with the modified vfs server, execute the following commands:

```bash
cd /
patch -t -p1 < ab123456.patch
cd /usr/src; make includes
cd /usr/src/minix/servers/vfs/; make clean && make && make install
cd /usr/src/releasetools; make do-hdboot
reboot
```