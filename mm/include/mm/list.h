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
	size_t type_offset; // offset from node->bytes to properly aligned address
	struct mm_ilist head;
} mm_list_t;

#endif
