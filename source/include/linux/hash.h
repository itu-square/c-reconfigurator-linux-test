// /*AFLA*/ #ifndef _LINUX_HASH_H
// /*AFLA*/ #define _LINUX_HASH_H
// /*AFLA*/ /* Fast hashing routine for ints,  longs and pointers.
// /*AFLA*/    (C) 2002 Nadia Yvette Chambers, IBM */
// /*AFLA*/ 
// /*AFLA*/ #include <asm/types.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The "GOLDEN_RATIO_PRIME" is used in ifs/btrfs/brtfs_inode.h and
// /*AFLA*/  * fs/inode.c.  It's not actually prime any more (the previous primes
// /*AFLA*/  * were actively bad for hashing), but the name remains.
// /*AFLA*/  */
// /*AFLA*/ #if BITS_PER_LONG == 32
// /*AFLA*/ #define GOLDEN_RATIO_PRIME GOLDEN_RATIO_32
// /*AFLA*/ #define hash_long(val, bits) hash_32(val, bits)
// /*AFLA*/ #elif BITS_PER_LONG == 64
// /*AFLA*/ #define hash_long(val, bits) hash_64(val, bits)
// /*AFLA*/ #define GOLDEN_RATIO_PRIME GOLDEN_RATIO_64
// /*AFLA*/ #else
// /*AFLA*/ #error Wordsize not 32 or 64
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This hash multiplies the input by a large odd number and takes the
// /*AFLA*/  * high bits.  Since multiplication propagates changes to the most
// /*AFLA*/  * significant end only, it is essential that the high bits of the
// /*AFLA*/  * product be used for the hash value.
// /*AFLA*/  *
// /*AFLA*/  * Chuck Lever verified the effectiveness of this technique:
// /*AFLA*/  * http://www.citi.umich.edu/techreports/reports/citi-tr-00-1.pdf
// /*AFLA*/  *
// /*AFLA*/  * Although a random odd number will do, it turns out that the golden
// /*AFLA*/  * ratio phi = (sqrt(5)-1)/2, or its negative, has particularly nice
// /*AFLA*/  * properties.  (See Knuth vol 3, section 6.4, exercise 9.)
// /*AFLA*/  *
// /*AFLA*/  * These are the negative, (1 - phi) = phi**2 = (3 - sqrt(5))/2,
// /*AFLA*/  * which is very slightly easier to multiply by and makes no
// /*AFLA*/  * difference to the hash distribution.
// /*AFLA*/  */
// /*AFLA*/ #define GOLDEN_RATIO_32 0x61C88647
// /*AFLA*/ #define GOLDEN_RATIO_64 0x61C8864680B583EBull
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_ARCH_HASH
// /*AFLA*/ /* This header may use the GOLDEN_RATIO_xx constants */
// /*AFLA*/ #include <asm/hash.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The _generic versions exist only so lib/test_hash.c can compare
// /*AFLA*/  * the arch-optimized versions with the generic.
// /*AFLA*/  *
// /*AFLA*/  * Note that if you change these, any <asm/hash.h> that aren't updated
// /*AFLA*/  * to match need to have their HAVE_ARCH_* define values updated so the
// /*AFLA*/  * self-test will not false-positive.
// /*AFLA*/  */
// /*AFLA*/ #ifndef HAVE_ARCH__HASH_32
// /*AFLA*/ #define __hash_32 __hash_32_generic
// /*AFLA*/ #endif
// /*AFLA*/ static inline u32 __hash_32_generic(u32 val)
// /*AFLA*/ {
// /*AFLA*/ 	return val * GOLDEN_RATIO_32;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifndef HAVE_ARCH_HASH_32
// /*AFLA*/ #define hash_32 hash_32_generic
// /*AFLA*/ #endif
// /*AFLA*/ static inline u32 hash_32_generic(u32 val, unsigned int bits)
// /*AFLA*/ {
// /*AFLA*/ 	/* High bits are more random, so use them. */
// /*AFLA*/ 	return __hash_32(val) >> (32 - bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifndef HAVE_ARCH_HASH_64
// /*AFLA*/ #define hash_64 hash_64_generic
// /*AFLA*/ #endif
// /*AFLA*/ static __always_inline u32 hash_64_generic(u64 val, unsigned int bits)
// /*AFLA*/ {
// /*AFLA*/ #if BITS_PER_LONG == 64
// /*AFLA*/ 	/* 64x64-bit multiply is efficient on all 64-bit processors */
// /*AFLA*/ 	return val * GOLDEN_RATIO_64 >> (64 - bits);
// /*AFLA*/ #else
// /*AFLA*/ 	/* Hash 64 bits using only 32x32-bit multiply. */
// /*AFLA*/ 	return hash_32((u32)val ^ __hash_32(val >> 32), bits);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u32 hash_ptr(const void *ptr, unsigned int bits)
// /*AFLA*/ {
// /*AFLA*/ 	return hash_long((unsigned long)ptr, bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* This really should be called fold32_ptr; it does no hashing to speak of. */
// /*AFLA*/ static inline u32 hash32_ptr(const void *ptr)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long val = (unsigned long)ptr;
// /*AFLA*/ 
// /*AFLA*/ #if BITS_PER_LONG == 64
// /*AFLA*/ 	val ^= (val >> 32);
// /*AFLA*/ #endif
// /*AFLA*/ 	return (u32)val;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_HASH_H */
