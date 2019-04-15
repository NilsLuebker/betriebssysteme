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
#define PROMPT "$ "

#define IS_CHILD_PROCESS(PID) (PID == 0)

void logout(char** argv);
void test(char** argv);
char* get_arg(char** line, size_t* line_size);
char** get_argv(char* line, size_t line_size);
void clean_argv(char** argv);
bool execute_buildins(char** argv);
void execute_system(char** argv);
void child_process(char* file, char** argv);
void parent_process(pid_t child_pid);

struct Buildin {
	char* keyword;
	void (*func)(char**);
};

#endif /* SHELL_H */
