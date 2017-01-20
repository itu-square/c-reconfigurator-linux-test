/*
 * asm-generic/int-ll64.h
 *
 * Integer declarations for architectures which use "long long"
 * for 64-bit types.
 */

#ifndef _UAPI_ASM_GENERIC_INT_LL64_H
#define _UAPI_ASM_GENERIC_INT_LL64_H

#include <asm/bitsperlong.h>

#ifndef __ASSEMBLY__
/*
 * __xx is ok: it doesn't pollute the POSIX namespace. Use these in the
 * header files exported to user space
 */

typedef __signed__ char __s8;
// /*AFLA*/ typedef unsigned char __u8;
// /*AFLA*/ 
// /*AFLA*/ typedef __signed__ short __s16;
// /*AFLA*/ typedef unsigned short __u16;
// /*AFLA*/ 
// /*AFLA*/ typedef __signed__ int __s32;
// /*AFLA*/ typedef unsigned int __u32;
// /*AFLA*/ 
// /*AFLA*/ #ifdef __GNUC__
// /*AFLA*/ __extension__ typedef __signed__ long long __s64;
// /*AFLA*/ __extension__ typedef unsigned long long __u64;
// /*AFLA*/ #else
// /*AFLA*/ typedef __signed__ long long __s64;
// /*AFLA*/ typedef unsigned long long __u64;
// /*AFLA*/ #endif
// /*AFLA*/ 
#endif /* __ASSEMBLY__ */


#endif /* _UAPI_ASM_GENERIC_INT_LL64_H */
