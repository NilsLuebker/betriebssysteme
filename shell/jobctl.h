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
	bool completed;
	bool stopped;
	int status;
} process;

typedef struct process_list {
	size_t size;
	process** first_process;
} process_list;

extern process* foreground_process;
extern process_list* background_processes;

void init_jobctl(void);
process* find_process(pid_t);
process* new_process(char**, pid_t);
void move_foreground(process*);
size_t move_background(process*);
void print_background_processes(void);
bool process_is_stopped(pid_t);
bool process_is_completed(pid_t);
void execute_system(char**);
void launch_process(char**, bool);
void wait_for_foreground_process();

#endif /* JOBCTL_H */
