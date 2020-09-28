#include <time.h>
#include "libmm/random.h"
#include "libmm/unit.h"

#define MIN 0
#define MAX 100
#define ITERATIONS 100000000

UNIT_CASE( uniform_dist_case, NULL, NULL ) {
	unsigned long hits[ MAX - MIN ] = { 0 };
	struct random r = { 0 };
	
	random_reset( &r, ( unsigned long ) time( NULL ) );

	for ( unsigned long i = 0; i < ITERATIONS; ++i ) {
		++hits[ random_next( &r, MIN, MAX ) ];
	}

	const float tolerence = 0.1f;

	for ( unsigned long i = 0; i < MAX - MIN; ++i ) {
		float hit = ( ( float ) hits[ i ] / ( float ) ITERATIONS ) * 100.0f;
		UNIT_ASSERT( hit > ( 1.0f - tolerence ) && hit < ( 1.0f + tolerence ), "random dist outside of acceptable range" );
	}

	return UNIT_DONE;
}

UNIT_SUITE( random_suite ) {
	UNIT_RUN( uniform_dist_case );
	return UNIT_DONE;
}
