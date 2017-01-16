// /*AFLA*/ /*
// /*AFLA*/  * Sleepable Read-Copy Update mechanism for mutual exclusion
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
// /*AFLA*/  * Copyright (C) IBM Corporation, 2006
// /*AFLA*/  * Copyright (C) Fujitsu, 2012
// /*AFLA*/  *
// /*AFLA*/  * Author: Paul McKenney <paulmck@us.ibm.com>
// /*AFLA*/  *	   Lai Jiangshan <laijs@cn.fujitsu.com>
// /*AFLA*/  *
// /*AFLA*/  * For detailed explanation of Read-Copy Update mechanism see -
// /*AFLA*/  * 		Documentation/RCU/ *.txt
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_SRCU_H
// /*AFLA*/ #define _LINUX_SRCU_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/mutex.h>
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ #include <linux/workqueue.h>
// /*AFLA*/ 
// /*AFLA*/ struct srcu_struct_array {
// /*AFLA*/ 	unsigned long c[2];
// /*AFLA*/ 	unsigned long seq[2];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct rcu_batch {
// /*AFLA*/ 	struct rcu_head *head, **tail;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define RCU_BATCH_INIT(name) { NULL, &(name.head) }
// /*AFLA*/ 
// /*AFLA*/ struct srcu_struct {
// /*AFLA*/ 	unsigned long completed;
// /*AFLA*/ 	struct srcu_struct_array __percpu *per_cpu_ref;
// /*AFLA*/ 	spinlock_t queue_lock; /* protect ->batch_queue, ->running */
// /*AFLA*/ 	bool running;
// /*AFLA*/ 	/* callbacks just queued */
// /*AFLA*/ 	struct rcu_batch batch_queue;
// /*AFLA*/ 	/* callbacks try to do the first check_zero */
// /*AFLA*/ 	struct rcu_batch batch_check0;
// /*AFLA*/ 	/* callbacks done with the first check_zero and the flip */
// /*AFLA*/ 	struct rcu_batch batch_check1;
// /*AFLA*/ 	struct rcu_batch batch_done;
// /*AFLA*/ 	struct delayed_work work;
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ 	struct lockdep_map dep_map;
// /*AFLA*/ #endif /* #ifdef CONFIG_DEBUG_LOCK_ALLOC */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ 
// /*AFLA*/ int __init_srcu_struct(struct srcu_struct *sp, const char *name,
// /*AFLA*/ 		       struct lock_class_key *key);
// /*AFLA*/ 
// /*AFLA*/ #define init_srcu_struct(sp) \
// /*AFLA*/ ({ \
// /*AFLA*/ 	static struct lock_class_key __srcu_key; \
// /*AFLA*/ 	\
// /*AFLA*/ 	__init_srcu_struct((sp), #sp, &__srcu_key); \
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __SRCU_DEP_MAP_INIT(srcu_name)	.dep_map = { .name = #srcu_name },
// /*AFLA*/ #else /* #ifdef CONFIG_DEBUG_LOCK_ALLOC */
// /*AFLA*/ 
// /*AFLA*/ int init_srcu_struct(struct srcu_struct *sp);
// /*AFLA*/ 
// /*AFLA*/ #define __SRCU_DEP_MAP_INIT(srcu_name)
// /*AFLA*/ #endif /* #else #ifdef CONFIG_DEBUG_LOCK_ALLOC */
// /*AFLA*/ 
// /*AFLA*/ void process_srcu(struct work_struct *work);
// /*AFLA*/ 
// /*AFLA*/ #define __SRCU_STRUCT_INIT(name)					\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		.completed = -300,					\
// /*AFLA*/ 		.per_cpu_ref = &name##_srcu_array,			\
// /*AFLA*/ 		.queue_lock = __SPIN_LOCK_UNLOCKED(name.queue_lock),	\
// /*AFLA*/ 		.running = false,					\
// /*AFLA*/ 		.batch_queue = RCU_BATCH_INIT(name.batch_queue),	\
// /*AFLA*/ 		.batch_check0 = RCU_BATCH_INIT(name.batch_check0),	\
// /*AFLA*/ 		.batch_check1 = RCU_BATCH_INIT(name.batch_check1),	\
// /*AFLA*/ 		.batch_done = RCU_BATCH_INIT(name.batch_done),		\
// /*AFLA*/ 		.work = __DELAYED_WORK_INITIALIZER(name.work, process_srcu, 0),\
// /*AFLA*/ 		__SRCU_DEP_MAP_INIT(name)				\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Define and initialize a srcu struct at build time.
// /*AFLA*/  * Do -not- call init_srcu_struct() nor cleanup_srcu_struct() on it.
// /*AFLA*/  *
// /*AFLA*/  * Note that although DEFINE_STATIC_SRCU() hides the name from other
// /*AFLA*/  * files, the per-CPU variable rules nevertheless require that the
// /*AFLA*/  * chosen name be globally unique.  These rules also prohibit use of
// /*AFLA*/  * DEFINE_STATIC_SRCU() within a function.  If these rules are too
// /*AFLA*/  * restrictive, declare the srcu_struct manually.  For example, in
// /*AFLA*/  * each file:
// /*AFLA*/  *
// /*AFLA*/  *	static struct srcu_struct my_srcu;
// /*AFLA*/  *
// /*AFLA*/  * Then, before the first use of each my_srcu, manually initialize it:
// /*AFLA*/  *
// /*AFLA*/  *	init_srcu_struct(&my_srcu);
// /*AFLA*/  *
// /*AFLA*/  * See include/linux/percpu-defs.h for the rules on per-CPU variables.
// /*AFLA*/  */
// /*AFLA*/ #define __DEFINE_SRCU(name, is_static)					\
// /*AFLA*/ 	static DEFINE_PER_CPU(struct srcu_struct_array, name##_srcu_array);\
// /*AFLA*/ 	is_static struct srcu_struct name = __SRCU_STRUCT_INIT(name)
// /*AFLA*/ #define DEFINE_SRCU(name)		__DEFINE_SRCU(name, /* not static */)
// /*AFLA*/ #define DEFINE_STATIC_SRCU(name)	__DEFINE_SRCU(name, static)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * call_srcu() - Queue a callback for invocation after an SRCU grace period
// /*AFLA*/  * @sp: srcu_struct in queue the callback
// /*AFLA*/  * @head: structure to be used for queueing the SRCU callback.
// /*AFLA*/  * @func: function to be invoked after the SRCU grace period
// /*AFLA*/  *
// /*AFLA*/  * The callback function will be invoked some time after a full SRCU
// /*AFLA*/  * grace period elapses, in other words after all pre-existing SRCU
// /*AFLA*/  * read-side critical sections have completed.  However, the callback
// /*AFLA*/  * function might well execute concurrently with other SRCU read-side
// /*AFLA*/  * critical sections that started after call_srcu() was invoked.  SRCU
// /*AFLA*/  * read-side critical sections are delimited by srcu_read_lock() and
// /*AFLA*/  * srcu_read_unlock(), and may be nested.
// /*AFLA*/  *
// /*AFLA*/  * The callback will be invoked from process context, but must nevertheless
// /*AFLA*/  * be fast and must not block.
// /*AFLA*/  */
// /*AFLA*/ void call_srcu(struct srcu_struct *sp, struct rcu_head *head,
// /*AFLA*/ 		void (*func)(struct rcu_head *head));
// /*AFLA*/ 
// /*AFLA*/ void cleanup_srcu_struct(struct srcu_struct *sp);
// /*AFLA*/ int __srcu_read_lock(struct srcu_struct *sp) __acquires(sp);
// /*AFLA*/ void __srcu_read_unlock(struct srcu_struct *sp, int idx) __releases(sp);
// /*AFLA*/ void synchronize_srcu(struct srcu_struct *sp);
// /*AFLA*/ void synchronize_srcu_expedited(struct srcu_struct *sp);
// /*AFLA*/ unsigned long srcu_batches_completed(struct srcu_struct *sp);
// /*AFLA*/ void srcu_barrier(struct srcu_struct *sp);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * srcu_read_lock_held - might we be in SRCU read-side critical section?
// /*AFLA*/  *
// /*AFLA*/  * If CONFIG_DEBUG_LOCK_ALLOC is selected, returns nonzero iff in an SRCU
// /*AFLA*/  * read-side critical section.  In absence of CONFIG_DEBUG_LOCK_ALLOC,
// /*AFLA*/  * this assumes we are in an SRCU read-side critical section unless it can
// /*AFLA*/  * prove otherwise.
// /*AFLA*/  *
// /*AFLA*/  * Checks debug_lockdep_rcu_enabled() to prevent false positives during boot
// /*AFLA*/  * and while lockdep is disabled.
// /*AFLA*/  *
// /*AFLA*/  * Note that SRCU is based on its own statemachine and it doesn't
// /*AFLA*/  * relies on normal RCU, it can be called from the CPU which
// /*AFLA*/  * is in the idle loop from an RCU point of view or offline.
// /*AFLA*/  */
// /*AFLA*/ static inline int srcu_read_lock_held(struct srcu_struct *sp)
// /*AFLA*/ {
// /*AFLA*/ 	if (!debug_lockdep_rcu_enabled())
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	return lock_is_held(&sp->dep_map);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else /* #ifdef CONFIG_DEBUG_LOCK_ALLOC */
// /*AFLA*/ 
// /*AFLA*/ static inline int srcu_read_lock_held(struct srcu_struct *sp)
// /*AFLA*/ {
// /*AFLA*/ 	return 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* #else #ifdef CONFIG_DEBUG_LOCK_ALLOC */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * srcu_dereference_check - fetch SRCU-protected pointer for later dereferencing
// /*AFLA*/  * @p: the pointer to fetch and protect for later dereferencing
// /*AFLA*/  * @sp: pointer to the srcu_struct, which is used to check that we
// /*AFLA*/  *	really are in an SRCU read-side critical section.
// /*AFLA*/  * @c: condition to check for update-side use
// /*AFLA*/  *
// /*AFLA*/  * If PROVE_RCU is enabled, invoking this outside of an RCU read-side
// /*AFLA*/  * critical section will result in an RCU-lockdep splat, unless @c evaluates
// /*AFLA*/  * to 1.  The @c argument will normally be a logical expression containing
// /*AFLA*/  * lockdep_is_held() calls.
// /*AFLA*/  */
// /*AFLA*/ #define srcu_dereference_check(p, sp, c) \
// /*AFLA*/ 	__rcu_dereference_check((p), (c) || srcu_read_lock_held(sp), __rcu)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * srcu_dereference - fetch SRCU-protected pointer for later dereferencing
// /*AFLA*/  * @p: the pointer to fetch and protect for later dereferencing
// /*AFLA*/  * @sp: pointer to the srcu_struct, which is used to check that we
// /*AFLA*/  *	really are in an SRCU read-side critical section.
// /*AFLA*/  *
// /*AFLA*/  * Makes rcu_dereference_check() do the dirty work.  If PROVE_RCU
// /*AFLA*/  * is enabled, invoking this outside of an RCU read-side critical
// /*AFLA*/  * section will result in an RCU-lockdep splat.
// /*AFLA*/  */
// /*AFLA*/ #define srcu_dereference(p, sp) srcu_dereference_check((p), (sp), 0)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * srcu_read_lock - register a new reader for an SRCU-protected structure.
// /*AFLA*/  * @sp: srcu_struct in which to register the new reader.
// /*AFLA*/  *
// /*AFLA*/  * Enter an SRCU read-side critical section.  Note that SRCU read-side
// /*AFLA*/  * critical sections may be nested.  However, it is illegal to
// /*AFLA*/  * call anything that waits on an SRCU grace period for the same
// /*AFLA*/  * srcu_struct, whether directly or indirectly.  Please note that
// /*AFLA*/  * one way to indirectly wait on an SRCU grace period is to acquire
// /*AFLA*/  * a mutex that is held elsewhere while calling synchronize_srcu() or
// /*AFLA*/  * synchronize_srcu_expedited().
// /*AFLA*/  *
// /*AFLA*/  * Note that srcu_read_lock() and the matching srcu_read_unlock() must
// /*AFLA*/  * occur in the same context, for example, it is illegal to invoke
// /*AFLA*/  * srcu_read_unlock() in an irq handler if the matching srcu_read_lock()
// /*AFLA*/  * was invoked in process context.
// /*AFLA*/  */
// /*AFLA*/ static inline int srcu_read_lock(struct srcu_struct *sp) __acquires(sp)
// /*AFLA*/ {
// /*AFLA*/ 	int retval;
// /*AFLA*/ 
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	retval = __srcu_read_lock(sp);
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ 	rcu_lock_acquire(&(sp)->dep_map);
// /*AFLA*/ 	return retval;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * srcu_read_unlock - unregister a old reader from an SRCU-protected structure.
// /*AFLA*/  * @sp: srcu_struct in which to unregister the old reader.
// /*AFLA*/  * @idx: return value from corresponding srcu_read_lock().
// /*AFLA*/  *
// /*AFLA*/  * Exit an SRCU read-side critical section.
// /*AFLA*/  */
// /*AFLA*/ static inline void srcu_read_unlock(struct srcu_struct *sp, int idx)
// /*AFLA*/ 	__releases(sp)
// /*AFLA*/ {
// /*AFLA*/ 	rcu_lock_release(&(sp)->dep_map);
// /*AFLA*/ 	__srcu_read_unlock(sp, idx);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * smp_mb__after_srcu_read_unlock - ensure full ordering after srcu_read_unlock
// /*AFLA*/  *
// /*AFLA*/  * Converts the preceding srcu_read_unlock into a two-way memory barrier.
// /*AFLA*/  *
// /*AFLA*/  * Call this after srcu_read_unlock, to guarantee that all memory operations
// /*AFLA*/  * that occur after smp_mb__after_srcu_read_unlock will appear to happen after
// /*AFLA*/  * the preceding srcu_read_unlock.
// /*AFLA*/  */
// /*AFLA*/ static inline void smp_mb__after_srcu_read_unlock(void)
// /*AFLA*/ {
// /*AFLA*/ 	/* __srcu_read_unlock has smp_mb() internally so nothing to do here. */
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
