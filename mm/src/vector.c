#include <stdlib.h>
#include <string.h>
#include "mm/vector.h"

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
			MM_FREE( this->begin );
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

void* mm_vector_find( struct mm_vector *this, void *buf, int ( *cmp )( const void*, const void* ) ) {
	void *pos;

	MM_VECTOR_FOR_EACH( this, pos ) {
		if ( !cmp( buf, pos ) ) {
			return pos;
		}
	}

	return NULL;
}

bool mm_vector_insert( struct mm_vector *this, void *pos, void *buf ) {
	ptrdiff_t offset = ( unsigned char* ) pos - this->begin;

	if ( !mm_vector_resize( this, mm_vector_size( this ) + 1 ) ) {
		return false;
	}

	pos = this->begin + offset;
	offset = mm_vector_bsize( this ) - offset;

	if ( offset ) {
		memmove( ( unsigned char* ) pos + this->type_size, pos, offset );
	}

	memcpy( pos, buf, this->type_size );

	return true;
}

void mm_vector_erase( struct mm_vector *this, void *pos, void *buf ){
	ptrdiff_t offset = mm_vector_bsize( this ) - ( ( unsigned char* ) pos - this->begin );

	if ( buf ) {
		memcpy( buf, pos, this->type_size );
	}

	if ( offset ) {
		memmove( pos, ( unsigned char* ) pos + this->type_size, offset );
	}

	mm_vector_resize( this, mm_vector_size( this ) - 1 );
}
