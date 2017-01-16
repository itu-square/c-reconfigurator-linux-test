// /*AFLA*/ #ifndef __LINUX_RWLOCK_TYPES_H
// /*AFLA*/ #define __LINUX_RWLOCK_TYPES_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * include/linux/rwlock_types.h - generic rwlock type definitions
// /*AFLA*/  *				  and initializers
// /*AFLA*/  *
// /*AFLA*/  * portions Copyright 2005, Red Hat, Inc., Ingo Molnar
// /*AFLA*/  * Released under the General Public License (GPL).
// /*AFLA*/  */
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	arch_rwlock_t raw_lock;
// /*AFLA*/ #ifdef CONFIG_GENERIC_LOCKBREAK
// /*AFLA*/ 	unsigned int break_lock;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_DEBUG_SPINLOCK
// /*AFLA*/ 	unsigned int magic, owner_cpu;
// /*AFLA*/ 	void *owner;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ 	struct lockdep_map dep_map;
// /*AFLA*/ #endif
// /*AFLA*/ } rwlock_t;
// /*AFLA*/ 
// /*AFLA*/ #define RWLOCK_MAGIC		0xdeaf1eed
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ # define RW_DEP_MAP_INIT(lockname)	.dep_map = { .name = #lockname }
// /*AFLA*/ #else
// /*AFLA*/ # define RW_DEP_MAP_INIT(lockname)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_SPINLOCK
// /*AFLA*/ #define __RW_LOCK_UNLOCKED(lockname)					\
// /*AFLA*/ 	(rwlock_t)	{	.raw_lock = __ARCH_RW_LOCK_UNLOCKED,	\
// /*AFLA*/ 				.magic = RWLOCK_MAGIC,			\
// /*AFLA*/ 				.owner = SPINLOCK_OWNER_INIT,		\
// /*AFLA*/ 				.owner_cpu = -1,			\
// /*AFLA*/ 				RW_DEP_MAP_INIT(lockname) }
// /*AFLA*/ #else
// /*AFLA*/ #define __RW_LOCK_UNLOCKED(lockname) \
// /*AFLA*/ 	(rwlock_t)	{	.raw_lock = __ARCH_RW_LOCK_UNLOCKED,	\
// /*AFLA*/ 				RW_DEP_MAP_INIT(lockname) }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_RWLOCK(x)	rwlock_t x = __RW_LOCK_UNLOCKED(x)
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_RWLOCK_TYPES_H */
