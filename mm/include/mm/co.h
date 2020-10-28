#ifndef MM_CO_H
#define MM_CO_H

/*! \file */

/*
	cooperitive multi-tasking via co routines
	longjmp seems like the obvious choice
	but for whatever reason, c allows switch
	cases to be capable of jumping inside of loops
	and other constructs.

	example usage
*/

/*!
	\brief Constants returned by coroutines indicating their current state
*/
typedef enum mm_co_state {
	MM_CO_SUSPENDED, //!< coroutine has yielded, but can be resumed
	MM_CO_WAITING, //!< waiting on another coroutine ( invoked another coroutine )
	MM_CO_DONE //!< coroutine has finished.
} mm_co_state_t;

/*!
	\brief Used to hold what part of a coroutine's impl we have made it to.

	this struct should be embedded inside another one to provide state to a coroutine.

	For example.
	\code{.c}
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
	
	static void run_coroutine( void ) {
		struct state gen = { 0 };
	
		while( MM_CO_RESUME( generator( &gen ) ) ) {
			MM_PRINTF( "%d\n", gen.i );
		}
	}
	\endcode
*/
typedef struct mm_co {
	unsigned short line; //!< holds state nessecary to resume a given coroutine.
} mm_co_t;

/*!
	\brief Create new body for a coroutine. Any implementation needs to accept a pointer
	       to something that contains a mm_co instance.
*/
#define MM_COROUTINE( name, ... )\
	enum mm_co_state name( __VA_ARGS__ )

#define MM_CO_LINE( co )\
	( co )->line

#define MM_CO_JMP\
	case __LINE__:

#define MM_CO_RET( co, status )\
	MM_CO_LINE( co ) = __LINE__;\
	return status\

/*!
	\brief Initialize mm_co instance to start at the beginning of a coroutine.
*/
#define MM_CO_INIT( co )\
	MM_CO_LINE( co ) = 0

/*!
	\brief Define the start of a coroutine body. This must be called
	       before a coroutine can be yielded.
*/
#define MM_CO_BEGIN( co )\
	switch( MM_CO_LINE( co ) ) {\
		case 0:;

/*!
	\brief Exit the current coroutine and mark it as complete.
*/
#define MM_CO_DONE( co )\
	do {\
		MM_CO_JMP;\
		MM_CO_RET( co, MM_CO_DONE );\
	} while( 0 )

/*!
	\brief Define the end of a routine body. This must be called at
	       the end of a function block.
*/
#define MM_CO_END( co )\
	MM_CO_JMP;\
	}\
	MM_CO_RET( co, MM_CO_DONE )

/*!
	\breif Use this macro to wrap a function call to a coroutine.

	E.g. MM_CO_RESUME( my_coroutine( &co ) ). The macro
	will return true when the coroutine has finished.
*/
#define MM_CO_RESUME( call )\
	( ( call ) != MM_CO_DONE )

/*!
	\brief Return from the current coroutine and resume at the same position.
*/
#define MM_CO_YIELD( co )\
	do {\
		MM_CO_RET( co, MM_CO_SUSPENDED );\
		MM_CO_JMP;\
	} while( 0 )

#define MM_CO_DO_RET_WHILE( co, cond, status )\
	do {\
		MM_CO_JMP;\
		\
		if ( cond ) {\
			MM_CO_RET( co, status );\
		}\
	} while( 0 )

/*!
	\breif Yield the current coroutine while the given condition is true.
*/
#define MM_CO_YIELD_WHILE( co, cond )\
	MM_CO_DO_RET_WHILE( co, cond, MM_CO_SUSPENDED )

/*!
	\brief Yield the current coroutine until the given condition is true.
*/
#define MM_CO_YIELD_UNTIL( co, cond )\
	MM_CO_YIELD_WHILE( co, !( cond ) )

/*!
	\brief Yield the current coroutine until the passed courtine call
	       has finished.

	The value passed to 'call' is the same as MM_CO_RESUME().
*/
#define MM_CO_WAIT( co, call )\
	MM_CO_DO_RET_WHILE( co, !MM_CO_RESUME( call ), MM_CO_WAITING )

/*!
	\brief Initialize a another coroutine and yield until it has finished.
*/
#define MM_CO_INIT_WAIT( co, other_co, call )\
	do {\
		MM_CO_INIT( other_co );\
		MM_CO_WAIT( co, call );\
	} while( 0 )

#endif
