#ifndef BUILDINS_H
#define BUILDINS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define BUILDINS_SIZE 3

struct Buildin {
	char* keyword;
	void (*func)(char**);
};

bool execute_buildins(char**);
void logout(char**);
void test(char**);

#endif /* BUILDINS_H */
