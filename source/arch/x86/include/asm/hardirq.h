// /*AFLA*/ #ifndef _ASM_X86_HARDIRQ_H
// /*AFLA*/ #define _ASM_X86_HARDIRQ_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/threads.h>
// /*AFLA*/ #include <linux/irq.h>
// /*AFLA*/ 
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	unsigned int __softirq_pending;
// /*AFLA*/ 	unsigned int __nmi_count;	/* arch dependent */
// /*AFLA*/ #ifdef CONFIG_X86_LOCAL_APIC
// /*AFLA*/ 	unsigned int apic_timer_irqs;	/* arch dependent */
// /*AFLA*/ 	unsigned int irq_spurious_count;
// /*AFLA*/ 	unsigned int icr_read_retry_count;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_HAVE_KVM
// /*AFLA*/ 	unsigned int kvm_posted_intr_ipis;
// /*AFLA*/ 	unsigned int kvm_posted_intr_wakeup_ipis;
// /*AFLA*/ #endif
// /*AFLA*/ 	unsigned int x86_platform_ipis;	/* arch dependent */
// /*AFLA*/ 	unsigned int apic_perf_irqs;
// /*AFLA*/ 	unsigned int apic_irq_work_irqs;
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 	unsigned int irq_resched_count;
// /*AFLA*/ 	unsigned int irq_call_count;
// /*AFLA*/ 	unsigned int irq_tlb_count;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_X86_THERMAL_VECTOR
// /*AFLA*/ 	unsigned int irq_thermal_count;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_X86_MCE_THRESHOLD
// /*AFLA*/ 	unsigned int irq_threshold_count;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_X86_MCE_AMD
// /*AFLA*/ 	unsigned int irq_deferred_error_count;
// /*AFLA*/ #endif
// /*AFLA*/ #if IS_ENABLED(CONFIG_HYPERV) || defined(CONFIG_XEN)
// /*AFLA*/ 	unsigned int irq_hv_callback_count;
// /*AFLA*/ #endif
// /*AFLA*/ } ____cacheline_aligned irq_cpustat_t;
// /*AFLA*/ 
// /*AFLA*/ DECLARE_PER_CPU_SHARED_ALIGNED(irq_cpustat_t, irq_stat);
// /*AFLA*/ 
// /*AFLA*/ #define __ARCH_IRQ_STAT
// /*AFLA*/ 
// /*AFLA*/ #define inc_irq_stat(member)	this_cpu_inc(irq_stat.member)
// /*AFLA*/ 
// /*AFLA*/ #define local_softirq_pending()	this_cpu_read(irq_stat.__softirq_pending)
// /*AFLA*/ 
// /*AFLA*/ #define __ARCH_SET_SOFTIRQ_PENDING
// /*AFLA*/ 
// /*AFLA*/ #define set_softirq_pending(x)	\
// /*AFLA*/ 		this_cpu_write(irq_stat.__softirq_pending, (x))
// /*AFLA*/ #define or_softirq_pending(x)	this_cpu_or(irq_stat.__softirq_pending, (x))
// /*AFLA*/ 
// /*AFLA*/ extern void ack_bad_irq(unsigned int irq);
// /*AFLA*/ 
// /*AFLA*/ extern u64 arch_irq_stat_cpu(unsigned int cpu);
// /*AFLA*/ #define arch_irq_stat_cpu	arch_irq_stat_cpu
// /*AFLA*/ 
// /*AFLA*/ extern u64 arch_irq_stat(void);
// /*AFLA*/ #define arch_irq_stat		arch_irq_stat
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_HARDIRQ_H */
