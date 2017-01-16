// /*AFLA*/ #ifndef _ASM_GENERIC_BITOPS_EXT2_ATOMIC_SETBIT_H_
// /*AFLA*/ #define _ASM_GENERIC_BITOPS_EXT2_ATOMIC_SETBIT_H_
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Atomic bitops based version of ext2 atomic bitops
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define ext2_set_bit_atomic(l, nr, addr)	test_and_set_bit_le(nr, addr)
// /*AFLA*/ #define ext2_clear_bit_atomic(l, nr, addr)	test_and_clear_bit_le(nr, addr)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_GENERIC_BITOPS_EXT2_ATOMIC_SETBIT_H_ */
