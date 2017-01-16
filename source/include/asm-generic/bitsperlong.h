// /*AFLA*/ #ifndef __ASM_GENERIC_BITS_PER_LONG
// /*AFLA*/ #define __ASM_GENERIC_BITS_PER_LONG
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/asm-generic/bitsperlong.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_64BIT
// /*AFLA*/ #define BITS_PER_LONG 64
// /*AFLA*/ #else
// /*AFLA*/ #define BITS_PER_LONG 32
// /*AFLA*/ #endif /* CONFIG_64BIT */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * FIXME: The check currently breaks x86-64 build, so it's
// /*AFLA*/  * temporarily disabled. Please fix x86-64 and reenable
// /*AFLA*/  */
// /*AFLA*/ #if 0 && BITS_PER_LONG != __BITS_PER_LONG
// /*AFLA*/ #error Inconsistent word size. Check asm/bitsperlong.h
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef BITS_PER_LONG_LONG
// /*AFLA*/ #define BITS_PER_LONG_LONG 64
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASM_GENERIC_BITS_PER_LONG */
