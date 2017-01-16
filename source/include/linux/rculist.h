// /*AFLA*/ #ifndef _LINUX_RCULIST_H
// /*AFLA*/ #define _LINUX_RCULIST_H
// /*AFLA*/ 
// /*AFLA*/ #ifdef __KERNEL__
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * RCU-protected list version
// /*AFLA*/  */
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Why is there no list_empty_rcu()?  Because list_empty() serves this
// /*AFLA*/  * purpose.  The list_empty() function fetches the RCU-protected pointer
// /*AFLA*/  * and compares it to the address of the list head, but neither dereferences
// /*AFLA*/  * this pointer itself nor provides this pointer to the caller.  Therefore,
// /*AFLA*/  * it is not necessary to use rcu_dereference(), so that list_empty() can
// /*AFLA*/  * be used anywhere you would want to use a list_empty_rcu().
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * INIT_LIST_HEAD_RCU - Initialize a list_head visible to RCU readers
// /*AFLA*/  * @list: list to be initialized
// /*AFLA*/  *
// /*AFLA*/  * You should instead use INIT_LIST_HEAD() for normal initialization and
// /*AFLA*/  * cleanup tasks, when readers have no access to the list being initialized.
// /*AFLA*/  * However, if the list being initialized is visible to readers, you
// /*AFLA*/  * need to keep the compiler from being too mischievous.
// /*AFLA*/  */
// /*AFLA*/ static inline void INIT_LIST_HEAD_RCU(struct list_head *list)
// /*AFLA*/ {
// /*AFLA*/ 	WRITE_ONCE(list->next, list);
// /*AFLA*/ 	WRITE_ONCE(list->prev, list);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * return the ->next pointer of a list_head in an rcu safe
// /*AFLA*/  * way, we must not access it directly
// /*AFLA*/  */
// /*AFLA*/ #define list_next_rcu(list)	(*((struct list_head __rcu **)(&(list)->next)))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Insert a new entry between two known consecutive entries.
// /*AFLA*/  *
// /*AFLA*/  * This is only for internal list manipulation where we know
// /*AFLA*/  * the prev/next entries already!
// /*AFLA*/  */
// /*AFLA*/ #ifndef CONFIG_DEBUG_LIST
// /*AFLA*/ static inline void __list_add_rcu(struct list_head *new,
// /*AFLA*/ 		struct list_head *prev, struct list_head *next)
// /*AFLA*/ {
// /*AFLA*/ 	new->next = next;
// /*AFLA*/ 	new->prev = prev;
// /*AFLA*/ 	rcu_assign_pointer(list_next_rcu(prev), new);
// /*AFLA*/ 	next->prev = new;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ void __list_add_rcu(struct list_head *new,
// /*AFLA*/ 		    struct list_head *prev, struct list_head *next);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_add_rcu - add a new entry to rcu-protected list
// /*AFLA*/  * @new: new entry to be added
// /*AFLA*/  * @head: list head to add it after
// /*AFLA*/  *
// /*AFLA*/  * Insert a new entry after the specified head.
// /*AFLA*/  * This is good for implementing stacks.
// /*AFLA*/  *
// /*AFLA*/  * The caller must take whatever precautions are necessary
// /*AFLA*/  * (such as holding appropriate locks) to avoid racing
// /*AFLA*/  * with another list-mutation primitive, such as list_add_rcu()
// /*AFLA*/  * or list_del_rcu(), running on this same list.
// /*AFLA*/  * However, it is perfectly legal to run concurrently with
// /*AFLA*/  * the _rcu list-traversal primitives, such as
// /*AFLA*/  * list_for_each_entry_rcu().
// /*AFLA*/  */
// /*AFLA*/ static inline void list_add_rcu(struct list_head *new, struct list_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	__list_add_rcu(new, head, head->next);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_add_tail_rcu - add a new entry to rcu-protected list
// /*AFLA*/  * @new: new entry to be added
// /*AFLA*/  * @head: list head to add it before
// /*AFLA*/  *
// /*AFLA*/  * Insert a new entry before the specified head.
// /*AFLA*/  * This is useful for implementing queues.
// /*AFLA*/  *
// /*AFLA*/  * The caller must take whatever precautions are necessary
// /*AFLA*/  * (such as holding appropriate locks) to avoid racing
// /*AFLA*/  * with another list-mutation primitive, such as list_add_tail_rcu()
// /*AFLA*/  * or list_del_rcu(), running on this same list.
// /*AFLA*/  * However, it is perfectly legal to run concurrently with
// /*AFLA*/  * the _rcu list-traversal primitives, such as
// /*AFLA*/  * list_for_each_entry_rcu().
// /*AFLA*/  */
// /*AFLA*/ static inline void list_add_tail_rcu(struct list_head *new,
// /*AFLA*/ 					struct list_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	__list_add_rcu(new, head->prev, head);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_del_rcu - deletes entry from list without re-initialization
// /*AFLA*/  * @entry: the element to delete from the list.
// /*AFLA*/  *
// /*AFLA*/  * Note: list_empty() on entry does not return true after this,
// /*AFLA*/  * the entry is in an undefined state. It is useful for RCU based
// /*AFLA*/  * lockfree traversal.
// /*AFLA*/  *
// /*AFLA*/  * In particular, it means that we can not poison the forward
// /*AFLA*/  * pointers that may still be used for walking the list.
// /*AFLA*/  *
// /*AFLA*/  * The caller must take whatever precautions are necessary
// /*AFLA*/  * (such as holding appropriate locks) to avoid racing
// /*AFLA*/  * with another list-mutation primitive, such as list_del_rcu()
// /*AFLA*/  * or list_add_rcu(), running on this same list.
// /*AFLA*/  * However, it is perfectly legal to run concurrently with
// /*AFLA*/  * the _rcu list-traversal primitives, such as
// /*AFLA*/  * list_for_each_entry_rcu().
// /*AFLA*/  *
// /*AFLA*/  * Note that the caller is not permitted to immediately free
// /*AFLA*/  * the newly deleted entry.  Instead, either synchronize_rcu()
// /*AFLA*/  * or call_rcu() must be used to defer freeing until an RCU
// /*AFLA*/  * grace period has elapsed.
// /*AFLA*/  */
// /*AFLA*/ static inline void list_del_rcu(struct list_head *entry)
// /*AFLA*/ {
// /*AFLA*/ 	__list_del_entry(entry);
// /*AFLA*/ 	entry->prev = LIST_POISON2;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_del_init_rcu - deletes entry from hash list with re-initialization
// /*AFLA*/  * @n: the element to delete from the hash list.
// /*AFLA*/  *
// /*AFLA*/  * Note: list_unhashed() on the node return true after this. It is
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
// /*AFLA*/  * list-mutation primitive, such as hlist_add_head_rcu() or
// /*AFLA*/  * hlist_del_rcu(), running on this same list.  However, it is
// /*AFLA*/  * perfectly legal to run concurrently with the _rcu list-traversal
// /*AFLA*/  * primitives, such as hlist_for_each_entry_rcu().
// /*AFLA*/  */
// /*AFLA*/ static inline void hlist_del_init_rcu(struct hlist_node *n)
// /*AFLA*/ {
// /*AFLA*/ 	if (!hlist_unhashed(n)) {
// /*AFLA*/ 		__hlist_del(n);
// /*AFLA*/ 		n->pprev = NULL;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_replace_rcu - replace old entry by new one
// /*AFLA*/  * @old : the element to be replaced
// /*AFLA*/  * @new : the new element to insert
// /*AFLA*/  *
// /*AFLA*/  * The @old entry will be replaced with the @new entry atomically.
// /*AFLA*/  * Note: @old should not be empty.
// /*AFLA*/  */
// /*AFLA*/ static inline void list_replace_rcu(struct list_head *old,
// /*AFLA*/ 				struct list_head *new)
// /*AFLA*/ {
// /*AFLA*/ 	new->next = old->next;
// /*AFLA*/ 	new->prev = old->prev;
// /*AFLA*/ 	rcu_assign_pointer(list_next_rcu(new->prev), new);
// /*AFLA*/ 	new->next->prev = new;
// /*AFLA*/ 	old->prev = LIST_POISON2;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __list_splice_init_rcu - join an RCU-protected list into an existing list.
// /*AFLA*/  * @list:	the RCU-protected list to splice
// /*AFLA*/  * @prev:	points to the last element of the existing list
// /*AFLA*/  * @next:	points to the first element of the existing list
// /*AFLA*/  * @sync:	function to sync: synchronize_rcu(), synchronize_sched(), ...
// /*AFLA*/  *
// /*AFLA*/  * The list pointed to by @prev and @next can be RCU-read traversed
// /*AFLA*/  * concurrently with this function.
// /*AFLA*/  *
// /*AFLA*/  * Note that this function blocks.
// /*AFLA*/  *
// /*AFLA*/  * Important note: the caller must take whatever action is necessary to prevent
// /*AFLA*/  * any other updates to the existing list.  In principle, it is possible to
// /*AFLA*/  * modify the list as soon as sync() begins execution. If this sort of thing
// /*AFLA*/  * becomes necessary, an alternative version based on call_rcu() could be
// /*AFLA*/  * created.  But only if -really- needed -- there is no shortage of RCU API
// /*AFLA*/  * members.
// /*AFLA*/  */
// /*AFLA*/ static inline void __list_splice_init_rcu(struct list_head *list,
// /*AFLA*/ 					  struct list_head *prev,
// /*AFLA*/ 					  struct list_head *next,
// /*AFLA*/ 					  void (*sync)(void))
// /*AFLA*/ {
// /*AFLA*/ 	struct list_head *first = list->next;
// /*AFLA*/ 	struct list_head *last = list->prev;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * "first" and "last" tracking list, so initialize it.  RCU readers
// /*AFLA*/ 	 * have access to this list, so we must use INIT_LIST_HEAD_RCU()
// /*AFLA*/ 	 * instead of INIT_LIST_HEAD().
// /*AFLA*/ 	 */
// /*AFLA*/ 
// /*AFLA*/ 	INIT_LIST_HEAD_RCU(list);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * At this point, the list body still points to the source list.
// /*AFLA*/ 	 * Wait for any readers to finish using the list before splicing
// /*AFLA*/ 	 * the list body into the new list.  Any new readers will see
// /*AFLA*/ 	 * an empty list.
// /*AFLA*/ 	 */
// /*AFLA*/ 
// /*AFLA*/ 	sync();
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Readers are finished with the source list, so perform splice.
// /*AFLA*/ 	 * The order is important if the new list is global and accessible
// /*AFLA*/ 	 * to concurrent RCU readers.  Note that RCU readers are not
// /*AFLA*/ 	 * permitted to traverse the prev pointers without excluding
// /*AFLA*/ 	 * this function.
// /*AFLA*/ 	 */
// /*AFLA*/ 
// /*AFLA*/ 	last->next = next;
// /*AFLA*/ 	rcu_assign_pointer(list_next_rcu(prev), first);
// /*AFLA*/ 	first->prev = prev;
// /*AFLA*/ 	next->prev = last;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_splice_init_rcu - splice an RCU-protected list into an existing list,
// /*AFLA*/  *                        designed for stacks.
// /*AFLA*/  * @list:	the RCU-protected list to splice
// /*AFLA*/  * @head:	the place in the existing list to splice the first list into
// /*AFLA*/  * @sync:	function to sync: synchronize_rcu(), synchronize_sched(), ...
// /*AFLA*/  */
// /*AFLA*/ static inline void list_splice_init_rcu(struct list_head *list,
// /*AFLA*/ 					struct list_head *head,
// /*AFLA*/ 					void (*sync)(void))
// /*AFLA*/ {
// /*AFLA*/ 	if (!list_empty(list))
// /*AFLA*/ 		__list_splice_init_rcu(list, head, head->next, sync);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_splice_tail_init_rcu - splice an RCU-protected list into an existing
// /*AFLA*/  *                             list, designed for queues.
// /*AFLA*/  * @list:	the RCU-protected list to splice
// /*AFLA*/  * @head:	the place in the existing list to splice the first list into
// /*AFLA*/  * @sync:	function to sync: synchronize_rcu(), synchronize_sched(), ...
// /*AFLA*/  */
// /*AFLA*/ static inline void list_splice_tail_init_rcu(struct list_head *list,
// /*AFLA*/ 					     struct list_head *head,
// /*AFLA*/ 					     void (*sync)(void))
// /*AFLA*/ {
// /*AFLA*/ 	if (!list_empty(list))
// /*AFLA*/ 		__list_splice_init_rcu(list, head->prev, head, sync);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_entry_rcu - get the struct for this entry
// /*AFLA*/  * @ptr:        the &struct list_head pointer.
// /*AFLA*/  * @type:       the type of the struct this is embedded in.
// /*AFLA*/  * @member:     the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * This primitive may safely run concurrently with the _rcu list-mutation
// /*AFLA*/  * primitives such as list_add_rcu() as long as it's guarded by rcu_read_lock().
// /*AFLA*/  */
// /*AFLA*/ #define list_entry_rcu(ptr, type, member) \
// /*AFLA*/ 	container_of(lockless_dereference(ptr), type, member)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * Where are list_empty_rcu() and list_first_entry_rcu()?
// /*AFLA*/  *
// /*AFLA*/  * Implementing those functions following their counterparts list_empty() and
// /*AFLA*/  * list_first_entry() is not advisable because they lead to subtle race
// /*AFLA*/  * conditions as the following snippet shows:
// /*AFLA*/  *
// /*AFLA*/  * if (!list_empty_rcu(mylist)) {
// /*AFLA*/  *	struct foo *bar = list_first_entry_rcu(mylist, struct foo, list_member);
// /*AFLA*/  *	do_something(bar);
// /*AFLA*/  * }
// /*AFLA*/  *
// /*AFLA*/  * The list may not be empty when list_empty_rcu checks it, but it may be when
// /*AFLA*/  * list_first_entry_rcu rereads the ->next pointer.
// /*AFLA*/  *
// /*AFLA*/  * Rereading the ->next pointer is not a problem for list_empty() and
// /*AFLA*/  * list_first_entry() because they would be protected by a lock that blocks
// /*AFLA*/  * writers.
// /*AFLA*/  *
// /*AFLA*/  * See list_first_or_null_rcu for an alternative.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_first_or_null_rcu - get the first element from a list
// /*AFLA*/  * @ptr:        the list head to take the element from.
// /*AFLA*/  * @type:       the type of the struct this is embedded in.
// /*AFLA*/  * @member:     the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * Note that if the list is empty, it returns NULL.
// /*AFLA*/  *
// /*AFLA*/  * This primitive may safely run concurrently with the _rcu list-mutation
// /*AFLA*/  * primitives such as list_add_rcu() as long as it's guarded by rcu_read_lock().
// /*AFLA*/  */
// /*AFLA*/ #define list_first_or_null_rcu(ptr, type, member) \
// /*AFLA*/ ({ \
// /*AFLA*/ 	struct list_head *__ptr = (ptr); \
// /*AFLA*/ 	struct list_head *__next = READ_ONCE(__ptr->next); \
// /*AFLA*/ 	likely(__ptr != __next) ? list_entry_rcu(__next, type, member) : NULL; \
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_next_or_null_rcu - get the first element from a list
// /*AFLA*/  * @head:	the head for the list.
// /*AFLA*/  * @ptr:        the list head to take the next element from.
// /*AFLA*/  * @type:       the type of the struct this is embedded in.
// /*AFLA*/  * @member:     the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * Note that if the ptr is at the end of the list, NULL is returned.
// /*AFLA*/  *
// /*AFLA*/  * This primitive may safely run concurrently with the _rcu list-mutation
// /*AFLA*/  * primitives such as list_add_rcu() as long as it's guarded by rcu_read_lock().
// /*AFLA*/  */
// /*AFLA*/ #define list_next_or_null_rcu(head, ptr, type, member) \
// /*AFLA*/ ({ \
// /*AFLA*/ 	struct list_head *__head = (head); \
// /*AFLA*/ 	struct list_head *__ptr = (ptr); \
// /*AFLA*/ 	struct list_head *__next = READ_ONCE(__ptr->next); \
// /*AFLA*/ 	likely(__next != __head) ? list_entry_rcu(__next, type, \
// /*AFLA*/ 						  member) : NULL; \
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_for_each_entry_rcu	-	iterate over rcu list of given type
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * This list-traversal primitive may safely run concurrently with
// /*AFLA*/  * the _rcu list-mutation primitives such as list_add_rcu()
// /*AFLA*/  * as long as the traversal is guarded by rcu_read_lock().
// /*AFLA*/  */
// /*AFLA*/ #define list_for_each_entry_rcu(pos, head, member) \
// /*AFLA*/ 	for (pos = list_entry_rcu((head)->next, typeof(*pos), member); \
// /*AFLA*/ 		&pos->member != (head); \
// /*AFLA*/ 		pos = list_entry_rcu(pos->member.next, typeof(*pos), member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_entry_lockless - get the struct for this entry
// /*AFLA*/  * @ptr:        the &struct list_head pointer.
// /*AFLA*/  * @type:       the type of the struct this is embedded in.
// /*AFLA*/  * @member:     the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * This primitive may safely run concurrently with the _rcu list-mutation
// /*AFLA*/  * primitives such as list_add_rcu(), but requires some implicit RCU
// /*AFLA*/  * read-side guarding.  One example is running within a special
// /*AFLA*/  * exception-time environment where preemption is disabled and where
// /*AFLA*/  * lockdep cannot be invoked (in which case updaters must use RCU-sched,
// /*AFLA*/  * as in synchronize_sched(), call_rcu_sched(), and friends).  Another
// /*AFLA*/  * example is when items are added to the list, but never deleted.
// /*AFLA*/  */
// /*AFLA*/ #define list_entry_lockless(ptr, type, member) \
// /*AFLA*/ 	container_of((typeof(ptr))lockless_dereference(ptr), type, member)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_for_each_entry_lockless - iterate over rcu list of given type
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the list_struct within the struct.
// /*AFLA*/  *
// /*AFLA*/  * This primitive may safely run concurrently with the _rcu list-mutation
// /*AFLA*/  * primitives such as list_add_rcu(), but requires some implicit RCU
// /*AFLA*/  * read-side guarding.  One example is running within a special
// /*AFLA*/  * exception-time environment where preemption is disabled and where
// /*AFLA*/  * lockdep cannot be invoked (in which case updaters must use RCU-sched,
// /*AFLA*/  * as in synchronize_sched(), call_rcu_sched(), and friends).  Another
// /*AFLA*/  * example is when items are added to the list, but never deleted.
// /*AFLA*/  */
// /*AFLA*/ #define list_for_each_entry_lockless(pos, head, member) \
// /*AFLA*/ 	for (pos = list_entry_lockless((head)->next, typeof(*pos), member); \
// /*AFLA*/ 	     &pos->member != (head); \
// /*AFLA*/ 	     pos = list_entry_lockless(pos->member.next, typeof(*pos), member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_for_each_entry_continue_rcu - continue iteration over list of given type
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the list_head within the struct.
// /*AFLA*/  *
// /*AFLA*/  * Continue to iterate over list of given type, continuing after
// /*AFLA*/  * the current position.
// /*AFLA*/  */
// /*AFLA*/ #define list_for_each_entry_continue_rcu(pos, head, member) 		\
// /*AFLA*/ 	for (pos = list_entry_rcu(pos->member.next, typeof(*pos), member); \
// /*AFLA*/ 	     &pos->member != (head);	\
// /*AFLA*/ 	     pos = list_entry_rcu(pos->member.next, typeof(*pos), member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_del_rcu - deletes entry from hash list without re-initialization
// /*AFLA*/  * @n: the element to delete from the hash list.
// /*AFLA*/  *
// /*AFLA*/  * Note: list_unhashed() on entry does not return true after this,
// /*AFLA*/  * the entry is in an undefined state. It is useful for RCU based
// /*AFLA*/  * lockfree traversal.
// /*AFLA*/  *
// /*AFLA*/  * In particular, it means that we can not poison the forward
// /*AFLA*/  * pointers that may still be used for walking the hash list.
// /*AFLA*/  *
// /*AFLA*/  * The caller must take whatever precautions are necessary
// /*AFLA*/  * (such as holding appropriate locks) to avoid racing
// /*AFLA*/  * with another list-mutation primitive, such as hlist_add_head_rcu()
// /*AFLA*/  * or hlist_del_rcu(), running on this same list.
// /*AFLA*/  * However, it is perfectly legal to run concurrently with
// /*AFLA*/  * the _rcu list-traversal primitives, such as
// /*AFLA*/  * hlist_for_each_entry().
// /*AFLA*/  */
// /*AFLA*/ static inline void hlist_del_rcu(struct hlist_node *n)
// /*AFLA*/ {
// /*AFLA*/ 	__hlist_del(n);
// /*AFLA*/ 	n->pprev = LIST_POISON2;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_replace_rcu - replace old entry by new one
// /*AFLA*/  * @old : the element to be replaced
// /*AFLA*/  * @new : the new element to insert
// /*AFLA*/  *
// /*AFLA*/  * The @old entry will be replaced with the @new entry atomically.
// /*AFLA*/  */
// /*AFLA*/ static inline void hlist_replace_rcu(struct hlist_node *old,
// /*AFLA*/ 					struct hlist_node *new)
// /*AFLA*/ {
// /*AFLA*/ 	struct hlist_node *next = old->next;
// /*AFLA*/ 
// /*AFLA*/ 	new->next = next;
// /*AFLA*/ 	new->pprev = old->pprev;
// /*AFLA*/ 	rcu_assign_pointer(*(struct hlist_node __rcu **)new->pprev, new);
// /*AFLA*/ 	if (next)
// /*AFLA*/ 		new->next->pprev = &new->next;
// /*AFLA*/ 	old->pprev = LIST_POISON2;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * return the first or the next element in an RCU protected hlist
// /*AFLA*/  */
// /*AFLA*/ #define hlist_first_rcu(head)	(*((struct hlist_node __rcu **)(&(head)->first)))
// /*AFLA*/ #define hlist_next_rcu(node)	(*((struct hlist_node __rcu **)(&(node)->next)))
// /*AFLA*/ #define hlist_pprev_rcu(node)	(*((struct hlist_node __rcu **)((node)->pprev)))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_add_head_rcu
// /*AFLA*/  * @n: the element to add to the hash list.
// /*AFLA*/  * @h: the list to add to.
// /*AFLA*/  *
// /*AFLA*/  * Description:
// /*AFLA*/  * Adds the specified element to the specified hlist,
// /*AFLA*/  * while permitting racing traversals.
// /*AFLA*/  *
// /*AFLA*/  * The caller must take whatever precautions are necessary
// /*AFLA*/  * (such as holding appropriate locks) to avoid racing
// /*AFLA*/  * with another list-mutation primitive, such as hlist_add_head_rcu()
// /*AFLA*/  * or hlist_del_rcu(), running on this same list.
// /*AFLA*/  * However, it is perfectly legal to run concurrently with
// /*AFLA*/  * the _rcu list-traversal primitives, such as
// /*AFLA*/  * hlist_for_each_entry_rcu(), used to prevent memory-consistency
// /*AFLA*/  * problems on Alpha CPUs.  Regardless of the type of CPU, the
// /*AFLA*/  * list-traversal primitive must be guarded by rcu_read_lock().
// /*AFLA*/  */
// /*AFLA*/ static inline void hlist_add_head_rcu(struct hlist_node *n,
// /*AFLA*/ 					struct hlist_head *h)
// /*AFLA*/ {
// /*AFLA*/ 	struct hlist_node *first = h->first;
// /*AFLA*/ 
// /*AFLA*/ 	n->next = first;
// /*AFLA*/ 	n->pprev = &h->first;
// /*AFLA*/ 	rcu_assign_pointer(hlist_first_rcu(h), n);
// /*AFLA*/ 	if (first)
// /*AFLA*/ 		first->pprev = &n->next;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_add_tail_rcu
// /*AFLA*/  * @n: the element to add to the hash list.
// /*AFLA*/  * @h: the list to add to.
// /*AFLA*/  *
// /*AFLA*/  * Description:
// /*AFLA*/  * Adds the specified element to the specified hlist,
// /*AFLA*/  * while permitting racing traversals.
// /*AFLA*/  *
// /*AFLA*/  * The caller must take whatever precautions are necessary
// /*AFLA*/  * (such as holding appropriate locks) to avoid racing
// /*AFLA*/  * with another list-mutation primitive, such as hlist_add_head_rcu()
// /*AFLA*/  * or hlist_del_rcu(), running on this same list.
// /*AFLA*/  * However, it is perfectly legal to run concurrently with
// /*AFLA*/  * the _rcu list-traversal primitives, such as
// /*AFLA*/  * hlist_for_each_entry_rcu(), used to prevent memory-consistency
// /*AFLA*/  * problems on Alpha CPUs.  Regardless of the type of CPU, the
// /*AFLA*/  * list-traversal primitive must be guarded by rcu_read_lock().
// /*AFLA*/  */
// /*AFLA*/ static inline void hlist_add_tail_rcu(struct hlist_node *n,
// /*AFLA*/ 				      struct hlist_head *h)
// /*AFLA*/ {
// /*AFLA*/ 	struct hlist_node *i, *last = NULL;
// /*AFLA*/ 
// /*AFLA*/ 	for (i = hlist_first_rcu(h); i; i = hlist_next_rcu(i))
// /*AFLA*/ 		last = i;
// /*AFLA*/ 
// /*AFLA*/ 	if (last) {
// /*AFLA*/ 		n->next = last->next;
// /*AFLA*/ 		n->pprev = &last->next;
// /*AFLA*/ 		rcu_assign_pointer(hlist_next_rcu(last), n);
// /*AFLA*/ 	} else {
// /*AFLA*/ 		hlist_add_head_rcu(n, h);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_add_before_rcu
// /*AFLA*/  * @n: the new element to add to the hash list.
// /*AFLA*/  * @next: the existing element to add the new element before.
// /*AFLA*/  *
// /*AFLA*/  * Description:
// /*AFLA*/  * Adds the specified element to the specified hlist
// /*AFLA*/  * before the specified node while permitting racing traversals.
// /*AFLA*/  *
// /*AFLA*/  * The caller must take whatever precautions are necessary
// /*AFLA*/  * (such as holding appropriate locks) to avoid racing
// /*AFLA*/  * with another list-mutation primitive, such as hlist_add_head_rcu()
// /*AFLA*/  * or hlist_del_rcu(), running on this same list.
// /*AFLA*/  * However, it is perfectly legal to run concurrently with
// /*AFLA*/  * the _rcu list-traversal primitives, such as
// /*AFLA*/  * hlist_for_each_entry_rcu(), used to prevent memory-consistency
// /*AFLA*/  * problems on Alpha CPUs.
// /*AFLA*/  */
// /*AFLA*/ static inline void hlist_add_before_rcu(struct hlist_node *n,
// /*AFLA*/ 					struct hlist_node *next)
// /*AFLA*/ {
// /*AFLA*/ 	n->pprev = next->pprev;
// /*AFLA*/ 	n->next = next;
// /*AFLA*/ 	rcu_assign_pointer(hlist_pprev_rcu(n), n);
// /*AFLA*/ 	next->pprev = &n->next;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_add_behind_rcu
// /*AFLA*/  * @n: the new element to add to the hash list.
// /*AFLA*/  * @prev: the existing element to add the new element after.
// /*AFLA*/  *
// /*AFLA*/  * Description:
// /*AFLA*/  * Adds the specified element to the specified hlist
// /*AFLA*/  * after the specified node while permitting racing traversals.
// /*AFLA*/  *
// /*AFLA*/  * The caller must take whatever precautions are necessary
// /*AFLA*/  * (such as holding appropriate locks) to avoid racing
// /*AFLA*/  * with another list-mutation primitive, such as hlist_add_head_rcu()
// /*AFLA*/  * or hlist_del_rcu(), running on this same list.
// /*AFLA*/  * However, it is perfectly legal to run concurrently with
// /*AFLA*/  * the _rcu list-traversal primitives, such as
// /*AFLA*/  * hlist_for_each_entry_rcu(), used to prevent memory-consistency
// /*AFLA*/  * problems on Alpha CPUs.
// /*AFLA*/  */
// /*AFLA*/ static inline void hlist_add_behind_rcu(struct hlist_node *n,
// /*AFLA*/ 					struct hlist_node *prev)
// /*AFLA*/ {
// /*AFLA*/ 	n->next = prev->next;
// /*AFLA*/ 	n->pprev = &prev->next;
// /*AFLA*/ 	rcu_assign_pointer(hlist_next_rcu(prev), n);
// /*AFLA*/ 	if (n->next)
// /*AFLA*/ 		n->next->pprev = &n->next;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __hlist_for_each_rcu(pos, head)				\
// /*AFLA*/ 	for (pos = rcu_dereference(hlist_first_rcu(head));	\
// /*AFLA*/ 	     pos;						\
// /*AFLA*/ 	     pos = rcu_dereference(hlist_next_rcu(pos)))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_for_each_entry_rcu - iterate over rcu list of given type
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the hlist_node within the struct.
// /*AFLA*/  *
// /*AFLA*/  * This list-traversal primitive may safely run concurrently with
// /*AFLA*/  * the _rcu list-mutation primitives such as hlist_add_head_rcu()
// /*AFLA*/  * as long as the traversal is guarded by rcu_read_lock().
// /*AFLA*/  */
// /*AFLA*/ #define hlist_for_each_entry_rcu(pos, head, member)			\
// /*AFLA*/ 	for (pos = hlist_entry_safe (rcu_dereference_raw(hlist_first_rcu(head)),\
// /*AFLA*/ 			typeof(*(pos)), member);			\
// /*AFLA*/ 		pos;							\
// /*AFLA*/ 		pos = hlist_entry_safe(rcu_dereference_raw(hlist_next_rcu(\
// /*AFLA*/ 			&(pos)->member)), typeof(*(pos)), member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_for_each_entry_rcu_notrace - iterate over rcu list of given type (for tracing)
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the hlist_node within the struct.
// /*AFLA*/  *
// /*AFLA*/  * This list-traversal primitive may safely run concurrently with
// /*AFLA*/  * the _rcu list-mutation primitives such as hlist_add_head_rcu()
// /*AFLA*/  * as long as the traversal is guarded by rcu_read_lock().
// /*AFLA*/  *
// /*AFLA*/  * This is the same as hlist_for_each_entry_rcu() except that it does
// /*AFLA*/  * not do any RCU debugging or tracing.
// /*AFLA*/  */
// /*AFLA*/ #define hlist_for_each_entry_rcu_notrace(pos, head, member)			\
// /*AFLA*/ 	for (pos = hlist_entry_safe (rcu_dereference_raw_notrace(hlist_first_rcu(head)),\
// /*AFLA*/ 			typeof(*(pos)), member);			\
// /*AFLA*/ 		pos;							\
// /*AFLA*/ 		pos = hlist_entry_safe(rcu_dereference_raw_notrace(hlist_next_rcu(\
// /*AFLA*/ 			&(pos)->member)), typeof(*(pos)), member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_for_each_entry_rcu_bh - iterate over rcu list of given type
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @head:	the head for your list.
// /*AFLA*/  * @member:	the name of the hlist_node within the struct.
// /*AFLA*/  *
// /*AFLA*/  * This list-traversal primitive may safely run concurrently with
// /*AFLA*/  * the _rcu list-mutation primitives such as hlist_add_head_rcu()
// /*AFLA*/  * as long as the traversal is guarded by rcu_read_lock().
// /*AFLA*/  */
// /*AFLA*/ #define hlist_for_each_entry_rcu_bh(pos, head, member)			\
// /*AFLA*/ 	for (pos = hlist_entry_safe(rcu_dereference_bh(hlist_first_rcu(head)),\
// /*AFLA*/ 			typeof(*(pos)), member);			\
// /*AFLA*/ 		pos;							\
// /*AFLA*/ 		pos = hlist_entry_safe(rcu_dereference_bh(hlist_next_rcu(\
// /*AFLA*/ 			&(pos)->member)), typeof(*(pos)), member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_for_each_entry_continue_rcu - iterate over a hlist continuing after current point
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @member:	the name of the hlist_node within the struct.
// /*AFLA*/  */
// /*AFLA*/ #define hlist_for_each_entry_continue_rcu(pos, member)			\
// /*AFLA*/ 	for (pos = hlist_entry_safe(rcu_dereference_raw(hlist_next_rcu( \
// /*AFLA*/ 			&(pos)->member)), typeof(*(pos)), member);	\
// /*AFLA*/ 	     pos;							\
// /*AFLA*/ 	     pos = hlist_entry_safe(rcu_dereference_raw(hlist_next_rcu(	\
// /*AFLA*/ 			&(pos)->member)), typeof(*(pos)), member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_for_each_entry_continue_rcu_bh - iterate over a hlist continuing after current point
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @member:	the name of the hlist_node within the struct.
// /*AFLA*/  */
// /*AFLA*/ #define hlist_for_each_entry_continue_rcu_bh(pos, member)		\
// /*AFLA*/ 	for (pos = hlist_entry_safe(rcu_dereference_bh(hlist_next_rcu(  \
// /*AFLA*/ 			&(pos)->member)), typeof(*(pos)), member);	\
// /*AFLA*/ 	     pos;							\
// /*AFLA*/ 	     pos = hlist_entry_safe(rcu_dereference_bh(hlist_next_rcu(	\
// /*AFLA*/ 			&(pos)->member)), typeof(*(pos)), member))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hlist_for_each_entry_from_rcu - iterate over a hlist continuing from current point
// /*AFLA*/  * @pos:	the type * to use as a loop cursor.
// /*AFLA*/  * @member:	the name of the hlist_node within the struct.
// /*AFLA*/  */
// /*AFLA*/ #define hlist_for_each_entry_from_rcu(pos, member)			\
// /*AFLA*/ 	for (; pos;							\
// /*AFLA*/ 	     pos = hlist_entry_safe(rcu_dereference_raw(hlist_next_rcu(	\
// /*AFLA*/ 			&(pos)->member)), typeof(*(pos)), member))
// /*AFLA*/ 
// /*AFLA*/ #endif	/* __KERNEL__ */
// /*AFLA*/ #endif
