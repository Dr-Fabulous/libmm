#ifndef LIBMM_RANDOM_H
#define LIBMM_RANDOM_H
#include "libmm/common.h"

struct random {
	unsigned long counter;
	unsigned long a;
	unsigned long b;
	unsigned long c;
	unsigned long d;
} random_t;

LIBMM_FUNCTION unsigned long random_next( struct random *this, unsigned long min, unsigned long max );
LIBMM_FUNCTION void random_reset( struct random *this, unsigned long seed );

#endif
