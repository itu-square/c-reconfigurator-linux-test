// /*AFLA*/ /*
// /*AFLA*/  * include/asm-generic/seccomp.h
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2014 Linaro Limited
// /*AFLA*/  * Author: AKASHI Takahiro <takahiro.akashi@linaro.org>
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify
// /*AFLA*/  * it under the terms of the GNU General Public License version 2 as
// /*AFLA*/  * published by the Free Software Foundation.
// /*AFLA*/  */
// /*AFLA*/ #ifndef _ASM_GENERIC_SECCOMP_H
// /*AFLA*/ #define _ASM_GENERIC_SECCOMP_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/unistd.h>
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_COMPAT) && !defined(__NR_seccomp_read_32)
// /*AFLA*/ #define __NR_seccomp_read_32		__NR_read
// /*AFLA*/ #define __NR_seccomp_write_32		__NR_write
// /*AFLA*/ #define __NR_seccomp_exit_32		__NR_exit
// /*AFLA*/ #ifndef __NR_seccomp_sigreturn_32
// /*AFLA*/ #define __NR_seccomp_sigreturn_32	__NR_rt_sigreturn
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* CONFIG_COMPAT && ! already defined */
// /*AFLA*/ 
// /*AFLA*/ #define __NR_seccomp_read		__NR_read
// /*AFLA*/ #define __NR_seccomp_write		__NR_write
// /*AFLA*/ #define __NR_seccomp_exit		__NR_exit
// /*AFLA*/ #ifndef __NR_seccomp_sigreturn
// /*AFLA*/ #define __NR_seccomp_sigreturn		__NR_rt_sigreturn
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_COMPAT
// /*AFLA*/ #ifndef get_compat_mode1_syscalls
// /*AFLA*/ static inline const int *get_compat_mode1_syscalls(void)
// /*AFLA*/ {
// /*AFLA*/ 	static const int mode1_syscalls_32[] = {
// /*AFLA*/ 		__NR_seccomp_read_32, __NR_seccomp_write_32,
// /*AFLA*/ 		__NR_seccomp_exit_32, __NR_seccomp_sigreturn_32,
// /*AFLA*/ 		0, /* null terminated */
// /*AFLA*/ 	};
// /*AFLA*/ 	return mode1_syscalls_32;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* CONFIG_COMPAT */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_GENERIC_SECCOMP_H */
