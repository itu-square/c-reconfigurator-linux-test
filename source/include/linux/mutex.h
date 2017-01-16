// /*AFLA*/ /*
// /*AFLA*/  * Mutexes: blocking mutual exclusion locks
// /*AFLA*/  *
// /*AFLA*/  * started by Ingo Molnar:
// /*AFLA*/  *
// /*AFLA*/  *  Copyright (C) 2004, 2005, 2006 Red Hat, Inc., Ingo Molnar <mingo@redhat.com>
// /*AFLA*/  *
// /*AFLA*/  * This file contains the main data structure and API definitions.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __LINUX_MUTEX_H
// /*AFLA*/ #define __LINUX_MUTEX_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/current.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/spinlock_types.h>
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ #include <linux/lockdep.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <asm/processor.h>
// /*AFLA*/ #include <linux/osq_lock.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Simple, straightforward mutexes with strict semantics:
// /*AFLA*/  *
// /*AFLA*/  * - only one task can hold the mutex at a time
// /*AFLA*/  * - only the owner can unlock the mutex
// /*AFLA*/  * - multiple unlocks are not permitted
// /*AFLA*/  * - recursive locking is not permitted
// /*AFLA*/  * - a mutex object must be initialized via the API
// /*AFLA*/  * - a mutex object must not be initialized via memset or copying
// /*AFLA*/  * - task may not exit with mutex held
// /*AFLA*/  * - memory areas where held locks reside must not be freed
// /*AFLA*/  * - held mutexes must not be reinitialized
// /*AFLA*/  * - mutexes may not be used in hardware or software interrupt
// /*AFLA*/  *   contexts such as tasklets and timers
// /*AFLA*/  *
// /*AFLA*/  * These semantics are fully enforced when DEBUG_MUTEXES is
// /*AFLA*/  * enabled. Furthermore, besides enforcing the above rules, the mutex
// /*AFLA*/  * debugging code also implements a number of additional features
// /*AFLA*/  * that make lock debugging easier and faster:
// /*AFLA*/  *
// /*AFLA*/  * - uses symbolic names of mutexes, whenever they are printed in debug output
// /*AFLA*/  * - point-of-acquire tracking, symbolic lookup of function names
// /*AFLA*/  * - list of all locks held in the system, printout of them
// /*AFLA*/  * - owner tracking
// /*AFLA*/  * - detects self-recursing locks and prints out all relevant info
// /*AFLA*/  * - detects multi-task circular deadlocks and prints out all affected
// /*AFLA*/  *   locks and tasks (and only those tasks)
// /*AFLA*/  */
// /*AFLA*/ struct mutex {
// /*AFLA*/ 	/* 1: unlocked, 0: locked, negative: locked, possible waiters */
// /*AFLA*/ 	atomic_t		count;
// /*AFLA*/ 	spinlock_t		wait_lock;
// /*AFLA*/ 	struct list_head	wait_list;
// /*AFLA*/ #if defined(CONFIG_DEBUG_MUTEXES) || defined(CONFIG_MUTEX_SPIN_ON_OWNER)
// /*AFLA*/ 	struct task_struct	*owner;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_MUTEX_SPIN_ON_OWNER
// /*AFLA*/ 	struct optimistic_spin_queue osq; /* Spinner MCS lock */
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_DEBUG_MUTEXES
// /*AFLA*/ 	void			*magic;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ 	struct lockdep_map	dep_map;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is the control structure for tasks blocked on mutex,
// /*AFLA*/  * which resides on the blocked task's kernel stack:
// /*AFLA*/  */
// /*AFLA*/ struct mutex_waiter {
// /*AFLA*/ 	struct list_head	list;
// /*AFLA*/ 	struct task_struct	*task;
// /*AFLA*/ #ifdef CONFIG_DEBUG_MUTEXES
// /*AFLA*/ 	void			*magic;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_MUTEXES
// /*AFLA*/ # include <linux/mutex-debug.h>
// /*AFLA*/ #else
// /*AFLA*/ # define __DEBUG_MUTEX_INITIALIZER(lockname)
// /*AFLA*/ /**
// /*AFLA*/  * mutex_init - initialize the mutex
// /*AFLA*/  * @mutex: the mutex to be initialized
// /*AFLA*/  *
// /*AFLA*/  * Initialize the mutex to unlocked state.
// /*AFLA*/  *
// /*AFLA*/  * It is not allowed to initialize an already locked mutex.
// /*AFLA*/  */
// /*AFLA*/ # define mutex_init(mutex) \
// /*AFLA*/ do {							\
// /*AFLA*/ 	static struct lock_class_key __key;		\
// /*AFLA*/ 							\
// /*AFLA*/ 	__mutex_init((mutex), #mutex, &__key);		\
// /*AFLA*/ } while (0)
// /*AFLA*/ static inline void mutex_destroy(struct mutex *lock) {}
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ # define __DEP_MAP_MUTEX_INITIALIZER(lockname) \
// /*AFLA*/ 		, .dep_map = { .name = #lockname }
// /*AFLA*/ #else
// /*AFLA*/ # define __DEP_MAP_MUTEX_INITIALIZER(lockname)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __MUTEX_INITIALIZER(lockname) \
// /*AFLA*/ 		{ .count = ATOMIC_INIT(1) \
// /*AFLA*/ 		, .wait_lock = __SPIN_LOCK_UNLOCKED(lockname.wait_lock) \
// /*AFLA*/ 		, .wait_list = LIST_HEAD_INIT(lockname.wait_list) \
// /*AFLA*/ 		__DEBUG_MUTEX_INITIALIZER(lockname) \
// /*AFLA*/ 		__DEP_MAP_MUTEX_INITIALIZER(lockname) }
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_MUTEX(mutexname) \
// /*AFLA*/ 	struct mutex mutexname = __MUTEX_INITIALIZER(mutexname)
// /*AFLA*/ 
// /*AFLA*/ extern void __mutex_init(struct mutex *lock, const char *name,
// /*AFLA*/ 			 struct lock_class_key *key);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * mutex_is_locked - is the mutex locked
// /*AFLA*/  * @lock: the mutex to be queried
// /*AFLA*/  *
// /*AFLA*/  * Returns 1 if the mutex is locked, 0 if unlocked.
// /*AFLA*/  */
// /*AFLA*/ static inline int mutex_is_locked(struct mutex *lock)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_read(&lock->count) != 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * See kernel/locking/mutex.c for detailed documentation of these APIs.
// /*AFLA*/  * Also see Documentation/locking/mutex-design.txt.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ extern void mutex_lock_nested(struct mutex *lock, unsigned int subclass);
// /*AFLA*/ extern void _mutex_lock_nest_lock(struct mutex *lock, struct lockdep_map *nest_lock);
// /*AFLA*/ 
// /*AFLA*/ extern int __must_check mutex_lock_interruptible_nested(struct mutex *lock,
// /*AFLA*/ 					unsigned int subclass);
// /*AFLA*/ extern int __must_check mutex_lock_killable_nested(struct mutex *lock,
// /*AFLA*/ 					unsigned int subclass);
// /*AFLA*/ 
// /*AFLA*/ #define mutex_lock(lock) mutex_lock_nested(lock, 0)
// /*AFLA*/ #define mutex_lock_interruptible(lock) mutex_lock_interruptible_nested(lock, 0)
// /*AFLA*/ #define mutex_lock_killable(lock) mutex_lock_killable_nested(lock, 0)
// /*AFLA*/ 
// /*AFLA*/ #define mutex_lock_nest_lock(lock, nest_lock)				\
// /*AFLA*/ do {									\
// /*AFLA*/ 	typecheck(struct lockdep_map *, &(nest_lock)->dep_map);	\
// /*AFLA*/ 	_mutex_lock_nest_lock(lock, &(nest_lock)->dep_map);		\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ extern void mutex_lock(struct mutex *lock);
// /*AFLA*/ extern int __must_check mutex_lock_interruptible(struct mutex *lock);
// /*AFLA*/ extern int __must_check mutex_lock_killable(struct mutex *lock);
// /*AFLA*/ 
// /*AFLA*/ # define mutex_lock_nested(lock, subclass) mutex_lock(lock)
// /*AFLA*/ # define mutex_lock_interruptible_nested(lock, subclass) mutex_lock_interruptible(lock)
// /*AFLA*/ # define mutex_lock_killable_nested(lock, subclass) mutex_lock_killable(lock)
// /*AFLA*/ # define mutex_lock_nest_lock(lock, nest_lock) mutex_lock(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * NOTE: mutex_trylock() follows the spin_trylock() convention,
// /*AFLA*/  *       not the down_trylock() convention!
// /*AFLA*/  *
// /*AFLA*/  * Returns 1 if the mutex has been acquired successfully, and 0 on contention.
// /*AFLA*/  */
// /*AFLA*/ extern int mutex_trylock(struct mutex *lock);
// /*AFLA*/ extern void mutex_unlock(struct mutex *lock);
// /*AFLA*/ 
// /*AFLA*/ extern int atomic_dec_and_mutex_lock(atomic_t *cnt, struct mutex *lock);
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_MUTEX_H */
