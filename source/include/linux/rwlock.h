// /*AFLA*/ #ifndef __LINUX_RWLOCK_H
// /*AFLA*/ #define __LINUX_RWLOCK_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef __LINUX_SPINLOCK_H
// /*AFLA*/ # error "please don't include this file directly"
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * rwlock related methods
// /*AFLA*/  *
// /*AFLA*/  * split out from spinlock.h
// /*AFLA*/  *
// /*AFLA*/  * portions Copyright 2005, Red Hat, Inc., Ingo Molnar
// /*AFLA*/  * Released under the General Public License (GPL).
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_SPINLOCK
// /*AFLA*/   extern void __rwlock_init(rwlock_t *lock, const char *name,
// /*AFLA*/ 			    struct lock_class_key *key);
// /*AFLA*/ # define rwlock_init(lock)					\
// /*AFLA*/ do {								\
// /*AFLA*/ 	static struct lock_class_key __key;			\
// /*AFLA*/ 								\
// /*AFLA*/ 	__rwlock_init((lock), #lock, &__key);			\
// /*AFLA*/ } while (0)
// /*AFLA*/ #else
// /*AFLA*/ # define rwlock_init(lock)					\
// /*AFLA*/ 	do { *(lock) = __RW_LOCK_UNLOCKED(lock); } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_SPINLOCK
// /*AFLA*/  extern void do_raw_read_lock(rwlock_t *lock) __acquires(lock);
// /*AFLA*/ #define do_raw_read_lock_flags(lock, flags) do_raw_read_lock(lock)
// /*AFLA*/  extern int do_raw_read_trylock(rwlock_t *lock);
// /*AFLA*/  extern void do_raw_read_unlock(rwlock_t *lock) __releases(lock);
// /*AFLA*/  extern void do_raw_write_lock(rwlock_t *lock) __acquires(lock);
// /*AFLA*/ #define do_raw_write_lock_flags(lock, flags) do_raw_write_lock(lock)
// /*AFLA*/  extern int do_raw_write_trylock(rwlock_t *lock);
// /*AFLA*/  extern void do_raw_write_unlock(rwlock_t *lock) __releases(lock);
// /*AFLA*/ #else
// /*AFLA*/ # define do_raw_read_lock(rwlock)	do {__acquire(lock); arch_read_lock(&(rwlock)->raw_lock); } while (0)
// /*AFLA*/ # define do_raw_read_lock_flags(lock, flags) \
// /*AFLA*/ 		do {__acquire(lock); arch_read_lock_flags(&(lock)->raw_lock, *(flags)); } while (0)
// /*AFLA*/ # define do_raw_read_trylock(rwlock)	arch_read_trylock(&(rwlock)->raw_lock)
// /*AFLA*/ # define do_raw_read_unlock(rwlock)	do {arch_read_unlock(&(rwlock)->raw_lock); __release(lock); } while (0)
// /*AFLA*/ # define do_raw_write_lock(rwlock)	do {__acquire(lock); arch_write_lock(&(rwlock)->raw_lock); } while (0)
// /*AFLA*/ # define do_raw_write_lock_flags(lock, flags) \
// /*AFLA*/ 		do {__acquire(lock); arch_write_lock_flags(&(lock)->raw_lock, *(flags)); } while (0)
// /*AFLA*/ # define do_raw_write_trylock(rwlock)	arch_write_trylock(&(rwlock)->raw_lock)
// /*AFLA*/ # define do_raw_write_unlock(rwlock)	do {arch_write_unlock(&(rwlock)->raw_lock); __release(lock); } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define read_can_lock(rwlock)		arch_read_can_lock(&(rwlock)->raw_lock)
// /*AFLA*/ #define write_can_lock(rwlock)		arch_write_can_lock(&(rwlock)->raw_lock)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Define the various rw_lock methods.  Note we define these
// /*AFLA*/  * regardless of whether CONFIG_SMP or CONFIG_PREEMPT are set. The various
// /*AFLA*/  * methods are defined as nops in the case they are not required.
// /*AFLA*/  */
// /*AFLA*/ #define read_trylock(lock)	__cond_lock(lock, _raw_read_trylock(lock))
// /*AFLA*/ #define write_trylock(lock)	__cond_lock(lock, _raw_write_trylock(lock))
// /*AFLA*/ 
// /*AFLA*/ #define write_lock(lock)	_raw_write_lock(lock)
// /*AFLA*/ #define read_lock(lock)		_raw_read_lock(lock)
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK)
// /*AFLA*/ 
// /*AFLA*/ #define read_lock_irqsave(lock, flags)			\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		typecheck(unsigned long, flags);	\
// /*AFLA*/ 		flags = _raw_read_lock_irqsave(lock);	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #define write_lock_irqsave(lock, flags)			\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		typecheck(unsigned long, flags);	\
// /*AFLA*/ 		flags = _raw_write_lock_irqsave(lock);	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ #define read_lock_irqsave(lock, flags)			\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		typecheck(unsigned long, flags);	\
// /*AFLA*/ 		_raw_read_lock_irqsave(lock, flags);	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #define write_lock_irqsave(lock, flags)			\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		typecheck(unsigned long, flags);	\
// /*AFLA*/ 		_raw_write_lock_irqsave(lock, flags);	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define read_lock_irq(lock)		_raw_read_lock_irq(lock)
// /*AFLA*/ #define read_lock_bh(lock)		_raw_read_lock_bh(lock)
// /*AFLA*/ #define write_lock_irq(lock)		_raw_write_lock_irq(lock)
// /*AFLA*/ #define write_lock_bh(lock)		_raw_write_lock_bh(lock)
// /*AFLA*/ #define read_unlock(lock)		_raw_read_unlock(lock)
// /*AFLA*/ #define write_unlock(lock)		_raw_write_unlock(lock)
// /*AFLA*/ #define read_unlock_irq(lock)		_raw_read_unlock_irq(lock)
// /*AFLA*/ #define write_unlock_irq(lock)		_raw_write_unlock_irq(lock)
// /*AFLA*/ 
// /*AFLA*/ #define read_unlock_irqrestore(lock, flags)			\
// /*AFLA*/ 	do {							\
// /*AFLA*/ 		typecheck(unsigned long, flags);		\
// /*AFLA*/ 		_raw_read_unlock_irqrestore(lock, flags);	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #define read_unlock_bh(lock)		_raw_read_unlock_bh(lock)
// /*AFLA*/ 
// /*AFLA*/ #define write_unlock_irqrestore(lock, flags)		\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		typecheck(unsigned long, flags);	\
// /*AFLA*/ 		_raw_write_unlock_irqrestore(lock, flags);	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #define write_unlock_bh(lock)		_raw_write_unlock_bh(lock)
// /*AFLA*/ 
// /*AFLA*/ #define write_trylock_irqsave(lock, flags) \
// /*AFLA*/ ({ \
// /*AFLA*/ 	local_irq_save(flags); \
// /*AFLA*/ 	write_trylock(lock) ? \
// /*AFLA*/ 	1 : ({ local_irq_restore(flags); 0; }); \
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_RWLOCK_H */
