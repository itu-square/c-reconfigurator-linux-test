// /*AFLA*/ #ifndef __LINUX_RWLOCK_API_SMP_H
// /*AFLA*/ #define __LINUX_RWLOCK_API_SMP_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef __LINUX_SPINLOCK_API_SMP_H
// /*AFLA*/ # error "please don't include this file directly"
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * include/linux/rwlock_api_smp.h
// /*AFLA*/  *
// /*AFLA*/  * spinlock API declarations on SMP (and debug)
// /*AFLA*/  * (implemented in kernel/spinlock.c)
// /*AFLA*/  *
// /*AFLA*/  * portions Copyright 2005, Red Hat, Inc., Ingo Molnar
// /*AFLA*/  * Released under the General Public License (GPL).
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ void __lockfunc _raw_read_lock(rwlock_t *lock)		__acquires(lock);
// /*AFLA*/ void __lockfunc _raw_write_lock(rwlock_t *lock)		__acquires(lock);
// /*AFLA*/ void __lockfunc _raw_read_lock_bh(rwlock_t *lock)	__acquires(lock);
// /*AFLA*/ void __lockfunc _raw_write_lock_bh(rwlock_t *lock)	__acquires(lock);
// /*AFLA*/ void __lockfunc _raw_read_lock_irq(rwlock_t *lock)	__acquires(lock);
// /*AFLA*/ void __lockfunc _raw_write_lock_irq(rwlock_t *lock)	__acquires(lock);
// /*AFLA*/ unsigned long __lockfunc _raw_read_lock_irqsave(rwlock_t *lock)
// /*AFLA*/ 							__acquires(lock);
// /*AFLA*/ unsigned long __lockfunc _raw_write_lock_irqsave(rwlock_t *lock)
// /*AFLA*/ 							__acquires(lock);
// /*AFLA*/ int __lockfunc _raw_read_trylock(rwlock_t *lock);
// /*AFLA*/ int __lockfunc _raw_write_trylock(rwlock_t *lock);
// /*AFLA*/ void __lockfunc _raw_read_unlock(rwlock_t *lock)	__releases(lock);
// /*AFLA*/ void __lockfunc _raw_write_unlock(rwlock_t *lock)	__releases(lock);
// /*AFLA*/ void __lockfunc _raw_read_unlock_bh(rwlock_t *lock)	__releases(lock);
// /*AFLA*/ void __lockfunc _raw_write_unlock_bh(rwlock_t *lock)	__releases(lock);
// /*AFLA*/ void __lockfunc _raw_read_unlock_irq(rwlock_t *lock)	__releases(lock);
// /*AFLA*/ void __lockfunc _raw_write_unlock_irq(rwlock_t *lock)	__releases(lock);
// /*AFLA*/ void __lockfunc
// /*AFLA*/ _raw_read_unlock_irqrestore(rwlock_t *lock, unsigned long flags)
// /*AFLA*/ 							__releases(lock);
// /*AFLA*/ void __lockfunc
// /*AFLA*/ _raw_write_unlock_irqrestore(rwlock_t *lock, unsigned long flags)
// /*AFLA*/ 							__releases(lock);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_READ_LOCK
// /*AFLA*/ #define _raw_read_lock(lock) __raw_read_lock(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_WRITE_LOCK
// /*AFLA*/ #define _raw_write_lock(lock) __raw_write_lock(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_READ_LOCK_BH
// /*AFLA*/ #define _raw_read_lock_bh(lock) __raw_read_lock_bh(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_WRITE_LOCK_BH
// /*AFLA*/ #define _raw_write_lock_bh(lock) __raw_write_lock_bh(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_READ_LOCK_IRQ
// /*AFLA*/ #define _raw_read_lock_irq(lock) __raw_read_lock_irq(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_WRITE_LOCK_IRQ
// /*AFLA*/ #define _raw_write_lock_irq(lock) __raw_write_lock_irq(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_READ_LOCK_IRQSAVE
// /*AFLA*/ #define _raw_read_lock_irqsave(lock) __raw_read_lock_irqsave(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_WRITE_LOCK_IRQSAVE
// /*AFLA*/ #define _raw_write_lock_irqsave(lock) __raw_write_lock_irqsave(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_READ_TRYLOCK
// /*AFLA*/ #define _raw_read_trylock(lock) __raw_read_trylock(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_WRITE_TRYLOCK
// /*AFLA*/ #define _raw_write_trylock(lock) __raw_write_trylock(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_READ_UNLOCK
// /*AFLA*/ #define _raw_read_unlock(lock) __raw_read_unlock(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_WRITE_UNLOCK
// /*AFLA*/ #define _raw_write_unlock(lock) __raw_write_unlock(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_READ_UNLOCK_BH
// /*AFLA*/ #define _raw_read_unlock_bh(lock) __raw_read_unlock_bh(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_WRITE_UNLOCK_BH
// /*AFLA*/ #define _raw_write_unlock_bh(lock) __raw_write_unlock_bh(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_READ_UNLOCK_IRQ
// /*AFLA*/ #define _raw_read_unlock_irq(lock) __raw_read_unlock_irq(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_WRITE_UNLOCK_IRQ
// /*AFLA*/ #define _raw_write_unlock_irq(lock) __raw_write_unlock_irq(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_READ_UNLOCK_IRQRESTORE
// /*AFLA*/ #define _raw_read_unlock_irqrestore(lock, flags) \
// /*AFLA*/ 	__raw_read_unlock_irqrestore(lock, flags)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_WRITE_UNLOCK_IRQRESTORE
// /*AFLA*/ #define _raw_write_unlock_irqrestore(lock, flags) \
// /*AFLA*/ 	__raw_write_unlock_irqrestore(lock, flags)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline int __raw_read_trylock(rwlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	if (do_raw_read_trylock(lock)) {
// /*AFLA*/ 		rwlock_acquire_read(&lock->dep_map, 0, 1, _RET_IP_);
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	}
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __raw_write_trylock(rwlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	if (do_raw_write_trylock(lock)) {
// /*AFLA*/ 		rwlock_acquire(&lock->dep_map, 0, 1, _RET_IP_);
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	}
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * If lockdep is enabled then we use the non-preemption spin-ops
// /*AFLA*/  * even on CONFIG_PREEMPT, because lockdep assumes that interrupts are
// /*AFLA*/  * not re-enabled during lock-acquire (which the preempt-spin-ops do):
// /*AFLA*/  */
// /*AFLA*/ #if !defined(CONFIG_GENERIC_LOCKBREAK) || defined(CONFIG_DEBUG_LOCK_ALLOC)
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_read_lock(rwlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	rwlock_acquire_read(&lock->dep_map, 0, 0, _RET_IP_);
// /*AFLA*/ 	LOCK_CONTENDED(lock, do_raw_read_trylock, do_raw_read_lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long __raw_read_lock_irqsave(rwlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long flags;
// /*AFLA*/ 
// /*AFLA*/ 	local_irq_save(flags);
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	rwlock_acquire_read(&lock->dep_map, 0, 0, _RET_IP_);
// /*AFLA*/ 	LOCK_CONTENDED_FLAGS(lock, do_raw_read_trylock, do_raw_read_lock,
// /*AFLA*/ 			     do_raw_read_lock_flags, &flags);
// /*AFLA*/ 	return flags;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_read_lock_irq(rwlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	local_irq_disable();
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	rwlock_acquire_read(&lock->dep_map, 0, 0, _RET_IP_);
// /*AFLA*/ 	LOCK_CONTENDED(lock, do_raw_read_trylock, do_raw_read_lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_read_lock_bh(rwlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	__local_bh_disable_ip(_RET_IP_, SOFTIRQ_LOCK_OFFSET);
// /*AFLA*/ 	rwlock_acquire_read(&lock->dep_map, 0, 0, _RET_IP_);
// /*AFLA*/ 	LOCK_CONTENDED(lock, do_raw_read_trylock, do_raw_read_lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long __raw_write_lock_irqsave(rwlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long flags;
// /*AFLA*/ 
// /*AFLA*/ 	local_irq_save(flags);
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	rwlock_acquire(&lock->dep_map, 0, 0, _RET_IP_);
// /*AFLA*/ 	LOCK_CONTENDED_FLAGS(lock, do_raw_write_trylock, do_raw_write_lock,
// /*AFLA*/ 			     do_raw_write_lock_flags, &flags);
// /*AFLA*/ 	return flags;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_write_lock_irq(rwlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	local_irq_disable();
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	rwlock_acquire(&lock->dep_map, 0, 0, _RET_IP_);
// /*AFLA*/ 	LOCK_CONTENDED(lock, do_raw_write_trylock, do_raw_write_lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_write_lock_bh(rwlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	__local_bh_disable_ip(_RET_IP_, SOFTIRQ_LOCK_OFFSET);
// /*AFLA*/ 	rwlock_acquire(&lock->dep_map, 0, 0, _RET_IP_);
// /*AFLA*/ 	LOCK_CONTENDED(lock, do_raw_write_trylock, do_raw_write_lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_write_lock(rwlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	rwlock_acquire(&lock->dep_map, 0, 0, _RET_IP_);
// /*AFLA*/ 	LOCK_CONTENDED(lock, do_raw_write_trylock, do_raw_write_lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_PREEMPT */
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_write_unlock(rwlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	rwlock_release(&lock->dep_map, 1, _RET_IP_);
// /*AFLA*/ 	do_raw_write_unlock(lock);
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_read_unlock(rwlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	rwlock_release(&lock->dep_map, 1, _RET_IP_);
// /*AFLA*/ 	do_raw_read_unlock(lock);
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ __raw_read_unlock_irqrestore(rwlock_t *lock, unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ 	rwlock_release(&lock->dep_map, 1, _RET_IP_);
// /*AFLA*/ 	do_raw_read_unlock(lock);
// /*AFLA*/ 	local_irq_restore(flags);
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_read_unlock_irq(rwlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	rwlock_release(&lock->dep_map, 1, _RET_IP_);
// /*AFLA*/ 	do_raw_read_unlock(lock);
// /*AFLA*/ 	local_irq_enable();
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_read_unlock_bh(rwlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	rwlock_release(&lock->dep_map, 1, _RET_IP_);
// /*AFLA*/ 	do_raw_read_unlock(lock);
// /*AFLA*/ 	__local_bh_enable_ip(_RET_IP_, SOFTIRQ_LOCK_OFFSET);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_write_unlock_irqrestore(rwlock_t *lock,
// /*AFLA*/ 					     unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ 	rwlock_release(&lock->dep_map, 1, _RET_IP_);
// /*AFLA*/ 	do_raw_write_unlock(lock);
// /*AFLA*/ 	local_irq_restore(flags);
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_write_unlock_irq(rwlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	rwlock_release(&lock->dep_map, 1, _RET_IP_);
// /*AFLA*/ 	do_raw_write_unlock(lock);
// /*AFLA*/ 	local_irq_enable();
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_write_unlock_bh(rwlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	rwlock_release(&lock->dep_map, 1, _RET_IP_);
// /*AFLA*/ 	do_raw_write_unlock(lock);
// /*AFLA*/ 	__local_bh_enable_ip(_RET_IP_, SOFTIRQ_LOCK_OFFSET);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_RWLOCK_API_SMP_H */
