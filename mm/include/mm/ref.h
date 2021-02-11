#ifndef MM_REF_H
#define MM_REF_H
#include <assert.h>
#include "mm/common.h"

#if __STDC_VERSION__ >= 201112L && !defined( __STDC_NO_ATOMICS__ )
#include <stdatomic.h>
#define MM_REF_USING_ATOMIC 1
#else
#define MM_REF_USING_ATOMIC 0
#endif

typedef struct mm_ref {
#if MM_REF_USING_ATOMIC
	atomic_int count;
#else
	int count;
#endif
} mm_ref_t;

#define MM_REF_INIT()\
	{ .count = 0 }

#define MM_REF_DECLARE( name )\
	struct mm_ref name = MM_REF_INIT()

static inline void mm_ref_construct( struct mm_ref * const this ) {
	*this = ( struct mm_ref ) MM_REF_INIT();
}

static inline int mm_ref_count( struct mm_ref const * const this ) {
#if MM_REF_USING_ATOMIC
	return atomic_load_explicit( &this->count, memory_order_acquire );
#else
	return this->count;
#endif
}

static inline bool mm_ref_null( struct mm_ref * const this ) {
	return mm_ref_count( this ) == -1;
}

static inline void mm_ref_inc( struct mm_ref * const this ) {
	assert( !mm_ref_null( this ) );

#if MM_REF_USING_ATOMIC
	atomic_fetch_add_explicit( &this->count, 1, memory_order_release );
#else
	++this->count;
#endif
}

static inline bool mm_ref_dec( struct mm_ref * const this ) {
	assert( !mm_ref_null( this ) );

#if MM_REF_USING_ATOMIC
	if ( atomic_fetch_sub_explicit( &this->count, 1, memory_order_acq_rel ) == 1 ) {
		atomic_store_explicit( &this->count, -1, memory_order_release );
		return true;
	}
#else
	if ( !( --this->count ) ) {
		this->count = -1;
		return true;
	}
#endif

	return false;
}

#undef MM_REF_USING_ATOMIC
#endif
