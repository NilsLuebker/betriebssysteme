#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <stdbool.h>

#define EXECUTABLE_PATH "/bin/"

char* get_word(char** line, size_t* line_size) {
	char* buf = malloc(*line_size);
	char* buf_begin = buf;
	bool found_word = false;
	for(int i = 1; i < *line_size || **line == '\0'; i++) {
		if(isspace((unsigned int) **line)) {
			(*line)++;
			continue;
		}
		*buf = **line;
		buf++, (*line)++;
		if(!isspace((unsigned int) **line)) continue;
		*buf = '\0';
		buf = buf_begin;
		found_word = true;
		*line_size -= i;
		break;
	}
	char* result = NULL;
	if(found_word) {
		result = malloc(sizeof(char) * (strlen(buf) + 1));
		strcpy(result, buf);
	}
	free(buf_begin);
	return result;
}

char** get_argv(char* line, size_t line_size) {
	size_t argv_len = 1;
	char** argv = malloc(sizeof(char*));
	argv[0] = (char*) NULL;
	char* word;
	while( (word = get_word(&line, &line_size)) ) {
		argv[argv_len - 1] = word;
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
		/* printf("size:%lu len:%lu text:%s", line_size, str_len, line); */

		char** argv = get_argv(line, line_size);
		/* char* file; */
		/* if(argv) { */
		/* 	file = malloc(sizeof(char) * (strlen(argv[0]) + 1)); */
		/* 	strcpy(file, *argv); */
		/* } */

		pid_t child_pid = fork();
		if(child_pid == 0) {
			int error = execvp(argv[0], argv);
			printf("Error %d\n", error);
			exit(0);
		}
		int status;
		pid_t pid = waitpid(child_pid, &status, 0);
		/* printf("pid: %d status: %d\n", pid, status); */
		/* free(file); */
		char** argv_begin = argv;
		while(*argv) {
			free(*argv);
			argv++;
		}
		free(argv_begin);
	}

	return 0;
}
