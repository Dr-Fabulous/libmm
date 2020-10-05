#include "mm/cmp.h"

#define X( name, type )\
	int MM_CMP_NAME( name )( const void *lhs, const void *rhs ) {\
		return ( *( type* ) lhs ) - ( *( type* ) rhs );\
	}

MM_CMP_X_TYPES
#undef X
