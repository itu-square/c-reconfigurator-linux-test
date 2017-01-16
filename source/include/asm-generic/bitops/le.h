// /*AFLA*/ #ifndef _ASM_GENERIC_BITOPS_LE_H_
// /*AFLA*/ #define _ASM_GENERIC_BITOPS_LE_H_
// /*AFLA*/ 
// /*AFLA*/ #include <asm/types.h>
// /*AFLA*/ #include <asm/byteorder.h>
// /*AFLA*/ 
// /*AFLA*/ #if defined(__LITTLE_ENDIAN)
// /*AFLA*/ 
// /*AFLA*/ #define BITOP_LE_SWIZZLE	0
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long find_next_zero_bit_le(const void *addr,
// /*AFLA*/ 		unsigned long size, unsigned long offset)
// /*AFLA*/ {
// /*AFLA*/ 	return find_next_zero_bit(addr, size, offset);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long find_next_bit_le(const void *addr,
// /*AFLA*/ 		unsigned long size, unsigned long offset)
// /*AFLA*/ {
// /*AFLA*/ 	return find_next_bit(addr, size, offset);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long find_first_zero_bit_le(const void *addr,
// /*AFLA*/ 		unsigned long size)
// /*AFLA*/ {
// /*AFLA*/ 	return find_first_zero_bit(addr, size);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #elif defined(__BIG_ENDIAN)
// /*AFLA*/ 
// /*AFLA*/ #define BITOP_LE_SWIZZLE	((BITS_PER_LONG-1) & ~0x7)
// /*AFLA*/ 
// /*AFLA*/ #ifndef find_next_zero_bit_le
// /*AFLA*/ extern unsigned long find_next_zero_bit_le(const void *addr,
// /*AFLA*/ 		unsigned long size, unsigned long offset);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef find_next_bit_le
// /*AFLA*/ extern unsigned long find_next_bit_le(const void *addr,
// /*AFLA*/ 		unsigned long size, unsigned long offset);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef find_first_zero_bit_le
// /*AFLA*/ #define find_first_zero_bit_le(addr, size) \
// /*AFLA*/ 	find_next_zero_bit_le((addr), (size), 0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ #error "Please fix <asm/byteorder.h>"
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline int test_bit_le(int nr, const void *addr)
// /*AFLA*/ {
// /*AFLA*/ 	return test_bit(nr ^ BITOP_LE_SWIZZLE, addr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void set_bit_le(int nr, void *addr)
// /*AFLA*/ {
// /*AFLA*/ 	set_bit(nr ^ BITOP_LE_SWIZZLE, addr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void clear_bit_le(int nr, void *addr)
// /*AFLA*/ {
// /*AFLA*/ 	clear_bit(nr ^ BITOP_LE_SWIZZLE, addr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __set_bit_le(int nr, void *addr)
// /*AFLA*/ {
// /*AFLA*/ 	__set_bit(nr ^ BITOP_LE_SWIZZLE, addr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __clear_bit_le(int nr, void *addr)
// /*AFLA*/ {
// /*AFLA*/ 	__clear_bit(nr ^ BITOP_LE_SWIZZLE, addr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int test_and_set_bit_le(int nr, void *addr)
// /*AFLA*/ {
// /*AFLA*/ 	return test_and_set_bit(nr ^ BITOP_LE_SWIZZLE, addr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int test_and_clear_bit_le(int nr, void *addr)
// /*AFLA*/ {
// /*AFLA*/ 	return test_and_clear_bit(nr ^ BITOP_LE_SWIZZLE, addr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __test_and_set_bit_le(int nr, void *addr)
// /*AFLA*/ {
// /*AFLA*/ 	return __test_and_set_bit(nr ^ BITOP_LE_SWIZZLE, addr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __test_and_clear_bit_le(int nr, void *addr)
// /*AFLA*/ {
// /*AFLA*/ 	return __test_and_clear_bit(nr ^ BITOP_LE_SWIZZLE, addr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_GENERIC_BITOPS_LE_H_ */
