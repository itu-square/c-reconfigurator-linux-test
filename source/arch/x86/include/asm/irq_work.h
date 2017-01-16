// /*AFLA*/ #ifndef _ASM_IRQ_WORK_H
// /*AFLA*/ #define _ASM_IRQ_WORK_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/cpufeature.h>
// /*AFLA*/ 
// /*AFLA*/ static inline bool arch_irq_work_has_interrupt(void)
// /*AFLA*/ {
// /*AFLA*/ 	return boot_cpu_has(X86_FEATURE_APIC);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_IRQ_WORK_H */
