#ifndef MM_ILIST_H
#define MM_ILIST_H
#include "mm/common.h"

typedef struct mm_ilist {
	struct mm_ilist *prev;
	struct mm_ilist *next;
} mm_ilist_t;

static inline void mm_ilist_init( struct mm_ilist *head ) {
	head->prev = head;
	head->next = next;
}

static inline bool mm_ilist_empty( struct mm_ilist *head ) {
	return head->next == head;
}

static inline bool mm_ilist_has_one( struct mm_ilist *head ) {
	return !mm_ilist_empty( head ) && head->prev == head->next;
}

static inline bool mm_ilist_is_first( struct mm_ilist *head, struct mm_ilist *pos ) {
	return head->next == pos;
}

static inline bool mm_ilist_is_last( struct mm_ilist *head, struct mm_ilist *pos ) {
	return head->prev == pos;
}

static inline void mm_ilist_do_add( struct mm_ilist *prev, struct mm_ilist *next, struct mm_ilist *pos ) {
	prev->next = pos;
	next->prev = pos;
	pos->prev = prev;
	pos->next = next;
}

static inline void mm_ilist_do_del( struct mm_ilist *prev, struct mm_ilist *next ) {
	prev->next = next;
	next->prev = prev;
}

static inline void mm_ilist_add( struct mm_ilist *head, struct mm_ilist *pos ) {
	mm_ilist_do_add( head, head->next, pos );
}

static inline void mm_ilist_add_tail( struct mm_ilist *head, struct mm_ilist *pos ) {
	mm_ilist_do_add( head->prev, head, pos );
}

static inline void mm_ilist_del( struct mm_ilist *pos ) {
	mm_ilist_do_del( pos->prev, pos->next );
	mm_ilist_init( pos );
}

static inline void mm_ilist_move( struct mm_ilist *head, struct mm_ilist *pos ) {
	mm_ilist_do_del( pos->prev, pos->next );
	mm_ilist_add( head, pos );
}

static inline void mm_ilist_move_tail( struct mm_ilist *head, struct mm_ilist *pos ) {
	mm_ilist_do_del( pos->prev, pos->next );
	mm_ilist_add_tail( head, pos );
}

static inline void mm_ilist_do_replace( struct mm_ilist *old_pos, struct mm_ilist *new_pos ) {
	new_pos->prev = old_pos->prev;
	new_pos->next = old_pos->next;
	new_pos->prev->next = new_pos;
	new_pos->next->prev = new_pos;
}

static inline void mm_ilist_do_replace( struct mm_ilist *old_pos, struct mm_ilist *new_pos ) {
	mm_ilist_do_replace( old_pos, new_pos );
	mm_ilist_init( old_pos );
}

static inline void mm_ilist_swap( struct mm_ilist *pos_1, struct mm_ilist *pos_2 ) {
	struct mm_ilist *tmp = pos_2->prev;

	mm_ilist_do_del( pos_2 );
	mm_ilist_replace( pos_1, pos_2 );

	if ( tmp == pos_1 ) {
		tmp = pos_2;
	}

	mm_ilist_add( pos_1, tmp );
}

static inline void mm_ilist_rotate_left( struct mm_ilist *head ) {
	if ( !mm_ilist_empty( head ) ) {
		mm_ilist_move_tail( head, head->next );
	}
}

static inline void mm_ilist_rotate_right( struct mm_ilist *head ) {
	if ( !mm_ilist_empty( head ) ) {
		mm_ilist_move( head, head->prev );
	}
}

static inline void mm_ilist_rotate_to( struct mm_ilist *head, struct mm_ilist *pos ) {
	mm_ilist_move_tail( head, pos );
}

static inline void mm_ilist_do_cut( struct mm_ilist *head, struct mm_ilist *other, struct mm_ilist *pos ) {
	struct end = pos->next;

	other->next = head->next;
	other->next->prev = other;
	other->prev = pos;
	pos->next = other;
	head->next = end;
	end->prev = head;
}

static inline void mm_ilist_cut( struct mm_ilist *head, struct mm_ilist *other, struct mm_ilist *pos ) {
	if ( mm_ilist_empty( head )
	|| ( mm_ilist_has_one( head ) && head != pos && head->next != pos ) ) {
		return;
	}

	if ( head == pos ) {
		mm_ilist_init( other );
	} else {
		mm_ilist_do_cut( head, other, pos );
	}
}

