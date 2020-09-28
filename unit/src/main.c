#include "libmm/unit.h"

UNIT_IMPORT( co_suite );
UNIT_IMPORT( random_suite );

int main( int argc, const char *argv[] ) {
	UNIT_RUN_SUITE( co_suite );
	UNIT_RUN_SUITE( random_suite );

	return EXIT_SUCCESS;
}
