#include <stdio.h>
#include "mm/log.h"

void mm_openlog( const char *name, enum mm_log_facility facility ) {
#ifdef MM_USING_SYSLOG
	if ( !facility ) {
		facility = MM_USER;
	}

	openlog( name, 0, facility );
#endif
}

void mm_closelog( void ) {
#ifdef MM_USING_SYSLOG
	closelog();
#endif
}

static inline void vlog( FILE *f, enum mm_log_level level, const char *prefix, const char *format, va_list args ) {
#ifdef MM_USING_SYSLOG
	va_list tmp;
	va_copy( tmp, args );
	vsyslog( level, format, tmp );
	va_end( tmp );
#endif
	MM_FPRINTF( f, "%s", prefix );
	MM_VFPRINTF( f, format, args );
	MM_FPRINTF( f, "\n" );
	MM_FFLUSH( f );
}

void mm_vlog( enum mm_log_level level, const char *format, va_list args ) {
	switch( level ) {
		case MM_INFO: vlog( MM_STDOUT, MM_INFO, "", format, args ); break;
		case MM_WARN: vlog( MM_STDOUT, MM_WARN, "\033[34mWARNING\033[0m: ", format, args ); break;
		case MM_ERR: vlog( MM_STDOUT, MM_ERR, "\033[31mERROR\033[0m: ", format, args ); break;
		case MM_CRIT: vlog( MM_STDOUT, MM_ERR, "\033[31mCRITICAL033[0m: ", format, args ); break;
	}
}

void mm_log( enum mm_log_level level, const char *format, ... ) {
	va_list args;
	va_start( args, format );	
	mm_vlog( level, format, args );
	va_end( args );
}
