// /*AFLA*/ #ifndef _LINUX_IRQDESC_H
// /*AFLA*/ #define _LINUX_IRQDESC_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ #include <linux/kobject.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Core internal functions to deal with irq descriptors
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct irq_affinity_notify;
// /*AFLA*/ struct proc_dir_entry;
// /*AFLA*/ struct module;
// /*AFLA*/ struct irq_desc;
// /*AFLA*/ struct irq_domain;
// /*AFLA*/ struct pt_regs;
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct irq_desc - interrupt descriptor
// /*AFLA*/  * @irq_common_data:	per irq and chip data passed down to chip functions
// /*AFLA*/  * @kstat_irqs:		irq stats per cpu
// /*AFLA*/  * @handle_irq:		highlevel irq-events handler
// /*AFLA*/  * @preflow_handler:	handler called before the flow handler (currently used by sparc)
// /*AFLA*/  * @action:		the irq action chain
// /*AFLA*/  * @status:		status information
// /*AFLA*/  * @core_internal_state__do_not_mess_with_it: core internal status information
// /*AFLA*/  * @depth:		disable-depth, for nested irq_disable() calls
// /*AFLA*/  * @wake_depth:		enable depth, for multiple irq_set_irq_wake() callers
// /*AFLA*/  * @irq_count:		stats field to detect stalled irqs
// /*AFLA*/  * @last_unhandled:	aging timer for unhandled count
// /*AFLA*/  * @irqs_unhandled:	stats field for spurious unhandled interrupts
// /*AFLA*/  * @threads_handled:	stats field for deferred spurious detection of threaded handlers
// /*AFLA*/  * @threads_handled_last: comparator field for deferred spurious detection of theraded handlers
// /*AFLA*/  * @lock:		locking for SMP
// /*AFLA*/  * @affinity_hint:	hint to user space for preferred irq affinity
// /*AFLA*/  * @affinity_notify:	context for notification of affinity changes
// /*AFLA*/  * @pending_mask:	pending rebalanced interrupts
// /*AFLA*/  * @threads_oneshot:	bitfield to handle shared oneshot threads
// /*AFLA*/  * @threads_active:	number of irqaction threads currently running
// /*AFLA*/  * @wait_for_threads:	wait queue for sync_irq to wait for threaded handlers
// /*AFLA*/  * @nr_actions:		number of installed actions on this descriptor
// /*AFLA*/  * @no_suspend_depth:	number of irqactions on a irq descriptor with
// /*AFLA*/  *			IRQF_NO_SUSPEND set
// /*AFLA*/  * @force_resume_depth:	number of irqactions on a irq descriptor with
// /*AFLA*/  *			IRQF_FORCE_RESUME set
// /*AFLA*/  * @rcu:		rcu head for delayed free
// /*AFLA*/  * @kobj:		kobject used to represent this struct in sysfs
// /*AFLA*/  * @dir:		/proc/irq/ procfs entry
// /*AFLA*/  * @name:		flow handler name for /proc/interrupts output
// /*AFLA*/  */
// /*AFLA*/ struct irq_desc {
// /*AFLA*/ 	struct irq_common_data	irq_common_data;
// /*AFLA*/ 	struct irq_data		irq_data;
// /*AFLA*/ 	unsigned int __percpu	*kstat_irqs;
// /*AFLA*/ 	irq_flow_handler_t	handle_irq;
// /*AFLA*/ #ifdef CONFIG_IRQ_PREFLOW_FASTEOI
// /*AFLA*/ 	irq_preflow_handler_t	preflow_handler;
// /*AFLA*/ #endif
// /*AFLA*/ 	struct irqaction	*action;	/* IRQ action list */
// /*AFLA*/ 	unsigned int		status_use_accessors;
// /*AFLA*/ 	unsigned int		core_internal_state__do_not_mess_with_it;
// /*AFLA*/ 	unsigned int		depth;		/* nested irq disables */
// /*AFLA*/ 	unsigned int		wake_depth;	/* nested wake enables */
// /*AFLA*/ 	unsigned int		irq_count;	/* For detecting broken IRQs */
// /*AFLA*/ 	unsigned long		last_unhandled;	/* Aging timer for unhandled count */
// /*AFLA*/ 	unsigned int		irqs_unhandled;
// /*AFLA*/ 	atomic_t		threads_handled;
// /*AFLA*/ 	int			threads_handled_last;
// /*AFLA*/ 	raw_spinlock_t		lock;
// /*AFLA*/ 	struct cpumask		*percpu_enabled;
// /*AFLA*/ 	const struct cpumask	*percpu_affinity;
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 	const struct cpumask	*affinity_hint;
// /*AFLA*/ 	struct irq_affinity_notify *affinity_notify;
// /*AFLA*/ #ifdef CONFIG_GENERIC_PENDING_IRQ
// /*AFLA*/ 	cpumask_var_t		pending_mask;
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 	unsigned long		threads_oneshot;
// /*AFLA*/ 	atomic_t		threads_active;
// /*AFLA*/ 	wait_queue_head_t       wait_for_threads;
// /*AFLA*/ #ifdef CONFIG_PM_SLEEP
// /*AFLA*/ 	unsigned int		nr_actions;
// /*AFLA*/ 	unsigned int		no_suspend_depth;
// /*AFLA*/ 	unsigned int		cond_suspend_depth;
// /*AFLA*/ 	unsigned int		force_resume_depth;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_PROC_FS
// /*AFLA*/ 	struct proc_dir_entry	*dir;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_SPARSE_IRQ
// /*AFLA*/ 	struct rcu_head		rcu;
// /*AFLA*/ 	struct kobject		kobj;
// /*AFLA*/ #endif
// /*AFLA*/ 	int			parent_irq;
// /*AFLA*/ 	struct module		*owner;
// /*AFLA*/ 	const char		*name;
// /*AFLA*/ } ____cacheline_internodealigned_in_smp;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SPARSE_IRQ
// /*AFLA*/ extern void irq_lock_sparse(void);
// /*AFLA*/ extern void irq_unlock_sparse(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void irq_lock_sparse(void) { }
// /*AFLA*/ static inline void irq_unlock_sparse(void) { }
// /*AFLA*/ extern struct irq_desc irq_desc[NR_IRQS];
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline struct irq_desc *irq_data_to_desc(struct irq_data *data)
// /*AFLA*/ {
// /*AFLA*/ 	return container_of(data->common, struct irq_desc, irq_common_data);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int irq_desc_get_irq(struct irq_desc *desc)
// /*AFLA*/ {
// /*AFLA*/ 	return desc->irq_data.irq;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct irq_data *irq_desc_get_irq_data(struct irq_desc *desc)
// /*AFLA*/ {
// /*AFLA*/ 	return &desc->irq_data;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct irq_chip *irq_desc_get_chip(struct irq_desc *desc)
// /*AFLA*/ {
// /*AFLA*/ 	return desc->irq_data.chip;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void *irq_desc_get_chip_data(struct irq_desc *desc)
// /*AFLA*/ {
// /*AFLA*/ 	return desc->irq_data.chip_data;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void *irq_desc_get_handler_data(struct irq_desc *desc)
// /*AFLA*/ {
// /*AFLA*/ 	return desc->irq_common_data.handler_data;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct msi_desc *irq_desc_get_msi_desc(struct irq_desc *desc)
// /*AFLA*/ {
// /*AFLA*/ 	return desc->irq_common_data.msi_desc;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Architectures call this to let the generic IRQ layer
// /*AFLA*/  * handle an interrupt.
// /*AFLA*/  */
// /*AFLA*/ static inline void generic_handle_irq_desc(struct irq_desc *desc)
// /*AFLA*/ {
// /*AFLA*/ 	desc->handle_irq(desc);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ int generic_handle_irq(unsigned int irq);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HANDLE_DOMAIN_IRQ
// /*AFLA*/ /*
// /*AFLA*/  * Convert a HW interrupt number to a logical one using a IRQ domain,
// /*AFLA*/  * and handle the result interrupt number. Return -EINVAL if
// /*AFLA*/  * conversion failed. Providing a NULL domain indicates that the
// /*AFLA*/  * conversion has already been done.
// /*AFLA*/  */
// /*AFLA*/ int __handle_domain_irq(struct irq_domain *domain, unsigned int hwirq,
// /*AFLA*/ 			bool lookup, struct pt_regs *regs);
// /*AFLA*/ 
// /*AFLA*/ static inline int handle_domain_irq(struct irq_domain *domain,
// /*AFLA*/ 				    unsigned int hwirq, struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ 	return __handle_domain_irq(domain, hwirq, true, regs);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Test to see if a driver has successfully requested an irq */
// /*AFLA*/ static inline int irq_desc_has_action(struct irq_desc *desc)
// /*AFLA*/ {
// /*AFLA*/ 	return desc->action != NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int irq_has_action(unsigned int irq)
// /*AFLA*/ {
// /*AFLA*/ 	return irq_desc_has_action(irq_to_desc(irq));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * irq_set_handler_locked - Set irq handler from a locked region
// /*AFLA*/  * @data:	Pointer to the irq_data structure which identifies the irq
// /*AFLA*/  * @handler:	Flow control handler function for this interrupt
// /*AFLA*/  *
// /*AFLA*/  * Sets the handler in the irq descriptor associated to @data.
// /*AFLA*/  *
// /*AFLA*/  * Must be called with irq_desc locked and valid parameters. Typical
// /*AFLA*/  * call site is the irq_set_type() callback.
// /*AFLA*/  */
// /*AFLA*/ static inline void irq_set_handler_locked(struct irq_data *data,
// /*AFLA*/ 					  irq_flow_handler_t handler)
// /*AFLA*/ {
// /*AFLA*/ 	struct irq_desc *desc = irq_data_to_desc(data);
// /*AFLA*/ 
// /*AFLA*/ 	desc->handle_irq = handler;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * irq_set_chip_handler_name_locked - Set chip, handler and name from a locked region
// /*AFLA*/  * @data:	Pointer to the irq_data structure for which the chip is set
// /*AFLA*/  * @chip:	Pointer to the new irq chip
// /*AFLA*/  * @handler:	Flow control handler function for this interrupt
// /*AFLA*/  * @name:	Name of the interrupt
// /*AFLA*/  *
// /*AFLA*/  * Replace the irq chip at the proper hierarchy level in @data and
// /*AFLA*/  * sets the handler and name in the associated irq descriptor.
// /*AFLA*/  *
// /*AFLA*/  * Must be called with irq_desc locked and valid parameters.
// /*AFLA*/  */
// /*AFLA*/ static inline void
// /*AFLA*/ irq_set_chip_handler_name_locked(struct irq_data *data, struct irq_chip *chip,
// /*AFLA*/ 				 irq_flow_handler_t handler, const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	struct irq_desc *desc = irq_data_to_desc(data);
// /*AFLA*/ 
// /*AFLA*/ 	desc->handle_irq = handler;
// /*AFLA*/ 	desc->name = name;
// /*AFLA*/ 	data->chip = chip;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int irq_balancing_disabled(unsigned int irq)
// /*AFLA*/ {
// /*AFLA*/ 	struct irq_desc *desc;
// /*AFLA*/ 
// /*AFLA*/ 	desc = irq_to_desc(irq);
// /*AFLA*/ 	return desc->status_use_accessors & IRQ_NO_BALANCING_MASK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int irq_is_percpu(unsigned int irq)
// /*AFLA*/ {
// /*AFLA*/ 	struct irq_desc *desc;
// /*AFLA*/ 
// /*AFLA*/ 	desc = irq_to_desc(irq);
// /*AFLA*/ 	return desc->status_use_accessors & IRQ_PER_CPU;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ irq_set_lockdep_class(unsigned int irq, struct lock_class_key *class)
// /*AFLA*/ {
// /*AFLA*/ 	struct irq_desc *desc = irq_to_desc(irq);
// /*AFLA*/ 
// /*AFLA*/ 	if (desc)
// /*AFLA*/ 		lockdep_set_class(&desc->lock, class);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_IRQ_PREFLOW_FASTEOI
// /*AFLA*/ static inline void
// /*AFLA*/ __irq_set_preflow_handler(unsigned int irq, irq_preflow_handler_t handler)
// /*AFLA*/ {
// /*AFLA*/ 	struct irq_desc *desc;
// /*AFLA*/ 
// /*AFLA*/ 	desc = irq_to_desc(irq);
// /*AFLA*/ 	desc->preflow_handler = handler;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
