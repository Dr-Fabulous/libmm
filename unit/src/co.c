#include "libmm/unit.h"
#include "libmm/co.h"

struct counter_state {
	struct co co;
	int i;
};

static enum co_state counter( struct counter_state *this ) {
	co_begin( &this->co );

	for ( this->i = 0; this->i < 5; ++this->i ) {
		co_yield( &this->co );
	}

	co_end( &this->co );
}

UNIT_CASE( counter_case, NULL, NULL ) {
	struct counter_state st = { 0 };
	int i = 0;

	while( co_resume( counter( &st ) ) ) {
		UNIT_ASSERT( i++ == st.i, "increment values not equal" );
	}

	return UNIT_DONE;
}

UNIT_SUITE( co_suite ) {
	UNIT_RUN( counter_case );
	return UNIT_DONE;
}
