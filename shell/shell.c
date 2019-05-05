#include "shell.h"

interactive_shell shell;

void child_handler(int sig)
{
	int child_status;
	pid_t child_pid = -1;
	process* child_process;
	while( (child_pid = waitpid(-1, &child_status, WUNTRACED | WNOHANG)) )
	{
		printf("%d\n", child_pid);
		if(child_pid == -1)
			break;
		/* Find child process and update it's status */
		child_process = find_process(child_pid);
		child_process->status = child_status;
		if(WIFEXITED(child_status))
			child_process->completed = true;
		if(WIFSTOPPED(child_status))
			child_process->stopped = true;
		/* if(WIFCONTINUED(child_pid)) */
		/* 	child_process->stopped = false; */
	}
}

void init_shell()
{
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
	signal(SIGCHLD, SIG_IGN);

	/* Listen for SIGCHLD signals */
	/* signal(SIGCHLD, &child_handler); */

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
		child_handler(0);
		printf(PROMPT);
		size_t str_len = getline(&line, &line_size, stdin);


		if(str_len <= 1) continue;

		char** argv = get_argv(line);

		if(!argv) continue;

		if(!execute_buildins(argv))
			execute_system(argv);
	}

	return 0;
}
