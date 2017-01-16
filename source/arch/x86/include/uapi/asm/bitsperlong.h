// /*AFLA*/ #ifndef __ASM_X86_BITSPERLONG_H
// /*AFLA*/ #define __ASM_X86_BITSPERLONG_H
// /*AFLA*/ 
// /*AFLA*/ #if defined(__x86_64__) && !defined(__ILP32__)
// /*AFLA*/ # define __BITS_PER_LONG 64
// /*AFLA*/ #else
// /*AFLA*/ # define __BITS_PER_LONG 32
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/bitsperlong.h>
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASM_X86_BITSPERLONG_H */
// /*AFLA*/ 
