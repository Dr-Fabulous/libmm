#include "mm/rbtree.h"

static inline void mm_rbtree_find_nearest( struct mm_rbtree *this, const void *key, int *diff, struct mm_rbnode **node ) {
	*diff = 0;
	*node = NULL;

	mm_rbcmp_t cmp = this->cmp;
	struct mm_rbnode *next = this->root;

	while( next ) {
		*node = next;
		*diff = cmp( key, ( *node )->key );

		if ( *diff < 0 ) {
			next = ( *node )->lhs;
		} else if ( *diff > 0 ) {
			next = ( *node )->rhs;
		} else {
			next = NULL;
		}
	}
}

struct mm_rbnode* mm_rbtree_find( struct mm_rbtree *this, enum mm_rbfind f_type, const void *key ) {
	int diff;
	struct mm_rbnode *node;

	mm_rbtree_find_nearest( this, key, &diff, &node );

	switch( f_type ) {
		case MM_RB_FIND_EQUAL: return !diff ? node : NULL;
		case MM_RB_FIND_LESS_EQUAL: return diff <= 0 ? node : NULL;
		case MM_RB_FIND_GREATER_EQUAL: return diff >= 0 ? node : NULL;
		default: return NULL;
	}
}

static void mm_rbnode_init( struct mm_rbnode *this, const void *key ) {
	mm_list_init( &this->pos );
	this->parent = NULL;
	this->lhs = NULL;
	this->rhs = NULL;
	this->colour = MM_RB_RED;
	this->key = key;
}

/*
	the code below is using the Wikipedia entry for rbtrees as a reference.
*/

static inline void mm_rbtree_rotate_left( struct mm_rbtree *this, struct mm_rbnode *node ) {
	struct mm_rbnode *parent = node->parent;
	struct mm_rbnode *tmp = node->rhs;

	node->rhs = tmp->lhs;

	if ( node->rhs ) {
		node->rhs->parent = node;
	}

	node->parent = tmp;
	tmp->lhs = node;
	tmp->parent = parent;

	if ( parent ) {
		if ( parent->lhs == node ) {
			parent->lhs = tmp;
		} else {
			parent->rhs = tmp;
		}
	} else {
		this->root = tmp;
	}
}

static inline void mm_rbtree_rotate_right( struct mm_rbtree *this, struct mm_rbnode *node ) {
	struct mm_rbnode *parent = node->parent;
	struct mm_rbnode *tmp = node->lhs;

	node->lhs = tmp->rhs;

	if ( node->lhs ) {
		node->lhs->parent = node;
	}

	node->parent = tmp;
	tmp->rhs = node;
	tmp->parent = parent;

	if ( parent ) {
		if ( parent->lhs == node ) {
			parent->lhs = tmp;
		} else {
			parent->rhs = tmp;
		}
	} else {
		this->root = tmp;
	}
}

static void mm_rbtree_insert_fix( struct mm_rbtree *this, struct mm_rbnode *node ) {
	struct mm_rbnode *parent;
	struct mm_rbnode *grandparent;
	struct mm_rbnode *uncle;

	for( ;; ) {
		parent = node->parent;

		if ( !parent ) {
			node->colour = MM_RB_BLACK;
			return;
		} else if ( parent->colour == MM_RB_BLACK ) {
			return;
		}

		grandparent = parent->parent;
		uncle = ( grandparent->rhs == parent ) ? grandparent->lhs : grandparent->rhs;

		if ( !uncle || uncle->colour == MM_RB_BLACK ) {
			if ( node == parent->rhs && parent == grandparent->lhs ) {
				mm_rbtree_rotate_left( this, parent );
			} else if ( node == parent->lhs && parent == grandparent->rhs ) {
				mm_rbtree_rotate_right( this, parent );
			}

			if ( parent == node->rhs ) {
				mm_rbtree_rotate_left( this, grandparent );
			} else {
				mm_rbtree_rotate_right( this, grandparent );
			}

			node->colour = MM_RB_BLACK;
			grandparent->colour = MM_RB_RED;

			return;
		}

		parent->colour = MM_RB_BLACK;
		uncle->colour = MM_RB_BLACK;
		grandparent->colour = MM_RB_RED;
		node = grandparent;
	}
}

bool mm_rbtree_insert( struct mm_rbtree *this, struct mm_rbnode *node, const void *key ) {
	mm_rbnode_init( node, key );

	if ( !this->root ) {
		this->root = node;
		mm_list_add( &this->head, &node->pos );
		node->colour = MM_RB_BLACK;

		return true;
	}

	int diff;
	struct mm_rbnode *parent;

	mm_rbtree_find_nearest( this, key, &diff, &parent );

	if ( diff < 0 ) {
		parent->lhs = node;
		mm_list_add_tail( &parent->pos, &node->pos );
	} else if ( diff > 0 ) {
		parent->rhs = node;
		mm_list_add( &parent->pos, &node->pos );
	} else {
		return false;
	}

	node->parent = parent;
	mm_rbtree_insert_fix( this, node );

	return true;
}

static inline struct mm_rbnode* mm_rbnode_get_sibling( struct mm_rbnode *node, struct mm_rbnode *parent ) {
	if ( node == parent->rhs ) {
		return parent->lhs;
	} else {
		return parent->rhs;
	}
}

