#include "shell.h"

const struct Buildin BUILDINS[BUILDINS_SIZE] = {
	{ .keyword="logout", .func=logout },
	{ .keyword="exit",   .func=logout },
	{ .keyword="test",   .func=test }
};

void logout(char** argv) {
	printf("bye\n");
	exit(0);
}

void test(char** argv) {
	if(*argv) {
		printf("Arg: %s\n", *argv);
	} else {
		printf("No Args\n");
	}
}

char* get_arg(char** line, size_t* line_size) {
	char* buf = malloc(*line_size);
	char* buf_begin = buf;
	char* result = NULL;
	bool found_arg = false;
	for(int i = 1; i < *line_size && **line; i++) {
		if(isspace((unsigned int) **line)) {
			(*line)++;
			continue;
		}
		*buf = **line;
		buf++, (*line)++;
		if(!isspace((unsigned int) **line) && **line != '\0') continue;
		*buf = '\0';
		found_arg = true;
		*line_size -= i;
		break;
	}
	if(found_arg) {
		result = malloc(sizeof(char) * (strlen(buf_begin) + 1));
		strcpy(result, buf_begin);
	}
	free(buf_begin);
	return result;
}

char** get_argv(char* line, size_t line_size) {
	size_t argv_len = 1;
	char** argv = malloc(sizeof(char*));
	argv[0] = (char*) NULL;
	char* arg;
	while( (arg = get_arg(&line, &line_size)) ) {
		argv[argv_len - 1] = arg;
		argv_len++;
		argv = realloc(argv, sizeof(char*) * argv_len);
		argv[argv_len - 1] = (char*) NULL;
	}
	if(!*argv) {
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

		char** argv = get_argv(line, str_len);

		if(!argv) continue;

		if(!execute_buildins(argv))
			execute_system(argv);

		clean_argv(argv);
	}

	return 0;
}
