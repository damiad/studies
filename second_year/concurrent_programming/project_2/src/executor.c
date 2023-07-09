#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdbool.h>

#include "err.h"

#define MAX_N_TASKS 4096
#define MAX_INPUT_LENGTH 512
#define MAX_OUTPUT_LENGTH 1023

// Struct for storing information about a task
typedef struct Task {
  pid_t pid;
  int task_id;
  int stdout_pipe; // pipe for redirecting stdout of the task
  int stderr_pipe; // pipe for redirecting stderr of the task
  char last_stdout_line[MAX_OUTPUT_LENGTH]; // last line printed to stdout
  char last_stderr_line[MAX_OUTPUT_LENGTH]; // last line printed to stderr
  pthread_t stdout_thread; // helper to read stdout
  pthread_t stderr_thread; // helper to read stderr
  pthread_t signal_thread; // helper to manage end signal
  pthread_mutex_t out_mutex; // mutex for last_stdout_line
  pthread_mutex_t err_mutex; // mutex for last_stderr_line
} Task;

Task tasks[MAX_N_TASKS]; // global array for storing all tasks
int next_task_id = 0; // global counter for assigning task IDs
pthread_mutex_t stream_mutex; // mutex for write

void read_pipe(int pipe, char* src, pthread_mutex_t* task_mutex) {
  FILE* output = fdopen(pipe, "r");
  char line[MAX_OUTPUT_LENGTH];

  while (fgets(line, MAX_OUTPUT_LENGTH, output)) {
    if (line[strlen(line) - 1] == '\n')
      line[strlen(line) - 1] = '\0';
    ASSERT_ZERO(pthread_mutex_lock(task_mutex));
    strcpy(src, line);
    ASSERT_ZERO(pthread_mutex_unlock(task_mutex));
  }
  fclose(output);
}

void* read_stdout(void* arg) {
  int* task_id = arg;
  read_pipe(tasks[*task_id].stdout_pipe, tasks[*task_id].last_stdout_line,
            &tasks[*task_id].out_mutex);
  return NULL;
}

void* read_stderr(void* arg) {
  int* task_id = arg;
  read_pipe(tasks[*task_id].stderr_pipe, tasks[*task_id].last_stderr_line,
            &tasks[*task_id].err_mutex);
  return NULL;
}

void* signal_checker(void* arg) {
  pid_t pid;
  int status;
  int* task_id = arg;

  while (true) {
    pid = waitpid(tasks[*task_id].pid, &status, 0);
    if (pid != 0) {
      if (WIFEXITED(status)) {
        ASSERT_ZERO(pthread_mutex_lock(&stream_mutex));
        printf("Task %d ended: status %d.\n", *task_id, WEXITSTATUS(status));
        ASSERT_ZERO(pthread_mutex_unlock(&stream_mutex));
        return NULL;
      }
      if (pid == -1) {
        ASSERT_ZERO(pthread_mutex_lock(&stream_mutex));
        printf("Task %d ended: signalled.\n", *task_id);
        ASSERT_ZERO(pthread_mutex_unlock(&stream_mutex));
        return NULL;
      }
    }
  }
}

void run(char** args) {
  int task_id = next_task_id;
  next_task_id++;

  tasks[task_id].task_id = task_id;
  tasks[task_id].last_stdout_line[0] = '\0';
  tasks[task_id].last_stderr_line[0] = '\0';
  ASSERT_ZERO(pthread_mutex_init(&tasks[task_id].out_mutex, NULL));
  ASSERT_ZERO(pthread_mutex_init(&tasks[task_id].err_mutex, NULL));

  int stdout_pipe[2];
  int stderr_pipe[2];
  ASSERT_SYS_OK(pipe(stdout_pipe));
  ASSERT_SYS_OK(pipe(stderr_pipe));

  set_close_on_exec(stdout_pipe[0], true);
  set_close_on_exec(stderr_pipe[0], true);

  pid_t pid = fork();
  ASSERT_SYS_OK(pid);

  if (!pid) { // Child process: redirect stdout and stderr to pipes and execute the command
    ASSERT_SYS_OK(dup2(stdout_pipe[1], STDOUT_FILENO));
    ASSERT_SYS_OK(close(stdout_pipe[1]));

    ASSERT_SYS_OK(dup2(stderr_pipe[1], STDERR_FILENO));
    ASSERT_SYS_OK(close(stderr_pipe[1]));

    ASSERT_SYS_OK(execvp(args[0], args));
  } else { // Parent process: store task informations
    ASSERT_SYS_OK(close(stdout_pipe[1]));
    ASSERT_SYS_OK(close(stderr_pipe[1]));
    tasks[task_id].pid = pid;
    tasks[task_id].stdout_pipe = stdout_pipe[0];
    tasks[task_id].stderr_pipe = stderr_pipe[0];

    ASSERT_ZERO(pthread_create(&tasks[task_id].stdout_thread, NULL, read_stdout,
                               &tasks[task_id].task_id));
    ASSERT_ZERO(pthread_create(&tasks[task_id].stderr_thread, NULL, read_stderr,
                               &tasks[task_id].task_id));

    ASSERT_ZERO(pthread_mutex_lock(&stream_mutex));
    printf("Task %d started: pid %i.\n", task_id, pid); // first print start message
    ASSERT_ZERO(pthread_mutex_unlock(&stream_mutex));

    ASSERT_ZERO(pthread_create(&tasks[task_id].signal_thread, NULL,
                               signal_checker, &tasks[task_id].task_id));
  }
}

