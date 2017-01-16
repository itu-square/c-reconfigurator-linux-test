// /*AFLA*/ #ifndef _LINUX_IRQ_H
// /*AFLA*/ #define _LINUX_IRQ_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Please do not include this file in generic code.  There is currently
// /*AFLA*/  * no requirement for any architecture to implement anything held
// /*AFLA*/  * within this file.
// /*AFLA*/  *
// /*AFLA*/  * Thanks. --rmk
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/smp.h>
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ #include <linux/cache.h>
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ #include <linux/gfp.h>
// /*AFLA*/ #include <linux/irqhandler.h>
// /*AFLA*/ #include <linux/irqreturn.h>
// /*AFLA*/ #include <linux/irqnr.h>
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ #include <linux/topology.h>
// /*AFLA*/ #include <linux/wait.h>
// /*AFLA*/ #include <linux/io.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/irq.h>
// /*AFLA*/ #include <asm/ptrace.h>
// /*AFLA*/ #include <asm/irq_regs.h>
// /*AFLA*/ 
// /*AFLA*/ struct seq_file;
// /*AFLA*/ struct module;
// /*AFLA*/ struct msi_msg;
// /*AFLA*/ enum irqchip_irq_state;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * IRQ line status.
// /*AFLA*/  *
// /*AFLA*/  * Bits 0-7 are the same as the IRQF_* bits in linux/interrupt.h
// /*AFLA*/  *
// /*AFLA*/  * IRQ_TYPE_NONE		- default, unspecified type
// /*AFLA*/  * IRQ_TYPE_EDGE_RISING		- rising edge triggered
// /*AFLA*/  * IRQ_TYPE_EDGE_FALLING	- falling edge triggered
// /*AFLA*/  * IRQ_TYPE_EDGE_BOTH		- rising and falling edge triggered
// /*AFLA*/  * IRQ_TYPE_LEVEL_HIGH		- high level triggered
// /*AFLA*/  * IRQ_TYPE_LEVEL_LOW		- low level triggered
// /*AFLA*/  * IRQ_TYPE_LEVEL_MASK		- Mask to filter out the level bits
// /*AFLA*/  * IRQ_TYPE_SENSE_MASK		- Mask for all the above bits
// /*AFLA*/  * IRQ_TYPE_DEFAULT		- For use by some PICs to ask irq_set_type
// /*AFLA*/  *				  to setup the HW to a sane default (used
// /*AFLA*/  *                                by irqdomain map() callbacks to synchronize
// /*AFLA*/  *                                the HW state and SW flags for a newly
// /*AFLA*/  *                                allocated descriptor).
// /*AFLA*/  *
// /*AFLA*/  * IRQ_TYPE_PROBE		- Special flag for probing in progress
// /*AFLA*/  *
// /*AFLA*/  * Bits which can be modified via irq_set/clear/modify_status_flags()
// /*AFLA*/  * IRQ_LEVEL			- Interrupt is level type. Will be also
// /*AFLA*/  *				  updated in the code when the above trigger
// /*AFLA*/  *				  bits are modified via irq_set_irq_type()
// /*AFLA*/  * IRQ_PER_CPU			- Mark an interrupt PER_CPU. Will protect
// /*AFLA*/  *				  it from affinity setting
// /*AFLA*/  * IRQ_NOPROBE			- Interrupt cannot be probed by autoprobing
// /*AFLA*/  * IRQ_NOREQUEST		- Interrupt cannot be requested via
// /*AFLA*/  *				  request_irq()
// /*AFLA*/  * IRQ_NOTHREAD			- Interrupt cannot be threaded
// /*AFLA*/  * IRQ_NOAUTOEN			- Interrupt is not automatically enabled in
// /*AFLA*/  *				  request/setup_irq()
// /*AFLA*/  * IRQ_NO_BALANCING		- Interrupt cannot be balanced (affinity set)
// /*AFLA*/  * IRQ_MOVE_PCNTXT		- Interrupt can be migrated from process context
// /*AFLA*/  * IRQ_NESTED_THREAD		- Interrupt nests into another thread
// /*AFLA*/  * IRQ_PER_CPU_DEVID		- Dev_id is a per-cpu variable
// /*AFLA*/  * IRQ_IS_POLLED		- Always polled by another interrupt. Exclude
// /*AFLA*/  *				  it from the spurious interrupt detection
// /*AFLA*/  *				  mechanism and from core side polling.
// /*AFLA*/  * IRQ_DISABLE_UNLAZY		- Disable lazy irq disable
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	IRQ_TYPE_NONE		= 0x00000000,
// /*AFLA*/ 	IRQ_TYPE_EDGE_RISING	= 0x00000001,
// /*AFLA*/ 	IRQ_TYPE_EDGE_FALLING	= 0x00000002,
// /*AFLA*/ 	IRQ_TYPE_EDGE_BOTH	= (IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING),
// /*AFLA*/ 	IRQ_TYPE_LEVEL_HIGH	= 0x00000004,
// /*AFLA*/ 	IRQ_TYPE_LEVEL_LOW	= 0x00000008,
// /*AFLA*/ 	IRQ_TYPE_LEVEL_MASK	= (IRQ_TYPE_LEVEL_LOW | IRQ_TYPE_LEVEL_HIGH),
// /*AFLA*/ 	IRQ_TYPE_SENSE_MASK	= 0x0000000f,
// /*AFLA*/ 	IRQ_TYPE_DEFAULT	= IRQ_TYPE_SENSE_MASK,
// /*AFLA*/ 
// /*AFLA*/ 	IRQ_TYPE_PROBE		= 0x00000010,
// /*AFLA*/ 
// /*AFLA*/ 	IRQ_LEVEL		= (1 <<  8),
// /*AFLA*/ 	IRQ_PER_CPU		= (1 <<  9),
// /*AFLA*/ 	IRQ_NOPROBE		= (1 << 10),
// /*AFLA*/ 	IRQ_NOREQUEST		= (1 << 11),
// /*AFLA*/ 	IRQ_NOAUTOEN		= (1 << 12),
// /*AFLA*/ 	IRQ_NO_BALANCING	= (1 << 13),
// /*AFLA*/ 	IRQ_MOVE_PCNTXT		= (1 << 14),
// /*AFLA*/ 	IRQ_NESTED_THREAD	= (1 << 15),
// /*AFLA*/ 	IRQ_NOTHREAD		= (1 << 16),
// /*AFLA*/ 	IRQ_PER_CPU_DEVID	= (1 << 17),
// /*AFLA*/ 	IRQ_IS_POLLED		= (1 << 18),
// /*AFLA*/ 	IRQ_DISABLE_UNLAZY	= (1 << 19),
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define IRQF_MODIFY_MASK	\
// /*AFLA*/ 	(IRQ_TYPE_SENSE_MASK | IRQ_NOPROBE | IRQ_NOREQUEST | \
// /*AFLA*/ 	 IRQ_NOAUTOEN | IRQ_MOVE_PCNTXT | IRQ_LEVEL | IRQ_NO_BALANCING | \
// /*AFLA*/ 	 IRQ_PER_CPU | IRQ_NESTED_THREAD | IRQ_NOTHREAD | IRQ_PER_CPU_DEVID | \
// /*AFLA*/ 	 IRQ_IS_POLLED | IRQ_DISABLE_UNLAZY)
// /*AFLA*/ 
// /*AFLA*/ #define IRQ_NO_BALANCING_MASK	(IRQ_PER_CPU | IRQ_NO_BALANCING)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Return value for chip->irq_set_affinity()
// /*AFLA*/  *
// /*AFLA*/  * IRQ_SET_MASK_OK	- OK, core updates irq_common_data.affinity
// /*AFLA*/  * IRQ_SET_MASK_NOCPY	- OK, chip did update irq_common_data.affinity
// /*AFLA*/  * IRQ_SET_MASK_OK_DONE	- Same as IRQ_SET_MASK_OK for core. Special code to
// /*AFLA*/  *			  support stacked irqchips, which indicates skipping
// /*AFLA*/  *			  all descendent irqchips.
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	IRQ_SET_MASK_OK = 0,
// /*AFLA*/ 	IRQ_SET_MASK_OK_NOCOPY,
// /*AFLA*/ 	IRQ_SET_MASK_OK_DONE,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct msi_desc;
// /*AFLA*/ struct irq_domain;
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct irq_common_data - per irq data shared by all irqchips
// /*AFLA*/  * @state_use_accessors: status information for irq chip functions.
// /*AFLA*/  *			Use accessor functions to deal with it
// /*AFLA*/  * @node:		node index useful for balancing
// /*AFLA*/  * @handler_data:	per-IRQ data for the irq_chip methods
// /*AFLA*/  * @affinity:		IRQ affinity on SMP. If this is an IPI
// /*AFLA*/  *			related irq, then this is the mask of the
// /*AFLA*/  *			CPUs to which an IPI can be sent.
// /*AFLA*/  * @msi_desc:		MSI descriptor
// /*AFLA*/  * @ipi_offset:		Offset of first IPI target cpu in @affinity. Optional.
// /*AFLA*/  */
// /*AFLA*/ struct irq_common_data {
// /*AFLA*/ 	unsigned int		__private state_use_accessors;
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ 	unsigned int		node;
// /*AFLA*/ #endif
// /*AFLA*/ 	void			*handler_data;
// /*AFLA*/ 	struct msi_desc		*msi_desc;
// /*AFLA*/ 	cpumask_var_t		affinity;
// /*AFLA*/ #ifdef CONFIG_GENERIC_IRQ_IPI
// /*AFLA*/ 	unsigned int		ipi_offset;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct irq_data - per irq chip data passed down to chip functions
// /*AFLA*/  * @mask:		precomputed bitmask for accessing the chip registers
// /*AFLA*/  * @irq:		interrupt number
// /*AFLA*/  * @hwirq:		hardware interrupt number, local to the interrupt domain
// /*AFLA*/  * @common:		point to data shared by all irqchips
// /*AFLA*/  * @chip:		low level interrupt hardware access
// /*AFLA*/  * @domain:		Interrupt translation domain; responsible for mapping
// /*AFLA*/  *			between hwirq number and linux irq number.
// /*AFLA*/  * @parent_data:	pointer to parent struct irq_data to support hierarchy
// /*AFLA*/  *			irq_domain
// /*AFLA*/  * @chip_data:		platform-specific per-chip private data for the chip
// /*AFLA*/  *			methods, to allow shared chip implementations
// /*AFLA*/  */
// /*AFLA*/ struct irq_data {
// /*AFLA*/ 	u32			mask;
// /*AFLA*/ 	unsigned int		irq;
// /*AFLA*/ 	unsigned long		hwirq;
// /*AFLA*/ 	struct irq_common_data	*common;
// /*AFLA*/ 	struct irq_chip		*chip;
// /*AFLA*/ 	struct irq_domain	*domain;
// /*AFLA*/ #ifdef	CONFIG_IRQ_DOMAIN_HIERARCHY
// /*AFLA*/ 	struct irq_data		*parent_data;
// /*AFLA*/ #endif
// /*AFLA*/ 	void			*chip_data;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Bit masks for irq_common_data.state_use_accessors
// /*AFLA*/  *
// /*AFLA*/  * IRQD_TRIGGER_MASK		- Mask for the trigger type bits
// /*AFLA*/  * IRQD_SETAFFINITY_PENDING	- Affinity setting is pending
// /*AFLA*/  * IRQD_NO_BALANCING		- Balancing disabled for this IRQ
// /*AFLA*/  * IRQD_PER_CPU			- Interrupt is per cpu
// /*AFLA*/  * IRQD_AFFINITY_SET		- Interrupt affinity was set
// /*AFLA*/  * IRQD_LEVEL			- Interrupt is level triggered
// /*AFLA*/  * IRQD_WAKEUP_STATE		- Interrupt is configured for wakeup
// /*AFLA*/  *				  from suspend
// /*AFLA*/  * IRDQ_MOVE_PCNTXT		- Interrupt can be moved in process
// /*AFLA*/  *				  context
// /*AFLA*/  * IRQD_IRQ_DISABLED		- Disabled state of the interrupt
// /*AFLA*/  * IRQD_IRQ_MASKED		- Masked state of the interrupt
// /*AFLA*/  * IRQD_IRQ_INPROGRESS		- In progress state of the interrupt
// /*AFLA*/  * IRQD_WAKEUP_ARMED		- Wakeup mode armed
// /*AFLA*/  * IRQD_FORWARDED_TO_VCPU	- The interrupt is forwarded to a VCPU
// /*AFLA*/  * IRQD_AFFINITY_MANAGED	- Affinity is auto-managed by the kernel
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	IRQD_TRIGGER_MASK		= 0xf,
// /*AFLA*/ 	IRQD_SETAFFINITY_PENDING	= (1 <<  8),
// /*AFLA*/ 	IRQD_NO_BALANCING		= (1 << 10),
// /*AFLA*/ 	IRQD_PER_CPU			= (1 << 11),
// /*AFLA*/ 	IRQD_AFFINITY_SET		= (1 << 12),
// /*AFLA*/ 	IRQD_LEVEL			= (1 << 13),
// /*AFLA*/ 	IRQD_WAKEUP_STATE		= (1 << 14),
// /*AFLA*/ 	IRQD_MOVE_PCNTXT		= (1 << 15),
// /*AFLA*/ 	IRQD_IRQ_DISABLED		= (1 << 16),
// /*AFLA*/ 	IRQD_IRQ_MASKED			= (1 << 17),
// /*AFLA*/ 	IRQD_IRQ_INPROGRESS		= (1 << 18),
// /*AFLA*/ 	IRQD_WAKEUP_ARMED		= (1 << 19),
// /*AFLA*/ 	IRQD_FORWARDED_TO_VCPU		= (1 << 20),
// /*AFLA*/ 	IRQD_AFFINITY_MANAGED		= (1 << 21),
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define __irqd_to_state(d) ACCESS_PRIVATE((d)->common, state_use_accessors)
// /*AFLA*/ 
// /*AFLA*/ static inline bool irqd_is_setaffinity_pending(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return __irqd_to_state(d) & IRQD_SETAFFINITY_PENDING;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool irqd_is_per_cpu(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return __irqd_to_state(d) & IRQD_PER_CPU;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool irqd_can_balance(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return !(__irqd_to_state(d) & (IRQD_PER_CPU | IRQD_NO_BALANCING));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool irqd_affinity_was_set(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return __irqd_to_state(d) & IRQD_AFFINITY_SET;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void irqd_mark_affinity_was_set(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	__irqd_to_state(d) |= IRQD_AFFINITY_SET;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u32 irqd_get_trigger_type(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return __irqd_to_state(d) & IRQD_TRIGGER_MASK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Must only be called inside irq_chip.irq_set_type() functions.
// /*AFLA*/  */
// /*AFLA*/ static inline void irqd_set_trigger_type(struct irq_data *d, u32 type)
// /*AFLA*/ {
// /*AFLA*/ 	__irqd_to_state(d) &= ~IRQD_TRIGGER_MASK;
// /*AFLA*/ 	__irqd_to_state(d) |= type & IRQD_TRIGGER_MASK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool irqd_is_level_type(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return __irqd_to_state(d) & IRQD_LEVEL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool irqd_is_wakeup_set(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return __irqd_to_state(d) & IRQD_WAKEUP_STATE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool irqd_can_move_in_process_context(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return __irqd_to_state(d) & IRQD_MOVE_PCNTXT;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool irqd_irq_disabled(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return __irqd_to_state(d) & IRQD_IRQ_DISABLED;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool irqd_irq_masked(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return __irqd_to_state(d) & IRQD_IRQ_MASKED;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool irqd_irq_inprogress(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return __irqd_to_state(d) & IRQD_IRQ_INPROGRESS;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool irqd_is_wakeup_armed(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return __irqd_to_state(d) & IRQD_WAKEUP_ARMED;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool irqd_is_forwarded_to_vcpu(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return __irqd_to_state(d) & IRQD_FORWARDED_TO_VCPU;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void irqd_set_forwarded_to_vcpu(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	__irqd_to_state(d) |= IRQD_FORWARDED_TO_VCPU;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void irqd_clr_forwarded_to_vcpu(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	__irqd_to_state(d) &= ~IRQD_FORWARDED_TO_VCPU;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool irqd_affinity_is_managed(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return __irqd_to_state(d) & IRQD_AFFINITY_MANAGED;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #undef __irqd_to_state
// /*AFLA*/ 
// /*AFLA*/ static inline irq_hw_number_t irqd_to_hwirq(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return d->hwirq;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct irq_chip - hardware interrupt chip descriptor
// /*AFLA*/  *
// /*AFLA*/  * @parent_device:	pointer to parent device for irqchip
// /*AFLA*/  * @name:		name for /proc/interrupts
// /*AFLA*/  * @irq_startup:	start up the interrupt (defaults to ->enable if NULL)
// /*AFLA*/  * @irq_shutdown:	shut down the interrupt (defaults to ->disable if NULL)
// /*AFLA*/  * @irq_enable:		enable the interrupt (defaults to chip->unmask if NULL)
// /*AFLA*/  * @irq_disable:	disable the interrupt
// /*AFLA*/  * @irq_ack:		start of a new interrupt
// /*AFLA*/  * @irq_mask:		mask an interrupt source
// /*AFLA*/  * @irq_mask_ack:	ack and mask an interrupt source
// /*AFLA*/  * @irq_unmask:		unmask an interrupt source
// /*AFLA*/  * @irq_eoi:		end of interrupt
// /*AFLA*/  * @irq_set_affinity:	set the CPU affinity on SMP machines
// /*AFLA*/  * @irq_retrigger:	resend an IRQ to the CPU
// /*AFLA*/  * @irq_set_type:	set the flow type (IRQ_TYPE_LEVEL/etc.) of an IRQ
// /*AFLA*/  * @irq_set_wake:	enable/disable power-management wake-on of an IRQ
// /*AFLA*/  * @irq_bus_lock:	function to lock access to slow bus (i2c) chips
// /*AFLA*/  * @irq_bus_sync_unlock:function to sync and unlock slow bus (i2c) chips
// /*AFLA*/  * @irq_cpu_online:	configure an interrupt source for a secondary CPU
// /*AFLA*/  * @irq_cpu_offline:	un-configure an interrupt source for a secondary CPU
// /*AFLA*/  * @irq_suspend:	function called from core code on suspend once per
// /*AFLA*/  *			chip, when one or more interrupts are installed
// /*AFLA*/  * @irq_resume:		function called from core code on resume once per chip,
// /*AFLA*/  *			when one ore more interrupts are installed
// /*AFLA*/  * @irq_pm_shutdown:	function called from core code on shutdown once per chip
// /*AFLA*/  * @irq_calc_mask:	Optional function to set irq_data.mask for special cases
// /*AFLA*/  * @irq_print_chip:	optional to print special chip info in show_interrupts
// /*AFLA*/  * @irq_request_resources:	optional to request resources before calling
// /*AFLA*/  *				any other callback related to this irq
// /*AFLA*/  * @irq_release_resources:	optional to release resources acquired with
// /*AFLA*/  *				irq_request_resources
// /*AFLA*/  * @irq_compose_msi_msg:	optional to compose message content for MSI
// /*AFLA*/  * @irq_write_msi_msg:	optional to write message content for MSI
// /*AFLA*/  * @irq_get_irqchip_state:	return the internal state of an interrupt
// /*AFLA*/  * @irq_set_irqchip_state:	set the internal state of a interrupt
// /*AFLA*/  * @irq_set_vcpu_affinity:	optional to target a vCPU in a virtual machine
// /*AFLA*/  * @ipi_send_single:	send a single IPI to destination cpus
// /*AFLA*/  * @ipi_send_mask:	send an IPI to destination cpus in cpumask
// /*AFLA*/  * @flags:		chip specific flags
// /*AFLA*/  */
// /*AFLA*/ struct irq_chip {
// /*AFLA*/ 	struct device	*parent_device;
// /*AFLA*/ 	const char	*name;
// /*AFLA*/ 	unsigned int	(*irq_startup)(struct irq_data *data);
// /*AFLA*/ 	void		(*irq_shutdown)(struct irq_data *data);
// /*AFLA*/ 	void		(*irq_enable)(struct irq_data *data);
// /*AFLA*/ 	void		(*irq_disable)(struct irq_data *data);
// /*AFLA*/ 
// /*AFLA*/ 	void		(*irq_ack)(struct irq_data *data);
// /*AFLA*/ 	void		(*irq_mask)(struct irq_data *data);
// /*AFLA*/ 	void		(*irq_mask_ack)(struct irq_data *data);
// /*AFLA*/ 	void		(*irq_unmask)(struct irq_data *data);
// /*AFLA*/ 	void		(*irq_eoi)(struct irq_data *data);
// /*AFLA*/ 
// /*AFLA*/ 	int		(*irq_set_affinity)(struct irq_data *data, const struct cpumask *dest, bool force);
// /*AFLA*/ 	int		(*irq_retrigger)(struct irq_data *data);
// /*AFLA*/ 	int		(*irq_set_type)(struct irq_data *data, unsigned int flow_type);
// /*AFLA*/ 	int		(*irq_set_wake)(struct irq_data *data, unsigned int on);
// /*AFLA*/ 
// /*AFLA*/ 	void		(*irq_bus_lock)(struct irq_data *data);
// /*AFLA*/ 	void		(*irq_bus_sync_unlock)(struct irq_data *data);
// /*AFLA*/ 
// /*AFLA*/ 	void		(*irq_cpu_online)(struct irq_data *data);
// /*AFLA*/ 	void		(*irq_cpu_offline)(struct irq_data *data);
// /*AFLA*/ 
// /*AFLA*/ 	void		(*irq_suspend)(struct irq_data *data);
// /*AFLA*/ 	void		(*irq_resume)(struct irq_data *data);
// /*AFLA*/ 	void		(*irq_pm_shutdown)(struct irq_data *data);
// /*AFLA*/ 
// /*AFLA*/ 	void		(*irq_calc_mask)(struct irq_data *data);
// /*AFLA*/ 
// /*AFLA*/ 	void		(*irq_print_chip)(struct irq_data *data, struct seq_file *p);
// /*AFLA*/ 	int		(*irq_request_resources)(struct irq_data *data);
// /*AFLA*/ 	void		(*irq_release_resources)(struct irq_data *data);
// /*AFLA*/ 
// /*AFLA*/ 	void		(*irq_compose_msi_msg)(struct irq_data *data, struct msi_msg *msg);
// /*AFLA*/ 	void		(*irq_write_msi_msg)(struct irq_data *data, struct msi_msg *msg);
// /*AFLA*/ 
// /*AFLA*/ 	int		(*irq_get_irqchip_state)(struct irq_data *data, enum irqchip_irq_state which, bool *state);
// /*AFLA*/ 	int		(*irq_set_irqchip_state)(struct irq_data *data, enum irqchip_irq_state which, bool state);
// /*AFLA*/ 
// /*AFLA*/ 	int		(*irq_set_vcpu_affinity)(struct irq_data *data, void *vcpu_info);
// /*AFLA*/ 
// /*AFLA*/ 	void		(*ipi_send_single)(struct irq_data *data, unsigned int cpu);
// /*AFLA*/ 	void		(*ipi_send_mask)(struct irq_data *data, const struct cpumask *dest);
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long	flags;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * irq_chip specific flags
// /*AFLA*/  *
// /*AFLA*/  * IRQCHIP_SET_TYPE_MASKED:	Mask before calling chip.irq_set_type()
// /*AFLA*/  * IRQCHIP_EOI_IF_HANDLED:	Only issue irq_eoi() when irq was handled
// /*AFLA*/  * IRQCHIP_MASK_ON_SUSPEND:	Mask non wake irqs in the suspend path
// /*AFLA*/  * IRQCHIP_ONOFFLINE_ENABLED:	Only call irq_on/off_line callbacks
// /*AFLA*/  *				when irq enabled
// /*AFLA*/  * IRQCHIP_SKIP_SET_WAKE:	Skip chip.irq_set_wake(), for this irq chip
// /*AFLA*/  * IRQCHIP_ONESHOT_SAFE:	One shot does not require mask/unmask
// /*AFLA*/  * IRQCHIP_EOI_THREADED:	Chip requires eoi() on unmask in threaded mode
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	IRQCHIP_SET_TYPE_MASKED		= (1 <<  0),
// /*AFLA*/ 	IRQCHIP_EOI_IF_HANDLED		= (1 <<  1),
// /*AFLA*/ 	IRQCHIP_MASK_ON_SUSPEND		= (1 <<  2),
// /*AFLA*/ 	IRQCHIP_ONOFFLINE_ENABLED	= (1 <<  3),
// /*AFLA*/ 	IRQCHIP_SKIP_SET_WAKE		= (1 <<  4),
// /*AFLA*/ 	IRQCHIP_ONESHOT_SAFE		= (1 <<  5),
// /*AFLA*/ 	IRQCHIP_EOI_THREADED		= (1 <<  6),
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #include <linux/irqdesc.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Pick up the arch-dependent methods:
// /*AFLA*/  */
// /*AFLA*/ #include <asm/hw_irq.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef NR_IRQS_LEGACY
// /*AFLA*/ # define NR_IRQS_LEGACY 0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef ARCH_IRQ_INIT_FLAGS
// /*AFLA*/ # define ARCH_IRQ_INIT_FLAGS	0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define IRQ_DEFAULT_INIT_FLAGS	ARCH_IRQ_INIT_FLAGS
// /*AFLA*/ 
// /*AFLA*/ struct irqaction;
// /*AFLA*/ extern int setup_irq(unsigned int irq, struct irqaction *new);
// /*AFLA*/ extern void remove_irq(unsigned int irq, struct irqaction *act);
// /*AFLA*/ extern int setup_percpu_irq(unsigned int irq, struct irqaction *new);
// /*AFLA*/ extern void remove_percpu_irq(unsigned int irq, struct irqaction *act);
// /*AFLA*/ 
// /*AFLA*/ extern void irq_cpu_online(void);
// /*AFLA*/ extern void irq_cpu_offline(void);
// /*AFLA*/ extern int irq_set_affinity_locked(struct irq_data *data,
// /*AFLA*/ 				   const struct cpumask *cpumask, bool force);
// /*AFLA*/ extern int irq_set_vcpu_affinity(unsigned int irq, void *vcpu_info);
// /*AFLA*/ 
// /*AFLA*/ extern void irq_migrate_all_off_this_cpu(void);
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_SMP) && defined(CONFIG_GENERIC_PENDING_IRQ)
// /*AFLA*/ void irq_move_irq(struct irq_data *data);
// /*AFLA*/ void irq_move_masked_irq(struct irq_data *data);
// /*AFLA*/ #else
// /*AFLA*/ static inline void irq_move_irq(struct irq_data *data) { }
// /*AFLA*/ static inline void irq_move_masked_irq(struct irq_data *data) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern int no_irq_affinity;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HARDIRQS_SW_RESEND
// /*AFLA*/ int irq_set_parent(int irq, int parent_irq);
// /*AFLA*/ #else
// /*AFLA*/ static inline int irq_set_parent(int irq, int parent_irq)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Built-in IRQ handlers for various IRQ types,
// /*AFLA*/  * callable via desc->handle_irq()
// /*AFLA*/  */
// /*AFLA*/ extern void handle_level_irq(struct irq_desc *desc);
// /*AFLA*/ extern void handle_fasteoi_irq(struct irq_desc *desc);
// /*AFLA*/ extern void handle_edge_irq(struct irq_desc *desc);
// /*AFLA*/ extern void handle_edge_eoi_irq(struct irq_desc *desc);
// /*AFLA*/ extern void handle_simple_irq(struct irq_desc *desc);
// /*AFLA*/ extern void handle_untracked_irq(struct irq_desc *desc);
// /*AFLA*/ extern void handle_percpu_irq(struct irq_desc *desc);
// /*AFLA*/ extern void handle_percpu_devid_irq(struct irq_desc *desc);
// /*AFLA*/ extern void handle_bad_irq(struct irq_desc *desc);
// /*AFLA*/ extern void handle_nested_irq(unsigned int irq);
// /*AFLA*/ 
// /*AFLA*/ extern int irq_chip_compose_msi_msg(struct irq_data *data, struct msi_msg *msg);
// /*AFLA*/ extern int irq_chip_pm_get(struct irq_data *data);
// /*AFLA*/ extern int irq_chip_pm_put(struct irq_data *data);
// /*AFLA*/ #ifdef	CONFIG_IRQ_DOMAIN_HIERARCHY
// /*AFLA*/ extern void irq_chip_enable_parent(struct irq_data *data);
// /*AFLA*/ extern void irq_chip_disable_parent(struct irq_data *data);
// /*AFLA*/ extern void irq_chip_ack_parent(struct irq_data *data);
// /*AFLA*/ extern int irq_chip_retrigger_hierarchy(struct irq_data *data);
// /*AFLA*/ extern void irq_chip_mask_parent(struct irq_data *data);
// /*AFLA*/ extern void irq_chip_unmask_parent(struct irq_data *data);
// /*AFLA*/ extern void irq_chip_eoi_parent(struct irq_data *data);
// /*AFLA*/ extern int irq_chip_set_affinity_parent(struct irq_data *data,
// /*AFLA*/ 					const struct cpumask *dest,
// /*AFLA*/ 					bool force);
// /*AFLA*/ extern int irq_chip_set_wake_parent(struct irq_data *data, unsigned int on);
// /*AFLA*/ extern int irq_chip_set_vcpu_affinity_parent(struct irq_data *data,
// /*AFLA*/ 					     void *vcpu_info);
// /*AFLA*/ extern int irq_chip_set_type_parent(struct irq_data *data, unsigned int type);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Handling of unhandled and spurious interrupts: */
// /*AFLA*/ extern void note_interrupt(struct irq_desc *desc, irqreturn_t action_ret);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* Enable/disable irq debugging output: */
// /*AFLA*/ extern int noirqdebug_setup(char *str);
// /*AFLA*/ 
// /*AFLA*/ /* Checks whether the interrupt can be requested by request_irq(): */
// /*AFLA*/ extern int can_request_irq(unsigned int irq, unsigned long irqflags);
// /*AFLA*/ 
// /*AFLA*/ /* Dummy irq-chip implementations: */
// /*AFLA*/ extern struct irq_chip no_irq_chip;
// /*AFLA*/ extern struct irq_chip dummy_irq_chip;
// /*AFLA*/ 
// /*AFLA*/ extern void
// /*AFLA*/ irq_set_chip_and_handler_name(unsigned int irq, struct irq_chip *chip,
// /*AFLA*/ 			      irq_flow_handler_t handle, const char *name);
// /*AFLA*/ 
// /*AFLA*/ static inline void irq_set_chip_and_handler(unsigned int irq, struct irq_chip *chip,
// /*AFLA*/ 					    irq_flow_handler_t handle)
// /*AFLA*/ {
// /*AFLA*/ 	irq_set_chip_and_handler_name(irq, chip, handle, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int irq_set_percpu_devid(unsigned int irq);
// /*AFLA*/ extern int irq_set_percpu_devid_partition(unsigned int irq,
// /*AFLA*/ 					  const struct cpumask *affinity);
// /*AFLA*/ extern int irq_get_percpu_devid_partition(unsigned int irq,
// /*AFLA*/ 					  struct cpumask *affinity);
// /*AFLA*/ 
// /*AFLA*/ extern void
// /*AFLA*/ __irq_set_handler(unsigned int irq, irq_flow_handler_t handle, int is_chained,
// /*AFLA*/ 		  const char *name);
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ irq_set_handler(unsigned int irq, irq_flow_handler_t handle)
// /*AFLA*/ {
// /*AFLA*/ 	__irq_set_handler(irq, handle, 0, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Set a highlevel chained flow handler for a given IRQ.
// /*AFLA*/  * (a chained handler is automatically enabled and set to
// /*AFLA*/  *  IRQ_NOREQUEST, IRQ_NOPROBE, and IRQ_NOTHREAD)
// /*AFLA*/  */
// /*AFLA*/ static inline void
// /*AFLA*/ irq_set_chained_handler(unsigned int irq, irq_flow_handler_t handle)
// /*AFLA*/ {
// /*AFLA*/ 	__irq_set_handler(irq, handle, 1, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Set a highlevel chained flow handler and its data for a given IRQ.
// /*AFLA*/  * (a chained handler is automatically enabled and set to
// /*AFLA*/  *  IRQ_NOREQUEST, IRQ_NOPROBE, and IRQ_NOTHREAD)
// /*AFLA*/  */
// /*AFLA*/ void
// /*AFLA*/ irq_set_chained_handler_and_data(unsigned int irq, irq_flow_handler_t handle,
// /*AFLA*/ 				 void *data);
// /*AFLA*/ 
// /*AFLA*/ void irq_modify_status(unsigned int irq, unsigned long clr, unsigned long set);
// /*AFLA*/ 
// /*AFLA*/ static inline void irq_set_status_flags(unsigned int irq, unsigned long set)
// /*AFLA*/ {
// /*AFLA*/ 	irq_modify_status(irq, 0, set);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void irq_clear_status_flags(unsigned int irq, unsigned long clr)
// /*AFLA*/ {
// /*AFLA*/ 	irq_modify_status(irq, clr, 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void irq_set_noprobe(unsigned int irq)
// /*AFLA*/ {
// /*AFLA*/ 	irq_modify_status(irq, 0, IRQ_NOPROBE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void irq_set_probe(unsigned int irq)
// /*AFLA*/ {
// /*AFLA*/ 	irq_modify_status(irq, IRQ_NOPROBE, 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void irq_set_nothread(unsigned int irq)
// /*AFLA*/ {
// /*AFLA*/ 	irq_modify_status(irq, 0, IRQ_NOTHREAD);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void irq_set_thread(unsigned int irq)
// /*AFLA*/ {
// /*AFLA*/ 	irq_modify_status(irq, IRQ_NOTHREAD, 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void irq_set_nested_thread(unsigned int irq, bool nest)
// /*AFLA*/ {
// /*AFLA*/ 	if (nest)
// /*AFLA*/ 		irq_set_status_flags(irq, IRQ_NESTED_THREAD);
// /*AFLA*/ 	else
// /*AFLA*/ 		irq_clear_status_flags(irq, IRQ_NESTED_THREAD);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void irq_set_percpu_devid_flags(unsigned int irq)
// /*AFLA*/ {
// /*AFLA*/ 	irq_set_status_flags(irq,
// /*AFLA*/ 			     IRQ_NOAUTOEN | IRQ_PER_CPU | IRQ_NOTHREAD |
// /*AFLA*/ 			     IRQ_NOPROBE | IRQ_PER_CPU_DEVID);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Set/get chip/data for an IRQ: */
// /*AFLA*/ extern int irq_set_chip(unsigned int irq, struct irq_chip *chip);
// /*AFLA*/ extern int irq_set_handler_data(unsigned int irq, void *data);
// /*AFLA*/ extern int irq_set_chip_data(unsigned int irq, void *data);
// /*AFLA*/ extern int irq_set_irq_type(unsigned int irq, unsigned int type);
// /*AFLA*/ extern int irq_set_msi_desc(unsigned int irq, struct msi_desc *entry);
// /*AFLA*/ extern int irq_set_msi_desc_off(unsigned int irq_base, unsigned int irq_offset,
// /*AFLA*/ 				struct msi_desc *entry);
// /*AFLA*/ extern struct irq_data *irq_get_irq_data(unsigned int irq);
// /*AFLA*/ 
// /*AFLA*/ static inline struct irq_chip *irq_get_chip(unsigned int irq)
// /*AFLA*/ {
// /*AFLA*/ 	struct irq_data *d = irq_get_irq_data(irq);
// /*AFLA*/ 	return d ? d->chip : NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct irq_chip *irq_data_get_irq_chip(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return d->chip;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void *irq_get_chip_data(unsigned int irq)
// /*AFLA*/ {
// /*AFLA*/ 	struct irq_data *d = irq_get_irq_data(irq);
// /*AFLA*/ 	return d ? d->chip_data : NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void *irq_data_get_irq_chip_data(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return d->chip_data;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void *irq_get_handler_data(unsigned int irq)
// /*AFLA*/ {
// /*AFLA*/ 	struct irq_data *d = irq_get_irq_data(irq);
// /*AFLA*/ 	return d ? d->common->handler_data : NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void *irq_data_get_irq_handler_data(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return d->common->handler_data;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct msi_desc *irq_get_msi_desc(unsigned int irq)
// /*AFLA*/ {
// /*AFLA*/ 	struct irq_data *d = irq_get_irq_data(irq);
// /*AFLA*/ 	return d ? d->common->msi_desc : NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct msi_desc *irq_data_get_msi_desc(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return d->common->msi_desc;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u32 irq_get_trigger_type(unsigned int irq)
// /*AFLA*/ {
// /*AFLA*/ 	struct irq_data *d = irq_get_irq_data(irq);
// /*AFLA*/ 	return d ? irqd_get_trigger_type(d) : 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int irq_common_data_get_node(struct irq_common_data *d)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ 	return d->node;
// /*AFLA*/ #else
// /*AFLA*/ 	return 0;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int irq_data_get_node(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return irq_common_data_get_node(d->common);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct cpumask *irq_get_affinity_mask(int irq)
// /*AFLA*/ {
// /*AFLA*/ 	struct irq_data *d = irq_get_irq_data(irq);
// /*AFLA*/ 
// /*AFLA*/ 	return d ? d->common->affinity : NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct cpumask *irq_data_get_affinity_mask(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return d->common->affinity;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ unsigned int arch_dynirq_lower_bound(unsigned int from);
// /*AFLA*/ 
// /*AFLA*/ int __irq_alloc_descs(int irq, unsigned int from, unsigned int cnt, int node,
// /*AFLA*/ 		      struct module *owner, const struct cpumask *affinity);
// /*AFLA*/ 
// /*AFLA*/ /* use macros to avoid needing export.h for THIS_MODULE */
// /*AFLA*/ #define irq_alloc_descs(irq, from, cnt, node)	\
// /*AFLA*/ 	__irq_alloc_descs(irq, from, cnt, node, THIS_MODULE, NULL)
// /*AFLA*/ 
// /*AFLA*/ #define irq_alloc_desc(node)			\
// /*AFLA*/ 	irq_alloc_descs(-1, 0, 1, node)
// /*AFLA*/ 
// /*AFLA*/ #define irq_alloc_desc_at(at, node)		\
// /*AFLA*/ 	irq_alloc_descs(at, at, 1, node)
// /*AFLA*/ 
// /*AFLA*/ #define irq_alloc_desc_from(from, node)		\
// /*AFLA*/ 	irq_alloc_descs(-1, from, 1, node)
// /*AFLA*/ 
// /*AFLA*/ #define irq_alloc_descs_from(from, cnt, node)	\
// /*AFLA*/ 	irq_alloc_descs(-1, from, cnt, node)
// /*AFLA*/ 
// /*AFLA*/ void irq_free_descs(unsigned int irq, unsigned int cnt);
// /*AFLA*/ static inline void irq_free_desc(unsigned int irq)
// /*AFLA*/ {
// /*AFLA*/ 	irq_free_descs(irq, 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_GENERIC_IRQ_LEGACY_ALLOC_HWIRQ
// /*AFLA*/ unsigned int irq_alloc_hwirqs(int cnt, int node);
// /*AFLA*/ static inline unsigned int irq_alloc_hwirq(int node)
// /*AFLA*/ {
// /*AFLA*/ 	return irq_alloc_hwirqs(1, node);
// /*AFLA*/ }
// /*AFLA*/ void irq_free_hwirqs(unsigned int from, int cnt);
// /*AFLA*/ static inline void irq_free_hwirq(unsigned int irq)
// /*AFLA*/ {
// /*AFLA*/ 	return irq_free_hwirqs(irq, 1);
// /*AFLA*/ }
// /*AFLA*/ int arch_setup_hwirq(unsigned int irq, int node);
// /*AFLA*/ void arch_teardown_hwirq(unsigned int irq);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_GENERIC_IRQ_LEGACY
// /*AFLA*/ void irq_init_desc(unsigned int irq);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct irq_chip_regs - register offsets for struct irq_gci
// /*AFLA*/  * @enable:	Enable register offset to reg_base
// /*AFLA*/  * @disable:	Disable register offset to reg_base
// /*AFLA*/  * @mask:	Mask register offset to reg_base
// /*AFLA*/  * @ack:	Ack register offset to reg_base
// /*AFLA*/  * @eoi:	Eoi register offset to reg_base
// /*AFLA*/  * @type:	Type configuration register offset to reg_base
// /*AFLA*/  * @polarity:	Polarity configuration register offset to reg_base
// /*AFLA*/  */
// /*AFLA*/ struct irq_chip_regs {
// /*AFLA*/ 	unsigned long		enable;
// /*AFLA*/ 	unsigned long		disable;
// /*AFLA*/ 	unsigned long		mask;
// /*AFLA*/ 	unsigned long		ack;
// /*AFLA*/ 	unsigned long		eoi;
// /*AFLA*/ 	unsigned long		type;
// /*AFLA*/ 	unsigned long		polarity;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct irq_chip_type - Generic interrupt chip instance for a flow type
// /*AFLA*/  * @chip:		The real interrupt chip which provides the callbacks
// /*AFLA*/  * @regs:		Register offsets for this chip
// /*AFLA*/  * @handler:		Flow handler associated with this chip
// /*AFLA*/  * @type:		Chip can handle these flow types
// /*AFLA*/  * @mask_cache_priv:	Cached mask register private to the chip type
// /*AFLA*/  * @mask_cache:		Pointer to cached mask register
// /*AFLA*/  *
// /*AFLA*/  * A irq_generic_chip can have several instances of irq_chip_type when
// /*AFLA*/  * it requires different functions and register offsets for different
// /*AFLA*/  * flow types.
// /*AFLA*/  */
// /*AFLA*/ struct irq_chip_type {
// /*AFLA*/ 	struct irq_chip		chip;
// /*AFLA*/ 	struct irq_chip_regs	regs;
// /*AFLA*/ 	irq_flow_handler_t	handler;
// /*AFLA*/ 	u32			type;
// /*AFLA*/ 	u32			mask_cache_priv;
// /*AFLA*/ 	u32			*mask_cache;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct irq_chip_generic - Generic irq chip data structure
// /*AFLA*/  * @lock:		Lock to protect register and cache data access
// /*AFLA*/  * @reg_base:		Register base address (virtual)
// /*AFLA*/  * @reg_readl:		Alternate I/O accessor (defaults to readl if NULL)
// /*AFLA*/  * @reg_writel:		Alternate I/O accessor (defaults to writel if NULL)
// /*AFLA*/  * @suspend:		Function called from core code on suspend once per
// /*AFLA*/  *			chip; can be useful instead of irq_chip::suspend to
// /*AFLA*/  *			handle chip details even when no interrupts are in use
// /*AFLA*/  * @resume:		Function called from core code on resume once per chip;
// /*AFLA*/  *			can be useful instead of irq_chip::suspend to handle
// /*AFLA*/  *			chip details even when no interrupts are in use
// /*AFLA*/  * @irq_base:		Interrupt base nr for this chip
// /*AFLA*/  * @irq_cnt:		Number of interrupts handled by this chip
// /*AFLA*/  * @mask_cache:		Cached mask register shared between all chip types
// /*AFLA*/  * @type_cache:		Cached type register
// /*AFLA*/  * @polarity_cache:	Cached polarity register
// /*AFLA*/  * @wake_enabled:	Interrupt can wakeup from suspend
// /*AFLA*/  * @wake_active:	Interrupt is marked as an wakeup from suspend source
// /*AFLA*/  * @num_ct:		Number of available irq_chip_type instances (usually 1)
// /*AFLA*/  * @private:		Private data for non generic chip callbacks
// /*AFLA*/  * @installed:		bitfield to denote installed interrupts
// /*AFLA*/  * @unused:		bitfield to denote unused interrupts
// /*AFLA*/  * @domain:		irq domain pointer
// /*AFLA*/  * @list:		List head for keeping track of instances
// /*AFLA*/  * @chip_types:		Array of interrupt irq_chip_types
// /*AFLA*/  *
// /*AFLA*/  * Note, that irq_chip_generic can have multiple irq_chip_type
// /*AFLA*/  * implementations which can be associated to a particular irq line of
// /*AFLA*/  * an irq_chip_generic instance. That allows to share and protect
// /*AFLA*/  * state in an irq_chip_generic instance when we need to implement
// /*AFLA*/  * different flow mechanisms (level/edge) for it.
// /*AFLA*/  */
// /*AFLA*/ struct irq_chip_generic {
// /*AFLA*/ 	raw_spinlock_t		lock;
// /*AFLA*/ 	void __iomem		*reg_base;
// /*AFLA*/ 	u32			(*reg_readl)(void __iomem *addr);
// /*AFLA*/ 	void			(*reg_writel)(u32 val, void __iomem *addr);
// /*AFLA*/ 	void			(*suspend)(struct irq_chip_generic *gc);
// /*AFLA*/ 	void			(*resume)(struct irq_chip_generic *gc);
// /*AFLA*/ 	unsigned int		irq_base;
// /*AFLA*/ 	unsigned int		irq_cnt;
// /*AFLA*/ 	u32			mask_cache;
// /*AFLA*/ 	u32			type_cache;
// /*AFLA*/ 	u32			polarity_cache;
// /*AFLA*/ 	u32			wake_enabled;
// /*AFLA*/ 	u32			wake_active;
// /*AFLA*/ 	unsigned int		num_ct;
// /*AFLA*/ 	void			*private;
// /*AFLA*/ 	unsigned long		installed;
// /*AFLA*/ 	unsigned long		unused;
// /*AFLA*/ 	struct irq_domain	*domain;
// /*AFLA*/ 	struct list_head	list;
// /*AFLA*/ 	struct irq_chip_type	chip_types[0];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * enum irq_gc_flags - Initialization flags for generic irq chips
// /*AFLA*/  * @IRQ_GC_INIT_MASK_CACHE:	Initialize the mask_cache by reading mask reg
// /*AFLA*/  * @IRQ_GC_INIT_NESTED_LOCK:	Set the lock class of the irqs to nested for
// /*AFLA*/  *				irq chips which need to call irq_set_wake() on
// /*AFLA*/  *				the parent irq. Usually GPIO implementations
// /*AFLA*/  * @IRQ_GC_MASK_CACHE_PER_TYPE:	Mask cache is chip type private
// /*AFLA*/  * @IRQ_GC_NO_MASK:		Do not calculate irq_data->mask
// /*AFLA*/  * @IRQ_GC_BE_IO:		Use big-endian register accesses (default: LE)
// /*AFLA*/  */
// /*AFLA*/ enum irq_gc_flags {
// /*AFLA*/ 	IRQ_GC_INIT_MASK_CACHE		= 1 << 0,
// /*AFLA*/ 	IRQ_GC_INIT_NESTED_LOCK		= 1 << 1,
// /*AFLA*/ 	IRQ_GC_MASK_CACHE_PER_TYPE	= 1 << 2,
// /*AFLA*/ 	IRQ_GC_NO_MASK			= 1 << 3,
// /*AFLA*/ 	IRQ_GC_BE_IO			= 1 << 4,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * struct irq_domain_chip_generic - Generic irq chip data structure for irq domains
// /*AFLA*/  * @irqs_per_chip:	Number of interrupts per chip
// /*AFLA*/  * @num_chips:		Number of chips
// /*AFLA*/  * @irq_flags_to_set:	IRQ* flags to set on irq setup
// /*AFLA*/  * @irq_flags_to_clear:	IRQ* flags to clear on irq setup
// /*AFLA*/  * @gc_flags:		Generic chip specific setup flags
// /*AFLA*/  * @gc:			Array of pointers to generic interrupt chips
// /*AFLA*/  */
// /*AFLA*/ struct irq_domain_chip_generic {
// /*AFLA*/ 	unsigned int		irqs_per_chip;
// /*AFLA*/ 	unsigned int		num_chips;
// /*AFLA*/ 	unsigned int		irq_flags_to_clear;
// /*AFLA*/ 	unsigned int		irq_flags_to_set;
// /*AFLA*/ 	enum irq_gc_flags	gc_flags;
// /*AFLA*/ 	struct irq_chip_generic	*gc[0];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Generic chip callback functions */
// /*AFLA*/ void irq_gc_noop(struct irq_data *d);
// /*AFLA*/ void irq_gc_mask_disable_reg(struct irq_data *d);
// /*AFLA*/ void irq_gc_mask_set_bit(struct irq_data *d);
// /*AFLA*/ void irq_gc_mask_clr_bit(struct irq_data *d);
// /*AFLA*/ void irq_gc_unmask_enable_reg(struct irq_data *d);
// /*AFLA*/ void irq_gc_ack_set_bit(struct irq_data *d);
// /*AFLA*/ void irq_gc_ack_clr_bit(struct irq_data *d);
// /*AFLA*/ void irq_gc_mask_disable_reg_and_ack(struct irq_data *d);
// /*AFLA*/ void irq_gc_eoi(struct irq_data *d);
// /*AFLA*/ int irq_gc_set_wake(struct irq_data *d, unsigned int on);
// /*AFLA*/ 
// /*AFLA*/ /* Setup functions for irq_chip_generic */
// /*AFLA*/ int irq_map_generic_chip(struct irq_domain *d, unsigned int virq,
// /*AFLA*/ 			 irq_hw_number_t hw_irq);
// /*AFLA*/ struct irq_chip_generic *
// /*AFLA*/ irq_alloc_generic_chip(const char *name, int nr_ct, unsigned int irq_base,
// /*AFLA*/ 		       void __iomem *reg_base, irq_flow_handler_t handler);
// /*AFLA*/ void irq_setup_generic_chip(struct irq_chip_generic *gc, u32 msk,
// /*AFLA*/ 			    enum irq_gc_flags flags, unsigned int clr,
// /*AFLA*/ 			    unsigned int set);
// /*AFLA*/ int irq_setup_alt_chip(struct irq_data *d, unsigned int type);
// /*AFLA*/ void irq_remove_generic_chip(struct irq_chip_generic *gc, u32 msk,
// /*AFLA*/ 			     unsigned int clr, unsigned int set);
// /*AFLA*/ 
// /*AFLA*/ struct irq_chip_generic *irq_get_domain_generic_chip(struct irq_domain *d, unsigned int hw_irq);
// /*AFLA*/ 
// /*AFLA*/ int __irq_alloc_domain_generic_chips(struct irq_domain *d, int irqs_per_chip,
// /*AFLA*/ 				     int num_ct, const char *name,
// /*AFLA*/ 				     irq_flow_handler_t handler,
// /*AFLA*/ 				     unsigned int clr, unsigned int set,
// /*AFLA*/ 				     enum irq_gc_flags flags);
// /*AFLA*/ 
// /*AFLA*/ #define irq_alloc_domain_generic_chips(d, irqs_per_chip, num_ct, name,	\
// /*AFLA*/ 				       handler,	clr, set, flags)	\
// /*AFLA*/ ({									\
// /*AFLA*/ 	MAYBE_BUILD_BUG_ON(irqs_per_chip > 32);				\
// /*AFLA*/ 	__irq_alloc_domain_generic_chips(d, irqs_per_chip, num_ct, name,\
// /*AFLA*/ 					 handler, clr, set, flags);	\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ static inline struct irq_chip_type *irq_data_get_chip_type(struct irq_data *d)
// /*AFLA*/ {
// /*AFLA*/ 	return container_of(d->chip, struct irq_chip_type, chip);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define IRQ_MSK(n) (u32)((n) < 32 ? ((1 << (n)) - 1) : UINT_MAX)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ static inline void irq_gc_lock(struct irq_chip_generic *gc)
// /*AFLA*/ {
// /*AFLA*/ 	raw_spin_lock(&gc->lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void irq_gc_unlock(struct irq_chip_generic *gc)
// /*AFLA*/ {
// /*AFLA*/ 	raw_spin_unlock(&gc->lock);
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline void irq_gc_lock(struct irq_chip_generic *gc) { }
// /*AFLA*/ static inline void irq_gc_unlock(struct irq_chip_generic *gc) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The irqsave variants are for usage in non interrupt code. Do not use
// /*AFLA*/  * them in irq_chip callbacks. Use irq_gc_lock() instead.
// /*AFLA*/  */
// /*AFLA*/ #define irq_gc_lock_irqsave(gc, flags)	\
// /*AFLA*/ 	raw_spin_lock_irqsave(&(gc)->lock, flags)
// /*AFLA*/ 
// /*AFLA*/ #define irq_gc_unlock_irqrestore(gc, flags)	\
// /*AFLA*/ 	raw_spin_unlock_irqrestore(&(gc)->lock, flags)
// /*AFLA*/ 
// /*AFLA*/ static inline void irq_reg_writel(struct irq_chip_generic *gc,
// /*AFLA*/ 				  u32 val, int reg_offset)
// /*AFLA*/ {
// /*AFLA*/ 	if (gc->reg_writel)
// /*AFLA*/ 		gc->reg_writel(val, gc->reg_base + reg_offset);
// /*AFLA*/ 	else
// /*AFLA*/ 		writel(val, gc->reg_base + reg_offset);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u32 irq_reg_readl(struct irq_chip_generic *gc,
// /*AFLA*/ 				int reg_offset)
// /*AFLA*/ {
// /*AFLA*/ 	if (gc->reg_readl)
// /*AFLA*/ 		return gc->reg_readl(gc->reg_base + reg_offset);
// /*AFLA*/ 	else
// /*AFLA*/ 		return readl(gc->reg_base + reg_offset);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Contrary to Linux irqs, for hardware irqs the irq number 0 is valid */
// /*AFLA*/ #define INVALID_HWIRQ	(~0UL)
// /*AFLA*/ irq_hw_number_t ipi_get_hwirq(unsigned int irq, unsigned int cpu);
// /*AFLA*/ int __ipi_send_single(struct irq_desc *desc, unsigned int cpu);
// /*AFLA*/ int __ipi_send_mask(struct irq_desc *desc, const struct cpumask *dest);
// /*AFLA*/ int ipi_send_single(unsigned int virq, unsigned int cpu);
// /*AFLA*/ int ipi_send_mask(unsigned int virq, const struct cpumask *dest);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_IRQ_H */
