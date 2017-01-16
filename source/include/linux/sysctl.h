// /*AFLA*/ /*
// /*AFLA*/  * sysctl.h: General linux system control interface
// /*AFLA*/  *
// /*AFLA*/  * Begun 24 March 1995, Stephen Tweedie
// /*AFLA*/  *
// /*AFLA*/  ****************************************************************
// /*AFLA*/  ****************************************************************
// /*AFLA*/  **
// /*AFLA*/  **  WARNING:
// /*AFLA*/  **  The values in this file are exported to user space via 
// /*AFLA*/  **  the sysctl() binary interface.  Do *NOT* change the
// /*AFLA*/  **  numbering of any existing values here, and do not change
// /*AFLA*/  **  any numbers within any one set of values.  If you have to
// /*AFLA*/  **  redefine an existing interface, use a new number for it.
// /*AFLA*/  **  The kernel will then return -ENOTDIR to any application using
// /*AFLA*/  **  the old binary interface.
// /*AFLA*/  **
// /*AFLA*/  ****************************************************************
// /*AFLA*/  ****************************************************************
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LINUX_SYSCTL_H
// /*AFLA*/ #define _LINUX_SYSCTL_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ #include <linux/wait.h>
// /*AFLA*/ #include <linux/rbtree.h>
// /*AFLA*/ #include <linux/uidgid.h>
// /*AFLA*/ #include <uapi/linux/sysctl.h>
// /*AFLA*/ 
// /*AFLA*/ /* For the /proc/sys support */
// /*AFLA*/ struct completion;
// /*AFLA*/ struct ctl_table;
// /*AFLA*/ struct nsproxy;
// /*AFLA*/ struct ctl_table_root;
// /*AFLA*/ struct ctl_table_header;
// /*AFLA*/ struct ctl_dir;
// /*AFLA*/ 
// /*AFLA*/ typedef int proc_handler (struct ctl_table *ctl, int write,
// /*AFLA*/ 			  void __user *buffer, size_t *lenp, loff_t *ppos);
// /*AFLA*/ 
// /*AFLA*/ extern int proc_dostring(struct ctl_table *, int,
// /*AFLA*/ 			 void __user *, size_t *, loff_t *);
// /*AFLA*/ extern int proc_dointvec(struct ctl_table *, int,
// /*AFLA*/ 			 void __user *, size_t *, loff_t *);
// /*AFLA*/ extern int proc_douintvec(struct ctl_table *, int,
// /*AFLA*/ 			 void __user *, size_t *, loff_t *);
// /*AFLA*/ extern int proc_dointvec_minmax(struct ctl_table *, int,
// /*AFLA*/ 				void __user *, size_t *, loff_t *);
// /*AFLA*/ extern int proc_dointvec_jiffies(struct ctl_table *, int,
// /*AFLA*/ 				 void __user *, size_t *, loff_t *);
// /*AFLA*/ extern int proc_dointvec_userhz_jiffies(struct ctl_table *, int,
// /*AFLA*/ 					void __user *, size_t *, loff_t *);
// /*AFLA*/ extern int proc_dointvec_ms_jiffies(struct ctl_table *, int,
// /*AFLA*/ 				    void __user *, size_t *, loff_t *);
// /*AFLA*/ extern int proc_doulongvec_minmax(struct ctl_table *, int,
// /*AFLA*/ 				  void __user *, size_t *, loff_t *);
// /*AFLA*/ extern int proc_doulongvec_ms_jiffies_minmax(struct ctl_table *table, int,
// /*AFLA*/ 				      void __user *, size_t *, loff_t *);
// /*AFLA*/ extern int proc_do_large_bitmap(struct ctl_table *, int,
// /*AFLA*/ 				void __user *, size_t *, loff_t *);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Register a set of sysctl names by calling register_sysctl_table
// /*AFLA*/  * with an initialised array of struct ctl_table's.  An entry with 
// /*AFLA*/  * NULL procname terminates the table.  table->de will be
// /*AFLA*/  * set up by the registration and need not be initialised in advance.
// /*AFLA*/  *
// /*AFLA*/  * sysctl names can be mirrored automatically under /proc/sys.  The
// /*AFLA*/  * procname supplied controls /proc naming.
// /*AFLA*/  *
// /*AFLA*/  * The table's mode will be honoured both for sys_sysctl(2) and
// /*AFLA*/  * proc-fs access.
// /*AFLA*/  *
// /*AFLA*/  * Leaf nodes in the sysctl tree will be represented by a single file
// /*AFLA*/  * under /proc; non-leaf nodes will be represented by directories.  A
// /*AFLA*/  * null procname disables /proc mirroring at this node.
// /*AFLA*/  *
// /*AFLA*/  * sysctl(2) can automatically manage read and write requests through
// /*AFLA*/  * the sysctl table.  The data and maxlen fields of the ctl_table
// /*AFLA*/  * struct enable minimal validation of the values being written to be
// /*AFLA*/  * performed, and the mode field allows minimal authentication.
// /*AFLA*/  * 
// /*AFLA*/  * There must be a proc_handler routine for any terminal nodes
// /*AFLA*/  * mirrored under /proc/sys (non-terminals are handled by a built-in
// /*AFLA*/  * directory handler).  Several default handlers are available to
// /*AFLA*/  * cover common cases.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* Support for userspace poll() to watch for changes */
// /*AFLA*/ struct ctl_table_poll {
// /*AFLA*/ 	atomic_t event;
// /*AFLA*/ 	wait_queue_head_t wait;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline void *proc_sys_poll_event(struct ctl_table_poll *poll)
// /*AFLA*/ {
// /*AFLA*/ 	return (void *)(unsigned long)atomic_read(&poll->event);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __CTL_TABLE_POLL_INITIALIZER(name) {				\
// /*AFLA*/ 	.event = ATOMIC_INIT(0),					\
// /*AFLA*/ 	.wait = __WAIT_QUEUE_HEAD_INITIALIZER(name.wait) }
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_CTL_TABLE_POLL(name)					\
// /*AFLA*/ 	struct ctl_table_poll name = __CTL_TABLE_POLL_INITIALIZER(name)
// /*AFLA*/ 
// /*AFLA*/ /* A sysctl table is an array of struct ctl_table: */
// /*AFLA*/ struct ctl_table 
// /*AFLA*/ {
// /*AFLA*/ 	const char *procname;		/* Text ID for /proc/sys, or zero */
// /*AFLA*/ 	void *data;
// /*AFLA*/ 	int maxlen;
// /*AFLA*/ 	umode_t mode;
// /*AFLA*/ 	struct ctl_table *child;	/* Deprecated */
// /*AFLA*/ 	proc_handler *proc_handler;	/* Callback for text formatting */
// /*AFLA*/ 	struct ctl_table_poll *poll;
// /*AFLA*/ 	void *extra1;
// /*AFLA*/ 	void *extra2;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct ctl_node {
// /*AFLA*/ 	struct rb_node node;
// /*AFLA*/ 	struct ctl_table_header *header;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* struct ctl_table_header is used to maintain dynamic lists of
// /*AFLA*/    struct ctl_table trees. */
// /*AFLA*/ struct ctl_table_header
// /*AFLA*/ {
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct {
// /*AFLA*/ 			struct ctl_table *ctl_table;
// /*AFLA*/ 			int used;
// /*AFLA*/ 			int count;
// /*AFLA*/ 			int nreg;
// /*AFLA*/ 		};
// /*AFLA*/ 		struct rcu_head rcu;
// /*AFLA*/ 	};
// /*AFLA*/ 	struct completion *unregistering;
// /*AFLA*/ 	struct ctl_table *ctl_table_arg;
// /*AFLA*/ 	struct ctl_table_root *root;
// /*AFLA*/ 	struct ctl_table_set *set;
// /*AFLA*/ 	struct ctl_dir *parent;
// /*AFLA*/ 	struct ctl_node *node;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct ctl_dir {
// /*AFLA*/ 	/* Header must be at the start of ctl_dir */
// /*AFLA*/ 	struct ctl_table_header header;
// /*AFLA*/ 	struct rb_root root;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct ctl_table_set {
// /*AFLA*/ 	int (*is_seen)(struct ctl_table_set *);
// /*AFLA*/ 	struct ctl_dir dir;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct ctl_table_root {
// /*AFLA*/ 	struct ctl_table_set default_set;
// /*AFLA*/ 	struct ctl_table_set *(*lookup)(struct ctl_table_root *root);
// /*AFLA*/ 	void (*set_ownership)(struct ctl_table_header *head,
// /*AFLA*/ 			      struct ctl_table *table,
// /*AFLA*/ 			      kuid_t *uid, kgid_t *gid);
// /*AFLA*/ 	int (*permissions)(struct ctl_table_header *head, struct ctl_table *table);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* struct ctl_path describes where in the hierarchy a table is added */
// /*AFLA*/ struct ctl_path {
// /*AFLA*/ 	const char *procname;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SYSCTL
// /*AFLA*/ 
// /*AFLA*/ void proc_sys_poll_notify(struct ctl_table_poll *poll);
// /*AFLA*/ 
// /*AFLA*/ extern void setup_sysctl_set(struct ctl_table_set *p,
// /*AFLA*/ 	struct ctl_table_root *root,
// /*AFLA*/ 	int (*is_seen)(struct ctl_table_set *));
// /*AFLA*/ extern void retire_sysctl_set(struct ctl_table_set *set);
// /*AFLA*/ 
// /*AFLA*/ void register_sysctl_root(struct ctl_table_root *root);
// /*AFLA*/ struct ctl_table_header *__register_sysctl_table(
// /*AFLA*/ 	struct ctl_table_set *set,
// /*AFLA*/ 	const char *path, struct ctl_table *table);
// /*AFLA*/ struct ctl_table_header *__register_sysctl_paths(
// /*AFLA*/ 	struct ctl_table_set *set,
// /*AFLA*/ 	const struct ctl_path *path, struct ctl_table *table);
// /*AFLA*/ struct ctl_table_header *register_sysctl(const char *path, struct ctl_table *table);
// /*AFLA*/ struct ctl_table_header *register_sysctl_table(struct ctl_table * table);
// /*AFLA*/ struct ctl_table_header *register_sysctl_paths(const struct ctl_path *path,
// /*AFLA*/ 						struct ctl_table *table);
// /*AFLA*/ 
// /*AFLA*/ void unregister_sysctl_table(struct ctl_table_header * table);
// /*AFLA*/ 
// /*AFLA*/ extern int sysctl_init(void);
// /*AFLA*/ 
// /*AFLA*/ extern struct ctl_table sysctl_mount_point[];
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_SYSCTL */
// /*AFLA*/ static inline struct ctl_table_header *register_sysctl_table(struct ctl_table * table)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct ctl_table_header *register_sysctl_paths(
// /*AFLA*/ 			const struct ctl_path *path, struct ctl_table *table)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void unregister_sysctl_table(struct ctl_table_header * table)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void setup_sysctl_set(struct ctl_table_set *p,
// /*AFLA*/ 	struct ctl_table_root *root,
// /*AFLA*/ 	int (*is_seen)(struct ctl_table_set *))
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_SYSCTL */
// /*AFLA*/ 
// /*AFLA*/ int sysctl_max_threads(struct ctl_table *table, int write,
// /*AFLA*/ 		       void __user *buffer, size_t *lenp, loff_t *ppos);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_SYSCTL_H */
