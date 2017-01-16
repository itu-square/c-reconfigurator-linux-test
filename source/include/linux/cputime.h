// /*AFLA*/ #ifndef __LINUX_CPUTIME_H
// /*AFLA*/ #define __LINUX_CPUTIME_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/cputime.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef cputime_to_nsecs
// /*AFLA*/ # define cputime_to_nsecs(__ct)	\
// /*AFLA*/ 	(cputime_to_usecs(__ct) * NSEC_PER_USEC)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef nsecs_to_cputime
// /*AFLA*/ # define nsecs_to_cputime(__nsecs)	\
// /*AFLA*/ 	usecs_to_cputime((__nsecs) / NSEC_PER_USEC)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_CPUTIME_H */
