// /*AFLA*/ #ifndef _ASM_X86_HW_IRQ_H
// /*AFLA*/ #define _ASM_X86_HW_IRQ_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * (C) 1992, 1993 Linus Torvalds, (C) 1997 Ingo Molnar
// /*AFLA*/  *
// /*AFLA*/  * moved some of the old arch/i386/kernel/irq.h to here. VY
// /*AFLA*/  *
// /*AFLA*/  * IRQ/IPI changes taken from work by Thomas Radke
// /*AFLA*/  * <tomsoft@informatik.tu-chemnitz.de>
// /*AFLA*/  *
// /*AFLA*/  * hacked by Andi Kleen for x86-64.
// /*AFLA*/  * unified by tglx
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <asm/irq_vectors.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #include <linux/percpu.h>
// /*AFLA*/ #include <linux/profile.h>
// /*AFLA*/ #include <linux/smp.h>
// /*AFLA*/ 
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <asm/irq.h>
// /*AFLA*/ #include <asm/sections.h>
// /*AFLA*/ 
// /*AFLA*/ /* Interrupt handlers registered during init_IRQ */
// /*AFLA*/ extern asmlinkage void apic_timer_interrupt(void);
// /*AFLA*/ extern asmlinkage void x86_platform_ipi(void);
// /*AFLA*/ extern asmlinkage void kvm_posted_intr_ipi(void);
// /*AFLA*/ extern asmlinkage void kvm_posted_intr_wakeup_ipi(void);
// /*AFLA*/ extern asmlinkage void error_interrupt(void);
// /*AFLA*/ extern asmlinkage void irq_work_interrupt(void);
// /*AFLA*/ 
// /*AFLA*/ extern asmlinkage void spurious_interrupt(void);
// /*AFLA*/ extern asmlinkage void thermal_interrupt(void);
// /*AFLA*/ extern asmlinkage void reschedule_interrupt(void);
// /*AFLA*/ 
// /*AFLA*/ extern asmlinkage void irq_move_cleanup_interrupt(void);
// /*AFLA*/ extern asmlinkage void reboot_interrupt(void);
// /*AFLA*/ extern asmlinkage void threshold_interrupt(void);
// /*AFLA*/ extern asmlinkage void deferred_error_interrupt(void);
// /*AFLA*/ 
// /*AFLA*/ extern asmlinkage void call_function_interrupt(void);
// /*AFLA*/ extern asmlinkage void call_function_single_interrupt(void);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TRACING
// /*AFLA*/ /* Interrupt handlers registered during init_IRQ */
// /*AFLA*/ extern void trace_apic_timer_interrupt(void);
// /*AFLA*/ extern void trace_x86_platform_ipi(void);
// /*AFLA*/ extern void trace_error_interrupt(void);
// /*AFLA*/ extern void trace_irq_work_interrupt(void);
// /*AFLA*/ extern void trace_spurious_interrupt(void);
// /*AFLA*/ extern void trace_thermal_interrupt(void);
// /*AFLA*/ extern void trace_reschedule_interrupt(void);
// /*AFLA*/ extern void trace_threshold_interrupt(void);
// /*AFLA*/ extern void trace_deferred_error_interrupt(void);
// /*AFLA*/ extern void trace_call_function_interrupt(void);
// /*AFLA*/ extern void trace_call_function_single_interrupt(void);
// /*AFLA*/ #define trace_irq_move_cleanup_interrupt  irq_move_cleanup_interrupt
// /*AFLA*/ #define trace_reboot_interrupt  reboot_interrupt
// /*AFLA*/ #define trace_kvm_posted_intr_ipi kvm_posted_intr_ipi
// /*AFLA*/ #define trace_kvm_posted_intr_wakeup_ipi kvm_posted_intr_wakeup_ipi
// /*AFLA*/ #endif /* CONFIG_TRACING */
// /*AFLA*/ 
// /*AFLA*/ #ifdef	CONFIG_X86_LOCAL_APIC
// /*AFLA*/ struct irq_data;
// /*AFLA*/ struct pci_dev;
// /*AFLA*/ struct msi_desc;
// /*AFLA*/ 
// /*AFLA*/ enum irq_alloc_type {
// /*AFLA*/ 	X86_IRQ_ALLOC_TYPE_IOAPIC = 1,
// /*AFLA*/ 	X86_IRQ_ALLOC_TYPE_HPET,
// /*AFLA*/ 	X86_IRQ_ALLOC_TYPE_MSI,
// /*AFLA*/ 	X86_IRQ_ALLOC_TYPE_MSIX,
// /*AFLA*/ 	X86_IRQ_ALLOC_TYPE_DMAR,
// /*AFLA*/ 	X86_IRQ_ALLOC_TYPE_UV,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct irq_alloc_info {
// /*AFLA*/ 	enum irq_alloc_type	type;
// /*AFLA*/ 	u32			flags;
// /*AFLA*/ 	const struct cpumask	*mask;	/* CPU mask for vector allocation */
// /*AFLA*/ 	union {
// /*AFLA*/ 		int		unused;
// /*AFLA*/ #ifdef	CONFIG_HPET_TIMER
// /*AFLA*/ 		struct {
// /*AFLA*/ 			int		hpet_id;
// /*AFLA*/ 			int		hpet_index;
// /*AFLA*/ 			void		*hpet_data;
// /*AFLA*/ 		};
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef	CONFIG_PCI_MSI
// /*AFLA*/ 		struct {
// /*AFLA*/ 			struct pci_dev	*msi_dev;
// /*AFLA*/ 			irq_hw_number_t	msi_hwirq;
// /*AFLA*/ 		};
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef	CONFIG_X86_IO_APIC
// /*AFLA*/ 		struct {
// /*AFLA*/ 			int		ioapic_id;
// /*AFLA*/ 			int		ioapic_pin;
// /*AFLA*/ 			int		ioapic_node;
// /*AFLA*/ 			u32		ioapic_trigger : 1;
// /*AFLA*/ 			u32		ioapic_polarity : 1;
// /*AFLA*/ 			u32		ioapic_valid : 1;
// /*AFLA*/ 			struct IO_APIC_route_entry *ioapic_entry;
// /*AFLA*/ 		};
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef	CONFIG_DMAR_TABLE
// /*AFLA*/ 		struct {
// /*AFLA*/ 			int		dmar_id;
// /*AFLA*/ 			void		*dmar_data;
// /*AFLA*/ 		};
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef	CONFIG_HT_IRQ
// /*AFLA*/ 		struct {
// /*AFLA*/ 			int		ht_pos;
// /*AFLA*/ 			int		ht_idx;
// /*AFLA*/ 			struct pci_dev	*ht_dev;
// /*AFLA*/ 			void		*ht_update;
// /*AFLA*/ 		};
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef	CONFIG_X86_UV
// /*AFLA*/ 		struct {
// /*AFLA*/ 			int		uv_limit;
// /*AFLA*/ 			int		uv_blade;
// /*AFLA*/ 			unsigned long	uv_offset;
// /*AFLA*/ 			char		*uv_name;
// /*AFLA*/ 		};
// /*AFLA*/ #endif
// /*AFLA*/ #if IS_ENABLED(CONFIG_VMD)
// /*AFLA*/ 		struct {
// /*AFLA*/ 			struct msi_desc *desc;
// /*AFLA*/ 		};
// /*AFLA*/ #endif
// /*AFLA*/ 	};
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct irq_cfg {
// /*AFLA*/ 	unsigned int		dest_apicid;
// /*AFLA*/ 	u8			vector;
// /*AFLA*/ 	u8			old_vector;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct irq_cfg *irq_cfg(unsigned int irq);
// /*AFLA*/ extern struct irq_cfg *irqd_cfg(struct irq_data *irq_data);
// /*AFLA*/ extern void lock_vector_lock(void);
// /*AFLA*/ extern void unlock_vector_lock(void);
// /*AFLA*/ extern void setup_vector_irq(int cpu);
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ extern void send_cleanup_vector(struct irq_cfg *);
// /*AFLA*/ extern void irq_complete_move(struct irq_cfg *cfg);
// /*AFLA*/ #else
// /*AFLA*/ static inline void send_cleanup_vector(struct irq_cfg *c) { }
// /*AFLA*/ static inline void irq_complete_move(struct irq_cfg *c) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void apic_ack_edge(struct irq_data *data);
// /*AFLA*/ #else	/*  CONFIG_X86_LOCAL_APIC */
// /*AFLA*/ static inline void lock_vector_lock(void) {}
// /*AFLA*/ static inline void unlock_vector_lock(void) {}
// /*AFLA*/ #endif	/* CONFIG_X86_LOCAL_APIC */
// /*AFLA*/ 
// /*AFLA*/ /* Statistics */
// /*AFLA*/ extern atomic_t irq_err_count;
// /*AFLA*/ extern atomic_t irq_mis_count;
// /*AFLA*/ 
// /*AFLA*/ extern void elcr_set_level_irq(unsigned int irq);
// /*AFLA*/ 
// /*AFLA*/ extern char irq_entries_start[];
// /*AFLA*/ #ifdef CONFIG_TRACING
// /*AFLA*/ #define trace_irq_entries_start irq_entries_start
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define VECTOR_UNUSED		NULL
// /*AFLA*/ #define VECTOR_RETRIGGERED	((void *)~0UL)
// /*AFLA*/ 
// /*AFLA*/ typedef struct irq_desc* vector_irq_t[NR_VECTORS];
// /*AFLA*/ DECLARE_PER_CPU(vector_irq_t, vector_irq);
// /*AFLA*/ 
// /*AFLA*/ #endif /* !ASSEMBLY_ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_HW_IRQ_H */
