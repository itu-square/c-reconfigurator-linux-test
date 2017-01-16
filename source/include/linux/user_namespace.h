// /*AFLA*/ #ifndef _LINUX_USER_NAMESPACE_H
// /*AFLA*/ #define _LINUX_USER_NAMESPACE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/kref.h>
// /*AFLA*/ #include <linux/nsproxy.h>
// /*AFLA*/ #include <linux/ns_common.h>
// /*AFLA*/ #include <linux/sched.h>
// /*AFLA*/ #include <linux/err.h>
// /*AFLA*/ 
// /*AFLA*/ #define UID_GID_MAP_MAX_EXTENTS 5
// /*AFLA*/ 
// /*AFLA*/ struct uid_gid_map {	/* 64 bytes -- 1 cache line */
// /*AFLA*/ 	u32 nr_extents;
// /*AFLA*/ 	struct uid_gid_extent {
// /*AFLA*/ 		u32 first;
// /*AFLA*/ 		u32 lower_first;
// /*AFLA*/ 		u32 count;
// /*AFLA*/ 	} extent[UID_GID_MAP_MAX_EXTENTS];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define USERNS_SETGROUPS_ALLOWED 1UL
// /*AFLA*/ 
// /*AFLA*/ #define USERNS_INIT_FLAGS USERNS_SETGROUPS_ALLOWED
// /*AFLA*/ 
// /*AFLA*/ struct ucounts;
// /*AFLA*/ 
// /*AFLA*/ enum ucount_type {
// /*AFLA*/ 	UCOUNT_USER_NAMESPACES,
// /*AFLA*/ 	UCOUNT_PID_NAMESPACES,
// /*AFLA*/ 	UCOUNT_UTS_NAMESPACES,
// /*AFLA*/ 	UCOUNT_IPC_NAMESPACES,
// /*AFLA*/ 	UCOUNT_NET_NAMESPACES,
// /*AFLA*/ 	UCOUNT_MNT_NAMESPACES,
// /*AFLA*/ 	UCOUNT_CGROUP_NAMESPACES,
// /*AFLA*/ 	UCOUNT_COUNTS,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct user_namespace {
// /*AFLA*/ 	struct uid_gid_map	uid_map;
// /*AFLA*/ 	struct uid_gid_map	gid_map;
// /*AFLA*/ 	struct uid_gid_map	projid_map;
// /*AFLA*/ 	atomic_t		count;
// /*AFLA*/ 	struct user_namespace	*parent;
// /*AFLA*/ 	int			level;
// /*AFLA*/ 	kuid_t			owner;
// /*AFLA*/ 	kgid_t			group;
// /*AFLA*/ 	struct ns_common	ns;
// /*AFLA*/ 	unsigned long		flags;
// /*AFLA*/ 
// /*AFLA*/ 	/* Register of per-UID persistent keyrings for this namespace */
// /*AFLA*/ #ifdef CONFIG_PERSISTENT_KEYRINGS
// /*AFLA*/ 	struct key		*persistent_keyring_register;
// /*AFLA*/ 	struct rw_semaphore	persistent_keyring_register_sem;
// /*AFLA*/ #endif
// /*AFLA*/ 	struct work_struct	work;
// /*AFLA*/ #ifdef CONFIG_SYSCTL
// /*AFLA*/ 	struct ctl_table_set	set;
// /*AFLA*/ 	struct ctl_table_header *sysctls;
// /*AFLA*/ #endif
// /*AFLA*/ 	struct ucounts		*ucounts;
// /*AFLA*/ 	int ucount_max[UCOUNT_COUNTS];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct ucounts {
// /*AFLA*/ 	struct hlist_node node;
// /*AFLA*/ 	struct user_namespace *ns;
// /*AFLA*/ 	kuid_t uid;
// /*AFLA*/ 	atomic_t count;
// /*AFLA*/ 	atomic_t ucount[UCOUNT_COUNTS];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct user_namespace init_user_ns;
// /*AFLA*/ 
// /*AFLA*/ bool setup_userns_sysctls(struct user_namespace *ns);
// /*AFLA*/ void retire_userns_sysctls(struct user_namespace *ns);
// /*AFLA*/ struct ucounts *inc_ucount(struct user_namespace *ns, kuid_t uid, enum ucount_type type);
// /*AFLA*/ void dec_ucount(struct ucounts *ucounts, enum ucount_type type);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_USER_NS
// /*AFLA*/ 
// /*AFLA*/ static inline struct user_namespace *get_user_ns(struct user_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	if (ns)
// /*AFLA*/ 		atomic_inc(&ns->count);
// /*AFLA*/ 	return ns;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int create_user_ns(struct cred *new);
// /*AFLA*/ extern int unshare_userns(unsigned long unshare_flags, struct cred **new_cred);
// /*AFLA*/ extern void __put_user_ns(struct user_namespace *ns);
// /*AFLA*/ 
// /*AFLA*/ static inline void put_user_ns(struct user_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	if (ns && atomic_dec_and_test(&ns->count))
// /*AFLA*/ 		__put_user_ns(ns);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct seq_operations;
// /*AFLA*/ extern const struct seq_operations proc_uid_seq_operations;
// /*AFLA*/ extern const struct seq_operations proc_gid_seq_operations;
// /*AFLA*/ extern const struct seq_operations proc_projid_seq_operations;
// /*AFLA*/ extern ssize_t proc_uid_map_write(struct file *, const char __user *, size_t, loff_t *);
// /*AFLA*/ extern ssize_t proc_gid_map_write(struct file *, const char __user *, size_t, loff_t *);
// /*AFLA*/ extern ssize_t proc_projid_map_write(struct file *, const char __user *, size_t, loff_t *);
// /*AFLA*/ extern ssize_t proc_setgroups_write(struct file *, const char __user *, size_t, loff_t *);
// /*AFLA*/ extern int proc_setgroups_show(struct seq_file *m, void *v);
// /*AFLA*/ extern bool userns_may_setgroups(const struct user_namespace *ns);
// /*AFLA*/ extern bool current_in_userns(const struct user_namespace *target_ns);
// /*AFLA*/ 
// /*AFLA*/ struct ns_common *ns_get_owner(struct ns_common *ns);
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline struct user_namespace *get_user_ns(struct user_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	return &init_user_ns;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int create_user_ns(struct cred *new)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int unshare_userns(unsigned long unshare_flags,
// /*AFLA*/ 				 struct cred **new_cred)
// /*AFLA*/ {
// /*AFLA*/ 	if (unshare_flags & CLONE_NEWUSER)
// /*AFLA*/ 		return -EINVAL;
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void put_user_ns(struct user_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool userns_may_setgroups(const struct user_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool current_in_userns(const struct user_namespace *target_ns)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct ns_common *ns_get_owner(struct ns_common *ns)
// /*AFLA*/ {
// /*AFLA*/ 	return ERR_PTR(-EPERM);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_USER_H */
