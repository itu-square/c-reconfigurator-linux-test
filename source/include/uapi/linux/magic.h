// /*AFLA*/ #ifndef __LINUX_MAGIC_H__
// /*AFLA*/ #define __LINUX_MAGIC_H__
// /*AFLA*/ 
// /*AFLA*/ #define ADFS_SUPER_MAGIC	0xadf5
// /*AFLA*/ #define AFFS_SUPER_MAGIC	0xadff
// /*AFLA*/ #define AFS_SUPER_MAGIC                0x5346414F
// /*AFLA*/ #define AUTOFS_SUPER_MAGIC	0x0187
// /*AFLA*/ #define CODA_SUPER_MAGIC	0x73757245
// /*AFLA*/ #define CRAMFS_MAGIC		0x28cd3d45	/* some random number */
// /*AFLA*/ #define CRAMFS_MAGIC_WEND	0x453dcd28	/* magic number with the wrong endianess */
// /*AFLA*/ #define DEBUGFS_MAGIC          0x64626720
// /*AFLA*/ #define SECURITYFS_MAGIC	0x73636673
// /*AFLA*/ #define SELINUX_MAGIC		0xf97cff8c
// /*AFLA*/ #define SMACK_MAGIC		0x43415d53	/* "SMAC" */
// /*AFLA*/ #define RAMFS_MAGIC		0x858458f6	/* some random number */
// /*AFLA*/ #define TMPFS_MAGIC		0x01021994
// /*AFLA*/ #define HUGETLBFS_MAGIC 	0x958458f6	/* some random number */
// /*AFLA*/ #define SQUASHFS_MAGIC		0x73717368
// /*AFLA*/ #define ECRYPTFS_SUPER_MAGIC	0xf15f
// /*AFLA*/ #define EFS_SUPER_MAGIC		0x414A53
// /*AFLA*/ #define EXT2_SUPER_MAGIC	0xEF53
// /*AFLA*/ #define EXT3_SUPER_MAGIC	0xEF53
// /*AFLA*/ #define XENFS_SUPER_MAGIC	0xabba1974
// /*AFLA*/ #define EXT4_SUPER_MAGIC	0xEF53
// /*AFLA*/ #define BTRFS_SUPER_MAGIC	0x9123683E
// /*AFLA*/ #define NILFS_SUPER_MAGIC	0x3434
// /*AFLA*/ #define F2FS_SUPER_MAGIC	0xF2F52010
// /*AFLA*/ #define HPFS_SUPER_MAGIC	0xf995e849
// /*AFLA*/ #define ISOFS_SUPER_MAGIC	0x9660
// /*AFLA*/ #define JFFS2_SUPER_MAGIC	0x72b6
// /*AFLA*/ #define PSTOREFS_MAGIC		0x6165676C
// /*AFLA*/ #define EFIVARFS_MAGIC		0xde5e81e4
// /*AFLA*/ #define HOSTFS_SUPER_MAGIC	0x00c0ffee
// /*AFLA*/ #define OVERLAYFS_SUPER_MAGIC	0x794c7630
// /*AFLA*/ 
// /*AFLA*/ #define MINIX_SUPER_MAGIC	0x137F		/* minix v1 fs, 14 char names */
// /*AFLA*/ #define MINIX_SUPER_MAGIC2	0x138F		/* minix v1 fs, 30 char names */
// /*AFLA*/ #define MINIX2_SUPER_MAGIC	0x2468		/* minix v2 fs, 14 char names */
// /*AFLA*/ #define MINIX2_SUPER_MAGIC2	0x2478		/* minix v2 fs, 30 char names */
// /*AFLA*/ #define MINIX3_SUPER_MAGIC	0x4d5a		/* minix v3 fs, 60 char names */
// /*AFLA*/ 
// /*AFLA*/ #define MSDOS_SUPER_MAGIC	0x4d44		/* MD */
// /*AFLA*/ #define NCP_SUPER_MAGIC		0x564c		/* Guess, what 0x564c is :-) */
// /*AFLA*/ #define NFS_SUPER_MAGIC		0x6969
// /*AFLA*/ #define OPENPROM_SUPER_MAGIC	0x9fa1
// /*AFLA*/ #define QNX4_SUPER_MAGIC	0x002f		/* qnx4 fs detection */
// /*AFLA*/ #define QNX6_SUPER_MAGIC	0x68191122	/* qnx6 fs detection */
// /*AFLA*/ 
// /*AFLA*/ #define REISERFS_SUPER_MAGIC	0x52654973	/* used by gcc */
// /*AFLA*/ 					/* used by file system utilities that
// /*AFLA*/ 	                                   look at the superblock, etc.  */
// /*AFLA*/ #define REISERFS_SUPER_MAGIC_STRING	"ReIsErFs"
// /*AFLA*/ #define REISER2FS_SUPER_MAGIC_STRING	"ReIsEr2Fs"
// /*AFLA*/ #define REISER2FS_JR_SUPER_MAGIC_STRING	"ReIsEr3Fs"
// /*AFLA*/ 
// /*AFLA*/ #define SMB_SUPER_MAGIC		0x517B
// /*AFLA*/ #define CGROUP_SUPER_MAGIC	0x27e0eb
// /*AFLA*/ #define CGROUP2_SUPER_MAGIC	0x63677270
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define STACK_END_MAGIC		0x57AC6E9D
// /*AFLA*/ 
// /*AFLA*/ #define TRACEFS_MAGIC          0x74726163
// /*AFLA*/ 
// /*AFLA*/ #define V9FS_MAGIC		0x01021997
// /*AFLA*/ 
// /*AFLA*/ #define BDEVFS_MAGIC            0x62646576
// /*AFLA*/ #define DAXFS_MAGIC             0x64646178
// /*AFLA*/ #define BINFMTFS_MAGIC          0x42494e4d
// /*AFLA*/ #define DEVPTS_SUPER_MAGIC	0x1cd1
// /*AFLA*/ #define FUTEXFS_SUPER_MAGIC	0xBAD1DEA
// /*AFLA*/ #define PIPEFS_MAGIC            0x50495045
// /*AFLA*/ #define PROC_SUPER_MAGIC	0x9fa0
// /*AFLA*/ #define SOCKFS_MAGIC		0x534F434B
// /*AFLA*/ #define SYSFS_MAGIC		0x62656572
// /*AFLA*/ #define USBDEVICE_SUPER_MAGIC	0x9fa2
// /*AFLA*/ #define MTD_INODE_FS_MAGIC      0x11307854
// /*AFLA*/ #define ANON_INODE_FS_MAGIC	0x09041934
// /*AFLA*/ #define BTRFS_TEST_MAGIC	0x73727279
// /*AFLA*/ #define NSFS_MAGIC		0x6e736673
// /*AFLA*/ #define BPF_FS_MAGIC		0xcafe4a11
// /*AFLA*/ /* Since UDF 2.01 is ISO 13346 based... */
// /*AFLA*/ #define UDF_SUPER_MAGIC		0x15013346
// /*AFLA*/ #define BALLOON_KVM_MAGIC	0x13661366
// /*AFLA*/ #define ZSMALLOC_MAGIC		0x58295829
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_MAGIC_H__ */
