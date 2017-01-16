// /*AFLA*/ #ifndef __LINUX_SPINLOCK_API_SMP_H
// /*AFLA*/ #define __LINUX_SPINLOCK_API_SMP_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef __LINUX_SPINLOCK_H
// /*AFLA*/ # error "please don't include this file directly"
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * include/linux/spinlock_api_smp.h
// /*AFLA*/  *
// /*AFLA*/  * spinlock API declarations on SMP (and debug)
// /*AFLA*/  * (implemented in kernel/spinlock.c)
// /*AFLA*/  *
// /*AFLA*/  * portions Copyright 2005, Red Hat, Inc., Ingo Molnar
// /*AFLA*/  * Released under the General Public License (GPL).
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ int in_lock_functions(unsigned long addr);
// /*AFLA*/ 
// /*AFLA*/ #define assert_raw_spin_locked(x)	BUG_ON(!raw_spin_is_locked(x))
// /*AFLA*/ 
// /*AFLA*/ void __lockfunc _raw_spin_lock(raw_spinlock_t *lock)		__acquires(lock);
// /*AFLA*/ void __lockfunc _raw_spin_lock_nested(raw_spinlock_t *lock, int subclass)
// /*AFLA*/ 								__acquires(lock);
// /*AFLA*/ void __lockfunc _raw_spin_lock_bh_nested(raw_spinlock_t *lock, int subclass)
// /*AFLA*/ 								__acquires(lock);
// /*AFLA*/ void __lockfunc
// /*AFLA*/ _raw_spin_lock_nest_lock(raw_spinlock_t *lock, struct lockdep_map *map)
// /*AFLA*/ 								__acquires(lock);
// /*AFLA*/ void __lockfunc _raw_spin_lock_bh(raw_spinlock_t *lock)		__acquires(lock);
// /*AFLA*/ void __lockfunc _raw_spin_lock_irq(raw_spinlock_t *lock)
// /*AFLA*/ 								__acquires(lock);
// /*AFLA*/ 
// /*AFLA*/ unsigned long __lockfunc _raw_spin_lock_irqsave(raw_spinlock_t *lock)
// /*AFLA*/ 								__acquires(lock);
// /*AFLA*/ unsigned long __lockfunc
// /*AFLA*/ _raw_spin_lock_irqsave_nested(raw_spinlock_t *lock, int subclass)
// /*AFLA*/ 								__acquires(lock);
// /*AFLA*/ int __lockfunc _raw_spin_trylock(raw_spinlock_t *lock);
// /*AFLA*/ int __lockfunc _raw_spin_trylock_bh(raw_spinlock_t *lock);
// /*AFLA*/ void __lockfunc _raw_spin_unlock(raw_spinlock_t *lock)		__releases(lock);
// /*AFLA*/ void __lockfunc _raw_spin_unlock_bh(raw_spinlock_t *lock)	__releases(lock);
// /*AFLA*/ void __lockfunc _raw_spin_unlock_irq(raw_spinlock_t *lock)	__releases(lock);
// /*AFLA*/ void __lockfunc
// /*AFLA*/ _raw_spin_unlock_irqrestore(raw_spinlock_t *lock, unsigned long flags)
// /*AFLA*/ 								__releases(lock);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_SPIN_LOCK
// /*AFLA*/ #define _raw_spin_lock(lock) __raw_spin_lock(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_SPIN_LOCK_BH
// /*AFLA*/ #define _raw_spin_lock_bh(lock) __raw_spin_lock_bh(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_SPIN_LOCK_IRQ
// /*AFLA*/ #define _raw_spin_lock_irq(lock) __raw_spin_lock_irq(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_SPIN_LOCK_IRQSAVE
// /*AFLA*/ #define _raw_spin_lock_irqsave(lock) __raw_spin_lock_irqsave(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_SPIN_TRYLOCK
// /*AFLA*/ #define _raw_spin_trylock(lock) __raw_spin_trylock(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_SPIN_TRYLOCK_BH
// /*AFLA*/ #define _raw_spin_trylock_bh(lock) __raw_spin_trylock_bh(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_UNINLINE_SPIN_UNLOCK
// /*AFLA*/ #define _raw_spin_unlock(lock) __raw_spin_unlock(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_SPIN_UNLOCK_BH
// /*AFLA*/ #define _raw_spin_unlock_bh(lock) __raw_spin_unlock_bh(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_SPIN_UNLOCK_IRQ
// /*AFLA*/ #define _raw_spin_unlock_irq(lock) __raw_spin_unlock_irq(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_INLINE_SPIN_UNLOCK_IRQRESTORE
// /*AFLA*/ #define _raw_spin_unlock_irqrestore(lock, flags) __raw_spin_unlock_irqrestore(lock, flags)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline int __raw_spin_trylock(raw_spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	if (do_raw_spin_trylock(lock)) {
// /*AFLA*/ 		spin_acquire(&lock->dep_map, 0, 1, _RET_IP_);
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
// /*AFLA*/ static inline unsigned long __raw_spin_lock_irqsave(raw_spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long flags;
// /*AFLA*/ 
// /*AFLA*/ 	local_irq_save(flags);
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	spin_acquire(&lock->dep_map, 0, 0, _RET_IP_);
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * On lockdep we dont want the hand-coded irq-enable of
// /*AFLA*/ 	 * do_raw_spin_lock_flags() code, because lockdep assumes
// /*AFLA*/ 	 * that interrupts are not re-enabled during lock-acquire:
// /*AFLA*/ 	 */
// /*AFLA*/ #ifdef CONFIG_LOCKDEP
// /*AFLA*/ 	LOCK_CONTENDED(lock, do_raw_spin_trylock, do_raw_spin_lock);
// /*AFLA*/ #else
// /*AFLA*/ 	do_raw_spin_lock_flags(lock, &flags);
// /*AFLA*/ #endif
// /*AFLA*/ 	return flags;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_spin_lock_irq(raw_spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	local_irq_disable();
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	spin_acquire(&lock->dep_map, 0, 0, _RET_IP_);
// /*AFLA*/ 	LOCK_CONTENDED(lock, do_raw_spin_trylock, do_raw_spin_lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_spin_lock_bh(raw_spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	__local_bh_disable_ip(_RET_IP_, SOFTIRQ_LOCK_OFFSET);
// /*AFLA*/ 	spin_acquire(&lock->dep_map, 0, 0, _RET_IP_);
// /*AFLA*/ 	LOCK_CONTENDED(lock, do_raw_spin_trylock, do_raw_spin_lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_spin_lock(raw_spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	spin_acquire(&lock->dep_map, 0, 0, _RET_IP_);
// /*AFLA*/ 	LOCK_CONTENDED(lock, do_raw_spin_trylock, do_raw_spin_lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* !CONFIG_GENERIC_LOCKBREAK || CONFIG_DEBUG_LOCK_ALLOC */
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_spin_unlock(raw_spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	spin_release(&lock->dep_map, 1, _RET_IP_);
// /*AFLA*/ 	do_raw_spin_unlock(lock);
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_spin_unlock_irqrestore(raw_spinlock_t *lock,
// /*AFLA*/ 					    unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ 	spin_release(&lock->dep_map, 1, _RET_IP_);
// /*AFLA*/ 	do_raw_spin_unlock(lock);
// /*AFLA*/ 	local_irq_restore(flags);
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_spin_unlock_irq(raw_spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	spin_release(&lock->dep_map, 1, _RET_IP_);
// /*AFLA*/ 	do_raw_spin_unlock(lock);
// /*AFLA*/ 	local_irq_enable();
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __raw_spin_unlock_bh(raw_spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	spin_release(&lock->dep_map, 1, _RET_IP_);
// /*AFLA*/ 	do_raw_spin_unlock(lock);
// /*AFLA*/ 	__local_bh_enable_ip(_RET_IP_, SOFTIRQ_LOCK_OFFSET);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __raw_spin_trylock_bh(raw_spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	__local_bh_disable_ip(_RET_IP_, SOFTIRQ_LOCK_OFFSET);
// /*AFLA*/ 	if (do_raw_spin_trylock(lock)) {
// /*AFLA*/ 		spin_acquire(&lock->dep_map, 0, 1, _RET_IP_);
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	}
// /*AFLA*/ 	__local_bh_enable_ip(_RET_IP_, SOFTIRQ_LOCK_OFFSET);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #include <linux/rwlock_api_smp.h>
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_SPINLOCK_API_SMP_H */
