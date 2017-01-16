// /*AFLA*/ #ifndef _ASM_X86_UNISTD_H
// /*AFLA*/ #define _ASM_X86_UNISTD_H 1
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/asm/unistd.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ # ifdef CONFIG_X86_X32_ABI
// /*AFLA*/ #  define __SYSCALL_MASK (~(__X32_SYSCALL_BIT))
// /*AFLA*/ # else
// /*AFLA*/ #  define __SYSCALL_MASK (~0)
// /*AFLA*/ # endif
// /*AFLA*/ 
// /*AFLA*/ # ifdef CONFIG_X86_32
// /*AFLA*/ 
// /*AFLA*/ #  include <asm/unistd_32.h>
// /*AFLA*/ #  define __ARCH_WANT_STAT64
// /*AFLA*/ #  define __ARCH_WANT_SYS_IPC
// /*AFLA*/ #  define __ARCH_WANT_SYS_OLD_MMAP
// /*AFLA*/ #  define __ARCH_WANT_SYS_OLD_SELECT
// /*AFLA*/ 
// /*AFLA*/ # else
// /*AFLA*/ 
// /*AFLA*/ #  include <asm/unistd_64.h>
// /*AFLA*/ #  include <asm/unistd_64_x32.h>
// /*AFLA*/ #  define __ARCH_WANT_COMPAT_SYS_TIME
// /*AFLA*/ #  define __ARCH_WANT_COMPAT_SYS_GETDENTS64
// /*AFLA*/ #  define __ARCH_WANT_COMPAT_SYS_PREADV64
// /*AFLA*/ #  define __ARCH_WANT_COMPAT_SYS_PWRITEV64
// /*AFLA*/ #  define __ARCH_WANT_COMPAT_SYS_PREADV64V2
// /*AFLA*/ #  define __ARCH_WANT_COMPAT_SYS_PWRITEV64V2
// /*AFLA*/ 
// /*AFLA*/ # endif
// /*AFLA*/ 
// /*AFLA*/ # define __ARCH_WANT_OLD_READDIR
// /*AFLA*/ # define __ARCH_WANT_OLD_STAT
// /*AFLA*/ # define __ARCH_WANT_SYS_ALARM
// /*AFLA*/ # define __ARCH_WANT_SYS_FADVISE64
// /*AFLA*/ # define __ARCH_WANT_SYS_GETHOSTNAME
// /*AFLA*/ # define __ARCH_WANT_SYS_GETPGRP
// /*AFLA*/ # define __ARCH_WANT_SYS_LLSEEK
// /*AFLA*/ # define __ARCH_WANT_SYS_NICE
// /*AFLA*/ # define __ARCH_WANT_SYS_OLDUMOUNT
// /*AFLA*/ # define __ARCH_WANT_SYS_OLD_GETRLIMIT
// /*AFLA*/ # define __ARCH_WANT_SYS_OLD_UNAME
// /*AFLA*/ # define __ARCH_WANT_SYS_PAUSE
// /*AFLA*/ # define __ARCH_WANT_SYS_SIGNAL
// /*AFLA*/ # define __ARCH_WANT_SYS_SIGPENDING
// /*AFLA*/ # define __ARCH_WANT_SYS_SIGPROCMASK
// /*AFLA*/ # define __ARCH_WANT_SYS_SOCKETCALL
// /*AFLA*/ # define __ARCH_WANT_SYS_TIME
// /*AFLA*/ # define __ARCH_WANT_SYS_UTIME
// /*AFLA*/ # define __ARCH_WANT_SYS_WAITPID
// /*AFLA*/ # define __ARCH_WANT_SYS_FORK
// /*AFLA*/ # define __ARCH_WANT_SYS_VFORK
// /*AFLA*/ # define __ARCH_WANT_SYS_CLONE
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_UNISTD_H */
