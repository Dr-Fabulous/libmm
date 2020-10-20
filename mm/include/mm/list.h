#ifndef MM_LIST_H
#define MM_LIST_H
#include "mm/common.h"

typedef struct mm_list {
	struct mm_list *prev;
	struct mm_list *next;
} mm_list_t;

static inline void mm_list_init( struct mm_list *head ) {
	head->prev = head;
	head->next = next;
}

static inline bool mm_list_empty( struct mm_list *head ) {
	return head->next == head;
}

static inline bool mm_list_has_one( struct mm_list *head ) {
	return !mm_list_empty( head ) && head->prev == head->next;
}

static inline bool mm_list_is_first( struct mm_list *head, struct mm_list *pos ) {
	return head->next == pos;
}

static inline bool mm_list_is_last( struct mm_list *head, struct mm_list *pos ) {
	return head->prev == pos;
}

static inline void mm_list_do_add( struct mm_list *prev, struct mm_list *next, struct mm_list *pos ) {
	prev->next = pos;
	next->prev = pos;
	pos->prev = prev;
	pos->next = next;
}

static inline void mm_list_do_del( struct mm_list *prev, struct mm_list *next ) {
	prev->next = next;
	next->prev = prev;
}

static inline void mm_list_add( struct mm_list *head, struct mm_list *pos ) {
	mm_list_do_add( head, head->next, pos );
}

static inline void mm_list_add_tail( struct mm_list *head, struct mm_list *pos ) {
	mm_list_do_add( head->prev, head, pos );
}

static inline void mm_list_del( struct mm_list *pos ) {
	mm_list_do_del( pos->prev, pos->next );
	mm_list_init( pos );
}

static inline void mm_list_move( struct mm_list *head, struct mm_list *pos ) {
	mm_list_do_del( pos->prev, pos->next );
	mm_list_add( head, pos );
}

static inline void mm_list_move_tail( struct mm_list *head, struct mm_list *pos ) {
	mm_list_do_del( pos->prev, pos->next );
	mm_list_add_tail( head, pos );
}

static inline void mm_list_do_replace( struct mm_list *old_pos, struct mm_list *new_pos ) {
	new_pos->prev = old_pos->prev;
	new_pos->next = old_pos->next;
	new_pos->prev->next = new_pos;
	new_pos->next->prev = new_pos;
}

static inline void mm_list_replace( struct mm_list *old_pos, struct mm_list *new_pos ) {
	mm_list_do_replace( old_pos, new_pos );
	mm_list_init( old_pos );
}

static inline void mm_list_swap( struct mm_list *pos_1, struct mm_list *pos_2 ) {
	struct mm_list *tmp = pos_2->prev;

	mm_list_do_del( pos_2 );
	mm_list_replace( pos_1, pos_2 );

	if ( tmp == pos_1 ) {
		tmp = pos_2;
	}

	mm_list_add( pos_1, tmp );
}

static inline void mm_list_rotate_left( struct mm_list *head ) {
	if ( !mm_list_empty( head ) ) {
		mm_list_move_tail( head, head->next );
	}
}

static inline void mm_list_rotate_right( struct mm_list *head ) {
	if ( !mm_list_empty( head ) ) {
		mm_list_move( head, head->prev );
	}
}

static inline void mm_list_rotate_to( struct mm_list *head, struct mm_list *pos ) {
	mm_list_move_tail( head, pos );
}

static inline void mm_list_do_cut( struct mm_list *head, struct mm_list *other, struct mm_list *pos ) {
	struct end = pos->next;

	other->next = head->next;
	other->next->prev = other;
	other->prev = pos;
	pos->next = other;
	head->next = end;
	end->prev = head;
}

static inline void mm_list_cut( struct mm_list *head, struct mm_list *other, struct mm_list *pos ) {
	if ( mm_list_empty( head )
	|| ( mm_list_has_one( head ) && head != pos && head->next != pos ) ) {
		return;
	}

	if ( head == pos ) {
		mm_list_init( other );
	} else {
		mm_list_do_cut( head, other, pos );
	}
}

