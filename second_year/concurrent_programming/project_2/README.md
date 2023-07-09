# Executor - C Programming Final Assignment
This is the English translation of the assignment "Duże zadanie zaliczeniowe z C: egzekutor" for the course "Concurrent Programming."

### Task Description
Sara is developing an algorithm and writing programs that take a long time to execute, periodically outputting progress information. Sara would like to test her programs by running them on various examples and checking the output they produce. However, there are a large number of examples to check and a lot of output information to process. Your task is to create an executor program that will help Sara by executing the specified tasks, checking their last words (while the program is running), and terminating them.

The executor program should support the following commands (each command is a separate input line):

- `run`: The `run` command followed by program name and arguments creates a new task by starting the execution of the specified program in the background. Tasks are identified by numbers 0, 1, 2, ..., in the order of their creation (i.e., the order of `run` commands). The executor outputs the message `Task T started: pid P.` where T is the task identifier and P is the process ID of the executing program. Any output produced by the program on stdout or stderr should not be shown. The program will not expect any input from stdin (can be left unchanged).

- `out`: The `out` command followed by a task identifier T outputs the message `Task T stdout: 'S'.` where S is the last line (without a newline character) printed by the program of task number T. In a less optimized solution, the executor may wait until the program of task T prints the next line (or EOF) and use it as S. In a fully optimized solution, the executor should immediately output the last line (unless the program of task T prints additional lines in a short time, in which case any of those lines can be used). The program of task T should continue running.

- `err`: The `err` command followed by a task identifier T outputs the message `Task T stderr: 'S'.` similar to the `out` command, but for stderr output of the program of task T. The message should be printed on the standard output of the executor (like all other executor commands).

- `kill`: The `kill` command followed by a task identifier T sends the SIGINT signal to the program of task T. If the program has already terminated, the signal can be sent (ineffectively, ignoring any error) or nothing can be done.

### Auxiliary Commands
The executor program should also support the following auxiliary commands:

- `sleep N`: The `sleep` command followed by an integer N sleeps the executor for N milliseconds (using usleep or any other function that uses microseconds). During this time, the executor should not consume any CPU resources. It should simply wait before processing the next command. This command does not affect any running tasks.

- `quit` or end of input (EOF): The `quit` command or the end of input terminates the executor program (and all tasks).

- Empty line: An empty line should be ignored, and the executor should proceed to the next command.

### Task Completion
When the program of a task T terminates, the executor should output the message `Task T ended: status X.` where T is the task identifier and X is the exit code of the program. If the exit code is not available (because the task was interrupted by a signal or the system), the message should be `Task T ended: signalled.` instead.

If the program terminates while the executor is processing a command, the completion message should be printed only after the command processing is finished. In a less optimized solution, the message can be delayed until the next command (and its completion). In a fully optimized solution, the message should be printed immediately after the program terminates, during the executor's wait for the next command (unless the executor is currently processing a command).

After the executor has output the completion message for the last (so far started) task, there should be no processes or threads created by the executor, except for the main executor thread and possibly one auxiliary thread/process.

### Example
Running the following command in a bash shell:
```
echo -e "foo\nbar" > in.txt;
echo -e "run cat in.txt\nsleep 100\nout 0" | ./executor
```
should produce the following output:
```
Task 0 started: pid 1234.
Task 0 ended: status 0.
Task 0 stdout: 'bar'.
```

## Notes
- The memory consumption of the executor program should be independent of the output length and the execution time of the tasks. It should be O(1) regardless of the output size or task duration.
- The executor should not create any regular files.
- The executor should not accept the next command until the previous command processing is complete.
- The use of the poll() function and its variants (ppoll, select, pselect, epoll) is prohibited.
- In addition to the functions discussed in the course, other C or POSIX functions can be used, except for poll(). Functions like waitpid (with the WNOHANG option to check if a process has finished) and fdopen (to operate on streams, e.g., getline or fgets) may be useful.
- Active waiting is not allowed. If there is no input for a long time and the programs do not produce any output, the executor should not consume CPU resources. Specifically, the executor should not periodically wake up.
- The executed programs should not have any open file descriptors other than the standard ones.
- To minimize memory handling and error handling difficulties in the C language:
  - All executor commands will be valid and not longer than 511 characters (including newline characters).
  - All lines printed by the executed programs will not exceed 1022 characters (including newline characters). The last line may or may not contain a newline character.
  - There will be at most 4096 tasks. The executor can allocate memory proportional to the number of tasks multiplied by the maximum line length, even if not all tasks are executed. For example, you can have a global array `struct Task tasks[MAX_N_TASKS];`.
  - The executed programs will never print null characters. The executor commands will not contain null characters, quotes, or backslashes. In particular, program arguments are simply separated by spaces and can be processed using the `split_string` function from `utils.h`.
  - In case of any error, you can simply terminate the executor, for example, by calling `exit(1)`.
  - You can print any diagnostic information, especially error messages, to stderr.