#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

struct parameter_t {
	int name;
	int default_value;
	int* global_ptr;
};

int b, l, m, n, r = 0;

#define PARAMLIST_SIZE 5
struct parameter_t parameter_list[PARAMLIST_SIZE] = {
	{.name='b', .default_value=3, .global_ptr=&b},
	{.name='l', .default_value=10, .global_ptr=&l},
	{.name='m', .default_value=5, .global_ptr=&m},
	{.name='n', .default_value=2, .global_ptr=&n},
	{.name='r', .default_value=1, .global_ptr=&r},
};

void init_defaults(void)
{
	for(int i = 0; i < PARAMLIST_SIZE; ++i) {
		*(parameter_list[i].global_ptr) = parameter_list[i].default_value;
	}
}

void print_parameters(void)
{
	printf("{%d}[\n", PARAMLIST_SIZE);
	for(int i = 0; i < PARAMLIST_SIZE; ++i) {
		if(i != 0) printf(",\n");
		if(*(parameter_list[i].global_ptr) == parameter_list[i].default_value) {
			printf("\t~%c: %d", parameter_list[i].name, *(parameter_list[i].global_ptr));
		} else {
			printf("\t %c: %d", parameter_list[i].name, *(parameter_list[i].global_ptr));
		}
	}
	printf("\n]\n");
}

void parse_args(int argc, char* argv[])
{
	int c;
	while ((c = getopt(argc, argv, "b:l:m:n:r:")) != -1) {
		if(c == '?') {
			printf("Use (blmnr).\n");
			continue;
		}
		for(int i = 0; i < PARAMLIST_SIZE; ++i) {
			if(parameter_list[i].name == c) {
				long arg_value = strtol(optarg, NULL, 10);
				if(arg_value == LONG_MAX || arg_value == LONG_MIN) {
					printf("The value provided for flag '%c' was too long.\n", c);
					break;
				} else if(arg_value == 0) {
					printf("The value '%s' could not be converted to a number.\n", optarg);
					break;
				} else if(arg_value < 0) {
					printf("No values smaller then 0 allowed.\n");
					break;
				}
				printf("Flag '%c' has value %ld.\n", c, arg_value);
				*(parameter_list[i].global_ptr) = (int) arg_value;
				break;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	init_defaults();
	parse_args(argc, argv);
	print_parameters();

	return 0;
}
