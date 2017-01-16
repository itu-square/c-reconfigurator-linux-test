// /*AFLA*/ #ifndef _ASM_X86_ATOMIC64_64_H
// /*AFLA*/ #define _ASM_X86_ATOMIC64_64_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <asm/alternative.h>
// /*AFLA*/ #include <asm/cmpxchg.h>
// /*AFLA*/ 
// /*AFLA*/ /* The 64-bit atomic type */
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC64_INIT(i)	{ (i) }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic64_read - read atomic64 variable
// /*AFLA*/  * @v: pointer of type atomic64_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically reads the value of @v.
// /*AFLA*/  * Doesn't imply a read memory barrier.
// /*AFLA*/  */
// /*AFLA*/ static inline long atomic64_read(const atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return READ_ONCE((v)->counter);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic64_set - set atomic64 variable
// /*AFLA*/  * @v: pointer to type atomic64_t
// /*AFLA*/  * @i: required value
// /*AFLA*/  *
// /*AFLA*/  * Atomically sets the value of @v to @i.
// /*AFLA*/  */
// /*AFLA*/ static inline void atomic64_set(atomic64_t *v, long i)
// /*AFLA*/ {
// /*AFLA*/ 	WRITE_ONCE(v->counter, i);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic64_add - add integer to atomic64 variable
// /*AFLA*/  * @i: integer value to add
// /*AFLA*/  * @v: pointer to type atomic64_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically adds @i to @v.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void atomic64_add(long i, atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile(LOCK_PREFIX "addq %1,%0"
// /*AFLA*/ 		     : "=m" (v->counter)
// /*AFLA*/ 		     : "er" (i), "m" (v->counter));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic64_sub - subtract the atomic64 variable
// /*AFLA*/  * @i: integer value to subtract
// /*AFLA*/  * @v: pointer to type atomic64_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically subtracts @i from @v.
// /*AFLA*/  */
// /*AFLA*/ static inline void atomic64_sub(long i, atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile(LOCK_PREFIX "subq %1,%0"
// /*AFLA*/ 		     : "=m" (v->counter)
// /*AFLA*/ 		     : "er" (i), "m" (v->counter));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic64_sub_and_test - subtract value from variable and test result
// /*AFLA*/  * @i: integer value to subtract
// /*AFLA*/  * @v: pointer to type atomic64_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically subtracts @i from @v and returns
// /*AFLA*/  * true if the result is zero, or false for all
// /*AFLA*/  * other cases.
// /*AFLA*/  */
// /*AFLA*/ static inline bool atomic64_sub_and_test(long i, atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	GEN_BINARY_RMWcc(LOCK_PREFIX "subq", v->counter, "er", i, "%0", e);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic64_inc - increment atomic64 variable
// /*AFLA*/  * @v: pointer to type atomic64_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically increments @v by 1.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void atomic64_inc(atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile(LOCK_PREFIX "incq %0"
// /*AFLA*/ 		     : "=m" (v->counter)
// /*AFLA*/ 		     : "m" (v->counter));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic64_dec - decrement atomic64 variable
// /*AFLA*/  * @v: pointer to type atomic64_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically decrements @v by 1.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void atomic64_dec(atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile(LOCK_PREFIX "decq %0"
// /*AFLA*/ 		     : "=m" (v->counter)
// /*AFLA*/ 		     : "m" (v->counter));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic64_dec_and_test - decrement and test
// /*AFLA*/  * @v: pointer to type atomic64_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically decrements @v by 1 and
// /*AFLA*/  * returns true if the result is 0, or false for all other
// /*AFLA*/  * cases.
// /*AFLA*/  */
// /*AFLA*/ static inline bool atomic64_dec_and_test(atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	GEN_UNARY_RMWcc(LOCK_PREFIX "decq", v->counter, "%0", e);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic64_inc_and_test - increment and test
// /*AFLA*/  * @v: pointer to type atomic64_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically increments @v by 1
// /*AFLA*/  * and returns true if the result is zero, or false for all
// /*AFLA*/  * other cases.
// /*AFLA*/  */
// /*AFLA*/ static inline bool atomic64_inc_and_test(atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	GEN_UNARY_RMWcc(LOCK_PREFIX "incq", v->counter, "%0", e);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic64_add_negative - add and test if negative
// /*AFLA*/  * @i: integer value to add
// /*AFLA*/  * @v: pointer to type atomic64_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically adds @i to @v and returns true
// /*AFLA*/  * if the result is negative, or false when
// /*AFLA*/  * result is greater than or equal to zero.
// /*AFLA*/  */
// /*AFLA*/ static inline bool atomic64_add_negative(long i, atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	GEN_BINARY_RMWcc(LOCK_PREFIX "addq", v->counter, "er", i, "%0", s);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic64_add_return - add and return
// /*AFLA*/  * @i: integer value to add
// /*AFLA*/  * @v: pointer to type atomic64_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically adds @i to @v and returns @i + @v
// /*AFLA*/  */
// /*AFLA*/ static __always_inline long atomic64_add_return(long i, atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return i + xadd(&v->counter, i);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline long atomic64_sub_return(long i, atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic64_add_return(-i, v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline long atomic64_fetch_add(long i, atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return xadd(&v->counter, i);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline long atomic64_fetch_sub(long i, atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return xadd(&v->counter, -i);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define atomic64_inc_return(v)  (atomic64_add_return(1, (v)))
// /*AFLA*/ #define atomic64_dec_return(v)  (atomic64_sub_return(1, (v)))
// /*AFLA*/ 
// /*AFLA*/ static inline long atomic64_cmpxchg(atomic64_t *v, long old, long new)
// /*AFLA*/ {
// /*AFLA*/ 	return cmpxchg(&v->counter, old, new);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline long atomic64_xchg(atomic64_t *v, long new)
// /*AFLA*/ {
// /*AFLA*/ 	return xchg(&v->counter, new);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic64_add_unless - add unless the number is a given value
// /*AFLA*/  * @v: pointer of type atomic64_t
// /*AFLA*/  * @a: the amount to add to v...
// /*AFLA*/  * @u: ...unless v is equal to u.
// /*AFLA*/  *
// /*AFLA*/  * Atomically adds @a to @v, so long as it was not @u.
// /*AFLA*/  * Returns the old value of @v.
// /*AFLA*/  */
// /*AFLA*/ static inline bool atomic64_add_unless(atomic64_t *v, long a, long u)
// /*AFLA*/ {
// /*AFLA*/ 	long c, old;
// /*AFLA*/ 	c = atomic64_read(v);
// /*AFLA*/ 	for (;;) {
// /*AFLA*/ 		if (unlikely(c == (u)))
// /*AFLA*/ 			break;
// /*AFLA*/ 		old = atomic64_cmpxchg((v), c, c + (a));
// /*AFLA*/ 		if (likely(old == c))
// /*AFLA*/ 			break;
// /*AFLA*/ 		c = old;
// /*AFLA*/ 	}
// /*AFLA*/ 	return c != (u);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define atomic64_inc_not_zero(v) atomic64_add_unless((v), 1, 0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * atomic64_dec_if_positive - decrement by 1 if old value positive
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  *
// /*AFLA*/  * The function returns the old value of *v minus 1, even if
// /*AFLA*/  * the atomic variable, v, was not decremented.
// /*AFLA*/  */
// /*AFLA*/ static inline long atomic64_dec_if_positive(atomic64_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	long c, old, dec;
// /*AFLA*/ 	c = atomic64_read(v);
// /*AFLA*/ 	for (;;) {
// /*AFLA*/ 		dec = c - 1;
// /*AFLA*/ 		if (unlikely(dec < 0))
// /*AFLA*/ 			break;
// /*AFLA*/ 		old = atomic64_cmpxchg((v), c, dec);
// /*AFLA*/ 		if (likely(old == c))
// /*AFLA*/ 			break;
// /*AFLA*/ 		c = old;
// /*AFLA*/ 	}
// /*AFLA*/ 	return dec;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC64_OP(op)							\
// /*AFLA*/ static inline void atomic64_##op(long i, atomic64_t *v)			\
// /*AFLA*/ {									\
// /*AFLA*/ 	asm volatile(LOCK_PREFIX #op"q %1,%0"				\
// /*AFLA*/ 			: "+m" (v->counter)				\
// /*AFLA*/ 			: "er" (i)					\
// /*AFLA*/ 			: "memory");					\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC64_FETCH_OP(op, c_op)					\
// /*AFLA*/ static inline long atomic64_fetch_##op(long i, atomic64_t *v)		\
// /*AFLA*/ {									\
// /*AFLA*/ 	long old, val = atomic64_read(v);				\
// /*AFLA*/ 	for (;;) {							\
// /*AFLA*/ 		old = atomic64_cmpxchg(v, val, val c_op i);		\
// /*AFLA*/ 		if (old == val)						\
// /*AFLA*/ 			break;						\
// /*AFLA*/ 		val = old;						\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	return old;							\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC64_OPS(op, c_op)						\
// /*AFLA*/ 	ATOMIC64_OP(op)							\
// /*AFLA*/ 	ATOMIC64_FETCH_OP(op, c_op)
// /*AFLA*/ 
// /*AFLA*/ ATOMIC64_OPS(and, &)
// /*AFLA*/ ATOMIC64_OPS(or, |)
// /*AFLA*/ ATOMIC64_OPS(xor, ^)
// /*AFLA*/ 
// /*AFLA*/ #undef ATOMIC64_OPS
// /*AFLA*/ #undef ATOMIC64_FETCH_OP
// /*AFLA*/ #undef ATOMIC64_OP
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_ATOMIC64_64_H */
