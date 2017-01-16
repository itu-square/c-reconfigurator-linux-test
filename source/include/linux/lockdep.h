// /*AFLA*/ /*
// /*AFLA*/  * Runtime locking correctness validator
// /*AFLA*/  *
// /*AFLA*/  *  Copyright (C) 2006,2007 Red Hat, Inc., Ingo Molnar <mingo@redhat.com>
// /*AFLA*/  *  Copyright (C) 2007 Red Hat, Inc., Peter Zijlstra
// /*AFLA*/  *
// /*AFLA*/  * see Documentation/locking/lockdep-design.txt for more details.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __LINUX_LOCKDEP_H
// /*AFLA*/ #define __LINUX_LOCKDEP_H
// /*AFLA*/ 
// /*AFLA*/ struct task_struct;
// /*AFLA*/ struct lockdep_map;
// /*AFLA*/ 
// /*AFLA*/ /* for sysctl */
// /*AFLA*/ extern int prove_locking;
// /*AFLA*/ extern int lock_stat;
// /*AFLA*/ 
// /*AFLA*/ #define MAX_LOCKDEP_SUBCLASSES		8UL
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_LOCKDEP
// /*AFLA*/ 
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/debug_locks.h>
// /*AFLA*/ #include <linux/stacktrace.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We'd rather not expose kernel/lockdep_states.h this wide, but we do need
// /*AFLA*/  * the total number of states... :-(
// /*AFLA*/  */
// /*AFLA*/ #define XXX_LOCK_USAGE_STATES		(1+3*4)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * NR_LOCKDEP_CACHING_CLASSES ... Number of classes
// /*AFLA*/  * cached in the instance of lockdep_map
// /*AFLA*/  *
// /*AFLA*/  * Currently main class (subclass == 0) and signle depth subclass
// /*AFLA*/  * are cached in lockdep_map. This optimization is mainly targeting
// /*AFLA*/  * on rq->lock. double_rq_lock() acquires this highly competitive with
// /*AFLA*/  * single depth.
// /*AFLA*/  */
// /*AFLA*/ #define NR_LOCKDEP_CACHING_CLASSES	2
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Lock-classes are keyed via unique addresses, by embedding the
// /*AFLA*/  * lockclass-key into the kernel (or module) .data section. (For
// /*AFLA*/  * static locks we use the lock address itself as the key.)
// /*AFLA*/  */
// /*AFLA*/ struct lockdep_subclass_key {
// /*AFLA*/ 	char __one_byte;
// /*AFLA*/ } __attribute__ ((__packed__));
// /*AFLA*/ 
// /*AFLA*/ struct lock_class_key {
// /*AFLA*/ 	struct lockdep_subclass_key	subkeys[MAX_LOCKDEP_SUBCLASSES];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct lock_class_key __lockdep_no_validate__;
// /*AFLA*/ 
// /*AFLA*/ #define LOCKSTAT_POINTS		4
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The lock-class itself:
// /*AFLA*/  */
// /*AFLA*/ struct lock_class {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * class-hash:
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct hlist_node		hash_entry;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * global list of all lock-classes:
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_head		lock_entry;
// /*AFLA*/ 
// /*AFLA*/ 	struct lockdep_subclass_key	*key;
// /*AFLA*/ 	unsigned int			subclass;
// /*AFLA*/ 	unsigned int			dep_gen_id;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * IRQ/softirq usage tracking bits:
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long			usage_mask;
// /*AFLA*/ 	struct stack_trace		usage_traces[XXX_LOCK_USAGE_STATES];
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * These fields represent a directed graph of lock dependencies,
// /*AFLA*/ 	 * to every node we attach a list of "forward" and a list of
// /*AFLA*/ 	 * "backward" graph nodes.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_head		locks_after, locks_before;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Generation counter, when doing certain classes of graph walking,
// /*AFLA*/ 	 * to ensure that we check one node only once:
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned int			version;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Statistics counter:
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long			ops;
// /*AFLA*/ 
// /*AFLA*/ 	const char			*name;
// /*AFLA*/ 	int				name_version;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_LOCK_STAT
// /*AFLA*/ 	unsigned long			contention_point[LOCKSTAT_POINTS];
// /*AFLA*/ 	unsigned long			contending_point[LOCKSTAT_POINTS];
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_LOCK_STAT
// /*AFLA*/ struct lock_time {
// /*AFLA*/ 	s64				min;
// /*AFLA*/ 	s64				max;
// /*AFLA*/ 	s64				total;
// /*AFLA*/ 	unsigned long			nr;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum bounce_type {
// /*AFLA*/ 	bounce_acquired_write,
// /*AFLA*/ 	bounce_acquired_read,
// /*AFLA*/ 	bounce_contended_write,
// /*AFLA*/ 	bounce_contended_read,
// /*AFLA*/ 	nr_bounce_types,
// /*AFLA*/ 
// /*AFLA*/ 	bounce_acquired = bounce_acquired_write,
// /*AFLA*/ 	bounce_contended = bounce_contended_write,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct lock_class_stats {
// /*AFLA*/ 	unsigned long			contention_point[LOCKSTAT_POINTS];
// /*AFLA*/ 	unsigned long			contending_point[LOCKSTAT_POINTS];
// /*AFLA*/ 	struct lock_time		read_waittime;
// /*AFLA*/ 	struct lock_time		write_waittime;
// /*AFLA*/ 	struct lock_time		read_holdtime;
// /*AFLA*/ 	struct lock_time		write_holdtime;
// /*AFLA*/ 	unsigned long			bounces[nr_bounce_types];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct lock_class_stats lock_stats(struct lock_class *class);
// /*AFLA*/ void clear_lock_stats(struct lock_class *class);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Map the lock object (the lock instance) to the lock-class object.
// /*AFLA*/  * This is embedded into specific lock instances:
// /*AFLA*/  */
// /*AFLA*/ struct lockdep_map {
// /*AFLA*/ 	struct lock_class_key		*key;
// /*AFLA*/ 	struct lock_class		*class_cache[NR_LOCKDEP_CACHING_CLASSES];
// /*AFLA*/ 	const char			*name;
// /*AFLA*/ #ifdef CONFIG_LOCK_STAT
// /*AFLA*/ 	int				cpu;
// /*AFLA*/ 	unsigned long			ip;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline void lockdep_copy_map(struct lockdep_map *to,
// /*AFLA*/ 				    struct lockdep_map *from)
// /*AFLA*/ {
// /*AFLA*/ 	int i;
// /*AFLA*/ 
// /*AFLA*/ 	*to = *from;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Since the class cache can be modified concurrently we could observe
// /*AFLA*/ 	 * half pointers (64bit arch using 32bit copy insns). Therefore clear
// /*AFLA*/ 	 * the caches and take the performance hit.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * XXX it doesn't work well with lockdep_set_class_and_subclass(), since
// /*AFLA*/ 	 *     that relies on cache abuse.
// /*AFLA*/ 	 */
// /*AFLA*/ 	for (i = 0; i < NR_LOCKDEP_CACHING_CLASSES; i++)
// /*AFLA*/ 		to->class_cache[i] = NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Every lock has a list of other locks that were taken after it.
// /*AFLA*/  * We only grow the list, never remove from it:
// /*AFLA*/  */
// /*AFLA*/ struct lock_list {
// /*AFLA*/ 	struct list_head		entry;
// /*AFLA*/ 	struct lock_class		*class;
// /*AFLA*/ 	struct stack_trace		trace;
// /*AFLA*/ 	int				distance;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The parent field is used to implement breadth-first search, and the
// /*AFLA*/ 	 * bit 0 is reused to indicate if the lock has been accessed in BFS.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct lock_list		*parent;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We record lock dependency chains, so that we can cache them:
// /*AFLA*/  */
// /*AFLA*/ struct lock_chain {
// /*AFLA*/ 	/* see BUILD_BUG_ON()s in lookup_chain_cache() */
// /*AFLA*/ 	unsigned int			irq_context :  2,
// /*AFLA*/ 					depth       :  6,
// /*AFLA*/ 					base	    : 24;
// /*AFLA*/ 	/* 4 byte hole */
// /*AFLA*/ 	struct hlist_node		entry;
// /*AFLA*/ 	u64				chain_key;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define MAX_LOCKDEP_KEYS_BITS		13
// /*AFLA*/ /*
// /*AFLA*/  * Subtract one because we offset hlock->class_idx by 1 in order
// /*AFLA*/  * to make 0 mean no class. This avoids overflowing the class_idx
// /*AFLA*/  * bitfield and hitting the BUG in hlock_class().
// /*AFLA*/  */
// /*AFLA*/ #define MAX_LOCKDEP_KEYS		((1UL << MAX_LOCKDEP_KEYS_BITS) - 1)
// /*AFLA*/ 
// /*AFLA*/ struct held_lock {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * One-way hash of the dependency chain up to this point. We
// /*AFLA*/ 	 * hash the hashes step by step as the dependency chain grows.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * We use it for dependency-caching and we skip detection
// /*AFLA*/ 	 * passes and dependency-updates if there is a cache-hit, so
// /*AFLA*/ 	 * it is absolutely critical for 100% coverage of the validator
// /*AFLA*/ 	 * to have a unique key value for every unique dependency path
// /*AFLA*/ 	 * that can occur in the system, to make a unique hash value
// /*AFLA*/ 	 * as likely as possible - hence the 64-bit width.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * The task struct holds the current hash value (initialized
// /*AFLA*/ 	 * with zero), here we store the previous hash value:
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64				prev_chain_key;
// /*AFLA*/ 	unsigned long			acquire_ip;
// /*AFLA*/ 	struct lockdep_map		*instance;
// /*AFLA*/ 	struct lockdep_map		*nest_lock;
// /*AFLA*/ #ifdef CONFIG_LOCK_STAT
// /*AFLA*/ 	u64 				waittime_stamp;
// /*AFLA*/ 	u64				holdtime_stamp;
// /*AFLA*/ #endif
// /*AFLA*/ 	unsigned int			class_idx:MAX_LOCKDEP_KEYS_BITS;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The lock-stack is unified in that the lock chains of interrupt
// /*AFLA*/ 	 * contexts nest ontop of process context chains, but we 'separate'
// /*AFLA*/ 	 * the hashes by starting with 0 if we cross into an interrupt
// /*AFLA*/ 	 * context, and we also keep do not add cross-context lock
// /*AFLA*/ 	 * dependencies - the lock usage graph walking covers that area
// /*AFLA*/ 	 * anyway, and we'd just unnecessarily increase the number of
// /*AFLA*/ 	 * dependencies otherwise. [Note: hardirq and softirq contexts
// /*AFLA*/ 	 * are separated from each other too.]
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * The following field is used to detect when we cross into an
// /*AFLA*/ 	 * interrupt context:
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned int irq_context:2; /* bit 0 - soft, bit 1 - hard */
// /*AFLA*/ 	unsigned int trylock:1;						/* 16 bits */
// /*AFLA*/ 
// /*AFLA*/ 	unsigned int read:2;        /* see lock_acquire() comment */
// /*AFLA*/ 	unsigned int check:1;       /* see lock_acquire() comment */
// /*AFLA*/ 	unsigned int hardirqs_off:1;
// /*AFLA*/ 	unsigned int references:12;					/* 32 bits */
// /*AFLA*/ 	unsigned int pin_count;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Initialization, self-test and debugging-output methods:
// /*AFLA*/  */
// /*AFLA*/ extern void lockdep_info(void);
// /*AFLA*/ extern void lockdep_reset(void);
// /*AFLA*/ extern void lockdep_reset_lock(struct lockdep_map *lock);
// /*AFLA*/ extern void lockdep_free_key_range(void *start, unsigned long size);
// /*AFLA*/ extern asmlinkage void lockdep_sys_exit(void);
// /*AFLA*/ 
// /*AFLA*/ extern void lockdep_off(void);
// /*AFLA*/ extern void lockdep_on(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These methods are used by specific locking variants (spinlocks,
// /*AFLA*/  * rwlocks, mutexes and rwsems) to pass init/acquire/release events
// /*AFLA*/  * to lockdep:
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ extern void lockdep_init_map(struct lockdep_map *lock, const char *name,
// /*AFLA*/ 			     struct lock_class_key *key, int subclass);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * To initialize a lockdep_map statically use this macro.
// /*AFLA*/  * Note that _name must not be NULL.
// /*AFLA*/  */
// /*AFLA*/ #define STATIC_LOCKDEP_MAP_INIT(_name, _key) \
// /*AFLA*/ 	{ .name = (_name), .key = (void *)(_key), }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Reinitialize a lock key - for cases where there is special locking or
// /*AFLA*/  * special initialization of locks so that the validator gets the scope
// /*AFLA*/  * of dependencies wrong: they are either too broad (they need a class-split)
// /*AFLA*/  * or they are too narrow (they suffer from a false class-split):
// /*AFLA*/  */
// /*AFLA*/ #define lockdep_set_class(lock, key) \
// /*AFLA*/ 		lockdep_init_map(&(lock)->dep_map, #key, key, 0)
// /*AFLA*/ #define lockdep_set_class_and_name(lock, key, name) \
// /*AFLA*/ 		lockdep_init_map(&(lock)->dep_map, name, key, 0)
// /*AFLA*/ #define lockdep_set_class_and_subclass(lock, key, sub) \
// /*AFLA*/ 		lockdep_init_map(&(lock)->dep_map, #key, key, sub)
// /*AFLA*/ #define lockdep_set_subclass(lock, sub)	\
// /*AFLA*/ 		lockdep_init_map(&(lock)->dep_map, #lock, \
// /*AFLA*/ 				 (lock)->dep_map.key, sub)
// /*AFLA*/ 
// /*AFLA*/ #define lockdep_set_novalidate_class(lock) \
// /*AFLA*/ 	lockdep_set_class_and_name(lock, &__lockdep_no_validate__, #lock)
// /*AFLA*/ /*
// /*AFLA*/  * Compare locking classes
// /*AFLA*/  */
// /*AFLA*/ #define lockdep_match_class(lock, key) lockdep_match_key(&(lock)->dep_map, key)
// /*AFLA*/ 
// /*AFLA*/ static inline int lockdep_match_key(struct lockdep_map *lock,
// /*AFLA*/ 				    struct lock_class_key *key)
// /*AFLA*/ {
// /*AFLA*/ 	return lock->key == key;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Acquire a lock.
// /*AFLA*/  *
// /*AFLA*/  * Values for "read":
// /*AFLA*/  *
// /*AFLA*/  *   0: exclusive (write) acquire
// /*AFLA*/  *   1: read-acquire (no recursion allowed)
// /*AFLA*/  *   2: read-acquire with same-instance recursion allowed
// /*AFLA*/  *
// /*AFLA*/  * Values for check:
// /*AFLA*/  *
// /*AFLA*/  *   0: simple checks (freeing, held-at-exit-time, etc.)
// /*AFLA*/  *   1: full validation
// /*AFLA*/  */
// /*AFLA*/ extern void lock_acquire(struct lockdep_map *lock, unsigned int subclass,
// /*AFLA*/ 			 int trylock, int read, int check,
// /*AFLA*/ 			 struct lockdep_map *nest_lock, unsigned long ip);
// /*AFLA*/ 
// /*AFLA*/ extern void lock_release(struct lockdep_map *lock, int nested,
// /*AFLA*/ 			 unsigned long ip);
// /*AFLA*/ 
// /*AFLA*/ #define lockdep_is_held(lock)	lock_is_held(&(lock)->dep_map)
// /*AFLA*/ 
// /*AFLA*/ extern int lock_is_held(struct lockdep_map *lock);
// /*AFLA*/ 
// /*AFLA*/ extern void lock_set_class(struct lockdep_map *lock, const char *name,
// /*AFLA*/ 			   struct lock_class_key *key, unsigned int subclass,
// /*AFLA*/ 			   unsigned long ip);
// /*AFLA*/ 
// /*AFLA*/ static inline void lock_set_subclass(struct lockdep_map *lock,
// /*AFLA*/ 		unsigned int subclass, unsigned long ip)
// /*AFLA*/ {
// /*AFLA*/ 	lock_set_class(lock, lock->name, lock->key, subclass, ip);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void lockdep_set_current_reclaim_state(gfp_t gfp_mask);
// /*AFLA*/ extern void lockdep_clear_current_reclaim_state(void);
// /*AFLA*/ extern void lockdep_trace_alloc(gfp_t mask);
// /*AFLA*/ 
// /*AFLA*/ struct pin_cookie { unsigned int val; };
// /*AFLA*/ 
// /*AFLA*/ #define NIL_COOKIE (struct pin_cookie){ .val = 0U, }
// /*AFLA*/ 
// /*AFLA*/ extern struct pin_cookie lock_pin_lock(struct lockdep_map *lock);
// /*AFLA*/ extern void lock_repin_lock(struct lockdep_map *lock, struct pin_cookie);
// /*AFLA*/ extern void lock_unpin_lock(struct lockdep_map *lock, struct pin_cookie);
// /*AFLA*/ 
// /*AFLA*/ # define INIT_LOCKDEP				.lockdep_recursion = 0, .lockdep_reclaim_gfp = 0,
// /*AFLA*/ 
// /*AFLA*/ #define lockdep_depth(tsk)	(debug_locks ? (tsk)->lockdep_depth : 0)
// /*AFLA*/ 
// /*AFLA*/ #define lockdep_assert_held(l)	do {				\
// /*AFLA*/ 		WARN_ON(debug_locks && !lockdep_is_held(l));	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #define lockdep_assert_held_once(l)	do {				\
// /*AFLA*/ 		WARN_ON_ONCE(debug_locks && !lockdep_is_held(l));	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #define lockdep_recursing(tsk)	((tsk)->lockdep_recursion)
// /*AFLA*/ 
// /*AFLA*/ #define lockdep_pin_lock(l)	lock_pin_lock(&(l)->dep_map)
// /*AFLA*/ #define lockdep_repin_lock(l,c)	lock_repin_lock(&(l)->dep_map, (c))
// /*AFLA*/ #define lockdep_unpin_lock(l,c)	lock_unpin_lock(&(l)->dep_map, (c))
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_LOCKDEP */
// /*AFLA*/ 
// /*AFLA*/ static inline void lockdep_off(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void lockdep_on(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ # define lock_acquire(l, s, t, r, c, n, i)	do { } while (0)
// /*AFLA*/ # define lock_release(l, n, i)			do { } while (0)
// /*AFLA*/ # define lock_set_class(l, n, k, s, i)		do { } while (0)
// /*AFLA*/ # define lock_set_subclass(l, s, i)		do { } while (0)
// /*AFLA*/ # define lockdep_set_current_reclaim_state(g)	do { } while (0)
// /*AFLA*/ # define lockdep_clear_current_reclaim_state()	do { } while (0)
// /*AFLA*/ # define lockdep_trace_alloc(g)			do { } while (0)
// /*AFLA*/ # define lockdep_info()				do { } while (0)
// /*AFLA*/ # define lockdep_init_map(lock, name, key, sub) \
// /*AFLA*/ 		do { (void)(name); (void)(key); } while (0)
// /*AFLA*/ # define lockdep_set_class(lock, key)		do { (void)(key); } while (0)
// /*AFLA*/ # define lockdep_set_class_and_name(lock, key, name) \
// /*AFLA*/ 		do { (void)(key); (void)(name); } while (0)
// /*AFLA*/ #define lockdep_set_class_and_subclass(lock, key, sub) \
// /*AFLA*/ 		do { (void)(key); } while (0)
// /*AFLA*/ #define lockdep_set_subclass(lock, sub)		do { } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define lockdep_set_novalidate_class(lock) do { } while (0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We don't define lockdep_match_class() and lockdep_match_key() for !LOCKDEP
// /*AFLA*/  * case since the result is not well defined and the caller should rather
// /*AFLA*/  * #ifdef the call himself.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ # define INIT_LOCKDEP
// /*AFLA*/ # define lockdep_reset()		do { debug_locks = 1; } while (0)
// /*AFLA*/ # define lockdep_free_key_range(start, size)	do { } while (0)
// /*AFLA*/ # define lockdep_sys_exit() 			do { } while (0)
// /*AFLA*/ /*
// /*AFLA*/  * The class key takes no space if lockdep is disabled:
// /*AFLA*/  */
// /*AFLA*/ struct lock_class_key { };
// /*AFLA*/ 
// /*AFLA*/ #define lockdep_depth(tsk)	(0)
// /*AFLA*/ 
// /*AFLA*/ #define lockdep_assert_held(l)			do { (void)(l); } while (0)
// /*AFLA*/ #define lockdep_assert_held_once(l)		do { (void)(l); } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define lockdep_recursing(tsk)			(0)
// /*AFLA*/ 
// /*AFLA*/ struct pin_cookie { };
// /*AFLA*/ 
// /*AFLA*/ #define NIL_COOKIE (struct pin_cookie){ }
// /*AFLA*/ 
// /*AFLA*/ #define lockdep_pin_lock(l)			({ struct pin_cookie cookie; cookie; })
// /*AFLA*/ #define lockdep_repin_lock(l, c)		do { (void)(l); (void)(c); } while (0)
// /*AFLA*/ #define lockdep_unpin_lock(l, c)		do { (void)(l); (void)(c); } while (0)
// /*AFLA*/ 
// /*AFLA*/ #endif /* !LOCKDEP */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_LOCK_STAT
// /*AFLA*/ 
// /*AFLA*/ extern void lock_contended(struct lockdep_map *lock, unsigned long ip);
// /*AFLA*/ extern void lock_acquired(struct lockdep_map *lock, unsigned long ip);
// /*AFLA*/ 
// /*AFLA*/ #define LOCK_CONTENDED(_lock, try, lock)			\
// /*AFLA*/ do {								\
// /*AFLA*/ 	if (!try(_lock)) {					\
// /*AFLA*/ 		lock_contended(&(_lock)->dep_map, _RET_IP_);	\
// /*AFLA*/ 		lock(_lock);					\
// /*AFLA*/ 	}							\
// /*AFLA*/ 	lock_acquired(&(_lock)->dep_map, _RET_IP_);			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define LOCK_CONTENDED_RETURN(_lock, try, lock)			\
// /*AFLA*/ ({								\
// /*AFLA*/ 	int ____err = 0;					\
// /*AFLA*/ 	if (!try(_lock)) {					\
// /*AFLA*/ 		lock_contended(&(_lock)->dep_map, _RET_IP_);	\
// /*AFLA*/ 		____err = lock(_lock);				\
// /*AFLA*/ 	}							\
// /*AFLA*/ 	if (!____err)						\
// /*AFLA*/ 		lock_acquired(&(_lock)->dep_map, _RET_IP_);	\
// /*AFLA*/ 	____err;						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_LOCK_STAT */
// /*AFLA*/ 
// /*AFLA*/ #define lock_contended(lockdep_map, ip) do {} while (0)
// /*AFLA*/ #define lock_acquired(lockdep_map, ip) do {} while (0)
// /*AFLA*/ 
// /*AFLA*/ #define LOCK_CONTENDED(_lock, try, lock) \
// /*AFLA*/ 	lock(_lock)
// /*AFLA*/ 
// /*AFLA*/ #define LOCK_CONTENDED_RETURN(_lock, try, lock) \
// /*AFLA*/ 	lock(_lock)
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_LOCK_STAT */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_LOCKDEP
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * On lockdep we dont want the hand-coded irq-enable of
// /*AFLA*/  * _raw_*_lock_flags() code, because lockdep assumes
// /*AFLA*/  * that interrupts are not re-enabled during lock-acquire:
// /*AFLA*/  */
// /*AFLA*/ #define LOCK_CONTENDED_FLAGS(_lock, try, lock, lockfl, flags) \
// /*AFLA*/ 	LOCK_CONTENDED((_lock), (try), (lock))
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_LOCKDEP */
// /*AFLA*/ 
// /*AFLA*/ #define LOCK_CONTENDED_FLAGS(_lock, try, lock, lockfl, flags) \
// /*AFLA*/ 	lockfl((_lock), (flags))
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_LOCKDEP */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TRACE_IRQFLAGS
// /*AFLA*/ extern void print_irqtrace_events(struct task_struct *curr);
// /*AFLA*/ #else
// /*AFLA*/ static inline void print_irqtrace_events(struct task_struct *curr)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * For trivial one-depth nesting of a lock-class, the following
// /*AFLA*/  * global define can be used. (Subsystems with multiple levels
// /*AFLA*/  * of nesting should define their own lock-nesting subclasses.)
// /*AFLA*/  */
// /*AFLA*/ #define SINGLE_DEPTH_NESTING			1
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Map the dependency ops to NOP or to real lockdep ops, depending
// /*AFLA*/  * on the per lock-class debug mode:
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define lock_acquire_exclusive(l, s, t, n, i)		lock_acquire(l, s, t, 0, 1, n, i)
// /*AFLA*/ #define lock_acquire_shared(l, s, t, n, i)		lock_acquire(l, s, t, 1, 1, n, i)
// /*AFLA*/ #define lock_acquire_shared_recursive(l, s, t, n, i)	lock_acquire(l, s, t, 2, 1, n, i)
// /*AFLA*/ 
// /*AFLA*/ #define spin_acquire(l, s, t, i)		lock_acquire_exclusive(l, s, t, NULL, i)
// /*AFLA*/ #define spin_acquire_nest(l, s, t, n, i)	lock_acquire_exclusive(l, s, t, n, i)
// /*AFLA*/ #define spin_release(l, n, i)			lock_release(l, n, i)
// /*AFLA*/ 
// /*AFLA*/ #define rwlock_acquire(l, s, t, i)		lock_acquire_exclusive(l, s, t, NULL, i)
// /*AFLA*/ #define rwlock_acquire_read(l, s, t, i)		lock_acquire_shared_recursive(l, s, t, NULL, i)
// /*AFLA*/ #define rwlock_release(l, n, i)			lock_release(l, n, i)
// /*AFLA*/ 
// /*AFLA*/ #define seqcount_acquire(l, s, t, i)		lock_acquire_exclusive(l, s, t, NULL, i)
// /*AFLA*/ #define seqcount_acquire_read(l, s, t, i)	lock_acquire_shared_recursive(l, s, t, NULL, i)
// /*AFLA*/ #define seqcount_release(l, n, i)		lock_release(l, n, i)
// /*AFLA*/ 
// /*AFLA*/ #define mutex_acquire(l, s, t, i)		lock_acquire_exclusive(l, s, t, NULL, i)
// /*AFLA*/ #define mutex_acquire_nest(l, s, t, n, i)	lock_acquire_exclusive(l, s, t, n, i)
// /*AFLA*/ #define mutex_release(l, n, i)			lock_release(l, n, i)
// /*AFLA*/ 
// /*AFLA*/ #define rwsem_acquire(l, s, t, i)		lock_acquire_exclusive(l, s, t, NULL, i)
// /*AFLA*/ #define rwsem_acquire_nest(l, s, t, n, i)	lock_acquire_exclusive(l, s, t, n, i)
// /*AFLA*/ #define rwsem_acquire_read(l, s, t, i)		lock_acquire_shared(l, s, t, NULL, i)
// /*AFLA*/ #define rwsem_release(l, n, i)			lock_release(l, n, i)
// /*AFLA*/ 
// /*AFLA*/ #define lock_map_acquire(l)			lock_acquire_exclusive(l, 0, 0, NULL, _THIS_IP_)
// /*AFLA*/ #define lock_map_acquire_read(l)		lock_acquire_shared_recursive(l, 0, 0, NULL, _THIS_IP_)
// /*AFLA*/ #define lock_map_acquire_tryread(l)		lock_acquire_shared_recursive(l, 0, 1, NULL, _THIS_IP_)
// /*AFLA*/ #define lock_map_release(l)			lock_release(l, 1, _THIS_IP_)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PROVE_LOCKING
// /*AFLA*/ # define might_lock(lock) 						\
// /*AFLA*/ do {									\
// /*AFLA*/ 	typecheck(struct lockdep_map *, &(lock)->dep_map);		\
// /*AFLA*/ 	lock_acquire(&(lock)->dep_map, 0, 0, 0, 1, NULL, _THIS_IP_);	\
// /*AFLA*/ 	lock_release(&(lock)->dep_map, 0, _THIS_IP_);			\
// /*AFLA*/ } while (0)
// /*AFLA*/ # define might_lock_read(lock) 						\
// /*AFLA*/ do {									\
// /*AFLA*/ 	typecheck(struct lockdep_map *, &(lock)->dep_map);		\
// /*AFLA*/ 	lock_acquire(&(lock)->dep_map, 0, 0, 1, 1, NULL, _THIS_IP_);	\
// /*AFLA*/ 	lock_release(&(lock)->dep_map, 0, _THIS_IP_);			\
// /*AFLA*/ } while (0)
// /*AFLA*/ #else
// /*AFLA*/ # define might_lock(lock) do { } while (0)
// /*AFLA*/ # define might_lock_read(lock) do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_LOCKDEP
// /*AFLA*/ void lockdep_rcu_suspicious(const char *file, const int line, const char *s);
// /*AFLA*/ #else
// /*AFLA*/ static inline void
// /*AFLA*/ lockdep_rcu_suspicious(const char *file, const int line, const char *s)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_LOCKDEP_H */
