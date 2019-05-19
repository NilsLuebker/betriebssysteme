#ifndef VENDOR_H
#define VENDOR_H

#include <stdlib.h>

#include "customer.h"

#define NANO_SECOND_MULTIPLIER 1000000

#define MS_TO_NS(ns) (ns * NANO_SECOND_MULTIPLIER)

void* vendor_thread_main(void* data);

#endif /* VENDOR_H */
