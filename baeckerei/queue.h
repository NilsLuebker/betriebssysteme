#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include <stdio.h>

struct queue_node_s {
	struct queue_node_s* next;
	void* data;
};

struct queue_s {
	struct queue_node_s* front;
	struct queue_node_s* back;
	size_t size_limit;
	size_t size;
	pthread_mutex_t mut;
};

typedef struct queue_s queue_t;

bool queue_destroy(queue_t* queue);
bool queue_empty(queue_t* queue);
bool queue_full(queue_t* queue);
queue_t* queue_new(size_t size_limit);
void* queue_dequeue(queue_t* queue);
bool queue_enqueue(queue_t* queue, void* data);

#endif /* QUEUE_H */
