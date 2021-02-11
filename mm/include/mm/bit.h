#ifndef MM_BIT_H
#define MM_BIT_H
#include "mm/common.h"

#if MM_USING_BUILTINS && MM_USING_MSVC
#include <intrin.h>
#endif

#define X_UNSIGNED_BIT_TYPES\
	X( unsigned char, cu )\
	X( unsigned short, hu )\
	X( unsigned int, u )\
	X( unsigned long, lu )\
	X( unsigned long long, llu )


#define X_ALL_BIT_TYPES\
	X( char, c )\
	X( short, h )\
	X( int, i )\
	X( long, l )\
	X( long long, ll )\
	X_UNSIGNED_BIT_TYPES

#define X( type, suffix )\
	static inline type mm_imin_##suffix( type a, type b ) {\
		return b ^ ( ( a ^ b ) & -( a < b ) );\
	}

X_ALL_BIT_TYPES
#undef X

#if __STDC_VERSION__ >= 201112L
#define mm_imin( a, b )\
	_Generic( ( a ),\
		char: mm_imin_c,\
		short: mm_imin_h,\
		int: mm_imin_i,\
		long: mm_imin_l,\
		long long: mm_imin_ll,\
		unsigned char: mm_imin_cu,\
		unsigned short: mm_imin_hu,\
		unsigned int: mm_imin_u,\
		unsigned long: mm_imin_lu,\
		unsigned long long: mm_imin_llu\
	)( a, b )
#endif

#define X( type, suffix )\
	static inline type mm_imax_##suffix( type a, type b ) {\
		return a ^ ( ( a ^ b ) & -( a < b ) );\
	}

X_ALL_BIT_TYPES
#undef X

#if __STDC_VERSION__ >= 201112L
#define mm_imax( a, b )\
	_Generic( ( a ),\
		char: mm_imax_c,\
		short: mm_imax_h,\
		int: mm_imax_i,\
		long: mm_imax_l,\
		long long: mm_imax_ll,\
		unsigned char: mm_imax_cu,\
		unsigned short: mm_imax_hu,\
		unsigned int: mm_imax_u,\
		unsigned long: mm_imax_lu,\
		unsigned long long: mm_imax_llu\
	)( a, b )
#endif

#define X( type, suffix )\
	static inline type mm_iclamp_##suffix( type min, type max, type v ) {\
		return mm_imax_##suffix( min, mm_imin_##suffix( max, v ) );\
	}

X_ALL_BIT_TYPES
#undef X

#if __STDC_VERSION__ >= 201112L
#define mm_iclamp( a, b )\
	_Generic( ( a ),\
		char: mm_iclamp_c,\
		short: mm_iclamp_h,\
		int: mm_iclamp_i,\
		long: mm_iclamp_l,\
		long long: mm_iclamp_ll,\
		unsigned char: mm_iclamp_cu,\
		unsigned short: mm_iclamp_hu,\
		unsigned int: mm_iclamp_u,\
		unsigned long: mm_iclamp_lu,\
		unsigned long long: mm_iclamp_llu\
	)( a, b )
#endif

#if MM_USING_BUILTINS && MM_USING_MSVC
static inline unsigned char mm_popcount_cu( unsigned char i ) { return __popcnt16( i ); }
#define mm_popcount_hu __popcnt16
#define mm_popcount_u __popcnt
#define mm_popcount_lu __popcnt
#define mm_popcount_llu __popcnt64
#elif MM_USING_BUILTINS && ( MM_GCC_HAS_BUILTIN( __builtin_popcount ) || MM_GCC_IS_VERSION_OR_LATER( 3, 4, 6 ) )
static inline unsigned char mm_popcount_cu( unsigned char i ) { return __builtin_popcount( i ); }
static inline unsigned short mm_popcount_hu( unsigned short i ) { return __builtin_popcount( i ); }
#define mm_popcount_u __builtin_popcount
#define mm_popcount_lu __builtin_popcountl
#define mm_popcount_llu __builtin_popcountll
#else
#define X( type, suffix )\
	static inline type mm_popcount_##suffix( type i ) {\
		i = i - ( ( i >> 1 ) & ( type ) ~( type ) 0 / 3 );\
		i = ( i & ( type ) ~( type ) 0 / 15 * 3 ) + ( ( i >> 2 ) & ( type ) ~( type ) 0 / 15 * 3 );\
		i = ( i + ( i >> 4 ) ) & ( type )~( type ) 0 / 255 * 15;\
		return ( type ) ( i * ( ( type ) ~( type ) 0 / 255 ) ) >> ( sizeof( type ) - 1 ) * CHAR_BIT;\
	}

X_UNSIGNED_BIT_TYPES
#undef X
#endif

