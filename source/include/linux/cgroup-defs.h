// /*AFLA*/ /*
// /*AFLA*/  * linux/cgroup-defs.h - basic definitions for cgroup
// /*AFLA*/  *
// /*AFLA*/  * This file provides basic type and interface.  Include this file directly
// /*AFLA*/  * only if necessary to avoid cyclic dependencies.
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LINUX_CGROUP_DEFS_H
// /*AFLA*/ #define _LINUX_CGROUP_DEFS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/limits.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/idr.h>
// /*AFLA*/ #include <linux/wait.h>
// /*AFLA*/ #include <linux/mutex.h>
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ #include <linux/percpu-refcount.h>
// /*AFLA*/ #include <linux/percpu-rwsem.h>
// /*AFLA*/ #include <linux/workqueue.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_CGROUPS
// /*AFLA*/ 
// /*AFLA*/ struct cgroup;
// /*AFLA*/ struct cgroup_root;
// /*AFLA*/ struct cgroup_subsys;
// /*AFLA*/ struct cgroup_taskset;
// /*AFLA*/ struct kernfs_node;
// /*AFLA*/ struct kernfs_ops;
// /*AFLA*/ struct kernfs_open_file;
// /*AFLA*/ struct seq_file;
// /*AFLA*/ 
// /*AFLA*/ #define MAX_CGROUP_TYPE_NAMELEN 32
// /*AFLA*/ #define MAX_CGROUP_ROOT_NAMELEN 64
// /*AFLA*/ #define MAX_CFTYPE_NAME		64
// /*AFLA*/ 
// /*AFLA*/ /* define the enumeration of all cgroup subsystems */
// /*AFLA*/ #define SUBSYS(_x) _x ## _cgrp_id,
// /*AFLA*/ enum cgroup_subsys_id {
// /*AFLA*/ #include <linux/cgroup_subsys.h>
// /*AFLA*/ 	CGROUP_SUBSYS_COUNT,
// /*AFLA*/ };
// /*AFLA*/ #undef SUBSYS
// /*AFLA*/ 
// /*AFLA*/ /* bits in struct cgroup_subsys_state flags field */
// /*AFLA*/ enum {
// /*AFLA*/ 	CSS_NO_REF	= (1 << 0), /* no reference counting for this css */
// /*AFLA*/ 	CSS_ONLINE	= (1 << 1), /* between ->css_online() and ->css_offline() */
// /*AFLA*/ 	CSS_RELEASED	= (1 << 2), /* refcnt reached zero, released */
// /*AFLA*/ 	CSS_VISIBLE	= (1 << 3), /* css is visible to userland */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* bits in struct cgroup flags field */
// /*AFLA*/ enum {
// /*AFLA*/ 	/* Control Group requires release notifications to userspace */
// /*AFLA*/ 	CGRP_NOTIFY_ON_RELEASE,
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Clone the parent's configuration when creating a new child
// /*AFLA*/ 	 * cpuset cgroup.  For historical reasons, this option can be
// /*AFLA*/ 	 * specified at mount time and thus is implemented here.
// /*AFLA*/ 	 */
// /*AFLA*/ 	CGRP_CPUSET_CLONE_CHILDREN,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* cgroup_root->flags */
// /*AFLA*/ enum {
// /*AFLA*/ 	CGRP_ROOT_NOPREFIX	= (1 << 1), /* mounted subsystems have no named prefix */
// /*AFLA*/ 	CGRP_ROOT_XATTR		= (1 << 2), /* supports extended attributes */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* cftype->flags */
// /*AFLA*/ enum {
// /*AFLA*/ 	CFTYPE_ONLY_ON_ROOT	= (1 << 0),	/* only create on root cgrp */
// /*AFLA*/ 	CFTYPE_NOT_ON_ROOT	= (1 << 1),	/* don't create on root cgrp */
// /*AFLA*/ 	CFTYPE_NO_PREFIX	= (1 << 3),	/* (DON'T USE FOR NEW FILES) no subsys prefix */
// /*AFLA*/ 	CFTYPE_WORLD_WRITABLE	= (1 << 4),	/* (DON'T USE FOR NEW FILES) S_IWUGO */
// /*AFLA*/ 
// /*AFLA*/ 	/* internal flags, do not use outside cgroup core proper */
// /*AFLA*/ 	__CFTYPE_ONLY_ON_DFL	= (1 << 16),	/* only on default hierarchy */
// /*AFLA*/ 	__CFTYPE_NOT_ON_DFL	= (1 << 17),	/* not on default hierarchy */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * cgroup_file is the handle for a file instance created in a cgroup which
// /*AFLA*/  * is used, for example, to generate file changed notifications.  This can
// /*AFLA*/  * be obtained by setting cftype->file_offset.
// /*AFLA*/  */
// /*AFLA*/ struct cgroup_file {
// /*AFLA*/ 	/* do not access any fields from outside cgroup core */
// /*AFLA*/ 	struct kernfs_node *kn;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Per-subsystem/per-cgroup state maintained by the system.  This is the
// /*AFLA*/  * fundamental structural building block that controllers deal with.
// /*AFLA*/  *
// /*AFLA*/  * Fields marked with "PI:" are public and immutable and may be accessed
// /*AFLA*/  * directly without synchronization.
// /*AFLA*/  */
// /*AFLA*/ struct cgroup_subsys_state {
// /*AFLA*/ 	/* PI: the cgroup that this css is attached to */
// /*AFLA*/ 	struct cgroup *cgroup;
// /*AFLA*/ 
// /*AFLA*/ 	/* PI: the cgroup subsystem that this css is attached to */
// /*AFLA*/ 	struct cgroup_subsys *ss;
// /*AFLA*/ 
// /*AFLA*/ 	/* reference count - access via css_[try]get() and css_put() */
// /*AFLA*/ 	struct percpu_ref refcnt;
// /*AFLA*/ 
// /*AFLA*/ 	/* PI: the parent css */
// /*AFLA*/ 	struct cgroup_subsys_state *parent;
// /*AFLA*/ 
// /*AFLA*/ 	/* siblings list anchored at the parent's ->children */
// /*AFLA*/ 	struct list_head sibling;
// /*AFLA*/ 	struct list_head children;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * PI: Subsys-unique ID.  0 is unused and root is always 1.  The
// /*AFLA*/ 	 * matching css can be looked up using css_from_id().
// /*AFLA*/ 	 */
// /*AFLA*/ 	int id;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned int flags;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Monotonically increasing unique serial number which defines a
// /*AFLA*/ 	 * uniform order among all csses.  It's guaranteed that all
// /*AFLA*/ 	 * ->children lists are in the ascending order of ->serial_nr and
// /*AFLA*/ 	 * used to allow interrupting and resuming iterations.
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64 serial_nr;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Incremented by online self and children.  Used to guarantee that
// /*AFLA*/ 	 * parents are not offlined before their children.
// /*AFLA*/ 	 */
// /*AFLA*/ 	atomic_t online_cnt;
// /*AFLA*/ 
// /*AFLA*/ 	/* percpu_ref killing and RCU release */
// /*AFLA*/ 	struct rcu_head rcu_head;
// /*AFLA*/ 	struct work_struct destroy_work;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A css_set is a structure holding pointers to a set of
// /*AFLA*/  * cgroup_subsys_state objects. This saves space in the task struct
// /*AFLA*/  * object and speeds up fork()/exit(), since a single inc/dec and a
// /*AFLA*/  * list_add()/del() can bump the reference count on the entire cgroup
// /*AFLA*/  * set for a task.
// /*AFLA*/  */
// /*AFLA*/ struct css_set {
// /*AFLA*/ 	/* Reference count */
// /*AFLA*/ 	atomic_t refcount;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * List running through all cgroup groups in the same hash
// /*AFLA*/ 	 * slot. Protected by css_set_lock
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct hlist_node hlist;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Lists running through all tasks using this cgroup group.
// /*AFLA*/ 	 * mg_tasks lists tasks which belong to this cset but are in the
// /*AFLA*/ 	 * process of being migrated out or in.  Protected by
// /*AFLA*/ 	 * css_set_rwsem, but, during migration, once tasks are moved to
// /*AFLA*/ 	 * mg_tasks, it can be read safely while holding cgroup_mutex.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_head tasks;
// /*AFLA*/ 	struct list_head mg_tasks;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * List of cgrp_cset_links pointing at cgroups referenced from this
// /*AFLA*/ 	 * css_set.  Protected by css_set_lock.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_head cgrp_links;
// /*AFLA*/ 
// /*AFLA*/ 	/* the default cgroup associated with this css_set */
// /*AFLA*/ 	struct cgroup *dfl_cgrp;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Set of subsystem states, one for each subsystem. This array is
// /*AFLA*/ 	 * immutable after creation apart from the init_css_set during
// /*AFLA*/ 	 * subsystem registration (at boot time).
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct cgroup_subsys_state *subsys[CGROUP_SUBSYS_COUNT];
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * List of csets participating in the on-going migration either as
// /*AFLA*/ 	 * source or destination.  Protected by cgroup_mutex.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_head mg_preload_node;
// /*AFLA*/ 	struct list_head mg_node;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If this cset is acting as the source of migration the following
// /*AFLA*/ 	 * two fields are set.  mg_src_cgrp and mg_dst_cgrp are
// /*AFLA*/ 	 * respectively the source and destination cgroups of the on-going
// /*AFLA*/ 	 * migration.  mg_dst_cset is the destination cset the target tasks
// /*AFLA*/ 	 * on this cset should be migrated to.  Protected by cgroup_mutex.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct cgroup *mg_src_cgrp;
// /*AFLA*/ 	struct cgroup *mg_dst_cgrp;
// /*AFLA*/ 	struct css_set *mg_dst_cset;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * On the default hierarhcy, ->subsys[ssid] may point to a css
// /*AFLA*/ 	 * attached to an ancestor instead of the cgroup this css_set is
// /*AFLA*/ 	 * associated with.  The following node is anchored at
// /*AFLA*/ 	 * ->subsys[ssid]->cgroup->e_csets[ssid] and provides a way to
// /*AFLA*/ 	 * iterate through all css's attached to a given cgroup.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_head e_cset_node[CGROUP_SUBSYS_COUNT];
// /*AFLA*/ 
// /*AFLA*/ 	/* all css_task_iters currently walking this cset */
// /*AFLA*/ 	struct list_head task_iters;
// /*AFLA*/ 
// /*AFLA*/ 	/* dead and being drained, ignore for migration */
// /*AFLA*/ 	bool dead;
// /*AFLA*/ 
// /*AFLA*/ 	/* For RCU-protected deletion */
// /*AFLA*/ 	struct rcu_head rcu_head;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct cgroup {
// /*AFLA*/ 	/* self css with NULL ->ss, points back to this cgroup */
// /*AFLA*/ 	struct cgroup_subsys_state self;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long flags;		/* "unsigned long" so bitops work */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * idr allocated in-hierarchy ID.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * ID 0 is not used, the ID of the root cgroup is always 1, and a
// /*AFLA*/ 	 * new cgroup will be assigned with a smallest available ID.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Allocating/Removing ID must be protected by cgroup_mutex.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int id;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The depth this cgroup is at.  The root is at depth zero and each
// /*AFLA*/ 	 * step down the hierarchy increments the level.  This along with
// /*AFLA*/ 	 * ancestor_ids[] can determine whether a given cgroup is a
// /*AFLA*/ 	 * descendant of another without traversing the hierarchy.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int level;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Each non-empty css_set associated with this cgroup contributes
// /*AFLA*/ 	 * one to populated_cnt.  All children with non-zero popuplated_cnt
// /*AFLA*/ 	 * of their own contribute one.  The count is zero iff there's no
// /*AFLA*/ 	 * task in this cgroup or its subtree.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int populated_cnt;
// /*AFLA*/ 
// /*AFLA*/ 	struct kernfs_node *kn;		/* cgroup kernfs entry */
// /*AFLA*/ 	struct cgroup_file procs_file;	/* handle for "cgroup.procs" */
// /*AFLA*/ 	struct cgroup_file events_file;	/* handle for "cgroup.events" */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The bitmask of subsystems enabled on the child cgroups.
// /*AFLA*/ 	 * ->subtree_control is the one configured through
// /*AFLA*/ 	 * "cgroup.subtree_control" while ->child_ss_mask is the effective
// /*AFLA*/ 	 * one which may have more subsystems enabled.  Controller knobs
// /*AFLA*/ 	 * are made available iff it's enabled in ->subtree_control.
// /*AFLA*/ 	 */
// /*AFLA*/ 	u16 subtree_control;
// /*AFLA*/ 	u16 subtree_ss_mask;
// /*AFLA*/ 	u16 old_subtree_control;
// /*AFLA*/ 	u16 old_subtree_ss_mask;
// /*AFLA*/ 
// /*AFLA*/ 	/* Private pointers for each registered subsystem */
// /*AFLA*/ 	struct cgroup_subsys_state __rcu *subsys[CGROUP_SUBSYS_COUNT];
// /*AFLA*/ 
// /*AFLA*/ 	struct cgroup_root *root;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * List of cgrp_cset_links pointing at css_sets with tasks in this
// /*AFLA*/ 	 * cgroup.  Protected by css_set_lock.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_head cset_links;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * On the default hierarchy, a css_set for a cgroup with some
// /*AFLA*/ 	 * susbsys disabled will point to css's which are associated with
// /*AFLA*/ 	 * the closest ancestor which has the subsys enabled.  The
// /*AFLA*/ 	 * following lists all css_sets which point to this cgroup's css
// /*AFLA*/ 	 * for the given subsystem.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_head e_csets[CGROUP_SUBSYS_COUNT];
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * list of pidlists, up to two for each namespace (one for procs, one
// /*AFLA*/ 	 * for tasks); created on demand.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_head pidlists;
// /*AFLA*/ 	struct mutex pidlist_mutex;
// /*AFLA*/ 
// /*AFLA*/ 	/* used to wait for offlining of csses */
// /*AFLA*/ 	wait_queue_head_t offline_waitq;
// /*AFLA*/ 
// /*AFLA*/ 	/* used to schedule release agent */
// /*AFLA*/ 	struct work_struct release_agent_work;
// /*AFLA*/ 
// /*AFLA*/ 	/* ids of the ancestors at each level including self */
// /*AFLA*/ 	int ancestor_ids[];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A cgroup_root represents the root of a cgroup hierarchy, and may be
// /*AFLA*/  * associated with a kernfs_root to form an active hierarchy.  This is
// /*AFLA*/  * internal to cgroup core.  Don't access directly from controllers.
// /*AFLA*/  */
// /*AFLA*/ struct cgroup_root {
// /*AFLA*/ 	struct kernfs_root *kf_root;
// /*AFLA*/ 
// /*AFLA*/ 	/* The bitmask of subsystems attached to this hierarchy */
// /*AFLA*/ 	unsigned int subsys_mask;
// /*AFLA*/ 
// /*AFLA*/ 	/* Unique id for this hierarchy. */
// /*AFLA*/ 	int hierarchy_id;
// /*AFLA*/ 
// /*AFLA*/ 	/* The root cgroup.  Root is destroyed on its release. */
// /*AFLA*/ 	struct cgroup cgrp;
// /*AFLA*/ 
// /*AFLA*/ 	/* for cgrp->ancestor_ids[0] */
// /*AFLA*/ 	int cgrp_ancestor_id_storage;
// /*AFLA*/ 
// /*AFLA*/ 	/* Number of cgroups in the hierarchy, used only for /proc/cgroups */
// /*AFLA*/ 	atomic_t nr_cgrps;
// /*AFLA*/ 
// /*AFLA*/ 	/* A list running through the active hierarchies */
// /*AFLA*/ 	struct list_head root_list;
// /*AFLA*/ 
// /*AFLA*/ 	/* Hierarchy-specific flags */
// /*AFLA*/ 	unsigned int flags;
// /*AFLA*/ 
// /*AFLA*/ 	/* IDs for cgroups in this hierarchy */
// /*AFLA*/ 	struct idr cgroup_idr;
// /*AFLA*/ 
// /*AFLA*/ 	/* The path to use for release notifications. */
// /*AFLA*/ 	char release_agent_path[PATH_MAX];
// /*AFLA*/ 
// /*AFLA*/ 	/* The name for this hierarchy - may be empty */
// /*AFLA*/ 	char name[MAX_CGROUP_ROOT_NAMELEN];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * struct cftype: handler definitions for cgroup control files
// /*AFLA*/  *
// /*AFLA*/  * When reading/writing to a file:
// /*AFLA*/  *	- the cgroup to use is file->f_path.dentry->d_parent->d_fsdata
// /*AFLA*/  *	- the 'cftype' of the file is file->f_path.dentry->d_fsdata
// /*AFLA*/  */
// /*AFLA*/ struct cftype {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * By convention, the name should begin with the name of the
// /*AFLA*/ 	 * subsystem, followed by a period.  Zero length string indicates
// /*AFLA*/ 	 * end of cftype array.
// /*AFLA*/ 	 */
// /*AFLA*/ 	char name[MAX_CFTYPE_NAME];
// /*AFLA*/ 	unsigned long private;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The maximum length of string, excluding trailing nul, that can
// /*AFLA*/ 	 * be passed to write.  If < PAGE_SIZE-1, PAGE_SIZE-1 is assumed.
// /*AFLA*/ 	 */
// /*AFLA*/ 	size_t max_write_len;
// /*AFLA*/ 
// /*AFLA*/ 	/* CFTYPE_* flags */
// /*AFLA*/ 	unsigned int flags;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If non-zero, should contain the offset from the start of css to
// /*AFLA*/ 	 * a struct cgroup_file field.  cgroup will record the handle of
// /*AFLA*/ 	 * the created file into it.  The recorded handle can be used as
// /*AFLA*/ 	 * long as the containing css remains accessible.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned int file_offset;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Fields used for internal bookkeeping.  Initialized automatically
// /*AFLA*/ 	 * during registration.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct cgroup_subsys *ss;	/* NULL for cgroup core files */
// /*AFLA*/ 	struct list_head node;		/* anchored at ss->cfts */
// /*AFLA*/ 	struct kernfs_ops *kf_ops;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * read_u64() is a shortcut for the common case of returning a
// /*AFLA*/ 	 * single integer. Use it in place of read()
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64 (*read_u64)(struct cgroup_subsys_state *css, struct cftype *cft);
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * read_s64() is a signed version of read_u64()
// /*AFLA*/ 	 */
// /*AFLA*/ 	s64 (*read_s64)(struct cgroup_subsys_state *css, struct cftype *cft);
// /*AFLA*/ 
// /*AFLA*/ 	/* generic seq_file read interface */
// /*AFLA*/ 	int (*seq_show)(struct seq_file *sf, void *v);
// /*AFLA*/ 
// /*AFLA*/ 	/* optional ops, implement all or none */
// /*AFLA*/ 	void *(*seq_start)(struct seq_file *sf, loff_t *ppos);
// /*AFLA*/ 	void *(*seq_next)(struct seq_file *sf, void *v, loff_t *ppos);
// /*AFLA*/ 	void (*seq_stop)(struct seq_file *sf, void *v);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * write_u64() is a shortcut for the common case of accepting
// /*AFLA*/ 	 * a single integer (as parsed by simple_strtoull) from
// /*AFLA*/ 	 * userspace. Use in place of write(); return 0 or error.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int (*write_u64)(struct cgroup_subsys_state *css, struct cftype *cft,
// /*AFLA*/ 			 u64 val);
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * write_s64() is a signed version of write_u64()
// /*AFLA*/ 	 */
// /*AFLA*/ 	int (*write_s64)(struct cgroup_subsys_state *css, struct cftype *cft,
// /*AFLA*/ 			 s64 val);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * write() is the generic write callback which maps directly to
// /*AFLA*/ 	 * kernfs write operation and overrides all other operations.
// /*AFLA*/ 	 * Maximum write size is determined by ->max_write_len.  Use
// /*AFLA*/ 	 * of_css/cft() to access the associated css and cft.
// /*AFLA*/ 	 */
// /*AFLA*/ 	ssize_t (*write)(struct kernfs_open_file *of,
// /*AFLA*/ 			 char *buf, size_t nbytes, loff_t off);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ 	struct lock_class_key	lockdep_key;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Control Group subsystem type.
// /*AFLA*/  * See Documentation/cgroups/cgroups.txt for details
// /*AFLA*/  */
// /*AFLA*/ struct cgroup_subsys {
// /*AFLA*/ 	struct cgroup_subsys_state *(*css_alloc)(struct cgroup_subsys_state *parent_css);
// /*AFLA*/ 	int (*css_online)(struct cgroup_subsys_state *css);
// /*AFLA*/ 	void (*css_offline)(struct cgroup_subsys_state *css);
// /*AFLA*/ 	void (*css_released)(struct cgroup_subsys_state *css);
// /*AFLA*/ 	void (*css_free)(struct cgroup_subsys_state *css);
// /*AFLA*/ 	void (*css_reset)(struct cgroup_subsys_state *css);
// /*AFLA*/ 
// /*AFLA*/ 	int (*can_attach)(struct cgroup_taskset *tset);
// /*AFLA*/ 	void (*cancel_attach)(struct cgroup_taskset *tset);
// /*AFLA*/ 	void (*attach)(struct cgroup_taskset *tset);
// /*AFLA*/ 	void (*post_attach)(void);
// /*AFLA*/ 	int (*can_fork)(struct task_struct *task);
// /*AFLA*/ 	void (*cancel_fork)(struct task_struct *task);
// /*AFLA*/ 	void (*fork)(struct task_struct *task);
// /*AFLA*/ 	void (*exit)(struct task_struct *task);
// /*AFLA*/ 	void (*free)(struct task_struct *task);
// /*AFLA*/ 	void (*bind)(struct cgroup_subsys_state *root_css);
// /*AFLA*/ 
// /*AFLA*/ 	bool early_init:1;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If %true, the controller, on the default hierarchy, doesn't show
// /*AFLA*/ 	 * up in "cgroup.controllers" or "cgroup.subtree_control", is
// /*AFLA*/ 	 * implicitly enabled on all cgroups on the default hierarchy, and
// /*AFLA*/ 	 * bypasses the "no internal process" constraint.  This is for
// /*AFLA*/ 	 * utility type controllers which is transparent to userland.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * An implicit controller can be stolen from the default hierarchy
// /*AFLA*/ 	 * anytime and thus must be okay with offline csses from previous
// /*AFLA*/ 	 * hierarchies coexisting with csses for the current one.
// /*AFLA*/ 	 */
// /*AFLA*/ 	bool implicit_on_dfl:1;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If %false, this subsystem is properly hierarchical -
// /*AFLA*/ 	 * configuration, resource accounting and restriction on a parent
// /*AFLA*/ 	 * cgroup cover those of its children.  If %true, hierarchy support
// /*AFLA*/ 	 * is broken in some ways - some subsystems ignore hierarchy
// /*AFLA*/ 	 * completely while others are only implemented half-way.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * It's now disallowed to create nested cgroups if the subsystem is
// /*AFLA*/ 	 * broken and cgroup core will emit a warning message on such
// /*AFLA*/ 	 * cases.  Eventually, all subsystems will be made properly
// /*AFLA*/ 	 * hierarchical and this will go away.
// /*AFLA*/ 	 */
// /*AFLA*/ 	bool broken_hierarchy:1;
// /*AFLA*/ 	bool warned_broken_hierarchy:1;
// /*AFLA*/ 
// /*AFLA*/ 	/* the following two fields are initialized automtically during boot */
// /*AFLA*/ 	int id;
// /*AFLA*/ 	const char *name;
// /*AFLA*/ 
// /*AFLA*/ 	/* optional, initialized automatically during boot if not set */
// /*AFLA*/ 	const char *legacy_name;
// /*AFLA*/ 
// /*AFLA*/ 	/* link to parent, protected by cgroup_lock() */
// /*AFLA*/ 	struct cgroup_root *root;
// /*AFLA*/ 
// /*AFLA*/ 	/* idr for css->id */
// /*AFLA*/ 	struct idr css_idr;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * List of cftypes.  Each entry is the first entry of an array
// /*AFLA*/ 	 * terminated by zero length name.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_head cfts;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Base cftypes which are automatically registered.  The two can
// /*AFLA*/ 	 * point to the same array.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct cftype *dfl_cftypes;	/* for the default hierarchy */
// /*AFLA*/ 	struct cftype *legacy_cftypes;	/* for the legacy hierarchies */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * A subsystem may depend on other subsystems.  When such subsystem
// /*AFLA*/ 	 * is enabled on a cgroup, the depended-upon subsystems are enabled
// /*AFLA*/ 	 * together if available.  Subsystems enabled due to dependency are
// /*AFLA*/ 	 * not visible to userland until explicitly enabled.  The following
// /*AFLA*/ 	 * specifies the mask of subsystems that this one depends on.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned int depends_on;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct percpu_rw_semaphore cgroup_threadgroup_rwsem;
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cgroup_threadgroup_change_begin - threadgroup exclusion for cgroups
// /*AFLA*/  * @tsk: target task
// /*AFLA*/  *
// /*AFLA*/  * Called from threadgroup_change_begin() and allows cgroup operations to
// /*AFLA*/  * synchronize against threadgroup changes using a percpu_rw_semaphore.
// /*AFLA*/  */
// /*AFLA*/ static inline void cgroup_threadgroup_change_begin(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	percpu_down_read(&cgroup_threadgroup_rwsem);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cgroup_threadgroup_change_end - threadgroup exclusion for cgroups
// /*AFLA*/  * @tsk: target task
// /*AFLA*/  *
// /*AFLA*/  * Called from threadgroup_change_end().  Counterpart of
// /*AFLA*/  * cgroup_threadcgroup_change_begin().
// /*AFLA*/  */
// /*AFLA*/ static inline void cgroup_threadgroup_change_end(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	percpu_up_read(&cgroup_threadgroup_rwsem);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else	/* CONFIG_CGROUPS */
// /*AFLA*/ 
// /*AFLA*/ #define CGROUP_SUBSYS_COUNT 0
// /*AFLA*/ 
// /*AFLA*/ static inline void cgroup_threadgroup_change_begin(struct task_struct *tsk) {}
// /*AFLA*/ static inline void cgroup_threadgroup_change_end(struct task_struct *tsk) {}
// /*AFLA*/ 
// /*AFLA*/ #endif	/* CONFIG_CGROUPS */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SOCK_CGROUP_DATA
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * sock_cgroup_data is embedded at sock->sk_cgrp_data and contains
// /*AFLA*/  * per-socket cgroup information except for memcg association.
// /*AFLA*/  *
// /*AFLA*/  * On legacy hierarchies, net_prio and net_cls controllers directly set
// /*AFLA*/  * attributes on each sock which can then be tested by the network layer.
// /*AFLA*/  * On the default hierarchy, each sock is associated with the cgroup it was
// /*AFLA*/  * created in and the networking layer can match the cgroup directly.
// /*AFLA*/  *
// /*AFLA*/  * To avoid carrying all three cgroup related fields separately in sock,
// /*AFLA*/  * sock_cgroup_data overloads (prioidx, classid) and the cgroup pointer.
// /*AFLA*/  * On boot, sock_cgroup_data records the cgroup that the sock was created
// /*AFLA*/  * in so that cgroup2 matches can be made; however, once either net_prio or
// /*AFLA*/  * net_cls starts being used, the area is overriden to carry prioidx and/or
// /*AFLA*/  * classid.  The two modes are distinguished by whether the lowest bit is
// /*AFLA*/  * set.  Clear bit indicates cgroup pointer while set bit prioidx and
// /*AFLA*/  * classid.
// /*AFLA*/  *
// /*AFLA*/  * While userland may start using net_prio or net_cls at any time, once
// /*AFLA*/  * either is used, cgroup2 matching no longer works.  There is no reason to
// /*AFLA*/  * mix the two and this is in line with how legacy and v2 compatibility is
// /*AFLA*/  * handled.  On mode switch, cgroup references which are already being
// /*AFLA*/  * pointed to by socks may be leaked.  While this can be remedied by adding
// /*AFLA*/  * synchronization around sock_cgroup_data, given that the number of leaked
// /*AFLA*/  * cgroups is bound and highly unlikely to be high, this seems to be the
// /*AFLA*/  * better trade-off.
// /*AFLA*/  */
// /*AFLA*/ struct sock_cgroup_data {
// /*AFLA*/ 	union {
// /*AFLA*/ #ifdef __LITTLE_ENDIAN
// /*AFLA*/ 		struct {
// /*AFLA*/ 			u8	is_data;
// /*AFLA*/ 			u8	padding;
// /*AFLA*/ 			u16	prioidx;
// /*AFLA*/ 			u32	classid;
// /*AFLA*/ 		} __packed;
// /*AFLA*/ #else
// /*AFLA*/ 		struct {
// /*AFLA*/ 			u32	classid;
// /*AFLA*/ 			u16	prioidx;
// /*AFLA*/ 			u8	padding;
// /*AFLA*/ 			u8	is_data;
// /*AFLA*/ 		} __packed;
// /*AFLA*/ #endif
// /*AFLA*/ 		u64		val;
// /*AFLA*/ 	};
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * There's a theoretical window where the following accessors race with
// /*AFLA*/  * updaters and return part of the previous pointer as the prioidx or
// /*AFLA*/  * classid.  Such races are short-lived and the result isn't critical.
// /*AFLA*/  */
// /*AFLA*/ static inline u16 sock_cgroup_prioidx(struct sock_cgroup_data *skcd)
// /*AFLA*/ {
// /*AFLA*/ 	/* fallback to 1 which is always the ID of the root cgroup */
// /*AFLA*/ 	return (skcd->is_data & 1) ? skcd->prioidx : 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u32 sock_cgroup_classid(struct sock_cgroup_data *skcd)
// /*AFLA*/ {
// /*AFLA*/ 	/* fallback to 0 which is the unconfigured default classid */
// /*AFLA*/ 	return (skcd->is_data & 1) ? skcd->classid : 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * If invoked concurrently, the updaters may clobber each other.  The
// /*AFLA*/  * caller is responsible for synchronization.
// /*AFLA*/  */
// /*AFLA*/ static inline void sock_cgroup_set_prioidx(struct sock_cgroup_data *skcd,
// /*AFLA*/ 					   u16 prioidx)
// /*AFLA*/ {
// /*AFLA*/ 	struct sock_cgroup_data skcd_buf = {{ .val = READ_ONCE(skcd->val) }};
// /*AFLA*/ 
// /*AFLA*/ 	if (sock_cgroup_prioidx(&skcd_buf) == prioidx)
// /*AFLA*/ 		return;
// /*AFLA*/ 
// /*AFLA*/ 	if (!(skcd_buf.is_data & 1)) {
// /*AFLA*/ 		skcd_buf.val = 0;
// /*AFLA*/ 		skcd_buf.is_data = 1;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	skcd_buf.prioidx = prioidx;
// /*AFLA*/ 	WRITE_ONCE(skcd->val, skcd_buf.val);	/* see sock_cgroup_ptr() */
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sock_cgroup_set_classid(struct sock_cgroup_data *skcd,
// /*AFLA*/ 					   u32 classid)
// /*AFLA*/ {
// /*AFLA*/ 	struct sock_cgroup_data skcd_buf = {{ .val = READ_ONCE(skcd->val) }};
// /*AFLA*/ 
// /*AFLA*/ 	if (sock_cgroup_classid(&skcd_buf) == classid)
// /*AFLA*/ 		return;
// /*AFLA*/ 
// /*AFLA*/ 	if (!(skcd_buf.is_data & 1)) {
// /*AFLA*/ 		skcd_buf.val = 0;
// /*AFLA*/ 		skcd_buf.is_data = 1;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	skcd_buf.classid = classid;
// /*AFLA*/ 	WRITE_ONCE(skcd->val, skcd_buf.val);	/* see sock_cgroup_ptr() */
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else	/* CONFIG_SOCK_CGROUP_DATA */
// /*AFLA*/ 
// /*AFLA*/ struct sock_cgroup_data {
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif	/* CONFIG_SOCK_CGROUP_DATA */
// /*AFLA*/ 
// /*AFLA*/ #endif	/* _LINUX_CGROUP_DEFS_H */
