#include "jobctl.h"
#include "argparse.h"
#include "shell.h"

process* foreground_process = NULL;
process_list* background_processes = NULL;

void new_foreground_process(char** argv, pid_t pid)
{
	/* If there already is a foreground process free it's memory */
	if(foreground_process != NULL)
	{
		clean_argv(foreground_process->argv);
		free(foreground_process);
	}

	/* Give the process access to the terminal */
	setpgid(pid, pid);
	tcsetpgrp(shell.terminal, pid);

	/* Add new foreground process */
	foreground_process = malloc(sizeof(process));
	*foreground_process = (process) {
		.argv=argv,
		.pid=pid,
		.status=0
	};
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
			/* Prarent process */
			if(foreground)
			{
				new_foreground_process(argv, child_pid);
				wait_for_foreground_process();
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

	/* Stop ignoring job control signals in the child process. */
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
		printf("stopped\n");
	}
}
