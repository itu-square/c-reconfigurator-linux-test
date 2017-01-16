// /*AFLA*/ #ifndef _LINUX_BITOPS_H
// /*AFLA*/ #define _LINUX_BITOPS_H
// /*AFLA*/ #include <asm/types.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef	__KERNEL__
// /*AFLA*/ #define BIT(nr)			(1UL << (nr))
// /*AFLA*/ #define BIT_ULL(nr)		(1ULL << (nr))
// /*AFLA*/ #define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
// /*AFLA*/ #define BIT_WORD(nr)		((nr) / BITS_PER_LONG)
// /*AFLA*/ #define BIT_ULL_MASK(nr)	(1ULL << ((nr) % BITS_PER_LONG_LONG))
// /*AFLA*/ #define BIT_ULL_WORD(nr)	((nr) / BITS_PER_LONG_LONG)
// /*AFLA*/ #define BITS_PER_BYTE		8
// /*AFLA*/ #define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Create a contiguous bitmask starting at bit position @l and ending at
// /*AFLA*/  * position @h. For example
// /*AFLA*/  * GENMASK_ULL(39, 21) gives us the 64bit vector 0x000000ffffe00000.
// /*AFLA*/  */
// /*AFLA*/ #define GENMASK(h, l) \
// /*AFLA*/ 	(((~0UL) << (l)) & (~0UL >> (BITS_PER_LONG - 1 - (h))))
// /*AFLA*/ 
// /*AFLA*/ #define GENMASK_ULL(h, l) \
// /*AFLA*/ 	(((~0ULL) << (l)) & (~0ULL >> (BITS_PER_LONG_LONG - 1 - (h))))
// /*AFLA*/ 
// /*AFLA*/ extern unsigned int __sw_hweight8(unsigned int w);
// /*AFLA*/ extern unsigned int __sw_hweight16(unsigned int w);
// /*AFLA*/ extern unsigned int __sw_hweight32(unsigned int w);
// /*AFLA*/ extern unsigned long __sw_hweight64(__u64 w);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Include this here because some architectures need generic_ffs/fls in
// /*AFLA*/  * scope
// /*AFLA*/  */
// /*AFLA*/ #include <asm/bitops.h>
// /*AFLA*/ 
// /*AFLA*/ #define for_each_set_bit(bit, addr, size) \
// /*AFLA*/ 	for ((bit) = find_first_bit((addr), (size));		\
// /*AFLA*/ 	     (bit) < (size);					\
// /*AFLA*/ 	     (bit) = find_next_bit((addr), (size), (bit) + 1))
// /*AFLA*/ 
// /*AFLA*/ /* same as for_each_set_bit() but use bit as value to start with */
// /*AFLA*/ #define for_each_set_bit_from(bit, addr, size) \
// /*AFLA*/ 	for ((bit) = find_next_bit((addr), (size), (bit));	\
// /*AFLA*/ 	     (bit) < (size);					\
// /*AFLA*/ 	     (bit) = find_next_bit((addr), (size), (bit) + 1))
// /*AFLA*/ 
// /*AFLA*/ #define for_each_clear_bit(bit, addr, size) \
// /*AFLA*/ 	for ((bit) = find_first_zero_bit((addr), (size));	\
// /*AFLA*/ 	     (bit) < (size);					\
// /*AFLA*/ 	     (bit) = find_next_zero_bit((addr), (size), (bit) + 1))
// /*AFLA*/ 
// /*AFLA*/ /* same as for_each_clear_bit() but use bit as value to start with */
// /*AFLA*/ #define for_each_clear_bit_from(bit, addr, size) \
// /*AFLA*/ 	for ((bit) = find_next_zero_bit((addr), (size), (bit));	\
// /*AFLA*/ 	     (bit) < (size);					\
// /*AFLA*/ 	     (bit) = find_next_zero_bit((addr), (size), (bit) + 1))
// /*AFLA*/ 
// /*AFLA*/ static inline int get_bitmask_order(unsigned int count)
// /*AFLA*/ {
// /*AFLA*/ 	int order;
// /*AFLA*/ 
// /*AFLA*/ 	order = fls(count);
// /*AFLA*/ 	return order;	/* We could be slightly more clever with -1 here... */
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline unsigned long hweight_long(unsigned long w)
// /*AFLA*/ {
// /*AFLA*/ 	return sizeof(w) == 4 ? hweight32(w) : hweight64(w);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rol64 - rotate a 64-bit value left
// /*AFLA*/  * @word: value to rotate
// /*AFLA*/  * @shift: bits to roll
// /*AFLA*/  */
// /*AFLA*/ static inline __u64 rol64(__u64 word, unsigned int shift)
// /*AFLA*/ {
// /*AFLA*/ 	return (word << shift) | (word >> (64 - shift));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ror64 - rotate a 64-bit value right
// /*AFLA*/  * @word: value to rotate
// /*AFLA*/  * @shift: bits to roll
// /*AFLA*/  */
// /*AFLA*/ static inline __u64 ror64(__u64 word, unsigned int shift)
// /*AFLA*/ {
// /*AFLA*/ 	return (word >> shift) | (word << (64 - shift));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rol32 - rotate a 32-bit value left
// /*AFLA*/  * @word: value to rotate
// /*AFLA*/  * @shift: bits to roll
// /*AFLA*/  */
// /*AFLA*/ static inline __u32 rol32(__u32 word, unsigned int shift)
// /*AFLA*/ {
// /*AFLA*/ 	return (word << shift) | (word >> ((-shift) & 31));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ror32 - rotate a 32-bit value right
// /*AFLA*/  * @word: value to rotate
// /*AFLA*/  * @shift: bits to roll
// /*AFLA*/  */
// /*AFLA*/ static inline __u32 ror32(__u32 word, unsigned int shift)
// /*AFLA*/ {
// /*AFLA*/ 	return (word >> shift) | (word << (32 - shift));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rol16 - rotate a 16-bit value left
// /*AFLA*/  * @word: value to rotate
// /*AFLA*/  * @shift: bits to roll
// /*AFLA*/  */
// /*AFLA*/ static inline __u16 rol16(__u16 word, unsigned int shift)
// /*AFLA*/ {
// /*AFLA*/ 	return (word << shift) | (word >> (16 - shift));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ror16 - rotate a 16-bit value right
// /*AFLA*/  * @word: value to rotate
// /*AFLA*/  * @shift: bits to roll
// /*AFLA*/  */
// /*AFLA*/ static inline __u16 ror16(__u16 word, unsigned int shift)
// /*AFLA*/ {
// /*AFLA*/ 	return (word >> shift) | (word << (16 - shift));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rol8 - rotate an 8-bit value left
// /*AFLA*/  * @word: value to rotate
// /*AFLA*/  * @shift: bits to roll
// /*AFLA*/  */
// /*AFLA*/ static inline __u8 rol8(__u8 word, unsigned int shift)
// /*AFLA*/ {
// /*AFLA*/ 	return (word << shift) | (word >> (8 - shift));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ror8 - rotate an 8-bit value right
// /*AFLA*/  * @word: value to rotate
// /*AFLA*/  * @shift: bits to roll
// /*AFLA*/  */
// /*AFLA*/ static inline __u8 ror8(__u8 word, unsigned int shift)
// /*AFLA*/ {
// /*AFLA*/ 	return (word >> shift) | (word << (8 - shift));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * sign_extend32 - sign extend a 32-bit value using specified bit as sign-bit
// /*AFLA*/  * @value: value to sign extend
// /*AFLA*/  * @index: 0 based bit index (0<=index<32) to sign bit
// /*AFLA*/  *
// /*AFLA*/  * This is safe to use for 16- and 8-bit types as well.
// /*AFLA*/  */
// /*AFLA*/ static inline __s32 sign_extend32(__u32 value, int index)
// /*AFLA*/ {
// /*AFLA*/ 	__u8 shift = 31 - index;
// /*AFLA*/ 	return (__s32)(value << shift) >> shift;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * sign_extend64 - sign extend a 64-bit value using specified bit as sign-bit
// /*AFLA*/  * @value: value to sign extend
// /*AFLA*/  * @index: 0 based bit index (0<=index<64) to sign bit
// /*AFLA*/  */
// /*AFLA*/ static inline __s64 sign_extend64(__u64 value, int index)
// /*AFLA*/ {
// /*AFLA*/ 	__u8 shift = 63 - index;
// /*AFLA*/ 	return (__s64)(value << shift) >> shift;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned fls_long(unsigned long l)
// /*AFLA*/ {
// /*AFLA*/ 	if (sizeof(l) == 4)
// /*AFLA*/ 		return fls(l);
// /*AFLA*/ 	return fls64(l);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int get_count_order(unsigned int count)
// /*AFLA*/ {
// /*AFLA*/ 	int order;
// /*AFLA*/ 
// /*AFLA*/ 	order = fls(count) - 1;
// /*AFLA*/ 	if (count & (count - 1))
// /*AFLA*/ 		order++;
// /*AFLA*/ 	return order;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * get_count_order_long - get order after rounding @l up to power of 2
// /*AFLA*/  * @l: parameter
// /*AFLA*/  *
// /*AFLA*/  * it is same as get_count_order() but with long type parameter
// /*AFLA*/  */
// /*AFLA*/ static inline int get_count_order_long(unsigned long l)
// /*AFLA*/ {
// /*AFLA*/ 	if (l == 0UL)
// /*AFLA*/ 		return -1;
// /*AFLA*/ 	else if (l & (l - 1UL))
// /*AFLA*/ 		return (int)fls_long(l);
// /*AFLA*/ 	else
// /*AFLA*/ 		return (int)fls_long(l) - 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __ffs64 - find first set bit in a 64 bit word
// /*AFLA*/  * @word: The 64 bit word
// /*AFLA*/  *
// /*AFLA*/  * On 64 bit arches this is a synomyn for __ffs
// /*AFLA*/  * The result is not defined if no bits are set, so check that @word
// /*AFLA*/  * is non-zero before calling this.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long __ffs64(u64 word)
// /*AFLA*/ {
// /*AFLA*/ #if BITS_PER_LONG == 32
// /*AFLA*/ 	if (((u32)word) == 0UL)
// /*AFLA*/ 		return __ffs((u32)(word >> 32)) + 32;
// /*AFLA*/ #elif BITS_PER_LONG != 64
// /*AFLA*/ #error BITS_PER_LONG not 32 or 64
// /*AFLA*/ #endif
// /*AFLA*/ 	return __ffs((unsigned long)word);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef __KERNEL__
// /*AFLA*/ 
// /*AFLA*/ #ifndef set_mask_bits
// /*AFLA*/ #define set_mask_bits(ptr, _mask, _bits)	\
// /*AFLA*/ ({								\
// /*AFLA*/ 	const typeof(*ptr) mask = (_mask), bits = (_bits);	\
// /*AFLA*/ 	typeof(*ptr) old, new;					\
// /*AFLA*/ 								\
// /*AFLA*/ 	do {							\
// /*AFLA*/ 		old = ACCESS_ONCE(*ptr);			\
// /*AFLA*/ 		new = (old & ~mask) | bits;			\
// /*AFLA*/ 	} while (cmpxchg(ptr, old, new) != old);		\
// /*AFLA*/ 								\
// /*AFLA*/ 	new;							\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef bit_clear_unless
// /*AFLA*/ #define bit_clear_unless(ptr, _clear, _test)	\
// /*AFLA*/ ({								\
// /*AFLA*/ 	const typeof(*ptr) clear = (_clear), test = (_test);	\
// /*AFLA*/ 	typeof(*ptr) old, new;					\
// /*AFLA*/ 								\
// /*AFLA*/ 	do {							\
// /*AFLA*/ 		old = ACCESS_ONCE(*ptr);			\
// /*AFLA*/ 		new = old & ~clear;				\
// /*AFLA*/ 	} while (!(old & test) &&				\
// /*AFLA*/ 		 cmpxchg(ptr, old, new) != old);		\
// /*AFLA*/ 								\
// /*AFLA*/ 	!(old & test);						\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef find_last_bit
// /*AFLA*/ /**
// /*AFLA*/  * find_last_bit - find the last set bit in a memory region
// /*AFLA*/  * @addr: The address to start the search at
// /*AFLA*/  * @size: The number of bits to search
// /*AFLA*/  *
// /*AFLA*/  * Returns the bit number of the last set bit, or size.
// /*AFLA*/  */
// /*AFLA*/ extern unsigned long find_last_bit(const unsigned long *addr,
// /*AFLA*/ 				   unsigned long size);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* __KERNEL__ */
// /*AFLA*/ #endif
