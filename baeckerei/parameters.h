#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>

#define PARAMLIST_SIZE 5

struct parameter_s {
	int name;
	int default_value;
	int min_value;
	int max_value;
	int* global_ptr;
};

typedef struct parameter_s parameter_t;

extern int b, l, m, n, r;

void init_defaults(void);
void print_parameters(void);
void parse_args(int argc, char* argv[]);

#endif /* PARAMETERS_H */
