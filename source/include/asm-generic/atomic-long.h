// /*AFLA*/ #ifndef _ASM_GENERIC_ATOMIC_LONG_H
// /*AFLA*/ #define _ASM_GENERIC_ATOMIC_LONG_H
// /*AFLA*/ /*
// /*AFLA*/  * Copyright (C) 2005 Silicon Graphics, Inc.
// /*AFLA*/  *	Christoph Lameter
// /*AFLA*/  *
// /*AFLA*/  * Allows to provide arch independent atomic definitions without the need to
// /*AFLA*/  * edit all arch specific atomic.h files.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <asm/types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Suppport for atomic_long_t
// /*AFLA*/  *
// /*AFLA*/  * Casts for parameters are avoided for existing atomic functions in order to
// /*AFLA*/  * avoid issues with cast-as-lval under gcc 4.x and other limitations that the
// /*AFLA*/  * macros of a platform may have.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #if BITS_PER_LONG == 64
// /*AFLA*/ 
// /*AFLA*/ typedef atomic64_t atomic_long_t;
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC_LONG_INIT(i)	ATOMIC64_INIT(i)
// /*AFLA*/ #define ATOMIC_LONG_PFX(x)	atomic64 ## x
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ typedef atomic_t atomic_long_t;
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC_LONG_INIT(i)	ATOMIC_INIT(i)
// /*AFLA*/ #define ATOMIC_LONG_PFX(x)	atomic ## x
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC_LONG_READ_OP(mo)						\
// /*AFLA*/ static inline long atomic_long_read##mo(const atomic_long_t *l)		\
// /*AFLA*/ {									\
// /*AFLA*/ 	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;		\
// /*AFLA*/ 									\
// /*AFLA*/ 	return (long)ATOMIC_LONG_PFX(_read##mo)(v);			\
// /*AFLA*/ }
// /*AFLA*/ ATOMIC_LONG_READ_OP()
// /*AFLA*/ ATOMIC_LONG_READ_OP(_acquire)
// /*AFLA*/ 
// /*AFLA*/ #undef ATOMIC_LONG_READ_OP
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC_LONG_SET_OP(mo)						\
// /*AFLA*/ static inline void atomic_long_set##mo(atomic_long_t *l, long i)	\
// /*AFLA*/ {									\
// /*AFLA*/ 	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;		\
// /*AFLA*/ 									\
// /*AFLA*/ 	ATOMIC_LONG_PFX(_set##mo)(v, i);				\
// /*AFLA*/ }
// /*AFLA*/ ATOMIC_LONG_SET_OP()
// /*AFLA*/ ATOMIC_LONG_SET_OP(_release)
// /*AFLA*/ 
// /*AFLA*/ #undef ATOMIC_LONG_SET_OP
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC_LONG_ADD_SUB_OP(op, mo)					\
// /*AFLA*/ static inline long							\
// /*AFLA*/ atomic_long_##op##_return##mo(long i, atomic_long_t *l)			\
// /*AFLA*/ {									\
// /*AFLA*/ 	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;		\
// /*AFLA*/ 									\
// /*AFLA*/ 	return (long)ATOMIC_LONG_PFX(_##op##_return##mo)(i, v);		\
// /*AFLA*/ }
// /*AFLA*/ ATOMIC_LONG_ADD_SUB_OP(add,)
// /*AFLA*/ ATOMIC_LONG_ADD_SUB_OP(add, _relaxed)
// /*AFLA*/ ATOMIC_LONG_ADD_SUB_OP(add, _acquire)
// /*AFLA*/ ATOMIC_LONG_ADD_SUB_OP(add, _release)
// /*AFLA*/ ATOMIC_LONG_ADD_SUB_OP(sub,)
// /*AFLA*/ ATOMIC_LONG_ADD_SUB_OP(sub, _relaxed)
// /*AFLA*/ ATOMIC_LONG_ADD_SUB_OP(sub, _acquire)
// /*AFLA*/ ATOMIC_LONG_ADD_SUB_OP(sub, _release)
// /*AFLA*/ 
// /*AFLA*/ #undef ATOMIC_LONG_ADD_SUB_OP
// /*AFLA*/ 
// /*AFLA*/ #define atomic_long_cmpxchg_relaxed(l, old, new) \
// /*AFLA*/ 	(ATOMIC_LONG_PFX(_cmpxchg_relaxed)((ATOMIC_LONG_PFX(_t) *)(l), \
// /*AFLA*/ 					   (old), (new)))
// /*AFLA*/ #define atomic_long_cmpxchg_acquire(l, old, new) \
// /*AFLA*/ 	(ATOMIC_LONG_PFX(_cmpxchg_acquire)((ATOMIC_LONG_PFX(_t) *)(l), \
// /*AFLA*/ 					   (old), (new)))
// /*AFLA*/ #define atomic_long_cmpxchg_release(l, old, new) \
// /*AFLA*/ 	(ATOMIC_LONG_PFX(_cmpxchg_release)((ATOMIC_LONG_PFX(_t) *)(l), \
// /*AFLA*/ 					   (old), (new)))
// /*AFLA*/ #define atomic_long_cmpxchg(l, old, new) \
// /*AFLA*/ 	(ATOMIC_LONG_PFX(_cmpxchg)((ATOMIC_LONG_PFX(_t) *)(l), (old), (new)))
// /*AFLA*/ 
// /*AFLA*/ #define atomic_long_xchg_relaxed(v, new) \
// /*AFLA*/ 	(ATOMIC_LONG_PFX(_xchg_relaxed)((ATOMIC_LONG_PFX(_t) *)(v), (new)))
// /*AFLA*/ #define atomic_long_xchg_acquire(v, new) \
// /*AFLA*/ 	(ATOMIC_LONG_PFX(_xchg_acquire)((ATOMIC_LONG_PFX(_t) *)(v), (new)))
// /*AFLA*/ #define atomic_long_xchg_release(v, new) \
// /*AFLA*/ 	(ATOMIC_LONG_PFX(_xchg_release)((ATOMIC_LONG_PFX(_t) *)(v), (new)))
// /*AFLA*/ #define atomic_long_xchg(v, new) \
// /*AFLA*/ 	(ATOMIC_LONG_PFX(_xchg)((ATOMIC_LONG_PFX(_t) *)(v), (new)))
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void atomic_long_inc(atomic_long_t *l)
// /*AFLA*/ {
// /*AFLA*/ 	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;
// /*AFLA*/ 
// /*AFLA*/ 	ATOMIC_LONG_PFX(_inc)(v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void atomic_long_dec(atomic_long_t *l)
// /*AFLA*/ {
// /*AFLA*/ 	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;
// /*AFLA*/ 
// /*AFLA*/ 	ATOMIC_LONG_PFX(_dec)(v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC_LONG_FETCH_OP(op, mo)					\
// /*AFLA*/ static inline long							\
// /*AFLA*/ atomic_long_fetch_##op##mo(long i, atomic_long_t *l)			\
// /*AFLA*/ {									\
// /*AFLA*/ 	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;		\
// /*AFLA*/ 									\
// /*AFLA*/ 	return (long)ATOMIC_LONG_PFX(_fetch_##op##mo)(i, v);		\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(add, )
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(add, _relaxed)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(add, _acquire)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(add, _release)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(sub, )
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(sub, _relaxed)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(sub, _acquire)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(sub, _release)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(and, )
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(and, _relaxed)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(and, _acquire)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(and, _release)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(andnot, )
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(andnot, _relaxed)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(andnot, _acquire)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(andnot, _release)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(or, )
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(or, _relaxed)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(or, _acquire)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(or, _release)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(xor, )
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(xor, _relaxed)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(xor, _acquire)
// /*AFLA*/ ATOMIC_LONG_FETCH_OP(xor, _release)
// /*AFLA*/ 
// /*AFLA*/ #undef ATOMIC_LONG_FETCH_OP
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC_LONG_FETCH_INC_DEC_OP(op, mo)					\
// /*AFLA*/ static inline long							\
// /*AFLA*/ atomic_long_fetch_##op##mo(atomic_long_t *l)				\
// /*AFLA*/ {									\
// /*AFLA*/ 	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;		\
// /*AFLA*/ 									\
// /*AFLA*/ 	return (long)ATOMIC_LONG_PFX(_fetch_##op##mo)(v);		\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ ATOMIC_LONG_FETCH_INC_DEC_OP(inc,)
// /*AFLA*/ ATOMIC_LONG_FETCH_INC_DEC_OP(inc, _relaxed)
// /*AFLA*/ ATOMIC_LONG_FETCH_INC_DEC_OP(inc, _acquire)
// /*AFLA*/ ATOMIC_LONG_FETCH_INC_DEC_OP(inc, _release)
// /*AFLA*/ ATOMIC_LONG_FETCH_INC_DEC_OP(dec,)
// /*AFLA*/ ATOMIC_LONG_FETCH_INC_DEC_OP(dec, _relaxed)
// /*AFLA*/ ATOMIC_LONG_FETCH_INC_DEC_OP(dec, _acquire)
// /*AFLA*/ ATOMIC_LONG_FETCH_INC_DEC_OP(dec, _release)
// /*AFLA*/ 
// /*AFLA*/ #undef ATOMIC_LONG_FETCH_INC_DEC_OP
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC_LONG_OP(op)						\
// /*AFLA*/ static __always_inline void						\
// /*AFLA*/ atomic_long_##op(long i, atomic_long_t *l)				\
// /*AFLA*/ {									\
// /*AFLA*/ 	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;		\
// /*AFLA*/ 									\
// /*AFLA*/ 	ATOMIC_LONG_PFX(_##op)(i, v);					\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ ATOMIC_LONG_OP(add)
// /*AFLA*/ ATOMIC_LONG_OP(sub)
// /*AFLA*/ ATOMIC_LONG_OP(and)
// /*AFLA*/ ATOMIC_LONG_OP(andnot)
// /*AFLA*/ ATOMIC_LONG_OP(or)
// /*AFLA*/ ATOMIC_LONG_OP(xor)
// /*AFLA*/ 
// /*AFLA*/ #undef ATOMIC_LONG_OP
// /*AFLA*/ 
// /*AFLA*/ static inline int atomic_long_sub_and_test(long i, atomic_long_t *l)
// /*AFLA*/ {
// /*AFLA*/ 	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;
// /*AFLA*/ 
// /*AFLA*/ 	return ATOMIC_LONG_PFX(_sub_and_test)(i, v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int atomic_long_dec_and_test(atomic_long_t *l)
// /*AFLA*/ {
// /*AFLA*/ 	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;
// /*AFLA*/ 
// /*AFLA*/ 	return ATOMIC_LONG_PFX(_dec_and_test)(v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int atomic_long_inc_and_test(atomic_long_t *l)
// /*AFLA*/ {
// /*AFLA*/ 	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;
// /*AFLA*/ 
// /*AFLA*/ 	return ATOMIC_LONG_PFX(_inc_and_test)(v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int atomic_long_add_negative(long i, atomic_long_t *l)
// /*AFLA*/ {
// /*AFLA*/ 	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;
// /*AFLA*/ 
// /*AFLA*/ 	return ATOMIC_LONG_PFX(_add_negative)(i, v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC_LONG_INC_DEC_OP(op, mo)					\
// /*AFLA*/ static inline long							\
// /*AFLA*/ atomic_long_##op##_return##mo(atomic_long_t *l)				\
// /*AFLA*/ {									\
// /*AFLA*/ 	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;		\
// /*AFLA*/ 									\
// /*AFLA*/ 	return (long)ATOMIC_LONG_PFX(_##op##_return##mo)(v);		\
// /*AFLA*/ }
// /*AFLA*/ ATOMIC_LONG_INC_DEC_OP(inc,)
// /*AFLA*/ ATOMIC_LONG_INC_DEC_OP(inc, _relaxed)
// /*AFLA*/ ATOMIC_LONG_INC_DEC_OP(inc, _acquire)
// /*AFLA*/ ATOMIC_LONG_INC_DEC_OP(inc, _release)
// /*AFLA*/ ATOMIC_LONG_INC_DEC_OP(dec,)
// /*AFLA*/ ATOMIC_LONG_INC_DEC_OP(dec, _relaxed)
// /*AFLA*/ ATOMIC_LONG_INC_DEC_OP(dec, _acquire)
// /*AFLA*/ ATOMIC_LONG_INC_DEC_OP(dec, _release)
// /*AFLA*/ 
// /*AFLA*/ #undef ATOMIC_LONG_INC_DEC_OP
// /*AFLA*/ 
// /*AFLA*/ static inline long atomic_long_add_unless(atomic_long_t *l, long a, long u)
// /*AFLA*/ {
// /*AFLA*/ 	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;
// /*AFLA*/ 
// /*AFLA*/ 	return (long)ATOMIC_LONG_PFX(_add_unless)(v, a, u);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define atomic_long_inc_not_zero(l) \
// /*AFLA*/ 	ATOMIC_LONG_PFX(_inc_not_zero)((ATOMIC_LONG_PFX(_t) *)(l))
// /*AFLA*/ 
// /*AFLA*/ #endif  /*  _ASM_GENERIC_ATOMIC_LONG_H  */
