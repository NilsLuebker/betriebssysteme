#include "jobctl.h"
#include "argparse.h"
#include "shell.h"

process* foreground_process = NULL;
process_list* background_processes = NULL;

void init_jobctl()
{
	background_processes = malloc(sizeof(process_list));
	*background_processes = (process_list) {
		.size=1,
		.first_process=malloc(sizeof(process*))
	};
	background_processes->first_process[0] = NULL;
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
	process** result = background_processes->first_process;
	while(result)
	{
		if((*result)->pid == child_pid)
			return *result;
		(*result)++;
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

size_t process_list_append(process* p)
{
	/* Resize the array so another process can be inserted */
	process_list_resize(1);
	
	/* Append the process */
	size_t last_elem = background_processes->size - 2;
	background_processes->first_process[last_elem] = p;

	/* Add NULL termination */
	size_t last_index = background_processes->size - 1;
	background_processes->first_process[last_index] = NULL;

	/* Return the index at which the process was added */
	return last_elem;
}

void process_list_remove(process* p, bool clean)
{
	process* found = NULL;
	for(size_t i = 0; i < background_processes->size; i++)
	{
		if(background_processes->first_process[i] != p || found)
			continue;
		if(!found)
			found = background_processes->first_process[i];
		if(i+1 < background_processes->size)
			background_processes->first_process[i] = background_processes->first_process[i+1];
		else
			background_processes->first_process[i] = NULL;
	}
	if(found)
	{
		if(clean)
		{
			clean_process(found);
			found = NULL;
		}
		process_list_resize(-1);
	}
}

void process_list_resize(int n)
{
	size_t scalar = 1;
	size_t i = 0;
	
	/* Increment/Decrement process list size */
	background_processes->size += n;

	/* Only resize by power of two eg. 2, 4, 8, 16, 32.. */
	while((scalar<<=++i) < background_processes->size);

	/* Resize the struct */
	background_processes->first_process = realloc(
		background_processes->first_process,
		sizeof(process*) * scalar
	);
}

void print_background_processes()
{
	for(size_t i = 0; i < background_processes->size; i++)
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
				process_list_append(child_process);
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

	if(WIFSTOPPED(foreground_process->status))
	{
		foreground_process->stopped = true;
		size_t bgid = process_list_append(foreground_process);
		printf("[%lu]+\tStopped\t%s\n", bgid, foreground_process->argv[0]);
		foreground_process = NULL;
	}
}
