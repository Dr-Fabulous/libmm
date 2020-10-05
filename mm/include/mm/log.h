#ifndef MM_LOG_H
#define MM_LOG_H
#include <stdarg.h>
#include "mm/common.h"

#if MM_HAS_INCLUDE( <syslog.h> )
#define _BSD_SOURCE
#include <syslog.h>
#define MM_USING_SYSLOG
#endif

#ifdef MM_USING_SYSLOG
typedef enum mm_log_facility {
	MM_DAEMON = LOG_DAEMON,
	MM_USER = LOG_USER
} mm_log_facility_t;

typedef enum mm_log_level {
	MM_INFO = LOG_INFO,
	MM_WARN = LOG_WARNING,
	MM_ERR = LOG_ERR,
	MM_CRIT = LOG_CRIT
} mm_log_level_t;
#else
typedef enum mm_log_facility {
	MM_DAEMON = 1,
	MM_USER
} mm_log_facility_t;

typedef enum mm_log_level {
	MM_INFO = 1,
	MM_WARN,
	MM_ERR,
	MM_CRIT
} mm_log_level_t;
#endif

MM_API void mm_openlog( const char *name, enum mm_log_facility facility );
MM_API void mm_closelog( void );
MM_API void mm_vlog( enum mm_log_level level, const char *format, va_list args );
MM_API void mm_log( enum mm_log_level level, const char *format, ... );

#ifdef MM_DEBUG
#define MM_DEBUG_VLOG( level, format, args) mm_vlog( level, format, args )
#define MM_DEBUG_LOG( level, ... ) mm_log( level, __VA_ARGS__ )
#else
#define MM_DEBUG_VLOG( level, format, args )
#define MM_DEBUG_LOG( level, ... )
#endif

#endif
