// /*AFLA*/ #ifndef _UAPI_ASM_GENERIC_SIGINFO_H
// /*AFLA*/ #define _UAPI_ASM_GENERIC_SIGINFO_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ typedef union sigval {
// /*AFLA*/ 	int sival_int;
// /*AFLA*/ 	void __user *sival_ptr;
// /*AFLA*/ } sigval_t;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is the size (including padding) of the part of the
// /*AFLA*/  * struct siginfo that is before the union.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __ARCH_SI_PREAMBLE_SIZE
// /*AFLA*/ #define __ARCH_SI_PREAMBLE_SIZE	(3 * sizeof(int))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define SI_MAX_SIZE	128
// /*AFLA*/ #ifndef SI_PAD_SIZE
// /*AFLA*/ #define SI_PAD_SIZE	((SI_MAX_SIZE - __ARCH_SI_PREAMBLE_SIZE) / sizeof(int))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ARCH_SI_UID_T
// /*AFLA*/ #define __ARCH_SI_UID_T	__kernel_uid32_t
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The default "si_band" type is "long", as specified by POSIX.
// /*AFLA*/  * However, some architectures want to override this to "int"
// /*AFLA*/  * for historical compatibility reasons, so we allow that.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __ARCH_SI_BAND_T
// /*AFLA*/ #define __ARCH_SI_BAND_T long
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ARCH_SI_CLOCK_T
// /*AFLA*/ #define __ARCH_SI_CLOCK_T __kernel_clock_t
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ARCH_SI_ATTRIBUTES
// /*AFLA*/ #define __ARCH_SI_ATTRIBUTES
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef HAVE_ARCH_SIGINFO_T
// /*AFLA*/ 
// /*AFLA*/ typedef struct siginfo {
// /*AFLA*/ 	int si_signo;
// /*AFLA*/ 	int si_errno;
// /*AFLA*/ 	int si_code;
// /*AFLA*/ 
// /*AFLA*/ 	union {
// /*AFLA*/ 		int _pad[SI_PAD_SIZE];
// /*AFLA*/ 
// /*AFLA*/ 		/* kill() */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__kernel_pid_t _pid;	/* sender's pid */
// /*AFLA*/ 			__ARCH_SI_UID_T _uid;	/* sender's uid */
// /*AFLA*/ 		} _kill;
// /*AFLA*/ 
// /*AFLA*/ 		/* POSIX.1b timers */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__kernel_timer_t _tid;	/* timer id */
// /*AFLA*/ 			int _overrun;		/* overrun count */
// /*AFLA*/ 			char _pad[sizeof( __ARCH_SI_UID_T) - sizeof(int)];
// /*AFLA*/ 			sigval_t _sigval;	/* same as below */
// /*AFLA*/ 			int _sys_private;       /* not to be passed to user */
// /*AFLA*/ 		} _timer;
// /*AFLA*/ 
// /*AFLA*/ 		/* POSIX.1b signals */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__kernel_pid_t _pid;	/* sender's pid */
// /*AFLA*/ 			__ARCH_SI_UID_T _uid;	/* sender's uid */
// /*AFLA*/ 			sigval_t _sigval;
// /*AFLA*/ 		} _rt;
// /*AFLA*/ 
// /*AFLA*/ 		/* SIGCHLD */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__kernel_pid_t _pid;	/* which child */
// /*AFLA*/ 			__ARCH_SI_UID_T _uid;	/* sender's uid */
// /*AFLA*/ 			int _status;		/* exit code */
// /*AFLA*/ 			__ARCH_SI_CLOCK_T _utime;
// /*AFLA*/ 			__ARCH_SI_CLOCK_T _stime;
// /*AFLA*/ 		} _sigchld;
// /*AFLA*/ 
// /*AFLA*/ 		/* SIGILL, SIGFPE, SIGSEGV, SIGBUS */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			void __user *_addr; /* faulting insn/memory ref. */
// /*AFLA*/ #ifdef __ARCH_SI_TRAPNO
// /*AFLA*/ 			int _trapno;	/* TRAP # which caused the signal */
// /*AFLA*/ #endif
// /*AFLA*/ 			short _addr_lsb; /* LSB of the reported address */
// /*AFLA*/ 			union {
// /*AFLA*/ 				/* used when si_code=SEGV_BNDERR */
// /*AFLA*/ 				struct {
// /*AFLA*/ 					void __user *_lower;
// /*AFLA*/ 					void __user *_upper;
// /*AFLA*/ 				} _addr_bnd;
// /*AFLA*/ 				/* used when si_code=SEGV_PKUERR */
// /*AFLA*/ 				__u32 _pkey;
// /*AFLA*/ 			};
// /*AFLA*/ 		} _sigfault;
// /*AFLA*/ 
// /*AFLA*/ 		/* SIGPOLL */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__ARCH_SI_BAND_T _band;	/* POLL_IN, POLL_OUT, POLL_MSG */
// /*AFLA*/ 			int _fd;
// /*AFLA*/ 		} _sigpoll;
// /*AFLA*/ 
// /*AFLA*/ 		/* SIGSYS */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			void __user *_call_addr; /* calling user insn */
// /*AFLA*/ 			int _syscall;	/* triggering system call number */
// /*AFLA*/ 			unsigned int _arch;	/* AUDIT_ARCH_* of syscall */
// /*AFLA*/ 		} _sigsys;
// /*AFLA*/ 	} _sifields;
// /*AFLA*/ } __ARCH_SI_ATTRIBUTES siginfo_t;
// /*AFLA*/ 
// /*AFLA*/ /* If the arch shares siginfo, then it has SIGSYS. */
// /*AFLA*/ #define __ARCH_SIGSYS
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * How these fields are to be accessed.
// /*AFLA*/  */
// /*AFLA*/ #define si_pid		_sifields._kill._pid
// /*AFLA*/ #define si_uid		_sifields._kill._uid
// /*AFLA*/ #define si_tid		_sifields._timer._tid
// /*AFLA*/ #define si_overrun	_sifields._timer._overrun
// /*AFLA*/ #define si_sys_private  _sifields._timer._sys_private
// /*AFLA*/ #define si_status	_sifields._sigchld._status
// /*AFLA*/ #define si_utime	_sifields._sigchld._utime
// /*AFLA*/ #define si_stime	_sifields._sigchld._stime
// /*AFLA*/ #define si_value	_sifields._rt._sigval
// /*AFLA*/ #define si_int		_sifields._rt._sigval.sival_int
// /*AFLA*/ #define si_ptr		_sifields._rt._sigval.sival_ptr
// /*AFLA*/ #define si_addr		_sifields._sigfault._addr
// /*AFLA*/ #ifdef __ARCH_SI_TRAPNO
// /*AFLA*/ #define si_trapno	_sifields._sigfault._trapno
// /*AFLA*/ #endif
// /*AFLA*/ #define si_addr_lsb	_sifields._sigfault._addr_lsb
// /*AFLA*/ #define si_lower	_sifields._sigfault._addr_bnd._lower
// /*AFLA*/ #define si_upper	_sifields._sigfault._addr_bnd._upper
// /*AFLA*/ #define si_pkey		_sifields._sigfault._pkey
// /*AFLA*/ #define si_band		_sifields._sigpoll._band
// /*AFLA*/ #define si_fd		_sifields._sigpoll._fd
// /*AFLA*/ #ifdef __ARCH_SIGSYS
// /*AFLA*/ #define si_call_addr	_sifields._sigsys._call_addr
// /*AFLA*/ #define si_syscall	_sifields._sigsys._syscall
// /*AFLA*/ #define si_arch		_sifields._sigsys._arch
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __KERNEL__
// /*AFLA*/ #define __SI_KILL	0
// /*AFLA*/ #define __SI_TIMER	0
// /*AFLA*/ #define __SI_POLL	0
// /*AFLA*/ #define __SI_FAULT	0
// /*AFLA*/ #define __SI_CHLD	0
// /*AFLA*/ #define __SI_RT		0
// /*AFLA*/ #define __SI_MESGQ	0
// /*AFLA*/ #define __SI_SYS	0
// /*AFLA*/ #define __SI_CODE(T,N)	(N)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * si_code values
// /*AFLA*/  * Digital reserves positive values for kernel-generated signals.
// /*AFLA*/  */
// /*AFLA*/ #define SI_USER		0		/* sent by kill, sigsend, raise */
// /*AFLA*/ #define SI_KERNEL	0x80		/* sent by the kernel from somewhere */
// /*AFLA*/ #define SI_QUEUE	-1		/* sent by sigqueue */
// /*AFLA*/ #define SI_TIMER __SI_CODE(__SI_TIMER,-2) /* sent by timer expiration */
// /*AFLA*/ #define SI_MESGQ __SI_CODE(__SI_MESGQ,-3) /* sent by real time mesq state change */
// /*AFLA*/ #define SI_ASYNCIO	-4		/* sent by AIO completion */
// /*AFLA*/ #define SI_SIGIO	-5		/* sent by queued SIGIO */
// /*AFLA*/ #define SI_TKILL	-6		/* sent by tkill system call */
// /*AFLA*/ #define SI_DETHREAD	-7		/* sent by execve() killing subsidiary threads */
// /*AFLA*/ 
// /*AFLA*/ #define SI_FROMUSER(siptr)	((siptr)->si_code <= 0)
// /*AFLA*/ #define SI_FROMKERNEL(siptr)	((siptr)->si_code > 0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * SIGILL si_codes
// /*AFLA*/  */
// /*AFLA*/ #define ILL_ILLOPC	(__SI_FAULT|1)	/* illegal opcode */
// /*AFLA*/ #define ILL_ILLOPN	(__SI_FAULT|2)	/* illegal operand */
// /*AFLA*/ #define ILL_ILLADR	(__SI_FAULT|3)	/* illegal addressing mode */
// /*AFLA*/ #define ILL_ILLTRP	(__SI_FAULT|4)	/* illegal trap */
// /*AFLA*/ #define ILL_PRVOPC	(__SI_FAULT|5)	/* privileged opcode */
// /*AFLA*/ #define ILL_PRVREG	(__SI_FAULT|6)	/* privileged register */
// /*AFLA*/ #define ILL_COPROC	(__SI_FAULT|7)	/* coprocessor error */
// /*AFLA*/ #define ILL_BADSTK	(__SI_FAULT|8)	/* internal stack error */
// /*AFLA*/ #define NSIGILL		8
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * SIGFPE si_codes
// /*AFLA*/  */
// /*AFLA*/ #define FPE_INTDIV	(__SI_FAULT|1)	/* integer divide by zero */
// /*AFLA*/ #define FPE_INTOVF	(__SI_FAULT|2)	/* integer overflow */
// /*AFLA*/ #define FPE_FLTDIV	(__SI_FAULT|3)	/* floating point divide by zero */
// /*AFLA*/ #define FPE_FLTOVF	(__SI_FAULT|4)	/* floating point overflow */
// /*AFLA*/ #define FPE_FLTUND	(__SI_FAULT|5)	/* floating point underflow */
// /*AFLA*/ #define FPE_FLTRES	(__SI_FAULT|6)	/* floating point inexact result */
// /*AFLA*/ #define FPE_FLTINV	(__SI_FAULT|7)	/* floating point invalid operation */
// /*AFLA*/ #define FPE_FLTSUB	(__SI_FAULT|8)	/* subscript out of range */
// /*AFLA*/ #define NSIGFPE		8
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * SIGSEGV si_codes
// /*AFLA*/  */
// /*AFLA*/ #define SEGV_MAPERR	(__SI_FAULT|1)	/* address not mapped to object */
// /*AFLA*/ #define SEGV_ACCERR	(__SI_FAULT|2)	/* invalid permissions for mapped object */
// /*AFLA*/ #define SEGV_BNDERR	(__SI_FAULT|3)  /* failed address bound checks */
// /*AFLA*/ #define SEGV_PKUERR	(__SI_FAULT|4)  /* failed protection key checks */
// /*AFLA*/ #define NSIGSEGV	4
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * SIGBUS si_codes
// /*AFLA*/  */
// /*AFLA*/ #define BUS_ADRALN	(__SI_FAULT|1)	/* invalid address alignment */
// /*AFLA*/ #define BUS_ADRERR	(__SI_FAULT|2)	/* non-existent physical address */
// /*AFLA*/ #define BUS_OBJERR	(__SI_FAULT|3)	/* object specific hardware error */
// /*AFLA*/ /* hardware memory error consumed on a machine check: action required */
// /*AFLA*/ #define BUS_MCEERR_AR	(__SI_FAULT|4)
// /*AFLA*/ /* hardware memory error detected in process but not consumed: action optional*/
// /*AFLA*/ #define BUS_MCEERR_AO	(__SI_FAULT|5)
// /*AFLA*/ #define NSIGBUS		5
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * SIGTRAP si_codes
// /*AFLA*/  */
// /*AFLA*/ #define TRAP_BRKPT	(__SI_FAULT|1)	/* process breakpoint */
// /*AFLA*/ #define TRAP_TRACE	(__SI_FAULT|2)	/* process trace trap */
// /*AFLA*/ #define TRAP_BRANCH     (__SI_FAULT|3)  /* process taken branch trap */
// /*AFLA*/ #define TRAP_HWBKPT     (__SI_FAULT|4)  /* hardware breakpoint/watchpoint */
// /*AFLA*/ #define NSIGTRAP	4
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * SIGCHLD si_codes
// /*AFLA*/  */
// /*AFLA*/ #define CLD_EXITED	(__SI_CHLD|1)	/* child has exited */
// /*AFLA*/ #define CLD_KILLED	(__SI_CHLD|2)	/* child was killed */
// /*AFLA*/ #define CLD_DUMPED	(__SI_CHLD|3)	/* child terminated abnormally */
// /*AFLA*/ #define CLD_TRAPPED	(__SI_CHLD|4)	/* traced child has trapped */
// /*AFLA*/ #define CLD_STOPPED	(__SI_CHLD|5)	/* child has stopped */
// /*AFLA*/ #define CLD_CONTINUED	(__SI_CHLD|6)	/* stopped child has continued */
// /*AFLA*/ #define NSIGCHLD	6
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * SIGPOLL si_codes
// /*AFLA*/  */
// /*AFLA*/ #define POLL_IN		(__SI_POLL|1)	/* data input available */
// /*AFLA*/ #define POLL_OUT	(__SI_POLL|2)	/* output buffers available */
// /*AFLA*/ #define POLL_MSG	(__SI_POLL|3)	/* input message available */
// /*AFLA*/ #define POLL_ERR	(__SI_POLL|4)	/* i/o error */
// /*AFLA*/ #define POLL_PRI	(__SI_POLL|5)	/* high priority input available */
// /*AFLA*/ #define POLL_HUP	(__SI_POLL|6)	/* device disconnected */
// /*AFLA*/ #define NSIGPOLL	6
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * SIGSYS si_codes
// /*AFLA*/  */
// /*AFLA*/ #define SYS_SECCOMP		(__SI_SYS|1)	/* seccomp triggered */
// /*AFLA*/ #define NSIGSYS	1
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * sigevent definitions
// /*AFLA*/  * 
// /*AFLA*/  * It seems likely that SIGEV_THREAD will have to be handled from 
// /*AFLA*/  * userspace, libpthread transmuting it to SIGEV_SIGNAL, which the
// /*AFLA*/  * thread manager then catches and does the appropriate nonsense.
// /*AFLA*/  * However, everything is written out here so as to not get lost.
// /*AFLA*/  */
// /*AFLA*/ #define SIGEV_SIGNAL	0	/* notify via signal */
// /*AFLA*/ #define SIGEV_NONE	1	/* other notification: meaningless */
// /*AFLA*/ #define SIGEV_THREAD	2	/* deliver via thread creation */
// /*AFLA*/ #define SIGEV_THREAD_ID 4	/* deliver to thread */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This works because the alignment is ok on all current architectures
// /*AFLA*/  * but we leave open this being overridden in the future
// /*AFLA*/  */
// /*AFLA*/ #ifndef __ARCH_SIGEV_PREAMBLE_SIZE
// /*AFLA*/ #define __ARCH_SIGEV_PREAMBLE_SIZE	(sizeof(int) * 2 + sizeof(sigval_t))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define SIGEV_MAX_SIZE	64
// /*AFLA*/ #define SIGEV_PAD_SIZE	((SIGEV_MAX_SIZE - __ARCH_SIGEV_PREAMBLE_SIZE) \
// /*AFLA*/ 		/ sizeof(int))
// /*AFLA*/ 
// /*AFLA*/ typedef struct sigevent {
// /*AFLA*/ 	sigval_t sigev_value;
// /*AFLA*/ 	int sigev_signo;
// /*AFLA*/ 	int sigev_notify;
// /*AFLA*/ 	union {
// /*AFLA*/ 		int _pad[SIGEV_PAD_SIZE];
// /*AFLA*/ 		 int _tid;
// /*AFLA*/ 
// /*AFLA*/ 		struct {
// /*AFLA*/ 			void (*_function)(sigval_t);
// /*AFLA*/ 			void *_attribute;	/* really pthread_attr_t */
// /*AFLA*/ 		} _sigev_thread;
// /*AFLA*/ 	} _sigev_un;
// /*AFLA*/ } sigevent_t;
// /*AFLA*/ 
// /*AFLA*/ #define sigev_notify_function	_sigev_un._sigev_thread._function
// /*AFLA*/ #define sigev_notify_attributes	_sigev_un._sigev_thread._attribute
// /*AFLA*/ #define sigev_notify_thread_id	 _sigev_un._tid
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_ASM_GENERIC_SIGINFO_H */
