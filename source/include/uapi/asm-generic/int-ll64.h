// /*AFLA*/ /*
// /*AFLA*/  * asm-generic/int-ll64.h
// /*AFLA*/  *
// /*AFLA*/  * Integer declarations for architectures which use "long long"
// /*AFLA*/  * for 64-bit types.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _UAPI_ASM_GENERIC_INT_LL64_H
// /*AFLA*/ #define _UAPI_ASM_GENERIC_INT_LL64_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/bitsperlong.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ /*
// /*AFLA*/  * __xx is ok: it doesn't pollute the POSIX namespace. Use these in the
// /*AFLA*/  * header files exported to user space
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ typedef __signed__ char __s8;
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
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_ASM_GENERIC_INT_LL64_H */
