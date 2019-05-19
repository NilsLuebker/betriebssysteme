#include "queue.h"

bool queue_destroy(queue_t* queue)
{
	if (queue == NULL)
		return false;

	while (queue->front != NULL)
	{
		struct queue_node_s *node = queue->front;
		queue->front = node->next;
		free(node);
	}

	pthread_mutex_destroy(&queue->mut);
	free(queue);
	return true;
}

bool queue_empty(queue_t* queue)
{
	bool result = false;
	pthread_mutex_lock(&queue->mut);
	if (queue == NULL || queue->front == NULL)
		result = true;
	pthread_mutex_unlock(&queue->mut);
	return result;
}

bool queue_full(queue_t* queue)
{
	bool result = false;
	pthread_mutex_lock(&queue->mut);
	if (queue->size == queue->size_limit)
		result = true;
	pthread_mutex_unlock(&queue->mut);
	return result;
}

void queue_init(queue_t* queue, size_t size_limit)
{
	queue->back = NULL;
	queue->front = NULL;
	queue->size_limit = size_limit;
	queue->size = 0;
	pthread_mutex_init(&queue->mut, NULL);
}

queue_t *queue_new(size_t size_limit)
{
	queue_t *queue = malloc(sizeof(*queue));

	if (queue == NULL)
		return NULL;

	queue_init(queue, size_limit);
	return queue;
}

void* queue_dequeue(queue_t* queue)
{
	pthread_mutex_lock(&queue->mut);
	if (queue == NULL || queue->front == NULL)
	{
		pthread_mutex_unlock(&queue->mut);
		return NULL;
	}

	struct queue_node_s *node = queue->front;
	void *data = node->data;
	queue->front = node->next;

	if (queue->front == NULL)
		queue->back = NULL;

	free(node);
	queue->size--;
	pthread_mutex_unlock(&queue->mut);
	return data;
}

bool queue_enqueue(queue_t* queue, void* data)
{
	if (queue == NULL)
		return false;

	pthread_mutex_lock(&queue->mut);
	if(queue->size >= queue->size_limit)
	{
		pthread_mutex_unlock(&queue->mut);
		return false;
	}

	struct queue_node_s* node = malloc(sizeof(*node));
	if (node == NULL)
	{
		pthread_mutex_unlock(&queue->mut);
		return false;
	}

	node->data = data;
	node->next = NULL;
	if (queue->back == NULL)
		queue->front = queue->back = node;
	else
	{
		queue->back->next = node;
		queue->back = node;
	}

	queue->size++;
	pthread_mutex_unlock(&queue->mut);
	return true;
}
