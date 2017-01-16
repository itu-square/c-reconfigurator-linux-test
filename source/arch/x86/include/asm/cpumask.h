// /*AFLA*/ #ifndef _ASM_X86_CPUMASK_H
// /*AFLA*/ #define _ASM_X86_CPUMASK_H
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ 
// /*AFLA*/ extern cpumask_var_t cpu_callin_mask;
// /*AFLA*/ extern cpumask_var_t cpu_callout_mask;
// /*AFLA*/ extern cpumask_var_t cpu_initialized_mask;
// /*AFLA*/ extern cpumask_var_t cpu_sibling_setup_mask;
// /*AFLA*/ 
// /*AFLA*/ extern void setup_cpu_local_masks(void);
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ #endif /* _ASM_X86_CPUMASK_H */
