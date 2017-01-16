// /*AFLA*/ #ifndef _ASM_X86_IDLE_H
// /*AFLA*/ #define _ASM_X86_IDLE_H
// /*AFLA*/ 
// /*AFLA*/ #define IDLE_START 1
// /*AFLA*/ #define IDLE_END 2
// /*AFLA*/ 
// /*AFLA*/ struct notifier_block;
// /*AFLA*/ void idle_notifier_register(struct notifier_block *n);
// /*AFLA*/ void idle_notifier_unregister(struct notifier_block *n);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ void enter_idle(void);
// /*AFLA*/ void exit_idle(void);
// /*AFLA*/ #else /* !CONFIG_X86_64 */
// /*AFLA*/ static inline void enter_idle(void) { }
// /*AFLA*/ static inline void exit_idle(void) { }
// /*AFLA*/ static inline void __exit_idle(void) { }
// /*AFLA*/ #endif /* CONFIG_X86_64 */
// /*AFLA*/ 
// /*AFLA*/ void amd_e400_remove_cpu(int cpu);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_IDLE_H */
