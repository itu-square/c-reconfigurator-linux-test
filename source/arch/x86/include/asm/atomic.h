// /*AFLA*/ #ifndef _ASM_X86_ATOMIC_H
// /*AFLA*/ #define _ASM_X86_ATOMIC_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <asm/alternative.h>
// /*AFLA*/ #include <asm/cmpxchg.h>
// /*AFLA*/ #include <asm/rmwcc.h>
// /*AFLA*/ #include <asm/barrier.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Atomic operations that C can't guarantee us.  Useful for
// /*AFLA*/  * resource counting etc..
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC_INIT(i)	{ (i) }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic_read - read atomic variable
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically reads the value of @v.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline int atomic_read(const atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return READ_ONCE((v)->counter);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic_set - set atomic variable
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  * @i: required value
// /*AFLA*/  *
// /*AFLA*/  * Atomically sets the value of @v to @i.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void atomic_set(atomic_t *v, int i)
// /*AFLA*/ {
// /*AFLA*/ 	WRITE_ONCE(v->counter, i);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic_add - add integer to atomic variable
// /*AFLA*/  * @i: integer value to add
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically adds @i to @v.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void atomic_add(int i, atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile(LOCK_PREFIX "addl %1,%0"
// /*AFLA*/ 		     : "+m" (v->counter)
// /*AFLA*/ 		     : "ir" (i));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic_sub - subtract integer from atomic variable
// /*AFLA*/  * @i: integer value to subtract
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically subtracts @i from @v.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void atomic_sub(int i, atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile(LOCK_PREFIX "subl %1,%0"
// /*AFLA*/ 		     : "+m" (v->counter)
// /*AFLA*/ 		     : "ir" (i));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic_sub_and_test - subtract value from variable and test result
// /*AFLA*/  * @i: integer value to subtract
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically subtracts @i from @v and returns
// /*AFLA*/  * true if the result is zero, or false for all
// /*AFLA*/  * other cases.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline bool atomic_sub_and_test(int i, atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	GEN_BINARY_RMWcc(LOCK_PREFIX "subl", v->counter, "er", i, "%0", e);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic_inc - increment atomic variable
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically increments @v by 1.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void atomic_inc(atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile(LOCK_PREFIX "incl %0"
// /*AFLA*/ 		     : "+m" (v->counter));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic_dec - decrement atomic variable
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically decrements @v by 1.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void atomic_dec(atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile(LOCK_PREFIX "decl %0"
// /*AFLA*/ 		     : "+m" (v->counter));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic_dec_and_test - decrement and test
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically decrements @v by 1 and
// /*AFLA*/  * returns true if the result is 0, or false for all other
// /*AFLA*/  * cases.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline bool atomic_dec_and_test(atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	GEN_UNARY_RMWcc(LOCK_PREFIX "decl", v->counter, "%0", e);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic_inc_and_test - increment and test
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically increments @v by 1
// /*AFLA*/  * and returns true if the result is zero, or false for all
// /*AFLA*/  * other cases.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline bool atomic_inc_and_test(atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	GEN_UNARY_RMWcc(LOCK_PREFIX "incl", v->counter, "%0", e);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic_add_negative - add and test if negative
// /*AFLA*/  * @i: integer value to add
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically adds @i to @v and returns true
// /*AFLA*/  * if the result is negative, or false when
// /*AFLA*/  * result is greater than or equal to zero.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline bool atomic_add_negative(int i, atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	GEN_BINARY_RMWcc(LOCK_PREFIX "addl", v->counter, "er", i, "%0", s);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic_add_return - add integer and return
// /*AFLA*/  * @i: integer value to add
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically adds @i to @v and returns @i + @v
// /*AFLA*/  */
// /*AFLA*/ static __always_inline int atomic_add_return(int i, atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return i + xadd(&v->counter, i);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic_sub_return - subtract integer and return
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  * @i: integer value to subtract
// /*AFLA*/  *
// /*AFLA*/  * Atomically subtracts @i from @v and returns @v - @i
// /*AFLA*/  */
// /*AFLA*/ static __always_inline int atomic_sub_return(int i, atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_add_return(-i, v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define atomic_inc_return(v)  (atomic_add_return(1, v))
// /*AFLA*/ #define atomic_dec_return(v)  (atomic_sub_return(1, v))
// /*AFLA*/ 
// /*AFLA*/ static __always_inline int atomic_fetch_add(int i, atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return xadd(&v->counter, i);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline int atomic_fetch_sub(int i, atomic_t *v)
// /*AFLA*/ {
// /*AFLA*/ 	return xadd(&v->counter, -i);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline int atomic_cmpxchg(atomic_t *v, int old, int new)
// /*AFLA*/ {
// /*AFLA*/ 	return cmpxchg(&v->counter, old, new);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int atomic_xchg(atomic_t *v, int new)
// /*AFLA*/ {
// /*AFLA*/ 	return xchg(&v->counter, new);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC_OP(op)							\
// /*AFLA*/ static inline void atomic_##op(int i, atomic_t *v)			\
// /*AFLA*/ {									\
// /*AFLA*/ 	asm volatile(LOCK_PREFIX #op"l %1,%0"				\
// /*AFLA*/ 			: "+m" (v->counter)				\
// /*AFLA*/ 			: "ir" (i)					\
// /*AFLA*/ 			: "memory");					\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC_FETCH_OP(op, c_op)					\
// /*AFLA*/ static inline int atomic_fetch_##op(int i, atomic_t *v)		\
// /*AFLA*/ {									\
// /*AFLA*/ 	int old, val = atomic_read(v);					\
// /*AFLA*/ 	for (;;) {							\
// /*AFLA*/ 		old = atomic_cmpxchg(v, val, val c_op i);		\
// /*AFLA*/ 		if (old == val)						\
// /*AFLA*/ 			break;						\
// /*AFLA*/ 		val = old;						\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	return old;							\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC_OPS(op, c_op)						\
// /*AFLA*/ 	ATOMIC_OP(op)							\
// /*AFLA*/ 	ATOMIC_FETCH_OP(op, c_op)
// /*AFLA*/ 
// /*AFLA*/ ATOMIC_OPS(and, &)
// /*AFLA*/ ATOMIC_OPS(or , |)
// /*AFLA*/ ATOMIC_OPS(xor, ^)
// /*AFLA*/ 
// /*AFLA*/ #undef ATOMIC_OPS
// /*AFLA*/ #undef ATOMIC_FETCH_OP
// /*AFLA*/ #undef ATOMIC_OP
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __atomic_add_unless - add unless the number is already a given value
// /*AFLA*/  * @v: pointer of type atomic_t
// /*AFLA*/  * @a: the amount to add to v...
// /*AFLA*/  * @u: ...unless v is equal to u.
// /*AFLA*/  *
// /*AFLA*/  * Atomically adds @a to @v, so long as @v was not already @u.
// /*AFLA*/  * Returns the old value of @v.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline int __atomic_add_unless(atomic_t *v, int a, int u)
// /*AFLA*/ {
// /*AFLA*/ 	int c, old;
// /*AFLA*/ 	c = atomic_read(v);
// /*AFLA*/ 	for (;;) {
// /*AFLA*/ 		if (unlikely(c == (u)))
// /*AFLA*/ 			break;
// /*AFLA*/ 		old = atomic_cmpxchg((v), c, c + (a));
// /*AFLA*/ 		if (likely(old == c))
// /*AFLA*/ 			break;
// /*AFLA*/ 		c = old;
// /*AFLA*/ 	}
// /*AFLA*/ 	return c;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * atomic_inc_short - increment of a short integer
// /*AFLA*/  * @v: pointer to type int
// /*AFLA*/  *
// /*AFLA*/  * Atomically adds 1 to @v
// /*AFLA*/  * Returns the new value of @u
// /*AFLA*/  */
// /*AFLA*/ static __always_inline short int atomic_inc_short(short int *v)
// /*AFLA*/ {
// /*AFLA*/ 	asm(LOCK_PREFIX "addw $1, %0" : "+m" (*v));
// /*AFLA*/ 	return *v;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ # include <asm/atomic64_32.h>
// /*AFLA*/ #else
// /*AFLA*/ # include <asm/atomic64_64.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_ATOMIC_H */
