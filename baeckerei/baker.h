#ifndef BAKER_H
#define BAKER_H

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <stdio.h>

#include "colors.h"

struct baker_s {
	int breads;
	int breads_per_second;
	int seconds;
	pthread_mutex_t mut;
};

typedef struct baker_s baker_t;
extern size_t number_of_customers;

void* baker_thread_main(void* data);
void baker_init(baker_t* baker, int breads_per_second, int seconds);
baker_t* baker_new(int breads_per_second, int seconds);
void baker_destroy(baker_t* baker);
void baker_add_breads(baker_t* baker, int breads);
int baker_get_breads(baker_t* baker);
bool baker_has_breads(baker_t* baker);
int baker_take_breads(baker_t* baker, int breads);

#endif /* BAKER_H */
