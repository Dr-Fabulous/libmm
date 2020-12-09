#include "mm/vector.h"
#include "mm/alloc.h"
#include <assert.h>
#include <string.h>

static void reset( struct mm_vector *this ) {
	this->begin = NULL;
	this->end = NULL;
	this->capacity = NULL;
}

bool mm_vector_construct( struct mm_vector *this, size_t capacity, size_t type_size ) {
	this->dynamic = true;
	return mm_vector_set_capacity( this, capacity, type_size );
}

void mm_vector_construct_with_buf( struct mm_vector *this, void *buf, size_t size ) {
	this->dynamic = false;
	this->begin = buf;
	this->end = buf;
	this->capacity = this->begin + size;
}

bool mm_vector_copy( struct mm_vector *this, struct mm_vector *other ) {
	size_t size = mm_vector_bsize( other );

	if ( !mm_vector_resize( this, 1, size ) ) {
		return false;
	}

	memcpy( this->begin, other->begin, size );

	return true;
}

void mm_vector_move( struct mm_vector *this, struct mm_vector *other ) {
	*this = *other;
	reset( other );
}

void mm_vector_destroy( struct mm_vector *this ) {
	if ( !this->dynamic ) {
		return;
	}

	if ( !mm_vector_null( this ) ) {
		MM_FREE( this->begin );
	}

	reset( this );
}

bool mm_vector_set_capacity( struct mm_vector *this, size_t new_capacity, size_t type_size ){
	if ( !this->dynamic ) {
		return false;
	}

	if ( !new_capacity ) {
		if ( !mm_vector_null( this ) ) {
			mm_vector_destroy( this );
		}

		return true;
	}

	new_capacity *= type_size;
	size_t size = mm_vector_bsize( this );

	if ( size > new_capacity ) {
		size = new_capacity;
	}

	void *begin = MM_REALLOC( this->begin, new_capacity );

	if ( !begin ) {
		return false;
	}

	this->begin = begin;
	this->end = this->begin + size;
	this->capacity = this->begin + new_capacity;

	return true;
}

bool mm_vector_resize( struct mm_vector *this, size_t new_size, size_t type_size ){
	if ( mm_vector_capacity( this, type_size ) < new_size && !mm_vector_set_capacity( this, new_size, type_size ) ) {
		return false;
	}

	this->end = this->begin + new_size * type_size;

	return true;
}

void* mm_vector_find( struct mm_vector *this, void *buf, size_t type_size, int( *cmp )( const void*, const void* ) ) {
	void *pos;

	MM_VECTOR_FOR_EACH( this, pos, type_size ) {
		if ( !cmp( buf, pos ) ) {
			return pos;
		}
	}

	return NULL;
}

void* mm_vector_emplace( struct mm_vector *this, void *pos, size_t type_size ) {
	assert( ( unsigned char* ) pos >= this->begin && ( unsigned char* ) pos <= this->end );

	ptrdiff_t s_offset = ( unsigned char* ) pos - this->begin;
	ptrdiff_t e_offset = mm_vector_bsize( this ) - s_offset;

	if ( !mm_vector_resize( this, 1, mm_vector_bsize( this ) + type_size ) ) {
		return NULL;
	}

	pos = this->begin + s_offset;

	if ( e_offset ) {
		memmove( ( unsigned char* ) pos + type_size, pos, e_offset );
	}

	memset( pos, 0, type_size );
	
	return pos;
}

bool mm_vector_insert( struct mm_vector *this, void *pos, void *buf, size_t type_size ) {
	assert( ( unsigned char* ) pos >= this->begin && ( unsigned char* ) pos <= this->end );
	
	pos = mm_vector_emplace( this, pos, type_size );

	if ( pos ) {
		memcpy( pos, buf, type_size );
		return true;
	} else {
		return false;
	}
}

void mm_vector_erase( struct mm_vector *this, void *pos, void *buf, size_t type_size ){
	assert( ( unsigned char* ) pos >= this->begin && ( unsigned char* ) pos < this->end );
	
	ptrdiff_t offset = mm_vector_bsize( this ) - ( ( unsigned char* ) pos - this->begin ) - type_size;

	if ( buf ) {
		memcpy( buf, pos, type_size );
	}

	if ( offset ) {
		memmove( pos, ( unsigned char* ) pos + type_size, offset );
	}

	mm_vector_resize( this, mm_vector_size( this, type_size ) - 1, type_size );
}
