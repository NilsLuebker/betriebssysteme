#include "customer.h"

void* customer_thread_main(void* data)
{
	customer_t* self = (customer_t*) data;

	while(true)
	{
		customer_stroll();
		if (!baker_has_breads(stock)) continue;
		if (queue_full(queue)) continue;
		if (!queue_enqueue(queue, (void*) self)) continue;
		sem_wait(&self->sem);
		break;
	}

	customer_destroy(self);
	return NULL;
}

void customer_stroll(void)
{
	sleep((unsigned int)((random() % 12) + 1));
}

void customer_init(customer_t* customer)
{
	customer->wanted_breads = (unsigned int)((random() % 12) + 1);
	customer->gotten_breads = 0;
	pthread_mutex_init(&customer->mut, NULL);
	sem_init(&customer->sem, 0, 0);
}

customer_t* customer_new()
{
	customer_t* customer = malloc(sizeof(customer_t));
	customer_init(customer);
	return customer;
}

void customer_destroy(customer_t* customer)
{
	pthread_mutex_destroy(&customer->mut);
	sem_destroy(&customer->sem);
	free(customer);
}
