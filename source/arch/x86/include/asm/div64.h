// /*AFLA*/ #ifndef _ASM_X86_DIV64_H
// /*AFLA*/ #define _ASM_X86_DIV64_H
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/log2.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * do_div() is NOT a C function. It wants to return
// /*AFLA*/  * two values (the quotient and the remainder), but
// /*AFLA*/  * since that doesn't work very well in C, what it
// /*AFLA*/  * does is:
// /*AFLA*/  *
// /*AFLA*/  * - modifies the 64-bit dividend _in_place_
// /*AFLA*/  * - returns the 32-bit remainder
// /*AFLA*/  *
// /*AFLA*/  * This ends up being the most efficient "calling
// /*AFLA*/  * convention" on x86.
// /*AFLA*/  */
// /*AFLA*/ #define do_div(n, base)						\
// /*AFLA*/ ({								\
// /*AFLA*/ 	unsigned long __upper, __low, __high, __mod, __base;	\
// /*AFLA*/ 	__base = (base);					\
// /*AFLA*/ 	if (__builtin_constant_p(__base) && is_power_of_2(__base)) { \
// /*AFLA*/ 		__mod = n & (__base - 1);			\
// /*AFLA*/ 		n >>= ilog2(__base);				\
// /*AFLA*/ 	} else {						\
// /*AFLA*/ 		asm("" : "=a" (__low), "=d" (__high) : "A" (n));\
// /*AFLA*/ 		__upper = __high;				\
// /*AFLA*/ 		if (__high) {					\
// /*AFLA*/ 			__upper = __high % (__base);		\
// /*AFLA*/ 			__high = __high / (__base);		\
// /*AFLA*/ 		}						\
// /*AFLA*/ 		asm("divl %2" : "=a" (__low), "=d" (__mod)	\
// /*AFLA*/ 			: "rm" (__base), "0" (__low), "1" (__upper));	\
// /*AFLA*/ 		asm("" : "=A" (n) : "a" (__low), "d" (__high));	\
// /*AFLA*/ 	}							\
// /*AFLA*/ 	__mod;							\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ static inline u64 div_u64_rem(u64 dividend, u32 divisor, u32 *remainder)
// /*AFLA*/ {
// /*AFLA*/ 	union {
// /*AFLA*/ 		u64 v64;
// /*AFLA*/ 		u32 v32[2];
// /*AFLA*/ 	} d = { dividend };
// /*AFLA*/ 	u32 upper;
// /*AFLA*/ 
// /*AFLA*/ 	upper = d.v32[1];
// /*AFLA*/ 	d.v32[1] = 0;
// /*AFLA*/ 	if (upper >= divisor) {
// /*AFLA*/ 		d.v32[1] = upper / divisor;
// /*AFLA*/ 		upper %= divisor;
// /*AFLA*/ 	}
// /*AFLA*/ 	asm ("divl %2" : "=a" (d.v32[0]), "=d" (*remainder) :
// /*AFLA*/ 		"rm" (divisor), "0" (d.v32[0]), "1" (upper));
// /*AFLA*/ 	return d.v64;
// /*AFLA*/ }
// /*AFLA*/ #define div_u64_rem	div_u64_rem
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ # include <asm-generic/div64.h>
// /*AFLA*/ #endif /* CONFIG_X86_32 */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_DIV64_H */
