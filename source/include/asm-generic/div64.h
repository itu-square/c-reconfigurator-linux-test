// /*AFLA*/ #ifndef _ASM_GENERIC_DIV64_H
// /*AFLA*/ #define _ASM_GENERIC_DIV64_H
// /*AFLA*/ /*
// /*AFLA*/  * Copyright (C) 2003 Bernardo Innocenti <bernie@develer.com>
// /*AFLA*/  * Based on former asm-ppc/div64.h and asm-m68knommu/div64.h
// /*AFLA*/  *
// /*AFLA*/  * Optimization for constant divisors on 32-bit machines:
// /*AFLA*/  * Copyright (C) 2006-2015 Nicolas Pitre
// /*AFLA*/  *
// /*AFLA*/  * The semantics of do_div() are:
// /*AFLA*/  *
// /*AFLA*/  * uint32_t do_div(uint64_t *n, uint32_t base)
// /*AFLA*/  * {
// /*AFLA*/  * 	uint32_t remainder = *n % base;
// /*AFLA*/  * 	*n = *n / base;
// /*AFLA*/  * 	return remainder;
// /*AFLA*/  * }
// /*AFLA*/  *
// /*AFLA*/  * NOTE: macro parameter n is evaluated multiple times,
// /*AFLA*/  *       beware of side effects!
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ 
// /*AFLA*/ #if BITS_PER_LONG == 64
// /*AFLA*/ 
// /*AFLA*/ # define do_div(n,base) ({					\
// /*AFLA*/ 	uint32_t __base = (base);				\
// /*AFLA*/ 	uint32_t __rem;						\
// /*AFLA*/ 	__rem = ((uint64_t)(n)) % __base;			\
// /*AFLA*/ 	(n) = ((uint64_t)(n)) / __base;				\
// /*AFLA*/ 	__rem;							\
// /*AFLA*/  })
// /*AFLA*/ 
// /*AFLA*/ #elif BITS_PER_LONG == 32
// /*AFLA*/ 
// /*AFLA*/ #include <linux/log2.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * If the divisor happens to be constant, we determine the appropriate
// /*AFLA*/  * inverse at compile time to turn the division into a few inline
// /*AFLA*/  * multiplications which ought to be much faster. And yet only if compiling
// /*AFLA*/  * with a sufficiently recent gcc version to perform proper 64-bit constant
// /*AFLA*/  * propagation.
// /*AFLA*/  *
// /*AFLA*/  * (It is unfortunate that gcc doesn't perform all this internally.)
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __div64_const32_is_OK
// /*AFLA*/ #define __div64_const32_is_OK (__GNUC__ >= 4)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __div64_const32(n, ___b)					\
// /*AFLA*/ ({									\
// /*AFLA*/ 	/*								\
// /*AFLA*/ 	 * Multiplication by reciprocal of b: n / b = n * (p / b) / p	\
// /*AFLA*/ 	 *								\
// /*AFLA*/ 	 * We rely on the fact that most of this code gets optimized	\
// /*AFLA*/ 	 * away at compile time due to constant propagation and only	\
// /*AFLA*/ 	 * a few multiplication instructions should remain.		\
// /*AFLA*/ 	 * Hence this monstrous macro (static inline doesn't always	\
// /*AFLA*/ 	 * do the trick here).						\
// /*AFLA*/ 	 */								\
// /*AFLA*/ 	uint64_t ___res, ___x, ___t, ___m, ___n = (n);			\
// /*AFLA*/ 	uint32_t ___p, ___bias;						\
// /*AFLA*/ 									\
// /*AFLA*/ 	/* determine MSB of b */					\
// /*AFLA*/ 	___p = 1 << ilog2(___b);					\
// /*AFLA*/ 									\
// /*AFLA*/ 	/* compute m = ((p << 64) + b - 1) / b */			\
// /*AFLA*/ 	___m = (~0ULL / ___b) * ___p;					\
// /*AFLA*/ 	___m += (((~0ULL % ___b + 1) * ___p) + ___b - 1) / ___b;	\
// /*AFLA*/ 									\
// /*AFLA*/ 	/* one less than the dividend with highest result */		\
// /*AFLA*/ 	___x = ~0ULL / ___b * ___b - 1;					\
// /*AFLA*/ 									\
// /*AFLA*/ 	/* test our ___m with res = m * x / (p << 64) */		\
// /*AFLA*/ 	___res = ((___m & 0xffffffff) * (___x & 0xffffffff)) >> 32;	\
// /*AFLA*/ 	___t = ___res += (___m & 0xffffffff) * (___x >> 32);		\
// /*AFLA*/ 	___res += (___x & 0xffffffff) * (___m >> 32);			\
// /*AFLA*/ 	___t = (___res < ___t) ? (1ULL << 32) : 0;			\
// /*AFLA*/ 	___res = (___res >> 32) + ___t;					\
// /*AFLA*/ 	___res += (___m >> 32) * (___x >> 32);				\
// /*AFLA*/ 	___res /= ___p;							\
// /*AFLA*/ 									\
// /*AFLA*/ 	/* Now sanitize and optimize what we've got. */			\
// /*AFLA*/ 	if (~0ULL % (___b / (___b & -___b)) == 0) {			\
// /*AFLA*/ 		/* special case, can be simplified to ... */		\
// /*AFLA*/ 		___n /= (___b & -___b);					\
// /*AFLA*/ 		___m = ~0ULL / (___b / (___b & -___b));			\
// /*AFLA*/ 		___p = 1;						\
// /*AFLA*/ 		___bias = 1;						\
// /*AFLA*/ 	} else if (___res != ___x / ___b) {				\
// /*AFLA*/ 		/*							\
// /*AFLA*/ 		 * We can't get away without a bias to compensate	\
// /*AFLA*/ 		 * for bit truncation errors.  To avoid it we'd need an	\
// /*AFLA*/ 		 * additional bit to represent m which would overflow	\
// /*AFLA*/ 		 * a 64-bit variable.					\
// /*AFLA*/ 		 *							\
// /*AFLA*/ 		 * Instead we do m = p / b and n / b = (n * m + m) / p.	\
// /*AFLA*/ 		 */							\
// /*AFLA*/ 		___bias = 1;						\
// /*AFLA*/ 		/* Compute m = (p << 64) / b */				\
// /*AFLA*/ 		___m = (~0ULL / ___b) * ___p;				\
// /*AFLA*/ 		___m += ((~0ULL % ___b + 1) * ___p) / ___b;		\
// /*AFLA*/ 	} else {							\
// /*AFLA*/ 		/*							\
// /*AFLA*/ 		 * Reduce m / p, and try to clear bit 31 of m when	\
// /*AFLA*/ 		 * possible, otherwise that'll need extra overflow	\
// /*AFLA*/ 		 * handling later.					\
// /*AFLA*/ 		 */							\
// /*AFLA*/ 		uint32_t ___bits = -(___m & -___m);			\
// /*AFLA*/ 		___bits |= ___m >> 32;					\
// /*AFLA*/ 		___bits = (~___bits) << 1;				\
// /*AFLA*/ 		/*							\
// /*AFLA*/ 		 * If ___bits == 0 then setting bit 31 is  unavoidable.	\
// /*AFLA*/ 		 * Simply apply the maximum possible reduction in that	\
// /*AFLA*/ 		 * case. Otherwise the MSB of ___bits indicates the	\
// /*AFLA*/ 		 * best reduction we should apply.			\
// /*AFLA*/ 		 */							\
// /*AFLA*/ 		if (!___bits) {						\
// /*AFLA*/ 			___p /= (___m & -___m);				\
// /*AFLA*/ 			___m /= (___m & -___m);				\
// /*AFLA*/ 		} else {						\
// /*AFLA*/ 			___p >>= ilog2(___bits);			\
// /*AFLA*/ 			___m >>= ilog2(___bits);			\
// /*AFLA*/ 		}							\
// /*AFLA*/ 		/* No bias needed. */					\
// /*AFLA*/ 		___bias = 0;						\
// /*AFLA*/ 	}								\
// /*AFLA*/ 									\
// /*AFLA*/ 	/*								\
// /*AFLA*/ 	 * Now we have a combination of 2 conditions:			\
// /*AFLA*/ 	 *								\
// /*AFLA*/ 	 * 1) whether or not we need to apply a bias, and		\
// /*AFLA*/ 	 *								\
// /*AFLA*/ 	 * 2) whether or not there might be an overflow in the cross	\
// /*AFLA*/ 	 *    product determined by (___m & ((1 << 63) | (1 << 31))).	\
// /*AFLA*/ 	 *								\
// /*AFLA*/ 	 * Select the best way to do (m_bias + m * n) / (1 << 64).	\
// /*AFLA*/ 	 * From now on there will be actual runtime code generated.	\
// /*AFLA*/ 	 */								\
// /*AFLA*/ 	___res = __arch_xprod_64(___m, ___n, ___bias);			\
// /*AFLA*/ 									\
// /*AFLA*/ 	___res /= ___p;							\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #ifndef __arch_xprod_64
// /*AFLA*/ /*
// /*AFLA*/  * Default C implementation for __arch_xprod_64()
// /*AFLA*/  *
// /*AFLA*/  * Prototype: uint64_t __arch_xprod_64(const uint64_t m, uint64_t n, bool bias)
// /*AFLA*/  * Semantic:  retval = ((bias ? m : 0) + m * n) >> 64
// /*AFLA*/  *
// /*AFLA*/  * The product is a 128-bit value, scaled down to 64 bits.
// /*AFLA*/  * Assuming constant propagation to optimize away unused conditional code.
// /*AFLA*/  * Architectures may provide their own optimized assembly implementation.
// /*AFLA*/  */
// /*AFLA*/ static inline uint64_t __arch_xprod_64(const uint64_t m, uint64_t n, bool bias)
// /*AFLA*/ {
// /*AFLA*/ 	uint32_t m_lo = m;
// /*AFLA*/ 	uint32_t m_hi = m >> 32;
// /*AFLA*/ 	uint32_t n_lo = n;
// /*AFLA*/ 	uint32_t n_hi = n >> 32;
// /*AFLA*/ 	uint64_t res, tmp;
// /*AFLA*/ 
// /*AFLA*/ 	if (!bias) {
// /*AFLA*/ 		res = ((uint64_t)m_lo * n_lo) >> 32;
// /*AFLA*/ 	} else if (!(m & ((1ULL << 63) | (1ULL << 31)))) {
// /*AFLA*/ 		/* there can't be any overflow here */
// /*AFLA*/ 		res = (m + (uint64_t)m_lo * n_lo) >> 32;
// /*AFLA*/ 	} else {
// /*AFLA*/ 		res = m + (uint64_t)m_lo * n_lo;
// /*AFLA*/ 		tmp = (res < m) ? (1ULL << 32) : 0;
// /*AFLA*/ 		res = (res >> 32) + tmp;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	if (!(m & ((1ULL << 63) | (1ULL << 31)))) {
// /*AFLA*/ 		/* there can't be any overflow here */
// /*AFLA*/ 		res += (uint64_t)m_lo * n_hi;
// /*AFLA*/ 		res += (uint64_t)m_hi * n_lo;
// /*AFLA*/ 		res >>= 32;
// /*AFLA*/ 	} else {
// /*AFLA*/ 		tmp = res += (uint64_t)m_lo * n_hi;
// /*AFLA*/ 		res += (uint64_t)m_hi * n_lo;
// /*AFLA*/ 		tmp = (res < tmp) ? (1ULL << 32) : 0;
// /*AFLA*/ 		res = (res >> 32) + tmp;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	res += (uint64_t)m_hi * n_hi;
// /*AFLA*/ 
// /*AFLA*/ 	return res;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __div64_32
// /*AFLA*/ extern uint32_t __div64_32(uint64_t *dividend, uint32_t divisor);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* The unnecessary pointer compare is there
// /*AFLA*/  * to check for type safety (n must be 64bit)
// /*AFLA*/  */
// /*AFLA*/ # define do_div(n,base) ({				\
// /*AFLA*/ 	uint32_t __base = (base);			\
// /*AFLA*/ 	uint32_t __rem;					\
// /*AFLA*/ 	(void)(((typeof((n)) *)0) == ((uint64_t *)0));	\
// /*AFLA*/ 	if (__builtin_constant_p(__base) &&		\
// /*AFLA*/ 	    is_power_of_2(__base)) {			\
// /*AFLA*/ 		__rem = (n) & (__base - 1);		\
// /*AFLA*/ 		(n) >>= ilog2(__base);			\
// /*AFLA*/ 	} else if (__div64_const32_is_OK &&		\
// /*AFLA*/ 		   __builtin_constant_p(__base) &&	\
// /*AFLA*/ 		   __base != 0) {			\
// /*AFLA*/ 		uint32_t __res_lo, __n_lo = (n);	\
// /*AFLA*/ 		(n) = __div64_const32(n, __base);	\
// /*AFLA*/ 		/* the remainder can be computed with 32-bit regs */ \
// /*AFLA*/ 		__res_lo = (n);				\
// /*AFLA*/ 		__rem = __n_lo - __res_lo * __base;	\
// /*AFLA*/ 	} else if (likely(((n) >> 32) == 0)) {		\
// /*AFLA*/ 		__rem = (uint32_t)(n) % __base;		\
// /*AFLA*/ 		(n) = (uint32_t)(n) / __base;		\
// /*AFLA*/ 	} else 						\
// /*AFLA*/ 		__rem = __div64_32(&(n), __base);	\
// /*AFLA*/ 	__rem;						\
// /*AFLA*/  })
// /*AFLA*/ 
// /*AFLA*/ #else /* BITS_PER_LONG == ?? */
// /*AFLA*/ 
// /*AFLA*/ # error do_div() does not yet support the C64
// /*AFLA*/ 
// /*AFLA*/ #endif /* BITS_PER_LONG */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_GENERIC_DIV64_H */
