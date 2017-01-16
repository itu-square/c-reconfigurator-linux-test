// /*AFLA*/ #ifndef _LINUX_LIST_BL_H
// /*AFLA*/ #define _LINUX_LIST_BL_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/bit_spinlock.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Special version of lists, where head of the list has a lock in the lowest
// /*AFLA*/  * bit. This is useful for scalable hash tables without increasing memory
// /*AFLA*/  * footprint overhead.
// /*AFLA*/  *
// /*AFLA*/  * For modification operations, the 0 bit of hlist_bl_head->first
// /*AFLA*/  * pointer must be set.
// /*AFLA*/  *
// /*AFLA*/  * With some small modifications, this can easily be adapted to store several
// /*AFLA*/  * arbitrary bits (not just a single lock bit), if the need arises to store
// /*AFLA*/  * some fast and compact auxiliary data.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK)
// /*AFLA*/ #define LIST_BL_LOCKMASK	1UL
// /*AFLA*/ #else
// /*AFLA*/ #define LIST_BL_LOCKMASK	0UL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LIST
// /*AFLA*/ #define LIST_BL_BUG_ON(x) BUG_ON(x)
// /*AFLA*/ #else
// /*AFLA*/ #define LIST_BL_BUG_ON(x)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ struct hlist_bl_head {
// /*AFLA*/ 	struct hlist_bl_node *first;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct hlist_bl_node {
// /*AFLA*/ 	struct hlist_bl_node *next, **pprev;
// /*AFLA*/ };
// /*AFLA*/ #define INIT_HLIST_BL_HEAD(ptr) \
// /*AFLA*/ 	((ptr)->first = NULL)
// /*AFLA*/ 
// /*AFLA*/ static inline void INIT_HLIST_BL_NODE(struct hlist_bl_node *h)
// /*AFLA*/ {
// /*AFLA*/ 	h->next = NULL;
// /*AFLA*/ 	h->pprev = NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define hlist_bl_entry(ptr, type, member) container_of(ptr,type,member)
// /*AFLA*/ 
// /*AFLA*/ static inline bool  hlist_bl_unhashed(const struct hlist_bl_node *h)
// /*AFLA*/ {
// /*AFLA*/ 	return !h->pprev;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct hlist_bl_node *hlist_bl_first(struct hlist_bl_head *h)
// /*AFLA*/ {
// /*AFLA*/ 	return (struct hlist_bl_node *)
// /*AFLA*/ 		((unsigned long)h->first & ~LIST_BL_LOCKMASK);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void hlist_bl_set_first(struct hlist_bl_head *h,
// /*AFLA*/ 					struct hlist_bl_node *n)
// /*AFLA*/ {
// /*AFLA*/ 	LIST_BL_BUG_ON((unsigned long)n & LIST_BL_LOCKMASK);
// /*AFLA*/ 	LIST_BL_BUG_ON(((unsigned long)h->first & LIST_BL_LOCKMASK) !=
// /*AFLA*/ 							LIST_BL_LOCKMASK);
// /*AFLA*/ 	h->first = (struct hlist_bl_node *)((unsigned long)n | LIST_BL_LOCKMASK);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool hlist_bl_empty(const struct hlist_bl_head *h)
// /*AFLA*/ {
// /*AFLA*/ 	return !((unsigned long)READ_ONCE(h->first) & ~LIST_BL_LOCKMASK);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void hlist_bl_add_head(struct hlist_bl_node *n,
// /*AFLA*/ 					struct hlist_bl_head *h)
// /*AFLA*/ {
// /*AFLA*/ 	struct hlist_bl_node *first = hlist_bl_first(h);
// /*AFLA*/ 
// /*AFLA*/ 	n->next = first;
// /*AFLA*/ 	if (first)
// /*AFLA*/ 		first->pprev = &n->next;
// /*AFLA*/ 	n->pprev = &h->first;
// /*AFLA*/ 	hlist_bl_set_first(h, n);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __hlist_bl_del(struct hlist_bl_node *n)
// /*AFLA*/ {
// /*AFLA*/ 	struct hlist_bl_node *next = n->next;
// /*AFLA*/ 	struct hlist_bl_node **pprev = n->pprev;
// /*AFLA*/ 
// /*AFLA*/ 	LIST_BL_BUG_ON((unsigned long)n & LIST_BL_LOCKMASK);
// /*AFLA*/ 
// /*AFLA*/ 	/* pprev may be `first`, so be careful not to lose the lock bit */
// /*AFLA*/ 	WRITE_ONCE(*pprev,
// /*AFLA*/ 		   (struct hlist_bl_node *)
// /*AFLA*/ 			((unsigned long)next |
// /*AFLA*/ 			 ((unsigned long)*pprev & LIST_BL_LOCKMASK)));
// /*AFLA*/ 	if (next)
// /*AFLA*/ 		next->pprev = pprev;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void hlist_bl_del(struct hlist_bl_node *n)
// /*AFLA*/ {
// /*AFLA*/ 	__hlist_bl_del(n);
// /*AFLA*/ 	n->next = LIST_POISON1;
// /*AFLA*/ 	n->pprev = LIST_POISON2;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void hlist_bl_del_init(struct hlist_bl_node *n)
// /*AFLA*/ {
// /*AFLA*/ 	if (!hlist_bl_unhashed(n)) {
// /*AFLA*/ 		__hlist_bl_del(n);
// /*AFLA*/ 		INIT_HLIST_BL_NODE(n);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void hlist_bl_lock(struct hlist_bl_head *b)
// /*AFLA*/ {
// /*AFLA*/ 	bit_spin_lock(0, (unsigned long *)b);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void hlist_bl_unlock(struct hlist_bl_head *b)
// /*AFLA*/ {
// /*AFLA*/ 	__bit_spin_unlock(0, (unsigned long *)b);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool hlist_bl_is_locked(struct hlist_bl_head *b)
// /*AFLA*/ {
// /*AFLA*/ 	return bit_spin_is_locked(0, (unsigned long *)b);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_bl_for_each_entry	- iterate over list of given type
// /*AFLA*/  * @tpos:	the type * to use as a loop cursor.
// /*AFLA*/  * @pos:	the &struct hlist_node to use as a loop cursor.
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the hlist_node within the struct.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ #define hlist_bl_for_each_entry(tpos, pos, head, member)		\
// /*AFLA*/ 	for (pos = hlist_bl_first(head);				\
// /*AFLA*/ 	     pos &&							\
// /*AFLA*/ 		({ tpos = hlist_bl_entry(pos, typeof(*tpos), member); 1;}); \
// /*AFLA*/ 	     pos = pos->next)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_bl_for_each_entry_safe - iterate over list of given type safe against removal of list entry
// /*AFLA*/  * @tpos:	the type * to use as a loop cursor.
// /*AFLA*/  * @pos:	the &struct hlist_node to use as a loop cursor.
// /*AFLA*/  * @n:		another &struct hlist_node to use as temporary storage
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the hlist_node within the struct.
// /*AFLA*/  */
// /*AFLA*/ #define hlist_bl_for_each_entry_safe(tpos, pos, n, head, member)	 \
// /*AFLA*/ 	for (pos = hlist_bl_first(head);				 \
// /*AFLA*/ 	     pos && ({ n = pos->next; 1; }) && 				 \
// /*AFLA*/ 		({ tpos = hlist_bl_entry(pos, typeof(*tpos), member); 1;}); \
// /*AFLA*/ 	     pos = n)
// /*AFLA*/ 
// /*AFLA*/ #endif
