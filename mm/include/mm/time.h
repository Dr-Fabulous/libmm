#ifndef MM_TIME_H
#define MM_TIME_H

#if MM_USING_POSIX
#define _POSIX_C_SOURCE 199309L
#endif

#include <time.h>

#if MM_USING_POSIX
#undef _POSIX_C_SOURCE
#endif

#include "mm/common.h"

#if !MM_USING_POSIX && __STDC_VERSION__ < 201109L
struct timespec {
	time_t tv_sec;
	long tv_nsec;
};
#endif

#define MM_TIMESPEC_NSEC_MAX 999999999

typedef struct timespec timespec_t;
typedef struct timespec mm_timespec_t;

MM_API bool mm_timespec_get( struct timespec * out );
MM_API void mm_timespec_add( const struct timespec * lhs, const struct timespec * rhs, struct timespec * out );
MM_API void mm_timespec_sub( const struct timespec * lhs, const struct timespec * rhs, struct timespec * out );

#endif
