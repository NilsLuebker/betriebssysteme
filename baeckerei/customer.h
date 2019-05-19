#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#include "baeckerei.h"
#include "queue.h"

struct customer_s {
	unsigned int wanted_breads;
	unsigned int gotten_breads;
	pthread_mutex_t mut;
	sem_t sem;
};

typedef struct customer_s customer_t;

void* customer_thread_main(void* data);
void customer_stroll(void);
void customer_init(customer_t* customer);
customer_t* customer_new();
void customer_destroy(customer_t* customer);

#endif /* CUSTOMER_H */