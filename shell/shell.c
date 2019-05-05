#include "shell.h"
#include "buildins.h"
#include "argparse.h"

void lel(int a) {
	printf("boi");
	exit(0);
}

void execute_system(char** argv)
{
	bool should_be_background_process = check_last_char(argv, '&');
	if(should_be_background_process) argv = trim_last_arg(argv);
	pid_t pid = fork();
	switch(pid)
	{
		case -1:
			printf("Error while trying to fork()!");
			exit(EXIT_FAILURE);
		case 0:
			child_process(argv[0], argv);
		default:
			if(!should_be_background_process)
				parent_process(pid);
	}
}

void child_process(char* file, char** argv)
{
	int error = execvp(file, argv);
	printf("Command not found [%d]\n", error);
	exit(0);
}

void parent_process(pid_t child_pid)
{
	int wstatus;
	pid_t pid = waitpid(child_pid, &wstatus, 0);
}

pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;
int shell_is_interactive;


/* Make sure the shell is running interactively as the foreground job
 *    before proceeding. */

void init_shell()
{

	/* See if we are running interactively.  */
	shell_terminal = STDIN_FILENO;
	shell_is_interactive = isatty (shell_terminal);

	if (shell_is_interactive)
	{
		printf("test");
		/* Loop until we are in the foreground.  */
		while (tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp ()))
			kill (- shell_pgid, SIGTTIN);

		/* Ignore interactive and job-control signals.  */
		signal (SIGINT, SIG_IGN);
		signal (SIGQUIT, SIG_IGN);
		signal (SIGTSTP, SIG_IGN);
		signal (SIGTTIN, SIG_IGN);
		signal (SIGTTOU, SIG_IGN);
		signal (SIGCHLD, SIG_IGN);

		/* Put ourselves in our own process group.  */
		shell_pgid = getpid ();
		if (setpgid (shell_pgid, shell_pgid) < 0)
		{
			perror ("Couldn't put the shell in its own process group");
			exit (1);
		}

		/* Grab control of the terminal.  */
		tcsetpgrp (shell_terminal, shell_pgid);

		/* Save default terminal attributes for shell.  */
		tcgetattr (shell_terminal, &shell_tmodes);
	}
}

int main(int argc, char** args)
{
	init_shell();
	signal(SIGINT, lel);
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

		clean_argv(argv);
	}

	return 0;
}
