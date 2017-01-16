// /*AFLA*/ #ifndef _LINUX_ERRNO_H
// /*AFLA*/ #define _LINUX_ERRNO_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/errno.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These should never be seen by user programs.  To return one of ERESTART*
// /*AFLA*/  * codes, signal_pending() MUST be set.  Note that ptrace can observe these
// /*AFLA*/  * at syscall exit tracing, but they will never be left for the debugged user
// /*AFLA*/  * process to see.
// /*AFLA*/  */
// /*AFLA*/ #define ERESTARTSYS	512
// /*AFLA*/ #define ERESTARTNOINTR	513
// /*AFLA*/ #define ERESTARTNOHAND	514	/* restart if no handler.. */
// /*AFLA*/ #define ENOIOCTLCMD	515	/* No ioctl command */
// /*AFLA*/ #define ERESTART_RESTARTBLOCK 516 /* restart by calling sys_restart_syscall */
// /*AFLA*/ #define EPROBE_DEFER	517	/* Driver requests probe retry */
// /*AFLA*/ #define EOPENSTALE	518	/* open found a stale dentry */
// /*AFLA*/ 
// /*AFLA*/ /* Defined for the NFSv3 protocol */
// /*AFLA*/ #define EBADHANDLE	521	/* Illegal NFS file handle */
// /*AFLA*/ #define ENOTSYNC	522	/* Update synchronization mismatch */
// /*AFLA*/ #define EBADCOOKIE	523	/* Cookie is stale */
// /*AFLA*/ #define ENOTSUPP	524	/* Operation is not supported */
// /*AFLA*/ #define ETOOSMALL	525	/* Buffer or request is too small */
// /*AFLA*/ #define ESERVERFAULT	526	/* An untranslatable error occurred */
// /*AFLA*/ #define EBADTYPE	527	/* Type not supported by server */
// /*AFLA*/ #define EJUKEBOX	528	/* Request initiated, but will not complete before timeout */
// /*AFLA*/ #define EIOCBQUEUED	529	/* iocb queued, will get completion event */
// /*AFLA*/ #define ERECALLCONFLICT	530	/* conflict with recalled state */
// /*AFLA*/ 
// /*AFLA*/ #endif
