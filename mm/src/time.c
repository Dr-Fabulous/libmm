#include "mm/time.h"

bool mm_timespec_get( struct timespec * const out ) {
#if MM_USING_POSIX
	return !clock_gettime( CLOCK_MONOTONIC, out );
#elif __STDC_VERSION__ >= 201109L
	return timespec_get( out, TIME_UTC ) == TIME_UTC;
#else
	clock_t time = clock();

	out->tv_sec = ( time_t ) ( time / CLOCKS_PER_SEC );
	out->time_nsec = ( long ) ( ( time % CLOCKS_PER_SEC ) / ( CLOCKS_PER_SEC / 1000 ) );

	return time != ( clock_t ) -1;
#endif
}

static inline void mm_timespec_fix( struct timespec * const this ) {
	if ( this->tv_nsec > MM_TIMESPEC_NSEC_MAX ) {
		this->tv_sec += ( time_t ) ( this->tv_nsec / MM_TIMESPEC_NSEC_MAX );
		this->tv_nsec %= MM_TIMESPEC_NSEC_MAX;
	} else if ( this->tv_nsec < 0 ) {
		this->tv_sec += ( time_t ) ( this->tv_nsec / MM_TIMESPEC_NSEC_MAX );
		this->tv_nsec = MM_TIMESPEC_NSEC_MAX - this->tv_nsec;
	}
}

void mm_timespec_add( struct timespec const * const lhs, struct timespec const * const rhs, struct timespec * const out ) {
	out->tv_sec = lhs->tv_sec + rhs->tv_sec;
	out->tv_nsec = lhs->tv_nsec + rhs->tv_nsec;

	mm_timespec_fix( out );
}

void mm_timespec_sub( struct timespec const * const lhs, struct timespec const * const rhs, struct timespec * const out ) {
	out->tv_sec = lhs->tv_sec - rhs->tv_sec;
	out->tv_nsec = lhs->tv_nsec - rhs->tv_nsec;

	mm_timespec_fix( out );
}

int mm_timespec_sign( struct timespec const * const t ) {
	if ( t->tv_sec < 0 ) {
		return -1;
	} else if ( t->tv_sec > 0 ) {
		return 1;
	} else {
		return 0;
	}
}

int mm_timespec_cmp( struct timespec const * const lhs, struct timespec const * const rhs ) {
	struct timespec diff;

	mm_timespec_sub( lhs, rhs, &diff );

	return mm_timespec_sign( &diff );
}
