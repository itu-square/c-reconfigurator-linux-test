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
// /*AFLA*/ #ifndef _LINUX_QUOTA_
// /*AFLA*/ #define _LINUX_QUOTA_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/mutex.h>
// /*AFLA*/ #include <linux/rwsem.h>
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/wait.h>
// /*AFLA*/ #include <linux/percpu_counter.h>
// /*AFLA*/ 
// /*AFLA*/ #include <linux/dqblk_xfs.h>
// /*AFLA*/ #include <linux/dqblk_v1.h>
// /*AFLA*/ #include <linux/dqblk_v2.h>
// /*AFLA*/ 
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/uidgid.h>
// /*AFLA*/ #include <linux/projid.h>
// /*AFLA*/ #include <uapi/linux/quota.h>
// /*AFLA*/ 
// /*AFLA*/ #undef USRQUOTA
// /*AFLA*/ #undef GRPQUOTA
// /*AFLA*/ #undef PRJQUOTA
// /*AFLA*/ enum quota_type {
// /*AFLA*/ 	USRQUOTA = 0,		/* element used for user quotas */
// /*AFLA*/ 	GRPQUOTA = 1,		/* element used for group quotas */
// /*AFLA*/ 	PRJQUOTA = 2,		/* element used for project quotas */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Masks for quota types when used as a bitmask */
// /*AFLA*/ #define QTYPE_MASK_USR (1 << USRQUOTA)
// /*AFLA*/ #define QTYPE_MASK_GRP (1 << GRPQUOTA)
// /*AFLA*/ #define QTYPE_MASK_PRJ (1 << PRJQUOTA)
// /*AFLA*/ 
// /*AFLA*/ typedef __kernel_uid32_t qid_t; /* Type in which we store ids in memory */
// /*AFLA*/ typedef long long qsize_t;	/* Type in which we store sizes */
// /*AFLA*/ 
// /*AFLA*/ struct kqid {			/* Type in which we store the quota identifier */
// /*AFLA*/ 	union {
// /*AFLA*/ 		kuid_t uid;
// /*AFLA*/ 		kgid_t gid;
// /*AFLA*/ 		kprojid_t projid;
// /*AFLA*/ 	};
// /*AFLA*/ 	enum quota_type type;  /* USRQUOTA (uid) or GRPQUOTA (gid) or PRJQUOTA (projid) */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern bool qid_eq(struct kqid left, struct kqid right);
// /*AFLA*/ extern bool qid_lt(struct kqid left, struct kqid right);
// /*AFLA*/ extern qid_t from_kqid(struct user_namespace *to, struct kqid qid);
// /*AFLA*/ extern qid_t from_kqid_munged(struct user_namespace *to, struct kqid qid);
// /*AFLA*/ extern bool qid_valid(struct kqid qid);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  *	make_kqid - Map a user-namespace, type, qid tuple into a kqid.
// /*AFLA*/  *	@from: User namespace that the qid is in
// /*AFLA*/  *	@type: The type of quota
// /*AFLA*/  *	@qid: Quota identifier
// /*AFLA*/  *
// /*AFLA*/  *	Maps a user-namespace, type qid tuple into a kernel internal
// /*AFLA*/  *	kqid, and returns that kqid.
// /*AFLA*/  *
// /*AFLA*/  *	When there is no mapping defined for the user-namespace, type,
// /*AFLA*/  *	qid tuple an invalid kqid is returned.  Callers are expected to
// /*AFLA*/  *	test for and handle handle invalid kqids being returned.
// /*AFLA*/  *	Invalid kqids may be tested for using qid_valid().
// /*AFLA*/  */
// /*AFLA*/ static inline struct kqid make_kqid(struct user_namespace *from,
// /*AFLA*/ 				    enum quota_type type, qid_t qid)
// /*AFLA*/ {
// /*AFLA*/ 	struct kqid kqid;
// /*AFLA*/ 
// /*AFLA*/ 	kqid.type = type;
// /*AFLA*/ 	switch (type) {
// /*AFLA*/ 	case USRQUOTA:
// /*AFLA*/ 		kqid.uid = make_kuid(from, qid);
// /*AFLA*/ 		break;
// /*AFLA*/ 	case GRPQUOTA:
// /*AFLA*/ 		kqid.gid = make_kgid(from, qid);
// /*AFLA*/ 		break;
// /*AFLA*/ 	case PRJQUOTA:
// /*AFLA*/ 		kqid.projid = make_kprojid(from, qid);
// /*AFLA*/ 		break;
// /*AFLA*/ 	default:
// /*AFLA*/ 		BUG();
// /*AFLA*/ 	}
// /*AFLA*/ 	return kqid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  *	make_kqid_invalid - Explicitly make an invalid kqid
// /*AFLA*/  *	@type: The type of quota identifier
// /*AFLA*/  *
// /*AFLA*/  *	Returns an invalid kqid with the specified type.
// /*AFLA*/  */
// /*AFLA*/ static inline struct kqid make_kqid_invalid(enum quota_type type)
// /*AFLA*/ {
// /*AFLA*/ 	struct kqid kqid;
// /*AFLA*/ 
// /*AFLA*/ 	kqid.type = type;
// /*AFLA*/ 	switch (type) {
// /*AFLA*/ 	case USRQUOTA:
// /*AFLA*/ 		kqid.uid = INVALID_UID;
// /*AFLA*/ 		break;
// /*AFLA*/ 	case GRPQUOTA:
// /*AFLA*/ 		kqid.gid = INVALID_GID;
// /*AFLA*/ 		break;
// /*AFLA*/ 	case PRJQUOTA:
// /*AFLA*/ 		kqid.projid = INVALID_PROJID;
// /*AFLA*/ 		break;
// /*AFLA*/ 	default:
// /*AFLA*/ 		BUG();
// /*AFLA*/ 	}
// /*AFLA*/ 	return kqid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  *	make_kqid_uid - Make a kqid from a kuid
// /*AFLA*/  *	@uid: The kuid to make the quota identifier from
// /*AFLA*/  */
// /*AFLA*/ static inline struct kqid make_kqid_uid(kuid_t uid)
// /*AFLA*/ {
// /*AFLA*/ 	struct kqid kqid;
// /*AFLA*/ 	kqid.type = USRQUOTA;
// /*AFLA*/ 	kqid.uid = uid;
// /*AFLA*/ 	return kqid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  *	make_kqid_gid - Make a kqid from a kgid
// /*AFLA*/  *	@gid: The kgid to make the quota identifier from
// /*AFLA*/  */
// /*AFLA*/ static inline struct kqid make_kqid_gid(kgid_t gid)
// /*AFLA*/ {
// /*AFLA*/ 	struct kqid kqid;
// /*AFLA*/ 	kqid.type = GRPQUOTA;
// /*AFLA*/ 	kqid.gid = gid;
// /*AFLA*/ 	return kqid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  *	make_kqid_projid - Make a kqid from a projid
// /*AFLA*/  *	@projid: The kprojid to make the quota identifier from
// /*AFLA*/  */
// /*AFLA*/ static inline struct kqid make_kqid_projid(kprojid_t projid)
// /*AFLA*/ {
// /*AFLA*/ 	struct kqid kqid;
// /*AFLA*/ 	kqid.type = PRJQUOTA;
// /*AFLA*/ 	kqid.projid = projid;
// /*AFLA*/ 	return kqid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  *	qid_has_mapping - Report if a qid maps into a user namespace.
// /*AFLA*/  *	@ns:  The user namespace to see if a value maps into.
// /*AFLA*/  *	@qid: The kernel internal quota identifier to test.
// /*AFLA*/  */
// /*AFLA*/ static inline bool qid_has_mapping(struct user_namespace *ns, struct kqid qid)
// /*AFLA*/ {
// /*AFLA*/ 	return from_kqid(ns, qid) != (qid_t) -1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ extern spinlock_t dq_data_lock;
// /*AFLA*/ 
// /*AFLA*/ /* Maximal numbers of writes for quota operation (insert/delete/update)
// /*AFLA*/  * (over VFS all formats) */
// /*AFLA*/ #define DQUOT_INIT_ALLOC max(V1_INIT_ALLOC, V2_INIT_ALLOC)
// /*AFLA*/ #define DQUOT_INIT_REWRITE max(V1_INIT_REWRITE, V2_INIT_REWRITE)
// /*AFLA*/ #define DQUOT_DEL_ALLOC max(V1_DEL_ALLOC, V2_DEL_ALLOC)
// /*AFLA*/ #define DQUOT_DEL_REWRITE max(V1_DEL_REWRITE, V2_DEL_REWRITE)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Data for one user/group kept in memory
// /*AFLA*/  */
// /*AFLA*/ struct mem_dqblk {
// /*AFLA*/ 	qsize_t dqb_bhardlimit;	/* absolute limit on disk blks alloc */
// /*AFLA*/ 	qsize_t dqb_bsoftlimit;	/* preferred limit on disk blks */
// /*AFLA*/ 	qsize_t dqb_curspace;	/* current used space */
// /*AFLA*/ 	qsize_t dqb_rsvspace;   /* current reserved space for delalloc*/
// /*AFLA*/ 	qsize_t dqb_ihardlimit;	/* absolute limit on allocated inodes */
// /*AFLA*/ 	qsize_t dqb_isoftlimit;	/* preferred inode limit */
// /*AFLA*/ 	qsize_t dqb_curinodes;	/* current # allocated inodes */
// /*AFLA*/ 	time64_t dqb_btime;	/* time limit for excessive disk use */
// /*AFLA*/ 	time64_t dqb_itime;	/* time limit for excessive inode use */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Data for one quotafile kept in memory
// /*AFLA*/  */
// /*AFLA*/ struct quota_format_type;
// /*AFLA*/ 
// /*AFLA*/ struct mem_dqinfo {
// /*AFLA*/ 	struct quota_format_type *dqi_format;
// /*AFLA*/ 	int dqi_fmt_id;		/* Id of the dqi_format - used when turning
// /*AFLA*/ 				 * quotas on after remount RW */
// /*AFLA*/ 	struct list_head dqi_dirty_list;	/* List of dirty dquots */
// /*AFLA*/ 	unsigned long dqi_flags;
// /*AFLA*/ 	unsigned int dqi_bgrace;
// /*AFLA*/ 	unsigned int dqi_igrace;
// /*AFLA*/ 	qsize_t dqi_max_spc_limit;
// /*AFLA*/ 	qsize_t dqi_max_ino_limit;
// /*AFLA*/ 	void *dqi_priv;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct super_block;
// /*AFLA*/ 
// /*AFLA*/ /* Mask for flags passed to userspace */
// /*AFLA*/ #define DQF_GETINFO_MASK (DQF_ROOT_SQUASH | DQF_SYS_FILE)
// /*AFLA*/ /* Mask for flags modifiable from userspace */
// /*AFLA*/ #define DQF_SETINFO_MASK DQF_ROOT_SQUASH
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	DQF_INFO_DIRTY_B = DQF_PRIVATE,
// /*AFLA*/ };
// /*AFLA*/ #define DQF_INFO_DIRTY (1 << DQF_INFO_DIRTY_B)	/* Is info dirty? */
// /*AFLA*/ 
// /*AFLA*/ extern void mark_info_dirty(struct super_block *sb, int type);
// /*AFLA*/ static inline int info_dirty(struct mem_dqinfo *info)
// /*AFLA*/ {
// /*AFLA*/ 	return test_bit(DQF_INFO_DIRTY_B, &info->dqi_flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	DQST_LOOKUPS,
// /*AFLA*/ 	DQST_DROPS,
// /*AFLA*/ 	DQST_READS,
// /*AFLA*/ 	DQST_WRITES,
// /*AFLA*/ 	DQST_CACHE_HITS,
// /*AFLA*/ 	DQST_ALLOC_DQUOTS,
// /*AFLA*/ 	DQST_FREE_DQUOTS,
// /*AFLA*/ 	DQST_SYNCS,
// /*AFLA*/ 	_DQST_DQSTAT_LAST
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct dqstats {
// /*AFLA*/ 	int stat[_DQST_DQSTAT_LAST];
// /*AFLA*/ 	struct percpu_counter counter[_DQST_DQSTAT_LAST];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct dqstats *dqstats_pcpu;
// /*AFLA*/ extern struct dqstats dqstats;
// /*AFLA*/ 
// /*AFLA*/ static inline void dqstats_inc(unsigned int type)
// /*AFLA*/ {
// /*AFLA*/ 	percpu_counter_inc(&dqstats.counter[type]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void dqstats_dec(unsigned int type)
// /*AFLA*/ {
// /*AFLA*/ 	percpu_counter_dec(&dqstats.counter[type]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define DQ_MOD_B	0	/* dquot modified since read */
// /*AFLA*/ #define DQ_BLKS_B	1	/* uid/gid has been warned about blk limit */
// /*AFLA*/ #define DQ_INODES_B	2	/* uid/gid has been warned about inode limit */
// /*AFLA*/ #define DQ_FAKE_B	3	/* no limits only usage */
// /*AFLA*/ #define DQ_READ_B	4	/* dquot was read into memory */
// /*AFLA*/ #define DQ_ACTIVE_B	5	/* dquot is active (dquot_release not called) */
// /*AFLA*/ #define DQ_LASTSET_B	6	/* Following 6 bits (see QIF_) are reserved\
// /*AFLA*/ 				 * for the mask of entries set via SETQUOTA\
// /*AFLA*/ 				 * quotactl. They are set under dq_data_lock\
// /*AFLA*/ 				 * and the quota format handling dquot can\
// /*AFLA*/ 				 * clear them when it sees fit. */
// /*AFLA*/ 
// /*AFLA*/ struct dquot {
// /*AFLA*/ 	struct hlist_node dq_hash;	/* Hash list in memory */
// /*AFLA*/ 	struct list_head dq_inuse;	/* List of all quotas */
// /*AFLA*/ 	struct list_head dq_free;	/* Free list element */
// /*AFLA*/ 	struct list_head dq_dirty;	/* List of dirty dquots */
// /*AFLA*/ 	struct mutex dq_lock;		/* dquot IO lock */
// /*AFLA*/ 	atomic_t dq_count;		/* Use count */
// /*AFLA*/ 	wait_queue_head_t dq_wait_unused;	/* Wait queue for dquot to become unused */
// /*AFLA*/ 	struct super_block *dq_sb;	/* superblock this applies to */
// /*AFLA*/ 	struct kqid dq_id;		/* ID this applies to (uid, gid, projid) */
// /*AFLA*/ 	loff_t dq_off;			/* Offset of dquot on disk */
// /*AFLA*/ 	unsigned long dq_flags;		/* See DQ_* */
// /*AFLA*/ 	struct mem_dqblk dq_dqb;	/* Diskquota usage */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Operations which must be implemented by each quota format */
// /*AFLA*/ struct quota_format_ops {
// /*AFLA*/ 	int (*check_quota_file)(struct super_block *sb, int type);	/* Detect whether file is in our format */
// /*AFLA*/ 	int (*read_file_info)(struct super_block *sb, int type);	/* Read main info about file - called on quotaon() */
// /*AFLA*/ 	int (*write_file_info)(struct super_block *sb, int type);	/* Write main info about file */
// /*AFLA*/ 	int (*free_file_info)(struct super_block *sb, int type);	/* Called on quotaoff() */
// /*AFLA*/ 	int (*read_dqblk)(struct dquot *dquot);		/* Read structure for one user */
// /*AFLA*/ 	int (*commit_dqblk)(struct dquot *dquot);	/* Write structure for one user */
// /*AFLA*/ 	int (*release_dqblk)(struct dquot *dquot);	/* Called when last reference to dquot is being dropped */
// /*AFLA*/ 	int (*get_next_id)(struct super_block *sb, struct kqid *qid);	/* Get next ID with existing structure in the quota file */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Operations working with dquots */
// /*AFLA*/ struct dquot_operations {
// /*AFLA*/ 	int (*write_dquot) (struct dquot *);		/* Ordinary dquot write */
// /*AFLA*/ 	struct dquot *(*alloc_dquot)(struct super_block *, int);	/* Allocate memory for new dquot */
// /*AFLA*/ 	void (*destroy_dquot)(struct dquot *);		/* Free memory for dquot */
// /*AFLA*/ 	int (*acquire_dquot) (struct dquot *);		/* Quota is going to be created on disk */
// /*AFLA*/ 	int (*release_dquot) (struct dquot *);		/* Quota is going to be deleted from disk */
// /*AFLA*/ 	int (*mark_dirty) (struct dquot *);		/* Dquot is marked dirty */
// /*AFLA*/ 	int (*write_info) (struct super_block *, int);	/* Write of quota "superblock" */
// /*AFLA*/ 	/* get reserved quota for delayed alloc, value returned is managed by
// /*AFLA*/ 	 * quota code only */
// /*AFLA*/ 	qsize_t *(*get_reserved_space) (struct inode *);
// /*AFLA*/ 	int (*get_projid) (struct inode *, kprojid_t *);/* Get project ID */
// /*AFLA*/ 	/* Get next ID with active quota structure */
// /*AFLA*/ 	int (*get_next_id) (struct super_block *sb, struct kqid *qid);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct path;
// /*AFLA*/ 
// /*AFLA*/ /* Structure for communicating via ->get_dqblk() & ->set_dqblk() */
// /*AFLA*/ struct qc_dqblk {
// /*AFLA*/ 	int d_fieldmask;	/* mask of fields to change in ->set_dqblk() */
// /*AFLA*/ 	u64 d_spc_hardlimit;	/* absolute limit on used space */
// /*AFLA*/ 	u64 d_spc_softlimit;	/* preferred limit on used space */
// /*AFLA*/ 	u64 d_ino_hardlimit;	/* maximum # allocated inodes */
// /*AFLA*/ 	u64 d_ino_softlimit;	/* preferred inode limit */
// /*AFLA*/ 	u64 d_space;		/* Space owned by the user */
// /*AFLA*/ 	u64 d_ino_count;	/* # inodes owned by the user */
// /*AFLA*/ 	s64 d_ino_timer;	/* zero if within inode limits */
// /*AFLA*/ 				/* if not, we refuse service */
// /*AFLA*/ 	s64 d_spc_timer;	/* similar to above; for space */
// /*AFLA*/ 	int d_ino_warns;	/* # warnings issued wrt num inodes */
// /*AFLA*/ 	int d_spc_warns;	/* # warnings issued wrt used space */
// /*AFLA*/ 	u64 d_rt_spc_hardlimit;	/* absolute limit on realtime space */
// /*AFLA*/ 	u64 d_rt_spc_softlimit;	/* preferred limit on RT space */
// /*AFLA*/ 	u64 d_rt_space;		/* realtime space owned */
// /*AFLA*/ 	s64 d_rt_spc_timer;	/* similar to above; for RT space */
// /*AFLA*/ 	int d_rt_spc_warns;	/* # warnings issued wrt RT space */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Field specifiers for ->set_dqblk() in struct qc_dqblk and also for
// /*AFLA*/  * ->set_info() in struct qc_info
// /*AFLA*/  */
// /*AFLA*/ #define	QC_INO_SOFT	(1<<0)
// /*AFLA*/ #define	QC_INO_HARD	(1<<1)
// /*AFLA*/ #define	QC_SPC_SOFT	(1<<2)
// /*AFLA*/ #define	QC_SPC_HARD	(1<<3)
// /*AFLA*/ #define	QC_RT_SPC_SOFT	(1<<4)
// /*AFLA*/ #define	QC_RT_SPC_HARD	(1<<5)
// /*AFLA*/ #define QC_LIMIT_MASK (QC_INO_SOFT | QC_INO_HARD | QC_SPC_SOFT | QC_SPC_HARD | \
// /*AFLA*/ 		       QC_RT_SPC_SOFT | QC_RT_SPC_HARD)
// /*AFLA*/ #define	QC_SPC_TIMER	(1<<6)
// /*AFLA*/ #define	QC_INO_TIMER	(1<<7)
// /*AFLA*/ #define	QC_RT_SPC_TIMER	(1<<8)
// /*AFLA*/ #define QC_TIMER_MASK (QC_SPC_TIMER | QC_INO_TIMER | QC_RT_SPC_TIMER)
// /*AFLA*/ #define	QC_SPC_WARNS	(1<<9)
// /*AFLA*/ #define	QC_INO_WARNS	(1<<10)
// /*AFLA*/ #define	QC_RT_SPC_WARNS	(1<<11)
// /*AFLA*/ #define QC_WARNS_MASK (QC_SPC_WARNS | QC_INO_WARNS | QC_RT_SPC_WARNS)
// /*AFLA*/ #define	QC_SPACE	(1<<12)
// /*AFLA*/ #define	QC_INO_COUNT	(1<<13)
// /*AFLA*/ #define	QC_RT_SPACE	(1<<14)
// /*AFLA*/ #define QC_ACCT_MASK (QC_SPACE | QC_INO_COUNT | QC_RT_SPACE)
// /*AFLA*/ #define QC_FLAGS	(1<<15)
// /*AFLA*/ 
// /*AFLA*/ #define QCI_SYSFILE		(1 << 0)	/* Quota file is hidden from userspace */
// /*AFLA*/ #define QCI_ROOT_SQUASH		(1 << 1)	/* Root squash turned on */
// /*AFLA*/ #define QCI_ACCT_ENABLED	(1 << 2)	/* Quota accounting enabled */
// /*AFLA*/ #define QCI_LIMITS_ENFORCED	(1 << 3)	/* Quota limits enforced */
// /*AFLA*/ 
// /*AFLA*/ /* Structures for communicating via ->get_state */
// /*AFLA*/ struct qc_type_state {
// /*AFLA*/ 	unsigned int flags;		/* Flags QCI_* */
// /*AFLA*/ 	unsigned int spc_timelimit;	/* Time after which space softlimit is
// /*AFLA*/ 					 * enforced */
// /*AFLA*/ 	unsigned int ino_timelimit;	/* Ditto for inode softlimit */
// /*AFLA*/ 	unsigned int rt_spc_timelimit;	/* Ditto for real-time space */
// /*AFLA*/ 	unsigned int spc_warnlimit;	/* Limit for number of space warnings */
// /*AFLA*/ 	unsigned int ino_warnlimit;	/* Ditto for inodes */
// /*AFLA*/ 	unsigned int rt_spc_warnlimit;	/* Ditto for real-time space */
// /*AFLA*/ 	unsigned long long ino;		/* Inode number of quota file */
// /*AFLA*/ 	blkcnt_t blocks;		/* Number of 512-byte blocks in the file */
// /*AFLA*/ 	blkcnt_t nextents;		/* Number of extents in the file */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct qc_state {
// /*AFLA*/ 	unsigned int s_incoredqs;	/* Number of dquots in core */
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Per quota type information. The array should really have
// /*AFLA*/ 	 * max(MAXQUOTAS, XQM_MAXQUOTAS) entries. BUILD_BUG_ON in
// /*AFLA*/ 	 * quota_getinfo() makes sure XQM_MAXQUOTAS is large enough.  Once VFS
// /*AFLA*/ 	 * supports project quotas, this can be changed to MAXQUOTAS
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct qc_type_state s_state[XQM_MAXQUOTAS];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Structure for communicating via ->set_info */
// /*AFLA*/ struct qc_info {
// /*AFLA*/ 	int i_fieldmask;	/* mask of fields to change in ->set_info() */
// /*AFLA*/ 	unsigned int i_flags;		/* Flags QCI_* */
// /*AFLA*/ 	unsigned int i_spc_timelimit;	/* Time after which space softlimit is
// /*AFLA*/ 					 * enforced */
// /*AFLA*/ 	unsigned int i_ino_timelimit;	/* Ditto for inode softlimit */
// /*AFLA*/ 	unsigned int i_rt_spc_timelimit;/* Ditto for real-time space */
// /*AFLA*/ 	unsigned int i_spc_warnlimit;	/* Limit for number of space warnings */
// /*AFLA*/ 	unsigned int i_ino_warnlimit;	/* Limit for number of inode warnings */
// /*AFLA*/ 	unsigned int i_rt_spc_warnlimit;	/* Ditto for real-time space */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Operations handling requests from userspace */
// /*AFLA*/ struct quotactl_ops {
// /*AFLA*/ 	int (*quota_on)(struct super_block *, int, int, struct path *);
// /*AFLA*/ 	int (*quota_off)(struct super_block *, int);
// /*AFLA*/ 	int (*quota_enable)(struct super_block *, unsigned int);
// /*AFLA*/ 	int (*quota_disable)(struct super_block *, unsigned int);
// /*AFLA*/ 	int (*quota_sync)(struct super_block *, int);
// /*AFLA*/ 	int (*set_info)(struct super_block *, int, struct qc_info *);
// /*AFLA*/ 	int (*get_dqblk)(struct super_block *, struct kqid, struct qc_dqblk *);
// /*AFLA*/ 	int (*get_nextdqblk)(struct super_block *, struct kqid *,
// /*AFLA*/ 			     struct qc_dqblk *);
// /*AFLA*/ 	int (*set_dqblk)(struct super_block *, struct kqid, struct qc_dqblk *);
// /*AFLA*/ 	int (*get_state)(struct super_block *, struct qc_state *);
// /*AFLA*/ 	int (*rm_xquota)(struct super_block *, unsigned int);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct quota_format_type {
// /*AFLA*/ 	int qf_fmt_id;	/* Quota format id */
// /*AFLA*/ 	const struct quota_format_ops *qf_ops;	/* Operations of format */
// /*AFLA*/ 	struct module *qf_owner;		/* Module implementing quota format */
// /*AFLA*/ 	struct quota_format_type *qf_next;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * Quota state flags - they actually come in two flavors - for users and groups.
// /*AFLA*/  *
// /*AFLA*/  * Actual typed flags layout:
// /*AFLA*/  *				USRQUOTA	GRPQUOTA
// /*AFLA*/  *  DQUOT_USAGE_ENABLED		0x0001		0x0002
// /*AFLA*/  *  DQUOT_LIMITS_ENABLED	0x0004		0x0008
// /*AFLA*/  *  DQUOT_SUSPENDED		0x0010		0x0020
// /*AFLA*/  *
// /*AFLA*/  * Following bits are used for non-typed flags:
// /*AFLA*/  *  DQUOT_QUOTA_SYS_FILE	0x0040
// /*AFLA*/  *  DQUOT_NEGATIVE_USAGE	0x0080
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	_DQUOT_USAGE_ENABLED = 0,		/* Track disk usage for users */
// /*AFLA*/ 	_DQUOT_LIMITS_ENABLED,			/* Enforce quota limits for users */
// /*AFLA*/ 	_DQUOT_SUSPENDED,			/* User diskquotas are off, but
// /*AFLA*/ 						 * we have necessary info in
// /*AFLA*/ 						 * memory to turn them on */
// /*AFLA*/ 	_DQUOT_STATE_FLAGS
// /*AFLA*/ };
// /*AFLA*/ #define DQUOT_USAGE_ENABLED	(1 << _DQUOT_USAGE_ENABLED * MAXQUOTAS)
// /*AFLA*/ #define DQUOT_LIMITS_ENABLED	(1 << _DQUOT_LIMITS_ENABLED * MAXQUOTAS)
// /*AFLA*/ #define DQUOT_SUSPENDED		(1 << _DQUOT_SUSPENDED * MAXQUOTAS)
// /*AFLA*/ #define DQUOT_STATE_FLAGS	(DQUOT_USAGE_ENABLED | DQUOT_LIMITS_ENABLED | \
// /*AFLA*/ 				 DQUOT_SUSPENDED)
// /*AFLA*/ /* Other quota flags */
// /*AFLA*/ #define DQUOT_STATE_LAST	(_DQUOT_STATE_FLAGS * MAXQUOTAS)
// /*AFLA*/ #define DQUOT_QUOTA_SYS_FILE	(1 << DQUOT_STATE_LAST)
// /*AFLA*/ 						/* Quota file is a special
// /*AFLA*/ 						 * system file and user cannot
// /*AFLA*/ 						 * touch it. Filesystem is
// /*AFLA*/ 						 * responsible for setting
// /*AFLA*/ 						 * S_NOQUOTA, S_NOATIME flags
// /*AFLA*/ 						 */
// /*AFLA*/ #define DQUOT_NEGATIVE_USAGE	(1 << (DQUOT_STATE_LAST + 1))
// /*AFLA*/ 					       /* Allow negative quota usage */
// /*AFLA*/ static inline unsigned int dquot_state_flag(unsigned int flags, int type)
// /*AFLA*/ {
// /*AFLA*/ 	return flags << type;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int dquot_generic_flag(unsigned int flags, int type)
// /*AFLA*/ {
// /*AFLA*/ 	return (flags >> type) & DQUOT_STATE_FLAGS;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Bitmap of quota types where flag is set in flags */
// /*AFLA*/ static __always_inline unsigned dquot_state_types(unsigned flags, unsigned flag)
// /*AFLA*/ {
// /*AFLA*/ 	BUILD_BUG_ON_NOT_POWER_OF_2(flag);
// /*AFLA*/ 	return (flags / flag) & ((1 << MAXQUOTAS) - 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_QUOTA_NETLINK_INTERFACE
// /*AFLA*/ extern void quota_send_warning(struct kqid qid, dev_t dev,
// /*AFLA*/ 			       const char warntype);
// /*AFLA*/ #else
// /*AFLA*/ static inline void quota_send_warning(struct kqid qid, dev_t dev,
// /*AFLA*/ 				      const char warntype)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_QUOTA_NETLINK_INTERFACE */
// /*AFLA*/ 
// /*AFLA*/ struct quota_info {
// /*AFLA*/ 	unsigned int flags;			/* Flags for diskquotas on this device */
// /*AFLA*/ 	struct mutex dqio_mutex;		/* lock device while I/O in progress */
// /*AFLA*/ 	struct mutex dqonoff_mutex;		/* Serialize quotaon & quotaoff */
// /*AFLA*/ 	struct inode *files[MAXQUOTAS];		/* inodes of quotafiles */
// /*AFLA*/ 	struct mem_dqinfo info[MAXQUOTAS];	/* Information for each quota type */
// /*AFLA*/ 	const struct quota_format_ops *ops[MAXQUOTAS];	/* Operations for each type */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ int register_quota_format(struct quota_format_type *fmt);
// /*AFLA*/ void unregister_quota_format(struct quota_format_type *fmt);
// /*AFLA*/ 
// /*AFLA*/ struct quota_module_name {
// /*AFLA*/ 	int qm_fmt_id;
// /*AFLA*/ 	char *qm_mod_name;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define INIT_QUOTA_MODULE_NAMES {\
// /*AFLA*/ 	{QFMT_VFS_OLD, "quota_v1"},\
// /*AFLA*/ 	{QFMT_VFS_V0, "quota_v2"},\
// /*AFLA*/ 	{QFMT_VFS_V1, "quota_v2"},\
// /*AFLA*/ 	{0, NULL}}
// /*AFLA*/ 
// /*AFLA*/ #endif /* _QUOTA_ */
