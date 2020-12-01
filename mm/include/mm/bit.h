#ifndef MM_BIT_H
#define MM_BIT_H
#include "mm/common.h"

#if MM_USING_BUILTINS && MM_USING_MSVC
#include <intrin.h>
#endif

static inline short mm_imin_s16( short a, short b ) {
	return b ^ ( ( a ^ b ) & -( a < b ) );
}

static inline long mm_imin_s32( long a, long b ) {
	return b ^ ( ( a ^ b ) & -( a < b ) );
}

static inline long long mm_imin_s64( long long a, long long b ) {
	return b ^ ( ( a ^ b ) & -( a < b ) );
}

static inline unsigned short mm_imin_u16( unsigned short a, unsigned short b ) {
	return b ^ ( ( a ^ b ) & -( a < b ) );
}

static inline unsigned long mm_imin_u32( unsigned long a, unsigned long b ) {
	return b ^ ( ( a ^ b ) & -( a < b ) );
}

static inline unsigned long long mm_imin_u64( unsigned long long a, unsigned long long b ) {
	return b ^ ( ( a ^ b ) & -( a < b ) );
}

#if MM_C_STD >= 11
#define mm_imin( a, b ) _Generic( ( a ) < ( b ),\
		char: mm_imin_s16,\
		short: mm_imin_s16,\
		int: mm_imin_s32,\
		long: mm_imin_s32,\
		long long: mm_imin_s64\
		unsigned char: mm_imin_u16,\
		unsigned short: mm_imin_u16,\
		unsigned int: mm_imin_u32,\
		unsigned long: mm_imin_u32,\
		unsigned long long: mm_imin_u64\
	)( a, b )
#endif
static inline short mm_imax_s16( short a, short b ) {
	return a ^ ( ( a ^ b ) & -( a < b ) );
}

static inline long mm_imax_s32( long a, long b ) {
	return a ^ ( ( a ^ b ) & -( a < b ) );
}

static inline long long mm_imax_s64( long long a, long long b ) {
	return a ^ ( ( a ^ b ) & -( a < b ) );
}

static inline unsigned short mm_imax_u16( unsigned short a, unsigned short b ) {
	return a ^ ( ( a ^ b ) & -( a < b ) );
}

static inline unsigned long mm_imax_u32( unsigned long a, unsigned long b ) {
	return a ^ ( ( a ^ b ) & -( a < b ) );
}

static inline unsigned long long mm_imax_u64( unsigned long long a, unsigned long long b ) {
	return a ^ ( ( a ^ b ) & -( a < b ) );
}

#if MM_C_STD >= 11
#define mm_imax( a, b ) _Generic( ( a ) < ( b ),\
		char: mm_imax_s16,\
		short: mm_imax_s16,\
		int: mm_imax_s32,\
		long: mm_imax_s32,\
		long long: mm_imax_s64\
		unsigned char: mm_imax_u16,\
		unsigned short: mm_imax_u16,\
		unsigned int: mm_imax_u32,\
		unsigned long: mm_imax_u32,\
		unsigned long long: mm_imax_u64\
	)( a, b )
#endif

static inline short mm_iclamp_s16( short min, short max, short v ) {
	return mm_imax_s16( min, mm_imin_s16( max, v ) );
}

static inline long mm_iclamp_s32( long min, long max, long v ) {
	return mm_imax_s32( min, mm_imin_s32( max, v ) );
}

static inline long long mm_iclamp_s64( long long min, long long max, long long v ) {
	return mm_imax_s64( min, mm_imin_s64( max, v ) );
}

static inline unsigned short mm_iclamp_u16( unsigned short min, unsigned short max, unsigned short v ) {
	return mm_imax_u16( min, mm_imin_u16( max, v ) );
}

static inline unsigned long mm_iclamp_u32( unsigned long min, unsigned long max, unsigned long v ) {
	return mm_imax_u32( min, mm_imin_u32( max, v ) );
}

static inline unsigned long mm_iclamp_u64( unsigned long min, unsigned long max, unsigned long v ) {
	return mm_imax_u64( min, mm_imin_u64( max, v ) );
}

#if MM_C_STD >= 11
#define mm_iclamp( a, b, v ) _Generic( ( v ),\
		char: mm_clamp_s16,\
		short: mm_clamp_s16,\
		int: mm_clamp_s32,\
		long: mm_clamp_s32,\
		long long: mm_clamp_s64\
		unsigned char: mm_clamp_u16,\
		unsigned short: mm_clamp_u16,\
		unsigned int: mm_clamp_u32,\
		unsigned long: mm_clamp_u32,\
		unsigned long long: mm_clamp_u64\
	)( a, b, v )
#endif

