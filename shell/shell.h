#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include <sys/wait.h>

#define PROMPT "\e[93m$\e[39m "

#define IS_CHILD_PROCESS(PID) (PID == 0)

void execute_system(char**);
void child_process(char*, char**);
void parent_process(pid_t);

#endif /* SHELL_H */
