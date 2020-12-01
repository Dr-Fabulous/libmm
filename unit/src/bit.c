#include "mm/bit.h"
#include "mm/unit.h"

static uintmax_t count_bits( uintmax_t i ) {
	uintmax_t c = 0;

	for (; i; ++c ) {
		i &= i - 1;
	}

	return c;
}

MM_UNIT_CASE( popcnt_case, NULL, NULL ) {
	for ( unsigned short i = 1; i < USHRT_MAX; ++i ) {
		MM_UNIT_ASSERT_EQ( count_bits( i ), mm_popcount_u16( i ) );
	}

	for ( unsigned long i = 1; i < USHRT_MAX; ++i ) {
		MM_UNIT_ASSERT_EQ( count_bits( i << 8 ), mm_popcount_u32( i << 8 ) );
	}

	for ( unsigned long long i = 1; i < USHRT_MAX; ++i ) {
		MM_UNIT_ASSERT_EQ( count_bits( i << 16 ), mm_popcount_u64( i << 16 ) );
	}

	return MM_UNIT_DONE;
}

static uintmax_t clz( uintmax_t i, uintmax_t n ) {
	uintmax_t c = 0;
	
	while( !( ( i >> n) & 1 ) ) {
		++c;
		--n;
	}

	return c;
}

MM_UNIT_CASE( clz_case, NULL, NULL ) {
	for ( unsigned short i = 1; i < USHRT_MAX; ++i ) {
		MM_UNIT_ASSERT_EQ( clz( i, 15 ), mm_clz_u16( i ) );
	}

	for ( unsigned long i = 1; i < USHRT_MAX; ++i ) {
		MM_UNIT_ASSERT_EQ( clz( i << 8, 31 ), mm_clz_u32( i << 8 ) );
	}

	for ( unsigned long long i = 1; i < USHRT_MAX; ++i ) {
		MM_UNIT_ASSERT_EQ( clz( i << 16, 63 ), mm_clz_u64( i << 16 ) );
	}

	return MM_UNIT_DONE;
}

static uintmax_t ctz( uintmax_t i ) {
	uintmax_t c = 0;
	
	while( !( i & 1 ) ) {
		i >>= 1;
		++c;
	}

	return c;
}

MM_UNIT_CASE( ctz_case, NULL, NULL ) {
	for ( unsigned short i = 1; i < USHRT_MAX; ++i ) {
		mm_log( MM_INFO, "%hu %hu %hu", i, ctz( i ), mm_ctz_u16( i ) );
		MM_UNIT_ASSERT_EQ( ctz( i ), mm_ctz_u16( i ) );
	}

	for ( unsigned long i = 1; i < USHRT_MAX; ++i ) {
		MM_UNIT_ASSERT_EQ( ctz( i << 8 ), mm_ctz_u32( i << 8 ) );
	}

	for ( unsigned long long i = 1; i < USHRT_MAX; ++i ) {
		MM_UNIT_ASSERT_EQ( ctz( i << 16 ), mm_ctz_u64( i << 16 ) );
	}

	return MM_UNIT_DONE;
}

MM_UNIT_SUITE( bit_suite ) {
	MM_UNIT_RUN( popcnt_case );
	MM_UNIT_RUN( clz_case );
	MM_UNIT_RUN( ctz_case );

	return MM_UNIT_DONE;
}

int main( int argc, const char *argv[] ) {
	MM_UNIT_RUN_SUITE( bit_suite );
	return 0;
}
