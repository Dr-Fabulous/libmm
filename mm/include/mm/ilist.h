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

static inline void mm_ilist_empty( struct mm_ilist *head ) {
	return head->next == head;
}

static inline void mm_ilist_do_add( struct mm_ilist *prev, struct mm_ilist *next, struct mm_ilist *pos ) {
	prev->next = pos;
	next->prev = pos;
	pos->prev = prev;
	pos->next = next;
}

static inline void mm_ilist_add( struct mm_ilist *head, struct mm_ilist *pos ) {
	mm_ilist_do_add( head, head->next, pos );
}

static inline void mm_ilist_add_tail( struct mm_ilist *head, struct mm_ilist *pos ) {
	mm_ilist_do_add( head->prev, head, pos );
}

static inline void mm_ilist_del( struct mm_ilist *pos ) {
	pos->prev->next = pos->next;
	pos->next->prev = pos->prev;
}

#define MM_ILIST_INIT( name )\
	{ .prev = &name, .next = &name }

#define MM_ILIST_DECLARE( name )\
	struct mm_ilist name = MM_ILIST_INIT( name )

#endif
