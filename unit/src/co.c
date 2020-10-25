#include "mm/unit.h"
#include "mm/co.h"

struct counter_state {
	struct mm_co co;
	int i;
};

MM_COROUTINE( counter, struct counter_state *this ) {
	MM_CO_BEGIN( &this->co );

	for( this->i = 0; this->i < 5; ++this->i ) {
		MM_CO_YIELD( &this->co );
	}

	MM_CO_END( &this->co );
}

MM_UNIT_CASE( counter_case, NULL, NULL ) {
	struct counter_state st = { 0 };
	int i = 0;
	
	while( MM_CO_RESUME( counter( &st ) ) ) {
		MM_UNIT_ASSERT_EQ( i++, st.i );
	}

	return MM_UNIT_DONE;
}

MM_UNIT_SUITE( co_suite ) {
	MM_UNIT_RUN( counter_case );
	return MM_UNIT_DONE;
}

int main( int argc, const char *argv[] ) {
	MM_UNIT_RUN_SUITE( co_suite );
	return 0;
}
