// /*AFLA*/ /*
// /*AFLA*/  * Per-cpu current frame pointer - the location of the last exception frame on
// /*AFLA*/  * the stack, stored in the per-cpu area.
// /*AFLA*/  *
// /*AFLA*/  * Jeremy Fitzhardinge <jeremy@goop.org>
// /*AFLA*/  */
// /*AFLA*/ #ifndef _ASM_X86_IRQ_REGS_H
// /*AFLA*/ #define _ASM_X86_IRQ_REGS_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/percpu.h>
// /*AFLA*/ 
// /*AFLA*/ #define ARCH_HAS_OWN_IRQ_REGS
// /*AFLA*/ 
// /*AFLA*/ DECLARE_PER_CPU(struct pt_regs *, irq_regs);
// /*AFLA*/ 
// /*AFLA*/ static inline struct pt_regs *get_irq_regs(void)
// /*AFLA*/ {
// /*AFLA*/ 	return this_cpu_read(irq_regs);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct pt_regs *set_irq_regs(struct pt_regs *new_regs)
// /*AFLA*/ {
// /*AFLA*/ 	struct pt_regs *old_regs;
// /*AFLA*/ 
// /*AFLA*/ 	old_regs = get_irq_regs();
// /*AFLA*/ 	this_cpu_write(irq_regs, new_regs);
// /*AFLA*/ 
// /*AFLA*/ 	return old_regs;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_IRQ_REGS_32_H */
