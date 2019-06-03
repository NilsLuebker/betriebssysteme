#include "main.h"

sig_atomic_t volatile not_dead = true;

void sigterm_handler(int sig)
{
	if(sig == SIGTERM) {
		not_dead = false;
	}
}

useless_stuff_t* create_useless_stuff(void)
{
	useless_stuff_t* stuff = malloc(sizeof(useless_stuff_t));
	stuff->a = random();
	stuff->b = random();
	stuff->c = random();
	stuff->next = NULL;
	return stuff;
}

int main(void)
{
	srandom((unsigned int)time(NULL));

	signal(SIGTERM, &sigterm_handler);

	useless_stuff_t* root = create_useless_stuff();
	useless_stuff_t* iter = root;

	for(int i = 0; i < SIZE; i++)
	{
		iter->next = create_useless_stuff();
		iter = iter->next;
	}

	while(not_dead)
	{
		iter = root;
		while(iter->next)
		{
			iter->a = iter->b * iter->c % RAND_MAX;
			iter->b = iter->a * iter->c % RAND_MAX;
			iter->c = iter->b * iter->a % RAND_MAX;
			iter = iter->next;
		}
	}

	while(root->next)
	{
		iter = root->next;
		free(root);
		root = iter;
	}

	return 0;
}
