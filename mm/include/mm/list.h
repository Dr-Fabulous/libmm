#ifndef LIBMM_MM_LIST_H
#define LIBMM_MM_LIST_H
#include <stdbool.h>

typedef struct mm_list_node {
	struct mm_list_node *prev;
	struct mm_list_node *next;
} mm_list_node_t;

static inline void mm_list_init( struct mm_list_node *head ) {
	head->prev = head;
	head->next = head;
}

static inline bool mm_list_empty( struct mm_list_node *head ) {
	return head->next == head;
}

static inline void _mm_list_add_( struct mm_list_node *prev, struct mm_list_node *next, struct mm_list_node *node ) {
	prev->next = node;
	next->prev = node;
	node->prev = prev;
	node->next = next;
}

static inline void mm_list_add( struct mm_list_node *head, struct mm_list_node *node ) {
	_mm_list_add_( head, head->next, node );
}

static inline void mm_list_add_tail( struct mm_list_node *head, struct mm_list_node *node ) {
	_mm_list_add_( head->prev, head, node );
}

static inline void mm_list_del( struct mm_list_node *node ) {
	node->prev->next = node->next;
	node->next->prev = node->prev;
}

#define MM_LIST_INIT( name )\
	{ .prev = &name, .next = &name }

#define MM_LIST_HEAD( name )\
	struct mm_list_node name = MM_LIST_INIT( name )

#define MM_LIST_ENTRY( ptr, type, member )\
	CONTAINER_OF( ptr, type, member )

#define MM_LIST_NEXT_ENTRY( ptr, type, member )\
	MM_LIST_ENTRY( ( ptr )->member.next, type, member )

#define MM_LIST_FIRST_ENTRY( head, type, member )\
	MM_LIST_ENTRY( ( head )->next, type, member )

#define mm_list_for_each( head, pos )\
	for ( ( pos ) = ( head )->next; ( pos ) != ( head ); ( pos ) = ( pos )->next )

#define mm_list_for_each_safe( head, pos, tmp )\
	for( ( pos ) = ( head )->next, ( tmp ) = ( pos )->next;\
	     ( pos ) != ( head );\
	     ( pos ) = ( tmp ), ( tmp ) = ( pos )->next )

#define mm_list_for_each_entry( head, pos, type, member )\
	for( ( pos ) = MM_LIST_FIRST_ENTRY( head, type, member );\
	    &( pos )->member != head;\
	     ( pos ) = MM_LIST_NEXT_ENTRY( pos, type, member ) )

#define mm_list_for_each_entry_safe( head, pos, tmp, type, member )\
	for( ( pos ) = MM_LIST_FIRST_ENTRY( head, type, member ), ( tmp ) = MM_LIST_NEXT_ENTRY( pos, type, member );\
	    &( pos ) != ( head );\
	     ( pos ) = ( tmp ), ( tmp ) = MM_LIST_NEXT_ENTRY( pos, type, member ) )

#endif
