#ifndef MM_BIT_H
#define MM_BIT_H
#include "mm/common.h"

#if MM_HAS_BUILTIN( __builtin_popcount )
#define mm_popcount_8 __builtin_popcount
#define mm_popcount_16 __builtin_popcount
#define mm_popcount_32 __builtin_popcountl
#define mm_popcount_64 __builtin_popcountll
#else
static inline size_t mm_popcount_8( unsigned char *byte ) {
	size_t count = 0;

	for ( size_t i = 0; i < sizeof( *byte ) << 3; ++i ) {
		count += ( *byte >> i ) & 0x1;
	}

	return count;
}

static inline size_t mm_popcount_16( uint_least16_t i ) {
	size_t count = 0;

	for( size_t j = 0; i < sizeof( j ); ++j ) {
		count += mm_popcount_byte( ( unsigned char* ) &i + j );
	}

	return count;
}
#endif

#endif
