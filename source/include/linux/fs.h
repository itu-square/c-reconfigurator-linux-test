// /*AFLA*/ #ifndef _LINUX_FS_H
// /*AFLA*/ #define _LINUX_FS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ #include <linux/wait.h>
// /*AFLA*/ #include <linux/kdev_t.h>
// /*AFLA*/ #include <linux/dcache.h>
// /*AFLA*/ #include <linux/path.h>
// /*AFLA*/ #include <linux/stat.h>
// /*AFLA*/ #include <linux/cache.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/list_lru.h>
// /*AFLA*/ #include <linux/llist.h>
// /*AFLA*/ #include <linux/radix-tree.h>
// /*AFLA*/ #include <linux/rbtree.h>
// /*AFLA*/ #include <linux/init.h>
// /*AFLA*/ #include <linux/pid.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ #include <linux/mutex.h>
// /*AFLA*/ #include <linux/rwsem.h>
// /*AFLA*/ #include <linux/capability.h>
// /*AFLA*/ #include <linux/semaphore.h>
// /*AFLA*/ #include <linux/fiemap.h>
// /*AFLA*/ #include <linux/rculist_bl.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/shrinker.h>
// /*AFLA*/ #include <linux/migrate_mode.h>
// /*AFLA*/ #include <linux/uidgid.h>
// /*AFLA*/ #include <linux/lockdep.h>
// /*AFLA*/ #include <linux/percpu-rwsem.h>
// /*AFLA*/ #include <linux/blk_types.h>
// /*AFLA*/ #include <linux/workqueue.h>
// /*AFLA*/ #include <linux/percpu-rwsem.h>
// /*AFLA*/ #include <linux/delayed_call.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/byteorder.h>
// /*AFLA*/ #include <uapi/linux/fs.h>
// /*AFLA*/ 
// /*AFLA*/ struct backing_dev_info;
// /*AFLA*/ struct bdi_writeback;
// /*AFLA*/ struct export_operations;
// /*AFLA*/ struct hd_geometry;
// /*AFLA*/ struct iovec;
// /*AFLA*/ struct kiocb;
// /*AFLA*/ struct kobject;
// /*AFLA*/ struct pipe_inode_info;
// /*AFLA*/ struct poll_table_struct;
// /*AFLA*/ struct kstatfs;
// /*AFLA*/ struct vm_area_struct;
// /*AFLA*/ struct vfsmount;
// /*AFLA*/ struct cred;
// /*AFLA*/ struct swap_info_struct;
// /*AFLA*/ struct seq_file;
// /*AFLA*/ struct workqueue_struct;
// /*AFLA*/ struct iov_iter;
// /*AFLA*/ struct fscrypt_info;
// /*AFLA*/ struct fscrypt_operations;
// /*AFLA*/ 
// /*AFLA*/ extern void __init inode_init(void);
// /*AFLA*/ extern void __init inode_init_early(void);
// /*AFLA*/ extern void __init files_init(void);
// /*AFLA*/ extern void __init files_maxfiles_init(void);
// /*AFLA*/ 
// /*AFLA*/ extern struct files_stat_struct files_stat;
// /*AFLA*/ extern unsigned long get_max_files(void);
// /*AFLA*/ extern unsigned int sysctl_nr_open;
// /*AFLA*/ extern struct inodes_stat_t inodes_stat;
// /*AFLA*/ extern int leases_enable, lease_break_time;
// /*AFLA*/ extern int sysctl_protected_symlinks;
// /*AFLA*/ extern int sysctl_protected_hardlinks;
// /*AFLA*/ 
// /*AFLA*/ struct buffer_head;
// /*AFLA*/ typedef int (get_block_t)(struct inode *inode, sector_t iblock,
// /*AFLA*/ 			struct buffer_head *bh_result, int create);
// /*AFLA*/ typedef int (dio_iodone_t)(struct kiocb *iocb, loff_t offset,
// /*AFLA*/ 			ssize_t bytes, void *private);
// /*AFLA*/ 
// /*AFLA*/ #define MAY_EXEC		0x00000001
// /*AFLA*/ #define MAY_WRITE		0x00000002
// /*AFLA*/ #define MAY_READ		0x00000004
// /*AFLA*/ #define MAY_APPEND		0x00000008
// /*AFLA*/ #define MAY_ACCESS		0x00000010
// /*AFLA*/ #define MAY_OPEN		0x00000020
// /*AFLA*/ #define MAY_CHDIR		0x00000040
// /*AFLA*/ /* called from RCU mode, don't block */
// /*AFLA*/ #define MAY_NOT_BLOCK		0x00000080
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * flags in file.f_mode.  Note that FMODE_READ and FMODE_WRITE must correspond
// /*AFLA*/  * to O_WRONLY and O_RDWR via the strange trick in __dentry_open()
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* file is open for reading */
// /*AFLA*/ #define FMODE_READ		((__force fmode_t)0x1)
// /*AFLA*/ /* file is open for writing */
// /*AFLA*/ #define FMODE_WRITE		((__force fmode_t)0x2)
// /*AFLA*/ /* file is seekable */
// /*AFLA*/ #define FMODE_LSEEK		((__force fmode_t)0x4)
// /*AFLA*/ /* file can be accessed using pread */
// /*AFLA*/ #define FMODE_PREAD		((__force fmode_t)0x8)
// /*AFLA*/ /* file can be accessed using pwrite */
// /*AFLA*/ #define FMODE_PWRITE		((__force fmode_t)0x10)
// /*AFLA*/ /* File is opened for execution with sys_execve / sys_uselib */
// /*AFLA*/ #define FMODE_EXEC		((__force fmode_t)0x20)
// /*AFLA*/ /* File is opened with O_NDELAY (only set for block devices) */
// /*AFLA*/ #define FMODE_NDELAY		((__force fmode_t)0x40)
// /*AFLA*/ /* File is opened with O_EXCL (only set for block devices) */
// /*AFLA*/ #define FMODE_EXCL		((__force fmode_t)0x80)
// /*AFLA*/ /* File is opened using open(.., 3, ..) and is writeable only for ioctls
// /*AFLA*/    (specialy hack for floppy.c) */
// /*AFLA*/ #define FMODE_WRITE_IOCTL	((__force fmode_t)0x100)
// /*AFLA*/ /* 32bit hashes as llseek() offset (for directories) */
// /*AFLA*/ #define FMODE_32BITHASH         ((__force fmode_t)0x200)
// /*AFLA*/ /* 64bit hashes as llseek() offset (for directories) */
// /*AFLA*/ #define FMODE_64BITHASH         ((__force fmode_t)0x400)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Don't update ctime and mtime.
// /*AFLA*/  *
// /*AFLA*/  * Currently a special hack for the XFS open_by_handle ioctl, but we'll
// /*AFLA*/  * hopefully graduate it to a proper O_CMTIME flag supported by open(2) soon.
// /*AFLA*/  */
// /*AFLA*/ #define FMODE_NOCMTIME		((__force fmode_t)0x800)
// /*AFLA*/ 
// /*AFLA*/ /* Expect random access pattern */
// /*AFLA*/ #define FMODE_RANDOM		((__force fmode_t)0x1000)
// /*AFLA*/ 
// /*AFLA*/ /* File is huge (eg. /dev/kmem): treat loff_t as unsigned */
// /*AFLA*/ #define FMODE_UNSIGNED_OFFSET	((__force fmode_t)0x2000)
// /*AFLA*/ 
// /*AFLA*/ /* File is opened with O_PATH; almost nothing can be done with it */
// /*AFLA*/ #define FMODE_PATH		((__force fmode_t)0x4000)
// /*AFLA*/ 
// /*AFLA*/ /* File needs atomic accesses to f_pos */
// /*AFLA*/ #define FMODE_ATOMIC_POS	((__force fmode_t)0x8000)
// /*AFLA*/ /* Write access to underlying fs */
// /*AFLA*/ #define FMODE_WRITER		((__force fmode_t)0x10000)
// /*AFLA*/ /* Has read method(s) */
// /*AFLA*/ #define FMODE_CAN_READ          ((__force fmode_t)0x20000)
// /*AFLA*/ /* Has write method(s) */
// /*AFLA*/ #define FMODE_CAN_WRITE         ((__force fmode_t)0x40000)
// /*AFLA*/ 
// /*AFLA*/ /* File was opened by fanotify and shouldn't generate fanotify events */
// /*AFLA*/ #define FMODE_NONOTIFY		((__force fmode_t)0x4000000)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Flag for rw_copy_check_uvector and compat_rw_copy_check_uvector
// /*AFLA*/  * that indicates that they should check the contents of the iovec are
// /*AFLA*/  * valid, but not check the memory that the iovec elements
// /*AFLA*/  * points too.
// /*AFLA*/  */
// /*AFLA*/ #define CHECK_IOVEC_ONLY -1
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The below are the various read and write flags that we support. Some of
// /*AFLA*/  * them include behavioral modifiers that send information down to the
// /*AFLA*/  * block layer and IO scheduler. They should be used along with a req_op.
// /*AFLA*/  * Terminology:
// /*AFLA*/  *
// /*AFLA*/  *	The block layer uses device plugging to defer IO a little bit, in
// /*AFLA*/  *	the hope that we will see more IO very shortly. This increases
// /*AFLA*/  *	coalescing of adjacent IO and thus reduces the number of IOs we
// /*AFLA*/  *	have to send to the device. It also allows for better queuing,
// /*AFLA*/  *	if the IO isn't mergeable. If the caller is going to be waiting
// /*AFLA*/  *	for the IO, then he must ensure that the device is unplugged so
// /*AFLA*/  *	that the IO is dispatched to the driver.
// /*AFLA*/  *
// /*AFLA*/  *	All IO is handled async in Linux. This is fine for background
// /*AFLA*/  *	writes, but for reads or writes that someone waits for completion
// /*AFLA*/  *	on, we want to notify the block layer and IO scheduler so that they
// /*AFLA*/  *	know about it. That allows them to make better scheduling
// /*AFLA*/  *	decisions. So when the below references 'sync' and 'async', it
// /*AFLA*/  *	is referencing this priority hint.
// /*AFLA*/  *
// /*AFLA*/  * With that in mind, the available types are:
// /*AFLA*/  *
// /*AFLA*/  * READ			A normal read operation. Device will be plugged.
// /*AFLA*/  * READ_SYNC		A synchronous read. Device is not plugged, caller can
// /*AFLA*/  *			immediately wait on this read without caring about
// /*AFLA*/  *			unplugging.
// /*AFLA*/  * WRITE		A normal async write. Device will be plugged.
// /*AFLA*/  * WRITE_SYNC		Synchronous write. Identical to WRITE, but passes down
// /*AFLA*/  *			the hint that someone will be waiting on this IO
// /*AFLA*/  *			shortly. The write equivalent of READ_SYNC.
// /*AFLA*/  * WRITE_ODIRECT	Special case write for O_DIRECT only.
// /*AFLA*/  * WRITE_FLUSH		Like WRITE_SYNC but with preceding cache flush.
// /*AFLA*/  * WRITE_FUA		Like WRITE_SYNC but data is guaranteed to be on
// /*AFLA*/  *			non-volatile media on completion.
// /*AFLA*/  * WRITE_FLUSH_FUA	Combination of WRITE_FLUSH and FUA. The IO is preceded
// /*AFLA*/  *			by a cache flush and data is guaranteed to be on
// /*AFLA*/  *			non-volatile media on completion.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ #define RW_MASK			REQ_OP_WRITE
// /*AFLA*/ 
// /*AFLA*/ #define READ			REQ_OP_READ
// /*AFLA*/ #define WRITE			REQ_OP_WRITE
// /*AFLA*/ 
// /*AFLA*/ #define READ_SYNC		REQ_SYNC
// /*AFLA*/ #define WRITE_SYNC		(REQ_SYNC | REQ_NOIDLE)
// /*AFLA*/ #define WRITE_ODIRECT		REQ_SYNC
// /*AFLA*/ #define WRITE_FLUSH		(REQ_SYNC | REQ_NOIDLE | REQ_PREFLUSH)
// /*AFLA*/ #define WRITE_FUA		(REQ_SYNC | REQ_NOIDLE | REQ_FUA)
// /*AFLA*/ #define WRITE_FLUSH_FUA		(REQ_SYNC | REQ_NOIDLE | REQ_PREFLUSH | REQ_FUA)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Attribute flags.  These should be or-ed together to figure out what
// /*AFLA*/  * has been changed!
// /*AFLA*/  */
// /*AFLA*/ #define ATTR_MODE	(1 << 0)
// /*AFLA*/ #define ATTR_UID	(1 << 1)
// /*AFLA*/ #define ATTR_GID	(1 << 2)
// /*AFLA*/ #define ATTR_SIZE	(1 << 3)
// /*AFLA*/ #define ATTR_ATIME	(1 << 4)
// /*AFLA*/ #define ATTR_MTIME	(1 << 5)
// /*AFLA*/ #define ATTR_CTIME	(1 << 6)
// /*AFLA*/ #define ATTR_ATIME_SET	(1 << 7)
// /*AFLA*/ #define ATTR_MTIME_SET	(1 << 8)
// /*AFLA*/ #define ATTR_FORCE	(1 << 9) /* Not a change, but a change it */
// /*AFLA*/ #define ATTR_ATTR_FLAG	(1 << 10)
// /*AFLA*/ #define ATTR_KILL_SUID	(1 << 11)
// /*AFLA*/ #define ATTR_KILL_SGID	(1 << 12)
// /*AFLA*/ #define ATTR_FILE	(1 << 13)
// /*AFLA*/ #define ATTR_KILL_PRIV	(1 << 14)
// /*AFLA*/ #define ATTR_OPEN	(1 << 15) /* Truncating from open(O_TRUNC) */
// /*AFLA*/ #define ATTR_TIMES_SET	(1 << 16)
// /*AFLA*/ #define ATTR_TOUCH	(1 << 17)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Whiteout is represented by a char device.  The following constants define the
// /*AFLA*/  * mode and device number to use.
// /*AFLA*/  */
// /*AFLA*/ #define WHITEOUT_MODE 0
// /*AFLA*/ #define WHITEOUT_DEV 0
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is the Inode Attributes structure, used for notify_change().  It
// /*AFLA*/  * uses the above definitions as flags, to know which values have changed.
// /*AFLA*/  * Also, in this manner, a Filesystem can look at only the values it cares
// /*AFLA*/  * about.  Basically, these are the attributes that the VFS layer can
// /*AFLA*/  * request to change from the FS layer.
// /*AFLA*/  *
// /*AFLA*/  * Derek Atkins <warlord@MIT.EDU> 94-10-20
// /*AFLA*/  */
// /*AFLA*/ struct iattr {
// /*AFLA*/ 	unsigned int	ia_valid;
// /*AFLA*/ 	umode_t		ia_mode;
// /*AFLA*/ 	kuid_t		ia_uid;
// /*AFLA*/ 	kgid_t		ia_gid;
// /*AFLA*/ 	loff_t		ia_size;
// /*AFLA*/ 	struct timespec	ia_atime;
// /*AFLA*/ 	struct timespec	ia_mtime;
// /*AFLA*/ 	struct timespec	ia_ctime;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Not an attribute, but an auxiliary info for filesystems wanting to
// /*AFLA*/ 	 * implement an ftruncate() like method.  NOTE: filesystem should
// /*AFLA*/ 	 * check for (ia_valid & ATTR_FILE), and not for (ia_file != NULL).
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct file	*ia_file;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Includes for diskquotas.
// /*AFLA*/  */
// /*AFLA*/ #include <linux/quota.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Maximum number of layers of fs stack.  Needs to be limited to
// /*AFLA*/  * prevent kernel stack overflow
// /*AFLA*/  */
// /*AFLA*/ #define FILESYSTEM_MAX_STACK_DEPTH 2
// /*AFLA*/ 
// /*AFLA*/ /** 
// /*AFLA*/  * enum positive_aop_returns - aop return codes with specific semantics
// /*AFLA*/  *
// /*AFLA*/  * @AOP_WRITEPAGE_ACTIVATE: Informs the caller that page writeback has
// /*AFLA*/  * 			    completed, that the page is still locked, and
// /*AFLA*/  * 			    should be considered active.  The VM uses this hint
// /*AFLA*/  * 			    to return the page to the active list -- it won't
// /*AFLA*/  * 			    be a candidate for writeback again in the near
// /*AFLA*/  * 			    future.  Other callers must be careful to unlock
// /*AFLA*/  * 			    the page if they get this return.  Returned by
// /*AFLA*/  * 			    writepage(); 
// /*AFLA*/  *
// /*AFLA*/  * @AOP_TRUNCATED_PAGE: The AOP method that was handed a locked page has
// /*AFLA*/  *  			unlocked it and the page might have been truncated.
// /*AFLA*/  *  			The caller should back up to acquiring a new page and
// /*AFLA*/  *  			trying again.  The aop will be taking reasonable
// /*AFLA*/  *  			precautions not to livelock.  If the caller held a page
// /*AFLA*/  *  			reference, it should drop it before retrying.  Returned
// /*AFLA*/  *  			by readpage().
// /*AFLA*/  *
// /*AFLA*/  * address_space_operation functions return these large constants to indicate
// /*AFLA*/  * special semantics to the caller.  These are much larger than the bytes in a
// /*AFLA*/  * page to allow for functions that return the number of bytes operated on in a
// /*AFLA*/  * given page.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ enum positive_aop_returns {
// /*AFLA*/ 	AOP_WRITEPAGE_ACTIVATE	= 0x80000,
// /*AFLA*/ 	AOP_TRUNCATED_PAGE	= 0x80001,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define AOP_FLAG_UNINTERRUPTIBLE	0x0001 /* will not do a short write */
// /*AFLA*/ #define AOP_FLAG_CONT_EXPAND		0x0002 /* called from cont_expand */
// /*AFLA*/ #define AOP_FLAG_NOFS			0x0004 /* used by filesystem to direct
// /*AFLA*/ 						* helper code (eg buffer layer)
// /*AFLA*/ 						* to clear GFP_FS from alloc */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * oh the beauties of C type declarations.
// /*AFLA*/  */
// /*AFLA*/ struct page;
// /*AFLA*/ struct address_space;
// /*AFLA*/ struct writeback_control;
// /*AFLA*/ 
// /*AFLA*/ #define IOCB_EVENTFD		(1 << 0)
// /*AFLA*/ #define IOCB_APPEND		(1 << 1)
// /*AFLA*/ #define IOCB_DIRECT		(1 << 2)
// /*AFLA*/ #define IOCB_HIPRI		(1 << 3)
// /*AFLA*/ #define IOCB_DSYNC		(1 << 4)
// /*AFLA*/ #define IOCB_SYNC		(1 << 5)
// /*AFLA*/ #define IOCB_WRITE		(1 << 6)
// /*AFLA*/ 
// /*AFLA*/ struct kiocb {
// /*AFLA*/ 	struct file		*ki_filp;
// /*AFLA*/ 	loff_t			ki_pos;
// /*AFLA*/ 	void (*ki_complete)(struct kiocb *iocb, long ret, long ret2);
// /*AFLA*/ 	void			*private;
// /*AFLA*/ 	int			ki_flags;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline bool is_sync_kiocb(struct kiocb *kiocb)
// /*AFLA*/ {
// /*AFLA*/ 	return kiocb->ki_complete == NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int iocb_flags(struct file *file);
// /*AFLA*/ 
// /*AFLA*/ static inline void init_sync_kiocb(struct kiocb *kiocb, struct file *filp)
// /*AFLA*/ {
// /*AFLA*/ 	*kiocb = (struct kiocb) {
// /*AFLA*/ 		.ki_filp = filp,
// /*AFLA*/ 		.ki_flags = iocb_flags(filp),
// /*AFLA*/ 	};
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * "descriptor" for what we're up to with a read.
// /*AFLA*/  * This allows us to use the same read code yet
// /*AFLA*/  * have multiple different users of the data that
// /*AFLA*/  * we read from a file.
// /*AFLA*/  *
// /*AFLA*/  * The simplest case just copies the data to user
// /*AFLA*/  * mode.
// /*AFLA*/  */
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	size_t written;
// /*AFLA*/ 	size_t count;
// /*AFLA*/ 	union {
// /*AFLA*/ 		char __user *buf;
// /*AFLA*/ 		void *data;
// /*AFLA*/ 	} arg;
// /*AFLA*/ 	int error;
// /*AFLA*/ } read_descriptor_t;
// /*AFLA*/ 
// /*AFLA*/ typedef int (*read_actor_t)(read_descriptor_t *, struct page *,
// /*AFLA*/ 		unsigned long, unsigned long);
// /*AFLA*/ 
// /*AFLA*/ struct address_space_operations {
// /*AFLA*/ 	int (*writepage)(struct page *page, struct writeback_control *wbc);
// /*AFLA*/ 	int (*readpage)(struct file *, struct page *);
// /*AFLA*/ 
// /*AFLA*/ 	/* Write back some dirty pages from this mapping. */
// /*AFLA*/ 	int (*writepages)(struct address_space *, struct writeback_control *);
// /*AFLA*/ 
// /*AFLA*/ 	/* Set a page dirty.  Return true if this dirtied it */
// /*AFLA*/ 	int (*set_page_dirty)(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ 	int (*readpages)(struct file *filp, struct address_space *mapping,
// /*AFLA*/ 			struct list_head *pages, unsigned nr_pages);
// /*AFLA*/ 
// /*AFLA*/ 	int (*write_begin)(struct file *, struct address_space *mapping,
// /*AFLA*/ 				loff_t pos, unsigned len, unsigned flags,
// /*AFLA*/ 				struct page **pagep, void **fsdata);
// /*AFLA*/ 	int (*write_end)(struct file *, struct address_space *mapping,
// /*AFLA*/ 				loff_t pos, unsigned len, unsigned copied,
// /*AFLA*/ 				struct page *page, void *fsdata);
// /*AFLA*/ 
// /*AFLA*/ 	/* Unfortunately this kludge is needed for FIBMAP. Don't use it */
// /*AFLA*/ 	sector_t (*bmap)(struct address_space *, sector_t);
// /*AFLA*/ 	void (*invalidatepage) (struct page *, unsigned int, unsigned int);
// /*AFLA*/ 	int (*releasepage) (struct page *, gfp_t);
// /*AFLA*/ 	void (*freepage)(struct page *);
// /*AFLA*/ 	ssize_t (*direct_IO)(struct kiocb *, struct iov_iter *iter);
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * migrate the contents of a page to the specified target. If
// /*AFLA*/ 	 * migrate_mode is MIGRATE_ASYNC, it must not block.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int (*migratepage) (struct address_space *,
// /*AFLA*/ 			struct page *, struct page *, enum migrate_mode);
// /*AFLA*/ 	bool (*isolate_page)(struct page *, isolate_mode_t);
// /*AFLA*/ 	void (*putback_page)(struct page *);
// /*AFLA*/ 	int (*launder_page) (struct page *);
// /*AFLA*/ 	int (*is_partially_uptodate) (struct page *, unsigned long,
// /*AFLA*/ 					unsigned long);
// /*AFLA*/ 	void (*is_dirty_writeback) (struct page *, bool *, bool *);
// /*AFLA*/ 	int (*error_remove_page)(struct address_space *, struct page *);
// /*AFLA*/ 
// /*AFLA*/ 	/* swapfile support */
// /*AFLA*/ 	int (*swap_activate)(struct swap_info_struct *sis, struct file *file,
// /*AFLA*/ 				sector_t *span);
// /*AFLA*/ 	void (*swap_deactivate)(struct file *file);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern const struct address_space_operations empty_aops;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * pagecache_write_begin/pagecache_write_end must be used by general code
// /*AFLA*/  * to write into the pagecache.
// /*AFLA*/  */
// /*AFLA*/ int pagecache_write_begin(struct file *, struct address_space *mapping,
// /*AFLA*/ 				loff_t pos, unsigned len, unsigned flags,
// /*AFLA*/ 				struct page **pagep, void **fsdata);
// /*AFLA*/ 
// /*AFLA*/ int pagecache_write_end(struct file *, struct address_space *mapping,
// /*AFLA*/ 				loff_t pos, unsigned len, unsigned copied,
// /*AFLA*/ 				struct page *page, void *fsdata);
// /*AFLA*/ 
// /*AFLA*/ struct address_space {
// /*AFLA*/ 	struct inode		*host;		/* owner: inode, block_device */
// /*AFLA*/ 	struct radix_tree_root	page_tree;	/* radix tree of all pages */
// /*AFLA*/ 	spinlock_t		tree_lock;	/* and lock protecting it */
// /*AFLA*/ 	atomic_t		i_mmap_writable;/* count VM_SHARED mappings */
// /*AFLA*/ 	struct rb_root		i_mmap;		/* tree of private and shared mappings */
// /*AFLA*/ 	struct rw_semaphore	i_mmap_rwsem;	/* protect tree, count, list */
// /*AFLA*/ 	/* Protected by tree_lock together with the radix tree */
// /*AFLA*/ 	unsigned long		nrpages;	/* number of total pages */
// /*AFLA*/ 	/* number of shadow or DAX exceptional entries */
// /*AFLA*/ 	unsigned long		nrexceptional;
// /*AFLA*/ 	pgoff_t			writeback_index;/* writeback starts here */
// /*AFLA*/ 	const struct address_space_operations *a_ops;	/* methods */
// /*AFLA*/ 	unsigned long		flags;		/* error bits */
// /*AFLA*/ 	spinlock_t		private_lock;	/* for use by the address_space */
// /*AFLA*/ 	gfp_t			gfp_mask;	/* implicit gfp mask for allocations */
// /*AFLA*/ 	struct list_head	private_list;	/* ditto */
// /*AFLA*/ 	void			*private_data;	/* ditto */
// /*AFLA*/ } __attribute__((aligned(sizeof(long))));
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * On most architectures that alignment is already the case; but
// /*AFLA*/ 	 * must be enforced here for CRIS, to let the least significant bit
// /*AFLA*/ 	 * of struct page's "mapping" pointer be used for PAGE_MAPPING_ANON.
// /*AFLA*/ 	 */
// /*AFLA*/ struct request_queue;
// /*AFLA*/ 
// /*AFLA*/ struct block_device {
// /*AFLA*/ 	dev_t			bd_dev;  /* not a kdev_t - it's a search key */
// /*AFLA*/ 	int			bd_openers;
// /*AFLA*/ 	struct inode *		bd_inode;	/* will die */
// /*AFLA*/ 	struct super_block *	bd_super;
// /*AFLA*/ 	struct mutex		bd_mutex;	/* open/close mutex */
// /*AFLA*/ 	void *			bd_claiming;
// /*AFLA*/ 	void *			bd_holder;
// /*AFLA*/ 	int			bd_holders;
// /*AFLA*/ 	bool			bd_write_holder;
// /*AFLA*/ #ifdef CONFIG_SYSFS
// /*AFLA*/ 	struct list_head	bd_holder_disks;
// /*AFLA*/ #endif
// /*AFLA*/ 	struct block_device *	bd_contains;
// /*AFLA*/ 	unsigned		bd_block_size;
// /*AFLA*/ 	struct hd_struct *	bd_part;
// /*AFLA*/ 	/* number of times partitions within this device have been opened. */
// /*AFLA*/ 	unsigned		bd_part_count;
// /*AFLA*/ 	int			bd_invalidated;
// /*AFLA*/ 	struct gendisk *	bd_disk;
// /*AFLA*/ 	struct request_queue *  bd_queue;
// /*AFLA*/ 	struct list_head	bd_list;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Private data.  You must have bd_claim'ed the block_device
// /*AFLA*/ 	 * to use this.  NOTE:  bd_claim allows an owner to claim
// /*AFLA*/ 	 * the same device multiple times, the owner must take special
// /*AFLA*/ 	 * care to not mess up bd_private for that case.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long		bd_private;
// /*AFLA*/ 
// /*AFLA*/ 	/* The counter of freeze processes */
// /*AFLA*/ 	int			bd_fsfreeze_count;
// /*AFLA*/ 	/* Mutex for freeze */
// /*AFLA*/ 	struct mutex		bd_fsfreeze_mutex;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Radix-tree tags, for tagging dirty and writeback pages within the pagecache
// /*AFLA*/  * radix trees
// /*AFLA*/  */
// /*AFLA*/ #define PAGECACHE_TAG_DIRTY	0
// /*AFLA*/ #define PAGECACHE_TAG_WRITEBACK	1
// /*AFLA*/ #define PAGECACHE_TAG_TOWRITE	2
// /*AFLA*/ 
// /*AFLA*/ int mapping_tagged(struct address_space *mapping, int tag);
// /*AFLA*/ 
// /*AFLA*/ static inline void i_mmap_lock_write(struct address_space *mapping)
// /*AFLA*/ {
// /*AFLA*/ 	down_write(&mapping->i_mmap_rwsem);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void i_mmap_unlock_write(struct address_space *mapping)
// /*AFLA*/ {
// /*AFLA*/ 	up_write(&mapping->i_mmap_rwsem);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void i_mmap_lock_read(struct address_space *mapping)
// /*AFLA*/ {
// /*AFLA*/ 	down_read(&mapping->i_mmap_rwsem);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void i_mmap_unlock_read(struct address_space *mapping)
// /*AFLA*/ {
// /*AFLA*/ 	up_read(&mapping->i_mmap_rwsem);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Might pages of this file be mapped into userspace?
// /*AFLA*/  */
// /*AFLA*/ static inline int mapping_mapped(struct address_space *mapping)
// /*AFLA*/ {
// /*AFLA*/ 	return	!RB_EMPTY_ROOT(&mapping->i_mmap);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Might pages of this file have been modified in userspace?
// /*AFLA*/  * Note that i_mmap_writable counts all VM_SHARED vmas: do_mmap_pgoff
// /*AFLA*/  * marks vma as VM_SHARED if it is shared, and the file was opened for
// /*AFLA*/  * writing i.e. vma may be mprotected writable even if now readonly.
// /*AFLA*/  *
// /*AFLA*/  * If i_mmap_writable is negative, no new writable mappings are allowed. You
// /*AFLA*/  * can only deny writable mappings, if none exists right now.
// /*AFLA*/  */
// /*AFLA*/ static inline int mapping_writably_mapped(struct address_space *mapping)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_read(&mapping->i_mmap_writable) > 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int mapping_map_writable(struct address_space *mapping)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_inc_unless_negative(&mapping->i_mmap_writable) ?
// /*AFLA*/ 		0 : -EPERM;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void mapping_unmap_writable(struct address_space *mapping)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_dec(&mapping->i_mmap_writable);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int mapping_deny_writable(struct address_space *mapping)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_dec_unless_positive(&mapping->i_mmap_writable) ?
// /*AFLA*/ 		0 : -EBUSY;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void mapping_allow_writable(struct address_space *mapping)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_inc(&mapping->i_mmap_writable);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Use sequence counter to get consistent i_size on 32-bit processors.
// /*AFLA*/  */
// /*AFLA*/ #if BITS_PER_LONG==32 && defined(CONFIG_SMP)
// /*AFLA*/ #include <linux/seqlock.h>
// /*AFLA*/ #define __NEED_I_SIZE_ORDERED
// /*AFLA*/ #define i_size_ordered_init(inode) seqcount_init(&inode->i_size_seqcount)
// /*AFLA*/ #else
// /*AFLA*/ #define i_size_ordered_init(inode) do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct posix_acl;
// /*AFLA*/ #define ACL_NOT_CACHED ((void *)(-1))
// /*AFLA*/ #define ACL_DONT_CACHE ((void *)(-3))
// /*AFLA*/ 
// /*AFLA*/ static inline struct posix_acl *
// /*AFLA*/ uncached_acl_sentinel(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	return (void *)task + 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool
// /*AFLA*/ is_uncached_acl(struct posix_acl *acl)
// /*AFLA*/ {
// /*AFLA*/ 	return (long)acl & 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define IOP_FASTPERM	0x0001
// /*AFLA*/ #define IOP_LOOKUP	0x0002
// /*AFLA*/ #define IOP_NOFOLLOW	0x0004
// /*AFLA*/ #define IOP_XATTR	0x0008
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Keep mostly read-only and often accessed (especially for
// /*AFLA*/  * the RCU path lookup and 'stat' data) fields at the beginning
// /*AFLA*/  * of the 'struct inode'
// /*AFLA*/  */
// /*AFLA*/ struct inode {
// /*AFLA*/ 	umode_t			i_mode;
// /*AFLA*/ 	unsigned short		i_opflags;
// /*AFLA*/ 	kuid_t			i_uid;
// /*AFLA*/ 	kgid_t			i_gid;
// /*AFLA*/ 	unsigned int		i_flags;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FS_POSIX_ACL
// /*AFLA*/ 	struct posix_acl	*i_acl;
// /*AFLA*/ 	struct posix_acl	*i_default_acl;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	const struct inode_operations	*i_op;
// /*AFLA*/ 	struct super_block	*i_sb;
// /*AFLA*/ 	struct address_space	*i_mapping;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SECURITY
// /*AFLA*/ 	void			*i_security;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	/* Stat data, not accessed from path walking */
// /*AFLA*/ 	unsigned long		i_ino;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Filesystems may only read i_nlink directly.  They shall use the
// /*AFLA*/ 	 * following functions for modification:
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *    (set|clear|inc|drop)_nlink
// /*AFLA*/ 	 *    inode_(inc|dec)_link_count
// /*AFLA*/ 	 */
// /*AFLA*/ 	union {
// /*AFLA*/ 		const unsigned int i_nlink;
// /*AFLA*/ 		unsigned int __i_nlink;
// /*AFLA*/ 	};
// /*AFLA*/ 	dev_t			i_rdev;
// /*AFLA*/ 	loff_t			i_size;
// /*AFLA*/ 	struct timespec		i_atime;
// /*AFLA*/ 	struct timespec		i_mtime;
// /*AFLA*/ 	struct timespec		i_ctime;
// /*AFLA*/ 	spinlock_t		i_lock;	/* i_blocks, i_bytes, maybe i_size */
// /*AFLA*/ 	unsigned short          i_bytes;
// /*AFLA*/ 	unsigned int		i_blkbits;
// /*AFLA*/ 	blkcnt_t		i_blocks;
// /*AFLA*/ 
// /*AFLA*/ #ifdef __NEED_I_SIZE_ORDERED
// /*AFLA*/ 	seqcount_t		i_size_seqcount;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	/* Misc */
// /*AFLA*/ 	unsigned long		i_state;
// /*AFLA*/ 	struct rw_semaphore	i_rwsem;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long		dirtied_when;	/* jiffies of first dirtying */
// /*AFLA*/ 	unsigned long		dirtied_time_when;
// /*AFLA*/ 
// /*AFLA*/ 	struct hlist_node	i_hash;
// /*AFLA*/ 	struct list_head	i_io_list;	/* backing dev IO list */
// /*AFLA*/ #ifdef CONFIG_CGROUP_WRITEBACK
// /*AFLA*/ 	struct bdi_writeback	*i_wb;		/* the associated cgroup wb */
// /*AFLA*/ 
// /*AFLA*/ 	/* foreign inode detection, see wbc_detach_inode() */
// /*AFLA*/ 	int			i_wb_frn_winner;
// /*AFLA*/ 	u16			i_wb_frn_avg_time;
// /*AFLA*/ 	u16			i_wb_frn_history;
// /*AFLA*/ #endif
// /*AFLA*/ 	struct list_head	i_lru;		/* inode LRU list */
// /*AFLA*/ 	struct list_head	i_sb_list;
// /*AFLA*/ 	struct list_head	i_wb_list;	/* backing dev writeback list */
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct hlist_head	i_dentry;
// /*AFLA*/ 		struct rcu_head		i_rcu;
// /*AFLA*/ 	};
// /*AFLA*/ 	u64			i_version;
// /*AFLA*/ 	atomic_t		i_count;
// /*AFLA*/ 	atomic_t		i_dio_count;
// /*AFLA*/ 	atomic_t		i_writecount;
// /*AFLA*/ #ifdef CONFIG_IMA
// /*AFLA*/ 	atomic_t		i_readcount; /* struct files open RO */
// /*AFLA*/ #endif
// /*AFLA*/ 	const struct file_operations	*i_fop;	/* former ->i_op->default_file_ops */
// /*AFLA*/ 	struct file_lock_context	*i_flctx;
// /*AFLA*/ 	struct address_space	i_data;
// /*AFLA*/ 	struct list_head	i_devices;
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct pipe_inode_info	*i_pipe;
// /*AFLA*/ 		struct block_device	*i_bdev;
// /*AFLA*/ 		struct cdev		*i_cdev;
// /*AFLA*/ 		char			*i_link;
// /*AFLA*/ 		unsigned		i_dir_seq;
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ 	__u32			i_generation;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FSNOTIFY
// /*AFLA*/ 	__u32			i_fsnotify_mask; /* all events this inode cares about */
// /*AFLA*/ 	struct hlist_head	i_fsnotify_marks;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if IS_ENABLED(CONFIG_FS_ENCRYPTION)
// /*AFLA*/ 	struct fscrypt_info	*i_crypt_info;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	void			*i_private; /* fs or device private pointer */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline int inode_unhashed(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return hlist_unhashed(&inode->i_hash);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * inode->i_mutex nesting subclasses for the lock validator:
// /*AFLA*/  *
// /*AFLA*/  * 0: the object of the current VFS operation
// /*AFLA*/  * 1: parent
// /*AFLA*/  * 2: child/target
// /*AFLA*/  * 3: xattr
// /*AFLA*/  * 4: second non-directory
// /*AFLA*/  * 5: second parent (when locking independent directories in rename)
// /*AFLA*/  *
// /*AFLA*/  * I_MUTEX_NONDIR2 is for certain operations (such as rename) which lock two
// /*AFLA*/  * non-directories at once.
// /*AFLA*/  *
// /*AFLA*/  * The locking order between these classes is
// /*AFLA*/  * parent[2] -> child -> grandchild -> normal -> xattr -> second non-directory
// /*AFLA*/  */
// /*AFLA*/ enum inode_i_mutex_lock_class
// /*AFLA*/ {
// /*AFLA*/ 	I_MUTEX_NORMAL,
// /*AFLA*/ 	I_MUTEX_PARENT,
// /*AFLA*/ 	I_MUTEX_CHILD,
// /*AFLA*/ 	I_MUTEX_XATTR,
// /*AFLA*/ 	I_MUTEX_NONDIR2,
// /*AFLA*/ 	I_MUTEX_PARENT2,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline void inode_lock(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	down_write(&inode->i_rwsem);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void inode_unlock(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	up_write(&inode->i_rwsem);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void inode_lock_shared(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	down_read(&inode->i_rwsem);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void inode_unlock_shared(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	up_read(&inode->i_rwsem);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int inode_trylock(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return down_write_trylock(&inode->i_rwsem);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int inode_trylock_shared(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return down_read_trylock(&inode->i_rwsem);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int inode_is_locked(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return rwsem_is_locked(&inode->i_rwsem);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void inode_lock_nested(struct inode *inode, unsigned subclass)
// /*AFLA*/ {
// /*AFLA*/ 	down_write_nested(&inode->i_rwsem, subclass);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void lock_two_nondirectories(struct inode *, struct inode*);
// /*AFLA*/ void unlock_two_nondirectories(struct inode *, struct inode*);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * NOTE: in a 32bit arch with a preemptable kernel and
// /*AFLA*/  * an UP compile the i_size_read/write must be atomic
// /*AFLA*/  * with respect to the local cpu (unlike with preempt disabled),
// /*AFLA*/  * but they don't need to be atomic with respect to other cpus like in
// /*AFLA*/  * true SMP (so they need either to either locally disable irq around
// /*AFLA*/  * the read or for example on x86 they can be still implemented as a
// /*AFLA*/  * cmpxchg8b without the need of the lock prefix). For SMP compiles
// /*AFLA*/  * and 64bit archs it makes no difference if preempt is enabled or not.
// /*AFLA*/  */
// /*AFLA*/ static inline loff_t i_size_read(const struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ #if BITS_PER_LONG==32 && defined(CONFIG_SMP)
// /*AFLA*/ 	loff_t i_size;
// /*AFLA*/ 	unsigned int seq;
// /*AFLA*/ 
// /*AFLA*/ 	do {
// /*AFLA*/ 		seq = read_seqcount_begin(&inode->i_size_seqcount);
// /*AFLA*/ 		i_size = inode->i_size;
// /*AFLA*/ 	} while (read_seqcount_retry(&inode->i_size_seqcount, seq));
// /*AFLA*/ 	return i_size;
// /*AFLA*/ #elif BITS_PER_LONG==32 && defined(CONFIG_PREEMPT)
// /*AFLA*/ 	loff_t i_size;
// /*AFLA*/ 
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	i_size = inode->i_size;
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ 	return i_size;
// /*AFLA*/ #else
// /*AFLA*/ 	return inode->i_size;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * NOTE: unlike i_size_read(), i_size_write() does need locking around it
// /*AFLA*/  * (normally i_mutex), otherwise on 32bit/SMP an update of i_size_seqcount
// /*AFLA*/  * can be lost, resulting in subsequent i_size_read() calls spinning forever.
// /*AFLA*/  */
// /*AFLA*/ static inline void i_size_write(struct inode *inode, loff_t i_size)
// /*AFLA*/ {
// /*AFLA*/ #if BITS_PER_LONG==32 && defined(CONFIG_SMP)
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	write_seqcount_begin(&inode->i_size_seqcount);
// /*AFLA*/ 	inode->i_size = i_size;
// /*AFLA*/ 	write_seqcount_end(&inode->i_size_seqcount);
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ #elif BITS_PER_LONG==32 && defined(CONFIG_PREEMPT)
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	inode->i_size = i_size;
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ #else
// /*AFLA*/ 	inode->i_size = i_size;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned iminor(const struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return MINOR(inode->i_rdev);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned imajor(const struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return MAJOR(inode->i_rdev);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct block_device *I_BDEV(struct inode *inode);
// /*AFLA*/ 
// /*AFLA*/ struct fown_struct {
// /*AFLA*/ 	rwlock_t lock;          /* protects pid, uid, euid fields */
// /*AFLA*/ 	struct pid *pid;	/* pid or -pgrp where SIGIO should be sent */
// /*AFLA*/ 	enum pid_type pid_type;	/* Kind of process group SIGIO should be sent to */
// /*AFLA*/ 	kuid_t uid, euid;	/* uid/euid of process setting the owner */
// /*AFLA*/ 	int signum;		/* posix.1b rt signal to be delivered on IO */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Track a single file's readahead state
// /*AFLA*/  */
// /*AFLA*/ struct file_ra_state {
// /*AFLA*/ 	pgoff_t start;			/* where readahead started */
// /*AFLA*/ 	unsigned int size;		/* # of readahead pages */
// /*AFLA*/ 	unsigned int async_size;	/* do asynchronous readahead when
// /*AFLA*/ 					   there are only # of pages ahead */
// /*AFLA*/ 
// /*AFLA*/ 	unsigned int ra_pages;		/* Maximum readahead window */
// /*AFLA*/ 	unsigned int mmap_miss;		/* Cache miss stat for mmap accesses */
// /*AFLA*/ 	loff_t prev_pos;		/* Cache last read() position */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Check if @index falls in the readahead windows.
// /*AFLA*/  */
// /*AFLA*/ static inline int ra_has_index(struct file_ra_state *ra, pgoff_t index)
// /*AFLA*/ {
// /*AFLA*/ 	return (index >= ra->start &&
// /*AFLA*/ 		index <  ra->start + ra->size);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct file {
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct llist_node	fu_llist;
// /*AFLA*/ 		struct rcu_head 	fu_rcuhead;
// /*AFLA*/ 	} f_u;
// /*AFLA*/ 	struct path		f_path;
// /*AFLA*/ 	struct inode		*f_inode;	/* cached value */
// /*AFLA*/ 	const struct file_operations	*f_op;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Protects f_ep_links, f_flags.
// /*AFLA*/ 	 * Must not be taken from IRQ context.
// /*AFLA*/ 	 */
// /*AFLA*/ 	spinlock_t		f_lock;
// /*AFLA*/ 	atomic_long_t		f_count;
// /*AFLA*/ 	unsigned int 		f_flags;
// /*AFLA*/ 	fmode_t			f_mode;
// /*AFLA*/ 	struct mutex		f_pos_lock;
// /*AFLA*/ 	loff_t			f_pos;
// /*AFLA*/ 	struct fown_struct	f_owner;
// /*AFLA*/ 	const struct cred	*f_cred;
// /*AFLA*/ 	struct file_ra_state	f_ra;
// /*AFLA*/ 
// /*AFLA*/ 	u64			f_version;
// /*AFLA*/ #ifdef CONFIG_SECURITY
// /*AFLA*/ 	void			*f_security;
// /*AFLA*/ #endif
// /*AFLA*/ 	/* needed for tty driver, and maybe others */
// /*AFLA*/ 	void			*private_data;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_EPOLL
// /*AFLA*/ 	/* Used by fs/eventpoll.c to link all the hooks to this file */
// /*AFLA*/ 	struct list_head	f_ep_links;
// /*AFLA*/ 	struct list_head	f_tfile_llink;
// /*AFLA*/ #endif /* #ifdef CONFIG_EPOLL */
// /*AFLA*/ 	struct address_space	*f_mapping;
// /*AFLA*/ } __attribute__((aligned(4)));	/* lest something weird decides that 2 is OK */
// /*AFLA*/ 
// /*AFLA*/ struct file_handle {
// /*AFLA*/ 	__u32 handle_bytes;
// /*AFLA*/ 	int handle_type;
// /*AFLA*/ 	/* file identifier */
// /*AFLA*/ 	unsigned char f_handle[0];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline struct file *get_file(struct file *f)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_long_inc(&f->f_count);
// /*AFLA*/ 	return f;
// /*AFLA*/ }
// /*AFLA*/ #define get_file_rcu(x) atomic_long_inc_not_zero(&(x)->f_count)
// /*AFLA*/ #define fput_atomic(x)	atomic_long_add_unless(&(x)->f_count, -1, 1)
// /*AFLA*/ #define file_count(x)	atomic_long_read(&(x)->f_count)
// /*AFLA*/ 
// /*AFLA*/ #define	MAX_NON_LFS	((1UL<<31) - 1)
// /*AFLA*/ 
// /*AFLA*/ /* Page cache limit. The filesystems should put that into their s_maxbytes 
// /*AFLA*/    limits, otherwise bad things can happen in VM. */ 
// /*AFLA*/ #if BITS_PER_LONG==32
// /*AFLA*/ #define MAX_LFS_FILESIZE	(((loff_t)PAGE_SIZE << (BITS_PER_LONG-1))-1)
// /*AFLA*/ #elif BITS_PER_LONG==64
// /*AFLA*/ #define MAX_LFS_FILESIZE 	((loff_t)0x7fffffffffffffffLL)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define FL_POSIX	1
// /*AFLA*/ #define FL_FLOCK	2
// /*AFLA*/ #define FL_DELEG	4	/* NFSv4 delegation */
// /*AFLA*/ #define FL_ACCESS	8	/* not trying to lock, just looking */
// /*AFLA*/ #define FL_EXISTS	16	/* when unlocking, test for existence */
// /*AFLA*/ #define FL_LEASE	32	/* lease held on this file */
// /*AFLA*/ #define FL_CLOSE	64	/* unlock on close */
// /*AFLA*/ #define FL_SLEEP	128	/* A blocking lock */
// /*AFLA*/ #define FL_DOWNGRADE_PENDING	256 /* Lease is being downgraded */
// /*AFLA*/ #define FL_UNLOCK_PENDING	512 /* Lease is being broken */
// /*AFLA*/ #define FL_OFDLCK	1024	/* lock is "owned" by struct file */
// /*AFLA*/ #define FL_LAYOUT	2048	/* outstanding pNFS layout */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Special return value from posix_lock_file() and vfs_lock_file() for
// /*AFLA*/  * asynchronous locking.
// /*AFLA*/  */
// /*AFLA*/ #define FILE_LOCK_DEFERRED 1
// /*AFLA*/ 
// /*AFLA*/ /* legacy typedef, should eventually be removed */
// /*AFLA*/ typedef void *fl_owner_t;
// /*AFLA*/ 
// /*AFLA*/ struct file_lock;
// /*AFLA*/ 
// /*AFLA*/ struct file_lock_operations {
// /*AFLA*/ 	void (*fl_copy_lock)(struct file_lock *, struct file_lock *);
// /*AFLA*/ 	void (*fl_release_private)(struct file_lock *);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct lock_manager_operations {
// /*AFLA*/ 	int (*lm_compare_owner)(struct file_lock *, struct file_lock *);
// /*AFLA*/ 	unsigned long (*lm_owner_key)(struct file_lock *);
// /*AFLA*/ 	fl_owner_t (*lm_get_owner)(fl_owner_t);
// /*AFLA*/ 	void (*lm_put_owner)(fl_owner_t);
// /*AFLA*/ 	void (*lm_notify)(struct file_lock *);	/* unblock callback */
// /*AFLA*/ 	int (*lm_grant)(struct file_lock *, int);
// /*AFLA*/ 	bool (*lm_break)(struct file_lock *);
// /*AFLA*/ 	int (*lm_change)(struct file_lock *, int, struct list_head *);
// /*AFLA*/ 	void (*lm_setup)(struct file_lock *, void **);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct lock_manager {
// /*AFLA*/ 	struct list_head list;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * NFSv4 and up also want opens blocked during the grace period;
// /*AFLA*/ 	 * NLM doesn't care:
// /*AFLA*/ 	 */
// /*AFLA*/ 	bool block_opens;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct net;
// /*AFLA*/ void locks_start_grace(struct net *, struct lock_manager *);
// /*AFLA*/ void locks_end_grace(struct lock_manager *);
// /*AFLA*/ int locks_in_grace(struct net *);
// /*AFLA*/ int opens_in_grace(struct net *);
// /*AFLA*/ 
// /*AFLA*/ /* that will die - we need it for nfs_lock_info */
// /*AFLA*/ #include <linux/nfs_fs_i.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * struct file_lock represents a generic "file lock". It's used to represent
// /*AFLA*/  * POSIX byte range locks, BSD (flock) locks, and leases. It's important to
// /*AFLA*/  * note that the same struct is used to represent both a request for a lock and
// /*AFLA*/  * the lock itself, but the same object is never used for both.
// /*AFLA*/  *
// /*AFLA*/  * FIXME: should we create a separate "struct lock_request" to help distinguish
// /*AFLA*/  * these two uses?
// /*AFLA*/  *
// /*AFLA*/  * The varous i_flctx lists are ordered by:
// /*AFLA*/  *
// /*AFLA*/  * 1) lock owner
// /*AFLA*/  * 2) lock range start
// /*AFLA*/  * 3) lock range end
// /*AFLA*/  *
// /*AFLA*/  * Obviously, the last two criteria only matter for POSIX locks.
// /*AFLA*/  */
// /*AFLA*/ struct file_lock {
// /*AFLA*/ 	struct file_lock *fl_next;	/* singly linked list for this inode  */
// /*AFLA*/ 	struct list_head fl_list;	/* link into file_lock_context */
// /*AFLA*/ 	struct hlist_node fl_link;	/* node in global lists */
// /*AFLA*/ 	struct list_head fl_block;	/* circular list of blocked processes */
// /*AFLA*/ 	fl_owner_t fl_owner;
// /*AFLA*/ 	unsigned int fl_flags;
// /*AFLA*/ 	unsigned char fl_type;
// /*AFLA*/ 	unsigned int fl_pid;
// /*AFLA*/ 	int fl_link_cpu;		/* what cpu's list is this on? */
// /*AFLA*/ 	struct pid *fl_nspid;
// /*AFLA*/ 	wait_queue_head_t fl_wait;
// /*AFLA*/ 	struct file *fl_file;
// /*AFLA*/ 	loff_t fl_start;
// /*AFLA*/ 	loff_t fl_end;
// /*AFLA*/ 
// /*AFLA*/ 	struct fasync_struct *	fl_fasync; /* for lease break notifications */
// /*AFLA*/ 	/* for lease breaks: */
// /*AFLA*/ 	unsigned long fl_break_time;
// /*AFLA*/ 	unsigned long fl_downgrade_time;
// /*AFLA*/ 
// /*AFLA*/ 	const struct file_lock_operations *fl_ops;	/* Callbacks for filesystems */
// /*AFLA*/ 	const struct lock_manager_operations *fl_lmops;	/* Callbacks for lockmanagers */
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct nfs_lock_info	nfs_fl;
// /*AFLA*/ 		struct nfs4_lock_info	nfs4_fl;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			struct list_head link;	/* link in AFS vnode's pending_locks list */
// /*AFLA*/ 			int state;		/* state of grant or error if -ve */
// /*AFLA*/ 		} afs;
// /*AFLA*/ 	} fl_u;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct file_lock_context {
// /*AFLA*/ 	spinlock_t		flc_lock;
// /*AFLA*/ 	struct list_head	flc_flock;
// /*AFLA*/ 	struct list_head	flc_posix;
// /*AFLA*/ 	struct list_head	flc_lease;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* The following constant reflects the upper bound of the file/locking space */
// /*AFLA*/ #ifndef OFFSET_MAX
// /*AFLA*/ #define INT_LIMIT(x)	(~((x)1 << (sizeof(x)*8 - 1)))
// /*AFLA*/ #define OFFSET_MAX	INT_LIMIT(loff_t)
// /*AFLA*/ #define OFFT_OFFSET_MAX	INT_LIMIT(off_t)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <linux/fcntl.h>
// /*AFLA*/ 
// /*AFLA*/ extern void send_sigio(struct fown_struct *fown, int fd, int band);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Return the inode to use for locking
// /*AFLA*/  *
// /*AFLA*/  * For overlayfs this should be the overlay inode, not the real inode returned
// /*AFLA*/  * by file_inode().  For any other fs file_inode(filp) and locks_inode(filp) are
// /*AFLA*/  * equal.
// /*AFLA*/  */
// /*AFLA*/ static inline struct inode *locks_inode(const struct file *f)
// /*AFLA*/ {
// /*AFLA*/ 	return f->f_path.dentry->d_inode;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FILE_LOCKING
// /*AFLA*/ extern int fcntl_getlk(struct file *, unsigned int, struct flock __user *);
// /*AFLA*/ extern int fcntl_setlk(unsigned int, struct file *, unsigned int,
// /*AFLA*/ 			struct flock __user *);
// /*AFLA*/ 
// /*AFLA*/ #if BITS_PER_LONG == 32
// /*AFLA*/ extern int fcntl_getlk64(struct file *, unsigned int, struct flock64 __user *);
// /*AFLA*/ extern int fcntl_setlk64(unsigned int, struct file *, unsigned int,
// /*AFLA*/ 			struct flock64 __user *);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern int fcntl_setlease(unsigned int fd, struct file *filp, long arg);
// /*AFLA*/ extern int fcntl_getlease(struct file *filp);
// /*AFLA*/ 
// /*AFLA*/ /* fs/locks.c */
// /*AFLA*/ void locks_free_lock_context(struct inode *inode);
// /*AFLA*/ void locks_free_lock(struct file_lock *fl);
// /*AFLA*/ extern void locks_init_lock(struct file_lock *);
// /*AFLA*/ extern struct file_lock * locks_alloc_lock(void);
// /*AFLA*/ extern void locks_copy_lock(struct file_lock *, struct file_lock *);
// /*AFLA*/ extern void locks_copy_conflock(struct file_lock *, struct file_lock *);
// /*AFLA*/ extern void locks_remove_posix(struct file *, fl_owner_t);
// /*AFLA*/ extern void locks_remove_file(struct file *);
// /*AFLA*/ extern void locks_release_private(struct file_lock *);
// /*AFLA*/ extern void posix_test_lock(struct file *, struct file_lock *);
// /*AFLA*/ extern int posix_lock_file(struct file *, struct file_lock *, struct file_lock *);
// /*AFLA*/ extern int posix_unblock_lock(struct file_lock *);
// /*AFLA*/ extern int vfs_test_lock(struct file *, struct file_lock *);
// /*AFLA*/ extern int vfs_lock_file(struct file *, unsigned int, struct file_lock *, struct file_lock *);
// /*AFLA*/ extern int vfs_cancel_lock(struct file *filp, struct file_lock *fl);
// /*AFLA*/ extern int locks_lock_inode_wait(struct inode *inode, struct file_lock *fl);
// /*AFLA*/ extern int __break_lease(struct inode *inode, unsigned int flags, unsigned int type);
// /*AFLA*/ extern void lease_get_mtime(struct inode *, struct timespec *time);
// /*AFLA*/ extern int generic_setlease(struct file *, long, struct file_lock **, void **priv);
// /*AFLA*/ extern int vfs_setlease(struct file *, long, struct file_lock **, void **);
// /*AFLA*/ extern int lease_modify(struct file_lock *, int, struct list_head *);
// /*AFLA*/ struct files_struct;
// /*AFLA*/ extern void show_fd_locks(struct seq_file *f,
// /*AFLA*/ 			 struct file *filp, struct files_struct *files);
// /*AFLA*/ #else /* !CONFIG_FILE_LOCKING */
// /*AFLA*/ static inline int fcntl_getlk(struct file *file, unsigned int cmd,
// /*AFLA*/ 			      struct flock __user *user)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int fcntl_setlk(unsigned int fd, struct file *file,
// /*AFLA*/ 			      unsigned int cmd, struct flock __user *user)
// /*AFLA*/ {
// /*AFLA*/ 	return -EACCES;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #if BITS_PER_LONG == 32
// /*AFLA*/ static inline int fcntl_getlk64(struct file *file, unsigned int cmd,
// /*AFLA*/ 				struct flock64 __user *user)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int fcntl_setlk64(unsigned int fd, struct file *file,
// /*AFLA*/ 				unsigned int cmd, struct flock64 __user *user)
// /*AFLA*/ {
// /*AFLA*/ 	return -EACCES;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ static inline int fcntl_setlease(unsigned int fd, struct file *filp, long arg)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int fcntl_getlease(struct file *filp)
// /*AFLA*/ {
// /*AFLA*/ 	return F_UNLCK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ locks_free_lock_context(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void locks_init_lock(struct file_lock *fl)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void locks_copy_conflock(struct file_lock *new, struct file_lock *fl)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void locks_copy_lock(struct file_lock *new, struct file_lock *fl)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void locks_remove_posix(struct file *filp, fl_owner_t owner)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void locks_remove_file(struct file *filp)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void posix_test_lock(struct file *filp, struct file_lock *fl)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int posix_lock_file(struct file *filp, struct file_lock *fl,
// /*AFLA*/ 				  struct file_lock *conflock)
// /*AFLA*/ {
// /*AFLA*/ 	return -ENOLCK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int posix_unblock_lock(struct file_lock *waiter)
// /*AFLA*/ {
// /*AFLA*/ 	return -ENOENT;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int vfs_test_lock(struct file *filp, struct file_lock *fl)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int vfs_lock_file(struct file *filp, unsigned int cmd,
// /*AFLA*/ 				struct file_lock *fl, struct file_lock *conf)
// /*AFLA*/ {
// /*AFLA*/ 	return -ENOLCK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int vfs_cancel_lock(struct file *filp, struct file_lock *fl)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int locks_lock_inode_wait(struct inode *inode, struct file_lock *fl)
// /*AFLA*/ {
// /*AFLA*/ 	return -ENOLCK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __break_lease(struct inode *inode, unsigned int mode, unsigned int type)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void lease_get_mtime(struct inode *inode, struct timespec *time)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int generic_setlease(struct file *filp, long arg,
// /*AFLA*/ 				    struct file_lock **flp, void **priv)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int vfs_setlease(struct file *filp, long arg,
// /*AFLA*/ 			       struct file_lock **lease, void **priv)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int lease_modify(struct file_lock *fl, int arg,
// /*AFLA*/ 			       struct list_head *dispose)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct files_struct;
// /*AFLA*/ static inline void show_fd_locks(struct seq_file *f,
// /*AFLA*/ 			struct file *filp, struct files_struct *files) {}
// /*AFLA*/ #endif /* !CONFIG_FILE_LOCKING */
// /*AFLA*/ 
// /*AFLA*/ static inline struct inode *file_inode(const struct file *f)
// /*AFLA*/ {
// /*AFLA*/ 	return f->f_inode;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct dentry *file_dentry(const struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	return d_real(file->f_path.dentry, file_inode(file), 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int locks_lock_file_wait(struct file *filp, struct file_lock *fl)
// /*AFLA*/ {
// /*AFLA*/ 	return locks_lock_inode_wait(locks_inode(filp), fl);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct fasync_struct {
// /*AFLA*/ 	spinlock_t		fa_lock;
// /*AFLA*/ 	int			magic;
// /*AFLA*/ 	int			fa_fd;
// /*AFLA*/ 	struct fasync_struct	*fa_next; /* singly linked list */
// /*AFLA*/ 	struct file		*fa_file;
// /*AFLA*/ 	struct rcu_head		fa_rcu;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define FASYNC_MAGIC 0x4601
// /*AFLA*/ 
// /*AFLA*/ /* SMP safe fasync helpers: */
// /*AFLA*/ extern int fasync_helper(int, struct file *, int, struct fasync_struct **);
// /*AFLA*/ extern struct fasync_struct *fasync_insert_entry(int, struct file *, struct fasync_struct **, struct fasync_struct *);
// /*AFLA*/ extern int fasync_remove_entry(struct file *, struct fasync_struct **);
// /*AFLA*/ extern struct fasync_struct *fasync_alloc(void);
// /*AFLA*/ extern void fasync_free(struct fasync_struct *);
// /*AFLA*/ 
// /*AFLA*/ /* can be called from interrupts */
// /*AFLA*/ extern void kill_fasync(struct fasync_struct **, int, int);
// /*AFLA*/ 
// /*AFLA*/ extern void __f_setown(struct file *filp, struct pid *, enum pid_type, int force);
// /*AFLA*/ extern void f_setown(struct file *filp, unsigned long arg, int force);
// /*AFLA*/ extern void f_delown(struct file *filp);
// /*AFLA*/ extern pid_t f_getown(struct file *filp);
// /*AFLA*/ extern int send_sigurg(struct fown_struct *fown);
// /*AFLA*/ 
// /*AFLA*/ struct mm_struct;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  *	Umount options
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define MNT_FORCE	0x00000001	/* Attempt to forcibily umount */
// /*AFLA*/ #define MNT_DETACH	0x00000002	/* Just detach from the tree */
// /*AFLA*/ #define MNT_EXPIRE	0x00000004	/* Mark for expiry */
// /*AFLA*/ #define UMOUNT_NOFOLLOW	0x00000008	/* Don't follow symlink on umount */
// /*AFLA*/ #define UMOUNT_UNUSED	0x80000000	/* Flag guaranteed to be unused */
// /*AFLA*/ 
// /*AFLA*/ /* sb->s_iflags */
// /*AFLA*/ #define SB_I_CGROUPWB	0x00000001	/* cgroup-aware writeback enabled */
// /*AFLA*/ #define SB_I_NOEXEC	0x00000002	/* Ignore executables on this fs */
// /*AFLA*/ #define SB_I_NODEV	0x00000004	/* Ignore devices on this fs */
// /*AFLA*/ 
// /*AFLA*/ /* sb->s_iflags to limit user namespace mounts */
// /*AFLA*/ #define SB_I_USERNS_VISIBLE		0x00000010 /* fstype already mounted */
// /*AFLA*/ 
// /*AFLA*/ /* Possible states of 'frozen' field */
// /*AFLA*/ enum {
// /*AFLA*/ 	SB_UNFROZEN = 0,		/* FS is unfrozen */
// /*AFLA*/ 	SB_FREEZE_WRITE	= 1,		/* Writes, dir ops, ioctls frozen */
// /*AFLA*/ 	SB_FREEZE_PAGEFAULT = 2,	/* Page faults stopped as well */
// /*AFLA*/ 	SB_FREEZE_FS = 3,		/* For internal FS use (e.g. to stop
// /*AFLA*/ 					 * internal threads if needed) */
// /*AFLA*/ 	SB_FREEZE_COMPLETE = 4,		/* ->freeze_fs finished successfully */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define SB_FREEZE_LEVELS (SB_FREEZE_COMPLETE - 1)
// /*AFLA*/ 
// /*AFLA*/ struct sb_writers {
// /*AFLA*/ 	int				frozen;		/* Is sb frozen? */
// /*AFLA*/ 	wait_queue_head_t		wait_unfrozen;	/* for get_super_thawed() */
// /*AFLA*/ 	struct percpu_rw_semaphore	rw_sem[SB_FREEZE_LEVELS];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct super_block {
// /*AFLA*/ 	struct list_head	s_list;		/* Keep this first */
// /*AFLA*/ 	dev_t			s_dev;		/* search index; _not_ kdev_t */
// /*AFLA*/ 	unsigned char		s_blocksize_bits;
// /*AFLA*/ 	unsigned long		s_blocksize;
// /*AFLA*/ 	loff_t			s_maxbytes;	/* Max file size */
// /*AFLA*/ 	struct file_system_type	*s_type;
// /*AFLA*/ 	const struct super_operations	*s_op;
// /*AFLA*/ 	const struct dquot_operations	*dq_op;
// /*AFLA*/ 	const struct quotactl_ops	*s_qcop;
// /*AFLA*/ 	const struct export_operations *s_export_op;
// /*AFLA*/ 	unsigned long		s_flags;
// /*AFLA*/ 	unsigned long		s_iflags;	/* internal SB_I_* flags */
// /*AFLA*/ 	unsigned long		s_magic;
// /*AFLA*/ 	struct dentry		*s_root;
// /*AFLA*/ 	struct rw_semaphore	s_umount;
// /*AFLA*/ 	int			s_count;
// /*AFLA*/ 	atomic_t		s_active;
// /*AFLA*/ #ifdef CONFIG_SECURITY
// /*AFLA*/ 	void                    *s_security;
// /*AFLA*/ #endif
// /*AFLA*/ 	const struct xattr_handler **s_xattr;
// /*AFLA*/ 
// /*AFLA*/ 	const struct fscrypt_operations	*s_cop;
// /*AFLA*/ 
// /*AFLA*/ 	struct hlist_bl_head	s_anon;		/* anonymous dentries for (nfs) exporting */
// /*AFLA*/ 	struct list_head	s_mounts;	/* list of mounts; _not_ for fs use */
// /*AFLA*/ 	struct block_device	*s_bdev;
// /*AFLA*/ 	struct backing_dev_info *s_bdi;
// /*AFLA*/ 	struct mtd_info		*s_mtd;
// /*AFLA*/ 	struct hlist_node	s_instances;
// /*AFLA*/ 	unsigned int		s_quota_types;	/* Bitmask of supported quota types */
// /*AFLA*/ 	struct quota_info	s_dquot;	/* Diskquota specific options */
// /*AFLA*/ 
// /*AFLA*/ 	struct sb_writers	s_writers;
// /*AFLA*/ 
// /*AFLA*/ 	char s_id[32];				/* Informational name */
// /*AFLA*/ 	u8 s_uuid[16];				/* UUID */
// /*AFLA*/ 
// /*AFLA*/ 	void 			*s_fs_info;	/* Filesystem private info */
// /*AFLA*/ 	unsigned int		s_max_links;
// /*AFLA*/ 	fmode_t			s_mode;
// /*AFLA*/ 
// /*AFLA*/ 	/* Granularity of c/m/atime in ns.
// /*AFLA*/ 	   Cannot be worse than a second */
// /*AFLA*/ 	u32		   s_time_gran;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The next field is for VFS *only*. No filesystems have any business
// /*AFLA*/ 	 * even looking at it. You had been warned.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct mutex s_vfs_rename_mutex;	/* Kludge */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Filesystem subtype.  If non-empty the filesystem type field
// /*AFLA*/ 	 * in /proc/mounts will be "type.subtype"
// /*AFLA*/ 	 */
// /*AFLA*/ 	char *s_subtype;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Saved mount options for lazy filesystems using
// /*AFLA*/ 	 * generic_show_options()
// /*AFLA*/ 	 */
// /*AFLA*/ 	char __rcu *s_options;
// /*AFLA*/ 	const struct dentry_operations *s_d_op; /* default d_op for dentries */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Saved pool identifier for cleancache (-1 means none)
// /*AFLA*/ 	 */
// /*AFLA*/ 	int cleancache_poolid;
// /*AFLA*/ 
// /*AFLA*/ 	struct shrinker s_shrink;	/* per-sb shrinker handle */
// /*AFLA*/ 
// /*AFLA*/ 	/* Number of inodes with nlink == 0 but still referenced */
// /*AFLA*/ 	atomic_long_t s_remove_count;
// /*AFLA*/ 
// /*AFLA*/ 	/* Being remounted read-only */
// /*AFLA*/ 	int s_readonly_remount;
// /*AFLA*/ 
// /*AFLA*/ 	/* AIO completions deferred from interrupt context */
// /*AFLA*/ 	struct workqueue_struct *s_dio_done_wq;
// /*AFLA*/ 	struct hlist_head s_pins;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Owning user namespace and default context in which to
// /*AFLA*/ 	 * interpret filesystem uids, gids, quotas, device nodes,
// /*AFLA*/ 	 * xattrs and security labels.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct user_namespace *s_user_ns;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Keep the lru lists last in the structure so they always sit on their
// /*AFLA*/ 	 * own individual cachelines.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_lru		s_dentry_lru ____cacheline_aligned_in_smp;
// /*AFLA*/ 	struct list_lru		s_inode_lru ____cacheline_aligned_in_smp;
// /*AFLA*/ 	struct rcu_head		rcu;
// /*AFLA*/ 	struct work_struct	destroy_work;
// /*AFLA*/ 
// /*AFLA*/ 	struct mutex		s_sync_lock;	/* sync serialisation lock */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Indicates how deep in a filesystem stack this SB is
// /*AFLA*/ 	 */
// /*AFLA*/ 	int s_stack_depth;
// /*AFLA*/ 
// /*AFLA*/ 	/* s_inode_list_lock protects s_inodes */
// /*AFLA*/ 	spinlock_t		s_inode_list_lock ____cacheline_aligned_in_smp;
// /*AFLA*/ 	struct list_head	s_inodes;	/* all inodes */
// /*AFLA*/ 
// /*AFLA*/ 	spinlock_t		s_inode_wblist_lock;
// /*AFLA*/ 	struct list_head	s_inodes_wb;	/* writeback inodes */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Helper functions so that in most cases filesystems will
// /*AFLA*/  * not need to deal directly with kuid_t and kgid_t and can
// /*AFLA*/  * instead deal with the raw numeric values that are stored
// /*AFLA*/  * in the filesystem.
// /*AFLA*/  */
// /*AFLA*/ static inline uid_t i_uid_read(const struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return from_kuid(inode->i_sb->s_user_ns, inode->i_uid);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline gid_t i_gid_read(const struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return from_kgid(inode->i_sb->s_user_ns, inode->i_gid);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void i_uid_write(struct inode *inode, uid_t uid)
// /*AFLA*/ {
// /*AFLA*/ 	inode->i_uid = make_kuid(inode->i_sb->s_user_ns, uid);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void i_gid_write(struct inode *inode, gid_t gid)
// /*AFLA*/ {
// /*AFLA*/ 	inode->i_gid = make_kgid(inode->i_sb->s_user_ns, gid);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct timespec current_fs_time(struct super_block *sb);
// /*AFLA*/ extern struct timespec current_time(struct inode *inode);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Snapshotting support.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ void __sb_end_write(struct super_block *sb, int level);
// /*AFLA*/ int __sb_start_write(struct super_block *sb, int level, bool wait);
// /*AFLA*/ 
// /*AFLA*/ #define __sb_writers_acquired(sb, lev)	\
// /*AFLA*/ 	percpu_rwsem_acquire(&(sb)->s_writers.rw_sem[(lev)-1], 1, _THIS_IP_)
// /*AFLA*/ #define __sb_writers_release(sb, lev)	\
// /*AFLA*/ 	percpu_rwsem_release(&(sb)->s_writers.rw_sem[(lev)-1], 1, _THIS_IP_)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * sb_end_write - drop write access to a superblock
// /*AFLA*/  * @sb: the super we wrote to
// /*AFLA*/  *
// /*AFLA*/  * Decrement number of writers to the filesystem. Wake up possible waiters
// /*AFLA*/  * wanting to freeze the filesystem.
// /*AFLA*/  */
// /*AFLA*/ static inline void sb_end_write(struct super_block *sb)
// /*AFLA*/ {
// /*AFLA*/ 	__sb_end_write(sb, SB_FREEZE_WRITE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * sb_end_pagefault - drop write access to a superblock from a page fault
// /*AFLA*/  * @sb: the super we wrote to
// /*AFLA*/  *
// /*AFLA*/  * Decrement number of processes handling write page fault to the filesystem.
// /*AFLA*/  * Wake up possible waiters wanting to freeze the filesystem.
// /*AFLA*/  */
// /*AFLA*/ static inline void sb_end_pagefault(struct super_block *sb)
// /*AFLA*/ {
// /*AFLA*/ 	__sb_end_write(sb, SB_FREEZE_PAGEFAULT);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * sb_end_intwrite - drop write access to a superblock for internal fs purposes
// /*AFLA*/  * @sb: the super we wrote to
// /*AFLA*/  *
// /*AFLA*/  * Decrement fs-internal number of writers to the filesystem.  Wake up possible
// /*AFLA*/  * waiters wanting to freeze the filesystem.
// /*AFLA*/  */
// /*AFLA*/ static inline void sb_end_intwrite(struct super_block *sb)
// /*AFLA*/ {
// /*AFLA*/ 	__sb_end_write(sb, SB_FREEZE_FS);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * sb_start_write - get write access to a superblock
// /*AFLA*/  * @sb: the super we write to
// /*AFLA*/  *
// /*AFLA*/  * When a process wants to write data or metadata to a file system (i.e. dirty
// /*AFLA*/  * a page or an inode), it should embed the operation in a sb_start_write() -
// /*AFLA*/  * sb_end_write() pair to get exclusion against file system freezing. This
// /*AFLA*/  * function increments number of writers preventing freezing. If the file
// /*AFLA*/  * system is already frozen, the function waits until the file system is
// /*AFLA*/  * thawed.
// /*AFLA*/  *
// /*AFLA*/  * Since freeze protection behaves as a lock, users have to preserve
// /*AFLA*/  * ordering of freeze protection and other filesystem locks. Generally,
// /*AFLA*/  * freeze protection should be the outermost lock. In particular, we have:
// /*AFLA*/  *
// /*AFLA*/  * sb_start_write
// /*AFLA*/  *   -> i_mutex			(write path, truncate, directory ops, ...)
// /*AFLA*/  *   -> s_umount		(freeze_super, thaw_super)
// /*AFLA*/  */
// /*AFLA*/ static inline void sb_start_write(struct super_block *sb)
// /*AFLA*/ {
// /*AFLA*/ 	__sb_start_write(sb, SB_FREEZE_WRITE, true);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sb_start_write_trylock(struct super_block *sb)
// /*AFLA*/ {
// /*AFLA*/ 	return __sb_start_write(sb, SB_FREEZE_WRITE, false);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * sb_start_pagefault - get write access to a superblock from a page fault
// /*AFLA*/  * @sb: the super we write to
// /*AFLA*/  *
// /*AFLA*/  * When a process starts handling write page fault, it should embed the
// /*AFLA*/  * operation into sb_start_pagefault() - sb_end_pagefault() pair to get
// /*AFLA*/  * exclusion against file system freezing. This is needed since the page fault
// /*AFLA*/  * is going to dirty a page. This function increments number of running page
// /*AFLA*/  * faults preventing freezing. If the file system is already frozen, the
// /*AFLA*/  * function waits until the file system is thawed.
// /*AFLA*/  *
// /*AFLA*/  * Since page fault freeze protection behaves as a lock, users have to preserve
// /*AFLA*/  * ordering of freeze protection and other filesystem locks. It is advised to
// /*AFLA*/  * put sb_start_pagefault() close to mmap_sem in lock ordering. Page fault
// /*AFLA*/  * handling code implies lock dependency:
// /*AFLA*/  *
// /*AFLA*/  * mmap_sem
// /*AFLA*/  *   -> sb_start_pagefault
// /*AFLA*/  */
// /*AFLA*/ static inline void sb_start_pagefault(struct super_block *sb)
// /*AFLA*/ {
// /*AFLA*/ 	__sb_start_write(sb, SB_FREEZE_PAGEFAULT, true);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * sb_start_intwrite - get write access to a superblock for internal fs purposes
// /*AFLA*/  * @sb: the super we write to
// /*AFLA*/  *
// /*AFLA*/  * This is the third level of protection against filesystem freezing. It is
// /*AFLA*/  * free for use by a filesystem. The only requirement is that it must rank
// /*AFLA*/  * below sb_start_pagefault.
// /*AFLA*/  *
// /*AFLA*/  * For example filesystem can call sb_start_intwrite() when starting a
// /*AFLA*/  * transaction which somewhat eases handling of freezing for internal sources
// /*AFLA*/  * of filesystem changes (internal fs threads, discarding preallocation on file
// /*AFLA*/  * close, etc.).
// /*AFLA*/  */
// /*AFLA*/ static inline void sb_start_intwrite(struct super_block *sb)
// /*AFLA*/ {
// /*AFLA*/ 	__sb_start_write(sb, SB_FREEZE_FS, true);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ extern bool inode_owner_or_capable(const struct inode *inode);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * VFS helper functions..
// /*AFLA*/  */
// /*AFLA*/ extern int vfs_create(struct inode *, struct dentry *, umode_t, bool);
// /*AFLA*/ extern int vfs_mkdir(struct inode *, struct dentry *, umode_t);
// /*AFLA*/ extern int vfs_mknod(struct inode *, struct dentry *, umode_t, dev_t);
// /*AFLA*/ extern int vfs_symlink(struct inode *, struct dentry *, const char *);
// /*AFLA*/ extern int vfs_link(struct dentry *, struct inode *, struct dentry *, struct inode **);
// /*AFLA*/ extern int vfs_rmdir(struct inode *, struct dentry *);
// /*AFLA*/ extern int vfs_unlink(struct inode *, struct dentry *, struct inode **);
// /*AFLA*/ extern int vfs_rename(struct inode *, struct dentry *, struct inode *, struct dentry *, struct inode **, unsigned int);
// /*AFLA*/ extern int vfs_whiteout(struct inode *, struct dentry *);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * VFS file helper functions.
// /*AFLA*/  */
// /*AFLA*/ extern void inode_init_owner(struct inode *inode, const struct inode *dir,
// /*AFLA*/ 			umode_t mode);
// /*AFLA*/ extern bool may_open_dev(const struct path *path);
// /*AFLA*/ /*
// /*AFLA*/  * VFS FS_IOC_FIEMAP helper definitions.
// /*AFLA*/  */
// /*AFLA*/ struct fiemap_extent_info {
// /*AFLA*/ 	unsigned int fi_flags;		/* Flags as passed from user */
// /*AFLA*/ 	unsigned int fi_extents_mapped;	/* Number of mapped extents */
// /*AFLA*/ 	unsigned int fi_extents_max;	/* Size of fiemap_extent array */
// /*AFLA*/ 	struct fiemap_extent __user *fi_extents_start; /* Start of
// /*AFLA*/ 							fiemap_extent array */
// /*AFLA*/ };
// /*AFLA*/ int fiemap_fill_next_extent(struct fiemap_extent_info *info, u64 logical,
// /*AFLA*/ 			    u64 phys, u64 len, u32 flags);
// /*AFLA*/ int fiemap_check_flags(struct fiemap_extent_info *fieinfo, u32 fs_flags);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * File types
// /*AFLA*/  *
// /*AFLA*/  * NOTE! These match bits 12..15 of stat.st_mode
// /*AFLA*/  * (ie "(i_mode >> 12) & 15").
// /*AFLA*/  */
// /*AFLA*/ #define DT_UNKNOWN	0
// /*AFLA*/ #define DT_FIFO		1
// /*AFLA*/ #define DT_CHR		2
// /*AFLA*/ #define DT_DIR		4
// /*AFLA*/ #define DT_BLK		6
// /*AFLA*/ #define DT_REG		8
// /*AFLA*/ #define DT_LNK		10
// /*AFLA*/ #define DT_SOCK		12
// /*AFLA*/ #define DT_WHT		14
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is the "filldir" function type, used by readdir() to let
// /*AFLA*/  * the kernel specify what kind of dirent layout it wants to have.
// /*AFLA*/  * This allows the kernel to read directories into kernel space or
// /*AFLA*/  * to have different dirent layouts depending on the binary type.
// /*AFLA*/  */
// /*AFLA*/ struct dir_context;
// /*AFLA*/ typedef int (*filldir_t)(struct dir_context *, const char *, int, loff_t, u64,
// /*AFLA*/ 			 unsigned);
// /*AFLA*/ 
// /*AFLA*/ struct dir_context {
// /*AFLA*/ 	const filldir_t actor;
// /*AFLA*/ 	loff_t pos;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct block_device_operations;
// /*AFLA*/ 
// /*AFLA*/ /* These macros are for out of kernel modules to test that
// /*AFLA*/  * the kernel supports the unlocked_ioctl and compat_ioctl
// /*AFLA*/  * fields in struct file_operations. */
// /*AFLA*/ #define HAVE_COMPAT_IOCTL 1
// /*AFLA*/ #define HAVE_UNLOCKED_IOCTL 1
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These flags let !MMU mmap() govern direct device mapping vs immediate
// /*AFLA*/  * copying more easily for MAP_PRIVATE, especially for ROM filesystems.
// /*AFLA*/  *
// /*AFLA*/  * NOMMU_MAP_COPY:	Copy can be mapped (MAP_PRIVATE)
// /*AFLA*/  * NOMMU_MAP_DIRECT:	Can be mapped directly (MAP_SHARED)
// /*AFLA*/  * NOMMU_MAP_READ:	Can be mapped for reading
// /*AFLA*/  * NOMMU_MAP_WRITE:	Can be mapped for writing
// /*AFLA*/  * NOMMU_MAP_EXEC:	Can be mapped for execution
// /*AFLA*/  */
// /*AFLA*/ #define NOMMU_MAP_COPY		0x00000001
// /*AFLA*/ #define NOMMU_MAP_DIRECT	0x00000008
// /*AFLA*/ #define NOMMU_MAP_READ		VM_MAYREAD
// /*AFLA*/ #define NOMMU_MAP_WRITE		VM_MAYWRITE
// /*AFLA*/ #define NOMMU_MAP_EXEC		VM_MAYEXEC
// /*AFLA*/ 
// /*AFLA*/ #define NOMMU_VMFLAGS \
// /*AFLA*/ 	(NOMMU_MAP_READ | NOMMU_MAP_WRITE | NOMMU_MAP_EXEC)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ struct iov_iter;
// /*AFLA*/ 
// /*AFLA*/ struct file_operations {
// /*AFLA*/ 	struct module *owner;
// /*AFLA*/ 	loff_t (*llseek) (struct file *, loff_t, int);
// /*AFLA*/ 	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
// /*AFLA*/ 	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
// /*AFLA*/ 	ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
// /*AFLA*/ 	ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
// /*AFLA*/ 	int (*iterate) (struct file *, struct dir_context *);
// /*AFLA*/ 	int (*iterate_shared) (struct file *, struct dir_context *);
// /*AFLA*/ 	unsigned int (*poll) (struct file *, struct poll_table_struct *);
// /*AFLA*/ 	long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
// /*AFLA*/ 	long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
// /*AFLA*/ 	int (*mmap) (struct file *, struct vm_area_struct *);
// /*AFLA*/ 	int (*open) (struct inode *, struct file *);
// /*AFLA*/ 	int (*flush) (struct file *, fl_owner_t id);
// /*AFLA*/ 	int (*release) (struct inode *, struct file *);
// /*AFLA*/ 	int (*fsync) (struct file *, loff_t, loff_t, int datasync);
// /*AFLA*/ 	int (*fasync) (int, struct file *, int);
// /*AFLA*/ 	int (*lock) (struct file *, int, struct file_lock *);
// /*AFLA*/ 	ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
// /*AFLA*/ 	unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
// /*AFLA*/ 	int (*check_flags)(int);
// /*AFLA*/ 	int (*flock) (struct file *, int, struct file_lock *);
// /*AFLA*/ 	ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
// /*AFLA*/ 	ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
// /*AFLA*/ 	int (*setlease)(struct file *, long, struct file_lock **, void **);
// /*AFLA*/ 	long (*fallocate)(struct file *file, int mode, loff_t offset,
// /*AFLA*/ 			  loff_t len);
// /*AFLA*/ 	void (*show_fdinfo)(struct seq_file *m, struct file *f);
// /*AFLA*/ #ifndef CONFIG_MMU
// /*AFLA*/ 	unsigned (*mmap_capabilities)(struct file *);
// /*AFLA*/ #endif
// /*AFLA*/ 	ssize_t (*copy_file_range)(struct file *, loff_t, struct file *,
// /*AFLA*/ 			loff_t, size_t, unsigned int);
// /*AFLA*/ 	int (*clone_file_range)(struct file *, loff_t, struct file *, loff_t,
// /*AFLA*/ 			u64);
// /*AFLA*/ 	ssize_t (*dedupe_file_range)(struct file *, u64, u64, struct file *,
// /*AFLA*/ 			u64);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct inode_operations {
// /*AFLA*/ 	struct dentry * (*lookup) (struct inode *,struct dentry *, unsigned int);
// /*AFLA*/ 	const char * (*get_link) (struct dentry *, struct inode *, struct delayed_call *);
// /*AFLA*/ 	int (*permission) (struct inode *, int);
// /*AFLA*/ 	struct posix_acl * (*get_acl)(struct inode *, int);
// /*AFLA*/ 
// /*AFLA*/ 	int (*readlink) (struct dentry *, char __user *,int);
// /*AFLA*/ 
// /*AFLA*/ 	int (*create) (struct inode *,struct dentry *, umode_t, bool);
// /*AFLA*/ 	int (*link) (struct dentry *,struct inode *,struct dentry *);
// /*AFLA*/ 	int (*unlink) (struct inode *,struct dentry *);
// /*AFLA*/ 	int (*symlink) (struct inode *,struct dentry *,const char *);
// /*AFLA*/ 	int (*mkdir) (struct inode *,struct dentry *,umode_t);
// /*AFLA*/ 	int (*rmdir) (struct inode *,struct dentry *);
// /*AFLA*/ 	int (*mknod) (struct inode *,struct dentry *,umode_t,dev_t);
// /*AFLA*/ 	int (*rename) (struct inode *, struct dentry *,
// /*AFLA*/ 			struct inode *, struct dentry *, unsigned int);
// /*AFLA*/ 	int (*setattr) (struct dentry *, struct iattr *);
// /*AFLA*/ 	int (*getattr) (struct vfsmount *mnt, struct dentry *, struct kstat *);
// /*AFLA*/ 	ssize_t (*listxattr) (struct dentry *, char *, size_t);
// /*AFLA*/ 	int (*fiemap)(struct inode *, struct fiemap_extent_info *, u64 start,
// /*AFLA*/ 		      u64 len);
// /*AFLA*/ 	int (*update_time)(struct inode *, struct timespec *, int);
// /*AFLA*/ 	int (*atomic_open)(struct inode *, struct dentry *,
// /*AFLA*/ 			   struct file *, unsigned open_flag,
// /*AFLA*/ 			   umode_t create_mode, int *opened);
// /*AFLA*/ 	int (*tmpfile) (struct inode *, struct dentry *, umode_t);
// /*AFLA*/ 	int (*set_acl)(struct inode *, struct posix_acl *, int);
// /*AFLA*/ } ____cacheline_aligned;
// /*AFLA*/ 
// /*AFLA*/ ssize_t rw_copy_check_uvector(int type, const struct iovec __user * uvector,
// /*AFLA*/ 			      unsigned long nr_segs, unsigned long fast_segs,
// /*AFLA*/ 			      struct iovec *fast_pointer,
// /*AFLA*/ 			      struct iovec **ret_pointer);
// /*AFLA*/ 
// /*AFLA*/ extern ssize_t __vfs_read(struct file *, char __user *, size_t, loff_t *);
// /*AFLA*/ extern ssize_t __vfs_write(struct file *, const char __user *, size_t, loff_t *);
// /*AFLA*/ extern ssize_t vfs_read(struct file *, char __user *, size_t, loff_t *);
// /*AFLA*/ extern ssize_t vfs_write(struct file *, const char __user *, size_t, loff_t *);
// /*AFLA*/ extern ssize_t vfs_readv(struct file *, const struct iovec __user *,
// /*AFLA*/ 		unsigned long, loff_t *, int);
// /*AFLA*/ extern ssize_t vfs_writev(struct file *, const struct iovec __user *,
// /*AFLA*/ 		unsigned long, loff_t *, int);
// /*AFLA*/ extern ssize_t vfs_copy_file_range(struct file *, loff_t , struct file *,
// /*AFLA*/ 				   loff_t, size_t, unsigned int);
// /*AFLA*/ extern int vfs_clone_file_range(struct file *file_in, loff_t pos_in,
// /*AFLA*/ 		struct file *file_out, loff_t pos_out, u64 len);
// /*AFLA*/ extern int vfs_dedupe_file_range(struct file *file,
// /*AFLA*/ 				 struct file_dedupe_range *same);
// /*AFLA*/ 
// /*AFLA*/ struct super_operations {
// /*AFLA*/    	struct inode *(*alloc_inode)(struct super_block *sb);
// /*AFLA*/ 	void (*destroy_inode)(struct inode *);
// /*AFLA*/ 
// /*AFLA*/    	void (*dirty_inode) (struct inode *, int flags);
// /*AFLA*/ 	int (*write_inode) (struct inode *, struct writeback_control *wbc);
// /*AFLA*/ 	int (*drop_inode) (struct inode *);
// /*AFLA*/ 	void (*evict_inode) (struct inode *);
// /*AFLA*/ 	void (*put_super) (struct super_block *);
// /*AFLA*/ 	int (*sync_fs)(struct super_block *sb, int wait);
// /*AFLA*/ 	int (*freeze_super) (struct super_block *);
// /*AFLA*/ 	int (*freeze_fs) (struct super_block *);
// /*AFLA*/ 	int (*thaw_super) (struct super_block *);
// /*AFLA*/ 	int (*unfreeze_fs) (struct super_block *);
// /*AFLA*/ 	int (*statfs) (struct dentry *, struct kstatfs *);
// /*AFLA*/ 	int (*remount_fs) (struct super_block *, int *, char *);
// /*AFLA*/ 	void (*umount_begin) (struct super_block *);
// /*AFLA*/ 
// /*AFLA*/ 	int (*show_options)(struct seq_file *, struct dentry *);
// /*AFLA*/ 	int (*show_devname)(struct seq_file *, struct dentry *);
// /*AFLA*/ 	int (*show_path)(struct seq_file *, struct dentry *);
// /*AFLA*/ 	int (*show_stats)(struct seq_file *, struct dentry *);
// /*AFLA*/ #ifdef CONFIG_QUOTA
// /*AFLA*/ 	ssize_t (*quota_read)(struct super_block *, int, char *, size_t, loff_t);
// /*AFLA*/ 	ssize_t (*quota_write)(struct super_block *, int, const char *, size_t, loff_t);
// /*AFLA*/ 	struct dquot **(*get_dquots)(struct inode *);
// /*AFLA*/ #endif
// /*AFLA*/ 	int (*bdev_try_to_free_page)(struct super_block*, struct page*, gfp_t);
// /*AFLA*/ 	long (*nr_cached_objects)(struct super_block *,
// /*AFLA*/ 				  struct shrink_control *);
// /*AFLA*/ 	long (*free_cached_objects)(struct super_block *,
// /*AFLA*/ 				    struct shrink_control *);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Inode flags - they have no relation to superblock flags now
// /*AFLA*/  */
// /*AFLA*/ #define S_SYNC		1	/* Writes are synced at once */
// /*AFLA*/ #define S_NOATIME	2	/* Do not update access times */
// /*AFLA*/ #define S_APPEND	4	/* Append-only file */
// /*AFLA*/ #define S_IMMUTABLE	8	/* Immutable file */
// /*AFLA*/ #define S_DEAD		16	/* removed, but still open directory */
// /*AFLA*/ #define S_NOQUOTA	32	/* Inode is not counted to quota */
// /*AFLA*/ #define S_DIRSYNC	64	/* Directory modifications are synchronous */
// /*AFLA*/ #define S_NOCMTIME	128	/* Do not update file c/mtime */
// /*AFLA*/ #define S_SWAPFILE	256	/* Do not truncate: swapon got its bmaps */
// /*AFLA*/ #define S_PRIVATE	512	/* Inode is fs-internal */
// /*AFLA*/ #define S_IMA		1024	/* Inode has an associated IMA struct */
// /*AFLA*/ #define S_AUTOMOUNT	2048	/* Automount/referral quasi-directory */
// /*AFLA*/ #define S_NOSEC		4096	/* no suid or xattr security attributes */
// /*AFLA*/ #ifdef CONFIG_FS_DAX
// /*AFLA*/ #define S_DAX		8192	/* Direct Access, avoiding the page cache */
// /*AFLA*/ #else
// /*AFLA*/ #define S_DAX		0	/* Make all the DAX code disappear */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Note that nosuid etc flags are inode-specific: setting some file-system
// /*AFLA*/  * flags just means all the inodes inherit those flags by default. It might be
// /*AFLA*/  * possible to override it selectively if you really wanted to with some
// /*AFLA*/  * ioctl() that is not currently implemented.
// /*AFLA*/  *
// /*AFLA*/  * Exception: MS_RDONLY is always applied to the entire file system.
// /*AFLA*/  *
// /*AFLA*/  * Unfortunately, it is possible to change a filesystems flags with it mounted
// /*AFLA*/  * with files in use.  This means that all of the inodes will not have their
// /*AFLA*/  * i_flags updated.  Hence, i_flags no longer inherit the superblock mount
// /*AFLA*/  * flags, so these have to be checked separately. -- rmk@arm.uk.linux.org
// /*AFLA*/  */
// /*AFLA*/ #define __IS_FLG(inode, flg)	((inode)->i_sb->s_flags & (flg))
// /*AFLA*/ 
// /*AFLA*/ #define IS_RDONLY(inode)	((inode)->i_sb->s_flags & MS_RDONLY)
// /*AFLA*/ #define IS_SYNC(inode)		(__IS_FLG(inode, MS_SYNCHRONOUS) || \
// /*AFLA*/ 					((inode)->i_flags & S_SYNC))
// /*AFLA*/ #define IS_DIRSYNC(inode)	(__IS_FLG(inode, MS_SYNCHRONOUS|MS_DIRSYNC) || \
// /*AFLA*/ 					((inode)->i_flags & (S_SYNC|S_DIRSYNC)))
// /*AFLA*/ #define IS_MANDLOCK(inode)	__IS_FLG(inode, MS_MANDLOCK)
// /*AFLA*/ #define IS_NOATIME(inode)	__IS_FLG(inode, MS_RDONLY|MS_NOATIME)
// /*AFLA*/ #define IS_I_VERSION(inode)	__IS_FLG(inode, MS_I_VERSION)
// /*AFLA*/ 
// /*AFLA*/ #define IS_NOQUOTA(inode)	((inode)->i_flags & S_NOQUOTA)
// /*AFLA*/ #define IS_APPEND(inode)	((inode)->i_flags & S_APPEND)
// /*AFLA*/ #define IS_IMMUTABLE(inode)	((inode)->i_flags & S_IMMUTABLE)
// /*AFLA*/ #define IS_POSIXACL(inode)	__IS_FLG(inode, MS_POSIXACL)
// /*AFLA*/ 
// /*AFLA*/ #define IS_DEADDIR(inode)	((inode)->i_flags & S_DEAD)
// /*AFLA*/ #define IS_NOCMTIME(inode)	((inode)->i_flags & S_NOCMTIME)
// /*AFLA*/ #define IS_SWAPFILE(inode)	((inode)->i_flags & S_SWAPFILE)
// /*AFLA*/ #define IS_PRIVATE(inode)	((inode)->i_flags & S_PRIVATE)
// /*AFLA*/ #define IS_IMA(inode)		((inode)->i_flags & S_IMA)
// /*AFLA*/ #define IS_AUTOMOUNT(inode)	((inode)->i_flags & S_AUTOMOUNT)
// /*AFLA*/ #define IS_NOSEC(inode)		((inode)->i_flags & S_NOSEC)
// /*AFLA*/ #define IS_DAX(inode)		((inode)->i_flags & S_DAX)
// /*AFLA*/ 
// /*AFLA*/ #define IS_WHITEOUT(inode)	(S_ISCHR(inode->i_mode) && \
// /*AFLA*/ 				 (inode)->i_rdev == WHITEOUT_DEV)
// /*AFLA*/ 
// /*AFLA*/ static inline bool HAS_UNMAPPED_ID(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return !uid_valid(inode->i_uid) || !gid_valid(inode->i_gid);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Inode state bits.  Protected by inode->i_lock
// /*AFLA*/  *
// /*AFLA*/  * Three bits determine the dirty state of the inode, I_DIRTY_SYNC,
// /*AFLA*/  * I_DIRTY_DATASYNC and I_DIRTY_PAGES.
// /*AFLA*/  *
// /*AFLA*/  * Four bits define the lifetime of an inode.  Initially, inodes are I_NEW,
// /*AFLA*/  * until that flag is cleared.  I_WILL_FREE, I_FREEING and I_CLEAR are set at
// /*AFLA*/  * various stages of removing an inode.
// /*AFLA*/  *
// /*AFLA*/  * Two bits are used for locking and completion notification, I_NEW and I_SYNC.
// /*AFLA*/  *
// /*AFLA*/  * I_DIRTY_SYNC		Inode is dirty, but doesn't have to be written on
// /*AFLA*/  *			fdatasync().  i_atime is the usual cause.
// /*AFLA*/  * I_DIRTY_DATASYNC	Data-related inode changes pending. We keep track of
// /*AFLA*/  *			these changes separately from I_DIRTY_SYNC so that we
// /*AFLA*/  *			don't have to write inode on fdatasync() when only
// /*AFLA*/  *			mtime has changed in it.
// /*AFLA*/  * I_DIRTY_PAGES	Inode has dirty pages.  Inode itself may be clean.
// /*AFLA*/  * I_NEW		Serves as both a mutex and completion notification.
// /*AFLA*/  *			New inodes set I_NEW.  If two processes both create
// /*AFLA*/  *			the same inode, one of them will release its inode and
// /*AFLA*/  *			wait for I_NEW to be released before returning.
// /*AFLA*/  *			Inodes in I_WILL_FREE, I_FREEING or I_CLEAR state can
// /*AFLA*/  *			also cause waiting on I_NEW, without I_NEW actually
// /*AFLA*/  *			being set.  find_inode() uses this to prevent returning
// /*AFLA*/  *			nearly-dead inodes.
// /*AFLA*/  * I_WILL_FREE		Must be set when calling write_inode_now() if i_count
// /*AFLA*/  *			is zero.  I_FREEING must be set when I_WILL_FREE is
// /*AFLA*/  *			cleared.
// /*AFLA*/  * I_FREEING		Set when inode is about to be freed but still has dirty
// /*AFLA*/  *			pages or buffers attached or the inode itself is still
// /*AFLA*/  *			dirty.
// /*AFLA*/  * I_CLEAR		Added by clear_inode().  In this state the inode is
// /*AFLA*/  *			clean and can be destroyed.  Inode keeps I_FREEING.
// /*AFLA*/  *
// /*AFLA*/  *			Inodes that are I_WILL_FREE, I_FREEING or I_CLEAR are
// /*AFLA*/  *			prohibited for many purposes.  iget() must wait for
// /*AFLA*/  *			the inode to be completely released, then create it
// /*AFLA*/  *			anew.  Other functions will just ignore such inodes,
// /*AFLA*/  *			if appropriate.  I_NEW is used for waiting.
// /*AFLA*/  *
// /*AFLA*/  * I_SYNC		Writeback of inode is running. The bit is set during
// /*AFLA*/  *			data writeback, and cleared with a wakeup on the bit
// /*AFLA*/  *			address once it is done. The bit is also used to pin
// /*AFLA*/  *			the inode in memory for flusher thread.
// /*AFLA*/  *
// /*AFLA*/  * I_REFERENCED		Marks the inode as recently references on the LRU list.
// /*AFLA*/  *
// /*AFLA*/  * I_DIO_WAKEUP		Never set.  Only used as a key for wait_on_bit().
// /*AFLA*/  *
// /*AFLA*/  * I_WB_SWITCH		Cgroup bdi_writeback switching in progress.  Used to
// /*AFLA*/  *			synchronize competing switching instances and to tell
// /*AFLA*/  *			wb stat updates to grab mapping->tree_lock.  See
// /*AFLA*/  *			inode_switch_wb_work_fn() for details.
// /*AFLA*/  *
// /*AFLA*/  * Q: What is the difference between I_WILL_FREE and I_FREEING?
// /*AFLA*/  */
// /*AFLA*/ #define I_DIRTY_SYNC		(1 << 0)
// /*AFLA*/ #define I_DIRTY_DATASYNC	(1 << 1)
// /*AFLA*/ #define I_DIRTY_PAGES		(1 << 2)
// /*AFLA*/ #define __I_NEW			3
// /*AFLA*/ #define I_NEW			(1 << __I_NEW)
// /*AFLA*/ #define I_WILL_FREE		(1 << 4)
// /*AFLA*/ #define I_FREEING		(1 << 5)
// /*AFLA*/ #define I_CLEAR			(1 << 6)
// /*AFLA*/ #define __I_SYNC		7
// /*AFLA*/ #define I_SYNC			(1 << __I_SYNC)
// /*AFLA*/ #define I_REFERENCED		(1 << 8)
// /*AFLA*/ #define __I_DIO_WAKEUP		9
// /*AFLA*/ #define I_DIO_WAKEUP		(1 << __I_DIO_WAKEUP)
// /*AFLA*/ #define I_LINKABLE		(1 << 10)
// /*AFLA*/ #define I_DIRTY_TIME		(1 << 11)
// /*AFLA*/ #define __I_DIRTY_TIME_EXPIRED	12
// /*AFLA*/ #define I_DIRTY_TIME_EXPIRED	(1 << __I_DIRTY_TIME_EXPIRED)
// /*AFLA*/ #define I_WB_SWITCH		(1 << 13)
// /*AFLA*/ 
// /*AFLA*/ #define I_DIRTY (I_DIRTY_SYNC | I_DIRTY_DATASYNC | I_DIRTY_PAGES)
// /*AFLA*/ #define I_DIRTY_ALL (I_DIRTY | I_DIRTY_TIME)
// /*AFLA*/ 
// /*AFLA*/ extern void __mark_inode_dirty(struct inode *, int);
// /*AFLA*/ static inline void mark_inode_dirty(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	__mark_inode_dirty(inode, I_DIRTY);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void mark_inode_dirty_sync(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	__mark_inode_dirty(inode, I_DIRTY_SYNC);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void inc_nlink(struct inode *inode);
// /*AFLA*/ extern void drop_nlink(struct inode *inode);
// /*AFLA*/ extern void clear_nlink(struct inode *inode);
// /*AFLA*/ extern void set_nlink(struct inode *inode, unsigned int nlink);
// /*AFLA*/ 
// /*AFLA*/ static inline void inode_inc_link_count(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	inc_nlink(inode);
// /*AFLA*/ 	mark_inode_dirty(inode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void inode_dec_link_count(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	drop_nlink(inode);
// /*AFLA*/ 	mark_inode_dirty(inode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * inode_inc_iversion - increments i_version
// /*AFLA*/  * @inode: inode that need to be updated
// /*AFLA*/  *
// /*AFLA*/  * Every time the inode is modified, the i_version field will be incremented.
// /*AFLA*/  * The filesystem has to be mounted with i_version flag
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static inline void inode_inc_iversion(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/        spin_lock(&inode->i_lock);
// /*AFLA*/        inode->i_version++;
// /*AFLA*/        spin_unlock(&inode->i_lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ enum file_time_flags {
// /*AFLA*/ 	S_ATIME = 1,
// /*AFLA*/ 	S_MTIME = 2,
// /*AFLA*/ 	S_CTIME = 4,
// /*AFLA*/ 	S_VERSION = 8,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern void touch_atime(const struct path *);
// /*AFLA*/ static inline void file_accessed(struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	if (!(file->f_flags & O_NOATIME))
// /*AFLA*/ 		touch_atime(&file->f_path);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ int sync_inode(struct inode *inode, struct writeback_control *wbc);
// /*AFLA*/ int sync_inode_metadata(struct inode *inode, int wait);
// /*AFLA*/ 
// /*AFLA*/ struct file_system_type {
// /*AFLA*/ 	const char *name;
// /*AFLA*/ 	int fs_flags;
// /*AFLA*/ #define FS_REQUIRES_DEV		1 
// /*AFLA*/ #define FS_BINARY_MOUNTDATA	2
// /*AFLA*/ #define FS_HAS_SUBTYPE		4
// /*AFLA*/ #define FS_USERNS_MOUNT		8	/* Can be mounted by userns root */
// /*AFLA*/ #define FS_RENAME_DOES_D_MOVE	32768	/* FS will handle d_move() during rename() internally. */
// /*AFLA*/ 	struct dentry *(*mount) (struct file_system_type *, int,
// /*AFLA*/ 		       const char *, void *);
// /*AFLA*/ 	void (*kill_sb) (struct super_block *);
// /*AFLA*/ 	struct module *owner;
// /*AFLA*/ 	struct file_system_type * next;
// /*AFLA*/ 	struct hlist_head fs_supers;
// /*AFLA*/ 
// /*AFLA*/ 	struct lock_class_key s_lock_key;
// /*AFLA*/ 	struct lock_class_key s_umount_key;
// /*AFLA*/ 	struct lock_class_key s_vfs_rename_key;
// /*AFLA*/ 	struct lock_class_key s_writers_key[SB_FREEZE_LEVELS];
// /*AFLA*/ 
// /*AFLA*/ 	struct lock_class_key i_lock_key;
// /*AFLA*/ 	struct lock_class_key i_mutex_key;
// /*AFLA*/ 	struct lock_class_key i_mutex_dir_key;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define MODULE_ALIAS_FS(NAME) MODULE_ALIAS("fs-" NAME)
// /*AFLA*/ 
// /*AFLA*/ extern struct dentry *mount_ns(struct file_system_type *fs_type,
// /*AFLA*/ 	int flags, void *data, void *ns, struct user_namespace *user_ns,
// /*AFLA*/ 	int (*fill_super)(struct super_block *, void *, int));
// /*AFLA*/ extern struct dentry *mount_bdev(struct file_system_type *fs_type,
// /*AFLA*/ 	int flags, const char *dev_name, void *data,
// /*AFLA*/ 	int (*fill_super)(struct super_block *, void *, int));
// /*AFLA*/ extern struct dentry *mount_single(struct file_system_type *fs_type,
// /*AFLA*/ 	int flags, void *data,
// /*AFLA*/ 	int (*fill_super)(struct super_block *, void *, int));
// /*AFLA*/ extern struct dentry *mount_nodev(struct file_system_type *fs_type,
// /*AFLA*/ 	int flags, void *data,
// /*AFLA*/ 	int (*fill_super)(struct super_block *, void *, int));
// /*AFLA*/ extern struct dentry *mount_subtree(struct vfsmount *mnt, const char *path);
// /*AFLA*/ void generic_shutdown_super(struct super_block *sb);
// /*AFLA*/ void kill_block_super(struct super_block *sb);
// /*AFLA*/ void kill_anon_super(struct super_block *sb);
// /*AFLA*/ void kill_litter_super(struct super_block *sb);
// /*AFLA*/ void deactivate_super(struct super_block *sb);
// /*AFLA*/ void deactivate_locked_super(struct super_block *sb);
// /*AFLA*/ int set_anon_super(struct super_block *s, void *data);
// /*AFLA*/ int get_anon_bdev(dev_t *);
// /*AFLA*/ void free_anon_bdev(dev_t);
// /*AFLA*/ struct super_block *sget_userns(struct file_system_type *type,
// /*AFLA*/ 			int (*test)(struct super_block *,void *),
// /*AFLA*/ 			int (*set)(struct super_block *,void *),
// /*AFLA*/ 			int flags, struct user_namespace *user_ns,
// /*AFLA*/ 			void *data);
// /*AFLA*/ struct super_block *sget(struct file_system_type *type,
// /*AFLA*/ 			int (*test)(struct super_block *,void *),
// /*AFLA*/ 			int (*set)(struct super_block *,void *),
// /*AFLA*/ 			int flags, void *data);
// /*AFLA*/ extern struct dentry *mount_pseudo_xattr(struct file_system_type *, char *,
// /*AFLA*/ 					 const struct super_operations *ops,
// /*AFLA*/ 					 const struct xattr_handler **xattr,
// /*AFLA*/ 					 const struct dentry_operations *dops,
// /*AFLA*/ 					 unsigned long);
// /*AFLA*/ 
// /*AFLA*/ static inline struct dentry *
// /*AFLA*/ mount_pseudo(struct file_system_type *fs_type, char *name,
// /*AFLA*/ 	     const struct super_operations *ops,
// /*AFLA*/ 	     const struct dentry_operations *dops, unsigned long magic)
// /*AFLA*/ {
// /*AFLA*/ 	return mount_pseudo_xattr(fs_type, name, ops, NULL, dops, magic);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Alas, no aliases. Too much hassle with bringing module.h everywhere */
// /*AFLA*/ #define fops_get(fops) \
// /*AFLA*/ 	(((fops) && try_module_get((fops)->owner) ? (fops) : NULL))
// /*AFLA*/ #define fops_put(fops) \
// /*AFLA*/ 	do { if (fops) module_put((fops)->owner); } while(0)
// /*AFLA*/ /*
// /*AFLA*/  * This one is to be used *ONLY* from ->open() instances.
// /*AFLA*/  * fops must be non-NULL, pinned down *and* module dependencies
// /*AFLA*/  * should be sufficient to pin the caller down as well.
// /*AFLA*/  */
// /*AFLA*/ #define replace_fops(f, fops) \
// /*AFLA*/ 	do {	\
// /*AFLA*/ 		struct file *__file = (f); \
// /*AFLA*/ 		fops_put(__file->f_op); \
// /*AFLA*/ 		BUG_ON(!(__file->f_op = (fops))); \
// /*AFLA*/ 	} while(0)
// /*AFLA*/ 
// /*AFLA*/ extern int register_filesystem(struct file_system_type *);
// /*AFLA*/ extern int unregister_filesystem(struct file_system_type *);
// /*AFLA*/ extern struct vfsmount *kern_mount_data(struct file_system_type *, void *data);
// /*AFLA*/ #define kern_mount(type) kern_mount_data(type, NULL)
// /*AFLA*/ extern void kern_unmount(struct vfsmount *mnt);
// /*AFLA*/ extern int may_umount_tree(struct vfsmount *);
// /*AFLA*/ extern int may_umount(struct vfsmount *);
// /*AFLA*/ extern long do_mount(const char *, const char __user *,
// /*AFLA*/ 		     const char *, unsigned long, void *);
// /*AFLA*/ extern struct vfsmount *collect_mounts(struct path *);
// /*AFLA*/ extern void drop_collected_mounts(struct vfsmount *);
// /*AFLA*/ extern int iterate_mounts(int (*)(struct vfsmount *, void *), void *,
// /*AFLA*/ 			  struct vfsmount *);
// /*AFLA*/ extern int vfs_statfs(struct path *, struct kstatfs *);
// /*AFLA*/ extern int user_statfs(const char __user *, struct kstatfs *);
// /*AFLA*/ extern int fd_statfs(int, struct kstatfs *);
// /*AFLA*/ extern int vfs_ustat(dev_t, struct kstatfs *);
// /*AFLA*/ extern int freeze_super(struct super_block *super);
// /*AFLA*/ extern int thaw_super(struct super_block *super);
// /*AFLA*/ extern bool our_mnt(struct vfsmount *mnt);
// /*AFLA*/ 
// /*AFLA*/ extern int current_umask(void);
// /*AFLA*/ 
// /*AFLA*/ extern void ihold(struct inode * inode);
// /*AFLA*/ extern void iput(struct inode *);
// /*AFLA*/ extern int generic_update_time(struct inode *, struct timespec *, int);
// /*AFLA*/ 
// /*AFLA*/ /* /sys/fs */
// /*AFLA*/ extern struct kobject *fs_kobj;
// /*AFLA*/ 
// /*AFLA*/ #define MAX_RW_COUNT (INT_MAX & PAGE_MASK)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MANDATORY_FILE_LOCKING
// /*AFLA*/ extern int locks_mandatory_locked(struct file *);
// /*AFLA*/ extern int locks_mandatory_area(struct inode *, struct file *, loff_t, loff_t, unsigned char);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Candidates for mandatory locking have the setgid bit set
// /*AFLA*/  * but no group execute bit -  an otherwise meaningless combination.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static inline int __mandatory_lock(struct inode *ino)
// /*AFLA*/ {
// /*AFLA*/ 	return (ino->i_mode & (S_ISGID | S_IXGRP)) == S_ISGID;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * ... and these candidates should be on MS_MANDLOCK mounted fs,
// /*AFLA*/  * otherwise these will be advisory locks
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static inline int mandatory_lock(struct inode *ino)
// /*AFLA*/ {
// /*AFLA*/ 	return IS_MANDLOCK(ino) && __mandatory_lock(ino);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int locks_verify_locked(struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	if (mandatory_lock(locks_inode(file)))
// /*AFLA*/ 		return locks_mandatory_locked(file);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int locks_verify_truncate(struct inode *inode,
// /*AFLA*/ 				    struct file *f,
// /*AFLA*/ 				    loff_t size)
// /*AFLA*/ {
// /*AFLA*/ 	if (!inode->i_flctx || !mandatory_lock(inode))
// /*AFLA*/ 		return 0;
// /*AFLA*/ 
// /*AFLA*/ 	if (size < inode->i_size) {
// /*AFLA*/ 		return locks_mandatory_area(inode, f, size, inode->i_size - 1,
// /*AFLA*/ 				F_WRLCK);
// /*AFLA*/ 	} else {
// /*AFLA*/ 		return locks_mandatory_area(inode, f, inode->i_size, size - 1,
// /*AFLA*/ 				F_WRLCK);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_MANDATORY_FILE_LOCKING */
// /*AFLA*/ 
// /*AFLA*/ static inline int locks_mandatory_locked(struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int locks_mandatory_area(struct inode *inode, struct file *filp,
// /*AFLA*/                                        loff_t start, loff_t end, unsigned char type)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __mandatory_lock(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int mandatory_lock(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int locks_verify_locked(struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int locks_verify_truncate(struct inode *inode, struct file *filp,
// /*AFLA*/ 					size_t size)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_MANDATORY_FILE_LOCKING */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FILE_LOCKING
// /*AFLA*/ static inline int break_lease(struct inode *inode, unsigned int mode)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Since this check is lockless, we must ensure that any refcounts
// /*AFLA*/ 	 * taken are done before checking i_flctx->flc_lease. Otherwise, we
// /*AFLA*/ 	 * could end up racing with tasks trying to set a new lease on this
// /*AFLA*/ 	 * file.
// /*AFLA*/ 	 */
// /*AFLA*/ 	smp_mb();
// /*AFLA*/ 	if (inode->i_flctx && !list_empty_careful(&inode->i_flctx->flc_lease))
// /*AFLA*/ 		return __break_lease(inode, mode, FL_LEASE);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int break_deleg(struct inode *inode, unsigned int mode)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Since this check is lockless, we must ensure that any refcounts
// /*AFLA*/ 	 * taken are done before checking i_flctx->flc_lease. Otherwise, we
// /*AFLA*/ 	 * could end up racing with tasks trying to set a new lease on this
// /*AFLA*/ 	 * file.
// /*AFLA*/ 	 */
// /*AFLA*/ 	smp_mb();
// /*AFLA*/ 	if (inode->i_flctx && !list_empty_careful(&inode->i_flctx->flc_lease))
// /*AFLA*/ 		return __break_lease(inode, mode, FL_DELEG);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int try_break_deleg(struct inode *inode, struct inode **delegated_inode)
// /*AFLA*/ {
// /*AFLA*/ 	int ret;
// /*AFLA*/ 
// /*AFLA*/ 	ret = break_deleg(inode, O_WRONLY|O_NONBLOCK);
// /*AFLA*/ 	if (ret == -EWOULDBLOCK && delegated_inode) {
// /*AFLA*/ 		*delegated_inode = inode;
// /*AFLA*/ 		ihold(inode);
// /*AFLA*/ 	}
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int break_deleg_wait(struct inode **delegated_inode)
// /*AFLA*/ {
// /*AFLA*/ 	int ret;
// /*AFLA*/ 
// /*AFLA*/ 	ret = break_deleg(*delegated_inode, O_WRONLY);
// /*AFLA*/ 	iput(*delegated_inode);
// /*AFLA*/ 	*delegated_inode = NULL;
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int break_layout(struct inode *inode, bool wait)
// /*AFLA*/ {
// /*AFLA*/ 	smp_mb();
// /*AFLA*/ 	if (inode->i_flctx && !list_empty_careful(&inode->i_flctx->flc_lease))
// /*AFLA*/ 		return __break_lease(inode,
// /*AFLA*/ 				wait ? O_WRONLY : O_WRONLY | O_NONBLOCK,
// /*AFLA*/ 				FL_LAYOUT);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_FILE_LOCKING */
// /*AFLA*/ static inline int break_lease(struct inode *inode, unsigned int mode)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int break_deleg(struct inode *inode, unsigned int mode)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int try_break_deleg(struct inode *inode, struct inode **delegated_inode)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int break_deleg_wait(struct inode **delegated_inode)
// /*AFLA*/ {
// /*AFLA*/ 	BUG();
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int break_layout(struct inode *inode, bool wait)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_FILE_LOCKING */
// /*AFLA*/ 
// /*AFLA*/ /* fs/open.c */
// /*AFLA*/ struct audit_names;
// /*AFLA*/ struct filename {
// /*AFLA*/ 	const char		*name;	/* pointer to actual string */
// /*AFLA*/ 	const __user char	*uptr;	/* original userland pointer */
// /*AFLA*/ 	struct audit_names	*aname;
// /*AFLA*/ 	int			refcnt;
// /*AFLA*/ 	const char		iname[];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern long vfs_truncate(const struct path *, loff_t);
// /*AFLA*/ extern int do_truncate(struct dentry *, loff_t start, unsigned int time_attrs,
// /*AFLA*/ 		       struct file *filp);
// /*AFLA*/ extern int vfs_fallocate(struct file *file, int mode, loff_t offset,
// /*AFLA*/ 			loff_t len);
// /*AFLA*/ extern long do_sys_open(int dfd, const char __user *filename, int flags,
// /*AFLA*/ 			umode_t mode);
// /*AFLA*/ extern struct file *file_open_name(struct filename *, int, umode_t);
// /*AFLA*/ extern struct file *filp_open(const char *, int, umode_t);
// /*AFLA*/ extern struct file *file_open_root(struct dentry *, struct vfsmount *,
// /*AFLA*/ 				   const char *, int, umode_t);
// /*AFLA*/ extern struct file * dentry_open(const struct path *, int, const struct cred *);
// /*AFLA*/ extern int filp_close(struct file *, fl_owner_t id);
// /*AFLA*/ 
// /*AFLA*/ extern struct filename *getname_flags(const char __user *, int, int *);
// /*AFLA*/ extern struct filename *getname(const char __user *);
// /*AFLA*/ extern struct filename *getname_kernel(const char *);
// /*AFLA*/ extern void putname(struct filename *name);
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	FILE_CREATED = 1,
// /*AFLA*/ 	FILE_OPENED = 2
// /*AFLA*/ };
// /*AFLA*/ extern int finish_open(struct file *file, struct dentry *dentry,
// /*AFLA*/ 			int (*open)(struct inode *, struct file *),
// /*AFLA*/ 			int *opened);
// /*AFLA*/ extern int finish_no_open(struct file *file, struct dentry *dentry);
// /*AFLA*/ 
// /*AFLA*/ /* fs/ioctl.c */
// /*AFLA*/ 
// /*AFLA*/ extern int ioctl_preallocate(struct file *filp, void __user *argp);
// /*AFLA*/ 
// /*AFLA*/ /* fs/dcache.c */
// /*AFLA*/ extern void __init vfs_caches_init_early(void);
// /*AFLA*/ extern void __init vfs_caches_init(void);
// /*AFLA*/ 
// /*AFLA*/ extern struct kmem_cache *names_cachep;
// /*AFLA*/ 
// /*AFLA*/ #define __getname()		kmem_cache_alloc(names_cachep, GFP_KERNEL)
// /*AFLA*/ #define __putname(name)		kmem_cache_free(names_cachep, (void *)(name))
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_BLOCK
// /*AFLA*/ extern int register_blkdev(unsigned int, const char *);
// /*AFLA*/ extern void unregister_blkdev(unsigned int, const char *);
// /*AFLA*/ extern struct block_device *bdget(dev_t);
// /*AFLA*/ extern struct block_device *bdgrab(struct block_device *bdev);
// /*AFLA*/ extern void bd_set_size(struct block_device *, loff_t size);
// /*AFLA*/ extern void bd_forget(struct inode *inode);
// /*AFLA*/ extern void bdput(struct block_device *);
// /*AFLA*/ extern void invalidate_bdev(struct block_device *);
// /*AFLA*/ extern void iterate_bdevs(void (*)(struct block_device *, void *), void *);
// /*AFLA*/ extern int sync_blockdev(struct block_device *bdev);
// /*AFLA*/ extern void kill_bdev(struct block_device *);
// /*AFLA*/ extern struct super_block *freeze_bdev(struct block_device *);
// /*AFLA*/ extern void emergency_thaw_all(void);
// /*AFLA*/ extern int thaw_bdev(struct block_device *bdev, struct super_block *sb);
// /*AFLA*/ extern int fsync_bdev(struct block_device *);
// /*AFLA*/ 
// /*AFLA*/ extern struct super_block *blockdev_superblock;
// /*AFLA*/ 
// /*AFLA*/ static inline bool sb_is_blkdev_sb(struct super_block *sb)
// /*AFLA*/ {
// /*AFLA*/ 	return sb == blockdev_superblock;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline void bd_forget(struct inode *inode) {}
// /*AFLA*/ static inline int sync_blockdev(struct block_device *bdev) { return 0; }
// /*AFLA*/ static inline void kill_bdev(struct block_device *bdev) {}
// /*AFLA*/ static inline void invalidate_bdev(struct block_device *bdev) {}
// /*AFLA*/ 
// /*AFLA*/ static inline struct super_block *freeze_bdev(struct block_device *sb)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int thaw_bdev(struct block_device *bdev, struct super_block *sb)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void iterate_bdevs(void (*f)(struct block_device *, void *), void *arg)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool sb_is_blkdev_sb(struct super_block *sb)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ extern int sync_filesystem(struct super_block *);
// /*AFLA*/ extern const struct file_operations def_blk_fops;
// /*AFLA*/ extern const struct file_operations def_chr_fops;
// /*AFLA*/ #ifdef CONFIG_BLOCK
// /*AFLA*/ extern int ioctl_by_bdev(struct block_device *, unsigned, unsigned long);
// /*AFLA*/ extern int blkdev_ioctl(struct block_device *, fmode_t, unsigned, unsigned long);
// /*AFLA*/ extern long compat_blkdev_ioctl(struct file *, unsigned, unsigned long);
// /*AFLA*/ extern int blkdev_get(struct block_device *bdev, fmode_t mode, void *holder);
// /*AFLA*/ extern struct block_device *blkdev_get_by_path(const char *path, fmode_t mode,
// /*AFLA*/ 					       void *holder);
// /*AFLA*/ extern struct block_device *blkdev_get_by_dev(dev_t dev, fmode_t mode,
// /*AFLA*/ 					      void *holder);
// /*AFLA*/ extern void blkdev_put(struct block_device *bdev, fmode_t mode);
// /*AFLA*/ extern int __blkdev_reread_part(struct block_device *bdev);
// /*AFLA*/ extern int blkdev_reread_part(struct block_device *bdev);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SYSFS
// /*AFLA*/ extern int bd_link_disk_holder(struct block_device *bdev, struct gendisk *disk);
// /*AFLA*/ extern void bd_unlink_disk_holder(struct block_device *bdev,
// /*AFLA*/ 				  struct gendisk *disk);
// /*AFLA*/ #else
// /*AFLA*/ static inline int bd_link_disk_holder(struct block_device *bdev,
// /*AFLA*/ 				      struct gendisk *disk)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void bd_unlink_disk_holder(struct block_device *bdev,
// /*AFLA*/ 					 struct gendisk *disk)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* fs/char_dev.c */
// /*AFLA*/ #define CHRDEV_MAJOR_HASH_SIZE	255
// /*AFLA*/ /* Marks the bottom of the first segment of free char majors */
// /*AFLA*/ #define CHRDEV_MAJOR_DYN_END 234
// /*AFLA*/ extern int alloc_chrdev_region(dev_t *, unsigned, unsigned, const char *);
// /*AFLA*/ extern int register_chrdev_region(dev_t, unsigned, const char *);
// /*AFLA*/ extern int __register_chrdev(unsigned int major, unsigned int baseminor,
// /*AFLA*/ 			     unsigned int count, const char *name,
// /*AFLA*/ 			     const struct file_operations *fops);
// /*AFLA*/ extern void __unregister_chrdev(unsigned int major, unsigned int baseminor,
// /*AFLA*/ 				unsigned int count, const char *name);
// /*AFLA*/ extern void unregister_chrdev_region(dev_t, unsigned);
// /*AFLA*/ extern void chrdev_show(struct seq_file *,off_t);
// /*AFLA*/ 
// /*AFLA*/ static inline int register_chrdev(unsigned int major, const char *name,
// /*AFLA*/ 				  const struct file_operations *fops)
// /*AFLA*/ {
// /*AFLA*/ 	return __register_chrdev(major, 0, 256, name, fops);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void unregister_chrdev(unsigned int major, const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	__unregister_chrdev(major, 0, 256, name);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* fs/block_dev.c */
// /*AFLA*/ #define BDEVNAME_SIZE	32	/* Largest string for a blockdev identifier */
// /*AFLA*/ #define BDEVT_SIZE	10	/* Largest string for MAJ:MIN for blkdev */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_BLOCK
// /*AFLA*/ #define BLKDEV_MAJOR_HASH_SIZE	255
// /*AFLA*/ extern const char *__bdevname(dev_t, char *buffer);
// /*AFLA*/ extern const char *bdevname(struct block_device *bdev, char *buffer);
// /*AFLA*/ extern struct block_device *lookup_bdev(const char *);
// /*AFLA*/ extern void blkdev_show(struct seq_file *,off_t);
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ #define BLKDEV_MAJOR_HASH_SIZE	0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void init_special_inode(struct inode *, umode_t, dev_t);
// /*AFLA*/ 
// /*AFLA*/ /* Invalid inode operations -- fs/bad_inode.c */
// /*AFLA*/ extern void make_bad_inode(struct inode *);
// /*AFLA*/ extern bool is_bad_inode(struct inode *);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_BLOCK
// /*AFLA*/ static inline bool op_is_write(unsigned int op)
// /*AFLA*/ {
// /*AFLA*/ 	return op == REQ_OP_READ ? false : true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * return data direction, READ or WRITE
// /*AFLA*/  */
// /*AFLA*/ static inline int bio_data_dir(struct bio *bio)
// /*AFLA*/ {
// /*AFLA*/ 	return op_is_write(bio_op(bio)) ? WRITE : READ;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void check_disk_size_change(struct gendisk *disk,
// /*AFLA*/ 				   struct block_device *bdev);
// /*AFLA*/ extern int revalidate_disk(struct gendisk *);
// /*AFLA*/ extern int check_disk_change(struct block_device *);
// /*AFLA*/ extern int __invalidate_device(struct block_device *, bool);
// /*AFLA*/ extern int invalidate_partition(struct gendisk *, int);
// /*AFLA*/ #endif
// /*AFLA*/ unsigned long invalidate_mapping_pages(struct address_space *mapping,
// /*AFLA*/ 					pgoff_t start, pgoff_t end);
// /*AFLA*/ 
// /*AFLA*/ static inline void invalidate_remote_inode(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	if (S_ISREG(inode->i_mode) || S_ISDIR(inode->i_mode) ||
// /*AFLA*/ 	    S_ISLNK(inode->i_mode))
// /*AFLA*/ 		invalidate_mapping_pages(inode->i_mapping, 0, -1);
// /*AFLA*/ }
// /*AFLA*/ extern int invalidate_inode_pages2(struct address_space *mapping);
// /*AFLA*/ extern int invalidate_inode_pages2_range(struct address_space *mapping,
// /*AFLA*/ 					 pgoff_t start, pgoff_t end);
// /*AFLA*/ extern int write_inode_now(struct inode *, int);
// /*AFLA*/ extern int filemap_fdatawrite(struct address_space *);
// /*AFLA*/ extern int filemap_flush(struct address_space *);
// /*AFLA*/ extern int filemap_fdatawait(struct address_space *);
// /*AFLA*/ extern void filemap_fdatawait_keep_errors(struct address_space *);
// /*AFLA*/ extern int filemap_fdatawait_range(struct address_space *, loff_t lstart,
// /*AFLA*/ 				   loff_t lend);
// /*AFLA*/ extern int filemap_write_and_wait(struct address_space *mapping);
// /*AFLA*/ extern int filemap_write_and_wait_range(struct address_space *mapping,
// /*AFLA*/ 				        loff_t lstart, loff_t lend);
// /*AFLA*/ extern int __filemap_fdatawrite_range(struct address_space *mapping,
// /*AFLA*/ 				loff_t start, loff_t end, int sync_mode);
// /*AFLA*/ extern int filemap_fdatawrite_range(struct address_space *mapping,
// /*AFLA*/ 				loff_t start, loff_t end);
// /*AFLA*/ extern int filemap_check_errors(struct address_space *mapping);
// /*AFLA*/ 
// /*AFLA*/ extern int vfs_fsync_range(struct file *file, loff_t start, loff_t end,
// /*AFLA*/ 			   int datasync);
// /*AFLA*/ extern int vfs_fsync(struct file *file, int datasync);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Sync the bytes written if this was a synchronous write.  Expect ki_pos
// /*AFLA*/  * to already be updated for the write, and will return either the amount
// /*AFLA*/  * of bytes passed in, or an error if syncing the file failed.
// /*AFLA*/  */
// /*AFLA*/ static inline ssize_t generic_write_sync(struct kiocb *iocb, ssize_t count)
// /*AFLA*/ {
// /*AFLA*/ 	if (iocb->ki_flags & IOCB_DSYNC) {
// /*AFLA*/ 		int ret = vfs_fsync_range(iocb->ki_filp,
// /*AFLA*/ 				iocb->ki_pos - count, iocb->ki_pos - 1,
// /*AFLA*/ 				(iocb->ki_flags & IOCB_SYNC) ? 0 : 1);
// /*AFLA*/ 		if (ret)
// /*AFLA*/ 			return ret;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	return count;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void emergency_sync(void);
// /*AFLA*/ extern void emergency_remount(void);
// /*AFLA*/ #ifdef CONFIG_BLOCK
// /*AFLA*/ extern sector_t bmap(struct inode *, sector_t);
// /*AFLA*/ #endif
// /*AFLA*/ extern int notify_change(struct dentry *, struct iattr *, struct inode **);
// /*AFLA*/ extern int inode_permission(struct inode *, int);
// /*AFLA*/ extern int __inode_permission(struct inode *, int);
// /*AFLA*/ extern int generic_permission(struct inode *, int);
// /*AFLA*/ extern int __check_sticky(struct inode *dir, struct inode *inode);
// /*AFLA*/ 
// /*AFLA*/ static inline bool execute_ok(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return (inode->i_mode & S_IXUGO) || S_ISDIR(inode->i_mode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void file_start_write(struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	if (!S_ISREG(file_inode(file)->i_mode))
// /*AFLA*/ 		return;
// /*AFLA*/ 	__sb_start_write(file_inode(file)->i_sb, SB_FREEZE_WRITE, true);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool file_start_write_trylock(struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	if (!S_ISREG(file_inode(file)->i_mode))
// /*AFLA*/ 		return true;
// /*AFLA*/ 	return __sb_start_write(file_inode(file)->i_sb, SB_FREEZE_WRITE, false);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void file_end_write(struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	if (!S_ISREG(file_inode(file)->i_mode))
// /*AFLA*/ 		return;
// /*AFLA*/ 	__sb_end_write(file_inode(file)->i_sb, SB_FREEZE_WRITE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * get_write_access() gets write permission for a file.
// /*AFLA*/  * put_write_access() releases this write permission.
// /*AFLA*/  * This is used for regular files.
// /*AFLA*/  * We cannot support write (and maybe mmap read-write shared) accesses and
// /*AFLA*/  * MAP_DENYWRITE mmappings simultaneously. The i_writecount field of an inode
// /*AFLA*/  * can have the following values:
// /*AFLA*/  * 0: no writers, no VM_DENYWRITE mappings
// /*AFLA*/  * < 0: (-i_writecount) vm_area_structs with VM_DENYWRITE set exist
// /*AFLA*/  * > 0: (i_writecount) users are writing to the file.
// /*AFLA*/  *
// /*AFLA*/  * Normally we operate on that counter with atomic_{inc,dec} and it's safe
// /*AFLA*/  * except for the cases where we don't hold i_writecount yet. Then we need to
// /*AFLA*/  * use {get,deny}_write_access() - these functions check the sign and refuse
// /*AFLA*/  * to do the change if sign is wrong.
// /*AFLA*/  */
// /*AFLA*/ static inline int get_write_access(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_inc_unless_negative(&inode->i_writecount) ? 0 : -ETXTBSY;
// /*AFLA*/ }
// /*AFLA*/ static inline int deny_write_access(struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	struct inode *inode = file_inode(file);
// /*AFLA*/ 	return atomic_dec_unless_positive(&inode->i_writecount) ? 0 : -ETXTBSY;
// /*AFLA*/ }
// /*AFLA*/ static inline void put_write_access(struct inode * inode)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_dec(&inode->i_writecount);
// /*AFLA*/ }
// /*AFLA*/ static inline void allow_write_access(struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	if (file)
// /*AFLA*/ 		atomic_inc(&file_inode(file)->i_writecount);
// /*AFLA*/ }
// /*AFLA*/ static inline bool inode_is_open_for_write(const struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_read(&inode->i_writecount) > 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_IMA
// /*AFLA*/ static inline void i_readcount_dec(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	BUG_ON(!atomic_read(&inode->i_readcount));
// /*AFLA*/ 	atomic_dec(&inode->i_readcount);
// /*AFLA*/ }
// /*AFLA*/ static inline void i_readcount_inc(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_inc(&inode->i_readcount);
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline void i_readcount_dec(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ static inline void i_readcount_inc(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ extern int do_pipe_flags(int *, int);
// /*AFLA*/ 
// /*AFLA*/ #define __kernel_read_file_id(id) \
// /*AFLA*/ 	id(UNKNOWN, unknown)		\
// /*AFLA*/ 	id(FIRMWARE, firmware)		\
// /*AFLA*/ 	id(FIRMWARE_PREALLOC_BUFFER, firmware)	\
// /*AFLA*/ 	id(MODULE, kernel-module)		\
// /*AFLA*/ 	id(KEXEC_IMAGE, kexec-image)		\
// /*AFLA*/ 	id(KEXEC_INITRAMFS, kexec-initramfs)	\
// /*AFLA*/ 	id(POLICY, security-policy)		\
// /*AFLA*/ 	id(MAX_ID, )
// /*AFLA*/ 
// /*AFLA*/ #define __fid_enumify(ENUM, dummy) READING_ ## ENUM,
// /*AFLA*/ #define __fid_stringify(dummy, str) #str,
// /*AFLA*/ 
// /*AFLA*/ enum kernel_read_file_id {
// /*AFLA*/ 	__kernel_read_file_id(__fid_enumify)
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static const char * const kernel_read_file_str[] = {
// /*AFLA*/ 	__kernel_read_file_id(__fid_stringify)
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline const char *kernel_read_file_id_str(enum kernel_read_file_id id)
// /*AFLA*/ {
// /*AFLA*/ 	if (id < 0 || id >= READING_MAX_ID)
// /*AFLA*/ 		return kernel_read_file_str[READING_UNKNOWN];
// /*AFLA*/ 
// /*AFLA*/ 	return kernel_read_file_str[id];
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int kernel_read(struct file *, loff_t, char *, unsigned long);
// /*AFLA*/ extern int kernel_read_file(struct file *, void **, loff_t *, loff_t,
// /*AFLA*/ 			    enum kernel_read_file_id);
// /*AFLA*/ extern int kernel_read_file_from_path(char *, void **, loff_t *, loff_t,
// /*AFLA*/ 				      enum kernel_read_file_id);
// /*AFLA*/ extern int kernel_read_file_from_fd(int, void **, loff_t *, loff_t,
// /*AFLA*/ 				    enum kernel_read_file_id);
// /*AFLA*/ extern ssize_t kernel_write(struct file *, const char *, size_t, loff_t);
// /*AFLA*/ extern ssize_t __kernel_write(struct file *, const char *, size_t, loff_t *);
// /*AFLA*/ extern struct file * open_exec(const char *);
// /*AFLA*/  
// /*AFLA*/ /* fs/dcache.c -- generic fs support functions */
// /*AFLA*/ extern bool is_subdir(struct dentry *, struct dentry *);
// /*AFLA*/ extern bool path_is_under(struct path *, struct path *);
// /*AFLA*/ 
// /*AFLA*/ extern char *file_path(struct file *, char *, int);
// /*AFLA*/ 
// /*AFLA*/ #include <linux/err.h>
// /*AFLA*/ 
// /*AFLA*/ /* needed for stackable file system support */
// /*AFLA*/ extern loff_t default_llseek(struct file *file, loff_t offset, int whence);
// /*AFLA*/ 
// /*AFLA*/ extern loff_t vfs_llseek(struct file *file, loff_t offset, int whence);
// /*AFLA*/ 
// /*AFLA*/ extern int inode_init_always(struct super_block *, struct inode *);
// /*AFLA*/ extern void inode_init_once(struct inode *);
// /*AFLA*/ extern void address_space_init_once(struct address_space *mapping);
// /*AFLA*/ extern struct inode * igrab(struct inode *);
// /*AFLA*/ extern ino_t iunique(struct super_block *, ino_t);
// /*AFLA*/ extern int inode_needs_sync(struct inode *inode);
// /*AFLA*/ extern int generic_delete_inode(struct inode *inode);
// /*AFLA*/ static inline int generic_drop_inode(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return !inode->i_nlink || inode_unhashed(inode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct inode *ilookup5_nowait(struct super_block *sb,
// /*AFLA*/ 		unsigned long hashval, int (*test)(struct inode *, void *),
// /*AFLA*/ 		void *data);
// /*AFLA*/ extern struct inode *ilookup5(struct super_block *sb, unsigned long hashval,
// /*AFLA*/ 		int (*test)(struct inode *, void *), void *data);
// /*AFLA*/ extern struct inode *ilookup(struct super_block *sb, unsigned long ino);
// /*AFLA*/ 
// /*AFLA*/ extern struct inode * iget5_locked(struct super_block *, unsigned long, int (*test)(struct inode *, void *), int (*set)(struct inode *, void *), void *);
// /*AFLA*/ extern struct inode * iget_locked(struct super_block *, unsigned long);
// /*AFLA*/ extern struct inode *find_inode_nowait(struct super_block *,
// /*AFLA*/ 				       unsigned long,
// /*AFLA*/ 				       int (*match)(struct inode *,
// /*AFLA*/ 						    unsigned long, void *),
// /*AFLA*/ 				       void *data);
// /*AFLA*/ extern int insert_inode_locked4(struct inode *, unsigned long, int (*test)(struct inode *, void *), void *);
// /*AFLA*/ extern int insert_inode_locked(struct inode *);
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ extern void lockdep_annotate_inode_mutex_key(struct inode *inode);
// /*AFLA*/ #else
// /*AFLA*/ static inline void lockdep_annotate_inode_mutex_key(struct inode *inode) { };
// /*AFLA*/ #endif
// /*AFLA*/ extern void unlock_new_inode(struct inode *);
// /*AFLA*/ extern unsigned int get_next_ino(void);
// /*AFLA*/ 
// /*AFLA*/ extern void __iget(struct inode * inode);
// /*AFLA*/ extern void iget_failed(struct inode *);
// /*AFLA*/ extern void clear_inode(struct inode *);
// /*AFLA*/ extern void __destroy_inode(struct inode *);
// /*AFLA*/ extern struct inode *new_inode_pseudo(struct super_block *sb);
// /*AFLA*/ extern struct inode *new_inode(struct super_block *sb);
// /*AFLA*/ extern void free_inode_nonrcu(struct inode *inode);
// /*AFLA*/ extern int should_remove_suid(struct dentry *);
// /*AFLA*/ extern int file_remove_privs(struct file *);
// /*AFLA*/ 
// /*AFLA*/ extern void __insert_inode_hash(struct inode *, unsigned long hashval);
// /*AFLA*/ static inline void insert_inode_hash(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	__insert_inode_hash(inode, inode->i_ino);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void __remove_inode_hash(struct inode *);
// /*AFLA*/ static inline void remove_inode_hash(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	if (!inode_unhashed(inode) && !hlist_fake(&inode->i_hash))
// /*AFLA*/ 		__remove_inode_hash(inode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void inode_sb_list_add(struct inode *inode);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_BLOCK
// /*AFLA*/ extern blk_qc_t submit_bio(struct bio *);
// /*AFLA*/ extern int bdev_read_only(struct block_device *);
// /*AFLA*/ #endif
// /*AFLA*/ extern int set_blocksize(struct block_device *, int);
// /*AFLA*/ extern int sb_set_blocksize(struct super_block *, int);
// /*AFLA*/ extern int sb_min_blocksize(struct super_block *, int);
// /*AFLA*/ 
// /*AFLA*/ extern int generic_file_mmap(struct file *, struct vm_area_struct *);
// /*AFLA*/ extern int generic_file_readonly_mmap(struct file *, struct vm_area_struct *);
// /*AFLA*/ extern ssize_t generic_write_checks(struct kiocb *, struct iov_iter *);
// /*AFLA*/ extern ssize_t generic_file_read_iter(struct kiocb *, struct iov_iter *);
// /*AFLA*/ extern ssize_t __generic_file_write_iter(struct kiocb *, struct iov_iter *);
// /*AFLA*/ extern ssize_t generic_file_write_iter(struct kiocb *, struct iov_iter *);
// /*AFLA*/ extern ssize_t generic_file_direct_write(struct kiocb *, struct iov_iter *);
// /*AFLA*/ extern ssize_t generic_perform_write(struct file *, struct iov_iter *, loff_t);
// /*AFLA*/ 
// /*AFLA*/ ssize_t vfs_iter_read(struct file *file, struct iov_iter *iter, loff_t *ppos);
// /*AFLA*/ ssize_t vfs_iter_write(struct file *file, struct iov_iter *iter, loff_t *ppos);
// /*AFLA*/ 
// /*AFLA*/ /* fs/block_dev.c */
// /*AFLA*/ extern ssize_t blkdev_read_iter(struct kiocb *iocb, struct iov_iter *to);
// /*AFLA*/ extern ssize_t blkdev_write_iter(struct kiocb *iocb, struct iov_iter *from);
// /*AFLA*/ extern int blkdev_fsync(struct file *filp, loff_t start, loff_t end,
// /*AFLA*/ 			int datasync);
// /*AFLA*/ extern void block_sync_page(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ /* fs/splice.c */
// /*AFLA*/ extern ssize_t generic_file_splice_read(struct file *, loff_t *,
// /*AFLA*/ 		struct pipe_inode_info *, size_t, unsigned int);
// /*AFLA*/ extern ssize_t iter_file_splice_write(struct pipe_inode_info *,
// /*AFLA*/ 		struct file *, loff_t *, size_t, unsigned int);
// /*AFLA*/ extern ssize_t generic_splice_sendpage(struct pipe_inode_info *pipe,
// /*AFLA*/ 		struct file *out, loff_t *, size_t len, unsigned int flags);
// /*AFLA*/ extern long do_splice_direct(struct file *in, loff_t *ppos, struct file *out,
// /*AFLA*/ 		loff_t *opos, size_t len, unsigned int flags);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ extern void
// /*AFLA*/ file_ra_state_init(struct file_ra_state *ra, struct address_space *mapping);
// /*AFLA*/ extern loff_t noop_llseek(struct file *file, loff_t offset, int whence);
// /*AFLA*/ extern loff_t no_llseek(struct file *file, loff_t offset, int whence);
// /*AFLA*/ extern loff_t vfs_setpos(struct file *file, loff_t offset, loff_t maxsize);
// /*AFLA*/ extern loff_t generic_file_llseek(struct file *file, loff_t offset, int whence);
// /*AFLA*/ extern loff_t generic_file_llseek_size(struct file *file, loff_t offset,
// /*AFLA*/ 		int whence, loff_t maxsize, loff_t eof);
// /*AFLA*/ extern loff_t fixed_size_llseek(struct file *file, loff_t offset,
// /*AFLA*/ 		int whence, loff_t size);
// /*AFLA*/ extern loff_t no_seek_end_llseek_size(struct file *, loff_t, int, loff_t);
// /*AFLA*/ extern loff_t no_seek_end_llseek(struct file *, loff_t, int);
// /*AFLA*/ extern int generic_file_open(struct inode * inode, struct file * filp);
// /*AFLA*/ extern int nonseekable_open(struct inode * inode, struct file * filp);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_BLOCK
// /*AFLA*/ typedef void (dio_submit_t)(struct bio *bio, struct inode *inode,
// /*AFLA*/ 			    loff_t file_offset);
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	/* need locking between buffered and direct access */
// /*AFLA*/ 	DIO_LOCKING	= 0x01,
// /*AFLA*/ 
// /*AFLA*/ 	/* filesystem does not support filling holes */
// /*AFLA*/ 	DIO_SKIP_HOLES	= 0x02,
// /*AFLA*/ 
// /*AFLA*/ 	/* filesystem can handle aio writes beyond i_size */
// /*AFLA*/ 	DIO_ASYNC_EXTEND = 0x04,
// /*AFLA*/ 
// /*AFLA*/ 	/* inode/fs/bdev does not need truncate protection */
// /*AFLA*/ 	DIO_SKIP_DIO_COUNT = 0x08,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ void dio_end_io(struct bio *bio, int error);
// /*AFLA*/ 
// /*AFLA*/ ssize_t __blockdev_direct_IO(struct kiocb *iocb, struct inode *inode,
// /*AFLA*/ 			     struct block_device *bdev, struct iov_iter *iter,
// /*AFLA*/ 			     get_block_t get_block,
// /*AFLA*/ 			     dio_iodone_t end_io, dio_submit_t submit_io,
// /*AFLA*/ 			     int flags);
// /*AFLA*/ 
// /*AFLA*/ static inline ssize_t blockdev_direct_IO(struct kiocb *iocb,
// /*AFLA*/ 					 struct inode *inode,
// /*AFLA*/ 					 struct iov_iter *iter,
// /*AFLA*/ 					 get_block_t get_block)
// /*AFLA*/ {
// /*AFLA*/ 	return __blockdev_direct_IO(iocb, inode, inode->i_sb->s_bdev, iter,
// /*AFLA*/ 			get_block, NULL, NULL, DIO_LOCKING | DIO_SKIP_HOLES);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ void inode_dio_wait(struct inode *inode);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * inode_dio_begin - signal start of a direct I/O requests
// /*AFLA*/  * @inode: inode the direct I/O happens on
// /*AFLA*/  *
// /*AFLA*/  * This is called once we've finished processing a direct I/O request,
// /*AFLA*/  * and is used to wake up callers waiting for direct I/O to be quiesced.
// /*AFLA*/  */
// /*AFLA*/ static inline void inode_dio_begin(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_inc(&inode->i_dio_count);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * inode_dio_end - signal finish of a direct I/O requests
// /*AFLA*/  * @inode: inode the direct I/O happens on
// /*AFLA*/  *
// /*AFLA*/  * This is called once we've finished processing a direct I/O request,
// /*AFLA*/  * and is used to wake up callers waiting for direct I/O to be quiesced.
// /*AFLA*/  */
// /*AFLA*/ static inline void inode_dio_end(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	if (atomic_dec_and_test(&inode->i_dio_count))
// /*AFLA*/ 		wake_up_bit(&inode->i_state, __I_DIO_WAKEUP);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void inode_set_flags(struct inode *inode, unsigned int flags,
// /*AFLA*/ 			    unsigned int mask);
// /*AFLA*/ 
// /*AFLA*/ extern const struct file_operations generic_ro_fops;
// /*AFLA*/ 
// /*AFLA*/ #define special_file(m) (S_ISCHR(m)||S_ISBLK(m)||S_ISFIFO(m)||S_ISSOCK(m))
// /*AFLA*/ 
// /*AFLA*/ extern int readlink_copy(char __user *, int, const char *);
// /*AFLA*/ extern int page_readlink(struct dentry *, char __user *, int);
// /*AFLA*/ extern const char *page_get_link(struct dentry *, struct inode *,
// /*AFLA*/ 				 struct delayed_call *);
// /*AFLA*/ extern void page_put_link(void *);
// /*AFLA*/ extern int __page_symlink(struct inode *inode, const char *symname, int len,
// /*AFLA*/ 		int nofs);
// /*AFLA*/ extern int page_symlink(struct inode *inode, const char *symname, int len);
// /*AFLA*/ extern const struct inode_operations page_symlink_inode_operations;
// /*AFLA*/ extern void kfree_link(void *);
// /*AFLA*/ extern int generic_readlink(struct dentry *, char __user *, int);
// /*AFLA*/ extern void generic_fillattr(struct inode *, struct kstat *);
// /*AFLA*/ int vfs_getattr_nosec(struct path *path, struct kstat *stat);
// /*AFLA*/ extern int vfs_getattr(struct path *, struct kstat *);
// /*AFLA*/ void __inode_add_bytes(struct inode *inode, loff_t bytes);
// /*AFLA*/ void inode_add_bytes(struct inode *inode, loff_t bytes);
// /*AFLA*/ void __inode_sub_bytes(struct inode *inode, loff_t bytes);
// /*AFLA*/ void inode_sub_bytes(struct inode *inode, loff_t bytes);
// /*AFLA*/ loff_t inode_get_bytes(struct inode *inode);
// /*AFLA*/ void inode_set_bytes(struct inode *inode, loff_t bytes);
// /*AFLA*/ const char *simple_get_link(struct dentry *, struct inode *,
// /*AFLA*/ 			    struct delayed_call *);
// /*AFLA*/ extern const struct inode_operations simple_symlink_inode_operations;
// /*AFLA*/ 
// /*AFLA*/ extern int iterate_dir(struct file *, struct dir_context *);
// /*AFLA*/ 
// /*AFLA*/ extern int vfs_stat(const char __user *, struct kstat *);
// /*AFLA*/ extern int vfs_lstat(const char __user *, struct kstat *);
// /*AFLA*/ extern int vfs_fstat(unsigned int, struct kstat *);
// /*AFLA*/ extern int vfs_fstatat(int , const char __user *, struct kstat *, int);
// /*AFLA*/ extern const char *vfs_get_link(struct dentry *, struct delayed_call *);
// /*AFLA*/ 
// /*AFLA*/ extern int __generic_block_fiemap(struct inode *inode,
// /*AFLA*/ 				  struct fiemap_extent_info *fieinfo,
// /*AFLA*/ 				  loff_t start, loff_t len,
// /*AFLA*/ 				  get_block_t *get_block);
// /*AFLA*/ extern int generic_block_fiemap(struct inode *inode,
// /*AFLA*/ 				struct fiemap_extent_info *fieinfo, u64 start,
// /*AFLA*/ 				u64 len, get_block_t *get_block);
// /*AFLA*/ 
// /*AFLA*/ extern void get_filesystem(struct file_system_type *fs);
// /*AFLA*/ extern void put_filesystem(struct file_system_type *fs);
// /*AFLA*/ extern struct file_system_type *get_fs_type(const char *name);
// /*AFLA*/ extern struct super_block *get_super(struct block_device *);
// /*AFLA*/ extern struct super_block *get_super_thawed(struct block_device *);
// /*AFLA*/ extern struct super_block *get_active_super(struct block_device *bdev);
// /*AFLA*/ extern void drop_super(struct super_block *sb);
// /*AFLA*/ extern void iterate_supers(void (*)(struct super_block *, void *), void *);
// /*AFLA*/ extern void iterate_supers_type(struct file_system_type *,
// /*AFLA*/ 			        void (*)(struct super_block *, void *), void *);
// /*AFLA*/ 
// /*AFLA*/ extern int dcache_dir_open(struct inode *, struct file *);
// /*AFLA*/ extern int dcache_dir_close(struct inode *, struct file *);
// /*AFLA*/ extern loff_t dcache_dir_lseek(struct file *, loff_t, int);
// /*AFLA*/ extern int dcache_readdir(struct file *, struct dir_context *);
// /*AFLA*/ extern int simple_setattr(struct dentry *, struct iattr *);
// /*AFLA*/ extern int simple_getattr(struct vfsmount *, struct dentry *, struct kstat *);
// /*AFLA*/ extern int simple_statfs(struct dentry *, struct kstatfs *);
// /*AFLA*/ extern int simple_open(struct inode *inode, struct file *file);
// /*AFLA*/ extern int simple_link(struct dentry *, struct inode *, struct dentry *);
// /*AFLA*/ extern int simple_unlink(struct inode *, struct dentry *);
// /*AFLA*/ extern int simple_rmdir(struct inode *, struct dentry *);
// /*AFLA*/ extern int simple_rename(struct inode *, struct dentry *,
// /*AFLA*/ 			 struct inode *, struct dentry *, unsigned int);
// /*AFLA*/ extern int noop_fsync(struct file *, loff_t, loff_t, int);
// /*AFLA*/ extern int simple_empty(struct dentry *);
// /*AFLA*/ extern int simple_readpage(struct file *file, struct page *page);
// /*AFLA*/ extern int simple_write_begin(struct file *file, struct address_space *mapping,
// /*AFLA*/ 			loff_t pos, unsigned len, unsigned flags,
// /*AFLA*/ 			struct page **pagep, void **fsdata);
// /*AFLA*/ extern int simple_write_end(struct file *file, struct address_space *mapping,
// /*AFLA*/ 			loff_t pos, unsigned len, unsigned copied,
// /*AFLA*/ 			struct page *page, void *fsdata);
// /*AFLA*/ extern int always_delete_dentry(const struct dentry *);
// /*AFLA*/ extern struct inode *alloc_anon_inode(struct super_block *);
// /*AFLA*/ extern int simple_nosetlease(struct file *, long, struct file_lock **, void **);
// /*AFLA*/ extern const struct dentry_operations simple_dentry_operations;
// /*AFLA*/ 
// /*AFLA*/ extern struct dentry *simple_lookup(struct inode *, struct dentry *, unsigned int flags);
// /*AFLA*/ extern ssize_t generic_read_dir(struct file *, char __user *, size_t, loff_t *);
// /*AFLA*/ extern const struct file_operations simple_dir_operations;
// /*AFLA*/ extern const struct inode_operations simple_dir_inode_operations;
// /*AFLA*/ extern void make_empty_dir_inode(struct inode *inode);
// /*AFLA*/ extern bool is_empty_dir_inode(struct inode *inode);
// /*AFLA*/ struct tree_descr { char *name; const struct file_operations *ops; int mode; };
// /*AFLA*/ struct dentry *d_alloc_name(struct dentry *, const char *);
// /*AFLA*/ extern int simple_fill_super(struct super_block *, unsigned long, struct tree_descr *);
// /*AFLA*/ extern int simple_pin_fs(struct file_system_type *, struct vfsmount **mount, int *count);
// /*AFLA*/ extern void simple_release_fs(struct vfsmount **mount, int *count);
// /*AFLA*/ 
// /*AFLA*/ extern ssize_t simple_read_from_buffer(void __user *to, size_t count,
// /*AFLA*/ 			loff_t *ppos, const void *from, size_t available);
// /*AFLA*/ extern ssize_t simple_write_to_buffer(void *to, size_t available, loff_t *ppos,
// /*AFLA*/ 		const void __user *from, size_t count);
// /*AFLA*/ 
// /*AFLA*/ extern int __generic_file_fsync(struct file *, loff_t, loff_t, int);
// /*AFLA*/ extern int generic_file_fsync(struct file *, loff_t, loff_t, int);
// /*AFLA*/ 
// /*AFLA*/ extern int generic_check_addressable(unsigned, u64);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MIGRATION
// /*AFLA*/ extern int buffer_migrate_page(struct address_space *,
// /*AFLA*/ 				struct page *, struct page *,
// /*AFLA*/ 				enum migrate_mode);
// /*AFLA*/ #else
// /*AFLA*/ #define buffer_migrate_page NULL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern int setattr_prepare(struct dentry *, struct iattr *);
// /*AFLA*/ extern int inode_newsize_ok(const struct inode *, loff_t offset);
// /*AFLA*/ extern void setattr_copy(struct inode *inode, const struct iattr *attr);
// /*AFLA*/ 
// /*AFLA*/ extern int file_update_time(struct file *file);
// /*AFLA*/ 
// /*AFLA*/ extern int generic_show_options(struct seq_file *m, struct dentry *root);
// /*AFLA*/ extern void save_mount_options(struct super_block *sb, char *options);
// /*AFLA*/ extern void replace_mount_options(struct super_block *sb, char *options);
// /*AFLA*/ 
// /*AFLA*/ static inline bool io_is_direct(struct file *filp)
// /*AFLA*/ {
// /*AFLA*/ 	return (filp->f_flags & O_DIRECT) || IS_DAX(filp->f_mapping->host);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int iocb_flags(struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	int res = 0;
// /*AFLA*/ 	if (file->f_flags & O_APPEND)
// /*AFLA*/ 		res |= IOCB_APPEND;
// /*AFLA*/ 	if (io_is_direct(file))
// /*AFLA*/ 		res |= IOCB_DIRECT;
// /*AFLA*/ 	if ((file->f_flags & O_DSYNC) || IS_SYNC(file->f_mapping->host))
// /*AFLA*/ 		res |= IOCB_DSYNC;
// /*AFLA*/ 	if (file->f_flags & __O_SYNC)
// /*AFLA*/ 		res |= IOCB_SYNC;
// /*AFLA*/ 	return res;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline ino_t parent_ino(struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	ino_t res;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Don't strictly need d_lock here? If the parent ino could change
// /*AFLA*/ 	 * then surely we'd have a deeper race in the caller?
// /*AFLA*/ 	 */
// /*AFLA*/ 	spin_lock(&dentry->d_lock);
// /*AFLA*/ 	res = dentry->d_parent->d_inode->i_ino;
// /*AFLA*/ 	spin_unlock(&dentry->d_lock);
// /*AFLA*/ 	return res;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Transaction based IO helpers */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * An argresp is stored in an allocated page and holds the
// /*AFLA*/  * size of the argument or response, along with its content
// /*AFLA*/  */
// /*AFLA*/ struct simple_transaction_argresp {
// /*AFLA*/ 	ssize_t size;
// /*AFLA*/ 	char data[0];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define SIMPLE_TRANSACTION_LIMIT (PAGE_SIZE - sizeof(struct simple_transaction_argresp))
// /*AFLA*/ 
// /*AFLA*/ char *simple_transaction_get(struct file *file, const char __user *buf,
// /*AFLA*/ 				size_t size);
// /*AFLA*/ ssize_t simple_transaction_read(struct file *file, char __user *buf,
// /*AFLA*/ 				size_t size, loff_t *pos);
// /*AFLA*/ int simple_transaction_release(struct inode *inode, struct file *file);
// /*AFLA*/ 
// /*AFLA*/ void simple_transaction_set(struct file *file, size_t n);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * simple attribute files
// /*AFLA*/  *
// /*AFLA*/  * These attributes behave similar to those in sysfs:
// /*AFLA*/  *
// /*AFLA*/  * Writing to an attribute immediately sets a value, an open file can be
// /*AFLA*/  * written to multiple times.
// /*AFLA*/  *
// /*AFLA*/  * Reading from an attribute creates a buffer from the value that might get
// /*AFLA*/  * read with multiple read calls. When the attribute has been read
// /*AFLA*/  * completely, no further read calls are possible until the file is opened
// /*AFLA*/  * again.
// /*AFLA*/  *
// /*AFLA*/  * All attributes contain a text representation of a numeric value
// /*AFLA*/  * that are accessed with the get() and set() functions.
// /*AFLA*/  */
// /*AFLA*/ #define DEFINE_SIMPLE_ATTRIBUTE(__fops, __get, __set, __fmt)		\
// /*AFLA*/ static int __fops ## _open(struct inode *inode, struct file *file)	\
// /*AFLA*/ {									\
// /*AFLA*/ 	__simple_attr_check_format(__fmt, 0ull);			\
// /*AFLA*/ 	return simple_attr_open(inode, file, __get, __set, __fmt);	\
// /*AFLA*/ }									\
// /*AFLA*/ static const struct file_operations __fops = {				\
// /*AFLA*/ 	.owner	 = THIS_MODULE,						\
// /*AFLA*/ 	.open	 = __fops ## _open,					\
// /*AFLA*/ 	.release = simple_attr_release,					\
// /*AFLA*/ 	.read	 = simple_attr_read,					\
// /*AFLA*/ 	.write	 = simple_attr_write,					\
// /*AFLA*/ 	.llseek	 = generic_file_llseek,					\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline __printf(1, 2)
// /*AFLA*/ void __simple_attr_check_format(const char *fmt, ...)
// /*AFLA*/ {
// /*AFLA*/ 	/* don't do anything, just let the compiler check the arguments; */
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ int simple_attr_open(struct inode *inode, struct file *file,
// /*AFLA*/ 		     int (*get)(void *, u64 *), int (*set)(void *, u64),
// /*AFLA*/ 		     const char *fmt);
// /*AFLA*/ int simple_attr_release(struct inode *inode, struct file *file);
// /*AFLA*/ ssize_t simple_attr_read(struct file *file, char __user *buf,
// /*AFLA*/ 			 size_t len, loff_t *ppos);
// /*AFLA*/ ssize_t simple_attr_write(struct file *file, const char __user *buf,
// /*AFLA*/ 			  size_t len, loff_t *ppos);
// /*AFLA*/ 
// /*AFLA*/ struct ctl_table;
// /*AFLA*/ int proc_nr_files(struct ctl_table *table, int write,
// /*AFLA*/ 		  void __user *buffer, size_t *lenp, loff_t *ppos);
// /*AFLA*/ int proc_nr_dentry(struct ctl_table *table, int write,
// /*AFLA*/ 		  void __user *buffer, size_t *lenp, loff_t *ppos);
// /*AFLA*/ int proc_nr_inodes(struct ctl_table *table, int write,
// /*AFLA*/ 		   void __user *buffer, size_t *lenp, loff_t *ppos);
// /*AFLA*/ int __init get_filesystem_list(char *buf);
// /*AFLA*/ 
// /*AFLA*/ #define __FMODE_EXEC		((__force int) FMODE_EXEC)
// /*AFLA*/ #define __FMODE_NONOTIFY	((__force int) FMODE_NONOTIFY)
// /*AFLA*/ 
// /*AFLA*/ #define ACC_MODE(x) ("\004\002\006\006"[(x)&O_ACCMODE])
// /*AFLA*/ #define OPEN_FMODE(flag) ((__force fmode_t)(((flag + 1) & O_ACCMODE) | \
// /*AFLA*/ 					    (flag & __FMODE_NONOTIFY)))
// /*AFLA*/ 
// /*AFLA*/ static inline bool is_sxid(umode_t mode)
// /*AFLA*/ {
// /*AFLA*/ 	return (mode & S_ISUID) || ((mode & S_ISGID) && (mode & S_IXGRP));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int check_sticky(struct inode *dir, struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	if (!(dir->i_mode & S_ISVTX))
// /*AFLA*/ 		return 0;
// /*AFLA*/ 
// /*AFLA*/ 	return __check_sticky(dir, inode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void inode_has_no_xattr(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	if (!is_sxid(inode->i_mode) && (inode->i_sb->s_flags & MS_NOSEC))
// /*AFLA*/ 		inode->i_flags |= S_NOSEC;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool is_root_inode(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return inode == inode->i_sb->s_root->d_inode;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool dir_emit(struct dir_context *ctx,
// /*AFLA*/ 			    const char *name, int namelen,
// /*AFLA*/ 			    u64 ino, unsigned type)
// /*AFLA*/ {
// /*AFLA*/ 	return ctx->actor(ctx, name, namelen, ctx->pos, ino, type) == 0;
// /*AFLA*/ }
// /*AFLA*/ static inline bool dir_emit_dot(struct file *file, struct dir_context *ctx)
// /*AFLA*/ {
// /*AFLA*/ 	return ctx->actor(ctx, ".", 1, ctx->pos,
// /*AFLA*/ 			  file->f_path.dentry->d_inode->i_ino, DT_DIR) == 0;
// /*AFLA*/ }
// /*AFLA*/ static inline bool dir_emit_dotdot(struct file *file, struct dir_context *ctx)
// /*AFLA*/ {
// /*AFLA*/ 	return ctx->actor(ctx, "..", 2, ctx->pos,
// /*AFLA*/ 			  parent_ino(file->f_path.dentry), DT_DIR) == 0;
// /*AFLA*/ }
// /*AFLA*/ static inline bool dir_emit_dots(struct file *file, struct dir_context *ctx)
// /*AFLA*/ {
// /*AFLA*/ 	if (ctx->pos == 0) {
// /*AFLA*/ 		if (!dir_emit_dot(file, ctx))
// /*AFLA*/ 			return false;
// /*AFLA*/ 		ctx->pos = 1;
// /*AFLA*/ 	}
// /*AFLA*/ 	if (ctx->pos == 1) {
// /*AFLA*/ 		if (!dir_emit_dotdot(file, ctx))
// /*AFLA*/ 			return false;
// /*AFLA*/ 		ctx->pos = 2;
// /*AFLA*/ 	}
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ static inline bool dir_relax(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	inode_unlock(inode);
// /*AFLA*/ 	inode_lock(inode);
// /*AFLA*/ 	return !IS_DEADDIR(inode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool dir_relax_shared(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	inode_unlock_shared(inode);
// /*AFLA*/ 	inode_lock_shared(inode);
// /*AFLA*/ 	return !IS_DEADDIR(inode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern bool path_noexec(const struct path *path);
// /*AFLA*/ extern void inode_nohighmem(struct inode *inode);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_FS_H */
