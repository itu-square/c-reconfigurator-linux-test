// /*AFLA*/ #ifndef _ASM_GENERIC_LOCAL64_H
// /*AFLA*/ #define _ASM_GENERIC_LOCAL64_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/percpu.h>
// /*AFLA*/ #include <asm/types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A signed long type for operations which are atomic for a single CPU.
// /*AFLA*/  * Usually used in combination with per-cpu variables.
// /*AFLA*/  *
// /*AFLA*/  * This is the default implementation, which uses atomic64_t.  Which is
// /*AFLA*/  * rather pointless.  The whole point behind local64_t is that some processors
// /*AFLA*/  * can perform atomic adds and subtracts in a manner which is atomic wrt IRQs
// /*AFLA*/  * running on this CPU.  local64_t allows exploitation of such capabilities.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* Implement in terms of atomics. */
// /*AFLA*/ 
// /*AFLA*/ #if BITS_PER_LONG == 64
// /*AFLA*/ 
// /*AFLA*/ #include <asm/local.h>
// /*AFLA*/ 
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	local_t a;
// /*AFLA*/ } local64_t;
// /*AFLA*/ 
// /*AFLA*/ #define LOCAL64_INIT(i)	{ LOCAL_INIT(i) }
// /*AFLA*/ 
// /*AFLA*/ #define local64_read(l)		local_read(&(l)->a)
// /*AFLA*/ #define local64_set(l,i)	local_set((&(l)->a),(i))
// /*AFLA*/ #define local64_inc(l)		local_inc(&(l)->a)
// /*AFLA*/ #define local64_dec(l)		local_dec(&(l)->a)
// /*AFLA*/ #define local64_add(i,l)	local_add((i),(&(l)->a))
// /*AFLA*/ #define local64_sub(i,l)	local_sub((i),(&(l)->a))
// /*AFLA*/ 
// /*AFLA*/ #define local64_sub_and_test(i, l) local_sub_and_test((i), (&(l)->a))
// /*AFLA*/ #define local64_dec_and_test(l) local_dec_and_test(&(l)->a)
// /*AFLA*/ #define local64_inc_and_test(l) local_inc_and_test(&(l)->a)
// /*AFLA*/ #define local64_add_negative(i, l) local_add_negative((i), (&(l)->a))
// /*AFLA*/ #define local64_add_return(i, l) local_add_return((i), (&(l)->a))
// /*AFLA*/ #define local64_sub_return(i, l) local_sub_return((i), (&(l)->a))
// /*AFLA*/ #define local64_inc_return(l)	local_inc_return(&(l)->a)
// /*AFLA*/ 
// /*AFLA*/ #define local64_cmpxchg(l, o, n) local_cmpxchg((&(l)->a), (o), (n))
// /*AFLA*/ #define local64_xchg(l, n)	local_xchg((&(l)->a), (n))
// /*AFLA*/ #define local64_add_unless(l, _a, u) local_add_unless((&(l)->a), (_a), (u))
// /*AFLA*/ #define local64_inc_not_zero(l)	local_inc_not_zero(&(l)->a)
// /*AFLA*/ 
// /*AFLA*/ /* Non-atomic variants, ie. preemption disabled and won't be touched
// /*AFLA*/  * in interrupt, etc.  Some archs can optimize this case well. */
// /*AFLA*/ #define __local64_inc(l)	local64_set((l), local64_read(l) + 1)
// /*AFLA*/ #define __local64_dec(l)	local64_set((l), local64_read(l) - 1)
// /*AFLA*/ #define __local64_add(i,l)	local64_set((l), local64_read(l) + (i))
// /*AFLA*/ #define __local64_sub(i,l)	local64_set((l), local64_read(l) - (i))
// /*AFLA*/ 
// /*AFLA*/ #else /* BITS_PER_LONG != 64 */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ 
// /*AFLA*/ /* Don't use typedef: don't want them to be mixed with atomic_t's. */
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	atomic64_t a;
// /*AFLA*/ } local64_t;
// /*AFLA*/ 
// /*AFLA*/ #define LOCAL64_INIT(i)	{ ATOMIC_LONG_INIT(i) }
// /*AFLA*/ 
// /*AFLA*/ #define local64_read(l)		atomic64_read(&(l)->a)
// /*AFLA*/ #define local64_set(l,i)	atomic64_set((&(l)->a),(i))
// /*AFLA*/ #define local64_inc(l)		atomic64_inc(&(l)->a)
// /*AFLA*/ #define local64_dec(l)		atomic64_dec(&(l)->a)
// /*AFLA*/ #define local64_add(i,l)	atomic64_add((i),(&(l)->a))
// /*AFLA*/ #define local64_sub(i,l)	atomic64_sub((i),(&(l)->a))
// /*AFLA*/ 
// /*AFLA*/ #define local64_sub_and_test(i, l) atomic64_sub_and_test((i), (&(l)->a))
// /*AFLA*/ #define local64_dec_and_test(l) atomic64_dec_and_test(&(l)->a)
// /*AFLA*/ #define local64_inc_and_test(l) atomic64_inc_and_test(&(l)->a)
// /*AFLA*/ #define local64_add_negative(i, l) atomic64_add_negative((i), (&(l)->a))
// /*AFLA*/ #define local64_add_return(i, l) atomic64_add_return((i), (&(l)->a))
// /*AFLA*/ #define local64_sub_return(i, l) atomic64_sub_return((i), (&(l)->a))
// /*AFLA*/ #define local64_inc_return(l)	atomic64_inc_return(&(l)->a)
// /*AFLA*/ 
// /*AFLA*/ #define local64_cmpxchg(l, o, n) atomic64_cmpxchg((&(l)->a), (o), (n))
// /*AFLA*/ #define local64_xchg(l, n)	atomic64_xchg((&(l)->a), (n))
// /*AFLA*/ #define local64_add_unless(l, _a, u) atomic64_add_unless((&(l)->a), (_a), (u))
// /*AFLA*/ #define local64_inc_not_zero(l)	atomic64_inc_not_zero(&(l)->a)
// /*AFLA*/ 
// /*AFLA*/ /* Non-atomic variants, ie. preemption disabled and won't be touched
// /*AFLA*/  * in interrupt, etc.  Some archs can optimize this case well. */
// /*AFLA*/ #define __local64_inc(l)	local64_set((l), local64_read(l) + 1)
// /*AFLA*/ #define __local64_dec(l)	local64_set((l), local64_read(l) - 1)
// /*AFLA*/ #define __local64_add(i,l)	local64_set((l), local64_read(l) + (i))
// /*AFLA*/ #define __local64_sub(i,l)	local64_set((l), local64_read(l) - (i))
// /*AFLA*/ 
// /*AFLA*/ #endif /* BITS_PER_LONG != 64 */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_GENERIC_LOCAL64_H */
