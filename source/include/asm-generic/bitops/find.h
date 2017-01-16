// /*AFLA*/ #ifndef _ASM_GENERIC_BITOPS_FIND_H_
// /*AFLA*/ #define _ASM_GENERIC_BITOPS_FIND_H_
// /*AFLA*/ 
// /*AFLA*/ #ifndef find_next_bit
// /*AFLA*/ /**
// /*AFLA*/  * find_next_bit - find the next set bit in a memory region
// /*AFLA*/  * @addr: The address to base the search on
// /*AFLA*/  * @offset: The bitnumber to start searching at
// /*AFLA*/  * @size: The bitmap size in bits
// /*AFLA*/  *
// /*AFLA*/  * Returns the bit number for the next set bit
// /*AFLA*/  * If no bits are set, returns @size.
// /*AFLA*/  */
// /*AFLA*/ extern unsigned long find_next_bit(const unsigned long *addr, unsigned long
// /*AFLA*/ 		size, unsigned long offset);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef find_next_zero_bit
// /*AFLA*/ /**
// /*AFLA*/  * find_next_zero_bit - find the next cleared bit in a memory region
// /*AFLA*/  * @addr: The address to base the search on
// /*AFLA*/  * @offset: The bitnumber to start searching at
// /*AFLA*/  * @size: The bitmap size in bits
// /*AFLA*/  *
// /*AFLA*/  * Returns the bit number of the next zero bit
// /*AFLA*/  * If no bits are zero, returns @size.
// /*AFLA*/  */
// /*AFLA*/ extern unsigned long find_next_zero_bit(const unsigned long *addr, unsigned
// /*AFLA*/ 		long size, unsigned long offset);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_GENERIC_FIND_FIRST_BIT
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * find_first_bit - find the first set bit in a memory region
// /*AFLA*/  * @addr: The address to start the search at
// /*AFLA*/  * @size: The maximum number of bits to search
// /*AFLA*/  *
// /*AFLA*/  * Returns the bit number of the first set bit.
// /*AFLA*/  * If no bits are set, returns @size.
// /*AFLA*/  */
// /*AFLA*/ extern unsigned long find_first_bit(const unsigned long *addr,
// /*AFLA*/ 				    unsigned long size);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * find_first_zero_bit - find the first cleared bit in a memory region
// /*AFLA*/  * @addr: The address to start the search at
// /*AFLA*/  * @size: The maximum number of bits to search
// /*AFLA*/  *
// /*AFLA*/  * Returns the bit number of the first cleared bit.
// /*AFLA*/  * If no bits are zero, returns @size.
// /*AFLA*/  */
// /*AFLA*/ extern unsigned long find_first_zero_bit(const unsigned long *addr,
// /*AFLA*/ 					 unsigned long size);
// /*AFLA*/ #else /* CONFIG_GENERIC_FIND_FIRST_BIT */
// /*AFLA*/ 
// /*AFLA*/ #define find_first_bit(addr, size) find_next_bit((addr), (size), 0)
// /*AFLA*/ #define find_first_zero_bit(addr, size) find_next_zero_bit((addr), (size), 0)
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_GENERIC_FIND_FIRST_BIT */
// /*AFLA*/ 
// /*AFLA*/ #endif /*_ASM_GENERIC_BITOPS_FIND_H_ */
