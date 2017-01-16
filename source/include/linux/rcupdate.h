// /*AFLA*/ /*
// /*AFLA*/  * Read-Copy Update mechanism for mutual exclusion
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify
// /*AFLA*/  * it under the terms of the GNU General Public License as published by
// /*AFLA*/  * the Free Software Foundation; either version 2 of the License, or
// /*AFLA*/  * (at your option) any later version.
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it will be useful,
// /*AFLA*/  * but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/  * GNU General Public License for more details.
// /*AFLA*/  *
// /*AFLA*/  * You should have received a copy of the GNU General Public License
// /*AFLA*/  * along with this program; if not, you can access it online at
// /*AFLA*/  * http://www.gnu.org/licenses/gpl-2.0.html.
// /*AFLA*/  *
// /*AFLA*/  * Copyright IBM Corporation, 2001
// /*AFLA*/  *
// /*AFLA*/  * Author: Dipankar Sarma <dipankar@in.ibm.com>
// /*AFLA*/  *
// /*AFLA*/  * Based on the original work by Paul McKenney <paulmck@us.ibm.com>
// /*AFLA*/  * and inputs from Rusty Russell, Andrea Arcangeli and Andi Kleen.
// /*AFLA*/  * Papers:
// /*AFLA*/  * http://www.rdrop.com/users/paulmck/paper/rclockpdcsproof.pdf
// /*AFLA*/  * http://lse.sourceforge.net/locking/rclock_OLS.2001.05.01c.sc.pdf (OLS2001)
// /*AFLA*/  *
// /*AFLA*/  * For detailed explanation of Read-Copy Update mechanism see -
// /*AFLA*/  *		http://lse.sourceforge.net/locking/rcupdate.html
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __LINUX_RCUPDATE_H
// /*AFLA*/ #define __LINUX_RCUPDATE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/cache.h>
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/threads.h>
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ #include <linux/seqlock.h>
// /*AFLA*/ #include <linux/lockdep.h>
// /*AFLA*/ #include <linux/completion.h>
// /*AFLA*/ #include <linux/debugobjects.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/ktime.h>
// /*AFLA*/ #include <linux/irqflags.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/barrier.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_TINY_RCU
// /*AFLA*/ extern int rcu_expedited; /* for sysctl */
// /*AFLA*/ extern int rcu_normal;    /* also for sysctl */
// /*AFLA*/ #endif /* #ifndef CONFIG_TINY_RCU */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TINY_RCU
// /*AFLA*/ /* Tiny RCU doesn't expedite, as its purpose in life is instead to be tiny. */
// /*AFLA*/ static inline bool rcu_gp_is_normal(void)  /* Internal RCU use. */
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ static inline bool rcu_gp_is_expedited(void)  /* Internal RCU use. */
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void rcu_expedite_gp(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void rcu_unexpedite_gp(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #else /* #ifdef CONFIG_TINY_RCU */
// /*AFLA*/ bool rcu_gp_is_normal(void);     /* Internal RCU use. */
// /*AFLA*/ bool rcu_gp_is_expedited(void);  /* Internal RCU use. */
// /*AFLA*/ void rcu_expedite_gp(void);
// /*AFLA*/ void rcu_unexpedite_gp(void);
// /*AFLA*/ #endif /* #else #ifdef CONFIG_TINY_RCU */
// /*AFLA*/ 
// /*AFLA*/ enum rcutorture_type {
// /*AFLA*/ 	RCU_FLAVOR,
// /*AFLA*/ 	RCU_BH_FLAVOR,
// /*AFLA*/ 	RCU_SCHED_FLAVOR,
// /*AFLA*/ 	RCU_TASKS_FLAVOR,
// /*AFLA*/ 	SRCU_FLAVOR,
// /*AFLA*/ 	INVALID_RCU_FLAVOR
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_TREE_RCU) || defined(CONFIG_PREEMPT_RCU)
// /*AFLA*/ void rcutorture_get_gp_data(enum rcutorture_type test_type, int *flags,
// /*AFLA*/ 			    unsigned long *gpnum, unsigned long *completed);
// /*AFLA*/ void rcutorture_record_test_transition(void);
// /*AFLA*/ void rcutorture_record_progress(unsigned long vernum);
// /*AFLA*/ void do_trace_rcu_torture_read(const char *rcutorturename,
// /*AFLA*/ 			       struct rcu_head *rhp,
// /*AFLA*/ 			       unsigned long secs,
// /*AFLA*/ 			       unsigned long c_old,
// /*AFLA*/ 			       unsigned long c);
// /*AFLA*/ #else
// /*AFLA*/ static inline void rcutorture_get_gp_data(enum rcutorture_type test_type,
// /*AFLA*/ 					  int *flags,
// /*AFLA*/ 					  unsigned long *gpnum,
// /*AFLA*/ 					  unsigned long *completed)
// /*AFLA*/ {
// /*AFLA*/ 	*flags = 0;
// /*AFLA*/ 	*gpnum = 0;
// /*AFLA*/ 	*completed = 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void rcutorture_record_test_transition(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void rcutorture_record_progress(unsigned long vernum)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #ifdef CONFIG_RCU_TRACE
// /*AFLA*/ void do_trace_rcu_torture_read(const char *rcutorturename,
// /*AFLA*/ 			       struct rcu_head *rhp,
// /*AFLA*/ 			       unsigned long secs,
// /*AFLA*/ 			       unsigned long c_old,
// /*AFLA*/ 			       unsigned long c);
// /*AFLA*/ #else
// /*AFLA*/ #define do_trace_rcu_torture_read(rcutorturename, rhp, secs, c_old, c) \
// /*AFLA*/ 	do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define UINT_CMP_GE(a, b)	(UINT_MAX / 2 >= (a) - (b))
// /*AFLA*/ #define UINT_CMP_LT(a, b)	(UINT_MAX / 2 < (a) - (b))
// /*AFLA*/ #define ULONG_CMP_GE(a, b)	(ULONG_MAX / 2 >= (a) - (b))
// /*AFLA*/ #define ULONG_CMP_LT(a, b)	(ULONG_MAX / 2 < (a) - (b))
// /*AFLA*/ #define ulong2long(a)		(*(long *)(&(a)))
// /*AFLA*/ 
// /*AFLA*/ /* Exported common interfaces */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PREEMPT_RCU
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * call_rcu() - Queue an RCU callback for invocation after a grace period.
// /*AFLA*/  * @head: structure to be used for queueing the RCU updates.
// /*AFLA*/  * @func: actual callback function to be invoked after the grace period
// /*AFLA*/  *
// /*AFLA*/  * The callback function will be invoked some time after a full grace
// /*AFLA*/  * period elapses, in other words after all pre-existing RCU read-side
// /*AFLA*/  * critical sections have completed.  However, the callback function
// /*AFLA*/  * might well execute concurrently with RCU read-side critical sections
// /*AFLA*/  * that started after call_rcu() was invoked.  RCU read-side critical
// /*AFLA*/  * sections are delimited by rcu_read_lock() and rcu_read_unlock(),
// /*AFLA*/  * and may be nested.
// /*AFLA*/  *
// /*AFLA*/  * Note that all CPUs must agree that the grace period extended beyond
// /*AFLA*/  * all pre-existing RCU read-side critical section.  On systems with more
// /*AFLA*/  * than one CPU, this means that when "func()" is invoked, each CPU is
// /*AFLA*/  * guaranteed to have executed a full memory barrier since the end of its
// /*AFLA*/  * last RCU read-side critical section whose beginning preceded the call
// /*AFLA*/  * to call_rcu().  It also means that each CPU executing an RCU read-side
// /*AFLA*/  * critical section that continues beyond the start of "func()" must have
// /*AFLA*/  * executed a memory barrier after the call_rcu() but before the beginning
// /*AFLA*/  * of that RCU read-side critical section.  Note that these guarantees
// /*AFLA*/  * include CPUs that are offline, idle, or executing in user mode, as
// /*AFLA*/  * well as CPUs that are executing in the kernel.
// /*AFLA*/  *
// /*AFLA*/  * Furthermore, if CPU A invoked call_rcu() and CPU B invoked the
// /*AFLA*/  * resulting RCU callback function "func()", then both CPU A and CPU B are
// /*AFLA*/  * guaranteed to execute a full memory barrier during the time interval
// /*AFLA*/  * between the call to call_rcu() and the invocation of "func()" -- even
// /*AFLA*/  * if CPU A and CPU B are the same CPU (but again only if the system has
// /*AFLA*/  * more than one CPU).
// /*AFLA*/  */
// /*AFLA*/ void call_rcu(struct rcu_head *head,
// /*AFLA*/ 	      rcu_callback_t func);
// /*AFLA*/ 
// /*AFLA*/ #else /* #ifdef CONFIG_PREEMPT_RCU */
// /*AFLA*/ 
// /*AFLA*/ /* In classic RCU, call_rcu() is just call_rcu_sched(). */
// /*AFLA*/ #define	call_rcu	call_rcu_sched
// /*AFLA*/ 
// /*AFLA*/ #endif /* #else #ifdef CONFIG_PREEMPT_RCU */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * call_rcu_bh() - Queue an RCU for invocation after a quicker grace period.
// /*AFLA*/  * @head: structure to be used for queueing the RCU updates.
// /*AFLA*/  * @func: actual callback function to be invoked after the grace period
// /*AFLA*/  *
// /*AFLA*/  * The callback function will be invoked some time after a full grace
// /*AFLA*/  * period elapses, in other words after all currently executing RCU
// /*AFLA*/  * read-side critical sections have completed. call_rcu_bh() assumes
// /*AFLA*/  * that the read-side critical sections end on completion of a softirq
// /*AFLA*/  * handler. This means that read-side critical sections in process
// /*AFLA*/  * context must not be interrupted by softirqs. This interface is to be
// /*AFLA*/  * used when most of the read-side critical sections are in softirq context.
// /*AFLA*/  * RCU read-side critical sections are delimited by :
// /*AFLA*/  *  - rcu_read_lock() and  rcu_read_unlock(), if in interrupt context.
// /*AFLA*/  *  OR
// /*AFLA*/  *  - rcu_read_lock_bh() and rcu_read_unlock_bh(), if in process context.
// /*AFLA*/  *  These may be nested.
// /*AFLA*/  *
// /*AFLA*/  * See the description of call_rcu() for more detailed information on
// /*AFLA*/  * memory ordering guarantees.
// /*AFLA*/  */
// /*AFLA*/ void call_rcu_bh(struct rcu_head *head,
// /*AFLA*/ 		 rcu_callback_t func);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * call_rcu_sched() - Queue an RCU for invocation after sched grace period.
// /*AFLA*/  * @head: structure to be used for queueing the RCU updates.
// /*AFLA*/  * @func: actual callback function to be invoked after the grace period
// /*AFLA*/  *
// /*AFLA*/  * The callback function will be invoked some time after a full grace
// /*AFLA*/  * period elapses, in other words after all currently executing RCU
// /*AFLA*/  * read-side critical sections have completed. call_rcu_sched() assumes
// /*AFLA*/  * that the read-side critical sections end on enabling of preemption
// /*AFLA*/  * or on voluntary preemption.
// /*AFLA*/  * RCU read-side critical sections are delimited by :
// /*AFLA*/  *  - rcu_read_lock_sched() and  rcu_read_unlock_sched(),
// /*AFLA*/  *  OR
// /*AFLA*/  *  anything that disables preemption.
// /*AFLA*/  *  These may be nested.
// /*AFLA*/  *
// /*AFLA*/  * See the description of call_rcu() for more detailed information on
// /*AFLA*/  * memory ordering guarantees.
// /*AFLA*/  */
// /*AFLA*/ void call_rcu_sched(struct rcu_head *head,
// /*AFLA*/ 		    rcu_callback_t func);
// /*AFLA*/ 
// /*AFLA*/ void synchronize_sched(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Structure allowing asynchronous waiting on RCU.
// /*AFLA*/  */
// /*AFLA*/ struct rcu_synchronize {
// /*AFLA*/ 	struct rcu_head head;
// /*AFLA*/ 	struct completion completion;
// /*AFLA*/ };
// /*AFLA*/ void wakeme_after_rcu(struct rcu_head *head);
// /*AFLA*/ 
// /*AFLA*/ void __wait_rcu_gp(bool checktiny, int n, call_rcu_func_t *crcu_array,
// /*AFLA*/ 		   struct rcu_synchronize *rs_array);
// /*AFLA*/ 
// /*AFLA*/ #define _wait_rcu_gp(checktiny, ...) \
// /*AFLA*/ do {									\
// /*AFLA*/ 	call_rcu_func_t __crcu_array[] = { __VA_ARGS__ };		\
// /*AFLA*/ 	struct rcu_synchronize __rs_array[ARRAY_SIZE(__crcu_array)];	\
// /*AFLA*/ 	__wait_rcu_gp(checktiny, ARRAY_SIZE(__crcu_array),		\
// /*AFLA*/ 			__crcu_array, __rs_array);			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define wait_rcu_gp(...) _wait_rcu_gp(false, __VA_ARGS__)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * synchronize_rcu_mult - Wait concurrently for multiple grace periods
// /*AFLA*/  * @...: List of call_rcu() functions for the flavors to wait on.
// /*AFLA*/  *
// /*AFLA*/  * This macro waits concurrently for multiple flavors of RCU grace periods.
// /*AFLA*/  * For example, synchronize_rcu_mult(call_rcu, call_rcu_bh) would wait
// /*AFLA*/  * on concurrent RCU and RCU-bh grace periods.  Waiting on a give SRCU
// /*AFLA*/  * domain requires you to write a wrapper function for that SRCU domain's
// /*AFLA*/  * call_srcu() function, supplying the corresponding srcu_struct.
// /*AFLA*/  *
// /*AFLA*/  * If Tiny RCU, tell _wait_rcu_gp() not to bother waiting for RCU
// /*AFLA*/  * or RCU-bh, given that anywhere synchronize_rcu_mult() can be called
// /*AFLA*/  * is automatically a grace period.
// /*AFLA*/  */
// /*AFLA*/ #define synchronize_rcu_mult(...) \
// /*AFLA*/ 	_wait_rcu_gp(IS_ENABLED(CONFIG_TINY_RCU), __VA_ARGS__)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * call_rcu_tasks() - Queue an RCU for invocation task-based grace period
// /*AFLA*/  * @head: structure to be used for queueing the RCU updates.
// /*AFLA*/  * @func: actual callback function to be invoked after the grace period
// /*AFLA*/  *
// /*AFLA*/  * The callback function will be invoked some time after a full grace
// /*AFLA*/  * period elapses, in other words after all currently executing RCU
// /*AFLA*/  * read-side critical sections have completed. call_rcu_tasks() assumes
// /*AFLA*/  * that the read-side critical sections end at a voluntary context
// /*AFLA*/  * switch (not a preemption!), entry into idle, or transition to usermode
// /*AFLA*/  * execution.  As such, there are no read-side primitives analogous to
// /*AFLA*/  * rcu_read_lock() and rcu_read_unlock() because this primitive is intended
// /*AFLA*/  * to determine that all tasks have passed through a safe state, not so
// /*AFLA*/  * much for data-strcuture synchronization.
// /*AFLA*/  *
// /*AFLA*/  * See the description of call_rcu() for more detailed information on
// /*AFLA*/  * memory ordering guarantees.
// /*AFLA*/  */
// /*AFLA*/ void call_rcu_tasks(struct rcu_head *head, rcu_callback_t func);
// /*AFLA*/ void synchronize_rcu_tasks(void);
// /*AFLA*/ void rcu_barrier_tasks(void);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PREEMPT_RCU
// /*AFLA*/ 
// /*AFLA*/ void __rcu_read_lock(void);
// /*AFLA*/ void __rcu_read_unlock(void);
// /*AFLA*/ void rcu_read_unlock_special(struct task_struct *t);
// /*AFLA*/ void synchronize_rcu(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Defined as a macro as it is a very low level header included from
// /*AFLA*/  * areas that don't even know about current.  This gives the rcu_read_lock()
// /*AFLA*/  * nesting depth, but makes sense only if CONFIG_PREEMPT_RCU -- in other
// /*AFLA*/  * types of kernel builds, the rcu_read_lock() nesting depth is unknowable.
// /*AFLA*/  */
// /*AFLA*/ #define rcu_preempt_depth() (current->rcu_read_lock_nesting)
// /*AFLA*/ 
// /*AFLA*/ #else /* #ifdef CONFIG_PREEMPT_RCU */
// /*AFLA*/ 
// /*AFLA*/ static inline void __rcu_read_lock(void)
// /*AFLA*/ {
// /*AFLA*/ 	if (IS_ENABLED(CONFIG_PREEMPT_COUNT))
// /*AFLA*/ 		preempt_disable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __rcu_read_unlock(void)
// /*AFLA*/ {
// /*AFLA*/ 	if (IS_ENABLED(CONFIG_PREEMPT_COUNT))
// /*AFLA*/ 		preempt_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void synchronize_rcu(void)
// /*AFLA*/ {
// /*AFLA*/ 	synchronize_sched();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int rcu_preempt_depth(void)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* #else #ifdef CONFIG_PREEMPT_RCU */
// /*AFLA*/ 
// /*AFLA*/ /* Internal to kernel */
// /*AFLA*/ void rcu_init(void);
// /*AFLA*/ void rcu_sched_qs(void);
// /*AFLA*/ void rcu_bh_qs(void);
// /*AFLA*/ void rcu_check_callbacks(int user);
// /*AFLA*/ void rcu_report_dead(unsigned int cpu);
// /*AFLA*/ void rcu_cpu_starting(unsigned int cpu);
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_TINY_RCU
// /*AFLA*/ void rcu_end_inkernel_boot(void);
// /*AFLA*/ #else /* #ifndef CONFIG_TINY_RCU */
// /*AFLA*/ static inline void rcu_end_inkernel_boot(void) { }
// /*AFLA*/ #endif /* #ifndef CONFIG_TINY_RCU */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_RCU_STALL_COMMON
// /*AFLA*/ void rcu_sysrq_start(void);
// /*AFLA*/ void rcu_sysrq_end(void);
// /*AFLA*/ #else /* #ifdef CONFIG_RCU_STALL_COMMON */
// /*AFLA*/ static inline void rcu_sysrq_start(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void rcu_sysrq_end(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif /* #else #ifdef CONFIG_RCU_STALL_COMMON */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NO_HZ_FULL
// /*AFLA*/ void rcu_user_enter(void);
// /*AFLA*/ void rcu_user_exit(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void rcu_user_enter(void) { }
// /*AFLA*/ static inline void rcu_user_exit(void) { }
// /*AFLA*/ #endif /* CONFIG_NO_HZ_FULL */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_RCU_NOCB_CPU
// /*AFLA*/ void rcu_init_nohz(void);
// /*AFLA*/ #else /* #ifdef CONFIG_RCU_NOCB_CPU */
// /*AFLA*/ static inline void rcu_init_nohz(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif /* #else #ifdef CONFIG_RCU_NOCB_CPU */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * RCU_NONIDLE - Indicate idle-loop code that needs RCU readers
// /*AFLA*/  * @a: Code that RCU needs to pay attention to.
// /*AFLA*/  *
// /*AFLA*/  * RCU, RCU-bh, and RCU-sched read-side critical sections are forbidden
// /*AFLA*/  * in the inner idle loop, that is, between the rcu_idle_enter() and
// /*AFLA*/  * the rcu_idle_exit() -- RCU will happily ignore any such read-side
// /*AFLA*/  * critical sections.  However, things like powertop need tracepoints
// /*AFLA*/  * in the inner idle loop.
// /*AFLA*/  *
// /*AFLA*/  * This macro provides the way out:  RCU_NONIDLE(do_something_with_RCU())
// /*AFLA*/  * will tell RCU that it needs to pay attention, invoke its argument
// /*AFLA*/  * (in this example, calling the do_something_with_RCU() function),
// /*AFLA*/  * and then tell RCU to go back to ignoring this CPU.  It is permissible
// /*AFLA*/  * to nest RCU_NONIDLE() wrappers, but not indefinitely (but the limit is
// /*AFLA*/  * on the order of a million or so, even on 32-bit systems).  It is
// /*AFLA*/  * not legal to block within RCU_NONIDLE(), nor is it permissible to
// /*AFLA*/  * transfer control either into or out of RCU_NONIDLE()'s statement.
// /*AFLA*/  */
// /*AFLA*/ #define RCU_NONIDLE(a) \
// /*AFLA*/ 	do { \
// /*AFLA*/ 		rcu_irq_enter_irqson(); \
// /*AFLA*/ 		do { a; } while (0); \
// /*AFLA*/ 		rcu_irq_exit_irqson(); \
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Note a voluntary context switch for RCU-tasks benefit.  This is a
// /*AFLA*/  * macro rather than an inline function to avoid #include hell.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_TASKS_RCU
// /*AFLA*/ #define TASKS_RCU(x) x
// /*AFLA*/ extern struct srcu_struct tasks_rcu_exit_srcu;
// /*AFLA*/ #define rcu_note_voluntary_context_switch(t) \
// /*AFLA*/ 	do { \
// /*AFLA*/ 		rcu_all_qs(); \
// /*AFLA*/ 		if (READ_ONCE((t)->rcu_tasks_holdout)) \
// /*AFLA*/ 			WRITE_ONCE((t)->rcu_tasks_holdout, false); \
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #else /* #ifdef CONFIG_TASKS_RCU */
// /*AFLA*/ #define TASKS_RCU(x) do { } while (0)
// /*AFLA*/ #define rcu_note_voluntary_context_switch(t)	rcu_all_qs()
// /*AFLA*/ #endif /* #else #ifdef CONFIG_TASKS_RCU */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cond_resched_rcu_qs - Report potential quiescent states to RCU
// /*AFLA*/  *
// /*AFLA*/  * This macro resembles cond_resched(), except that it is defined to
// /*AFLA*/  * report potential quiescent states to RCU-tasks even if the cond_resched()
// /*AFLA*/  * machinery were to be shut off, as some advocate for PREEMPT kernels.
// /*AFLA*/  */
// /*AFLA*/ #define cond_resched_rcu_qs() \
// /*AFLA*/ do { \
// /*AFLA*/ 	if (!cond_resched()) \
// /*AFLA*/ 		rcu_note_voluntary_context_switch(current); \
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_DEBUG_LOCK_ALLOC) || defined(CONFIG_RCU_TRACE) || defined(CONFIG_SMP)
// /*AFLA*/ bool __rcu_is_watching(void);
// /*AFLA*/ #endif /* #if defined(CONFIG_DEBUG_LOCK_ALLOC) || defined(CONFIG_RCU_TRACE) || defined(CONFIG_SMP) */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Infrastructure to implement the synchronize_() primitives in
// /*AFLA*/  * TREE_RCU and rcu_barrier_() primitives in TINY_RCU.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_TREE_RCU) || defined(CONFIG_PREEMPT_RCU)
// /*AFLA*/ #include <linux/rcutree.h>
// /*AFLA*/ #elif defined(CONFIG_TINY_RCU)
// /*AFLA*/ #include <linux/rcutiny.h>
// /*AFLA*/ #else
// /*AFLA*/ #error "Unknown RCU implementation specified to kernel configuration"
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * init_rcu_head_on_stack()/destroy_rcu_head_on_stack() are needed for dynamic
// /*AFLA*/  * initialization and destruction of rcu_head on the stack. rcu_head structures
// /*AFLA*/  * allocated dynamically in the heap or defined statically don't need any
// /*AFLA*/  * initialization.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_DEBUG_OBJECTS_RCU_HEAD
// /*AFLA*/ void init_rcu_head(struct rcu_head *head);
// /*AFLA*/ void destroy_rcu_head(struct rcu_head *head);
// /*AFLA*/ void init_rcu_head_on_stack(struct rcu_head *head);
// /*AFLA*/ void destroy_rcu_head_on_stack(struct rcu_head *head);
// /*AFLA*/ #else /* !CONFIG_DEBUG_OBJECTS_RCU_HEAD */
// /*AFLA*/ static inline void init_rcu_head(struct rcu_head *head)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void destroy_rcu_head(struct rcu_head *head)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void init_rcu_head_on_stack(struct rcu_head *head)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void destroy_rcu_head_on_stack(struct rcu_head *head)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif	/* #else !CONFIG_DEBUG_OBJECTS_RCU_HEAD */
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_HOTPLUG_CPU) && defined(CONFIG_PROVE_RCU)
// /*AFLA*/ bool rcu_lockdep_current_cpu_online(void);
// /*AFLA*/ #else /* #if defined(CONFIG_HOTPLUG_CPU) && defined(CONFIG_PROVE_RCU) */
// /*AFLA*/ static inline bool rcu_lockdep_current_cpu_online(void)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ #endif /* #else #if defined(CONFIG_HOTPLUG_CPU) && defined(CONFIG_PROVE_RCU) */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ 
// /*AFLA*/ static inline void rcu_lock_acquire(struct lockdep_map *map)
// /*AFLA*/ {
// /*AFLA*/ 	lock_acquire(map, 0, 0, 2, 0, NULL, _THIS_IP_);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void rcu_lock_release(struct lockdep_map *map)
// /*AFLA*/ {
// /*AFLA*/ 	lock_release(map, 1, _THIS_IP_);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct lockdep_map rcu_lock_map;
// /*AFLA*/ extern struct lockdep_map rcu_bh_lock_map;
// /*AFLA*/ extern struct lockdep_map rcu_sched_lock_map;
// /*AFLA*/ extern struct lockdep_map rcu_callback_map;
// /*AFLA*/ int debug_lockdep_rcu_enabled(void);
// /*AFLA*/ 
// /*AFLA*/ int rcu_read_lock_held(void);
// /*AFLA*/ int rcu_read_lock_bh_held(void);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rcu_read_lock_sched_held() - might we be in RCU-sched read-side critical section?
// /*AFLA*/  *
// /*AFLA*/  * If CONFIG_DEBUG_LOCK_ALLOC is selected, returns nonzero iff in an
// /*AFLA*/  * RCU-sched read-side critical section.  In absence of
// /*AFLA*/  * CONFIG_DEBUG_LOCK_ALLOC, this assumes we are in an RCU-sched read-side
// /*AFLA*/  * critical section unless it can prove otherwise.
// /*AFLA*/  */
// /*AFLA*/ int rcu_read_lock_sched_held(void);
// /*AFLA*/ 
// /*AFLA*/ #else /* #ifdef CONFIG_DEBUG_LOCK_ALLOC */
// /*AFLA*/ 
// /*AFLA*/ # define rcu_lock_acquire(a)		do { } while (0)
// /*AFLA*/ # define rcu_lock_release(a)		do { } while (0)
// /*AFLA*/ 
// /*AFLA*/ static inline int rcu_read_lock_held(void)
// /*AFLA*/ {
// /*AFLA*/ 	return 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int rcu_read_lock_bh_held(void)
// /*AFLA*/ {
// /*AFLA*/ 	return 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int rcu_read_lock_sched_held(void)
// /*AFLA*/ {
// /*AFLA*/ 	return !preemptible();
// /*AFLA*/ }
// /*AFLA*/ #endif /* #else #ifdef CONFIG_DEBUG_LOCK_ALLOC */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PROVE_RCU
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * RCU_LOCKDEP_WARN - emit lockdep splat if specified condition is met
// /*AFLA*/  * @c: condition to check
// /*AFLA*/  * @s: informative message
// /*AFLA*/  */
// /*AFLA*/ #define RCU_LOCKDEP_WARN(c, s)						\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		static bool __section(.data.unlikely) __warned;		\
// /*AFLA*/ 		if (debug_lockdep_rcu_enabled() && !__warned && (c)) {	\
// /*AFLA*/ 			__warned = true;				\
// /*AFLA*/ 			lockdep_rcu_suspicious(__FILE__, __LINE__, s);	\
// /*AFLA*/ 		}							\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_PROVE_RCU) && !defined(CONFIG_PREEMPT_RCU)
// /*AFLA*/ static inline void rcu_preempt_sleep_check(void)
// /*AFLA*/ {
// /*AFLA*/ 	RCU_LOCKDEP_WARN(lock_is_held(&rcu_lock_map),
// /*AFLA*/ 			 "Illegal context switch in RCU read-side critical section");
// /*AFLA*/ }
// /*AFLA*/ #else /* #ifdef CONFIG_PROVE_RCU */
// /*AFLA*/ static inline void rcu_preempt_sleep_check(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif /* #else #ifdef CONFIG_PROVE_RCU */
// /*AFLA*/ 
// /*AFLA*/ #define rcu_sleep_check()						\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		rcu_preempt_sleep_check();				\
// /*AFLA*/ 		RCU_LOCKDEP_WARN(lock_is_held(&rcu_bh_lock_map),	\
// /*AFLA*/ 				 "Illegal context switch in RCU-bh read-side critical section"); \
// /*AFLA*/ 		RCU_LOCKDEP_WARN(lock_is_held(&rcu_sched_lock_map),	\
// /*AFLA*/ 				 "Illegal context switch in RCU-sched read-side critical section"); \
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #else /* #ifdef CONFIG_PROVE_RCU */
// /*AFLA*/ 
// /*AFLA*/ #define RCU_LOCKDEP_WARN(c, s) do { } while (0)
// /*AFLA*/ #define rcu_sleep_check() do { } while (0)
// /*AFLA*/ 
// /*AFLA*/ #endif /* #else #ifdef CONFIG_PROVE_RCU */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Helper functions for rcu_dereference_check(), rcu_dereference_protected()
// /*AFLA*/  * and rcu_assign_pointer().  Some of these could be folded into their
// /*AFLA*/  * callers, but they are left separate in order to ease introduction of
// /*AFLA*/  * multiple flavors of pointers to match the multiple flavors of RCU
// /*AFLA*/  * (e.g., __rcu_bh, * __rcu_sched, and __srcu), should this make sense in
// /*AFLA*/  * the future.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifdef __CHECKER__
// /*AFLA*/ #define rcu_dereference_sparse(p, space) \
// /*AFLA*/ 	((void)(((typeof(*p) space *)p) == p))
// /*AFLA*/ #else /* #ifdef __CHECKER__ */
// /*AFLA*/ #define rcu_dereference_sparse(p, space)
// /*AFLA*/ #endif /* #else #ifdef __CHECKER__ */
// /*AFLA*/ 
// /*AFLA*/ #define __rcu_access_pointer(p, space) \
// /*AFLA*/ ({ \
// /*AFLA*/ 	typeof(*p) *_________p1 = (typeof(*p) *__force)READ_ONCE(p); \
// /*AFLA*/ 	rcu_dereference_sparse(p, space); \
// /*AFLA*/ 	((typeof(*p) __force __kernel *)(_________p1)); \
// /*AFLA*/ })
// /*AFLA*/ #define __rcu_dereference_check(p, c, space) \
// /*AFLA*/ ({ \
// /*AFLA*/ 	/* Dependency order vs. p above. */ \
// /*AFLA*/ 	typeof(*p) *________p1 = (typeof(*p) *__force)lockless_dereference(p); \
// /*AFLA*/ 	RCU_LOCKDEP_WARN(!(c), "suspicious rcu_dereference_check() usage"); \
// /*AFLA*/ 	rcu_dereference_sparse(p, space); \
// /*AFLA*/ 	((typeof(*p) __force __kernel *)(________p1)); \
// /*AFLA*/ })
// /*AFLA*/ #define __rcu_dereference_protected(p, c, space) \
// /*AFLA*/ ({ \
// /*AFLA*/ 	RCU_LOCKDEP_WARN(!(c), "suspicious rcu_dereference_protected() usage"); \
// /*AFLA*/ 	rcu_dereference_sparse(p, space); \
// /*AFLA*/ 	((typeof(*p) __force __kernel *)(p)); \
// /*AFLA*/ })
// /*AFLA*/ #define rcu_dereference_raw(p) \
// /*AFLA*/ ({ \
// /*AFLA*/ 	/* Dependency order vs. p above. */ \
// /*AFLA*/ 	typeof(p) ________p1 = lockless_dereference(p); \
// /*AFLA*/ 	((typeof(*p) __force __kernel *)(________p1)); \
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * RCU_INITIALIZER() - statically initialize an RCU-protected global variable
// /*AFLA*/  * @v: The value to statically initialize with.
// /*AFLA*/  */
// /*AFLA*/ #define RCU_INITIALIZER(v) (typeof(*(v)) __force __rcu *)(v)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rcu_assign_pointer() - assign to RCU-protected pointer
// /*AFLA*/  * @p: pointer to assign to
// /*AFLA*/  * @v: value to assign (publish)
// /*AFLA*/  *
// /*AFLA*/  * Assigns the specified value to the specified RCU-protected
// /*AFLA*/  * pointer, ensuring that any concurrent RCU readers will see
// /*AFLA*/  * any prior initialization.
// /*AFLA*/  *
// /*AFLA*/  * Inserts memory barriers on architectures that require them
// /*AFLA*/  * (which is most of them), and also prevents the compiler from
// /*AFLA*/  * reordering the code that initializes the structure after the pointer
// /*AFLA*/  * assignment.  More importantly, this call documents which pointers
// /*AFLA*/  * will be dereferenced by RCU read-side code.
// /*AFLA*/  *
// /*AFLA*/  * In some special cases, you may use RCU_INIT_POINTER() instead
// /*AFLA*/  * of rcu_assign_pointer().  RCU_INIT_POINTER() is a bit faster due
// /*AFLA*/  * to the fact that it does not constrain either the CPU or the compiler.
// /*AFLA*/  * That said, using RCU_INIT_POINTER() when you should have used
// /*AFLA*/  * rcu_assign_pointer() is a very bad thing that results in
// /*AFLA*/  * impossible-to-diagnose memory corruption.  So please be careful.
// /*AFLA*/  * See the RCU_INIT_POINTER() comment header for details.
// /*AFLA*/  *
// /*AFLA*/  * Note that rcu_assign_pointer() evaluates each of its arguments only
// /*AFLA*/  * once, appearances notwithstanding.  One of the "extra" evaluations
// /*AFLA*/  * is in typeof() and the other visible only to sparse (__CHECKER__),
// /*AFLA*/  * neither of which actually execute the argument.  As with most cpp
// /*AFLA*/  * macros, this execute-arguments-only-once property is important, so
// /*AFLA*/  * please be careful when making changes to rcu_assign_pointer() and the
// /*AFLA*/  * other macros that it invokes.
// /*AFLA*/  */
// /*AFLA*/ #define rcu_assign_pointer(p, v)					      \
// /*AFLA*/ ({									      \
// /*AFLA*/ 	uintptr_t _r_a_p__v = (uintptr_t)(v);				      \
// /*AFLA*/ 									      \
// /*AFLA*/ 	if (__builtin_constant_p(v) && (_r_a_p__v) == (uintptr_t)NULL)	      \
// /*AFLA*/ 		WRITE_ONCE((p), (typeof(p))(_r_a_p__v));		      \
// /*AFLA*/ 	else								      \
// /*AFLA*/ 		smp_store_release(&p, RCU_INITIALIZER((typeof(p))_r_a_p__v)); \
// /*AFLA*/ 	_r_a_p__v;							      \
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rcu_access_pointer() - fetch RCU pointer with no dereferencing
// /*AFLA*/  * @p: The pointer to read
// /*AFLA*/  *
// /*AFLA*/  * Return the value of the specified RCU-protected pointer, but omit the
// /*AFLA*/  * smp_read_barrier_depends() and keep the READ_ONCE().  This is useful
// /*AFLA*/  * when the value of this pointer is accessed, but the pointer is not
// /*AFLA*/  * dereferenced, for example, when testing an RCU-protected pointer against
// /*AFLA*/  * NULL.  Although rcu_access_pointer() may also be used in cases where
// /*AFLA*/  * update-side locks prevent the value of the pointer from changing, you
// /*AFLA*/  * should instead use rcu_dereference_protected() for this use case.
// /*AFLA*/  *
// /*AFLA*/  * It is also permissible to use rcu_access_pointer() when read-side
// /*AFLA*/  * access to the pointer was removed at least one grace period ago, as
// /*AFLA*/  * is the case in the context of the RCU callback that is freeing up
// /*AFLA*/  * the data, or after a synchronize_rcu() returns.  This can be useful
// /*AFLA*/  * when tearing down multi-linked structures after a grace period
// /*AFLA*/  * has elapsed.
// /*AFLA*/  */
// /*AFLA*/ #define rcu_access_pointer(p) __rcu_access_pointer((p), __rcu)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rcu_dereference_check() - rcu_dereference with debug checking
// /*AFLA*/  * @p: The pointer to read, prior to dereferencing
// /*AFLA*/  * @c: The conditions under which the dereference will take place
// /*AFLA*/  *
// /*AFLA*/  * Do an rcu_dereference(), but check that the conditions under which the
// /*AFLA*/  * dereference will take place are correct.  Typically the conditions
// /*AFLA*/  * indicate the various locking conditions that should be held at that
// /*AFLA*/  * point.  The check should return true if the conditions are satisfied.
// /*AFLA*/  * An implicit check for being in an RCU read-side critical section
// /*AFLA*/  * (rcu_read_lock()) is included.
// /*AFLA*/  *
// /*AFLA*/  * For example:
// /*AFLA*/  *
// /*AFLA*/  *	bar = rcu_dereference_check(foo->bar, lockdep_is_held(&foo->lock));
// /*AFLA*/  *
// /*AFLA*/  * could be used to indicate to lockdep that foo->bar may only be dereferenced
// /*AFLA*/  * if either rcu_read_lock() is held, or that the lock required to replace
// /*AFLA*/  * the bar struct at foo->bar is held.
// /*AFLA*/  *
// /*AFLA*/  * Note that the list of conditions may also include indications of when a lock
// /*AFLA*/  * need not be held, for example during initialisation or destruction of the
// /*AFLA*/  * target struct:
// /*AFLA*/  *
// /*AFLA*/  *	bar = rcu_dereference_check(foo->bar, lockdep_is_held(&foo->lock) ||
// /*AFLA*/  *					      atomic_read(&foo->usage) == 0);
// /*AFLA*/  *
// /*AFLA*/  * Inserts memory barriers on architectures that require them
// /*AFLA*/  * (currently only the Alpha), prevents the compiler from refetching
// /*AFLA*/  * (and from merging fetches), and, more importantly, documents exactly
// /*AFLA*/  * which pointers are protected by RCU and checks that the pointer is
// /*AFLA*/  * annotated as __rcu.
// /*AFLA*/  */
// /*AFLA*/ #define rcu_dereference_check(p, c) \
// /*AFLA*/ 	__rcu_dereference_check((p), (c) || rcu_read_lock_held(), __rcu)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rcu_dereference_bh_check() - rcu_dereference_bh with debug checking
// /*AFLA*/  * @p: The pointer to read, prior to dereferencing
// /*AFLA*/  * @c: The conditions under which the dereference will take place
// /*AFLA*/  *
// /*AFLA*/  * This is the RCU-bh counterpart to rcu_dereference_check().
// /*AFLA*/  */
// /*AFLA*/ #define rcu_dereference_bh_check(p, c) \
// /*AFLA*/ 	__rcu_dereference_check((p), (c) || rcu_read_lock_bh_held(), __rcu)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rcu_dereference_sched_check() - rcu_dereference_sched with debug checking
// /*AFLA*/  * @p: The pointer to read, prior to dereferencing
// /*AFLA*/  * @c: The conditions under which the dereference will take place
// /*AFLA*/  *
// /*AFLA*/  * This is the RCU-sched counterpart to rcu_dereference_check().
// /*AFLA*/  */
// /*AFLA*/ #define rcu_dereference_sched_check(p, c) \
// /*AFLA*/ 	__rcu_dereference_check((p), (c) || rcu_read_lock_sched_held(), \
// /*AFLA*/ 				__rcu)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The tracing infrastructure traces RCU (we want that), but unfortunately
// /*AFLA*/  * some of the RCU checks causes tracing to lock up the system.
// /*AFLA*/  *
// /*AFLA*/  * The no-tracing version of rcu_dereference_raw() must not call
// /*AFLA*/  * rcu_read_lock_held().
// /*AFLA*/  */
// /*AFLA*/ #define rcu_dereference_raw_notrace(p) __rcu_dereference_check((p), 1, __rcu)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rcu_dereference_protected() - fetch RCU pointer when updates prevented
// /*AFLA*/  * @p: The pointer to read, prior to dereferencing
// /*AFLA*/  * @c: The conditions under which the dereference will take place
// /*AFLA*/  *
// /*AFLA*/  * Return the value of the specified RCU-protected pointer, but omit
// /*AFLA*/  * both the smp_read_barrier_depends() and the READ_ONCE().  This
// /*AFLA*/  * is useful in cases where update-side locks prevent the value of the
// /*AFLA*/  * pointer from changing.  Please note that this primitive does -not-
// /*AFLA*/  * prevent the compiler from repeating this reference or combining it
// /*AFLA*/  * with other references, so it should not be used without protection
// /*AFLA*/  * of appropriate locks.
// /*AFLA*/  *
// /*AFLA*/  * This function is only for update-side use.  Using this function
// /*AFLA*/  * when protected only by rcu_read_lock() will result in infrequent
// /*AFLA*/  * but very ugly failures.
// /*AFLA*/  */
// /*AFLA*/ #define rcu_dereference_protected(p, c) \
// /*AFLA*/ 	__rcu_dereference_protected((p), (c), __rcu)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rcu_dereference() - fetch RCU-protected pointer for dereferencing
// /*AFLA*/  * @p: The pointer to read, prior to dereferencing
// /*AFLA*/  *
// /*AFLA*/  * This is a simple wrapper around rcu_dereference_check().
// /*AFLA*/  */
// /*AFLA*/ #define rcu_dereference(p) rcu_dereference_check(p, 0)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rcu_dereference_bh() - fetch an RCU-bh-protected pointer for dereferencing
// /*AFLA*/  * @p: The pointer to read, prior to dereferencing
// /*AFLA*/  *
// /*AFLA*/  * Makes rcu_dereference_check() do the dirty work.
// /*AFLA*/  */
// /*AFLA*/ #define rcu_dereference_bh(p) rcu_dereference_bh_check(p, 0)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rcu_dereference_sched() - fetch RCU-sched-protected pointer for dereferencing
// /*AFLA*/  * @p: The pointer to read, prior to dereferencing
// /*AFLA*/  *
// /*AFLA*/  * Makes rcu_dereference_check() do the dirty work.
// /*AFLA*/  */
// /*AFLA*/ #define rcu_dereference_sched(p) rcu_dereference_sched_check(p, 0)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rcu_pointer_handoff() - Hand off a pointer from RCU to other mechanism
// /*AFLA*/  * @p: The pointer to hand off
// /*AFLA*/  *
// /*AFLA*/  * This is simply an identity function, but it documents where a pointer
// /*AFLA*/  * is handed off from RCU to some other synchronization mechanism, for
// /*AFLA*/  * example, reference counting or locking.  In C11, it would map to
// /*AFLA*/  * kill_dependency().  It could be used as follows:
// /*AFLA*/  *
// /*AFLA*/  *	rcu_read_lock();
// /*AFLA*/  *	p = rcu_dereference(gp);
// /*AFLA*/  *	long_lived = is_long_lived(p);
// /*AFLA*/  *	if (long_lived) {
// /*AFLA*/  *		if (!atomic_inc_not_zero(p->refcnt))
// /*AFLA*/  *			long_lived = false;
// /*AFLA*/  *		else
// /*AFLA*/  *			p = rcu_pointer_handoff(p);
// /*AFLA*/  *	}
// /*AFLA*/  *	rcu_read_unlock();
// /*AFLA*/  */
// /*AFLA*/ #define rcu_pointer_handoff(p) (p)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rcu_read_lock() - mark the beginning of an RCU read-side critical section
// /*AFLA*/  *
// /*AFLA*/  * When synchronize_rcu() is invoked on one CPU while other CPUs
// /*AFLA*/  * are within RCU read-side critical sections, then the
// /*AFLA*/  * synchronize_rcu() is guaranteed to block until after all the other
// /*AFLA*/  * CPUs exit their critical sections.  Similarly, if call_rcu() is invoked
// /*AFLA*/  * on one CPU while other CPUs are within RCU read-side critical
// /*AFLA*/  * sections, invocation of the corresponding RCU callback is deferred
// /*AFLA*/  * until after the all the other CPUs exit their critical sections.
// /*AFLA*/  *
// /*AFLA*/  * Note, however, that RCU callbacks are permitted to run concurrently
// /*AFLA*/  * with new RCU read-side critical sections.  One way that this can happen
// /*AFLA*/  * is via the following sequence of events: (1) CPU 0 enters an RCU
// /*AFLA*/  * read-side critical section, (2) CPU 1 invokes call_rcu() to register
// /*AFLA*/  * an RCU callback, (3) CPU 0 exits the RCU read-side critical section,
// /*AFLA*/  * (4) CPU 2 enters a RCU read-side critical section, (5) the RCU
// /*AFLA*/  * callback is invoked.  This is legal, because the RCU read-side critical
// /*AFLA*/  * section that was running concurrently with the call_rcu() (and which
// /*AFLA*/  * therefore might be referencing something that the corresponding RCU
// /*AFLA*/  * callback would free up) has completed before the corresponding
// /*AFLA*/  * RCU callback is invoked.
// /*AFLA*/  *
// /*AFLA*/  * RCU read-side critical sections may be nested.  Any deferred actions
// /*AFLA*/  * will be deferred until the outermost RCU read-side critical section
// /*AFLA*/  * completes.
// /*AFLA*/  *
// /*AFLA*/  * You can avoid reading and understanding the next paragraph by
// /*AFLA*/  * following this rule: don't put anything in an rcu_read_lock() RCU
// /*AFLA*/  * read-side critical section that would block in a !PREEMPT kernel.
// /*AFLA*/  * But if you want the full story, read on!
// /*AFLA*/  *
// /*AFLA*/  * In non-preemptible RCU implementations (TREE_RCU and TINY_RCU),
// /*AFLA*/  * it is illegal to block while in an RCU read-side critical section.
// /*AFLA*/  * In preemptible RCU implementations (PREEMPT_RCU) in CONFIG_PREEMPT
// /*AFLA*/  * kernel builds, RCU read-side critical sections may be preempted,
// /*AFLA*/  * but explicit blocking is illegal.  Finally, in preemptible RCU
// /*AFLA*/  * implementations in real-time (with -rt patchset) kernel builds, RCU
// /*AFLA*/  * read-side critical sections may be preempted and they may also block, but
// /*AFLA*/  * only when acquiring spinlocks that are subject to priority inheritance.
// /*AFLA*/  */
// /*AFLA*/ static inline void rcu_read_lock(void)
// /*AFLA*/ {
// /*AFLA*/ 	__rcu_read_lock();
// /*AFLA*/ 	__acquire(RCU);
// /*AFLA*/ 	rcu_lock_acquire(&rcu_lock_map);
// /*AFLA*/ 	RCU_LOCKDEP_WARN(!rcu_is_watching(),
// /*AFLA*/ 			 "rcu_read_lock() used illegally while idle");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * So where is rcu_write_lock()?  It does not exist, as there is no
// /*AFLA*/  * way for writers to lock out RCU readers.  This is a feature, not
// /*AFLA*/  * a bug -- this property is what provides RCU's performance benefits.
// /*AFLA*/  * Of course, writers must coordinate with each other.  The normal
// /*AFLA*/  * spinlock primitives work well for this, but any other technique may be
// /*AFLA*/  * used as well.  RCU does not care how the writers keep out of each
// /*AFLA*/  * others' way, as long as they do so.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rcu_read_unlock() - marks the end of an RCU read-side critical section.
// /*AFLA*/  *
// /*AFLA*/  * In most situations, rcu_read_unlock() is immune from deadlock.
// /*AFLA*/  * However, in kernels built with CONFIG_RCU_BOOST, rcu_read_unlock()
// /*AFLA*/  * is responsible for deboosting, which it does via rt_mutex_unlock().
// /*AFLA*/  * Unfortunately, this function acquires the scheduler's runqueue and
// /*AFLA*/  * priority-inheritance spinlocks.  This means that deadlock could result
// /*AFLA*/  * if the caller of rcu_read_unlock() already holds one of these locks or
// /*AFLA*/  * any lock that is ever acquired while holding them; or any lock which
// /*AFLA*/  * can be taken from interrupt context because rcu_boost()->rt_mutex_lock()
// /*AFLA*/  * does not disable irqs while taking ->wait_lock.
// /*AFLA*/  *
// /*AFLA*/  * That said, RCU readers are never priority boosted unless they were
// /*AFLA*/  * preempted.  Therefore, one way to avoid deadlock is to make sure
// /*AFLA*/  * that preemption never happens within any RCU read-side critical
// /*AFLA*/  * section whose outermost rcu_read_unlock() is called with one of
// /*AFLA*/  * rt_mutex_unlock()'s locks held.  Such preemption can be avoided in
// /*AFLA*/  * a number of ways, for example, by invoking preempt_disable() before
// /*AFLA*/  * critical section's outermost rcu_read_lock().
// /*AFLA*/  *
// /*AFLA*/  * Given that the set of locks acquired by rt_mutex_unlock() might change
// /*AFLA*/  * at any time, a somewhat more future-proofed approach is to make sure
// /*AFLA*/  * that that preemption never happens within any RCU read-side critical
// /*AFLA*/  * section whose outermost rcu_read_unlock() is called with irqs disabled.
// /*AFLA*/  * This approach relies on the fact that rt_mutex_unlock() currently only
// /*AFLA*/  * acquires irq-disabled locks.
// /*AFLA*/  *
// /*AFLA*/  * The second of these two approaches is best in most situations,
// /*AFLA*/  * however, the first approach can also be useful, at least to those
// /*AFLA*/  * developers willing to keep abreast of the set of locks acquired by
// /*AFLA*/  * rt_mutex_unlock().
// /*AFLA*/  *
// /*AFLA*/  * See rcu_read_lock() for more information.
// /*AFLA*/  */
// /*AFLA*/ static inline void rcu_read_unlock(void)
// /*AFLA*/ {
// /*AFLA*/ 	RCU_LOCKDEP_WARN(!rcu_is_watching(),
// /*AFLA*/ 			 "rcu_read_unlock() used illegally while idle");
// /*AFLA*/ 	__release(RCU);
// /*AFLA*/ 	__rcu_read_unlock();
// /*AFLA*/ 	rcu_lock_release(&rcu_lock_map); /* Keep acq info for rls diags. */
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rcu_read_lock_bh() - mark the beginning of an RCU-bh critical section
// /*AFLA*/  *
// /*AFLA*/  * This is equivalent of rcu_read_lock(), but to be used when updates
// /*AFLA*/  * are being done using call_rcu_bh() or synchronize_rcu_bh(). Since
// /*AFLA*/  * both call_rcu_bh() and synchronize_rcu_bh() consider completion of a
// /*AFLA*/  * softirq handler to be a quiescent state, a process in RCU read-side
// /*AFLA*/  * critical section must be protected by disabling softirqs. Read-side
// /*AFLA*/  * critical sections in interrupt context can use just rcu_read_lock(),
// /*AFLA*/  * though this should at least be commented to avoid confusing people
// /*AFLA*/  * reading the code.
// /*AFLA*/  *
// /*AFLA*/  * Note that rcu_read_lock_bh() and the matching rcu_read_unlock_bh()
// /*AFLA*/  * must occur in the same context, for example, it is illegal to invoke
// /*AFLA*/  * rcu_read_unlock_bh() from one task if the matching rcu_read_lock_bh()
// /*AFLA*/  * was invoked from some other task.
// /*AFLA*/  */
// /*AFLA*/ static inline void rcu_read_lock_bh(void)
// /*AFLA*/ {
// /*AFLA*/ 	local_bh_disable();
// /*AFLA*/ 	__acquire(RCU_BH);
// /*AFLA*/ 	rcu_lock_acquire(&rcu_bh_lock_map);
// /*AFLA*/ 	RCU_LOCKDEP_WARN(!rcu_is_watching(),
// /*AFLA*/ 			 "rcu_read_lock_bh() used illegally while idle");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * rcu_read_unlock_bh - marks the end of a softirq-only RCU critical section
// /*AFLA*/  *
// /*AFLA*/  * See rcu_read_lock_bh() for more information.
// /*AFLA*/  */
// /*AFLA*/ static inline void rcu_read_unlock_bh(void)
// /*AFLA*/ {
// /*AFLA*/ 	RCU_LOCKDEP_WARN(!rcu_is_watching(),
// /*AFLA*/ 			 "rcu_read_unlock_bh() used illegally while idle");
// /*AFLA*/ 	rcu_lock_release(&rcu_bh_lock_map);
// /*AFLA*/ 	__release(RCU_BH);
// /*AFLA*/ 	local_bh_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rcu_read_lock_sched() - mark the beginning of a RCU-sched critical section
// /*AFLA*/  *
// /*AFLA*/  * This is equivalent of rcu_read_lock(), but to be used when updates
// /*AFLA*/  * are being done using call_rcu_sched() or synchronize_rcu_sched().
// /*AFLA*/  * Read-side critical sections can also be introduced by anything that
// /*AFLA*/  * disables preemption, including local_irq_disable() and friends.
// /*AFLA*/  *
// /*AFLA*/  * Note that rcu_read_lock_sched() and the matching rcu_read_unlock_sched()
// /*AFLA*/  * must occur in the same context, for example, it is illegal to invoke
// /*AFLA*/  * rcu_read_unlock_sched() from process context if the matching
// /*AFLA*/  * rcu_read_lock_sched() was invoked from an NMI handler.
// /*AFLA*/  */
// /*AFLA*/ static inline void rcu_read_lock_sched(void)
// /*AFLA*/ {
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	__acquire(RCU_SCHED);
// /*AFLA*/ 	rcu_lock_acquire(&rcu_sched_lock_map);
// /*AFLA*/ 	RCU_LOCKDEP_WARN(!rcu_is_watching(),
// /*AFLA*/ 			 "rcu_read_lock_sched() used illegally while idle");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Used by lockdep and tracing: cannot be traced, cannot call lockdep. */
// /*AFLA*/ static inline notrace void rcu_read_lock_sched_notrace(void)
// /*AFLA*/ {
// /*AFLA*/ 	preempt_disable_notrace();
// /*AFLA*/ 	__acquire(RCU_SCHED);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * rcu_read_unlock_sched - marks the end of a RCU-classic critical section
// /*AFLA*/  *
// /*AFLA*/  * See rcu_read_lock_sched for more information.
// /*AFLA*/  */
// /*AFLA*/ static inline void rcu_read_unlock_sched(void)
// /*AFLA*/ {
// /*AFLA*/ 	RCU_LOCKDEP_WARN(!rcu_is_watching(),
// /*AFLA*/ 			 "rcu_read_unlock_sched() used illegally while idle");
// /*AFLA*/ 	rcu_lock_release(&rcu_sched_lock_map);
// /*AFLA*/ 	__release(RCU_SCHED);
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Used by lockdep and tracing: cannot be traced, cannot call lockdep. */
// /*AFLA*/ static inline notrace void rcu_read_unlock_sched_notrace(void)
// /*AFLA*/ {
// /*AFLA*/ 	__release(RCU_SCHED);
// /*AFLA*/ 	preempt_enable_notrace();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * RCU_INIT_POINTER() - initialize an RCU protected pointer
// /*AFLA*/  *
// /*AFLA*/  * Initialize an RCU-protected pointer in special cases where readers
// /*AFLA*/  * do not need ordering constraints on the CPU or the compiler.  These
// /*AFLA*/  * special cases are:
// /*AFLA*/  *
// /*AFLA*/  * 1.	This use of RCU_INIT_POINTER() is NULLing out the pointer -or-
// /*AFLA*/  * 2.	The caller has taken whatever steps are required to prevent
// /*AFLA*/  *	RCU readers from concurrently accessing this pointer -or-
// /*AFLA*/  * 3.	The referenced data structure has already been exposed to
// /*AFLA*/  *	readers either at compile time or via rcu_assign_pointer() -and-
// /*AFLA*/  *	a.	You have not made -any- reader-visible changes to
// /*AFLA*/  *		this structure since then -or-
// /*AFLA*/  *	b.	It is OK for readers accessing this structure from its
// /*AFLA*/  *		new location to see the old state of the structure.  (For
// /*AFLA*/  *		example, the changes were to statistical counters or to
// /*AFLA*/  *		other state where exact synchronization is not required.)
// /*AFLA*/  *
// /*AFLA*/  * Failure to follow these rules governing use of RCU_INIT_POINTER() will
// /*AFLA*/  * result in impossible-to-diagnose memory corruption.  As in the structures
// /*AFLA*/  * will look OK in crash dumps, but any concurrent RCU readers might
// /*AFLA*/  * see pre-initialized values of the referenced data structure.  So
// /*AFLA*/  * please be very careful how you use RCU_INIT_POINTER()!!!
// /*AFLA*/  *
// /*AFLA*/  * If you are creating an RCU-protected linked structure that is accessed
// /*AFLA*/  * by a single external-to-structure RCU-protected pointer, then you may
// /*AFLA*/  * use RCU_INIT_POINTER() to initialize the internal RCU-protected
// /*AFLA*/  * pointers, but you must use rcu_assign_pointer() to initialize the
// /*AFLA*/  * external-to-structure pointer -after- you have completely initialized
// /*AFLA*/  * the reader-accessible portions of the linked structure.
// /*AFLA*/  *
// /*AFLA*/  * Note that unlike rcu_assign_pointer(), RCU_INIT_POINTER() provides no
// /*AFLA*/  * ordering guarantees for either the CPU or the compiler.
// /*AFLA*/  */
// /*AFLA*/ #define RCU_INIT_POINTER(p, v) \
// /*AFLA*/ 	do { \
// /*AFLA*/ 		rcu_dereference_sparse(p, __rcu); \
// /*AFLA*/ 		WRITE_ONCE(p, RCU_INITIALIZER(v)); \
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * RCU_POINTER_INITIALIZER() - statically initialize an RCU protected pointer
// /*AFLA*/  *
// /*AFLA*/  * GCC-style initialization for an RCU-protected pointer in a structure field.
// /*AFLA*/  */
// /*AFLA*/ #define RCU_POINTER_INITIALIZER(p, v) \
// /*AFLA*/ 		.p = RCU_INITIALIZER(v)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Does the specified offset indicate that the corresponding rcu_head
// /*AFLA*/  * structure can be handled by kfree_rcu()?
// /*AFLA*/  */
// /*AFLA*/ #define __is_kfree_rcu_offset(offset) ((offset) < 4096)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Helper macro for kfree_rcu() to prevent argument-expansion eyestrain.
// /*AFLA*/  */
// /*AFLA*/ #define __kfree_rcu(head, offset) \
// /*AFLA*/ 	do { \
// /*AFLA*/ 		BUILD_BUG_ON(!__is_kfree_rcu_offset(offset)); \
// /*AFLA*/ 		kfree_call_rcu(head, (rcu_callback_t)(unsigned long)(offset)); \
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kfree_rcu() - kfree an object after a grace period.
// /*AFLA*/  * @ptr:	pointer to kfree
// /*AFLA*/  * @rcu_head:	the name of the struct rcu_head within the type of @ptr.
// /*AFLA*/  *
// /*AFLA*/  * Many rcu callbacks functions just call kfree() on the base structure.
// /*AFLA*/  * These functions are trivial, but their size adds up, and furthermore
// /*AFLA*/  * when they are used in a kernel module, that module must invoke the
// /*AFLA*/  * high-latency rcu_barrier() function at module-unload time.
// /*AFLA*/  *
// /*AFLA*/  * The kfree_rcu() function handles this issue.  Rather than encoding a
// /*AFLA*/  * function address in the embedded rcu_head structure, kfree_rcu() instead
// /*AFLA*/  * encodes the offset of the rcu_head structure within the base structure.
// /*AFLA*/  * Because the functions are not allowed in the low-order 4096 bytes of
// /*AFLA*/  * kernel virtual memory, offsets up to 4095 bytes can be accommodated.
// /*AFLA*/  * If the offset is larger than 4095 bytes, a compile-time error will
// /*AFLA*/  * be generated in __kfree_rcu().  If this error is triggered, you can
// /*AFLA*/  * either fall back to use of call_rcu() or rearrange the structure to
// /*AFLA*/  * position the rcu_head structure into the first 4096 bytes.
// /*AFLA*/  *
// /*AFLA*/  * Note that the allowable offset might decrease in the future, for example,
// /*AFLA*/  * to allow something like kmem_cache_free_rcu().
// /*AFLA*/  *
// /*AFLA*/  * The BUILD_BUG_ON check must not involve any function calls, hence the
// /*AFLA*/  * checks are done in macros here.
// /*AFLA*/  */
// /*AFLA*/ #define kfree_rcu(ptr, rcu_head)					\
// /*AFLA*/ 	__kfree_rcu(&((ptr)->rcu_head), offsetof(typeof(*(ptr)), rcu_head))
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TINY_RCU
// /*AFLA*/ static inline int rcu_needs_cpu(u64 basemono, u64 *nextevt)
// /*AFLA*/ {
// /*AFLA*/ 	*nextevt = KTIME_MAX;
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif /* #ifdef CONFIG_TINY_RCU */
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_RCU_NOCB_CPU_ALL)
// /*AFLA*/ static inline bool rcu_is_nocb_cpu(int cpu) { return true; }
// /*AFLA*/ #elif defined(CONFIG_RCU_NOCB_CPU)
// /*AFLA*/ bool rcu_is_nocb_cpu(int cpu);
// /*AFLA*/ #else
// /*AFLA*/ static inline bool rcu_is_nocb_cpu(int cpu) { return false; }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* Only for use by adaptive-ticks code. */
// /*AFLA*/ #ifdef CONFIG_NO_HZ_FULL_SYSIDLE
// /*AFLA*/ bool rcu_sys_is_idle(void);
// /*AFLA*/ void rcu_sysidle_force_exit(void);
// /*AFLA*/ #else /* #ifdef CONFIG_NO_HZ_FULL_SYSIDLE */
// /*AFLA*/ 
// /*AFLA*/ static inline bool rcu_sys_is_idle(void)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void rcu_sysidle_force_exit(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* #else #ifdef CONFIG_NO_HZ_FULL_SYSIDLE */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Dump the ftrace buffer, but only one time per callsite per boot.
// /*AFLA*/  */
// /*AFLA*/ #define rcu_ftrace_dump(oops_dump_mode) \
// /*AFLA*/ do { \
// /*AFLA*/ 	static atomic_t ___rfd_beenhere = ATOMIC_INIT(0); \
// /*AFLA*/ 	\
// /*AFLA*/ 	if (!atomic_read(&___rfd_beenhere) && \
// /*AFLA*/ 	    !atomic_xchg(&___rfd_beenhere, 1)) \
// /*AFLA*/ 		ftrace_dump(oops_dump_mode); \
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_RCUPDATE_H */
