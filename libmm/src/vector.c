#include <stdlib.h>
#include <string.h>
#include "libmm/vector.h"

static void reset( struct vector *this ) {
	this->begin = NULL;
	this->end = NULL;
	this->capacity = NULL;
}

bool vector_construct( struct vector *this, size_t type_size, size_t capacity ) {
	this->type_size = type_size;
	return vector_set_capacity( this, capacity );
}

bool vector_copy( struct vector *this, struct vector *other ) {
	this->type_size = other->type_size;
	size_t size = vector_size( other );

	if ( !vector_resize( this, size ) ) {
		return false;
	}

	memcpy( this->begin, other->begin, size );

	return true;
}

void vector_move( struct vector *this, struct vector *other ) {
	*this = *other;
	reset( other );
}

void vector_destroy( struct vector *this ) {
	if ( !vector_null( this ) ) {
		LIBMM_FREE( this->begin );
	}

	reset( this );
}

bool vector_null( struct vector *this ) {
	return !this->begin;
}

bool vector_empty( struct vector *this ) {
	return vector_null( this ) || this->begin == this->end;
}

size_t vector_bsize( struct vector *this ) {
	return this->end - this->begin;
}

size_t vector_size( struct vector *this ) {
	return vector_bsize( this ) / this->type_size;
}

size_t vector_bcapacity( struct vector *this ){
	return ( this->capacity - this->begin );
}

size_t vector_capacity( struct vector *this ){
	return vector_bcapacity( this ) / this->type_size;
}

bool vector_set_capacity( struct vector *this, size_t new_capacity ){
	if ( !new_capacity ) {
		if ( !vector_null( this ) ) {
			LIBMM_FREE( this->begin );
		}

		return true;
	}

	new_capacity *= this->type_size;
	size_t size = vector_bsize( this );

	if ( size > new_capacity ) {
		size = new_capacity;
	}

	void *begin = LIBMM_REALLOC( this->begin, new_capacity );

	if ( !begin ) {
		return false;
	}

	this->begin = begin;
	this->end = this->begin + size;
	this->capacity = this->begin + new_capacity;

	return true;
}

bool vector_resize( struct vector *this, size_t new_size ){
	if ( vector_capacity( this ) < new_size && !vector_set_capacity( this, new_size ) ) {
		return false;
	}

	this->end = this->begin + this->type_size * new_size;

	return true;
}

bool vector_reserve( struct vector *this, size_t delta ){
	return vector_set_capacity( this, vector_capacity( this ) + delta );
}

void vector_shrink( struct vector *this ){
	vector_set_capacity( this, vector_size( this ) );
}

void vector_clear( struct vector *this ) {
	this->end = this->begin;
}

void* vector_at( struct vector *this, size_t idx ){
	return this->begin + idx;
}

void* vector_begin( struct vector *this ){
	return this->begin;
}

void* vector_end( struct vector *this ){
	return this->end;
}

void* vector_back( struct vector *this ) {
	if ( vector_null( this ) ) {
		return this->end - this->type_size;
	} else {
		return NULL;
	}
}

void* vector_find( struct vector *this, void *buf, int ( *cmp )( const void*, const void* ) ) {
	void *pos;

	LIBMM_VECTOR_FOR_EACH( this, pos ) {
		if ( !cmp( buf, pos ) ) {
			return pos;
		}
	}

	return NULL;
}

void* vector_search( struct vector *this, void *buf, int ( *cmp )( const void*, const void* ) ) {
	return bsearch(
		buf,
		vector_begin( this ),
		vector_bsize( this ),
		this->type_size,
		cmp
	);
}

void vector_sort( struct vector *this, int ( *cmp )( const void*, const void* ) ) {
	qsort(
		vector_begin( this ),
		vector_bsize( this ),
		this->type_size,
		cmp
	);
}

bool vector_insert( struct vector *this, void *pos, void *buf ) {
	ptrdiff_t offset = ( unsigned char* ) pos - this->begin;

	if ( !vector_resize( this, vector_size( this ) + 1 ) ) {
		return false;
	}

	pos = this->begin + offset;
	offset = vector_bsize( this ) - offset;

	if ( offset ) {
		memmove( ( unsigned char* ) pos + this->type_size, pos, offset );
	}

	memcpy( pos, buf, this->type_size );

	return true;
}

bool vector_push( struct vector *this, void *buf ){
	return vector_insert( this, this->end, buf );
}

void vector_erase( struct vector *this, void *pos, void *buf ){
	ptrdiff_t offset = vector_bsize( this ) - ( ( unsigned char* ) pos - this->begin );

	if ( buf ) {
		memcpy( buf, pos, this->type_size );
	}

	if ( offset ) {
		memmove( pos, ( unsigned char* ) pos + this->type_size, offset );
	}

	vector_resize( this, vector_size( this ) - 1 );
}

void vector_pop( struct vector *this, void *buf ){
	vector_erase( this, this->end - this->type_size, buf );
}
