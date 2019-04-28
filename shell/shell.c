#include "shell.h"
#include "buildins.h"
#include "argparse.h"

void signal_handler(int signal)
{
	printf("got signal");
	exit(0);
}

void execute_system(char** argv) {
	bool should_be_background_process = check_last_char(argv, '&');
	if(should_be_background_process) argv = trim_last_arg(argv);
	pid_t pid = fork();
	switch(pid) {
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

void child_process(char* file, char** argv) {
	int error = execvp(file, argv);
	printf("Command not found [%d]\n", error);
	exit(0);
}

void parent_process(pid_t child_pid) {
	int wstatus;
	pid_t pid = waitpid(child_pid, &wstatus, 0);
}

int main(int argc, char** args) {
	signal(SIGSTOP, &signal_handler);
	signal(SIGTSTP, &signal_handler);
	char* line = NULL;
	size_t line_size = 0;

	while(true) {
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
