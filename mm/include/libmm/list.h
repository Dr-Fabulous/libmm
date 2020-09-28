#ifndef LIBMM_LIST_H
#define LIBMM_LIST_H
#include <stdbool.h>

typedef struct list_node {
	struct list_node *prev;
	struct list_node *next;
} list_node_t;

static inline void list_init( struct list_node *head ) {
	head->prev = head;
	head->next = head;
}

static inline bool list_empty( struct list_node *head ) {
	return head->next == head;
}

static inline void _list_add_( struct list_node *prev, struct list_node *next, struct list_node *node ) {
	prev->next = node;
	next->prev = node;
	node->prev = prev;
	node->next = next;
}

static inline void list_add( struct list_node *head, struct list_node *node ) {
	_list_add_( head, head->next, node );
}

static inline void list_add_tail( struct list_node *head, struct list_node *node ) {
	_list_add_( head->prev, head, node );
}

static inline void list_del( struct list_node *node ) {
	node->prev->next = node->next;
	node->next->prev = node->prev;
}

#define LIST_INIT( name )\
	{ .prev = &name, .next = &name }

#define LIST_HEAD( name )\
	struct list_node name = LIST_INIT( name )

#define LIST_ENTRY( ptr, type, member )\
	CONTAINER_OF( ptr, type, member )

#define LIST_NEXT_ENTRY( ptr, type, member )\
	LIST_ENTRY( ( ptr )->member.next, type, member )

#define LIST_FIRST_ENTRY( head, type, member )\
	LIST_ENTRY( ( head )->next, type, member )

#define list_for_each( head, pos )\
	for ( ( pos ) = ( head )->next; ( pos ) != ( head ); ( pos ) = ( pos )->next )

#define list_for_each_safe( head, pos, tmp )\
	for( ( pos ) = ( head )->next, ( tmp ) = ( pos )->next;\
	     ( pos ) != ( head );\
	     ( pos ) = ( tmp ), ( tmp ) = ( pos )->next )

#define list_for_each_entry( head, pos, type, member )\
	for( ( pos ) = LIST_FIRST_ENTRY( head, type, member );\
	    &( pos )->member != head;\
	     ( pos ) = LIST_NEXT_ENTRY( pos, type, member ) )

#define list_for_each_entry_safe( head, pos, tmp, type, member )\
	for( ( pos ) = LIST_FIRST_ENTRY( head, type, member ), ( tmp ) = LIST_NEXT_ENTRY( pos, type, member );\
	    &( pos ) != ( head );\
	     ( pos ) = ( tmp ), ( tmp ) = LIST_NEXT_ENTRY( pos, type, member ) )

#endif
