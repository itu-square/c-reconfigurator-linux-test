// /*AFLA*/ /*
// /*AFLA*/  * Descending-priority-sorted double-linked list
// /*AFLA*/  *
// /*AFLA*/  * (C) 2002-2003 Intel Corp
// /*AFLA*/  * Inaky Perez-Gonzalez <inaky.perez-gonzalez@intel.com>.
// /*AFLA*/  *
// /*AFLA*/  * 2001-2005 (c) MontaVista Software, Inc.
// /*AFLA*/  * Daniel Walker <dwalker@mvista.com>
// /*AFLA*/  *
// /*AFLA*/  * (C) 2005 Thomas Gleixner <tglx@linutronix.de>
// /*AFLA*/  *
// /*AFLA*/  * Simplifications of the original code by
// /*AFLA*/  * Oleg Nesterov <oleg@tv-sign.ru>
// /*AFLA*/  *
// /*AFLA*/  * Licensed under the FSF's GNU Public License v2 or later.
// /*AFLA*/  *
// /*AFLA*/  * Based on simple lists (include/linux/list.h).
// /*AFLA*/  *
// /*AFLA*/  * This is a priority-sorted list of nodes; each node has a
// /*AFLA*/  * priority from INT_MIN (highest) to INT_MAX (lowest).
// /*AFLA*/  *
// /*AFLA*/  * Addition is O(K), removal is O(1), change of priority of a node is
// /*AFLA*/  * O(K) and K is the number of RT priority levels used in the system.
// /*AFLA*/  * (1 <= K <= 99)
// /*AFLA*/  *
// /*AFLA*/  * This list is really a list of lists:
// /*AFLA*/  *
// /*AFLA*/  *  - The tier 1 list is the prio_list, different priority nodes.
// /*AFLA*/  *
// /*AFLA*/  *  - The tier 2 list is the node_list, serialized nodes.
// /*AFLA*/  *
// /*AFLA*/  * Simple ASCII art explanation:
// /*AFLA*/  *
// /*AFLA*/  * pl:prio_list (only for plist_node)
// /*AFLA*/  * nl:node_list
// /*AFLA*/  *   HEAD|             NODE(S)
// /*AFLA*/  *       |
// /*AFLA*/  *       ||------------------------------------|
// /*AFLA*/  *       ||->|pl|<->|pl|<--------------->|pl|<-|
// /*AFLA*/  *       |   |10|   |21|   |21|   |21|   |40|   (prio)
// /*AFLA*/  *       |   |  |   |  |   |  |   |  |   |  |
// /*AFLA*/  *       |   |  |   |  |   |  |   |  |   |  |
// /*AFLA*/  * |->|nl|<->|nl|<->|nl|<->|nl|<->|nl|<->|nl|<-|
// /*AFLA*/  * |-------------------------------------------|
// /*AFLA*/  *
// /*AFLA*/  * The nodes on the prio_list list are sorted by priority to simplify
// /*AFLA*/  * the insertion of new nodes. There are no nodes with duplicate
// /*AFLA*/  * priorites on the list.
// /*AFLA*/  *
// /*AFLA*/  * The nodes on the node_list are ordered by priority and can contain
// /*AFLA*/  * entries which have the same priority. Those entries are ordered
// /*AFLA*/  * FIFO
// /*AFLA*/  *
// /*AFLA*/  * Addition means: look for the prio_list node in the prio_list
// /*AFLA*/  * for the priority of the node and insert it before the node_list
// /*AFLA*/  * entry of the next prio_list node. If it is the first node of
// /*AFLA*/  * that priority, add it to the prio_list in the right position and
// /*AFLA*/  * insert it into the serialized node_list list
// /*AFLA*/  *
// /*AFLA*/  * Removal means remove it from the node_list and remove it from
// /*AFLA*/  * the prio_list if the node_list list_head is non empty. In case
// /*AFLA*/  * of removal from the prio_list it must be checked whether other
// /*AFLA*/  * entries of the same priority are on the list or not. If there
// /*AFLA*/  * is another entry of the same priority then this entry has to
// /*AFLA*/  * replace the removed entry on the prio_list. If the entry which
// /*AFLA*/  * is removed is the only entry of this priority then a simple
// /*AFLA*/  * remove from both list is sufficient.
// /*AFLA*/  *
// /*AFLA*/  * INT_MIN is the highest priority, 0 is the medium highest, INT_MAX
// /*AFLA*/  * is lowest priority.
// /*AFLA*/  *
// /*AFLA*/  * No locking is done, up to the caller.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LINUX_PLIST_H_
// /*AFLA*/ #define _LINUX_PLIST_H_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ 
// /*AFLA*/ struct plist_head {
// /*AFLA*/ 	struct list_head node_list;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct plist_node {
// /*AFLA*/ 	int			prio;
// /*AFLA*/ 	struct list_head	prio_list;
// /*AFLA*/ 	struct list_head	node_list;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * PLIST_HEAD_INIT - static struct plist_head initializer
// /*AFLA*/  * @head:	struct plist_head variable name
// /*AFLA*/  */
// /*AFLA*/ #define PLIST_HEAD_INIT(head)				\
// /*AFLA*/ {							\
// /*AFLA*/ 	.node_list = LIST_HEAD_INIT((head).node_list)	\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * PLIST_HEAD - declare and init plist_head
// /*AFLA*/  * @head:	name for struct plist_head variable
// /*AFLA*/  */
// /*AFLA*/ #define PLIST_HEAD(head) \
// /*AFLA*/ 	struct plist_head head = PLIST_HEAD_INIT(head)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * PLIST_NODE_INIT - static struct plist_node initializer
// /*AFLA*/  * @node:	struct plist_node variable name
// /*AFLA*/  * @__prio:	initial node priority
// /*AFLA*/  */
// /*AFLA*/ #define PLIST_NODE_INIT(node, __prio)			\
// /*AFLA*/ {							\
// /*AFLA*/ 	.prio  = (__prio),				\
// /*AFLA*/ 	.prio_list = LIST_HEAD_INIT((node).prio_list),	\
// /*AFLA*/ 	.node_list = LIST_HEAD_INIT((node).node_list),	\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * plist_head_init - dynamic struct plist_head initializer
// /*AFLA*/  * @head:	&struct plist_head pointer
// /*AFLA*/  */
// /*AFLA*/ static inline void
// /*AFLA*/ plist_head_init(struct plist_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	INIT_LIST_HEAD(&head->node_list);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * plist_node_init - Dynamic struct plist_node initializer
// /*AFLA*/  * @node:	&struct plist_node pointer
// /*AFLA*/  * @prio:	initial node priority
// /*AFLA*/  */
// /*AFLA*/ static inline void plist_node_init(struct plist_node *node, int prio)
// /*AFLA*/ {
// /*AFLA*/ 	node->prio = prio;
// /*AFLA*/ 	INIT_LIST_HEAD(&node->prio_list);
// /*AFLA*/ 	INIT_LIST_HEAD(&node->node_list);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void plist_add(struct plist_node *node, struct plist_head *head);
// /*AFLA*/ extern void plist_del(struct plist_node *node, struct plist_head *head);
// /*AFLA*/ 
// /*AFLA*/ extern void plist_requeue(struct plist_node *node, struct plist_head *head);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * plist_for_each - iterate over the plist
// /*AFLA*/  * @pos:	the type * to use as a loop counter
// /*AFLA*/  * @head:	the head for your list
// /*AFLA*/  */
// /*AFLA*/ #define plist_for_each(pos, head)	\
// /*AFLA*/ 	 list_for_each_entry(pos, &(head)->node_list, node_list)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * plist_for_each_continue - continue iteration over the plist
// /*AFLA*/  * @pos:	the type * to use as a loop cursor
// /*AFLA*/  * @head:	the head for your list
// /*AFLA*/  *
// /*AFLA*/  * Continue to iterate over plist, continuing after the current position.
// /*AFLA*/  */
// /*AFLA*/ #define plist_for_each_continue(pos, head)	\
// /*AFLA*/ 	 list_for_each_entry_continue(pos, &(head)->node_list, node_list)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * plist_for_each_safe - iterate safely over a plist of given type
// /*AFLA*/  * @pos:	the type * to use as a loop counter
// /*AFLA*/  * @n:	another type * to use as temporary storage
// /*AFLA*/  * @head:	the head for your list
// /*AFLA*/  *
// /*AFLA*/  * Iterate over a plist of given type, safe against removal of list entry.
// /*AFLA*/  */
// /*AFLA*/ #define plist_for_each_safe(pos, n, head)	\
// /*AFLA*/ 	 list_for_each_entry_safe(pos, n, &(head)->node_list, node_list)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * plist_for_each_entry	- iterate over list of given type
// /*AFLA*/  * @pos:	the type * to use as a loop counter
// /*AFLA*/  * @head:	the head for your list
// /*AFLA*/  * @mem:	the name of the list_head within the struct
// /*AFLA*/  */
// /*AFLA*/ #define plist_for_each_entry(pos, head, mem)	\
// /*AFLA*/ 	 list_for_each_entry(pos, &(head)->node_list, mem.node_list)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * plist_for_each_entry_continue - continue iteration over list of given type
// /*AFLA*/  * @pos:	the type * to use as a loop cursor
// /*AFLA*/  * @head:	the head for your list
// /*AFLA*/  * @m:		the name of the list_head within the struct
// /*AFLA*/  *
// /*AFLA*/  * Continue to iterate over list of given type, continuing after
// /*AFLA*/  * the current position.
// /*AFLA*/  */
// /*AFLA*/ #define plist_for_each_entry_continue(pos, head, m)	\
// /*AFLA*/ 	list_for_each_entry_continue(pos, &(head)->node_list, m.node_list)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * plist_for_each_entry_safe - iterate safely over list of given type
// /*AFLA*/  * @pos:	the type * to use as a loop counter
// /*AFLA*/  * @n:		another type * to use as temporary storage
// /*AFLA*/  * @head:	the head for your list
// /*AFLA*/  * @m:		the name of the list_head within the struct
// /*AFLA*/  *
// /*AFLA*/  * Iterate over list of given type, safe against removal of list entry.
// /*AFLA*/  */
// /*AFLA*/ #define plist_for_each_entry_safe(pos, n, head, m)	\
// /*AFLA*/ 	list_for_each_entry_safe(pos, n, &(head)->node_list, m.node_list)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * plist_head_empty - return !0 if a plist_head is empty
// /*AFLA*/  * @head:	&struct plist_head pointer
// /*AFLA*/  */
// /*AFLA*/ static inline int plist_head_empty(const struct plist_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	return list_empty(&head->node_list);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * plist_node_empty - return !0 if plist_node is not on a list
// /*AFLA*/  * @node:	&struct plist_node pointer
// /*AFLA*/  */
// /*AFLA*/ static inline int plist_node_empty(const struct plist_node *node)
// /*AFLA*/ {
// /*AFLA*/ 	return list_empty(&node->node_list);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* All functions below assume the plist_head is not empty. */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * plist_first_entry - get the struct for the first entry
// /*AFLA*/  * @head:	the &struct plist_head pointer
// /*AFLA*/  * @type:	the type of the struct this is embedded in
// /*AFLA*/  * @member:	the name of the list_head within the struct
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_DEBUG_PI_LIST
// /*AFLA*/ # define plist_first_entry(head, type, member)	\
// /*AFLA*/ ({ \
// /*AFLA*/ 	WARN_ON(plist_head_empty(head)); \
// /*AFLA*/ 	container_of(plist_first(head), type, member); \
// /*AFLA*/ })
// /*AFLA*/ #else
// /*AFLA*/ # define plist_first_entry(head, type, member)	\
// /*AFLA*/ 	container_of(plist_first(head), type, member)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * plist_last_entry - get the struct for the last entry
// /*AFLA*/  * @head:	the &struct plist_head pointer
// /*AFLA*/  * @type:	the type of the struct this is embedded in
// /*AFLA*/  * @member:	the name of the list_head within the struct
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_DEBUG_PI_LIST
// /*AFLA*/ # define plist_last_entry(head, type, member)	\
// /*AFLA*/ ({ \
// /*AFLA*/ 	WARN_ON(plist_head_empty(head)); \
// /*AFLA*/ 	container_of(plist_last(head), type, member); \
// /*AFLA*/ })
// /*AFLA*/ #else
// /*AFLA*/ # define plist_last_entry(head, type, member)	\
// /*AFLA*/ 	container_of(plist_last(head), type, member)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * plist_next - get the next entry in list
// /*AFLA*/  * @pos:	the type * to cursor
// /*AFLA*/  */
// /*AFLA*/ #define plist_next(pos) \
// /*AFLA*/ 	list_next_entry(pos, node_list)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * plist_prev - get the prev entry in list
// /*AFLA*/  * @pos:	the type * to cursor
// /*AFLA*/  */
// /*AFLA*/ #define plist_prev(pos) \
// /*AFLA*/ 	list_prev_entry(pos, node_list)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * plist_first - return the first node (and thus, highest priority)
// /*AFLA*/  * @head:	the &struct plist_head pointer
// /*AFLA*/  *
// /*AFLA*/  * Assumes the plist is _not_ empty.
// /*AFLA*/  */
// /*AFLA*/ static inline struct plist_node *plist_first(const struct plist_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	return list_entry(head->node_list.next,
// /*AFLA*/ 			  struct plist_node, node_list);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * plist_last - return the last node (and thus, lowest priority)
// /*AFLA*/  * @head:	the &struct plist_head pointer
// /*AFLA*/  *
// /*AFLA*/  * Assumes the plist is _not_ empty.
// /*AFLA*/  */
// /*AFLA*/ static inline struct plist_node *plist_last(const struct plist_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	return list_entry(head->node_list.prev,
// /*AFLA*/ 			  struct plist_node, node_list);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