static void mm_rbtree_remove_fix_rotations( struct mm_rbtree *this, struct mm_rbnode *parent, struct mm_rbnode *prev ) {
	struct mm_rbnode *sibling;
	struct mm_rbnode *l_nephew;
	struct mm_rbnode *r_nephew;

	// case 2
	sibling = mm_rbnode_get_sibling( prev, parent );

	if ( sibling->colour == MM_RB_RED ) {
		parent->colour = MM_RB_RED;
		sibling->colour = MM_RB_BLACK;

		if ( prev == parent->lhs ) {
			mm_rbtree_rotate_left( this, parent );
		} else {
			mm_rbtree_rotate_right( this, parent );
		}

		sibling = mm_rbnode_get_sibling( prev, parent );
	}

	// case 3 & 4
	l_nephew = sibling->lhs;
	r_nephew = sibling->rhs;

	if ( ( !l_nephew || l_nephew->colour == MM_RB_BLACK )
	  && ( !r_nephew || r_nephew->colour == MM_RB_BLACK ) ) {
		sibling->colour = MM_RB_RED;
		parent->colour = MM_RB_BLACK;
		return;
	}

	// case 5
	if ( ( l_nephew && l_nephew->colour == MM_RB_RED )
	  && ( !r_nephew || r_nephew->colour == MM_RB_BLACK ) ) {
		sibling->colour = MM_RB_RED;
		l_nephew->colour = MM_RB_BLACK;

		mm_rbtree_rotate_right( this, sibling );
		sibling = mm_rbnode_get_sibling( prev, parent );
	} else if ( ( !l_nephew || l_nephew->colour == MM_RB_BLACK )
		 && ( r_nephew && r_nephew->colour == MM_RB_RED ) ) {
		sibling->colour = MM_RB_RED;
		r_nephew->colour = MM_RB_BLACK;

		mm_rbtree_rotate_left( this, sibling );
		sibling = mm_rbnode_get_sibling( prev, parent );
	}

	// case 6
	sibling->colour = parent->colour;
	parent->colour = MM_RB_BLACK;

	if ( prev == parent->lhs ) {
		sibling->rhs->colour = MM_RB_BLACK;
		mm_rbtree_rotate_left( this, parent );
	} else {
		sibling->lhs->colour = MM_RB_BLACK;
		mm_rbtree_rotate_right( this, parent );
	}
}

static inline void mm_rbtree_swap_child( struct mm_rbtree *this, struct mm_rbnode *old, struct mm_rbnode *new ) {
	struct mm_rbnode *parent = old->parent;

	if ( !parent ) {
		this->root = new;
	} else if ( parent->lhs == old ) {
		parent->lhs = new;
	} else {
		parent->rhs = new;
	}
}

// call when removing a black leaf ( has NULL children )
// according to Wikipedia this is the most 'complex' case
static void mm_rbtree_remove_fix( struct mm_rbtree *this, struct mm_rbnode *parent ) {
	struct mm_rbnode *prev = NULL;
	struct mm_rbnode *sibling;
	struct mm_rbnode *l_nephew;
	struct mm_rbnode *r_nephew;

	while( parent ) {
		sibling = parent->rhs == prev ? parent->lhs : parent->rhs;
		l_nephew = sibling->lhs;
		r_nephew = sibling->rhs;

		if ( sibling->colour == MM_RB_RED
		|| ( l_nephew && l_nephew->colour == MM_RB_RED )
		|| ( r_nephew && r_nephew->colour == MM_RB_RED ) ) {
			mm_rbtree_remove_fix_rotations( this, parent, prev );
			return;
		}

		// case 3 & 4
		sibling->colour == MM_RB_RED;

		if ( parent->colour == MM_RB_RED ) {
			parent->colour = MM_RB_BLACK;
			return;
		}

		prev = parent;
		parent = parent->parent;
	}
}

void mm_rbtree_remove( struct mm_rbtree *this, struct mm_rbnode *node ) {
	mm_list_del( &node->pos );

	if ( !node->lhs && !node->rhs ) { // no children
		mm_rbtree_swap_child( this, node, NULL );

		if ( node->colour == MM_RB_BLACK ) {
			mm_rbtree_remove_fix( this, node->parent );
		}
	} else if ( node->lhs && !node->rhs ) { // replace with lhs
		mm_rbtree_swap_child( this, node, node->lhs );
		node->lhs->parent = node->parent;
		node->lhs->colour = MM_RB_BLACK;
	} else if ( !node->lhs && node->rhs ) { // replace with rhs
		mm_rbtree_swap_child( this, node, node->rhs );
		node->rhs->parent = node->parent;
		node->rhs->colour = MM_RB_BLACK;
	} else { // both children are valid nodes
		struct mm_rbnode *successor = node->rhs;
		struct mm_rbnode *parent;
		struct mm_rbnode *child;
		struct mm_rbnode *next = NULL;

		for(; successor->lhs; successor = successor->lhs );
		parent = successor->parent;
		child = successor->rhs;

		parent->lhs = child;

		successor->lhs = node->lhs;
		successor->lhs->parent = successor;
		successor->rhs = node->rhs;
		successor->rhs->parent = successor;

		if ( child ) {
			child->parent = parent;
		} else if ( successor->colour == MM_RB_BLACK ) {
			next = parent;
		}

		successor->colour = node->colour;
		mm_rbtree_swap_child( this, node, successor );

		if ( next ) {
			mm_rbtree_remove_fix( this, next );
		}
	}
}

bool mm_rbtree_remove_key( struct mm_rbtree *this, const void *key ) {
	struct mm_rbnode *node = mm_rbtree_find( this, MM_RB_FIND_EQUAL, key );

	if ( !node ) {
		return false;
	}

	mm_rbtree_remove( this, node );

	return true;
}
