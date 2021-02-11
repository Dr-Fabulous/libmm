#ifndef MM_SLEEP_H
#define MM_SLEEP_H
#include "mm/common.h"

#if __STDC_VERSION__ >= 201112L && !defined( __STDC_NO_THREADS__ )
#include <threads.h>
#define mm_sleep thrd_sleep
#elif MM_USING_POSIX && _POSIX_C_SOURCE >= 199309L
#include <time.h>
#define mm_sleep nanosleep
#elif MM_USING_WINDOWS
#include <syncapi.h>
#include "mm/time.h"

static inline int mm_sleep( struct timespec const * const dur, struct timespec * const rem ) {
	Sleep( ( DWORD ) ( dur->tv_sec * 1000 + ( dur->tv_nsec / 1000000 ) ) );

	if ( rem ) {
		rem->tv_sec = ( time_t ) 0;
		rem->tv_nsec = 0L;
	}

	return 0;
}

#else
#error "Sleep unsupported on current platform"
#endif

static inline void mm_full_sleep( struct timespec const * const duration ) {
	struct timespec dur = *duration;
	struct timespec rem;

	while( mm_sleep( &dur, &rem ) ) {
		dur = rem;
	}
}

#endif
