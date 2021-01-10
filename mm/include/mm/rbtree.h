#ifndef MM_RBTREE_H
#define MM_RBTREE_H
#include "mm/common.h"
#include "mm/list.h"

typedef enum mm_rbcolour {
	MM_RB_RED,
	MM_RB_BLACK
} mm_rbcolour_t;

typedef struct mm_rbnode {
	struct mm_list pos;
	struct mm_rbnode *parent;
	struct mm_rbnode *lhs;
	struct mm_rbnode *rhs;
	enum mm_rbcolour colour;
	const void *key;
} mm_rbnode_t;

typedef enum mm_rbfind {
	MM_RB_FIND_EQUAL,
	MM_RB_FIND_LESS_EQUAL,
	MM_RB_FIND_GREATER_EQUAL
} mm_rbfind_t;

typedef int ( *mm_rbcmp_t )( const void*, const void* );

typedef struct mm_rbtree {
	struct mm_rbnode *root;
	struct mm_list head;
	mm_rbcmp_t cmp;
} mm_rbtree_t;

#define MM_RBTREE_INIT( name, _cmp )\
	{ .root = NULL,\
	  .head = MM_LIST_INIT( ( name ).head ),\
	  .cmp = _cmp }

#define MM_RBTREE_DECLARE( name, allow_dups, cmp )\
	struct mm_rbtree name = MM_RBTREE_INIT( name, allow_dups, cmp )

static inline void mm_rbtree_init( struct mm_rbtree *this, mm_rbcmp_t cmp ) {
	*this = ( struct mm_rbtree ) MM_RBTREE_INIT( *this, cmp );
}

static inline bool mm_rbtree_empty( struct mm_rbtree *this ) {
	return !this->root;
}

MM_API struct mm_rbnode* mm_rbtree_find( struct mm_rbtree *this, enum mm_rbfind f_type, const void *key );
MM_API bool mm_rbtree_insert( struct mm_rbtree *this, struct mm_rbnode *node, const void *key );
MM_API void mm_rbtree_remove( struct mm_rbtree *this, struct mm_rbnode *node );
MM_API bool mm_rbtree_remove_key( struct mm_rbtree *this, const void *key );

#define MM_RBTREE_NODE_CONTAINER( node, type, mem )\
	MM_COND_CONTAINER_OF( node, type, mem )

#define MM_RBTREE_FIND_CONTAINER( tree, f_type, key, type, mem )\
	MM_RBTREE_NODE_CONTAINER( mm_rbtree_find( tree, f_type, key ), type, mem )

#define MM_RBTREE_FIRST_NODE( tree )\
	MM_LIST_FIRST_CONTAINER( &( tree )->head, struct mm_rbnode, pos )

#define MM_RBTREE_LAST_NODE( tree )\
	MM_LIST_LAST_CONTAINER( &( tree )->head, struct mm_rbnode, pos )

#define MM_RBTREE_NEXT_NODE( pos )\
	MM_LIST_NEXT_CONTAINER( &( pos )->pos, struct mm_rbnode, pos )

#define MM_RBTREE_PREV_NODE( pos )\
	MM_LIST_PREV_CONTAINER( &( pos )->pos, struct mm_rbnode, pos )

#define MM_RBTREE_FIRST_CONTAINER( tree, type, mem )\
	MM_CONTAINER_OF( MM_RBTREE_FIRST_NODE( tree ), type, mem )

#define MM_RBTREE_LAST_CONTAINER( tree, type, mem )\
	MM_CONTAINER_OF( MM_RBTREE_LAST_NODE( tree ), type, mem )

#define MM_RBTREE_NEXT_CONTAINER( pos, type, mem )\
	MM_CONTAINER_OF( MM_RBTREE_NEXT_NODE( pos ), type, mem )

#define MM_RBTREE_PREV_CONTAINER( pos, type, mem )\
	MM_CONTAINER_OF( MM_RBTREE_PREV_NODE( pos ), type, mem )

#define MM_RBTREE_FOREACH( tree, pos )\
	MM_LIST_FOREACH( &( tree )->head, pos )

#define MM_RBTREE_FOREACH_REVERSE( tree, pos )\
	MM_LIST_FOREACH_REVERSE( &( tree )->head, pos )

#define MM_RBTREE_FOREACH_SAFE( tree, pos, tmp )\
	MM_LIST_FOREACH_SAFE( &( tree )->head, pos, tmp )

#define MM_RBTREE_FOREACH_REVERSE_SAFE( tree, pos, tmp )\
	MM_LIST_FOREACH_REVERSE_SAFE( &( tree )->head, pos, tmp )

#define MM_RBTREE_FOREACH_CONTAINER( tree, pos, type, mem )\
	MM_LIST_FOREACH_CONTAINER( &( tree )->head, pos, type, mem )

#define MM_RBTREE_FOREACH_CONTAINER_REVERSE( tree, pos, type, mem )\
	MM_LIST_FOREACH_CONTAINER_REVERSE( &( tree )->head, pos, type, mem )

#define MM_RBTREE_FOREACH_CONTAINER_REVERSE_SAFE( tree, pos, tmp, type, mem )\
	MM_LIST_FOREACH_CONTAINER_REVERSE_SAFE( &( tree )->head, pos, tmp, type, mem )

#define MM_RBTREE_DO_REMOVE_ALL( tree, pos, tmp, get_first, get_next )\
	for( ( pos ) = get_first,\
	     ( tmp ) = get_next,\
	     ( tree )->root = NULL,\
	     mm_list_construct( &( tree )->head );\
	     ( tree )->size;\
	     ( pos ) = ( tmp ),\
	     ( tmp ) = get_next,\
	     --( tree )->size )

/*!
	\breif Remove all nodes from an mm_rbtree without performing any balancing.
	
	This is faster than using a MM_RBTREE_FOREACH_SAFE() that calls mm_rbtree_remove()
	\param tree mm_rbtree to clear
	\param pos pointer to mm_rbnode
	\param tmp pointer to mm_rbnode
	\param
*/
#define MM_RBTREE_REMOVE_ALL( tree, pos, tmp )\
	MM_RBTREE_DO_REMOVE_ALL( tree,\
				 pos,\
				 tmp,\
				 MM_RBTREE_FIRST_NODE( tree ),\
				 MM_RBTREE_NEXT_NODE( pos ) )

/*!
	\breif Remove all nodes from an mm_rbtree without performing any balancing.
	
	This is faster than using a MM_RBTREE_FOREACH_CONTAINER_SAFE() that calls mm_rbtree_remove()
	\param tree mm_rbtree to clear
	\param pos pointer to container type
	\param tmp pointer to container type
	\param type type that contains an mm_rbnode
	\param mem name of the mm_rbnode member
*/
#define MM_RBTREE_REMOVE_ALL_CONTAINERS( tree, pos, tmp, type, mem )\
	MM_RBTREE_DO_REMOVE_ALL( tree,\
				 pos,\
				 tmp,\
				 MM_RBTREE_FIRST_CONTAINER( tree, type mem ),\
				 MM_RBTREE_NEXT_CONTAINER( pos, type, mem ) )

#endif
