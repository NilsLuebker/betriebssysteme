#include "jobctl.h"
#include "argparse.h"
#include "shell.h"

process* foreground_process = NULL;
process_list* background_processes = NULL;

void init_jobctl()
{
	background_processes = new_process_list();
}

process* new_process(char** argv, pid_t pid)
{
	/* Create the new process */
	process* result = malloc(sizeof(process));
	*result = (process) {
		.argv=argv,
		.pid=pid,
		.status=0
	};

	/* Put the newly create process into it's own process group */
	setpgid(result->pid, result->pid);
	return result;
}

process_list* new_process_list()
{
	process_list* result = malloc(sizeof(process_list));
	*result = (process_list) {
		.length=1,
		.size=1,
		.first_process=malloc(sizeof(process*))
	};
	result->first_process[0] = NULL;
	return result;
}

void clean_process(process* p)
{
	clean_argv(p->argv);
	free(p);
}

process* find_process(pid_t child_pid)
{
	/* Check if child_pid is the current foreground process */
	if(foreground_process && foreground_process->pid == child_pid)
		return foreground_process;

	/* Check if child_pid is a background process */
	for(size_t i = 0; i < background_processes->length; i++)
	{
		if(background_processes->first_process[i]->pid == child_pid)
			return background_processes->first_process[i];
	}

	/* child_pid was not found */
	return NULL;
}

void move_foreground(process* p)
{
	/* If there already is a foreground process free it's memory */
	if(foreground_process != NULL)
	{
		clean_process(foreground_process);
	}

	/* Give the process access to the terminal */
	tcsetpgrp(shell.terminal, p->pid);

	/* Add new foreground process */
	foreground_process = p;
}

size_t list_append(process_list* list, process* p)
{
	/* Resize the array so another process can be inserted */
	list_resize(list, list->length + 1);
	
	/* Append the process */
	size_t last_elem = list->length - 2;
	list->first_process[last_elem] = p;

	/* Add NULL termination */
	size_t last_index = list->length - 1;
	list->first_process[last_index] = NULL;

	/* Return the index at which the process was added */
	return last_elem;
}

void list_remove(process_list* list, process* p, bool clean)
{
	process* found = NULL;
	for(size_t i = 0; i < list->length; i++)
	{
		if(list->first_process[i] != p || found)
			continue;
		if(!found)
			found = list->first_process[i];
		if(i+1 < list->length)
			list->first_process[i] = list->first_process[i+1];
		else
			list->first_process[i] = NULL;
	}
	if(found)
	{
		if(clean)
		{
			clean_process(found);
			found = NULL;
		}
		list_resize(list, list->length - 1);
	}
}

void list_resize(process_list* list, int new_length)
{
	size_t new_size = 1;
	size_t i = 0;
	
	/* Increment/Decrement process list length */
	list->length = new_length;

	/* Only resize by power of two eg. 2, 4, 8, 16, 32.. */
	while((new_size<<=++i) < list->length);

	if(new_size != list->size)
	{
		/* Resize the struct */
		list->first_process = realloc(
			list->first_process,
			sizeof(process*) * new_size
		);
		list->size = new_size;
	}
}

void list_gc(process_list* list)
{
	process_list* processes_to_remove = new_process_list();

	for(size_t i = 0; i < list->length; i++)
	{
		if(!list->first_process[i]) continue;
		if(list->first_process[i]->completed)
			list_append(processes_to_remove, list->first_process[i]);
	}

	for(size_t i = 0; i < processes_to_remove->length; i++)
	{
		if(!processes_to_remove->first_process[i]) continue;
		list_remove(list, processes_to_remove->first_process[i], true);
	}

	free(processes_to_remove);
}

void print_background_processes()
{
	for(size_t i = 0; i < background_processes->length; i++)
	{
		if(!background_processes->first_process[i])
			continue;
		if(background_processes->first_process[i]->stopped)
			printf(
				"[%lu]\tStopped\t\t%s\n",
				i,
				background_processes->first_process[i]->argv[0]
			);
		else if(background_processes->first_process[i]->completed)
			printf(
				"[%lu]\tCompleted\t%s\n",
				i,
				background_processes->first_process[i]->argv[0]
			);
		else
			printf(
				"[%lu]\tRunning\t%s\n",
				i,
				background_processes->first_process[i]->argv[0]
			);
	}
}

void execute_system(char** argv)
{
	/* Check for & and and remove it if it's present */
	bool foreground = !check_last_char(argv, '&');
	if(!foreground) argv = trim_last_arg(argv);

	/* Create a new process */
	pid_t child_pid = fork();
	switch (child_pid)
	{
		case -1:
			printf("Error while trying to fork()!");
			exit(EXIT_FAILURE);
		case 0:
			/* Child process */
			launch_process(argv, foreground);
			break;
		default:
		{
			/* Prarent process */
			process* child_process = new_process(argv, child_pid);
			if(foreground)
			{
				move_foreground(child_process);
				wait_for_foreground_process();
			}
			else
			{
				list_append(background_processes, child_process);
			}
		}
	}
}

void launch_process(char** argv, bool foreground)
{
	/* Put the process into it's own process group */
	pid_t pid = getpid();
	setpgid(pid, pid);

	/* Give the process group access to the terminal */
	if (foreground)
		tcsetpgrp(shell.terminal, pid);

	/* Setting job control signals in the child process to default. */
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTTOU, SIG_DFL);
	signal(SIGCHLD, SIG_DFL);

	/* Execute the new process */
	int error = execvp(argv[0], argv);

	/* Only reached if execvp fails */
	printf("Command not found [%d]\n", error);
	exit(0);
}

void wait_for_foreground_process()
{
	/* Wait for the child process to terminate or stop */
	waitpid(
		foreground_process->pid,
		&foreground_process->status,
		WUNTRACED
	);

	put_shell_in_foreground();

	/* If the process got a SIGTSTP put it in the background */
	if(WIFSTOPPED(foreground_process->status))
	{
		foreground_process->stopped = true;
		size_t bgid = list_append(background_processes, foreground_process);
		printf("[%lu]+\tStopped\t%s\n", bgid, foreground_process->argv[0]);
	}
	/* If the process was finished clean up */
	else
	{
		clean_process(foreground_process);
	}
	/* Don't point at freed memory location */
	foreground_process = NULL;
}
