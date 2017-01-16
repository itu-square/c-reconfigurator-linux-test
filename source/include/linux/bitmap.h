// /*AFLA*/ #ifndef __LINUX_BITMAP_H
// /*AFLA*/ #define __LINUX_BITMAP_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/bitops.h>
// /*AFLA*/ #include <linux/string.h>
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * bitmaps provide bit arrays that consume one or more unsigned
// /*AFLA*/  * longs.  The bitmap interface and available operations are listed
// /*AFLA*/  * here, in bitmap.h
// /*AFLA*/  *
// /*AFLA*/  * Function implementations generic to all architectures are in
// /*AFLA*/  * lib/bitmap.c.  Functions implementations that are architecture
// /*AFLA*/  * specific are in various include/asm-<arch>/bitops.h headers
// /*AFLA*/  * and other arch/<arch> specific files.
// /*AFLA*/  *
// /*AFLA*/  * See lib/bitmap.c for more details.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The available bitmap operations and their rough meaning in the
// /*AFLA*/  * case that the bitmap is a single unsigned long are thus:
// /*AFLA*/  *
// /*AFLA*/  * Note that nbits should be always a compile time evaluable constant.
// /*AFLA*/  * Otherwise many inlines will generate horrible code.
// /*AFLA*/  *
// /*AFLA*/  * bitmap_zero(dst, nbits)			*dst = 0UL
// /*AFLA*/  * bitmap_fill(dst, nbits)			*dst = ~0UL
// /*AFLA*/  * bitmap_copy(dst, src, nbits)			*dst = *src
// /*AFLA*/  * bitmap_and(dst, src1, src2, nbits)		*dst = *src1 & *src2
// /*AFLA*/  * bitmap_or(dst, src1, src2, nbits)		*dst = *src1 | *src2
// /*AFLA*/  * bitmap_xor(dst, src1, src2, nbits)		*dst = *src1 ^ *src2
// /*AFLA*/  * bitmap_andnot(dst, src1, src2, nbits)	*dst = *src1 & ~(*src2)
// /*AFLA*/  * bitmap_complement(dst, src, nbits)		*dst = ~(*src)
// /*AFLA*/  * bitmap_equal(src1, src2, nbits)		Are *src1 and *src2 equal?
// /*AFLA*/  * bitmap_intersects(src1, src2, nbits) 	Do *src1 and *src2 overlap?
// /*AFLA*/  * bitmap_subset(src1, src2, nbits)		Is *src1 a subset of *src2?
// /*AFLA*/  * bitmap_empty(src, nbits)			Are all bits zero in *src?
// /*AFLA*/  * bitmap_full(src, nbits)			Are all bits set in *src?
// /*AFLA*/  * bitmap_weight(src, nbits)			Hamming Weight: number set bits
// /*AFLA*/  * bitmap_set(dst, pos, nbits)			Set specified bit area
// /*AFLA*/  * bitmap_clear(dst, pos, nbits)		Clear specified bit area
// /*AFLA*/  * bitmap_find_next_zero_area(buf, len, pos, n, mask)	Find bit free area
// /*AFLA*/  * bitmap_find_next_zero_area_off(buf, len, pos, n, mask)	as above
// /*AFLA*/  * bitmap_shift_right(dst, src, n, nbits)	*dst = *src >> n
// /*AFLA*/  * bitmap_shift_left(dst, src, n, nbits)	*dst = *src << n
// /*AFLA*/  * bitmap_remap(dst, src, old, new, nbits)	*dst = map(old, new)(src)
// /*AFLA*/  * bitmap_bitremap(oldbit, old, new, nbits)	newbit = map(old, new)(oldbit)
// /*AFLA*/  * bitmap_onto(dst, orig, relmap, nbits)	*dst = orig relative to relmap
// /*AFLA*/  * bitmap_fold(dst, orig, sz, nbits)		dst bits = orig bits mod sz
// /*AFLA*/  * bitmap_parse(buf, buflen, dst, nbits)	Parse bitmap dst from kernel buf
// /*AFLA*/  * bitmap_parse_user(ubuf, ulen, dst, nbits)	Parse bitmap dst from user buf
// /*AFLA*/  * bitmap_parselist(buf, dst, nbits)		Parse bitmap dst from kernel buf
// /*AFLA*/  * bitmap_parselist_user(buf, dst, nbits)	Parse bitmap dst from user buf
// /*AFLA*/  * bitmap_find_free_region(bitmap, bits, order)	Find and allocate bit region
// /*AFLA*/  * bitmap_release_region(bitmap, pos, order)	Free specified bit region
// /*AFLA*/  * bitmap_allocate_region(bitmap, pos, order)	Allocate specified bit region
// /*AFLA*/  * bitmap_from_u32array(dst, nbits, buf, nwords) *dst = *buf (nwords 32b words)
// /*AFLA*/  * bitmap_to_u32array(buf, nwords, src, nbits)	*buf = *dst (nwords 32b words)
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Also the following operations in asm/bitops.h apply to bitmaps.
// /*AFLA*/  *
// /*AFLA*/  * set_bit(bit, addr)			*addr |= bit
// /*AFLA*/  * clear_bit(bit, addr)			*addr &= ~bit
// /*AFLA*/  * change_bit(bit, addr)		*addr ^= bit
// /*AFLA*/  * test_bit(bit, addr)			Is bit set in *addr?
// /*AFLA*/  * test_and_set_bit(bit, addr)		Set bit and return old value
// /*AFLA*/  * test_and_clear_bit(bit, addr)	Clear bit and return old value
// /*AFLA*/  * test_and_change_bit(bit, addr)	Change bit and return old value
// /*AFLA*/  * find_first_zero_bit(addr, nbits)	Position first zero bit in *addr
// /*AFLA*/  * find_first_bit(addr, nbits)		Position first set bit in *addr
// /*AFLA*/  * find_next_zero_bit(addr, nbits, bit)	Position next zero bit in *addr >= bit
// /*AFLA*/  * find_next_bit(addr, nbits, bit)	Position next set bit in *addr >= bit
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The DECLARE_BITMAP(name,bits) macro, in linux/types.h, can be used
// /*AFLA*/  * to declare an array named 'name' of just enough unsigned longs to
// /*AFLA*/  * contain all bit positions from 0 to 'bits' - 1.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * lib/bitmap.c provides these functions:
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ extern int __bitmap_empty(const unsigned long *bitmap, unsigned int nbits);
// /*AFLA*/ extern int __bitmap_full(const unsigned long *bitmap, unsigned int nbits);
// /*AFLA*/ extern int __bitmap_equal(const unsigned long *bitmap1,
// /*AFLA*/ 			  const unsigned long *bitmap2, unsigned int nbits);
// /*AFLA*/ extern void __bitmap_complement(unsigned long *dst, const unsigned long *src,
// /*AFLA*/ 			unsigned int nbits);
// /*AFLA*/ extern void __bitmap_shift_right(unsigned long *dst, const unsigned long *src,
// /*AFLA*/ 				unsigned int shift, unsigned int nbits);
// /*AFLA*/ extern void __bitmap_shift_left(unsigned long *dst, const unsigned long *src,
// /*AFLA*/ 				unsigned int shift, unsigned int nbits);
// /*AFLA*/ extern int __bitmap_and(unsigned long *dst, const unsigned long *bitmap1,
// /*AFLA*/ 			const unsigned long *bitmap2, unsigned int nbits);
// /*AFLA*/ extern void __bitmap_or(unsigned long *dst, const unsigned long *bitmap1,
// /*AFLA*/ 			const unsigned long *bitmap2, unsigned int nbits);
// /*AFLA*/ extern void __bitmap_xor(unsigned long *dst, const unsigned long *bitmap1,
// /*AFLA*/ 			const unsigned long *bitmap2, unsigned int nbits);
// /*AFLA*/ extern int __bitmap_andnot(unsigned long *dst, const unsigned long *bitmap1,
// /*AFLA*/ 			const unsigned long *bitmap2, unsigned int nbits);
// /*AFLA*/ extern int __bitmap_intersects(const unsigned long *bitmap1,
// /*AFLA*/ 			const unsigned long *bitmap2, unsigned int nbits);
// /*AFLA*/ extern int __bitmap_subset(const unsigned long *bitmap1,
// /*AFLA*/ 			const unsigned long *bitmap2, unsigned int nbits);
// /*AFLA*/ extern int __bitmap_weight(const unsigned long *bitmap, unsigned int nbits);
// /*AFLA*/ 
// /*AFLA*/ extern void bitmap_set(unsigned long *map, unsigned int start, int len);
// /*AFLA*/ extern void bitmap_clear(unsigned long *map, unsigned int start, int len);
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long bitmap_find_next_zero_area_off(unsigned long *map,
// /*AFLA*/ 						    unsigned long size,
// /*AFLA*/ 						    unsigned long start,
// /*AFLA*/ 						    unsigned int nr,
// /*AFLA*/ 						    unsigned long align_mask,
// /*AFLA*/ 						    unsigned long align_offset);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * bitmap_find_next_zero_area - find a contiguous aligned zero area
// /*AFLA*/  * @map: The address to base the search on
// /*AFLA*/  * @size: The bitmap size in bits
// /*AFLA*/  * @start: The bitnumber to start searching at
// /*AFLA*/  * @nr: The number of zeroed bits we're looking for
// /*AFLA*/  * @align_mask: Alignment mask for zero area
// /*AFLA*/  *
// /*AFLA*/  * The @align_mask should be one less than a power of 2; the effect is that
// /*AFLA*/  * the bit offset of all zero areas this function finds is multiples of that
// /*AFLA*/  * power of 2. A @align_mask of 0 means no alignment is required.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long
// /*AFLA*/ bitmap_find_next_zero_area(unsigned long *map,
// /*AFLA*/ 			   unsigned long size,
// /*AFLA*/ 			   unsigned long start,
// /*AFLA*/ 			   unsigned int nr,
// /*AFLA*/ 			   unsigned long align_mask)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_find_next_zero_area_off(map, size, start, nr,
// /*AFLA*/ 					      align_mask, 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int __bitmap_parse(const char *buf, unsigned int buflen, int is_user,
// /*AFLA*/ 			unsigned long *dst, int nbits);
// /*AFLA*/ extern int bitmap_parse_user(const char __user *ubuf, unsigned int ulen,
// /*AFLA*/ 			unsigned long *dst, int nbits);
// /*AFLA*/ extern int bitmap_parselist(const char *buf, unsigned long *maskp,
// /*AFLA*/ 			int nmaskbits);
// /*AFLA*/ extern int bitmap_parselist_user(const char __user *ubuf, unsigned int ulen,
// /*AFLA*/ 			unsigned long *dst, int nbits);
// /*AFLA*/ extern void bitmap_remap(unsigned long *dst, const unsigned long *src,
// /*AFLA*/ 		const unsigned long *old, const unsigned long *new, unsigned int nbits);
// /*AFLA*/ extern int bitmap_bitremap(int oldbit,
// /*AFLA*/ 		const unsigned long *old, const unsigned long *new, int bits);
// /*AFLA*/ extern void bitmap_onto(unsigned long *dst, const unsigned long *orig,
// /*AFLA*/ 		const unsigned long *relmap, unsigned int bits);
// /*AFLA*/ extern void bitmap_fold(unsigned long *dst, const unsigned long *orig,
// /*AFLA*/ 		unsigned int sz, unsigned int nbits);
// /*AFLA*/ extern int bitmap_find_free_region(unsigned long *bitmap, unsigned int bits, int order);
// /*AFLA*/ extern void bitmap_release_region(unsigned long *bitmap, unsigned int pos, int order);
// /*AFLA*/ extern int bitmap_allocate_region(unsigned long *bitmap, unsigned int pos, int order);
// /*AFLA*/ extern unsigned int bitmap_from_u32array(unsigned long *bitmap,
// /*AFLA*/ 					 unsigned int nbits,
// /*AFLA*/ 					 const u32 *buf,
// /*AFLA*/ 					 unsigned int nwords);
// /*AFLA*/ extern unsigned int bitmap_to_u32array(u32 *buf,
// /*AFLA*/ 				       unsigned int nwords,
// /*AFLA*/ 				       const unsigned long *bitmap,
// /*AFLA*/ 				       unsigned int nbits);
// /*AFLA*/ #ifdef __BIG_ENDIAN
// /*AFLA*/ extern void bitmap_copy_le(unsigned long *dst, const unsigned long *src, unsigned int nbits);
// /*AFLA*/ #else
// /*AFLA*/ #define bitmap_copy_le bitmap_copy
// /*AFLA*/ #endif
// /*AFLA*/ extern unsigned int bitmap_ord_to_pos(const unsigned long *bitmap, unsigned int ord, unsigned int nbits);
// /*AFLA*/ extern int bitmap_print_to_pagebuf(bool list, char *buf,
// /*AFLA*/ 				   const unsigned long *maskp, int nmaskbits);
// /*AFLA*/ 
// /*AFLA*/ #define BITMAP_FIRST_WORD_MASK(start) (~0UL << ((start) & (BITS_PER_LONG - 1)))
// /*AFLA*/ #define BITMAP_LAST_WORD_MASK(nbits) (~0UL >> (-(nbits) & (BITS_PER_LONG - 1)))
// /*AFLA*/ 
// /*AFLA*/ #define small_const_nbits(nbits) \
// /*AFLA*/ 	(__builtin_constant_p(nbits) && (nbits) <= BITS_PER_LONG)
// /*AFLA*/ 
// /*AFLA*/ static inline void bitmap_zero(unsigned long *dst, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	if (small_const_nbits(nbits))
// /*AFLA*/ 		*dst = 0UL;
// /*AFLA*/ 	else {
// /*AFLA*/ 		unsigned int len = BITS_TO_LONGS(nbits) * sizeof(unsigned long);
// /*AFLA*/ 		memset(dst, 0, len);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void bitmap_fill(unsigned long *dst, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned int nlongs = BITS_TO_LONGS(nbits);
// /*AFLA*/ 	if (!small_const_nbits(nbits)) {
// /*AFLA*/ 		unsigned int len = (nlongs - 1) * sizeof(unsigned long);
// /*AFLA*/ 		memset(dst, 0xff,  len);
// /*AFLA*/ 	}
// /*AFLA*/ 	dst[nlongs - 1] = BITMAP_LAST_WORD_MASK(nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void bitmap_copy(unsigned long *dst, const unsigned long *src,
// /*AFLA*/ 			unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	if (small_const_nbits(nbits))
// /*AFLA*/ 		*dst = *src;
// /*AFLA*/ 	else {
// /*AFLA*/ 		unsigned int len = BITS_TO_LONGS(nbits) * sizeof(unsigned long);
// /*AFLA*/ 		memcpy(dst, src, len);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int bitmap_and(unsigned long *dst, const unsigned long *src1,
// /*AFLA*/ 			const unsigned long *src2, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	if (small_const_nbits(nbits))
// /*AFLA*/ 		return (*dst = *src1 & *src2 & BITMAP_LAST_WORD_MASK(nbits)) != 0;
// /*AFLA*/ 	return __bitmap_and(dst, src1, src2, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void bitmap_or(unsigned long *dst, const unsigned long *src1,
// /*AFLA*/ 			const unsigned long *src2, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	if (small_const_nbits(nbits))
// /*AFLA*/ 		*dst = *src1 | *src2;
// /*AFLA*/ 	else
// /*AFLA*/ 		__bitmap_or(dst, src1, src2, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void bitmap_xor(unsigned long *dst, const unsigned long *src1,
// /*AFLA*/ 			const unsigned long *src2, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	if (small_const_nbits(nbits))
// /*AFLA*/ 		*dst = *src1 ^ *src2;
// /*AFLA*/ 	else
// /*AFLA*/ 		__bitmap_xor(dst, src1, src2, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int bitmap_andnot(unsigned long *dst, const unsigned long *src1,
// /*AFLA*/ 			const unsigned long *src2, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	if (small_const_nbits(nbits))
// /*AFLA*/ 		return (*dst = *src1 & ~(*src2) & BITMAP_LAST_WORD_MASK(nbits)) != 0;
// /*AFLA*/ 	return __bitmap_andnot(dst, src1, src2, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void bitmap_complement(unsigned long *dst, const unsigned long *src,
// /*AFLA*/ 			unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	if (small_const_nbits(nbits))
// /*AFLA*/ 		*dst = ~(*src);
// /*AFLA*/ 	else
// /*AFLA*/ 		__bitmap_complement(dst, src, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int bitmap_equal(const unsigned long *src1,
// /*AFLA*/ 			const unsigned long *src2, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	if (small_const_nbits(nbits))
// /*AFLA*/ 		return !((*src1 ^ *src2) & BITMAP_LAST_WORD_MASK(nbits));
// /*AFLA*/ #ifdef CONFIG_S390
// /*AFLA*/ 	if (__builtin_constant_p(nbits) && (nbits % BITS_PER_LONG) == 0)
// /*AFLA*/ 		return !memcmp(src1, src2, nbits / 8);
// /*AFLA*/ #endif
// /*AFLA*/ 	return __bitmap_equal(src1, src2, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int bitmap_intersects(const unsigned long *src1,
// /*AFLA*/ 			const unsigned long *src2, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	if (small_const_nbits(nbits))
// /*AFLA*/ 		return ((*src1 & *src2) & BITMAP_LAST_WORD_MASK(nbits)) != 0;
// /*AFLA*/ 	else
// /*AFLA*/ 		return __bitmap_intersects(src1, src2, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int bitmap_subset(const unsigned long *src1,
// /*AFLA*/ 			const unsigned long *src2, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	if (small_const_nbits(nbits))
// /*AFLA*/ 		return ! ((*src1 & ~(*src2)) & BITMAP_LAST_WORD_MASK(nbits));
// /*AFLA*/ 	else
// /*AFLA*/ 		return __bitmap_subset(src1, src2, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int bitmap_empty(const unsigned long *src, unsigned nbits)
// /*AFLA*/ {
// /*AFLA*/ 	if (small_const_nbits(nbits))
// /*AFLA*/ 		return ! (*src & BITMAP_LAST_WORD_MASK(nbits));
// /*AFLA*/ 
// /*AFLA*/ 	return find_first_bit(src, nbits) == nbits;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int bitmap_full(const unsigned long *src, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	if (small_const_nbits(nbits))
// /*AFLA*/ 		return ! (~(*src) & BITMAP_LAST_WORD_MASK(nbits));
// /*AFLA*/ 
// /*AFLA*/ 	return find_first_zero_bit(src, nbits) == nbits;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline int bitmap_weight(const unsigned long *src, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	if (small_const_nbits(nbits))
// /*AFLA*/ 		return hweight_long(*src & BITMAP_LAST_WORD_MASK(nbits));
// /*AFLA*/ 	return __bitmap_weight(src, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void bitmap_shift_right(unsigned long *dst, const unsigned long *src,
// /*AFLA*/ 				unsigned int shift, int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	if (small_const_nbits(nbits))
// /*AFLA*/ 		*dst = (*src & BITMAP_LAST_WORD_MASK(nbits)) >> shift;
// /*AFLA*/ 	else
// /*AFLA*/ 		__bitmap_shift_right(dst, src, shift, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void bitmap_shift_left(unsigned long *dst, const unsigned long *src,
// /*AFLA*/ 				unsigned int shift, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	if (small_const_nbits(nbits))
// /*AFLA*/ 		*dst = (*src << shift) & BITMAP_LAST_WORD_MASK(nbits);
// /*AFLA*/ 	else
// /*AFLA*/ 		__bitmap_shift_left(dst, src, shift, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int bitmap_parse(const char *buf, unsigned int buflen,
// /*AFLA*/ 			unsigned long *maskp, int nmaskbits)
// /*AFLA*/ {
// /*AFLA*/ 	return __bitmap_parse(buf, buflen, 0, maskp, nmaskbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * bitmap_from_u64 - Check and swap words within u64.
// /*AFLA*/  *  @mask: source bitmap
// /*AFLA*/  *  @dst:  destination bitmap
// /*AFLA*/  *
// /*AFLA*/  * In 32-bit Big Endian kernel, when using (u32 *)(&val)[*]
// /*AFLA*/  * to read u64 mask, we will get the wrong word.
// /*AFLA*/  * That is "(u32 *)(&val)[0]" gets the upper 32 bits,
// /*AFLA*/  * but we expect the lower 32-bits of u64.
// /*AFLA*/  */
// /*AFLA*/ static inline void bitmap_from_u64(unsigned long *dst, u64 mask)
// /*AFLA*/ {
// /*AFLA*/ 	dst[0] = mask & ULONG_MAX;
// /*AFLA*/ 
// /*AFLA*/ 	if (sizeof(mask) > sizeof(unsigned long))
// /*AFLA*/ 		dst[1] = mask >> 32;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_BITMAP_H */
