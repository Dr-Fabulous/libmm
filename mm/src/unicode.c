#include "mm/unicode.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define UTF8_1 ( UCHAR_C( 0x0 ), UCHAR_C( 0x80 ), CHAR32_C( 0x7f ) )
#define UTF8_2 ( UCHAR_C( 0xc0 ), UCHAR_C( 0xe0 ), CHAR32_C( 0x7ff ) )
#define UTF8_3 ( UCHAR_C( 0xe0 ), UCHAR_C( 0xf0 ), CHAR32_C( 0xffff ) )
#define UTF8_4 ( UCHAR_C( 0xf0 ), UCHAR_C( 0xf8), CHAR32_C( 0x10ffff ) )
#define UTF8_5 ( UCHAR_C( 0xf8 ), UCHAR_C( 0xfc ), CHAR32_C( 0x0 ) )
#define UTF8_6 ( UCHAR_C( 0xfc ), UCHAR_C( 0xfe ), CHAR32_C( 0x0 ) )
#define UTF8_T ( UCHAR_C( 0x80 ), UCHAR_C( 0xc0 ), CHAR32_C( 0x0 ) )

#define UTF8_PREFIX( tuple ) MM_GET( tuple, 0 )
#define UTF8_MASK( tuple ) MM_GET( tuple, 1 )
#define UTF8_MAX( tuple ) MM_GET( tuple, 2 )
#define UTF8_IS( tuple, byte ) ( ( ( *( unsigned char* ) ( byte ) ) & UTF8_MASK( tuple ) ) == UTF8_PREFIX( tuple ) )

char* mm_utf8_next( char const *str, char const *end ) {
	if ( str >= end ) {
		return NULL;
	} else if ( UTF8_IS( UTF8_1, str ) ) {
		++str;
	} else if ( UTF8_IS( UTF8_2, str ) ) {
		str += 2;
	} else if ( UTF8_IS( UTF8_3, str ) ) {
		str += 3;
	} else if ( UTF8_IS( UTF8_4, str ) ) {
		str += 4;
	} else if ( UTF8_IS( UTF8_T, str ) ) {
		do {
			++str;
		} while( str < end && UTF8_IS( UTF8_T, str ) );
	} else {
		str = end;
	}

	if ( ( UTF8_IS( UTF8_1, str ) && str <= ( end - 1 ) )
	||   ( UTF8_IS( UTF8_2, str ) && str <= ( end - 2 ) )
	||   ( UTF8_IS( UTF8_3, str ) && str <= ( end - 3 ) )
	||   ( UTF8_IS( UTF8_4, str ) && str <= ( end - 4 ) ) )
	{
		return ( char* ) str;
	} else {
		return NULL;
	}
}

char* mm_utf8_prev( char const *str, char const *start ) {
	if ( str < start ) {
		return NULL;
	}

	--str;

	while( str >= start && UTF8_IS( UTF8_T, str ) ) {
		--str;
	}

	if ( str >= start &&
	    ( UTF8_IS( UTF8_1, str )
	   || UTF8_IS( UTF8_2, str )
	   || UTF8_IS( UTF8_3, str )
	   || UTF8_IS( UTF8_4, str ) ) )
	{
		return ( char* ) str;
	} else {
		return NULL;
	}
}

char const* mm_utf8_skip_bom( char const *str, size_t bytes ) {
	unsigned char const *pos = ( unsigned char const* ) str;

	if ( bytes >= 3
	  && pos[ 0 ] == 0xef
	  && pos[ 1 ] == 0xbb
	  && pos[ 2 ] == 0xbf )
	{
		return str + 3;
	} else {
		return str;
	}
}

