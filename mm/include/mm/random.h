#ifndef MM_RANDOM_H
#define MM_RANDOM_H
#include "mm/common.h"

struct mm_random {
	unsigned long counter;
	unsigned long a;
	unsigned long b;
	unsigned long c;
	unsigned long d;
} mm_random_t;

MM_API unsigned long mm_random_next( struct mm_random *this, unsigned long min, unsigned long max );
MM_API void mm_random_reset( struct mm_random *this, unsigned long seed );

#endif
