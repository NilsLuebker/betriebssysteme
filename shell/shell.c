#include "shell.h"

interactive_shell shell;

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
	char* line = NULL;
	size_t line_size = 0;

	while(true)
	{
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
