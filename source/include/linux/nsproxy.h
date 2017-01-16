// /*AFLA*/ #ifndef _LINUX_NSPROXY_H
// /*AFLA*/ #define _LINUX_NSPROXY_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/sched.h>
// /*AFLA*/ 
// /*AFLA*/ struct mnt_namespace;
// /*AFLA*/ struct uts_namespace;
// /*AFLA*/ struct ipc_namespace;
// /*AFLA*/ struct pid_namespace;
// /*AFLA*/ struct cgroup_namespace;
// /*AFLA*/ struct fs_struct;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A structure to contain pointers to all per-process
// /*AFLA*/  * namespaces - fs (mount), uts, network, sysvipc, etc.
// /*AFLA*/  *
// /*AFLA*/  * The pid namespace is an exception -- it's accessed using
// /*AFLA*/  * task_active_pid_ns.  The pid namespace here is the
// /*AFLA*/  * namespace that children will use.
// /*AFLA*/  *
// /*AFLA*/  * 'count' is the number of tasks holding a reference.
// /*AFLA*/  * The count for each namespace, then, will be the number
// /*AFLA*/  * of nsproxies pointing to it, not the number of tasks.
// /*AFLA*/  *
// /*AFLA*/  * The nsproxy is shared by tasks which share all namespaces.
// /*AFLA*/  * As soon as a single namespace is cloned or unshared, the
// /*AFLA*/  * nsproxy is copied.
// /*AFLA*/  */
// /*AFLA*/ struct nsproxy {
// /*AFLA*/ 	atomic_t count;
// /*AFLA*/ 	struct uts_namespace *uts_ns;
// /*AFLA*/ 	struct ipc_namespace *ipc_ns;
// /*AFLA*/ 	struct mnt_namespace *mnt_ns;
// /*AFLA*/ 	struct pid_namespace *pid_ns_for_children;
// /*AFLA*/ 	struct net 	     *net_ns;
// /*AFLA*/ 	struct cgroup_namespace *cgroup_ns;
// /*AFLA*/ };
// /*AFLA*/ extern struct nsproxy init_nsproxy;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * the namespaces access rules are:
// /*AFLA*/  *
// /*AFLA*/  *  1. only current task is allowed to change tsk->nsproxy pointer or
// /*AFLA*/  *     any pointer on the nsproxy itself.  Current must hold the task_lock
// /*AFLA*/  *     when changing tsk->nsproxy.
// /*AFLA*/  *
// /*AFLA*/  *  2. when accessing (i.e. reading) current task's namespaces - no
// /*AFLA*/  *     precautions should be taken - just dereference the pointers
// /*AFLA*/  *
// /*AFLA*/  *  3. the access to other task namespaces is performed like this
// /*AFLA*/  *     task_lock(task);
// /*AFLA*/  *     nsproxy = task->nsproxy;
// /*AFLA*/  *     if (nsproxy != NULL) {
// /*AFLA*/  *             / *
// /*AFLA*/  *               * work with the namespaces here
// /*AFLA*/  *               * e.g. get the reference on one of them
// /*AFLA*/  *               * /
// /*AFLA*/  *     } / *
// /*AFLA*/  *         * NULL task->nsproxy means that this task is
// /*AFLA*/  *         * almost dead (zombie)
// /*AFLA*/  *         * /
// /*AFLA*/  *     task_unlock(task);
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ int copy_namespaces(unsigned long flags, struct task_struct *tsk);
// /*AFLA*/ void exit_task_namespaces(struct task_struct *tsk);
// /*AFLA*/ void switch_task_namespaces(struct task_struct *tsk, struct nsproxy *new);
// /*AFLA*/ void free_nsproxy(struct nsproxy *ns);
// /*AFLA*/ int unshare_nsproxy_namespaces(unsigned long, struct nsproxy **,
// /*AFLA*/ 	struct cred *, struct fs_struct *);
// /*AFLA*/ int __init nsproxy_cache_init(void);
// /*AFLA*/ 
// /*AFLA*/ static inline void put_nsproxy(struct nsproxy *ns)
// /*AFLA*/ {
// /*AFLA*/ 	if (atomic_dec_and_test(&ns->count)) {
// /*AFLA*/ 		free_nsproxy(ns);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void get_nsproxy(struct nsproxy *ns)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_inc(&ns->count);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
