#ifndef MM_VEC_H
#define MM_VEC_H
#include "mm/common.h"
#include "mm/bit.h"

#ifndef MM_VEC_BUF_SIZE
#define MM_VEC_BUF_SIZE ( sizeof( struct mm_vec_ext ) )
#endif

typedef struct mm_vec_ext {
	unsigned char *begin;
	unsigned char *capacity;
} mm_vec_ext_t;

typedef union mm_vec_mem {
	struct mm_vec_ext ext;
	unsigned char buf[ MM_VEC_BUF_SIZE ];
} mm_vec_mem_t;

typedef struct mm_vec {
	unsigned char is_static : 1;
	unsigned char ext_buf : 1;
	unsigned char dyn_buf : 1;
	union mm_vec_mem mem;
	unsigned char *end;
} mm_vec_t;

typedef int ( *mm_vec_cmp_t )( void const*, void const* );

MM_API void mm_vec_construct( struct mm_vec * restrict this, bool can_alloc, void * restrict buf, size_t size );
MM_API bool mm_vec_copy( struct mm_vec * restrict this, struct mm_vec * restrict other );
MM_API void mm_vec_move( struct mm_vec * restrict this, struct mm_vec * restrict other );
MM_API void mm_vec_destroy( struct mm_vec *this );

static inline bool mm_vec_can_alloc( struct mm_vec const *this ) {
	return !this->is_static;
}

static inline bool mm_vec_inline( struct mm_vec const *this ) {
	return !this->dyn_buf;
}

static inline bool mm_vec_ext( struct mm_vec const *this ) {
	return this->ext_buf;
}

static inline size_t mm_vec_capacity( struct mm_vec const *this, size_t tsize ) {
	if ( this->ext_buf || this->dyn_buf ) {
		return ( this->mem.ext.capacity - this->mem.ext.begin ) / tsize;
	} else {
		return sizeof( this->mem.buf ) / tsize;
	}
}

static inline void* mm_vec_begin( struct mm_vec *this ) {
	if ( mm_vec_inline( this ) ) {
		return this->mem.buf;
	} else {
		return this->mem.ext.begin;
	}
}

static inline void* mm_vec_end( struct mm_vec *this ) {
	return this->end;
}

static inline size_t mm_vec_size( struct mm_vec *this, size_t tsize ) {
	return  ( ( unsigned char* ) mm_vec_end( this ) - ( unsigned char* ) mm_vec_begin( this ) ) / tsize;
}

static inline bool mm_vec_empty( struct mm_vec *this ) {
	return mm_vec_begin( this ) == mm_vec_end( this );
}

MM_API bool mm_vec_set_capacity( struct mm_vec *this, size_t nmem, size_t tsize );
MM_API bool mm_vec_resize( struct mm_vec *this, size_t nmem, size_t tsize );

static inline bool mm_vec_reserve( struct mm_vec *this, size_t nmem, size_t tsize ) {
	return mm_vec_resize( this, mm_vec_size( this, tsize ) + nmem, tsize );
}

static inline bool mm_vec_shrink_to_fit( struct mm_vec *this ) {
	return mm_vec_set_capacity( this, mm_vec_size( this, 1 ), 1 );
}

static inline void mm_vec_clear( struct mm_vec *this ) {
	if ( !mm_vec_inline( this ) ) {
		this->end = this->mem.ext.begin;
	} else {
		this->end = this->mem.buf;
	}
}

static inline void* mm_vec_back( struct mm_vec *this, size_t tsize ) {
	assert( !mm_vec_empty( this ) );
	return ( unsigned char* ) mm_vec_end( this ) - tsize;
}

static inline void* mm_vec_at( struct mm_vec *this, size_t idx, size_t tsize ) {
	assert( !mm_vec_empty( this ) );
	return ( unsigned char* ) mm_vec_begin( this ) + idx * tsize;
}

static inline void* mm_vec_find( struct mm_vec *this, void const *buf, size_t tsize, mm_vec_cmp_t cmp ) {
	for ( unsigned char *pos = mm_vec_begin( this ); pos != mm_vec_end( this ); pos += tsize ) {
		if ( !cmp( pos, buf ) ) {
			return pos;
		}
	}

	return NULL;
}

static inline void* mm_vec_bsearch( struct mm_vec *this, void const *buf, size_t tsize, mm_vec_cmp_t cmp ) {
	return bsearch( buf,
			mm_vec_begin( this ),
			mm_vec_size( this, tsize ),
			tsize,
			cmp );
}

static inline void mm_vec_sort( struct mm_vec *this, size_t tsize, mm_vec_cmp_t cmp ) {
	qsort( mm_vec_begin( this ),
	       mm_vec_size( this, tsize ),
	       tsize,
	       cmp );
}

MM_API void* mm_vec_emplace( struct mm_vec *this, void *pos, size_t tsize );

static inline void* mm_vec_emplace_front( struct mm_vec *this, size_t tsize ) {
	return mm_vec_emplace( this, mm_vec_begin( this ), tsize );
}

static inline void* mm_vec_emplace_back( struct mm_vec *this, size_t tsize ) {
	return mm_vec_emplace( this, mm_vec_end( this ), tsize );
}

MM_API bool mm_vec_insert( struct mm_vec *this, void *pos, void const *buf, size_t tsize );

static inline bool mm_vec_push_front( struct mm_vec *this, void const *buf, size_t tsize ) {
	return mm_vec_insert( this, mm_vec_begin( this ), buf, tsize );
}

static inline bool mm_vec_push_back( struct mm_vec *this, void const *buf, size_t tsize ) {
	return mm_vec_insert( this, mm_vec_end( this ), buf, tsize );
}

MM_API void mm_vec_erase( struct mm_vec *this, void *pos, void *buf, size_t tsize );

static inline void* mm_vec_pop_front( struct mm_vec *this, void *buf, size_t tsize ) {
	mm_vec_erase( this, mm_vec_begin( this ), buf, tsize );
}

static inline void* mm_vec_pop_back( struct mm_vec *this, void *buf, size_t tsize ) {
	mm_vec_erase( this, mm_vec_end( this ), buf, tsize );
}

#define MM_VEC_NEXT( pos, tsize )\
	pos = ( void* ) ( ( unsigned char* ) ( pos ) + ( tsize )  )

#define MM_VEC_PREV( pos, tsize )\
	pos = ( void* ) ( ( unsigned char* ) ( pos ) - ( tsize )  )

#define MM_VEC_FOREACH( vec, pos, type )\
	assert( !mm_vec_empty( vec ) );\
	for ( ( pos ) = mm_vec_begin( vec );\
	      ( pos ) != mm_vec_end( vec );\
	      ( pos ) = MM_VEC_NEXT( pos, sizeof( type ) ) )

#define MM_VEC_FOREACH_REVERSE( vec, pos, type )\
	assert( !mm_vec_empty( vec ) );\
	for ( ( pos ) = mm_vec_back( vec );\
	      ( unsigned char* ) ( pos ) >= ( unsigned char* ) mm_vec_begin( vec );\
	      ( pos ) = MM_VEC_PREV( pos, sizeof( type ) ) )

#endif
