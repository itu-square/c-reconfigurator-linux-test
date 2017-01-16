// /*AFLA*/ /*
// /*AFLA*/  * FS_IOC_FIEMAP ioctl infrastructure.
// /*AFLA*/  *
// /*AFLA*/  * Some portions copyright (C) 2007 Cluster File Systems, Inc
// /*AFLA*/  *
// /*AFLA*/  * Authors: Mark Fasheh <mfasheh@suse.com>
// /*AFLA*/  *          Kalpak Shah <kalpak.shah@sun.com>
// /*AFLA*/  *          Andreas Dilger <adilger@sun.com>
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_FIEMAP_H
// /*AFLA*/ #define _LINUX_FIEMAP_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ struct fiemap_extent {
// /*AFLA*/ 	__u64 fe_logical;  /* logical offset in bytes for the start of
// /*AFLA*/ 			    * the extent from the beginning of the file */
// /*AFLA*/ 	__u64 fe_physical; /* physical offset in bytes for the start
// /*AFLA*/ 			    * of the extent from the beginning of the disk */
// /*AFLA*/ 	__u64 fe_length;   /* length in bytes for this extent */
// /*AFLA*/ 	__u64 fe_reserved64[2];
// /*AFLA*/ 	__u32 fe_flags;    /* FIEMAP_EXTENT_* flags for this extent */
// /*AFLA*/ 	__u32 fe_reserved[3];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct fiemap {
// /*AFLA*/ 	__u64 fm_start;		/* logical offset (inclusive) at
// /*AFLA*/ 				 * which to start mapping (in) */
// /*AFLA*/ 	__u64 fm_length;	/* logical length of mapping which
// /*AFLA*/ 				 * userspace wants (in) */
// /*AFLA*/ 	__u32 fm_flags;		/* FIEMAP_FLAG_* flags for request (in/out) */
// /*AFLA*/ 	__u32 fm_mapped_extents;/* number of extents that were mapped (out) */
// /*AFLA*/ 	__u32 fm_extent_count;  /* size of fm_extents array (in) */
// /*AFLA*/ 	__u32 fm_reserved;
// /*AFLA*/ 	struct fiemap_extent fm_extents[0]; /* array of mapped extents (out) */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define FIEMAP_MAX_OFFSET	(~0ULL)
// /*AFLA*/ 
// /*AFLA*/ #define FIEMAP_FLAG_SYNC	0x00000001 /* sync file data before map */
// /*AFLA*/ #define FIEMAP_FLAG_XATTR	0x00000002 /* map extended attribute tree */
// /*AFLA*/ #define FIEMAP_FLAG_CACHE	0x00000004 /* request caching of the extents */
// /*AFLA*/ 
// /*AFLA*/ #define FIEMAP_FLAGS_COMPAT	(FIEMAP_FLAG_SYNC | FIEMAP_FLAG_XATTR)
// /*AFLA*/ 
// /*AFLA*/ #define FIEMAP_EXTENT_LAST		0x00000001 /* Last extent in file. */
// /*AFLA*/ #define FIEMAP_EXTENT_UNKNOWN		0x00000002 /* Data location unknown. */
// /*AFLA*/ #define FIEMAP_EXTENT_DELALLOC		0x00000004 /* Location still pending.
// /*AFLA*/ 						    * Sets EXTENT_UNKNOWN. */
// /*AFLA*/ #define FIEMAP_EXTENT_ENCODED		0x00000008 /* Data can not be read
// /*AFLA*/ 						    * while fs is unmounted */
// /*AFLA*/ #define FIEMAP_EXTENT_DATA_ENCRYPTED	0x00000080 /* Data is encrypted by fs.
// /*AFLA*/ 						    * Sets EXTENT_NO_BYPASS. */
// /*AFLA*/ #define FIEMAP_EXTENT_NOT_ALIGNED	0x00000100 /* Extent offsets may not be
// /*AFLA*/ 						    * block aligned. */
// /*AFLA*/ #define FIEMAP_EXTENT_DATA_INLINE	0x00000200 /* Data mixed with metadata.
// /*AFLA*/ 						    * Sets EXTENT_NOT_ALIGNED.*/
// /*AFLA*/ #define FIEMAP_EXTENT_DATA_TAIL		0x00000400 /* Multiple files in block.
// /*AFLA*/ 						    * Sets EXTENT_NOT_ALIGNED.*/
// /*AFLA*/ #define FIEMAP_EXTENT_UNWRITTEN		0x00000800 /* Space allocated, but
// /*AFLA*/ 						    * no data (i.e. zero). */
// /*AFLA*/ #define FIEMAP_EXTENT_MERGED		0x00001000 /* File does not natively
// /*AFLA*/ 						    * support extents. Result
// /*AFLA*/ 						    * merged for efficiency. */
// /*AFLA*/ #define FIEMAP_EXTENT_SHARED		0x00002000 /* Space shared with other
// /*AFLA*/ 						    * files. */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_FIEMAP_H */
