// /*AFLA*/ #ifndef _UAPI_LINUX_RESOURCE_H
// /*AFLA*/ #define _UAPI_LINUX_RESOURCE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/time.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Resource control/accounting header file for linux
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Definition of struct rusage taken from BSD 4.3 Reno
// /*AFLA*/  * 
// /*AFLA*/  * We don't support all of these yet, but we might as well have them....
// /*AFLA*/  * Otherwise, each time we add new items, programs which depend on this
// /*AFLA*/  * structure will lose.  This reduces the chances of that happening.
// /*AFLA*/  */
// /*AFLA*/ #define	RUSAGE_SELF	0
// /*AFLA*/ #define	RUSAGE_CHILDREN	(-1)
// /*AFLA*/ #define RUSAGE_BOTH	(-2)		/* sys_wait4() uses this */
// /*AFLA*/ #define	RUSAGE_THREAD	1		/* only the calling thread */
// /*AFLA*/ 
// /*AFLA*/ struct	rusage {
// /*AFLA*/ 	struct timeval ru_utime;	/* user time used */
// /*AFLA*/ 	struct timeval ru_stime;	/* system time used */
// /*AFLA*/ 	__kernel_long_t	ru_maxrss;	/* maximum resident set size */
// /*AFLA*/ 	__kernel_long_t	ru_ixrss;	/* integral shared memory size */
// /*AFLA*/ 	__kernel_long_t	ru_idrss;	/* integral unshared data size */
// /*AFLA*/ 	__kernel_long_t	ru_isrss;	/* integral unshared stack size */
// /*AFLA*/ 	__kernel_long_t	ru_minflt;	/* page reclaims */
// /*AFLA*/ 	__kernel_long_t	ru_majflt;	/* page faults */
// /*AFLA*/ 	__kernel_long_t	ru_nswap;	/* swaps */
// /*AFLA*/ 	__kernel_long_t	ru_inblock;	/* block input operations */
// /*AFLA*/ 	__kernel_long_t	ru_oublock;	/* block output operations */
// /*AFLA*/ 	__kernel_long_t	ru_msgsnd;	/* messages sent */
// /*AFLA*/ 	__kernel_long_t	ru_msgrcv;	/* messages received */
// /*AFLA*/ 	__kernel_long_t	ru_nsignals;	/* signals received */
// /*AFLA*/ 	__kernel_long_t	ru_nvcsw;	/* voluntary context switches */
// /*AFLA*/ 	__kernel_long_t	ru_nivcsw;	/* involuntary " */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct rlimit {
// /*AFLA*/ 	__kernel_ulong_t	rlim_cur;
// /*AFLA*/ 	__kernel_ulong_t	rlim_max;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define RLIM64_INFINITY		(~0ULL)
// /*AFLA*/ 
// /*AFLA*/ struct rlimit64 {
// /*AFLA*/ 	__u64 rlim_cur;
// /*AFLA*/ 	__u64 rlim_max;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define	PRIO_MIN	(-20)
// /*AFLA*/ #define	PRIO_MAX	20
// /*AFLA*/ 
// /*AFLA*/ #define	PRIO_PROCESS	0
// /*AFLA*/ #define	PRIO_PGRP	1
// /*AFLA*/ #define	PRIO_USER	2
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Limit the stack by to some sane default: root can always
// /*AFLA*/  * increase this limit if needed..  8MB seems reasonable.
// /*AFLA*/  */
// /*AFLA*/ #define _STK_LIM	(8*1024*1024)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * GPG2 wants 64kB of mlocked memory, to make sure pass phrases
// /*AFLA*/  * and other sensitive information are never written to disk.
// /*AFLA*/  */
// /*AFLA*/ #define MLOCK_LIMIT	((PAGE_SIZE > 64*1024) ? PAGE_SIZE : 64*1024)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Due to binary compatibility, the actual resource numbers
// /*AFLA*/  * may be different for different linux versions..
// /*AFLA*/  */
// /*AFLA*/ #include <asm/resource.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_RESOURCE_H */
