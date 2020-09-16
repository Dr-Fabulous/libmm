#ifndef LIBMM_RBTREE_H
#define LIBMM_RBTREE_H
#include "libmm/list.h"

typedef enum rbtree_colour {
	RB_RED,
	RB_BLACK
} rbtree_colour_t;

typedef struct rbtree_node {
	struct list_node list_node; // used for traversal, non reentrant but requires no additional memory
	struct rbtree_node *parent;
	struct rbtree_node *lhs;
	struct rbtree_node *rhs;
	enum rbtree_colour colour;
	void *key;
} rbtree_node_t;

typedef struct rbtree {
	struct rbtree_node *root;
	int ( *cmp )( void*, void* );
} rbtree_t;

#define RBTREE_INIT( name, cmp_f )\
	{ .root = NULL, .cmp = cmp_f }

#define RBTREE_HEAD( name, cmp_f )\
	struct rbtree name = RBTREE_INIT( name, cmp_f )

#endif
