// /*AFLA*/ #ifndef __LINUX_SMP_H
// /*AFLA*/ #define __LINUX_SMP_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  *	Generic SMP support
// /*AFLA*/  *		Alan Cox. <alan@redhat.com>
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ #include <linux/init.h>
// /*AFLA*/ #include <linux/llist.h>
// /*AFLA*/ 
// /*AFLA*/ typedef void (*smp_call_func_t)(void *info);
// /*AFLA*/ struct call_single_data {
// /*AFLA*/ 	struct llist_node llist;
// /*AFLA*/ 	smp_call_func_t func;
// /*AFLA*/ 	void *info;
// /*AFLA*/ 	unsigned int flags;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* total number of cpus in this system (may exceed NR_CPUS) */
// /*AFLA*/ extern unsigned int total_cpus;
// /*AFLA*/ 
// /*AFLA*/ int smp_call_function_single(int cpuid, smp_call_func_t func, void *info,
// /*AFLA*/ 			     int wait);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Call a function on all processors
// /*AFLA*/  */
// /*AFLA*/ int on_each_cpu(smp_call_func_t func, void *info, int wait);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Call a function on processors specified by mask, which might include
// /*AFLA*/  * the local one.
// /*AFLA*/  */
// /*AFLA*/ void on_each_cpu_mask(const struct cpumask *mask, smp_call_func_t func,
// /*AFLA*/ 		void *info, bool wait);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Call a function on each processor for which the supplied function
// /*AFLA*/  * cond_func returns a positive value. This may include the local
// /*AFLA*/  * processor.
// /*AFLA*/  */
// /*AFLA*/ void on_each_cpu_cond(bool (*cond_func)(int cpu, void *info),
// /*AFLA*/ 		smp_call_func_t func, void *info, bool wait,
// /*AFLA*/ 		gfp_t gfp_flags);
// /*AFLA*/ 
// /*AFLA*/ int smp_call_function_single_async(int cpu, struct call_single_data *csd);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 
// /*AFLA*/ #include <linux/preempt.h>
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/thread_info.h>
// /*AFLA*/ #include <asm/smp.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * main cross-CPU interfaces, handles INIT, TLB flush, STOP, etc.
// /*AFLA*/  * (defined in asm header):
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * stops all CPUs but the current one:
// /*AFLA*/  */
// /*AFLA*/ extern void smp_send_stop(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * sends a 'reschedule' event to another CPU:
// /*AFLA*/  */
// /*AFLA*/ extern void smp_send_reschedule(int cpu);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Prepare machine for booting other CPUs.
// /*AFLA*/  */
// /*AFLA*/ extern void smp_prepare_cpus(unsigned int max_cpus);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Bring a CPU up
// /*AFLA*/  */
// /*AFLA*/ extern int __cpu_up(unsigned int cpunum, struct task_struct *tidle);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Final polishing of CPUs
// /*AFLA*/  */
// /*AFLA*/ extern void smp_cpus_done(unsigned int max_cpus);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Call a function on all other processors
// /*AFLA*/  */
// /*AFLA*/ int smp_call_function(smp_call_func_t func, void *info, int wait);
// /*AFLA*/ void smp_call_function_many(const struct cpumask *mask,
// /*AFLA*/ 			    smp_call_func_t func, void *info, bool wait);
// /*AFLA*/ 
// /*AFLA*/ int smp_call_function_any(const struct cpumask *mask,
// /*AFLA*/ 			  smp_call_func_t func, void *info, int wait);
// /*AFLA*/ 
// /*AFLA*/ void kick_all_cpus_sync(void);
// /*AFLA*/ void wake_up_all_idle_cpus(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Generic and arch helpers
// /*AFLA*/  */
// /*AFLA*/ void __init call_function_init(void);
// /*AFLA*/ void generic_smp_call_function_single_interrupt(void);
// /*AFLA*/ #define generic_smp_call_function_interrupt \
// /*AFLA*/ 	generic_smp_call_function_single_interrupt
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Mark the boot cpu "online" so that it can call console drivers in
// /*AFLA*/  * printk() and can access its per-cpu storage.
// /*AFLA*/  */
// /*AFLA*/ void smp_prepare_boot_cpu(void);
// /*AFLA*/ 
// /*AFLA*/ extern unsigned int setup_max_cpus;
// /*AFLA*/ extern void __init setup_nr_cpu_ids(void);
// /*AFLA*/ extern void __init smp_init(void);
// /*AFLA*/ 
// /*AFLA*/ #else /* !SMP */
// /*AFLA*/ 
// /*AFLA*/ static inline void smp_send_stop(void) { }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  *	These macros fold the SMP functionality into a single CPU system
// /*AFLA*/  */
// /*AFLA*/ #define raw_smp_processor_id()			0
// /*AFLA*/ static inline int up_smp_call_function(smp_call_func_t func, void *info)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #define smp_call_function(func, info, wait) \
// /*AFLA*/ 			(up_smp_call_function(func, info))
// /*AFLA*/ 
// /*AFLA*/ static inline void smp_send_reschedule(int cpu) { }
// /*AFLA*/ #define smp_prepare_boot_cpu()			do {} while (0)
// /*AFLA*/ #define smp_call_function_many(mask, func, info, wait) \
// /*AFLA*/ 			(up_smp_call_function(func, info))
// /*AFLA*/ static inline void call_function_init(void) { }
// /*AFLA*/ 
// /*AFLA*/ static inline int
// /*AFLA*/ smp_call_function_any(const struct cpumask *mask, smp_call_func_t func,
// /*AFLA*/ 		      void *info, int wait)
// /*AFLA*/ {
// /*AFLA*/ 	return smp_call_function_single(0, func, info, wait);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void kick_all_cpus_sync(void) {  }
// /*AFLA*/ static inline void wake_up_all_idle_cpus(void) {  }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_UP_LATE_INIT
// /*AFLA*/ extern void __init up_late_init(void);
// /*AFLA*/ static inline void smp_init(void) { up_late_init(); }
// /*AFLA*/ #else
// /*AFLA*/ static inline void smp_init(void) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* !SMP */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * smp_processor_id(): get the current CPU ID.
// /*AFLA*/  *
// /*AFLA*/  * if DEBUG_PREEMPT is enabled then we check whether it is
// /*AFLA*/  * used in a preemption-safe way. (smp_processor_id() is safe
// /*AFLA*/  * if it's used in a preemption-off critical section, or in
// /*AFLA*/  * a thread that is bound to the current CPU.)
// /*AFLA*/  *
// /*AFLA*/  * NOTE: raw_smp_processor_id() is for internal use only
// /*AFLA*/  * (smp_processor_id() is the preferred variant), but in rare
// /*AFLA*/  * instances it might also be used to turn off false positives
// /*AFLA*/  * (i.e. smp_processor_id() use that the debugging code reports but
// /*AFLA*/  * which use for some reason is legal). Don't use this to hack around
// /*AFLA*/  * the warning message, as your code might not work under PREEMPT.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_DEBUG_PREEMPT
// /*AFLA*/   extern unsigned int debug_smp_processor_id(void);
// /*AFLA*/ # define smp_processor_id() debug_smp_processor_id()
// /*AFLA*/ #else
// /*AFLA*/ # define smp_processor_id() raw_smp_processor_id()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define get_cpu()		({ preempt_disable(); smp_processor_id(); })
// /*AFLA*/ #define put_cpu()		preempt_enable()
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Callback to arch code if there's nosmp or maxcpus=0 on the
// /*AFLA*/  * boot command line:
// /*AFLA*/  */
// /*AFLA*/ extern void arch_disable_smp_support(void);
// /*AFLA*/ 
// /*AFLA*/ extern void arch_enable_nonboot_cpus_begin(void);
// /*AFLA*/ extern void arch_enable_nonboot_cpus_end(void);
// /*AFLA*/ 
// /*AFLA*/ void smp_setup_processor_id(void);
// /*AFLA*/ 
// /*AFLA*/ int smp_call_on_cpu(unsigned int cpu, int (*func)(void *), void *par,
// /*AFLA*/ 		    bool phys);
// /*AFLA*/ 
// /*AFLA*/ /* SMP core functions */
// /*AFLA*/ int smpcfd_prepare_cpu(unsigned int cpu);
// /*AFLA*/ int smpcfd_dead_cpu(unsigned int cpu);
// /*AFLA*/ int smpcfd_dying_cpu(unsigned int cpu);
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_SMP_H */
