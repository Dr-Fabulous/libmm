#ifndef MM_SLEEP_H
#define MM_SLEEP_H
#include "mm/common.h"

#if __STDC_VERSION__ >= 201112L && !defined( __STDC_NO_THREADS__ )
#include <thread.h>
#define mm_sleep thrd_sleep
#elif MM_USING_POSIX
#define _POSIX_C_SOURCE 199309L
#include <time.h>
#define mm_sleep nanosleep
#undef _POSIX_C_SOURCE
#elif MM_USING_WINDOWS
#include <syncapi.h>
#include "mm/time.h"

static inline int mm_sleep( struct timespec *dur, struct timespec *rem ) {
	Sleep( ( DWORD ) ( dur->tv_sec * 1000 + ( dur->tv_nsec / 1000000 ) ) );

	if ( rem ) {
		rem->tv_sec = 0;
		rem->tv_nsec = 0;
	}

	return 0;
}

#else
#error "Unsupported platform"
#endif
