#ifndef MM_VECTOR_H
#define MM_VECTOR_H
#include <stdlib.h>
#include "mm/common.h"

typedef struct mm_vector {
	size_t type_size;
	int ( *type_cmp )( const void*, const void* );
	unsigned char *begin;
	unsigned char *end;
	unsigned char *capacity;
} mm_vector_t;

MM_API bool mm_vector_construct( struct mm_vector *this, size_t type_size, size_t capacity );
MM_API bool mm_vector_copy( struct mm_vector *this, struct mm_vector *other );
MM_API void mm_vector_move( struct mm_vector *this, struct mm_vector *other );
MM_API void mm_vector_destroy( struct mm_vector *this );

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

MM_API bool mm_vector_set_capacity( struct mm_vector *this, size_t new_capacity );
MM_API bool mm_vector_resize( struct mm_vector *this, size_t new_size );

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

MM_API void* mm_vector_find( struct mm_vector *this, void *buf );

static inline void* mm_vector_search( struct mm_vector *this, void *buf ) {
	return bsearch(
		buf,
		mm_vector_begin( this ),
		mm_vector_bsize( this ),
		this->type_size,
		this->type_cmp
	);
}

static inline void mm_vector_sort( struct mm_vector *this ) {
	qsort(
		mm_vector_begin( this ),
		mm_vector_bsize( this ),
		this->type_size,
		this->type_cmp
	);
}

MM_API void* mm_vector_emplace( struct mm_vector *this, void *pos );
MM_API bool mm_vector_insert( struct mm_vector *this, void *pos, void *buf );

static inline void* mm_vector_emplace_front( struct mm_vector *this ) {
	return mm_vector_emplace( this, this->begin );
}

static inline void* mm_vector_emplace_back( struct mm_vector *this ) {
	return mm_vector_emplace( this, this->end );
}

static inline bool mm_vector_push_front( struct mm_vector *this, void *buf ) {
	return mm_vector_insert( this, this->begin, buf );
}

static inline bool mm_vector_push_back( struct mm_vector *this, void *buf ) {
	return mm_vector_insert( this, this->end, buf );
}

MM_API void mm_vector_erase( struct mm_vector *this, void *pos, void *buf );

static inline void mm_vector_pop_front( struct mm_vector *this, void *buf ) {
	mm_vector_erase( this, this->end - this->type_size, buf );
}

static inline void mm_vector_pop_back( struct mm_vector *this, void *buf ) {
	mm_vector_erase( this, this->end - this->type_size, buf );
}

#define MM_VECTOR_INIT( type, cmp )\
	{ .type_size = sizeof( type ), .type_cmp = cmp }

#define MM_VECTOR_DECLARE( name, type, cmp )\
	struct mm_vector name = MM_VECTOR_INIT( type, cmp )

#define MM_VECTOR_NEXT( vec, pos )\
	( ( void* ) ( ( unsigned char* ) ( pos ) + ( vec )->type_size ) )

#define MM_VECTOR_PREV( vec, pos )\
	( ( void* ) ( ( unsigned char* ) ( pos ) - ( vec )->type_size ) )

#define MM_VECTOR_AS( vec, pos, type )\
	( ( type* ) mm_vector_at( vec, pos ) )

#define MM_VECTOR_FOR_EACH( vec, pos )\
	for( ( pos ) = mm_vector_begin( vec );\
	     ( pos ) < mm_vector_end( vec );\
	     ( pos ) = MM_VECTOR_NEXT( vec, pos ) )

#define MM_VECTOR_FOR_EACH_REVERSE( vec, pos )\
	for( ( pos ) < mm_vector_end( vec );\
	     ( pos ) >= mm_vector_begin( vec );\
	     ( pos ) = MM_VECTOR_PREV( vec, pos ) )
#endif
