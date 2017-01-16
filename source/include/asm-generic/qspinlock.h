// /*AFLA*/ /*
// /*AFLA*/  * Queued spinlock
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify
// /*AFLA*/  * it under the terms of the GNU General Public License as published by
// /*AFLA*/  * the Free Software Foundation; either version 2 of the License, or
// /*AFLA*/  * (at your option) any later version.
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it will be useful,
// /*AFLA*/  * but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/  * GNU General Public License for more details.
// /*AFLA*/  *
// /*AFLA*/  * (C) Copyright 2013-2015 Hewlett-Packard Development Company, L.P.
// /*AFLA*/  * (C) Copyright 2015 Hewlett-Packard Enterprise Development LP
// /*AFLA*/  *
// /*AFLA*/  * Authors: Waiman Long <waiman.long@hpe.com>
// /*AFLA*/  */
// /*AFLA*/ #ifndef __ASM_GENERIC_QSPINLOCK_H
// /*AFLA*/ #define __ASM_GENERIC_QSPINLOCK_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/qspinlock_types.h>
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * queued_spin_unlock_wait - wait until the _current_ lock holder releases the lock
// /*AFLA*/  * @lock : Pointer to queued spinlock structure
// /*AFLA*/  *
// /*AFLA*/  * There is a very slight possibility of live-lock if the lockers keep coming
// /*AFLA*/  * and the waiter is just unfortunate enough to not see any unlock state.
// /*AFLA*/  */
// /*AFLA*/ #ifndef queued_spin_unlock_wait
// /*AFLA*/ extern void queued_spin_unlock_wait(struct qspinlock *lock);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * queued_spin_is_locked - is the spinlock locked?
// /*AFLA*/  * @lock: Pointer to queued spinlock structure
// /*AFLA*/  * Return: 1 if it is locked, 0 otherwise
// /*AFLA*/  */
// /*AFLA*/ #ifndef queued_spin_is_locked
// /*AFLA*/ static __always_inline int queued_spin_is_locked(struct qspinlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * See queued_spin_unlock_wait().
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Any !0 state indicates it is locked, even if _Q_LOCKED_VAL
// /*AFLA*/ 	 * isn't immediately observable.
// /*AFLA*/ 	 */
// /*AFLA*/ 	return atomic_read(&lock->val);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * queued_spin_value_unlocked - is the spinlock structure unlocked?
// /*AFLA*/  * @lock: queued spinlock structure
// /*AFLA*/  * Return: 1 if it is unlocked, 0 otherwise
// /*AFLA*/  *
// /*AFLA*/  * N.B. Whenever there are tasks waiting for the lock, it is considered
// /*AFLA*/  *      locked wrt the lockref code to avoid lock stealing by the lockref
// /*AFLA*/  *      code and change things underneath the lock. This also allows some
// /*AFLA*/  *      optimizations to be applied without conflict with lockref.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline int queued_spin_value_unlocked(struct qspinlock lock)
// /*AFLA*/ {
// /*AFLA*/ 	return !atomic_read(&lock.val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * queued_spin_is_contended - check if the lock is contended
// /*AFLA*/  * @lock : Pointer to queued spinlock structure
// /*AFLA*/  * Return: 1 if lock contended, 0 otherwise
// /*AFLA*/  */
// /*AFLA*/ static __always_inline int queued_spin_is_contended(struct qspinlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_read(&lock->val) & ~_Q_LOCKED_MASK;
// /*AFLA*/ }
// /*AFLA*/ /**
// /*AFLA*/  * queued_spin_trylock - try to acquire the queued spinlock
// /*AFLA*/  * @lock : Pointer to queued spinlock structure
// /*AFLA*/  * Return: 1 if lock acquired, 0 if failed
// /*AFLA*/  */
// /*AFLA*/ static __always_inline int queued_spin_trylock(struct qspinlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	if (!atomic_read(&lock->val) &&
// /*AFLA*/ 	   (atomic_cmpxchg_acquire(&lock->val, 0, _Q_LOCKED_VAL) == 0))
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void queued_spin_lock_slowpath(struct qspinlock *lock, u32 val);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * queued_spin_lock - acquire a queued spinlock
// /*AFLA*/  * @lock: Pointer to queued spinlock structure
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void queued_spin_lock(struct qspinlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	u32 val;
// /*AFLA*/ 
// /*AFLA*/ 	val = atomic_cmpxchg_acquire(&lock->val, 0, _Q_LOCKED_VAL);
// /*AFLA*/ 	if (likely(val == 0))
// /*AFLA*/ 		return;
// /*AFLA*/ 	queued_spin_lock_slowpath(lock, val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifndef queued_spin_unlock
// /*AFLA*/ /**
// /*AFLA*/  * queued_spin_unlock - release a queued spinlock
// /*AFLA*/  * @lock : Pointer to queued spinlock structure
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void queued_spin_unlock(struct qspinlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * unlock() needs release semantics:
// /*AFLA*/ 	 */
// /*AFLA*/ 	(void)atomic_sub_return_release(_Q_LOCKED_VAL, &lock->val);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef virt_spin_lock
// /*AFLA*/ static __always_inline bool virt_spin_lock(struct qspinlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Remapping spinlock architecture specific functions to the corresponding
// /*AFLA*/  * queued spinlock functions.
// /*AFLA*/  */
// /*AFLA*/ #define arch_spin_is_locked(l)		queued_spin_is_locked(l)
// /*AFLA*/ #define arch_spin_is_contended(l)	queued_spin_is_contended(l)
// /*AFLA*/ #define arch_spin_value_unlocked(l)	queued_spin_value_unlocked(l)
// /*AFLA*/ #define arch_spin_lock(l)		queued_spin_lock(l)
// /*AFLA*/ #define arch_spin_trylock(l)		queued_spin_trylock(l)
// /*AFLA*/ #define arch_spin_unlock(l)		queued_spin_unlock(l)
// /*AFLA*/ #define arch_spin_lock_flags(l, f)	queued_spin_lock(l)
// /*AFLA*/ #define arch_spin_unlock_wait(l)	queued_spin_unlock_wait(l)
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASM_GENERIC_QSPINLOCK_H */
