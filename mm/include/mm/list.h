#ifndef MM_LIST_H
#define MM_LIST_H
#include "mm/common.h"
#include "mm/ilist.h"

typedef struct mm_list_node {
	struct mm_ilist pos;
	unsigned char bytes[];
} mm_list_node_t;

typedef struct mm_list {
	size_t type_size;
	size_t type_align;
	struct mm_ilist head;
} mm_list_t;

#define MM_LIST_INIT( name, type )\
	{ .type_size = sizeof( type ), .type_align = alignof( type ), .head = MM_ILIST_INIT( head ) }

#define MM_LIST_DECLARE( name, type )\
	struct mm_list name = MM_LIST_INIT( name, type )

#endif
