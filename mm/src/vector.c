#include "mm/vector.h"
#include "mm/assert.h"
#include <stdlib.h>
#include <string.h>

static void reset( struct mm_vector *this ) {
	this->begin = NULL;
	this->end = NULL;
	this->capacity = NULL;
}

bool mm_vector_construct( struct mm_vector *this, size_t type_size, size_t capacity ) {
	this->type_size = type_size;
	return mm_vector_set_capacity( this, capacity );
}

bool mm_vector_copy( struct mm_vector *this, struct mm_vector *other ) {
	this->type_size = other->type_size;
	size_t size = mm_vector_size( other );

	if ( !mm_vector_resize( this, size ) ) {
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
	if ( !mm_vector_null( this ) ) {
		MM_FREE( this->begin );
	}

	reset( this );
}

bool mm_vector_set_capacity( struct mm_vector *this, size_t new_capacity ){
	if ( !new_capacity ) {
		if ( !mm_vector_null( this ) ) {
			mm_vector_destroy( this );
		}

		return true;
	}

	new_capacity *= this->type_size;
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

bool mm_vector_resize( struct mm_vector *this, size_t new_size ){
	if ( mm_vector_capacity( this ) < new_size && !mm_vector_set_capacity( this, new_size ) ) {
		return false;
	}

	this->end = this->begin + this->type_size * new_size;

	return true;
}

void* mm_vector_find( struct mm_vector *this, void *buf ) {
	void *pos;

	MM_VECTOR_FOR_EACH( this, pos ) {
		if ( !this->type_cmp( buf, pos ) ) {
			return pos;
		}
	}

	return NULL;
}

void* mm_vector_emplace( struct mm_vector *this, void *pos ) {
	MM_ASSERT( ( unsigned char* ) pos >= this->begin && ( unsigned char* ) pos <= this->end );
	MM_ASSERT( ( uintptr_t ) pos % this->type_size == 0 );
	
	ptrdiff_t s_offset = ( unsigned char* ) pos - this->begin;
	ptrdiff_t e_offset = mm_vector_bsize( this ) - s_offset;

	if ( !mm_vector_resize( this, mm_vector_size( this ) + 1 ) ) {
		return NULL;
	}

	pos = this->begin + s_offset;

	if ( e_offset ) {
		memmove( ( unsigned char* ) pos + this->type_size, pos, e_offset );
	}

	memset( pos, 0, this->type_size );
	
	return pos;
}

bool mm_vector_insert( struct mm_vector *this, void *pos, void *buf ) {
	MM_ASSERT( ( unsigned char* ) pos >= this->begin && ( unsigned char* ) pos <= this->end );
	MM_ASSERT( ( uintptr_t ) pos % this->type_size == 0 );
	
	pos = mm_vector_emplace( this, pos );

	if ( pos ) {
		memcpy( pos, buf, this->type_size );
		return true;
	} else {
		return false;
	}
}

void mm_vector_erase( struct mm_vector *this, void *pos, void *buf ){
	MM_ASSERT( ( unsigned char* ) pos >= this->begin && ( unsigned char* ) pos < this->end );
	MM_ASSERT( ( uintptr_t ) pos % this->type_size == 0 );
	
	ptrdiff_t offset = mm_vector_bsize( this ) - ( ( unsigned char* ) pos - this->begin ) - this->type_size;

	if ( buf ) {
		memcpy( buf, pos, this->type_size );
	}

	if ( offset ) {
		memmove( pos, ( unsigned char* ) pos + this->type_size, offset );
	}

	mm_vector_resize( this, mm_vector_size( this ) - 1 );
}
