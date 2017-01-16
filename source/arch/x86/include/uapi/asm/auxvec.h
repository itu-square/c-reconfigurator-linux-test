// /*AFLA*/ #ifndef _ASM_X86_AUXVEC_H
// /*AFLA*/ #define _ASM_X86_AUXVEC_H
// /*AFLA*/ /*
// /*AFLA*/  * Architecture-neutral AT_ values in 0-17, leave some room
// /*AFLA*/  * for more of them, start the x86-specific ones at 32.
// /*AFLA*/  */
// /*AFLA*/ #ifdef __i386__
// /*AFLA*/ #define AT_SYSINFO		32
// /*AFLA*/ #endif
// /*AFLA*/ #define AT_SYSINFO_EHDR		33
// /*AFLA*/ 
// /*AFLA*/ /* entries in ARCH_DLINFO: */
// /*AFLA*/ #if defined(CONFIG_IA32_EMULATION) || !defined(CONFIG_X86_64)
// /*AFLA*/ # define AT_VECTOR_SIZE_ARCH 2
// /*AFLA*/ #else /* else it's non-compat x86-64 */
// /*AFLA*/ # define AT_VECTOR_SIZE_ARCH 1
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_AUXVEC_H */
