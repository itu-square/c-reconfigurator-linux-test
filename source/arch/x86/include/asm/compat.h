// /*AFLA*/ #ifndef _ASM_X86_COMPAT_H
// /*AFLA*/ #define _ASM_X86_COMPAT_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Architecture specific compatibility types
// /*AFLA*/  */
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/sched.h>
// /*AFLA*/ #include <asm/processor.h>
// /*AFLA*/ #include <asm/user32.h>
// /*AFLA*/ #include <asm/unistd.h>
// /*AFLA*/ 
// /*AFLA*/ #define COMPAT_USER_HZ		100
// /*AFLA*/ #define COMPAT_UTS_MACHINE	"i686\0\0"
// /*AFLA*/ 
// /*AFLA*/ typedef u32		compat_size_t;
// /*AFLA*/ typedef s32		compat_ssize_t;
// /*AFLA*/ typedef s32		compat_time_t;
// /*AFLA*/ typedef s32		compat_clock_t;
// /*AFLA*/ typedef s32		compat_pid_t;
// /*AFLA*/ typedef u16		__compat_uid_t;
// /*AFLA*/ typedef u16		__compat_gid_t;
// /*AFLA*/ typedef u32		__compat_uid32_t;
// /*AFLA*/ typedef u32		__compat_gid32_t;
// /*AFLA*/ typedef u16		compat_mode_t;
// /*AFLA*/ typedef u32		compat_ino_t;
// /*AFLA*/ typedef u16		compat_dev_t;
// /*AFLA*/ typedef s32		compat_off_t;
// /*AFLA*/ typedef s64		compat_loff_t;
// /*AFLA*/ typedef u16		compat_nlink_t;
// /*AFLA*/ typedef u16		compat_ipc_pid_t;
// /*AFLA*/ typedef s32		compat_daddr_t;
// /*AFLA*/ typedef u32		compat_caddr_t;
// /*AFLA*/ typedef __kernel_fsid_t	compat_fsid_t;
// /*AFLA*/ typedef s32		compat_timer_t;
// /*AFLA*/ typedef s32		compat_key_t;
// /*AFLA*/ 
// /*AFLA*/ typedef s32		compat_int_t;
// /*AFLA*/ typedef s32		compat_long_t;
// /*AFLA*/ typedef s64 __attribute__((aligned(4))) compat_s64;
// /*AFLA*/ typedef u32		compat_uint_t;
// /*AFLA*/ typedef u32		compat_ulong_t;
// /*AFLA*/ typedef u32		compat_u32;
// /*AFLA*/ typedef u64 __attribute__((aligned(4))) compat_u64;
// /*AFLA*/ typedef u32		compat_uptr_t;
// /*AFLA*/ 
// /*AFLA*/ struct compat_timespec {
// /*AFLA*/ 	compat_time_t	tv_sec;
// /*AFLA*/ 	s32		tv_nsec;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct compat_timeval {
// /*AFLA*/ 	compat_time_t	tv_sec;
// /*AFLA*/ 	s32		tv_usec;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct compat_stat {
// /*AFLA*/ 	compat_dev_t	st_dev;
// /*AFLA*/ 	u16		__pad1;
// /*AFLA*/ 	compat_ino_t	st_ino;
// /*AFLA*/ 	compat_mode_t	st_mode;
// /*AFLA*/ 	compat_nlink_t	st_nlink;
// /*AFLA*/ 	__compat_uid_t	st_uid;
// /*AFLA*/ 	__compat_gid_t	st_gid;
// /*AFLA*/ 	compat_dev_t	st_rdev;
// /*AFLA*/ 	u16		__pad2;
// /*AFLA*/ 	u32		st_size;
// /*AFLA*/ 	u32		st_blksize;
// /*AFLA*/ 	u32		st_blocks;
// /*AFLA*/ 	u32		st_atime;
// /*AFLA*/ 	u32		st_atime_nsec;
// /*AFLA*/ 	u32		st_mtime;
// /*AFLA*/ 	u32		st_mtime_nsec;
// /*AFLA*/ 	u32		st_ctime;
// /*AFLA*/ 	u32		st_ctime_nsec;
// /*AFLA*/ 	u32		__unused4;
// /*AFLA*/ 	u32		__unused5;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct compat_flock {
// /*AFLA*/ 	short		l_type;
// /*AFLA*/ 	short		l_whence;
// /*AFLA*/ 	compat_off_t	l_start;
// /*AFLA*/ 	compat_off_t	l_len;
// /*AFLA*/ 	compat_pid_t	l_pid;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define F_GETLK64	12	/*  using 'struct flock64' */
// /*AFLA*/ #define F_SETLK64	13
// /*AFLA*/ #define F_SETLKW64	14
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * IA32 uses 4 byte alignment for 64 bit quantities,
// /*AFLA*/  * so we need to pack this structure.
// /*AFLA*/  */
// /*AFLA*/ struct compat_flock64 {
// /*AFLA*/ 	short		l_type;
// /*AFLA*/ 	short		l_whence;
// /*AFLA*/ 	compat_loff_t	l_start;
// /*AFLA*/ 	compat_loff_t	l_len;
// /*AFLA*/ 	compat_pid_t	l_pid;
// /*AFLA*/ } __attribute__((packed));
// /*AFLA*/ 
// /*AFLA*/ struct compat_statfs {
// /*AFLA*/ 	int		f_type;
// /*AFLA*/ 	int		f_bsize;
// /*AFLA*/ 	int		f_blocks;
// /*AFLA*/ 	int		f_bfree;
// /*AFLA*/ 	int		f_bavail;
// /*AFLA*/ 	int		f_files;
// /*AFLA*/ 	int		f_ffree;
// /*AFLA*/ 	compat_fsid_t	f_fsid;
// /*AFLA*/ 	int		f_namelen;	/* SunOS ignores this field. */
// /*AFLA*/ 	int		f_frsize;
// /*AFLA*/ 	int		f_flags;
// /*AFLA*/ 	int		f_spare[4];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define COMPAT_RLIM_OLD_INFINITY	0x7fffffff
// /*AFLA*/ #define COMPAT_RLIM_INFINITY		0xffffffff
// /*AFLA*/ 
// /*AFLA*/ typedef u32		compat_old_sigset_t;	/* at least 32 bits */
// /*AFLA*/ 
// /*AFLA*/ #define _COMPAT_NSIG		64
// /*AFLA*/ #define _COMPAT_NSIG_BPW	32
// /*AFLA*/ 
// /*AFLA*/ typedef u32               compat_sigset_word;
// /*AFLA*/ 
// /*AFLA*/ typedef union compat_sigval {
// /*AFLA*/ 	compat_int_t	sival_int;
// /*AFLA*/ 	compat_uptr_t	sival_ptr;
// /*AFLA*/ } compat_sigval_t;
// /*AFLA*/ 
// /*AFLA*/ typedef struct compat_siginfo {
// /*AFLA*/ 	int si_signo;
// /*AFLA*/ 	int si_errno;
// /*AFLA*/ 	int si_code;
// /*AFLA*/ 
// /*AFLA*/ 	union {
// /*AFLA*/ 		int _pad[128/sizeof(int) - 3];
// /*AFLA*/ 
// /*AFLA*/ 		/* kill() */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			unsigned int _pid;	/* sender's pid */
// /*AFLA*/ 			unsigned int _uid;	/* sender's uid */
// /*AFLA*/ 		} _kill;
// /*AFLA*/ 
// /*AFLA*/ 		/* POSIX.1b timers */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			compat_timer_t _tid;	/* timer id */
// /*AFLA*/ 			int _overrun;		/* overrun count */
// /*AFLA*/ 			compat_sigval_t _sigval;	/* same as below */
// /*AFLA*/ 			int _sys_private;	/* not to be passed to user */
// /*AFLA*/ 			int _overrun_incr;	/* amount to add to overrun */
// /*AFLA*/ 		} _timer;
// /*AFLA*/ 
// /*AFLA*/ 		/* POSIX.1b signals */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			unsigned int _pid;	/* sender's pid */
// /*AFLA*/ 			unsigned int _uid;	/* sender's uid */
// /*AFLA*/ 			compat_sigval_t _sigval;
// /*AFLA*/ 		} _rt;
// /*AFLA*/ 
// /*AFLA*/ 		/* SIGCHLD */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			unsigned int _pid;	/* which child */
// /*AFLA*/ 			unsigned int _uid;	/* sender's uid */
// /*AFLA*/ 			int _status;		/* exit code */
// /*AFLA*/ 			compat_clock_t _utime;
// /*AFLA*/ 			compat_clock_t _stime;
// /*AFLA*/ 		} _sigchld;
// /*AFLA*/ 
// /*AFLA*/ 		/* SIGCHLD (x32 version) */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			unsigned int _pid;	/* which child */
// /*AFLA*/ 			unsigned int _uid;	/* sender's uid */
// /*AFLA*/ 			int _status;		/* exit code */
// /*AFLA*/ 			compat_s64 _utime;
// /*AFLA*/ 			compat_s64 _stime;
// /*AFLA*/ 		} _sigchld_x32;
// /*AFLA*/ 
// /*AFLA*/ 		/* SIGILL, SIGFPE, SIGSEGV, SIGBUS */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			unsigned int _addr;	/* faulting insn/memory ref. */
// /*AFLA*/ 			short int _addr_lsb;	/* Valid LSB of the reported address. */
// /*AFLA*/ 			union {
// /*AFLA*/ 				/* used when si_code=SEGV_BNDERR */
// /*AFLA*/ 				struct {
// /*AFLA*/ 					compat_uptr_t _lower;
// /*AFLA*/ 					compat_uptr_t _upper;
// /*AFLA*/ 				} _addr_bnd;
// /*AFLA*/ 				/* used when si_code=SEGV_PKUERR */
// /*AFLA*/ 				compat_u32 _pkey;
// /*AFLA*/ 			};
// /*AFLA*/ 		} _sigfault;
// /*AFLA*/ 
// /*AFLA*/ 		/* SIGPOLL */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			int _band;	/* POLL_IN, POLL_OUT, POLL_MSG */
// /*AFLA*/ 			int _fd;
// /*AFLA*/ 		} _sigpoll;
// /*AFLA*/ 
// /*AFLA*/ 		struct {
// /*AFLA*/ 			unsigned int _call_addr; /* calling insn */
// /*AFLA*/ 			int _syscall;	/* triggering system call number */
// /*AFLA*/ 			unsigned int _arch;	/* AUDIT_ARCH_* of syscall */
// /*AFLA*/ 		} _sigsys;
// /*AFLA*/ 	} _sifields;
// /*AFLA*/ } compat_siginfo_t;
// /*AFLA*/ 
// /*AFLA*/ #define COMPAT_OFF_T_MAX	0x7fffffff
// /*AFLA*/ #define COMPAT_LOFF_T_MAX	0x7fffffffffffffffL
// /*AFLA*/ 
// /*AFLA*/ struct compat_ipc64_perm {
// /*AFLA*/ 	compat_key_t key;
// /*AFLA*/ 	__compat_uid32_t uid;
// /*AFLA*/ 	__compat_gid32_t gid;
// /*AFLA*/ 	__compat_uid32_t cuid;
// /*AFLA*/ 	__compat_gid32_t cgid;
// /*AFLA*/ 	unsigned short mode;
// /*AFLA*/ 	unsigned short __pad1;
// /*AFLA*/ 	unsigned short seq;
// /*AFLA*/ 	unsigned short __pad2;
// /*AFLA*/ 	compat_ulong_t unused1;
// /*AFLA*/ 	compat_ulong_t unused2;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct compat_semid64_ds {
// /*AFLA*/ 	struct compat_ipc64_perm sem_perm;
// /*AFLA*/ 	compat_time_t  sem_otime;
// /*AFLA*/ 	compat_ulong_t __unused1;
// /*AFLA*/ 	compat_time_t  sem_ctime;
// /*AFLA*/ 	compat_ulong_t __unused2;
// /*AFLA*/ 	compat_ulong_t sem_nsems;
// /*AFLA*/ 	compat_ulong_t __unused3;
// /*AFLA*/ 	compat_ulong_t __unused4;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct compat_msqid64_ds {
// /*AFLA*/ 	struct compat_ipc64_perm msg_perm;
// /*AFLA*/ 	compat_time_t  msg_stime;
// /*AFLA*/ 	compat_ulong_t __unused1;
// /*AFLA*/ 	compat_time_t  msg_rtime;
// /*AFLA*/ 	compat_ulong_t __unused2;
// /*AFLA*/ 	compat_time_t  msg_ctime;
// /*AFLA*/ 	compat_ulong_t __unused3;
// /*AFLA*/ 	compat_ulong_t msg_cbytes;
// /*AFLA*/ 	compat_ulong_t msg_qnum;
// /*AFLA*/ 	compat_ulong_t msg_qbytes;
// /*AFLA*/ 	compat_pid_t   msg_lspid;
// /*AFLA*/ 	compat_pid_t   msg_lrpid;
// /*AFLA*/ 	compat_ulong_t __unused4;
// /*AFLA*/ 	compat_ulong_t __unused5;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct compat_shmid64_ds {
// /*AFLA*/ 	struct compat_ipc64_perm shm_perm;
// /*AFLA*/ 	compat_size_t  shm_segsz;
// /*AFLA*/ 	compat_time_t  shm_atime;
// /*AFLA*/ 	compat_ulong_t __unused1;
// /*AFLA*/ 	compat_time_t  shm_dtime;
// /*AFLA*/ 	compat_ulong_t __unused2;
// /*AFLA*/ 	compat_time_t  shm_ctime;
// /*AFLA*/ 	compat_ulong_t __unused3;
// /*AFLA*/ 	compat_pid_t   shm_cpid;
// /*AFLA*/ 	compat_pid_t   shm_lpid;
// /*AFLA*/ 	compat_ulong_t shm_nattch;
// /*AFLA*/ 	compat_ulong_t __unused4;
// /*AFLA*/ 	compat_ulong_t __unused5;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The type of struct elf_prstatus.pr_reg in compatible core dumps.
// /*AFLA*/  */
// /*AFLA*/ typedef struct user_regs_struct compat_elf_gregset_t;
// /*AFLA*/ 
// /*AFLA*/ /* Full regset -- prstatus on x32, otherwise on ia32 */
// /*AFLA*/ #define PRSTATUS_SIZE(S, R) (R != sizeof(S.pr_reg) ? 144 : 296)
// /*AFLA*/ #define SET_PR_FPVALID(S, V, R) \
// /*AFLA*/   do { *(int *) (((void *) &((S)->pr_reg)) + R) = (V); } \
// /*AFLA*/   while (0)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_X32_ABI
// /*AFLA*/ #define COMPAT_USE_64BIT_TIME \
// /*AFLA*/ 	(!!(task_pt_regs(current)->orig_ax & __X32_SYSCALL_BIT))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A pointer passed in from user mode. This should not
// /*AFLA*/  * be used for syscall parameters, just declare them
// /*AFLA*/  * as pointers because the syscall entry code will have
// /*AFLA*/  * appropriately converted them already.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static inline void __user *compat_ptr(compat_uptr_t uptr)
// /*AFLA*/ {
// /*AFLA*/ 	return (void __user *)(unsigned long)uptr;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline compat_uptr_t ptr_to_compat(void __user *uptr)
// /*AFLA*/ {
// /*AFLA*/ 	return (u32)(unsigned long)uptr;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __user *arch_compat_alloc_user_space(long len)
// /*AFLA*/ {
// /*AFLA*/ 	compat_uptr_t sp;
// /*AFLA*/ 
// /*AFLA*/ 	if (test_thread_flag(TIF_IA32)) {
// /*AFLA*/ 		sp = task_pt_regs(current)->sp;
// /*AFLA*/ 	} else {
// /*AFLA*/ 		/* -128 for the x32 ABI redzone */
// /*AFLA*/ 		sp = task_pt_regs(current)->sp - 128;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	return (void __user *)round_down(sp - len, 16);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool in_x32_syscall(void)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_X86_X32_ABI
// /*AFLA*/ 	if (task_pt_regs(current)->orig_ax & __X32_SYSCALL_BIT)
// /*AFLA*/ 		return true;
// /*AFLA*/ #endif
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool in_compat_syscall(void)
// /*AFLA*/ {
// /*AFLA*/ 	return in_ia32_syscall() || in_x32_syscall();
// /*AFLA*/ }
// /*AFLA*/ #define in_compat_syscall in_compat_syscall	/* override the generic impl */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_COMPAT_H */
