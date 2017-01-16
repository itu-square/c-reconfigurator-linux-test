// /*AFLA*/ #ifndef _ASM_X86_SMP_H
// /*AFLA*/ #define _ASM_X86_SMP_H
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ #include <asm/percpu.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We need the APIC definitions automatically as part of 'smp.h'
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_X86_LOCAL_APIC
// /*AFLA*/ # include <asm/mpspec.h>
// /*AFLA*/ # include <asm/apic.h>
// /*AFLA*/ # ifdef CONFIG_X86_IO_APIC
// /*AFLA*/ #  include <asm/io_apic.h>
// /*AFLA*/ # endif
// /*AFLA*/ #endif
// /*AFLA*/ #include <asm/thread_info.h>
// /*AFLA*/ #include <asm/cpumask.h>
// /*AFLA*/ 
// /*AFLA*/ extern int smp_num_siblings;
// /*AFLA*/ extern unsigned int num_processors;
// /*AFLA*/ 
// /*AFLA*/ DECLARE_PER_CPU_READ_MOSTLY(cpumask_var_t, cpu_sibling_map);
// /*AFLA*/ DECLARE_PER_CPU_READ_MOSTLY(cpumask_var_t, cpu_core_map);
// /*AFLA*/ /* cpus sharing the last level cache: */
// /*AFLA*/ DECLARE_PER_CPU_READ_MOSTLY(cpumask_var_t, cpu_llc_shared_map);
// /*AFLA*/ DECLARE_PER_CPU_READ_MOSTLY(u16, cpu_llc_id);
// /*AFLA*/ DECLARE_PER_CPU_READ_MOSTLY(int, cpu_number);
// /*AFLA*/ 
// /*AFLA*/ static inline struct cpumask *cpu_llc_shared_mask(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return per_cpu(cpu_llc_shared_map, cpu);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ DECLARE_EARLY_PER_CPU_READ_MOSTLY(u16, x86_cpu_to_apicid);
// /*AFLA*/ DECLARE_EARLY_PER_CPU_READ_MOSTLY(u32, x86_cpu_to_acpiid);
// /*AFLA*/ DECLARE_EARLY_PER_CPU_READ_MOSTLY(u16, x86_bios_cpu_apicid);
// /*AFLA*/ #if defined(CONFIG_X86_LOCAL_APIC) && defined(CONFIG_X86_32)
// /*AFLA*/ DECLARE_EARLY_PER_CPU_READ_MOSTLY(int, x86_cpu_to_logical_apicid);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct task_struct;
// /*AFLA*/ 
// /*AFLA*/ struct smp_ops {
// /*AFLA*/ 	void (*smp_prepare_boot_cpu)(void);
// /*AFLA*/ 	void (*smp_prepare_cpus)(unsigned max_cpus);
// /*AFLA*/ 	void (*smp_cpus_done)(unsigned max_cpus);
// /*AFLA*/ 
// /*AFLA*/ 	void (*stop_other_cpus)(int wait);
// /*AFLA*/ 	void (*crash_stop_other_cpus)(void);
// /*AFLA*/ 	void (*smp_send_reschedule)(int cpu);
// /*AFLA*/ 
// /*AFLA*/ 	int (*cpu_up)(unsigned cpu, struct task_struct *tidle);
// /*AFLA*/ 	int (*cpu_disable)(void);
// /*AFLA*/ 	void (*cpu_die)(unsigned int cpu);
// /*AFLA*/ 	void (*play_dead)(void);
// /*AFLA*/ 
// /*AFLA*/ 	void (*send_call_func_ipi)(const struct cpumask *mask);
// /*AFLA*/ 	void (*send_call_func_single_ipi)(int cpu);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Globals due to paravirt */
// /*AFLA*/ extern void set_cpu_sibling_map(int cpu);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ extern struct smp_ops smp_ops;
// /*AFLA*/ 
// /*AFLA*/ static inline void smp_send_stop(void)
// /*AFLA*/ {
// /*AFLA*/ 	smp_ops.stop_other_cpus(0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void stop_other_cpus(void)
// /*AFLA*/ {
// /*AFLA*/ 	smp_ops.stop_other_cpus(1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void smp_prepare_boot_cpu(void)
// /*AFLA*/ {
// /*AFLA*/ 	smp_ops.smp_prepare_boot_cpu();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void smp_prepare_cpus(unsigned int max_cpus)
// /*AFLA*/ {
// /*AFLA*/ 	smp_ops.smp_prepare_cpus(max_cpus);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void smp_cpus_done(unsigned int max_cpus)
// /*AFLA*/ {
// /*AFLA*/ 	smp_ops.smp_cpus_done(max_cpus);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __cpu_up(unsigned int cpu, struct task_struct *tidle)
// /*AFLA*/ {
// /*AFLA*/ 	return smp_ops.cpu_up(cpu, tidle);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __cpu_disable(void)
// /*AFLA*/ {
// /*AFLA*/ 	return smp_ops.cpu_disable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __cpu_die(unsigned int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	smp_ops.cpu_die(cpu);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void play_dead(void)
// /*AFLA*/ {
// /*AFLA*/ 	smp_ops.play_dead();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void smp_send_reschedule(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	smp_ops.smp_send_reschedule(cpu);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void arch_send_call_function_single_ipi(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	smp_ops.send_call_func_single_ipi(cpu);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void arch_send_call_function_ipi_mask(const struct cpumask *mask)
// /*AFLA*/ {
// /*AFLA*/ 	smp_ops.send_call_func_ipi(mask);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void cpu_disable_common(void);
// /*AFLA*/ void native_smp_prepare_boot_cpu(void);
// /*AFLA*/ void native_smp_prepare_cpus(unsigned int max_cpus);
// /*AFLA*/ void native_smp_cpus_done(unsigned int max_cpus);
// /*AFLA*/ void common_cpu_up(unsigned int cpunum, struct task_struct *tidle);
// /*AFLA*/ int native_cpu_up(unsigned int cpunum, struct task_struct *tidle);
// /*AFLA*/ int native_cpu_disable(void);
// /*AFLA*/ int common_cpu_die(unsigned int cpu);
// /*AFLA*/ void native_cpu_die(unsigned int cpu);
// /*AFLA*/ void hlt_play_dead(void);
// /*AFLA*/ void native_play_dead(void);
// /*AFLA*/ void play_dead_common(void);
// /*AFLA*/ void wbinvd_on_cpu(int cpu);
// /*AFLA*/ int wbinvd_on_all_cpus(void);
// /*AFLA*/ 
// /*AFLA*/ void native_send_call_func_ipi(const struct cpumask *mask);
// /*AFLA*/ void native_send_call_func_single_ipi(int cpu);
// /*AFLA*/ void x86_idle_thread_init(unsigned int cpu, struct task_struct *idle);
// /*AFLA*/ 
// /*AFLA*/ void smp_store_boot_cpu_info(void);
// /*AFLA*/ void smp_store_cpu_info(int id);
// /*AFLA*/ #define cpu_physical_id(cpu)	per_cpu(x86_cpu_to_apicid, cpu)
// /*AFLA*/ #define cpu_acpi_id(cpu)	per_cpu(x86_cpu_to_acpiid, cpu)
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_SMP */
// /*AFLA*/ #define wbinvd_on_cpu(cpu)     wbinvd()
// /*AFLA*/ static inline int wbinvd_on_all_cpus(void)
// /*AFLA*/ {
// /*AFLA*/ 	wbinvd();
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #define smp_num_siblings	1
// /*AFLA*/ #endif /* CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ extern unsigned disabled_cpus;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32_SMP
// /*AFLA*/ /*
// /*AFLA*/  * This function is needed by all SMP systems. It must _always_ be valid
// /*AFLA*/  * from the initial startup. We map APIC_BASE very early in page_setup(),
// /*AFLA*/  * so this is correct in the x86 case.
// /*AFLA*/  */
// /*AFLA*/ #define raw_smp_processor_id() (this_cpu_read(cpu_number))
// /*AFLA*/ extern int safe_smp_processor_id(void);
// /*AFLA*/ 
// /*AFLA*/ #elif defined(CONFIG_X86_64_SMP)
// /*AFLA*/ #define raw_smp_processor_id() (this_cpu_read(cpu_number))
// /*AFLA*/ 
// /*AFLA*/ #define safe_smp_processor_id()		smp_processor_id()
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_LOCAL_APIC
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_X86_64
// /*AFLA*/ static inline int logical_smp_processor_id(void)
// /*AFLA*/ {
// /*AFLA*/ 	/* we don't want to mark this access volatile - bad code generation */
// /*AFLA*/ 	return GET_APIC_LOGICAL_ID(apic_read(APIC_LDR));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern int hard_smp_processor_id(void);
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_X86_LOCAL_APIC */
// /*AFLA*/ 
// /*AFLA*/ # ifndef CONFIG_SMP
// /*AFLA*/ #  define hard_smp_processor_id()	0
// /*AFLA*/ # endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_X86_LOCAL_APIC */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_NMI_SELFTEST
// /*AFLA*/ extern void nmi_selftest(void);
// /*AFLA*/ #else
// /*AFLA*/ #define nmi_selftest() do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ #endif /* _ASM_X86_SMP_H */
