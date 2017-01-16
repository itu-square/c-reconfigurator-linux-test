// /*AFLA*/ #ifndef _ASM_X86_PTRACE_H
// /*AFLA*/ #define _ASM_X86_PTRACE_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/segment.h>
// /*AFLA*/ #include <asm/page_types.h>
// /*AFLA*/ #include <uapi/asm/ptrace.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #ifdef __i386__
// /*AFLA*/ 
// /*AFLA*/ struct pt_regs {
// /*AFLA*/ 	unsigned long bx;
// /*AFLA*/ 	unsigned long cx;
// /*AFLA*/ 	unsigned long dx;
// /*AFLA*/ 	unsigned long si;
// /*AFLA*/ 	unsigned long di;
// /*AFLA*/ 	unsigned long bp;
// /*AFLA*/ 	unsigned long ax;
// /*AFLA*/ 	unsigned long ds;
// /*AFLA*/ 	unsigned long es;
// /*AFLA*/ 	unsigned long fs;
// /*AFLA*/ 	unsigned long gs;
// /*AFLA*/ 	unsigned long orig_ax;
// /*AFLA*/ 	unsigned long ip;
// /*AFLA*/ 	unsigned long cs;
// /*AFLA*/ 	unsigned long flags;
// /*AFLA*/ 	unsigned long sp;
// /*AFLA*/ 	unsigned long ss;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #else /* __i386__ */
// /*AFLA*/ 
// /*AFLA*/ struct pt_regs {
// /*AFLA*/ /*
// /*AFLA*/  * C ABI says these regs are callee-preserved. They aren't saved on kernel entry
// /*AFLA*/  * unless syscall needs a complete, fully filled "struct pt_regs".
// /*AFLA*/  */
// /*AFLA*/ 	unsigned long r15;
// /*AFLA*/ 	unsigned long r14;
// /*AFLA*/ 	unsigned long r13;
// /*AFLA*/ 	unsigned long r12;
// /*AFLA*/ 	unsigned long bp;
// /*AFLA*/ 	unsigned long bx;
// /*AFLA*/ /* These regs are callee-clobbered. Always saved on kernel entry. */
// /*AFLA*/ 	unsigned long r11;
// /*AFLA*/ 	unsigned long r10;
// /*AFLA*/ 	unsigned long r9;
// /*AFLA*/ 	unsigned long r8;
// /*AFLA*/ 	unsigned long ax;
// /*AFLA*/ 	unsigned long cx;
// /*AFLA*/ 	unsigned long dx;
// /*AFLA*/ 	unsigned long si;
// /*AFLA*/ 	unsigned long di;
// /*AFLA*/ /*
// /*AFLA*/  * On syscall entry, this is syscall#. On CPU exception, this is error code.
// /*AFLA*/  * On hw interrupt, it's IRQ number:
// /*AFLA*/  */
// /*AFLA*/ 	unsigned long orig_ax;
// /*AFLA*/ /* Return frame for iretq */
// /*AFLA*/ 	unsigned long ip;
// /*AFLA*/ 	unsigned long cs;
// /*AFLA*/ 	unsigned long flags;
// /*AFLA*/ 	unsigned long sp;
// /*AFLA*/ 	unsigned long ss;
// /*AFLA*/ /* top of stack page */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* !__i386__ */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PARAVIRT
// /*AFLA*/ #include <asm/paravirt_types.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct cpuinfo_x86;
// /*AFLA*/ struct task_struct;
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long profile_pc(struct pt_regs *regs);
// /*AFLA*/ #define profile_pc profile_pc
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long
// /*AFLA*/ convert_ip_to_linear(struct task_struct *child, struct pt_regs *regs);
// /*AFLA*/ extern void send_sigtrap(struct task_struct *tsk, struct pt_regs *regs,
// /*AFLA*/ 			 int error_code, int si_code);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long regs_return_value(struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ 	return regs->ax;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * user_mode(regs) determines whether a register set came from user
// /*AFLA*/  * mode.  On x86_32, this is true if V8086 mode was enabled OR if the
// /*AFLA*/  * register set was from protected mode with RPL-3 CS value.  This
// /*AFLA*/  * tricky test checks that with one comparison.
// /*AFLA*/  *
// /*AFLA*/  * On x86_64, vm86 mode is mercifully nonexistent, and we don't need
// /*AFLA*/  * the extra check.
// /*AFLA*/  */
// /*AFLA*/ static inline int user_mode(struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 	return ((regs->cs & SEGMENT_RPL_MASK) | (regs->flags & X86_VM_MASK)) >= USER_RPL;
// /*AFLA*/ #else
// /*AFLA*/ 	return !!(regs->cs & 3);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int v8086_mode(struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 	return (regs->flags & X86_VM_MASK);
// /*AFLA*/ #else
// /*AFLA*/ 	return 0;	/* No V86 mode support in long mode */
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ static inline bool user_64bit_mode(struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ #ifndef CONFIG_PARAVIRT
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * On non-paravirt systems, this is the only long mode CPL 3
// /*AFLA*/ 	 * selector.  We do not allow long mode selectors in the LDT.
// /*AFLA*/ 	 */
// /*AFLA*/ 	return regs->cs == __USER_CS;
// /*AFLA*/ #else
// /*AFLA*/ 	/* Headers are too twisted for this to go in paravirt.h. */
// /*AFLA*/ 	return regs->cs == __USER_CS || regs->cs == pv_info.extra_user_64bit_cs;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define current_user_stack_pointer()	current_pt_regs()->sp
// /*AFLA*/ #define compat_user_stack_pointer()	current_pt_regs()->sp
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ extern unsigned long kernel_stack_pointer(struct pt_regs *regs);
// /*AFLA*/ #else
// /*AFLA*/ static inline unsigned long kernel_stack_pointer(struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ 	return regs->sp;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define GET_IP(regs) ((regs)->ip)
// /*AFLA*/ #define GET_FP(regs) ((regs)->bp)
// /*AFLA*/ #define GET_USP(regs) ((regs)->sp)
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/ptrace.h>
// /*AFLA*/ 
// /*AFLA*/ /* Query offset/name of register from its name/offset */
// /*AFLA*/ extern int regs_query_register_offset(const char *name);
// /*AFLA*/ extern const char *regs_query_register_name(unsigned int offset);
// /*AFLA*/ #define MAX_REG_OFFSET (offsetof(struct pt_regs, ss))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * regs_get_register() - get register value from its offset
// /*AFLA*/  * @regs:	pt_regs from which register value is gotten.
// /*AFLA*/  * @offset:	offset number of the register.
// /*AFLA*/  *
// /*AFLA*/  * regs_get_register returns the value of a register. The @offset is the
// /*AFLA*/  * offset of the register in struct pt_regs address which specified by @regs.
// /*AFLA*/  * If @offset is bigger than MAX_REG_OFFSET, this returns 0.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long regs_get_register(struct pt_regs *regs,
// /*AFLA*/ 					      unsigned int offset)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(offset > MAX_REG_OFFSET))
// /*AFLA*/ 		return 0;
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Traps from the kernel do not save sp and ss.
// /*AFLA*/ 	 * Use the helper function to retrieve sp.
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (offset == offsetof(struct pt_regs, sp) &&
// /*AFLA*/ 	    regs->cs == __KERNEL_CS)
// /*AFLA*/ 		return kernel_stack_pointer(regs);
// /*AFLA*/ #endif
// /*AFLA*/ 	return *(unsigned long *)((unsigned long)regs + offset);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * regs_within_kernel_stack() - check the address in the stack
// /*AFLA*/  * @regs:	pt_regs which contains kernel stack pointer.
// /*AFLA*/  * @addr:	address which is checked.
// /*AFLA*/  *
// /*AFLA*/  * regs_within_kernel_stack() checks @addr is within the kernel stack page(s).
// /*AFLA*/  * If @addr is within the kernel stack, it returns true. If not, returns false.
// /*AFLA*/  */
// /*AFLA*/ static inline int regs_within_kernel_stack(struct pt_regs *regs,
// /*AFLA*/ 					   unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	return ((addr & ~(THREAD_SIZE - 1))  ==
// /*AFLA*/ 		(kernel_stack_pointer(regs) & ~(THREAD_SIZE - 1)));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * regs_get_kernel_stack_nth() - get Nth entry of the stack
// /*AFLA*/  * @regs:	pt_regs which contains kernel stack pointer.
// /*AFLA*/  * @n:		stack entry number.
// /*AFLA*/  *
// /*AFLA*/  * regs_get_kernel_stack_nth() returns @n th entry of the kernel stack which
// /*AFLA*/  * is specified by @regs. If the @n th entry is NOT in the kernel stack,
// /*AFLA*/  * this returns 0.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long regs_get_kernel_stack_nth(struct pt_regs *regs,
// /*AFLA*/ 						      unsigned int n)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long *addr = (unsigned long *)kernel_stack_pointer(regs);
// /*AFLA*/ 	addr += n;
// /*AFLA*/ 	if (regs_within_kernel_stack(regs, (unsigned long)addr))
// /*AFLA*/ 		return *addr;
// /*AFLA*/ 	else
// /*AFLA*/ 		return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define arch_has_single_step()	(1)
// /*AFLA*/ #ifdef CONFIG_X86_DEBUGCTLMSR
// /*AFLA*/ #define arch_has_block_step()	(1)
// /*AFLA*/ #else
// /*AFLA*/ #define arch_has_block_step()	(boot_cpu_data.x86 >= 6)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define ARCH_HAS_USER_SINGLE_STEP_INFO
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * When hitting ptrace_stop(), we cannot return using SYSRET because
// /*AFLA*/  * that does not restore the full CPU state, only a minimal set.  The
// /*AFLA*/  * ptracer can change arbitrary register values, which is usually okay
// /*AFLA*/  * because the usual ptrace stops run off the signal delivery path which
// /*AFLA*/  * forces IRET; however, ptrace_event() stops happen in arbitrary places
// /*AFLA*/  * in the kernel and don't force IRET path.
// /*AFLA*/  *
// /*AFLA*/  * So force IRET path after a ptrace stop.
// /*AFLA*/  */
// /*AFLA*/ #define arch_ptrace_stop_needed(code, info)				\
// /*AFLA*/ ({									\
// /*AFLA*/ 	force_iret();							\
// /*AFLA*/ 	false;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ struct user_desc;
// /*AFLA*/ extern int do_get_thread_area(struct task_struct *p, int idx,
// /*AFLA*/ 			      struct user_desc __user *info);
// /*AFLA*/ extern int do_set_thread_area(struct task_struct *p, int idx,
// /*AFLA*/ 			      struct user_desc __user *info, int can_allocate);
// /*AFLA*/ 
// /*AFLA*/ #endif /* !__ASSEMBLY__ */
// /*AFLA*/ #endif /* _ASM_X86_PTRACE_H */
