// /*AFLA*/ /*
// /*AFLA*/  * Copyright (c) 1982, 1986 Regents of the University of California.
// /*AFLA*/  * All rights reserved.
// /*AFLA*/  *
// /*AFLA*/  * This code is derived from software contributed to Berkeley by
// /*AFLA*/  * Robert Elz at The University of Melbourne.
// /*AFLA*/  *
// /*AFLA*/  * Redistribution and use in source and binary forms, with or without
// /*AFLA*/  * modification, are permitted provided that the following conditions
// /*AFLA*/  * are met:
// /*AFLA*/  * 1. Redistributions of source code must retain the above copyright
// /*AFLA*/  *    notice, this list of conditions and the following disclaimer.
// /*AFLA*/  * 2. Redistributions in binary form must reproduce the above copyright
// /*AFLA*/  *    notice, this list of conditions and the following disclaimer in the
// /*AFLA*/  *    documentation and/or other materials provided with the distribution.
// /*AFLA*/  * 3. Neither the name of the University nor the names of its contributors
// /*AFLA*/  *    may be used to endorse or promote products derived from this software
// /*AFLA*/  *    without specific prior written permission.
// /*AFLA*/  *
// /*AFLA*/  * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
// /*AFLA*/  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// /*AFLA*/  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// /*AFLA*/  * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
// /*AFLA*/  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// /*AFLA*/  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// /*AFLA*/  * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// /*AFLA*/  * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// /*AFLA*/  * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// /*AFLA*/  * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// /*AFLA*/  * SUCH DAMAGE.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _UAPI_LINUX_QUOTA_
// /*AFLA*/ #define _UAPI_LINUX_QUOTA_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ #define __DQUOT_VERSION__	"dquot_6.6.0"
// /*AFLA*/ 
// /*AFLA*/ #define MAXQUOTAS 3
// /*AFLA*/ #define USRQUOTA  0		/* element used for user quotas */
// /*AFLA*/ #define GRPQUOTA  1		/* element used for group quotas */
// /*AFLA*/ #define PRJQUOTA  2		/* element used for project quotas */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Definitions for the default names of the quotas files.
// /*AFLA*/  */
// /*AFLA*/ #define INITQFNAMES { \
// /*AFLA*/ 	"user",    /* USRQUOTA */ \
// /*AFLA*/ 	"group",   /* GRPQUOTA */ \
// /*AFLA*/ 	"project", /* PRJQUOTA */ \
// /*AFLA*/ 	"undefined", \
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Command definitions for the 'quotactl' system call.
// /*AFLA*/  * The commands are broken into a main command defined below
// /*AFLA*/  * and a subcommand that is used to convey the type of
// /*AFLA*/  * quota that is being manipulated (see above).
// /*AFLA*/  */
// /*AFLA*/ #define SUBCMDMASK  0x00ff
// /*AFLA*/ #define SUBCMDSHIFT 8
// /*AFLA*/ #define QCMD(cmd, type)  (((cmd) << SUBCMDSHIFT) | ((type) & SUBCMDMASK))
// /*AFLA*/ 
// /*AFLA*/ #define Q_SYNC     0x800001	/* sync disk copy of a filesystems quotas */
// /*AFLA*/ #define Q_QUOTAON  0x800002	/* turn quotas on */
// /*AFLA*/ #define Q_QUOTAOFF 0x800003	/* turn quotas off */
// /*AFLA*/ #define Q_GETFMT   0x800004	/* get quota format used on given filesystem */
// /*AFLA*/ #define Q_GETINFO  0x800005	/* get information about quota files */
// /*AFLA*/ #define Q_SETINFO  0x800006	/* set information about quota files */
// /*AFLA*/ #define Q_GETQUOTA 0x800007	/* get user quota structure */
// /*AFLA*/ #define Q_SETQUOTA 0x800008	/* set user quota structure */
// /*AFLA*/ #define Q_GETNEXTQUOTA 0x800009	/* get disk limits and usage >= ID */
// /*AFLA*/ 
// /*AFLA*/ /* Quota format type IDs */
// /*AFLA*/ #define	QFMT_VFS_OLD 1
// /*AFLA*/ #define	QFMT_VFS_V0 2
// /*AFLA*/ #define QFMT_OCFS2 3
// /*AFLA*/ #define	QFMT_VFS_V1 4
// /*AFLA*/ 
// /*AFLA*/ /* Size of block in which space limits are passed through the quota
// /*AFLA*/  * interface */
// /*AFLA*/ #define QIF_DQBLKSIZE_BITS 10
// /*AFLA*/ #define QIF_DQBLKSIZE (1 << QIF_DQBLKSIZE_BITS)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Quota structure used for communication with userspace via quotactl
// /*AFLA*/  * Following flags are used to specify which fields are valid
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	QIF_BLIMITS_B = 0,
// /*AFLA*/ 	QIF_SPACE_B,
// /*AFLA*/ 	QIF_ILIMITS_B,
// /*AFLA*/ 	QIF_INODES_B,
// /*AFLA*/ 	QIF_BTIME_B,
// /*AFLA*/ 	QIF_ITIME_B,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define QIF_BLIMITS	(1 << QIF_BLIMITS_B)
// /*AFLA*/ #define QIF_SPACE	(1 << QIF_SPACE_B)
// /*AFLA*/ #define QIF_ILIMITS	(1 << QIF_ILIMITS_B)
// /*AFLA*/ #define QIF_INODES	(1 << QIF_INODES_B)
// /*AFLA*/ #define QIF_BTIME	(1 << QIF_BTIME_B)
// /*AFLA*/ #define QIF_ITIME	(1 << QIF_ITIME_B)
// /*AFLA*/ #define QIF_LIMITS	(QIF_BLIMITS | QIF_ILIMITS)
// /*AFLA*/ #define QIF_USAGE	(QIF_SPACE | QIF_INODES)
// /*AFLA*/ #define QIF_TIMES	(QIF_BTIME | QIF_ITIME)
// /*AFLA*/ #define QIF_ALL		(QIF_LIMITS | QIF_USAGE | QIF_TIMES)
// /*AFLA*/ 
// /*AFLA*/ struct if_dqblk {
// /*AFLA*/ 	__u64 dqb_bhardlimit;
// /*AFLA*/ 	__u64 dqb_bsoftlimit;
// /*AFLA*/ 	__u64 dqb_curspace;
// /*AFLA*/ 	__u64 dqb_ihardlimit;
// /*AFLA*/ 	__u64 dqb_isoftlimit;
// /*AFLA*/ 	__u64 dqb_curinodes;
// /*AFLA*/ 	__u64 dqb_btime;
// /*AFLA*/ 	__u64 dqb_itime;
// /*AFLA*/ 	__u32 dqb_valid;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct if_nextdqblk {
// /*AFLA*/ 	__u64 dqb_bhardlimit;
// /*AFLA*/ 	__u64 dqb_bsoftlimit;
// /*AFLA*/ 	__u64 dqb_curspace;
// /*AFLA*/ 	__u64 dqb_ihardlimit;
// /*AFLA*/ 	__u64 dqb_isoftlimit;
// /*AFLA*/ 	__u64 dqb_curinodes;
// /*AFLA*/ 	__u64 dqb_btime;
// /*AFLA*/ 	__u64 dqb_itime;
// /*AFLA*/ 	__u32 dqb_valid;
// /*AFLA*/ 	__u32 dqb_id;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Structure used for setting quota information about file via quotactl
// /*AFLA*/  * Following flags are used to specify which fields are valid
// /*AFLA*/  */
// /*AFLA*/ #define IIF_BGRACE	1
// /*AFLA*/ #define IIF_IGRACE	2
// /*AFLA*/ #define IIF_FLAGS	4
// /*AFLA*/ #define IIF_ALL		(IIF_BGRACE | IIF_IGRACE | IIF_FLAGS)
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	DQF_ROOT_SQUASH_B = 0,
// /*AFLA*/ 	DQF_SYS_FILE_B = 16,
// /*AFLA*/ 	/* Kernel internal flags invisible to userspace */
// /*AFLA*/ 	DQF_PRIVATE
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Root squash enabled (for v1 quota format) */
// /*AFLA*/ #define DQF_ROOT_SQUASH	(1 << DQF_ROOT_SQUASH_B)
// /*AFLA*/ /* Quota stored in a system file */
// /*AFLA*/ #define DQF_SYS_FILE	(1 << DQF_SYS_FILE_B)
// /*AFLA*/ 
// /*AFLA*/ struct if_dqinfo {
// /*AFLA*/ 	__u64 dqi_bgrace;
// /*AFLA*/ 	__u64 dqi_igrace;
// /*AFLA*/ 	__u32 dqi_flags;	/* DFQ_* */
// /*AFLA*/ 	__u32 dqi_valid;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Definitions for quota netlink interface
// /*AFLA*/  */
// /*AFLA*/ #define QUOTA_NL_NOWARN 0
// /*AFLA*/ #define QUOTA_NL_IHARDWARN 1		/* Inode hardlimit reached */
// /*AFLA*/ #define QUOTA_NL_ISOFTLONGWARN 2 	/* Inode grace time expired */
// /*AFLA*/ #define QUOTA_NL_ISOFTWARN 3		/* Inode softlimit reached */
// /*AFLA*/ #define QUOTA_NL_BHARDWARN 4		/* Block hardlimit reached */
// /*AFLA*/ #define QUOTA_NL_BSOFTLONGWARN 5	/* Block grace time expired */
// /*AFLA*/ #define QUOTA_NL_BSOFTWARN 6		/* Block softlimit reached */
// /*AFLA*/ #define QUOTA_NL_IHARDBELOW 7		/* Usage got below inode hardlimit */
// /*AFLA*/ #define QUOTA_NL_ISOFTBELOW 8		/* Usage got below inode softlimit */
// /*AFLA*/ #define QUOTA_NL_BHARDBELOW 9		/* Usage got below block hardlimit */
// /*AFLA*/ #define QUOTA_NL_BSOFTBELOW 10		/* Usage got below block softlimit */
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	QUOTA_NL_C_UNSPEC,
// /*AFLA*/ 	QUOTA_NL_C_WARNING,
// /*AFLA*/ 	__QUOTA_NL_C_MAX,
// /*AFLA*/ };
// /*AFLA*/ #define QUOTA_NL_C_MAX (__QUOTA_NL_C_MAX - 1)
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	QUOTA_NL_A_UNSPEC,
// /*AFLA*/ 	QUOTA_NL_A_QTYPE,
// /*AFLA*/ 	QUOTA_NL_A_EXCESS_ID,
// /*AFLA*/ 	QUOTA_NL_A_WARNING,
// /*AFLA*/ 	QUOTA_NL_A_DEV_MAJOR,
// /*AFLA*/ 	QUOTA_NL_A_DEV_MINOR,
// /*AFLA*/ 	QUOTA_NL_A_CAUSED_ID,
// /*AFLA*/ 	QUOTA_NL_A_PAD,
// /*AFLA*/ 	__QUOTA_NL_A_MAX,
// /*AFLA*/ };
// /*AFLA*/ #define QUOTA_NL_A_MAX (__QUOTA_NL_A_MAX - 1)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_QUOTA_ */
