// /*AFLA*/ #ifndef __ASM_GENERIC_SIGNAL_DEFS_H
// /*AFLA*/ #define __ASM_GENERIC_SIGNAL_DEFS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef SIG_BLOCK
// /*AFLA*/ #define SIG_BLOCK          0	/* for blocking signals */
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef SIG_UNBLOCK
// /*AFLA*/ #define SIG_UNBLOCK        1	/* for unblocking signals */
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef SIG_SETMASK
// /*AFLA*/ #define SIG_SETMASK        2	/* for setting the signal mask */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ typedef void __signalfn_t(int);
// /*AFLA*/ typedef __signalfn_t __user *__sighandler_t;
// /*AFLA*/ 
// /*AFLA*/ typedef void __restorefn_t(void);
// /*AFLA*/ typedef __restorefn_t __user *__sigrestore_t;
// /*AFLA*/ 
// /*AFLA*/ #define SIG_DFL	((__force __sighandler_t)0)	/* default signal handling */
// /*AFLA*/ #define SIG_IGN	((__force __sighandler_t)1)	/* ignore signal */
// /*AFLA*/ #define SIG_ERR	((__force __sighandler_t)-1)	/* error return from signal */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASM_GENERIC_SIGNAL_DEFS_H */