static inline void mm_ilist_do_splice( struct mm_ilist *prev, struct mm_ilist *next, struct mm_ilist *head ) {
	head->next->prev = prev;
	prev->next = head->next;
	head->prev->next = next;
	next->prev = head->prev;
}

static inline void mm_ilist_splice( struct mm_ilist *head, struct mm_ilist *other ) {
	if ( !mm_ilist_empty( other ) ) {
		mm_ilist_do_splice( head, head->next, other );
		mm_ilist_init( other );
	}
}

static inline void mm_ilist_splice_tail( struct mm_ilist *head, struct mm_ilist *other ) {
	if ( !mm_ilist_empty( other ) ) {
		mm_ilist_do_splice( head->prev, head, other );
		mm_ilist_init( other );
	}
}

#define MM_ILIST_INIT( name )\
	{ .prev = &name, .next = &name }

#define MM_ILIST_DECLARE( name )\
	struct mm_ilist name = MM_ILIST_INIT( name )

#define MM_ILIST_FOREACH( head, pos )\
	for ( ( pos ) = ( head )->next;\
	      ( pos ) != ( head );\
	      ( pos ) = ( pos )->next )

#define MM_ILIST_FOREACH_REVERSE( head, pos )\
	for( ( pos ) = ( head )->prev;\
	     ( pos ) != ( head );\
	     ( pos ) = ( pos )->prev )

#define MM_ILIST_FOREACH_SAFE( head, pos, tmp )\
	for ( ( pos ) = ( head )->next, ( tmp ) = ( pos )->next;\
	      ( pos ) != ( head );\
	      ( pos ) = ( tmp ), ( tmp ) = ( pos )->next )

#define MM_ILIST_FOREACH_REVERSE_SAFE( head, pos, tmp )\
	for ( ( pos ) = ( head )->prev, ( tmp ) = ( pos )->prev;\
	      ( pos ) != ( head );\
	      ( pos ) = ( tmp ), ( tmp ) = ( pos )->prev )

#define MM_ILIST_FIRST_CONTAINER( head, type, member )\
	MM_CONTAINER_OF( head->next, type, member )

#define MM_ILIST_LAST_CONTAINER( head, type, member )\
	MM_CONTAINER_OF( head->prev, type, member )

#define MM_ILIST_NEXT_CONTAINER( pos, type, member )\
	MM_CONTAINER_OF( ( pos )->member->next, type, member )

#define MM_ILIST_PREV_CONTAINER( pos, type, member )\
	MM_CONTAINER_OF( ( pos )->member->prev, type, member )

#define MM_ILIST_FOREACH_CONTAINER( head, pos, type, member )\
	for ( ( pos ) = MM_ILIST_FIRST_CONTAINER( head, type, member );\
	      &( pos )->member != ( head );\
	      ( pos ) = MM_ILIST_NEXT_CONTAINER( pos, type, member ) )

#define MM_ILIST_FOREACH_CONTAINER_REVERSE( head, pos, type, member )\
	for ( ( pos ) = MM_ILIST_LAST_CONTAINER( head, type, member );\
	      &( pos )->member != ( head );\
	      ( pos ) = MM_ILIST_PREV_CONTAINER( pos, type, member ) )

#define MM_ILIST_FOREACH_CONTAINER_SAFE( head, pos, tmp, type, member )\
	for ( ( pos ) = MM_ILIST_FIRST_CONTAINER( head, type, member ), ( tmp ) = MM_ILIST_NEXT_CONTAINER( pos, type, member );\
	      &( pos )->member != ( head );\
	      ( pos ) = ( tmp ), ( tmp ) = MM_ILIST_NEXT_CONTAINER( pos, type, member ) )

#define MM_ILIST_FOREACH_CONTAINER_REVERSE_SAFE( head, pos, tmp, type, member )\
	for ( ( pos ) = MM_ILIST_LAST_CONTAINER( head, type, member ), ( tmp ) = MM_ILIST_PREV_CONTAINER( pos, type, member );\
	      &( pos )->member != ( head );\
	      ( pos ) = ( tmp ), ( tmp ) = MM_ILIST_PREV_CONTAINER( pos, type, member ) )

#endif
