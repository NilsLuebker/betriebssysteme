#ifndef JOBCTL_H
#define JOBCTL_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct process {
	char** argv;
	pid_t pid;
	bool compleded;
	bool stopped;
	int status;
} process;

process* foreground_process = NULL;
process* background_processes = NULL;

process* find_process(pid_t);
bool process_is_stopped(pid_t);
bool process_is_completed(pid_t);
bool start_process(char**);

#endif /* JOBCTL_H */