#if MM_USING_BUILTINS && MM_USING_MSVC
#define mm_popcount_u16 __popcnt16
#define mm_popcount_u32 __popcnt
#define mm_popcount_u64 __popcnt64
#elif MM_USING_BUILTINS && ( MM_GCC_HAS_BUILTIN( __builtin_popcount ) || MM_GCC_IS_VERSION_OR_LATER( 3, 4, 6 ) )
#define mm_popcount_u16 __builtin_popcount
#define mm_popcount_u32 __builtin_popcountl
#define mm_popcount_u64 __builtin_popcountll
#else
#define MM_POPCOUNT_IMPL( type, i )\
	do {\
		i = i - ( ( i >> 1 ) & ( type ) ~( type ) 0 / 3 );\
		i = ( i & ( type ) ~( type ) 0 / 15 * 3 ) + ( ( i >> 2 ) & ( type ) ~( type ) 0 / 15 * 3 );\
		i = ( i + ( i >> 4 ) ) & ( type )~( type ) 0 / 255 * 15;\
		return ( type ) ( i * ( ( type ) ~( type ) 0 / 255 ) ) >> ( sizeof( type ) - 1 ) * CHAR_BIT;\
	} while( 0 )
static inline unsigned short mm_popcount_u16( unsigned short i ) {
	MM_POPCOUNT_IMPL( unsigned short, i );
}

static inline unsigned long mm_popcount_u32( unsigned long i ) {
	MM_POPCOUNT_IMPL( unsigned long, i );
}

static inline unsigned long long mm_popcount_u64( unsigned long long i ) {
	MM_POPCOUNT_IMPL( unsigned long long, i );
}
#undef MM_POPCOUNT_IMPL
#endif

#if MM_C_STD >= 11
#define mm_popcount( i ) _Generic( ( i )\
		unsigned char: mm_popcount_u16,\
		unsigned short: mm_popcount_u16,\
		unsigned int: mm_popcount_u32,\
		unsigned long: mm_popcount_u32,\
		unsigned long long: mm_popcount_u64\
	)( i )
#endif

// count leading zeros, assumes value is nonzero
#if MM_USING_BUILTIN && MM_USING_MSVC
// microsoft notes that this intrinsic may be buggy on some intel chips
#define mm_clz_u16 __lzcnt16
#define mm_clz_u32 __lzcnt
#define mm_clz_u64 __lzcnt64
#elif MM_USING_BUILTINS && ( MM_GCC_HAS_BUILTIN( __builtin_clz ) || MM_GCC_IS_VERSION_OR_LATER( 3, 4, 6 ) )
#define mm_clz_u16( i ) ( ( unsigned short ) ( __builtin_clz( i ) - ( MM_BITSOF( unsigned int ) - 16 ) ) )
#define mm_clz_u32( i ) ( ( unsigned long ) ( __builtin_clzl( i ) - ( MM_BITSOF( unsigned long ) - 32 ) ) )
#define mm_clz_u64( i ) ( ( unsigned long long ) ( __builtin_clzll( i ) - ( MM_BITSOF( unsigned long long ) - 64 ) ) )
#else
static inline unsigned short mm_clz_u16( unsigned short i ) {
	i |= i >> 8;
	i |= i >> 4;
	i |= i >> 2;
	i |= i >> 1;

	return mm_popcount_u16( ~i );
}

static inline unsigned long mm_clz_u32( unsigned long i ) {
	i |= i >> 16;
	i |= i >> 8;
	i |= i >> 4;
	i |= i >> 2;
	i |= i >> 1;

	return mm_popcount_u32( ~i );
}

static inline unsigned long long mm_clz_u64( unsigned long long i ) {
	i |= i >> 32;
	i |= i >> 16;
	i |= i >> 8;
	i |= i >> 4;
	i |= i >> 2;
	i |= i >> 1;

	return mm_popcount_u64( ~i );
}
#endif

#if MM_C_STD >= 11
#define mm_clz( i ) _Generic( ( i )\
		unsigned char: mm_clz_u16,\
		unsigned short: mm_clz_u16,\
		unsigned int: mm_clz_u32,\
		unsigned long: mm_clz_u32,\
		unsigned long long: mm_clz_u64\
	)( i )
#endif

// count trailing zeros, assumes value is nonzero
// no instrinstic availible on MSVC
#if MM_USING_BUILTIN && ( MM_GCC_HAS_BUILTIN( __builtin_clz ) || MM_GCC_IS_VERSION_OR_LATER( 3, 4, 6 ) )
#define mm_ctz_u16( i ) ( ( unsigned short ) ( __builtin_ctz( i ) - ( MM_BITSOF( unsigned int ) - 16 ) ) )
#define mm_ctz_u32( i ) ( ( unsigned long ) ( __builtin_ctzl( i ) - ( MM_BITSOF( unsigned long ) - 32 ) ) )
#define mm_ctz_u64( i ) ( ( unsigned long long ) ( __builtin_ctzll( i ) - ( MM_BITSOF( unsigned long long ) - 64 ) ) )
#else
static inline unsigned short mm_ctz_u16( unsigned short i ) {
	i |= i << 8;
	i |= i << 4;
	i |= i << 2;
	i |= i << 1;

	return mm_popcount_u16( ~i );
}

static inline unsigned long mm_ctz_u32( unsigned long i ) {
	i |= i << 16;
	i |= i << 8;
	i |= i << 4;
	i |= i << 2;
	i |= i << 1;

	return mm_popcount_u32( ~i );
}

static inline unsigned long long mm_ctz_u64( unsigned long long i ) {
	i |= i << 32;
	i |= i << 16;
	i |= i << 8;
	i |= i << 4;
	i |= i << 2;
	i |= i << 1;

	return mm_popcount_u64( ~i );
}
#endif

// bitset ops

#endif
