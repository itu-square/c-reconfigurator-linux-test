// /*AFLA*/ #ifndef _LINUX_SCHED_H
// /*AFLA*/ #define _LINUX_SCHED_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/sched.h>
// /*AFLA*/ 
// /*AFLA*/ #include <linux/sched/prio.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ struct sched_param {
// /*AFLA*/ 	int sched_priority;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #include <asm/param.h>	/* for HZ */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/capability.h>
// /*AFLA*/ #include <linux/threads.h>
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/timex.h>
// /*AFLA*/ #include <linux/jiffies.h>
// /*AFLA*/ #include <linux/plist.h>
// /*AFLA*/ #include <linux/rbtree.h>
// /*AFLA*/ #include <linux/thread_info.h>
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ #include <linux/nodemask.h>
// /*AFLA*/ #include <linux/mm_types.h>
// /*AFLA*/ #include <linux/preempt.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/page.h>
// /*AFLA*/ #include <asm/ptrace.h>
// /*AFLA*/ #include <linux/cputime.h>
// /*AFLA*/ 
// /*AFLA*/ #include <linux/smp.h>
// /*AFLA*/ #include <linux/sem.h>
// /*AFLA*/ #include <linux/shm.h>
// /*AFLA*/ #include <linux/signal.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/completion.h>
// /*AFLA*/ #include <linux/pid.h>
// /*AFLA*/ #include <linux/percpu.h>
// /*AFLA*/ #include <linux/topology.h>
// /*AFLA*/ #include <linux/seccomp.h>
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ #include <linux/rculist.h>
// /*AFLA*/ #include <linux/rtmutex.h>
// /*AFLA*/ 
// /*AFLA*/ #include <linux/time.h>
// /*AFLA*/ #include <linux/param.h>
// /*AFLA*/ #include <linux/resource.h>
// /*AFLA*/ #include <linux/timer.h>
// /*AFLA*/ #include <linux/hrtimer.h>
// /*AFLA*/ #include <linux/kcov.h>
// /*AFLA*/ #include <linux/task_io_accounting.h>
// /*AFLA*/ #include <linux/latencytop.h>
// /*AFLA*/ #include <linux/cred.h>
// /*AFLA*/ #include <linux/llist.h>
// /*AFLA*/ #include <linux/uidgid.h>
// /*AFLA*/ #include <linux/gfp.h>
// /*AFLA*/ #include <linux/magic.h>
// /*AFLA*/ #include <linux/cgroup-defs.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/processor.h>
// /*AFLA*/ 
// /*AFLA*/ #define SCHED_ATTR_SIZE_VER0	48	/* sizeof first published struct */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Extended scheduling parameters data structure.
// /*AFLA*/  *
// /*AFLA*/  * This is needed because the original struct sched_param can not be
// /*AFLA*/  * altered without introducing ABI issues with legacy applications
// /*AFLA*/  * (e.g., in sched_getparam()).
// /*AFLA*/  *
// /*AFLA*/  * However, the possibility of specifying more than just a priority for
// /*AFLA*/  * the tasks may be useful for a wide variety of application fields, e.g.,
// /*AFLA*/  * multimedia, streaming, automation and control, and many others.
// /*AFLA*/  *
// /*AFLA*/  * This variant (sched_attr) is meant at describing a so-called
// /*AFLA*/  * sporadic time-constrained task. In such model a task is specified by:
// /*AFLA*/  *  - the activation period or minimum instance inter-arrival time;
// /*AFLA*/  *  - the maximum (or average, depending on the actual scheduling
// /*AFLA*/  *    discipline) computation time of all instances, a.k.a. runtime;
// /*AFLA*/  *  - the deadline (relative to the actual activation time) of each
// /*AFLA*/  *    instance.
// /*AFLA*/  * Very briefly, a periodic (sporadic) task asks for the execution of
// /*AFLA*/  * some specific computation --which is typically called an instance--
// /*AFLA*/  * (at most) every period. Moreover, each instance typically lasts no more
// /*AFLA*/  * than the runtime and must be completed by time instant t equal to
// /*AFLA*/  * the instance activation time + the deadline.
// /*AFLA*/  *
// /*AFLA*/  * This is reflected by the actual fields of the sched_attr structure:
// /*AFLA*/  *
// /*AFLA*/  *  @size		size of the structure, for fwd/bwd compat.
// /*AFLA*/  *
// /*AFLA*/  *  @sched_policy	task's scheduling policy
// /*AFLA*/  *  @sched_flags	for customizing the scheduler behaviour
// /*AFLA*/  *  @sched_nice		task's nice value      (SCHED_NORMAL/BATCH)
// /*AFLA*/  *  @sched_priority	task's static priority (SCHED_FIFO/RR)
// /*AFLA*/  *  @sched_deadline	representative of the task's deadline
// /*AFLA*/  *  @sched_runtime	representative of the task's runtime
// /*AFLA*/  *  @sched_period	representative of the task's period
// /*AFLA*/  *
// /*AFLA*/  * Given this task model, there are a multiplicity of scheduling algorithms
// /*AFLA*/  * and policies, that can be used to ensure all the tasks will make their
// /*AFLA*/  * timing constraints.
// /*AFLA*/  *
// /*AFLA*/  * As of now, the SCHED_DEADLINE policy (sched_dl scheduling class) is the
// /*AFLA*/  * only user of this new interface. More information about the algorithm
// /*AFLA*/  * available in the scheduling class file or in Documentation/.
// /*AFLA*/  */
// /*AFLA*/ struct sched_attr {
// /*AFLA*/ 	u32 size;
// /*AFLA*/ 
// /*AFLA*/ 	u32 sched_policy;
// /*AFLA*/ 	u64 sched_flags;
// /*AFLA*/ 
// /*AFLA*/ 	/* SCHED_NORMAL, SCHED_BATCH */
// /*AFLA*/ 	s32 sched_nice;
// /*AFLA*/ 
// /*AFLA*/ 	/* SCHED_FIFO, SCHED_RR */
// /*AFLA*/ 	u32 sched_priority;
// /*AFLA*/ 
// /*AFLA*/ 	/* SCHED_DEADLINE */
// /*AFLA*/ 	u64 sched_runtime;
// /*AFLA*/ 	u64 sched_deadline;
// /*AFLA*/ 	u64 sched_period;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct futex_pi_state;
// /*AFLA*/ struct robust_list_head;
// /*AFLA*/ struct bio_list;
// /*AFLA*/ struct fs_struct;
// /*AFLA*/ struct perf_event_context;
// /*AFLA*/ struct blk_plug;
// /*AFLA*/ struct filename;
// /*AFLA*/ struct nameidata;
// /*AFLA*/ 
// /*AFLA*/ #define VMACACHE_BITS 2
// /*AFLA*/ #define VMACACHE_SIZE (1U << VMACACHE_BITS)
// /*AFLA*/ #define VMACACHE_MASK (VMACACHE_SIZE - 1)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These are the constant used to fake the fixed-point load-average
// /*AFLA*/  * counting. Some notes:
// /*AFLA*/  *  - 11 bit fractions expand to 22 bits by the multiplies: this gives
// /*AFLA*/  *    a load-average precision of 10 bits integer + 11 bits fractional
// /*AFLA*/  *  - if you want to count load-averages more often, you need more
// /*AFLA*/  *    precision, or rounding will get you. With 2-second counting freq,
// /*AFLA*/  *    the EXP_n values would be 1981, 2034 and 2043 if still using only
// /*AFLA*/  *    11 bit fractions.
// /*AFLA*/  */
// /*AFLA*/ extern unsigned long avenrun[];		/* Load averages */
// /*AFLA*/ extern void get_avenrun(unsigned long *loads, unsigned long offset, int shift);
// /*AFLA*/ 
// /*AFLA*/ #define FSHIFT		11		/* nr of bits of precision */
// /*AFLA*/ #define FIXED_1		(1<<FSHIFT)	/* 1.0 as fixed-point */
// /*AFLA*/ #define LOAD_FREQ	(5*HZ+1)	/* 5 sec intervals */
// /*AFLA*/ #define EXP_1		1884		/* 1/exp(5sec/1min) as fixed-point */
// /*AFLA*/ #define EXP_5		2014		/* 1/exp(5sec/5min) */
// /*AFLA*/ #define EXP_15		2037		/* 1/exp(5sec/15min) */
// /*AFLA*/ 
// /*AFLA*/ #define CALC_LOAD(load,exp,n) \
// /*AFLA*/ 	load *= exp; \
// /*AFLA*/ 	load += n*(FIXED_1-exp); \
// /*AFLA*/ 	load >>= FSHIFT;
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long total_forks;
// /*AFLA*/ extern int nr_threads;
// /*AFLA*/ DECLARE_PER_CPU(unsigned long, process_counts);
// /*AFLA*/ extern int nr_processes(void);
// /*AFLA*/ extern unsigned long nr_running(void);
// /*AFLA*/ extern bool single_task_running(void);
// /*AFLA*/ extern unsigned long nr_iowait(void);
// /*AFLA*/ extern unsigned long nr_iowait_cpu(int cpu);
// /*AFLA*/ extern void get_iowait_load(unsigned long *nr_waiters, unsigned long *load);
// /*AFLA*/ 
// /*AFLA*/ extern void calc_global_load(unsigned long ticks);
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_SMP) && defined(CONFIG_NO_HZ_COMMON)
// /*AFLA*/ extern void cpu_load_update_nohz_start(void);
// /*AFLA*/ extern void cpu_load_update_nohz_stop(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void cpu_load_update_nohz_start(void) { }
// /*AFLA*/ static inline void cpu_load_update_nohz_stop(void) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void dump_cpu_task(int cpu);
// /*AFLA*/ 
// /*AFLA*/ struct seq_file;
// /*AFLA*/ struct cfs_rq;
// /*AFLA*/ struct task_group;
// /*AFLA*/ #ifdef CONFIG_SCHED_DEBUG
// /*AFLA*/ extern void proc_sched_show_task(struct task_struct *p, struct seq_file *m);
// /*AFLA*/ extern void proc_sched_set_task(struct task_struct *p);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Task state bitmask. NOTE! These bits are also
// /*AFLA*/  * encoded in fs/proc/array.c: get_task_state().
// /*AFLA*/  *
// /*AFLA*/  * We have two separate sets of flags: task->state
// /*AFLA*/  * is about runnability, while task->exit_state are
// /*AFLA*/  * about the task exiting. Confusing, but this way
// /*AFLA*/  * modifying one set can't modify the other one by
// /*AFLA*/  * mistake.
// /*AFLA*/  */
// /*AFLA*/ #define TASK_RUNNING		0
// /*AFLA*/ #define TASK_INTERRUPTIBLE	1
// /*AFLA*/ #define TASK_UNINTERRUPTIBLE	2
// /*AFLA*/ #define __TASK_STOPPED		4
// /*AFLA*/ #define __TASK_TRACED		8
// /*AFLA*/ /* in tsk->exit_state */
// /*AFLA*/ #define EXIT_DEAD		16
// /*AFLA*/ #define EXIT_ZOMBIE		32
// /*AFLA*/ #define EXIT_TRACE		(EXIT_ZOMBIE | EXIT_DEAD)
// /*AFLA*/ /* in tsk->state again */
// /*AFLA*/ #define TASK_DEAD		64
// /*AFLA*/ #define TASK_WAKEKILL		128
// /*AFLA*/ #define TASK_WAKING		256
// /*AFLA*/ #define TASK_PARKED		512
// /*AFLA*/ #define TASK_NOLOAD		1024
// /*AFLA*/ #define TASK_NEW		2048
// /*AFLA*/ #define TASK_STATE_MAX		4096
// /*AFLA*/ 
// /*AFLA*/ #define TASK_STATE_TO_CHAR_STR "RSDTtXZxKWPNn"
// /*AFLA*/ 
// /*AFLA*/ extern char ___assert_task_state[1 - 2*!!(
// /*AFLA*/ 		sizeof(TASK_STATE_TO_CHAR_STR)-1 != ilog2(TASK_STATE_MAX)+1)];
// /*AFLA*/ 
// /*AFLA*/ /* Convenience macros for the sake of set_task_state */
// /*AFLA*/ #define TASK_KILLABLE		(TASK_WAKEKILL | TASK_UNINTERRUPTIBLE)
// /*AFLA*/ #define TASK_STOPPED		(TASK_WAKEKILL | __TASK_STOPPED)
// /*AFLA*/ #define TASK_TRACED		(TASK_WAKEKILL | __TASK_TRACED)
// /*AFLA*/ 
// /*AFLA*/ #define TASK_IDLE		(TASK_UNINTERRUPTIBLE | TASK_NOLOAD)
// /*AFLA*/ 
// /*AFLA*/ /* Convenience macros for the sake of wake_up */
// /*AFLA*/ #define TASK_NORMAL		(TASK_INTERRUPTIBLE | TASK_UNINTERRUPTIBLE)
// /*AFLA*/ #define TASK_ALL		(TASK_NORMAL | __TASK_STOPPED | __TASK_TRACED)
// /*AFLA*/ 
// /*AFLA*/ /* get_task_state() */
// /*AFLA*/ #define TASK_REPORT		(TASK_RUNNING | TASK_INTERRUPTIBLE | \
// /*AFLA*/ 				 TASK_UNINTERRUPTIBLE | __TASK_STOPPED | \
// /*AFLA*/ 				 __TASK_TRACED | EXIT_ZOMBIE | EXIT_DEAD)
// /*AFLA*/ 
// /*AFLA*/ #define task_is_traced(task)	((task->state & __TASK_TRACED) != 0)
// /*AFLA*/ #define task_is_stopped(task)	((task->state & __TASK_STOPPED) != 0)
// /*AFLA*/ #define task_is_stopped_or_traced(task)	\
// /*AFLA*/ 			((task->state & (__TASK_STOPPED | __TASK_TRACED)) != 0)
// /*AFLA*/ #define task_contributes_to_load(task)	\
// /*AFLA*/ 				((task->state & TASK_UNINTERRUPTIBLE) != 0 && \
// /*AFLA*/ 				 (task->flags & PF_FROZEN) == 0 && \
// /*AFLA*/ 				 (task->state & TASK_NOLOAD) == 0)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_ATOMIC_SLEEP
// /*AFLA*/ 
// /*AFLA*/ #define __set_task_state(tsk, state_value)			\
// /*AFLA*/ 	do {							\
// /*AFLA*/ 		(tsk)->task_state_change = _THIS_IP_;		\
// /*AFLA*/ 		(tsk)->state = (state_value);			\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #define set_task_state(tsk, state_value)			\
// /*AFLA*/ 	do {							\
// /*AFLA*/ 		(tsk)->task_state_change = _THIS_IP_;		\
// /*AFLA*/ 		smp_store_mb((tsk)->state, (state_value));		\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * set_current_state() includes a barrier so that the write of current->state
// /*AFLA*/  * is correctly serialised wrt the caller's subsequent test of whether to
// /*AFLA*/  * actually sleep:
// /*AFLA*/  *
// /*AFLA*/  *	set_current_state(TASK_UNINTERRUPTIBLE);
// /*AFLA*/  *	if (do_i_need_to_sleep())
// /*AFLA*/  *		schedule();
// /*AFLA*/  *
// /*AFLA*/  * If the caller does not need such serialisation then use __set_current_state()
// /*AFLA*/  */
// /*AFLA*/ #define __set_current_state(state_value)			\
// /*AFLA*/ 	do {							\
// /*AFLA*/ 		current->task_state_change = _THIS_IP_;		\
// /*AFLA*/ 		current->state = (state_value);			\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #define set_current_state(state_value)				\
// /*AFLA*/ 	do {							\
// /*AFLA*/ 		current->task_state_change = _THIS_IP_;		\
// /*AFLA*/ 		smp_store_mb(current->state, (state_value));		\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ #define __set_task_state(tsk, state_value)		\
// /*AFLA*/ 	do { (tsk)->state = (state_value); } while (0)
// /*AFLA*/ #define set_task_state(tsk, state_value)		\
// /*AFLA*/ 	smp_store_mb((tsk)->state, (state_value))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * set_current_state() includes a barrier so that the write of current->state
// /*AFLA*/  * is correctly serialised wrt the caller's subsequent test of whether to
// /*AFLA*/  * actually sleep:
// /*AFLA*/  *
// /*AFLA*/  *	set_current_state(TASK_UNINTERRUPTIBLE);
// /*AFLA*/  *	if (do_i_need_to_sleep())
// /*AFLA*/  *		schedule();
// /*AFLA*/  *
// /*AFLA*/  * If the caller does not need such serialisation then use __set_current_state()
// /*AFLA*/  */
// /*AFLA*/ #define __set_current_state(state_value)		\
// /*AFLA*/ 	do { current->state = (state_value); } while (0)
// /*AFLA*/ #define set_current_state(state_value)			\
// /*AFLA*/ 	smp_store_mb(current->state, (state_value))
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Task command name length */
// /*AFLA*/ #define TASK_COMM_LEN 16
// /*AFLA*/ 
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This serializes "schedule()" and also protects
// /*AFLA*/  * the run-queue from deletions/modifications (but
// /*AFLA*/  * _adding_ to the beginning of the run-queue has
// /*AFLA*/  * a separate lock).
// /*AFLA*/  */
// /*AFLA*/ extern rwlock_t tasklist_lock;
// /*AFLA*/ extern spinlock_t mmlist_lock;
// /*AFLA*/ 
// /*AFLA*/ struct task_struct;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PROVE_RCU
// /*AFLA*/ extern int lockdep_tasklist_lock_is_held(void);
// /*AFLA*/ #endif /* #ifdef CONFIG_PROVE_RCU */
// /*AFLA*/ 
// /*AFLA*/ extern void sched_init(void);
// /*AFLA*/ extern void sched_init_smp(void);
// /*AFLA*/ extern asmlinkage void schedule_tail(struct task_struct *prev);
// /*AFLA*/ extern void init_idle(struct task_struct *idle, int cpu);
// /*AFLA*/ extern void init_idle_bootup_task(struct task_struct *idle);
// /*AFLA*/ 
// /*AFLA*/ extern cpumask_var_t cpu_isolated_map;
// /*AFLA*/ 
// /*AFLA*/ extern int runqueue_is_locked(int cpu);
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_SMP) && defined(CONFIG_NO_HZ_COMMON)
// /*AFLA*/ extern void nohz_balance_enter_idle(int cpu);
// /*AFLA*/ extern void set_cpu_sd_state_idle(void);
// /*AFLA*/ extern int get_nohz_timer_target(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void nohz_balance_enter_idle(int cpu) { }
// /*AFLA*/ static inline void set_cpu_sd_state_idle(void) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Only dump TASK_* tasks. (0 for all tasks)
// /*AFLA*/  */
// /*AFLA*/ extern void show_state_filter(unsigned long state_filter);
// /*AFLA*/ 
// /*AFLA*/ static inline void show_state(void)
// /*AFLA*/ {
// /*AFLA*/ 	show_state_filter(0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void show_regs(struct pt_regs *);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * TASK is a pointer to the task whose backtrace we want to see (or NULL for current
// /*AFLA*/  * task), SP is the stack pointer of the first frame that should be shown in the back
// /*AFLA*/  * trace (or NULL if the entire call-chain of the task should be shown).
// /*AFLA*/  */
// /*AFLA*/ extern void show_stack(struct task_struct *task, unsigned long *sp);
// /*AFLA*/ 
// /*AFLA*/ extern void cpu_init (void);
// /*AFLA*/ extern void trap_init(void);
// /*AFLA*/ extern void update_process_times(int user);
// /*AFLA*/ extern void scheduler_tick(void);
// /*AFLA*/ extern int sched_cpu_starting(unsigned int cpu);
// /*AFLA*/ extern int sched_cpu_activate(unsigned int cpu);
// /*AFLA*/ extern int sched_cpu_deactivate(unsigned int cpu);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HOTPLUG_CPU
// /*AFLA*/ extern int sched_cpu_dying(unsigned int cpu);
// /*AFLA*/ #else
// /*AFLA*/ # define sched_cpu_dying	NULL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void sched_show_task(struct task_struct *p);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_LOCKUP_DETECTOR
// /*AFLA*/ extern void touch_softlockup_watchdog_sched(void);
// /*AFLA*/ extern void touch_softlockup_watchdog(void);
// /*AFLA*/ extern void touch_softlockup_watchdog_sync(void);
// /*AFLA*/ extern void touch_all_softlockup_watchdogs(void);
// /*AFLA*/ extern int proc_dowatchdog_thresh(struct ctl_table *table, int write,
// /*AFLA*/ 				  void __user *buffer,
// /*AFLA*/ 				  size_t *lenp, loff_t *ppos);
// /*AFLA*/ extern unsigned int  softlockup_panic;
// /*AFLA*/ extern unsigned int  hardlockup_panic;
// /*AFLA*/ void lockup_detector_init(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void touch_softlockup_watchdog_sched(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void touch_softlockup_watchdog(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void touch_softlockup_watchdog_sync(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void touch_all_softlockup_watchdogs(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void lockup_detector_init(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DETECT_HUNG_TASK
// /*AFLA*/ void reset_hung_task_detector(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void reset_hung_task_detector(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Attach to any functions which should be ignored in wchan output. */
// /*AFLA*/ #define __sched		__attribute__((__section__(".sched.text")))
// /*AFLA*/ 
// /*AFLA*/ /* Linker adds these: start and end of __sched functions */
// /*AFLA*/ extern char __sched_text_start[], __sched_text_end[];
// /*AFLA*/ 
// /*AFLA*/ /* Is this address in the __sched functions? */
// /*AFLA*/ extern int in_sched_functions(unsigned long addr);
// /*AFLA*/ 
// /*AFLA*/ #define	MAX_SCHEDULE_TIMEOUT	LONG_MAX
// /*AFLA*/ extern signed long schedule_timeout(signed long timeout);
// /*AFLA*/ extern signed long schedule_timeout_interruptible(signed long timeout);
// /*AFLA*/ extern signed long schedule_timeout_killable(signed long timeout);
// /*AFLA*/ extern signed long schedule_timeout_uninterruptible(signed long timeout);
// /*AFLA*/ extern signed long schedule_timeout_idle(signed long timeout);
// /*AFLA*/ asmlinkage void schedule(void);
// /*AFLA*/ extern void schedule_preempt_disabled(void);
// /*AFLA*/ 
// /*AFLA*/ extern long io_schedule_timeout(long timeout);
// /*AFLA*/ 
// /*AFLA*/ static inline void io_schedule(void)
// /*AFLA*/ {
// /*AFLA*/ 	io_schedule_timeout(MAX_SCHEDULE_TIMEOUT);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void __noreturn do_task_dead(void);
// /*AFLA*/ 
// /*AFLA*/ struct nsproxy;
// /*AFLA*/ struct user_namespace;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ extern void arch_pick_mmap_layout(struct mm_struct *mm);
// /*AFLA*/ extern unsigned long
// /*AFLA*/ arch_get_unmapped_area(struct file *, unsigned long, unsigned long,
// /*AFLA*/ 		       unsigned long, unsigned long);
// /*AFLA*/ extern unsigned long
// /*AFLA*/ arch_get_unmapped_area_topdown(struct file *filp, unsigned long addr,
// /*AFLA*/ 			  unsigned long len, unsigned long pgoff,
// /*AFLA*/ 			  unsigned long flags);
// /*AFLA*/ #else
// /*AFLA*/ static inline void arch_pick_mmap_layout(struct mm_struct *mm) {}
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define SUID_DUMP_DISABLE	0	/* No setuid dumping */
// /*AFLA*/ #define SUID_DUMP_USER		1	/* Dump as user of process */
// /*AFLA*/ #define SUID_DUMP_ROOT		2	/* Dump as root */
// /*AFLA*/ 
// /*AFLA*/ /* mm flags */
// /*AFLA*/ 
// /*AFLA*/ /* for SUID_DUMP_* above */
// /*AFLA*/ #define MMF_DUMPABLE_BITS 2
// /*AFLA*/ #define MMF_DUMPABLE_MASK ((1 << MMF_DUMPABLE_BITS) - 1)
// /*AFLA*/ 
// /*AFLA*/ extern void set_dumpable(struct mm_struct *mm, int value);
// /*AFLA*/ /*
// /*AFLA*/  * This returns the actual value of the suid_dumpable flag. For things
// /*AFLA*/  * that are using this for checking for privilege transitions, it must
// /*AFLA*/  * test against SUID_DUMP_USER rather than treating it as a boolean
// /*AFLA*/  * value.
// /*AFLA*/  */
// /*AFLA*/ static inline int __get_dumpable(unsigned long mm_flags)
// /*AFLA*/ {
// /*AFLA*/ 	return mm_flags & MMF_DUMPABLE_MASK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int get_dumpable(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	return __get_dumpable(mm->flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* coredump filter bits */
// /*AFLA*/ #define MMF_DUMP_ANON_PRIVATE	2
// /*AFLA*/ #define MMF_DUMP_ANON_SHARED	3
// /*AFLA*/ #define MMF_DUMP_MAPPED_PRIVATE	4
// /*AFLA*/ #define MMF_DUMP_MAPPED_SHARED	5
// /*AFLA*/ #define MMF_DUMP_ELF_HEADERS	6
// /*AFLA*/ #define MMF_DUMP_HUGETLB_PRIVATE 7
// /*AFLA*/ #define MMF_DUMP_HUGETLB_SHARED  8
// /*AFLA*/ #define MMF_DUMP_DAX_PRIVATE	9
// /*AFLA*/ #define MMF_DUMP_DAX_SHARED	10
// /*AFLA*/ 
// /*AFLA*/ #define MMF_DUMP_FILTER_SHIFT	MMF_DUMPABLE_BITS
// /*AFLA*/ #define MMF_DUMP_FILTER_BITS	9
// /*AFLA*/ #define MMF_DUMP_FILTER_MASK \
// /*AFLA*/ 	(((1 << MMF_DUMP_FILTER_BITS) - 1) << MMF_DUMP_FILTER_SHIFT)
// /*AFLA*/ #define MMF_DUMP_FILTER_DEFAULT \
// /*AFLA*/ 	((1 << MMF_DUMP_ANON_PRIVATE) |	(1 << MMF_DUMP_ANON_SHARED) |\
// /*AFLA*/ 	 (1 << MMF_DUMP_HUGETLB_PRIVATE) | MMF_DUMP_MASK_DEFAULT_ELF)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_CORE_DUMP_DEFAULT_ELF_HEADERS
// /*AFLA*/ # define MMF_DUMP_MASK_DEFAULT_ELF	(1 << MMF_DUMP_ELF_HEADERS)
// /*AFLA*/ #else
// /*AFLA*/ # define MMF_DUMP_MASK_DEFAULT_ELF	0
// /*AFLA*/ #endif
// /*AFLA*/ 					/* leave room for more dump flags */
// /*AFLA*/ #define MMF_VM_MERGEABLE	16	/* KSM may merge identical pages */
// /*AFLA*/ #define MMF_VM_HUGEPAGE		17	/* set when VM_HUGEPAGE is set on vma */
// /*AFLA*/ #define MMF_EXE_FILE_CHANGED	18	/* see prctl_set_mm_exe_file() */
// /*AFLA*/ 
// /*AFLA*/ #define MMF_HAS_UPROBES		19	/* has uprobes */
// /*AFLA*/ #define MMF_RECALC_UPROBES	20	/* MMF_HAS_UPROBES can be wrong */
// /*AFLA*/ #define MMF_OOM_SKIP		21	/* mm is of no interest for the OOM killer */
// /*AFLA*/ #define MMF_UNSTABLE		22	/* mm is unstable for copy_from_user */
// /*AFLA*/ #define MMF_HUGE_ZERO_PAGE	23      /* mm has ever used the global huge zero page */
// /*AFLA*/ 
// /*AFLA*/ #define MMF_INIT_MASK		(MMF_DUMPABLE_MASK | MMF_DUMP_FILTER_MASK)
// /*AFLA*/ 
// /*AFLA*/ struct sighand_struct {
// /*AFLA*/ 	atomic_t		count;
// /*AFLA*/ 	struct k_sigaction	action[_NSIG];
// /*AFLA*/ 	spinlock_t		siglock;
// /*AFLA*/ 	wait_queue_head_t	signalfd_wqh;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct pacct_struct {
// /*AFLA*/ 	int			ac_flag;
// /*AFLA*/ 	long			ac_exitcode;
// /*AFLA*/ 	unsigned long		ac_mem;
// /*AFLA*/ 	cputime_t		ac_utime, ac_stime;
// /*AFLA*/ 	unsigned long		ac_minflt, ac_majflt;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct cpu_itimer {
// /*AFLA*/ 	cputime_t expires;
// /*AFLA*/ 	cputime_t incr;
// /*AFLA*/ 	u32 error;
// /*AFLA*/ 	u32 incr_error;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct prev_cputime - snaphsot of system and user cputime
// /*AFLA*/  * @utime: time spent in user mode
// /*AFLA*/  * @stime: time spent in system mode
// /*AFLA*/  * @lock: protects the above two fields
// /*AFLA*/  *
// /*AFLA*/  * Stores previous user/system time values such that we can guarantee
// /*AFLA*/  * monotonicity.
// /*AFLA*/  */
// /*AFLA*/ struct prev_cputime {
// /*AFLA*/ #ifndef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
// /*AFLA*/ 	cputime_t utime;
// /*AFLA*/ 	cputime_t stime;
// /*AFLA*/ 	raw_spinlock_t lock;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline void prev_cputime_init(struct prev_cputime *prev)
// /*AFLA*/ {
// /*AFLA*/ #ifndef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
// /*AFLA*/ 	prev->utime = prev->stime = 0;
// /*AFLA*/ 	raw_spin_lock_init(&prev->lock);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct task_cputime - collected CPU time counts
// /*AFLA*/  * @utime:		time spent in user mode, in &cputime_t units
// /*AFLA*/  * @stime:		time spent in kernel mode, in &cputime_t units
// /*AFLA*/  * @sum_exec_runtime:	total time spent on the CPU, in nanoseconds
// /*AFLA*/  *
// /*AFLA*/  * This structure groups together three kinds of CPU time that are tracked for
// /*AFLA*/  * threads and thread groups.  Most things considering CPU time want to group
// /*AFLA*/  * these counts together and treat all three of them in parallel.
// /*AFLA*/  */
// /*AFLA*/ struct task_cputime {
// /*AFLA*/ 	cputime_t utime;
// /*AFLA*/ 	cputime_t stime;
// /*AFLA*/ 	unsigned long long sum_exec_runtime;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Alternate field names when used to cache expirations. */
// /*AFLA*/ #define virt_exp	utime
// /*AFLA*/ #define prof_exp	stime
// /*AFLA*/ #define sched_exp	sum_exec_runtime
// /*AFLA*/ 
// /*AFLA*/ #define INIT_CPUTIME	\
// /*AFLA*/ 	(struct task_cputime) {					\
// /*AFLA*/ 		.utime = 0,					\
// /*AFLA*/ 		.stime = 0,					\
// /*AFLA*/ 		.sum_exec_runtime = 0,				\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is the atomic variant of task_cputime, which can be used for
// /*AFLA*/  * storing and updating task_cputime statistics without locking.
// /*AFLA*/  */
// /*AFLA*/ struct task_cputime_atomic {
// /*AFLA*/ 	atomic64_t utime;
// /*AFLA*/ 	atomic64_t stime;
// /*AFLA*/ 	atomic64_t sum_exec_runtime;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define INIT_CPUTIME_ATOMIC \
// /*AFLA*/ 	(struct task_cputime_atomic) {				\
// /*AFLA*/ 		.utime = ATOMIC64_INIT(0),			\
// /*AFLA*/ 		.stime = ATOMIC64_INIT(0),			\
// /*AFLA*/ 		.sum_exec_runtime = ATOMIC64_INIT(0),		\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ #define PREEMPT_DISABLED	(PREEMPT_DISABLE_OFFSET + PREEMPT_ENABLED)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Disable preemption until the scheduler is running -- use an unconditional
// /*AFLA*/  * value so that it also works on !PREEMPT_COUNT kernels.
// /*AFLA*/  *
// /*AFLA*/  * Reset by start_kernel()->sched_init()->init_idle()->init_idle_preempt_count().
// /*AFLA*/  */
// /*AFLA*/ #define INIT_PREEMPT_COUNT	PREEMPT_OFFSET
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Initial preempt_count value; reflects the preempt_count schedule invariant
// /*AFLA*/  * which states that during context switches:
// /*AFLA*/  *
// /*AFLA*/  *    preempt_count() == 2*PREEMPT_DISABLE_OFFSET
// /*AFLA*/  *
// /*AFLA*/  * Note: PREEMPT_DISABLE_OFFSET is 0 for !PREEMPT_COUNT kernels.
// /*AFLA*/  * Note: See finish_task_switch().
// /*AFLA*/  */
// /*AFLA*/ #define FORK_PREEMPT_COUNT	(2*PREEMPT_DISABLE_OFFSET + PREEMPT_ENABLED)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct thread_group_cputimer - thread group interval timer counts
// /*AFLA*/  * @cputime_atomic:	atomic thread group interval timers.
// /*AFLA*/  * @running:		true when there are timers running and
// /*AFLA*/  *			@cputime_atomic receives updates.
// /*AFLA*/  * @checking_timer:	true when a thread in the group is in the
// /*AFLA*/  *			process of checking for thread group timers.
// /*AFLA*/  *
// /*AFLA*/  * This structure contains the version of task_cputime, above, that is
// /*AFLA*/  * used for thread group CPU timer calculations.
// /*AFLA*/  */
// /*AFLA*/ struct thread_group_cputimer {
// /*AFLA*/ 	struct task_cputime_atomic cputime_atomic;
// /*AFLA*/ 	bool running;
// /*AFLA*/ 	bool checking_timer;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #include <linux/rwsem.h>
// /*AFLA*/ struct autogroup;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * NOTE! "signal_struct" does not have its own
// /*AFLA*/  * locking, because a shared signal_struct always
// /*AFLA*/  * implies a shared sighand_struct, so locking
// /*AFLA*/  * sighand_struct is always a proper superset of
// /*AFLA*/  * the locking of signal_struct.
// /*AFLA*/  */
// /*AFLA*/ struct signal_struct {
// /*AFLA*/ 	atomic_t		sigcnt;
// /*AFLA*/ 	atomic_t		live;
// /*AFLA*/ 	int			nr_threads;
// /*AFLA*/ 	struct list_head	thread_head;
// /*AFLA*/ 
// /*AFLA*/ 	wait_queue_head_t	wait_chldexit;	/* for wait4() */
// /*AFLA*/ 
// /*AFLA*/ 	/* current thread group signal load-balancing target: */
// /*AFLA*/ 	struct task_struct	*curr_target;
// /*AFLA*/ 
// /*AFLA*/ 	/* shared signal handling: */
// /*AFLA*/ 	struct sigpending	shared_pending;
// /*AFLA*/ 
// /*AFLA*/ 	/* thread group exit support */
// /*AFLA*/ 	int			group_exit_code;
// /*AFLA*/ 	/* overloaded:
// /*AFLA*/ 	 * - notify group_exit_task when ->count is equal to notify_count
// /*AFLA*/ 	 * - everyone except group_exit_task is stopped during signal delivery
// /*AFLA*/ 	 *   of fatal signals, group_exit_task processes the signal.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int			notify_count;
// /*AFLA*/ 	struct task_struct	*group_exit_task;
// /*AFLA*/ 
// /*AFLA*/ 	/* thread group stop support, overloads group_exit_code too */
// /*AFLA*/ 	int			group_stop_count;
// /*AFLA*/ 	unsigned int		flags; /* see SIGNAL_* flags below */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * PR_SET_CHILD_SUBREAPER marks a process, like a service
// /*AFLA*/ 	 * manager, to re-parent orphan (double-forking) child processes
// /*AFLA*/ 	 * to this process instead of 'init'. The service manager is
// /*AFLA*/ 	 * able to receive SIGCHLD signals and is able to investigate
// /*AFLA*/ 	 * the process until it calls wait(). All children of this
// /*AFLA*/ 	 * process will inherit a flag if they should look for a
// /*AFLA*/ 	 * child_subreaper process at exit.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned int		is_child_subreaper:1;
// /*AFLA*/ 	unsigned int		has_child_subreaper:1;
// /*AFLA*/ 
// /*AFLA*/ 	/* POSIX.1b Interval Timers */
// /*AFLA*/ 	int			posix_timer_id;
// /*AFLA*/ 	struct list_head	posix_timers;
// /*AFLA*/ 
// /*AFLA*/ 	/* ITIMER_REAL timer for the process */
// /*AFLA*/ 	struct hrtimer real_timer;
// /*AFLA*/ 	struct pid *leader_pid;
// /*AFLA*/ 	ktime_t it_real_incr;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * ITIMER_PROF and ITIMER_VIRTUAL timers for the process, we use
// /*AFLA*/ 	 * CPUCLOCK_PROF and CPUCLOCK_VIRT for indexing array as these
// /*AFLA*/ 	 * values are defined to 0 and 1 respectively
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct cpu_itimer it[2];
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Thread group totals for process CPU timers.
// /*AFLA*/ 	 * See thread_group_cputimer(), et al, for details.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct thread_group_cputimer cputimer;
// /*AFLA*/ 
// /*AFLA*/ 	/* Earliest-expiration cache. */
// /*AFLA*/ 	struct task_cputime cputime_expires;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NO_HZ_FULL
// /*AFLA*/ 	atomic_t tick_dep_mask;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	struct list_head cpu_timers[3];
// /*AFLA*/ 
// /*AFLA*/ 	struct pid *tty_old_pgrp;
// /*AFLA*/ 
// /*AFLA*/ 	/* boolean value for session group leader */
// /*AFLA*/ 	int leader;
// /*AFLA*/ 
// /*AFLA*/ 	struct tty_struct *tty; /* NULL if no tty */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SCHED_AUTOGROUP
// /*AFLA*/ 	struct autogroup *autogroup;
// /*AFLA*/ #endif
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Cumulative resource counters for dead threads in the group,
// /*AFLA*/ 	 * and for reaped dead child processes forked by this group.
// /*AFLA*/ 	 * Live threads maintain their own counters and add to these
// /*AFLA*/ 	 * in __exit_signal, except for the group leader.
// /*AFLA*/ 	 */
// /*AFLA*/ 	seqlock_t stats_lock;
// /*AFLA*/ 	cputime_t utime, stime, cutime, cstime;
// /*AFLA*/ 	cputime_t gtime;
// /*AFLA*/ 	cputime_t cgtime;
// /*AFLA*/ 	struct prev_cputime prev_cputime;
// /*AFLA*/ 	unsigned long nvcsw, nivcsw, cnvcsw, cnivcsw;
// /*AFLA*/ 	unsigned long min_flt, maj_flt, cmin_flt, cmaj_flt;
// /*AFLA*/ 	unsigned long inblock, oublock, cinblock, coublock;
// /*AFLA*/ 	unsigned long maxrss, cmaxrss;
// /*AFLA*/ 	struct task_io_accounting ioac;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Cumulative ns of schedule CPU time fo dead threads in the
// /*AFLA*/ 	 * group, not including a zombie group leader, (This only differs
// /*AFLA*/ 	 * from jiffies_to_ns(utime + stime) if sched_clock uses something
// /*AFLA*/ 	 * other than jiffies.)
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long long sum_sched_runtime;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * We don't bother to synchronize most readers of this at all,
// /*AFLA*/ 	 * because there is no reader checking a limit that actually needs
// /*AFLA*/ 	 * to get both rlim_cur and rlim_max atomically, and either one
// /*AFLA*/ 	 * alone is a single word that can safely be read normally.
// /*AFLA*/ 	 * getrlimit/setrlimit use task_lock(current->group_leader) to
// /*AFLA*/ 	 * protect this instead of the siglock, because they really
// /*AFLA*/ 	 * have no need to disable irqs.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct rlimit rlim[RLIM_NLIMITS];
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_BSD_PROCESS_ACCT
// /*AFLA*/ 	struct pacct_struct pacct;	/* per-process accounting information */
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_TASKSTATS
// /*AFLA*/ 	struct taskstats *stats;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_AUDIT
// /*AFLA*/ 	unsigned audit_tty;
// /*AFLA*/ 	struct tty_audit_buf *tty_audit_buf;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Thread is the potential origin of an oom condition; kill first on
// /*AFLA*/ 	 * oom
// /*AFLA*/ 	 */
// /*AFLA*/ 	bool oom_flag_origin;
// /*AFLA*/ 	short oom_score_adj;		/* OOM kill score adjustment */
// /*AFLA*/ 	short oom_score_adj_min;	/* OOM kill score adjustment min value.
// /*AFLA*/ 					 * Only settable by CAP_SYS_RESOURCE. */
// /*AFLA*/ 	struct mm_struct *oom_mm;	/* recorded mm when the thread group got
// /*AFLA*/ 					 * killed by the oom killer */
// /*AFLA*/ 
// /*AFLA*/ 	struct mutex cred_guard_mutex;	/* guard against foreign influences on
// /*AFLA*/ 					 * credential calculations
// /*AFLA*/ 					 * (notably. ptrace) */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Bits in flags field of signal_struct.
// /*AFLA*/  */
// /*AFLA*/ #define SIGNAL_STOP_STOPPED	0x00000001 /* job control stop in effect */
// /*AFLA*/ #define SIGNAL_STOP_CONTINUED	0x00000002 /* SIGCONT since WCONTINUED reap */
// /*AFLA*/ #define SIGNAL_GROUP_EXIT	0x00000004 /* group exit in progress */
// /*AFLA*/ #define SIGNAL_GROUP_COREDUMP	0x00000008 /* coredump in progress */
// /*AFLA*/ /*
// /*AFLA*/  * Pending notifications to parent.
// /*AFLA*/  */
// /*AFLA*/ #define SIGNAL_CLD_STOPPED	0x00000010
// /*AFLA*/ #define SIGNAL_CLD_CONTINUED	0x00000020
// /*AFLA*/ #define SIGNAL_CLD_MASK		(SIGNAL_CLD_STOPPED|SIGNAL_CLD_CONTINUED)
// /*AFLA*/ 
// /*AFLA*/ #define SIGNAL_UNKILLABLE	0x00000040 /* for init: ignore fatal signals */
// /*AFLA*/ 
// /*AFLA*/ /* If true, all threads except ->group_exit_task have pending SIGKILL */
// /*AFLA*/ static inline int signal_group_exit(const struct signal_struct *sig)
// /*AFLA*/ {
// /*AFLA*/ 	return	(sig->flags & SIGNAL_GROUP_EXIT) ||
// /*AFLA*/ 		(sig->group_exit_task != NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Some day this will be a full-fledged user tracking system..
// /*AFLA*/  */
// /*AFLA*/ struct user_struct {
// /*AFLA*/ 	atomic_t __count;	/* reference count */
// /*AFLA*/ 	atomic_t processes;	/* How many processes does this user have? */
// /*AFLA*/ 	atomic_t sigpending;	/* How many pending signals does this user have? */
// /*AFLA*/ #ifdef CONFIG_INOTIFY_USER
// /*AFLA*/ 	atomic_t inotify_watches; /* How many inotify watches does this user have? */
// /*AFLA*/ 	atomic_t inotify_devs;	/* How many inotify devs does this user have opened? */
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_FANOTIFY
// /*AFLA*/ 	atomic_t fanotify_listeners;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_EPOLL
// /*AFLA*/ 	atomic_long_t epoll_watches; /* The number of file descriptors currently watched */
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_POSIX_MQUEUE
// /*AFLA*/ 	/* protected by mq_lock	*/
// /*AFLA*/ 	unsigned long mq_bytes;	/* How many bytes can be allocated to mqueue? */
// /*AFLA*/ #endif
// /*AFLA*/ 	unsigned long locked_shm; /* How many pages of mlocked shm ? */
// /*AFLA*/ 	unsigned long unix_inflight;	/* How many files in flight in unix sockets */
// /*AFLA*/ 	atomic_long_t pipe_bufs;  /* how many pages are allocated in pipe buffers */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KEYS
// /*AFLA*/ 	struct key *uid_keyring;	/* UID specific keyring */
// /*AFLA*/ 	struct key *session_keyring;	/* UID's default session keyring */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	/* Hash table maintenance information */
// /*AFLA*/ 	struct hlist_node uidhash_node;
// /*AFLA*/ 	kuid_t uid;
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_PERF_EVENTS) || defined(CONFIG_BPF_SYSCALL)
// /*AFLA*/ 	atomic_long_t locked_vm;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern int uids_sysfs_init(void);
// /*AFLA*/ 
// /*AFLA*/ extern struct user_struct *find_user(kuid_t);
// /*AFLA*/ 
// /*AFLA*/ extern struct user_struct root_user;
// /*AFLA*/ #define INIT_USER (&root_user)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ struct backing_dev_info;
// /*AFLA*/ struct reclaim_state;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SCHED_INFO
// /*AFLA*/ struct sched_info {
// /*AFLA*/ 	/* cumulative counters */
// /*AFLA*/ 	unsigned long pcount;	      /* # of times run on this cpu */
// /*AFLA*/ 	unsigned long long run_delay; /* time spent waiting on a runqueue */
// /*AFLA*/ 
// /*AFLA*/ 	/* timestamps */
// /*AFLA*/ 	unsigned long long last_arrival,/* when we last ran on a cpu */
// /*AFLA*/ 			   last_queued;	/* when we were last queued to run */
// /*AFLA*/ };
// /*AFLA*/ #endif /* CONFIG_SCHED_INFO */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TASK_DELAY_ACCT
// /*AFLA*/ struct task_delay_info {
// /*AFLA*/ 	spinlock_t	lock;
// /*AFLA*/ 	unsigned int	flags;	/* Private per-task flags */
// /*AFLA*/ 
// /*AFLA*/ 	/* For each stat XXX, add following, aligned appropriately
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * struct timespec XXX_start, XXX_end;
// /*AFLA*/ 	 * u64 XXX_delay;
// /*AFLA*/ 	 * u32 XXX_count;
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Atomicity of updates to XXX_delay, XXX_count protected by
// /*AFLA*/ 	 * single lock above (split into XXX_lock if contention is an issue).
// /*AFLA*/ 	 */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * XXX_count is incremented on every XXX operation, the delay
// /*AFLA*/ 	 * associated with the operation is added to XXX_delay.
// /*AFLA*/ 	 * XXX_delay contains the accumulated delay time in nanoseconds.
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64 blkio_start;	/* Shared by blkio, swapin */
// /*AFLA*/ 	u64 blkio_delay;	/* wait for sync block io completion */
// /*AFLA*/ 	u64 swapin_delay;	/* wait for swapin block io completion */
// /*AFLA*/ 	u32 blkio_count;	/* total count of the number of sync block */
// /*AFLA*/ 				/* io operations performed */
// /*AFLA*/ 	u32 swapin_count;	/* total count of the number of swapin block */
// /*AFLA*/ 				/* io operations performed */
// /*AFLA*/ 
// /*AFLA*/ 	u64 freepages_start;
// /*AFLA*/ 	u64 freepages_delay;	/* wait for memory reclaim */
// /*AFLA*/ 	u32 freepages_count;	/* total count of memory reclaim */
// /*AFLA*/ };
// /*AFLA*/ #endif	/* CONFIG_TASK_DELAY_ACCT */
// /*AFLA*/ 
// /*AFLA*/ static inline int sched_info_on(void)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_SCHEDSTATS
// /*AFLA*/ 	return 1;
// /*AFLA*/ #elif defined(CONFIG_TASK_DELAY_ACCT)
// /*AFLA*/ 	extern int delayacct_on;
// /*AFLA*/ 	return delayacct_on;
// /*AFLA*/ #else
// /*AFLA*/ 	return 0;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SCHEDSTATS
// /*AFLA*/ void force_schedstat_enabled(void);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ enum cpu_idle_type {
// /*AFLA*/ 	CPU_IDLE,
// /*AFLA*/ 	CPU_NOT_IDLE,
// /*AFLA*/ 	CPU_NEWLY_IDLE,
// /*AFLA*/ 	CPU_MAX_IDLE_TYPES
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Integer metrics need fixed point arithmetic, e.g., sched/fair
// /*AFLA*/  * has a few: load, load_avg, util_avg, freq, and capacity.
// /*AFLA*/  *
// /*AFLA*/  * We define a basic fixed point arithmetic range, and then formalize
// /*AFLA*/  * all these metrics based on that basic range.
// /*AFLA*/  */
// /*AFLA*/ # define SCHED_FIXEDPOINT_SHIFT	10
// /*AFLA*/ # define SCHED_FIXEDPOINT_SCALE	(1L << SCHED_FIXEDPOINT_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Increase resolution of cpu_capacity calculations
// /*AFLA*/  */
// /*AFLA*/ #define SCHED_CAPACITY_SHIFT	SCHED_FIXEDPOINT_SHIFT
// /*AFLA*/ #define SCHED_CAPACITY_SCALE	(1L << SCHED_CAPACITY_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Wake-queues are lists of tasks with a pending wakeup, whose
// /*AFLA*/  * callers have already marked the task as woken internally,
// /*AFLA*/  * and can thus carry on. A common use case is being able to
// /*AFLA*/  * do the wakeups once the corresponding user lock as been
// /*AFLA*/  * released.
// /*AFLA*/  *
// /*AFLA*/  * We hold reference to each task in the list across the wakeup,
// /*AFLA*/  * thus guaranteeing that the memory is still valid by the time
// /*AFLA*/  * the actual wakeups are performed in wake_up_q().
// /*AFLA*/  *
// /*AFLA*/  * One per task suffices, because there's never a need for a task to be
// /*AFLA*/  * in two wake queues simultaneously; it is forbidden to abandon a task
// /*AFLA*/  * in a wake queue (a call to wake_up_q() _must_ follow), so if a task is
// /*AFLA*/  * already in a wake queue, the wakeup will happen soon and the second
// /*AFLA*/  * waker can just skip it.
// /*AFLA*/  *
// /*AFLA*/  * The WAKE_Q macro declares and initializes the list head.
// /*AFLA*/  * wake_up_q() does NOT reinitialize the list; it's expected to be
// /*AFLA*/  * called near the end of a function, where the fact that the queue is
// /*AFLA*/  * not used again will be easy to see by inspection.
// /*AFLA*/  *
// /*AFLA*/  * Note that this can cause spurious wakeups. schedule() callers
// /*AFLA*/  * must ensure the call is done inside a loop, confirming that the
// /*AFLA*/  * wakeup condition has in fact occurred.
// /*AFLA*/  */
// /*AFLA*/ struct wake_q_node {
// /*AFLA*/ 	struct wake_q_node *next;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct wake_q_head {
// /*AFLA*/ 	struct wake_q_node *first;
// /*AFLA*/ 	struct wake_q_node **lastp;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define WAKE_Q_TAIL ((struct wake_q_node *) 0x01)
// /*AFLA*/ 
// /*AFLA*/ #define WAKE_Q(name)					\
// /*AFLA*/ 	struct wake_q_head name = { WAKE_Q_TAIL, &name.first }
// /*AFLA*/ 
// /*AFLA*/ extern void wake_q_add(struct wake_q_head *head,
// /*AFLA*/ 		       struct task_struct *task);
// /*AFLA*/ extern void wake_up_q(struct wake_q_head *head);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * sched-domains (multiprocessor balancing) declarations:
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ #define SD_LOAD_BALANCE		0x0001	/* Do load balancing on this domain. */
// /*AFLA*/ #define SD_BALANCE_NEWIDLE	0x0002	/* Balance when about to become idle */
// /*AFLA*/ #define SD_BALANCE_EXEC		0x0004	/* Balance on exec */
// /*AFLA*/ #define SD_BALANCE_FORK		0x0008	/* Balance on fork, clone */
// /*AFLA*/ #define SD_BALANCE_WAKE		0x0010  /* Balance on wakeup */
// /*AFLA*/ #define SD_WAKE_AFFINE		0x0020	/* Wake task to waking CPU */
// /*AFLA*/ #define SD_ASYM_CPUCAPACITY	0x0040  /* Groups have different max cpu capacities */
// /*AFLA*/ #define SD_SHARE_CPUCAPACITY	0x0080	/* Domain members share cpu capacity */
// /*AFLA*/ #define SD_SHARE_POWERDOMAIN	0x0100	/* Domain members share power domain */
// /*AFLA*/ #define SD_SHARE_PKG_RESOURCES	0x0200	/* Domain members share cpu pkg resources */
// /*AFLA*/ #define SD_SERIALIZE		0x0400	/* Only a single load balancing instance */
// /*AFLA*/ #define SD_ASYM_PACKING		0x0800  /* Place busy groups earlier in the domain */
// /*AFLA*/ #define SD_PREFER_SIBLING	0x1000	/* Prefer to place tasks in a sibling domain */
// /*AFLA*/ #define SD_OVERLAP		0x2000	/* sched_domains of this level overlap */
// /*AFLA*/ #define SD_NUMA			0x4000	/* cross-node balancing */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SCHED_SMT
// /*AFLA*/ static inline int cpu_smt_flags(void)
// /*AFLA*/ {
// /*AFLA*/ 	return SD_SHARE_CPUCAPACITY | SD_SHARE_PKG_RESOURCES;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SCHED_MC
// /*AFLA*/ static inline int cpu_core_flags(void)
// /*AFLA*/ {
// /*AFLA*/ 	return SD_SHARE_PKG_RESOURCES;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ static inline int cpu_numa_flags(void)
// /*AFLA*/ {
// /*AFLA*/ 	return SD_NUMA;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct sched_domain_attr {
// /*AFLA*/ 	int relax_domain_level;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define SD_ATTR_INIT	(struct sched_domain_attr) {	\
// /*AFLA*/ 	.relax_domain_level = -1,			\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int sched_domain_level_max;
// /*AFLA*/ 
// /*AFLA*/ struct sched_group;
// /*AFLA*/ 
// /*AFLA*/ struct sched_domain_shared {
// /*AFLA*/ 	atomic_t	ref;
// /*AFLA*/ 	atomic_t	nr_busy_cpus;
// /*AFLA*/ 	int		has_idle_cores;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct sched_domain {
// /*AFLA*/ 	/* These fields must be setup */
// /*AFLA*/ 	struct sched_domain *parent;	/* top domain must be null terminated */
// /*AFLA*/ 	struct sched_domain *child;	/* bottom domain must be null terminated */
// /*AFLA*/ 	struct sched_group *groups;	/* the balancing groups of the domain */
// /*AFLA*/ 	unsigned long min_interval;	/* Minimum balance interval ms */
// /*AFLA*/ 	unsigned long max_interval;	/* Maximum balance interval ms */
// /*AFLA*/ 	unsigned int busy_factor;	/* less balancing by factor if busy */
// /*AFLA*/ 	unsigned int imbalance_pct;	/* No balance until over watermark */
// /*AFLA*/ 	unsigned int cache_nice_tries;	/* Leave cache hot tasks for # tries */
// /*AFLA*/ 	unsigned int busy_idx;
// /*AFLA*/ 	unsigned int idle_idx;
// /*AFLA*/ 	unsigned int newidle_idx;
// /*AFLA*/ 	unsigned int wake_idx;
// /*AFLA*/ 	unsigned int forkexec_idx;
// /*AFLA*/ 	unsigned int smt_gain;
// /*AFLA*/ 
// /*AFLA*/ 	int nohz_idle;			/* NOHZ IDLE status */
// /*AFLA*/ 	int flags;			/* See SD_* */
// /*AFLA*/ 	int level;
// /*AFLA*/ 
// /*AFLA*/ 	/* Runtime fields. */
// /*AFLA*/ 	unsigned long last_balance;	/* init to jiffies. units in jiffies */
// /*AFLA*/ 	unsigned int balance_interval;	/* initialise to 1. units in ms. */
// /*AFLA*/ 	unsigned int nr_balance_failed; /* initialise to 0 */
// /*AFLA*/ 
// /*AFLA*/ 	/* idle_balance() stats */
// /*AFLA*/ 	u64 max_newidle_lb_cost;
// /*AFLA*/ 	unsigned long next_decay_max_lb_cost;
// /*AFLA*/ 
// /*AFLA*/ 	u64 avg_scan_cost;		/* select_idle_sibling */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SCHEDSTATS
// /*AFLA*/ 	/* load_balance() stats */
// /*AFLA*/ 	unsigned int lb_count[CPU_MAX_IDLE_TYPES];
// /*AFLA*/ 	unsigned int lb_failed[CPU_MAX_IDLE_TYPES];
// /*AFLA*/ 	unsigned int lb_balanced[CPU_MAX_IDLE_TYPES];
// /*AFLA*/ 	unsigned int lb_imbalance[CPU_MAX_IDLE_TYPES];
// /*AFLA*/ 	unsigned int lb_gained[CPU_MAX_IDLE_TYPES];
// /*AFLA*/ 	unsigned int lb_hot_gained[CPU_MAX_IDLE_TYPES];
// /*AFLA*/ 	unsigned int lb_nobusyg[CPU_MAX_IDLE_TYPES];
// /*AFLA*/ 	unsigned int lb_nobusyq[CPU_MAX_IDLE_TYPES];
// /*AFLA*/ 
// /*AFLA*/ 	/* Active load balancing */
// /*AFLA*/ 	unsigned int alb_count;
// /*AFLA*/ 	unsigned int alb_failed;
// /*AFLA*/ 	unsigned int alb_pushed;
// /*AFLA*/ 
// /*AFLA*/ 	/* SD_BALANCE_EXEC stats */
// /*AFLA*/ 	unsigned int sbe_count;
// /*AFLA*/ 	unsigned int sbe_balanced;
// /*AFLA*/ 	unsigned int sbe_pushed;
// /*AFLA*/ 
// /*AFLA*/ 	/* SD_BALANCE_FORK stats */
// /*AFLA*/ 	unsigned int sbf_count;
// /*AFLA*/ 	unsigned int sbf_balanced;
// /*AFLA*/ 	unsigned int sbf_pushed;
// /*AFLA*/ 
// /*AFLA*/ 	/* try_to_wake_up() stats */
// /*AFLA*/ 	unsigned int ttwu_wake_remote;
// /*AFLA*/ 	unsigned int ttwu_move_affine;
// /*AFLA*/ 	unsigned int ttwu_move_balance;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_SCHED_DEBUG
// /*AFLA*/ 	char *name;
// /*AFLA*/ #endif
// /*AFLA*/ 	union {
// /*AFLA*/ 		void *private;		/* used during construction */
// /*AFLA*/ 		struct rcu_head rcu;	/* used during destruction */
// /*AFLA*/ 	};
// /*AFLA*/ 	struct sched_domain_shared *shared;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned int span_weight;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Span of all CPUs in this domain.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * NOTE: this field is variable length. (Allocated dynamically
// /*AFLA*/ 	 * by attaching extra space to the end of the structure,
// /*AFLA*/ 	 * depending on how many CPUs the kernel has booted up with)
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long span[0];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline struct cpumask *sched_domain_span(struct sched_domain *sd)
// /*AFLA*/ {
// /*AFLA*/ 	return to_cpumask(sd->span);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void partition_sched_domains(int ndoms_new, cpumask_var_t doms_new[],
// /*AFLA*/ 				    struct sched_domain_attr *dattr_new);
// /*AFLA*/ 
// /*AFLA*/ /* Allocate an array of sched domains, for partition_sched_domains(). */
// /*AFLA*/ cpumask_var_t *alloc_sched_domains(unsigned int ndoms);
// /*AFLA*/ void free_sched_domains(cpumask_var_t doms[], unsigned int ndoms);
// /*AFLA*/ 
// /*AFLA*/ bool cpus_share_cache(int this_cpu, int that_cpu);
// /*AFLA*/ 
// /*AFLA*/ typedef const struct cpumask *(*sched_domain_mask_f)(int cpu);
// /*AFLA*/ typedef int (*sched_domain_flags_f)(void);
// /*AFLA*/ 
// /*AFLA*/ #define SDTL_OVERLAP	0x01
// /*AFLA*/ 
// /*AFLA*/ struct sd_data {
// /*AFLA*/ 	struct sched_domain **__percpu sd;
// /*AFLA*/ 	struct sched_domain_shared **__percpu sds;
// /*AFLA*/ 	struct sched_group **__percpu sg;
// /*AFLA*/ 	struct sched_group_capacity **__percpu sgc;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct sched_domain_topology_level {
// /*AFLA*/ 	sched_domain_mask_f mask;
// /*AFLA*/ 	sched_domain_flags_f sd_flags;
// /*AFLA*/ 	int		    flags;
// /*AFLA*/ 	int		    numa_level;
// /*AFLA*/ 	struct sd_data      data;
// /*AFLA*/ #ifdef CONFIG_SCHED_DEBUG
// /*AFLA*/ 	char                *name;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern void set_sched_topology(struct sched_domain_topology_level *tl);
// /*AFLA*/ extern void wake_up_if_idle(int cpu);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SCHED_DEBUG
// /*AFLA*/ # define SD_INIT_NAME(type)		.name = #type
// /*AFLA*/ #else
// /*AFLA*/ # define SD_INIT_NAME(type)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ struct sched_domain_attr;
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ partition_sched_domains(int ndoms_new, cpumask_var_t doms_new[],
// /*AFLA*/ 			struct sched_domain_attr *dattr_new)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool cpus_share_cache(int this_cpu, int that_cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif	/* !CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ struct io_context;			/* See blkdev.h */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifdef ARCH_HAS_PREFETCH_SWITCH_STACK
// /*AFLA*/ extern void prefetch_stack(struct task_struct *t);
// /*AFLA*/ #else
// /*AFLA*/ static inline void prefetch_stack(struct task_struct *t) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct audit_context;		/* See audit.c */
// /*AFLA*/ struct mempolicy;
// /*AFLA*/ struct pipe_inode_info;
// /*AFLA*/ struct uts_namespace;
// /*AFLA*/ 
// /*AFLA*/ struct load_weight {
// /*AFLA*/ 	unsigned long weight;
// /*AFLA*/ 	u32 inv_weight;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The load_avg/util_avg accumulates an infinite geometric series
// /*AFLA*/  * (see __update_load_avg() in kernel/sched/fair.c).
// /*AFLA*/  *
// /*AFLA*/  * [load_avg definition]
// /*AFLA*/  *
// /*AFLA*/  *   load_avg = runnable% * scale_load_down(load)
// /*AFLA*/  *
// /*AFLA*/  * where runnable% is the time ratio that a sched_entity is runnable.
// /*AFLA*/  * For cfs_rq, it is the aggregated load_avg of all runnable and
// /*AFLA*/  * blocked sched_entities.
// /*AFLA*/  *
// /*AFLA*/  * load_avg may also take frequency scaling into account:
// /*AFLA*/  *
// /*AFLA*/  *   load_avg = runnable% * scale_load_down(load) * freq%
// /*AFLA*/  *
// /*AFLA*/  * where freq% is the CPU frequency normalized to the highest frequency.
// /*AFLA*/  *
// /*AFLA*/  * [util_avg definition]
// /*AFLA*/  *
// /*AFLA*/  *   util_avg = running% * SCHED_CAPACITY_SCALE
// /*AFLA*/  *
// /*AFLA*/  * where running% is the time ratio that a sched_entity is running on
// /*AFLA*/  * a CPU. For cfs_rq, it is the aggregated util_avg of all runnable
// /*AFLA*/  * and blocked sched_entities.
// /*AFLA*/  *
// /*AFLA*/  * util_avg may also factor frequency scaling and CPU capacity scaling:
// /*AFLA*/  *
// /*AFLA*/  *   util_avg = running% * SCHED_CAPACITY_SCALE * freq% * capacity%
// /*AFLA*/  *
// /*AFLA*/  * where freq% is the same as above, and capacity% is the CPU capacity
// /*AFLA*/  * normalized to the greatest capacity (due to uarch differences, etc).
// /*AFLA*/  *
// /*AFLA*/  * N.B., the above ratios (runnable%, running%, freq%, and capacity%)
// /*AFLA*/  * themselves are in the range of [0, 1]. To do fixed point arithmetics,
// /*AFLA*/  * we therefore scale them to as large a range as necessary. This is for
// /*AFLA*/  * example reflected by util_avg's SCHED_CAPACITY_SCALE.
// /*AFLA*/  *
// /*AFLA*/  * [Overflow issue]
// /*AFLA*/  *
// /*AFLA*/  * The 64-bit load_sum can have 4353082796 (=2^64/47742/88761) entities
// /*AFLA*/  * with the highest load (=88761), always runnable on a single cfs_rq,
// /*AFLA*/  * and should not overflow as the number already hits PID_MAX_LIMIT.
// /*AFLA*/  *
// /*AFLA*/  * For all other cases (including 32-bit kernels), struct load_weight's
// /*AFLA*/  * weight will overflow first before we do, because:
// /*AFLA*/  *
// /*AFLA*/  *    Max(load_avg) <= Max(load.weight)
// /*AFLA*/  *
// /*AFLA*/  * Then it is the load_weight's responsibility to consider overflow
// /*AFLA*/  * issues.
// /*AFLA*/  */
// /*AFLA*/ struct sched_avg {
// /*AFLA*/ 	u64 last_update_time, load_sum;
// /*AFLA*/ 	u32 util_sum, period_contrib;
// /*AFLA*/ 	unsigned long load_avg, util_avg;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SCHEDSTATS
// /*AFLA*/ struct sched_statistics {
// /*AFLA*/ 	u64			wait_start;
// /*AFLA*/ 	u64			wait_max;
// /*AFLA*/ 	u64			wait_count;
// /*AFLA*/ 	u64			wait_sum;
// /*AFLA*/ 	u64			iowait_count;
// /*AFLA*/ 	u64			iowait_sum;
// /*AFLA*/ 
// /*AFLA*/ 	u64			sleep_start;
// /*AFLA*/ 	u64			sleep_max;
// /*AFLA*/ 	s64			sum_sleep_runtime;
// /*AFLA*/ 
// /*AFLA*/ 	u64			block_start;
// /*AFLA*/ 	u64			block_max;
// /*AFLA*/ 	u64			exec_max;
// /*AFLA*/ 	u64			slice_max;
// /*AFLA*/ 
// /*AFLA*/ 	u64			nr_migrations_cold;
// /*AFLA*/ 	u64			nr_failed_migrations_affine;
// /*AFLA*/ 	u64			nr_failed_migrations_running;
// /*AFLA*/ 	u64			nr_failed_migrations_hot;
// /*AFLA*/ 	u64			nr_forced_migrations;
// /*AFLA*/ 
// /*AFLA*/ 	u64			nr_wakeups;
// /*AFLA*/ 	u64			nr_wakeups_sync;
// /*AFLA*/ 	u64			nr_wakeups_migrate;
// /*AFLA*/ 	u64			nr_wakeups_local;
// /*AFLA*/ 	u64			nr_wakeups_remote;
// /*AFLA*/ 	u64			nr_wakeups_affine;
// /*AFLA*/ 	u64			nr_wakeups_affine_attempts;
// /*AFLA*/ 	u64			nr_wakeups_passive;
// /*AFLA*/ 	u64			nr_wakeups_idle;
// /*AFLA*/ };
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct sched_entity {
// /*AFLA*/ 	struct load_weight	load;		/* for load-balancing */
// /*AFLA*/ 	struct rb_node		run_node;
// /*AFLA*/ 	struct list_head	group_node;
// /*AFLA*/ 	unsigned int		on_rq;
// /*AFLA*/ 
// /*AFLA*/ 	u64			exec_start;
// /*AFLA*/ 	u64			sum_exec_runtime;
// /*AFLA*/ 	u64			vruntime;
// /*AFLA*/ 	u64			prev_sum_exec_runtime;
// /*AFLA*/ 
// /*AFLA*/ 	u64			nr_migrations;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SCHEDSTATS
// /*AFLA*/ 	struct sched_statistics statistics;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FAIR_GROUP_SCHED
// /*AFLA*/ 	int			depth;
// /*AFLA*/ 	struct sched_entity	*parent;
// /*AFLA*/ 	/* rq on which this entity is (to be) queued: */
// /*AFLA*/ 	struct cfs_rq		*cfs_rq;
// /*AFLA*/ 	/* rq "owned" by this entity/group: */
// /*AFLA*/ 	struct cfs_rq		*my_q;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Per entity load average tracking.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Put into separate cache line so it does not
// /*AFLA*/ 	 * collide with read-mostly values above.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct sched_avg	avg ____cacheline_aligned_in_smp;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct sched_rt_entity {
// /*AFLA*/ 	struct list_head run_list;
// /*AFLA*/ 	unsigned long timeout;
// /*AFLA*/ 	unsigned long watchdog_stamp;
// /*AFLA*/ 	unsigned int time_slice;
// /*AFLA*/ 	unsigned short on_rq;
// /*AFLA*/ 	unsigned short on_list;
// /*AFLA*/ 
// /*AFLA*/ 	struct sched_rt_entity *back;
// /*AFLA*/ #ifdef CONFIG_RT_GROUP_SCHED
// /*AFLA*/ 	struct sched_rt_entity	*parent;
// /*AFLA*/ 	/* rq on which this entity is (to be) queued: */
// /*AFLA*/ 	struct rt_rq		*rt_rq;
// /*AFLA*/ 	/* rq "owned" by this entity/group: */
// /*AFLA*/ 	struct rt_rq		*my_q;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct sched_dl_entity {
// /*AFLA*/ 	struct rb_node	rb_node;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Original scheduling parameters. Copied here from sched_attr
// /*AFLA*/ 	 * during sched_setattr(), they will remain the same until
// /*AFLA*/ 	 * the next sched_setattr().
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64 dl_runtime;		/* maximum runtime for each instance	*/
// /*AFLA*/ 	u64 dl_deadline;	/* relative deadline of each instance	*/
// /*AFLA*/ 	u64 dl_period;		/* separation of two instances (period) */
// /*AFLA*/ 	u64 dl_bw;		/* dl_runtime / dl_deadline		*/
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Actual scheduling parameters. Initialized with the values above,
// /*AFLA*/ 	 * they are continously updated during task execution. Note that
// /*AFLA*/ 	 * the remaining runtime could be < 0 in case we are in overrun.
// /*AFLA*/ 	 */
// /*AFLA*/ 	s64 runtime;		/* remaining runtime for this instance	*/
// /*AFLA*/ 	u64 deadline;		/* absolute deadline for this instance	*/
// /*AFLA*/ 	unsigned int flags;	/* specifying the scheduler behaviour	*/
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Some bool flags:
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * @dl_throttled tells if we exhausted the runtime. If so, the
// /*AFLA*/ 	 * task has to wait for a replenishment to be performed at the
// /*AFLA*/ 	 * next firing of dl_timer.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * @dl_boosted tells if we are boosted due to DI. If so we are
// /*AFLA*/ 	 * outside bandwidth enforcement mechanism (but only until we
// /*AFLA*/ 	 * exit the critical section);
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * @dl_yielded tells if task gave up the cpu before consuming
// /*AFLA*/ 	 * all its available runtime during the last job.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int dl_throttled, dl_boosted, dl_yielded;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Bandwidth enforcement timer. Each -deadline task has its
// /*AFLA*/ 	 * own bandwidth to be enforced, thus we need one timer per task.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct hrtimer dl_timer;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ union rcu_special {
// /*AFLA*/ 	struct {
// /*AFLA*/ 		u8 blocked;
// /*AFLA*/ 		u8 need_qs;
// /*AFLA*/ 		u8 exp_need_qs;
// /*AFLA*/ 		u8 pad;	/* Otherwise the compiler can store garbage here. */
// /*AFLA*/ 	} b; /* Bits. */
// /*AFLA*/ 	u32 s; /* Set of bits. */
// /*AFLA*/ };
// /*AFLA*/ struct rcu_node;
// /*AFLA*/ 
// /*AFLA*/ enum perf_event_task_context {
// /*AFLA*/ 	perf_invalid_context = -1,
// /*AFLA*/ 	perf_hw_context = 0,
// /*AFLA*/ 	perf_sw_context,
// /*AFLA*/ 	perf_nr_task_contexts,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Track pages that require TLB flushes */
// /*AFLA*/ struct tlbflush_unmap_batch {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Each bit set is a CPU that potentially has a TLB entry for one of
// /*AFLA*/ 	 * the PFNs being flushed. See set_tlb_ubc_flush_pending().
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct cpumask cpumask;
// /*AFLA*/ 
// /*AFLA*/ 	/* True if any bit in cpumask is set */
// /*AFLA*/ 	bool flush_required;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If true then the PTE was dirty when unmapped. The entry must be
// /*AFLA*/ 	 * flushed before IO is initiated or a stale TLB entry potentially
// /*AFLA*/ 	 * allows an update without redirtying the page.
// /*AFLA*/ 	 */
// /*AFLA*/ 	bool writable;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct task_struct {
// /*AFLA*/ #ifdef CONFIG_THREAD_INFO_IN_TASK
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * For reasons of header soup (see current_thread_info()), this
// /*AFLA*/ 	 * must be the first element of task_struct.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct thread_info thread_info;
// /*AFLA*/ #endif
// /*AFLA*/ 	volatile long state;	/* -1 unrunnable, 0 runnable, >0 stopped */
// /*AFLA*/ 	void *stack;
// /*AFLA*/ 	atomic_t usage;
// /*AFLA*/ 	unsigned int flags;	/* per process flags, defined below */
// /*AFLA*/ 	unsigned int ptrace;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 	struct llist_node wake_entry;
// /*AFLA*/ 	int on_cpu;
// /*AFLA*/ #ifdef CONFIG_THREAD_INFO_IN_TASK
// /*AFLA*/ 	unsigned int cpu;	/* current CPU */
// /*AFLA*/ #endif
// /*AFLA*/ 	unsigned int wakee_flips;
// /*AFLA*/ 	unsigned long wakee_flip_decay_ts;
// /*AFLA*/ 	struct task_struct *last_wakee;
// /*AFLA*/ 
// /*AFLA*/ 	int wake_cpu;
// /*AFLA*/ #endif
// /*AFLA*/ 	int on_rq;
// /*AFLA*/ 
// /*AFLA*/ 	int prio, static_prio, normal_prio;
// /*AFLA*/ 	unsigned int rt_priority;
// /*AFLA*/ 	const struct sched_class *sched_class;
// /*AFLA*/ 	struct sched_entity se;
// /*AFLA*/ 	struct sched_rt_entity rt;
// /*AFLA*/ #ifdef CONFIG_CGROUP_SCHED
// /*AFLA*/ 	struct task_group *sched_task_group;
// /*AFLA*/ #endif
// /*AFLA*/ 	struct sched_dl_entity dl;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PREEMPT_NOTIFIERS
// /*AFLA*/ 	/* list of struct preempt_notifier: */
// /*AFLA*/ 	struct hlist_head preempt_notifiers;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_BLK_DEV_IO_TRACE
// /*AFLA*/ 	unsigned int btrace_seq;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	unsigned int policy;
// /*AFLA*/ 	int nr_cpus_allowed;
// /*AFLA*/ 	cpumask_t cpus_allowed;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PREEMPT_RCU
// /*AFLA*/ 	int rcu_read_lock_nesting;
// /*AFLA*/ 	union rcu_special rcu_read_unlock_special;
// /*AFLA*/ 	struct list_head rcu_node_entry;
// /*AFLA*/ 	struct rcu_node *rcu_blocked_node;
// /*AFLA*/ #endif /* #ifdef CONFIG_PREEMPT_RCU */
// /*AFLA*/ #ifdef CONFIG_TASKS_RCU
// /*AFLA*/ 	unsigned long rcu_tasks_nvcsw;
// /*AFLA*/ 	bool rcu_tasks_holdout;
// /*AFLA*/ 	struct list_head rcu_tasks_holdout_list;
// /*AFLA*/ 	int rcu_tasks_idle_cpu;
// /*AFLA*/ #endif /* #ifdef CONFIG_TASKS_RCU */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SCHED_INFO
// /*AFLA*/ 	struct sched_info sched_info;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	struct list_head tasks;
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 	struct plist_node pushable_tasks;
// /*AFLA*/ 	struct rb_node pushable_dl_tasks;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	struct mm_struct *mm, *active_mm;
// /*AFLA*/ 	/* per-thread vma caching */
// /*AFLA*/ 	u32 vmacache_seqnum;
// /*AFLA*/ 	struct vm_area_struct *vmacache[VMACACHE_SIZE];
// /*AFLA*/ #if defined(SPLIT_RSS_COUNTING)
// /*AFLA*/ 	struct task_rss_stat	rss_stat;
// /*AFLA*/ #endif
// /*AFLA*/ /* task state */
// /*AFLA*/ 	int exit_state;
// /*AFLA*/ 	int exit_code, exit_signal;
// /*AFLA*/ 	int pdeath_signal;  /*  The signal sent when the parent dies  */
// /*AFLA*/ 	unsigned long jobctl;	/* JOBCTL_*, siglock protected */
// /*AFLA*/ 
// /*AFLA*/ 	/* Used for emulating ABI behavior of previous Linux versions */
// /*AFLA*/ 	unsigned int personality;
// /*AFLA*/ 
// /*AFLA*/ 	/* scheduler bits, serialized by scheduler locks */
// /*AFLA*/ 	unsigned sched_reset_on_fork:1;
// /*AFLA*/ 	unsigned sched_contributes_to_load:1;
// /*AFLA*/ 	unsigned sched_migrated:1;
// /*AFLA*/ 	unsigned sched_remote_wakeup:1;
// /*AFLA*/ 	unsigned :0; /* force alignment to the next boundary */
// /*AFLA*/ 
// /*AFLA*/ 	/* unserialized, strictly 'current' */
// /*AFLA*/ 	unsigned in_execve:1; /* bit to tell LSMs we're in execve */
// /*AFLA*/ 	unsigned in_iowait:1;
// /*AFLA*/ #if !defined(TIF_RESTORE_SIGMASK)
// /*AFLA*/ 	unsigned restore_sigmask:1;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_MEMCG
// /*AFLA*/ 	unsigned memcg_may_oom:1;
// /*AFLA*/ #ifndef CONFIG_SLOB
// /*AFLA*/ 	unsigned memcg_kmem_skip_account:1;
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_COMPAT_BRK
// /*AFLA*/ 	unsigned brk_randomized:1;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long atomic_flags; /* Flags needing atomic access. */
// /*AFLA*/ 
// /*AFLA*/ 	struct restart_block restart_block;
// /*AFLA*/ 
// /*AFLA*/ 	pid_t pid;
// /*AFLA*/ 	pid_t tgid;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_CC_STACKPROTECTOR
// /*AFLA*/ 	/* Canary value for the -fstack-protector gcc feature */
// /*AFLA*/ 	unsigned long stack_canary;
// /*AFLA*/ #endif
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * pointers to (original) parent process, youngest child, younger sibling,
// /*AFLA*/ 	 * older sibling, respectively.  (p->father can be replaced with
// /*AFLA*/ 	 * p->real_parent->pid)
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct task_struct __rcu *real_parent; /* real parent process */
// /*AFLA*/ 	struct task_struct __rcu *parent; /* recipient of SIGCHLD, wait4() reports */
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * children/sibling forms the list of my natural children
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_head children;	/* list of my children */
// /*AFLA*/ 	struct list_head sibling;	/* linkage in my parent's children list */
// /*AFLA*/ 	struct task_struct *group_leader;	/* threadgroup leader */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * ptraced is the list of tasks this task is using ptrace on.
// /*AFLA*/ 	 * This includes both natural children and PTRACE_ATTACH targets.
// /*AFLA*/ 	 * p->ptrace_entry is p's link on the p->parent->ptraced list.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_head ptraced;
// /*AFLA*/ 	struct list_head ptrace_entry;
// /*AFLA*/ 
// /*AFLA*/ 	/* PID/PID hash table linkage. */
// /*AFLA*/ 	struct pid_link pids[PIDTYPE_MAX];
// /*AFLA*/ 	struct list_head thread_group;
// /*AFLA*/ 	struct list_head thread_node;
// /*AFLA*/ 
// /*AFLA*/ 	struct completion *vfork_done;		/* for vfork() */
// /*AFLA*/ 	int __user *set_child_tid;		/* CLONE_CHILD_SETTID */
// /*AFLA*/ 	int __user *clear_child_tid;		/* CLONE_CHILD_CLEARTID */
// /*AFLA*/ 
// /*AFLA*/ 	cputime_t utime, stime, utimescaled, stimescaled;
// /*AFLA*/ 	cputime_t gtime;
// /*AFLA*/ 	struct prev_cputime prev_cputime;
// /*AFLA*/ #ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
// /*AFLA*/ 	seqcount_t vtime_seqcount;
// /*AFLA*/ 	unsigned long long vtime_snap;
// /*AFLA*/ 	enum {
// /*AFLA*/ 		/* Task is sleeping or running in a CPU with VTIME inactive */
// /*AFLA*/ 		VTIME_INACTIVE = 0,
// /*AFLA*/ 		/* Task runs in userspace in a CPU with VTIME active */
// /*AFLA*/ 		VTIME_USER,
// /*AFLA*/ 		/* Task runs in kernelspace in a CPU with VTIME active */
// /*AFLA*/ 		VTIME_SYS,
// /*AFLA*/ 	} vtime_snap_whence;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NO_HZ_FULL
// /*AFLA*/ 	atomic_t tick_dep_mask;
// /*AFLA*/ #endif
// /*AFLA*/ 	unsigned long nvcsw, nivcsw; /* context switch counts */
// /*AFLA*/ 	u64 start_time;		/* monotonic time in nsec */
// /*AFLA*/ 	u64 real_start_time;	/* boot based time in nsec */
// /*AFLA*/ /* mm fault and swap info: this can arguably be seen as either mm-specific or thread-specific */
// /*AFLA*/ 	unsigned long min_flt, maj_flt;
// /*AFLA*/ 
// /*AFLA*/ 	struct task_cputime cputime_expires;
// /*AFLA*/ 	struct list_head cpu_timers[3];
// /*AFLA*/ 
// /*AFLA*/ /* process credentials */
// /*AFLA*/ 	const struct cred __rcu *real_cred; /* objective and real subjective task
// /*AFLA*/ 					 * credentials (COW) */
// /*AFLA*/ 	const struct cred __rcu *cred;	/* effective (overridable) subjective task
// /*AFLA*/ 					 * credentials (COW) */
// /*AFLA*/ 	char comm[TASK_COMM_LEN]; /* executable name excluding path
// /*AFLA*/ 				     - access with [gs]et_task_comm (which lock
// /*AFLA*/ 				       it with task_lock())
// /*AFLA*/ 				     - initialized normally by setup_new_exec */
// /*AFLA*/ /* file system info */
// /*AFLA*/ 	struct nameidata *nameidata;
// /*AFLA*/ #ifdef CONFIG_SYSVIPC
// /*AFLA*/ /* ipc stuff */
// /*AFLA*/ 	struct sysv_sem sysvsem;
// /*AFLA*/ 	struct sysv_shm sysvshm;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_DETECT_HUNG_TASK
// /*AFLA*/ /* hung task detection */
// /*AFLA*/ 	unsigned long last_switch_count;
// /*AFLA*/ #endif
// /*AFLA*/ /* filesystem information */
// /*AFLA*/ 	struct fs_struct *fs;
// /*AFLA*/ /* open file information */
// /*AFLA*/ 	struct files_struct *files;
// /*AFLA*/ /* namespaces */
// /*AFLA*/ 	struct nsproxy *nsproxy;
// /*AFLA*/ /* signal handlers */
// /*AFLA*/ 	struct signal_struct *signal;
// /*AFLA*/ 	struct sighand_struct *sighand;
// /*AFLA*/ 
// /*AFLA*/ 	sigset_t blocked, real_blocked;
// /*AFLA*/ 	sigset_t saved_sigmask;	/* restored if set_restore_sigmask() was used */
// /*AFLA*/ 	struct sigpending pending;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long sas_ss_sp;
// /*AFLA*/ 	size_t sas_ss_size;
// /*AFLA*/ 	unsigned sas_ss_flags;
// /*AFLA*/ 
// /*AFLA*/ 	struct callback_head *task_works;
// /*AFLA*/ 
// /*AFLA*/ 	struct audit_context *audit_context;
// /*AFLA*/ #ifdef CONFIG_AUDITSYSCALL
// /*AFLA*/ 	kuid_t loginuid;
// /*AFLA*/ 	unsigned int sessionid;
// /*AFLA*/ #endif
// /*AFLA*/ 	struct seccomp seccomp;
// /*AFLA*/ 
// /*AFLA*/ /* Thread group tracking */
// /*AFLA*/    	u32 parent_exec_id;
// /*AFLA*/    	u32 self_exec_id;
// /*AFLA*/ /* Protection of (de-)allocation: mm, files, fs, tty, keyrings, mems_allowed,
// /*AFLA*/  * mempolicy */
// /*AFLA*/ 	spinlock_t alloc_lock;
// /*AFLA*/ 
// /*AFLA*/ 	/* Protection of the PI data structures: */
// /*AFLA*/ 	raw_spinlock_t pi_lock;
// /*AFLA*/ 
// /*AFLA*/ 	struct wake_q_node wake_q;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_RT_MUTEXES
// /*AFLA*/ 	/* PI waiters blocked on a rt_mutex held by this task */
// /*AFLA*/ 	struct rb_root pi_waiters;
// /*AFLA*/ 	struct rb_node *pi_waiters_leftmost;
// /*AFLA*/ 	/* Deadlock detection and priority inheritance handling */
// /*AFLA*/ 	struct rt_mutex_waiter *pi_blocked_on;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_MUTEXES
// /*AFLA*/ 	/* mutex deadlock detection */
// /*AFLA*/ 	struct mutex_waiter *blocked_on;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_TRACE_IRQFLAGS
// /*AFLA*/ 	unsigned int irq_events;
// /*AFLA*/ 	unsigned long hardirq_enable_ip;
// /*AFLA*/ 	unsigned long hardirq_disable_ip;
// /*AFLA*/ 	unsigned int hardirq_enable_event;
// /*AFLA*/ 	unsigned int hardirq_disable_event;
// /*AFLA*/ 	int hardirqs_enabled;
// /*AFLA*/ 	int hardirq_context;
// /*AFLA*/ 	unsigned long softirq_disable_ip;
// /*AFLA*/ 	unsigned long softirq_enable_ip;
// /*AFLA*/ 	unsigned int softirq_disable_event;
// /*AFLA*/ 	unsigned int softirq_enable_event;
// /*AFLA*/ 	int softirqs_enabled;
// /*AFLA*/ 	int softirq_context;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_LOCKDEP
// /*AFLA*/ # define MAX_LOCK_DEPTH 48UL
// /*AFLA*/ 	u64 curr_chain_key;
// /*AFLA*/ 	int lockdep_depth;
// /*AFLA*/ 	unsigned int lockdep_recursion;
// /*AFLA*/ 	struct held_lock held_locks[MAX_LOCK_DEPTH];
// /*AFLA*/ 	gfp_t lockdep_reclaim_gfp;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_UBSAN
// /*AFLA*/ 	unsigned int in_ubsan;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* journalling filesystem info */
// /*AFLA*/ 	void *journal_info;
// /*AFLA*/ 
// /*AFLA*/ /* stacked block device info */
// /*AFLA*/ 	struct bio_list *bio_list;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_BLOCK
// /*AFLA*/ /* stack plugging */
// /*AFLA*/ 	struct blk_plug *plug;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* VM state */
// /*AFLA*/ 	struct reclaim_state *reclaim_state;
// /*AFLA*/ 
// /*AFLA*/ 	struct backing_dev_info *backing_dev_info;
// /*AFLA*/ 
// /*AFLA*/ 	struct io_context *io_context;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long ptrace_message;
// /*AFLA*/ 	siginfo_t *last_siginfo; /* For ptrace use.  */
// /*AFLA*/ 	struct task_io_accounting ioac;
// /*AFLA*/ #if defined(CONFIG_TASK_XACCT)
// /*AFLA*/ 	u64 acct_rss_mem1;	/* accumulated rss usage */
// /*AFLA*/ 	u64 acct_vm_mem1;	/* accumulated virtual memory usage */
// /*AFLA*/ 	cputime_t acct_timexpd;	/* stime + utime since last update */
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_CPUSETS
// /*AFLA*/ 	nodemask_t mems_allowed;	/* Protected by alloc_lock */
// /*AFLA*/ 	seqcount_t mems_allowed_seq;	/* Seqence no to catch updates */
// /*AFLA*/ 	int cpuset_mem_spread_rotor;
// /*AFLA*/ 	int cpuset_slab_spread_rotor;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_CGROUPS
// /*AFLA*/ 	/* Control Group info protected by css_set_lock */
// /*AFLA*/ 	struct css_set __rcu *cgroups;
// /*AFLA*/ 	/* cg_list protected by css_set_lock and tsk->alloc_lock */
// /*AFLA*/ 	struct list_head cg_list;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_FUTEX
// /*AFLA*/ 	struct robust_list_head __user *robust_list;
// /*AFLA*/ #ifdef CONFIG_COMPAT
// /*AFLA*/ 	struct compat_robust_list_head __user *compat_robust_list;
// /*AFLA*/ #endif
// /*AFLA*/ 	struct list_head pi_state_list;
// /*AFLA*/ 	struct futex_pi_state *pi_state_cache;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_PERF_EVENTS
// /*AFLA*/ 	struct perf_event_context *perf_event_ctxp[perf_nr_task_contexts];
// /*AFLA*/ 	struct mutex perf_event_mutex;
// /*AFLA*/ 	struct list_head perf_event_list;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_DEBUG_PREEMPT
// /*AFLA*/ 	unsigned long preempt_disable_ip;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ 	struct mempolicy *mempolicy;	/* Protected by alloc_lock */
// /*AFLA*/ 	short il_next;
// /*AFLA*/ 	short pref_node_fork;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_NUMA_BALANCING
// /*AFLA*/ 	int numa_scan_seq;
// /*AFLA*/ 	unsigned int numa_scan_period;
// /*AFLA*/ 	unsigned int numa_scan_period_max;
// /*AFLA*/ 	int numa_preferred_nid;
// /*AFLA*/ 	unsigned long numa_migrate_retry;
// /*AFLA*/ 	u64 node_stamp;			/* migration stamp  */
// /*AFLA*/ 	u64 last_task_numa_placement;
// /*AFLA*/ 	u64 last_sum_exec_runtime;
// /*AFLA*/ 	struct callback_head numa_work;
// /*AFLA*/ 
// /*AFLA*/ 	struct list_head numa_entry;
// /*AFLA*/ 	struct numa_group *numa_group;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * numa_faults is an array split into four regions:
// /*AFLA*/ 	 * faults_memory, faults_cpu, faults_memory_buffer, faults_cpu_buffer
// /*AFLA*/ 	 * in this precise order.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * faults_memory: Exponential decaying average of faults on a per-node
// /*AFLA*/ 	 * basis. Scheduling placement decisions are made based on these
// /*AFLA*/ 	 * counts. The values remain static for the duration of a PTE scan.
// /*AFLA*/ 	 * faults_cpu: Track the nodes the process was running on when a NUMA
// /*AFLA*/ 	 * hinting fault was incurred.
// /*AFLA*/ 	 * faults_memory_buffer and faults_cpu_buffer: Record faults per node
// /*AFLA*/ 	 * during the current scan window. When the scan completes, the counts
// /*AFLA*/ 	 * in faults_memory and faults_cpu decay and these values are copied.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long *numa_faults;
// /*AFLA*/ 	unsigned long total_numa_faults;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * numa_faults_locality tracks if faults recorded during the last
// /*AFLA*/ 	 * scan window were remote/local or failed to migrate. The task scan
// /*AFLA*/ 	 * period is adapted based on the locality of the faults with different
// /*AFLA*/ 	 * weights depending on whether they were shared or private faults
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long numa_faults_locality[3];
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long numa_pages_migrated;
// /*AFLA*/ #endif /* CONFIG_NUMA_BALANCING */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_ARCH_WANT_BATCHED_UNMAP_TLB_FLUSH
// /*AFLA*/ 	struct tlbflush_unmap_batch tlb_ubc;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	struct rcu_head rcu;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * cache last used pipe for splice
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct pipe_inode_info *splice_pipe;
// /*AFLA*/ 
// /*AFLA*/ 	struct page_frag task_frag;
// /*AFLA*/ 
// /*AFLA*/ #ifdef	CONFIG_TASK_DELAY_ACCT
// /*AFLA*/ 	struct task_delay_info *delays;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_FAULT_INJECTION
// /*AFLA*/ 	int make_it_fail;
// /*AFLA*/ #endif
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * when (nr_dirtied >= nr_dirtied_pause), it's time to call
// /*AFLA*/ 	 * balance_dirty_pages() for some dirty throttling pause
// /*AFLA*/ 	 */
// /*AFLA*/ 	int nr_dirtied;
// /*AFLA*/ 	int nr_dirtied_pause;
// /*AFLA*/ 	unsigned long dirty_paused_when; /* start of a write-and-pause period */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_LATENCYTOP
// /*AFLA*/ 	int latency_record_count;
// /*AFLA*/ 	struct latency_record latency_record[LT_SAVECOUNT];
// /*AFLA*/ #endif
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * time slack values; these are used to round up poll() and
// /*AFLA*/ 	 * select() etc timeout values. These are in nanoseconds.
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64 timer_slack_ns;
// /*AFLA*/ 	u64 default_timer_slack_ns;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KASAN
// /*AFLA*/ 	unsigned int kasan_depth;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_FUNCTION_GRAPH_TRACER
// /*AFLA*/ 	/* Index of current stored address in ret_stack */
// /*AFLA*/ 	int curr_ret_stack;
// /*AFLA*/ 	/* Stack of return addresses for return function tracing */
// /*AFLA*/ 	struct ftrace_ret_stack	*ret_stack;
// /*AFLA*/ 	/* time stamp for last schedule */
// /*AFLA*/ 	unsigned long long ftrace_timestamp;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Number of functions that haven't been traced
// /*AFLA*/ 	 * because of depth overrun.
// /*AFLA*/ 	 */
// /*AFLA*/ 	atomic_t trace_overrun;
// /*AFLA*/ 	/* Pause for the tracing */
// /*AFLA*/ 	atomic_t tracing_graph_pause;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_TRACING
// /*AFLA*/ 	/* state flags for use by tracers */
// /*AFLA*/ 	unsigned long trace;
// /*AFLA*/ 	/* bitmask and counter of trace recursion */
// /*AFLA*/ 	unsigned long trace_recursion;
// /*AFLA*/ #endif /* CONFIG_TRACING */
// /*AFLA*/ #ifdef CONFIG_KCOV
// /*AFLA*/ 	/* Coverage collection mode enabled for this task (0 if disabled). */
// /*AFLA*/ 	enum kcov_mode kcov_mode;
// /*AFLA*/ 	/* Size of the kcov_area. */
// /*AFLA*/ 	unsigned	kcov_size;
// /*AFLA*/ 	/* Buffer for coverage collection. */
// /*AFLA*/ 	void		*kcov_area;
// /*AFLA*/ 	/* kcov desciptor wired with this task or NULL. */
// /*AFLA*/ 	struct kcov	*kcov;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_MEMCG
// /*AFLA*/ 	struct mem_cgroup *memcg_in_oom;
// /*AFLA*/ 	gfp_t memcg_oom_gfp_mask;
// /*AFLA*/ 	int memcg_oom_order;
// /*AFLA*/ 
// /*AFLA*/ 	/* number of pages to reclaim on returning to userland */
// /*AFLA*/ 	unsigned int memcg_nr_pages_over_high;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_UPROBES
// /*AFLA*/ 	struct uprobe_task *utask;
// /*AFLA*/ #endif
// /*AFLA*/ #if defined(CONFIG_BCACHE) || defined(CONFIG_BCACHE_MODULE)
// /*AFLA*/ 	unsigned int	sequential_io;
// /*AFLA*/ 	unsigned int	sequential_io_avg;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_DEBUG_ATOMIC_SLEEP
// /*AFLA*/ 	unsigned long	task_state_change;
// /*AFLA*/ #endif
// /*AFLA*/ 	int pagefault_disabled;
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ 	struct task_struct *oom_reaper_list;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_VMAP_STACK
// /*AFLA*/ 	struct vm_struct *stack_vm_area;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_THREAD_INFO_IN_TASK
// /*AFLA*/ 	/* A live task holds one reference. */
// /*AFLA*/ 	atomic_t stack_refcount;
// /*AFLA*/ #endif
// /*AFLA*/ /* CPU-specific state of this task */
// /*AFLA*/ 	struct thread_struct thread;
// /*AFLA*/ /*
// /*AFLA*/  * WARNING: on x86, 'thread_struct' contains a variable-sized
// /*AFLA*/  * structure.  It *MUST* be at the end of 'task_struct'.
// /*AFLA*/  *
// /*AFLA*/  * Do not put anything below here!
// /*AFLA*/  */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_ARCH_WANTS_DYNAMIC_TASK_STRUCT
// /*AFLA*/ extern int arch_task_struct_size __read_mostly;
// /*AFLA*/ #else
// /*AFLA*/ # define arch_task_struct_size (sizeof(struct task_struct))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_VMAP_STACK
// /*AFLA*/ static inline struct vm_struct *task_stack_vm_area(const struct task_struct *t)
// /*AFLA*/ {
// /*AFLA*/ 	return t->stack_vm_area;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline struct vm_struct *task_stack_vm_area(const struct task_struct *t)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Future-safe accessor for struct task_struct's cpus_allowed. */
// /*AFLA*/ #define tsk_cpus_allowed(tsk) (&(tsk)->cpus_allowed)
// /*AFLA*/ 
// /*AFLA*/ static inline int tsk_nr_cpus_allowed(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return p->nr_cpus_allowed;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define TNF_MIGRATED	0x01
// /*AFLA*/ #define TNF_NO_GROUP	0x02
// /*AFLA*/ #define TNF_SHARED	0x04
// /*AFLA*/ #define TNF_FAULT_LOCAL	0x08
// /*AFLA*/ #define TNF_MIGRATE_FAIL 0x10
// /*AFLA*/ 
// /*AFLA*/ static inline bool in_vfork(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	bool ret;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * need RCU to access ->real_parent if CLONE_VM was used along with
// /*AFLA*/ 	 * CLONE_PARENT.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * We check real_parent->mm == tsk->mm because CLONE_VFORK does not
// /*AFLA*/ 	 * imply CLONE_VM
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * CLONE_VFORK can be used with CLONE_PARENT/CLONE_THREAD and thus
// /*AFLA*/ 	 * ->real_parent is not necessarily the task doing vfork(), so in
// /*AFLA*/ 	 * theory we can't rely on task_lock() if we want to dereference it.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * And in this case we can't trust the real_parent->mm == tsk->mm
// /*AFLA*/ 	 * check, it can be false negative. But we do not care, if init or
// /*AFLA*/ 	 * another oom-unkillable task does this it should blame itself.
// /*AFLA*/ 	 */
// /*AFLA*/ 	rcu_read_lock();
// /*AFLA*/ 	ret = tsk->vfork_done && tsk->real_parent->mm == tsk->mm;
// /*AFLA*/ 	rcu_read_unlock();
// /*AFLA*/ 
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA_BALANCING
// /*AFLA*/ extern void task_numa_fault(int last_node, int node, int pages, int flags);
// /*AFLA*/ extern pid_t task_numa_group_id(struct task_struct *p);
// /*AFLA*/ extern void set_numabalancing_state(bool enabled);
// /*AFLA*/ extern void task_numa_free(struct task_struct *p);
// /*AFLA*/ extern bool should_numa_migrate_memory(struct task_struct *p, struct page *page,
// /*AFLA*/ 					int src_nid, int dst_cpu);
// /*AFLA*/ #else
// /*AFLA*/ static inline void task_numa_fault(int last_node, int node, int pages,
// /*AFLA*/ 				   int flags)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline pid_t task_numa_group_id(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void set_numabalancing_state(bool enabled)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void task_numa_free(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline bool should_numa_migrate_memory(struct task_struct *p,
// /*AFLA*/ 				struct page *page, int src_nid, int dst_cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline struct pid *task_pid(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	return task->pids[PIDTYPE_PID].pid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct pid *task_tgid(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	return task->group_leader->pids[PIDTYPE_PID].pid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Without tasklist or rcu lock it is not safe to dereference
// /*AFLA*/  * the result of task_pgrp/task_session even if task == current,
// /*AFLA*/  * we can race with another thread doing sys_setsid/sys_setpgid.
// /*AFLA*/  */
// /*AFLA*/ static inline struct pid *task_pgrp(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	return task->group_leader->pids[PIDTYPE_PGID].pid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct pid *task_session(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	return task->group_leader->pids[PIDTYPE_SID].pid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct pid_namespace;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * the helpers to get the task's different pids as they are seen
// /*AFLA*/  * from various namespaces
// /*AFLA*/  *
// /*AFLA*/  * task_xid_nr()     : global id, i.e. the id seen from the init namespace;
// /*AFLA*/  * task_xid_vnr()    : virtual id, i.e. the id seen from the pid namespace of
// /*AFLA*/  *                     current.
// /*AFLA*/  * task_xid_nr_ns()  : id seen from the ns specified;
// /*AFLA*/  *
// /*AFLA*/  * set_task_vxid()   : assigns a virtual id to a task;
// /*AFLA*/  *
// /*AFLA*/  * see also pid_nr() etc in include/linux/pid.h
// /*AFLA*/  */
// /*AFLA*/ pid_t __task_pid_nr_ns(struct task_struct *task, enum pid_type type,
// /*AFLA*/ 			struct pid_namespace *ns);
// /*AFLA*/ 
// /*AFLA*/ static inline pid_t task_pid_nr(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return tsk->pid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pid_t task_pid_nr_ns(struct task_struct *tsk,
// /*AFLA*/ 					struct pid_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	return __task_pid_nr_ns(tsk, PIDTYPE_PID, ns);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pid_t task_pid_vnr(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return __task_pid_nr_ns(tsk, PIDTYPE_PID, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ static inline pid_t task_tgid_nr(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return tsk->tgid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ pid_t task_tgid_nr_ns(struct task_struct *tsk, struct pid_namespace *ns);
// /*AFLA*/ 
// /*AFLA*/ static inline pid_t task_tgid_vnr(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return pid_vnr(task_tgid(tsk));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ static inline int pid_alive(const struct task_struct *p);
// /*AFLA*/ static inline pid_t task_ppid_nr_ns(const struct task_struct *tsk, struct pid_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	pid_t pid = 0;
// /*AFLA*/ 
// /*AFLA*/ 	rcu_read_lock();
// /*AFLA*/ 	if (pid_alive(tsk))
// /*AFLA*/ 		pid = task_tgid_nr_ns(rcu_dereference(tsk->real_parent), ns);
// /*AFLA*/ 	rcu_read_unlock();
// /*AFLA*/ 
// /*AFLA*/ 	return pid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pid_t task_ppid_nr(const struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return task_ppid_nr_ns(tsk, &init_pid_ns);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pid_t task_pgrp_nr_ns(struct task_struct *tsk,
// /*AFLA*/ 					struct pid_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	return __task_pid_nr_ns(tsk, PIDTYPE_PGID, ns);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pid_t task_pgrp_vnr(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return __task_pid_nr_ns(tsk, PIDTYPE_PGID, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ static inline pid_t task_session_nr_ns(struct task_struct *tsk,
// /*AFLA*/ 					struct pid_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	return __task_pid_nr_ns(tsk, PIDTYPE_SID, ns);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pid_t task_session_vnr(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return __task_pid_nr_ns(tsk, PIDTYPE_SID, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* obsolete, do not use */
// /*AFLA*/ static inline pid_t task_pgrp_nr(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return task_pgrp_nr_ns(tsk, &init_pid_ns);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * pid_alive - check that a task structure is not stale
// /*AFLA*/  * @p: Task structure to be checked.
// /*AFLA*/  *
// /*AFLA*/  * Test if a process is not yet dead (at most zombie state)
// /*AFLA*/  * If pid_alive fails, then pointers within the task structure
// /*AFLA*/  * can be stale and must not be dereferenced.
// /*AFLA*/  *
// /*AFLA*/  * Return: 1 if the process is alive. 0 otherwise.
// /*AFLA*/  */
// /*AFLA*/ static inline int pid_alive(const struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return p->pids[PIDTYPE_PID].pid != NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * is_global_init - check if a task structure is init. Since init
// /*AFLA*/  * is free to have sub-threads we need to check tgid.
// /*AFLA*/  * @tsk: Task structure to be checked.
// /*AFLA*/  *
// /*AFLA*/  * Check if a task structure is the first user space task the kernel created.
// /*AFLA*/  *
// /*AFLA*/  * Return: 1 if the task structure is init. 0 otherwise.
// /*AFLA*/  */
// /*AFLA*/ static inline int is_global_init(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return task_tgid_nr(tsk) == 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct pid *cad_pid;
// /*AFLA*/ 
// /*AFLA*/ extern void free_task(struct task_struct *tsk);
// /*AFLA*/ #define get_task_struct(tsk) do { atomic_inc(&(tsk)->usage); } while(0)
// /*AFLA*/ 
// /*AFLA*/ extern void __put_task_struct(struct task_struct *t);
// /*AFLA*/ 
// /*AFLA*/ static inline void put_task_struct(struct task_struct *t)
// /*AFLA*/ {
// /*AFLA*/ 	if (atomic_dec_and_test(&t->usage))
// /*AFLA*/ 		__put_task_struct(t);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct task_struct *task_rcu_dereference(struct task_struct **ptask);
// /*AFLA*/ struct task_struct *try_get_task_struct(struct task_struct **ptask);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
// /*AFLA*/ extern void task_cputime(struct task_struct *t,
// /*AFLA*/ 			 cputime_t *utime, cputime_t *stime);
// /*AFLA*/ extern void task_cputime_scaled(struct task_struct *t,
// /*AFLA*/ 				cputime_t *utimescaled, cputime_t *stimescaled);
// /*AFLA*/ extern cputime_t task_gtime(struct task_struct *t);
// /*AFLA*/ #else
// /*AFLA*/ static inline void task_cputime(struct task_struct *t,
// /*AFLA*/ 				cputime_t *utime, cputime_t *stime)
// /*AFLA*/ {
// /*AFLA*/ 	if (utime)
// /*AFLA*/ 		*utime = t->utime;
// /*AFLA*/ 	if (stime)
// /*AFLA*/ 		*stime = t->stime;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void task_cputime_scaled(struct task_struct *t,
// /*AFLA*/ 				       cputime_t *utimescaled,
// /*AFLA*/ 				       cputime_t *stimescaled)
// /*AFLA*/ {
// /*AFLA*/ 	if (utimescaled)
// /*AFLA*/ 		*utimescaled = t->utimescaled;
// /*AFLA*/ 	if (stimescaled)
// /*AFLA*/ 		*stimescaled = t->stimescaled;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline cputime_t task_gtime(struct task_struct *t)
// /*AFLA*/ {
// /*AFLA*/ 	return t->gtime;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ extern void task_cputime_adjusted(struct task_struct *p, cputime_t *ut, cputime_t *st);
// /*AFLA*/ extern void thread_group_cputime_adjusted(struct task_struct *p, cputime_t *ut, cputime_t *st);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Per process flags
// /*AFLA*/  */
// /*AFLA*/ #define PF_EXITING	0x00000004	/* getting shut down */
// /*AFLA*/ #define PF_EXITPIDONE	0x00000008	/* pi exit done on shut down */
// /*AFLA*/ #define PF_VCPU		0x00000010	/* I'm a virtual CPU */
// /*AFLA*/ #define PF_WQ_WORKER	0x00000020	/* I'm a workqueue worker */
// /*AFLA*/ #define PF_FORKNOEXEC	0x00000040	/* forked but didn't exec */
// /*AFLA*/ #define PF_MCE_PROCESS  0x00000080      /* process policy on mce errors */
// /*AFLA*/ #define PF_SUPERPRIV	0x00000100	/* used super-user privileges */
// /*AFLA*/ #define PF_DUMPCORE	0x00000200	/* dumped core */
// /*AFLA*/ #define PF_SIGNALED	0x00000400	/* killed by a signal */
// /*AFLA*/ #define PF_MEMALLOC	0x00000800	/* Allocating memory */
// /*AFLA*/ #define PF_NPROC_EXCEEDED 0x00001000	/* set_user noticed that RLIMIT_NPROC was exceeded */
// /*AFLA*/ #define PF_USED_MATH	0x00002000	/* if unset the fpu must be initialized before use */
// /*AFLA*/ #define PF_USED_ASYNC	0x00004000	/* used async_schedule*(), used by module init */
// /*AFLA*/ #define PF_NOFREEZE	0x00008000	/* this thread should not be frozen */
// /*AFLA*/ #define PF_FROZEN	0x00010000	/* frozen for system suspend */
// /*AFLA*/ #define PF_FSTRANS	0x00020000	/* inside a filesystem transaction */
// /*AFLA*/ #define PF_KSWAPD	0x00040000	/* I am kswapd */
// /*AFLA*/ #define PF_MEMALLOC_NOIO 0x00080000	/* Allocating memory without IO involved */
// /*AFLA*/ #define PF_LESS_THROTTLE 0x00100000	/* Throttle me less: I clean memory */
// /*AFLA*/ #define PF_KTHREAD	0x00200000	/* I am a kernel thread */
// /*AFLA*/ #define PF_RANDOMIZE	0x00400000	/* randomize virtual address space */
// /*AFLA*/ #define PF_SWAPWRITE	0x00800000	/* Allowed to write to swap */
// /*AFLA*/ #define PF_NO_SETAFFINITY 0x04000000	/* Userland is not allowed to meddle with cpus_allowed */
// /*AFLA*/ #define PF_MCE_EARLY    0x08000000      /* Early kill for mce process policy */
// /*AFLA*/ #define PF_MUTEX_TESTER	0x20000000	/* Thread belongs to the rt mutex tester */
// /*AFLA*/ #define PF_FREEZER_SKIP	0x40000000	/* Freezer should not count it as freezable */
// /*AFLA*/ #define PF_SUSPEND_TASK 0x80000000      /* this thread called freeze_processes and should not be frozen */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Only the _current_ task can read/write to tsk->flags, but other
// /*AFLA*/  * tasks can access tsk->flags in readonly mode for example
// /*AFLA*/  * with tsk_used_math (like during threaded core dumping).
// /*AFLA*/  * There is however an exception to this rule during ptrace
// /*AFLA*/  * or during fork: the ptracer task is allowed to write to the
// /*AFLA*/  * child->flags of its traced child (same goes for fork, the parent
// /*AFLA*/  * can write to the child->flags), because we're guaranteed the
// /*AFLA*/  * child is not running and in turn not changing child->flags
// /*AFLA*/  * at the same time the parent does it.
// /*AFLA*/  */
// /*AFLA*/ #define clear_stopped_child_used_math(child) do { (child)->flags &= ~PF_USED_MATH; } while (0)
// /*AFLA*/ #define set_stopped_child_used_math(child) do { (child)->flags |= PF_USED_MATH; } while (0)
// /*AFLA*/ #define clear_used_math() clear_stopped_child_used_math(current)
// /*AFLA*/ #define set_used_math() set_stopped_child_used_math(current)
// /*AFLA*/ #define conditional_stopped_child_used_math(condition, child) \
// /*AFLA*/ 	do { (child)->flags &= ~PF_USED_MATH, (child)->flags |= (condition) ? PF_USED_MATH : 0; } while (0)
// /*AFLA*/ #define conditional_used_math(condition) \
// /*AFLA*/ 	conditional_stopped_child_used_math(condition, current)
// /*AFLA*/ #define copy_to_stopped_child_used_math(child) \
// /*AFLA*/ 	do { (child)->flags &= ~PF_USED_MATH, (child)->flags |= current->flags & PF_USED_MATH; } while (0)
// /*AFLA*/ /* NOTE: this will return 0 or PF_USED_MATH, it will never return 1 */
// /*AFLA*/ #define tsk_used_math(p) ((p)->flags & PF_USED_MATH)
// /*AFLA*/ #define used_math() tsk_used_math(current)
// /*AFLA*/ 
// /*AFLA*/ /* __GFP_IO isn't allowed if PF_MEMALLOC_NOIO is set in current->flags
// /*AFLA*/  * __GFP_FS is also cleared as it implies __GFP_IO.
// /*AFLA*/  */
// /*AFLA*/ static inline gfp_t memalloc_noio_flags(gfp_t flags)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(current->flags & PF_MEMALLOC_NOIO))
// /*AFLA*/ 		flags &= ~(__GFP_IO | __GFP_FS);
// /*AFLA*/ 	return flags;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int memalloc_noio_save(void)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned int flags = current->flags & PF_MEMALLOC_NOIO;
// /*AFLA*/ 	current->flags |= PF_MEMALLOC_NOIO;
// /*AFLA*/ 	return flags;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void memalloc_noio_restore(unsigned int flags)
// /*AFLA*/ {
// /*AFLA*/ 	current->flags = (current->flags & ~PF_MEMALLOC_NOIO) | flags;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Per-process atomic flags. */
// /*AFLA*/ #define PFA_NO_NEW_PRIVS 0	/* May not gain new privileges. */
// /*AFLA*/ #define PFA_SPREAD_PAGE  1      /* Spread page cache over cpuset */
// /*AFLA*/ #define PFA_SPREAD_SLAB  2      /* Spread some slab caches over cpuset */
// /*AFLA*/ #define PFA_LMK_WAITING  3      /* Lowmemorykiller is waiting */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define TASK_PFA_TEST(name, func)					\
// /*AFLA*/ 	static inline bool task_##func(struct task_struct *p)		\
// /*AFLA*/ 	{ return test_bit(PFA_##name, &p->atomic_flags); }
// /*AFLA*/ #define TASK_PFA_SET(name, func)					\
// /*AFLA*/ 	static inline void task_set_##func(struct task_struct *p)	\
// /*AFLA*/ 	{ set_bit(PFA_##name, &p->atomic_flags); }
// /*AFLA*/ #define TASK_PFA_CLEAR(name, func)					\
// /*AFLA*/ 	static inline void task_clear_##func(struct task_struct *p)	\
// /*AFLA*/ 	{ clear_bit(PFA_##name, &p->atomic_flags); }
// /*AFLA*/ 
// /*AFLA*/ TASK_PFA_TEST(NO_NEW_PRIVS, no_new_privs)
// /*AFLA*/ TASK_PFA_SET(NO_NEW_PRIVS, no_new_privs)
// /*AFLA*/ 
// /*AFLA*/ TASK_PFA_TEST(SPREAD_PAGE, spread_page)
// /*AFLA*/ TASK_PFA_SET(SPREAD_PAGE, spread_page)
// /*AFLA*/ TASK_PFA_CLEAR(SPREAD_PAGE, spread_page)
// /*AFLA*/ 
// /*AFLA*/ TASK_PFA_TEST(SPREAD_SLAB, spread_slab)
// /*AFLA*/ TASK_PFA_SET(SPREAD_SLAB, spread_slab)
// /*AFLA*/ TASK_PFA_CLEAR(SPREAD_SLAB, spread_slab)
// /*AFLA*/ 
// /*AFLA*/ TASK_PFA_TEST(LMK_WAITING, lmk_waiting)
// /*AFLA*/ TASK_PFA_SET(LMK_WAITING, lmk_waiting)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * task->jobctl flags
// /*AFLA*/  */
// /*AFLA*/ #define JOBCTL_STOP_SIGMASK	0xffff	/* signr of the last group stop */
// /*AFLA*/ 
// /*AFLA*/ #define JOBCTL_STOP_DEQUEUED_BIT 16	/* stop signal dequeued */
// /*AFLA*/ #define JOBCTL_STOP_PENDING_BIT	17	/* task should stop for group stop */
// /*AFLA*/ #define JOBCTL_STOP_CONSUME_BIT	18	/* consume group stop count */
// /*AFLA*/ #define JOBCTL_TRAP_STOP_BIT	19	/* trap for STOP */
// /*AFLA*/ #define JOBCTL_TRAP_NOTIFY_BIT	20	/* trap for NOTIFY */
// /*AFLA*/ #define JOBCTL_TRAPPING_BIT	21	/* switching to TRACED */
// /*AFLA*/ #define JOBCTL_LISTENING_BIT	22	/* ptracer is listening for events */
// /*AFLA*/ 
// /*AFLA*/ #define JOBCTL_STOP_DEQUEUED	(1UL << JOBCTL_STOP_DEQUEUED_BIT)
// /*AFLA*/ #define JOBCTL_STOP_PENDING	(1UL << JOBCTL_STOP_PENDING_BIT)
// /*AFLA*/ #define JOBCTL_STOP_CONSUME	(1UL << JOBCTL_STOP_CONSUME_BIT)
// /*AFLA*/ #define JOBCTL_TRAP_STOP	(1UL << JOBCTL_TRAP_STOP_BIT)
// /*AFLA*/ #define JOBCTL_TRAP_NOTIFY	(1UL << JOBCTL_TRAP_NOTIFY_BIT)
// /*AFLA*/ #define JOBCTL_TRAPPING		(1UL << JOBCTL_TRAPPING_BIT)
// /*AFLA*/ #define JOBCTL_LISTENING	(1UL << JOBCTL_LISTENING_BIT)
// /*AFLA*/ 
// /*AFLA*/ #define JOBCTL_TRAP_MASK	(JOBCTL_TRAP_STOP | JOBCTL_TRAP_NOTIFY)
// /*AFLA*/ #define JOBCTL_PENDING_MASK	(JOBCTL_STOP_PENDING | JOBCTL_TRAP_MASK)
// /*AFLA*/ 
// /*AFLA*/ extern bool task_set_jobctl_pending(struct task_struct *task,
// /*AFLA*/ 				    unsigned long mask);
// /*AFLA*/ extern void task_clear_jobctl_trapping(struct task_struct *task);
// /*AFLA*/ extern void task_clear_jobctl_pending(struct task_struct *task,
// /*AFLA*/ 				      unsigned long mask);
// /*AFLA*/ 
// /*AFLA*/ static inline void rcu_copy_process(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_PREEMPT_RCU
// /*AFLA*/ 	p->rcu_read_lock_nesting = 0;
// /*AFLA*/ 	p->rcu_read_unlock_special.s = 0;
// /*AFLA*/ 	p->rcu_blocked_node = NULL;
// /*AFLA*/ 	INIT_LIST_HEAD(&p->rcu_node_entry);
// /*AFLA*/ #endif /* #ifdef CONFIG_PREEMPT_RCU */
// /*AFLA*/ #ifdef CONFIG_TASKS_RCU
// /*AFLA*/ 	p->rcu_tasks_holdout = false;
// /*AFLA*/ 	INIT_LIST_HEAD(&p->rcu_tasks_holdout_list);
// /*AFLA*/ 	p->rcu_tasks_idle_cpu = -1;
// /*AFLA*/ #endif /* #ifdef CONFIG_TASKS_RCU */
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void tsk_restore_flags(struct task_struct *task,
// /*AFLA*/ 				unsigned long orig_flags, unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ 	task->flags &= ~flags;
// /*AFLA*/ 	task->flags |= orig_flags & flags;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int cpuset_cpumask_can_shrink(const struct cpumask *cur,
// /*AFLA*/ 				     const struct cpumask *trial);
// /*AFLA*/ extern int task_can_attach(struct task_struct *p,
// /*AFLA*/ 			   const struct cpumask *cs_cpus_allowed);
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ extern void do_set_cpus_allowed(struct task_struct *p,
// /*AFLA*/ 			       const struct cpumask *new_mask);
// /*AFLA*/ 
// /*AFLA*/ extern int set_cpus_allowed_ptr(struct task_struct *p,
// /*AFLA*/ 				const struct cpumask *new_mask);
// /*AFLA*/ #else
// /*AFLA*/ static inline void do_set_cpus_allowed(struct task_struct *p,
// /*AFLA*/ 				      const struct cpumask *new_mask)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline int set_cpus_allowed_ptr(struct task_struct *p,
// /*AFLA*/ 				       const struct cpumask *new_mask)
// /*AFLA*/ {
// /*AFLA*/ 	if (!cpumask_test_cpu(0, new_mask))
// /*AFLA*/ 		return -EINVAL;
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NO_HZ_COMMON
// /*AFLA*/ void calc_load_enter_idle(void);
// /*AFLA*/ void calc_load_exit_idle(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void calc_load_enter_idle(void) { }
// /*AFLA*/ static inline void calc_load_exit_idle(void) { }
// /*AFLA*/ #endif /* CONFIG_NO_HZ_COMMON */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Do not use outside of architecture code which knows its limitations.
// /*AFLA*/  *
// /*AFLA*/  * sched_clock() has no promise of monotonicity or bounded drift between
// /*AFLA*/  * CPUs, use (which you should not) requires disabling IRQs.
// /*AFLA*/  *
// /*AFLA*/  * Please use one of the three interfaces below.
// /*AFLA*/  */
// /*AFLA*/ extern unsigned long long notrace sched_clock(void);
// /*AFLA*/ /*
// /*AFLA*/  * See the comment in kernel/sched/clock.c
// /*AFLA*/  */
// /*AFLA*/ extern u64 running_clock(void);
// /*AFLA*/ extern u64 sched_clock_cpu(int cpu);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ extern void sched_clock_init(void);
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_HAVE_UNSTABLE_SCHED_CLOCK
// /*AFLA*/ static inline void sched_clock_tick(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sched_clock_idle_sleep_event(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sched_clock_idle_wakeup_event(u64 delta_ns)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u64 cpu_clock(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return sched_clock();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u64 local_clock(void)
// /*AFLA*/ {
// /*AFLA*/ 	return sched_clock();
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ /*
// /*AFLA*/  * Architectures can set this to 1 if they have specified
// /*AFLA*/  * CONFIG_HAVE_UNSTABLE_SCHED_CLOCK in their arch Kconfig,
// /*AFLA*/  * but then during bootup it turns out that sched_clock()
// /*AFLA*/  * is reliable after all:
// /*AFLA*/  */
// /*AFLA*/ extern int sched_clock_stable(void);
// /*AFLA*/ extern void set_sched_clock_stable(void);
// /*AFLA*/ extern void clear_sched_clock_stable(void);
// /*AFLA*/ 
// /*AFLA*/ extern void sched_clock_tick(void);
// /*AFLA*/ extern void sched_clock_idle_sleep_event(void);
// /*AFLA*/ extern void sched_clock_idle_wakeup_event(u64 delta_ns);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * As outlined in clock.c, provides a fast, high resolution, nanosecond
// /*AFLA*/  * time source that is monotonic per cpu argument and has bounded drift
// /*AFLA*/  * between cpus.
// /*AFLA*/  *
// /*AFLA*/  * ######################### BIG FAT WARNING ##########################
// /*AFLA*/  * # when comparing cpu_clock(i) to cpu_clock(j) for i != j, time can #
// /*AFLA*/  * # go backwards !!                                                  #
// /*AFLA*/  * ####################################################################
// /*AFLA*/  */
// /*AFLA*/ static inline u64 cpu_clock(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return sched_clock_cpu(cpu);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u64 local_clock(void)
// /*AFLA*/ {
// /*AFLA*/ 	return sched_clock_cpu(raw_smp_processor_id());
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_IRQ_TIME_ACCOUNTING
// /*AFLA*/ /*
// /*AFLA*/  * An i/f to runtime opt-in for irq time accounting based off of sched_clock.
// /*AFLA*/  * The reason for this explicit opt-in is not to have perf penalty with
// /*AFLA*/  * slow sched_clocks.
// /*AFLA*/  */
// /*AFLA*/ extern void enable_sched_clock_irqtime(void);
// /*AFLA*/ extern void disable_sched_clock_irqtime(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void enable_sched_clock_irqtime(void) {}
// /*AFLA*/ static inline void disable_sched_clock_irqtime(void) {}
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long long
// /*AFLA*/ task_sched_runtime(struct task_struct *task);
// /*AFLA*/ 
// /*AFLA*/ /* sched_exec is called by processes performing an exec */
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ extern void sched_exec(void);
// /*AFLA*/ #else
// /*AFLA*/ #define sched_exec()   {}
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void sched_clock_idle_sleep_event(void);
// /*AFLA*/ extern void sched_clock_idle_wakeup_event(u64 delta_ns);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HOTPLUG_CPU
// /*AFLA*/ extern void idle_task_exit(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void idle_task_exit(void) {}
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_NO_HZ_COMMON) && defined(CONFIG_SMP)
// /*AFLA*/ extern void wake_up_nohz_cpu(int cpu);
// /*AFLA*/ #else
// /*AFLA*/ static inline void wake_up_nohz_cpu(int cpu) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NO_HZ_FULL
// /*AFLA*/ extern u64 scheduler_tick_max_deferment(void);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SCHED_AUTOGROUP
// /*AFLA*/ extern void sched_autogroup_create_attach(struct task_struct *p);
// /*AFLA*/ extern void sched_autogroup_detach(struct task_struct *p);
// /*AFLA*/ extern void sched_autogroup_fork(struct signal_struct *sig);
// /*AFLA*/ extern void sched_autogroup_exit(struct signal_struct *sig);
// /*AFLA*/ extern void sched_autogroup_exit_task(struct task_struct *p);
// /*AFLA*/ #ifdef CONFIG_PROC_FS
// /*AFLA*/ extern void proc_sched_autogroup_show_task(struct task_struct *p, struct seq_file *m);
// /*AFLA*/ extern int proc_sched_autogroup_set_nice(struct task_struct *p, int nice);
// /*AFLA*/ #endif
// /*AFLA*/ #else
// /*AFLA*/ static inline void sched_autogroup_create_attach(struct task_struct *p) { }
// /*AFLA*/ static inline void sched_autogroup_detach(struct task_struct *p) { }
// /*AFLA*/ static inline void sched_autogroup_fork(struct signal_struct *sig) { }
// /*AFLA*/ static inline void sched_autogroup_exit(struct signal_struct *sig) { }
// /*AFLA*/ static inline void sched_autogroup_exit_task(struct task_struct *p) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern int yield_to(struct task_struct *p, bool preempt);
// /*AFLA*/ extern void set_user_nice(struct task_struct *p, long nice);
// /*AFLA*/ extern int task_prio(const struct task_struct *p);
// /*AFLA*/ /**
// /*AFLA*/  * task_nice - return the nice value of a given task.
// /*AFLA*/  * @p: the task in question.
// /*AFLA*/  *
// /*AFLA*/  * Return: The nice value [ -20 ... 0 ... 19 ].
// /*AFLA*/  */
// /*AFLA*/ static inline int task_nice(const struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return PRIO_TO_NICE((p)->static_prio);
// /*AFLA*/ }
// /*AFLA*/ extern int can_nice(const struct task_struct *p, const int nice);
// /*AFLA*/ extern int task_curr(const struct task_struct *p);
// /*AFLA*/ extern int idle_cpu(int cpu);
// /*AFLA*/ extern int sched_setscheduler(struct task_struct *, int,
// /*AFLA*/ 			      const struct sched_param *);
// /*AFLA*/ extern int sched_setscheduler_nocheck(struct task_struct *, int,
// /*AFLA*/ 				      const struct sched_param *);
// /*AFLA*/ extern int sched_setattr(struct task_struct *,
// /*AFLA*/ 			 const struct sched_attr *);
// /*AFLA*/ extern struct task_struct *idle_task(int cpu);
// /*AFLA*/ /**
// /*AFLA*/  * is_idle_task - is the specified task an idle task?
// /*AFLA*/  * @p: the task in question.
// /*AFLA*/  *
// /*AFLA*/  * Return: 1 if @p is an idle task. 0 otherwise.
// /*AFLA*/  */
// /*AFLA*/ static inline bool is_idle_task(const struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return p->pid == 0;
// /*AFLA*/ }
// /*AFLA*/ extern struct task_struct *curr_task(int cpu);
// /*AFLA*/ extern void ia64_set_curr_task(int cpu, struct task_struct *p);
// /*AFLA*/ 
// /*AFLA*/ void yield(void);
// /*AFLA*/ 
// /*AFLA*/ union thread_union {
// /*AFLA*/ #ifndef CONFIG_THREAD_INFO_IN_TASK
// /*AFLA*/ 	struct thread_info thread_info;
// /*AFLA*/ #endif
// /*AFLA*/ 	unsigned long stack[THREAD_SIZE/sizeof(long)];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_KSTACK_END
// /*AFLA*/ static inline int kstack_end(void *addr)
// /*AFLA*/ {
// /*AFLA*/ 	/* Reliable end of stack detection:
// /*AFLA*/ 	 * Some APM bios versions misalign the stack
// /*AFLA*/ 	 */
// /*AFLA*/ 	return !(((unsigned long)addr+sizeof(void*)-1) & (THREAD_SIZE-sizeof(void*)));
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern union thread_union init_thread_union;
// /*AFLA*/ extern struct task_struct init_task;
// /*AFLA*/ 
// /*AFLA*/ extern struct   mm_struct init_mm;
// /*AFLA*/ 
// /*AFLA*/ extern struct pid_namespace init_pid_ns;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * find a task by one of its numerical ids
// /*AFLA*/  *
// /*AFLA*/  * find_task_by_pid_ns():
// /*AFLA*/  *      finds a task by its pid in the specified namespace
// /*AFLA*/  * find_task_by_vpid():
// /*AFLA*/  *      finds a task by its virtual pid
// /*AFLA*/  *
// /*AFLA*/  * see also find_vpid() etc in include/linux/pid.h
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ extern struct task_struct *find_task_by_vpid(pid_t nr);
// /*AFLA*/ extern struct task_struct *find_task_by_pid_ns(pid_t nr,
// /*AFLA*/ 		struct pid_namespace *ns);
// /*AFLA*/ 
// /*AFLA*/ /* per-UID process charging. */
// /*AFLA*/ extern struct user_struct * alloc_uid(kuid_t);
// /*AFLA*/ static inline struct user_struct *get_uid(struct user_struct *u)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_inc(&u->__count);
// /*AFLA*/ 	return u;
// /*AFLA*/ }
// /*AFLA*/ extern void free_uid(struct user_struct *);
// /*AFLA*/ 
// /*AFLA*/ #include <asm/current.h>
// /*AFLA*/ 
// /*AFLA*/ extern void xtime_update(unsigned long ticks);
// /*AFLA*/ 
// /*AFLA*/ extern int wake_up_state(struct task_struct *tsk, unsigned int state);
// /*AFLA*/ extern int wake_up_process(struct task_struct *tsk);
// /*AFLA*/ extern void wake_up_new_task(struct task_struct *tsk);
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/  extern void kick_process(struct task_struct *tsk);
// /*AFLA*/ #else
// /*AFLA*/  static inline void kick_process(struct task_struct *tsk) { }
// /*AFLA*/ #endif
// /*AFLA*/ extern int sched_fork(unsigned long clone_flags, struct task_struct *p);
// /*AFLA*/ extern void sched_dead(struct task_struct *p);
// /*AFLA*/ 
// /*AFLA*/ extern void proc_caches_init(void);
// /*AFLA*/ extern void flush_signals(struct task_struct *);
// /*AFLA*/ extern void ignore_signals(struct task_struct *);
// /*AFLA*/ extern void flush_signal_handlers(struct task_struct *, int force_default);
// /*AFLA*/ extern int dequeue_signal(struct task_struct *tsk, sigset_t *mask, siginfo_t *info);
// /*AFLA*/ 
// /*AFLA*/ static inline int kernel_dequeue_signal(siginfo_t *info)
// /*AFLA*/ {
// /*AFLA*/ 	struct task_struct *tsk = current;
// /*AFLA*/ 	siginfo_t __info;
// /*AFLA*/ 	int ret;
// /*AFLA*/ 
// /*AFLA*/ 	spin_lock_irq(&tsk->sighand->siglock);
// /*AFLA*/ 	ret = dequeue_signal(tsk, &tsk->blocked, info ?: &__info);
// /*AFLA*/ 	spin_unlock_irq(&tsk->sighand->siglock);
// /*AFLA*/ 
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void kernel_signal_stop(void)
// /*AFLA*/ {
// /*AFLA*/ 	spin_lock_irq(&current->sighand->siglock);
// /*AFLA*/ 	if (current->jobctl & JOBCTL_STOP_DEQUEUED)
// /*AFLA*/ 		__set_current_state(TASK_STOPPED);
// /*AFLA*/ 	spin_unlock_irq(&current->sighand->siglock);
// /*AFLA*/ 
// /*AFLA*/ 	schedule();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void release_task(struct task_struct * p);
// /*AFLA*/ extern int send_sig_info(int, struct siginfo *, struct task_struct *);
// /*AFLA*/ extern int force_sigsegv(int, struct task_struct *);
// /*AFLA*/ extern int force_sig_info(int, struct siginfo *, struct task_struct *);
// /*AFLA*/ extern int __kill_pgrp_info(int sig, struct siginfo *info, struct pid *pgrp);
// /*AFLA*/ extern int kill_pid_info(int sig, struct siginfo *info, struct pid *pid);
// /*AFLA*/ extern int kill_pid_info_as_cred(int, struct siginfo *, struct pid *,
// /*AFLA*/ 				const struct cred *, u32);
// /*AFLA*/ extern int kill_pgrp(struct pid *pid, int sig, int priv);
// /*AFLA*/ extern int kill_pid(struct pid *pid, int sig, int priv);
// /*AFLA*/ extern int kill_proc_info(int, struct siginfo *, pid_t);
// /*AFLA*/ extern __must_check bool do_notify_parent(struct task_struct *, int);
// /*AFLA*/ extern void __wake_up_parent(struct task_struct *p, struct task_struct *parent);
// /*AFLA*/ extern void force_sig(int, struct task_struct *);
// /*AFLA*/ extern int send_sig(int, struct task_struct *, int);
// /*AFLA*/ extern int zap_other_threads(struct task_struct *p);
// /*AFLA*/ extern struct sigqueue *sigqueue_alloc(void);
// /*AFLA*/ extern void sigqueue_free(struct sigqueue *);
// /*AFLA*/ extern int send_sigqueue(struct sigqueue *,  struct task_struct *, int group);
// /*AFLA*/ extern int do_sigaction(int, struct k_sigaction *, struct k_sigaction *);
// /*AFLA*/ 
// /*AFLA*/ #ifdef TIF_RESTORE_SIGMASK
// /*AFLA*/ /*
// /*AFLA*/  * Legacy restore_sigmask accessors.  These are inefficient on
// /*AFLA*/  * SMP architectures because they require atomic operations.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * set_restore_sigmask() - make sure saved_sigmask processing gets done
// /*AFLA*/  *
// /*AFLA*/  * This sets TIF_RESTORE_SIGMASK and ensures that the arch signal code
// /*AFLA*/  * will run before returning to user mode, to process the flag.  For
// /*AFLA*/  * all callers, TIF_SIGPENDING is already set or it's no harm to set
// /*AFLA*/  * it.  TIF_RESTORE_SIGMASK need not be in the set of bits that the
// /*AFLA*/  * arch code will notice on return to user mode, in case those bits
// /*AFLA*/  * are scarce.  We set TIF_SIGPENDING here to ensure that the arch
// /*AFLA*/  * signal code always gets run when TIF_RESTORE_SIGMASK is set.
// /*AFLA*/  */
// /*AFLA*/ static inline void set_restore_sigmask(void)
// /*AFLA*/ {
// /*AFLA*/ 	set_thread_flag(TIF_RESTORE_SIGMASK);
// /*AFLA*/ 	WARN_ON(!test_thread_flag(TIF_SIGPENDING));
// /*AFLA*/ }
// /*AFLA*/ static inline void clear_restore_sigmask(void)
// /*AFLA*/ {
// /*AFLA*/ 	clear_thread_flag(TIF_RESTORE_SIGMASK);
// /*AFLA*/ }
// /*AFLA*/ static inline bool test_restore_sigmask(void)
// /*AFLA*/ {
// /*AFLA*/ 	return test_thread_flag(TIF_RESTORE_SIGMASK);
// /*AFLA*/ }
// /*AFLA*/ static inline bool test_and_clear_restore_sigmask(void)
// /*AFLA*/ {
// /*AFLA*/ 	return test_and_clear_thread_flag(TIF_RESTORE_SIGMASK);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else	/* TIF_RESTORE_SIGMASK */
// /*AFLA*/ 
// /*AFLA*/ /* Higher-quality implementation, used if TIF_RESTORE_SIGMASK doesn't exist. */
// /*AFLA*/ static inline void set_restore_sigmask(void)
// /*AFLA*/ {
// /*AFLA*/ 	current->restore_sigmask = true;
// /*AFLA*/ 	WARN_ON(!test_thread_flag(TIF_SIGPENDING));
// /*AFLA*/ }
// /*AFLA*/ static inline void clear_restore_sigmask(void)
// /*AFLA*/ {
// /*AFLA*/ 	current->restore_sigmask = false;
// /*AFLA*/ }
// /*AFLA*/ static inline bool test_restore_sigmask(void)
// /*AFLA*/ {
// /*AFLA*/ 	return current->restore_sigmask;
// /*AFLA*/ }
// /*AFLA*/ static inline bool test_and_clear_restore_sigmask(void)
// /*AFLA*/ {
// /*AFLA*/ 	if (!current->restore_sigmask)
// /*AFLA*/ 		return false;
// /*AFLA*/ 	current->restore_sigmask = false;
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline void restore_saved_sigmask(void)
// /*AFLA*/ {
// /*AFLA*/ 	if (test_and_clear_restore_sigmask())
// /*AFLA*/ 		__set_current_blocked(&current->saved_sigmask);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline sigset_t *sigmask_to_save(void)
// /*AFLA*/ {
// /*AFLA*/ 	sigset_t *res = &current->blocked;
// /*AFLA*/ 	if (unlikely(test_restore_sigmask()))
// /*AFLA*/ 		res = &current->saved_sigmask;
// /*AFLA*/ 	return res;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int kill_cad_pid(int sig, int priv)
// /*AFLA*/ {
// /*AFLA*/ 	return kill_pid(cad_pid, sig, priv);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* These can be the second arg to send_sig_info/send_group_sig_info.  */
// /*AFLA*/ #define SEND_SIG_NOINFO ((struct siginfo *) 0)
// /*AFLA*/ #define SEND_SIG_PRIV	((struct siginfo *) 1)
// /*AFLA*/ #define SEND_SIG_FORCED	((struct siginfo *) 2)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * True if we are on the alternate signal stack.
// /*AFLA*/  */
// /*AFLA*/ static inline int on_sig_stack(unsigned long sp)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If the signal stack is SS_AUTODISARM then, by construction, we
// /*AFLA*/ 	 * can't be on the signal stack unless user code deliberately set
// /*AFLA*/ 	 * SS_AUTODISARM when we were already on it.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * This improves reliability: if user state gets corrupted such that
// /*AFLA*/ 	 * the stack pointer points very close to the end of the signal stack,
// /*AFLA*/ 	 * then this check will enable the signal to be handled anyway.
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (current->sas_ss_flags & SS_AUTODISARM)
// /*AFLA*/ 		return 0;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_STACK_GROWSUP
// /*AFLA*/ 	return sp >= current->sas_ss_sp &&
// /*AFLA*/ 		sp - current->sas_ss_sp < current->sas_ss_size;
// /*AFLA*/ #else
// /*AFLA*/ 	return sp > current->sas_ss_sp &&
// /*AFLA*/ 		sp - current->sas_ss_sp <= current->sas_ss_size;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sas_ss_flags(unsigned long sp)
// /*AFLA*/ {
// /*AFLA*/ 	if (!current->sas_ss_size)
// /*AFLA*/ 		return SS_DISABLE;
// /*AFLA*/ 
// /*AFLA*/ 	return on_sig_stack(sp) ? SS_ONSTACK : 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sas_ss_reset(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	p->sas_ss_sp = 0;
// /*AFLA*/ 	p->sas_ss_size = 0;
// /*AFLA*/ 	p->sas_ss_flags = SS_DISABLE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long sigsp(unsigned long sp, struct ksignal *ksig)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely((ksig->ka.sa.sa_flags & SA_ONSTACK)) && ! sas_ss_flags(sp))
// /*AFLA*/ #ifdef CONFIG_STACK_GROWSUP
// /*AFLA*/ 		return current->sas_ss_sp;
// /*AFLA*/ #else
// /*AFLA*/ 		return current->sas_ss_sp + current->sas_ss_size;
// /*AFLA*/ #endif
// /*AFLA*/ 	return sp;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Routines for handling mm_structs
// /*AFLA*/  */
// /*AFLA*/ extern struct mm_struct * mm_alloc(void);
// /*AFLA*/ 
// /*AFLA*/ /* mmdrop drops the mm and the page tables */
// /*AFLA*/ extern void __mmdrop(struct mm_struct *);
// /*AFLA*/ static inline void mmdrop(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(atomic_dec_and_test(&mm->mm_count)))
// /*AFLA*/ 		__mmdrop(mm);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void mmdrop_async_fn(struct work_struct *work)
// /*AFLA*/ {
// /*AFLA*/ 	struct mm_struct *mm = container_of(work, struct mm_struct, async_put_work);
// /*AFLA*/ 	__mmdrop(mm);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void mmdrop_async(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(atomic_dec_and_test(&mm->mm_count))) {
// /*AFLA*/ 		INIT_WORK(&mm->async_put_work, mmdrop_async_fn);
// /*AFLA*/ 		schedule_work(&mm->async_put_work);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool mmget_not_zero(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_inc_not_zero(&mm->mm_users);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* mmput gets rid of the mappings and all user-space */
// /*AFLA*/ extern void mmput(struct mm_struct *);
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ /* same as above but performs the slow path from the async context. Can
// /*AFLA*/  * be called from the atomic context as well
// /*AFLA*/  */
// /*AFLA*/ extern void mmput_async(struct mm_struct *);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Grab a reference to a task's mm, if it is not already going away */
// /*AFLA*/ extern struct mm_struct *get_task_mm(struct task_struct *task);
// /*AFLA*/ /*
// /*AFLA*/  * Grab a reference to a task's mm, if it is not already going away
// /*AFLA*/  * and ptrace_may_access with the mode parameter passed to it
// /*AFLA*/  * succeeds.
// /*AFLA*/  */
// /*AFLA*/ extern struct mm_struct *mm_access(struct task_struct *task, unsigned int mode);
// /*AFLA*/ /* Remove the current tasks stale references to the old mm_struct */
// /*AFLA*/ extern void mm_release(struct task_struct *, struct mm_struct *);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_COPY_THREAD_TLS
// /*AFLA*/ extern int copy_thread_tls(unsigned long, unsigned long, unsigned long,
// /*AFLA*/ 			struct task_struct *, unsigned long);
// /*AFLA*/ #else
// /*AFLA*/ extern int copy_thread(unsigned long, unsigned long, unsigned long,
// /*AFLA*/ 			struct task_struct *);
// /*AFLA*/ 
// /*AFLA*/ /* Architectures that haven't opted into copy_thread_tls get the tls argument
// /*AFLA*/  * via pt_regs, so ignore the tls argument passed via C. */
// /*AFLA*/ static inline int copy_thread_tls(
// /*AFLA*/ 		unsigned long clone_flags, unsigned long sp, unsigned long arg,
// /*AFLA*/ 		struct task_struct *p, unsigned long tls)
// /*AFLA*/ {
// /*AFLA*/ 	return copy_thread(clone_flags, sp, arg, p);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ extern void flush_thread(void);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_EXIT_THREAD
// /*AFLA*/ extern void exit_thread(struct task_struct *tsk);
// /*AFLA*/ #else
// /*AFLA*/ static inline void exit_thread(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void exit_files(struct task_struct *);
// /*AFLA*/ extern void __cleanup_sighand(struct sighand_struct *);
// /*AFLA*/ 
// /*AFLA*/ extern void exit_itimers(struct signal_struct *);
// /*AFLA*/ extern void flush_itimer_signals(void);
// /*AFLA*/ 
// /*AFLA*/ extern void do_group_exit(int);
// /*AFLA*/ 
// /*AFLA*/ extern int do_execve(struct filename *,
// /*AFLA*/ 		     const char __user * const __user *,
// /*AFLA*/ 		     const char __user * const __user *);
// /*AFLA*/ extern int do_execveat(int, struct filename *,
// /*AFLA*/ 		       const char __user * const __user *,
// /*AFLA*/ 		       const char __user * const __user *,
// /*AFLA*/ 		       int);
// /*AFLA*/ extern long _do_fork(unsigned long, unsigned long, unsigned long, int __user *, int __user *, unsigned long);
// /*AFLA*/ extern long do_fork(unsigned long, unsigned long, unsigned long, int __user *, int __user *);
// /*AFLA*/ struct task_struct *fork_idle(int);
// /*AFLA*/ extern pid_t kernel_thread(int (*fn)(void *), void *arg, unsigned long flags);
// /*AFLA*/ 
// /*AFLA*/ extern void __set_task_comm(struct task_struct *tsk, const char *from, bool exec);
// /*AFLA*/ static inline void set_task_comm(struct task_struct *tsk, const char *from)
// /*AFLA*/ {
// /*AFLA*/ 	__set_task_comm(tsk, from, false);
// /*AFLA*/ }
// /*AFLA*/ extern char *get_task_comm(char *to, struct task_struct *tsk);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ void scheduler_ipi(void);
// /*AFLA*/ extern unsigned long wait_task_inactive(struct task_struct *, long match_state);
// /*AFLA*/ #else
// /*AFLA*/ static inline void scheduler_ipi(void) { }
// /*AFLA*/ static inline unsigned long wait_task_inactive(struct task_struct *p,
// /*AFLA*/ 					       long match_state)
// /*AFLA*/ {
// /*AFLA*/ 	return 1;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define tasklist_empty() \
// /*AFLA*/ 	list_empty(&init_task.tasks)
// /*AFLA*/ 
// /*AFLA*/ #define next_task(p) \
// /*AFLA*/ 	list_entry_rcu((p)->tasks.next, struct task_struct, tasks)
// /*AFLA*/ 
// /*AFLA*/ #define for_each_process(p) \
// /*AFLA*/ 	for (p = &init_task ; (p = next_task(p)) != &init_task ; )
// /*AFLA*/ 
// /*AFLA*/ extern bool current_is_single_threaded(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Careful: do_each_thread/while_each_thread is a double loop so
// /*AFLA*/  *          'break' will not work as expected - use goto instead.
// /*AFLA*/  */
// /*AFLA*/ #define do_each_thread(g, t) \
// /*AFLA*/ 	for (g = t = &init_task ; (g = t = next_task(g)) != &init_task ; ) do
// /*AFLA*/ 
// /*AFLA*/ #define while_each_thread(g, t) \
// /*AFLA*/ 	while ((t = next_thread(t)) != g)
// /*AFLA*/ 
// /*AFLA*/ #define __for_each_thread(signal, t)	\
// /*AFLA*/ 	list_for_each_entry_rcu(t, &(signal)->thread_head, thread_node)
// /*AFLA*/ 
// /*AFLA*/ #define for_each_thread(p, t)		\
// /*AFLA*/ 	__for_each_thread((p)->signal, t)
// /*AFLA*/ 
// /*AFLA*/ /* Careful: this is a double loop, 'break' won't work as expected. */
// /*AFLA*/ #define for_each_process_thread(p, t)	\
// /*AFLA*/ 	for_each_process(p) for_each_thread(p, t)
// /*AFLA*/ 
// /*AFLA*/ static inline int get_nr_threads(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return tsk->signal->nr_threads;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool thread_group_leader(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return p->exit_signal >= 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Do to the insanities of de_thread it is possible for a process
// /*AFLA*/  * to have the pid of the thread group leader without actually being
// /*AFLA*/  * the thread group leader.  For iteration through the pids in proc
// /*AFLA*/  * all we care about is that we have a task with the appropriate
// /*AFLA*/  * pid, we don't actually care if we have the right task.
// /*AFLA*/  */
// /*AFLA*/ static inline bool has_group_leader_pid(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return task_pid(p) == p->signal->leader_pid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline
// /*AFLA*/ bool same_thread_group(struct task_struct *p1, struct task_struct *p2)
// /*AFLA*/ {
// /*AFLA*/ 	return p1->signal == p2->signal;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct task_struct *next_thread(const struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return list_entry_rcu(p->thread_group.next,
// /*AFLA*/ 			      struct task_struct, thread_group);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int thread_group_empty(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return list_empty(&p->thread_group);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define delay_group_leader(p) \
// /*AFLA*/ 		(thread_group_leader(p) && !thread_group_empty(p))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Protects ->fs, ->files, ->mm, ->group_info, ->comm, keyring
// /*AFLA*/  * subscriptions and synchronises with wait4().  Also used in procfs.  Also
// /*AFLA*/  * pins the final release of task.io_context.  Also protects ->cpuset and
// /*AFLA*/  * ->cgroup.subsys[]. And ->vfork_done.
// /*AFLA*/  *
// /*AFLA*/  * Nests both inside and outside of read_lock(&tasklist_lock).
// /*AFLA*/  * It must not be nested with write_lock_irq(&tasklist_lock),
// /*AFLA*/  * neither inside nor outside.
// /*AFLA*/  */
// /*AFLA*/ static inline void task_lock(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	spin_lock(&p->alloc_lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void task_unlock(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	spin_unlock(&p->alloc_lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct sighand_struct *__lock_task_sighand(struct task_struct *tsk,
// /*AFLA*/ 							unsigned long *flags);
// /*AFLA*/ 
// /*AFLA*/ static inline struct sighand_struct *lock_task_sighand(struct task_struct *tsk,
// /*AFLA*/ 						       unsigned long *flags)
// /*AFLA*/ {
// /*AFLA*/ 	struct sighand_struct *ret;
// /*AFLA*/ 
// /*AFLA*/ 	ret = __lock_task_sighand(tsk, flags);
// /*AFLA*/ 	(void)__cond_lock(&tsk->sighand->siglock, ret);
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void unlock_task_sighand(struct task_struct *tsk,
// /*AFLA*/ 						unsigned long *flags)
// /*AFLA*/ {
// /*AFLA*/ 	spin_unlock_irqrestore(&tsk->sighand->siglock, *flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * threadgroup_change_begin - mark the beginning of changes to a threadgroup
// /*AFLA*/  * @tsk: task causing the changes
// /*AFLA*/  *
// /*AFLA*/  * All operations which modify a threadgroup - a new thread joining the
// /*AFLA*/  * group, death of a member thread (the assertion of PF_EXITING) and
// /*AFLA*/  * exec(2) dethreading the process and replacing the leader - are wrapped
// /*AFLA*/  * by threadgroup_change_{begin|end}().  This is to provide a place which
// /*AFLA*/  * subsystems needing threadgroup stability can hook into for
// /*AFLA*/  * synchronization.
// /*AFLA*/  */
// /*AFLA*/ static inline void threadgroup_change_begin(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	might_sleep();
// /*AFLA*/ 	cgroup_threadgroup_change_begin(tsk);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * threadgroup_change_end - mark the end of changes to a threadgroup
// /*AFLA*/  * @tsk: task causing the changes
// /*AFLA*/  *
// /*AFLA*/  * See threadgroup_change_begin().
// /*AFLA*/  */
// /*AFLA*/ static inline void threadgroup_change_end(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	cgroup_threadgroup_change_end(tsk);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_THREAD_INFO_IN_TASK
// /*AFLA*/ 
// /*AFLA*/ static inline struct thread_info *task_thread_info(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	return &task->thread_info;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * When accessing the stack of a non-current task that might exit, use
// /*AFLA*/  * try_get_task_stack() instead.  task_stack_page will return a pointer
// /*AFLA*/  * that could get freed out from under you.
// /*AFLA*/  */
// /*AFLA*/ static inline void *task_stack_page(const struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	return task->stack;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define setup_thread_stack(new,old)	do { } while(0)
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long *end_of_stack(const struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	return task->stack;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #elif !defined(__HAVE_THREAD_FUNCTIONS)
// /*AFLA*/ 
// /*AFLA*/ #define task_thread_info(task)	((struct thread_info *)(task)->stack)
// /*AFLA*/ #define task_stack_page(task)	((void *)(task)->stack)
// /*AFLA*/ 
// /*AFLA*/ static inline void setup_thread_stack(struct task_struct *p, struct task_struct *org)
// /*AFLA*/ {
// /*AFLA*/ 	*task_thread_info(p) = *task_thread_info(org);
// /*AFLA*/ 	task_thread_info(p)->task = p;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Return the address of the last usable long on the stack.
// /*AFLA*/  *
// /*AFLA*/  * When the stack grows down, this is just above the thread
// /*AFLA*/  * info struct. Going any lower will corrupt the threadinfo.
// /*AFLA*/  *
// /*AFLA*/  * When the stack grows up, this is the highest address.
// /*AFLA*/  * Beyond that position, we corrupt data on the next page.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long *end_of_stack(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_STACK_GROWSUP
// /*AFLA*/ 	return (unsigned long *)((unsigned long)task_thread_info(p) + THREAD_SIZE) - 1;
// /*AFLA*/ #else
// /*AFLA*/ 	return (unsigned long *)(task_thread_info(p) + 1);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_THREAD_INFO_IN_TASK
// /*AFLA*/ static inline void *try_get_task_stack(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_inc_not_zero(&tsk->stack_refcount) ?
// /*AFLA*/ 		task_stack_page(tsk) : NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void put_task_stack(struct task_struct *tsk);
// /*AFLA*/ #else
// /*AFLA*/ static inline void *try_get_task_stack(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return task_stack_page(tsk);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void put_task_stack(struct task_struct *tsk) {}
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define task_stack_end_corrupted(task) \
// /*AFLA*/ 		(*(end_of_stack(task)) != STACK_END_MAGIC)
// /*AFLA*/ 
// /*AFLA*/ static inline int object_is_on_stack(void *obj)
// /*AFLA*/ {
// /*AFLA*/ 	void *stack = task_stack_page(current);
// /*AFLA*/ 
// /*AFLA*/ 	return (obj >= stack) && (obj < (stack + THREAD_SIZE));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void thread_stack_cache_init(void);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_STACK_USAGE
// /*AFLA*/ static inline unsigned long stack_not_used(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long *n = end_of_stack(p);
// /*AFLA*/ 
// /*AFLA*/ 	do { 	/* Skip over canary */
// /*AFLA*/ # ifdef CONFIG_STACK_GROWSUP
// /*AFLA*/ 		n--;
// /*AFLA*/ # else
// /*AFLA*/ 		n++;
// /*AFLA*/ # endif
// /*AFLA*/ 	} while (!*n);
// /*AFLA*/ 
// /*AFLA*/ # ifdef CONFIG_STACK_GROWSUP
// /*AFLA*/ 	return (unsigned long)end_of_stack(p) - (unsigned long)n;
// /*AFLA*/ # else
// /*AFLA*/ 	return (unsigned long)n - (unsigned long)end_of_stack(p);
// /*AFLA*/ # endif
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ extern void set_task_stack_end_magic(struct task_struct *tsk);
// /*AFLA*/ 
// /*AFLA*/ /* set thread flags in other task's structures
// /*AFLA*/  * - see asm/thread_info.h for TIF_xxxx flags available
// /*AFLA*/  */
// /*AFLA*/ static inline void set_tsk_thread_flag(struct task_struct *tsk, int flag)
// /*AFLA*/ {
// /*AFLA*/ 	set_ti_thread_flag(task_thread_info(tsk), flag);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void clear_tsk_thread_flag(struct task_struct *tsk, int flag)
// /*AFLA*/ {
// /*AFLA*/ 	clear_ti_thread_flag(task_thread_info(tsk), flag);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int test_and_set_tsk_thread_flag(struct task_struct *tsk, int flag)
// /*AFLA*/ {
// /*AFLA*/ 	return test_and_set_ti_thread_flag(task_thread_info(tsk), flag);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int test_and_clear_tsk_thread_flag(struct task_struct *tsk, int flag)
// /*AFLA*/ {
// /*AFLA*/ 	return test_and_clear_ti_thread_flag(task_thread_info(tsk), flag);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int test_tsk_thread_flag(struct task_struct *tsk, int flag)
// /*AFLA*/ {
// /*AFLA*/ 	return test_ti_thread_flag(task_thread_info(tsk), flag);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void set_tsk_need_resched(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	set_tsk_thread_flag(tsk,TIF_NEED_RESCHED);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void clear_tsk_need_resched(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	clear_tsk_thread_flag(tsk,TIF_NEED_RESCHED);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int test_tsk_need_resched(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return unlikely(test_tsk_thread_flag(tsk,TIF_NEED_RESCHED));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int restart_syscall(void)
// /*AFLA*/ {
// /*AFLA*/ 	set_tsk_thread_flag(current, TIF_SIGPENDING);
// /*AFLA*/ 	return -ERESTARTNOINTR;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int signal_pending(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return unlikely(test_tsk_thread_flag(p,TIF_SIGPENDING));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __fatal_signal_pending(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return unlikely(sigismember(&p->pending.signal, SIGKILL));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int fatal_signal_pending(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return signal_pending(p) && __fatal_signal_pending(p);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int signal_pending_state(long state, struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	if (!(state & (TASK_INTERRUPTIBLE | TASK_WAKEKILL)))
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	if (!signal_pending(p))
// /*AFLA*/ 		return 0;
// /*AFLA*/ 
// /*AFLA*/ 	return (state & TASK_INTERRUPTIBLE) || __fatal_signal_pending(p);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * cond_resched() and cond_resched_lock(): latency reduction via
// /*AFLA*/  * explicit rescheduling in places that are safe. The return
// /*AFLA*/  * value indicates whether a reschedule was done in fact.
// /*AFLA*/  * cond_resched_lock() will drop the spinlock before scheduling,
// /*AFLA*/  * cond_resched_softirq() will enable bhs before scheduling.
// /*AFLA*/  */
// /*AFLA*/ #ifndef CONFIG_PREEMPT
// /*AFLA*/ extern int _cond_resched(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline int _cond_resched(void) { return 0; }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define cond_resched() ({			\
// /*AFLA*/ 	___might_sleep(__FILE__, __LINE__, 0);	\
// /*AFLA*/ 	_cond_resched();			\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ extern int __cond_resched_lock(spinlock_t *lock);
// /*AFLA*/ 
// /*AFLA*/ #define cond_resched_lock(lock) ({				\
// /*AFLA*/ 	___might_sleep(__FILE__, __LINE__, PREEMPT_LOCK_OFFSET);\
// /*AFLA*/ 	__cond_resched_lock(lock);				\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ extern int __cond_resched_softirq(void);
// /*AFLA*/ 
// /*AFLA*/ #define cond_resched_softirq() ({					\
// /*AFLA*/ 	___might_sleep(__FILE__, __LINE__, SOFTIRQ_DISABLE_OFFSET);	\
// /*AFLA*/ 	__cond_resched_softirq();					\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ static inline void cond_resched_rcu(void)
// /*AFLA*/ {
// /*AFLA*/ #if defined(CONFIG_DEBUG_ATOMIC_SLEEP) || !defined(CONFIG_PREEMPT_RCU)
// /*AFLA*/ 	rcu_read_unlock();
// /*AFLA*/ 	cond_resched();
// /*AFLA*/ 	rcu_read_lock();
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long get_preempt_disable_ip(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_DEBUG_PREEMPT
// /*AFLA*/ 	return p->preempt_disable_ip;
// /*AFLA*/ #else
// /*AFLA*/ 	return 0;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Does a critical section need to be broken due to another
// /*AFLA*/  * task waiting?: (technically does not depend on CONFIG_PREEMPT,
// /*AFLA*/  * but a general need for low latency)
// /*AFLA*/  */
// /*AFLA*/ static inline int spin_needbreak(spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_PREEMPT
// /*AFLA*/ 	return spin_is_contended(lock);
// /*AFLA*/ #else
// /*AFLA*/ 	return 0;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Idle thread specific functions to determine the need_resched
// /*AFLA*/  * polling state.
// /*AFLA*/  */
// /*AFLA*/ #ifdef TIF_POLLING_NRFLAG
// /*AFLA*/ static inline int tsk_is_polling(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return test_tsk_thread_flag(p, TIF_POLLING_NRFLAG);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __current_set_polling(void)
// /*AFLA*/ {
// /*AFLA*/ 	set_thread_flag(TIF_POLLING_NRFLAG);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool __must_check current_set_polling_and_test(void)
// /*AFLA*/ {
// /*AFLA*/ 	__current_set_polling();
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Polling state must be visible before we test NEED_RESCHED,
// /*AFLA*/ 	 * paired by resched_curr()
// /*AFLA*/ 	 */
// /*AFLA*/ 	smp_mb__after_atomic();
// /*AFLA*/ 
// /*AFLA*/ 	return unlikely(tif_need_resched());
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __current_clr_polling(void)
// /*AFLA*/ {
// /*AFLA*/ 	clear_thread_flag(TIF_POLLING_NRFLAG);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool __must_check current_clr_polling_and_test(void)
// /*AFLA*/ {
// /*AFLA*/ 	__current_clr_polling();
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Polling state must be visible before we test NEED_RESCHED,
// /*AFLA*/ 	 * paired by resched_curr()
// /*AFLA*/ 	 */
// /*AFLA*/ 	smp_mb__after_atomic();
// /*AFLA*/ 
// /*AFLA*/ 	return unlikely(tif_need_resched());
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ static inline int tsk_is_polling(struct task_struct *p) { return 0; }
// /*AFLA*/ static inline void __current_set_polling(void) { }
// /*AFLA*/ static inline void __current_clr_polling(void) { }
// /*AFLA*/ 
// /*AFLA*/ static inline bool __must_check current_set_polling_and_test(void)
// /*AFLA*/ {
// /*AFLA*/ 	return unlikely(tif_need_resched());
// /*AFLA*/ }
// /*AFLA*/ static inline bool __must_check current_clr_polling_and_test(void)
// /*AFLA*/ {
// /*AFLA*/ 	return unlikely(tif_need_resched());
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline void current_clr_polling(void)
// /*AFLA*/ {
// /*AFLA*/ 	__current_clr_polling();
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Ensure we check TIF_NEED_RESCHED after we clear the polling bit.
// /*AFLA*/ 	 * Once the bit is cleared, we'll get IPIs with every new
// /*AFLA*/ 	 * TIF_NEED_RESCHED and the IPI handler, scheduler_ipi(), will also
// /*AFLA*/ 	 * fold.
// /*AFLA*/ 	 */
// /*AFLA*/ 	smp_mb(); /* paired with resched_curr() */
// /*AFLA*/ 
// /*AFLA*/ 	preempt_fold_need_resched();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline bool need_resched(void)
// /*AFLA*/ {
// /*AFLA*/ 	return unlikely(tif_need_resched());
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Thread group CPU time accounting.
// /*AFLA*/  */
// /*AFLA*/ void thread_group_cputime(struct task_struct *tsk, struct task_cputime *times);
// /*AFLA*/ void thread_group_cputimer(struct task_struct *tsk, struct task_cputime *times);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Reevaluate whether the task has signals pending delivery.
// /*AFLA*/  * Wake the task if so.
// /*AFLA*/  * This is required every time the blocked sigset_t changes.
// /*AFLA*/  * callers must hold sighand->siglock.
// /*AFLA*/  */
// /*AFLA*/ extern void recalc_sigpending_and_wake(struct task_struct *t);
// /*AFLA*/ extern void recalc_sigpending(void);
// /*AFLA*/ 
// /*AFLA*/ extern void signal_wake_up_state(struct task_struct *t, unsigned int state);
// /*AFLA*/ 
// /*AFLA*/ static inline void signal_wake_up(struct task_struct *t, bool resume)
// /*AFLA*/ {
// /*AFLA*/ 	signal_wake_up_state(t, resume ? TASK_WAKEKILL : 0);
// /*AFLA*/ }
// /*AFLA*/ static inline void ptrace_signal_wake_up(struct task_struct *t, bool resume)
// /*AFLA*/ {
// /*AFLA*/ 	signal_wake_up_state(t, resume ? __TASK_TRACED : 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Wrappers for p->thread_info->cpu access. No-op on UP.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int task_cpu(const struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_THREAD_INFO_IN_TASK
// /*AFLA*/ 	return p->cpu;
// /*AFLA*/ #else
// /*AFLA*/ 	return task_thread_info(p)->cpu;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int task_node(const struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return cpu_to_node(task_cpu(p));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void set_task_cpu(struct task_struct *p, unsigned int cpu);
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int task_cpu(const struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void set_task_cpu(struct task_struct *p, unsigned int cpu)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ extern long sched_setaffinity(pid_t pid, const struct cpumask *new_mask);
// /*AFLA*/ extern long sched_getaffinity(pid_t pid, struct cpumask *mask);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_CGROUP_SCHED
// /*AFLA*/ extern struct task_group root_task_group;
// /*AFLA*/ #endif /* CONFIG_CGROUP_SCHED */
// /*AFLA*/ 
// /*AFLA*/ extern int task_can_switch_user(struct user_struct *up,
// /*AFLA*/ 					struct task_struct *tsk);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TASK_XACCT
// /*AFLA*/ static inline void add_rchar(struct task_struct *tsk, ssize_t amt)
// /*AFLA*/ {
// /*AFLA*/ 	tsk->ioac.rchar += amt;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void add_wchar(struct task_struct *tsk, ssize_t amt)
// /*AFLA*/ {
// /*AFLA*/ 	tsk->ioac.wchar += amt;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void inc_syscr(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	tsk->ioac.syscr++;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void inc_syscw(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	tsk->ioac.syscw++;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline void add_rchar(struct task_struct *tsk, ssize_t amt)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void add_wchar(struct task_struct *tsk, ssize_t amt)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void inc_syscr(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void inc_syscw(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef TASK_SIZE_OF
// /*AFLA*/ #define TASK_SIZE_OF(tsk)	TASK_SIZE
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MEMCG
// /*AFLA*/ extern void mm_update_next_owner(struct mm_struct *mm);
// /*AFLA*/ #else
// /*AFLA*/ static inline void mm_update_next_owner(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_MEMCG */
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long task_rlimit(const struct task_struct *tsk,
// /*AFLA*/ 		unsigned int limit)
// /*AFLA*/ {
// /*AFLA*/ 	return READ_ONCE(tsk->signal->rlim[limit].rlim_cur);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long task_rlimit_max(const struct task_struct *tsk,
// /*AFLA*/ 		unsigned int limit)
// /*AFLA*/ {
// /*AFLA*/ 	return READ_ONCE(tsk->signal->rlim[limit].rlim_max);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long rlimit(unsigned int limit)
// /*AFLA*/ {
// /*AFLA*/ 	return task_rlimit(current, limit);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long rlimit_max(unsigned int limit)
// /*AFLA*/ {
// /*AFLA*/ 	return task_rlimit_max(current, limit);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define SCHED_CPUFREQ_RT	(1U << 0)
// /*AFLA*/ #define SCHED_CPUFREQ_DL	(1U << 1)
// /*AFLA*/ #define SCHED_CPUFREQ_IOWAIT	(1U << 2)
// /*AFLA*/ 
// /*AFLA*/ #define SCHED_CPUFREQ_RT_DL	(SCHED_CPUFREQ_RT | SCHED_CPUFREQ_DL)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_CPU_FREQ
// /*AFLA*/ struct update_util_data {
// /*AFLA*/        void (*func)(struct update_util_data *data, u64 time, unsigned int flags);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ void cpufreq_add_update_util_hook(int cpu, struct update_util_data *data,
// /*AFLA*/                        void (*func)(struct update_util_data *data, u64 time,
// /*AFLA*/ 				    unsigned int flags));
// /*AFLA*/ void cpufreq_remove_update_util_hook(int cpu);
// /*AFLA*/ #endif /* CONFIG_CPU_FREQ */
// /*AFLA*/ 
// /*AFLA*/ #endif
