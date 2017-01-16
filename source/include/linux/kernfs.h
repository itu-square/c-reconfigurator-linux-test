// /*AFLA*/ /*
// /*AFLA*/  * kernfs.h - pseudo filesystem decoupled from vfs locking
// /*AFLA*/  *
// /*AFLA*/  * This file is released under the GPLv2.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __LINUX_KERNFS_H
// /*AFLA*/ #define __LINUX_KERNFS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/err.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/mutex.h>
// /*AFLA*/ #include <linux/idr.h>
// /*AFLA*/ #include <linux/lockdep.h>
// /*AFLA*/ #include <linux/rbtree.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/wait.h>
// /*AFLA*/ 
// /*AFLA*/ struct file;
// /*AFLA*/ struct dentry;
// /*AFLA*/ struct iattr;
// /*AFLA*/ struct seq_file;
// /*AFLA*/ struct vm_area_struct;
// /*AFLA*/ struct super_block;
// /*AFLA*/ struct file_system_type;
// /*AFLA*/ 
// /*AFLA*/ struct kernfs_open_node;
// /*AFLA*/ struct kernfs_iattrs;
// /*AFLA*/ 
// /*AFLA*/ enum kernfs_node_type {
// /*AFLA*/ 	KERNFS_DIR		= 0x0001,
// /*AFLA*/ 	KERNFS_FILE		= 0x0002,
// /*AFLA*/ 	KERNFS_LINK		= 0x0004,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define KERNFS_TYPE_MASK	0x000f
// /*AFLA*/ #define KERNFS_FLAG_MASK	~KERNFS_TYPE_MASK
// /*AFLA*/ 
// /*AFLA*/ enum kernfs_node_flag {
// /*AFLA*/ 	KERNFS_ACTIVATED	= 0x0010,
// /*AFLA*/ 	KERNFS_NS		= 0x0020,
// /*AFLA*/ 	KERNFS_HAS_SEQ_SHOW	= 0x0040,
// /*AFLA*/ 	KERNFS_HAS_MMAP		= 0x0080,
// /*AFLA*/ 	KERNFS_LOCKDEP		= 0x0100,
// /*AFLA*/ 	KERNFS_SUICIDAL		= 0x0400,
// /*AFLA*/ 	KERNFS_SUICIDED		= 0x0800,
// /*AFLA*/ 	KERNFS_EMPTY_DIR	= 0x1000,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* @flags for kernfs_create_root() */
// /*AFLA*/ enum kernfs_root_flag {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * kernfs_nodes are created in the deactivated state and invisible.
// /*AFLA*/ 	 * They require explicit kernfs_activate() to become visible.  This
// /*AFLA*/ 	 * can be used to make related nodes become visible atomically
// /*AFLA*/ 	 * after all nodes are created successfully.
// /*AFLA*/ 	 */
// /*AFLA*/ 	KERNFS_ROOT_CREATE_DEACTIVATED		= 0x0001,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * For regular flies, if the opener has CAP_DAC_OVERRIDE, open(2)
// /*AFLA*/ 	 * succeeds regardless of the RW permissions.  sysfs had an extra
// /*AFLA*/ 	 * layer of enforcement where open(2) fails with -EACCES regardless
// /*AFLA*/ 	 * of CAP_DAC_OVERRIDE if the permission doesn't have the
// /*AFLA*/ 	 * respective read or write access at all (none of S_IRUGO or
// /*AFLA*/ 	 * S_IWUGO) or the respective operation isn't implemented.  The
// /*AFLA*/ 	 * following flag enables that behavior.
// /*AFLA*/ 	 */
// /*AFLA*/ 	KERNFS_ROOT_EXTRA_OPEN_PERM_CHECK	= 0x0002,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* type-specific structures for kernfs_node union members */
// /*AFLA*/ struct kernfs_elem_dir {
// /*AFLA*/ 	unsigned long		subdirs;
// /*AFLA*/ 	/* children rbtree starts here and goes through kn->rb */
// /*AFLA*/ 	struct rb_root		children;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The kernfs hierarchy this directory belongs to.  This fits
// /*AFLA*/ 	 * better directly in kernfs_node but is here to save space.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct kernfs_root	*root;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct kernfs_elem_symlink {
// /*AFLA*/ 	struct kernfs_node	*target_kn;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct kernfs_elem_attr {
// /*AFLA*/ 	const struct kernfs_ops	*ops;
// /*AFLA*/ 	struct kernfs_open_node	*open;
// /*AFLA*/ 	loff_t			size;
// /*AFLA*/ 	struct kernfs_node	*notify_next;	/* for kernfs_notify() */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * kernfs_node - the building block of kernfs hierarchy.  Each and every
// /*AFLA*/  * kernfs node is represented by single kernfs_node.  Most fields are
// /*AFLA*/  * private to kernfs and shouldn't be accessed directly by kernfs users.
// /*AFLA*/  *
// /*AFLA*/  * As long as s_count reference is held, the kernfs_node itself is
// /*AFLA*/  * accessible.  Dereferencing elem or any other outer entity requires
// /*AFLA*/  * active reference.
// /*AFLA*/  */
// /*AFLA*/ struct kernfs_node {
// /*AFLA*/ 	atomic_t		count;
// /*AFLA*/ 	atomic_t		active;
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ 	struct lockdep_map	dep_map;
// /*AFLA*/ #endif
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Use kernfs_get_parent() and kernfs_name/path() instead of
// /*AFLA*/ 	 * accessing the following two fields directly.  If the node is
// /*AFLA*/ 	 * never moved to a different parent, it is safe to access the
// /*AFLA*/ 	 * parent directly.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct kernfs_node	*parent;
// /*AFLA*/ 	const char		*name;
// /*AFLA*/ 
// /*AFLA*/ 	struct rb_node		rb;
// /*AFLA*/ 
// /*AFLA*/ 	const void		*ns;	/* namespace tag */
// /*AFLA*/ 	unsigned int		hash;	/* ns + name hash */
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct kernfs_elem_dir		dir;
// /*AFLA*/ 		struct kernfs_elem_symlink	symlink;
// /*AFLA*/ 		struct kernfs_elem_attr		attr;
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ 	void			*priv;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned short		flags;
// /*AFLA*/ 	umode_t			mode;
// /*AFLA*/ 	unsigned int		ino;
// /*AFLA*/ 	struct kernfs_iattrs	*iattr;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * kernfs_syscall_ops may be specified on kernfs_create_root() to support
// /*AFLA*/  * syscalls.  These optional callbacks are invoked on the matching syscalls
// /*AFLA*/  * and can perform any kernfs operations which don't necessarily have to be
// /*AFLA*/  * the exact operation requested.  An active reference is held for each
// /*AFLA*/  * kernfs_node parameter.
// /*AFLA*/  */
// /*AFLA*/ struct kernfs_syscall_ops {
// /*AFLA*/ 	int (*remount_fs)(struct kernfs_root *root, int *flags, char *data);
// /*AFLA*/ 	int (*show_options)(struct seq_file *sf, struct kernfs_root *root);
// /*AFLA*/ 
// /*AFLA*/ 	int (*mkdir)(struct kernfs_node *parent, const char *name,
// /*AFLA*/ 		     umode_t mode);
// /*AFLA*/ 	int (*rmdir)(struct kernfs_node *kn);
// /*AFLA*/ 	int (*rename)(struct kernfs_node *kn, struct kernfs_node *new_parent,
// /*AFLA*/ 		      const char *new_name);
// /*AFLA*/ 	int (*show_path)(struct seq_file *sf, struct kernfs_node *kn,
// /*AFLA*/ 			 struct kernfs_root *root);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct kernfs_root {
// /*AFLA*/ 	/* published fields */
// /*AFLA*/ 	struct kernfs_node	*kn;
// /*AFLA*/ 	unsigned int		flags;	/* KERNFS_ROOT_* flags */
// /*AFLA*/ 
// /*AFLA*/ 	/* private fields, do not use outside kernfs proper */
// /*AFLA*/ 	struct ida		ino_ida;
// /*AFLA*/ 	struct kernfs_syscall_ops *syscall_ops;
// /*AFLA*/ 
// /*AFLA*/ 	/* list of kernfs_super_info of this root, protected by kernfs_mutex */
// /*AFLA*/ 	struct list_head	supers;
// /*AFLA*/ 
// /*AFLA*/ 	wait_queue_head_t	deactivate_waitq;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct kernfs_open_file {
// /*AFLA*/ 	/* published fields */
// /*AFLA*/ 	struct kernfs_node	*kn;
// /*AFLA*/ 	struct file		*file;
// /*AFLA*/ 	void			*priv;
// /*AFLA*/ 
// /*AFLA*/ 	/* private fields, do not use outside kernfs proper */
// /*AFLA*/ 	struct mutex		mutex;
// /*AFLA*/ 	struct mutex		prealloc_mutex;
// /*AFLA*/ 	int			event;
// /*AFLA*/ 	struct list_head	list;
// /*AFLA*/ 	char			*prealloc_buf;
// /*AFLA*/ 
// /*AFLA*/ 	size_t			atomic_write_len;
// /*AFLA*/ 	bool			mmapped;
// /*AFLA*/ 	const struct vm_operations_struct *vm_ops;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct kernfs_ops {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Read is handled by either seq_file or raw_read().
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * If seq_show() is present, seq_file path is active.  Other seq
// /*AFLA*/ 	 * operations are optional and if not implemented, the behavior is
// /*AFLA*/ 	 * equivalent to single_open().  @sf->private points to the
// /*AFLA*/ 	 * associated kernfs_open_file.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * read() is bounced through kernel buffer and a read larger than
// /*AFLA*/ 	 * PAGE_SIZE results in partial operation of PAGE_SIZE.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int (*seq_show)(struct seq_file *sf, void *v);
// /*AFLA*/ 
// /*AFLA*/ 	void *(*seq_start)(struct seq_file *sf, loff_t *ppos);
// /*AFLA*/ 	void *(*seq_next)(struct seq_file *sf, void *v, loff_t *ppos);
// /*AFLA*/ 	void (*seq_stop)(struct seq_file *sf, void *v);
// /*AFLA*/ 
// /*AFLA*/ 	ssize_t (*read)(struct kernfs_open_file *of, char *buf, size_t bytes,
// /*AFLA*/ 			loff_t off);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * write() is bounced through kernel buffer.  If atomic_write_len
// /*AFLA*/ 	 * is not set, a write larger than PAGE_SIZE results in partial
// /*AFLA*/ 	 * operations of PAGE_SIZE chunks.  If atomic_write_len is set,
// /*AFLA*/ 	 * writes upto the specified size are executed atomically but
// /*AFLA*/ 	 * larger ones are rejected with -E2BIG.
// /*AFLA*/ 	 */
// /*AFLA*/ 	size_t atomic_write_len;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * "prealloc" causes a buffer to be allocated at open for
// /*AFLA*/ 	 * all read/write requests.  As ->seq_show uses seq_read()
// /*AFLA*/ 	 * which does its own allocation, it is incompatible with
// /*AFLA*/ 	 * ->prealloc.  Provide ->read and ->write with ->prealloc.
// /*AFLA*/ 	 */
// /*AFLA*/ 	bool prealloc;
// /*AFLA*/ 	ssize_t (*write)(struct kernfs_open_file *of, char *buf, size_t bytes,
// /*AFLA*/ 			 loff_t off);
// /*AFLA*/ 
// /*AFLA*/ 	int (*mmap)(struct kernfs_open_file *of, struct vm_area_struct *vma);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ 	struct lock_class_key	lockdep_key;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KERNFS
// /*AFLA*/ 
// /*AFLA*/ static inline enum kernfs_node_type kernfs_type(struct kernfs_node *kn)
// /*AFLA*/ {
// /*AFLA*/ 	return kn->flags & KERNFS_TYPE_MASK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kernfs_enable_ns - enable namespace under a directory
// /*AFLA*/  * @kn: directory of interest, should be empty
// /*AFLA*/  *
// /*AFLA*/  * This is to be called right after @kn is created to enable namespace
// /*AFLA*/  * under it.  All children of @kn must have non-NULL namespace tags and
// /*AFLA*/  * only the ones which match the super_block's tag will be visible.
// /*AFLA*/  */
// /*AFLA*/ static inline void kernfs_enable_ns(struct kernfs_node *kn)
// /*AFLA*/ {
// /*AFLA*/ 	WARN_ON_ONCE(kernfs_type(kn) != KERNFS_DIR);
// /*AFLA*/ 	WARN_ON_ONCE(!RB_EMPTY_ROOT(&kn->dir.children));
// /*AFLA*/ 	kn->flags |= KERNFS_NS;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kernfs_ns_enabled - test whether namespace is enabled
// /*AFLA*/  * @kn: the node to test
// /*AFLA*/  *
// /*AFLA*/  * Test whether namespace filtering is enabled for the children of @ns.
// /*AFLA*/  */
// /*AFLA*/ static inline bool kernfs_ns_enabled(struct kernfs_node *kn)
// /*AFLA*/ {
// /*AFLA*/ 	return kn->flags & KERNFS_NS;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ int kernfs_name(struct kernfs_node *kn, char *buf, size_t buflen);
// /*AFLA*/ int kernfs_path_from_node(struct kernfs_node *root_kn, struct kernfs_node *kn,
// /*AFLA*/ 			  char *buf, size_t buflen);
// /*AFLA*/ void pr_cont_kernfs_name(struct kernfs_node *kn);
// /*AFLA*/ void pr_cont_kernfs_path(struct kernfs_node *kn);
// /*AFLA*/ struct kernfs_node *kernfs_get_parent(struct kernfs_node *kn);
// /*AFLA*/ struct kernfs_node *kernfs_find_and_get_ns(struct kernfs_node *parent,
// /*AFLA*/ 					   const char *name, const void *ns);
// /*AFLA*/ struct kernfs_node *kernfs_walk_and_get_ns(struct kernfs_node *parent,
// /*AFLA*/ 					   const char *path, const void *ns);
// /*AFLA*/ void kernfs_get(struct kernfs_node *kn);
// /*AFLA*/ void kernfs_put(struct kernfs_node *kn);
// /*AFLA*/ 
// /*AFLA*/ struct kernfs_node *kernfs_node_from_dentry(struct dentry *dentry);
// /*AFLA*/ struct kernfs_root *kernfs_root_from_sb(struct super_block *sb);
// /*AFLA*/ struct inode *kernfs_get_inode(struct super_block *sb, struct kernfs_node *kn);
// /*AFLA*/ 
// /*AFLA*/ struct dentry *kernfs_node_dentry(struct kernfs_node *kn,
// /*AFLA*/ 				  struct super_block *sb);
// /*AFLA*/ struct kernfs_root *kernfs_create_root(struct kernfs_syscall_ops *scops,
// /*AFLA*/ 				       unsigned int flags, void *priv);
// /*AFLA*/ void kernfs_destroy_root(struct kernfs_root *root);
// /*AFLA*/ 
// /*AFLA*/ struct kernfs_node *kernfs_create_dir_ns(struct kernfs_node *parent,
// /*AFLA*/ 					 const char *name, umode_t mode,
// /*AFLA*/ 					 void *priv, const void *ns);
// /*AFLA*/ struct kernfs_node *kernfs_create_empty_dir(struct kernfs_node *parent,
// /*AFLA*/ 					    const char *name);
// /*AFLA*/ struct kernfs_node *__kernfs_create_file(struct kernfs_node *parent,
// /*AFLA*/ 					 const char *name,
// /*AFLA*/ 					 umode_t mode, loff_t size,
// /*AFLA*/ 					 const struct kernfs_ops *ops,
// /*AFLA*/ 					 void *priv, const void *ns,
// /*AFLA*/ 					 struct lock_class_key *key);
// /*AFLA*/ struct kernfs_node *kernfs_create_link(struct kernfs_node *parent,
// /*AFLA*/ 				       const char *name,
// /*AFLA*/ 				       struct kernfs_node *target);
// /*AFLA*/ void kernfs_activate(struct kernfs_node *kn);
// /*AFLA*/ void kernfs_remove(struct kernfs_node *kn);
// /*AFLA*/ void kernfs_break_active_protection(struct kernfs_node *kn);
// /*AFLA*/ void kernfs_unbreak_active_protection(struct kernfs_node *kn);
// /*AFLA*/ bool kernfs_remove_self(struct kernfs_node *kn);
// /*AFLA*/ int kernfs_remove_by_name_ns(struct kernfs_node *parent, const char *name,
// /*AFLA*/ 			     const void *ns);
// /*AFLA*/ int kernfs_rename_ns(struct kernfs_node *kn, struct kernfs_node *new_parent,
// /*AFLA*/ 		     const char *new_name, const void *new_ns);
// /*AFLA*/ int kernfs_setattr(struct kernfs_node *kn, const struct iattr *iattr);
// /*AFLA*/ void kernfs_notify(struct kernfs_node *kn);
// /*AFLA*/ 
// /*AFLA*/ const void *kernfs_super_ns(struct super_block *sb);
// /*AFLA*/ struct dentry *kernfs_mount_ns(struct file_system_type *fs_type, int flags,
// /*AFLA*/ 			       struct kernfs_root *root, unsigned long magic,
// /*AFLA*/ 			       bool *new_sb_created, const void *ns);
// /*AFLA*/ void kernfs_kill_sb(struct super_block *sb);
// /*AFLA*/ struct super_block *kernfs_pin_sb(struct kernfs_root *root, const void *ns);
// /*AFLA*/ 
// /*AFLA*/ void kernfs_init(void);
// /*AFLA*/ 
// /*AFLA*/ #else	/* CONFIG_KERNFS */
// /*AFLA*/ 
// /*AFLA*/ static inline enum kernfs_node_type kernfs_type(struct kernfs_node *kn)
// /*AFLA*/ { return 0; }	/* whatever */
// /*AFLA*/ 
// /*AFLA*/ static inline void kernfs_enable_ns(struct kernfs_node *kn) { }
// /*AFLA*/ 
// /*AFLA*/ static inline bool kernfs_ns_enabled(struct kernfs_node *kn)
// /*AFLA*/ { return false; }
// /*AFLA*/ 
// /*AFLA*/ static inline int kernfs_name(struct kernfs_node *kn, char *buf, size_t buflen)
// /*AFLA*/ { return -ENOSYS; }
// /*AFLA*/ 
// /*AFLA*/ static inline int kernfs_path_from_node(struct kernfs_node *root_kn,
// /*AFLA*/ 					struct kernfs_node *kn,
// /*AFLA*/ 					char *buf, size_t buflen)
// /*AFLA*/ { return -ENOSYS; }
// /*AFLA*/ 
// /*AFLA*/ static inline void pr_cont_kernfs_name(struct kernfs_node *kn) { }
// /*AFLA*/ static inline void pr_cont_kernfs_path(struct kernfs_node *kn) { }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kernfs_node *kernfs_get_parent(struct kernfs_node *kn)
// /*AFLA*/ { return NULL; }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kernfs_node *
// /*AFLA*/ kernfs_find_and_get_ns(struct kernfs_node *parent, const char *name,
// /*AFLA*/ 		       const void *ns)
// /*AFLA*/ { return NULL; }
// /*AFLA*/ static inline struct kernfs_node *
// /*AFLA*/ kernfs_walk_and_get_ns(struct kernfs_node *parent, const char *path,
// /*AFLA*/ 		       const void *ns)
// /*AFLA*/ { return NULL; }
// /*AFLA*/ 
// /*AFLA*/ static inline void kernfs_get(struct kernfs_node *kn) { }
// /*AFLA*/ static inline void kernfs_put(struct kernfs_node *kn) { }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kernfs_node *kernfs_node_from_dentry(struct dentry *dentry)
// /*AFLA*/ { return NULL; }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kernfs_root *kernfs_root_from_sb(struct super_block *sb)
// /*AFLA*/ { return NULL; }
// /*AFLA*/ 
// /*AFLA*/ static inline struct inode *
// /*AFLA*/ kernfs_get_inode(struct super_block *sb, struct kernfs_node *kn)
// /*AFLA*/ { return NULL; }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kernfs_root *
// /*AFLA*/ kernfs_create_root(struct kernfs_syscall_ops *scops, unsigned int flags,
// /*AFLA*/ 		   void *priv)
// /*AFLA*/ { return ERR_PTR(-ENOSYS); }
// /*AFLA*/ 
// /*AFLA*/ static inline void kernfs_destroy_root(struct kernfs_root *root) { }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kernfs_node *
// /*AFLA*/ kernfs_create_dir_ns(struct kernfs_node *parent, const char *name,
// /*AFLA*/ 		     umode_t mode, void *priv, const void *ns)
// /*AFLA*/ { return ERR_PTR(-ENOSYS); }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kernfs_node *
// /*AFLA*/ __kernfs_create_file(struct kernfs_node *parent, const char *name,
// /*AFLA*/ 		     umode_t mode, loff_t size, const struct kernfs_ops *ops,
// /*AFLA*/ 		     void *priv, const void *ns, struct lock_class_key *key)
// /*AFLA*/ { return ERR_PTR(-ENOSYS); }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kernfs_node *
// /*AFLA*/ kernfs_create_link(struct kernfs_node *parent, const char *name,
// /*AFLA*/ 		   struct kernfs_node *target)
// /*AFLA*/ { return ERR_PTR(-ENOSYS); }
// /*AFLA*/ 
// /*AFLA*/ static inline void kernfs_activate(struct kernfs_node *kn) { }
// /*AFLA*/ 
// /*AFLA*/ static inline void kernfs_remove(struct kernfs_node *kn) { }
// /*AFLA*/ 
// /*AFLA*/ static inline bool kernfs_remove_self(struct kernfs_node *kn)
// /*AFLA*/ { return false; }
// /*AFLA*/ 
// /*AFLA*/ static inline int kernfs_remove_by_name_ns(struct kernfs_node *kn,
// /*AFLA*/ 					   const char *name, const void *ns)
// /*AFLA*/ { return -ENOSYS; }
// /*AFLA*/ 
// /*AFLA*/ static inline int kernfs_rename_ns(struct kernfs_node *kn,
// /*AFLA*/ 				   struct kernfs_node *new_parent,
// /*AFLA*/ 				   const char *new_name, const void *new_ns)
// /*AFLA*/ { return -ENOSYS; }
// /*AFLA*/ 
// /*AFLA*/ static inline int kernfs_setattr(struct kernfs_node *kn,
// /*AFLA*/ 				 const struct iattr *iattr)
// /*AFLA*/ { return -ENOSYS; }
// /*AFLA*/ 
// /*AFLA*/ static inline void kernfs_notify(struct kernfs_node *kn) { }
// /*AFLA*/ 
// /*AFLA*/ static inline const void *kernfs_super_ns(struct super_block *sb)
// /*AFLA*/ { return NULL; }
// /*AFLA*/ 
// /*AFLA*/ static inline struct dentry *
// /*AFLA*/ kernfs_mount_ns(struct file_system_type *fs_type, int flags,
// /*AFLA*/ 		struct kernfs_root *root, unsigned long magic,
// /*AFLA*/ 		bool *new_sb_created, const void *ns)
// /*AFLA*/ { return ERR_PTR(-ENOSYS); }
// /*AFLA*/ 
// /*AFLA*/ static inline void kernfs_kill_sb(struct super_block *sb) { }
// /*AFLA*/ 
// /*AFLA*/ static inline void kernfs_init(void) { }
// /*AFLA*/ 
// /*AFLA*/ #endif	/* CONFIG_KERNFS */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kernfs_path - build full path of a given node
// /*AFLA*/  * @kn: kernfs_node of interest
// /*AFLA*/  * @buf: buffer to copy @kn's name into
// /*AFLA*/  * @buflen: size of @buf
// /*AFLA*/  *
// /*AFLA*/  * Builds and returns the full path of @kn in @buf of @buflen bytes.  The
// /*AFLA*/  * path is built from the end of @buf so the returned pointer usually
// /*AFLA*/  * doesn't match @buf.  If @buf isn't long enough, @buf is nul terminated
// /*AFLA*/  * and %NULL is returned.
// /*AFLA*/  */
// /*AFLA*/ static inline int kernfs_path(struct kernfs_node *kn, char *buf, size_t buflen)
// /*AFLA*/ {
// /*AFLA*/ 	return kernfs_path_from_node(kn, NULL, buf, buflen);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kernfs_node *
// /*AFLA*/ kernfs_find_and_get(struct kernfs_node *kn, const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	return kernfs_find_and_get_ns(kn, name, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kernfs_node *
// /*AFLA*/ kernfs_walk_and_get(struct kernfs_node *kn, const char *path)
// /*AFLA*/ {
// /*AFLA*/ 	return kernfs_walk_and_get_ns(kn, path, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kernfs_node *
// /*AFLA*/ kernfs_create_dir(struct kernfs_node *parent, const char *name, umode_t mode,
// /*AFLA*/ 		  void *priv)
// /*AFLA*/ {
// /*AFLA*/ 	return kernfs_create_dir_ns(parent, name, mode, priv, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kernfs_node *
// /*AFLA*/ kernfs_create_file_ns(struct kernfs_node *parent, const char *name,
// /*AFLA*/ 		      umode_t mode, loff_t size, const struct kernfs_ops *ops,
// /*AFLA*/ 		      void *priv, const void *ns)
// /*AFLA*/ {
// /*AFLA*/ 	struct lock_class_key *key = NULL;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ 	key = (struct lock_class_key *)&ops->lockdep_key;
// /*AFLA*/ #endif
// /*AFLA*/ 	return __kernfs_create_file(parent, name, mode, size, ops, priv, ns,
// /*AFLA*/ 				    key);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kernfs_node *
// /*AFLA*/ kernfs_create_file(struct kernfs_node *parent, const char *name, umode_t mode,
// /*AFLA*/ 		   loff_t size, const struct kernfs_ops *ops, void *priv)
// /*AFLA*/ {
// /*AFLA*/ 	return kernfs_create_file_ns(parent, name, mode, size, ops, priv, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int kernfs_remove_by_name(struct kernfs_node *parent,
// /*AFLA*/ 					const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	return kernfs_remove_by_name_ns(parent, name, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int kernfs_rename(struct kernfs_node *kn,
// /*AFLA*/ 				struct kernfs_node *new_parent,
// /*AFLA*/ 				const char *new_name)
// /*AFLA*/ {
// /*AFLA*/ 	return kernfs_rename_ns(kn, new_parent, new_name, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct dentry *
// /*AFLA*/ kernfs_mount(struct file_system_type *fs_type, int flags,
// /*AFLA*/ 		struct kernfs_root *root, unsigned long magic,
// /*AFLA*/ 		bool *new_sb_created)
// /*AFLA*/ {
// /*AFLA*/ 	return kernfs_mount_ns(fs_type, flags, root,
// /*AFLA*/ 				magic, new_sb_created, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif	/* __LINUX_KERNFS_H */
