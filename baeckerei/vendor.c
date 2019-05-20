#include "vendor.h"

void* vendor_thread_main(void* data)
{
	printf("starting: vendor id[%s%lu%s]\n", YELLOW, pthread_self(), RESET);
	customer_t* current_customer = NULL;
	const struct timespec time_to_sleep = {.tv_nsec=MS_TO_NS(250)};
	while(number_of_customers)
	{
		nanosleep(&time_to_sleep, NULL);
		if (!baker_has_breads(stock)) continue;
		if (queue_empty(queue)) continue;
		current_customer = (customer_t*) queue_dequeue(queue);
		if (current_customer == NULL) continue;
		int breads = baker_take_breads(stock, current_customer->wanted_breads);
		customer_give_breads(current_customer, pthread_self(), breads);
		sem_post(&current_customer->sem);
		/* customer_give_breads(current_customer, ); */
	}
	return NULL;
}

