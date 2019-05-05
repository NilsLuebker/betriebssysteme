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
	process* result = malloc(sizeof(process));
	*result = (process) {
		.argv=argv,
		.pid=pid,
		.status=0
	};
	return result;

}

process* find_process(pid_t child_pid)
{
	if(foreground_process && foreground_process->pid == child_pid)
		return foreground_process;
	process* result = background_processes->first_process;
	while(result)
	{
		if(result->pid = child_pid)
			return result;
		result++;
	}
	return NULL;
}

void move_foreground(process* p)
{
	/* If there already is a foreground process free it's memory */
	if(foreground_process != NULL)
	{
		clean_argv(foreground_process->argv);
		free(foreground_process);
	}

	/* Give the process access to the terminal */
	setpgid(p->pid, p->pid);
	tcsetpgrp(shell.terminal, p->pid);

	/* Add new foreground process */
	foreground_process = p;
}

size_t move_background(process* p)
{
	/* Resize the array so another process can be inserted */
	background_processes->size++;
	background_processes->first_process = realloc(
		background_processes->first_process,
		sizeof(process*) * background_processes->size
	);

	/* Append the process */
	size_t last_elem = background_processes->size - 2;
	background_processes->first_process[last_elem] = p;

	/* Add NULL termination */
	size_t last_index = background_processes->size - 1;
	background_processes->first_process[last_index] = NULL;
	return last_elem;
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
		else
			printf(
				"[%lu]\tCompleted\t%s\n",
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
				move_background(child_process);
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

	/* Put the shell back in the foreground */
	tcsetpgrp(shell.terminal, shell.pgid);
	/* Restore the shells terminal modes */
	tcsetattr(shell.terminal, TCSADRAIN, &shell.tmodes);

	if(WIFSTOPPED(foreground_process->status))
	{
		foreground_process->stopped = true;
		size_t bgid = move_background(foreground_process);
		printf("[%lu]+\tStopped\t%s\n", bgid, foreground_process->argv[0]);
		foreground_process = NULL;
	}
}
