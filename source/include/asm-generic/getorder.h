// /*AFLA*/ #ifndef __ASM_GENERIC_GETORDER_H
// /*AFLA*/ #define __ASM_GENERIC_GETORDER_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/log2.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Runtime evaluation of get_order()
// /*AFLA*/  */
// /*AFLA*/ static inline __attribute_const__
// /*AFLA*/ int __get_order(unsigned long size)
// /*AFLA*/ {
// /*AFLA*/ 	int order;
// /*AFLA*/ 
// /*AFLA*/ 	size--;
// /*AFLA*/ 	size >>= PAGE_SHIFT;
// /*AFLA*/ #if BITS_PER_LONG == 32
// /*AFLA*/ 	order = fls(size);
// /*AFLA*/ #else
// /*AFLA*/ 	order = fls64(size);
// /*AFLA*/ #endif
// /*AFLA*/ 	return order;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * get_order - Determine the allocation order of a memory size
// /*AFLA*/  * @size: The size for which to get the order
// /*AFLA*/  *
// /*AFLA*/  * Determine the allocation order of a particular sized block of memory.  This
// /*AFLA*/  * is on a logarithmic scale, where:
// /*AFLA*/  *
// /*AFLA*/  *	0 -> 2^0 * PAGE_SIZE and below
// /*AFLA*/  *	1 -> 2^1 * PAGE_SIZE to 2^0 * PAGE_SIZE + 1
// /*AFLA*/  *	2 -> 2^2 * PAGE_SIZE to 2^1 * PAGE_SIZE + 1
// /*AFLA*/  *	3 -> 2^3 * PAGE_SIZE to 2^2 * PAGE_SIZE + 1
// /*AFLA*/  *	4 -> 2^4 * PAGE_SIZE to 2^3 * PAGE_SIZE + 1
// /*AFLA*/  *	...
// /*AFLA*/  *
// /*AFLA*/  * The order returned is used to find the smallest allocation granule required
// /*AFLA*/  * to hold an object of the specified size.
// /*AFLA*/  *
// /*AFLA*/  * The result is undefined if the size is 0.
// /*AFLA*/  *
// /*AFLA*/  * This function may be used to initialise variables with compile time
// /*AFLA*/  * evaluations of constants.
// /*AFLA*/  */
// /*AFLA*/ #define get_order(n)						\
// /*AFLA*/ (								\
// /*AFLA*/ 	__builtin_constant_p(n) ? (				\
// /*AFLA*/ 		((n) == 0UL) ? BITS_PER_LONG - PAGE_SHIFT :	\
// /*AFLA*/ 		(((n) < (1UL << PAGE_SHIFT)) ? 0 :		\
// /*AFLA*/ 		 ilog2((n) - 1) - PAGE_SHIFT + 1)		\
// /*AFLA*/ 	) :							\
// /*AFLA*/ 	__get_order(n)						\
// /*AFLA*/ )
// /*AFLA*/ 
// /*AFLA*/ #endif	/* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif	/* __ASM_GENERIC_GETORDER_H */