static inline void mm_list_do_splice( struct mm_list *prev, struct mm_list *next, struct mm_list *head ) {
	head->next->prev = prev;
	prev->next = head->next;
	head->prev->next = next;
	next->prev = head->prev;
}

static inline void mm_list_splice( struct mm_list *head, struct mm_list *other ) {
	if ( !mm_list_empty( other ) ) {
		mm_list_do_splice( head, head->next, other );
		mm_list_init( other );
	}
}

static inline void mm_list_splice_tail( struct mm_list *head, struct mm_list *other ) {
	if ( !mm_list_empty( other ) ) {
		mm_list_do_splice( head->prev, head, other );
		mm_list_init( other );
	}
}

#define MM_LIST_INIT( name )\
	{ .prev = &name, .next = &name }

#define MM_LIST_DECLARE( name )\
	struct mm_list name = MM_LIST_INIT( name )

#define MM_LIST_FOREACH( head, pos )\
	for ( ( pos ) = ( head )->next;\
	      ( pos ) != ( head );\
	      ( pos ) = ( pos )->next )

#define MM_LIST_FOREACH_REVERSE( head, pos )\
	for( ( pos ) = ( head )->prev;\
	     ( pos ) != ( head );\
	     ( pos ) = ( pos )->prev )

#define MM_LIST_FOREACH_SAFE( head, pos, tmp )\
	for ( ( pos ) = ( head )->next, ( tmp ) = ( pos )->next;\
	      ( pos ) != ( head );\
	      ( pos ) = ( tmp ), ( tmp ) = ( pos )->next )

#define MM_LIST_FOREACH_REVERSE_SAFE( head, pos, tmp )\
	for ( ( pos ) = ( head )->prev, ( tmp ) = ( pos )->prev;\
	      ( pos ) != ( head );\
	      ( pos ) = ( tmp ), ( tmp ) = ( pos )->prev )

#define MM_LIST_FIRST_CONTAINER( head, type, member )\
	MM_CONTAINER_OF( head->next, type, member )

#define MM_LIST_LAST_CONTAINER( head, type, member )\
	MM_CONTAINER_OF( head->prev, type, member )

#define MM_LIST_NEXT_CONTAINER( pos, type, member )\
	MM_CONTAINER_OF( ( pos )->member->next, type, member )

#define MM_LIST_PREV_CONTAINER( pos, type, member )\
	MM_CONTAINER_OF( ( pos )->member->prev, type, member )

#define MM_LIST_FOREACH_CONTAINER( head, pos, type, member )\
	for ( ( pos ) = MM_LIST_FIRST_CONTAINER( head, type, member );\
	      &( pos )->member != ( head );\
	      ( pos ) = MM_LIST_NEXT_CONTAINER( pos, type, member ) )

#define MM_LIST_FOREACH_CONTAINER_REVERSE( head, pos, type, member )\
	for ( ( pos ) = MM_LIST_LAST_CONTAINER( head, type, member );\
	      &( pos )->member != ( head );\
	      ( pos ) = MM_LIST_PREV_CONTAINER( pos, type, member ) )

#define MM_LIST_FOREACH_CONTAINER_SAFE( head, pos, tmp, type, member )\
	for ( ( pos ) = MM_LIST_FIRST_CONTAINER( head, type, member ), ( tmp ) = MM_LIST_NEXT_CONTAINER( pos, type, member );\
	      &( pos )->member != ( head );\
	      ( pos ) = ( tmp ), ( tmp ) = MM_LIST_NEXT_CONTAINER( pos, type, member ) )

#define MM_LIST_FOREACH_CONTAINER_REVERSE_SAFE( head, pos, tmp, type, member )\
	for ( ( pos ) = MM_LIST_LAST_CONTAINER( head, type, member ), ( tmp ) = MM_LIST_PREV_CONTAINER( pos, type, member );\
	      &( pos )->member != ( head );\
	      ( pos ) = ( tmp ), ( tmp ) = MM_LIST_PREV_CONTAINER( pos, type, member ) )

#endif
