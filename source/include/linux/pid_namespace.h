// /*AFLA*/ #ifndef _LINUX_PID_NS_H
// /*AFLA*/ #define _LINUX_PID_NS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/sched.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ #include <linux/mm.h>
// /*AFLA*/ #include <linux/workqueue.h>
// /*AFLA*/ #include <linux/threads.h>
// /*AFLA*/ #include <linux/nsproxy.h>
// /*AFLA*/ #include <linux/kref.h>
// /*AFLA*/ #include <linux/ns_common.h>
// /*AFLA*/ 
// /*AFLA*/ struct pidmap {
// /*AFLA*/        atomic_t nr_free;
// /*AFLA*/        void *page;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define BITS_PER_PAGE		(PAGE_SIZE * 8)
// /*AFLA*/ #define BITS_PER_PAGE_MASK	(BITS_PER_PAGE-1)
// /*AFLA*/ #define PIDMAP_ENTRIES		((PID_MAX_LIMIT+BITS_PER_PAGE-1)/BITS_PER_PAGE)
// /*AFLA*/ 
// /*AFLA*/ struct fs_pin;
// /*AFLA*/ 
// /*AFLA*/ struct pid_namespace {
// /*AFLA*/ 	struct kref kref;
// /*AFLA*/ 	struct pidmap pidmap[PIDMAP_ENTRIES];
// /*AFLA*/ 	struct rcu_head rcu;
// /*AFLA*/ 	int last_pid;
// /*AFLA*/ 	unsigned int nr_hashed;
// /*AFLA*/ 	struct task_struct *child_reaper;
// /*AFLA*/ 	struct kmem_cache *pid_cachep;
// /*AFLA*/ 	unsigned int level;
// /*AFLA*/ 	struct pid_namespace *parent;
// /*AFLA*/ #ifdef CONFIG_PROC_FS
// /*AFLA*/ 	struct vfsmount *proc_mnt;
// /*AFLA*/ 	struct dentry *proc_self;
// /*AFLA*/ 	struct dentry *proc_thread_self;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_BSD_PROCESS_ACCT
// /*AFLA*/ 	struct fs_pin *bacct;
// /*AFLA*/ #endif
// /*AFLA*/ 	struct user_namespace *user_ns;
// /*AFLA*/ 	struct ucounts *ucounts;
// /*AFLA*/ 	struct work_struct proc_work;
// /*AFLA*/ 	kgid_t pid_gid;
// /*AFLA*/ 	int hide_pid;
// /*AFLA*/ 	int reboot;	/* group exit code if this pidns was rebooted */
// /*AFLA*/ 	struct ns_common ns;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct pid_namespace init_pid_ns;
// /*AFLA*/ 
// /*AFLA*/ #define PIDNS_HASH_ADDING (1U << 31)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PID_NS
// /*AFLA*/ static inline struct pid_namespace *get_pid_ns(struct pid_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	if (ns != &init_pid_ns)
// /*AFLA*/ 		kref_get(&ns->kref);
// /*AFLA*/ 	return ns;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct pid_namespace *copy_pid_ns(unsigned long flags,
// /*AFLA*/ 	struct user_namespace *user_ns, struct pid_namespace *ns);
// /*AFLA*/ extern void zap_pid_ns_processes(struct pid_namespace *pid_ns);
// /*AFLA*/ extern int reboot_pid_ns(struct pid_namespace *pid_ns, int cmd);
// /*AFLA*/ extern void put_pid_ns(struct pid_namespace *ns);
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_PID_NS */
// /*AFLA*/ #include <linux/err.h>
// /*AFLA*/ 
// /*AFLA*/ static inline struct pid_namespace *get_pid_ns(struct pid_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	return ns;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct pid_namespace *copy_pid_ns(unsigned long flags,
// /*AFLA*/ 	struct user_namespace *user_ns, struct pid_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	if (flags & CLONE_NEWPID)
// /*AFLA*/ 		ns = ERR_PTR(-EINVAL);
// /*AFLA*/ 	return ns;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void put_pid_ns(struct pid_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void zap_pid_ns_processes(struct pid_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	BUG();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int reboot_pid_ns(struct pid_namespace *pid_ns, int cmd)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_PID_NS */
// /*AFLA*/ 
// /*AFLA*/ extern struct pid_namespace *task_active_pid_ns(struct task_struct *tsk);
// /*AFLA*/ void pidhash_init(void);
// /*AFLA*/ void pidmap_init(void);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_PID_NS_H */
