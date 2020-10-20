#ifndef MM_VECTOR_H
#define MM_VECTOR_H
#include <stdlib.h>
#include "mm/common.h"

/*! \file */

/*!
	\brief Dynamically resizing array.

	Capable of holding pre-allocated space or having none at all.
	A mm_vector without any allocated memory is referred to as a NULL mm_vector.
*/
typedef struct mm_vector {
	size_t type_size; //!< \brief size of stored type in bytes
	int ( *type_cmp )( const void*, const void* ); //!< \brief comparator function
	unsigned char *begin; //!< \brief start of allocated memory
	unsigned char *end; //!< \brief end of allocated memory used for inserted elements
	unsigned char *capacity; //!< \brief end of allocated memory
} mm_vector_t;

/*!
	\brief initialize a mm_vector to hold enough capaicty for items of type_size.

	This should not be called more than once on a mm_vector instance until it has been destroyed first.
	If capacity is set to 0 the mm_vector is constructed as a NULL mm_vector.
	\param this mm_vector to initialize.
	\param type_size element size in bytes.
	\param capacity how many elments to allocate space for.
	\return true or false depending on success to allocate memory.
*/
MM_API bool mm_vector_construct( struct mm_vector *this, size_t type_size, size_t capacity );

/*!
	\brief copy from one mm_vector to another.
	\param this mm_vector to copy to.
	\param other mm_vector to copy from.
	\return true or false depending on success to allocate memory.
*/
MM_API bool mm_vector_copy( struct mm_vector *this, struct mm_vector *other );

/*!
	\brief move from one mm_vector to another. The moved from mm_vector is made NULL.
	\param mm_vector to move to.
	\param mm_vector to move from.
*/
MM_API void mm_vector_move( struct mm_vector *this, struct mm_vector *other );

/*!
	\brief deallocate any held memory and reset all fields.
*/
MM_API void mm_vector_destroy( struct mm_vector *this );

/*!
	\param this pointer to mm_vector.
	\return true if the mm_vector hasn't been allocated any memory.
*/
static inline bool mm_vector_null( struct mm_vector *this ) {
	return this->begin == this->capacity;
}

/*!
	\param this pointer to mm_vector.
	\return true if the mm_vector has no items.
*/
static inline bool mm_vector_empty( struct mm_vector *this ) {
	return this->begin == this->end;
}

/*!
	\param this pointer to mm_vector.
	\return current storage capacity of mm_vector in bytes.
*/
static inline size_t mm_vector_bcapacity( struct mm_vector *this ) {
	return this->capacity - this->begin;
}

/*!
	\param this pointer to mm_vector.
	\return current storage capacity of mm_vector in elements.
*/
static inline size_t mm_vector_capacity( struct mm_vector *this) {
	return mm_vector_bcapacity( this ) / this->type_size;
}

/*!
	\param this pointer to mm_vector.
	\return return number of elements in mm_vector in bytes.
*/
static inline size_t mm_vector_bsize( struct mm_vector *this ) {
	return this->end - this->begin;
}

/*!
	\param this pointer to mm_vector.
	\return return number of elements in mm_vector.
*/
static inline size_t mm_vector_size( struct mm_vector *this ) {
	return mm_vector_bsize( this ) / this->type_size;
}

/*!
	\brief set storage capacity of a mm_vector.

	Capacity is set as a multiple of new_capacity * this->type_size.
	Can be used to reserve more space or shrink a mm_vector.
	If capacity is 0, the mm_vector will be made NULL.

	\param this pointer to mm_vector.
	\param new_capacity total storage as a number of elements.
	\return false if memory cannot be allocated.
*/
MM_API bool mm_vector_set_capacity( struct mm_vector *this, size_t new_capacity );

/*!
	\brief set size of a mm_vector.

	Size is set as a multiple of new_size * this->type_size.
	Can be used to extend a mm_vector.
	If a mm_vector's size is shrinked under it's capacity, the left over space isn't deallocated.
	Memory added by this method will be considered occupied ( contains an element ) but will need to be initialized.

	\param this pointer to mm_vector.
	\param new_size total size of mm_vector as a number of elements.
	\return false if memory cannot be allocated.
*/
MM_API bool mm_vector_resize( struct mm_vector *this, size_t new_size );

