// /*AFLA*/ #ifndef _ASM_X86_LOCAL_H
// /*AFLA*/ #define _ASM_X86_LOCAL_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/percpu.h>
// /*AFLA*/ 
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <asm/asm.h>
// /*AFLA*/ 
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	atomic_long_t a;
// /*AFLA*/ } local_t;
// /*AFLA*/ 
// /*AFLA*/ #define LOCAL_INIT(i)	{ ATOMIC_LONG_INIT(i) }
// /*AFLA*/ 
// /*AFLA*/ #define local_read(l)	atomic_long_read(&(l)->a)
// /*AFLA*/ #define local_set(l, i)	atomic_long_set(&(l)->a, (i))
// /*AFLA*/ 
// /*AFLA*/ static inline void local_inc(local_t *l)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile(_ASM_INC "%0"
// /*AFLA*/ 		     : "+m" (l->a.counter));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void local_dec(local_t *l)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile(_ASM_DEC "%0"
// /*AFLA*/ 		     : "+m" (l->a.counter));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void local_add(long i, local_t *l)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile(_ASM_ADD "%1,%0"
// /*AFLA*/ 		     : "+m" (l->a.counter)
// /*AFLA*/ 		     : "ir" (i));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void local_sub(long i, local_t *l)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile(_ASM_SUB "%1,%0"
// /*AFLA*/ 		     : "+m" (l->a.counter)
// /*AFLA*/ 		     : "ir" (i));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * local_sub_and_test - subtract value from variable and test result
// /*AFLA*/  * @i: integer value to subtract
// /*AFLA*/  * @l: pointer to type local_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically subtracts @i from @l and returns
// /*AFLA*/  * true if the result is zero, or false for all
// /*AFLA*/  * other cases.
// /*AFLA*/  */
// /*AFLA*/ static inline bool local_sub_and_test(long i, local_t *l)
// /*AFLA*/ {
// /*AFLA*/ 	GEN_BINARY_RMWcc(_ASM_SUB, l->a.counter, "er", i, "%0", e);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * local_dec_and_test - decrement and test
// /*AFLA*/  * @l: pointer to type local_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically decrements @l by 1 and
// /*AFLA*/  * returns true if the result is 0, or false for all other
// /*AFLA*/  * cases.
// /*AFLA*/  */
// /*AFLA*/ static inline bool local_dec_and_test(local_t *l)
// /*AFLA*/ {
// /*AFLA*/ 	GEN_UNARY_RMWcc(_ASM_DEC, l->a.counter, "%0", e);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * local_inc_and_test - increment and test
// /*AFLA*/  * @l: pointer to type local_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically increments @l by 1
// /*AFLA*/  * and returns true if the result is zero, or false for all
// /*AFLA*/  * other cases.
// /*AFLA*/  */
// /*AFLA*/ static inline bool local_inc_and_test(local_t *l)
// /*AFLA*/ {
// /*AFLA*/ 	GEN_UNARY_RMWcc(_ASM_INC, l->a.counter, "%0", e);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * local_add_negative - add and test if negative
// /*AFLA*/  * @i: integer value to add
// /*AFLA*/  * @l: pointer to type local_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically adds @i to @l and returns true
// /*AFLA*/  * if the result is negative, or false when
// /*AFLA*/  * result is greater than or equal to zero.
// /*AFLA*/  */
// /*AFLA*/ static inline bool local_add_negative(long i, local_t *l)
// /*AFLA*/ {
// /*AFLA*/ 	GEN_BINARY_RMWcc(_ASM_ADD, l->a.counter, "er", i, "%0", s);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * local_add_return - add and return
// /*AFLA*/  * @i: integer value to add
// /*AFLA*/  * @l: pointer to type local_t
// /*AFLA*/  *
// /*AFLA*/  * Atomically adds @i to @l and returns @i + @l
// /*AFLA*/  */
// /*AFLA*/ static inline long local_add_return(long i, local_t *l)
// /*AFLA*/ {
// /*AFLA*/ 	long __i = i;
// /*AFLA*/ 	asm volatile(_ASM_XADD "%0, %1;"
// /*AFLA*/ 		     : "+r" (i), "+m" (l->a.counter)
// /*AFLA*/ 		     : : "memory");
// /*AFLA*/ 	return i + __i;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline long local_sub_return(long i, local_t *l)
// /*AFLA*/ {
// /*AFLA*/ 	return local_add_return(-i, l);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define local_inc_return(l)  (local_add_return(1, l))
// /*AFLA*/ #define local_dec_return(l)  (local_sub_return(1, l))
// /*AFLA*/ 
// /*AFLA*/ #define local_cmpxchg(l, o, n) \
// /*AFLA*/ 	(cmpxchg_local(&((l)->a.counter), (o), (n)))
// /*AFLA*/ /* Always has a lock prefix */
// /*AFLA*/ #define local_xchg(l, n) (xchg(&((l)->a.counter), (n)))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * local_add_unless - add unless the number is a given value
// /*AFLA*/  * @l: pointer of type local_t
// /*AFLA*/  * @a: the amount to add to l...
// /*AFLA*/  * @u: ...unless l is equal to u.
// /*AFLA*/  *
// /*AFLA*/  * Atomically adds @a to @l, so long as it was not @u.
// /*AFLA*/  * Returns non-zero if @l was not @u, and zero otherwise.
// /*AFLA*/  */
// /*AFLA*/ #define local_add_unless(l, a, u)				\
// /*AFLA*/ ({								\
// /*AFLA*/ 	long c, old;						\
// /*AFLA*/ 	c = local_read((l));					\
// /*AFLA*/ 	for (;;) {						\
// /*AFLA*/ 		if (unlikely(c == (u)))				\
// /*AFLA*/ 			break;					\
// /*AFLA*/ 		old = local_cmpxchg((l), c, c + (a));		\
// /*AFLA*/ 		if (likely(old == c))				\
// /*AFLA*/ 			break;					\
// /*AFLA*/ 		c = old;					\
// /*AFLA*/ 	}							\
// /*AFLA*/ 	c != (u);						\
// /*AFLA*/ })
// /*AFLA*/ #define local_inc_not_zero(l) local_add_unless((l), 1, 0)
// /*AFLA*/ 
// /*AFLA*/ /* On x86_32, these are no better than the atomic variants.
// /*AFLA*/  * On x86-64 these are better than the atomic variants on SMP kernels
// /*AFLA*/  * because they dont use a lock prefix.
// /*AFLA*/  */
// /*AFLA*/ #define __local_inc(l)		local_inc(l)
// /*AFLA*/ #define __local_dec(l)		local_dec(l)
// /*AFLA*/ #define __local_add(i, l)	local_add((i), (l))
// /*AFLA*/ #define __local_sub(i, l)	local_sub((i), (l))
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_LOCAL_H */
