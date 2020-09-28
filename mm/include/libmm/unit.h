#ifndef LIBMM_UNIT_H
#define LIBMM_UNIT_H
#include <stdio.h>
#include <stdlib.h>
#include "libmm/common.h"

typedef struct unit_err {
	const char *err;
	const char *file;
	int line;
} unit_err_t;

static const struct unit_err UNIT_DONE = { 0 };

typedef struct unit {
	const char *name;
	bool ( *setup )( void );
	void ( *teardown )( void );
	struct unit_err ( *run )( void );
} unit_t;

#define UNIT_INFO( ... )\
	printf( __VA_ARGS__ )

#define UNIT_WARN( ... )\
	do {\
		fprintf( stderr, "\033[33mWARNING\033[0m: " );\
		fprintf( stderr, __VA_ARGS__ );\
	} while( 0 )

#define UNIT_ERR( ... )\
	do {\
		fprintf( stderr, "\033[31mERROR\033[0m: " );\
		fprintf( stderr, __VA_ARGS__ );\
	} while( 0 )

#define UNIT_ASSERT( cond, _err )\
	do {\
		if ( !( cond ) ) {\
			struct unit_err err;\
			\
			err.err = _err;\
			err.file = __FILE__;\
			err.line = __LINE__;\
			\
			return err;\
		}\
	} while( 0 )

#define UNIT_RUN_NAME( name )\
	name##_run

#define UNIT_CASE( cname, csetup, cteardown )\
	static struct unit_err UNIT_RUN_NAME( cname )( void );\
	\
	struct unit cname = {\
		.name = #cname,\
		.setup = csetup,\
		.teardown = cteardown,\
		.run = UNIT_RUN_NAME( cname )\
	};\
	\
	static struct unit_err UNIT_RUN_NAME( cname )( void )

#define UNIT_IMPORT( name )\
	extern struct unit name

#define UNIT_SUITE( sname )\
	struct unit_err UNIT_RUN_NAME( sname )( void );\
	\
	struct unit sname = {\
		.name = #sname,\
		.setup = NULL,\
		.teardown = NULL,\
		.run = UNIT_RUN_NAME( sname )\
	};\
	\
	struct unit_err UNIT_RUN_NAME( sname )( void )

#define UNIT_RUN( unit )\
	do {\
		UNIT_INFO( "running case '%s'\n", unit.name );\
		\
		if ( unit.setup && !unit.setup() ) {\
			struct unit_err err;\
			\
			err.err = "setup failed";\
			err.file = __FILE__;\
			err.line = __LINE__;\
			\
			return err;\
		}\
		\
		struct unit_err err = unit.run();\
		\
		if ( unit.teardown ) {\
			unit.teardown();\
		}\
		\
		if ( err.err ) {\
			return err;\
		}\
	} while( 0 )

#define UNIT_RUN_SUITE( suite )\
	do {\
		UNIT_INFO( "running suite '%s'\n", suite.name );\
		\
		struct unit_err err = suite.run();\
		\
		if ( err.err ) {\
			UNIT_ERR( "%s:%d: %s'\n", err.file, err.line, err.err );\
			exit( EXIT_FAILURE );\
		}\
	} while( 0 )

#endif
