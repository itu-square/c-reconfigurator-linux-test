// /*AFLA*/ #ifndef _UAPI_LINUX_SIGNAL_H
// /*AFLA*/ #define _UAPI_LINUX_SIGNAL_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/signal.h>
// /*AFLA*/ #include <asm/siginfo.h>
// /*AFLA*/ 
// /*AFLA*/ #define SS_ONSTACK	1
// /*AFLA*/ #define SS_DISABLE	2
// /*AFLA*/ 
// /*AFLA*/ /* bit-flags */
// /*AFLA*/ #define SS_AUTODISARM	(1U << 31)	/* disable sas during sighandling */
// /*AFLA*/ /* mask for all SS_xxx flags */
// /*AFLA*/ #define SS_FLAG_BITS	SS_AUTODISARM
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_SIGNAL_H */
