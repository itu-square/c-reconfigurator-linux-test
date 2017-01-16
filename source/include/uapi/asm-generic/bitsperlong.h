// /*AFLA*/ #ifndef _UAPI__ASM_GENERIC_BITS_PER_LONG
// /*AFLA*/ #define _UAPI__ASM_GENERIC_BITS_PER_LONG
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * There seems to be no way of detecting this automatically from user
// /*AFLA*/  * space, so 64 bit architectures should override this in their
// /*AFLA*/  * bitsperlong.h. In particular, an architecture that supports
// /*AFLA*/  * both 32 and 64 bit user space must not rely on CONFIG_64BIT
// /*AFLA*/  * to decide it, but rather check a compiler provided macro.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __BITS_PER_LONG
// /*AFLA*/ #define __BITS_PER_LONG 32
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI__ASM_GENERIC_BITS_PER_LONG */
