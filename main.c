#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

	int data_a = 2;
	int data_b = 5.0;
	int data_c = 'a';

void useless_function(void) {
	printf("\e[33m========================[Function]=========================\e[0m\e[0m\n");
	int stack_a = 2;
	int stack_b = 5.0;
	int stack_c = 'a';

	int* heap_a = malloc(sizeof(int));
	int* heap_b = malloc(sizeof(int));
	int* heap_c = malloc(sizeof(int));

	printf("Address: %p\n", &useless_function);
	printf("Stack:\t [ a:%p, b:%p, c:%p ]\n", &stack_a, &stack_b, &stack_c);
	printf("Heap:\t [ a:%p, b:%p, c:%p ]\n", heap_a, heap_b, heap_c);

	free(heap_a);
	free(heap_b);
	free(heap_c);
}

void print_limit(int resource, char* name) {
	struct rlimit stacklimit;
	getrlimit(resource, &stacklimit);
	printf("%s:\t [ soft:%ld, hard:%ld ]\n", name, stacklimit.rlim_cur, stacklimit.rlim_max);
}

int main(int argc, const int* argv[]) {
	printf("\e[33m=========================[Global]==========================\e[0m\n");

	printf("Data:\t [ a:%p, b:%p, c:%p ]\n", &data_a, &data_b, &data_c);

	printf("\e[33m==========================[Main]===========================\e[0m\n");
	int stack_a = 2;
	int stack_b = 5.0;
	int stack_c = 'a';

	int* heap_a = malloc(sizeof(int));
	int* heap_b = malloc(sizeof(int));
	int* heap_c = malloc(sizeof(int));

	printf("Address: %p\n", &main);
	printf("Stack:\t [ a:%p, b:%p, c:%p ]\n", &stack_a, &stack_b, &stack_c);
	printf("Heap:\t [ a:%p, b:%p, c:%p ]\n", heap_a, heap_b, heap_c);

	free(heap_a);
	free(heap_b);
	free(heap_c);

	useless_function();

	printf("\e[33m=========================[Limits]===========================\e[0m\n");
	print_limit(RLIMIT_STACK, "Stack");
	print_limit(RLIMIT_DATA, "Data");
}
