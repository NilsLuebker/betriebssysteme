#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <termios.h>
#include <signal.h>

#include <sys/wait.h>
#include <sys/types.h>

#include "buildins.h"
#include "argparse.h"
#include "jobctl.h"

#define PROMPT "\e[93m$\e[39m "

#define IS_CHILD_PROCESS(PID) (PID == 0)

typedef struct interactive_shell {
	pid_t pgid;
	struct termios tmodes;
	int terminal;
	bool is_interactive;
} interactive_shell;

extern interactive_shell shell;

void init_shell(void);
int main(int, char**);

#endif /* SHELL_H */
