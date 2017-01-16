// /*AFLA*/ #ifndef LLIST_H
// /*AFLA*/ #define LLIST_H
// /*AFLA*/ /*
// /*AFLA*/  * Lock-less NULL terminated single linked list
// /*AFLA*/  *
// /*AFLA*/  * If there are multiple producers and multiple consumers, llist_add
// /*AFLA*/  * can be used in producers and llist_del_all can be used in
// /*AFLA*/  * consumers.  They can work simultaneously without lock.  But
// /*AFLA*/  * llist_del_first can not be used here.  Because llist_del_first
// /*AFLA*/  * depends on list->first->next does not changed if list->first is not
// /*AFLA*/  * changed during its operation, but llist_del_first, llist_add,
// /*AFLA*/  * llist_add (or llist_del_all, llist_add, llist_add) sequence in
// /*AFLA*/  * another consumer may violate that.
// /*AFLA*/  *
// /*AFLA*/  * If there are multiple producers and one consumer, llist_add can be
// /*AFLA*/  * used in producers and llist_del_all or llist_del_first can be used
// /*AFLA*/  * in the consumer.
// /*AFLA*/  *
// /*AFLA*/  * This can be summarized as follow:
// /*AFLA*/  *
// /*AFLA*/  *           |   add    | del_first |  del_all
// /*AFLA*/  * add       |    -     |     -     |     -
// /*AFLA*/  * del_first |          |     L     |     L
// /*AFLA*/  * del_all   |          |           |     -
// /*AFLA*/  *
// /*AFLA*/  * Where "-" stands for no lock is needed, while "L" stands for lock
// /*AFLA*/  * is needed.
// /*AFLA*/  *
// /*AFLA*/  * The list entries deleted via llist_del_all can be traversed with
// /*AFLA*/  * traversing function such as llist_for_each etc.  But the list
// /*AFLA*/  * entries can not be traversed safely before deleted from the list.
// /*AFLA*/  * The order of deleted entries is from the newest to the oldest added
// /*AFLA*/  * one.  If you want to traverse from the oldest to the newest, you
// /*AFLA*/  * must reverse the order by yourself before traversing.
// /*AFLA*/  *
// /*AFLA*/  * The basic atomic operation of this list is cmpxchg on long.  On
// /*AFLA*/  * architectures that don't have NMI-safe cmpxchg implementation, the
// /*AFLA*/  * list can NOT be used in NMI handlers.  So code that uses the list in
// /*AFLA*/  * an NMI handler should depend on CONFIG_ARCH_HAVE_NMI_SAFE_CMPXCHG.
// /*AFLA*/  *
// /*AFLA*/  * Copyright 2010,2011 Intel Corp.
// /*AFLA*/  *   Author: Huang Ying <ying.huang@intel.com>
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or
// /*AFLA*/  * modify it under the terms of the GNU General Public License version
// /*AFLA*/  * 2 as published by the Free Software Foundation;
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it will be useful,
// /*AFLA*/  * but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/  * GNU General Public License for more details.
// /*AFLA*/  *
// /*AFLA*/  * You should have received a copy of the GNU General Public License
// /*AFLA*/  * along with this program; if not, write to the Free Software
// /*AFLA*/  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ 
// /*AFLA*/ struct llist_head {
// /*AFLA*/ 	struct llist_node *first;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct llist_node {
// /*AFLA*/ 	struct llist_node *next;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define LLIST_HEAD_INIT(name)	{ NULL }
// /*AFLA*/ #define LLIST_HEAD(name)	struct llist_head name = LLIST_HEAD_INIT(name)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * init_llist_head - initialize lock-less list head
// /*AFLA*/  * @head:	the head for your lock-less list
// /*AFLA*/  */
// /*AFLA*/ static inline void init_llist_head(struct llist_head *list)
// /*AFLA*/ {
// /*AFLA*/ 	list->first = NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * llist_entry - get the struct of this entry
// /*AFLA*/  * @ptr:	the &struct llist_node pointer.
// /*AFLA*/  * @type:	the type of the struct this is embedded in.
// /*AFLA*/  * @member:	the name of the llist_node within the struct.
// /*AFLA*/  */
// /*AFLA*/ #define llist_entry(ptr, type, member)		\
// /*AFLA*/ 	container_of(ptr, type, member)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * llist_for_each - iterate over some deleted entries of a lock-less list
// /*AFLA*/  * @pos:	the &struct llist_node to use as a loop cursor
// /*AFLA*/  * @node:	the first entry of deleted list entries
// /*AFLA*/  *
// /*AFLA*/  * In general, some entries of the lock-less list can be traversed
// /*AFLA*/  * safely only after being deleted from list, so start with an entry
// /*AFLA*/  * instead of list head.
// /*AFLA*/  *
// /*AFLA*/  * If being used on entries deleted from lock-less list directly, the
// /*AFLA*/  * traverse order is from the newest to the oldest added entry.  If
// /*AFLA*/  * you want to traverse from the oldest to the newest, you must
// /*AFLA*/  * reverse the order by yourself before traversing.
// /*AFLA*/  */
// /*AFLA*/ #define llist_for_each(pos, node)			\
// /*AFLA*/ 	for ((pos) = (node); pos; (pos) = (pos)->next)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * llist_for_each_entry - iterate over some deleted entries of lock-less list of given type
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @node:	the fist entry of deleted list entries.
// /*AFLA*/  * @member:	the name of the llist_node with the struct.
// /*AFLA*/  *
// /*AFLA*/  * In general, some entries of the lock-less list can be traversed
// /*AFLA*/  * safely only after being removed from list, so start with an entry
// /*AFLA*/  * instead of list head.
// /*AFLA*/  *
// /*AFLA*/  * If being used on entries deleted from lock-less list directly, the
// /*AFLA*/  * traverse order is from the newest to the oldest added entry.  If
// /*AFLA*/  * you want to traverse from the oldest to the newest, you must
// /*AFLA*/  * reverse the order by yourself before traversing.
// /*AFLA*/  */
// /*AFLA*/ #define llist_for_each_entry(pos, node, member)				\
// /*AFLA*/ 	for ((pos) = llist_entry((node), typeof(*(pos)), member);	\
// /*AFLA*/ 	     &(pos)->member != NULL;					\
// /*AFLA*/ 	     (pos) = llist_entry((pos)->member.next, typeof(*(pos)), member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * llist_for_each_entry_safe - iterate over some deleted entries of lock-less list of given type
// /*AFLA*/  *			       safe against removal of list entry
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @n:		another type * to use as temporary storage
// /*AFLA*/  * @node:	the first entry of deleted list entries.
// /*AFLA*/  * @member:	the name of the llist_node with the struct.
// /*AFLA*/  *
// /*AFLA*/  * In general, some entries of the lock-less list can be traversed
// /*AFLA*/  * safely only after being removed from list, so start with an entry
// /*AFLA*/  * instead of list head.
// /*AFLA*/  *
// /*AFLA*/  * If being used on entries deleted from lock-less list directly, the
// /*AFLA*/  * traverse order is from the newest to the oldest added entry.  If
// /*AFLA*/  * you want to traverse from the oldest to the newest, you must
// /*AFLA*/  * reverse the order by yourself before traversing.
// /*AFLA*/  */
// /*AFLA*/ #define llist_for_each_entry_safe(pos, n, node, member)			       \
// /*AFLA*/ 	for (pos = llist_entry((node), typeof(*pos), member);		       \
// /*AFLA*/ 	     &pos->member != NULL &&					       \
// /*AFLA*/ 	        (n = llist_entry(pos->member.next, typeof(*n), member), true); \
// /*AFLA*/ 	     pos = n)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * llist_empty - tests whether a lock-less list is empty
// /*AFLA*/  * @head:	the list to test
// /*AFLA*/  *
// /*AFLA*/  * Not guaranteed to be accurate or up to date.  Just a quick way to
// /*AFLA*/  * test whether the list is empty without deleting something from the
// /*AFLA*/  * list.
// /*AFLA*/  */
// /*AFLA*/ static inline bool llist_empty(const struct llist_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	return ACCESS_ONCE(head->first) == NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct llist_node *llist_next(struct llist_node *node)
// /*AFLA*/ {
// /*AFLA*/ 	return node->next;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern bool llist_add_batch(struct llist_node *new_first,
// /*AFLA*/ 			    struct llist_node *new_last,
// /*AFLA*/ 			    struct llist_head *head);
// /*AFLA*/ /**
// /*AFLA*/  * llist_add - add a new entry
// /*AFLA*/  * @new:	new entry to be added
// /*AFLA*/  * @head:	the head for your lock-less list
// /*AFLA*/  *
// /*AFLA*/  * Returns true if the list was empty prior to adding this entry.
// /*AFLA*/  */
// /*AFLA*/ static inline bool llist_add(struct llist_node *new, struct llist_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	return llist_add_batch(new, new, head);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * llist_del_all - delete all entries from lock-less list
// /*AFLA*/  * @head:	the head of lock-less list to delete all entries
// /*AFLA*/  *
// /*AFLA*/  * If list is empty, return NULL, otherwise, delete all entries and
// /*AFLA*/  * return the pointer to the first entry.  The order of entries
// /*AFLA*/  * deleted is from the newest to the oldest added one.
// /*AFLA*/  */
// /*AFLA*/ static inline struct llist_node *llist_del_all(struct llist_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	return xchg(&head->first, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct llist_node *llist_del_first(struct llist_head *head);
// /*AFLA*/ 
// /*AFLA*/ struct llist_node *llist_reverse_order(struct llist_node *head);
// /*AFLA*/ 
// /*AFLA*/ #endif /* LLIST_H */
