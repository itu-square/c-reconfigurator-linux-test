// /*AFLA*/ #ifndef _ASM_X86_QSPINLOCK_H
// /*AFLA*/ #define _ASM_X86_QSPINLOCK_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/cpufeature.h>
// /*AFLA*/ #include <asm-generic/qspinlock_types.h>
// /*AFLA*/ #include <asm/paravirt.h>
// /*AFLA*/ 
// /*AFLA*/ #define	queued_spin_unlock queued_spin_unlock
// /*AFLA*/ /**
// /*AFLA*/  * queued_spin_unlock - release a queued spinlock
// /*AFLA*/  * @lock : Pointer to queued spinlock structure
// /*AFLA*/  *
// /*AFLA*/  * A smp_store_release() on the least-significant byte.
// /*AFLA*/  */
// /*AFLA*/ static inline void native_queued_spin_unlock(struct qspinlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	smp_store_release((u8 *)lock, 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PARAVIRT_SPINLOCKS
// /*AFLA*/ extern void native_queued_spin_lock_slowpath(struct qspinlock *lock, u32 val);
// /*AFLA*/ extern void __pv_init_lock_hash(void);
// /*AFLA*/ extern void __pv_queued_spin_lock_slowpath(struct qspinlock *lock, u32 val);
// /*AFLA*/ extern void __raw_callee_save___pv_queued_spin_unlock(struct qspinlock *lock);
// /*AFLA*/ 
// /*AFLA*/ static inline void queued_spin_lock_slowpath(struct qspinlock *lock, u32 val)
// /*AFLA*/ {
// /*AFLA*/ 	pv_queued_spin_lock_slowpath(lock, val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void queued_spin_unlock(struct qspinlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	pv_queued_spin_unlock(lock);
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline void queued_spin_unlock(struct qspinlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	native_queued_spin_unlock(lock);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PARAVIRT
// /*AFLA*/ #define virt_spin_lock virt_spin_lock
// /*AFLA*/ static inline bool virt_spin_lock(struct qspinlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	if (!static_cpu_has(X86_FEATURE_HYPERVISOR))
// /*AFLA*/ 		return false;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * On hypervisors without PARAVIRT_SPINLOCKS support we fall
// /*AFLA*/ 	 * back to a Test-and-Set spinlock, because fair locks have
// /*AFLA*/ 	 * horrible lock 'holder' preemption issues.
// /*AFLA*/ 	 */
// /*AFLA*/ 
// /*AFLA*/ 	do {
// /*AFLA*/ 		while (atomic_read(&lock->val) != 0)
// /*AFLA*/ 			cpu_relax();
// /*AFLA*/ 	} while (atomic_cmpxchg(&lock->val, 0, _Q_LOCKED_VAL) != 0);
// /*AFLA*/ 
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_PARAVIRT */
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/qspinlock.h>
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_QSPINLOCK_H */