/*!
	\brief Allocate N extra spaces inside a mm_vector.

	this method internally uses mm_vector_set_capacity().

	\param this pointer to a mm_vector.
	\param delta number of spaces to allocate.
	\return false if memory cannot be allocated.
*/
static inline bool mm_vector_reserve( struct mm_vector *this, size_t delta ) {
	return mm_vector_set_capacity( this, mm_vector_capacity( this ) + delta );
}

/*!
	\brief Reduce mm_vector capacity to the current size.
	\param this pointer to a mm_vector.
*/
static inline void mm_vector_shrink( struct mm_vector *this ) {
	mm_vector_set_capacity( this, mm_vector_size( this ) );
}

/*!
	\brief Remove all stored elements in mm_vector.

	This just moves the end pointer back to the beginning.

	\param this pointer to a mm_vector.
*/
static inline void mm_vector_clear( struct mm_vector *this ) {
	this->end = this->begin;
}

/*!
	\brief Get pointer to Nth element.

	Note, this function assumes that the mm_vector already has elements.

	\param this pointer to a mm_vector.
	\param idx position to index.
	\return pointer to given element.
*/
static inline void* mm_vector_at( struct mm_vector *this, size_t idx ) {
	return this->begin + this->type_size * idx;
}

/*!
	\brief Get pointer to start of memory block.
	\param this pointer to a mm_vector.
*/
static inline void* mm_vector_begin( struct mm_vector *this ) {
	return this->begin;
}

/*!
	\brief Get pointer to end of a memory block.
	\param this pointer to a mm_vector.
*/
static inline void* mm_vector_end( struct mm_vector *this ) {
	return this->end;
}

/*!
	\brief Get pointer to element at the back of a mm_vector.
	\param this pointer to a mm_vector.
*/
static inline void* mm_vector_back( struct mm_vector *this ) {
	if ( mm_vector_null( this ) ) {
		return this->end - this->type_size;
	} else {
		return NULL;
	}
}

/*!
	\brief Search for element inside a mm_vector.

	Internally performs a linear search.

	\param this pointer to a mm_vector.
	\param buf pointer to a value to search for.
	\return pointer to element in mm_vector on success or NULL if it cannot be found.
*/
MM_API void* mm_vector_find( struct mm_vector *this, void *buf );


/*!
	\brief Search for element inside a mm_vector.

	Internally calls bsearch from libc.

	\param this pointer to a mm_vector.
	\param buf pointer to a value to search for.
	\return pointer to element in mm_vector on success or NULL if it cannot be found.
*/
static inline void* mm_vector_search( struct mm_vector *this, void *buf ) {
	return bsearch(
		buf,
		mm_vector_begin( this ),
		mm_vector_bsize( this ),
		this->type_size,
		this->type_cmp
	);
}

/*!
	\brief Sort elements inside a mm_vector.

	Internally calls qsort from libc, so the sort may not be stable.

	\param this pointer to a mm_vector.
*/
static inline void mm_vector_sort( struct mm_vector *this ) {
	qsort(
		mm_vector_begin( this ),
		mm_vector_bsize( this ),
		this->type_size,
		this->type_cmp
	);
}

/*!
	\brief Add new element at a given position.

	Add a new element at a given position.
	A position can be calucated using mm_vector_at, mm_vector_begin, mm_vector_end, mm_vector_find or mm_vector_search.

	\param this pointer to a mm_vector.
	\param pos pointer to a position within a mm_vector.
	\return pointer to newly inserted element. Returns NULL upon failure to allocate memory.
*/
MM_API void* mm_vector_emplace( struct mm_vector *this, void *pos );

/*!
	\brief Insert a new element at a given position.
	\param this pointer to a mm_vector.
	\param pos pointer to a position within a mm_vector.
	\param buf pointer to a value.
	\return false on failure to allocate memory.
*/
MM_API bool mm_vector_insert( struct mm_vector *this, void *pos, void *buf );

/*!
	\brief Emplace an element at the front of a mm_vector.
	\param this pointer to a mm_vector.
	\return pointer to newly inserted element. Returns NULL upon failure to allocate memory.
*/
static inline void* mm_vector_emplace_front( struct mm_vector *this ) {
	return mm_vector_emplace( this, this->begin );
}

/*!
	\brief Emplace an element at the back of a mm_vector.
	\param this pointer to a mm_vector.
	\return pointer to newly inserted element. Returns NULL upon failure to allocate memory.
*/
static inline void* mm_vector_emplace_back( struct mm_vector *this ) {
	return mm_vector_emplace( this, this->end );
}

