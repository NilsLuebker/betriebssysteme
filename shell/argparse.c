#include "argparse.h"

char* get_arg(char** line) {
	char* buf = malloc(STRLEN_INC_NULL(*line));
	char* buf_begin = buf;
	char* result = NULL;
	while(**line) {
		if(!isspace((unsigned int) **line)) {
			*buf = **line;
			buf++;
		} else if(buf != buf_begin) {
			*buf = '\0';
			result = malloc(sizeof(char) * STRLEN_INC_NULL(buf_begin));
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
		argv[LAST_INDEX(argv_len)] = arg;
		argv_len++;
		argv = realloc(argv, sizeof(char*) * argv_len);
		argv[LAST_INDEX(argv_len)] = (char*) NULL;
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

// Only works with NULL terminated arrays
size_t arrlen(void** arr) {
	size_t size = 0;
	while(*(arr++)){
		size++;
	}
	return size;
}

bool check_last_char(char** argv, char c) {
	size_t argv_length = arrlen((void**)argv);
	if(argv[LAST_INDEX(argv_length)][0] == c) return true;
	return false;
}

char** trim_last_arg(char** argv) {
	size_t argv_length = arrlen((void**)argv);
	argv = realloc(argv, sizeof(char*) * LAST_INDEX(argv_length));
	argv[LAST_INDEX(argv_length)] = NULL;
	return argv;
}
