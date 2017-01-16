// /*AFLA*/ #ifndef _X86_IRQFLAGS_H_
// /*AFLA*/ #define _X86_IRQFLAGS_H_
// /*AFLA*/ 
// /*AFLA*/ #include <asm/processor-flags.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ /* Provide __cpuidle; we can't safely include <linux/cpu.h> */
// /*AFLA*/ #define __cpuidle __attribute__((__section__(".cpuidle.text")))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Interrupt control:
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long native_save_fl(void)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long flags;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * "=rm" is safe here, because "pop" adjusts the stack before
// /*AFLA*/ 	 * it evaluates its effective address -- this is part of the
// /*AFLA*/ 	 * documented behavior of the "pop" instruction.
// /*AFLA*/ 	 */
// /*AFLA*/ 	asm volatile("# __raw_save_flags\n\t"
// /*AFLA*/ 		     "pushf ; pop %0"
// /*AFLA*/ 		     : "=rm" (flags)
// /*AFLA*/ 		     : /* no input */
// /*AFLA*/ 		     : "memory");
// /*AFLA*/ 
// /*AFLA*/ 	return flags;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_restore_fl(unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("push %0 ; popf"
// /*AFLA*/ 		     : /* no output */
// /*AFLA*/ 		     :"g" (flags)
// /*AFLA*/ 		     :"memory", "cc");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_irq_disable(void)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("cli": : :"memory");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_irq_enable(void)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("sti": : :"memory");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline __cpuidle void native_safe_halt(void)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("sti; hlt": : :"memory");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline __cpuidle void native_halt(void)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("hlt": : :"memory");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PARAVIRT
// /*AFLA*/ #include <asm/paravirt.h>
// /*AFLA*/ #else
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ static inline notrace unsigned long arch_local_save_flags(void)
// /*AFLA*/ {
// /*AFLA*/ 	return native_save_fl();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline notrace void arch_local_irq_restore(unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ 	native_restore_fl(flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline notrace void arch_local_irq_disable(void)
// /*AFLA*/ {
// /*AFLA*/ 	native_irq_disable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline notrace void arch_local_irq_enable(void)
// /*AFLA*/ {
// /*AFLA*/ 	native_irq_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Used in the idle loop; sti takes one instruction cycle
// /*AFLA*/  * to complete:
// /*AFLA*/  */
// /*AFLA*/ static inline __cpuidle void arch_safe_halt(void)
// /*AFLA*/ {
// /*AFLA*/ 	native_safe_halt();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Used when interrupts are already enabled or to
// /*AFLA*/  * shutdown the processor:
// /*AFLA*/  */
// /*AFLA*/ static inline __cpuidle void halt(void)
// /*AFLA*/ {
// /*AFLA*/ 	native_halt();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * For spinlocks, etc:
// /*AFLA*/  */
// /*AFLA*/ static inline notrace unsigned long arch_local_irq_save(void)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long flags = arch_local_save_flags();
// /*AFLA*/ 	arch_local_irq_disable();
// /*AFLA*/ 	return flags;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ #define ENABLE_INTERRUPTS(x)	sti
// /*AFLA*/ #define DISABLE_INTERRUPTS(x)	cli
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ #define SWAPGS	swapgs
// /*AFLA*/ /*
// /*AFLA*/  * Currently paravirt can't handle swapgs nicely when we
// /*AFLA*/  * don't have a stack we can rely on (such as a user space
// /*AFLA*/  * stack).  So we either find a way around these or just fault
// /*AFLA*/  * and emulate if a guest tries to call swapgs directly.
// /*AFLA*/  *
// /*AFLA*/  * Either way, this is a good way to document that we don't
// /*AFLA*/  * have a reliable stack. x86_64 only.
// /*AFLA*/  */
// /*AFLA*/ #define SWAPGS_UNSAFE_STACK	swapgs
// /*AFLA*/ 
// /*AFLA*/ #define PARAVIRT_ADJUST_EXCEPTION_FRAME	/*  */
// /*AFLA*/ 
// /*AFLA*/ #define INTERRUPT_RETURN	jmp native_iret
// /*AFLA*/ #define USERGS_SYSRET64				\
// /*AFLA*/ 	swapgs;					\
// /*AFLA*/ 	sysretq;
// /*AFLA*/ #define USERGS_SYSRET32				\
// /*AFLA*/ 	swapgs;					\
// /*AFLA*/ 	sysretl
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ #define INTERRUPT_RETURN		iret
// /*AFLA*/ #define ENABLE_INTERRUPTS_SYSEXIT	sti; sysexit
// /*AFLA*/ #define GET_CR0_INTO_EAX		movl %cr0, %eax
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ #endif /* CONFIG_PARAVIRT */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ static inline int arch_irqs_disabled_flags(unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ 	return !(flags & X86_EFLAGS_IF);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int arch_irqs_disabled(void)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long flags = arch_local_save_flags();
// /*AFLA*/ 
// /*AFLA*/ 	return arch_irqs_disabled_flags(flags);
// /*AFLA*/ }
// /*AFLA*/ #endif /* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #ifdef __ASSEMBLY__
// /*AFLA*/ #ifdef CONFIG_TRACE_IRQFLAGS
// /*AFLA*/ #  define TRACE_IRQS_ON		call trace_hardirqs_on_thunk;
// /*AFLA*/ #  define TRACE_IRQS_OFF	call trace_hardirqs_off_thunk;
// /*AFLA*/ #else
// /*AFLA*/ #  define TRACE_IRQS_ON
// /*AFLA*/ #  define TRACE_IRQS_OFF
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ #  ifdef CONFIG_X86_64
// /*AFLA*/ #    define LOCKDEP_SYS_EXIT		call lockdep_sys_exit_thunk
// /*AFLA*/ #    define LOCKDEP_SYS_EXIT_IRQ \
// /*AFLA*/ 	TRACE_IRQS_ON; \
// /*AFLA*/ 	sti; \
// /*AFLA*/ 	call lockdep_sys_exit_thunk; \
// /*AFLA*/ 	cli; \
// /*AFLA*/ 	TRACE_IRQS_OFF;
// /*AFLA*/ #  else
// /*AFLA*/ #    define LOCKDEP_SYS_EXIT \
// /*AFLA*/ 	pushl %eax;				\
// /*AFLA*/ 	pushl %ecx;				\
// /*AFLA*/ 	pushl %edx;				\
// /*AFLA*/ 	call lockdep_sys_exit;			\
// /*AFLA*/ 	popl %edx;				\
// /*AFLA*/ 	popl %ecx;				\
// /*AFLA*/ 	popl %eax;
// /*AFLA*/ #    define LOCKDEP_SYS_EXIT_IRQ
// /*AFLA*/ #  endif
// /*AFLA*/ #else
// /*AFLA*/ #  define LOCKDEP_SYS_EXIT
// /*AFLA*/ #  define LOCKDEP_SYS_EXIT_IRQ
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif
