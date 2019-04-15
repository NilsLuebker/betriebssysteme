#include "shell.h"

const struct Buildin BUILDINS[BUILDINS_SIZE] = {
	{ .keyword="logout", .func=logout },
	{ .keyword="exit",   .func=logout },
	{ .keyword="test",   .func=test }
};

void logout(char** argv) {
	char* line = NULL;
	size_t line_size = 0;
	while(true) {
		printf("Are you sure you want to logout? (y/n) ");
		getline(&line, &line_size, stdin);
		switch(line[0]) {
			case 'y':
				exit(0);
			case 'n':
				return;
		}
	}
}

void test(char** argv) {
	if(*argv) {
		printf("Arg: %s\n", *argv);
	} else {
		printf("No Args\n");
	}
}

char* get_arg(char** line) {
	char* buf = malloc(strlen(*line) + 1);
	char* buf_begin = buf;
	char* result = NULL;
	while(**line) {
		if(!isspace((unsigned int) **line)) {
			*buf = **line;
			buf++;
		} else if(buf != buf_begin) {
			*buf = '\0';
			result = malloc(sizeof(char) * (strlen(buf_begin) + 1));
			strcpy(result, buf_begin);
			break;
		}
		(*line)++;
	}
	free(buf_begin);
	return result;
}

char** get_argv(char* line) {
	size_t argv_len = 1;
	char** argv = malloc(sizeof(char*));
	argv[0] = (char*) NULL;
	char* arg;
	while( (arg = get_arg(&line)) ) {
		argv[argv_len - 1] = arg;
		argv_len++;
		argv = realloc(argv, sizeof(char*) * argv_len);
		argv[argv_len - 1] = (char*) NULL;
	}
	if(!argv[0]) {
		free(argv);
		return NULL;
	}
	return argv;
}

void clean_argv(char** argv) {
	char** argv_begin = argv;
	while(*argv) {
		free(*argv);
		argv++;
	}
	free(argv_begin);
}

bool execute_buildins(char** argv) {
	for(int i = 0; i < BUILDINS_SIZE; i++) {
		if(!strcmp(argv[0], BUILDINS[i].keyword)) {
			BUILDINS[i].func(++argv);
			return true;
		}
	}
	return false;
}

void execute_system(char** argv) {
	pid_t pid = fork();
	if(IS_CHILD_PROCESS(pid))
		child_process(argv[0], argv);
	else
		parent_process(pid);
}

void child_process(char* file, char** argv) {
	int error = execvp(file, argv);
	printf("Command not found [%d]\n", error);
	exit(0);
}

void parent_process(pid_t child_pid) {
	int status;
	pid_t pid = waitpid(child_pid, &status, 0);
}

int main(int argc, char** args) {
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
