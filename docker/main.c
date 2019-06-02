#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

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

useless_stuff_t* create_useless_stuff(void);

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

	printf("size: %lu\n", sizeof(useless_stuff_t));
	/* return 0; */
	useless_stuff_t* root = create_useless_stuff();
	useless_stuff_t* iter = root;

	for(int i = 0; i < SIZE; i++)
	{
		iter->next = create_useless_stuff();
		iter = iter->next;
	}

	while(true)
	{
		iter = root;
		while(iter->next)
		{
			printf("Stuff { a:%ld, b:%ld, c:%ld }\n", iter->a, iter->b, iter->c);
			iter->a = iter->b * iter->c % RAND_MAX;
			iter->b = iter->a * iter->c % RAND_MAX;
			iter->c = iter->b * iter->a % RAND_MAX;
			iter = iter->next;
		}
	}

	return 0;
}
