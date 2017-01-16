// /*AFLA*/ #ifndef _LINUX_IRQ_WORK_H
// /*AFLA*/ #define _LINUX_IRQ_WORK_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/llist.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * An entry can be in one of four states:
// /*AFLA*/  *
// /*AFLA*/  * free	     NULL, 0 -> {claimed}       : free to be used
// /*AFLA*/  * claimed   NULL, 3 -> {pending}       : claimed to be enqueued
// /*AFLA*/  * pending   next, 3 -> {busy}          : queued, pending callback
// /*AFLA*/  * busy      NULL, 2 -> {free, claimed} : callback in progress, can be claimed
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define IRQ_WORK_PENDING	1UL
// /*AFLA*/ #define IRQ_WORK_BUSY		2UL
// /*AFLA*/ #define IRQ_WORK_FLAGS		3UL
// /*AFLA*/ #define IRQ_WORK_LAZY		4UL /* Doesn't want IPI, wait for tick */
// /*AFLA*/ 
// /*AFLA*/ struct irq_work {
// /*AFLA*/ 	unsigned long flags;
// /*AFLA*/ 	struct llist_node llnode;
// /*AFLA*/ 	void (*func)(struct irq_work *);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline
// /*AFLA*/ void init_irq_work(struct irq_work *work, void (*func)(struct irq_work *))
// /*AFLA*/ {
// /*AFLA*/ 	work->flags = 0;
// /*AFLA*/ 	work->func = func;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_IRQ_WORK(name, _f) struct irq_work name = { .func = (_f), }
// /*AFLA*/ 
// /*AFLA*/ bool irq_work_queue(struct irq_work *work);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ bool irq_work_queue_on(struct irq_work *work, int cpu);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ void irq_work_tick(void);
// /*AFLA*/ void irq_work_sync(struct irq_work *work);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_IRQ_WORK
// /*AFLA*/ #include <asm/irq_work.h>
// /*AFLA*/ 
// /*AFLA*/ void irq_work_run(void);
// /*AFLA*/ bool irq_work_needs_cpu(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline bool irq_work_needs_cpu(void) { return false; }
// /*AFLA*/ static inline void irq_work_run(void) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_IRQ_WORK_H */
