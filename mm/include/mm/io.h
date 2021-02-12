#ifndef MM_IO_H
#define MM_IO_H
#include "mm/common.h"
#include "mm/unicode.h"
#include <stdio.h>

MM_API int mm_fputb( FILE * restrict f, void const * restrict buf, size_t bytes );
MM_API int mm_fputb_hi( FILE * restrict f, void const * restrict buf, size_t bytes );
MM_API int mm_sputb( char * restrict str, size_t size, void const * restrict buf, size_t bytes );
MM_API int mm_sputb_hi( char * restrict str, size_t size, void const * restrict buf, size_t bytes );

#define mm_putb( buf, bytes ) mm_fputb( stdout, buf, bytes )
#define mm_putb_hi( buf, bytes ) mm_fputb_hi( stdout, buf, bytes )

#define MM_UTF8_EOF INT64_C( -0x100000000 )

MM_API long long mm_utf8_fgetc( FILE *f );
MM_API long long mm_utf8_fungetc( FILE* f, long long c );
MM_API long long mm_utf8_fputc( FILE *f, long long c );

#endif
