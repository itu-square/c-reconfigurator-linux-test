// /*AFLA*/ /*
// /*AFLA*/  * Generic barrier definitions, originally based on MN10300 definitions.
// /*AFLA*/  *
// /*AFLA*/  * It should be possible to use these on really simple architectures,
// /*AFLA*/  * but it serves more as a starting point for new ports.
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
// /*AFLA*/  * Written by David Howells (dhowells@redhat.com)
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or
// /*AFLA*/  * modify it under the terms of the GNU General Public Licence
// /*AFLA*/  * as published by the Free Software Foundation; either version
// /*AFLA*/  * 2 of the Licence, or (at your option) any later version.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __ASM_GENERIC_BARRIER_H
// /*AFLA*/ #define __ASM_GENERIC_BARRIER_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef nop
// /*AFLA*/ #define nop()	asm volatile ("nop")
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Force strict CPU ordering. And yes, this is required on UP too when we're
// /*AFLA*/  * talking to devices.
// /*AFLA*/  *
// /*AFLA*/  * Fall back to compiler barriers if nothing better is provided.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef mb
// /*AFLA*/ #define mb()	barrier()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef rmb
// /*AFLA*/ #define rmb()	mb()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef wmb
// /*AFLA*/ #define wmb()	mb()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef dma_rmb
// /*AFLA*/ #define dma_rmb()	rmb()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef dma_wmb
// /*AFLA*/ #define dma_wmb()	wmb()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef read_barrier_depends
// /*AFLA*/ #define read_barrier_depends()		do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __smp_mb
// /*AFLA*/ #define __smp_mb()	mb()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __smp_rmb
// /*AFLA*/ #define __smp_rmb()	rmb()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __smp_wmb
// /*AFLA*/ #define __smp_wmb()	wmb()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __smp_read_barrier_depends
// /*AFLA*/ #define __smp_read_barrier_depends()	read_barrier_depends()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_mb
// /*AFLA*/ #define smp_mb()	__smp_mb()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_rmb
// /*AFLA*/ #define smp_rmb()	__smp_rmb()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_wmb
// /*AFLA*/ #define smp_wmb()	__smp_wmb()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_read_barrier_depends
// /*AFLA*/ #define smp_read_barrier_depends()	__smp_read_barrier_depends()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #else	/* !CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_mb
// /*AFLA*/ #define smp_mb()	barrier()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_rmb
// /*AFLA*/ #define smp_rmb()	barrier()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_wmb
// /*AFLA*/ #define smp_wmb()	barrier()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_read_barrier_depends
// /*AFLA*/ #define smp_read_barrier_depends()	do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif	/* CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __smp_store_mb
// /*AFLA*/ #define __smp_store_mb(var, value)  do { WRITE_ONCE(var, value); __smp_mb(); } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __smp_mb__before_atomic
// /*AFLA*/ #define __smp_mb__before_atomic()	__smp_mb()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __smp_mb__after_atomic
// /*AFLA*/ #define __smp_mb__after_atomic()	__smp_mb()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __smp_store_release
// /*AFLA*/ #define __smp_store_release(p, v)					\
// /*AFLA*/ do {									\
// /*AFLA*/ 	compiletime_assert_atomic_type(*p);				\
// /*AFLA*/ 	__smp_mb();							\
// /*AFLA*/ 	WRITE_ONCE(*p, v);						\
// /*AFLA*/ } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __smp_load_acquire
// /*AFLA*/ #define __smp_load_acquire(p)						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(*p) ___p1 = READ_ONCE(*p);				\
// /*AFLA*/ 	compiletime_assert_atomic_type(*p);				\
// /*AFLA*/ 	__smp_mb();							\
// /*AFLA*/ 	___p1;								\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_store_mb
// /*AFLA*/ #define smp_store_mb(var, value)  __smp_store_mb(var, value)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_mb__before_atomic
// /*AFLA*/ #define smp_mb__before_atomic()	__smp_mb__before_atomic()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_mb__after_atomic
// /*AFLA*/ #define smp_mb__after_atomic()	__smp_mb__after_atomic()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_store_release
// /*AFLA*/ #define smp_store_release(p, v) __smp_store_release(p, v)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_load_acquire
// /*AFLA*/ #define smp_load_acquire(p) __smp_load_acquire(p)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #else	/* !CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_store_mb
// /*AFLA*/ #define smp_store_mb(var, value)  do { WRITE_ONCE(var, value); barrier(); } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_mb__before_atomic
// /*AFLA*/ #define smp_mb__before_atomic()	barrier()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_mb__after_atomic
// /*AFLA*/ #define smp_mb__after_atomic()	barrier()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_store_release
// /*AFLA*/ #define smp_store_release(p, v)						\
// /*AFLA*/ do {									\
// /*AFLA*/ 	compiletime_assert_atomic_type(*p);				\
// /*AFLA*/ 	barrier();							\
// /*AFLA*/ 	WRITE_ONCE(*p, v);						\
// /*AFLA*/ } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef smp_load_acquire
// /*AFLA*/ #define smp_load_acquire(p)						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(*p) ___p1 = READ_ONCE(*p);				\
// /*AFLA*/ 	compiletime_assert_atomic_type(*p);				\
// /*AFLA*/ 	barrier();							\
// /*AFLA*/ 	___p1;								\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif	/* CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ /* Barriers for virtual machine guests when talking to an SMP host */
// /*AFLA*/ #define virt_mb() __smp_mb()
// /*AFLA*/ #define virt_rmb() __smp_rmb()
// /*AFLA*/ #define virt_wmb() __smp_wmb()
// /*AFLA*/ #define virt_read_barrier_depends() __smp_read_barrier_depends()
// /*AFLA*/ #define virt_store_mb(var, value) __smp_store_mb(var, value)
// /*AFLA*/ #define virt_mb__before_atomic() __smp_mb__before_atomic()
// /*AFLA*/ #define virt_mb__after_atomic()	__smp_mb__after_atomic()
// /*AFLA*/ #define virt_store_release(p, v) __smp_store_release(p, v)
// /*AFLA*/ #define virt_load_acquire(p) __smp_load_acquire(p)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * smp_acquire__after_ctrl_dep() - Provide ACQUIRE ordering after a control dependency
// /*AFLA*/  *
// /*AFLA*/  * A control dependency provides a LOAD->STORE order, the additional RMB
// /*AFLA*/  * provides LOAD->LOAD order, together they provide LOAD->{LOAD,STORE} order,
// /*AFLA*/  * aka. (load)-ACQUIRE.
// /*AFLA*/  *
// /*AFLA*/  * Architectures that do not do load speculation can have this be barrier().
// /*AFLA*/  */
// /*AFLA*/ #ifndef smp_acquire__after_ctrl_dep
// /*AFLA*/ #define smp_acquire__after_ctrl_dep()		smp_rmb()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * smp_cond_load_acquire() - (Spin) wait for cond with ACQUIRE ordering
// /*AFLA*/  * @ptr: pointer to the variable to wait on
// /*AFLA*/  * @cond: boolean expression to wait for
// /*AFLA*/  *
// /*AFLA*/  * Equivalent to using smp_load_acquire() on the condition variable but employs
// /*AFLA*/  * the control dependency of the wait to reduce the barrier on many platforms.
// /*AFLA*/  *
// /*AFLA*/  * Due to C lacking lambda expressions we load the value of *ptr into a
// /*AFLA*/  * pre-named variable @VAL to be used in @cond.
// /*AFLA*/  */
// /*AFLA*/ #ifndef smp_cond_load_acquire
// /*AFLA*/ #define smp_cond_load_acquire(ptr, cond_expr) ({		\
// /*AFLA*/ 	typeof(ptr) __PTR = (ptr);				\
// /*AFLA*/ 	typeof(*ptr) VAL;					\
// /*AFLA*/ 	for (;;) {						\
// /*AFLA*/ 		VAL = READ_ONCE(*__PTR);			\
// /*AFLA*/ 		if (cond_expr)					\
// /*AFLA*/ 			break;					\
// /*AFLA*/ 		cpu_relax();					\
// /*AFLA*/ 	}							\
// /*AFLA*/ 	smp_acquire__after_ctrl_dep();				\
// /*AFLA*/ 	VAL;							\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* !__ASSEMBLY__ */
// /*AFLA*/ #endif /* __ASM_GENERIC_BARRIER_H */