char const* mm_utf8_find_invalid( char const *str, size_t bytes ) {
	size_t trailing = 0;

	str = mm_utf8_skip_bom( str, bytes );

	for (str = mm_utf8_skip_bom( str, bytes ); bytes && *str != '\0'; ++str ) {
		if ( UTF8_IS( UTF8_1, str ) ) {
			if ( trailing ) {
				return str;
			}

			trailing = 0;
		} else if ( UTF8_IS( UTF8_2, str ) ) {
			if ( trailing ) {
				return str;
			}

			trailing = 1;
		} else if ( UTF8_IS( UTF8_3, str ) ) {
			if ( trailing ) {
				return str;
			}

			trailing = 2;
		} else if ( UTF8_IS( UTF8_4, str ) ) {
			if ( trailing ) {
				return str;
			}

			trailing = 3;
		} else if ( UTF8_IS( UTF8_T, str ) ) {
			if ( !trailing ) {
				return str;
			}

			--trailing;
		} else {
			return str;
		}
	}

	if ( trailing ) {
		return str;
	} else {
		return NULL;
	}
}

char const* mm_utf8_find_invalid_ascii( char const *str, size_t bytes ) {
	for (; *str != '\0' && bytes; ++str, --bytes ) {
		if ( !UTF8_IS( UTF8_1, str ) ) {
			return str;
		}
	}

	return NULL;
}

// normalize your strings!
int mm_utf8_cmp( char const * restrict lhs, char const * restrict rhs, size_t bytes ) {
	return strncmp( lhs, rhs, bytes );
}

int mm_utf32_valid( char32_t c ) {
	return c <= UTF8_MAX( UTF8_4 );
}

int mm_utf8_len( char const c ) {
	if ( UTF8_IS( UTF8_1, &c ) ) {
		return 1;
	} else if ( UTF8_IS( UTF8_2, &c ) ) {
		return 2;
	} else if ( UTF8_IS( UTF8_3, &c ) ) {
		return 3;
	} else if ( UTF8_IS( UTF8_4, &c ) ) {
		return 4;
	} else {
		return MM_UTF8_INVALID;
	}
}

int mm_utf32_to_utf8_len( char32_t const c ) {
	if ( c <= UTF8_MAX( UTF8_1 ) ) {
		return 1;
	} else if ( c <= UTF8_MAX( UTF8_2 ) ) {
		return 2;
	} else if ( c <= UTF8_MAX( UTF8_3 ) ) {
		return 3;
	} else if ( c <= UTF8_MAX( UTF8_4 ) ) {
		return 4;
	} else {
		return MM_UTF8_INVALID;
	}
}

int mm_utf8_to_utf32( char32_t * restrict dst, char const * restrict _src, size_t bytes ) {
	unsigned char const *src = ( unsigned char const* ) _src;

	if ( !*src ) {
		return 0;
	} else if ( UTF8_IS( UTF8_1, src ) ) {
		if ( bytes < 1 ) {
			return 0;
		}

		*dst = *src;

		return 1;
	} else if ( UTF8_IS( UTF8_2, src ) ) {
		if ( bytes < 2 ) {
			return 0;
		}

		*dst = ( ( ( char32_t ) src[ 0 ] & ~UTF8_MASK( UTF8_2 ) ) << 6 )
		     | ( ( ( char32_t ) src[ 1 ] & ~UTF8_MASK( UTF8_T ) ) );

		return 2;
	} else if ( UTF8_IS( UTF8_3, src ) ) {
		if ( bytes < 3 ) {
			return 0;
		}

		*dst = ( ( ( char32_t ) src[ 0 ] & ~UTF8_MASK( UTF8_3 ) ) << 12 )
		     | ( ( ( char32_t ) src[ 1 ] & ~UTF8_MASK( UTF8_T ) ) << 6 )
		     | ( ( char32_t ) src[ 2 ] & ~UTF8_MASK( UTF8_T ) );

		return 3;
	} else if ( UTF8_IS( UTF8_4, src ) ) {
		if ( bytes < 4 ) {
			return 0;
		}

		*dst = ( ( ( char32_t ) src[ 0 ] & ~UTF8_MASK( UTF8_4 ) ) << 18 )
		     | ( ( ( char32_t ) src[ 1 ] & ~UTF8_MASK( UTF8_T ) ) << 12 )
		     | ( ( ( char32_t ) src[ 2 ] & ~UTF8_MASK( UTF8_T ) ) << 6 )
		     | ( ( char32_t ) src[ 3 ] & ~UTF8_MASK( UTF8_T ) );

		return 4;
	} 

	return MM_UTF8_INVALID;
}

