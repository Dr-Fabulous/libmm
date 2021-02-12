#ifndef MM_MACRO_H
#define MM_MACRO_H

/*! \file */

/*!
	\param macro cpp macro to force expansion of
*/
#define MM_EXPAND( macro )\
	macro

/*!
	\param type type to get bitsize of
	\return total number of bits in given type
*/
#define MM_BITSOF( type )\
	( sizeof( type ) * CHAR_BIT )

/*!
	\param arr C array.
	\return total size of array in elements.
*/
#define MM_ARR_SIZE( arr )\
	( sizeof( arr ) / sizeof( ( arr )[ 0 ] ) )

/*!
	\param arr C array.
	\param pos pointer to position in the array.
	\return pointer to the next element in the array.
*/
#define MM_ARR_NEXT( arr, pos )\
	( ( void* ) ( ( unsigned char* ) ( pos ) + sizeof( ( arr )[ 0 ] ) ) )

/*!
	\brief Iterate across every item in a C array.
	\param arr C array.
	\param pos pointer to hold a position.
*/
#define MM_ARR_FOREACH( arr, pos )\
	for( ( pos ) = ( arr );\
	     ( pos ) != ( arr ) + MM_ARR_SIZE( arr );\
	     ( pos ) = MM_ARR_NEXT( arr, pos ) )

#define MM_INTERNAL_CAT( lhs, rhs )\
	lhs##rhs

/*!
	\brief Concat two tokens together, expanding any macros that are passed
	       prior to being concatted.
*/
#define MM_CAT( lhs, rhs )\
	MM_INTERNAL_CAT( lhs, rhs )

/*!
	\param type type to calculate offset in.
	\param member member to get offset of.
	\return distance from the start of a given type to it's member.
*/
#ifdef offsetof
#define MM_OFFSET_OF offsetof
#else
#define MM_OFFSET_OF( type, name )\
	( ( size_t ) &( ( type* ) NULL )->name )
#endif

/*!
	\param ptr pointer to member.
	\param type type to cast to.
	\param member member that's currently being pointing at.
	\return pointer to type that owns the member pointed to in pos
*/
#define MM_CONTAINER_OF( ptr, type, member )\
	( ( type* ) ( ( unsigned char* ) ( ptr ) - MM_OFFSET_OF( type, member ) ) )

/*!
	\param ptr pointer to member.
	\param type to cast to.
	\param member member that's being pointed at.
	\return pointer to type that owns the member pointed to in pos, or null if ptr is null.
*/
#define MM_COND_CONTAINER_OF( ptr, type, member )\
	( ( type* ) ( ( unsigned char* ) ( ptr ) ?: MM_OFFSET_OF( type, member ) - MM_OFFSET_OF( type, member ) ) )

#if __STDC_VERSION__ >= 201112L
#define MM_STATIC_ASSERT _Static_assert
#else
/*!
	\param cond compile time expression to be evaluated.
	\param msg message to present upon error.
*/
#define MM_STATIC_ASSERT( cond, msg )\
	typedef char mm_static_assert_ ## msg[ ( cond ) ? 1 : -1 ]
#endif

#define MM_GET_0( _0, ... ) _0
#define MM_GET_1( _0, _1, ... ) _1
#define MM_GET_2( _0, _1, _2, ... ) _2
#define MM_GET_3( _0, _1, _2, _3, ... ) _3
#define MM_GET_4( _0, _1, _2, _3, _4, ... ) _4
#define MM_GET_5( _0, _1, _2, _3, _4, _5, ... ) _5
#define MM_GET_6( _0, _1, _2, _3, _4, _5, _6, ... ) _6
#define MM_GET_7( _0, _1, _2, _3, _4, _5, _6, _7, ... ) _7
#define MM_GET_8( _0, _1, _2, _3, _4, _5, _6, _7, _8, ... ) _8
#define MM_GET_9( _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ... ) _9

/*!
	\param tuple of properties in the format ( 1, 2, 3 )
	\param idx property to get
*/
#define MM_GET( tuple, idx )\
	MM_CAT( MM_GET_, idx ) tuple

#endif
