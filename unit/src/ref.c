#include "mm/common.h"
#include "mm/ref.h"
#include "mm/unit.h"

struct state {
	struct mm_ref ref;
	bool destroyed;
};

static void state_construct( struct state * const this ) {
	mm_ref_construct( &this->ref );
	this->destroyed = false;
}

static void state_release( struct state * const this ) {
	if ( mm_ref_dec( &this->ref ) ) {
		this->destroyed = true;
	}
}

MM_UNIT_CASE( normal_case, NULL, NULL ) {
	struct state s;

	state_construct( &s );

	MM_UNIT_ASSERT_EQ( mm_ref_null( &s.ref ), false );
	MM_UNIT_ASSERT_EQ( mm_ref_count( &s.ref ), 0 );
	MM_UNIT_ASSERT_EQ( s.destroyed, false );

	mm_ref_inc( &s.ref );

	MM_UNIT_ASSERT_EQ( mm_ref_null( &s.ref ), false );
	MM_UNIT_ASSERT_EQ( mm_ref_count( &s.ref ), 1 );
	MM_UNIT_ASSERT_EQ( s.destroyed, false );

	state_release( &s );

	MM_UNIT_ASSERT_EQ( mm_ref_null( &s.ref ), true );
	MM_UNIT_ASSERT_EQ( mm_ref_count( &s.ref ), -1 );
	MM_UNIT_ASSERT_EQ( s.destroyed, true );

	return MM_UNIT_DONE;
}

MM_UNIT_SUITE( ref_suite ) {
	MM_UNIT_RUN( normal_case );

	return MM_UNIT_DONE;
}

int main( int argc, const char *argv[] ) {
	MM_UNIT_RUN_SUITE( ref_suite );
	return 0;
}
