// /*AFLA*/ /* rwsem.h: R/W semaphores, public interface
// /*AFLA*/  *
// /*AFLA*/  * Written by David Howells (dhowells@redhat.com).
// /*AFLA*/  * Derived from asm-i386/semaphore.h
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_RWSEM_H
// /*AFLA*/ #define _LINUX_RWSEM_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/err.h>
// /*AFLA*/ #ifdef CONFIG_RWSEM_SPIN_ON_OWNER
// /*AFLA*/ #include <linux/osq_lock.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct rw_semaphore;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_RWSEM_GENERIC_SPINLOCK
// /*AFLA*/ #include <linux/rwsem-spinlock.h> /* use a generic implementation */
// /*AFLA*/ #define __RWSEM_INIT_COUNT(name)	.count = RWSEM_UNLOCKED_VALUE
// /*AFLA*/ #else
// /*AFLA*/ /* All arch specific implementations share the same struct */
// /*AFLA*/ struct rw_semaphore {
// /*AFLA*/ 	atomic_long_t count;
// /*AFLA*/ 	struct list_head wait_list;
// /*AFLA*/ 	raw_spinlock_t wait_lock;
// /*AFLA*/ #ifdef CONFIG_RWSEM_SPIN_ON_OWNER
// /*AFLA*/ 	struct optimistic_spin_queue osq; /* spinner MCS lock */
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Write owner. Used as a speculative check to see
// /*AFLA*/ 	 * if the owner is running on the cpu.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct task_struct *owner;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ 	struct lockdep_map	dep_map;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct rw_semaphore *rwsem_down_read_failed(struct rw_semaphore *sem);
// /*AFLA*/ extern struct rw_semaphore *rwsem_down_write_failed(struct rw_semaphore *sem);
// /*AFLA*/ extern struct rw_semaphore *rwsem_down_write_failed_killable(struct rw_semaphore *sem);
// /*AFLA*/ extern struct rw_semaphore *rwsem_wake(struct rw_semaphore *);
// /*AFLA*/ extern struct rw_semaphore *rwsem_downgrade_wake(struct rw_semaphore *sem);
// /*AFLA*/ 
// /*AFLA*/ /* Include the arch specific part */
// /*AFLA*/ #include <asm/rwsem.h>
// /*AFLA*/ 
// /*AFLA*/ /* In all implementations count != 0 means locked */
// /*AFLA*/ static inline int rwsem_is_locked(struct rw_semaphore *sem)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_long_read(&sem->count) != 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __RWSEM_INIT_COUNT(name)	.count = ATOMIC_LONG_INIT(RWSEM_UNLOCKED_VALUE)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Common initializer macros and functions */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ # define __RWSEM_DEP_MAP_INIT(lockname) , .dep_map = { .name = #lockname }
// /*AFLA*/ #else
// /*AFLA*/ # define __RWSEM_DEP_MAP_INIT(lockname)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_RWSEM_SPIN_ON_OWNER
// /*AFLA*/ #define __RWSEM_OPT_INIT(lockname) , .osq = OSQ_LOCK_UNLOCKED, .owner = NULL
// /*AFLA*/ #else
// /*AFLA*/ #define __RWSEM_OPT_INIT(lockname)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __RWSEM_INITIALIZER(name)				\
// /*AFLA*/ 	{ __RWSEM_INIT_COUNT(name),				\
// /*AFLA*/ 	  .wait_list = LIST_HEAD_INIT((name).wait_list),	\
// /*AFLA*/ 	  .wait_lock = __RAW_SPIN_LOCK_UNLOCKED(name.wait_lock)	\
// /*AFLA*/ 	  __RWSEM_OPT_INIT(name)				\
// /*AFLA*/ 	  __RWSEM_DEP_MAP_INIT(name) }
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_RWSEM(name) \
// /*AFLA*/ 	struct rw_semaphore name = __RWSEM_INITIALIZER(name)
// /*AFLA*/ 
// /*AFLA*/ extern void __init_rwsem(struct rw_semaphore *sem, const char *name,
// /*AFLA*/ 			 struct lock_class_key *key);
// /*AFLA*/ 
// /*AFLA*/ #define init_rwsem(sem)						\
// /*AFLA*/ do {								\
// /*AFLA*/ 	static struct lock_class_key __key;			\
// /*AFLA*/ 								\
// /*AFLA*/ 	__init_rwsem((sem), #sem, &__key);			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is the same regardless of which rwsem implementation that is being used.
// /*AFLA*/  * It is just a heuristic meant to be called by somebody alreadying holding the
// /*AFLA*/  * rwsem to see if somebody from an incompatible type is wanting access to the
// /*AFLA*/  * lock.
// /*AFLA*/  */
// /*AFLA*/ static inline int rwsem_is_contended(struct rw_semaphore *sem)
// /*AFLA*/ {
// /*AFLA*/ 	return !list_empty(&sem->wait_list);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * lock for reading
// /*AFLA*/  */
// /*AFLA*/ extern void down_read(struct rw_semaphore *sem);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * trylock for reading -- returns 1 if successful, 0 if contention
// /*AFLA*/  */
// /*AFLA*/ extern int down_read_trylock(struct rw_semaphore *sem);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * lock for writing
// /*AFLA*/  */
// /*AFLA*/ extern void down_write(struct rw_semaphore *sem);
// /*AFLA*/ extern int __must_check down_write_killable(struct rw_semaphore *sem);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * trylock for writing -- returns 1 if successful, 0 if contention
// /*AFLA*/  */
// /*AFLA*/ extern int down_write_trylock(struct rw_semaphore *sem);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * release a read lock
// /*AFLA*/  */
// /*AFLA*/ extern void up_read(struct rw_semaphore *sem);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * release a write lock
// /*AFLA*/  */
// /*AFLA*/ extern void up_write(struct rw_semaphore *sem);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * downgrade write lock to read lock
// /*AFLA*/  */
// /*AFLA*/ extern void downgrade_write(struct rw_semaphore *sem);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ /*
// /*AFLA*/  * nested locking. NOTE: rwsems are not allowed to recurse
// /*AFLA*/  * (which occurs if the same task tries to acquire the same
// /*AFLA*/  * lock instance multiple times), but multiple locks of the
// /*AFLA*/  * same lock class might be taken, if the order of the locks
// /*AFLA*/  * is always the same. This ordering rule can be expressed
// /*AFLA*/  * to lockdep via the _nested() APIs, but enumerating the
// /*AFLA*/  * subclasses that are used. (If the nesting relationship is
// /*AFLA*/  * static then another method for expressing nested locking is
// /*AFLA*/  * the explicit definition of lock class keys and the use of
// /*AFLA*/  * lockdep_set_class() at lock initialization time.
// /*AFLA*/  * See Documentation/locking/lockdep-design.txt for more details.)
// /*AFLA*/  */
// /*AFLA*/ extern void down_read_nested(struct rw_semaphore *sem, int subclass);
// /*AFLA*/ extern void down_write_nested(struct rw_semaphore *sem, int subclass);
// /*AFLA*/ extern int down_write_killable_nested(struct rw_semaphore *sem, int subclass);
// /*AFLA*/ extern void _down_write_nest_lock(struct rw_semaphore *sem, struct lockdep_map *nest_lock);
// /*AFLA*/ 
// /*AFLA*/ # define down_write_nest_lock(sem, nest_lock)			\
// /*AFLA*/ do {								\
// /*AFLA*/ 	typecheck(struct lockdep_map *, &(nest_lock)->dep_map);	\
// /*AFLA*/ 	_down_write_nest_lock(sem, &(nest_lock)->dep_map);	\
// /*AFLA*/ } while (0);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Take/release a lock when not the owner will release it.
// /*AFLA*/  *
// /*AFLA*/  * [ This API should be avoided as much as possible - the
// /*AFLA*/  *   proper abstraction for this case is completions. ]
// /*AFLA*/  */
// /*AFLA*/ extern void down_read_non_owner(struct rw_semaphore *sem);
// /*AFLA*/ extern void up_read_non_owner(struct rw_semaphore *sem);
// /*AFLA*/ #else
// /*AFLA*/ # define down_read_nested(sem, subclass)		down_read(sem)
// /*AFLA*/ # define down_write_nest_lock(sem, nest_lock)	down_write(sem)
// /*AFLA*/ # define down_write_nested(sem, subclass)	down_write(sem)
// /*AFLA*/ # define down_write_killable_nested(sem, subclass)	down_write_killable(sem)
// /*AFLA*/ # define down_read_non_owner(sem)		down_read(sem)
// /*AFLA*/ # define up_read_non_owner(sem)			up_read(sem)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_RWSEM_H */
