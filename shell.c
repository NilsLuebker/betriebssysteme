#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define EXECUTABLE_PATH "/bin/"

char* get_word(char** line, size_t* line_size) {
	char* buf = malloc(*line_size);
	char* buf_begin = buf;
	for(int i = 0; i < *line_size || **line == '\0'; i++) {
		if(isspace((unsigned int) **line)) {
			(*line)++;
			continue;
		}
		*buf = **line;
		buf++, (*line)++;
		if(!isspace((unsigned int) **line)) continue;
		*buf = '\0';
		buf = buf_begin;
		line_size -= i;
		break;
	}
	char* result = malloc(sizeof(char) * (strlen(buf) + 1));
	strcpy(result, buf);
	free(buf);
	return result;
}

int parse_line(char* file_path, char** argv, char* line, size_t line_size) {
	char* buf = malloc(line_size);
	char* buf_begin = buf;
	int file_path_set = 0;
	while(*line) {
	}
	free(buf);
	return 0;
}

int main(int argc, char** args) {
	char* line = NULL;
	size_t line_size = 0;

	printf("$ ");

	size_t str_len = getline(&line, &line_size, stdin);
	if(str_len == -1) {
		printf("no line\n");
		return -1;
	}
	printf("size:%lu len:%lu text:%s", line_size, str_len, line);

	printf("%s\n", get_word(&line, &line_size));
	if(*line != '\0')
		printf("%s\n", get_word(&line, &line_size));

	/* printf("index:%d\n", getIndex(buf, 'a')); */
	/* char** argv; */
	/* char* file_path; */
	/* parse_line(&file_path, &argv, line, line_size); */
	/* args = (char**) malloc(3 * sizeof(char*)); */
	/* args[0] = malloc(sizeof(char) * (strlen("/bin/ls") + 1)); */
	/* args[0] = "/bin/ls"; */
	/* args[1] = malloc(sizeof(char) * 4); */
	/* args[1] = "-la"; */
	/* args[2] = NULL; */

	/* int error = execvp(file_path, argv); */
	/* free(line); */
	/* free(file_path); */
	return 0;
}
