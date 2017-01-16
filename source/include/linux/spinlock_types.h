// /*AFLA*/ #ifndef __LINUX_SPINLOCK_TYPES_H
// /*AFLA*/ #define __LINUX_SPINLOCK_TYPES_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * include/linux/spinlock_types.h - generic spinlock type definitions
// /*AFLA*/  *                                  and initializers
// /*AFLA*/  *
// /*AFLA*/  * portions Copyright 2005, Red Hat, Inc., Ingo Molnar
// /*AFLA*/  * Released under the General Public License (GPL).
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_SMP)
// /*AFLA*/ # include <asm/spinlock_types.h>
// /*AFLA*/ #else
// /*AFLA*/ # include <linux/spinlock_types_up.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <linux/lockdep.h>
// /*AFLA*/ 
// /*AFLA*/ typedef struct raw_spinlock {
// /*AFLA*/ 	arch_spinlock_t raw_lock;
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
// /*AFLA*/ } raw_spinlock_t;
// /*AFLA*/ 
// /*AFLA*/ #define SPINLOCK_MAGIC		0xdead4ead
// /*AFLA*/ 
// /*AFLA*/ #define SPINLOCK_OWNER_INIT	((void *)-1L)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ # define SPIN_DEP_MAP_INIT(lockname)	.dep_map = { .name = #lockname }
// /*AFLA*/ #else
// /*AFLA*/ # define SPIN_DEP_MAP_INIT(lockname)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_SPINLOCK
// /*AFLA*/ # define SPIN_DEBUG_INIT(lockname)		\
// /*AFLA*/ 	.magic = SPINLOCK_MAGIC,		\
// /*AFLA*/ 	.owner_cpu = -1,			\
// /*AFLA*/ 	.owner = SPINLOCK_OWNER_INIT,
// /*AFLA*/ #else
// /*AFLA*/ # define SPIN_DEBUG_INIT(lockname)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __RAW_SPIN_LOCK_INITIALIZER(lockname)	\
// /*AFLA*/ 	{					\
// /*AFLA*/ 	.raw_lock = __ARCH_SPIN_LOCK_UNLOCKED,	\
// /*AFLA*/ 	SPIN_DEBUG_INIT(lockname)		\
// /*AFLA*/ 	SPIN_DEP_MAP_INIT(lockname) }
// /*AFLA*/ 
// /*AFLA*/ #define __RAW_SPIN_LOCK_UNLOCKED(lockname)	\
// /*AFLA*/ 	(raw_spinlock_t) __RAW_SPIN_LOCK_INITIALIZER(lockname)
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_RAW_SPINLOCK(x)	raw_spinlock_t x = __RAW_SPIN_LOCK_UNLOCKED(x)
// /*AFLA*/ 
// /*AFLA*/ typedef struct spinlock {
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct raw_spinlock rlock;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ # define LOCK_PADSIZE (offsetof(struct raw_spinlock, dep_map))
// /*AFLA*/ 		struct {
// /*AFLA*/ 			u8 __padding[LOCK_PADSIZE];
// /*AFLA*/ 			struct lockdep_map dep_map;
// /*AFLA*/ 		};
// /*AFLA*/ #endif
// /*AFLA*/ 	};
// /*AFLA*/ } spinlock_t;
// /*AFLA*/ 
// /*AFLA*/ #define __SPIN_LOCK_INITIALIZER(lockname) \
// /*AFLA*/ 	{ { .rlock = __RAW_SPIN_LOCK_INITIALIZER(lockname) } }
// /*AFLA*/ 
// /*AFLA*/ #define __SPIN_LOCK_UNLOCKED(lockname) \
// /*AFLA*/ 	(spinlock_t ) __SPIN_LOCK_INITIALIZER(lockname)
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_SPINLOCK(x)	spinlock_t x = __SPIN_LOCK_UNLOCKED(x)
// /*AFLA*/ 
// /*AFLA*/ #include <linux/rwlock_types.h>
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_SPINLOCK_TYPES_H */
