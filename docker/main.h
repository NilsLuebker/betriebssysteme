#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <signal.h>

#define MEMORY_IN_MB 1024
#define SIZE (MEMORY_IN_MB * 1024 * 32)

struct useless_stuff_s
{
	long a;
	long b;
	long c;
	struct useless_stuff_s* next;
};

typedef struct useless_stuff_s useless_stuff_t;
extern volatile sig_atomic_t not_dead;

useless_stuff_t* create_useless_stuff(void);
void sigterm_handler(int);

#endif
