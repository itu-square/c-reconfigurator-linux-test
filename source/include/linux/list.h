// /*AFLA*/ #ifndef _LINUX_LIST_H
// /*AFLA*/ #define _LINUX_LIST_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/stddef.h>
// /*AFLA*/ #include <linux/poison.h>
// /*AFLA*/ #include <linux/const.h>
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Simple doubly linked list implementation.
// /*AFLA*/  *
// /*AFLA*/  * Some of the internal functions ("__xxx") are useful when
// /*AFLA*/  * manipulating whole lists rather than single entries, as
// /*AFLA*/  * sometimes we already know the next/prev entries and we can
// /*AFLA*/  * generate better code by using them directly rather than
// /*AFLA*/  * using the generic single-entry routines.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define LIST_HEAD_INIT(name) { &(name), &(name) }
// /*AFLA*/ 
// /*AFLA*/ #define LIST_HEAD(name) \
// /*AFLA*/ 	struct list_head name = LIST_HEAD_INIT(name)
// /*AFLA*/ 
// /*AFLA*/ static inline void INIT_LIST_HEAD(struct list_head *list)
// /*AFLA*/ {
// /*AFLA*/ 	WRITE_ONCE(list->next, list);
// /*AFLA*/ 	list->prev = list;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Insert a new entry between two known consecutive entries.
// /*AFLA*/  *
// /*AFLA*/  * This is only for internal list manipulation where we know
// /*AFLA*/  * the prev/next entries already!
// /*AFLA*/  */
// /*AFLA*/ #ifndef CONFIG_DEBUG_LIST
// /*AFLA*/ static inline void __list_add(struct list_head *new,
// /*AFLA*/ 			      struct list_head *prev,
// /*AFLA*/ 			      struct list_head *next)
// /*AFLA*/ {
// /*AFLA*/ 	next->prev = new;
// /*AFLA*/ 	new->next = next;
// /*AFLA*/ 	new->prev = prev;
// /*AFLA*/ 	WRITE_ONCE(prev->next, new);
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ extern void __list_add(struct list_head *new,
// /*AFLA*/ 			      struct list_head *prev,
// /*AFLA*/ 			      struct list_head *next);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_add - add a new entry
// /*AFLA*/  * @new: new entry to be added
// /*AFLA*/  * @head: list head to add it after
// /*AFLA*/  *
// /*AFLA*/  * Insert a new entry after the specified head.
// /*AFLA*/  * This is good for implementing stacks.
// /*AFLA*/  */
// /*AFLA*/ static inline void list_add(struct list_head *new, struct list_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	__list_add(new, head, head->next);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_add_tail - add a new entry
// /*AFLA*/  * @new: new entry to be added
// /*AFLA*/  * @head: list head to add it before
// /*AFLA*/  *
// /*AFLA*/  * Insert a new entry before the specified head.
// /*AFLA*/  * This is useful for implementing queues.
// /*AFLA*/  */
// /*AFLA*/ static inline void list_add_tail(struct list_head *new, struct list_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	__list_add(new, head->prev, head);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Delete a list entry by making the prev/next entries
// /*AFLA*/  * point to each other.
// /*AFLA*/  *
// /*AFLA*/  * This is only for internal list manipulation where we know
// /*AFLA*/  * the prev/next entries already!
// /*AFLA*/  */
// /*AFLA*/ static inline void __list_del(struct list_head * prev, struct list_head * next)
// /*AFLA*/ {
// /*AFLA*/ 	next->prev = prev;
// /*AFLA*/ 	WRITE_ONCE(prev->next, next);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_del - deletes entry from list.
// /*AFLA*/  * @entry: the element to delete from the list.
// /*AFLA*/  * Note: list_empty() on entry does not return true after this, the entry is
// /*AFLA*/  * in an undefined state.
// /*AFLA*/  */
// /*AFLA*/ #ifndef CONFIG_DEBUG_LIST
// /*AFLA*/ static inline void __list_del_entry(struct list_head *entry)
// /*AFLA*/ {
// /*AFLA*/ 	__list_del(entry->prev, entry->next);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void list_del(struct list_head *entry)
// /*AFLA*/ {
// /*AFLA*/ 	__list_del(entry->prev, entry->next);
// /*AFLA*/ 	entry->next = LIST_POISON1;
// /*AFLA*/ 	entry->prev = LIST_POISON2;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ extern void __list_del_entry(struct list_head *entry);
// /*AFLA*/ extern void list_del(struct list_head *entry);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_replace - replace old entry by new one
// /*AFLA*/  * @old : the element to be replaced
// /*AFLA*/  * @new : the new element to insert
// /*AFLA*/  *
// /*AFLA*/  * If @old was empty, it will be overwritten.
// /*AFLA*/  */
// /*AFLA*/ static inline void list_replace(struct list_head *old,
// /*AFLA*/ 				struct list_head *new)
// /*AFLA*/ {
// /*AFLA*/ 	new->next = old->next;
// /*AFLA*/ 	new->next->prev = new;
// /*AFLA*/ 	new->prev = old->prev;
// /*AFLA*/ 	new->prev->next = new;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void list_replace_init(struct list_head *old,
// /*AFLA*/ 					struct list_head *new)
// /*AFLA*/ {
// /*AFLA*/ 	list_replace(old, new);
// /*AFLA*/ 	INIT_LIST_HEAD(old);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_del_init - deletes entry from list and reinitialize it.
// /*AFLA*/  * @entry: the element to delete from the list.
// /*AFLA*/  */
// /*AFLA*/ static inline void list_del_init(struct list_head *entry)
// /*AFLA*/ {
// /*AFLA*/ 	__list_del_entry(entry);
// /*AFLA*/ 	INIT_LIST_HEAD(entry);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_move - delete from one list and add as another's head
// /*AFLA*/  * @list: the entry to move
// /*AFLA*/  * @head: the head that will precede our entry
// /*AFLA*/  */
// /*AFLA*/ static inline void list_move(struct list_head *list, struct list_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	__list_del_entry(list);
// /*AFLA*/ 	list_add(list, head);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_move_tail - delete from one list and add as another's tail
// /*AFLA*/  * @list: the entry to move
// /*AFLA*/  * @head: the head that will follow our entry
// /*AFLA*/  */
// /*AFLA*/ static inline void list_move_tail(struct list_head *list,
// /*AFLA*/ 				  struct list_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	__list_del_entry(list);
// /*AFLA*/ 	list_add_tail(list, head);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_is_last - tests whether @list is the last entry in list @head
// /*AFLA*/  * @list: the entry to test
// /*AFLA*/  * @head: the head of the list
// /*AFLA*/  */
// /*AFLA*/ static inline int list_is_last(const struct list_head *list,
// /*AFLA*/ 				const struct list_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	return list->next == head;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_empty - tests whether a list is empty
// /*AFLA*/  * @head: the list to test.
// /*AFLA*/  */
// /*AFLA*/ static inline int list_empty(const struct list_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	return READ_ONCE(head->next) == head;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_empty_careful - tests whether a list is empty and not being modified
// /*AFLA*/  * @head: the list to test
// /*AFLA*/  *
// /*AFLA*/  * Description:
// /*AFLA*/  * tests whether a list is empty _and_ checks that no other CPU might be
// /*AFLA*/  * in the process of modifying either member (next or prev)
// /*AFLA*/  *
// /*AFLA*/  * NOTE: using list_empty_careful() without synchronization
// /*AFLA*/  * can only be safe if the only activity that can happen
// /*AFLA*/  * to the list entry is list_del_init(). Eg. it cannot be used
// /*AFLA*/  * if another CPU could re-list_add() it.
// /*AFLA*/  */
// /*AFLA*/ static inline int list_empty_careful(const struct list_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	struct list_head *next = head->next;
// /*AFLA*/ 	return (next == head) && (next == head->prev);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_rotate_left - rotate the list to the left
// /*AFLA*/  * @head: the head of the list
// /*AFLA*/  */
// /*AFLA*/ static inline void list_rotate_left(struct list_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	struct list_head *first;
// /*AFLA*/ 
// /*AFLA*/ 	if (!list_empty(head)) {
// /*AFLA*/ 		first = head->next;
// /*AFLA*/ 		list_move_tail(first, head);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_is_singular - tests whether a list has just one entry.
// /*AFLA*/  * @head: the list to test.
// /*AFLA*/  */
// /*AFLA*/ static inline int list_is_singular(const struct list_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	return !list_empty(head) && (head->next == head->prev);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __list_cut_position(struct list_head *list,
// /*AFLA*/ 		struct list_head *head, struct list_head *entry)
// /*AFLA*/ {
// /*AFLA*/ 	struct list_head *new_first = entry->next;
// /*AFLA*/ 	list->next = head->next;
// /*AFLA*/ 	list->next->prev = list;
// /*AFLA*/ 	list->prev = entry;
// /*AFLA*/ 	entry->next = list;
// /*AFLA*/ 	head->next = new_first;
// /*AFLA*/ 	new_first->prev = head;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_cut_position - cut a list into two
// /*AFLA*/  * @list: a new list to add all removed entries
// /*AFLA*/  * @head: a list with entries
// /*AFLA*/  * @entry: an entry within head, could be the head itself
// /*AFLA*/  *	and if so we won't cut the list
// /*AFLA*/  *
// /*AFLA*/  * This helper moves the initial part of @head, up to and
// /*AFLA*/  * including @entry, from @head to @list. You should
// /*AFLA*/  * pass on @entry an element you know is on @head. @list
// /*AFLA*/  * should be an empty list or a list you do not care about
// /*AFLA*/  * losing its data.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ static inline void list_cut_position(struct list_head *list,
// /*AFLA*/ 		struct list_head *head, struct list_head *entry)
// /*AFLA*/ {
// /*AFLA*/ 	if (list_empty(head))
// /*AFLA*/ 		return;
// /*AFLA*/ 	if (list_is_singular(head) &&
// /*AFLA*/ 		(head->next != entry && head != entry))
// /*AFLA*/ 		return;
// /*AFLA*/ 	if (entry == head)
// /*AFLA*/ 		INIT_LIST_HEAD(list);
// /*AFLA*/ 	else
// /*AFLA*/ 		__list_cut_position(list, head, entry);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __list_splice(const struct list_head *list,
// /*AFLA*/ 				 struct list_head *prev,
// /*AFLA*/ 				 struct list_head *next)
// /*AFLA*/ {
// /*AFLA*/ 	struct list_head *first = list->next;
// /*AFLA*/ 	struct list_head *last = list->prev;
// /*AFLA*/ 
// /*AFLA*/ 	first->prev = prev;
// /*AFLA*/ 	prev->next = first;
// /*AFLA*/ 
// /*AFLA*/ 	last->next = next;
// /*AFLA*/ 	next->prev = last;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_splice - join two lists, this is designed for stacks
// /*AFLA*/  * @list: the new list to add.
// /*AFLA*/  * @head: the place to add it in the first list.
// /*AFLA*/  */
// /*AFLA*/ static inline void list_splice(const struct list_head *list,
// /*AFLA*/ 				struct list_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	if (!list_empty(list))
// /*AFLA*/ 		__list_splice(list, head, head->next);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_splice_tail - join two lists, each list being a queue
// /*AFLA*/  * @list: the new list to add.
// /*AFLA*/  * @head: the place to add it in the first list.
// /*AFLA*/  */
// /*AFLA*/ static inline void list_splice_tail(struct list_head *list,
// /*AFLA*/ 				struct list_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	if (!list_empty(list))
// /*AFLA*/ 		__list_splice(list, head->prev, head);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_splice_init - join two lists and reinitialise the emptied list.
// /*AFLA*/  * @list: the new list to add.
// /*AFLA*/  * @head: the place to add it in the first list.
// /*AFLA*/  *
// /*AFLA*/  * The list at @list is reinitialised
// /*AFLA*/  */
// /*AFLA*/ static inline void list_splice_init(struct list_head *list,
// /*AFLA*/ 				    struct list_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	if (!list_empty(list)) {
// /*AFLA*/ 		__list_splice(list, head, head->next);
// /*AFLA*/ 		INIT_LIST_HEAD(list);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_splice_tail_init - join two lists and reinitialise the emptied list
// /*AFLA*/  * @list: the new list to add.
// /*AFLA*/  * @head: the place to add it in the first list.
// /*AFLA*/  *
// /*AFLA*/  * Each of the lists is a queue.
// /*AFLA*/  * The list at @list is reinitialised
// /*AFLA*/  */
// /*AFLA*/ static inline void list_splice_tail_init(struct list_head *list,
// /*AFLA*/ 					 struct list_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	if (!list_empty(list)) {
// /*AFLA*/ 		__list_splice(list, head->prev, head);
// /*AFLA*/ 		INIT_LIST_HEAD(list);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_entry - get the struct for this entry
// /*AFLA*/  * @ptr:	the &struct list_head pointer.
// /*AFLA*/  * @type:	the type of the struct this is embedded in.
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  */
// /*AFLA*/ #define list_entry(ptr, type, member) \
// /*AFLA*/ 	container_of(ptr, type, member)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_first_entry - get the first element from a list
// /*AFLA*/  * @ptr:	the list head to take the element from.
// /*AFLA*/  * @type:	the type of the struct this is embedded in.
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * Note, that list is expected to be not empty.
// /*AFLA*/  */
// /*AFLA*/ #define list_first_entry(ptr, type, member) \
// /*AFLA*/ 	list_entry((ptr)->next, type, member)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_last_entry - get the last element from a list
// /*AFLA*/  * @ptr:	the list head to take the element from.
// /*AFLA*/  * @type:	the type of the struct this is embedded in.
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * Note, that list is expected to be not empty.
// /*AFLA*/  */
// /*AFLA*/ #define list_last_entry(ptr, type, member) \
// /*AFLA*/ 	list_entry((ptr)->prev, type, member)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_first_entry_or_null - get the first element from a list
// /*AFLA*/  * @ptr:	the list head to take the element from.
// /*AFLA*/  * @type:	the type of the struct this is embedded in.
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * Note that if the list is empty, it returns NULL.
// /*AFLA*/  */
// /*AFLA*/ #define list_first_entry_or_null(ptr, type, member) ({ \
// /*AFLA*/ 	struct list_head *head__ = (ptr); \
// /*AFLA*/ 	struct list_head *pos__ = READ_ONCE(head__->next); \
// /*AFLA*/ 	pos__ != head__ ? list_entry(pos__, type, member) : NULL; \
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_next_entry - get the next element in list
// /*AFLA*/  * @pos:	the type * to cursor
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  */
// /*AFLA*/ #define list_next_entry(pos, member) \
// /*AFLA*/ 	list_entry((pos)->member.next, typeof(*(pos)), member)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_prev_entry - get the prev element in list
// /*AFLA*/  * @pos:	the type * to cursor
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  */
// /*AFLA*/ #define list_prev_entry(pos, member) \
// /*AFLA*/ 	list_entry((pos)->member.prev, typeof(*(pos)), member)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_for_each	-	iterate over a list
// /*AFLA*/  * @pos:	the &struct list_head to use as a loop cursor.
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  */
// /*AFLA*/ #define list_for_each(pos, head) \
// /*AFLA*/ 	for (pos = (head)->next; pos != (head); pos = pos->next)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_for_each_prev	-	iterate over a list backwards
// /*AFLA*/  * @pos:	the &struct list_head to use as a loop cursor.
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  */
// /*AFLA*/ #define list_for_each_prev(pos, head) \
// /*AFLA*/ 	for (pos = (head)->prev; pos != (head); pos = pos->prev)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_for_each_safe - iterate over a list safe against removal of list entry
// /*AFLA*/  * @pos:	the &struct list_head to use as a loop cursor.
// /*AFLA*/  * @n:		another &struct list_head to use as temporary storage
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  */
// /*AFLA*/ #define list_for_each_safe(pos, n, head) \
// /*AFLA*/ 	for (pos = (head)->next, n = pos->next; pos != (head); \
// /*AFLA*/ 		pos = n, n = pos->next)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
// /*AFLA*/  * @pos:	the &struct list_head to use as a loop cursor.
// /*AFLA*/  * @n:		another &struct list_head to use as temporary storage
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  */
// /*AFLA*/ #define list_for_each_prev_safe(pos, n, head) \
// /*AFLA*/ 	for (pos = (head)->prev, n = pos->prev; \
// /*AFLA*/ 	     pos != (head); \
// /*AFLA*/ 	     pos = n, n = pos->prev)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_for_each_entry	-	iterate over list of given type
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  */
// /*AFLA*/ #define list_for_each_entry(pos, head, member)				\
// /*AFLA*/ 	for (pos = list_first_entry(head, typeof(*pos), member);	\
// /*AFLA*/ 	     &pos->member != (head);					\
// /*AFLA*/ 	     pos = list_next_entry(pos, member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_for_each_entry_reverse - iterate backwards over list of given type.
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  */
// /*AFLA*/ #define list_for_each_entry_reverse(pos, head, member)			\
// /*AFLA*/ 	for (pos = list_last_entry(head, typeof(*pos), member);		\
// /*AFLA*/ 	     &pos->member != (head); 					\
// /*AFLA*/ 	     pos = list_prev_entry(pos, member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_prepare_entry - prepare a pos entry for use in list_for_each_entry_continue()
// /*AFLA*/  * @pos:	the type * to use as a start point
// /*AFLA*/  * @head:	the head of the list
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * Prepares a pos entry for use as a start point in list_for_each_entry_continue().
// /*AFLA*/  */
// /*AFLA*/ #define list_prepare_entry(pos, head, member) \
// /*AFLA*/ 	((pos) ? : list_entry(head, typeof(*pos), member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_for_each_entry_continue - continue iteration over list of given type
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * Continue to iterate over list of given type, continuing after
// /*AFLA*/  * the current position.
// /*AFLA*/  */
// /*AFLA*/ #define list_for_each_entry_continue(pos, head, member) 		\
// /*AFLA*/ 	for (pos = list_next_entry(pos, member);			\
// /*AFLA*/ 	     &pos->member != (head);					\
// /*AFLA*/ 	     pos = list_next_entry(pos, member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_for_each_entry_continue_reverse - iterate backwards from the given point
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * Start to iterate over list of given type backwards, continuing after
// /*AFLA*/  * the current position.
// /*AFLA*/  */
// /*AFLA*/ #define list_for_each_entry_continue_reverse(pos, head, member)		\
// /*AFLA*/ 	for (pos = list_prev_entry(pos, member);			\
// /*AFLA*/ 	     &pos->member != (head);					\
// /*AFLA*/ 	     pos = list_prev_entry(pos, member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_for_each_entry_from - iterate over list of given type from the current point
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * Iterate over list of given type, continuing from current position.
// /*AFLA*/  */
// /*AFLA*/ #define list_for_each_entry_from(pos, head, member) 			\
// /*AFLA*/ 	for (; &pos->member != (head);					\
// /*AFLA*/ 	     pos = list_next_entry(pos, member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @n:		another type * to use as temporary storage
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  */
// /*AFLA*/ #define list_for_each_entry_safe(pos, n, head, member)			\
// /*AFLA*/ 	for (pos = list_first_entry(head, typeof(*pos), member),	\
// /*AFLA*/ 		n = list_next_entry(pos, member);			\
// /*AFLA*/ 	     &pos->member != (head); 					\
// /*AFLA*/ 	     pos = n, n = list_next_entry(n, member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_for_each_entry_safe_continue - continue list iteration safe against removal
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @n:		another type * to use as temporary storage
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * Iterate over list of given type, continuing after current point,
// /*AFLA*/  * safe against removal of list entry.
// /*AFLA*/  */
// /*AFLA*/ #define list_for_each_entry_safe_continue(pos, n, head, member) 		\
// /*AFLA*/ 	for (pos = list_next_entry(pos, member), 				\
// /*AFLA*/ 		n = list_next_entry(pos, member);				\
// /*AFLA*/ 	     &pos->member != (head);						\
// /*AFLA*/ 	     pos = n, n = list_next_entry(n, member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_for_each_entry_safe_from - iterate over list from current point safe against removal
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @n:		another type * to use as temporary storage
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * Iterate over list of given type from current point, safe against
// /*AFLA*/  * removal of list entry.
// /*AFLA*/  */
// /*AFLA*/ #define list_for_each_entry_safe_from(pos, n, head, member) 			\
// /*AFLA*/ 	for (n = list_next_entry(pos, member);					\
// /*AFLA*/ 	     &pos->member != (head);						\
// /*AFLA*/ 	     pos = n, n = list_next_entry(n, member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_for_each_entry_safe_reverse - iterate backwards over list safe against removal
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @n:		another type * to use as temporary storage
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * Iterate backwards over list of given type, safe against removal
// /*AFLA*/  * of list entry.
// /*AFLA*/  */
// /*AFLA*/ #define list_for_each_entry_safe_reverse(pos, n, head, member)		\
// /*AFLA*/ 	for (pos = list_last_entry(head, typeof(*pos), member),		\
// /*AFLA*/ 		n = list_prev_entry(pos, member);			\
// /*AFLA*/ 	     &pos->member != (head); 					\
// /*AFLA*/ 	     pos = n, n = list_prev_entry(n, member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_safe_reset_next - reset a stale list_for_each_entry_safe loop
// /*AFLA*/  * @pos:	the loop cursor used in the list_for_each_entry_safe loop
// /*AFLA*/  * @n:		temporary storage used in list_for_each_entry_safe
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * list_safe_reset_next is not safe to use in general if the list may be
// /*AFLA*/  * modified concurrently (eg. the lock is dropped in the loop body). An
// /*AFLA*/  * exception to this is if the cursor element (pos) is pinned in the list,
// /*AFLA*/  * and list_safe_reset_next is called after re-taking the lock and before
// /*AFLA*/  * completing the current iteration of the loop body.
// /*AFLA*/  */
// /*AFLA*/ #define list_safe_reset_next(pos, n, member)				\
// /*AFLA*/ 	n = list_next_entry(pos, member)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Double linked lists with a single pointer list head.
// /*AFLA*/  * Mostly useful for hash tables where the two pointer list head is
// /*AFLA*/  * too wasteful.
// /*AFLA*/  * You lose the ability to access the tail in O(1).
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define HLIST_HEAD_INIT { .first = NULL }
// /*AFLA*/ #define HLIST_HEAD(name) struct hlist_head name = {  .first = NULL }
// /*AFLA*/ #define INIT_HLIST_HEAD(ptr) ((ptr)->first = NULL)
// /*AFLA*/ static inline void INIT_HLIST_NODE(struct hlist_node *h)
// /*AFLA*/ {
// /*AFLA*/ 	h->next = NULL;
// /*AFLA*/ 	h->pprev = NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int hlist_unhashed(const struct hlist_node *h)
// /*AFLA*/ {
// /*AFLA*/ 	return !h->pprev;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int hlist_empty(const struct hlist_head *h)
// /*AFLA*/ {
// /*AFLA*/ 	return !READ_ONCE(h->first);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __hlist_del(struct hlist_node *n)
// /*AFLA*/ {
// /*AFLA*/ 	struct hlist_node *next = n->next;
// /*AFLA*/ 	struct hlist_node **pprev = n->pprev;
// /*AFLA*/ 
// /*AFLA*/ 	WRITE_ONCE(*pprev, next);
// /*AFLA*/ 	if (next)
// /*AFLA*/ 		next->pprev = pprev;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void hlist_del(struct hlist_node *n)
// /*AFLA*/ {
// /*AFLA*/ 	__hlist_del(n);
// /*AFLA*/ 	n->next = LIST_POISON1;
// /*AFLA*/ 	n->pprev = LIST_POISON2;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void hlist_del_init(struct hlist_node *n)
// /*AFLA*/ {
// /*AFLA*/ 	if (!hlist_unhashed(n)) {
// /*AFLA*/ 		__hlist_del(n);
// /*AFLA*/ 		INIT_HLIST_NODE(n);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
// /*AFLA*/ {
// /*AFLA*/ 	struct hlist_node *first = h->first;
// /*AFLA*/ 	n->next = first;
// /*AFLA*/ 	if (first)
// /*AFLA*/ 		first->pprev = &n->next;
// /*AFLA*/ 	WRITE_ONCE(h->first, n);
// /*AFLA*/ 	n->pprev = &h->first;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* next must be != NULL */
// /*AFLA*/ static inline void hlist_add_before(struct hlist_node *n,
// /*AFLA*/ 					struct hlist_node *next)
// /*AFLA*/ {
// /*AFLA*/ 	n->pprev = next->pprev;
// /*AFLA*/ 	n->next = next;
// /*AFLA*/ 	next->pprev = &n->next;
// /*AFLA*/ 	WRITE_ONCE(*(n->pprev), n);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void hlist_add_behind(struct hlist_node *n,
// /*AFLA*/ 				    struct hlist_node *prev)
// /*AFLA*/ {
// /*AFLA*/ 	n->next = prev->next;
// /*AFLA*/ 	WRITE_ONCE(prev->next, n);
// /*AFLA*/ 	n->pprev = &prev->next;
// /*AFLA*/ 
// /*AFLA*/ 	if (n->next)
// /*AFLA*/ 		n->next->pprev  = &n->next;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* after that we'll appear to be on some hlist and hlist_del will work */
// /*AFLA*/ static inline void hlist_add_fake(struct hlist_node *n)
// /*AFLA*/ {
// /*AFLA*/ 	n->pprev = &n->next;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool hlist_fake(struct hlist_node *h)
// /*AFLA*/ {
// /*AFLA*/ 	return h->pprev == &h->next;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Check whether the node is the only node of the head without
// /*AFLA*/  * accessing head:
// /*AFLA*/  */
// /*AFLA*/ static inline bool
// /*AFLA*/ hlist_is_singular_node(struct hlist_node *n, struct hlist_head *h)
// /*AFLA*/ {
// /*AFLA*/ 	return !n->next && n->pprev == &h->first;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Move a list from one list head to another. Fixup the pprev
// /*AFLA*/  * reference of the first entry if it exists.
// /*AFLA*/  */
// /*AFLA*/ static inline void hlist_move_list(struct hlist_head *old,
// /*AFLA*/ 				   struct hlist_head *new)
// /*AFLA*/ {
// /*AFLA*/ 	new->first = old->first;
// /*AFLA*/ 	if (new->first)
// /*AFLA*/ 		new->first->pprev = &new->first;
// /*AFLA*/ 	old->first = NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define hlist_entry(ptr, type, member) container_of(ptr,type,member)
// /*AFLA*/ 
// /*AFLA*/ #define hlist_for_each(pos, head) \
// /*AFLA*/ 	for (pos = (head)->first; pos ; pos = pos->next)
// /*AFLA*/ 
// /*AFLA*/ #define hlist_for_each_safe(pos, n, head) \
// /*AFLA*/ 	for (pos = (head)->first; pos && ({ n = pos->next; 1; }); \
// /*AFLA*/ 	     pos = n)
// /*AFLA*/ 
// /*AFLA*/ #define hlist_entry_safe(ptr, type, member) \
// /*AFLA*/ 	({ typeof(ptr) ____ptr = (ptr); \
// /*AFLA*/ 	   ____ptr ? hlist_entry(____ptr, type, member) : NULL; \
// /*AFLA*/ 	})
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_for_each_entry	- iterate over list of given type
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the hlist_node within the struct.
// /*AFLA*/  */
// /*AFLA*/ #define hlist_for_each_entry(pos, head, member)				\
// /*AFLA*/ 	for (pos = hlist_entry_safe((head)->first, typeof(*(pos)), member);\
// /*AFLA*/ 	     pos;							\
// /*AFLA*/ 	     pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_for_each_entry_continue - iterate over a hlist continuing after current point
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @member:	the name of the hlist_node within the struct.
// /*AFLA*/  */
// /*AFLA*/ #define hlist_for_each_entry_continue(pos, member)			\
// /*AFLA*/ 	for (pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member);\
// /*AFLA*/ 	     pos;							\
// /*AFLA*/ 	     pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_for_each_entry_from - iterate over a hlist continuing from current point
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @member:	the name of the hlist_node within the struct.
// /*AFLA*/  */
// /*AFLA*/ #define hlist_for_each_entry_from(pos, member)				\
// /*AFLA*/ 	for (; pos;							\
// /*AFLA*/ 	     pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @n:		another &struct hlist_node to use as temporary storage
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the hlist_node within the struct.
// /*AFLA*/  */
// /*AFLA*/ #define hlist_for_each_entry_safe(pos, n, head, member) 		\
// /*AFLA*/ 	for (pos = hlist_entry_safe((head)->first, typeof(*pos), member);\
// /*AFLA*/ 	     pos && ({ n = pos->member.next; 1; });			\
// /*AFLA*/ 	     pos = hlist_entry_safe(n, typeof(*pos), member))
// /*AFLA*/ 
// /*AFLA*/ #endif
