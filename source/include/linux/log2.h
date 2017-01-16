// /*AFLA*/ /* Integer base 2 logarithm calculation
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2006 Red Hat, Inc. All Rights Reserved.
// /*AFLA*/  * Written by David Howells (dhowells@redhat.com)
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or
// /*AFLA*/  * modify it under the terms of the GNU General Public License
// /*AFLA*/  * as published by the Free Software Foundation; either version
// /*AFLA*/  * 2 of the License, or (at your option) any later version.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_LOG2_H
// /*AFLA*/ #define _LINUX_LOG2_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/bitops.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * deal with unrepresentable constant logarithms
// /*AFLA*/  */
// /*AFLA*/ extern __attribute__((const, noreturn))
// /*AFLA*/ int ____ilog2_NaN(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * non-constant log of base 2 calculators
// /*AFLA*/  * - the arch may override these in asm/bitops.h if they can be implemented
// /*AFLA*/  *   more efficiently than using fls() and fls64()
// /*AFLA*/  * - the arch is not required to handle n==0 if implementing the fallback
// /*AFLA*/  */
// /*AFLA*/ #ifndef CONFIG_ARCH_HAS_ILOG2_U32
// /*AFLA*/ static inline __attribute__((const))
// /*AFLA*/ int __ilog2_u32(u32 n)
// /*AFLA*/ {
// /*AFLA*/ 	return fls(n) - 1;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_ARCH_HAS_ILOG2_U64
// /*AFLA*/ static inline __attribute__((const))
// /*AFLA*/ int __ilog2_u64(u64 n)
// /*AFLA*/ {
// /*AFLA*/ 	return fls64(n) - 1;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  *  Determine whether some value is a power of two, where zero is
// /*AFLA*/  * *not* considered a power of two.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static inline __attribute__((const))
// /*AFLA*/ bool is_power_of_2(unsigned long n)
// /*AFLA*/ {
// /*AFLA*/ 	return (n != 0 && ((n & (n - 1)) == 0));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * round up to nearest power of two
// /*AFLA*/  */
// /*AFLA*/ static inline __attribute__((const))
// /*AFLA*/ unsigned long __roundup_pow_of_two(unsigned long n)
// /*AFLA*/ {
// /*AFLA*/ 	return 1UL << fls_long(n - 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * round down to nearest power of two
// /*AFLA*/  */
// /*AFLA*/ static inline __attribute__((const))
// /*AFLA*/ unsigned long __rounddown_pow_of_two(unsigned long n)
// /*AFLA*/ {
// /*AFLA*/ 	return 1UL << (fls_long(n) - 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ilog2 - log of base 2 of 32-bit or a 64-bit unsigned value
// /*AFLA*/  * @n - parameter
// /*AFLA*/  *
// /*AFLA*/  * constant-capable log of base 2 calculation
// /*AFLA*/  * - this can be used to initialise global variables from constant data, hence
// /*AFLA*/  *   the massive ternary operator construction
// /*AFLA*/  *
// /*AFLA*/  * selects the appropriately-sized optimised version depending on sizeof(n)
// /*AFLA*/  */
// /*AFLA*/ #define ilog2(n)				\
// /*AFLA*/ (						\
// /*AFLA*/ 	__builtin_constant_p(n) ? (		\
// /*AFLA*/ 		(n) < 1 ? ____ilog2_NaN() :	\
// /*AFLA*/ 		(n) & (1ULL << 63) ? 63 :	\
// /*AFLA*/ 		(n) & (1ULL << 62) ? 62 :	\
// /*AFLA*/ 		(n) & (1ULL << 61) ? 61 :	\
// /*AFLA*/ 		(n) & (1ULL << 60) ? 60 :	\
// /*AFLA*/ 		(n) & (1ULL << 59) ? 59 :	\
// /*AFLA*/ 		(n) & (1ULL << 58) ? 58 :	\
// /*AFLA*/ 		(n) & (1ULL << 57) ? 57 :	\
// /*AFLA*/ 		(n) & (1ULL << 56) ? 56 :	\
// /*AFLA*/ 		(n) & (1ULL << 55) ? 55 :	\
// /*AFLA*/ 		(n) & (1ULL << 54) ? 54 :	\
// /*AFLA*/ 		(n) & (1ULL << 53) ? 53 :	\
// /*AFLA*/ 		(n) & (1ULL << 52) ? 52 :	\
// /*AFLA*/ 		(n) & (1ULL << 51) ? 51 :	\
// /*AFLA*/ 		(n) & (1ULL << 50) ? 50 :	\
// /*AFLA*/ 		(n) & (1ULL << 49) ? 49 :	\
// /*AFLA*/ 		(n) & (1ULL << 48) ? 48 :	\
// /*AFLA*/ 		(n) & (1ULL << 47) ? 47 :	\
// /*AFLA*/ 		(n) & (1ULL << 46) ? 46 :	\
// /*AFLA*/ 		(n) & (1ULL << 45) ? 45 :	\
// /*AFLA*/ 		(n) & (1ULL << 44) ? 44 :	\
// /*AFLA*/ 		(n) & (1ULL << 43) ? 43 :	\
// /*AFLA*/ 		(n) & (1ULL << 42) ? 42 :	\
// /*AFLA*/ 		(n) & (1ULL << 41) ? 41 :	\
// /*AFLA*/ 		(n) & (1ULL << 40) ? 40 :	\
// /*AFLA*/ 		(n) & (1ULL << 39) ? 39 :	\
// /*AFLA*/ 		(n) & (1ULL << 38) ? 38 :	\
// /*AFLA*/ 		(n) & (1ULL << 37) ? 37 :	\
// /*AFLA*/ 		(n) & (1ULL << 36) ? 36 :	\
// /*AFLA*/ 		(n) & (1ULL << 35) ? 35 :	\
// /*AFLA*/ 		(n) & (1ULL << 34) ? 34 :	\
// /*AFLA*/ 		(n) & (1ULL << 33) ? 33 :	\
// /*AFLA*/ 		(n) & (1ULL << 32) ? 32 :	\
// /*AFLA*/ 		(n) & (1ULL << 31) ? 31 :	\
// /*AFLA*/ 		(n) & (1ULL << 30) ? 30 :	\
// /*AFLA*/ 		(n) & (1ULL << 29) ? 29 :	\
// /*AFLA*/ 		(n) & (1ULL << 28) ? 28 :	\
// /*AFLA*/ 		(n) & (1ULL << 27) ? 27 :	\
// /*AFLA*/ 		(n) & (1ULL << 26) ? 26 :	\
// /*AFLA*/ 		(n) & (1ULL << 25) ? 25 :	\
// /*AFLA*/ 		(n) & (1ULL << 24) ? 24 :	\
// /*AFLA*/ 		(n) & (1ULL << 23) ? 23 :	\
// /*AFLA*/ 		(n) & (1ULL << 22) ? 22 :	\
// /*AFLA*/ 		(n) & (1ULL << 21) ? 21 :	\
// /*AFLA*/ 		(n) & (1ULL << 20) ? 20 :	\
// /*AFLA*/ 		(n) & (1ULL << 19) ? 19 :	\
// /*AFLA*/ 		(n) & (1ULL << 18) ? 18 :	\
// /*AFLA*/ 		(n) & (1ULL << 17) ? 17 :	\
// /*AFLA*/ 		(n) & (1ULL << 16) ? 16 :	\
// /*AFLA*/ 		(n) & (1ULL << 15) ? 15 :	\
// /*AFLA*/ 		(n) & (1ULL << 14) ? 14 :	\
// /*AFLA*/ 		(n) & (1ULL << 13) ? 13 :	\
// /*AFLA*/ 		(n) & (1ULL << 12) ? 12 :	\
// /*AFLA*/ 		(n) & (1ULL << 11) ? 11 :	\
// /*AFLA*/ 		(n) & (1ULL << 10) ? 10 :	\
// /*AFLA*/ 		(n) & (1ULL <<  9) ?  9 :	\
// /*AFLA*/ 		(n) & (1ULL <<  8) ?  8 :	\
// /*AFLA*/ 		(n) & (1ULL <<  7) ?  7 :	\
// /*AFLA*/ 		(n) & (1ULL <<  6) ?  6 :	\
// /*AFLA*/ 		(n) & (1ULL <<  5) ?  5 :	\
// /*AFLA*/ 		(n) & (1ULL <<  4) ?  4 :	\
// /*AFLA*/ 		(n) & (1ULL <<  3) ?  3 :	\
// /*AFLA*/ 		(n) & (1ULL <<  2) ?  2 :	\
// /*AFLA*/ 		(n) & (1ULL <<  1) ?  1 :	\
// /*AFLA*/ 		(n) & (1ULL <<  0) ?  0 :	\
// /*AFLA*/ 		____ilog2_NaN()			\
// /*AFLA*/ 				   ) :		\
// /*AFLA*/ 	(sizeof(n) <= 4) ?			\
// /*AFLA*/ 	__ilog2_u32(n) :			\
// /*AFLA*/ 	__ilog2_u64(n)				\
// /*AFLA*/  )
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * roundup_pow_of_two - round the given value up to nearest power of two
// /*AFLA*/  * @n - parameter
// /*AFLA*/  *
// /*AFLA*/  * round the given value up to the nearest power of two
// /*AFLA*/  * - the result is undefined when n == 0
// /*AFLA*/  * - this can be used to initialise global variables from constant data
// /*AFLA*/  */
// /*AFLA*/ #define roundup_pow_of_two(n)			\
// /*AFLA*/ (						\
// /*AFLA*/ 	__builtin_constant_p(n) ? (		\
// /*AFLA*/ 		(n == 1) ? 1 :			\
// /*AFLA*/ 		(1UL << (ilog2((n) - 1) + 1))	\
// /*AFLA*/ 				   ) :		\
// /*AFLA*/ 	__roundup_pow_of_two(n)			\
// /*AFLA*/  )
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rounddown_pow_of_two - round the given value down to nearest power of two
// /*AFLA*/  * @n - parameter
// /*AFLA*/  *
// /*AFLA*/  * round the given value down to the nearest power of two
// /*AFLA*/  * - the result is undefined when n == 0
// /*AFLA*/  * - this can be used to initialise global variables from constant data
// /*AFLA*/  */
// /*AFLA*/ #define rounddown_pow_of_two(n)			\
// /*AFLA*/ (						\
// /*AFLA*/ 	__builtin_constant_p(n) ? (		\
// /*AFLA*/ 		(1UL << ilog2(n))) :		\
// /*AFLA*/ 	__rounddown_pow_of_two(n)		\
// /*AFLA*/  )
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * order_base_2 - calculate the (rounded up) base 2 order of the argument
// /*AFLA*/  * @n: parameter
// /*AFLA*/  *
// /*AFLA*/  * The first few values calculated by this routine:
// /*AFLA*/  *  ob2(0) = 0
// /*AFLA*/  *  ob2(1) = 0
// /*AFLA*/  *  ob2(2) = 1
// /*AFLA*/  *  ob2(3) = 2
// /*AFLA*/  *  ob2(4) = 2
// /*AFLA*/  *  ob2(5) = 3
// /*AFLA*/  *  ... and so on.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define order_base_2(n) ilog2(roundup_pow_of_two(n))
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_LOG2_H */
