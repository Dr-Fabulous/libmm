#include "mm/common.h"
#include "mm/random.h"
#include "mm/unit.h"
#include <time.h>

#define MIN 0
#define MAX 100
#define ITERATIONS 100000
#define TOLERENCE 0.2f

MM_UNIT_CASE( uniform_dist_values_case, NULL, NULL ) {
	unsigned long hits[ MAX - MIN ] = { 0 };
	struct mm_random r = { 0 };
	
	mm_random_reset( &r, ( unsigned long ) time( NULL ) );

	for ( unsigned long i = 0; i < ITERATIONS; ++i ) {
		++hits[ mm_random_next( &r, MIN, MAX ) ];
	}

	for ( unsigned long i = 0; i < MAX - MIN; ++i ) {
		float hit = ( ( float ) hits[ i ] / ( float ) ITERATIONS ) * 100.0f;
		MM_UNIT_ASSERT_RANGE( 1.0f - TOLERENCE, 1.0f + TOLERENCE, hit );
	}

	return MM_UNIT_DONE;
}

MM_UNIT_CASE( uniform_dist_seeds_case, NULL, NULL ) {
	unsigned long hits[ MAX - MIN ] = { 0 };
	struct mm_random r = { 0 };
	
	for ( unsigned long i = 0; i < ITERATIONS; ++i ) {
		mm_random_reset( &r, i );
		++hits[ mm_random_next( &r, MIN, MAX ) ];
	}

	for ( unsigned long i = 0; i < MM_ARR_SIZE( hits ); ++i ) {
		float hit = ( ( float ) hits[ i ] / ( float ) ITERATIONS ) * 100.0f;
		MM_UNIT_ASSERT_RANGE( 1.0f - TOLERENCE, 1.0f + TOLERENCE, hit );
	}

	return MM_UNIT_DONE;
}

MM_UNIT_SUITE( random_suite ) {
	MM_UNIT_RUN( uniform_dist_values_case );
	MM_UNIT_RUN( uniform_dist_seeds_case );
	return MM_UNIT_DONE;
}

int main( int argc, const char *argv[] ) {
	MM_UNIT_RUN_SUITE( random_suite );
	return 0;
}
