#include "parameters.h"

int b, l, m, n, r = 0;

const parameter_t parameter_list[PARAMLIST_SIZE] = {
	/* gebackene broetchen pro r sekunden */
	{.name='b', .default_value=3, .min_value=1, .max_value=15, .global_ptr=&b},
	/* maximale laenge der schlange */
	{.name='l', .default_value=5, .min_value=1, .max_value=20, .global_ptr=&l},
	/* menge von kunden die sich anstellen */
	{.name='m', .default_value=10, .min_value=1, .max_value=100, .global_ptr=&m},
	/* anzahl an mitarbeitern */
	{.name='n', .default_value=2, .min_value=1, .max_value=10, .global_ptr=&n},
	/* zeit in sekunden zwischen dem backen der broetchen */
	{.name='r', .default_value=1, .min_value=1, .max_value=5, .global_ptr=&r},
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
				} else if(arg_value < parameter_list[i].min_value || arg_value > parameter_list[i].max_value) {
					printf(
						"The value '%ld' for flag '%c' did not fit the allowed range of [%d, %d]\n",
						arg_value,
						c,
						parameter_list[i].min_value,
						parameter_list[i].max_value
					);
					break;
				}
				printf("Flag '%c' has value %ld.\n", c, arg_value);
				*(parameter_list[i].global_ptr) = (int) arg_value;
				break;
			}
		}
	}
}

