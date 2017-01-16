// /*AFLA*/ #ifndef _LINUX_CGROUP_H
// /*AFLA*/ #define _LINUX_CGROUP_H
// /*AFLA*/ /*
// /*AFLA*/  *  cgroup interface
// /*AFLA*/  *
// /*AFLA*/  *  Copyright (C) 2003 BULL SA
// /*AFLA*/  *  Copyright (C) 2004-2006 Silicon Graphics, Inc.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/sched.h>
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ #include <linux/nodemask.h>
// /*AFLA*/ #include <linux/rculist.h>
// /*AFLA*/ #include <linux/cgroupstats.h>
// /*AFLA*/ #include <linux/fs.h>
// /*AFLA*/ #include <linux/seq_file.h>
// /*AFLA*/ #include <linux/kernfs.h>
// /*AFLA*/ #include <linux/jump_label.h>
// /*AFLA*/ #include <linux/nsproxy.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/ns_common.h>
// /*AFLA*/ #include <linux/nsproxy.h>
// /*AFLA*/ #include <linux/user_namespace.h>
// /*AFLA*/ 
// /*AFLA*/ #include <linux/cgroup-defs.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_CGROUPS
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * All weight knobs on the default hierarhcy should use the following min,
// /*AFLA*/  * default and max values.  The default value is the logarithmic center of
// /*AFLA*/  * MIN and MAX and allows 100x to be expressed in both directions.
// /*AFLA*/  */
// /*AFLA*/ #define CGROUP_WEIGHT_MIN		1
// /*AFLA*/ #define CGROUP_WEIGHT_DFL		100
// /*AFLA*/ #define CGROUP_WEIGHT_MAX		10000
// /*AFLA*/ 
// /*AFLA*/ /* a css_task_iter should be treated as an opaque object */
// /*AFLA*/ struct css_task_iter {
// /*AFLA*/ 	struct cgroup_subsys		*ss;
// /*AFLA*/ 
// /*AFLA*/ 	struct list_head		*cset_pos;
// /*AFLA*/ 	struct list_head		*cset_head;
// /*AFLA*/ 
// /*AFLA*/ 	struct list_head		*task_pos;
// /*AFLA*/ 	struct list_head		*tasks_head;
// /*AFLA*/ 	struct list_head		*mg_tasks_head;
// /*AFLA*/ 
// /*AFLA*/ 	struct css_set			*cur_cset;
// /*AFLA*/ 	struct task_struct		*cur_task;
// /*AFLA*/ 	struct list_head		iters_node;	/* css_set->task_iters */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct cgroup_root cgrp_dfl_root;
// /*AFLA*/ extern struct css_set init_css_set;
// /*AFLA*/ 
// /*AFLA*/ #define SUBSYS(_x) extern struct cgroup_subsys _x ## _cgrp_subsys;
// /*AFLA*/ #include <linux/cgroup_subsys.h>
// /*AFLA*/ #undef SUBSYS
// /*AFLA*/ 
// /*AFLA*/ #define SUBSYS(_x)								\
// /*AFLA*/ 	extern struct static_key_true _x ## _cgrp_subsys_enabled_key;		\
// /*AFLA*/ 	extern struct static_key_true _x ## _cgrp_subsys_on_dfl_key;
// /*AFLA*/ #include <linux/cgroup_subsys.h>
// /*AFLA*/ #undef SUBSYS
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cgroup_subsys_enabled - fast test on whether a subsys is enabled
// /*AFLA*/  * @ss: subsystem in question
// /*AFLA*/  */
// /*AFLA*/ #define cgroup_subsys_enabled(ss)						\
// /*AFLA*/ 	static_branch_likely(&ss ## _enabled_key)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cgroup_subsys_on_dfl - fast test on whether a subsys is on default hierarchy
// /*AFLA*/  * @ss: subsystem in question
// /*AFLA*/  */
// /*AFLA*/ #define cgroup_subsys_on_dfl(ss)						\
// /*AFLA*/ 	static_branch_likely(&ss ## _on_dfl_key)
// /*AFLA*/ 
// /*AFLA*/ bool css_has_online_children(struct cgroup_subsys_state *css);
// /*AFLA*/ struct cgroup_subsys_state *css_from_id(int id, struct cgroup_subsys *ss);
// /*AFLA*/ struct cgroup_subsys_state *cgroup_get_e_css(struct cgroup *cgroup,
// /*AFLA*/ 					     struct cgroup_subsys *ss);
// /*AFLA*/ struct cgroup_subsys_state *css_tryget_online_from_dir(struct dentry *dentry,
// /*AFLA*/ 						       struct cgroup_subsys *ss);
// /*AFLA*/ 
// /*AFLA*/ struct cgroup *cgroup_get_from_path(const char *path);
// /*AFLA*/ struct cgroup *cgroup_get_from_fd(int fd);
// /*AFLA*/ 
// /*AFLA*/ int cgroup_attach_task_all(struct task_struct *from, struct task_struct *);
// /*AFLA*/ int cgroup_transfer_tasks(struct cgroup *to, struct cgroup *from);
// /*AFLA*/ 
// /*AFLA*/ int cgroup_add_dfl_cftypes(struct cgroup_subsys *ss, struct cftype *cfts);
// /*AFLA*/ int cgroup_add_legacy_cftypes(struct cgroup_subsys *ss, struct cftype *cfts);
// /*AFLA*/ int cgroup_rm_cftypes(struct cftype *cfts);
// /*AFLA*/ void cgroup_file_notify(struct cgroup_file *cfile);
// /*AFLA*/ 
// /*AFLA*/ int task_cgroup_path(struct task_struct *task, char *buf, size_t buflen);
// /*AFLA*/ int cgroupstats_build(struct cgroupstats *stats, struct dentry *dentry);
// /*AFLA*/ int proc_cgroup_show(struct seq_file *m, struct pid_namespace *ns,
// /*AFLA*/ 		     struct pid *pid, struct task_struct *tsk);
// /*AFLA*/ 
// /*AFLA*/ void cgroup_fork(struct task_struct *p);
// /*AFLA*/ extern int cgroup_can_fork(struct task_struct *p);
// /*AFLA*/ extern void cgroup_cancel_fork(struct task_struct *p);
// /*AFLA*/ extern void cgroup_post_fork(struct task_struct *p);
// /*AFLA*/ void cgroup_exit(struct task_struct *p);
// /*AFLA*/ void cgroup_free(struct task_struct *p);
// /*AFLA*/ 
// /*AFLA*/ int cgroup_init_early(void);
// /*AFLA*/ int cgroup_init(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Iteration helpers and macros.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct cgroup_subsys_state *css_next_child(struct cgroup_subsys_state *pos,
// /*AFLA*/ 					   struct cgroup_subsys_state *parent);
// /*AFLA*/ struct cgroup_subsys_state *css_next_descendant_pre(struct cgroup_subsys_state *pos,
// /*AFLA*/ 						    struct cgroup_subsys_state *css);
// /*AFLA*/ struct cgroup_subsys_state *css_rightmost_descendant(struct cgroup_subsys_state *pos);
// /*AFLA*/ struct cgroup_subsys_state *css_next_descendant_post(struct cgroup_subsys_state *pos,
// /*AFLA*/ 						     struct cgroup_subsys_state *css);
// /*AFLA*/ 
// /*AFLA*/ struct task_struct *cgroup_taskset_first(struct cgroup_taskset *tset,
// /*AFLA*/ 					 struct cgroup_subsys_state **dst_cssp);
// /*AFLA*/ struct task_struct *cgroup_taskset_next(struct cgroup_taskset *tset,
// /*AFLA*/ 					struct cgroup_subsys_state **dst_cssp);
// /*AFLA*/ 
// /*AFLA*/ void css_task_iter_start(struct cgroup_subsys_state *css,
// /*AFLA*/ 			 struct css_task_iter *it);
// /*AFLA*/ struct task_struct *css_task_iter_next(struct css_task_iter *it);
// /*AFLA*/ void css_task_iter_end(struct css_task_iter *it);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * css_for_each_child - iterate through children of a css
// /*AFLA*/  * @pos: the css * to use as the loop cursor
// /*AFLA*/  * @parent: css whose children to walk
// /*AFLA*/  *
// /*AFLA*/  * Walk @parent's children.  Must be called under rcu_read_lock().
// /*AFLA*/  *
// /*AFLA*/  * If a subsystem synchronizes ->css_online() and the start of iteration, a
// /*AFLA*/  * css which finished ->css_online() is guaranteed to be visible in the
// /*AFLA*/  * future iterations and will stay visible until the last reference is put.
// /*AFLA*/  * A css which hasn't finished ->css_online() or already finished
// /*AFLA*/  * ->css_offline() may show up during traversal.  It's each subsystem's
// /*AFLA*/  * responsibility to synchronize against on/offlining.
// /*AFLA*/  *
// /*AFLA*/  * It is allowed to temporarily drop RCU read lock during iteration.  The
// /*AFLA*/  * caller is responsible for ensuring that @pos remains accessible until
// /*AFLA*/  * the start of the next iteration by, for example, bumping the css refcnt.
// /*AFLA*/  */
// /*AFLA*/ #define css_for_each_child(pos, parent)					\
// /*AFLA*/ 	for ((pos) = css_next_child(NULL, (parent)); (pos);		\
// /*AFLA*/ 	     (pos) = css_next_child((pos), (parent)))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * css_for_each_descendant_pre - pre-order walk of a css's descendants
// /*AFLA*/  * @pos: the css * to use as the loop cursor
// /*AFLA*/  * @root: css whose descendants to walk
// /*AFLA*/  *
// /*AFLA*/  * Walk @root's descendants.  @root is included in the iteration and the
// /*AFLA*/  * first node to be visited.  Must be called under rcu_read_lock().
// /*AFLA*/  *
// /*AFLA*/  * If a subsystem synchronizes ->css_online() and the start of iteration, a
// /*AFLA*/  * css which finished ->css_online() is guaranteed to be visible in the
// /*AFLA*/  * future iterations and will stay visible until the last reference is put.
// /*AFLA*/  * A css which hasn't finished ->css_online() or already finished
// /*AFLA*/  * ->css_offline() may show up during traversal.  It's each subsystem's
// /*AFLA*/  * responsibility to synchronize against on/offlining.
// /*AFLA*/  *
// /*AFLA*/  * For example, the following guarantees that a descendant can't escape
// /*AFLA*/  * state updates of its ancestors.
// /*AFLA*/  *
// /*AFLA*/  * my_online(@css)
// /*AFLA*/  * {
// /*AFLA*/  *	Lock @css's parent and @css;
// /*AFLA*/  *	Inherit state from the parent;
// /*AFLA*/  *	Unlock both.
// /*AFLA*/  * }
// /*AFLA*/  *
// /*AFLA*/  * my_update_state(@css)
// /*AFLA*/  * {
// /*AFLA*/  *	css_for_each_descendant_pre(@pos, @css) {
// /*AFLA*/  *		Lock @pos;
// /*AFLA*/  *		if (@pos == @css)
// /*AFLA*/  *			Update @css's state;
// /*AFLA*/  *		else
// /*AFLA*/  *			Verify @pos is alive and inherit state from its parent;
// /*AFLA*/  *		Unlock @pos;
// /*AFLA*/  *	}
// /*AFLA*/  * }
// /*AFLA*/  *
// /*AFLA*/  * As long as the inheriting step, including checking the parent state, is
// /*AFLA*/  * enclosed inside @pos locking, double-locking the parent isn't necessary
// /*AFLA*/  * while inheriting.  The state update to the parent is guaranteed to be
// /*AFLA*/  * visible by walking order and, as long as inheriting operations to the
// /*AFLA*/  * same @pos are atomic to each other, multiple updates racing each other
// /*AFLA*/  * still result in the correct state.  It's guaranateed that at least one
// /*AFLA*/  * inheritance happens for any css after the latest update to its parent.
// /*AFLA*/  *
// /*AFLA*/  * If checking parent's state requires locking the parent, each inheriting
// /*AFLA*/  * iteration should lock and unlock both @pos->parent and @pos.
// /*AFLA*/  *
// /*AFLA*/  * Alternatively, a subsystem may choose to use a single global lock to
// /*AFLA*/  * synchronize ->css_online() and ->css_offline() against tree-walking
// /*AFLA*/  * operations.
// /*AFLA*/  *
// /*AFLA*/  * It is allowed to temporarily drop RCU read lock during iteration.  The
// /*AFLA*/  * caller is responsible for ensuring that @pos remains accessible until
// /*AFLA*/  * the start of the next iteration by, for example, bumping the css refcnt.
// /*AFLA*/  */
// /*AFLA*/ #define css_for_each_descendant_pre(pos, css)				\
// /*AFLA*/ 	for ((pos) = css_next_descendant_pre(NULL, (css)); (pos);	\
// /*AFLA*/ 	     (pos) = css_next_descendant_pre((pos), (css)))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * css_for_each_descendant_post - post-order walk of a css's descendants
// /*AFLA*/  * @pos: the css * to use as the loop cursor
// /*AFLA*/  * @css: css whose descendants to walk
// /*AFLA*/  *
// /*AFLA*/  * Similar to css_for_each_descendant_pre() but performs post-order
// /*AFLA*/  * traversal instead.  @root is included in the iteration and the last
// /*AFLA*/  * node to be visited.
// /*AFLA*/  *
// /*AFLA*/  * If a subsystem synchronizes ->css_online() and the start of iteration, a
// /*AFLA*/  * css which finished ->css_online() is guaranteed to be visible in the
// /*AFLA*/  * future iterations and will stay visible until the last reference is put.
// /*AFLA*/  * A css which hasn't finished ->css_online() or already finished
// /*AFLA*/  * ->css_offline() may show up during traversal.  It's each subsystem's
// /*AFLA*/  * responsibility to synchronize against on/offlining.
// /*AFLA*/  *
// /*AFLA*/  * Note that the walk visibility guarantee example described in pre-order
// /*AFLA*/  * walk doesn't apply the same to post-order walks.
// /*AFLA*/  */
// /*AFLA*/ #define css_for_each_descendant_post(pos, css)				\
// /*AFLA*/ 	for ((pos) = css_next_descendant_post(NULL, (css)); (pos);	\
// /*AFLA*/ 	     (pos) = css_next_descendant_post((pos), (css)))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cgroup_taskset_for_each - iterate cgroup_taskset
// /*AFLA*/  * @task: the loop cursor
// /*AFLA*/  * @dst_css: the destination css
// /*AFLA*/  * @tset: taskset to iterate
// /*AFLA*/  *
// /*AFLA*/  * @tset may contain multiple tasks and they may belong to multiple
// /*AFLA*/  * processes.
// /*AFLA*/  *
// /*AFLA*/  * On the v2 hierarchy, there may be tasks from multiple processes and they
// /*AFLA*/  * may not share the source or destination csses.
// /*AFLA*/  *
// /*AFLA*/  * On traditional hierarchies, when there are multiple tasks in @tset, if a
// /*AFLA*/  * task of a process is in @tset, all tasks of the process are in @tset.
// /*AFLA*/  * Also, all are guaranteed to share the same source and destination csses.
// /*AFLA*/  *
// /*AFLA*/  * Iteration is not in any specific order.
// /*AFLA*/  */
// /*AFLA*/ #define cgroup_taskset_for_each(task, dst_css, tset)			\
// /*AFLA*/ 	for ((task) = cgroup_taskset_first((tset), &(dst_css));		\
// /*AFLA*/ 	     (task);							\
// /*AFLA*/ 	     (task) = cgroup_taskset_next((tset), &(dst_css)))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cgroup_taskset_for_each_leader - iterate group leaders in a cgroup_taskset
// /*AFLA*/  * @leader: the loop cursor
// /*AFLA*/  * @dst_css: the destination css
// /*AFLA*/  * @tset: takset to iterate
// /*AFLA*/  *
// /*AFLA*/  * Iterate threadgroup leaders of @tset.  For single-task migrations, @tset
// /*AFLA*/  * may not contain any.
// /*AFLA*/  */
// /*AFLA*/ #define cgroup_taskset_for_each_leader(leader, dst_css, tset)		\
// /*AFLA*/ 	for ((leader) = cgroup_taskset_first((tset), &(dst_css));	\
// /*AFLA*/ 	     (leader);							\
// /*AFLA*/ 	     (leader) = cgroup_taskset_next((tset), &(dst_css)))	\
// /*AFLA*/ 		if ((leader) != (leader)->group_leader)			\
// /*AFLA*/ 			;						\
// /*AFLA*/ 		else
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Inline functions.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * css_get - obtain a reference on the specified css
// /*AFLA*/  * @css: target css
// /*AFLA*/  *
// /*AFLA*/  * The caller must already have a reference.
// /*AFLA*/  */
// /*AFLA*/ static inline void css_get(struct cgroup_subsys_state *css)
// /*AFLA*/ {
// /*AFLA*/ 	if (!(css->flags & CSS_NO_REF))
// /*AFLA*/ 		percpu_ref_get(&css->refcnt);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * css_get_many - obtain references on the specified css
// /*AFLA*/  * @css: target css
// /*AFLA*/  * @n: number of references to get
// /*AFLA*/  *
// /*AFLA*/  * The caller must already have a reference.
// /*AFLA*/  */
// /*AFLA*/ static inline void css_get_many(struct cgroup_subsys_state *css, unsigned int n)
// /*AFLA*/ {
// /*AFLA*/ 	if (!(css->flags & CSS_NO_REF))
// /*AFLA*/ 		percpu_ref_get_many(&css->refcnt, n);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * css_tryget - try to obtain a reference on the specified css
// /*AFLA*/  * @css: target css
// /*AFLA*/  *
// /*AFLA*/  * Obtain a reference on @css unless it already has reached zero and is
// /*AFLA*/  * being released.  This function doesn't care whether @css is on or
// /*AFLA*/  * offline.  The caller naturally needs to ensure that @css is accessible
// /*AFLA*/  * but doesn't have to be holding a reference on it - IOW, RCU protected
// /*AFLA*/  * access is good enough for this function.  Returns %true if a reference
// /*AFLA*/  * count was successfully obtained; %false otherwise.
// /*AFLA*/  */
// /*AFLA*/ static inline bool css_tryget(struct cgroup_subsys_state *css)
// /*AFLA*/ {
// /*AFLA*/ 	if (!(css->flags & CSS_NO_REF))
// /*AFLA*/ 		return percpu_ref_tryget(&css->refcnt);
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * css_tryget_online - try to obtain a reference on the specified css if online
// /*AFLA*/  * @css: target css
// /*AFLA*/  *
// /*AFLA*/  * Obtain a reference on @css if it's online.  The caller naturally needs
// /*AFLA*/  * to ensure that @css is accessible but doesn't have to be holding a
// /*AFLA*/  * reference on it - IOW, RCU protected access is good enough for this
// /*AFLA*/  * function.  Returns %true if a reference count was successfully obtained;
// /*AFLA*/  * %false otherwise.
// /*AFLA*/  */
// /*AFLA*/ static inline bool css_tryget_online(struct cgroup_subsys_state *css)
// /*AFLA*/ {
// /*AFLA*/ 	if (!(css->flags & CSS_NO_REF))
// /*AFLA*/ 		return percpu_ref_tryget_live(&css->refcnt);
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * css_put - put a css reference
// /*AFLA*/  * @css: target css
// /*AFLA*/  *
// /*AFLA*/  * Put a reference obtained via css_get() and css_tryget_online().
// /*AFLA*/  */
// /*AFLA*/ static inline void css_put(struct cgroup_subsys_state *css)
// /*AFLA*/ {
// /*AFLA*/ 	if (!(css->flags & CSS_NO_REF))
// /*AFLA*/ 		percpu_ref_put(&css->refcnt);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * css_put_many - put css references
// /*AFLA*/  * @css: target css
// /*AFLA*/  * @n: number of references to put
// /*AFLA*/  *
// /*AFLA*/  * Put references obtained via css_get() and css_tryget_online().
// /*AFLA*/  */
// /*AFLA*/ static inline void css_put_many(struct cgroup_subsys_state *css, unsigned int n)
// /*AFLA*/ {
// /*AFLA*/ 	if (!(css->flags & CSS_NO_REF))
// /*AFLA*/ 		percpu_ref_put_many(&css->refcnt, n);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void cgroup_put(struct cgroup *cgrp)
// /*AFLA*/ {
// /*AFLA*/ 	css_put(&cgrp->self);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * task_css_set_check - obtain a task's css_set with extra access conditions
// /*AFLA*/  * @task: the task to obtain css_set for
// /*AFLA*/  * @__c: extra condition expression to be passed to rcu_dereference_check()
// /*AFLA*/  *
// /*AFLA*/  * A task's css_set is RCU protected, initialized and exited while holding
// /*AFLA*/  * task_lock(), and can only be modified while holding both cgroup_mutex
// /*AFLA*/  * and task_lock() while the task is alive.  This macro verifies that the
// /*AFLA*/  * caller is inside proper critical section and returns @task's css_set.
// /*AFLA*/  *
// /*AFLA*/  * The caller can also specify additional allowed conditions via @__c, such
// /*AFLA*/  * as locks used during the cgroup_subsys::attach() methods.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_PROVE_RCU
// /*AFLA*/ extern struct mutex cgroup_mutex;
// /*AFLA*/ extern spinlock_t css_set_lock;
// /*AFLA*/ #define task_css_set_check(task, __c)					\
// /*AFLA*/ 	rcu_dereference_check((task)->cgroups,				\
// /*AFLA*/ 		lockdep_is_held(&cgroup_mutex) ||			\
// /*AFLA*/ 		lockdep_is_held(&css_set_lock) ||			\
// /*AFLA*/ 		((task)->flags & PF_EXITING) || (__c))
// /*AFLA*/ #else
// /*AFLA*/ #define task_css_set_check(task, __c)					\
// /*AFLA*/ 	rcu_dereference((task)->cgroups)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * task_css_check - obtain css for (task, subsys) w/ extra access conds
// /*AFLA*/  * @task: the target task
// /*AFLA*/  * @subsys_id: the target subsystem ID
// /*AFLA*/  * @__c: extra condition expression to be passed to rcu_dereference_check()
// /*AFLA*/  *
// /*AFLA*/  * Return the cgroup_subsys_state for the (@task, @subsys_id) pair.  The
// /*AFLA*/  * synchronization rules are the same as task_css_set_check().
// /*AFLA*/  */
// /*AFLA*/ #define task_css_check(task, subsys_id, __c)				\
// /*AFLA*/ 	task_css_set_check((task), (__c))->subsys[(subsys_id)]
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * task_css_set - obtain a task's css_set
// /*AFLA*/  * @task: the task to obtain css_set for
// /*AFLA*/  *
// /*AFLA*/  * See task_css_set_check().
// /*AFLA*/  */
// /*AFLA*/ static inline struct css_set *task_css_set(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	return task_css_set_check(task, false);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * task_css - obtain css for (task, subsys)
// /*AFLA*/  * @task: the target task
// /*AFLA*/  * @subsys_id: the target subsystem ID
// /*AFLA*/  *
// /*AFLA*/  * See task_css_check().
// /*AFLA*/  */
// /*AFLA*/ static inline struct cgroup_subsys_state *task_css(struct task_struct *task,
// /*AFLA*/ 						   int subsys_id)
// /*AFLA*/ {
// /*AFLA*/ 	return task_css_check(task, subsys_id, false);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * task_get_css - find and get the css for (task, subsys)
// /*AFLA*/  * @task: the target task
// /*AFLA*/  * @subsys_id: the target subsystem ID
// /*AFLA*/  *
// /*AFLA*/  * Find the css for the (@task, @subsys_id) combination, increment a
// /*AFLA*/  * reference on and return it.  This function is guaranteed to return a
// /*AFLA*/  * valid css.
// /*AFLA*/  */
// /*AFLA*/ static inline struct cgroup_subsys_state *
// /*AFLA*/ task_get_css(struct task_struct *task, int subsys_id)
// /*AFLA*/ {
// /*AFLA*/ 	struct cgroup_subsys_state *css;
// /*AFLA*/ 
// /*AFLA*/ 	rcu_read_lock();
// /*AFLA*/ 	while (true) {
// /*AFLA*/ 		css = task_css(task, subsys_id);
// /*AFLA*/ 		if (likely(css_tryget_online(css)))
// /*AFLA*/ 			break;
// /*AFLA*/ 		cpu_relax();
// /*AFLA*/ 	}
// /*AFLA*/ 	rcu_read_unlock();
// /*AFLA*/ 	return css;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * task_css_is_root - test whether a task belongs to the root css
// /*AFLA*/  * @task: the target task
// /*AFLA*/  * @subsys_id: the target subsystem ID
// /*AFLA*/  *
// /*AFLA*/  * Test whether @task belongs to the root css on the specified subsystem.
// /*AFLA*/  * May be invoked in any context.
// /*AFLA*/  */
// /*AFLA*/ static inline bool task_css_is_root(struct task_struct *task, int subsys_id)
// /*AFLA*/ {
// /*AFLA*/ 	return task_css_check(task, subsys_id, true) ==
// /*AFLA*/ 		init_css_set.subsys[subsys_id];
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct cgroup *task_cgroup(struct task_struct *task,
// /*AFLA*/ 					 int subsys_id)
// /*AFLA*/ {
// /*AFLA*/ 	return task_css(task, subsys_id)->cgroup;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cgroup_is_descendant - test ancestry
// /*AFLA*/  * @cgrp: the cgroup to be tested
// /*AFLA*/  * @ancestor: possible ancestor of @cgrp
// /*AFLA*/  *
// /*AFLA*/  * Test whether @cgrp is a descendant of @ancestor.  It also returns %true
// /*AFLA*/  * if @cgrp == @ancestor.  This function is safe to call as long as @cgrp
// /*AFLA*/  * and @ancestor are accessible.
// /*AFLA*/  */
// /*AFLA*/ static inline bool cgroup_is_descendant(struct cgroup *cgrp,
// /*AFLA*/ 					struct cgroup *ancestor)
// /*AFLA*/ {
// /*AFLA*/ 	if (cgrp->root != ancestor->root || cgrp->level < ancestor->level)
// /*AFLA*/ 		return false;
// /*AFLA*/ 	return cgrp->ancestor_ids[ancestor->level] == ancestor->id;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * task_under_cgroup_hierarchy - test task's membership of cgroup ancestry
// /*AFLA*/  * @task: the task to be tested
// /*AFLA*/  * @ancestor: possible ancestor of @task's cgroup
// /*AFLA*/  *
// /*AFLA*/  * Tests whether @task's default cgroup hierarchy is a descendant of @ancestor.
// /*AFLA*/  * It follows all the same rules as cgroup_is_descendant, and only applies
// /*AFLA*/  * to the default hierarchy.
// /*AFLA*/  */
// /*AFLA*/ static inline bool task_under_cgroup_hierarchy(struct task_struct *task,
// /*AFLA*/ 					       struct cgroup *ancestor)
// /*AFLA*/ {
// /*AFLA*/ 	struct css_set *cset = task_css_set(task);
// /*AFLA*/ 
// /*AFLA*/ 	return cgroup_is_descendant(cset->dfl_cgrp, ancestor);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* no synchronization, the result can only be used as a hint */
// /*AFLA*/ static inline bool cgroup_is_populated(struct cgroup *cgrp)
// /*AFLA*/ {
// /*AFLA*/ 	return cgrp->populated_cnt;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* returns ino associated with a cgroup */
// /*AFLA*/ static inline ino_t cgroup_ino(struct cgroup *cgrp)
// /*AFLA*/ {
// /*AFLA*/ 	return cgrp->kn->ino;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* cft/css accessors for cftype->write() operation */
// /*AFLA*/ static inline struct cftype *of_cft(struct kernfs_open_file *of)
// /*AFLA*/ {
// /*AFLA*/ 	return of->kn->priv;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct cgroup_subsys_state *of_css(struct kernfs_open_file *of);
// /*AFLA*/ 
// /*AFLA*/ /* cft/css accessors for cftype->seq_*() operations */
// /*AFLA*/ static inline struct cftype *seq_cft(struct seq_file *seq)
// /*AFLA*/ {
// /*AFLA*/ 	return of_cft(seq->private);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct cgroup_subsys_state *seq_css(struct seq_file *seq)
// /*AFLA*/ {
// /*AFLA*/ 	return of_css(seq->private);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Name / path handling functions.  All are thin wrappers around the kernfs
// /*AFLA*/  * counterparts and can be called under any context.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static inline int cgroup_name(struct cgroup *cgrp, char *buf, size_t buflen)
// /*AFLA*/ {
// /*AFLA*/ 	return kernfs_name(cgrp->kn, buf, buflen);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int cgroup_path(struct cgroup *cgrp, char *buf, size_t buflen)
// /*AFLA*/ {
// /*AFLA*/ 	return kernfs_path(cgrp->kn, buf, buflen);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void pr_cont_cgroup_name(struct cgroup *cgrp)
// /*AFLA*/ {
// /*AFLA*/ 	pr_cont_kernfs_name(cgrp->kn);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void pr_cont_cgroup_path(struct cgroup *cgrp)
// /*AFLA*/ {
// /*AFLA*/ 	pr_cont_kernfs_path(cgrp->kn);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_CGROUPS */
// /*AFLA*/ 
// /*AFLA*/ struct cgroup_subsys_state;
// /*AFLA*/ struct cgroup;
// /*AFLA*/ 
// /*AFLA*/ static inline void css_put(struct cgroup_subsys_state *css) {}
// /*AFLA*/ static inline int cgroup_attach_task_all(struct task_struct *from,
// /*AFLA*/ 					 struct task_struct *t) { return 0; }
// /*AFLA*/ static inline int cgroupstats_build(struct cgroupstats *stats,
// /*AFLA*/ 				    struct dentry *dentry) { return -EINVAL; }
// /*AFLA*/ 
// /*AFLA*/ static inline void cgroup_fork(struct task_struct *p) {}
// /*AFLA*/ static inline int cgroup_can_fork(struct task_struct *p) { return 0; }
// /*AFLA*/ static inline void cgroup_cancel_fork(struct task_struct *p) {}
// /*AFLA*/ static inline void cgroup_post_fork(struct task_struct *p) {}
// /*AFLA*/ static inline void cgroup_exit(struct task_struct *p) {}
// /*AFLA*/ static inline void cgroup_free(struct task_struct *p) {}
// /*AFLA*/ 
// /*AFLA*/ static inline int cgroup_init_early(void) { return 0; }
// /*AFLA*/ static inline int cgroup_init(void) { return 0; }
// /*AFLA*/ 
// /*AFLA*/ static inline bool task_under_cgroup_hierarchy(struct task_struct *task,
// /*AFLA*/ 					       struct cgroup *ancestor)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ #endif /* !CONFIG_CGROUPS */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * sock->sk_cgrp_data handling.  For more info, see sock_cgroup_data
// /*AFLA*/  * definition in cgroup-defs.h.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_SOCK_CGROUP_DATA
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_CGROUP_NET_PRIO) || defined(CONFIG_CGROUP_NET_CLASSID)
// /*AFLA*/ extern spinlock_t cgroup_sk_update_lock;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ void cgroup_sk_alloc_disable(void);
// /*AFLA*/ void cgroup_sk_alloc(struct sock_cgroup_data *skcd);
// /*AFLA*/ void cgroup_sk_free(struct sock_cgroup_data *skcd);
// /*AFLA*/ 
// /*AFLA*/ static inline struct cgroup *sock_cgroup_ptr(struct sock_cgroup_data *skcd)
// /*AFLA*/ {
// /*AFLA*/ #if defined(CONFIG_CGROUP_NET_PRIO) || defined(CONFIG_CGROUP_NET_CLASSID)
// /*AFLA*/ 	unsigned long v;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * @skcd->val is 64bit but the following is safe on 32bit too as we
// /*AFLA*/ 	 * just need the lower ulong to be written and read atomically.
// /*AFLA*/ 	 */
// /*AFLA*/ 	v = READ_ONCE(skcd->val);
// /*AFLA*/ 
// /*AFLA*/ 	if (v & 1)
// /*AFLA*/ 		return &cgrp_dfl_root.cgrp;
// /*AFLA*/ 
// /*AFLA*/ 	return (struct cgroup *)(unsigned long)v ?: &cgrp_dfl_root.cgrp;
// /*AFLA*/ #else
// /*AFLA*/ 	return (struct cgroup *)(unsigned long)skcd->val;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else	/* CONFIG_CGROUP_DATA */
// /*AFLA*/ 
// /*AFLA*/ static inline void cgroup_sk_alloc(struct sock_cgroup_data *skcd) {}
// /*AFLA*/ static inline void cgroup_sk_free(struct sock_cgroup_data *skcd) {}
// /*AFLA*/ 
// /*AFLA*/ #endif	/* CONFIG_CGROUP_DATA */
// /*AFLA*/ 
// /*AFLA*/ struct cgroup_namespace {
// /*AFLA*/ 	atomic_t		count;
// /*AFLA*/ 	struct ns_common	ns;
// /*AFLA*/ 	struct user_namespace	*user_ns;
// /*AFLA*/ 	struct ucounts		*ucounts;
// /*AFLA*/ 	struct css_set          *root_cset;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct cgroup_namespace init_cgroup_ns;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_CGROUPS
// /*AFLA*/ 
// /*AFLA*/ void free_cgroup_ns(struct cgroup_namespace *ns);
// /*AFLA*/ 
// /*AFLA*/ struct cgroup_namespace *copy_cgroup_ns(unsigned long flags,
// /*AFLA*/ 					struct user_namespace *user_ns,
// /*AFLA*/ 					struct cgroup_namespace *old_ns);
// /*AFLA*/ 
// /*AFLA*/ int cgroup_path_ns(struct cgroup *cgrp, char *buf, size_t buflen,
// /*AFLA*/ 		   struct cgroup_namespace *ns);
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_CGROUPS */
// /*AFLA*/ 
// /*AFLA*/ static inline void free_cgroup_ns(struct cgroup_namespace *ns) { }
// /*AFLA*/ static inline struct cgroup_namespace *
// /*AFLA*/ copy_cgroup_ns(unsigned long flags, struct user_namespace *user_ns,
// /*AFLA*/ 	       struct cgroup_namespace *old_ns)
// /*AFLA*/ {
// /*AFLA*/ 	return old_ns;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* !CONFIG_CGROUPS */
// /*AFLA*/ 
// /*AFLA*/ static inline void get_cgroup_ns(struct cgroup_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	if (ns)
// /*AFLA*/ 		atomic_inc(&ns->count);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void put_cgroup_ns(struct cgroup_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	if (ns && atomic_dec_and_test(&ns->count))
// /*AFLA*/ 		free_cgroup_ns(ns);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_CGROUP_H */
