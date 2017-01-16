// /*AFLA*/ /*
// /*AFLA*/  * include/linux/idr.h
// /*AFLA*/  * 
// /*AFLA*/  * 2002-10-18  written by Jim Houston jim.houston@ccur.com
// /*AFLA*/  *	Copyright (C) 2002 by Concurrent Computer Corporation
// /*AFLA*/  *	Distributed under the GNU GPL license version 2.
// /*AFLA*/  *
// /*AFLA*/  * Small id to pointer translation service avoiding fixed sized
// /*AFLA*/  * tables.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __IDR_H__
// /*AFLA*/ #define __IDR_H__
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/bitops.h>
// /*AFLA*/ #include <linux/init.h>
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We want shallower trees and thus more bits covered at each layer.  8
// /*AFLA*/  * bits gives us large enough first layer for most use cases and maximum
// /*AFLA*/  * tree depth of 4.  Each idr_layer is slightly larger than 2k on 64bit and
// /*AFLA*/  * 1k on 32bit.
// /*AFLA*/  */
// /*AFLA*/ #define IDR_BITS 8
// /*AFLA*/ #define IDR_SIZE (1 << IDR_BITS)
// /*AFLA*/ #define IDR_MASK ((1 << IDR_BITS)-1)
// /*AFLA*/ 
// /*AFLA*/ struct idr_layer {
// /*AFLA*/ 	int			prefix;	/* the ID prefix of this idr_layer */
// /*AFLA*/ 	int			layer;	/* distance from leaf */
// /*AFLA*/ 	struct idr_layer __rcu	*ary[1<<IDR_BITS];
// /*AFLA*/ 	int			count;	/* When zero, we can release it */
// /*AFLA*/ 	union {
// /*AFLA*/ 		/* A zero bit means "space here" */
// /*AFLA*/ 		DECLARE_BITMAP(bitmap, IDR_SIZE);
// /*AFLA*/ 		struct rcu_head		rcu_head;
// /*AFLA*/ 	};
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct idr {
// /*AFLA*/ 	struct idr_layer __rcu	*hint;	/* the last layer allocated from */
// /*AFLA*/ 	struct idr_layer __rcu	*top;
// /*AFLA*/ 	int			layers;	/* only valid w/o concurrent changes */
// /*AFLA*/ 	int			cur;	/* current pos for cyclic allocation */
// /*AFLA*/ 	spinlock_t		lock;
// /*AFLA*/ 	int			id_free_cnt;
// /*AFLA*/ 	struct idr_layer	*id_free;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define IDR_INIT(name)							\
// /*AFLA*/ {									\
// /*AFLA*/ 	.lock			= __SPIN_LOCK_UNLOCKED(name.lock),	\
// /*AFLA*/ }
// /*AFLA*/ #define DEFINE_IDR(name)	struct idr name = IDR_INIT(name)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * DOC: idr sync
// /*AFLA*/  * idr synchronization (stolen from radix-tree.h)
// /*AFLA*/  *
// /*AFLA*/  * idr_find() is able to be called locklessly, using RCU. The caller must
// /*AFLA*/  * ensure calls to this function are made within rcu_read_lock() regions.
// /*AFLA*/  * Other readers (lock-free or otherwise) and modifications may be running
// /*AFLA*/  * concurrently.
// /*AFLA*/  *
// /*AFLA*/  * It is still required that the caller manage the synchronization and
// /*AFLA*/  * lifetimes of the items. So if RCU lock-free lookups are used, typically
// /*AFLA*/  * this would mean that the items have their own locks, or are amenable to
// /*AFLA*/  * lock-free access; and that the items are freed by RCU (or only freed after
// /*AFLA*/  * having been deleted from the idr tree *and* a synchronize_rcu() grace
// /*AFLA*/  * period).
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is what we export.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ void *idr_find_slowpath(struct idr *idp, int id);
// /*AFLA*/ void idr_preload(gfp_t gfp_mask);
// /*AFLA*/ int idr_alloc(struct idr *idp, void *ptr, int start, int end, gfp_t gfp_mask);
// /*AFLA*/ int idr_alloc_cyclic(struct idr *idr, void *ptr, int start, int end, gfp_t gfp_mask);
// /*AFLA*/ int idr_for_each(struct idr *idp,
// /*AFLA*/ 		 int (*fn)(int id, void *p, void *data), void *data);
// /*AFLA*/ void *idr_get_next(struct idr *idp, int *nextid);
// /*AFLA*/ void *idr_replace(struct idr *idp, void *ptr, int id);
// /*AFLA*/ void idr_remove(struct idr *idp, int id);
// /*AFLA*/ void idr_destroy(struct idr *idp);
// /*AFLA*/ void idr_init(struct idr *idp);
// /*AFLA*/ bool idr_is_empty(struct idr *idp);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * idr_preload_end - end preload section started with idr_preload()
// /*AFLA*/  *
// /*AFLA*/  * Each idr_preload() should be matched with an invocation of this
// /*AFLA*/  * function.  See idr_preload() for details.
// /*AFLA*/  */
// /*AFLA*/ static inline void idr_preload_end(void)
// /*AFLA*/ {
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * idr_find - return pointer for given id
// /*AFLA*/  * @idr: idr handle
// /*AFLA*/  * @id: lookup key
// /*AFLA*/  *
// /*AFLA*/  * Return the pointer given the id it has been registered with.  A %NULL
// /*AFLA*/  * return indicates that @id is not valid or you passed %NULL in
// /*AFLA*/  * idr_get_new().
// /*AFLA*/  *
// /*AFLA*/  * This function can be called under rcu_read_lock(), given that the leaf
// /*AFLA*/  * pointers lifetimes are correctly managed.
// /*AFLA*/  */
// /*AFLA*/ static inline void *idr_find(struct idr *idr, int id)
// /*AFLA*/ {
// /*AFLA*/ 	struct idr_layer *hint = rcu_dereference_raw(idr->hint);
// /*AFLA*/ 
// /*AFLA*/ 	if (hint && (id & ~IDR_MASK) == hint->prefix)
// /*AFLA*/ 		return rcu_dereference_raw(hint->ary[id & IDR_MASK]);
// /*AFLA*/ 
// /*AFLA*/ 	return idr_find_slowpath(idr, id);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * idr_for_each_entry - iterate over an idr's elements of a given type
// /*AFLA*/  * @idp:     idr handle
// /*AFLA*/  * @entry:   the type * to use as cursor
// /*AFLA*/  * @id:      id entry's key
// /*AFLA*/  *
// /*AFLA*/  * @entry and @id do not need to be initialized before the loop, and
// /*AFLA*/  * after normal terminatinon @entry is left with the value NULL.  This
// /*AFLA*/  * is convenient for a "not found" value.
// /*AFLA*/  */
// /*AFLA*/ #define idr_for_each_entry(idp, entry, id)			\
// /*AFLA*/ 	for (id = 0; ((entry) = idr_get_next(idp, &(id))) != NULL; ++id)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * idr_for_each_entry - continue iteration over an idr's elements of a given type
// /*AFLA*/  * @idp:     idr handle
// /*AFLA*/  * @entry:   the type * to use as cursor
// /*AFLA*/  * @id:      id entry's key
// /*AFLA*/  *
// /*AFLA*/  * Continue to iterate over list of given type, continuing after
// /*AFLA*/  * the current position.
// /*AFLA*/  */
// /*AFLA*/ #define idr_for_each_entry_continue(idp, entry, id)			\
// /*AFLA*/ 	for ((entry) = idr_get_next((idp), &(id));			\
// /*AFLA*/ 	     entry;							\
// /*AFLA*/ 	     ++id, (entry) = idr_get_next((idp), &(id)))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * IDA - IDR based id allocator, use when translation from id to
// /*AFLA*/  * pointer isn't necessary.
// /*AFLA*/  *
// /*AFLA*/  * IDA_BITMAP_LONGS is calculated to be one less to accommodate
// /*AFLA*/  * ida_bitmap->nr_busy so that the whole struct fits in 128 bytes.
// /*AFLA*/  */
// /*AFLA*/ #define IDA_CHUNK_SIZE		128	/* 128 bytes per chunk */
// /*AFLA*/ #define IDA_BITMAP_LONGS	(IDA_CHUNK_SIZE / sizeof(long) - 1)
// /*AFLA*/ #define IDA_BITMAP_BITS 	(IDA_BITMAP_LONGS * sizeof(long) * 8)
// /*AFLA*/ 
// /*AFLA*/ struct ida_bitmap {
// /*AFLA*/ 	long			nr_busy;
// /*AFLA*/ 	unsigned long		bitmap[IDA_BITMAP_LONGS];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct ida {
// /*AFLA*/ 	struct idr		idr;
// /*AFLA*/ 	struct ida_bitmap	*free_bitmap;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define IDA_INIT(name)		{ .idr = IDR_INIT((name).idr), .free_bitmap = NULL, }
// /*AFLA*/ #define DEFINE_IDA(name)	struct ida name = IDA_INIT(name)
// /*AFLA*/ 
// /*AFLA*/ int ida_pre_get(struct ida *ida, gfp_t gfp_mask);
// /*AFLA*/ int ida_get_new_above(struct ida *ida, int starting_id, int *p_id);
// /*AFLA*/ void ida_remove(struct ida *ida, int id);
// /*AFLA*/ void ida_destroy(struct ida *ida);
// /*AFLA*/ void ida_init(struct ida *ida);
// /*AFLA*/ 
// /*AFLA*/ int ida_simple_get(struct ida *ida, unsigned int start, unsigned int end,
// /*AFLA*/ 		   gfp_t gfp_mask);
// /*AFLA*/ void ida_simple_remove(struct ida *ida, unsigned int id);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ida_get_new - allocate new ID
// /*AFLA*/  * @ida:	idr handle
// /*AFLA*/  * @p_id:	pointer to the allocated handle
// /*AFLA*/  *
// /*AFLA*/  * Simple wrapper around ida_get_new_above() w/ @starting_id of zero.
// /*AFLA*/  */
// /*AFLA*/ static inline int ida_get_new(struct ida *ida, int *p_id)
// /*AFLA*/ {
// /*AFLA*/ 	return ida_get_new_above(ida, 0, p_id);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void __init idr_init_cache(void);
// /*AFLA*/ 
// /*AFLA*/ #endif /* __IDR_H__ */
