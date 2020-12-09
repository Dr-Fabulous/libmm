#ifndef MM_ENDIAN_H
#define MM_ENDIAN_H
#include "mm/common.h"

#if MM_GCC_HAS_INCLUDE( <endian.h> )
#include <endian.h>
#elif MM_GCC_HAS_INCLUDE( <machine/endian.h> )
#include <machine/endian.h>
#elif MM_GCC_HAS_INCLUDE( <sys/param.h> )
#include <sys/param.h>
#elif MM_GCC_HAS_INCLUDE( <sys/isadefs.h> )
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
#error "could not figure out endian order"
#endif

#ifdef MM_USE_BUILTIN && MM_USING_MSVC
#include <stdlib.h>
#define mm_bswap_u16( i ) _byteswap_ushort( i )
#define mm_bswap_u32( i ) _byteswap_ulong( i )
#define mm_bswap_u64( i ) _byteswap_uint64( i )
#elif MM_USE_BUILTIN && ( MM_GCC_HAS_BUILTIN( __builtin_bswap16 ) || MM_GCC_IS_VERSION_OR_LATER( 4, 8, 0 ) )
#define mm_bswap_u16( i ) __builtin_bswap16( i )
#define mm_bswap_u32( i ) __builtin_bswap32( i )
#define mm_bswap_u64( i ) __builtin_bswap64( i )
#else
static inline uint16_t mm_bswap_u16( uint16_t i ) {
	return ( ( i >> 8 ) & 0xFF )
	     | ( ( i & 0xFF ) << 8 );
}

static inline uint32_t mm_bswap_u32( uint32_t i ) {
	return ( ( i & 0xFF000000 ) >> 24 )
	     | ( ( i & 0x00FF0000 ) >> 8 )
	     | ( ( i & 0x0000FF00 ) << 8 )
	     | ( ( i & 0x000000FF ) << 24 );
}

static inline uint64_t mm_bswap_u64( uint64_t i ) {
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
#define mm_host_to_net_u16( i ) mm_bswap_u16( i )
#define mm_host_to_net_u32( i ) mm_bswap_u32( i )
#define mm_host_to_net_u64( i ) mm_bswap_u64( i )

static inline float mm_host_to_net_float( float f ) {
	uint32_t i = mm_host_to_net_u32( *( uint32_t* ) &f );
	return *( float* ) &i;
}

static inline double mm_host_to_net_double( double d ) {
	uint64_t i = mm_host_to_net_u64( *( uint64_t* ) &d );
	return *( double* ) &i;
}

#define mm_net_to_host_u16( i ) mm_host_to_net_u16( i )
#define mm_net_to_host_u32( i ) mm_host_to_net_u32( i )
#define mm_net_to_host_u64( i ) mm_host_to_net_u64( i )
#define mm_net_to_host_float( f ) mm_host_to_net_float( f )
#define mm_net_to_host_double( d ) mm_host_to_net_double( d )

#else
#define mm_host_to_net_u16( i ) ( i )
#define mm_host_to_net_u32( i ) ( i )
#define mm_host_to_net_u64( i ) ( i )
#define mm_host_to_net_float( f ) ( f )
#define mm_host_to_net_double( d ) ( d )
#define mm_net_to_host_u16( i ) ( i )
#define mm_net_to_host_u32( i ) ( i )
#define mm_net_to_host_u64( i ) ( i )
#define mm_net_to_host_float( f ) ( f )
#define mm_net_to_host_double( d ) ( d )
#endif

/** \def Generically swap bytes from host order to network order ( big endian ) */
#if MM_C_STD >= 11
#define mm_host_to_net( i ) _Generic( ( i ),\
		uint16_t: mm_host_to_net_u16,\
		uint32_t: mm_host_to_net_u32,\
		uint64_t: mm_host_to_net_u64,\
		float: mm_host_to_net_float,\
		double: mm_host_to_net_double\
	)( i )

/** \def Generically swap bytes from network order ( big endian ) to host order */
#define mm_net_to_host( i ) _Generic( ( i ),\
		uint16_t: mm_net_to_host_u16,\
		uint32_t: mm_net_to_host_u32,\
		uint64_t: mm_net_to_host_u64\
		float: mm_net_to_host_float,\
		double: mm_net_to_host_double\
	)( i )
#endif

#endif
