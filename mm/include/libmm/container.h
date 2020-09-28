#ifndef LIBMM_CONTAINER_H
#define LIBMM_CONTAINER_H
#include <stddef.h>
#define CONTAINER_OF( ptr, type, member ) ( ( type* ) ( ( unsigned char* ) ( ptr ) - offsetof( type, member ) ) )
#endif
