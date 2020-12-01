#ifndef MM_MACRO_H
#define MM_MACRO_H

/*! \file */

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

#endif