// Prints last_stdout_line from given task
void out(const char* arg) {
  char last_line[MAX_OUTPUT_LENGTH];
  int task_id = atoi(arg);
  ASSERT_ZERO(pthread_mutex_lock(&tasks[task_id].out_mutex));
  strcpy(last_line, tasks[task_id].last_stdout_line);
  ASSERT_ZERO(pthread_mutex_unlock(&tasks[task_id].out_mutex));

  ASSERT_ZERO(pthread_mutex_lock(&stream_mutex));
  printf("Task %d stdout: '%s'.\n", task_id, last_line);
  ASSERT_ZERO(pthread_mutex_unlock(&stream_mutex));
}

// Prints last_stderr_line from given task
void err(const char* arg) {
  char last_line[MAX_OUTPUT_LENGTH];
  int task_id = atoi(arg);
  ASSERT_ZERO(pthread_mutex_lock(&tasks[task_id].err_mutex));
  strcpy(last_line, tasks[task_id].last_stderr_line);
  ASSERT_ZERO(pthread_mutex_unlock(&tasks[task_id].err_mutex));

  ASSERT_ZERO(pthread_mutex_lock(&stream_mutex));
  printf("Task %d stderr: '%s'.\n", task_id, last_line);
  ASSERT_ZERO(pthread_mutex_unlock(&stream_mutex));
}

void kill_task(const char* arg) {
  int task_id = atoi(arg);
  kill(tasks[task_id].pid, SIGINT);
}

void sleep_executor(const char* arg) {
  int milliseconds = atoi(arg);
  usleep(milliseconds * 1000);
}

void quit() { // Kill all tasks
  for (int i = 0; i < next_task_id; i++) {
    kill(tasks[i].pid, SIGKILL);
    pthread_join(tasks[i].signal_thread, NULL);
    pthread_join(tasks[i].stdout_thread, NULL);
    pthread_join(tasks[i].stderr_thread, NULL);
    ASSERT_ZERO(pthread_mutex_destroy(&tasks[i].out_mutex));
    ASSERT_ZERO(pthread_mutex_destroy(&tasks[i].err_mutex));
  }
  next_task_id = 0;
  ASSERT_ZERO(pthread_mutex_destroy(&stream_mutex));
}

int main() {
  ASSERT_ZERO(pthread_mutex_init(&stream_mutex, NULL));
  char line[MAX_INPUT_LENGTH];
  while (read_line(line, MAX_INPUT_LENGTH, stdin)) {
    line[strlen(line) - 1] = '\0'; // For split_string to work
    char** args = split_string(line);

    if (strcmp(args[0], "run") == 0)
      run(args + 1);
    else if (strcmp(args[0], "out") == 0)
      out(args[1]);
    else if (strcmp(args[0], "err") == 0)
      err(args[1]);
    else if (strcmp(args[0], "kill") == 0)
      kill_task(args[1]);
    else if (strcmp(args[0], "sleep") == 0)
      sleep_executor(args[1]);
    else if (strcmp(args[0], "quit") == 0) {
      free_split_string(args);
      break;
    } // Empty line - input must be correct
    free_split_string(args);
  }
  quit();
  return 0;
}