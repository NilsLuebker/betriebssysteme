#ifndef JOBCTL_H
#define JOBCTL_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

typedef struct process {
	char** argv;
	pid_t pid;
	int status;
} process;

typedef struct process_list {
	size_t size;
	process* processes;
} process_list;

extern process* foreground_process;
extern process_list* background_processes;

process* find_process(pid_t);
void new_foreground_process(char**, pid_t);
bool add_background_process(char**, pid_t);
bool process_is_stopped(pid_t);
bool process_is_completed(pid_t);
void execute_system(char**);
void launch_process(char**, bool);
void wait_for_foreground_process();

#endif /* JOBCTL_H */