/*!
	\brief Insert an element at the front of a mm_vector.
	\param this pointer to a mm_vector.
	\param buf pointer to a value.
	\return false upon failure to allocate memory.
*/
static inline bool mm_vector_push_front( struct mm_vector *this, void *buf ) {
	return mm_vector_insert( this, this->begin, buf );
}

/*!
	\brief Insert an element at the back of a mm_vector.
	\param this pointer to a mm_vector.
	\param buf pointer to a value.
	\return false upon failure to allocate memory.
*/
static inline bool mm_vector_push_back( struct mm_vector *this, void *buf ) {
	return mm_vector_insert( this, this->end, buf );
}

/*!
	\brief Remove an element at a given position.
	\param this pointer to a mm_vector.
	\param pos element to remove.
	\param buf buffer to copy element into before removal. Can be NULL.
*/
MM_API void mm_vector_erase( struct mm_vector *this, void *pos, void *buf );

/*!
	\brief Remove element from the front of a mm_vector.
	\param this pointer to a mm_vector.
	\param buf buffer to copy element into before removal. Can be NULL.
*/
static inline void mm_vector_pop_front( struct mm_vector *this, void *buf ) {
	mm_vector_erase( this, this->end - this->type_size, buf );
}

/*!
	\brief Remove element from the back of a mm_vector.
	\param this pointer to a mm_vector.
	\param buf buffer to copy element into before removal. Can be NULL.
*/
static inline void mm_vector_pop_back( struct mm_vector *this, void *buf ) {
	mm_vector_erase( this, this->end - this->type_size, buf );
}

/*!
	\brief Initialize a NULL mm_vector for the given type and cmp
	\param type type or expression that can be passed to sizeof()
	\param cmp function pointer to comparison function, must fit the following prototype. int ( *cmp )( const void*, const void* )
*/
#define MM_VECTOR_INIT( type, cmp )\
	{ .type_size = sizeof( type ), .type_cmp = cmp }

/*!
	\brief Declare mm_vector variable initialized to a given type.
	\param name name of the variable
	\param type type or expression that can be passed to sizeof()
	\param cmp function pointer to comparison function, must fit the following prototype. int ( *cmp )( const void*, const void* )
*/
#define MM_VECTOR_DECLARE( name, type, cmp )\
	struct mm_vector name = MM_VECTOR_INIT( type, cmp )

/*!
	\brief Get pointer to next element in a mm_vector.
	\param vec pointer to a mm_vector.
	\param pos pointer to move from.
*/
#define MM_VECTOR_NEXT( vec, pos )\
	( ( void* ) ( ( unsigned char* ) ( pos ) + ( vec )->type_size ) )

/*!
	\brief Get pointer to previous element in a mm_vector.
	\param vec pointer to a mm_vector.
	\param pos pointer to move from.
*/
#define MM_VECTOR_PREV( vec, pos )\
	( ( void* ) ( ( unsigned char* ) ( pos ) - ( vec )->type_size ) )

/*!
	\brief Get a pointer to an element in a mm_vector casted to a particular type.
	\param vec pointer to a mm_vector.
	\param pos pointer to a mm_vector element.
	\param type type to cast as.
*/
#define MM_VECTOR_AT_AS( vec, pos, type )\
	( ( type* ) mm_vector_at( vec, pos ) )

/*!
	\brief Iterate across each element in a mm_vector.
	\param vec pointer to a mm_vector.
	\param pos pointer to hold current position.
*/
#define MM_VECTOR_FOR_EACH( vec, pos )\
	for( ( pos ) = mm_vector_begin( vec );\
	     ( pos ) < mm_vector_end( vec );\
	     ( pos ) = MM_VECTOR_NEXT( vec, pos ) )

/*!
	\brief Iterate each element backwards in a mm_vector.

	Note this version should be used if you plan to remove elements while iterating.

	\param vec pointer to a mm_vector.
	\param pos pointer to hold current position.
*/
#define MM_VECTOR_FOR_EACH_REVERSE( vec, pos )\
	for( ( pos ) < mm_vector_end( vec );\
	     ( pos ) >= mm_vector_begin( vec );\
	     ( pos ) = MM_VECTOR_PREV( vec, pos ) )
#endif
