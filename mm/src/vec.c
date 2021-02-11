#include <stdlib.h>
#include <string.h>
#include "mm/vec.h"

static inline void mm_vec_reset( struct mm_vec *this ) {
	*this = ( struct mm_vec ) { 0 };
}

void mm_vec_construct( struct mm_vec * restrict this, bool can_alloc, void * restrict buf, size_t size ) {
	mm_vec_reset( this );
	
	this->is_static = !can_alloc;

	if ( buf ) {
		this->ext_buf = 1;
		this->mem.ext.begin = buf;
		this->mem.ext.capacity = this->mem.ext.begin + size;
		this->end = this->mem.ext.begin;
	}
}

bool mm_vec_copy( struct mm_vec * restrict this, struct mm_vec * restrict other ) {
	size_t size = mm_vec_size( other, 1 );

	if ( !mm_vec_resize( this, 1, size ) ) {
		return false;
	}

	memcpy( mm_vec_begin( this ), mm_vec_begin( other ), size );

	return true;
}

void mm_vec_move( struct mm_vec * restrict this, struct mm_vec * restrict other ) {
	*this = *other;
	mm_vec_reset( other );
}

void mm_vec_destroy( struct mm_vec *this ) {
	mm_vec_set_capacity( this, 0, 0 );
}

bool mm_vec_set_capacity( struct mm_vec *this, size_t nmem, size_t tsize ) {
	size_t new_cap = nmem * tsize; // maybe replace with saturated arithmetic
	size_t size = mm_vec_size( this, 1 );
	unsigned char *new;

	if ( new_cap < size ) {
		size = new_cap;
	}

	if ( mm_vec_inline( this ) ) {
		if ( new_cap <= mm_vec_capacity( this, 1 ) ) {
			if ( mm_vec_ext( this ) ) {
				this->end = this->mem.ext.begin + size;
			} else {
				this->end = this->mem.buf + size;
			}

			return true;
		}

		new = MM_MALLOC( new_cap );

		if ( !new ) {
			return false;
		}

		if ( mm_vec_ext( this ) ) {
			memcpy( new, this->mem.ext.begin, size );
		} else {
			memcpy( new, this->mem.buf, size );
		}

		this->ext_buf = 0;
		this->dyn_buf = 1;
	} else {
		if ( new_cap < sizeof( this->mem.buf ) ) {
			unsigned char *old = this->mem.ext.begin;

			memcpy( this->mem.buf, old, size );
			this->dyn_buf = 0;
			this->end = this->mem.buf + size;
			
			MM_FREE( old );

			return true;
		}

		new = MM_REALLOC( this->mem.ext.begin, new_cap );

		if ( !new ) {
			return false;
		}
	}

	this->mem.ext.begin = new;
	this->mem.ext.capacity = this->mem.ext.begin + new_cap;
	this->end = this->mem.ext.begin + size;

	return true;
}

bool mm_vec_resize( struct mm_vec *this, size_t nmem, size_t tsize ) {
	if ( mm_vec_capacity( this, tsize ) < nmem && !mm_vec_set_capacity( this, nmem, tsize ) ) {
		return false;
	}

	this->end = ( unsigned char* ) mm_vec_begin( this ) + nmem * tsize;

	return true;
}

void* mm_vec_emplace( struct mm_vec *this, void *pos, size_t tsize ) {
	assert( ( unsigned char* ) pos >= ( unsigned char* ) mm_vec_begin( this )
	     && ( unsigned char* ) pos <= ( unsigned char* ) mm_vec_end( this ) );

	size_t start = ( unsigned char* ) pos - ( unsigned char* ) mm_vec_begin( this );
	size_t end = mm_vec_size( this, 1 ) - start;

	if ( !mm_vec_resize( this, mm_vec_size( this, tsize ) + 1, tsize ) ) {
		return NULL;
	}

	pos = ( unsigned char* ) mm_vec_begin( this ) + start;

	if ( end ) {
		memmove( ( unsigned char* ) pos + tsize, pos, end );
	}

	memset( pos, 0, tsize );

	return pos;
}

bool mm_vec_insert( struct mm_vec *this, void *pos, void const *buf, size_t tsize ) {
	pos = mm_vec_emplace( this, pos, tsize );

	if ( pos ) {
		memcpy( pos, buf, tsize );
		return true;
	} else {
		return false;
	}
}

void mm_vec_erase( struct mm_vec *this, void *pos, void *buf, size_t tsize ) {
	assert( ( unsigned char* ) pos >= ( unsigned char* ) mm_vec_begin( this )
	     && ( unsigned char* ) pos <= ( unsigned char* ) mm_vec_end( this ) );

	if ( buf ) {
		memcpy( buf, pos, tsize );
	}

	ptrdiff_t offset = mm_vec_size( this, 1 ) - ( ( unsigned char* ) pos - ( unsigned char* ) mm_vec_begin( this ) ) - tsize;

	if ( offset ) {
		memmove( pos, ( unsigned char* ) pos + tsize, offset );
	}

	mm_vec_resize( this, mm_vec_size( this, tsize ) - 1, tsize );
}
