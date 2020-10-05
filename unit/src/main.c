#include "mm/common.h"
#include "mm/unit.h"

MM_UNIT_IMPORT( co_suite );
MM_UNIT_IMPORT( random_suite );
MM_UNIT_IMPORT( vector_suite );

int main( int argc, const char *argv[] ) {
	MM_UNIT_RUN_SUITE( co_suite );
	MM_UNIT_RUN_SUITE( random_suite );
	MM_UNIT_RUN_SUITE( vector_suite );

	return EXIT_SUCCESS;
}
