#include "baker.h"

void* baker_thread_main(void* data)
{
	printf("starting: baker\n");
	baker_t* self = (baker_t*) data;
	for(int i = 0; i < 20; i++) {
		sleep((unsigned int) self->seconds);
		baker_add_breads(self, self->breads_per_second);
		printf("new breads\n");
	}
	return NULL;
}

void baker_init(baker_t* baker, int breads_per_second, int seconds)
{
	baker->breads = 0;
	baker->breads_per_second = breads_per_second;
	baker->seconds = seconds;
	pthread_mutex_init(&baker->mut, NULL);
}

baker_t* baker_new(int breads_per_second, int seconds)
{
	baker_t* baker = malloc(sizeof(baker_t));
	baker_init(baker, breads_per_second, seconds);
	return baker;
}

void baker_destroy(baker_t* baker)
{
	pthread_mutex_destroy(&baker->mut);
	free(baker);
}

void baker_add_breads(baker_t* baker, int breads)
{
	pthread_mutex_lock(&baker->mut);
	baker->breads += breads;
	pthread_mutex_unlock(&baker->mut);
}

int baker_get_breads(baker_t* baker)
{
	pthread_mutex_lock(&baker->mut);
	int breads = baker->breads;
	pthread_mutex_unlock(&baker->mut);
	return breads;
}

bool baker_has_breads(baker_t* baker)
{
	bool result = false;
	pthread_mutex_lock(&baker->mut);
	if (baker->breads > 0)
		result = true;
	pthread_mutex_unlock(&baker->mut);
	return result;
}

int baker_take_breads(baker_t* baker, int breads)
{
	pthread_mutex_lock(&baker->mut);
	if (baker->breads >= breads)
	{
		baker->breads -= breads;
	}
	else
	{
		breads = baker->breads;
		baker->breads = 0;
	}
	pthread_mutex_unlock(&baker->mut);
	return breads;
}