#if __STDC_VERSION__ >= 201112L
#define mm_popcount( i )\
	_Generic( ( i )\
		unsigned char: mm_popcount_cu,\
		unsigned short: mm_popcount_hu,\
		unsigned int: mm_popcount_u,\
		unsigned long: mm_popcount_lu,\
		unsigned long long: mm_popcount_llu,\
	)( i )
#endif

// count leading zeros, assumes value is nonzero
#if MM_USING_BUILTINS && ( MM_GCC_HAS_BUILTIN( __builtin_clz ) || MM_GCC_IS_VERSION_OR_LATER( 3, 4, 6 ) )
static inline unsigned char mm_clz_cu( unsigned char i ) { return __builtin_clz( i ) - ( MM_BITSOF( unsigned int ) - MM_BITSOF( i ) ); }
static inline unsigned short mm_clz_hu( unsigned short i ) { return __builtin_clz( i ) - ( MM_BITSOF( unsigned int ) - MM_BITSOF( i ) ); }
#define mm_clz_u __builtin_clz
#define mm_clz_lu __builtin_clzl
#define mm_clz_llu __builtin_clzll
#else
#define X( type, suffix )\
	static inline type mm_clz_##suffix( type v ) {\
		for ( size_t i = MM_BITSOF( type ); i >= 1; i >>= 1 ) {\
			v |= v >> i;\
		}\
		\
		return mm_popcount_##suffix( ~v );\
	}

X_UNSIGNED_BIT_TYPES
#undef X
#endif

#if __STDC_VERSION__ >= 201112L
#define mm_clz( i ) _Generic( ( i )\
		unsigned char: mm_clz_cu,\
		unsigned short: mm_clz_hu,\
		unsigned int: mm_clz_u,\
		unsigned long: mm_clz_lu,\
		unsigned long long: mm_clz_llu\
	)( i )
#endif

// count trailing zeros, assumes value is nonzero
#if MM_USING_BUILTINS && ( MM_GCC_HAS_BUILTIN( __builtin_ctz ) || MM_GCC_IS_VERSION_OR_LATER( 3, 4, 6 ) )
static inline unsigned char mm_ctz_cu( unsigned char i ) { return __builtin_ctz( i ); }
static inline unsigned short mm_ctz_hu( unsigned short i ) { return __builtin_ctz( i ); }
#define mm_ctz_u __builtin_ctz
#define mm_ctz_lu __builtin_ctzl
#define mm_ctz_llu __builtin_ctzll
#else
#define X( type, suffix )\
	static inline type mm_ctz_##suffix( type v ) {\
		for ( size_t i = MM_BITSOF( type ); i >= 1; i >>= 1 ) {\
			v |= v << i;\
		}\
		\
		return mm_popcount_##suffix( ~v );\
	}

X_UNSIGNED_BIT_TYPES
#undef X
#endif

#if __STDC_VERSION__ >= 201112L
#define mm_ctz( i ) _Generic( ( i )\
		unsigned char: mm_ctz_cu,\
		unsigned short: mm_ctz_hu,\
		unsigned int: mm_ctz_u,\
		unsigned long: mm_ctz_lu,\
		unsigned long long: mm_ctz_llu\
	)( i )
#endif
#undef X_UNSIGNED_BIT_TYPES
#undef X_ALL_BIT_TYPES

#define MM_BITSET_BITS_TO_SIZE( bits )\
	( ( ( bits ) + CHAR_BIT - 1 ) / CHAR_BIT )

#define MM_BITSET_POS_TO_IDX( pos )\
	( ( pos ) / CHAR_BIT )

#define MM_BITSET_MASK( pos )\
	( 1 << ( ( pos ) % CHAR_BIT ) )

#define MM_BITSET( name, bits )\
	unsigned char name[ MM_BITSET_BITS_TO_SIZE( bits ) ]

#define MM_BITSET_DECLARE( name, bits )\
	MM_BITSET( name, bits ) = { 0 }

#define MM_BITSET_SIZE( name )\
	MM_ARR_SIZE( name )

#define MM_BITSET_SET( name, pos )\
	( ( name )[ MM_BITSET_POS_TO_IDX( pos ) ] |= MM_BITSET_MASK( pos ) )

#define MM_BITSET_CLEAR( name, pos )\
	( ( name )[ MM_BITSET_POS_TO_IDX( pos ) ] &= ~MM_BITSET_MASK( pos ) )

#define MM_BITSET_TOGGLE( name, pos )\
	( ( name )[ MM_BITSET_POS_TO_IDX( pos ) ] ^= MM_BITSET_MASK( pos ) )

#define MM_BITSET_TEST( name, pos )\
	( ( name )[ MM_BITSET_POS_TO_IDX( pos ) ] & MM_BITSET_MASK( pos ) )

#endif
