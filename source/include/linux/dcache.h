// /*AFLA*/ #ifndef __LINUX_DCACHE_H
// /*AFLA*/ #define __LINUX_DCACHE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/rculist.h>
// /*AFLA*/ #include <linux/rculist_bl.h>
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/seqlock.h>
// /*AFLA*/ #include <linux/cache.h>
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ #include <linux/lockref.h>
// /*AFLA*/ #include <linux/stringhash.h>
// /*AFLA*/ 
// /*AFLA*/ struct path;
// /*AFLA*/ struct vfsmount;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * linux/include/linux/dcache.h
// /*AFLA*/  *
// /*AFLA*/  * Dirent cache data structures
// /*AFLA*/  *
// /*AFLA*/  * (C) Copyright 1997 Thomas Schoebel-Theuer,
// /*AFLA*/  * with heavy changes by Linus Torvalds
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define IS_ROOT(x) ((x) == (x)->d_parent)
// /*AFLA*/ 
// /*AFLA*/ /* The hash is always the low bits of hash_len */
// /*AFLA*/ #ifdef __LITTLE_ENDIAN
// /*AFLA*/  #define HASH_LEN_DECLARE u32 hash; u32 len
// /*AFLA*/  #define bytemask_from_count(cnt)	(~(~0ul << (cnt)*8))
// /*AFLA*/ #else
// /*AFLA*/  #define HASH_LEN_DECLARE u32 len; u32 hash
// /*AFLA*/  #define bytemask_from_count(cnt)	(~(~0ul >> (cnt)*8))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * "quick string" -- eases parameter passing, but more importantly
// /*AFLA*/  * saves "metadata" about the string (ie length and the hash).
// /*AFLA*/  *
// /*AFLA*/  * hash comes first so it snuggles against d_parent in the
// /*AFLA*/  * dentry.
// /*AFLA*/  */
// /*AFLA*/ struct qstr {
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct {
// /*AFLA*/ 			HASH_LEN_DECLARE;
// /*AFLA*/ 		};
// /*AFLA*/ 		u64 hash_len;
// /*AFLA*/ 	};
// /*AFLA*/ 	const unsigned char *name;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define QSTR_INIT(n,l) { { { .len = l } }, .name = n }
// /*AFLA*/ 
// /*AFLA*/ struct dentry_stat_t {
// /*AFLA*/ 	long nr_dentry;
// /*AFLA*/ 	long nr_unused;
// /*AFLA*/ 	long age_limit;          /* age in seconds */
// /*AFLA*/ 	long want_pages;         /* pages requested by system */
// /*AFLA*/ 	long dummy[2];
// /*AFLA*/ };
// /*AFLA*/ extern struct dentry_stat_t dentry_stat;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Try to keep struct dentry aligned on 64 byte cachelines (this will
// /*AFLA*/  * give reasonable cacheline footprint with larger lines without the
// /*AFLA*/  * large memory footprint increase).
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_64BIT
// /*AFLA*/ # define DNAME_INLINE_LEN 32 /* 192 bytes */
// /*AFLA*/ #else
// /*AFLA*/ # ifdef CONFIG_SMP
// /*AFLA*/ #  define DNAME_INLINE_LEN 36 /* 128 bytes */
// /*AFLA*/ # else
// /*AFLA*/ #  define DNAME_INLINE_LEN 40 /* 128 bytes */
// /*AFLA*/ # endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define d_lock	d_lockref.lock
// /*AFLA*/ 
// /*AFLA*/ struct dentry {
// /*AFLA*/ 	/* RCU lookup touched fields */
// /*AFLA*/ 	unsigned int d_flags;		/* protected by d_lock */
// /*AFLA*/ 	seqcount_t d_seq;		/* per dentry seqlock */
// /*AFLA*/ 	struct hlist_bl_node d_hash;	/* lookup hash list */
// /*AFLA*/ 	struct dentry *d_parent;	/* parent directory */
// /*AFLA*/ 	struct qstr d_name;
// /*AFLA*/ 	struct inode *d_inode;		/* Where the name belongs to - NULL is
// /*AFLA*/ 					 * negative */
// /*AFLA*/ 	unsigned char d_iname[DNAME_INLINE_LEN];	/* small names */
// /*AFLA*/ 
// /*AFLA*/ 	/* Ref lookup also touches following */
// /*AFLA*/ 	struct lockref d_lockref;	/* per-dentry lock and refcount */
// /*AFLA*/ 	const struct dentry_operations *d_op;
// /*AFLA*/ 	struct super_block *d_sb;	/* The root of the dentry tree */
// /*AFLA*/ 	unsigned long d_time;		/* used by d_revalidate */
// /*AFLA*/ 	void *d_fsdata;			/* fs-specific data */
// /*AFLA*/ 
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct list_head d_lru;		/* LRU list */
// /*AFLA*/ 		wait_queue_head_t *d_wait;	/* in-lookup ones only */
// /*AFLA*/ 	};
// /*AFLA*/ 	struct list_head d_child;	/* child of parent list */
// /*AFLA*/ 	struct list_head d_subdirs;	/* our children */
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * d_alias and d_rcu can share memory
// /*AFLA*/ 	 */
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct hlist_node d_alias;	/* inode alias list */
// /*AFLA*/ 		struct hlist_bl_node d_in_lookup_hash;	/* only for in-lookup ones */
// /*AFLA*/ 	 	struct rcu_head d_rcu;
// /*AFLA*/ 	} d_u;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * dentry->d_lock spinlock nesting subclasses:
// /*AFLA*/  *
// /*AFLA*/  * 0: normal
// /*AFLA*/  * 1: nested
// /*AFLA*/  */
// /*AFLA*/ enum dentry_d_lock_class
// /*AFLA*/ {
// /*AFLA*/ 	DENTRY_D_LOCK_NORMAL, /* implicitly used by plain spin_lock() APIs. */
// /*AFLA*/ 	DENTRY_D_LOCK_NESTED
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct dentry_operations {
// /*AFLA*/ 	int (*d_revalidate)(struct dentry *, unsigned int);
// /*AFLA*/ 	int (*d_weak_revalidate)(struct dentry *, unsigned int);
// /*AFLA*/ 	int (*d_hash)(const struct dentry *, struct qstr *);
// /*AFLA*/ 	int (*d_compare)(const struct dentry *,
// /*AFLA*/ 			unsigned int, const char *, const struct qstr *);
// /*AFLA*/ 	int (*d_delete)(const struct dentry *);
// /*AFLA*/ 	int (*d_init)(struct dentry *);
// /*AFLA*/ 	void (*d_release)(struct dentry *);
// /*AFLA*/ 	void (*d_prune)(struct dentry *);
// /*AFLA*/ 	void (*d_iput)(struct dentry *, struct inode *);
// /*AFLA*/ 	char *(*d_dname)(struct dentry *, char *, int);
// /*AFLA*/ 	struct vfsmount *(*d_automount)(struct path *);
// /*AFLA*/ 	int (*d_manage)(struct dentry *, bool);
// /*AFLA*/ 	struct dentry *(*d_real)(struct dentry *, const struct inode *,
// /*AFLA*/ 				 unsigned int);
// /*AFLA*/ } ____cacheline_aligned;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Locking rules for dentry_operations callbacks are to be found in
// /*AFLA*/  * Documentation/filesystems/Locking. Keep it updated!
// /*AFLA*/  *
// /*AFLA*/  * FUrther descriptions are found in Documentation/filesystems/vfs.txt.
// /*AFLA*/  * Keep it updated too!
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* d_flags entries */
// /*AFLA*/ #define DCACHE_OP_HASH			0x00000001
// /*AFLA*/ #define DCACHE_OP_COMPARE		0x00000002
// /*AFLA*/ #define DCACHE_OP_REVALIDATE		0x00000004
// /*AFLA*/ #define DCACHE_OP_DELETE		0x00000008
// /*AFLA*/ #define DCACHE_OP_PRUNE			0x00000010
// /*AFLA*/ 
// /*AFLA*/ #define	DCACHE_DISCONNECTED		0x00000020
// /*AFLA*/      /* This dentry is possibly not currently connected to the dcache tree, in
// /*AFLA*/       * which case its parent will either be itself, or will have this flag as
// /*AFLA*/       * well.  nfsd will not use a dentry with this bit set, but will first
// /*AFLA*/       * endeavour to clear the bit either by discovering that it is connected,
// /*AFLA*/       * or by performing lookup operations.   Any filesystem which supports
// /*AFLA*/       * nfsd_operations MUST have a lookup function which, if it finds a
// /*AFLA*/       * directory inode with a DCACHE_DISCONNECTED dentry, will d_move that
// /*AFLA*/       * dentry into place and return that dentry rather than the passed one,
// /*AFLA*/       * typically using d_splice_alias. */
// /*AFLA*/ 
// /*AFLA*/ #define DCACHE_REFERENCED		0x00000040 /* Recently used, don't discard. */
// /*AFLA*/ #define DCACHE_RCUACCESS		0x00000080 /* Entry has ever been RCU-visible */
// /*AFLA*/ 
// /*AFLA*/ #define DCACHE_CANT_MOUNT		0x00000100
// /*AFLA*/ #define DCACHE_GENOCIDE			0x00000200
// /*AFLA*/ #define DCACHE_SHRINK_LIST		0x00000400
// /*AFLA*/ 
// /*AFLA*/ #define DCACHE_OP_WEAK_REVALIDATE	0x00000800
// /*AFLA*/ 
// /*AFLA*/ #define DCACHE_NFSFS_RENAMED		0x00001000
// /*AFLA*/      /* this dentry has been "silly renamed" and has to be deleted on the last
// /*AFLA*/       * dput() */
// /*AFLA*/ #define DCACHE_COOKIE			0x00002000 /* For use by dcookie subsystem */
// /*AFLA*/ #define DCACHE_FSNOTIFY_PARENT_WATCHED	0x00004000
// /*AFLA*/      /* Parent inode is watched by some fsnotify listener */
// /*AFLA*/ 
// /*AFLA*/ #define DCACHE_DENTRY_KILLED		0x00008000
// /*AFLA*/ 
// /*AFLA*/ #define DCACHE_MOUNTED			0x00010000 /* is a mountpoint */
// /*AFLA*/ #define DCACHE_NEED_AUTOMOUNT		0x00020000 /* handle automount on this dir */
// /*AFLA*/ #define DCACHE_MANAGE_TRANSIT		0x00040000 /* manage transit from this dirent */
// /*AFLA*/ #define DCACHE_MANAGED_DENTRY \
// /*AFLA*/ 	(DCACHE_MOUNTED|DCACHE_NEED_AUTOMOUNT|DCACHE_MANAGE_TRANSIT)
// /*AFLA*/ 
// /*AFLA*/ #define DCACHE_LRU_LIST			0x00080000
// /*AFLA*/ 
// /*AFLA*/ #define DCACHE_ENTRY_TYPE		0x00700000
// /*AFLA*/ #define DCACHE_MISS_TYPE		0x00000000 /* Negative dentry (maybe fallthru to nowhere) */
// /*AFLA*/ #define DCACHE_WHITEOUT_TYPE		0x00100000 /* Whiteout dentry (stop pathwalk) */
// /*AFLA*/ #define DCACHE_DIRECTORY_TYPE		0x00200000 /* Normal directory */
// /*AFLA*/ #define DCACHE_AUTODIR_TYPE		0x00300000 /* Lookupless directory (presumed automount) */
// /*AFLA*/ #define DCACHE_REGULAR_TYPE		0x00400000 /* Regular file type (or fallthru to such) */
// /*AFLA*/ #define DCACHE_SPECIAL_TYPE		0x00500000 /* Other file type (or fallthru to such) */
// /*AFLA*/ #define DCACHE_SYMLINK_TYPE		0x00600000 /* Symlink (or fallthru to such) */
// /*AFLA*/ 
// /*AFLA*/ #define DCACHE_MAY_FREE			0x00800000
// /*AFLA*/ #define DCACHE_FALLTHRU			0x01000000 /* Fall through to lower layer */
// /*AFLA*/ #define DCACHE_ENCRYPTED_WITH_KEY	0x02000000 /* dir is encrypted with a valid key */
// /*AFLA*/ #define DCACHE_OP_REAL			0x04000000
// /*AFLA*/ 
// /*AFLA*/ #define DCACHE_PAR_LOOKUP		0x10000000 /* being looked up (with parent locked shared) */
// /*AFLA*/ #define DCACHE_DENTRY_CURSOR		0x20000000
// /*AFLA*/ 
// /*AFLA*/ extern seqlock_t rename_lock;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These are the low-level FS interfaces to the dcache..
// /*AFLA*/  */
// /*AFLA*/ extern void d_instantiate(struct dentry *, struct inode *);
// /*AFLA*/ extern struct dentry * d_instantiate_unique(struct dentry *, struct inode *);
// /*AFLA*/ extern int d_instantiate_no_diralias(struct dentry *, struct inode *);
// /*AFLA*/ extern void __d_drop(struct dentry *dentry);
// /*AFLA*/ extern void d_drop(struct dentry *dentry);
// /*AFLA*/ extern void d_delete(struct dentry *);
// /*AFLA*/ extern void d_set_d_op(struct dentry *dentry, const struct dentry_operations *op);
// /*AFLA*/ 
// /*AFLA*/ /* allocate/de-allocate */
// /*AFLA*/ extern struct dentry * d_alloc(struct dentry *, const struct qstr *);
// /*AFLA*/ extern struct dentry * d_alloc_pseudo(struct super_block *, const struct qstr *);
// /*AFLA*/ extern struct dentry * d_alloc_parallel(struct dentry *, const struct qstr *,
// /*AFLA*/ 					wait_queue_head_t *);
// /*AFLA*/ extern struct dentry * d_splice_alias(struct inode *, struct dentry *);
// /*AFLA*/ extern struct dentry * d_add_ci(struct dentry *, struct inode *, struct qstr *);
// /*AFLA*/ extern struct dentry * d_exact_alias(struct dentry *, struct inode *);
// /*AFLA*/ extern struct dentry *d_find_any_alias(struct inode *inode);
// /*AFLA*/ extern struct dentry * d_obtain_alias(struct inode *);
// /*AFLA*/ extern struct dentry * d_obtain_root(struct inode *);
// /*AFLA*/ extern void shrink_dcache_sb(struct super_block *);
// /*AFLA*/ extern void shrink_dcache_parent(struct dentry *);
// /*AFLA*/ extern void shrink_dcache_for_umount(struct super_block *);
// /*AFLA*/ extern void d_invalidate(struct dentry *);
// /*AFLA*/ 
// /*AFLA*/ /* only used at mount-time */
// /*AFLA*/ extern struct dentry * d_make_root(struct inode *);
// /*AFLA*/ 
// /*AFLA*/ /* <clickety>-<click> the ramfs-type tree */
// /*AFLA*/ extern void d_genocide(struct dentry *);
// /*AFLA*/ 
// /*AFLA*/ extern void d_tmpfile(struct dentry *, struct inode *);
// /*AFLA*/ 
// /*AFLA*/ extern struct dentry *d_find_alias(struct inode *);
// /*AFLA*/ extern void d_prune_aliases(struct inode *);
// /*AFLA*/ 
// /*AFLA*/ /* test whether we have any submounts in a subdir tree */
// /*AFLA*/ extern int have_submounts(struct dentry *);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This adds the entry to the hash queues.
// /*AFLA*/  */
// /*AFLA*/ extern void d_rehash(struct dentry *);
// /*AFLA*/  
// /*AFLA*/ extern void d_add(struct dentry *, struct inode *);
// /*AFLA*/ 
// /*AFLA*/ extern void dentry_update_name_case(struct dentry *, const struct qstr *);
// /*AFLA*/ 
// /*AFLA*/ /* used for rename() and baskets */
// /*AFLA*/ extern void d_move(struct dentry *, struct dentry *);
// /*AFLA*/ extern void d_exchange(struct dentry *, struct dentry *);
// /*AFLA*/ extern struct dentry *d_ancestor(struct dentry *, struct dentry *);
// /*AFLA*/ 
// /*AFLA*/ /* appendix may either be NULL or be used for transname suffixes */
// /*AFLA*/ extern struct dentry *d_lookup(const struct dentry *, const struct qstr *);
// /*AFLA*/ extern struct dentry *d_hash_and_lookup(struct dentry *, struct qstr *);
// /*AFLA*/ extern struct dentry *__d_lookup(const struct dentry *, const struct qstr *);
// /*AFLA*/ extern struct dentry *__d_lookup_rcu(const struct dentry *parent,
// /*AFLA*/ 				const struct qstr *name, unsigned *seq);
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned d_count(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return dentry->d_lockref.count;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * helper function for dentry_operations.d_dname() members
// /*AFLA*/  */
// /*AFLA*/ extern __printf(4, 5)
// /*AFLA*/ char *dynamic_dname(struct dentry *, char *, int, const char *, ...);
// /*AFLA*/ extern char *simple_dname(struct dentry *, char *, int);
// /*AFLA*/ 
// /*AFLA*/ extern char *__d_path(const struct path *, const struct path *, char *, int);
// /*AFLA*/ extern char *d_absolute_path(const struct path *, char *, int);
// /*AFLA*/ extern char *d_path(const struct path *, char *, int);
// /*AFLA*/ extern char *dentry_path_raw(struct dentry *, char *, int);
// /*AFLA*/ extern char *dentry_path(struct dentry *, char *, int);
// /*AFLA*/ 
// /*AFLA*/ /* Allocation counts.. */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  *	dget, dget_dlock -	get a reference to a dentry
// /*AFLA*/  *	@dentry: dentry to get a reference to
// /*AFLA*/  *
// /*AFLA*/  *	Given a dentry or %NULL pointer increment the reference count
// /*AFLA*/  *	if appropriate and return the dentry. A dentry will not be 
// /*AFLA*/  *	destroyed when it has references.
// /*AFLA*/  */
// /*AFLA*/ static inline struct dentry *dget_dlock(struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	if (dentry)
// /*AFLA*/ 		dentry->d_lockref.count++;
// /*AFLA*/ 	return dentry;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct dentry *dget(struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	if (dentry)
// /*AFLA*/ 		lockref_get(&dentry->d_lockref);
// /*AFLA*/ 	return dentry;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct dentry *dget_parent(struct dentry *dentry);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  *	d_unhashed -	is dentry hashed
// /*AFLA*/  *	@dentry: entry to check
// /*AFLA*/  *
// /*AFLA*/  *	Returns true if the dentry passed is not currently hashed.
// /*AFLA*/  */
// /*AFLA*/  
// /*AFLA*/ static inline int d_unhashed(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return hlist_bl_unhashed(&dentry->d_hash);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int d_unlinked(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return d_unhashed(dentry) && !IS_ROOT(dentry);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int cant_mount(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return (dentry->d_flags & DCACHE_CANT_MOUNT);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void dont_mount(struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	spin_lock(&dentry->d_lock);
// /*AFLA*/ 	dentry->d_flags |= DCACHE_CANT_MOUNT;
// /*AFLA*/ 	spin_unlock(&dentry->d_lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void __d_lookup_done(struct dentry *);
// /*AFLA*/ 
// /*AFLA*/ static inline int d_in_lookup(struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return dentry->d_flags & DCACHE_PAR_LOOKUP;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void d_lookup_done(struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(d_in_lookup(dentry))) {
// /*AFLA*/ 		spin_lock(&dentry->d_lock);
// /*AFLA*/ 		__d_lookup_done(dentry);
// /*AFLA*/ 		spin_unlock(&dentry->d_lock);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void dput(struct dentry *);
// /*AFLA*/ 
// /*AFLA*/ static inline bool d_managed(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return dentry->d_flags & DCACHE_MANAGED_DENTRY;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool d_mountpoint(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return dentry->d_flags & DCACHE_MOUNTED;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Directory cache entry type accessor functions.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned __d_entry_type(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return dentry->d_flags & DCACHE_ENTRY_TYPE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool d_is_miss(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return __d_entry_type(dentry) == DCACHE_MISS_TYPE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool d_is_whiteout(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return __d_entry_type(dentry) == DCACHE_WHITEOUT_TYPE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool d_can_lookup(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return __d_entry_type(dentry) == DCACHE_DIRECTORY_TYPE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool d_is_autodir(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return __d_entry_type(dentry) == DCACHE_AUTODIR_TYPE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool d_is_dir(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return d_can_lookup(dentry) || d_is_autodir(dentry);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool d_is_symlink(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return __d_entry_type(dentry) == DCACHE_SYMLINK_TYPE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool d_is_reg(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return __d_entry_type(dentry) == DCACHE_REGULAR_TYPE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool d_is_special(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return __d_entry_type(dentry) == DCACHE_SPECIAL_TYPE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool d_is_file(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return d_is_reg(dentry) || d_is_special(dentry);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool d_is_negative(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	// TODO: check d_is_whiteout(dentry) also.
// /*AFLA*/ 	return d_is_miss(dentry);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool d_is_positive(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return !d_is_negative(dentry);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * d_really_is_negative - Determine if a dentry is really negative (ignoring fallthroughs)
// /*AFLA*/  * @dentry: The dentry in question
// /*AFLA*/  *
// /*AFLA*/  * Returns true if the dentry represents either an absent name or a name that
// /*AFLA*/  * doesn't map to an inode (ie. ->d_inode is NULL).  The dentry could represent
// /*AFLA*/  * a true miss, a whiteout that isn't represented by a 0,0 chardev or a
// /*AFLA*/  * fallthrough marker in an opaque directory.
// /*AFLA*/  *
// /*AFLA*/  * Note!  (1) This should be used *only* by a filesystem to examine its own
// /*AFLA*/  * dentries.  It should not be used to look at some other filesystem's
// /*AFLA*/  * dentries.  (2) It should also be used in combination with d_inode() to get
// /*AFLA*/  * the inode.  (3) The dentry may have something attached to ->d_lower and the
// /*AFLA*/  * type field of the flags may be set to something other than miss or whiteout.
// /*AFLA*/  */
// /*AFLA*/ static inline bool d_really_is_negative(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return dentry->d_inode == NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * d_really_is_positive - Determine if a dentry is really positive (ignoring fallthroughs)
// /*AFLA*/  * @dentry: The dentry in question
// /*AFLA*/  *
// /*AFLA*/  * Returns true if the dentry represents a name that maps to an inode
// /*AFLA*/  * (ie. ->d_inode is not NULL).  The dentry might still represent a whiteout if
// /*AFLA*/  * that is represented on medium as a 0,0 chardev.
// /*AFLA*/  *
// /*AFLA*/  * Note!  (1) This should be used *only* by a filesystem to examine its own
// /*AFLA*/  * dentries.  It should not be used to look at some other filesystem's
// /*AFLA*/  * dentries.  (2) It should also be used in combination with d_inode() to get
// /*AFLA*/  * the inode.
// /*AFLA*/  */
// /*AFLA*/ static inline bool d_really_is_positive(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return dentry->d_inode != NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int simple_positive(struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return d_really_is_positive(dentry) && !d_unhashed(dentry);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void d_set_fallthru(struct dentry *dentry);
// /*AFLA*/ 
// /*AFLA*/ static inline bool d_is_fallthru(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return dentry->d_flags & DCACHE_FALLTHRU;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ extern int sysctl_vfs_cache_pressure;
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long vfs_pressure_ratio(unsigned long val)
// /*AFLA*/ {
// /*AFLA*/ 	return mult_frac(val, sysctl_vfs_cache_pressure, 100);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * d_inode - Get the actual inode of this dentry
// /*AFLA*/  * @dentry: The dentry to query
// /*AFLA*/  *
// /*AFLA*/  * This is the helper normal filesystems should use to get at their own inodes
// /*AFLA*/  * in their own dentries and ignore the layering superimposed upon them.
// /*AFLA*/  */
// /*AFLA*/ static inline struct inode *d_inode(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return dentry->d_inode;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * d_inode_rcu - Get the actual inode of this dentry with ACCESS_ONCE()
// /*AFLA*/  * @dentry: The dentry to query
// /*AFLA*/  *
// /*AFLA*/  * This is the helper normal filesystems should use to get at their own inodes
// /*AFLA*/  * in their own dentries and ignore the layering superimposed upon them.
// /*AFLA*/  */
// /*AFLA*/ static inline struct inode *d_inode_rcu(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return ACCESS_ONCE(dentry->d_inode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * d_backing_inode - Get upper or lower inode we should be using
// /*AFLA*/  * @upper: The upper layer
// /*AFLA*/  *
// /*AFLA*/  * This is the helper that should be used to get at the inode that will be used
// /*AFLA*/  * if this dentry were to be opened as a file.  The inode may be on the upper
// /*AFLA*/  * dentry or it may be on a lower dentry pinned by the upper.
// /*AFLA*/  *
// /*AFLA*/  * Normal filesystems should not use this to access their own inodes.
// /*AFLA*/  */
// /*AFLA*/ static inline struct inode *d_backing_inode(const struct dentry *upper)
// /*AFLA*/ {
// /*AFLA*/ 	struct inode *inode = upper->d_inode;
// /*AFLA*/ 
// /*AFLA*/ 	return inode;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * d_backing_dentry - Get upper or lower dentry we should be using
// /*AFLA*/  * @upper: The upper layer
// /*AFLA*/  *
// /*AFLA*/  * This is the helper that should be used to get the dentry of the inode that
// /*AFLA*/  * will be used if this dentry were opened as a file.  It may be the upper
// /*AFLA*/  * dentry or it may be a lower dentry pinned by the upper.
// /*AFLA*/  *
// /*AFLA*/  * Normal filesystems should not use this to access their own dentries.
// /*AFLA*/  */
// /*AFLA*/ static inline struct dentry *d_backing_dentry(struct dentry *upper)
// /*AFLA*/ {
// /*AFLA*/ 	return upper;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * d_real - Return the real dentry
// /*AFLA*/  * @dentry: the dentry to query
// /*AFLA*/  * @inode: inode to select the dentry from multiple layers (can be NULL)
// /*AFLA*/  * @flags: open flags to control copy-up behavior
// /*AFLA*/  *
// /*AFLA*/  * If dentry is on an union/overlay, then return the underlying, real dentry.
// /*AFLA*/  * Otherwise return the dentry itself.
// /*AFLA*/  *
// /*AFLA*/  * See also: Documentation/filesystems/vfs.txt
// /*AFLA*/  */
// /*AFLA*/ static inline struct dentry *d_real(struct dentry *dentry,
// /*AFLA*/ 				    const struct inode *inode,
// /*AFLA*/ 				    unsigned int flags)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(dentry->d_flags & DCACHE_OP_REAL))
// /*AFLA*/ 		return dentry->d_op->d_real(dentry, inode, flags);
// /*AFLA*/ 	else
// /*AFLA*/ 		return dentry;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * d_real_inode - Return the real inode
// /*AFLA*/  * @dentry: The dentry to query
// /*AFLA*/  *
// /*AFLA*/  * If dentry is on an union/overlay, then return the underlying, real inode.
// /*AFLA*/  * Otherwise return d_inode().
// /*AFLA*/  */
// /*AFLA*/ static inline struct inode *d_real_inode(const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	/* This usage of d_real() results in const dentry */
// /*AFLA*/ 	return d_backing_inode(d_real((struct dentry *) dentry, NULL, 0));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif	/* __LINUX_DCACHE_H */
