// /*AFLA*/ #ifndef _ASM_X86_FTRACE_H
// /*AFLA*/ #define _ASM_X86_FTRACE_H
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FUNCTION_TRACER
// /*AFLA*/ #ifdef CC_USING_FENTRY
// /*AFLA*/ # define MCOUNT_ADDR		((unsigned long)(__fentry__))
// /*AFLA*/ #else
// /*AFLA*/ # define MCOUNT_ADDR		((unsigned long)(mcount))
// /*AFLA*/ # define HAVE_FUNCTION_GRAPH_FP_TEST
// /*AFLA*/ #endif
// /*AFLA*/ #define MCOUNT_INSN_SIZE	5 /* sizeof mcount call */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DYNAMIC_FTRACE
// /*AFLA*/ #define ARCH_SUPPORTS_FTRACE_OPS 1
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define HAVE_FUNCTION_GRAPH_RET_ADDR_PTR
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ extern void mcount(void);
// /*AFLA*/ extern atomic_t modifying_ftrace_code;
// /*AFLA*/ extern void __fentry__(void);
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long ftrace_call_adjust(unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * addr is the address of the mcount call instruction.
// /*AFLA*/ 	 * recordmcount does the necessary offset calculation.
// /*AFLA*/ 	 */
// /*AFLA*/ 	return addr;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DYNAMIC_FTRACE
// /*AFLA*/ 
// /*AFLA*/ struct dyn_arch_ftrace {
// /*AFLA*/ 	/* No extra data needed for x86 */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ int ftrace_int3_handler(struct pt_regs *regs);
// /*AFLA*/ 
// /*AFLA*/ #define FTRACE_GRAPH_TRAMP_ADDR FTRACE_GRAPH_ADDR
// /*AFLA*/ 
// /*AFLA*/ #endif /*  CONFIG_DYNAMIC_FTRACE */
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ #endif /* CONFIG_FUNCTION_TRACER */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #if !defined(__ASSEMBLY__) && !defined(COMPILE_OFFSETS)
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_FTRACE_SYSCALLS) && defined(CONFIG_IA32_EMULATION)
// /*AFLA*/ #include <asm/compat.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Because ia32 syscalls do not map to x86_64 syscall numbers
// /*AFLA*/  * this screws up the trace output when tracing a ia32 task.
// /*AFLA*/  * Instead of reporting bogus syscalls, just do not trace them.
// /*AFLA*/  *
// /*AFLA*/  * If the user really wants these, then they should use the
// /*AFLA*/  * raw syscall tracepoints with filtering.
// /*AFLA*/  */
// /*AFLA*/ #define ARCH_TRACE_IGNORE_COMPAT_SYSCALLS 1
// /*AFLA*/ static inline bool arch_trace_is_compat_syscall(struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ 	if (in_compat_syscall())
// /*AFLA*/ 		return true;
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_FTRACE_SYSCALLS && CONFIG_IA32_EMULATION */
// /*AFLA*/ #endif /* !__ASSEMBLY__  && !COMPILE_OFFSETS */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_FTRACE_H */
