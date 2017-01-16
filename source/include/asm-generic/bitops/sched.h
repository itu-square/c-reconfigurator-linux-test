// /*AFLA*/ #ifndef _ASM_GENERIC_BITOPS_SCHED_H_
// /*AFLA*/ #define _ASM_GENERIC_BITOPS_SCHED_H_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>	/* unlikely() */
// /*AFLA*/ #include <asm/types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Every architecture must define this function. It's the fastest
// /*AFLA*/  * way of searching a 100-bit bitmap.  It's guaranteed that at least
// /*AFLA*/  * one of the 100 bits is cleared.
// /*AFLA*/  */
// /*AFLA*/ static inline int sched_find_first_bit(const unsigned long *b)
// /*AFLA*/ {
// /*AFLA*/ #if BITS_PER_LONG == 64
// /*AFLA*/ 	if (b[0])
// /*AFLA*/ 		return __ffs(b[0]);
// /*AFLA*/ 	return __ffs(b[1]) + 64;
// /*AFLA*/ #elif BITS_PER_LONG == 32
// /*AFLA*/ 	if (b[0])
// /*AFLA*/ 		return __ffs(b[0]);
// /*AFLA*/ 	if (b[1])
// /*AFLA*/ 		return __ffs(b[1]) + 32;
// /*AFLA*/ 	if (b[2])
// /*AFLA*/ 		return __ffs(b[2]) + 64;
// /*AFLA*/ 	return __ffs(b[3]) + 96;
// /*AFLA*/ #else
// /*AFLA*/ #error BITS_PER_LONG not defined
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_GENERIC_BITOPS_SCHED_H_ */
