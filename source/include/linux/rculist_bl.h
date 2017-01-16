// /*AFLA*/ #ifndef _LINUX_RCULIST_BL_H
// /*AFLA*/ #define _LINUX_RCULIST_BL_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * RCU-protected bl list version. See include/linux/list_bl.h.
// /*AFLA*/  */
// /*AFLA*/ #include <linux/list_bl.h>
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ 
// /*AFLA*/ static inline void hlist_bl_set_first_rcu(struct hlist_bl_head *h,
// /*AFLA*/ 					struct hlist_bl_node *n)
// /*AFLA*/ {
// /*AFLA*/ 	LIST_BL_BUG_ON((unsigned long)n & LIST_BL_LOCKMASK);
// /*AFLA*/ 	LIST_BL_BUG_ON(((unsigned long)h->first & LIST_BL_LOCKMASK) !=
// /*AFLA*/ 							LIST_BL_LOCKMASK);
// /*AFLA*/ 	rcu_assign_pointer(h->first,
// /*AFLA*/ 		(struct hlist_bl_node *)((unsigned long)n | LIST_BL_LOCKMASK));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct hlist_bl_node *hlist_bl_first_rcu(struct hlist_bl_head *h)
// /*AFLA*/ {
// /*AFLA*/ 	return (struct hlist_bl_node *)
// /*AFLA*/ 		((unsigned long)rcu_dereference_check(h->first, hlist_bl_is_locked(h)) & ~LIST_BL_LOCKMASK);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_bl_del_init_rcu - deletes entry from hash list with re-initialization
// /*AFLA*/  * @n: the element to delete from the hash list.
// /*AFLA*/  *
// /*AFLA*/  * Note: hlist_bl_unhashed() on the node returns true after this. It is
// /*AFLA*/  * useful for RCU based read lockfree traversal if the writer side
// /*AFLA*/  * must know if the list entry is still hashed or already unhashed.
// /*AFLA*/  *
// /*AFLA*/  * In particular, it means that we can not poison the forward pointers
// /*AFLA*/  * that may still be used for walking the hash list and we can only
// /*AFLA*/  * zero the pprev pointer so list_unhashed() will return true after
// /*AFLA*/  * this.
// /*AFLA*/  *
// /*AFLA*/  * The caller must take whatever precautions are necessary (such as
// /*AFLA*/  * holding appropriate locks) to avoid racing with another
// /*AFLA*/  * list-mutation primitive, such as hlist_bl_add_head_rcu() or
// /*AFLA*/  * hlist_bl_del_rcu(), running on this same list.  However, it is
// /*AFLA*/  * perfectly legal to run concurrently with the _rcu list-traversal
// /*AFLA*/  * primitives, such as hlist_bl_for_each_entry_rcu().
// /*AFLA*/  */
// /*AFLA*/ static inline void hlist_bl_del_init_rcu(struct hlist_bl_node *n)
// /*AFLA*/ {
// /*AFLA*/ 	if (!hlist_bl_unhashed(n)) {
// /*AFLA*/ 		__hlist_bl_del(n);
// /*AFLA*/ 		n->pprev = NULL;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_bl_del_rcu - deletes entry from hash list without re-initialization
// /*AFLA*/  * @n: the element to delete from the hash list.
// /*AFLA*/  *
// /*AFLA*/  * Note: hlist_bl_unhashed() on entry does not return true after this,
// /*AFLA*/  * the entry is in an undefined state. It is useful for RCU based
// /*AFLA*/  * lockfree traversal.
// /*AFLA*/  *
// /*AFLA*/  * In particular, it means that we can not poison the forward
// /*AFLA*/  * pointers that may still be used for walking the hash list.
// /*AFLA*/  *
// /*AFLA*/  * The caller must take whatever precautions are necessary
// /*AFLA*/  * (such as holding appropriate locks) to avoid racing
// /*AFLA*/  * with another list-mutation primitive, such as hlist_bl_add_head_rcu()
// /*AFLA*/  * or hlist_bl_del_rcu(), running on this same list.
// /*AFLA*/  * However, it is perfectly legal to run concurrently with
// /*AFLA*/  * the _rcu list-traversal primitives, such as
// /*AFLA*/  * hlist_bl_for_each_entry().
// /*AFLA*/  */
// /*AFLA*/ static inline void hlist_bl_del_rcu(struct hlist_bl_node *n)
// /*AFLA*/ {
// /*AFLA*/ 	__hlist_bl_del(n);
// /*AFLA*/ 	n->pprev = LIST_POISON2;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_bl_add_head_rcu
// /*AFLA*/  * @n: the element to add to the hash list.
// /*AFLA*/  * @h: the list to add to.
// /*AFLA*/  *
// /*AFLA*/  * Description:
// /*AFLA*/  * Adds the specified element to the specified hlist_bl,
// /*AFLA*/  * while permitting racing traversals.
// /*AFLA*/  *
// /*AFLA*/  * The caller must take whatever precautions are necessary
// /*AFLA*/  * (such as holding appropriate locks) to avoid racing
// /*AFLA*/  * with another list-mutation primitive, such as hlist_bl_add_head_rcu()
// /*AFLA*/  * or hlist_bl_del_rcu(), running on this same list.
// /*AFLA*/  * However, it is perfectly legal to run concurrently with
// /*AFLA*/  * the _rcu list-traversal primitives, such as
// /*AFLA*/  * hlist_bl_for_each_entry_rcu(), used to prevent memory-consistency
// /*AFLA*/  * problems on Alpha CPUs.  Regardless of the type of CPU, the
// /*AFLA*/  * list-traversal primitive must be guarded by rcu_read_lock().
// /*AFLA*/  */
// /*AFLA*/ static inline void hlist_bl_add_head_rcu(struct hlist_bl_node *n,
// /*AFLA*/ 					struct hlist_bl_head *h)
// /*AFLA*/ {
// /*AFLA*/ 	struct hlist_bl_node *first;
// /*AFLA*/ 
// /*AFLA*/ 	/* don't need hlist_bl_first_rcu because we're under lock */
// /*AFLA*/ 	first = hlist_bl_first(h);
// /*AFLA*/ 
// /*AFLA*/ 	n->next = first;
// /*AFLA*/ 	if (first)
// /*AFLA*/ 		first->pprev = &n->next;
// /*AFLA*/ 	n->pprev = &h->first;
// /*AFLA*/ 
// /*AFLA*/ 	/* need _rcu because we can have concurrent lock free readers */
// /*AFLA*/ 	hlist_bl_set_first_rcu(h, n);
// /*AFLA*/ }
// /*AFLA*/ /**
// /*AFLA*/  * hlist_bl_for_each_entry_rcu - iterate over rcu list of given type
// /*AFLA*/  * @tpos:	the type * to use as a loop cursor.
// /*AFLA*/  * @pos:	the &struct hlist_bl_node to use as a loop cursor.
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the hlist_bl_node within the struct.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ #define hlist_bl_for_each_entry_rcu(tpos, pos, head, member)		\
// /*AFLA*/ 	for (pos = hlist_bl_first_rcu(head);				\
// /*AFLA*/ 		pos &&							\
// /*AFLA*/ 		({ tpos = hlist_bl_entry(pos, typeof(*tpos), member); 1; }); \
// /*AFLA*/ 		pos = rcu_dereference_raw(pos->next))
// /*AFLA*/ 
// /*AFLA*/ #endif
