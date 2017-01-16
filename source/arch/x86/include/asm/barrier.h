// /*AFLA*/ #ifndef _ASM_X86_BARRIER_H
// /*AFLA*/ #define _ASM_X86_BARRIER_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/alternative.h>
// /*AFLA*/ #include <asm/nops.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Force strict CPU ordering.
// /*AFLA*/  * And yes, this might be required on UP too when we're talking
// /*AFLA*/  * to devices.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ #define mb() asm volatile(ALTERNATIVE("lock; addl $0,0(%%esp)", "mfence", \
// /*AFLA*/ 				      X86_FEATURE_XMM2) ::: "memory", "cc")
// /*AFLA*/ #define rmb() asm volatile(ALTERNATIVE("lock; addl $0,0(%%esp)", "lfence", \
// /*AFLA*/ 				       X86_FEATURE_XMM2) ::: "memory", "cc")
// /*AFLA*/ #define wmb() asm volatile(ALTERNATIVE("lock; addl $0,0(%%esp)", "sfence", \
// /*AFLA*/ 				       X86_FEATURE_XMM2) ::: "memory", "cc")
// /*AFLA*/ #else
// /*AFLA*/ #define mb() 	asm volatile("mfence":::"memory")
// /*AFLA*/ #define rmb()	asm volatile("lfence":::"memory")
// /*AFLA*/ #define wmb()	asm volatile("sfence" ::: "memory")
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_PPRO_FENCE
// /*AFLA*/ #define dma_rmb()	rmb()
// /*AFLA*/ #else
// /*AFLA*/ #define dma_rmb()	barrier()
// /*AFLA*/ #endif
// /*AFLA*/ #define dma_wmb()	barrier()
// /*AFLA*/ 
// /*AFLA*/ #define __smp_mb()	mb()
// /*AFLA*/ #define __smp_rmb()	dma_rmb()
// /*AFLA*/ #define __smp_wmb()	barrier()
// /*AFLA*/ #define __smp_store_mb(var, value) do { (void)xchg(&var, value); } while (0)
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_X86_PPRO_FENCE)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * For this option x86 doesn't have a strong TSO memory
// /*AFLA*/  * model and we should fall back to full barriers.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define __smp_store_release(p, v)					\
// /*AFLA*/ do {									\
// /*AFLA*/ 	compiletime_assert_atomic_type(*p);				\
// /*AFLA*/ 	__smp_mb();							\
// /*AFLA*/ 	WRITE_ONCE(*p, v);						\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define __smp_load_acquire(p)						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(*p) ___p1 = READ_ONCE(*p);				\
// /*AFLA*/ 	compiletime_assert_atomic_type(*p);				\
// /*AFLA*/ 	__smp_mb();							\
// /*AFLA*/ 	___p1;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #else /* regular x86 TSO memory ordering */
// /*AFLA*/ 
// /*AFLA*/ #define __smp_store_release(p, v)					\
// /*AFLA*/ do {									\
// /*AFLA*/ 	compiletime_assert_atomic_type(*p);				\
// /*AFLA*/ 	barrier();							\
// /*AFLA*/ 	WRITE_ONCE(*p, v);						\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define __smp_load_acquire(p)						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(*p) ___p1 = READ_ONCE(*p);				\
// /*AFLA*/ 	compiletime_assert_atomic_type(*p);				\
// /*AFLA*/ 	barrier();							\
// /*AFLA*/ 	___p1;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Atomic operations are already serializing on x86 */
// /*AFLA*/ #define __smp_mb__before_atomic()	barrier()
// /*AFLA*/ #define __smp_mb__after_atomic()	barrier()
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/barrier.h>
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_BARRIER_H */
