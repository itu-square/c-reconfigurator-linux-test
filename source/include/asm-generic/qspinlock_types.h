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
// /*AFLA*/  *
// /*AFLA*/  * Authors: Waiman Long <waiman.long@hp.com>
// /*AFLA*/  */
// /*AFLA*/ #ifndef __ASM_GENERIC_QSPINLOCK_TYPES_H
// /*AFLA*/ #define __ASM_GENERIC_QSPINLOCK_TYPES_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Including atomic.h with PARAVIRT on will cause compilation errors because
// /*AFLA*/  * of recursive header file incluson via paravirt_types.h. So don't include
// /*AFLA*/  * it if PARAVIRT is on.
// /*AFLA*/  */
// /*AFLA*/ #ifndef CONFIG_PARAVIRT
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ typedef struct qspinlock {
// /*AFLA*/ 	atomic_t	val;
// /*AFLA*/ } arch_spinlock_t;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Initializier
// /*AFLA*/  */
// /*AFLA*/ #define	__ARCH_SPIN_LOCK_UNLOCKED	{ ATOMIC_INIT(0) }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Bitfields in the atomic value:
// /*AFLA*/  *
// /*AFLA*/  * When NR_CPUS < 16K
// /*AFLA*/  *  0- 7: locked byte
// /*AFLA*/  *     8: pending
// /*AFLA*/  *  9-15: not used
// /*AFLA*/  * 16-17: tail index
// /*AFLA*/  * 18-31: tail cpu (+1)
// /*AFLA*/  *
// /*AFLA*/  * When NR_CPUS >= 16K
// /*AFLA*/  *  0- 7: locked byte
// /*AFLA*/  *     8: pending
// /*AFLA*/  *  9-10: tail index
// /*AFLA*/  * 11-31: tail cpu (+1)
// /*AFLA*/  */
// /*AFLA*/ #define	_Q_SET_MASK(type)	(((1U << _Q_ ## type ## _BITS) - 1)\
// /*AFLA*/ 				      << _Q_ ## type ## _OFFSET)
// /*AFLA*/ #define _Q_LOCKED_OFFSET	0
// /*AFLA*/ #define _Q_LOCKED_BITS		8
// /*AFLA*/ #define _Q_LOCKED_MASK		_Q_SET_MASK(LOCKED)
// /*AFLA*/ 
// /*AFLA*/ #define _Q_PENDING_OFFSET	(_Q_LOCKED_OFFSET + _Q_LOCKED_BITS)
// /*AFLA*/ #if CONFIG_NR_CPUS < (1U << 14)
// /*AFLA*/ #define _Q_PENDING_BITS		8
// /*AFLA*/ #else
// /*AFLA*/ #define _Q_PENDING_BITS		1
// /*AFLA*/ #endif
// /*AFLA*/ #define _Q_PENDING_MASK		_Q_SET_MASK(PENDING)
// /*AFLA*/ 
// /*AFLA*/ #define _Q_TAIL_IDX_OFFSET	(_Q_PENDING_OFFSET + _Q_PENDING_BITS)
// /*AFLA*/ #define _Q_TAIL_IDX_BITS	2
// /*AFLA*/ #define _Q_TAIL_IDX_MASK	_Q_SET_MASK(TAIL_IDX)
// /*AFLA*/ 
// /*AFLA*/ #define _Q_TAIL_CPU_OFFSET	(_Q_TAIL_IDX_OFFSET + _Q_TAIL_IDX_BITS)
// /*AFLA*/ #define _Q_TAIL_CPU_BITS	(32 - _Q_TAIL_CPU_OFFSET)
// /*AFLA*/ #define _Q_TAIL_CPU_MASK	_Q_SET_MASK(TAIL_CPU)
// /*AFLA*/ 
// /*AFLA*/ #define _Q_TAIL_OFFSET		_Q_TAIL_IDX_OFFSET
// /*AFLA*/ #define _Q_TAIL_MASK		(_Q_TAIL_IDX_MASK | _Q_TAIL_CPU_MASK)
// /*AFLA*/ 
// /*AFLA*/ #define _Q_LOCKED_VAL		(1U << _Q_LOCKED_OFFSET)
// /*AFLA*/ #define _Q_PENDING_VAL		(1U << _Q_PENDING_OFFSET)
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASM_GENERIC_QSPINLOCK_TYPES_H */
