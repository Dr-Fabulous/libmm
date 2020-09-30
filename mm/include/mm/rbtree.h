#ifndef MM_RBTREE_H
#define MM_RBTREE_H
#include "mm/list.h"

typedef enum mm_rbtree_colour {
	MM_RB_RED,
	MM_RB_BLACK
} mm_rbtree_colour_t;

typedef struct mm_rbtree_node {
	struct mm_list_node list_node; // used for traversal, non reentrant but requires no additional memory
	struct mm_rbtree_node *parent;
	struct mm_rbtree_node *lhs;
	struct mm_rbtree_node *rhs;
	enum mm_rbtree_colour colour;
	void *key;
} mm_rbtree_node_t;

typedef struct mm_rbtree {
	struct mm_rbtree_node *root;
	int ( *cmp )( void*, void* );
} mm_rbtree_t;

#define RBTREE_INIT( name, cmp_f )\
	{ .root = NULL, .cmp = cmp_f }

#define RBTREE_HEAD( name, cmp_f )\
	struct rbtree name = RBTREE_INIT( name, cmp_f )

#endif
