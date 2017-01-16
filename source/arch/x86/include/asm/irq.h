// /*AFLA*/ #ifndef _ASM_X86_IRQ_H
// /*AFLA*/ #define _ASM_X86_IRQ_H
// /*AFLA*/ /*
// /*AFLA*/  *	(C) 1992, 1993 Linus Torvalds, (C) 1997 Ingo Molnar
// /*AFLA*/  *
// /*AFLA*/  *	IRQ/IPI changes taken from work by Thomas Radke
// /*AFLA*/  *	<tomsoft@informatik.tu-chemnitz.de>
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <asm/apicdef.h>
// /*AFLA*/ #include <asm/irq_vectors.h>
// /*AFLA*/ 
// /*AFLA*/ static inline int irq_canonicalize(int irq)
// /*AFLA*/ {
// /*AFLA*/ 	return ((irq == 2) ? 9 : irq);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ extern void irq_ctx_init(int cpu);
// /*AFLA*/ #else
// /*AFLA*/ # define irq_ctx_init(cpu) do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __ARCH_HAS_DO_SOFTIRQ
// /*AFLA*/ 
// /*AFLA*/ struct irq_desc;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HOTPLUG_CPU
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ extern int check_irq_vectors_for_cpu_disable(void);
// /*AFLA*/ extern void fixup_irqs(void);
// /*AFLA*/ extern void irq_force_complete_move(struct irq_desc *desc);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_KVM
// /*AFLA*/ extern void kvm_set_posted_intr_wakeup_handler(void (*handler)(void));
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void (*x86_platform_ipi_callback)(void);
// /*AFLA*/ extern void native_init_IRQ(void);
// /*AFLA*/ 
// /*AFLA*/ extern bool handle_irq(struct irq_desc *desc, struct pt_regs *regs);
// /*AFLA*/ 
// /*AFLA*/ extern __visible unsigned int do_IRQ(struct pt_regs *regs);
// /*AFLA*/ 
// /*AFLA*/ /* Interrupt vector management */
// /*AFLA*/ extern DECLARE_BITMAP(used_vectors, NR_VECTORS);
// /*AFLA*/ extern int vector_used_by_percpu_irq(unsigned int vector);
// /*AFLA*/ 
// /*AFLA*/ extern void init_ISA_irqs(void);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_LOCAL_APIC
// /*AFLA*/ void arch_trigger_cpumask_backtrace(const struct cpumask *mask,
// /*AFLA*/ 				    bool exclude_self);
// /*AFLA*/ #define arch_trigger_cpumask_backtrace arch_trigger_cpumask_backtrace
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_IRQ_H */
