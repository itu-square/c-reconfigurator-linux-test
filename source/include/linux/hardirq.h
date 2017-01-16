// /*AFLA*/ #ifndef LINUX_HARDIRQ_H
// /*AFLA*/ #define LINUX_HARDIRQ_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/preempt.h>
// /*AFLA*/ #include <linux/lockdep.h>
// /*AFLA*/ #include <linux/ftrace_irq.h>
// /*AFLA*/ #include <linux/vtime.h>
// /*AFLA*/ #include <asm/hardirq.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ extern void synchronize_irq(unsigned int irq);
// /*AFLA*/ extern bool synchronize_hardirq(unsigned int irq);
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_TINY_RCU)
// /*AFLA*/ 
// /*AFLA*/ static inline void rcu_nmi_enter(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void rcu_nmi_exit(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ extern void rcu_nmi_enter(void);
// /*AFLA*/ extern void rcu_nmi_exit(void);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * It is safe to do non-atomic ops on ->hardirq_context,
// /*AFLA*/  * because NMI handlers may not preempt and the ops are
// /*AFLA*/  * always balanced, so the interrupted value of ->hardirq_context
// /*AFLA*/  * will always be restored.
// /*AFLA*/  */
// /*AFLA*/ #define __irq_enter()					\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		account_irq_enter_time(current);	\
// /*AFLA*/ 		preempt_count_add(HARDIRQ_OFFSET);	\
// /*AFLA*/ 		trace_hardirq_enter();			\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Enter irq context (on NO_HZ, update jiffies):
// /*AFLA*/  */
// /*AFLA*/ extern void irq_enter(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Exit irq context without processing softirqs:
// /*AFLA*/  */
// /*AFLA*/ #define __irq_exit()					\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		trace_hardirq_exit();			\
// /*AFLA*/ 		account_irq_exit_time(current);		\
// /*AFLA*/ 		preempt_count_sub(HARDIRQ_OFFSET);	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Exit irq context and process softirqs if needed:
// /*AFLA*/  */
// /*AFLA*/ extern void irq_exit(void);
// /*AFLA*/ 
// /*AFLA*/ #define nmi_enter()						\
// /*AFLA*/ 	do {							\
// /*AFLA*/ 		printk_nmi_enter();				\
// /*AFLA*/ 		lockdep_off();					\
// /*AFLA*/ 		ftrace_nmi_enter();				\
// /*AFLA*/ 		BUG_ON(in_nmi());				\
// /*AFLA*/ 		preempt_count_add(NMI_OFFSET + HARDIRQ_OFFSET);	\
// /*AFLA*/ 		rcu_nmi_enter();				\
// /*AFLA*/ 		trace_hardirq_enter();				\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #define nmi_exit()						\
// /*AFLA*/ 	do {							\
// /*AFLA*/ 		trace_hardirq_exit();				\
// /*AFLA*/ 		rcu_nmi_exit();					\
// /*AFLA*/ 		BUG_ON(!in_nmi());				\
// /*AFLA*/ 		preempt_count_sub(NMI_OFFSET + HARDIRQ_OFFSET);	\
// /*AFLA*/ 		ftrace_nmi_exit();				\
// /*AFLA*/ 		lockdep_on();					\
// /*AFLA*/ 		printk_nmi_exit();				\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #endif /* LINUX_HARDIRQ_H */
