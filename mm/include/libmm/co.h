#ifndef LIBMM_CO_H
#define LIBMM_CO_H

/*
	cooperitive multi-tasking via co routines
	longjmp seems like the obvious choice
	but for whatever reason, c allows switch
	cases to be capable of jumping inside of loops
	and other constructs.

	example usage

	struct state {
		struct co co;
		int i;
	};

	enum co_state generator( struct state *this ) {
		co_begin( &this->co );

		for ( this->i = 0; this->i < 10; ++this->i ) {
			co_yield( &this->co );
		}

		co_end( &this->co );
	}

	later on...
	struct state gen = {};

	while( co_resume( generator( &gen ) ) ) {
		printf( "%d\n", gen.i );
	}
*/

typedef enum co_state {
	CO_SUSPENDED, // yielded
	CO_WAITING, // waiting on another coroutine ( invoked another coroutine )
	CO_DONE
} co_state_t;

typedef struct co {
	unsigned short line;
} co_t;

#define co_line( co )\
	( co )->line

// for internal use
#define _co_jmp_\
	case __LINE__:

#define _co_ret_( co, status )\
	co_line( co ) = __LINE__;\
	return status\
	
#define co_init( co )\
	co_line( co ) = 0

#define co_begin( co )\
	switch( co_line( co ) ) {\
		case 0:;

#define co_done( co )\
	do {\
		_co_jmp_;\
		_co_ret_( co, CO_DONE );\
	} while( 0 )

#define co_end( co )\
	_co_jmp_;\
	}\
	_co_ret_( co, CO_DONE )

#define co_resume( call )\
	( ( call ) != CO_DONE )

#define co_yield( co )\
	do {\
		_co_ret_( co, CO_SUSPENDED );\
		_co_jmp_;\
	} while( 0 )

#define _co_ret_while_( co, cond, status )\
	do {\
		_co_jmp_;\
		\
		if ( cond ) {\
			_co_ret_( co, status );\
		}\
	} while( 0 )

#define co_yield_while( co, cond )\
	_co_ret_while( co, cond, CO_SUSPENDED )

#define co_yield_until( co, cond )\
	co_yield_while( co, !( cond ) )

#define co_wait( co, call )\
	_co_ret_while_( co, co_resume( call ), CO_WAITING )

#define co_init_wait( co, other_co, call )\
	do {\
		co_init( other_co );\
		co_wait( co, call );\
	} while( 0 )

#endif
