// /*AFLA*/ #ifndef _ASM_GENERIC_SIGINFO_H
// /*AFLA*/ #define _ASM_GENERIC_SIGINFO_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/asm-generic/siginfo.h>
// /*AFLA*/ 
// /*AFLA*/ #define __SI_MASK	0xffff0000u
// /*AFLA*/ #define __SI_KILL	(0 << 16)
// /*AFLA*/ #define __SI_TIMER	(1 << 16)
// /*AFLA*/ #define __SI_POLL	(2 << 16)
// /*AFLA*/ #define __SI_FAULT	(3 << 16)
// /*AFLA*/ #define __SI_CHLD	(4 << 16)
// /*AFLA*/ #define __SI_RT		(5 << 16)
// /*AFLA*/ #define __SI_MESGQ	(6 << 16)
// /*AFLA*/ #define __SI_SYS	(7 << 16)
// /*AFLA*/ #define __SI_CODE(T,N)	((T) | ((N) & 0xffff))
// /*AFLA*/ 
// /*AFLA*/ struct siginfo;
// /*AFLA*/ void do_schedule_next_timer(struct siginfo *info);
// /*AFLA*/ 
// /*AFLA*/ extern int copy_siginfo_to_user(struct siginfo __user *to, const struct siginfo *from);
// /*AFLA*/ 
// /*AFLA*/ #endif
