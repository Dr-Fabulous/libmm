#ifndef MM_UNIT_H
#define MM_UNIT_H
#include <stdio.h>
#include <stdlib.h>
#include "mm/common.h"
#include "mm/log.h"

typedef struct mm_unit_err {
	const char *err;
	const char *file;
	unsigned int line;
} mm_unit_err_t;

static const struct mm_unit_err MM_UNIT_DONE = { 0 };

typedef struct mm_unit {
	const char *name;
	bool ( *setup )( void );
	void ( *teardown )( void );
	struct mm_unit_err ( *run )( void );
} mm_unit_t;

#define MM_UNIT_ASSERT( cond, _err )\
	do {\
		if ( !( cond ) ) {\
			struct mm_unit_err err;\
			\
			err.err = _err;\
			err.file = __FILE__;\
			err.line = __LINE__;\
			\
			return err;\
		}\
	} while( 0 )

#define MM_UNIT_ASSERT_COND( cond )\
	MM_UNIT_ASSERT( cond, #cond )

#define MM_UNIT_ASSERT_SUM( lhs, op, rhs )\
	MM_UNIT_ASSERT_COND( ( lhs ) op ( rhs ) )

#define MM_UNIT_ASSERT_EQ( lhs, rhs )\
	MM_UNIT_ASSERT_SUM( lhs, ==, rhs )

#define MM_UNIT_ASSERT_LESS( lhs, rhs )\
	MM_UNIT_ASSERT_SUM( lhs, <, rhs )

#define MM_UNIT_ASSERT_GREATER( lhs, rhs )\
	MM_UNIT_ASSERT_SUM( lhs, >, rhs )

#define MM_UNIT_ASSERT_LESS_EQ( lhs, rhs )\
	MM_UNIT_ASSERT_SUM( lhs, <=, rhs )

#define MM_UNIT_ASSERT_GREATER_EQ( lhs, rhs )\
	MM_UNIT_ASSERT_SUM( lhs, >=, rhs )

#define MM_UNIT_ASSERT_RANGE( min, max, v )\
	MM_UNIT_ASSERT_COND( ( min ) <= ( v ) && ( v ) <= ( max ) )

#define MM_UNIT_RUN_NAME( name )\
	name##_run

#define MM_UNIT_CASE( cname, csetup, cteardown )\
	static struct mm_unit_err MM_UNIT_RUN_NAME( cname )( void );\
	\
	struct mm_unit cname = {\
		.name = #cname,\
		.setup = csetup,\
		.teardown = cteardown,\
		.run = MM_UNIT_RUN_NAME( cname )\
	};\
	\
	static struct mm_unit_err MM_UNIT_RUN_NAME( cname )( void )

#define MM_UNIT_IMPORT( name )\
	extern struct mm_unit name

#define MM_UNIT_SUITE( sname )\
	struct mm_unit_err MM_UNIT_RUN_NAME( sname )( void );\
	\
	struct mm_unit sname = {\
		.name = #sname,\
		.setup = NULL,\
		.teardown = NULL,\
		.run = MM_UNIT_RUN_NAME( sname )\
	};\
	\
	struct mm_unit_err MM_UNIT_RUN_NAME( sname )( void )

#define MM_UNIT_RUN( unit )\
	do {\
		mm_log( MM_INFO, "running case: '%s'", unit.name );\
		\
		if ( unit.setup && !unit.setup() ) {\
			struct mm_unit_err err;\
			\
			err.err = "setup failed";\
			err.file = __FILE__;\
			err.line = __LINE__;\
			\
			return err;\
		}\
		\
		struct mm_unit_err err = unit.run();\
		\
		if ( unit.teardown ) {\
			unit.teardown();\
		}\
		\
		if ( err.err ) {\
			return err;\
		}\
	} while( 0 )

#define MM_UNIT_RUN_SUITE( suite )\
	do {\
		mm_log( MM_INFO, "running suite: '%s'", suite.name );\
		\
		struct mm_unit_err err = suite.run();\
		\
		if ( err.err ) {\
			mm_log( MM_ERR, "%s:%d: '%s'", err.file, err.line, err.err );\
			exit( EXIT_FAILURE );\
		}\
	} while( 0 )

#endif
