// /*AFLA*/ /*
// /*AFLA*/  * Copyright (c) 1995-2001,2004 Silicon Graphics, Inc.  All Rights Reserved.
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or
// /*AFLA*/  * modify it under the terms of the GNU Lesser General Public License
// /*AFLA*/  * as published by the Free Software Foundation.
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it will be useful,
// /*AFLA*/  * but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/  * GNU Lesser General Public License for more details.
// /*AFLA*/  *
// /*AFLA*/  * You should have received a copy of the GNU Lesset General Public License
// /*AFLA*/  * along with this program; if not, write to the Free Software Foundation,
// /*AFLA*/  * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LINUX_DQBLK_XFS_H
// /*AFLA*/ #define _LINUX_DQBLK_XFS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Disk quota - quotactl(2) commands for the XFS Quota Manager (XQM).
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define XQM_CMD(x)	(('X'<<8)+(x))	/* note: forms first QCMD argument */
// /*AFLA*/ #define XQM_COMMAND(x)	(((x) & (0xff<<8)) == ('X'<<8))	/* test if for XFS */
// /*AFLA*/ 
// /*AFLA*/ #define XQM_USRQUOTA	0	/* system call user quota type */
// /*AFLA*/ #define XQM_GRPQUOTA	1	/* system call group quota type */
// /*AFLA*/ #define XQM_PRJQUOTA	2	/* system call project quota type */
// /*AFLA*/ #define XQM_MAXQUOTAS	3
// /*AFLA*/ 
// /*AFLA*/ #define Q_XQUOTAON	XQM_CMD(1)	/* enable accounting/enforcement */
// /*AFLA*/ #define Q_XQUOTAOFF	XQM_CMD(2)	/* disable accounting/enforcement */
// /*AFLA*/ #define Q_XGETQUOTA	XQM_CMD(3)	/* get disk limits and usage */
// /*AFLA*/ #define Q_XSETQLIM	XQM_CMD(4)	/* set disk limits */
// /*AFLA*/ #define Q_XGETQSTAT	XQM_CMD(5)	/* get quota subsystem status */
// /*AFLA*/ #define Q_XQUOTARM	XQM_CMD(6)	/* free disk space used by dquots */
// /*AFLA*/ #define Q_XQUOTASYNC	XQM_CMD(7)	/* delalloc flush, updates dquots */
// /*AFLA*/ #define Q_XGETQSTATV	XQM_CMD(8)	/* newer version of get quota */
// /*AFLA*/ #define Q_XGETNEXTQUOTA	XQM_CMD(9)	/* get disk limits and usage >= ID */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * fs_disk_quota structure:
// /*AFLA*/  *
// /*AFLA*/  * This contains the current quota information regarding a user/proj/group.
// /*AFLA*/  * It is 64-bit aligned, and all the blk units are in BBs (Basic Blocks) of
// /*AFLA*/  * 512 bytes.
// /*AFLA*/  */
// /*AFLA*/ #define FS_DQUOT_VERSION	1	/* fs_disk_quota.d_version */
// /*AFLA*/ typedef struct fs_disk_quota {
// /*AFLA*/ 	__s8		d_version;	/* version of this structure */
// /*AFLA*/ 	__s8		d_flags;	/* FS_{USER,PROJ,GROUP}_QUOTA */
// /*AFLA*/ 	__u16		d_fieldmask;	/* field specifier */
// /*AFLA*/ 	__u32		d_id;		/* user, project, or group ID */
// /*AFLA*/ 	__u64		d_blk_hardlimit;/* absolute limit on disk blks */
// /*AFLA*/ 	__u64		d_blk_softlimit;/* preferred limit on disk blks */
// /*AFLA*/ 	__u64		d_ino_hardlimit;/* maximum # allocated inodes */
// /*AFLA*/ 	__u64		d_ino_softlimit;/* preferred inode limit */
// /*AFLA*/ 	__u64		d_bcount;	/* # disk blocks owned by the user */
// /*AFLA*/ 	__u64		d_icount;	/* # inodes owned by the user */
// /*AFLA*/ 	__s32		d_itimer;	/* zero if within inode limits */
// /*AFLA*/ 					/* if not, we refuse service */
// /*AFLA*/ 	__s32		d_btimer;	/* similar to above; for disk blocks */
// /*AFLA*/ 	__u16	  	d_iwarns;       /* # warnings issued wrt num inodes */
// /*AFLA*/ 	__u16	  	d_bwarns;       /* # warnings issued wrt disk blocks */
// /*AFLA*/ 	__s32		d_padding2;	/* padding2 - for future use */
// /*AFLA*/ 	__u64		d_rtb_hardlimit;/* absolute limit on realtime blks */
// /*AFLA*/ 	__u64		d_rtb_softlimit;/* preferred limit on RT disk blks */
// /*AFLA*/ 	__u64		d_rtbcount;	/* # realtime blocks owned */
// /*AFLA*/ 	__s32		d_rtbtimer;	/* similar to above; for RT disk blks */
// /*AFLA*/ 	__u16	  	d_rtbwarns;     /* # warnings issued wrt RT disk blks */
// /*AFLA*/ 	__s16		d_padding3;	/* padding3 - for future use */	
// /*AFLA*/ 	char		d_padding4[8];	/* yet more padding */
// /*AFLA*/ } fs_disk_quota_t;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These fields are sent to Q_XSETQLIM to specify fields that need to change.
// /*AFLA*/  */
// /*AFLA*/ #define FS_DQ_ISOFT	(1<<0)
// /*AFLA*/ #define FS_DQ_IHARD	(1<<1)
// /*AFLA*/ #define FS_DQ_BSOFT	(1<<2)
// /*AFLA*/ #define FS_DQ_BHARD 	(1<<3)
// /*AFLA*/ #define FS_DQ_RTBSOFT	(1<<4)
// /*AFLA*/ #define FS_DQ_RTBHARD	(1<<5)
// /*AFLA*/ #define FS_DQ_LIMIT_MASK	(FS_DQ_ISOFT | FS_DQ_IHARD | FS_DQ_BSOFT | \
// /*AFLA*/ 				 FS_DQ_BHARD | FS_DQ_RTBSOFT | FS_DQ_RTBHARD)
// /*AFLA*/ /*
// /*AFLA*/  * These timers can only be set in super user's dquot. For others, timers are
// /*AFLA*/  * automatically started and stopped. Superusers timer values set the limits
// /*AFLA*/  * for the rest.  In case these values are zero, the DQ_{F,B}TIMELIMIT values
// /*AFLA*/  * defined below are used. 
// /*AFLA*/  * These values also apply only to the d_fieldmask field for Q_XSETQLIM.
// /*AFLA*/  */
// /*AFLA*/ #define FS_DQ_BTIMER	(1<<6)
// /*AFLA*/ #define FS_DQ_ITIMER	(1<<7)
// /*AFLA*/ #define FS_DQ_RTBTIMER 	(1<<8)
// /*AFLA*/ #define FS_DQ_TIMER_MASK	(FS_DQ_BTIMER | FS_DQ_ITIMER | FS_DQ_RTBTIMER)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Warning counts are set in both super user's dquot and others. For others,
// /*AFLA*/  * warnings are set/cleared by the administrators (or automatically by going
// /*AFLA*/  * below the soft limit).  Superusers warning values set the warning limits
// /*AFLA*/  * for the rest.  In case these values are zero, the DQ_{F,B}WARNLIMIT values
// /*AFLA*/  * defined below are used. 
// /*AFLA*/  * These values also apply only to the d_fieldmask field for Q_XSETQLIM.
// /*AFLA*/  */
// /*AFLA*/ #define FS_DQ_BWARNS	(1<<9)
// /*AFLA*/ #define FS_DQ_IWARNS	(1<<10)
// /*AFLA*/ #define FS_DQ_RTBWARNS	(1<<11)
// /*AFLA*/ #define FS_DQ_WARNS_MASK	(FS_DQ_BWARNS | FS_DQ_IWARNS | FS_DQ_RTBWARNS)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Accounting values.  These can only be set for filesystem with
// /*AFLA*/  * non-transactional quotas that require quotacheck(8) in userspace.
// /*AFLA*/  */
// /*AFLA*/ #define FS_DQ_BCOUNT		(1<<12)
// /*AFLA*/ #define FS_DQ_ICOUNT		(1<<13)
// /*AFLA*/ #define FS_DQ_RTBCOUNT		(1<<14)
// /*AFLA*/ #define FS_DQ_ACCT_MASK		(FS_DQ_BCOUNT | FS_DQ_ICOUNT | FS_DQ_RTBCOUNT)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Various flags related to quotactl(2).
// /*AFLA*/  */
// /*AFLA*/ #define FS_QUOTA_UDQ_ACCT	(1<<0)  /* user quota accounting */
// /*AFLA*/ #define FS_QUOTA_UDQ_ENFD	(1<<1)  /* user quota limits enforcement */
// /*AFLA*/ #define FS_QUOTA_GDQ_ACCT	(1<<2)  /* group quota accounting */
// /*AFLA*/ #define FS_QUOTA_GDQ_ENFD	(1<<3)  /* group quota limits enforcement */
// /*AFLA*/ #define FS_QUOTA_PDQ_ACCT	(1<<4)  /* project quota accounting */
// /*AFLA*/ #define FS_QUOTA_PDQ_ENFD	(1<<5)  /* project quota limits enforcement */
// /*AFLA*/ 
// /*AFLA*/ #define FS_USER_QUOTA		(1<<0)	/* user quota type */
// /*AFLA*/ #define FS_PROJ_QUOTA		(1<<1)	/* project quota type */
// /*AFLA*/ #define FS_GROUP_QUOTA		(1<<2)	/* group quota type */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * fs_quota_stat is the struct returned in Q_XGETQSTAT for a given file system.
// /*AFLA*/  * Provides a centralized way to get meta information about the quota subsystem.
// /*AFLA*/  * eg. space taken up for user and group quotas, number of dquots currently
// /*AFLA*/  * incore.
// /*AFLA*/  */
// /*AFLA*/ #define FS_QSTAT_VERSION	1	/* fs_quota_stat.qs_version */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Some basic information about 'quota files'.
// /*AFLA*/  */
// /*AFLA*/ typedef struct fs_qfilestat {
// /*AFLA*/ 	__u64		qfs_ino;	/* inode number */
// /*AFLA*/ 	__u64		qfs_nblks;	/* number of BBs 512-byte-blks */
// /*AFLA*/ 	__u32		qfs_nextents;	/* number of extents */
// /*AFLA*/ } fs_qfilestat_t;
// /*AFLA*/ 
// /*AFLA*/ typedef struct fs_quota_stat {
// /*AFLA*/ 	__s8		qs_version;	/* version number for future changes */
// /*AFLA*/ 	__u16		qs_flags;	/* FS_QUOTA_{U,P,G}DQ_{ACCT,ENFD} */
// /*AFLA*/ 	__s8		qs_pad;		/* unused */
// /*AFLA*/ 	fs_qfilestat_t	qs_uquota;	/* user quota storage information */
// /*AFLA*/ 	fs_qfilestat_t	qs_gquota;	/* group quota storage information */
// /*AFLA*/ 	__u32		qs_incoredqs;	/* number of dquots incore */
// /*AFLA*/ 	__s32		qs_btimelimit;  /* limit for blks timer */	
// /*AFLA*/ 	__s32		qs_itimelimit;  /* limit for inodes timer */	
// /*AFLA*/ 	__s32		qs_rtbtimelimit;/* limit for rt blks timer */	
// /*AFLA*/ 	__u16		qs_bwarnlimit;	/* limit for num warnings */
// /*AFLA*/ 	__u16		qs_iwarnlimit;	/* limit for num warnings */
// /*AFLA*/ } fs_quota_stat_t;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * fs_quota_statv is used by Q_XGETQSTATV for a given file system. It provides
// /*AFLA*/  * a centralized way to get meta information about the quota subsystem. eg.
// /*AFLA*/  * space taken up for user, group, and project quotas, number of dquots
// /*AFLA*/  * currently incore.
// /*AFLA*/  *
// /*AFLA*/  * This version has proper versioning support with appropriate padding for
// /*AFLA*/  * future expansions, and ability to expand for future without creating any
// /*AFLA*/  * backward compatibility issues.
// /*AFLA*/  *
// /*AFLA*/  * Q_XGETQSTATV uses the passed in value of the requested version via
// /*AFLA*/  * fs_quota_statv.qs_version to determine the return data layout of
// /*AFLA*/  * fs_quota_statv.  The kernel will fill the data fields relevant to that
// /*AFLA*/  * version.
// /*AFLA*/  *
// /*AFLA*/  * If kernel does not support user space caller specified version, EINVAL will
// /*AFLA*/  * be returned. User space caller can then reduce the version number and retry
// /*AFLA*/  * the same command.
// /*AFLA*/  */
// /*AFLA*/ #define FS_QSTATV_VERSION1	1	/* fs_quota_statv.qs_version */
// /*AFLA*/ /*
// /*AFLA*/  * Some basic information about 'quota files' for Q_XGETQSTATV command
// /*AFLA*/  */
// /*AFLA*/ struct fs_qfilestatv {
// /*AFLA*/ 	__u64		qfs_ino;	/* inode number */
// /*AFLA*/ 	__u64		qfs_nblks;	/* number of BBs 512-byte-blks */
// /*AFLA*/ 	__u32		qfs_nextents;	/* number of extents */
// /*AFLA*/ 	__u32		qfs_pad;	/* pad for 8-byte alignment */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct fs_quota_statv {
// /*AFLA*/ 	__s8			qs_version;	/* version for future changes */
// /*AFLA*/ 	__u8			qs_pad1;	/* pad for 16bit alignment */
// /*AFLA*/ 	__u16			qs_flags;	/* FS_QUOTA_.* flags */
// /*AFLA*/ 	__u32			qs_incoredqs;	/* number of dquots incore */
// /*AFLA*/ 	struct fs_qfilestatv	qs_uquota;	/* user quota information */
// /*AFLA*/ 	struct fs_qfilestatv	qs_gquota;	/* group quota information */
// /*AFLA*/ 	struct fs_qfilestatv	qs_pquota;	/* project quota information */
// /*AFLA*/ 	__s32			qs_btimelimit;  /* limit for blks timer */
// /*AFLA*/ 	__s32			qs_itimelimit;  /* limit for inodes timer */
// /*AFLA*/ 	__s32			qs_rtbtimelimit;/* limit for rt blks timer */
// /*AFLA*/ 	__u16			qs_bwarnlimit;	/* limit for num warnings */
// /*AFLA*/ 	__u16			qs_iwarnlimit;	/* limit for num warnings */
// /*AFLA*/ 	__u64			qs_pad2[8];	/* for future proofing */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif	/* _LINUX_DQBLK_XFS_H */
