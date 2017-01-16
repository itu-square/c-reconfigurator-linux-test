// /*AFLA*/ /*
// /*AFLA*/  * include/linux/cpu.h - generic cpu definition
// /*AFLA*/  *
// /*AFLA*/  * This is mainly for topological representation. We define the 
// /*AFLA*/  * basic 'struct cpu' here, which can be embedded in per-arch 
// /*AFLA*/  * definitions of processors.
// /*AFLA*/  *
// /*AFLA*/  * Basic handling of the devices is done in drivers/base/cpu.c
// /*AFLA*/  *
// /*AFLA*/  * CPUs are exported via sysfs in the devices/system/cpu
// /*AFLA*/  * directory. 
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LINUX_CPU_H_
// /*AFLA*/ #define _LINUX_CPU_H_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/node.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ #include <linux/cpuhotplug.h>
// /*AFLA*/ 
// /*AFLA*/ struct device;
// /*AFLA*/ struct device_node;
// /*AFLA*/ struct attribute_group;
// /*AFLA*/ 
// /*AFLA*/ struct cpu {
// /*AFLA*/ 	int node_id;		/* The node which contains the CPU */
// /*AFLA*/ 	int hotpluggable;	/* creates sysfs control file if hotpluggable */
// /*AFLA*/ 	struct device dev;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern void boot_cpu_init(void);
// /*AFLA*/ extern void boot_cpu_state_init(void);
// /*AFLA*/ 
// /*AFLA*/ extern int register_cpu(struct cpu *cpu, int num);
// /*AFLA*/ extern struct device *get_cpu_device(unsigned cpu);
// /*AFLA*/ extern bool cpu_is_hotpluggable(unsigned cpu);
// /*AFLA*/ extern bool arch_match_cpu_phys_id(int cpu, u64 phys_id);
// /*AFLA*/ extern bool arch_find_n_match_cpu_physical_id(struct device_node *cpun,
// /*AFLA*/ 					      int cpu, unsigned int *thread);
// /*AFLA*/ 
// /*AFLA*/ extern int cpu_add_dev_attr(struct device_attribute *attr);
// /*AFLA*/ extern void cpu_remove_dev_attr(struct device_attribute *attr);
// /*AFLA*/ 
// /*AFLA*/ extern int cpu_add_dev_attr_group(struct attribute_group *attrs);
// /*AFLA*/ extern void cpu_remove_dev_attr_group(struct attribute_group *attrs);
// /*AFLA*/ 
// /*AFLA*/ extern __printf(4, 5)
// /*AFLA*/ struct device *cpu_device_create(struct device *parent, void *drvdata,
// /*AFLA*/ 				 const struct attribute_group **groups,
// /*AFLA*/ 				 const char *fmt, ...);
// /*AFLA*/ #ifdef CONFIG_HOTPLUG_CPU
// /*AFLA*/ extern void unregister_cpu(struct cpu *cpu);
// /*AFLA*/ extern ssize_t arch_cpu_probe(const char *, size_t);
// /*AFLA*/ extern ssize_t arch_cpu_release(const char *, size_t);
// /*AFLA*/ #endif
// /*AFLA*/ struct notifier_block;
// /*AFLA*/ 
// /*AFLA*/ #define CPU_ONLINE		0x0002 /* CPU (unsigned)v is up */
// /*AFLA*/ #define CPU_UP_PREPARE		0x0003 /* CPU (unsigned)v coming up */
// /*AFLA*/ #define CPU_UP_CANCELED		0x0004 /* CPU (unsigned)v NOT coming up */
// /*AFLA*/ #define CPU_DOWN_PREPARE	0x0005 /* CPU (unsigned)v going down */
// /*AFLA*/ #define CPU_DOWN_FAILED		0x0006 /* CPU (unsigned)v NOT going down */
// /*AFLA*/ #define CPU_DEAD		0x0007 /* CPU (unsigned)v dead */
// /*AFLA*/ #define CPU_POST_DEAD		0x0009 /* CPU (unsigned)v dead, cpu_hotplug
// /*AFLA*/ 					* lock is dropped */
// /*AFLA*/ #define CPU_BROKEN		0x000B /* CPU (unsigned)v did not die properly,
// /*AFLA*/ 					* perhaps due to preemption. */
// /*AFLA*/ 
// /*AFLA*/ /* Used for CPU hotplug events occurring while tasks are frozen due to a suspend
// /*AFLA*/  * operation in progress
// /*AFLA*/  */
// /*AFLA*/ #define CPU_TASKS_FROZEN	0x0010
// /*AFLA*/ 
// /*AFLA*/ #define CPU_ONLINE_FROZEN	(CPU_ONLINE | CPU_TASKS_FROZEN)
// /*AFLA*/ #define CPU_UP_PREPARE_FROZEN	(CPU_UP_PREPARE | CPU_TASKS_FROZEN)
// /*AFLA*/ #define CPU_UP_CANCELED_FROZEN	(CPU_UP_CANCELED | CPU_TASKS_FROZEN)
// /*AFLA*/ #define CPU_DOWN_PREPARE_FROZEN	(CPU_DOWN_PREPARE | CPU_TASKS_FROZEN)
// /*AFLA*/ #define CPU_DOWN_FAILED_FROZEN	(CPU_DOWN_FAILED | CPU_TASKS_FROZEN)
// /*AFLA*/ #define CPU_DEAD_FROZEN		(CPU_DEAD | CPU_TASKS_FROZEN)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ extern bool cpuhp_tasks_frozen;
// /*AFLA*/ /* Need to know about CPUs going up/down? */
// /*AFLA*/ #if defined(CONFIG_HOTPLUG_CPU) || !defined(MODULE)
// /*AFLA*/ #define cpu_notifier(fn, pri) {					\
// /*AFLA*/ 	static struct notifier_block fn##_nb =			\
// /*AFLA*/ 		{ .notifier_call = fn, .priority = pri };	\
// /*AFLA*/ 	register_cpu_notifier(&fn##_nb);			\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __cpu_notifier(fn, pri) {				\
// /*AFLA*/ 	static struct notifier_block fn##_nb =			\
// /*AFLA*/ 		{ .notifier_call = fn, .priority = pri };	\
// /*AFLA*/ 	__register_cpu_notifier(&fn##_nb);			\
// /*AFLA*/ }
// /*AFLA*/ #else /* #if defined(CONFIG_HOTPLUG_CPU) || !defined(MODULE) */
// /*AFLA*/ #define cpu_notifier(fn, pri)	do { (void)(fn); } while (0)
// /*AFLA*/ #define __cpu_notifier(fn, pri)	do { (void)(fn); } while (0)
// /*AFLA*/ #endif /* #else #if defined(CONFIG_HOTPLUG_CPU) || !defined(MODULE) */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HOTPLUG_CPU
// /*AFLA*/ extern int register_cpu_notifier(struct notifier_block *nb);
// /*AFLA*/ extern int __register_cpu_notifier(struct notifier_block *nb);
// /*AFLA*/ extern void unregister_cpu_notifier(struct notifier_block *nb);
// /*AFLA*/ extern void __unregister_cpu_notifier(struct notifier_block *nb);
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ #ifndef MODULE
// /*AFLA*/ extern int register_cpu_notifier(struct notifier_block *nb);
// /*AFLA*/ extern int __register_cpu_notifier(struct notifier_block *nb);
// /*AFLA*/ #else
// /*AFLA*/ static inline int register_cpu_notifier(struct notifier_block *nb)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __register_cpu_notifier(struct notifier_block *nb)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline void unregister_cpu_notifier(struct notifier_block *nb)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __unregister_cpu_notifier(struct notifier_block *nb)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ int cpu_up(unsigned int cpu);
// /*AFLA*/ void notify_cpu_starting(unsigned int cpu);
// /*AFLA*/ extern void cpu_maps_update_begin(void);
// /*AFLA*/ extern void cpu_maps_update_done(void);
// /*AFLA*/ 
// /*AFLA*/ #define cpu_notifier_register_begin	cpu_maps_update_begin
// /*AFLA*/ #define cpu_notifier_register_done	cpu_maps_update_done
// /*AFLA*/ 
// /*AFLA*/ #else	/* CONFIG_SMP */
// /*AFLA*/ #define cpuhp_tasks_frozen	0
// /*AFLA*/ 
// /*AFLA*/ #define cpu_notifier(fn, pri)	do { (void)(fn); } while (0)
// /*AFLA*/ #define __cpu_notifier(fn, pri)	do { (void)(fn); } while (0)
// /*AFLA*/ 
// /*AFLA*/ static inline int register_cpu_notifier(struct notifier_block *nb)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __register_cpu_notifier(struct notifier_block *nb)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void unregister_cpu_notifier(struct notifier_block *nb)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __unregister_cpu_notifier(struct notifier_block *nb)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void cpu_maps_update_begin(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void cpu_maps_update_done(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void cpu_notifier_register_begin(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void cpu_notifier_register_done(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_SMP */
// /*AFLA*/ extern struct bus_type cpu_subsys;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HOTPLUG_CPU
// /*AFLA*/ /* Stop CPUs going up and down. */
// /*AFLA*/ 
// /*AFLA*/ extern void cpu_hotplug_begin(void);
// /*AFLA*/ extern void cpu_hotplug_done(void);
// /*AFLA*/ extern void get_online_cpus(void);
// /*AFLA*/ extern void put_online_cpus(void);
// /*AFLA*/ extern void cpu_hotplug_disable(void);
// /*AFLA*/ extern void cpu_hotplug_enable(void);
// /*AFLA*/ #define hotcpu_notifier(fn, pri)	cpu_notifier(fn, pri)
// /*AFLA*/ #define __hotcpu_notifier(fn, pri)	__cpu_notifier(fn, pri)
// /*AFLA*/ #define register_hotcpu_notifier(nb)	register_cpu_notifier(nb)
// /*AFLA*/ #define __register_hotcpu_notifier(nb)	__register_cpu_notifier(nb)
// /*AFLA*/ #define unregister_hotcpu_notifier(nb)	unregister_cpu_notifier(nb)
// /*AFLA*/ #define __unregister_hotcpu_notifier(nb)	__unregister_cpu_notifier(nb)
// /*AFLA*/ void clear_tasks_mm_cpumask(int cpu);
// /*AFLA*/ int cpu_down(unsigned int cpu);
// /*AFLA*/ 
// /*AFLA*/ #else		/* CONFIG_HOTPLUG_CPU */
// /*AFLA*/ 
// /*AFLA*/ static inline void cpu_hotplug_begin(void) {}
// /*AFLA*/ static inline void cpu_hotplug_done(void) {}
// /*AFLA*/ #define get_online_cpus()	do { } while (0)
// /*AFLA*/ #define put_online_cpus()	do { } while (0)
// /*AFLA*/ #define cpu_hotplug_disable()	do { } while (0)
// /*AFLA*/ #define cpu_hotplug_enable()	do { } while (0)
// /*AFLA*/ #define hotcpu_notifier(fn, pri)	do { (void)(fn); } while (0)
// /*AFLA*/ #define __hotcpu_notifier(fn, pri)	do { (void)(fn); } while (0)
// /*AFLA*/ /* These aren't inline functions due to a GCC bug. */
// /*AFLA*/ #define register_hotcpu_notifier(nb)	({ (void)(nb); 0; })
// /*AFLA*/ #define __register_hotcpu_notifier(nb)	({ (void)(nb); 0; })
// /*AFLA*/ #define unregister_hotcpu_notifier(nb)	({ (void)(nb); })
// /*AFLA*/ #define __unregister_hotcpu_notifier(nb)	({ (void)(nb); })
// /*AFLA*/ #endif		/* CONFIG_HOTPLUG_CPU */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PM_SLEEP_SMP
// /*AFLA*/ extern int freeze_secondary_cpus(int primary);
// /*AFLA*/ static inline int disable_nonboot_cpus(void)
// /*AFLA*/ {
// /*AFLA*/ 	return freeze_secondary_cpus(0);
// /*AFLA*/ }
// /*AFLA*/ extern void enable_nonboot_cpus(void);
// /*AFLA*/ #else /* !CONFIG_PM_SLEEP_SMP */
// /*AFLA*/ static inline int disable_nonboot_cpus(void) { return 0; }
// /*AFLA*/ static inline void enable_nonboot_cpus(void) {}
// /*AFLA*/ #endif /* !CONFIG_PM_SLEEP_SMP */
// /*AFLA*/ 
// /*AFLA*/ void cpu_startup_entry(enum cpuhp_state state);
// /*AFLA*/ 
// /*AFLA*/ void cpu_idle_poll_ctrl(bool enable);
// /*AFLA*/ 
// /*AFLA*/ /* Attach to any functions which should be considered cpuidle. */
// /*AFLA*/ #define __cpuidle	__attribute__((__section__(".cpuidle.text")))
// /*AFLA*/ 
// /*AFLA*/ bool cpu_in_idle(unsigned long pc);
// /*AFLA*/ 
// /*AFLA*/ void arch_cpu_idle(void);
// /*AFLA*/ void arch_cpu_idle_prepare(void);
// /*AFLA*/ void arch_cpu_idle_enter(void);
// /*AFLA*/ void arch_cpu_idle_exit(void);
// /*AFLA*/ void arch_cpu_idle_dead(void);
// /*AFLA*/ 
// /*AFLA*/ int cpu_report_state(int cpu);
// /*AFLA*/ int cpu_check_up_prepare(int cpu);
// /*AFLA*/ void cpu_set_state_online(int cpu);
// /*AFLA*/ #ifdef CONFIG_HOTPLUG_CPU
// /*AFLA*/ bool cpu_wait_death(unsigned int cpu, int seconds);
// /*AFLA*/ bool cpu_report_death(void);
// /*AFLA*/ void cpuhp_report_idle_dead(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void cpuhp_report_idle_dead(void) { }
// /*AFLA*/ #endif /* #ifdef CONFIG_HOTPLUG_CPU */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_CPU_H_ */
