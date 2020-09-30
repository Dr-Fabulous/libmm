#ifndef MM_VECTOR_H
#define MM_VECTOR_H
#include "mm/common.h"

typedef struct mm_vector {
	size_t type_size;
	unsigned char *begin;
	unsigned char *end;
	unsigned char *capacity;
} mm_vector_t;

MM_FUNCTION bool mm_vector_construct( struct mm_vector *this, size_t type_size, size_t capacity );
MM_FUNCTION bool mm_vector_copy( struct mm_vector *this, struct mm_vector *other );
MM_FUNCTION void mm_vector_move( struct mm_vector *this, struct mm_vector *other );
MM_FUNCTION void mm_vector_destroy( struct mm_vector *this );

static inline bool mm_vector_null( struct mm_vector *this ) {
	return this->begin == this->capacity;
}

static inline bool mm_vector_empty( struct mm_vector *this ) {
	return this->begin == this->end;
}

static inline size_t mm_vector_bcapacity( struct mm_vector *this ) {
	return this->capacity - this->begin;
}

static inline size_t mm_vector_capacity( struct mm_vector *this) {
	return mm_vector_bcapacity( this ) / this->type_size;
}

static inline size_t mm_vector_bsize( struct mm_vector *this ) {
	return this->end - this->begin;
}

static inline size_t mm_vector_size( struct mm_vector *this ) {
	return mm_vector_bsize( this ) / this->type_size;
}

MM_FUNCTION bool mm_vector_set_capacity( struct mm_vector *this, size_t new_capacity );
MM_FUNCTION bool mm_vector_resize( struct mm_vector *this, size_t new_size );

static inline bool mm_vector_reserve( struct mm_vector *this, size_t delta ) {
	return mm_vector_set_capacity( this, mm_vector_capacity( this ) + delta );
}

static inline void mm_vector_shrink( struct mm_vector *this ) {
	mm_vector_set_capacity( this, mm_vector_size( this ) );
}

static inline void mm_vector_clear( struct mm_vector *this ) {
	this->end = this->begin;
}

static inline void* mm_vector_at( struct mm_vector *this, size_t idx ) {
	return this->begin + this->type_size * idx;
}

static inline void* mm_vector_begin( struct mm_vector *this ) {
	return this->begin;
}

static inline void* mm_vector_end( struct mm_vector *this ) {
	return this->end;
}

static inline void* mm_vector_back( struct mm_vector *this ) {
	if ( mm_vector_null( this ) ) {
		return this->end - this->type_size;
	} else {
		return NULL;
	}
}

MM_FUNCTION void* mm_vector_find( struct mm_vector *this, void *buf, int ( *cmp )( const void*, const void* ) );

static inline void* mm_vector_search( struct mm_vector *this, void *buf, int ( *cmp )( const void*, const void* ) ) {
	return bsearch(
		buf,
		mm_vector_begin( this ),
		mm_vector_bsize( this ),
		this->type_size,
		cmp
	);
}

static inline void mm_vector_sort( struct mm_vector *this, int ( *cmp )( const void*, const void* ) ) {
	qsort(
		mm_vector_begin( this ),
		mm_vector_bsize( this ),
		this->type_size,
		cmp
	);
}

MM_FUNCTION bool mm_vector_insert( struct mm_vector *this, void *pos, void *buf );

static inline bool mm_vector_push( struct mm_vector *this, void *buf ) {
	return mm_vector_insert( this, this->end, buf );
}

MM_FUNCTION void mm_vector_erase( struct mm_vector *this, void *pos, void *buf );

static inline void mm_vector_pop( struct mm_vector *this, void *buf ) {
	mm_vector_erase( this, this->end - this->type_size, buf );
}

#define MM_VECTOR_INIT( type )\
	{ .type_size = sizeof( type ) }

#define MM_VECTOR_DECLARE( name, type )\
	struct mm_vector name = MM_VECTOR_INIT( type )

#define MM_VECTOR_NEXT( vec, pos )\
	( ( void* ) ( ( unsigned char* ) ( pos ) + ( vec )->type_size ) )

#define MM_VECTOR_PREV( vec, pos )\
	( ( void* ) ( ( unsigned char* ) ( pos ) - ( vec )->type_size ) )

#define MM_VECTOR_FOR_EACH( vec, pos )\
	for( ( pos ) = mm_vector_begin( vec );\
	     ( pos ) < mm_vector_end( vec );\
	     ( pos ) = MM_VECTOR_NEXT( vec, pos ) )

#define MM_VECTOR_FOR_EACH_REVERSE( vec, pos )\
	for( ( pos ) < mm_vector_end( vec );\
	     ( pos ) = mm_vector_begin( vec );\
	     ( pos ) = MM_VECTOR_PREV( vec, pos ) )
#endif
