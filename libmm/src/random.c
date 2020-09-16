#include <limits.h>
#include "libmm/random.h"

unsigned long random_next( struct random *this, unsigned long min, unsigned long max ) {
	unsigned long tmp = this->a;

	this->d = this->c;
	this->c = this->b;
	this->b = this->a;

	tmp ^= tmp >> 2;
	tmp ^= tmp << 1;
	tmp ^= this->a ^ ( this->a << 4 );

	this->a = tmp;
	this->counter += 362437;

	tmp = this->a + this->counter;

	return min + ( tmp % ( max - min ) );
}

void random_reset( struct random *this, unsigned long seed ) {
	this->counter = 0;
	this->a = seed;
	this->b = 0;
	this->c = 0;
	this->d = 0;
}
