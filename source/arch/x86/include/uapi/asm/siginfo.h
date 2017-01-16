// /*AFLA*/ #ifndef _ASM_X86_SIGINFO_H
// /*AFLA*/ #define _ASM_X86_SIGINFO_H
// /*AFLA*/ 
// /*AFLA*/ #ifdef __x86_64__
// /*AFLA*/ # ifdef __ILP32__ /* x32 */
// /*AFLA*/ typedef long long __kernel_si_clock_t __attribute__((aligned(4)));
// /*AFLA*/ #  define __ARCH_SI_CLOCK_T		__kernel_si_clock_t
// /*AFLA*/ #  define __ARCH_SI_ATTRIBUTES		__attribute__((aligned(8)))
// /*AFLA*/ # else /* x86-64 */
// /*AFLA*/ #  define __ARCH_SI_PREAMBLE_SIZE	(4 * sizeof(int))
// /*AFLA*/ # endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/siginfo.h>
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_SIGINFO_H */
