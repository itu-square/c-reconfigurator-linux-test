// /*AFLA*/ /*
// /*AFLA*/  * RT Mutexes: blocking mutual exclusion locks with PI support
// /*AFLA*/  *
// /*AFLA*/  * started by Ingo Molnar and Thomas Gleixner:
// /*AFLA*/  *
// /*AFLA*/  *  Copyright (C) 2004-2006 Red Hat, Inc., Ingo Molnar <mingo@redhat.com>
// /*AFLA*/  *  Copyright (C) 2006, Timesys Corp., Thomas Gleixner <tglx@timesys.com>
// /*AFLA*/  *
// /*AFLA*/  * This file contains the public data structure and API definitions.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __LINUX_RT_MUTEX_H
// /*AFLA*/ #define __LINUX_RT_MUTEX_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ #include <linux/rbtree.h>
// /*AFLA*/ #include <linux/spinlock_types.h>
// /*AFLA*/ 
// /*AFLA*/ extern int max_lock_depth; /* for sysctl */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * The rt_mutex structure
// /*AFLA*/  *
// /*AFLA*/  * @wait_lock:	spinlock to protect the structure
// /*AFLA*/  * @waiters:	rbtree root to enqueue waiters in priority order
// /*AFLA*/  * @waiters_leftmost: top waiter
// /*AFLA*/  * @owner:	the mutex owner
// /*AFLA*/  */
// /*AFLA*/ struct rt_mutex {
// /*AFLA*/ 	raw_spinlock_t		wait_lock;
// /*AFLA*/ 	struct rb_root          waiters;
// /*AFLA*/ 	struct rb_node          *waiters_leftmost;
// /*AFLA*/ 	struct task_struct	*owner;
// /*AFLA*/ #ifdef CONFIG_DEBUG_RT_MUTEXES
// /*AFLA*/ 	int			save_state;
// /*AFLA*/ 	const char 		*name, *file;
// /*AFLA*/ 	int			line;
// /*AFLA*/ 	void			*magic;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct rt_mutex_waiter;
// /*AFLA*/ struct hrtimer_sleeper;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_RT_MUTEXES
// /*AFLA*/  extern int rt_mutex_debug_check_no_locks_freed(const void *from,
// /*AFLA*/ 						unsigned long len);
// /*AFLA*/  extern void rt_mutex_debug_check_no_locks_held(struct task_struct *task);
// /*AFLA*/ #else
// /*AFLA*/  static inline int rt_mutex_debug_check_no_locks_freed(const void *from,
// /*AFLA*/ 						       unsigned long len)
// /*AFLA*/  {
// /*AFLA*/ 	return 0;
// /*AFLA*/  }
// /*AFLA*/ # define rt_mutex_debug_check_no_locks_held(task)	do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_RT_MUTEXES
// /*AFLA*/ # define __DEBUG_RT_MUTEX_INITIALIZER(mutexname) \
// /*AFLA*/ 	, .name = #mutexname, .file = __FILE__, .line = __LINE__
// /*AFLA*/ # define rt_mutex_init(mutex)			__rt_mutex_init(mutex, __func__)
// /*AFLA*/  extern void rt_mutex_debug_task_free(struct task_struct *tsk);
// /*AFLA*/ #else
// /*AFLA*/ # define __DEBUG_RT_MUTEX_INITIALIZER(mutexname)
// /*AFLA*/ # define rt_mutex_init(mutex)			__rt_mutex_init(mutex, NULL)
// /*AFLA*/ # define rt_mutex_debug_task_free(t)			do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __RT_MUTEX_INITIALIZER(mutexname) \
// /*AFLA*/ 	{ .wait_lock = __RAW_SPIN_LOCK_UNLOCKED(mutexname.wait_lock) \
// /*AFLA*/ 	, .waiters = RB_ROOT \
// /*AFLA*/ 	, .owner = NULL \
// /*AFLA*/ 	__DEBUG_RT_MUTEX_INITIALIZER(mutexname)}
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_RT_MUTEX(mutexname) \
// /*AFLA*/ 	struct rt_mutex mutexname = __RT_MUTEX_INITIALIZER(mutexname)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rt_mutex_is_locked - is the mutex locked
// /*AFLA*/  * @lock: the mutex to be queried
// /*AFLA*/  *
// /*AFLA*/  * Returns 1 if the mutex is locked, 0 if unlocked.
// /*AFLA*/  */
// /*AFLA*/ static inline int rt_mutex_is_locked(struct rt_mutex *lock)
// /*AFLA*/ {
// /*AFLA*/ 	return lock->owner != NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void __rt_mutex_init(struct rt_mutex *lock, const char *name);
// /*AFLA*/ extern void rt_mutex_destroy(struct rt_mutex *lock);
// /*AFLA*/ 
// /*AFLA*/ extern void rt_mutex_lock(struct rt_mutex *lock);
// /*AFLA*/ extern int rt_mutex_lock_interruptible(struct rt_mutex *lock);
// /*AFLA*/ extern int rt_mutex_timed_lock(struct rt_mutex *lock,
// /*AFLA*/ 			       struct hrtimer_sleeper *timeout);
// /*AFLA*/ 
// /*AFLA*/ extern int rt_mutex_trylock(struct rt_mutex *lock);
// /*AFLA*/ 
// /*AFLA*/ extern void rt_mutex_unlock(struct rt_mutex *lock);
// /*AFLA*/ 
// /*AFLA*/ #endif
