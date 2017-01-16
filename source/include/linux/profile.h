// /*AFLA*/ #ifndef _LINUX_PROFILE_H
// /*AFLA*/ #define _LINUX_PROFILE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/init.h>
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ #include <linux/cache.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/errno.h>
// /*AFLA*/ 
// /*AFLA*/ #define CPU_PROFILING	1
// /*AFLA*/ #define SCHED_PROFILING	2
// /*AFLA*/ #define SLEEP_PROFILING	3
// /*AFLA*/ #define KVM_PROFILING	4
// /*AFLA*/ 
// /*AFLA*/ struct proc_dir_entry;
// /*AFLA*/ struct pt_regs;
// /*AFLA*/ struct notifier_block;
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_PROFILING) && defined(CONFIG_PROC_FS)
// /*AFLA*/ void create_prof_cpu_mask(void);
// /*AFLA*/ int create_proc_profile(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void create_prof_cpu_mask(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int create_proc_profile(void)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ enum profile_type {
// /*AFLA*/ 	PROFILE_TASK_EXIT,
// /*AFLA*/ 	PROFILE_MUNMAP
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PROFILING
// /*AFLA*/ 
// /*AFLA*/ extern int prof_on __read_mostly;
// /*AFLA*/ 
// /*AFLA*/ /* init basic kernel profiler */
// /*AFLA*/ int profile_init(void);
// /*AFLA*/ int profile_setup(char *str);
// /*AFLA*/ void profile_tick(int type);
// /*AFLA*/ int setup_profiling_timer(unsigned int multiplier);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Add multiple profiler hits to a given address:
// /*AFLA*/  */
// /*AFLA*/ void profile_hits(int type, void *ip, unsigned int nr_hits);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Single profiler hit:
// /*AFLA*/  */
// /*AFLA*/ static inline void profile_hit(int type, void *ip)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Speedup for the common (no profiling enabled) case:
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (unlikely(prof_on == type))
// /*AFLA*/ 		profile_hits(type, ip, 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct task_struct;
// /*AFLA*/ struct mm_struct;
// /*AFLA*/ 
// /*AFLA*/ /* task is in do_exit() */
// /*AFLA*/ void profile_task_exit(struct task_struct * task);
// /*AFLA*/ 
// /*AFLA*/ /* task is dead, free task struct ? Returns 1 if
// /*AFLA*/  * the task was taken, 0 if the task should be freed.
// /*AFLA*/  */
// /*AFLA*/ int profile_handoff_task(struct task_struct * task);
// /*AFLA*/ 
// /*AFLA*/ /* sys_munmap */
// /*AFLA*/ void profile_munmap(unsigned long addr);
// /*AFLA*/ 
// /*AFLA*/ int task_handoff_register(struct notifier_block * n);
// /*AFLA*/ int task_handoff_unregister(struct notifier_block * n);
// /*AFLA*/ 
// /*AFLA*/ int profile_event_register(enum profile_type, struct notifier_block * n);
// /*AFLA*/ int profile_event_unregister(enum profile_type, struct notifier_block * n);
// /*AFLA*/ 
// /*AFLA*/ struct pt_regs;
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ #define prof_on 0
// /*AFLA*/ 
// /*AFLA*/ static inline int profile_init(void)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void profile_tick(int type)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void profile_hits(int type, void *ip, unsigned int nr_hits)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void profile_hit(int type, void *ip)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int task_handoff_register(struct notifier_block * n)
// /*AFLA*/ {
// /*AFLA*/ 	return -ENOSYS;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int task_handoff_unregister(struct notifier_block * n)
// /*AFLA*/ {
// /*AFLA*/ 	return -ENOSYS;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int profile_event_register(enum profile_type t, struct notifier_block * n)
// /*AFLA*/ {
// /*AFLA*/ 	return -ENOSYS;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int profile_event_unregister(enum profile_type t, struct notifier_block * n)
// /*AFLA*/ {
// /*AFLA*/ 	return -ENOSYS;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define profile_task_exit(a) do { } while (0)
// /*AFLA*/ #define profile_handoff_task(a) (0)
// /*AFLA*/ #define profile_munmap(a) do { } while (0)
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_PROFILING */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_PROFILE_H */
