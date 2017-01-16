// /*AFLA*/ #ifndef __LINUX_PREEMPT_H
// /*AFLA*/ #define __LINUX_PREEMPT_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * include/linux/preempt.h - macros for accessing and manipulating
// /*AFLA*/  * preempt_count (used for kernel preemption, interrupt count, etc.)
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We put the hardirq and softirq counter into the preemption
// /*AFLA*/  * counter. The bitmask has the following meaning:
// /*AFLA*/  *
// /*AFLA*/  * - bits 0-7 are the preemption count (max preemption depth: 256)
// /*AFLA*/  * - bits 8-15 are the softirq count (max # of softirqs: 256)
// /*AFLA*/  *
// /*AFLA*/  * The hardirq count could in theory be the same as the number of
// /*AFLA*/  * interrupts in the system, but we run all interrupt handlers with
// /*AFLA*/  * interrupts disabled, so we cannot have nesting interrupts. Though
// /*AFLA*/  * there are a few palaeontologic drivers which reenable interrupts in
// /*AFLA*/  * the handler, so we need more than one bit here.
// /*AFLA*/  *
// /*AFLA*/  *         PREEMPT_MASK:	0x000000ff
// /*AFLA*/  *         SOFTIRQ_MASK:	0x0000ff00
// /*AFLA*/  *         HARDIRQ_MASK:	0x000f0000
// /*AFLA*/  *             NMI_MASK:	0x00100000
// /*AFLA*/  * PREEMPT_NEED_RESCHED:	0x80000000
// /*AFLA*/  */
// /*AFLA*/ #define PREEMPT_BITS	8
// /*AFLA*/ #define SOFTIRQ_BITS	8
// /*AFLA*/ #define HARDIRQ_BITS	4
// /*AFLA*/ #define NMI_BITS	1
// /*AFLA*/ 
// /*AFLA*/ #define PREEMPT_SHIFT	0
// /*AFLA*/ #define SOFTIRQ_SHIFT	(PREEMPT_SHIFT + PREEMPT_BITS)
// /*AFLA*/ #define HARDIRQ_SHIFT	(SOFTIRQ_SHIFT + SOFTIRQ_BITS)
// /*AFLA*/ #define NMI_SHIFT	(HARDIRQ_SHIFT + HARDIRQ_BITS)
// /*AFLA*/ 
// /*AFLA*/ #define __IRQ_MASK(x)	((1UL << (x))-1)
// /*AFLA*/ 
// /*AFLA*/ #define PREEMPT_MASK	(__IRQ_MASK(PREEMPT_BITS) << PREEMPT_SHIFT)
// /*AFLA*/ #define SOFTIRQ_MASK	(__IRQ_MASK(SOFTIRQ_BITS) << SOFTIRQ_SHIFT)
// /*AFLA*/ #define HARDIRQ_MASK	(__IRQ_MASK(HARDIRQ_BITS) << HARDIRQ_SHIFT)
// /*AFLA*/ #define NMI_MASK	(__IRQ_MASK(NMI_BITS)     << NMI_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ #define PREEMPT_OFFSET	(1UL << PREEMPT_SHIFT)
// /*AFLA*/ #define SOFTIRQ_OFFSET	(1UL << SOFTIRQ_SHIFT)
// /*AFLA*/ #define HARDIRQ_OFFSET	(1UL << HARDIRQ_SHIFT)
// /*AFLA*/ #define NMI_OFFSET	(1UL << NMI_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ #define SOFTIRQ_DISABLE_OFFSET	(2 * SOFTIRQ_OFFSET)
// /*AFLA*/ 
// /*AFLA*/ /* We use the MSB mostly because its available */
// /*AFLA*/ #define PREEMPT_NEED_RESCHED	0x80000000
// /*AFLA*/ 
// /*AFLA*/ /* preempt_count() and related functions, depends on PREEMPT_NEED_RESCHED */
// /*AFLA*/ #include <asm/preempt.h>
// /*AFLA*/ 
// /*AFLA*/ #define hardirq_count()	(preempt_count() & HARDIRQ_MASK)
// /*AFLA*/ #define softirq_count()	(preempt_count() & SOFTIRQ_MASK)
// /*AFLA*/ #define irq_count()	(preempt_count() & (HARDIRQ_MASK | SOFTIRQ_MASK \
// /*AFLA*/ 				 | NMI_MASK))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Are we doing bottom half or hardware interrupt processing?
// /*AFLA*/  * Are we in a softirq context? Interrupt context?
// /*AFLA*/  * in_softirq - Are we currently processing softirq or have bh disabled?
// /*AFLA*/  * in_serving_softirq - Are we currently processing softirq?
// /*AFLA*/  */
// /*AFLA*/ #define in_irq()		(hardirq_count())
// /*AFLA*/ #define in_softirq()		(softirq_count())
// /*AFLA*/ #define in_interrupt()		(irq_count())
// /*AFLA*/ #define in_serving_softirq()	(softirq_count() & SOFTIRQ_OFFSET)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Are we in NMI context?
// /*AFLA*/  */
// /*AFLA*/ #define in_nmi()	(preempt_count() & NMI_MASK)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The preempt_count offset after preempt_disable();
// /*AFLA*/  */
// /*AFLA*/ #if defined(CONFIG_PREEMPT_COUNT)
// /*AFLA*/ # define PREEMPT_DISABLE_OFFSET	PREEMPT_OFFSET
// /*AFLA*/ #else
// /*AFLA*/ # define PREEMPT_DISABLE_OFFSET	0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The preempt_count offset after spin_lock()
// /*AFLA*/  */
// /*AFLA*/ #define PREEMPT_LOCK_OFFSET	PREEMPT_DISABLE_OFFSET
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The preempt_count offset needed for things like:
// /*AFLA*/  *
// /*AFLA*/  *  spin_lock_bh()
// /*AFLA*/  *
// /*AFLA*/  * Which need to disable both preemption (CONFIG_PREEMPT_COUNT) and
// /*AFLA*/  * softirqs, such that unlock sequences of:
// /*AFLA*/  *
// /*AFLA*/  *  spin_unlock();
// /*AFLA*/  *  local_bh_enable();
// /*AFLA*/  *
// /*AFLA*/  * Work as expected.
// /*AFLA*/  */
// /*AFLA*/ #define SOFTIRQ_LOCK_OFFSET (SOFTIRQ_DISABLE_OFFSET + PREEMPT_LOCK_OFFSET)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Are we running in atomic context?  WARNING: this macro cannot
// /*AFLA*/  * always detect atomic context; in particular, it cannot know about
// /*AFLA*/  * held spinlocks in non-preemptible kernels.  Thus it should not be
// /*AFLA*/  * used in the general case to determine whether sleeping is possible.
// /*AFLA*/  * Do not use in_atomic() in driver code.
// /*AFLA*/  */
// /*AFLA*/ #define in_atomic()	(preempt_count() != 0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Check whether we were atomic before we did preempt_disable():
// /*AFLA*/  * (used by the scheduler)
// /*AFLA*/  */
// /*AFLA*/ #define in_atomic_preempt_off() (preempt_count() != PREEMPT_DISABLE_OFFSET)
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_DEBUG_PREEMPT) || defined(CONFIG_PREEMPT_TRACER)
// /*AFLA*/ extern void preempt_count_add(int val);
// /*AFLA*/ extern void preempt_count_sub(int val);
// /*AFLA*/ #define preempt_count_dec_and_test() \
// /*AFLA*/ 	({ preempt_count_sub(1); should_resched(0); })
// /*AFLA*/ #else
// /*AFLA*/ #define preempt_count_add(val)	__preempt_count_add(val)
// /*AFLA*/ #define preempt_count_sub(val)	__preempt_count_sub(val)
// /*AFLA*/ #define preempt_count_dec_and_test() __preempt_count_dec_and_test()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __preempt_count_inc() __preempt_count_add(1)
// /*AFLA*/ #define __preempt_count_dec() __preempt_count_sub(1)
// /*AFLA*/ 
// /*AFLA*/ #define preempt_count_inc() preempt_count_add(1)
// /*AFLA*/ #define preempt_count_dec() preempt_count_sub(1)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PREEMPT_COUNT
// /*AFLA*/ 
// /*AFLA*/ #define preempt_disable() \
// /*AFLA*/ do { \
// /*AFLA*/ 	preempt_count_inc(); \
// /*AFLA*/ 	barrier(); \
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define sched_preempt_enable_no_resched() \
// /*AFLA*/ do { \
// /*AFLA*/ 	barrier(); \
// /*AFLA*/ 	preempt_count_dec(); \
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define preempt_enable_no_resched() sched_preempt_enable_no_resched()
// /*AFLA*/ 
// /*AFLA*/ #define preemptible()	(preempt_count() == 0 && !irqs_disabled())
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PREEMPT
// /*AFLA*/ #define preempt_enable() \
// /*AFLA*/ do { \
// /*AFLA*/ 	barrier(); \
// /*AFLA*/ 	if (unlikely(preempt_count_dec_and_test())) \
// /*AFLA*/ 		__preempt_schedule(); \
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define preempt_enable_notrace() \
// /*AFLA*/ do { \
// /*AFLA*/ 	barrier(); \
// /*AFLA*/ 	if (unlikely(__preempt_count_dec_and_test())) \
// /*AFLA*/ 		__preempt_schedule_notrace(); \
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define preempt_check_resched() \
// /*AFLA*/ do { \
// /*AFLA*/ 	if (should_resched(0)) \
// /*AFLA*/ 		__preempt_schedule(); \
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_PREEMPT */
// /*AFLA*/ #define preempt_enable() \
// /*AFLA*/ do { \
// /*AFLA*/ 	barrier(); \
// /*AFLA*/ 	preempt_count_dec(); \
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define preempt_enable_notrace() \
// /*AFLA*/ do { \
// /*AFLA*/ 	barrier(); \
// /*AFLA*/ 	__preempt_count_dec(); \
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define preempt_check_resched() do { } while (0)
// /*AFLA*/ #endif /* CONFIG_PREEMPT */
// /*AFLA*/ 
// /*AFLA*/ #define preempt_disable_notrace() \
// /*AFLA*/ do { \
// /*AFLA*/ 	__preempt_count_inc(); \
// /*AFLA*/ 	barrier(); \
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define preempt_enable_no_resched_notrace() \
// /*AFLA*/ do { \
// /*AFLA*/ 	barrier(); \
// /*AFLA*/ 	__preempt_count_dec(); \
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_PREEMPT_COUNT */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Even if we don't have any preemption, we need preempt disable/enable
// /*AFLA*/  * to be barriers, so that we don't have things like get_user/put_user
// /*AFLA*/  * that can cause faults and scheduling migrate into our preempt-protected
// /*AFLA*/  * region.
// /*AFLA*/  */
// /*AFLA*/ #define preempt_disable()			barrier()
// /*AFLA*/ #define sched_preempt_enable_no_resched()	barrier()
// /*AFLA*/ #define preempt_enable_no_resched()		barrier()
// /*AFLA*/ #define preempt_enable()			barrier()
// /*AFLA*/ #define preempt_check_resched()			do { } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define preempt_disable_notrace()		barrier()
// /*AFLA*/ #define preempt_enable_no_resched_notrace()	barrier()
// /*AFLA*/ #define preempt_enable_notrace()		barrier()
// /*AFLA*/ #define preemptible()				0
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_PREEMPT_COUNT */
// /*AFLA*/ 
// /*AFLA*/ #ifdef MODULE
// /*AFLA*/ /*
// /*AFLA*/  * Modules have no business playing preemption tricks.
// /*AFLA*/  */
// /*AFLA*/ #undef sched_preempt_enable_no_resched
// /*AFLA*/ #undef preempt_enable_no_resched
// /*AFLA*/ #undef preempt_enable_no_resched_notrace
// /*AFLA*/ #undef preempt_check_resched
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define preempt_set_need_resched() \
// /*AFLA*/ do { \
// /*AFLA*/ 	set_preempt_need_resched(); \
// /*AFLA*/ } while (0)
// /*AFLA*/ #define preempt_fold_need_resched() \
// /*AFLA*/ do { \
// /*AFLA*/ 	if (tif_need_resched()) \
// /*AFLA*/ 		set_preempt_need_resched(); \
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PREEMPT_NOTIFIERS
// /*AFLA*/ 
// /*AFLA*/ struct preempt_notifier;
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * preempt_ops - notifiers called when a task is preempted and rescheduled
// /*AFLA*/  * @sched_in: we're about to be rescheduled:
// /*AFLA*/  *    notifier: struct preempt_notifier for the task being scheduled
// /*AFLA*/  *    cpu:  cpu we're scheduled on
// /*AFLA*/  * @sched_out: we've just been preempted
// /*AFLA*/  *    notifier: struct preempt_notifier for the task being preempted
// /*AFLA*/  *    next: the task that's kicking us out
// /*AFLA*/  *
// /*AFLA*/  * Please note that sched_in and out are called under different
// /*AFLA*/  * contexts.  sched_out is called with rq lock held and irq disabled
// /*AFLA*/  * while sched_in is called without rq lock and irq enabled.  This
// /*AFLA*/  * difference is intentional and depended upon by its users.
// /*AFLA*/  */
// /*AFLA*/ struct preempt_ops {
// /*AFLA*/ 	void (*sched_in)(struct preempt_notifier *notifier, int cpu);
// /*AFLA*/ 	void (*sched_out)(struct preempt_notifier *notifier,
// /*AFLA*/ 			  struct task_struct *next);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * preempt_notifier - key for installing preemption notifiers
// /*AFLA*/  * @link: internal use
// /*AFLA*/  * @ops: defines the notifier functions to be called
// /*AFLA*/  *
// /*AFLA*/  * Usually used in conjunction with container_of().
// /*AFLA*/  */
// /*AFLA*/ struct preempt_notifier {
// /*AFLA*/ 	struct hlist_node link;
// /*AFLA*/ 	struct preempt_ops *ops;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ void preempt_notifier_inc(void);
// /*AFLA*/ void preempt_notifier_dec(void);
// /*AFLA*/ void preempt_notifier_register(struct preempt_notifier *notifier);
// /*AFLA*/ void preempt_notifier_unregister(struct preempt_notifier *notifier);
// /*AFLA*/ 
// /*AFLA*/ static inline void preempt_notifier_init(struct preempt_notifier *notifier,
// /*AFLA*/ 				     struct preempt_ops *ops)
// /*AFLA*/ {
// /*AFLA*/ 	INIT_HLIST_NODE(&notifier->link);
// /*AFLA*/ 	notifier->ops = ops;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_PREEMPT_H */
