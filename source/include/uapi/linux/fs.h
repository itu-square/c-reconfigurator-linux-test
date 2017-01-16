// /*AFLA*/ #ifndef _UAPI_LINUX_FS_H
// /*AFLA*/ #define _UAPI_LINUX_FS_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This file has definitions for some important file table structures
// /*AFLA*/  * and constants and structures used by various generic file system
// /*AFLA*/  * ioctl's.  Please do not make any changes in this file before
// /*AFLA*/  * sending patches for review to linux-fsdevel@vger.kernel.org and
// /*AFLA*/  * linux-api@vger.kernel.org.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/limits.h>
// /*AFLA*/ #include <linux/ioctl.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * It's silly to have NR_OPEN bigger than NR_FILE, but you can change
// /*AFLA*/  * the file limit at runtime and only root can increase the per-process
// /*AFLA*/  * nr_file rlimit, so it's safe to set up a ridiculously high absolute
// /*AFLA*/  * upper limit on files-per-process.
// /*AFLA*/  *
// /*AFLA*/  * Some programs (notably those using select()) may have to be 
// /*AFLA*/  * recompiled to take full advantage of the new limits..  
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* Fixed constants first: */
// /*AFLA*/ #undef NR_OPEN
// /*AFLA*/ #define INR_OPEN_CUR 1024	/* Initial setting for nfile rlimits */
// /*AFLA*/ #define INR_OPEN_MAX 4096	/* Hard limit for nfile rlimits */
// /*AFLA*/ 
// /*AFLA*/ #define BLOCK_SIZE_BITS 10
// /*AFLA*/ #define BLOCK_SIZE (1<<BLOCK_SIZE_BITS)
// /*AFLA*/ 
// /*AFLA*/ #define SEEK_SET	0	/* seek relative to beginning of file */
// /*AFLA*/ #define SEEK_CUR	1	/* seek relative to current file position */
// /*AFLA*/ #define SEEK_END	2	/* seek relative to end of file */
// /*AFLA*/ #define SEEK_DATA	3	/* seek to the next data */
// /*AFLA*/ #define SEEK_HOLE	4	/* seek to the next hole */
// /*AFLA*/ #define SEEK_MAX	SEEK_HOLE
// /*AFLA*/ 
// /*AFLA*/ #define RENAME_NOREPLACE	(1 << 0)	/* Don't overwrite target */
// /*AFLA*/ #define RENAME_EXCHANGE		(1 << 1)	/* Exchange source and dest */
// /*AFLA*/ #define RENAME_WHITEOUT		(1 << 2)	/* Whiteout source */
// /*AFLA*/ 
// /*AFLA*/ struct file_clone_range {
// /*AFLA*/ 	__s64 src_fd;
// /*AFLA*/ 	__u64 src_offset;
// /*AFLA*/ 	__u64 src_length;
// /*AFLA*/ 	__u64 dest_offset;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct fstrim_range {
// /*AFLA*/ 	__u64 start;
// /*AFLA*/ 	__u64 len;
// /*AFLA*/ 	__u64 minlen;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* extent-same (dedupe) ioctls; these MUST match the btrfs ioctl definitions */
// /*AFLA*/ #define FILE_DEDUPE_RANGE_SAME		0
// /*AFLA*/ #define FILE_DEDUPE_RANGE_DIFFERS	1
// /*AFLA*/ 
// /*AFLA*/ /* from struct btrfs_ioctl_file_extent_same_info */
// /*AFLA*/ struct file_dedupe_range_info {
// /*AFLA*/ 	__s64 dest_fd;		/* in - destination file */
// /*AFLA*/ 	__u64 dest_offset;	/* in - start of extent in destination */
// /*AFLA*/ 	__u64 bytes_deduped;	/* out - total # of bytes we were able
// /*AFLA*/ 				 * to dedupe from this file. */
// /*AFLA*/ 	/* status of this dedupe operation:
// /*AFLA*/ 	 * < 0 for error
// /*AFLA*/ 	 * == FILE_DEDUPE_RANGE_SAME if dedupe succeeds
// /*AFLA*/ 	 * == FILE_DEDUPE_RANGE_DIFFERS if data differs
// /*AFLA*/ 	 */
// /*AFLA*/ 	__s32 status;		/* out - see above description */
// /*AFLA*/ 	__u32 reserved;		/* must be zero */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* from struct btrfs_ioctl_file_extent_same_args */
// /*AFLA*/ struct file_dedupe_range {
// /*AFLA*/ 	__u64 src_offset;	/* in - start of extent in source */
// /*AFLA*/ 	__u64 src_length;	/* in - length of extent */
// /*AFLA*/ 	__u16 dest_count;	/* in - total elements in info array */
// /*AFLA*/ 	__u16 reserved1;	/* must be zero */
// /*AFLA*/ 	__u32 reserved2;	/* must be zero */
// /*AFLA*/ 	struct file_dedupe_range_info info[0];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* And dynamically-tunable limits and defaults: */
// /*AFLA*/ struct files_stat_struct {
// /*AFLA*/ 	unsigned long nr_files;		/* read only */
// /*AFLA*/ 	unsigned long nr_free_files;	/* read only */
// /*AFLA*/ 	unsigned long max_files;		/* tunable */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct inodes_stat_t {
// /*AFLA*/ 	long nr_inodes;
// /*AFLA*/ 	long nr_unused;
// /*AFLA*/ 	long dummy[5];		/* padding for sysctl ABI compatibility */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define NR_FILE  8192	/* this can well be larger on a larger system */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These are the fs-independent mount-flags: up to 32 flags are supported
// /*AFLA*/  */
// /*AFLA*/ #define MS_RDONLY	 1	/* Mount read-only */
// /*AFLA*/ #define MS_NOSUID	 2	/* Ignore suid and sgid bits */
// /*AFLA*/ #define MS_NODEV	 4	/* Disallow access to device special files */
// /*AFLA*/ #define MS_NOEXEC	 8	/* Disallow program execution */
// /*AFLA*/ #define MS_SYNCHRONOUS	16	/* Writes are synced at once */
// /*AFLA*/ #define MS_REMOUNT	32	/* Alter flags of a mounted FS */
// /*AFLA*/ #define MS_MANDLOCK	64	/* Allow mandatory locks on an FS */
// /*AFLA*/ #define MS_DIRSYNC	128	/* Directory modifications are synchronous */
// /*AFLA*/ #define MS_NOATIME	1024	/* Do not update access times. */
// /*AFLA*/ #define MS_NODIRATIME	2048	/* Do not update directory access times */
// /*AFLA*/ #define MS_BIND		4096
// /*AFLA*/ #define MS_MOVE		8192
// /*AFLA*/ #define MS_REC		16384
// /*AFLA*/ #define MS_VERBOSE	32768	/* War is peace. Verbosity is silence.
// /*AFLA*/ 				   MS_VERBOSE is deprecated. */
// /*AFLA*/ #define MS_SILENT	32768
// /*AFLA*/ #define MS_POSIXACL	(1<<16)	/* VFS does not apply the umask */
// /*AFLA*/ #define MS_UNBINDABLE	(1<<17)	/* change to unbindable */
// /*AFLA*/ #define MS_PRIVATE	(1<<18)	/* change to private */
// /*AFLA*/ #define MS_SLAVE	(1<<19)	/* change to slave */
// /*AFLA*/ #define MS_SHARED	(1<<20)	/* change to shared */
// /*AFLA*/ #define MS_RELATIME	(1<<21)	/* Update atime relative to mtime/ctime. */
// /*AFLA*/ #define MS_KERNMOUNT	(1<<22) /* this is a kern_mount call */
// /*AFLA*/ #define MS_I_VERSION	(1<<23) /* Update inode I_version field */
// /*AFLA*/ #define MS_STRICTATIME	(1<<24) /* Always perform atime updates */
// /*AFLA*/ #define MS_LAZYTIME	(1<<25) /* Update the on-disk [acm]times lazily */
// /*AFLA*/ 
// /*AFLA*/ /* These sb flags are internal to the kernel */
// /*AFLA*/ #define MS_NOREMOTELOCK	(1<<27)
// /*AFLA*/ #define MS_NOSEC	(1<<28)
// /*AFLA*/ #define MS_BORN		(1<<29)
// /*AFLA*/ #define MS_ACTIVE	(1<<30)
// /*AFLA*/ #define MS_NOUSER	(1<<31)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Superblock flags that can be altered by MS_REMOUNT
// /*AFLA*/  */
// /*AFLA*/ #define MS_RMT_MASK	(MS_RDONLY|MS_SYNCHRONOUS|MS_MANDLOCK|MS_I_VERSION|\
// /*AFLA*/ 			 MS_LAZYTIME)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Old magic mount flag and mask
// /*AFLA*/  */
// /*AFLA*/ #define MS_MGC_VAL 0xC0ED0000
// /*AFLA*/ #define MS_MGC_MSK 0xffff0000
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Structure for FS_IOC_FSGETXATTR[A] and FS_IOC_FSSETXATTR.
// /*AFLA*/  */
// /*AFLA*/ struct fsxattr {
// /*AFLA*/ 	__u32		fsx_xflags;	/* xflags field value (get/set) */
// /*AFLA*/ 	__u32		fsx_extsize;	/* extsize field value (get/set)*/
// /*AFLA*/ 	__u32		fsx_nextents;	/* nextents field value (get)	*/
// /*AFLA*/ 	__u32		fsx_projid;	/* project identifier (get/set) */
// /*AFLA*/ 	__u32		fsx_cowextsize;	/* CoW extsize field value (get/set)*/
// /*AFLA*/ 	unsigned char	fsx_pad[8];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Flags for the fsx_xflags field
// /*AFLA*/  */
// /*AFLA*/ #define FS_XFLAG_REALTIME	0x00000001	/* data in realtime volume */
// /*AFLA*/ #define FS_XFLAG_PREALLOC	0x00000002	/* preallocated file extents */
// /*AFLA*/ #define FS_XFLAG_IMMUTABLE	0x00000008	/* file cannot be modified */
// /*AFLA*/ #define FS_XFLAG_APPEND		0x00000010	/* all writes append */
// /*AFLA*/ #define FS_XFLAG_SYNC		0x00000020	/* all writes synchronous */
// /*AFLA*/ #define FS_XFLAG_NOATIME	0x00000040	/* do not update access time */
// /*AFLA*/ #define FS_XFLAG_NODUMP		0x00000080	/* do not include in backups */
// /*AFLA*/ #define FS_XFLAG_RTINHERIT	0x00000100	/* create with rt bit set */
// /*AFLA*/ #define FS_XFLAG_PROJINHERIT	0x00000200	/* create with parents projid */
// /*AFLA*/ #define FS_XFLAG_NOSYMLINKS	0x00000400	/* disallow symlink creation */
// /*AFLA*/ #define FS_XFLAG_EXTSIZE	0x00000800	/* extent size allocator hint */
// /*AFLA*/ #define FS_XFLAG_EXTSZINHERIT	0x00001000	/* inherit inode extent size */
// /*AFLA*/ #define FS_XFLAG_NODEFRAG	0x00002000	/* do not defragment */
// /*AFLA*/ #define FS_XFLAG_FILESTREAM	0x00004000	/* use filestream allocator */
// /*AFLA*/ #define FS_XFLAG_DAX		0x00008000	/* use DAX for IO */
// /*AFLA*/ #define FS_XFLAG_COWEXTSIZE	0x00010000	/* CoW extent size allocator hint */
// /*AFLA*/ #define FS_XFLAG_HASATTR	0x80000000	/* no DIFLAG for this	*/
// /*AFLA*/ 
// /*AFLA*/ /* the read-only stuff doesn't really belong here, but any other place is
// /*AFLA*/    probably as bad and I don't want to create yet another include file. */
// /*AFLA*/ 
// /*AFLA*/ #define BLKROSET   _IO(0x12,93)	/* set device read-only (0 = read-write) */
// /*AFLA*/ #define BLKROGET   _IO(0x12,94)	/* get read-only status (0 = read_write) */
// /*AFLA*/ #define BLKRRPART  _IO(0x12,95)	/* re-read partition table */
// /*AFLA*/ #define BLKGETSIZE _IO(0x12,96)	/* return device size /512 (long *arg) */
// /*AFLA*/ #define BLKFLSBUF  _IO(0x12,97)	/* flush buffer cache */
// /*AFLA*/ #define BLKRASET   _IO(0x12,98)	/* set read ahead for block device */
// /*AFLA*/ #define BLKRAGET   _IO(0x12,99)	/* get current read ahead setting */
// /*AFLA*/ #define BLKFRASET  _IO(0x12,100)/* set filesystem (mm/filemap.c) read-ahead */
// /*AFLA*/ #define BLKFRAGET  _IO(0x12,101)/* get filesystem (mm/filemap.c) read-ahead */
// /*AFLA*/ #define BLKSECTSET _IO(0x12,102)/* set max sectors per request (ll_rw_blk.c) */
// /*AFLA*/ #define BLKSECTGET _IO(0x12,103)/* get max sectors per request (ll_rw_blk.c) */
// /*AFLA*/ #define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
// /*AFLA*/ #if 0
// /*AFLA*/ #define BLKPG      _IO(0x12,105)/* See blkpg.h */
// /*AFLA*/ 
// /*AFLA*/ /* Some people are morons.  Do not use sizeof! */
// /*AFLA*/ 
// /*AFLA*/ #define BLKELVGET  _IOR(0x12,106,size_t)/* elevator get */
// /*AFLA*/ #define BLKELVSET  _IOW(0x12,107,size_t)/* elevator set */
// /*AFLA*/ /* This was here just to show that the number is taken -
// /*AFLA*/    probably all these _IO(0x12,*) ioctls should be moved to blkpg.h. */
// /*AFLA*/ #endif
// /*AFLA*/ /* A jump here: 108-111 have been used for various private purposes. */
// /*AFLA*/ #define BLKBSZGET  _IOR(0x12,112,size_t)
// /*AFLA*/ #define BLKBSZSET  _IOW(0x12,113,size_t)
// /*AFLA*/ #define BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
// /*AFLA*/ #define BLKTRACESETUP _IOWR(0x12,115,struct blk_user_trace_setup)
// /*AFLA*/ #define BLKTRACESTART _IO(0x12,116)
// /*AFLA*/ #define BLKTRACESTOP _IO(0x12,117)
// /*AFLA*/ #define BLKTRACETEARDOWN _IO(0x12,118)
// /*AFLA*/ #define BLKDISCARD _IO(0x12,119)
// /*AFLA*/ #define BLKIOMIN _IO(0x12,120)
// /*AFLA*/ #define BLKIOOPT _IO(0x12,121)
// /*AFLA*/ #define BLKALIGNOFF _IO(0x12,122)
// /*AFLA*/ #define BLKPBSZGET _IO(0x12,123)
// /*AFLA*/ #define BLKDISCARDZEROES _IO(0x12,124)
// /*AFLA*/ #define BLKSECDISCARD _IO(0x12,125)
// /*AFLA*/ #define BLKROTATIONAL _IO(0x12,126)
// /*AFLA*/ #define BLKZEROOUT _IO(0x12,127)
// /*AFLA*/ 
// /*AFLA*/ #define BMAP_IOCTL 1		/* obsolete - kept for compatibility */
// /*AFLA*/ #define FIBMAP	   _IO(0x00,1)	/* bmap access */
// /*AFLA*/ #define FIGETBSZ   _IO(0x00,2)	/* get the block size used for bmap */
// /*AFLA*/ #define FIFREEZE	_IOWR('X', 119, int)	/* Freeze */
// /*AFLA*/ #define FITHAW		_IOWR('X', 120, int)	/* Thaw */
// /*AFLA*/ #define FITRIM		_IOWR('X', 121, struct fstrim_range)	/* Trim */
// /*AFLA*/ #define FICLONE		_IOW(0x94, 9, int)
// /*AFLA*/ #define FICLONERANGE	_IOW(0x94, 13, struct file_clone_range)
// /*AFLA*/ #define FIDEDUPERANGE	_IOWR(0x94, 54, struct file_dedupe_range)
// /*AFLA*/ 
// /*AFLA*/ #define	FS_IOC_GETFLAGS			_IOR('f', 1, long)
// /*AFLA*/ #define	FS_IOC_SETFLAGS			_IOW('f', 2, long)
// /*AFLA*/ #define	FS_IOC_GETVERSION		_IOR('v', 1, long)
// /*AFLA*/ #define	FS_IOC_SETVERSION		_IOW('v', 2, long)
// /*AFLA*/ #define FS_IOC_FIEMAP			_IOWR('f', 11, struct fiemap)
// /*AFLA*/ #define FS_IOC32_GETFLAGS		_IOR('f', 1, int)
// /*AFLA*/ #define FS_IOC32_SETFLAGS		_IOW('f', 2, int)
// /*AFLA*/ #define FS_IOC32_GETVERSION		_IOR('v', 1, int)
// /*AFLA*/ #define FS_IOC32_SETVERSION		_IOW('v', 2, int)
// /*AFLA*/ #define FS_IOC_FSGETXATTR		_IOR ('X', 31, struct fsxattr)
// /*AFLA*/ #define FS_IOC_FSSETXATTR		_IOW ('X', 32, struct fsxattr)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * File system encryption support
// /*AFLA*/  */
// /*AFLA*/ /* Policy provided via an ioctl on the topmost directory */
// /*AFLA*/ #define FS_KEY_DESCRIPTOR_SIZE	8
// /*AFLA*/ 
// /*AFLA*/ struct fscrypt_policy {
// /*AFLA*/ 	__u8 version;
// /*AFLA*/ 	__u8 contents_encryption_mode;
// /*AFLA*/ 	__u8 filenames_encryption_mode;
// /*AFLA*/ 	__u8 flags;
// /*AFLA*/ 	__u8 master_key_descriptor[FS_KEY_DESCRIPTOR_SIZE];
// /*AFLA*/ } __packed;
// /*AFLA*/ 
// /*AFLA*/ #define FS_IOC_SET_ENCRYPTION_POLICY	_IOR('f', 19, struct fscrypt_policy)
// /*AFLA*/ #define FS_IOC_GET_ENCRYPTION_PWSALT	_IOW('f', 20, __u8[16])
// /*AFLA*/ #define FS_IOC_GET_ENCRYPTION_POLICY	_IOW('f', 21, struct fscrypt_policy)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Inode flags (FS_IOC_GETFLAGS / FS_IOC_SETFLAGS)
// /*AFLA*/  *
// /*AFLA*/  * Note: for historical reasons, these flags were originally used and
// /*AFLA*/  * defined for use by ext2/ext3, and then other file systems started
// /*AFLA*/  * using these flags so they wouldn't need to write their own version
// /*AFLA*/  * of chattr/lsattr (which was shipped as part of e2fsprogs).  You
// /*AFLA*/  * should think twice before trying to use these flags in new
// /*AFLA*/  * contexts, or trying to assign these flags, since they are used both
// /*AFLA*/  * as the UAPI and the on-disk encoding for ext2/3/4.  Also, we are
// /*AFLA*/  * almost out of 32-bit flags.  :-)
// /*AFLA*/  *
// /*AFLA*/  * We have recently hoisted FS_IOC_FSGETXATTR / FS_IOC_FSSETXATTR from
// /*AFLA*/  * XFS to the generic FS level interface.  This uses a structure that
// /*AFLA*/  * has padding and hence has more room to grow, so it may be more
// /*AFLA*/  * appropriate for many new use cases.
// /*AFLA*/  *
// /*AFLA*/  * Please do not change these flags or interfaces before checking with
// /*AFLA*/  * linux-fsdevel@vger.kernel.org and linux-api@vger.kernel.org.
// /*AFLA*/  */
// /*AFLA*/ #define	FS_SECRM_FL			0x00000001 /* Secure deletion */
// /*AFLA*/ #define	FS_UNRM_FL			0x00000002 /* Undelete */
// /*AFLA*/ #define	FS_COMPR_FL			0x00000004 /* Compress file */
// /*AFLA*/ #define FS_SYNC_FL			0x00000008 /* Synchronous updates */
// /*AFLA*/ #define FS_IMMUTABLE_FL			0x00000010 /* Immutable file */
// /*AFLA*/ #define FS_APPEND_FL			0x00000020 /* writes to file may only append */
// /*AFLA*/ #define FS_NODUMP_FL			0x00000040 /* do not dump file */
// /*AFLA*/ #define FS_NOATIME_FL			0x00000080 /* do not update atime */
// /*AFLA*/ /* Reserved for compression usage... */
// /*AFLA*/ #define FS_DIRTY_FL			0x00000100
// /*AFLA*/ #define FS_COMPRBLK_FL			0x00000200 /* One or more compressed clusters */
// /*AFLA*/ #define FS_NOCOMP_FL			0x00000400 /* Don't compress */
// /*AFLA*/ /* End compression flags --- maybe not all used */
// /*AFLA*/ #define FS_ENCRYPT_FL			0x00000800 /* Encrypted file */
// /*AFLA*/ #define FS_BTREE_FL			0x00001000 /* btree format dir */
// /*AFLA*/ #define FS_INDEX_FL			0x00001000 /* hash-indexed directory */
// /*AFLA*/ #define FS_IMAGIC_FL			0x00002000 /* AFS directory */
// /*AFLA*/ #define FS_JOURNAL_DATA_FL		0x00004000 /* Reserved for ext3 */
// /*AFLA*/ #define FS_NOTAIL_FL			0x00008000 /* file tail should not be merged */
// /*AFLA*/ #define FS_DIRSYNC_FL			0x00010000 /* dirsync behaviour (directories only) */
// /*AFLA*/ #define FS_TOPDIR_FL			0x00020000 /* Top of directory hierarchies*/
// /*AFLA*/ #define FS_HUGE_FILE_FL			0x00040000 /* Reserved for ext4 */
// /*AFLA*/ #define FS_EXTENT_FL			0x00080000 /* Extents */
// /*AFLA*/ #define FS_EA_INODE_FL			0x00200000 /* Inode used for large EA */
// /*AFLA*/ #define FS_EOFBLOCKS_FL			0x00400000 /* Reserved for ext4 */
// /*AFLA*/ #define FS_NOCOW_FL			0x00800000 /* Do not cow file */
// /*AFLA*/ #define FS_INLINE_DATA_FL		0x10000000 /* Reserved for ext4 */
// /*AFLA*/ #define FS_PROJINHERIT_FL		0x20000000 /* Create with parents projid */
// /*AFLA*/ #define FS_RESERVED_FL			0x80000000 /* reserved for ext2 lib */
// /*AFLA*/ 
// /*AFLA*/ #define FS_FL_USER_VISIBLE		0x0003DFFF /* User visible flags */
// /*AFLA*/ #define FS_FL_USER_MODIFIABLE		0x000380FF /* User modifiable flags */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define SYNC_FILE_RANGE_WAIT_BEFORE	1
// /*AFLA*/ #define SYNC_FILE_RANGE_WRITE		2
// /*AFLA*/ #define SYNC_FILE_RANGE_WAIT_AFTER	4
// /*AFLA*/ 
// /*AFLA*/ /* flags for preadv2/pwritev2: */
// /*AFLA*/ #define RWF_HIPRI			0x00000001 /* high priority request, poll if possible */
// /*AFLA*/ #define RWF_DSYNC			0x00000002 /* per-IO O_DSYNC */
// /*AFLA*/ #define RWF_SYNC			0x00000004 /* per-IO O_SYNC */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_FS_H */
