// /*AFLA*/ #ifndef _LINUX_KERNEL_VTIME_H
// /*AFLA*/ #define _LINUX_KERNEL_VTIME_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/context_tracking_state.h>
// /*AFLA*/ #ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
// /*AFLA*/ #include <asm/vtime.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ struct task_struct;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * vtime_accounting_cpu_enabled() definitions/declarations
// /*AFLA*/  */
// /*AFLA*/ #if defined(CONFIG_VIRT_CPU_ACCOUNTING_NATIVE)
// /*AFLA*/ static inline bool vtime_accounting_cpu_enabled(void) { return true; }
// /*AFLA*/ #elif defined(CONFIG_VIRT_CPU_ACCOUNTING_GEN)
// /*AFLA*/ /*
// /*AFLA*/  * Checks if vtime is enabled on some CPU. Cputime readers want to be careful
// /*AFLA*/  * in that case and compute the tickless cputime.
// /*AFLA*/  * For now vtime state is tied to context tracking. We might want to decouple
// /*AFLA*/  * those later if necessary.
// /*AFLA*/  */
// /*AFLA*/ static inline bool vtime_accounting_enabled(void)
// /*AFLA*/ {
// /*AFLA*/ 	return context_tracking_is_enabled();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool vtime_accounting_cpu_enabled(void)
// /*AFLA*/ {
// /*AFLA*/ 	if (vtime_accounting_enabled()) {
// /*AFLA*/ 		if (context_tracking_cpu_is_enabled())
// /*AFLA*/ 			return true;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ #else /* !CONFIG_VIRT_CPU_ACCOUNTING */
// /*AFLA*/ static inline bool vtime_accounting_cpu_enabled(void) { return false; }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Common vtime APIs
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_VIRT_CPU_ACCOUNTING
// /*AFLA*/ 
// /*AFLA*/ #ifdef __ARCH_HAS_VTIME_TASK_SWITCH
// /*AFLA*/ extern void vtime_task_switch(struct task_struct *prev);
// /*AFLA*/ #else
// /*AFLA*/ extern void vtime_common_task_switch(struct task_struct *prev);
// /*AFLA*/ static inline void vtime_task_switch(struct task_struct *prev)
// /*AFLA*/ {
// /*AFLA*/ 	if (vtime_accounting_cpu_enabled())
// /*AFLA*/ 		vtime_common_task_switch(prev);
// /*AFLA*/ }
// /*AFLA*/ #endif /* __ARCH_HAS_VTIME_TASK_SWITCH */
// /*AFLA*/ 
// /*AFLA*/ extern void vtime_account_system(struct task_struct *tsk);
// /*AFLA*/ extern void vtime_account_idle(struct task_struct *tsk);
// /*AFLA*/ extern void vtime_account_user(struct task_struct *tsk);
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_VIRT_CPU_ACCOUNTING */
// /*AFLA*/ 
// /*AFLA*/ static inline void vtime_task_switch(struct task_struct *prev) { }
// /*AFLA*/ static inline void vtime_account_system(struct task_struct *tsk) { }
// /*AFLA*/ static inline void vtime_account_user(struct task_struct *tsk) { }
// /*AFLA*/ #endif /* !CONFIG_VIRT_CPU_ACCOUNTING */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
// /*AFLA*/ extern void arch_vtime_task_switch(struct task_struct *tsk);
// /*AFLA*/ extern void vtime_user_enter(struct task_struct *tsk);
// /*AFLA*/ 
// /*AFLA*/ static inline void vtime_user_exit(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	vtime_account_user(tsk);
// /*AFLA*/ }
// /*AFLA*/ extern void vtime_guest_enter(struct task_struct *tsk);
// /*AFLA*/ extern void vtime_guest_exit(struct task_struct *tsk);
// /*AFLA*/ extern void vtime_init_idle(struct task_struct *tsk, int cpu);
// /*AFLA*/ #else /* !CONFIG_VIRT_CPU_ACCOUNTING_GEN  */
// /*AFLA*/ static inline void vtime_user_enter(struct task_struct *tsk) { }
// /*AFLA*/ static inline void vtime_user_exit(struct task_struct *tsk) { }
// /*AFLA*/ static inline void vtime_guest_enter(struct task_struct *tsk) { }
// /*AFLA*/ static inline void vtime_guest_exit(struct task_struct *tsk) { }
// /*AFLA*/ static inline void vtime_init_idle(struct task_struct *tsk, int cpu) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
// /*AFLA*/ extern void vtime_account_irq_enter(struct task_struct *tsk);
// /*AFLA*/ static inline void vtime_account_irq_exit(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	/* On hard|softirq exit we always account to hard|softirq cputime */
// /*AFLA*/ 	vtime_account_system(tsk);
// /*AFLA*/ }
// /*AFLA*/ #else /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE */
// /*AFLA*/ static inline void vtime_account_irq_enter(struct task_struct *tsk) { }
// /*AFLA*/ static inline void vtime_account_irq_exit(struct task_struct *tsk) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_IRQ_TIME_ACCOUNTING
// /*AFLA*/ extern void irqtime_account_irq(struct task_struct *tsk);
// /*AFLA*/ #else
// /*AFLA*/ static inline void irqtime_account_irq(struct task_struct *tsk) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline void account_irq_enter_time(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	vtime_account_irq_enter(tsk);
// /*AFLA*/ 	irqtime_account_irq(tsk);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void account_irq_exit_time(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	vtime_account_irq_exit(tsk);
// /*AFLA*/ 	irqtime_account_irq(tsk);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_KERNEL_VTIME_H */
