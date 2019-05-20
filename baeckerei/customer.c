#include "customer.h"

void* customer_thread_main(void* data)
{
	printf("starting: customer id[%s%lu%s]\n", MAGENTA, pthread_self(), RESET);
	customer_t* self = (customer_t*) data;

	while(true)
	{
		customer_stroll();
		if (!baker_has_breads(stock)) continue;
		if (queue_full(queue)) continue;
		if (!queue_enqueue(queue, (void*) self)) continue;
		break;
	}

	sem_wait(&self->sem);

	printf(
		"Customer %s%lu%s got %s%u%s breads from vendor %s%lu%s and wanted %s%u%s breads\n",
		MAGENTA, pthread_self(), RESET,
		GREEN, self->gotten_breads, RESET,
		YELLOW, self->vendor, RESET,
		BLUE, self->wanted_breads, RESET
	);
	
	number_of_customers--;
	customer_destroy(self);
	return NULL;
}

void customer_stroll(void)
{
	unsigned int time_to_sleep = (unsigned int)((random() % 12) + 1);
	printf(
		"Customer %s%lu%s is wandering around for %s%u%s\n",
		MAGENTA, pthread_self(), RESET,
		BLUE, time_to_sleep, RESET
	);
	sleep(time_to_sleep);
}

void customer_init(customer_t* customer)
{
	customer->wanted_breads = (int)((random() % 5) + 1);
	customer->gotten_breads = 0;
	customer->vendor = 0;
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
	sem_destroy(&customer->sem);
	free(customer);
}

void customer_give_breads(customer_t* customer, pthread_t vendor, int breads)
{
	customer->gotten_breads = breads;
	customer->vendor = vendor;
}
