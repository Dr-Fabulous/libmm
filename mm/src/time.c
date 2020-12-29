#include "mm/time.h"

MM_API bool mm_timespec_get( struct timespec *out ) {
#if MM_USING_POSIX
	return !clock_gettime( out, CLOCK_MONOTONIC );
#elif __STDC_VERSION__ >= 201109L
	return timespec_get( out, TIME_UTC ) == TIME_UTC;
#else
	clock_t time = clock();

	out->tv_sec = ( time_t ) ( time / CLOCKS_PER_SEC );
	out->time_nsec = ( long ) ( ( time % CLOCKS_PER_SEC ) / ( CLOCKS_PER_SEC / 1000 ) );

	return time != ( clock_t ) -1;
#endif
}

MM_API void mm_timespec_add( const struct timespec *lhs, const struct timespec *rhs, struct timespec *out ) {
	out->tv_sec = lhs->tv_sec + rhs->tv_sec;
	out->tv_nsec = lhs->tv_nsec + rhs->tv_nsec;

	if ( out->tv_nsec > MM_TIMESPEC_NSEC_MAX ) {
		++out->tv_sec;
		out->tv_nsec -= MM_TIMESPEC_NSEC_MAX;
	}
}

MM_API void mm_timespec_sub( const struct timespec *lhs, const struct timespec *rhs, struct timespec *out ) {
	out->tv_sec = lhs->tv_sec - rhs->tv_sec;
	out->tv_nsec = lhs->tv_nsec - rhs->tv_nsec;

	if ( out->tv_nsec < 0 ) {
		--out->tv_sec;
		out->tv_nsec += MM_TIMESPEC_NSEC_MAX;
	}
}
