#include "shell.h"

interactive_shell shell;

struct sigaction sa_child;

void child_handler(int sig)
{
	/* Only handle SIGCHLD signals */
	if(sig != SIGCHLD)
		return;

	int child_status;
	pid_t child_pid = -1;
	process* child_process;
	while( (child_pid = waitpid(WAIT_ANY, &child_status, WNOHANG | WCONTINUED | WUNTRACED)) > 0 )
	{
		if(child_pid == -1)
		{
			if(errno == ECHILD)
				printf("ECHILD\n");
			if(errno == EFAULT)
				printf("EFAULT\n");
			if(errno == EINTR)
				printf("EINTR\n");
			if(errno == EINVAL)
				printf("EINVAL\n");
			if(errno == ENOSYS)
				printf("ENOSYS\n");
			break;
		}

		/* Find child process and update it's status */
		child_process = find_process(child_pid);
		if(!child_process) continue;
		child_process->status = child_status;
		if(WIFEXITED(child_status))
		{
			printf("[%d] was completed (%s)\n", child_pid, child_process->argv[0]);
			child_process->completed = true;
		}
		if(WIFSTOPPED(child_status))
		{
			printf("[%d] was stopped (%s)\n", child_pid, child_process->argv[0]);
			child_process->stopped = true;
		}
		if(WIFCONTINUED(child_pid))
		{
			printf("[%d] was continued (%s)\n", child_pid, child_process->argv[0]);
			child_process->stopped = false;
		}
	}
}

void put_shell_in_foreground()
{
	/* Put the shell back in the foreground */
	tcsetpgrp(shell.terminal, shell.pgid);
	/* Restore the shells terminal modes */
	tcsetattr(shell.terminal, TCSADRAIN, &shell.tmodes);
}

void init_shell()
{
	/* Initialize sigaction struct for signal handling */
	sa_child.sa_handler = &child_handler;
	sa_child.sa_flags = SA_RESTART;
	sigemptyset(&sa_child.sa_mask);

	/* See if we are running interactively. */
	shell.terminal = STDIN_FILENO;
	shell.is_interactive = isatty(shell.terminal);
	if(!shell.is_interactive) return;

	/* Loop until we are in the foreground. */
	while(tcgetpgrp(shell.terminal) != (shell.pgid = getpgrp()))
		kill(-shell.pgid, SIGTTIN);

	/* Ignore interactive and job-control signals. */
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);

	/* Put ourselves in our own process group. */
	shell.pgid = getpid();
	if(setpgid(shell.pgid, shell.pgid) < 0)
	{
		perror("Couldn't put the shell in its own process group");
		exit(1);
	}

	/* Grab control of the terminal. */
	tcsetpgrp(shell.terminal, shell.pgid);

	/* Save default terminal attributes for shell. */
	tcgetattr(shell.terminal, &shell.tmodes);
}

int main(int argc, char** args)
{
	init_shell();
	init_jobctl();
	char* line = NULL;
	size_t line_size = 0;

	while(true)
	{
		child_handler(SIGCHLD);
		list_gc(background_processes);
		printf(PROMPT);

		/* Handle child events while waiting for input by the user */
		/* sigaction(SIGCHLD, &sa_child, NULL); */
		size_t str_len = getline(&line, &line_size, stdin);
		signal(SIGCHLD, SIG_DFL);


		if(str_len <= 1) continue;

		char** argv = get_argv(line);

		if(!argv) continue;

		if(!execute_buildins(argv))
			execute_system(argv);
	}

	return 0;
}
