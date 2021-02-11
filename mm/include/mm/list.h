#ifndef MM_LIST_H
#define MM_LIST_H
#include "mm/common.h"

/*! \file */

/*!
	\brief Invasive doubly linked list.

	Use by embedding into a struct and passing pointers
	to the given member variable.

	\code{.c}
	struct item {
		struct mm_list pos;
		int id;
	};

	static void f( void ) {
		MM_LIST_DECLARE( items );
		
		struct item *i = MM_MALLOC( sizeof( *i ) );
		mm_list_add( &items, &i->pos ); // pass a pointer to the list element
	}
	\endcode
*/
typedef struct mm_list {
	struct mm_list *prev; //!< pointer to previous node
	struct mm_list *next; //!< pointer to next node
} mm_list_t;

/*!
	\brief Initialize a list.

	An initialized list is made to point at itself in both directions.
	This turns out to be a more elegant way to handle having no elements.
	\param head list to initialize.
*/
static inline void mm_list_init( struct mm_list *head ) {
	head->prev = head;
	head->next = head;
}

/*!
	\brief Check if a list is empty.
	\param head list to check.
	\return true on being empty.
*/
static inline bool mm_list_empty( struct mm_list const *head ) {
	return head->next == head;
}

/*!
	\brief Check if a list has only one element.
	\param head list to check.
	\return true on having just one element.
*/
static inline bool mm_list_has_one( struct mm_list *head ) {
	return !mm_list_empty( head ) && head->prev == head->next;
}

/*!
	\brief Check if a position is the first element in a list.
	\param head list to check.
	\param pos node to check.
	\return true if pos is the first node of head.
*/
static inline bool mm_list_is_first( struct mm_list *head, struct mm_list *pos ) {
	return head->next == pos;
}

/*!
	\brief Check if a position is the last element in a list.
	\param head list to check.
	\param pos node to check.
	\return true if pos is the last node of head.
*/
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

/*!
	\brief add element to the head of a list.
	\param head list to add to.
	\param pos node to insert.
*/
static inline void mm_list_add( struct mm_list *head, struct mm_list *pos ) {
	mm_list_do_add( head, head->next, pos );
}

/*!
	\brief add element to the end of a list.
	\param head list to add to.
	\param pos node to insert.
*/
static inline void mm_list_add_tail( struct mm_list *head, struct mm_list *pos ) {
	mm_list_do_add( head->prev, head, pos );
}

/*!
	\breif remove a node from it's current list.
	\param pos node to remove.
*/
static inline void mm_list_del( struct mm_list *pos ) {
	mm_list_do_del( pos->prev, pos->next );
	mm_list_init( pos );
}

/*!
	\brief remove a node from it's current list and add to the beginning of another.
	\param head list to add node to.
	\param pos node to move.
*/
static inline void mm_list_move( struct mm_list *head, struct mm_list *pos ) {
	mm_list_do_del( pos->prev, pos->next );
	mm_list_add( head, pos );
}

/*!
	\brief remove a node from it's current list and add to the end of another.
	\param head list to add node to.
	\param pos node to move.
*/
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

/*!
	\brief remove a node from it's current list and insert a new node at the same position.
	\param old_pos node to remove.
	\param new_pos node to insert.
*/
static inline void mm_list_replace( struct mm_list *old_pos, struct mm_list *new_pos ) {
	mm_list_do_replace( old_pos, new_pos );
	mm_list_init( old_pos );
}

/*!
	\breif swap the positions of two nodes in the same list.
	\param pos_1 node to swap.
	\param pos_2 node to swap.
*/
static inline void mm_list_swap( struct mm_list *pos_1, struct mm_list *pos_2 ) {
	struct mm_list *tmp = pos_2->prev;

	mm_list_do_del( pos_2->prev, pos_2->next );
	mm_list_replace( pos_1, pos_2 );

	if ( tmp == pos_1 ) {
		tmp = pos_2;
	}

	mm_list_add( pos_1, tmp );
}

/*!
	\brief Rotate the list such that the current list will be shifted to the left by one space.
	\param head list to rotate.
*/
static inline void mm_list_rotate_left( struct mm_list *head ) {
	if ( !mm_list_empty( head ) ) {
		mm_list_move_tail( head, head->next );
	}
}

/*!
	\brief Rotate the list such that the current list will be shifted to the right by one space.
	\param head list to rotate.
*/
static inline void mm_list_rotate_right( struct mm_list *head ) {
	if ( !mm_list_empty( head ) ) {
		mm_list_move( head, head->prev );
	}
}

/*!
	\brief Rotate the list such that the provided position is next to the head.
	\param head list to rotate.
	\param pos position to rotate to.
*/
static inline void mm_list_rotate_to( struct mm_list *head, struct mm_list *pos ) {
	mm_list_move_tail( head, pos );
}

static inline void mm_list_do_cut( struct mm_list *head, struct mm_list *other, struct mm_list *pos ) {
	struct mm_list *end = pos->next;

	other->next = head->next;
	other->next->prev = other;
	other->prev = pos;
	pos->next = other;
	head->next = end;
	end->prev = head;
}

/*!
	\brief Move elements between pos and the tail in a list to another list.
	\param head list to move elements from.
	\param pos position in list to begin cut.
	\param other list to add cut elements to.
*/
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

/*!
	\brief Join two lists together, placing new elements at the head.
	\brief head list to add elements to.
	\brief other list to take elements from.
*/
static inline void mm_list_splice( struct mm_list *head, struct mm_list *other ) {
	if ( !mm_list_empty( other ) ) {
		mm_list_do_splice( head, head->next, other );
		mm_list_init( other );
	}
}

