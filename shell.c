#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define EXECUTABLE_PATH "/bin/"

int getIndex(char* str, char c) {
	char* found = strchr(str, c);
	if(found)
		return (int) (found - str)/sizeof(char);
	else
		return -1;
}

char** createArgv(char* line, size_t line_size, size_t str_len) {
}

char* getExecutableFile(char* line, size_t line_size, size_t str_len) {
	char buf[128];
	char* space_pos = strchr(line, ' ');
	for(int i = 0; line != space_pos || i < line_size; i++) {
		buf[i] = *line;
		line++;
	}
}

int main(int argc, char** argv) {
	char* buf = NULL;
	size_t buf_size = 0;

	size_t str_len = getline(&buf, &buf_size, stdin);
	if(str_len == -1) {
		printf("no line\n");
		return -1;
	}
	printf("size:%lu len:%lu text:%s", buf_size, str_len, buf);
	printf("index:%d\n", getIndex(buf, 'a'));
	char** args;
	args = (char**) malloc(3 * sizeof(char*));
	args[0] = malloc(sizeof(char) * (strlen("/bin/ls") + 1));
	args[0] = "/bin/ls";
	args[1] = malloc(sizeof(char) * 4);
	args[1] = "-la";
	args[2] = NULL;

	int error = execvp("/bin/ls", args);
	free(buf);
	return error;
}
