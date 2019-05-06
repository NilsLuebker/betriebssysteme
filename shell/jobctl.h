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
	size_t length;
	size_t size;
	process** first_process;
} process_list;

extern process* foreground_process;
extern process_list* background_processes;

void init_jobctl(void);
process* find_process(pid_t);
process* new_process(char**, pid_t);
process_list* new_process_list(void);
void clean_process(process*);
void move_foreground(process*);
size_t list_append(process_list*, process*);
void list_remove(process_list*, process*, bool);
void list_resize(process_list*, int n);
void list_gc(process_list*);
void print_background_processes(void);
void execute_system(char**);
void launch_process(char**, bool);
void wait_for_foreground_process(void);

#endif /* JOBCTL_H */
