#ifndef MM_UNICODE_H
#define MM_UNICODE_H
#include "mm/common.h"

#if __STDC_VERSION__ >= 201112L && defined( __STDC_UTF32__ )
#include <uchar.h>
#else
typedef uint_least32_t char32_t;
#endif

#define UCHAR_C UINT8_C
#define CHAR32_C UINT32_C
#define MM_UTF8_MAX 4
#define MM_UTF8_INVALID -1

MM_API char* mm_utf8_next( char const *str, char const *end );
MM_API char* mm_utf8_prev( char const *str, char const *start );
MM_API char const* mm_utf8_skip_bom( char const *str, size_t bytes );
MM_API char const* mm_utf8_find_invalid( char const *str, size_t bytes );
MM_API char const* mm_utf8_find_invalid_ascii( char const *str, size_t bytes );
MM_API int mm_utf8_cmp( char const * restrict lhs, char const * restrict rhs, size_t bytes );

MM_API int mm_utf8_len( char const c );
MM_API int mm_utf32_to_utf8_len( char32_t const c );
MM_API int mm_utf32_valid( char32_t c );

MM_API int mm_utf8_to_utf32( char32_t * restrict dst, char const * restrict _src, size_t bytes );
MM_API int mm_utf32_to_utf8( char * restrict _dst, char32_t const * restrict src, size_t bytes );
MM_API long long mm_utf32s_to_utf8s( char * restrict dst, size_t dst_size, char32_t const * restrict src, size_t src_size );
MM_API long long mm_utf8s_to_utf32s( char32_t * restrict dst, size_t dst_size, char const * restrict src, size_t src_size );

#define MM_UTF8_FOREACH( start, end, pos, str, bytes )\
	for( char const *start = str, *end = start + ( bytes ), *pos = start;\
	     pos;\
	     pos = mm_utf8_next( pos, end ) )

#define MM_UTF8_FOREACH_REVERSE( start, end, pos, str, bytes )\
	for( char const *start = ( str ) + ( bytes ) - 2, *end = ( str ), *pos = start;\
	     pos;\
	     pos = mm_utf8_prev( pos, end ) )

#endif
