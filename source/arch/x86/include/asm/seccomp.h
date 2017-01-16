// /*AFLA*/ #ifndef _ASM_X86_SECCOMP_H
// /*AFLA*/ #define _ASM_X86_SECCOMP_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/unistd.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ #define __NR_seccomp_sigreturn		__NR_sigreturn
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_COMPAT
// /*AFLA*/ #include <asm/ia32_unistd.h>
// /*AFLA*/ #define __NR_seccomp_read_32		__NR_ia32_read
// /*AFLA*/ #define __NR_seccomp_write_32		__NR_ia32_write
// /*AFLA*/ #define __NR_seccomp_exit_32		__NR_ia32_exit
// /*AFLA*/ #define __NR_seccomp_sigreturn_32	__NR_ia32_sigreturn
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/seccomp.h>
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_SECCOMP_H */
