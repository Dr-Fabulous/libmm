#include "mm/unicode.h"
#include <string.h>

#include <stdio.h>
// ( value, mask, max )
#define UTF8_ONE ( 0x0, 0x80, 0x7f )
#define UTF8_TWO ( 0xc0, 0xe0, 0x7ff )
#define UTF8_THREE ( 0xe0, 0xf0, 0xffff )
#define UTF8_FOUR ( 0xf0, 0xf8, 0x10ffff )
#define UTF8_TRAILING ( 0x80, 0xc0, 0x0 )

#define UTF8_IS( str, tuple )\
	( ( ( *( unsigned char* ) ( str ) ) & MM_GET( tuple, 1 ) ) == MM_GET( tuple, 0 ) )

#define UTF8_MAX( tuple )\
	MM_GET( tuple, 2 )

char* mm_utf8_next( char const *str, char const *end ) {
	if ( UTF8_IS( str, UTF8_ONE ) ) {
		++str;
	} else if ( UTF8_IS( str, UTF8_TWO ) ) {
		str += 2;
	} else if ( UTF8_IS( str, UTF8_THREE ) ) {
		str += 3;
	} else if ( UTF8_IS( str, UTF8_FOUR ) ) {
		str += 4;
	} else if ( UTF8_IS( str, UTF8_TRAILING ) ) {
		do {
			++str;
		} while( str < end && UTF8_IS( str, UTF8_TRAILING ) );
	} else {
		str = end;
	}

	if ( ( UTF8_IS( str, UTF8_ONE ) && str <= ( end - 1 ) )
	||   ( UTF8_IS( str, UTF8_TWO ) && str <= ( end - 2 ) )
	||   ( UTF8_IS( str, UTF8_THREE ) && str <= ( end - 3 ) )
	||   ( UTF8_IS( str, UTF8_FOUR ) && str <= ( end - 4 ) ) )
	{
		return ( char* ) str;
	} else {
		return NULL;
	}
}

char* mm_utf8_prev( char const *str, char const *start ) {
	--str;

	while( str >= start && UTF8_IS( str, UTF8_TRAILING ) ) {
		--str;
	}

	if ( str >= start &&
	    ( UTF8_IS( str, UTF8_ONE )
	   || UTF8_IS( str, UTF8_TWO )
	   || UTF8_IS( str, UTF8_THREE )
	   || UTF8_IS( str, UTF8_FOUR ) ) )
	{
		return ( char* ) str;
	} else {
		return NULL;
	}
}