/*!
	\brief Join two lists together, placing new elements at the tail.
	\brief head list to add elements to.
	\brief other list to take elements from.
*/
static inline void mm_list_splice_tail( struct mm_list *head, struct mm_list *other ) {
	if ( !mm_list_empty( other ) ) {
		mm_list_do_splice( head->prev, head, other );
		mm_list_init( other );
	}
}

/*!
	\brief Define brace initializer for an empty list.
	\param name variable the initializer is assigned to.
*/
#define MM_LIST_INIT( name )\
	{ .prev = &name, .next = &name }

/*!
	\brief Define list variable.

	This should be used to declare all of your list variables.
*/
#define MM_LIST_DECLARE( name )\
	struct mm_list name = MM_LIST_INIT( name )

/*!
	\brief Iterate across all mm_list nodes.
	\param head list to iterate across.
	\param pos mm_list pointer.
*/
#define MM_LIST_FOREACH( head, pos )\
	for ( ( pos ) = ( head )->next;\
	      ( pos ) != ( head );\
	      ( pos ) = ( pos )->next )
/*!
	\brief Iterate across all mm_list nodes in reverse order.

	\param head list to iterate across.
	\param pos mm_list pointer.
*/
#define MM_LIST_FOREACH_REVERSE( head, pos )\
	for( ( pos ) = ( head )->prev;\
	     ( pos ) != ( head );\
	     ( pos ) = ( pos )->prev )

/*!
	\brief Iterate across all mm_list nodes.

	It's safe to remove elements while using this loop.
	\param head list to iterate across.
	\param pos mm_list pointer.
	\param tmp mm_list pointer to hold next position.
*/
#define MM_LIST_FOREACH_SAFE( head, pos, tmp )\
	for ( ( pos ) = ( head )->next, ( tmp ) = ( pos )->next;\
	      ( pos ) != ( head );\
	      ( pos ) = ( tmp ), ( tmp ) = ( pos )->next )

/*!
	\brief Iterate across all mm_list nodes in reverse order.

	It's safe to remove elements while using this loop.
	\param head list to iterate across.
	\param pos mm_list pointer.
	\param tmp mm_list pointer to hold next position.
*/
#define MM_LIST_FOREACH_REVERSE_SAFE( head, pos, tmp )\
	for ( ( pos ) = ( head )->prev, ( tmp ) = ( pos )->prev;\
	      ( pos ) != ( head );\
	      ( pos ) = ( tmp ), ( tmp ) = ( pos )->prev )

#define MM_LIST_FIRST_CONTAINER( head, type, member )\
	MM_CONTAINER_OF( ( head )->next, type, member )

#define MM_LIST_LAST_CONTAINER( head, type, member )\
	MM_CONTAINER_OF( ( head )->prev, type, member )

#define MM_LIST_NEXT_CONTAINER( pos, type, member )\
	MM_CONTAINER_OF( ( pos )->member.next, type, member )

#define MM_LIST_PREV_CONTAINER( pos, type, member )\
	MM_CONTAINER_OF( ( pos )->member.prev, type, member )

/*!
	\brief Iterate across all containers in a list.

	A container here refers to a struct that has a mm_list as a member.
	\param head list to iterate across.
	\param pos container pointer.
	\param type typename for container.
	\param member member name for mm_list instance inside container.
*/
#define MM_LIST_FOREACH_CONTAINER( head, pos, type, member )\
	for ( ( pos ) = MM_LIST_FIRST_CONTAINER( head, type, member );\
	      &( pos )->member != ( head );\
	      ( pos ) = MM_LIST_NEXT_CONTAINER( pos, type, member ) )

/*!
	\brief Iterate across all containers in a list in reverse order.

	A container here refers to a struct that has a mm_list as a member.
	\param head list to iterate across.
	\param pos container pointer.
	\param type typename for container.
	\param member member name for mm_list instance inside container.
*/
#define MM_LIST_FOREACH_CONTAINER_REVERSE( head, pos, type, member )\
	for ( ( pos ) = MM_LIST_LAST_CONTAINER( head, type, member );\
	      &( pos )->member != ( head );\
	      ( pos ) = MM_LIST_PREV_CONTAINER( pos, type, member ) )

/*!
	\brief Iterate across all containers in a list.

	A container here refers to a struct that has a mm_list as a member.
	It's safe to remove elements while using this loop.
	\param head list to iterate across.
	\param pos container pointer.
	\param type typename for container.
	\param member member name for mm_list instance inside container.
*/
#define MM_LIST_FOREACH_CONTAINER_SAFE( head, pos, tmp, type, member )\
	for ( ( pos ) = MM_LIST_FIRST_CONTAINER( head, type, member ), ( tmp ) = MM_LIST_NEXT_CONTAINER( pos, type, member );\
	      &( pos )->member != ( head );\
	      ( pos ) = ( tmp ), ( tmp ) = MM_LIST_NEXT_CONTAINER( pos, type, member ) )

/*!
	\brief Iterate across all containers in a list in reverse order.

	A container here refers to a struct that has a mm_list as a member.
	It's safe to remove elements while using this loop.
	\param head list to iterate across.
	\param pos container pointer.
	\param type typename for container.
	\param member member name for mm_list instance inside container.
*/
#define MM_LIST_FOREACH_CONTAINER_REVERSE_SAFE( head, pos, tmp, type, member )\
	for ( ( pos ) = MM_LIST_LAST_CONTAINER( head, type, member ), ( tmp ) = MM_LIST_PREV_CONTAINER( pos, type, member );\
	      &( pos )->member != ( head );\
	      ( pos ) = ( tmp ), ( tmp ) = MM_LIST_PREV_CONTAINER( pos, type, member ) )

#endif
