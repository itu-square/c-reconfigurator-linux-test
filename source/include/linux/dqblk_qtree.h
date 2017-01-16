// /*AFLA*/ /*
// /*AFLA*/  *	Definitions of structures and functions for quota formats using trie
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_DQBLK_QTREE_H
// /*AFLA*/ #define _LINUX_DQBLK_QTREE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /* Numbers of blocks needed for updates - we count with the smallest
// /*AFLA*/  * possible block size (1024) */
// /*AFLA*/ #define QTREE_INIT_ALLOC 4
// /*AFLA*/ #define QTREE_INIT_REWRITE 2
// /*AFLA*/ #define QTREE_DEL_ALLOC 0
// /*AFLA*/ #define QTREE_DEL_REWRITE 6
// /*AFLA*/ 
// /*AFLA*/ struct dquot;
// /*AFLA*/ struct kqid;
// /*AFLA*/ 
// /*AFLA*/ /* Operations */
// /*AFLA*/ struct qtree_fmt_operations {
// /*AFLA*/ 	void (*mem2disk_dqblk)(void *disk, struct dquot *dquot);	/* Convert given entry from in memory format to disk one */
// /*AFLA*/ 	void (*disk2mem_dqblk)(struct dquot *dquot, void *disk);	/* Convert given entry from disk format to in memory one */
// /*AFLA*/ 	int (*is_id)(void *disk, struct dquot *dquot);	/* Is this structure for given id? */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Inmemory copy of version specific information */
// /*AFLA*/ struct qtree_mem_dqinfo {
// /*AFLA*/ 	struct super_block *dqi_sb;	/* Sb quota is on */
// /*AFLA*/ 	int dqi_type;			/* Quota type */
// /*AFLA*/ 	unsigned int dqi_blocks;	/* # of blocks in quota file */
// /*AFLA*/ 	unsigned int dqi_free_blk;	/* First block in list of free blocks */
// /*AFLA*/ 	unsigned int dqi_free_entry;	/* First block with free entry */
// /*AFLA*/ 	unsigned int dqi_blocksize_bits;	/* Block size of quota file */
// /*AFLA*/ 	unsigned int dqi_entry_size;	/* Size of quota entry in quota file */
// /*AFLA*/ 	unsigned int dqi_usable_bs;	/* Space usable in block for quota data */
// /*AFLA*/ 	unsigned int dqi_qtree_depth;	/* Precomputed depth of quota tree */
// /*AFLA*/ 	const struct qtree_fmt_operations *dqi_ops; /* Operations for entry manipulation */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ int qtree_write_dquot(struct qtree_mem_dqinfo *info, struct dquot *dquot);
// /*AFLA*/ int qtree_read_dquot(struct qtree_mem_dqinfo *info, struct dquot *dquot);
// /*AFLA*/ int qtree_delete_dquot(struct qtree_mem_dqinfo *info, struct dquot *dquot);
// /*AFLA*/ int qtree_release_dquot(struct qtree_mem_dqinfo *info, struct dquot *dquot);
// /*AFLA*/ int qtree_entry_unused(struct qtree_mem_dqinfo *info, char *disk);
// /*AFLA*/ static inline int qtree_depth(struct qtree_mem_dqinfo *info)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned int epb = info->dqi_usable_bs >> 2;
// /*AFLA*/ 	unsigned long long entries = epb;
// /*AFLA*/ 	int i;
// /*AFLA*/ 
// /*AFLA*/ 	for (i = 1; entries < (1ULL << 32); i++)
// /*AFLA*/ 		entries *= epb;
// /*AFLA*/ 	return i;
// /*AFLA*/ }
// /*AFLA*/ int qtree_get_next_id(struct qtree_mem_dqinfo *info, struct kqid *qid);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_DQBLK_QTREE_H */
