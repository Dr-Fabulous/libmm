#ifndef MM_ENDIAN_H
#define MM_ENDIAN_H
#include "mm/common.h"

#if MM_HAS_INCLUDE( <endian.h> )
#include <endian.h>
#elif MM_HAS_INCLUDE( <machine/endian.h> )
#include <machine/endian.h>
#elif MM_HAS_INCLUDE( <sys/param.h> )
#include <sys/param.h>
#elif MM_HAS_INCLUDE( <sys/isadefs.h> )
#include <sys/isadefs.h>
#endif

#if ( defined( __BYTE_ORDER__ ) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )\
 || ( defined( __BYTE_ORDER ) && __BYTE_ORDER == __BIG_ENDIAN )\
 || ( defined( _BYTE_ORDER ) && _BYTE_ORDER == _BIG_ENDIAN )\
 || ( defined( BYTE_ORDER ) && BYTE_ORDER == BIG_ENDIAN )\
 || ( defined( __sun ) && defined( __SVR4 ) && defined( _BIG_ENDIAN ) )\
 ||   defined( __ARMEB__ )\
 ||   defined( __THUMBEB__ )\
 ||   defined( __AARCH64EB__ )\
 ||   defined( _MIBSEB )\
 ||   defined( __MIBSEB )\
 ||   defined( __MIBSEB__ )\
 ||   defined( _M_PPC )

#define MM_LITTLE_ENDIAN 1
#define MM_BYTE_ORDER MM_LITTLE_ENDIAN

#elif ( defined( __BYTE_ORDER__ ) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)\
   || ( defined( __BYTE_ORDER ) && __BYTE_ORDER == __LITTLE_ENDIAN )\
   || ( defined( _BYTE_ORDER ) && _BYTE_ORDER == _LITTLE_ENDIAN )\
   || ( defined( BYTE_ORDER ) && BYTE_ORDER == LITTLE_ENDIAN )\
   || ( defined( __sun ) && defined( __SVR4 ) && defined( _LITTLE_ENDIAN ) )\
   ||   defined( __ARMEL__ )\
   ||   defined( __THUMBEL__ )\
   ||   defined( __AARCH64EL__ )\
   ||   defined( _MIPSEL )\
   ||   defined( __MIPSEL )\
   ||   defined( __MIPSEL__ )\
   ||   defined( _M_IX86 )\
   ||   defined( _M_X64 )\
   ||   defined( _M_IA64 )\
   ||   defined( _M_ARM )

#define MM_BIG_ENDIAN 1
#define MM_BYTE_ORDER MM_BIG_ENDIAN
#else
#error "unsupported compiler"
#endif

#ifdef _MSC_VER
#include <intrin.h>
#define mm_bswap_16( i ) _byteswap_ushort( i )
#define mm_bswap_32( i ) _byteswap_ulong( i )
#define mm_bswap_64( i ) _byteswap_uint64( i )
#elif ( __GNUC__ > 4 )\
   || ( __GNUC__ == 4 && __GNUC__MINOR__ >= 8 )\
   || MM_HAS_BUILTIN( __builtin_bswap16 )
#define mm_bswap_16( i ) __builtin_bswap16( i )
#define mm_bswap_32( i ) __builtin_bswap32( i )
#define mm_bswap_64( i ) __builtin_bswap64( i )
#else
static inline uint_least16_t mm_bswap_16( uint_least16_t i ) {
	return ( ( i >> 8 ) & 0xFF )
	     | ( ( i & 0xFF ) << 8 );
}

static inline uint_least32_t mm_bswap_32( uint_least32_t i ) {
	return ( ( i & 0xFF000000 ) >> 24 )
	     | ( ( i & 0x00FF0000 ) >> 8 )
	     | ( ( i & 0x0000FF00 ) << 8 )
	     | ( ( i & 0x000000FF ) << 24 );
}

static inline uint_least64_t mm_bswap_64( uint_least64_t i ) {
	return ( ( i & 0xFF00000000000000 ) >> 56 )
	     | ( ( i & 0x00FF000000000000 ) >> 40 )
	     | ( ( i & 0x0000FF0000000000 ) >> 24 )
	     | ( ( i & 0x000000FF00000000 ) >> 8 )
	     | ( ( i & 0x00000000FF000000 ) << 8 )
	     | ( ( i & 0x0000000000FF0000 ) << 24 )
	     | ( ( i & 0x000000000000FF00 ) << 40 )
	     | ( ( i & 0x00000000000000FF ) << 56 );
}
#endif

#ifdef MM_LITTLE_ENDIAN
#define mm_host_to_net_16( i ) mm_bswap_16( i )
#define mm_host_to_net_32( i ) mm_bswap_32( i )
#define mm_host_to_net_64( i ) mm_bswap_64( i )

static inline float mm_host_to_net_float( float f ) {
	uint_least32_t i = mm_host_to_net_32( *( uint_least32_t* ) &f );
	return *( float* ) &i;
}

static inline float mm_host_to_net_double( double d ) {
	uint_least64_t i = mm_host_to_net_64( *( uint_least64_t* ) &d );
	return *( double* ) &i;
}

#define mm_net_to_host_16( i ) mm_host_to_net_16( i )
#define mm_net_to_host_32( i ) mm_host_to_net_32( i )
#define mm_net_to_host_64( i ) mm_host_to_net_64( i )
#define mm_net_to_host_float( f ) mm_host_to_net_float( f )
#define mm_net_to_host_double( d ) mm_host_to_net_double( d )

#else
#define mm_host_to_net_16( i ) ( i )
#define mm_host_to_net_32( i ) ( i )
#define mm_host_to_net_64( i ) ( i )
#define mm_host_to_net_float( f ) ( f )
#define mm_host_to_net_double( d ) ( d )
#define mm_net_to_host_16( i ) ( i )
#define mm_net_to_host_32( i ) ( i )
#define mm_net_to_host_64( i ) ( i )
#define mm_net_to_host_float( f ) ( f )
#define mm_net_to_host_double( d ) ( d )
#endif

#if __STDC_VERSION__ >= 201112L
#define mm_host_to_net( i ) _Generic( ( i ),\
		unsigned short: mm_host_to_net_16,\
		unsigned int: mm_host_to_net_16,\
		unsigned long: mm_host_to_net_32,\
		unsigned long long: mm_host_to_net_64,\
		float: mm_host_to_net_float,\
		double: mm_host_to_net_double\
	)( i )

#define mm_net_to_host( i ) _Generic( ( i ),\
		unsigned short: mm_net_to_host_16,\
		unsigned int: mm_net_to_host_16,\
		unsigned long: mm_net_to_host_32,\
		unsigned long long: mm_net_to_host_64\
		float: mm_net_to_host_float,\
		double: mm_net_to_host_double\
	)( i )
#endif

#endif
