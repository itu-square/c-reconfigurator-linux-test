// /*AFLA*/ /* thread_info.h: low-level thread information
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2002  David Howells (dhowells@redhat.com)
// /*AFLA*/  * - Incorporating suggestions made by Linus Torvalds and Dave Miller
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _ASM_X86_THREAD_INFO_H
// /*AFLA*/ #define _ASM_X86_THREAD_INFO_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <asm/page.h>
// /*AFLA*/ #include <asm/percpu.h>
// /*AFLA*/ #include <asm/types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * TOP_OF_KERNEL_STACK_PADDING is a number of unused bytes that we
// /*AFLA*/  * reserve at the top of the kernel stack.  We do it because of a nasty
// /*AFLA*/  * 32-bit corner case.  On x86_32, the hardware stack frame is
// /*AFLA*/  * variable-length.  Except for vm86 mode, struct pt_regs assumes a
// /*AFLA*/  * maximum-length frame.  If we enter from CPL 0, the top 8 bytes of
// /*AFLA*/  * pt_regs don't actually exist.  Ordinarily this doesn't matter, but it
// /*AFLA*/  * does in at least one case:
// /*AFLA*/  *
// /*AFLA*/  * If we take an NMI early enough in SYSENTER, then we can end up with
// /*AFLA*/  * pt_regs that extends above sp0.  On the way out, in the espfix code,
// /*AFLA*/  * we can read the saved SS value, but that value will be above sp0.
// /*AFLA*/  * Without this offset, that can result in a page fault.  (We are
// /*AFLA*/  * careful that, in this case, the value we read doesn't matter.)
// /*AFLA*/  *
// /*AFLA*/  * In vm86 mode, the hardware frame is much longer still, so add 16
// /*AFLA*/  * bytes to make room for the real-mode segments.
// /*AFLA*/  *
// /*AFLA*/  * x86_64 has a fixed-length stack frame.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ # ifdef CONFIG_VM86
// /*AFLA*/ #  define TOP_OF_KERNEL_STACK_PADDING 16
// /*AFLA*/ # else
// /*AFLA*/ #  define TOP_OF_KERNEL_STACK_PADDING 8
// /*AFLA*/ # endif
// /*AFLA*/ #else
// /*AFLA*/ # define TOP_OF_KERNEL_STACK_PADDING 0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * low level task data that entry.S needs immediate access to
// /*AFLA*/  * - this struct should fit entirely inside of one cache line
// /*AFLA*/  * - this struct shares the supervisor stack pages
// /*AFLA*/  */
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ struct task_struct;
// /*AFLA*/ #include <asm/cpufeature.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ 
// /*AFLA*/ struct thread_info {
// /*AFLA*/ 	unsigned long		flags;		/* low level flags */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define INIT_THREAD_INFO(tsk)			\
// /*AFLA*/ {						\
// /*AFLA*/ 	.flags		= 0,			\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define init_stack		(init_thread_union.stack)
// /*AFLA*/ 
// /*AFLA*/ #else /* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #include <asm/asm-offsets.h>
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * thread information flags
// /*AFLA*/  * - these are process state flags that various assembly files
// /*AFLA*/  *   may need to access
// /*AFLA*/  * - pending work-to-be-done flags are in LSW
// /*AFLA*/  * - other flags in MSW
// /*AFLA*/  * Warning: layout of LSW is hardcoded in entry.S
// /*AFLA*/  */
// /*AFLA*/ #define TIF_SYSCALL_TRACE	0	/* syscall trace active */
// /*AFLA*/ #define TIF_NOTIFY_RESUME	1	/* callback before returning to user */
// /*AFLA*/ #define TIF_SIGPENDING		2	/* signal pending */
// /*AFLA*/ #define TIF_NEED_RESCHED	3	/* rescheduling necessary */
// /*AFLA*/ #define TIF_SINGLESTEP		4	/* reenable singlestep on user return*/
// /*AFLA*/ #define TIF_SYSCALL_EMU		6	/* syscall emulation active */
// /*AFLA*/ #define TIF_SYSCALL_AUDIT	7	/* syscall auditing active */
// /*AFLA*/ #define TIF_SECCOMP		8	/* secure computing */
// /*AFLA*/ #define TIF_USER_RETURN_NOTIFY	11	/* notify kernel of userspace return */
// /*AFLA*/ #define TIF_UPROBE		12	/* breakpointed or singlestepping */
// /*AFLA*/ #define TIF_NOTSC		16	/* TSC is not accessible in userland */
// /*AFLA*/ #define TIF_IA32		17	/* IA32 compatibility process */
// /*AFLA*/ #define TIF_NOHZ		19	/* in adaptive nohz mode */
// /*AFLA*/ #define TIF_MEMDIE		20	/* is terminating due to OOM killer */
// /*AFLA*/ #define TIF_POLLING_NRFLAG	21	/* idle is polling for TIF_NEED_RESCHED */
// /*AFLA*/ #define TIF_IO_BITMAP		22	/* uses I/O bitmap */
// /*AFLA*/ #define TIF_FORCED_TF		24	/* true if TF in eflags artificially */
// /*AFLA*/ #define TIF_BLOCKSTEP		25	/* set when we want DEBUGCTLMSR_BTF */
// /*AFLA*/ #define TIF_LAZY_MMU_UPDATES	27	/* task is updating the mmu lazily */
// /*AFLA*/ #define TIF_SYSCALL_TRACEPOINT	28	/* syscall tracepoint instrumentation */
// /*AFLA*/ #define TIF_ADDR32		29	/* 32-bit address space on 64 bits */
// /*AFLA*/ #define TIF_X32			30	/* 32-bit native x86-64 binary */
// /*AFLA*/ 
// /*AFLA*/ #define _TIF_SYSCALL_TRACE	(1 << TIF_SYSCALL_TRACE)
// /*AFLA*/ #define _TIF_NOTIFY_RESUME	(1 << TIF_NOTIFY_RESUME)
// /*AFLA*/ #define _TIF_SIGPENDING		(1 << TIF_SIGPENDING)
// /*AFLA*/ #define _TIF_SINGLESTEP		(1 << TIF_SINGLESTEP)
// /*AFLA*/ #define _TIF_NEED_RESCHED	(1 << TIF_NEED_RESCHED)
// /*AFLA*/ #define _TIF_SYSCALL_EMU	(1 << TIF_SYSCALL_EMU)
// /*AFLA*/ #define _TIF_SYSCALL_AUDIT	(1 << TIF_SYSCALL_AUDIT)
// /*AFLA*/ #define _TIF_SECCOMP		(1 << TIF_SECCOMP)
// /*AFLA*/ #define _TIF_USER_RETURN_NOTIFY	(1 << TIF_USER_RETURN_NOTIFY)
// /*AFLA*/ #define _TIF_UPROBE		(1 << TIF_UPROBE)
// /*AFLA*/ #define _TIF_NOTSC		(1 << TIF_NOTSC)
// /*AFLA*/ #define _TIF_IA32		(1 << TIF_IA32)
// /*AFLA*/ #define _TIF_NOHZ		(1 << TIF_NOHZ)
// /*AFLA*/ #define _TIF_POLLING_NRFLAG	(1 << TIF_POLLING_NRFLAG)
// /*AFLA*/ #define _TIF_IO_BITMAP		(1 << TIF_IO_BITMAP)
// /*AFLA*/ #define _TIF_FORCED_TF		(1 << TIF_FORCED_TF)
// /*AFLA*/ #define _TIF_BLOCKSTEP		(1 << TIF_BLOCKSTEP)
// /*AFLA*/ #define _TIF_LAZY_MMU_UPDATES	(1 << TIF_LAZY_MMU_UPDATES)
// /*AFLA*/ #define _TIF_SYSCALL_TRACEPOINT	(1 << TIF_SYSCALL_TRACEPOINT)
// /*AFLA*/ #define _TIF_ADDR32		(1 << TIF_ADDR32)
// /*AFLA*/ #define _TIF_X32		(1 << TIF_X32)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * work to do in syscall_trace_enter().  Also includes TIF_NOHZ for
// /*AFLA*/  * enter_from_user_mode()
// /*AFLA*/  */
// /*AFLA*/ #define _TIF_WORK_SYSCALL_ENTRY	\
// /*AFLA*/ 	(_TIF_SYSCALL_TRACE | _TIF_SYSCALL_EMU | _TIF_SYSCALL_AUDIT |	\
// /*AFLA*/ 	 _TIF_SECCOMP | _TIF_SYSCALL_TRACEPOINT |	\
// /*AFLA*/ 	 _TIF_NOHZ)
// /*AFLA*/ 
// /*AFLA*/ /* work to do on any return to user space */
// /*AFLA*/ #define _TIF_ALLWORK_MASK						\
// /*AFLA*/ 	((0x0000FFFF & ~_TIF_SECCOMP) | _TIF_SYSCALL_TRACEPOINT |	\
// /*AFLA*/ 	_TIF_NOHZ)
// /*AFLA*/ 
// /*AFLA*/ /* flags to check in __switch_to() */
// /*AFLA*/ #define _TIF_WORK_CTXSW							\
// /*AFLA*/ 	(_TIF_IO_BITMAP|_TIF_NOTSC|_TIF_BLOCKSTEP)
// /*AFLA*/ 
// /*AFLA*/ #define _TIF_WORK_CTXSW_PREV (_TIF_WORK_CTXSW|_TIF_USER_RETURN_NOTIFY)
// /*AFLA*/ #define _TIF_WORK_CTXSW_NEXT (_TIF_WORK_CTXSW)
// /*AFLA*/ 
// /*AFLA*/ #define STACK_WARN		(THREAD_SIZE/8)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * macros/functions for gaining access to the thread information structure
// /*AFLA*/  *
// /*AFLA*/  * preempt_count needs to be 1 initially, until the scheduler is functional.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long current_stack_pointer(void)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long sp;
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 	asm("mov %%rsp,%0" : "=g" (sp));
// /*AFLA*/ #else
// /*AFLA*/ 	asm("mov %%esp,%0" : "=g" (sp));
// /*AFLA*/ #endif
// /*AFLA*/ 	return sp;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Walks up the stack frames to make sure that the specified object is
// /*AFLA*/  * entirely contained by a single stack frame.
// /*AFLA*/  *
// /*AFLA*/  * Returns:
// /*AFLA*/  *		 1 if within a frame
// /*AFLA*/  *		-1 if placed across a frame boundary (or outside stack)
// /*AFLA*/  *		 0 unable to determine (no frame pointers, etc)
// /*AFLA*/  */
// /*AFLA*/ static inline int arch_within_stack_frames(const void * const stack,
// /*AFLA*/ 					   const void * const stackend,
// /*AFLA*/ 					   const void *obj, unsigned long len)
// /*AFLA*/ {
// /*AFLA*/ #if defined(CONFIG_FRAME_POINTER)
// /*AFLA*/ 	const void *frame = NULL;
// /*AFLA*/ 	const void *oldframe;
// /*AFLA*/ 
// /*AFLA*/ 	oldframe = __builtin_frame_address(1);
// /*AFLA*/ 	if (oldframe)
// /*AFLA*/ 		frame = __builtin_frame_address(2);
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * low ----------------------------------------------> high
// /*AFLA*/ 	 * [saved bp][saved ip][args][local vars][saved bp][saved ip]
// /*AFLA*/ 	 *                     ^----------------^
// /*AFLA*/ 	 *               allow copies only within here
// /*AFLA*/ 	 */
// /*AFLA*/ 	while (stack <= frame && frame < stackend) {
// /*AFLA*/ 		/*
// /*AFLA*/ 		 * If obj + len extends past the last frame, this
// /*AFLA*/ 		 * check won't pass and the next frame will be 0,
// /*AFLA*/ 		 * causing us to bail out and correctly report
// /*AFLA*/ 		 * the copy as invalid.
// /*AFLA*/ 		 */
// /*AFLA*/ 		if (obj + len <= frame)
// /*AFLA*/ 			return obj >= oldframe + 2 * sizeof(void *) ? 1 : -1;
// /*AFLA*/ 		oldframe = frame;
// /*AFLA*/ 		frame = *(const void * const *)frame;
// /*AFLA*/ 	}
// /*AFLA*/ 	return -1;
// /*AFLA*/ #else
// /*AFLA*/ 	return 0;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else /* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ # define cpu_current_top_of_stack (cpu_tss + TSS_sp0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_COMPAT
// /*AFLA*/ #define TS_I386_REGS_POKED	0x0004	/* regs poked by 32-bit ptracer */
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ #define in_ia32_syscall() true
// /*AFLA*/ #else
// /*AFLA*/ #define in_ia32_syscall() (IS_ENABLED(CONFIG_IA32_EMULATION) && \
// /*AFLA*/ 			   current->thread.status & TS_COMPAT)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Force syscall return via IRET by making it look as if there was
// /*AFLA*/  * some work pending. IRET is our most capable (but slowest) syscall
// /*AFLA*/  * return path, which is able to restore modified SS, CS and certain
// /*AFLA*/  * EFLAGS values that other (fast) syscall return instructions
// /*AFLA*/  * are not able to restore properly.
// /*AFLA*/  */
// /*AFLA*/ #define force_iret() set_thread_flag(TIF_NOTIFY_RESUME)
// /*AFLA*/ 
// /*AFLA*/ extern void arch_task_cache_init(void);
// /*AFLA*/ extern int arch_dup_task_struct(struct task_struct *dst, struct task_struct *src);
// /*AFLA*/ extern void arch_release_task_struct(struct task_struct *tsk);
// /*AFLA*/ #endif	/* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_THREAD_INFO_H */