char const* mm_utf8_invalid( char const *str, size_t bytes ) {
	size_t trailing = 0;

	for (; bytes && *str != '\0'; ++str ) {
		if ( UTF8_IS( str, UTF8_ONE ) ) {
			if ( trailing ) {
				return str;
			}

			trailing = 0;
		} else if ( UTF8_IS( str, UTF8_TWO ) ) {
			if ( trailing ) {
				return str;
			}

			trailing = 1;
		} else if ( UTF8_IS( str, UTF8_THREE ) ) {
			if ( trailing ) {
				return str;
			}

			trailing = 2;
		} else if ( UTF8_IS( str, UTF8_FOUR ) ) {
			if ( trailing ) {
				return str;
			}

			trailing = 3;
		} else if ( UTF8_IS( str, UTF8_TRAILING ) ) {
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


size_t mm_utf8_byte_len( char const *str, size_t max ) {
	return strnlen( str, max );
}

size_t mm_utf8_code_len( char const *str, size_t bytes ) {
	size_t len = 0;

	MM_UTF8_FOREACH( start, end, pos, str, bytes ) {
		++len;
	}

	return len;
}

int mm_utf8_cmp( char const * restrict lhs, char const * restrict rhs, size_t bytes ) {
	return strncmp( lhs, rhs, bytes );
}

char32_t const* mm_utf32_invalid( char32_t const *str, size_t size ) {
	for(; *str && size; ++str, --size ) {
		if ( *str > UTF8_MAX( UTF8_FOUR ) ) {
			return str;
		}
	}

	return NULL;
}


size_t mm_utf32_byte_len( char32_t const *str, size_t max ) {
	size_t len = 0;

	for (; *str && max; ++str, max -= 4 ) {
		len += sizeof( *str );
	}

	return len;
}

size_t mm_utf32_code_len( char32_t const *str, size_t size ) {
	size_t len = 0;

	for (; *str && size; ++str, --size ) {
		++len;
	}

	return len;
}

size_t mm_utf32_to_utf8_len( char32_t const *str, size_t size ) {
	size_t len = 0;

	for (; *str && size; ++str, --size ) {
		if ( *str <= UTF8_MAX( UTF8_ONE ) ) {
			++len;
		} else if ( *str <= UTF8_MAX( UTF8_TWO ) ) {
			len += 2;
		} else if ( *str <= UTF8_MAX( UTF8_THREE ) ) {
			len += 3;
		} else if ( *str <= UTF8_MAX( UTF8_FOUR ) ) {
			len += 4;
		} else {
			return 0;
		}
	}

	return len;
}

int mm_utf32_cmp( char32_t const * restrict lhs, char32_t const * restrict rhs, size_t max ) {
	for (; *lhs == *rhs && max; ++lhs, ++rhs, --max );
	return *lhs - *rhs;
}

int mm_c8_to_c32( char32_t * restrict dst, char const * restrict _src, size_t bytes ) {
	unsigned char const *src = ( unsigned char const* ) _src;

	if ( !*src ) {
		return 0;
	} else if ( UTF8_IS( src, UTF8_ONE ) ) {
		if ( bytes < 1 ) {
			return 0;
		}

		*dst = *src;

		return 1;
	} else if ( UTF8_IS( src, UTF8_TWO ) ) {
		if ( bytes < 2 ) {
			return 0;
		}

		*dst = ( ( ( char32_t ) src[ 0 ] & CHAR32_C( 0x1f ) ) << 6 )
		     | ( ( ( char32_t ) src[ 1 ] & CHAR32_C( 0x3f ) ) );

		return 2;
	} else if ( UTF8_IS( src, UTF8_THREE ) ) {
		if ( bytes < 3 ) {
			return 0;
		}

		*dst = ( ( ( char32_t ) src[ 0 ] & CHAR32_C( 0xf ) ) << 12 )
		     | ( ( ( char32_t ) src[ 1 ] & CHAR32_C( 0x3f ) ) << 6 )
		     | ( ( char32_t ) src[ 2 ] & CHAR32_C( 0x3f ) );

		return 3;
	} else if ( UTF8_IS( src, UTF8_FOUR ) ) {
		if ( bytes < 4 ) {
			return 0;
		}

		*dst = ( ( ( char32_t ) src[ 0 ] & CHAR32_C( 0x7 ) ) << 18 )
		     | ( ( ( char32_t ) src[ 1 ] & CHAR32_C( 0x3f ) ) << 12 )
		     | ( ( ( char32_t ) src[ 2 ] & CHAR32_C( 0x3f ) ) << 6 )
		     | ( ( char32_t ) src[ 3 ] & CHAR32_C( 0x3f ) );

		return 4;
	} 

	return -1;
}

int mm_c32_to_c8( char * restrict _dst, char32_t const * restrict src, size_t bytes ) {
	unsigned char *dst = ( unsigned char* ) _dst;

	if ( !*src ) {
		return 0;
	} else if ( *src <= UTF8_MAX( UTF8_ONE ) ) {
		if ( !bytes ) {
			return 0;
		}

		dst[ 0 ] = ( unsigned char ) *src;

		return 1;
	} else if ( *src <= UTF8_MAX( UTF8_TWO ) ) {
		if ( bytes < 2 ) {
			return 0;
		}

		dst[ 0 ] = UCHAR_C( 0xc0 ) | ( unsigned char ) ( *src >> 6 );
		dst[ 1 ] = UCHAR_C( 0x80 ) | ( unsigned char ) ( *src & UCHAR_C( 0x3f ) );

		return 2;
	} else if ( *src <= UTF8_MAX( UTF8_THREE ) ) {
		if ( bytes < 3 ) {
			return 0;
		}

		dst[ 0 ] = UCHAR_C( 0xe0 ) | ( *src >> 12 );
		dst[ 1 ] = UCHAR_C( 0x80 ) | ( ( *src >> 6 ) & UCHAR_C( 0x3f ) );
		dst[ 2 ] = UCHAR_C( 0x80 ) | ( *src & UCHAR_C( 0x3f ) );

		return 3;
	} else if ( *src <= UTF8_MAX( UTF8_FOUR ) ) {
		if ( bytes < 4 ) {
			return 0;
		}

		dst[ 0 ] = UCHAR_C( 0xf0 ) | ( unsigned char ) ( *src >> 18 );
		dst[ 1 ] = UCHAR_C( 0x80 ) | ( ( unsigned char ) ( *src >> 12 ) & UCHAR_C( 0x3f ) );
		dst[ 2 ] = UCHAR_C( 0x80 ) | ( ( unsigned char ) ( *src >> 6 ) & UCHAR_C( 0x3f ) );
		dst[ 3 ] = UCHAR_C( 0x80 ) | ( ( unsigned char ) *src & UCHAR_C( 0x3f ) );

		return 4;
	}

	return -1;
}


bool mm_utf32_to_utf8( char * restrict dst, size_t dst_size, char32_t const * restrict src, size_t src_size ) {
	int written;

	while( dst_size > 1 && src_size && ( written = mm_c32_to_c8( dst, src, dst_size ) ) > 0 ) {
		dst += written;
		++src;
		dst_size -= written;
		--src_size;
	}

	*dst = '\0';

	return written >= 0;
}

bool mm_utf8_to_utf32( char32_t * restrict dst, size_t dst_size, char const * restrict src, size_t src_size ) {
	int read;

	while( dst_size > 1 && src_size && ( read = mm_c8_to_c32( dst, src, src_size ) ) > 0 ) {
		dst++;
		src += read;
		--dst_size;
		src_size -= read;
	}

	*dst = '\0';

	return read >= 0;
}
