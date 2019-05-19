#ifndef BAECKEREI_H
#define BAECKEREI_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <time.h>

#include "parameters.h"
#include "queue.h"
#include "customer.h"
#include "vendor.h"
#include "baker.h"
#include "colors.h"

extern baker_t* stock;
extern queue_t* queue;
extern size_t number_of_customers;

int main(int argc, char* argv[]);

#endif /* BAECKEREI_H */
