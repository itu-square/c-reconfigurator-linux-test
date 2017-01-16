// /*AFLA*/ #ifndef __LINUX_BIT_SPINLOCK_H
// /*AFLA*/ #define __LINUX_BIT_SPINLOCK_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/preempt.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  *  bit-based spin_lock()
// /*AFLA*/  *
// /*AFLA*/  * Don't use this unless you really need to: spin_lock() and spin_unlock()
// /*AFLA*/  * are significantly faster.
// /*AFLA*/  */
// /*AFLA*/ static inline void bit_spin_lock(int bitnum, unsigned long *addr)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Assuming the lock is uncontended, this never enters
// /*AFLA*/ 	 * the body of the outer loop. If it is contended, then
// /*AFLA*/ 	 * within the inner loop a non-atomic test is used to
// /*AFLA*/ 	 * busywait with less bus contention for a good time to
// /*AFLA*/ 	 * attempt to acquire the lock bit.
// /*AFLA*/ 	 */
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ #if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK)
// /*AFLA*/ 	while (unlikely(test_and_set_bit_lock(bitnum, addr))) {
// /*AFLA*/ 		preempt_enable();
// /*AFLA*/ 		do {
// /*AFLA*/ 			cpu_relax();
// /*AFLA*/ 		} while (test_bit(bitnum, addr));
// /*AFLA*/ 		preempt_disable();
// /*AFLA*/ 	}
// /*AFLA*/ #endif
// /*AFLA*/ 	__acquire(bitlock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Return true if it was acquired
// /*AFLA*/  */
// /*AFLA*/ static inline int bit_spin_trylock(int bitnum, unsigned long *addr)
// /*AFLA*/ {
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ #if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK)
// /*AFLA*/ 	if (unlikely(test_and_set_bit_lock(bitnum, addr))) {
// /*AFLA*/ 		preempt_enable();
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	}
// /*AFLA*/ #endif
// /*AFLA*/ 	__acquire(bitlock);
// /*AFLA*/ 	return 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  *  bit-based spin_unlock()
// /*AFLA*/  */
// /*AFLA*/ static inline void bit_spin_unlock(int bitnum, unsigned long *addr)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_DEBUG_SPINLOCK
// /*AFLA*/ 	BUG_ON(!test_bit(bitnum, addr));
// /*AFLA*/ #endif
// /*AFLA*/ #if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK)
// /*AFLA*/ 	clear_bit_unlock(bitnum, addr);
// /*AFLA*/ #endif
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ 	__release(bitlock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  *  bit-based spin_unlock()
// /*AFLA*/  *  non-atomic version, which can be used eg. if the bit lock itself is
// /*AFLA*/  *  protecting the rest of the flags in the word.
// /*AFLA*/  */
// /*AFLA*/ static inline void __bit_spin_unlock(int bitnum, unsigned long *addr)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_DEBUG_SPINLOCK
// /*AFLA*/ 	BUG_ON(!test_bit(bitnum, addr));
// /*AFLA*/ #endif
// /*AFLA*/ #if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK)
// /*AFLA*/ 	__clear_bit_unlock(bitnum, addr);
// /*AFLA*/ #endif
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ 	__release(bitlock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Return true if the lock is held.
// /*AFLA*/  */
// /*AFLA*/ static inline int bit_spin_is_locked(int bitnum, unsigned long *addr)
// /*AFLA*/ {
// /*AFLA*/ #if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK)
// /*AFLA*/ 	return test_bit(bitnum, addr);
// /*AFLA*/ #elif defined CONFIG_PREEMPT_COUNT
// /*AFLA*/ 	return preempt_count();
// /*AFLA*/ #else
// /*AFLA*/ 	return 1;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_BIT_SPINLOCK_H */
// /*AFLA*/ 
