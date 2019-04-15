#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <stdbool.h>

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
	if(!*argv) return NULL;
	return argv;
}

int main(int argc, char** args) {
	char* line = NULL;
	size_t line_size = 0;

	while(true) {
		printf("$ ");
		size_t str_len = getline(&line, &line_size, stdin);
		if(str_len <= 1) {
			printf("quit\n");
			return 0;
		}

		char** argv = get_argv(line, str_len);

		pid_t child_pid = fork();
		if(child_pid == 0) {
			int error = execvp(argv[0], argv);
			printf("Command not found [%d]\n", error);
			exit(0);
		}
		int status;
		pid_t pid = waitpid(child_pid, &status, 0);
		char** argv_begin = argv;
		while(*argv) {
			free(*argv);
			argv++;
		}
		free(argv_begin);
	}

	return 0;
}
