#ifndef MM_CO_H
#define MM_CO_H

/*
	cooperitive multi-tasking via co routines
	longjmp seems like the obvious choice
	but for whatever reason, c allows switch
	cases to be capable of jumping inside of loops
	and other constructs.

	example usage

	struct state {
		struct mm_co co;
		int i;
	};

	MM_COROUTINE( generator, struct state *this ) {
		MM_CO_BEGIN( &this->co );

		for ( this->i = 0; this->i < 10; ++this->i ) {
			MM_CO_YIELD( &this->co );
		}

		MM_CO_END( &this->co );
	}

	later on...
	struct state gen = { 0 };

	while( MM_CO_RESUME( generator( &gen ) ) ) {
		printf( "%d\n", gen.i );
	}
*/

typedef enum mm_co_state {
	MM_CO_SUSPENDED, // yielded
	MM_CO_WAITING, // waiting on another coroutine ( invoked another coroutine )
	MM_CO_DONE
} mm_co_state_t;

typedef struct mm_co {
	unsigned short line;
} mm_co_t;

#define MM_COROUTINE( name, ... )\
	enum mm_co_state name( __VA_ARGS__ )

#define MM_CO_LINE( co )\
	( co )->line

// for internal use
#define _MM_CO_JMP_\
	case __LINE__:

#define _MM_CO_RET_( co, status )\
	MM_CO_LINE( co ) = __LINE__;\
	return status\
	
#define MM_CO_INIT( co )\
	MM_CO_LINE( co ) = 0

#define MM_CO_BEGIN( co )\
	switch( MM_CO_LINE( co ) ) {\
		case 0:;

#define MM_CO_DONE( co )\
	do {\
		_MM_CO_JMP_;\
		_MM_CO_RET_( co, MM_CO_DONE );\
	} while( 0 )

#define MM_CO_END( co )\
	_MM_CO_JMP_;\
	}\
	_MM_CO_RET_( co, MM_CO_DONE )

#define MM_CO_RESUME( call )\
	( ( call ) != MM_CO_DONE )

#define MM_CO_YIELD( co )\
	do {\
		_MM_CO_RET_( co, MM_CO_SUSPENDED );\
		_MM_CO_JMP_;\
	} while( 0 )

#define _MM_CO_RET_WHILE_( co, cond, status )\
	do {\
		_MM_CO_JMP_;\
		\
		if ( cond ) {\
			_MM_CO_RET_( co, status );\
		}\
	} while( 0 )

#define MM_CO_YIELD_WHILE( co, cond )\
	_MM_CO_RET_WHILE( co, cond, MM_CO_SUSPENDED )

#define MM_CO_YIELD_UNTIL( co, cond )\
	MM_CO_YIELD_WHILE( co, !( cond ) )

#define MM_CO_WAIT( co, call )\
	_MM_CO_RET_WHILE_( co, MM_CO_RESUME( call ), MM_CO_WAITING )

#define MM_CO_INIT_WAIT( co, other_co, call )\
	do {\
		MM_CO_INIT( other_co );\
		MM_CO_WAIT( co, call );\
	} while( 0 )

#endif
