// /*AFLA*/ /*
// /*AFLA*/  * Read-Copy Update mechanism for mutual exclusion (tree-based version)
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
// /*AFLA*/  * Copyright IBM Corporation, 2008
// /*AFLA*/  *
// /*AFLA*/  * Author: Dipankar Sarma <dipankar@in.ibm.com>
// /*AFLA*/  *	   Paul E. McKenney <paulmck@linux.vnet.ibm.com> Hierarchical algorithm
// /*AFLA*/  *
// /*AFLA*/  * Based on the original work by Paul McKenney <paulmck@us.ibm.com>
// /*AFLA*/  * and inputs from Rusty Russell, Andrea Arcangeli and Andi Kleen.
// /*AFLA*/  *
// /*AFLA*/  * For detailed explanation of Read-Copy Update mechanism see -
// /*AFLA*/  *	Documentation/RCU
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __LINUX_RCUTREE_H
// /*AFLA*/ #define __LINUX_RCUTREE_H
// /*AFLA*/ 
// /*AFLA*/ void rcu_note_context_switch(void);
// /*AFLA*/ int rcu_needs_cpu(u64 basem, u64 *nextevt);
// /*AFLA*/ void rcu_cpu_stall_reset(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Note a virtualization-based context switch.  This is simply a
// /*AFLA*/  * wrapper around rcu_note_context_switch(), which allows TINY_RCU
// /*AFLA*/  * to save a few bytes. The caller must have disabled interrupts.
// /*AFLA*/  */
// /*AFLA*/ static inline void rcu_virt_note_context_switch(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	rcu_note_context_switch();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void synchronize_rcu_bh(void);
// /*AFLA*/ void synchronize_sched_expedited(void);
// /*AFLA*/ void synchronize_rcu_expedited(void);
// /*AFLA*/ 
// /*AFLA*/ void kfree_call_rcu(struct rcu_head *head, rcu_callback_t func);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * synchronize_rcu_bh_expedited - Brute-force RCU-bh grace period
// /*AFLA*/  *
// /*AFLA*/  * Wait for an RCU-bh grace period to elapse, but use a "big hammer"
// /*AFLA*/  * approach to force the grace period to end quickly.  This consumes
// /*AFLA*/  * significant time on all CPUs and is unfriendly to real-time workloads,
// /*AFLA*/  * so is thus not recommended for any sort of common-case code.  In fact,
// /*AFLA*/  * if you are using synchronize_rcu_bh_expedited() in a loop, please
// /*AFLA*/  * restructure your code to batch your updates, and then use a single
// /*AFLA*/  * synchronize_rcu_bh() instead.
// /*AFLA*/  *
// /*AFLA*/  * Note that it is illegal to call this function while holding any lock
// /*AFLA*/  * that is acquired by a CPU-hotplug notifier.  And yes, it is also illegal
// /*AFLA*/  * to call this function from a CPU-hotplug notifier.  Failing to observe
// /*AFLA*/  * these restriction will result in deadlock.
// /*AFLA*/  */
// /*AFLA*/ static inline void synchronize_rcu_bh_expedited(void)
// /*AFLA*/ {
// /*AFLA*/ 	synchronize_sched_expedited();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void rcu_barrier(void);
// /*AFLA*/ void rcu_barrier_bh(void);
// /*AFLA*/ void rcu_barrier_sched(void);
// /*AFLA*/ unsigned long get_state_synchronize_rcu(void);
// /*AFLA*/ void cond_synchronize_rcu(unsigned long oldstate);
// /*AFLA*/ unsigned long get_state_synchronize_sched(void);
// /*AFLA*/ void cond_synchronize_sched(unsigned long oldstate);
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long rcutorture_testseq;
// /*AFLA*/ extern unsigned long rcutorture_vernum;
// /*AFLA*/ unsigned long rcu_batches_started(void);
// /*AFLA*/ unsigned long rcu_batches_started_bh(void);
// /*AFLA*/ unsigned long rcu_batches_started_sched(void);
// /*AFLA*/ unsigned long rcu_batches_completed(void);
// /*AFLA*/ unsigned long rcu_batches_completed_bh(void);
// /*AFLA*/ unsigned long rcu_batches_completed_sched(void);
// /*AFLA*/ unsigned long rcu_exp_batches_completed(void);
// /*AFLA*/ unsigned long rcu_exp_batches_completed_sched(void);
// /*AFLA*/ void show_rcu_gp_kthreads(void);
// /*AFLA*/ 
// /*AFLA*/ void rcu_force_quiescent_state(void);
// /*AFLA*/ void rcu_bh_force_quiescent_state(void);
// /*AFLA*/ void rcu_sched_force_quiescent_state(void);
// /*AFLA*/ 
// /*AFLA*/ void rcu_idle_enter(void);
// /*AFLA*/ void rcu_idle_exit(void);
// /*AFLA*/ void rcu_irq_enter(void);
// /*AFLA*/ void rcu_irq_exit(void);
// /*AFLA*/ void rcu_irq_enter_irqson(void);
// /*AFLA*/ void rcu_irq_exit_irqson(void);
// /*AFLA*/ 
// /*AFLA*/ void exit_rcu(void);
// /*AFLA*/ 
// /*AFLA*/ void rcu_scheduler_starting(void);
// /*AFLA*/ extern int rcu_scheduler_active __read_mostly;
// /*AFLA*/ 
// /*AFLA*/ bool rcu_is_watching(void);
// /*AFLA*/ 
// /*AFLA*/ void rcu_all_qs(void);
// /*AFLA*/ 
// /*AFLA*/ /* RCUtree hotplug events */
// /*AFLA*/ int rcutree_prepare_cpu(unsigned int cpu);
// /*AFLA*/ int rcutree_online_cpu(unsigned int cpu);
// /*AFLA*/ int rcutree_offline_cpu(unsigned int cpu);
// /*AFLA*/ int rcutree_dead_cpu(unsigned int cpu);
// /*AFLA*/ int rcutree_dying_cpu(unsigned int cpu);
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_RCUTREE_H */
