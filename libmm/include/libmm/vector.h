#ifndef LIBMM_VECTOR_H
#define LIBMM_VECTOR_H
#include "libmm/common.h"

typedef struct vector {
	size_t type_size;
	unsigned char *begin;
	unsigned char *end;
	unsigned char *capacity;
} vector_t;

LIBMM_EXPORT bool vector_construct( struct vector *this, size_t type_size, size_t capacity );
LIBMM_EXPORT bool vector_copy( struct vector *this, struct vector *other );
LIBMM_EXPORT void vector_move( struct vector *this, struct vector *other );
LIBMM_EXPORT void vector_destroy( struct vector *this );
LIBMM_EXPORT bool vector_null( struct vector *this );
LIBMM_EXPORT bool vector_empty( struct vector *this );

LIBMM_EXPORT size_t vector_bcapacity( struct vector *this );
LIBMM_EXPORT size_t vector_capacity( struct vector *this );
LIBMM_EXPORT size_t vector_bsize( struct vector *this );
LIBMM_EXPORT size_t vector_size( struct vector *this );
LIBMM_EXPORT bool vector_set_capacity( struct vector *this, size_t new_capacity );
LIBMM_EXPORT bool vector_resize( struct vector *this, size_t new_size );
LIBMM_EXPORT bool vector_reserve( struct vector *this, size_t delta );
LIBMM_EXPORT void vector_shrink( struct vector *this );
LIBMM_EXPORT void vector_clear( struct vector *this );

LIBMM_EXPORT void* vector_at( struct vector *this, size_t idx );
LIBMM_EXPORT void* vector_begin( struct vector *this );
LIBMM_EXPORT void* vector_end( struct vector *this );
LIBMM_EXPORT void* vector_back( struct vector *this );
LIBMM_EXPORT void* vector_find( struct vector *this, void *buf, int ( *cmp )( const void*, const void* ) );
LIBMM_EXPORT void* vector_search( struct vector *this, void *buf, int ( *cmp )( const void*, const void* ) );
LIBMM_EXPORT void vector_sort( struct vector *this, int ( *cmp )( const void*, const void* ) );

LIBMM_EXPORT bool vector_insert( struct vector *this, void *pos, void *buf );
LIBMM_EXPORT bool vector_push( struct vector *this, void *buf );

LIBMM_EXPORT void vector_erase( struct vector *this, void *pos, void *buf );
LIBMM_EXPORT void vector_pop( struct vector *this, void *buf );

#define LIBMM_VECTOR_INIT( type )\
	{ .type_size = sizeof( type ) }

#define LIBMM_VECTOR_DECLARE( name, type )\
	struct vector name = LIBMM_VECTOR_INIT( type )

#define LIBMM_VECTOR_NEXT( vec, pos )\
	( ( void* ) ( ( unsigned char* ) ( pos ) + ( vec )->type_size ) )

#define LIBMM_VECTOR_PREV( vec, pos )\
	( ( void* ) ( ( unsigned char* ) ( pos ) - ( vec )->type_size ) )

#define LIBMM_VECTOR_FOR_EACH( vec, pos )\
	for( ( pos ) = vector_begin( vec );\
	     ( pos ) < vector_end( vec );\
	     ( pos ) = LIBMM_VECTOR_NEXT( vec, pos ) )

#define LIBMM_VECTOR_FOR_EACH_REVERSE( vec, pos )\
	for( ( pos ) < vector_end( vec );\
	     ( pos ) = vector_begin( vec );\
	     ( pos ) = LIBMM_VECTOR_PREV( vec, pos ) )
#endif
