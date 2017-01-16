// /*AFLA*/ /*
// /*AFLA*/  * Block data types and constants.  Directly include this file only to
// /*AFLA*/  * break include dependency loop.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __LINUX_BLK_TYPES_H
// /*AFLA*/ #define __LINUX_BLK_TYPES_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/bvec.h>
// /*AFLA*/ 
// /*AFLA*/ struct bio_set;
// /*AFLA*/ struct bio;
// /*AFLA*/ struct bio_integrity_payload;
// /*AFLA*/ struct page;
// /*AFLA*/ struct block_device;
// /*AFLA*/ struct io_context;
// /*AFLA*/ struct cgroup_subsys_state;
// /*AFLA*/ typedef void (bio_end_io_t) (struct bio *);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_BLOCK
// /*AFLA*/ /*
// /*AFLA*/  * main unit of I/O for the block layer and lower layers (ie drivers and
// /*AFLA*/  * stacking drivers)
// /*AFLA*/  */
// /*AFLA*/ struct bio {
// /*AFLA*/ 	struct bio		*bi_next;	/* request queue link */
// /*AFLA*/ 	struct block_device	*bi_bdev;
// /*AFLA*/ 	int			bi_error;
// /*AFLA*/ 	unsigned int		bi_opf;		/* bottom bits req flags,
// /*AFLA*/ 						 * top bits REQ_OP. Use
// /*AFLA*/ 						 * accessors.
// /*AFLA*/ 						 */
// /*AFLA*/ 	unsigned short		bi_flags;	/* status, command, etc */
// /*AFLA*/ 	unsigned short		bi_ioprio;
// /*AFLA*/ 
// /*AFLA*/ 	struct bvec_iter	bi_iter;
// /*AFLA*/ 
// /*AFLA*/ 	/* Number of segments in this BIO after
// /*AFLA*/ 	 * physical address coalescing is performed.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned int		bi_phys_segments;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * To keep track of the max segment size, we account for the
// /*AFLA*/ 	 * sizes of the first and last mergeable segments in this bio.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned int		bi_seg_front_size;
// /*AFLA*/ 	unsigned int		bi_seg_back_size;
// /*AFLA*/ 
// /*AFLA*/ 	atomic_t		__bi_remaining;
// /*AFLA*/ 
// /*AFLA*/ 	bio_end_io_t		*bi_end_io;
// /*AFLA*/ 
// /*AFLA*/ 	void			*bi_private;
// /*AFLA*/ #ifdef CONFIG_BLK_CGROUP
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Optional ioc and css associated with this bio.  Put on bio
// /*AFLA*/ 	 * release.  Read comment on top of bio_associate_current().
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct io_context	*bi_ioc;
// /*AFLA*/ 	struct cgroup_subsys_state *bi_css;
// /*AFLA*/ #endif
// /*AFLA*/ 	union {
// /*AFLA*/ #if defined(CONFIG_BLK_DEV_INTEGRITY)
// /*AFLA*/ 		struct bio_integrity_payload *bi_integrity; /* data integrity */
// /*AFLA*/ #endif
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ 	unsigned short		bi_vcnt;	/* how many bio_vec's */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Everything starting with bi_max_vecs will be preserved by bio_reset()
// /*AFLA*/ 	 */
// /*AFLA*/ 
// /*AFLA*/ 	unsigned short		bi_max_vecs;	/* max bvl_vecs we can hold */
// /*AFLA*/ 
// /*AFLA*/ 	atomic_t		__bi_cnt;	/* pin count */
// /*AFLA*/ 
// /*AFLA*/ 	struct bio_vec		*bi_io_vec;	/* the actual vec list */
// /*AFLA*/ 
// /*AFLA*/ 	struct bio_set		*bi_pool;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * We can inline a number of vecs at the end of the bio, to avoid
// /*AFLA*/ 	 * double allocations for a small number of bio_vecs. This member
// /*AFLA*/ 	 * MUST obviously be kept at the very end of the bio.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct bio_vec		bi_inline_vecs[0];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define BIO_OP_SHIFT	(8 * FIELD_SIZEOF(struct bio, bi_opf) - REQ_OP_BITS)
// /*AFLA*/ #define bio_flags(bio)	((bio)->bi_opf & ((1 << BIO_OP_SHIFT) - 1))
// /*AFLA*/ #define bio_op(bio)	((bio)->bi_opf >> BIO_OP_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ #define bio_set_op_attrs(bio, op, op_flags) do {			\
// /*AFLA*/ 	if (__builtin_constant_p(op))					\
// /*AFLA*/ 		BUILD_BUG_ON((op) + 0U >= (1U << REQ_OP_BITS));		\
// /*AFLA*/ 	else								\
// /*AFLA*/ 		WARN_ON_ONCE((op) + 0U >= (1U << REQ_OP_BITS));		\
// /*AFLA*/ 	if (__builtin_constant_p(op_flags))				\
// /*AFLA*/ 		BUILD_BUG_ON((op_flags) + 0U >= (1U << BIO_OP_SHIFT));	\
// /*AFLA*/ 	else								\
// /*AFLA*/ 		WARN_ON_ONCE((op_flags) + 0U >= (1U << BIO_OP_SHIFT));	\
// /*AFLA*/ 	(bio)->bi_opf = bio_flags(bio);					\
// /*AFLA*/ 	(bio)->bi_opf |= (((op) + 0U) << BIO_OP_SHIFT);			\
// /*AFLA*/ 	(bio)->bi_opf |= (op_flags);					\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define BIO_RESET_BYTES		offsetof(struct bio, bi_max_vecs)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * bio flags
// /*AFLA*/  */
// /*AFLA*/ #define BIO_SEG_VALID	1	/* bi_phys_segments valid */
// /*AFLA*/ #define BIO_CLONED	2	/* doesn't own data */
// /*AFLA*/ #define BIO_BOUNCED	3	/* bio is a bounce bio */
// /*AFLA*/ #define BIO_USER_MAPPED 4	/* contains user pages */
// /*AFLA*/ #define BIO_NULL_MAPPED 5	/* contains invalid user pages */
// /*AFLA*/ #define BIO_QUIET	6	/* Make BIO Quiet */
// /*AFLA*/ #define BIO_CHAIN	7	/* chained bio, ->bi_remaining in effect */
// /*AFLA*/ #define BIO_REFFED	8	/* bio has elevated ->bi_cnt */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Flags starting here get preserved by bio_reset() - this includes
// /*AFLA*/  * BVEC_POOL_IDX()
// /*AFLA*/  */
// /*AFLA*/ #define BIO_RESET_BITS	10
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We support 6 different bvec pools, the last one is magic in that it
// /*AFLA*/  * is backed by a mempool.
// /*AFLA*/  */
// /*AFLA*/ #define BVEC_POOL_NR		6
// /*AFLA*/ #define BVEC_POOL_MAX		(BVEC_POOL_NR - 1)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Top 4 bits of bio flags indicate the pool the bvecs came from.  We add
// /*AFLA*/  * 1 to the actual index so that 0 indicates that there are no bvecs to be
// /*AFLA*/  * freed.
// /*AFLA*/  */
// /*AFLA*/ #define BVEC_POOL_BITS		(4)
// /*AFLA*/ #define BVEC_POOL_OFFSET	(16 - BVEC_POOL_BITS)
// /*AFLA*/ #define BVEC_POOL_IDX(bio)	((bio)->bi_flags >> BVEC_POOL_OFFSET)
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_BLOCK */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Request flags.  For use in the cmd_flags field of struct request, and in
// /*AFLA*/  * bi_opf of struct bio.  Note that some flags are only valid in either one.
// /*AFLA*/  */
// /*AFLA*/ enum rq_flag_bits {
// /*AFLA*/ 	/* common flags */
// /*AFLA*/ 	__REQ_FAILFAST_DEV,	/* no driver retries of device errors */
// /*AFLA*/ 	__REQ_FAILFAST_TRANSPORT, /* no driver retries of transport errors */
// /*AFLA*/ 	__REQ_FAILFAST_DRIVER,	/* no driver retries of driver errors */
// /*AFLA*/ 
// /*AFLA*/ 	__REQ_SYNC,		/* request is sync (sync write or read) */
// /*AFLA*/ 	__REQ_META,		/* metadata io request */
// /*AFLA*/ 	__REQ_PRIO,		/* boost priority in cfq */
// /*AFLA*/ 
// /*AFLA*/ 	__REQ_NOIDLE,		/* don't anticipate more IO after this one */
// /*AFLA*/ 	__REQ_INTEGRITY,	/* I/O includes block integrity payload */
// /*AFLA*/ 	__REQ_FUA,		/* forced unit access */
// /*AFLA*/ 	__REQ_PREFLUSH,		/* request for cache flush */
// /*AFLA*/ 
// /*AFLA*/ 	/* bio only flags */
// /*AFLA*/ 	__REQ_RAHEAD,		/* read ahead, can fail anytime */
// /*AFLA*/ 	__REQ_THROTTLED,	/* This bio has already been subjected to
// /*AFLA*/ 				 * throttling rules. Don't do it again. */
// /*AFLA*/ 
// /*AFLA*/ 	/* request only flags */
// /*AFLA*/ 	__REQ_SORTED,		/* elevator knows about this request */
// /*AFLA*/ 	__REQ_SOFTBARRIER,	/* may not be passed by ioscheduler */
// /*AFLA*/ 	__REQ_NOMERGE,		/* don't touch this for merging */
// /*AFLA*/ 	__REQ_STARTED,		/* drive already may have started this one */
// /*AFLA*/ 	__REQ_DONTPREP,		/* don't call prep for this one */
// /*AFLA*/ 	__REQ_QUEUED,		/* uses queueing */
// /*AFLA*/ 	__REQ_ELVPRIV,		/* elevator private data attached */
// /*AFLA*/ 	__REQ_FAILED,		/* set if the request failed */
// /*AFLA*/ 	__REQ_QUIET,		/* don't worry about errors */
// /*AFLA*/ 	__REQ_PREEMPT,		/* set for "ide_preempt" requests and also
// /*AFLA*/ 				   for requests for which the SCSI "quiesce"
// /*AFLA*/ 				   state must be ignored. */
// /*AFLA*/ 	__REQ_ALLOCED,		/* request came from our alloc pool */
// /*AFLA*/ 	__REQ_COPY_USER,	/* contains copies of user pages */
// /*AFLA*/ 	__REQ_FLUSH_SEQ,	/* request for flush sequence */
// /*AFLA*/ 	__REQ_IO_STAT,		/* account I/O stat */
// /*AFLA*/ 	__REQ_MIXED_MERGE,	/* merge of different types, fail separately */
// /*AFLA*/ 	__REQ_PM,		/* runtime pm request */
// /*AFLA*/ 	__REQ_HASHED,		/* on IO scheduler merge hash */
// /*AFLA*/ 	__REQ_MQ_INFLIGHT,	/* track inflight for MQ */
// /*AFLA*/ 	__REQ_NR_BITS,		/* stops here */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define REQ_FAILFAST_DEV	(1ULL << __REQ_FAILFAST_DEV)
// /*AFLA*/ #define REQ_FAILFAST_TRANSPORT	(1ULL << __REQ_FAILFAST_TRANSPORT)
// /*AFLA*/ #define REQ_FAILFAST_DRIVER	(1ULL << __REQ_FAILFAST_DRIVER)
// /*AFLA*/ #define REQ_SYNC		(1ULL << __REQ_SYNC)
// /*AFLA*/ #define REQ_META		(1ULL << __REQ_META)
// /*AFLA*/ #define REQ_PRIO		(1ULL << __REQ_PRIO)
// /*AFLA*/ #define REQ_NOIDLE		(1ULL << __REQ_NOIDLE)
// /*AFLA*/ #define REQ_INTEGRITY		(1ULL << __REQ_INTEGRITY)
// /*AFLA*/ 
// /*AFLA*/ #define REQ_FAILFAST_MASK \
// /*AFLA*/ 	(REQ_FAILFAST_DEV | REQ_FAILFAST_TRANSPORT | REQ_FAILFAST_DRIVER)
// /*AFLA*/ #define REQ_COMMON_MASK \
// /*AFLA*/ 	(REQ_FAILFAST_MASK | REQ_SYNC | REQ_META | REQ_PRIO | REQ_NOIDLE | \
// /*AFLA*/ 	 REQ_PREFLUSH | REQ_FUA | REQ_INTEGRITY | REQ_NOMERGE)
// /*AFLA*/ #define REQ_CLONE_MASK		REQ_COMMON_MASK
// /*AFLA*/ 
// /*AFLA*/ /* This mask is used for both bio and request merge checking */
// /*AFLA*/ #define REQ_NOMERGE_FLAGS \
// /*AFLA*/ 	(REQ_NOMERGE | REQ_STARTED | REQ_SOFTBARRIER | REQ_PREFLUSH | REQ_FUA | REQ_FLUSH_SEQ)
// /*AFLA*/ 
// /*AFLA*/ #define REQ_RAHEAD		(1ULL << __REQ_RAHEAD)
// /*AFLA*/ #define REQ_THROTTLED		(1ULL << __REQ_THROTTLED)
// /*AFLA*/ 
// /*AFLA*/ #define REQ_SORTED		(1ULL << __REQ_SORTED)
// /*AFLA*/ #define REQ_SOFTBARRIER		(1ULL << __REQ_SOFTBARRIER)
// /*AFLA*/ #define REQ_FUA			(1ULL << __REQ_FUA)
// /*AFLA*/ #define REQ_NOMERGE		(1ULL << __REQ_NOMERGE)
// /*AFLA*/ #define REQ_STARTED		(1ULL << __REQ_STARTED)
// /*AFLA*/ #define REQ_DONTPREP		(1ULL << __REQ_DONTPREP)
// /*AFLA*/ #define REQ_QUEUED		(1ULL << __REQ_QUEUED)
// /*AFLA*/ #define REQ_ELVPRIV		(1ULL << __REQ_ELVPRIV)
// /*AFLA*/ #define REQ_FAILED		(1ULL << __REQ_FAILED)
// /*AFLA*/ #define REQ_QUIET		(1ULL << __REQ_QUIET)
// /*AFLA*/ #define REQ_PREEMPT		(1ULL << __REQ_PREEMPT)
// /*AFLA*/ #define REQ_ALLOCED		(1ULL << __REQ_ALLOCED)
// /*AFLA*/ #define REQ_COPY_USER		(1ULL << __REQ_COPY_USER)
// /*AFLA*/ #define REQ_PREFLUSH		(1ULL << __REQ_PREFLUSH)
// /*AFLA*/ #define REQ_FLUSH_SEQ		(1ULL << __REQ_FLUSH_SEQ)
// /*AFLA*/ #define REQ_IO_STAT		(1ULL << __REQ_IO_STAT)
// /*AFLA*/ #define REQ_MIXED_MERGE		(1ULL << __REQ_MIXED_MERGE)
// /*AFLA*/ #define REQ_PM			(1ULL << __REQ_PM)
// /*AFLA*/ #define REQ_HASHED		(1ULL << __REQ_HASHED)
// /*AFLA*/ #define REQ_MQ_INFLIGHT		(1ULL << __REQ_MQ_INFLIGHT)
// /*AFLA*/ 
// /*AFLA*/ enum req_op {
// /*AFLA*/ 	REQ_OP_READ,
// /*AFLA*/ 	REQ_OP_WRITE,
// /*AFLA*/ 	REQ_OP_DISCARD,		/* request to discard sectors */
// /*AFLA*/ 	REQ_OP_SECURE_ERASE,	/* request to securely erase sectors */
// /*AFLA*/ 	REQ_OP_WRITE_SAME,	/* write same block many times */
// /*AFLA*/ 	REQ_OP_FLUSH,		/* request for cache flush */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define REQ_OP_BITS 3
// /*AFLA*/ 
// /*AFLA*/ typedef unsigned int blk_qc_t;
// /*AFLA*/ #define BLK_QC_T_NONE	-1U
// /*AFLA*/ #define BLK_QC_T_SHIFT	16
// /*AFLA*/ 
// /*AFLA*/ static inline bool blk_qc_t_valid(blk_qc_t cookie)
// /*AFLA*/ {
// /*AFLA*/ 	return cookie != BLK_QC_T_NONE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline blk_qc_t blk_tag_to_qc_t(unsigned int tag, unsigned int queue_num)
// /*AFLA*/ {
// /*AFLA*/ 	return tag | (queue_num << BLK_QC_T_SHIFT);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int blk_qc_t_to_queue_num(blk_qc_t cookie)
// /*AFLA*/ {
// /*AFLA*/ 	return cookie >> BLK_QC_T_SHIFT;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int blk_qc_t_to_tag(blk_qc_t cookie)
// /*AFLA*/ {
// /*AFLA*/ 	return cookie & ((1u << BLK_QC_T_SHIFT) - 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_BLK_TYPES_H */
