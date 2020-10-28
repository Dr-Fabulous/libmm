#ifndef MM_RANDOM_H
#define MM_RANDOM_H
#include "mm/common.h"

/*! \file */

/*!
	\brief Simple xorshift random number generator.
*/
struct mm_random {
	unsigned long counter;
	unsigned long a;
	unsigned long b;
	unsigned long c;
	unsigned long d;
} mm_random_t;

/*!
	\brief Get a new random number within the passed range.
*/
MM_API unsigned long mm_random_next( struct mm_random *this, unsigned long min, unsigned long max );

/*!
	\breif Reset & reseed a mm_random.
*/
MM_API void mm_random_reset( struct mm_random *this, unsigned long seed );

#endif
