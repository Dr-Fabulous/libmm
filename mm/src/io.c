#include "mm/io.h"
#include "mm/endian.h"

int mm_fputb( FILE * restrict f, void const * restrict buf, size_t bytes ) {
	unsigned int c;

	MM_BYTE_FOREACH( start, end, pos, buf, bytes ) {
		if ( pos != start ) {
			if ( fputc( ' ', f ) == EOF ) {
				return EOF;
			}
		}

		for ( int i = 7; i >= 0; --i ) {
			c = ( *pos >> i ) & 0x1 ? '1' : '0';

			if ( fputc( c, f ) == EOF ) {
				return EOF;
			}
		}
	}

	return 0;
}

int mm_fputb_hi( FILE * restrict f, void const * restrict buf, size_t bytes ) {
	unsigned int c;

	MM_BYTE_FOREACH_HI( start, end, pos, buf, bytes ) {
		if ( pos != start ) {
			if ( fputc( ' ', f ) == EOF ) {
				return EOF;
			}
		}

		for ( int i = 7; i >= 0; --i ) {
			c = ( *pos >> i ) & 0x1 ? '1' : '0';

			if ( fputc( c, f ) == EOF ) {
				return EOF;
			}
		}
	}

	return 0;
}

int mm_sputb( char * restrict str, size_t size, void const * restrict buf, size_t bytes ) {
	if ( !size ) {
		return 0;
	}

	char *pos = str;
	char *end = str + size - 1;
	unsigned char const *byte = buf;

	MM_BYTE_FOREACH( bstart, bend, bpos, buf, bytes ) {
		if ( bpos != bstart ) {
			if ( pos == end ) {
				goto add_null;
			}

			*pos = ' ';
			++pos;
		}

		for ( int i = 7; i >= 0; --i ) {
			if ( pos == end ) {
				goto add_null;
			}

			*pos = ( *bpos >> i ) ? '1' : '0';
			++pos;
		}
	}

	add_null:
	*pos = '\0';

	return pos - str;
}

int mm_sputb_hi( char * restrict str, size_t size, void const * restrict buf, size_t bytes ) {
	if ( !size ) {
		return 0;
	}

	char *pos = str;
	char *end = str + size - 1;
	unsigned char const *byte = buf;

	MM_BYTE_FOREACH_HI( bstart, bend, bpos, buf, bytes ) {
		if ( bpos != bstart ) {
			if ( pos == end ) {
				goto add_null;
			}

			*pos = ' ';
			++pos;
		}

		for ( int i = 7; i >= 0; --i ) {
			if ( pos == end ) {
				goto add_null;
			}

			*pos = ( *bpos >> i ) ? '1' : '0';
			++pos;
		}
	}

	add_null:
	*pos = '\0';

	return pos - str;
}

long long mm_utf8_fgetc( FILE *f ) {
	char buf[ MM_UTF8_MAX ] = { 0 };

	if ( !fread( buf, 1, 1, f ) ) {
		return 0;
	}

	int len = mm_utf8_len( buf[ 0 ] );

	if ( len < 0 ) {
		return len;
	}

	if ( fread( buf + 1, 1, len, f ) != ( size_t ) len ) {
		return MM_UTF8_EOF;
	}

	char32_t c = 0;
	int ret = mm_utf8_to_utf32( &c, buf, len );

	if ( ret < 0 ) {
		return ret;
	}

	return ( long long ) c;
}

long long mm_utf8_fungetc( FILE* f, long long c ) {
	int len = mm_utf32_to_utf8_len( c );

	if ( len < 0 ) {
		return len;
	}

	if ( fseek( f, -len, SEEK_CUR ) ) {
		return MM_UTF8_EOF;
	}

	return c;
}

long long mm_utf8_fputc( FILE *f, long long c ) {
	int len = mm_utf32_to_utf8_len( c );

	if ( len < 0 ) {
		return len;
	}

	char32_t ch = c;
	char buf[ MM_UTF8_MAX ] = { 0 };
	int ret = mm_utf32_to_utf8( buf, &ch, len );

	if ( ret < 0 ) {
		return ret;
	}

	if ( fwrite( buf, 1, len, f ) != ( size_t ) len ) {
		return MM_UTF8_EOF;
	}

	return c;
}
