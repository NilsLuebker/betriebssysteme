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

extern baker_t* stock;
extern queue_t* queue;

int main(int argc, char* argv[]);

#endif /* BAECKEREI_H */
