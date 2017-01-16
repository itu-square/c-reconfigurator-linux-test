// /*AFLA*/ #ifndef _UAPI_ASM_GENERIC_RESOURCE_H
// /*AFLA*/ #define _UAPI_ASM_GENERIC_RESOURCE_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Resource limit IDs
// /*AFLA*/  *
// /*AFLA*/  * ( Compatibility detail: there are architectures that have
// /*AFLA*/  *   a different rlimit ID order in the 5-9 range and want
// /*AFLA*/  *   to keep that order for binary compatibility. The reasons
// /*AFLA*/  *   are historic and all new rlimits are identical across all
// /*AFLA*/  *   arches. If an arch has such special order for some rlimits
// /*AFLA*/  *   then it defines them prior including asm-generic/resource.h. )
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define RLIMIT_CPU		0	/* CPU time in sec */
// /*AFLA*/ #define RLIMIT_FSIZE		1	/* Maximum filesize */
// /*AFLA*/ #define RLIMIT_DATA		2	/* max data size */
// /*AFLA*/ #define RLIMIT_STACK		3	/* max stack size */
// /*AFLA*/ #define RLIMIT_CORE		4	/* max core file size */
// /*AFLA*/ 
// /*AFLA*/ #ifndef RLIMIT_RSS
// /*AFLA*/ # define RLIMIT_RSS		5	/* max resident set size */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef RLIMIT_NPROC
// /*AFLA*/ # define RLIMIT_NPROC		6	/* max number of processes */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef RLIMIT_NOFILE
// /*AFLA*/ # define RLIMIT_NOFILE		7	/* max number of open files */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef RLIMIT_MEMLOCK
// /*AFLA*/ # define RLIMIT_MEMLOCK		8	/* max locked-in-memory address space */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef RLIMIT_AS
// /*AFLA*/ # define RLIMIT_AS		9	/* address space limit */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define RLIMIT_LOCKS		10	/* maximum file locks held */
// /*AFLA*/ #define RLIMIT_SIGPENDING	11	/* max number of pending signals */
// /*AFLA*/ #define RLIMIT_MSGQUEUE		12	/* maximum bytes in POSIX mqueues */
// /*AFLA*/ #define RLIMIT_NICE		13	/* max nice prio allowed to raise to
// /*AFLA*/ 					   0-39 for nice level 19 .. -20 */
// /*AFLA*/ #define RLIMIT_RTPRIO		14	/* maximum realtime priority */
// /*AFLA*/ #define RLIMIT_RTTIME		15	/* timeout for RT tasks in us */
// /*AFLA*/ #define RLIM_NLIMITS		16
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * SuS says limits have to be unsigned.
// /*AFLA*/  * Which makes a ton more sense anyway.
// /*AFLA*/  *
// /*AFLA*/  * Some architectures override this (for compatibility reasons):
// /*AFLA*/  */
// /*AFLA*/ #ifndef RLIM_INFINITY
// /*AFLA*/ # define RLIM_INFINITY		(~0UL)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_ASM_GENERIC_RESOURCE_H */
