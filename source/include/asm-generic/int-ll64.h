/*
 * asm-generic/int-ll64.h
 *
 * Integer declarations for architectures which use "long long"
 * for 64-bit types.
 */
#ifndef _ASM_GENERIC_INT_LL64_H
#define _ASM_GENERIC_INT_LL64_H

#include <uapi/asm-generic/int-ll64.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ typedef signed char s8;
// /*AFLA*/ typedef unsigned char u8;
// /*AFLA*/ 
// /*AFLA*/ typedef signed short s16;
// /*AFLA*/ typedef unsigned short u16;
// /*AFLA*/ 
// /*AFLA*/ typedef signed int s32;
// /*AFLA*/ typedef unsigned int u32;
// /*AFLA*/ 
// /*AFLA*/ typedef signed long long s64;
// /*AFLA*/ typedef unsigned long long u64;
// /*AFLA*/ 
// /*AFLA*/ #define S8_C(x)  x
// /*AFLA*/ #define U8_C(x)  x ## U
// /*AFLA*/ #define S16_C(x) x
// /*AFLA*/ #define U16_C(x) x ## U
// /*AFLA*/ #define S32_C(x) x
// /*AFLA*/ #define U32_C(x) x ## U
// /*AFLA*/ #define S64_C(x) x ## LL
// /*AFLA*/ #define U64_C(x) x ## ULL
// /*AFLA*/ 
// /*AFLA*/ #else /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #define S8_C(x)  x
// /*AFLA*/ #define U8_C(x)  x
// /*AFLA*/ #define S16_C(x) x
// /*AFLA*/ #define U16_C(x) x
// /*AFLA*/ #define S32_C(x) x
// /*AFLA*/ #define U32_C(x) x
// /*AFLA*/ #define S64_C(x) x
// /*AFLA*/ #define U64_C(x) x
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
#endif /* _ASM_GENERIC_INT_LL64_H */
