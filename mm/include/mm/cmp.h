#ifndef MM_CMP_H
#define MM_CMP_H
#include "mm/common.h"

#define MM_CMP_X_TYPES\
	X( signed_char, signed char )\
	X( char, char )\
	X( short, short )\
	X( int, int )\
	X( long, long )\
	X( long_long, long long )\
	X( unsigned_char, unsigned char )\
	X( unsigned_short, unsigned short )\
	X( unsigned_int, unsigned int )\
	X( unsigned_long, unsigned long )\
	X( unsigned_long_long, unsigned long long )\
	X( float, float )\
	X( double, double )

#define MM_CMP_NAME( name )\
	mm_cmp_##name

#define X( name, type )\
	MM_API int MM_CMP_NAME( name )( const void *lhs, const void *rhs );

MM_CMP_X_TYPES
#undef X

#define MM_CMP( type ) _Generic( ( type ),\
		signed char: MM_CMP_NAME( signed char ),\
		char: MM_CMP_NAME( char ),\
		short: MM_CMP_NAME( short ),\
		int: MM_CMP_NAME( int ),\
		long: MM_CMP_NAME( long ),\
		long long: MM_CMP_NAME( long long ),\
		unsigned char: MM_CMP_NAME( unsigned char ),\
		unsigned short: MM_CMP_NAME( unsigned short ),\
		unsigned int: MM_CMP_NAME( unsigned int ),\
		unsigned long: MM_CMP_NAME( unsigned long ),\
		unsigned long long: MM_CMP_NAME( unsigned long long ),\
		float: MM_CMP_NAME( float ),\
		double: MM_CMP_NAME( double )\
	)

#endif
