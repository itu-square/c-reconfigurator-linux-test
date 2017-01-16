// /*AFLA*/ /*
// /*AFLA*/  * Queue read/write lock
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
// /*AFLA*/  * (C) Copyright 2013-2014 Hewlett-Packard Development Company, L.P.
// /*AFLA*/  *
// /*AFLA*/  * Authors: Waiman Long <waiman.long@hp.com>
// /*AFLA*/  */
// /*AFLA*/ #ifndef __ASM_GENERIC_QRWLOCK_H
// /*AFLA*/ #define __ASM_GENERIC_QRWLOCK_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <asm/barrier.h>
// /*AFLA*/ #include <asm/processor.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/qrwlock_types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Writer states & reader shift and bias.
// /*AFLA*/  *
// /*AFLA*/  *       | +0 | +1 | +2 | +3 |
// /*AFLA*/  *   ----+----+----+----+----+
// /*AFLA*/  *    LE | 78 | 56 | 34 | 12 | 0x12345678
// /*AFLA*/  *   ----+----+----+----+----+
// /*AFLA*/  *       | wr |      rd      |
// /*AFLA*/  *       +----+----+----+----+
// /*AFLA*/  *
// /*AFLA*/  *   ----+----+----+----+----+
// /*AFLA*/  *    BE | 12 | 34 | 56 | 78 | 0x12345678
// /*AFLA*/  *   ----+----+----+----+----+
// /*AFLA*/  *       |      rd      | wr |
// /*AFLA*/  *       +----+----+----+----+
// /*AFLA*/  */
// /*AFLA*/ #define	_QW_WAITING	1		/* A writer is waiting	   */
// /*AFLA*/ #define	_QW_LOCKED	0xff		/* A writer holds the lock */
// /*AFLA*/ #define	_QW_WMASK	0xff		/* Writer mask		   */
// /*AFLA*/ #define	_QR_SHIFT	8		/* Reader count shift	   */
// /*AFLA*/ #define _QR_BIAS	(1U << _QR_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * External function declarations
// /*AFLA*/  */
// /*AFLA*/ extern void queued_read_lock_slowpath(struct qrwlock *lock, u32 cnts);
// /*AFLA*/ extern void queued_write_lock_slowpath(struct qrwlock *lock);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * queued_read_can_lock- would read_trylock() succeed?
// /*AFLA*/  * @lock: Pointer to queue rwlock structure
// /*AFLA*/  */
// /*AFLA*/ static inline int queued_read_can_lock(struct qrwlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	return !(atomic_read(&lock->cnts) & _QW_WMASK);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * queued_write_can_lock- would write_trylock() succeed?
// /*AFLA*/  * @lock: Pointer to queue rwlock structure
// /*AFLA*/  */
// /*AFLA*/ static inline int queued_write_can_lock(struct qrwlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	return !atomic_read(&lock->cnts);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * queued_read_trylock - try to acquire read lock of a queue rwlock
// /*AFLA*/  * @lock : Pointer to queue rwlock structure
// /*AFLA*/  * Return: 1 if lock acquired, 0 if failed
// /*AFLA*/  */
// /*AFLA*/ static inline int queued_read_trylock(struct qrwlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	u32 cnts;
// /*AFLA*/ 
// /*AFLA*/ 	cnts = atomic_read(&lock->cnts);
// /*AFLA*/ 	if (likely(!(cnts & _QW_WMASK))) {
// /*AFLA*/ 		cnts = (u32)atomic_add_return_acquire(_QR_BIAS, &lock->cnts);
// /*AFLA*/ 		if (likely(!(cnts & _QW_WMASK)))
// /*AFLA*/ 			return 1;
// /*AFLA*/ 		atomic_sub(_QR_BIAS, &lock->cnts);
// /*AFLA*/ 	}
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * queued_write_trylock - try to acquire write lock of a queue rwlock
// /*AFLA*/  * @lock : Pointer to queue rwlock structure
// /*AFLA*/  * Return: 1 if lock acquired, 0 if failed
// /*AFLA*/  */
// /*AFLA*/ static inline int queued_write_trylock(struct qrwlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	u32 cnts;
// /*AFLA*/ 
// /*AFLA*/ 	cnts = atomic_read(&lock->cnts);
// /*AFLA*/ 	if (unlikely(cnts))
// /*AFLA*/ 		return 0;
// /*AFLA*/ 
// /*AFLA*/ 	return likely(atomic_cmpxchg_acquire(&lock->cnts,
// /*AFLA*/ 					     cnts, cnts | _QW_LOCKED) == cnts);
// /*AFLA*/ }
// /*AFLA*/ /**
// /*AFLA*/  * queued_read_lock - acquire read lock of a queue rwlock
// /*AFLA*/  * @lock: Pointer to queue rwlock structure
// /*AFLA*/  */
// /*AFLA*/ static inline void queued_read_lock(struct qrwlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	u32 cnts;
// /*AFLA*/ 
// /*AFLA*/ 	cnts = atomic_add_return_acquire(_QR_BIAS, &lock->cnts);
// /*AFLA*/ 	if (likely(!(cnts & _QW_WMASK)))
// /*AFLA*/ 		return;
// /*AFLA*/ 
// /*AFLA*/ 	/* The slowpath will decrement the reader count, if necessary. */
// /*AFLA*/ 	queued_read_lock_slowpath(lock, cnts);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * queued_write_lock - acquire write lock of a queue rwlock
// /*AFLA*/  * @lock : Pointer to queue rwlock structure
// /*AFLA*/  */
// /*AFLA*/ static inline void queued_write_lock(struct qrwlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	/* Optimize for the unfair lock case where the fair flag is 0. */
// /*AFLA*/ 	if (atomic_cmpxchg_acquire(&lock->cnts, 0, _QW_LOCKED) == 0)
// /*AFLA*/ 		return;
// /*AFLA*/ 
// /*AFLA*/ 	queued_write_lock_slowpath(lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * queued_read_unlock - release read lock of a queue rwlock
// /*AFLA*/  * @lock : Pointer to queue rwlock structure
// /*AFLA*/  */
// /*AFLA*/ static inline void queued_read_unlock(struct qrwlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Atomically decrement the reader count
// /*AFLA*/ 	 */
// /*AFLA*/ 	(void)atomic_sub_return_release(_QR_BIAS, &lock->cnts);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __qrwlock_write_byte - retrieve the write byte address of a queue rwlock
// /*AFLA*/  * @lock : Pointer to queue rwlock structure
// /*AFLA*/  * Return: the write byte address of a queue rwlock
// /*AFLA*/  */
// /*AFLA*/ static inline u8 *__qrwlock_write_byte(struct qrwlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	return (u8 *)lock + 3 * IS_BUILTIN(CONFIG_CPU_BIG_ENDIAN);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * queued_write_unlock - release write lock of a queue rwlock
// /*AFLA*/  * @lock : Pointer to queue rwlock structure
// /*AFLA*/  */
// /*AFLA*/ static inline void queued_write_unlock(struct qrwlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	smp_store_release(__qrwlock_write_byte(lock), 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Remapping rwlock architecture specific functions to the corresponding
// /*AFLA*/  * queue rwlock functions.
// /*AFLA*/  */
// /*AFLA*/ #define arch_read_can_lock(l)	queued_read_can_lock(l)
// /*AFLA*/ #define arch_write_can_lock(l)	queued_write_can_lock(l)
// /*AFLA*/ #define arch_read_lock(l)	queued_read_lock(l)
// /*AFLA*/ #define arch_write_lock(l)	queued_write_lock(l)
// /*AFLA*/ #define arch_read_trylock(l)	queued_read_trylock(l)
// /*AFLA*/ #define arch_write_trylock(l)	queued_write_trylock(l)
// /*AFLA*/ #define arch_read_unlock(l)	queued_read_unlock(l)
// /*AFLA*/ #define arch_write_unlock(l)	queued_write_unlock(l)
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASM_GENERIC_QRWLOCK_H */