int mm_utf32_to_utf8( char * restrict _dst, char32_t const * restrict src, size_t bytes ) {
	unsigned char *dst = ( unsigned char* ) _dst;

	if ( !*src ) {
		return 0;
	} else if ( *src <= UTF8_MAX( UTF8_1 ) ) {
		if ( !bytes ) {
			return 0;
		}

		dst[ 0 ] = ( unsigned char ) *src;

		return 1;
	} else if ( *src <= UTF8_MAX( UTF8_2 ) ) {
		if ( bytes < 2 ) {
			return 0;
		}

		dst[ 0 ] = UTF8_PREFIX( UTF8_2 ) | ( unsigned char ) ( *src >> 6 );
		dst[ 1 ] = UTF8_PREFIX( UTF8_T ) | ( unsigned char ) ( *src & ~UTF8_MASK( UTF8_T ) );

		return 2;
	} else if ( *src <= UTF8_MAX( UTF8_3 ) ) {
		if ( bytes < 3 ) {
			return 0;
		}

		dst[ 0 ] = UTF8_PREFIX( UTF8_3 ) | ( unsigned char ) ( *src >> 12 );
		dst[ 1 ] = UTF8_PREFIX( UTF8_T ) | ( unsigned char ) ( ( *src >> 6 ) & ~UTF8_MASK( UTF8_T ) );
		dst[ 2 ] = UTF8_PREFIX( UTF8_T ) | ( unsigned char ) ( *src & ~UTF8_MASK( UTF8_T ) );

		return 3;
	} else if ( *src <= UTF8_MAX( UTF8_4 ) ) {
		if ( bytes < 4 ) {
			return 0;
		}

		dst[ 0 ] = UTF8_PREFIX( UTF8_4 ) | ( unsigned char ) ( *src >> 18 );
		dst[ 1 ] = UTF8_PREFIX( UTF8_T ) | ( ( unsigned char ) ( *src >> 12 ) & ~UTF8_MASK( UTF8_T ) );
		dst[ 2 ] = UTF8_PREFIX( UTF8_T ) | ( ( unsigned char ) ( *src >> 6 ) & ~UTF8_MASK( UTF8_T ) );
		dst[ 3 ] = UTF8_PREFIX( UTF8_T ) | ( ( unsigned char ) *src & ~UTF8_MASK( UTF8_T ) );

		return 4;
	}

	return MM_UTF8_INVALID;
}

long long mm_utf32s_to_utf8s( char * restrict dst, size_t dst_size, char32_t const * restrict src, size_t src_size ) {
	long long total = 0;
	int written;

	while( dst_size > 1 && src_size && ( written = mm_utf32_to_utf8( dst, src, dst_size ) ) > 0 ) {
		total += written;
		dst += written;
		++src;
		dst_size -= written;
		--src_size;
	}

	*dst = '\0';

	if ( written >= 0 ) {
		return total;
	} else {
		return MM_UTF8_INVALID;
	}
}

long long mm_utf8s_to_utf32s( char32_t * restrict dst, size_t dst_size, char const * restrict src, size_t src_size ) {
	long long total = 0;
	int read;

	while( dst_size > 1 && src_size && ( read = mm_utf8_to_utf32( dst, src, src_size ) ) > 0 ) {
		total += read;
		dst++;
		src += read;
		--dst_size;
		src_size -= read;
	}

	*dst = '\0';

	if ( read >= 0 ) {
		return total;
	} else {
		return MM_UTF8_INVALID;
	}
}
