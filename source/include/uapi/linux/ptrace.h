// /*AFLA*/ #ifndef _UAPI_LINUX_PTRACE_H
// /*AFLA*/ #define _UAPI_LINUX_PTRACE_H
// /*AFLA*/ /* ptrace.h */
// /*AFLA*/ /* structs and defines to help the user use the ptrace system call. */
// /*AFLA*/ 
// /*AFLA*/ /* has the defines to get at the registers. */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ #define PTRACE_TRACEME		   0
// /*AFLA*/ #define PTRACE_PEEKTEXT		   1
// /*AFLA*/ #define PTRACE_PEEKDATA		   2
// /*AFLA*/ #define PTRACE_PEEKUSR		   3
// /*AFLA*/ #define PTRACE_POKETEXT		   4
// /*AFLA*/ #define PTRACE_POKEDATA		   5
// /*AFLA*/ #define PTRACE_POKEUSR		   6
// /*AFLA*/ #define PTRACE_CONT		   7
// /*AFLA*/ #define PTRACE_KILL		   8
// /*AFLA*/ #define PTRACE_SINGLESTEP	   9
// /*AFLA*/ 
// /*AFLA*/ #define PTRACE_ATTACH		  16
// /*AFLA*/ #define PTRACE_DETACH		  17
// /*AFLA*/ 
// /*AFLA*/ #define PTRACE_SYSCALL		  24
// /*AFLA*/ 
// /*AFLA*/ /* 0x4200-0x4300 are reserved for architecture-independent additions.  */
// /*AFLA*/ #define PTRACE_SETOPTIONS	0x4200
// /*AFLA*/ #define PTRACE_GETEVENTMSG	0x4201
// /*AFLA*/ #define PTRACE_GETSIGINFO	0x4202
// /*AFLA*/ #define PTRACE_SETSIGINFO	0x4203
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Generic ptrace interface that exports the architecture specific regsets
// /*AFLA*/  * using the corresponding NT_* types (which are also used in the core dump).
// /*AFLA*/  * Please note that the NT_PRSTATUS note type in a core dump contains a full
// /*AFLA*/  * 'struct elf_prstatus'. But the user_regset for NT_PRSTATUS contains just the
// /*AFLA*/  * elf_gregset_t that is the pr_reg field of 'struct elf_prstatus'. For all the
// /*AFLA*/  * other user_regset flavors, the user_regset layout and the ELF core dump note
// /*AFLA*/  * payload are exactly the same layout.
// /*AFLA*/  *
// /*AFLA*/  * This interface usage is as follows:
// /*AFLA*/  *	struct iovec iov = { buf, len};
// /*AFLA*/  *
// /*AFLA*/  *	ret = ptrace(PTRACE_GETREGSET/PTRACE_SETREGSET, pid, NT_XXX_TYPE, &iov);
// /*AFLA*/  *
// /*AFLA*/  * On the successful completion, iov.len will be updated by the kernel,
// /*AFLA*/  * specifying how much the kernel has written/read to/from the user's iov.buf.
// /*AFLA*/  */
// /*AFLA*/ #define PTRACE_GETREGSET	0x4204
// /*AFLA*/ #define PTRACE_SETREGSET	0x4205
// /*AFLA*/ 
// /*AFLA*/ #define PTRACE_SEIZE		0x4206
// /*AFLA*/ #define PTRACE_INTERRUPT	0x4207
// /*AFLA*/ #define PTRACE_LISTEN		0x4208
// /*AFLA*/ 
// /*AFLA*/ #define PTRACE_PEEKSIGINFO	0x4209
// /*AFLA*/ 
// /*AFLA*/ struct ptrace_peeksiginfo_args {
// /*AFLA*/ 	__u64 off;	/* from which siginfo to start */
// /*AFLA*/ 	__u32 flags;
// /*AFLA*/ 	__s32 nr;	/* how may siginfos to take */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define PTRACE_GETSIGMASK	0x420a
// /*AFLA*/ #define PTRACE_SETSIGMASK	0x420b
// /*AFLA*/ 
// /*AFLA*/ #define PTRACE_SECCOMP_GET_FILTER	0x420c
// /*AFLA*/ 
// /*AFLA*/ /* Read signals from a shared (process wide) queue */
// /*AFLA*/ #define PTRACE_PEEKSIGINFO_SHARED	(1 << 0)
// /*AFLA*/ 
// /*AFLA*/ /* Wait extended result codes for the above trace options.  */
// /*AFLA*/ #define PTRACE_EVENT_FORK	1
// /*AFLA*/ #define PTRACE_EVENT_VFORK	2
// /*AFLA*/ #define PTRACE_EVENT_CLONE	3
// /*AFLA*/ #define PTRACE_EVENT_EXEC	4
// /*AFLA*/ #define PTRACE_EVENT_VFORK_DONE	5
// /*AFLA*/ #define PTRACE_EVENT_EXIT	6
// /*AFLA*/ #define PTRACE_EVENT_SECCOMP	7
// /*AFLA*/ /* Extended result codes which enabled by means other than options.  */
// /*AFLA*/ #define PTRACE_EVENT_STOP	128
// /*AFLA*/ 
// /*AFLA*/ /* Options set using PTRACE_SETOPTIONS or using PTRACE_SEIZE @data param */
// /*AFLA*/ #define PTRACE_O_TRACESYSGOOD	1
// /*AFLA*/ #define PTRACE_O_TRACEFORK	(1 << PTRACE_EVENT_FORK)
// /*AFLA*/ #define PTRACE_O_TRACEVFORK	(1 << PTRACE_EVENT_VFORK)
// /*AFLA*/ #define PTRACE_O_TRACECLONE	(1 << PTRACE_EVENT_CLONE)
// /*AFLA*/ #define PTRACE_O_TRACEEXEC	(1 << PTRACE_EVENT_EXEC)
// /*AFLA*/ #define PTRACE_O_TRACEVFORKDONE	(1 << PTRACE_EVENT_VFORK_DONE)
// /*AFLA*/ #define PTRACE_O_TRACEEXIT	(1 << PTRACE_EVENT_EXIT)
// /*AFLA*/ #define PTRACE_O_TRACESECCOMP	(1 << PTRACE_EVENT_SECCOMP)
// /*AFLA*/ 
// /*AFLA*/ /* eventless options */
// /*AFLA*/ #define PTRACE_O_EXITKILL		(1 << 20)
// /*AFLA*/ #define PTRACE_O_SUSPEND_SECCOMP	(1 << 21)
// /*AFLA*/ 
// /*AFLA*/ #define PTRACE_O_MASK		(\
// /*AFLA*/ 	0x000000ff | PTRACE_O_EXITKILL | PTRACE_O_SUSPEND_SECCOMP)
// /*AFLA*/ 
// /*AFLA*/ #include <asm/ptrace.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_PTRACE_H */
