// /*AFLA*/ #ifndef _UAPI_ASM_X86_PTRACE_H
// /*AFLA*/ #define _UAPI_ASM_X86_PTRACE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>	/* For __user */
// /*AFLA*/ #include <asm/ptrace-abi.h>
// /*AFLA*/ #include <asm/processor-flags.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #ifdef __i386__
// /*AFLA*/ /* this struct defines the way the registers are stored on the
// /*AFLA*/    stack during a system call. */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __KERNEL__
// /*AFLA*/ 
// /*AFLA*/ struct pt_regs {
// /*AFLA*/ 	long ebx;
// /*AFLA*/ 	long ecx;
// /*AFLA*/ 	long edx;
// /*AFLA*/ 	long esi;
// /*AFLA*/ 	long edi;
// /*AFLA*/ 	long ebp;
// /*AFLA*/ 	long eax;
// /*AFLA*/ 	int  xds;
// /*AFLA*/ 	int  xes;
// /*AFLA*/ 	int  xfs;
// /*AFLA*/ 	int  xgs;
// /*AFLA*/ 	long orig_eax;
// /*AFLA*/ 	long eip;
// /*AFLA*/ 	int  xcs;
// /*AFLA*/ 	long eflags;
// /*AFLA*/ 	long esp;
// /*AFLA*/ 	int  xss;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* __KERNEL__ */
// /*AFLA*/ 
// /*AFLA*/ #else /* __i386__ */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __KERNEL__
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
// /*AFLA*/ 	unsigned long rbp;
// /*AFLA*/ 	unsigned long rbx;
// /*AFLA*/ /* These regs are callee-clobbered. Always saved on kernel entry. */
// /*AFLA*/ 	unsigned long r11;
// /*AFLA*/ 	unsigned long r10;
// /*AFLA*/ 	unsigned long r9;
// /*AFLA*/ 	unsigned long r8;
// /*AFLA*/ 	unsigned long rax;
// /*AFLA*/ 	unsigned long rcx;
// /*AFLA*/ 	unsigned long rdx;
// /*AFLA*/ 	unsigned long rsi;
// /*AFLA*/ 	unsigned long rdi;
// /*AFLA*/ /*
// /*AFLA*/  * On syscall entry, this is syscall#. On CPU exception, this is error code.
// /*AFLA*/  * On hw interrupt, it's IRQ number:
// /*AFLA*/  */
// /*AFLA*/ 	unsigned long orig_rax;
// /*AFLA*/ /* Return frame for iretq */
// /*AFLA*/ 	unsigned long rip;
// /*AFLA*/ 	unsigned long cs;
// /*AFLA*/ 	unsigned long eflags;
// /*AFLA*/ 	unsigned long rsp;
// /*AFLA*/ 	unsigned long ss;
// /*AFLA*/ /* top of stack page */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* __KERNEL__ */
// /*AFLA*/ #endif /* !__i386__ */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_ASM_X86_PTRACE_H */
