// /*AFLA*/ #ifndef _ASM_X86_CURRENT_H
// /*AFLA*/ #define _ASM_X86_CURRENT_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <asm/percpu.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ struct task_struct;
// /*AFLA*/ 
// /*AFLA*/ DECLARE_PER_CPU(struct task_struct *, current_task);
// /*AFLA*/ 
// /*AFLA*/ static __always_inline struct task_struct *get_current(void)
// /*AFLA*/ {
// /*AFLA*/ 	return this_cpu_read_stable(current_task);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define current get_current()
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_CURRENT_H */
