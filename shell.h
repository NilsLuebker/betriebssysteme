#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <stdbool.h>

#define BUILDINS_SIZE 3
#define PROMPT "\e[93m$\e[39m "

#define IS_CHILD_PROCESS(PID) (PID == 0)
#define STRLEN_INC_NULL(str) (strlen(str) + 1)
#define LAST_INDEX(arr_len) (arr_len - 1)

void logout(char**);
void test(char**);
char* get_arg(char**);
char** get_argv(char*);
void clean_argv(char**);
bool execute_buildins(char**);
void execute_system(char**);
void child_process(char*, char**);
void parent_process(pid_t);

struct Buildin {
	char* keyword;
	void (*func)(char**);
};

#endif /* SHELL_H */
