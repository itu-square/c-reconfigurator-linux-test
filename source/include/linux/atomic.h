// /*AFLA*/ /* Atomic operations usable in machine independent code */
// /*AFLA*/ #ifndef _LINUX_ATOMIC_H
// /*AFLA*/ #define _LINUX_ATOMIC_H
// /*AFLA*/ #include <asm/atomic.h>
// /*AFLA*/ #include <asm/barrier.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Relaxed variants of xchg, cmpxchg and some atomic operations.
// /*AFLA*/  *
// /*AFLA*/  * We support four variants:
// /*AFLA*/  *
// /*AFLA*/  * - Fully ordered: The default implementation, no suffix required.
// /*AFLA*/  * - Acquire: Provides ACQUIRE semantics, _acquire suffix.
// /*AFLA*/  * - Release: Provides RELEASE semantics, _release suffix.
// /*AFLA*/  * - Relaxed: No ordering guarantees, _relaxed suffix.
// /*AFLA*/  *
// /*AFLA*/  * For compound atomics performing both a load and a store, ACQUIRE
// /*AFLA*/  * semantics apply only to the load and RELEASE semantics only to the
// /*AFLA*/  * store portion of the operation. Note that a failed cmpxchg_acquire
// /*AFLA*/  * does -not- imply any memory ordering constraints.
// /*AFLA*/  *
// /*AFLA*/  * See Documentation/memory-barriers.txt for ACQUIRE/RELEASE definitions.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_read_acquire
// /*AFLA*/ #define  atomic_read_acquire(v)		smp_load_acquire(&(v)->counter)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_set_release
// /*AFLA*/ #define  atomic_set_release(v, i)	smp_store_release(&(v)->counter, (i))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The idea here is to build acquire/release variants by adding explicit
// /*AFLA*/  * barriers on top of the relaxed variant. In the case where the relaxed
// /*AFLA*/  * variant is already fully ordered, no additional barriers are needed.
// /*AFLA*/  *
// /*AFLA*/  * Besides, if an arch has a special barrier for acquire/release, it could
// /*AFLA*/  * implement its own __atomic_op_* and use the same framework for building
// /*AFLA*/  * variants
// /*AFLA*/  */
// /*AFLA*/ #ifndef __atomic_op_acquire
// /*AFLA*/ #define __atomic_op_acquire(op, args...)				\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(op##_relaxed(args)) __ret  = op##_relaxed(args);		\
// /*AFLA*/ 	smp_mb__after_atomic();						\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __atomic_op_release
// /*AFLA*/ #define __atomic_op_release(op, args...)				\
// /*AFLA*/ ({									\
// /*AFLA*/ 	smp_mb__before_atomic();					\
// /*AFLA*/ 	op##_relaxed(args);						\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __atomic_op_fence
// /*AFLA*/ #define __atomic_op_fence(op, args...)					\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(op##_relaxed(args)) __ret;				\
// /*AFLA*/ 	smp_mb__before_atomic();					\
// /*AFLA*/ 	__ret = op##_relaxed(args);					\
// /*AFLA*/ 	smp_mb__after_atomic();						\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* atomic_add_return_relaxed */
// /*AFLA*/ #ifndef atomic_add_return_relaxed
// /*AFLA*/ #define  atomic_add_return_relaxed	atomic_add_return
// /*AFLA*/ #define  atomic_add_return_acquire	atomic_add_return
// /*AFLA*/ #define  atomic_add_return_release	atomic_add_return
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic_add_return_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_add_return_acquire
// /*AFLA*/ #define  atomic_add_return_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic_add_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_add_return_release
// /*AFLA*/ #define  atomic_add_return_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic_add_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_add_return
// /*AFLA*/ #define  atomic_add_return(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic_add_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic_add_return_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic_inc_return_relaxed */
// /*AFLA*/ #ifndef atomic_inc_return_relaxed
// /*AFLA*/ #define  atomic_inc_return_relaxed	atomic_inc_return
// /*AFLA*/ #define  atomic_inc_return_acquire	atomic_inc_return
// /*AFLA*/ #define  atomic_inc_return_release	atomic_inc_return
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic_inc_return_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_inc_return_acquire
// /*AFLA*/ #define  atomic_inc_return_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic_inc_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_inc_return_release
// /*AFLA*/ #define  atomic_inc_return_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic_inc_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_inc_return
// /*AFLA*/ #define  atomic_inc_return(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic_inc_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic_inc_return_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic_sub_return_relaxed */
// /*AFLA*/ #ifndef atomic_sub_return_relaxed
// /*AFLA*/ #define  atomic_sub_return_relaxed	atomic_sub_return
// /*AFLA*/ #define  atomic_sub_return_acquire	atomic_sub_return
// /*AFLA*/ #define  atomic_sub_return_release	atomic_sub_return
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic_sub_return_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_sub_return_acquire
// /*AFLA*/ #define  atomic_sub_return_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic_sub_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_sub_return_release
// /*AFLA*/ #define  atomic_sub_return_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic_sub_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_sub_return
// /*AFLA*/ #define  atomic_sub_return(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic_sub_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic_sub_return_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic_dec_return_relaxed */
// /*AFLA*/ #ifndef atomic_dec_return_relaxed
// /*AFLA*/ #define  atomic_dec_return_relaxed	atomic_dec_return
// /*AFLA*/ #define  atomic_dec_return_acquire	atomic_dec_return
// /*AFLA*/ #define  atomic_dec_return_release	atomic_dec_return
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic_dec_return_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_dec_return_acquire
// /*AFLA*/ #define  atomic_dec_return_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic_dec_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_dec_return_release
// /*AFLA*/ #define  atomic_dec_return_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic_dec_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_dec_return
// /*AFLA*/ #define  atomic_dec_return(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic_dec_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic_dec_return_relaxed */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* atomic_fetch_add_relaxed */
// /*AFLA*/ #ifndef atomic_fetch_add_relaxed
// /*AFLA*/ #define atomic_fetch_add_relaxed	atomic_fetch_add
// /*AFLA*/ #define atomic_fetch_add_acquire	atomic_fetch_add
// /*AFLA*/ #define atomic_fetch_add_release	atomic_fetch_add
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic_fetch_add_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_add_acquire
// /*AFLA*/ #define atomic_fetch_add_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic_fetch_add, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_add_release
// /*AFLA*/ #define atomic_fetch_add_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic_fetch_add, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_add
// /*AFLA*/ #define atomic_fetch_add(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic_fetch_add, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic_fetch_add_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic_fetch_inc_relaxed */
// /*AFLA*/ #ifndef atomic_fetch_inc_relaxed
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_inc
// /*AFLA*/ #define atomic_fetch_inc(v)	        atomic_fetch_add(1, (v))
// /*AFLA*/ #define atomic_fetch_inc_relaxed(v)	atomic_fetch_add_relaxed(1, (v))
// /*AFLA*/ #define atomic_fetch_inc_acquire(v)	atomic_fetch_add_acquire(1, (v))
// /*AFLA*/ #define atomic_fetch_inc_release(v)	atomic_fetch_add_release(1, (v))
// /*AFLA*/ #else /* atomic_fetch_inc */
// /*AFLA*/ #define atomic_fetch_inc_relaxed	atomic_fetch_inc
// /*AFLA*/ #define atomic_fetch_inc_acquire	atomic_fetch_inc
// /*AFLA*/ #define atomic_fetch_inc_release	atomic_fetch_inc
// /*AFLA*/ #endif /* atomic_fetch_inc */
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic_fetch_inc_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_inc_acquire
// /*AFLA*/ #define atomic_fetch_inc_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic_fetch_inc, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_inc_release
// /*AFLA*/ #define atomic_fetch_inc_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic_fetch_inc, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_inc
// /*AFLA*/ #define atomic_fetch_inc(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic_fetch_inc, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic_fetch_inc_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic_fetch_sub_relaxed */
// /*AFLA*/ #ifndef atomic_fetch_sub_relaxed
// /*AFLA*/ #define atomic_fetch_sub_relaxed	atomic_fetch_sub
// /*AFLA*/ #define atomic_fetch_sub_acquire	atomic_fetch_sub
// /*AFLA*/ #define atomic_fetch_sub_release	atomic_fetch_sub
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic_fetch_sub_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_sub_acquire
// /*AFLA*/ #define atomic_fetch_sub_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic_fetch_sub, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_sub_release
// /*AFLA*/ #define atomic_fetch_sub_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic_fetch_sub, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_sub
// /*AFLA*/ #define atomic_fetch_sub(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic_fetch_sub, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic_fetch_sub_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic_fetch_dec_relaxed */
// /*AFLA*/ #ifndef atomic_fetch_dec_relaxed
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_dec
// /*AFLA*/ #define atomic_fetch_dec(v)	        atomic_fetch_sub(1, (v))
// /*AFLA*/ #define atomic_fetch_dec_relaxed(v)	atomic_fetch_sub_relaxed(1, (v))
// /*AFLA*/ #define atomic_fetch_dec_acquire(v)	atomic_fetch_sub_acquire(1, (v))
// /*AFLA*/ #define atomic_fetch_dec_release(v)	atomic_fetch_sub_release(1, (v))
// /*AFLA*/ #else /* atomic_fetch_dec */
// /*AFLA*/ #define atomic_fetch_dec_relaxed	atomic_fetch_dec
// /*AFLA*/ #define atomic_fetch_dec_acquire	atomic_fetch_dec
// /*AFLA*/ #define atomic_fetch_dec_release	atomic_fetch_dec
// /*AFLA*/ #endif /* atomic_fetch_dec */
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic_fetch_dec_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_dec_acquire
// /*AFLA*/ #define atomic_fetch_dec_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic_fetch_dec, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_dec_release
// /*AFLA*/ #define atomic_fetch_dec_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic_fetch_dec, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_dec
// /*AFLA*/ #define atomic_fetch_dec(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic_fetch_dec, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic_fetch_dec_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic_fetch_or_relaxed */
// /*AFLA*/ #ifndef atomic_fetch_or_relaxed
// /*AFLA*/ #define atomic_fetch_or_relaxed	atomic_fetch_or
// /*AFLA*/ #define atomic_fetch_or_acquire	atomic_fetch_or
// /*AFLA*/ #define atomic_fetch_or_release	atomic_fetch_or
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic_fetch_or_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_or_acquire
// /*AFLA*/ #define atomic_fetch_or_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic_fetch_or, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_or_release
// /*AFLA*/ #define atomic_fetch_or_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic_fetch_or, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_or
// /*AFLA*/ #define atomic_fetch_or(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic_fetch_or, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic_fetch_or_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic_fetch_and_relaxed */
// /*AFLA*/ #ifndef atomic_fetch_and_relaxed
// /*AFLA*/ #define atomic_fetch_and_relaxed	atomic_fetch_and
// /*AFLA*/ #define atomic_fetch_and_acquire	atomic_fetch_and
// /*AFLA*/ #define atomic_fetch_and_release	atomic_fetch_and
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic_fetch_and_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_and_acquire
// /*AFLA*/ #define atomic_fetch_and_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic_fetch_and, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_and_release
// /*AFLA*/ #define atomic_fetch_and_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic_fetch_and, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_and
// /*AFLA*/ #define atomic_fetch_and(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic_fetch_and, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic_fetch_and_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifdef atomic_andnot
// /*AFLA*/ /* atomic_fetch_andnot_relaxed */
// /*AFLA*/ #ifndef atomic_fetch_andnot_relaxed
// /*AFLA*/ #define atomic_fetch_andnot_relaxed	atomic_fetch_andnot
// /*AFLA*/ #define atomic_fetch_andnot_acquire	atomic_fetch_andnot
// /*AFLA*/ #define atomic_fetch_andnot_release	atomic_fetch_andnot
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic_fetch_andnot_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_andnot_acquire
// /*AFLA*/ #define atomic_fetch_andnot_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic_fetch_andnot, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_andnot_release
// /*AFLA*/ #define atomic_fetch_andnot_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic_fetch_andnot, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_andnot
// /*AFLA*/ #define atomic_fetch_andnot(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic_fetch_andnot, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic_fetch_andnot_relaxed */
// /*AFLA*/ #endif /* atomic_andnot */
// /*AFLA*/ 
// /*AFLA*/ /* atomic_fetch_xor_relaxed */
// /*AFLA*/ #ifndef atomic_fetch_xor_relaxed
// /*AFLA*/ #define atomic_fetch_xor_relaxed	atomic_fetch_xor
// /*AFLA*/ #define atomic_fetch_xor_acquire	atomic_fetch_xor
// /*AFLA*/ #define atomic_fetch_xor_release	atomic_fetch_xor
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic_fetch_xor_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_xor_acquire
// /*AFLA*/ #define atomic_fetch_xor_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic_fetch_xor, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_xor_release
// /*AFLA*/ #define atomic_fetch_xor_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic_fetch_xor, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_fetch_xor
// /*AFLA*/ #define atomic_fetch_xor(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic_fetch_xor, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic_fetch_xor_relaxed */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* atomic_xchg_relaxed */
// /*AFLA*/ #ifndef atomic_xchg_relaxed
// /*AFLA*/ #define  atomic_xchg_relaxed		atomic_xchg
// /*AFLA*/ #define  atomic_xchg_acquire		atomic_xchg
// /*AFLA*/ #define  atomic_xchg_release		atomic_xchg
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic_xchg_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_xchg_acquire
// /*AFLA*/ #define  atomic_xchg_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic_xchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_xchg_release
// /*AFLA*/ #define  atomic_xchg_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic_xchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_xchg
// /*AFLA*/ #define  atomic_xchg(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic_xchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic_xchg_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic_cmpxchg_relaxed */
// /*AFLA*/ #ifndef atomic_cmpxchg_relaxed
// /*AFLA*/ #define  atomic_cmpxchg_relaxed		atomic_cmpxchg
// /*AFLA*/ #define  atomic_cmpxchg_acquire		atomic_cmpxchg
// /*AFLA*/ #define  atomic_cmpxchg_release		atomic_cmpxchg
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic_cmpxchg_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_cmpxchg_acquire
// /*AFLA*/ #define  atomic_cmpxchg_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic_cmpxchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_cmpxchg_release
// /*AFLA*/ #define  atomic_cmpxchg_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic_cmpxchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_cmpxchg
// /*AFLA*/ #define  atomic_cmpxchg(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic_cmpxchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic_cmpxchg_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* cmpxchg_relaxed */
// /*AFLA*/ #ifndef cmpxchg_relaxed
// /*AFLA*/ #define  cmpxchg_relaxed		cmpxchg
// /*AFLA*/ #define  cmpxchg_acquire		cmpxchg
// /*AFLA*/ #define  cmpxchg_release		cmpxchg
// /*AFLA*/ 
// /*AFLA*/ #else /* cmpxchg_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef cmpxchg_acquire
// /*AFLA*/ #define  cmpxchg_acquire(...)						\
// /*AFLA*/ 	__atomic_op_acquire(cmpxchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef cmpxchg_release
// /*AFLA*/ #define  cmpxchg_release(...)						\
// /*AFLA*/ 	__atomic_op_release(cmpxchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef cmpxchg
// /*AFLA*/ #define  cmpxchg(...)							\
// /*AFLA*/ 	__atomic_op_fence(cmpxchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* cmpxchg_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* cmpxchg64_relaxed */
// /*AFLA*/ #ifndef cmpxchg64_relaxed
// /*AFLA*/ #define  cmpxchg64_relaxed		cmpxchg64
// /*AFLA*/ #define  cmpxchg64_acquire		cmpxchg64
// /*AFLA*/ #define  cmpxchg64_release		cmpxchg64
// /*AFLA*/ 
// /*AFLA*/ #else /* cmpxchg64_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef cmpxchg64_acquire
// /*AFLA*/ #define  cmpxchg64_acquire(...)						\
// /*AFLA*/ 	__atomic_op_acquire(cmpxchg64, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef cmpxchg64_release
// /*AFLA*/ #define  cmpxchg64_release(...)						\
// /*AFLA*/ 	__atomic_op_release(cmpxchg64, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef cmpxchg64
// /*AFLA*/ #define  cmpxchg64(...)							\
// /*AFLA*/ 	__atomic_op_fence(cmpxchg64, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* cmpxchg64_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* xchg_relaxed */
// /*AFLA*/ #ifndef xchg_relaxed
// /*AFLA*/ #define  xchg_relaxed			xchg
// /*AFLA*/ #define  xchg_acquire			xchg
// /*AFLA*/ #define  xchg_release			xchg
// /*AFLA*/ 
// /*AFLA*/ #else /* xchg_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef xchg_acquire
// /*AFLA*/ #define  xchg_acquire(...)		__atomic_op_acquire(xchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef xchg_release
// /*AFLA*/ #define  xchg_release(...)		__atomic_op_release(xchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef xchg
// /*AFLA*/ #define  xchg(...)			__atomic_op_fence(xchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* xchg_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic_add_unless - add unless the number is already a given value
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  * @a: the amount to add to v...
// /*AFLA*/  * @u: ...unless v is equal to u.
// /*AFLA*/  *
// /*AFLA*/  * Atomically adds @a to @v, so long as @v was not already @u.
// /*AFLA*/  * Returns non-zero if @v was not @u, and zero otherwise.
// /*AFLA*/  */
// /*AFLA*/ static inline int atomic_add_unless(atomic_t *v, int a, int u)
// /*AFLA*/ {
// /*AFLA*/ 	return __atomic_add_unless(v, a, u) != u;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic_inc_not_zero - increment unless the number is zero
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically increments @v by 1, so long as @v is non-zero.
// /*AFLA*/  * Returns non-zero if @v was non-zero, and zero otherwise.
// /*AFLA*/  */
// /*AFLA*/ #ifndef atomic_inc_not_zero
// /*AFLA*/ #define atomic_inc_not_zero(v)		atomic_add_unless((v), 1, 0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_andnot
// /*AFLA*/ static inline void atomic_andnot(int i, atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_and(~i, v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int atomic_fetch_andnot(int i, atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_fetch_and(~i, v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int atomic_fetch_andnot_relaxed(int i, atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_fetch_and_relaxed(~i, v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int atomic_fetch_andnot_acquire(int i, atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_fetch_and_acquire(~i, v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int atomic_fetch_andnot_release(int i, atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_fetch_and_release(~i, v);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic_inc_not_zero_hint - increment if not null
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  * @hint: probable value of the atomic before the increment
// /*AFLA*/  *
// /*AFLA*/  * This version of atomic_inc_not_zero() gives a hint of probable
// /*AFLA*/  * value of the atomic. This helps processor to not read the memory
// /*AFLA*/  * before doing the atomic read/modify/write cycle, lowering
// /*AFLA*/  * number of bus transactions on some arches.
// /*AFLA*/  *
// /*AFLA*/  * Returns: 0 if increment was not done, 1 otherwise.
// /*AFLA*/  */
// /*AFLA*/ #ifndef atomic_inc_not_zero_hint
// /*AFLA*/ static inline int atomic_inc_not_zero_hint(atomic_t *v, int hint)
// /*AFLA*/ {
// /*AFLA*/ 	int val, c = hint;
// /*AFLA*/ 
// /*AFLA*/ 	/* sanity test, should be removed by compiler if hint is a constant */
// /*AFLA*/ 	if (!hint)
// /*AFLA*/ 		return atomic_inc_not_zero(v);
// /*AFLA*/ 
// /*AFLA*/ 	do {
// /*AFLA*/ 		val = atomic_cmpxchg(v, c, c + 1);
// /*AFLA*/ 		if (val == c)
// /*AFLA*/ 			return 1;
// /*AFLA*/ 		c = val;
// /*AFLA*/ 	} while (c);
// /*AFLA*/ 
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_inc_unless_negative
// /*AFLA*/ static inline int atomic_inc_unless_negative(atomic_t *p)
// /*AFLA*/ {
// /*AFLA*/ 	int v, v1;
// /*AFLA*/ 	for (v = 0; v >= 0; v = v1) {
// /*AFLA*/ 		v1 = atomic_cmpxchg(p, v, v + 1);
// /*AFLA*/ 		if (likely(v1 == v))
// /*AFLA*/ 			return 1;
// /*AFLA*/ 	}
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic_dec_unless_positive
// /*AFLA*/ static inline int atomic_dec_unless_positive(atomic_t *p)
// /*AFLA*/ {
// /*AFLA*/ 	int v, v1;
// /*AFLA*/ 	for (v = 0; v <= 0; v = v1) {
// /*AFLA*/ 		v1 = atomic_cmpxchg(p, v, v - 1);
// /*AFLA*/ 		if (likely(v1 == v))
// /*AFLA*/ 			return 1;
// /*AFLA*/ 	}
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * atomic_dec_if_positive - decrement by 1 if old value positive
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  *
// /*AFLA*/  * The function returns the old value of *v minus 1, even if
// /*AFLA*/  * the atomic variable, v, was not decremented.
// /*AFLA*/  */
// /*AFLA*/ #ifndef atomic_dec_if_positive
// /*AFLA*/ static inline int atomic_dec_if_positive(atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	int c, old, dec;
// /*AFLA*/ 	c = atomic_read(v);
// /*AFLA*/ 	for (;;) {
// /*AFLA*/ 		dec = c - 1;
// /*AFLA*/ 		if (unlikely(dec < 0))
// /*AFLA*/ 			break;
// /*AFLA*/ 		old = atomic_cmpxchg((v), c, dec);
// /*AFLA*/ 		if (likely(old == c))
// /*AFLA*/ 			break;
// /*AFLA*/ 		c = old;
// /*AFLA*/ 	}
// /*AFLA*/ 	return dec;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_GENERIC_ATOMIC64
// /*AFLA*/ #include <asm-generic/atomic64.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_read_acquire
// /*AFLA*/ #define  atomic64_read_acquire(v)	smp_load_acquire(&(v)->counter)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_set_release
// /*AFLA*/ #define  atomic64_set_release(v, i)	smp_store_release(&(v)->counter, (i))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* atomic64_add_return_relaxed */
// /*AFLA*/ #ifndef atomic64_add_return_relaxed
// /*AFLA*/ #define  atomic64_add_return_relaxed	atomic64_add_return
// /*AFLA*/ #define  atomic64_add_return_acquire	atomic64_add_return
// /*AFLA*/ #define  atomic64_add_return_release	atomic64_add_return
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic64_add_return_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_add_return_acquire
// /*AFLA*/ #define  atomic64_add_return_acquire(...)				\
// /*AFLA*/ 	__atomic_op_acquire(atomic64_add_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_add_return_release
// /*AFLA*/ #define  atomic64_add_return_release(...)				\
// /*AFLA*/ 	__atomic_op_release(atomic64_add_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_add_return
// /*AFLA*/ #define  atomic64_add_return(...)					\
// /*AFLA*/ 	__atomic_op_fence(atomic64_add_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic64_add_return_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic64_inc_return_relaxed */
// /*AFLA*/ #ifndef atomic64_inc_return_relaxed
// /*AFLA*/ #define  atomic64_inc_return_relaxed	atomic64_inc_return
// /*AFLA*/ #define  atomic64_inc_return_acquire	atomic64_inc_return
// /*AFLA*/ #define  atomic64_inc_return_release	atomic64_inc_return
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic64_inc_return_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_inc_return_acquire
// /*AFLA*/ #define  atomic64_inc_return_acquire(...)				\
// /*AFLA*/ 	__atomic_op_acquire(atomic64_inc_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_inc_return_release
// /*AFLA*/ #define  atomic64_inc_return_release(...)				\
// /*AFLA*/ 	__atomic_op_release(atomic64_inc_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_inc_return
// /*AFLA*/ #define  atomic64_inc_return(...)					\
// /*AFLA*/ 	__atomic_op_fence(atomic64_inc_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic64_inc_return_relaxed */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* atomic64_sub_return_relaxed */
// /*AFLA*/ #ifndef atomic64_sub_return_relaxed
// /*AFLA*/ #define  atomic64_sub_return_relaxed	atomic64_sub_return
// /*AFLA*/ #define  atomic64_sub_return_acquire	atomic64_sub_return
// /*AFLA*/ #define  atomic64_sub_return_release	atomic64_sub_return
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic64_sub_return_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_sub_return_acquire
// /*AFLA*/ #define  atomic64_sub_return_acquire(...)				\
// /*AFLA*/ 	__atomic_op_acquire(atomic64_sub_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_sub_return_release
// /*AFLA*/ #define  atomic64_sub_return_release(...)				\
// /*AFLA*/ 	__atomic_op_release(atomic64_sub_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_sub_return
// /*AFLA*/ #define  atomic64_sub_return(...)					\
// /*AFLA*/ 	__atomic_op_fence(atomic64_sub_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic64_sub_return_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic64_dec_return_relaxed */
// /*AFLA*/ #ifndef atomic64_dec_return_relaxed
// /*AFLA*/ #define  atomic64_dec_return_relaxed	atomic64_dec_return
// /*AFLA*/ #define  atomic64_dec_return_acquire	atomic64_dec_return
// /*AFLA*/ #define  atomic64_dec_return_release	atomic64_dec_return
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic64_dec_return_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_dec_return_acquire
// /*AFLA*/ #define  atomic64_dec_return_acquire(...)				\
// /*AFLA*/ 	__atomic_op_acquire(atomic64_dec_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_dec_return_release
// /*AFLA*/ #define  atomic64_dec_return_release(...)				\
// /*AFLA*/ 	__atomic_op_release(atomic64_dec_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_dec_return
// /*AFLA*/ #define  atomic64_dec_return(...)					\
// /*AFLA*/ 	__atomic_op_fence(atomic64_dec_return, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic64_dec_return_relaxed */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* atomic64_fetch_add_relaxed */
// /*AFLA*/ #ifndef atomic64_fetch_add_relaxed
// /*AFLA*/ #define atomic64_fetch_add_relaxed	atomic64_fetch_add
// /*AFLA*/ #define atomic64_fetch_add_acquire	atomic64_fetch_add
// /*AFLA*/ #define atomic64_fetch_add_release	atomic64_fetch_add
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic64_fetch_add_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_add_acquire
// /*AFLA*/ #define atomic64_fetch_add_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic64_fetch_add, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_add_release
// /*AFLA*/ #define atomic64_fetch_add_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic64_fetch_add, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_add
// /*AFLA*/ #define atomic64_fetch_add(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic64_fetch_add, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic64_fetch_add_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic64_fetch_inc_relaxed */
// /*AFLA*/ #ifndef atomic64_fetch_inc_relaxed
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_inc
// /*AFLA*/ #define atomic64_fetch_inc(v)		atomic64_fetch_add(1, (v))
// /*AFLA*/ #define atomic64_fetch_inc_relaxed(v)	atomic64_fetch_add_relaxed(1, (v))
// /*AFLA*/ #define atomic64_fetch_inc_acquire(v)	atomic64_fetch_add_acquire(1, (v))
// /*AFLA*/ #define atomic64_fetch_inc_release(v)	atomic64_fetch_add_release(1, (v))
// /*AFLA*/ #else /* atomic64_fetch_inc */
// /*AFLA*/ #define atomic64_fetch_inc_relaxed	atomic64_fetch_inc
// /*AFLA*/ #define atomic64_fetch_inc_acquire	atomic64_fetch_inc
// /*AFLA*/ #define atomic64_fetch_inc_release	atomic64_fetch_inc
// /*AFLA*/ #endif /* atomic64_fetch_inc */
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic64_fetch_inc_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_inc_acquire
// /*AFLA*/ #define atomic64_fetch_inc_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic64_fetch_inc, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_inc_release
// /*AFLA*/ #define atomic64_fetch_inc_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic64_fetch_inc, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_inc
// /*AFLA*/ #define atomic64_fetch_inc(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic64_fetch_inc, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic64_fetch_inc_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic64_fetch_sub_relaxed */
// /*AFLA*/ #ifndef atomic64_fetch_sub_relaxed
// /*AFLA*/ #define atomic64_fetch_sub_relaxed	atomic64_fetch_sub
// /*AFLA*/ #define atomic64_fetch_sub_acquire	atomic64_fetch_sub
// /*AFLA*/ #define atomic64_fetch_sub_release	atomic64_fetch_sub
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic64_fetch_sub_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_sub_acquire
// /*AFLA*/ #define atomic64_fetch_sub_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic64_fetch_sub, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_sub_release
// /*AFLA*/ #define atomic64_fetch_sub_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic64_fetch_sub, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_sub
// /*AFLA*/ #define atomic64_fetch_sub(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic64_fetch_sub, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic64_fetch_sub_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic64_fetch_dec_relaxed */
// /*AFLA*/ #ifndef atomic64_fetch_dec_relaxed
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_dec
// /*AFLA*/ #define atomic64_fetch_dec(v)		atomic64_fetch_sub(1, (v))
// /*AFLA*/ #define atomic64_fetch_dec_relaxed(v)	atomic64_fetch_sub_relaxed(1, (v))
// /*AFLA*/ #define atomic64_fetch_dec_acquire(v)	atomic64_fetch_sub_acquire(1, (v))
// /*AFLA*/ #define atomic64_fetch_dec_release(v)	atomic64_fetch_sub_release(1, (v))
// /*AFLA*/ #else /* atomic64_fetch_dec */
// /*AFLA*/ #define atomic64_fetch_dec_relaxed	atomic64_fetch_dec
// /*AFLA*/ #define atomic64_fetch_dec_acquire	atomic64_fetch_dec
// /*AFLA*/ #define atomic64_fetch_dec_release	atomic64_fetch_dec
// /*AFLA*/ #endif /* atomic64_fetch_dec */
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic64_fetch_dec_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_dec_acquire
// /*AFLA*/ #define atomic64_fetch_dec_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic64_fetch_dec, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_dec_release
// /*AFLA*/ #define atomic64_fetch_dec_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic64_fetch_dec, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_dec
// /*AFLA*/ #define atomic64_fetch_dec(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic64_fetch_dec, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic64_fetch_dec_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic64_fetch_or_relaxed */
// /*AFLA*/ #ifndef atomic64_fetch_or_relaxed
// /*AFLA*/ #define atomic64_fetch_or_relaxed	atomic64_fetch_or
// /*AFLA*/ #define atomic64_fetch_or_acquire	atomic64_fetch_or
// /*AFLA*/ #define atomic64_fetch_or_release	atomic64_fetch_or
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic64_fetch_or_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_or_acquire
// /*AFLA*/ #define atomic64_fetch_or_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic64_fetch_or, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_or_release
// /*AFLA*/ #define atomic64_fetch_or_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic64_fetch_or, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_or
// /*AFLA*/ #define atomic64_fetch_or(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic64_fetch_or, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic64_fetch_or_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic64_fetch_and_relaxed */
// /*AFLA*/ #ifndef atomic64_fetch_and_relaxed
// /*AFLA*/ #define atomic64_fetch_and_relaxed	atomic64_fetch_and
// /*AFLA*/ #define atomic64_fetch_and_acquire	atomic64_fetch_and
// /*AFLA*/ #define atomic64_fetch_and_release	atomic64_fetch_and
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic64_fetch_and_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_and_acquire
// /*AFLA*/ #define atomic64_fetch_and_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic64_fetch_and, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_and_release
// /*AFLA*/ #define atomic64_fetch_and_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic64_fetch_and, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_and
// /*AFLA*/ #define atomic64_fetch_and(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic64_fetch_and, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic64_fetch_and_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifdef atomic64_andnot
// /*AFLA*/ /* atomic64_fetch_andnot_relaxed */
// /*AFLA*/ #ifndef atomic64_fetch_andnot_relaxed
// /*AFLA*/ #define atomic64_fetch_andnot_relaxed	atomic64_fetch_andnot
// /*AFLA*/ #define atomic64_fetch_andnot_acquire	atomic64_fetch_andnot
// /*AFLA*/ #define atomic64_fetch_andnot_release	atomic64_fetch_andnot
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic64_fetch_andnot_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_andnot_acquire
// /*AFLA*/ #define atomic64_fetch_andnot_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic64_fetch_andnot, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_andnot_release
// /*AFLA*/ #define atomic64_fetch_andnot_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic64_fetch_andnot, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_andnot
// /*AFLA*/ #define atomic64_fetch_andnot(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic64_fetch_andnot, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic64_fetch_andnot_relaxed */
// /*AFLA*/ #endif /* atomic64_andnot */
// /*AFLA*/ 
// /*AFLA*/ /* atomic64_fetch_xor_relaxed */
// /*AFLA*/ #ifndef atomic64_fetch_xor_relaxed
// /*AFLA*/ #define atomic64_fetch_xor_relaxed	atomic64_fetch_xor
// /*AFLA*/ #define atomic64_fetch_xor_acquire	atomic64_fetch_xor
// /*AFLA*/ #define atomic64_fetch_xor_release	atomic64_fetch_xor
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic64_fetch_xor_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_xor_acquire
// /*AFLA*/ #define atomic64_fetch_xor_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic64_fetch_xor, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_xor_release
// /*AFLA*/ #define atomic64_fetch_xor_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic64_fetch_xor, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_fetch_xor
// /*AFLA*/ #define atomic64_fetch_xor(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic64_fetch_xor, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic64_fetch_xor_relaxed */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* atomic64_xchg_relaxed */
// /*AFLA*/ #ifndef atomic64_xchg_relaxed
// /*AFLA*/ #define  atomic64_xchg_relaxed		atomic64_xchg
// /*AFLA*/ #define  atomic64_xchg_acquire		atomic64_xchg
// /*AFLA*/ #define  atomic64_xchg_release		atomic64_xchg
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic64_xchg_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_xchg_acquire
// /*AFLA*/ #define  atomic64_xchg_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic64_xchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_xchg_release
// /*AFLA*/ #define  atomic64_xchg_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic64_xchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_xchg
// /*AFLA*/ #define  atomic64_xchg(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic64_xchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic64_xchg_relaxed */
// /*AFLA*/ 
// /*AFLA*/ /* atomic64_cmpxchg_relaxed */
// /*AFLA*/ #ifndef atomic64_cmpxchg_relaxed
// /*AFLA*/ #define  atomic64_cmpxchg_relaxed	atomic64_cmpxchg
// /*AFLA*/ #define  atomic64_cmpxchg_acquire	atomic64_cmpxchg
// /*AFLA*/ #define  atomic64_cmpxchg_release	atomic64_cmpxchg
// /*AFLA*/ 
// /*AFLA*/ #else /* atomic64_cmpxchg_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_cmpxchg_acquire
// /*AFLA*/ #define  atomic64_cmpxchg_acquire(...)					\
// /*AFLA*/ 	__atomic_op_acquire(atomic64_cmpxchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_cmpxchg_release
// /*AFLA*/ #define  atomic64_cmpxchg_release(...)					\
// /*AFLA*/ 	__atomic_op_release(atomic64_cmpxchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_cmpxchg
// /*AFLA*/ #define  atomic64_cmpxchg(...)						\
// /*AFLA*/ 	__atomic_op_fence(atomic64_cmpxchg, __VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* atomic64_cmpxchg_relaxed */
// /*AFLA*/ 
// /*AFLA*/ #ifndef atomic64_andnot
// /*AFLA*/ static inline void atomic64_andnot(long long i, atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	atomic64_and(~i, v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline long long atomic64_fetch_andnot(long long i, atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic64_fetch_and(~i, v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline long long atomic64_fetch_andnot_relaxed(long long i, atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic64_fetch_and_relaxed(~i, v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline long long atomic64_fetch_andnot_acquire(long long i, atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic64_fetch_and_acquire(~i, v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline long long atomic64_fetch_andnot_release(long long i, atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic64_fetch_and_release(~i, v);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/atomic-long.h>
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_ATOMIC_H */
