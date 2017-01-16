// /*AFLA*/ #ifndef _UAPI_ASM_X86_SIGNAL_H
// /*AFLA*/ #define _UAPI_ASM_X86_SIGNAL_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/time.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ 
// /*AFLA*/ /* Avoid too many header ordering problems.  */
// /*AFLA*/ struct siginfo;
// /*AFLA*/ 
// /*AFLA*/ #ifndef __KERNEL__
// /*AFLA*/ /* Here we must cater to libcs that poke about in kernel headers.  */
// /*AFLA*/ 
// /*AFLA*/ #define NSIG		32
// /*AFLA*/ typedef unsigned long sigset_t;
// /*AFLA*/ 
// /*AFLA*/ #endif /* __KERNEL__ */
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define SIGHUP		 1
// /*AFLA*/ #define SIGINT		 2
// /*AFLA*/ #define SIGQUIT		 3
// /*AFLA*/ #define SIGILL		 4
// /*AFLA*/ #define SIGTRAP		 5
// /*AFLA*/ #define SIGABRT		 6
// /*AFLA*/ #define SIGIOT		 6
// /*AFLA*/ #define SIGBUS		 7
// /*AFLA*/ #define SIGFPE		 8
// /*AFLA*/ #define SIGKILL		 9
// /*AFLA*/ #define SIGUSR1		10
// /*AFLA*/ #define SIGSEGV		11
// /*AFLA*/ #define SIGUSR2		12
// /*AFLA*/ #define SIGPIPE		13
// /*AFLA*/ #define SIGALRM		14
// /*AFLA*/ #define SIGTERM		15
// /*AFLA*/ #define SIGSTKFLT	16
// /*AFLA*/ #define SIGCHLD		17
// /*AFLA*/ #define SIGCONT		18
// /*AFLA*/ #define SIGSTOP		19
// /*AFLA*/ #define SIGTSTP		20
// /*AFLA*/ #define SIGTTIN		21
// /*AFLA*/ #define SIGTTOU		22
// /*AFLA*/ #define SIGURG		23
// /*AFLA*/ #define SIGXCPU		24
// /*AFLA*/ #define SIGXFSZ		25
// /*AFLA*/ #define SIGVTALRM	26
// /*AFLA*/ #define SIGPROF		27
// /*AFLA*/ #define SIGWINCH	28
// /*AFLA*/ #define SIGIO		29
// /*AFLA*/ #define SIGPOLL		SIGIO
// /*AFLA*/ /*
// /*AFLA*/ #define SIGLOST		29
// /*AFLA*/ */
// /*AFLA*/ #define SIGPWR		30
// /*AFLA*/ #define SIGSYS		31
// /*AFLA*/ #define	SIGUNUSED	31
// /*AFLA*/ 
// /*AFLA*/ /* These should not be considered constants from userland.  */
// /*AFLA*/ #define SIGRTMIN	32
// /*AFLA*/ #define SIGRTMAX	_NSIG
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * SA_FLAGS values:
// /*AFLA*/  *
// /*AFLA*/  * SA_ONSTACK indicates that a registered stack_t will be used.
// /*AFLA*/  * SA_RESTART flag to get restarting signals (which were the default long ago)
// /*AFLA*/  * SA_NOCLDSTOP flag to turn off SIGCHLD when children stop.
// /*AFLA*/  * SA_RESETHAND clears the handler when the signal is delivered.
// /*AFLA*/  * SA_NOCLDWAIT flag on SIGCHLD to inhibit zombies.
// /*AFLA*/  * SA_NODEFER prevents the current signal from being masked in the handler.
// /*AFLA*/  *
// /*AFLA*/  * SA_ONESHOT and SA_NOMASK are the historical Linux names for the Single
// /*AFLA*/  * Unix names RESETHAND and NODEFER respectively.
// /*AFLA*/  */
// /*AFLA*/ #define SA_NOCLDSTOP	0x00000001u
// /*AFLA*/ #define SA_NOCLDWAIT	0x00000002u
// /*AFLA*/ #define SA_SIGINFO	0x00000004u
// /*AFLA*/ #define SA_ONSTACK	0x08000000u
// /*AFLA*/ #define SA_RESTART	0x10000000u
// /*AFLA*/ #define SA_NODEFER	0x40000000u
// /*AFLA*/ #define SA_RESETHAND	0x80000000u
// /*AFLA*/ 
// /*AFLA*/ #define SA_NOMASK	SA_NODEFER
// /*AFLA*/ #define SA_ONESHOT	SA_RESETHAND
// /*AFLA*/ 
// /*AFLA*/ #define SA_RESTORER	0x04000000
// /*AFLA*/ 
// /*AFLA*/ #define MINSIGSTKSZ	2048
// /*AFLA*/ #define SIGSTKSZ	8192
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/signal-defs.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ # ifndef __KERNEL__
// /*AFLA*/ /* Here we must cater to libcs that poke about in kernel headers.  */
// /*AFLA*/ #ifdef __i386__
// /*AFLA*/ 
// /*AFLA*/ struct sigaction {
// /*AFLA*/ 	union {
// /*AFLA*/ 	  __sighandler_t _sa_handler;
// /*AFLA*/ 	  void (*_sa_sigaction)(int, struct siginfo *, void *);
// /*AFLA*/ 	} _u;
// /*AFLA*/ 	sigset_t sa_mask;
// /*AFLA*/ 	unsigned long sa_flags;
// /*AFLA*/ 	void (*sa_restorer)(void);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define sa_handler	_u._sa_handler
// /*AFLA*/ #define sa_sigaction	_u._sa_sigaction
// /*AFLA*/ 
// /*AFLA*/ #else /* __i386__ */
// /*AFLA*/ 
// /*AFLA*/ struct sigaction {
// /*AFLA*/ 	__sighandler_t sa_handler;
// /*AFLA*/ 	unsigned long sa_flags;
// /*AFLA*/ 	__sigrestore_t sa_restorer;
// /*AFLA*/ 	sigset_t sa_mask;		/* mask last for extensibility */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* !__i386__ */
// /*AFLA*/ # endif /* ! __KERNEL__ */
// /*AFLA*/ 
// /*AFLA*/ typedef struct sigaltstack {
// /*AFLA*/ 	void __user *ss_sp;
// /*AFLA*/ 	int ss_flags;
// /*AFLA*/ 	size_t ss_size;
// /*AFLA*/ } stack_t;
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_ASM_X86_SIGNAL_H */
