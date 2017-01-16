// /*AFLA*/ /*
// /*AFLA*/  * Copyright (C) 2001 Momchil Velikov
// /*AFLA*/  * Portions Copyright (C) 2001 Christoph Hellwig
// /*AFLA*/  * Copyright (C) 2006 Nick Piggin
// /*AFLA*/  * Copyright (C) 2012 Konstantin Khlebnikov
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or
// /*AFLA*/  * modify it under the terms of the GNU General Public License as
// /*AFLA*/  * published by the Free Software Foundation; either version 2, or (at
// /*AFLA*/  * your option) any later version.
// /*AFLA*/  * 
// /*AFLA*/  * This program is distributed in the hope that it will be useful, but
// /*AFLA*/  * WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// /*AFLA*/  * General Public License for more details.
// /*AFLA*/  * 
// /*AFLA*/  * You should have received a copy of the GNU General Public License
// /*AFLA*/  * along with this program; if not, write to the Free Software
// /*AFLA*/  * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LINUX_RADIX_TREE_H
// /*AFLA*/ #define _LINUX_RADIX_TREE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/bitops.h>
// /*AFLA*/ #include <linux/preempt.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The bottom two bits of the slot determine how the remaining bits in the
// /*AFLA*/  * slot are interpreted:
// /*AFLA*/  *
// /*AFLA*/  * 00 - data pointer
// /*AFLA*/  * 01 - internal entry
// /*AFLA*/  * 10 - exceptional entry
// /*AFLA*/  * 11 - this bit combination is currently unused/reserved
// /*AFLA*/  *
// /*AFLA*/  * The internal entry may be a pointer to the next level in the tree, a
// /*AFLA*/  * sibling entry, or an indicator that the entry in this slot has been moved
// /*AFLA*/  * to another location in the tree and the lookup should be restarted.  While
// /*AFLA*/  * NULL fits the 'data pointer' pattern, it means that there is no entry in
// /*AFLA*/  * the tree for this index (no matter what level of the tree it is found at).
// /*AFLA*/  * This means that you cannot store NULL in the tree as a value for the index.
// /*AFLA*/  */
// /*AFLA*/ #define RADIX_TREE_ENTRY_MASK		3UL
// /*AFLA*/ #define RADIX_TREE_INTERNAL_NODE	1UL
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Most users of the radix tree store pointers but shmem/tmpfs stores swap
// /*AFLA*/  * entries in the same tree.  They are marked as exceptional entries to
// /*AFLA*/  * distinguish them from pointers to struct page.
// /*AFLA*/  * EXCEPTIONAL_ENTRY tests the bit, EXCEPTIONAL_SHIFT shifts content past it.
// /*AFLA*/  */
// /*AFLA*/ #define RADIX_TREE_EXCEPTIONAL_ENTRY	2
// /*AFLA*/ #define RADIX_TREE_EXCEPTIONAL_SHIFT	2
// /*AFLA*/ 
// /*AFLA*/ static inline bool radix_tree_is_internal_node(void *ptr)
// /*AFLA*/ {
// /*AFLA*/ 	return ((unsigned long)ptr & RADIX_TREE_ENTRY_MASK) ==
// /*AFLA*/ 				RADIX_TREE_INTERNAL_NODE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*** radix-tree API starts here ***/
// /*AFLA*/ 
// /*AFLA*/ #define RADIX_TREE_MAX_TAGS 3
// /*AFLA*/ 
// /*AFLA*/ #ifndef RADIX_TREE_MAP_SHIFT
// /*AFLA*/ #define RADIX_TREE_MAP_SHIFT	(CONFIG_BASE_SMALL ? 4 : 6)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define RADIX_TREE_MAP_SIZE	(1UL << RADIX_TREE_MAP_SHIFT)
// /*AFLA*/ #define RADIX_TREE_MAP_MASK	(RADIX_TREE_MAP_SIZE-1)
// /*AFLA*/ 
// /*AFLA*/ #define RADIX_TREE_TAG_LONGS	\
// /*AFLA*/ 	((RADIX_TREE_MAP_SIZE + BITS_PER_LONG - 1) / BITS_PER_LONG)
// /*AFLA*/ 
// /*AFLA*/ #define RADIX_TREE_INDEX_BITS  (8 /* CHAR_BIT */ * sizeof(unsigned long))
// /*AFLA*/ #define RADIX_TREE_MAX_PATH (DIV_ROUND_UP(RADIX_TREE_INDEX_BITS, \
// /*AFLA*/ 					  RADIX_TREE_MAP_SHIFT))
// /*AFLA*/ 
// /*AFLA*/ /* Internally used bits of node->count */
// /*AFLA*/ #define RADIX_TREE_COUNT_SHIFT	(RADIX_TREE_MAP_SHIFT + 1)
// /*AFLA*/ #define RADIX_TREE_COUNT_MASK	((1UL << RADIX_TREE_COUNT_SHIFT) - 1)
// /*AFLA*/ 
// /*AFLA*/ struct radix_tree_node {
// /*AFLA*/ 	unsigned char	shift;	/* Bits remaining in each slot */
// /*AFLA*/ 	unsigned char	offset;	/* Slot offset in parent */
// /*AFLA*/ 	unsigned int	count;
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct {
// /*AFLA*/ 			/* Used when ascending tree */
// /*AFLA*/ 			struct radix_tree_node *parent;
// /*AFLA*/ 			/* For tree user */
// /*AFLA*/ 			void *private_data;
// /*AFLA*/ 		};
// /*AFLA*/ 		/* Used when freeing node */
// /*AFLA*/ 		struct rcu_head	rcu_head;
// /*AFLA*/ 	};
// /*AFLA*/ 	/* For tree user */
// /*AFLA*/ 	struct list_head private_list;
// /*AFLA*/ 	void __rcu	*slots[RADIX_TREE_MAP_SIZE];
// /*AFLA*/ 	unsigned long	tags[RADIX_TREE_MAX_TAGS][RADIX_TREE_TAG_LONGS];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* root tags are stored in gfp_mask, shifted by __GFP_BITS_SHIFT */
// /*AFLA*/ struct radix_tree_root {
// /*AFLA*/ 	gfp_t			gfp_mask;
// /*AFLA*/ 	struct radix_tree_node	__rcu *rnode;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define RADIX_TREE_INIT(mask)	{					\
// /*AFLA*/ 	.gfp_mask = (mask),						\
// /*AFLA*/ 	.rnode = NULL,							\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define RADIX_TREE(name, mask) \
// /*AFLA*/ 	struct radix_tree_root name = RADIX_TREE_INIT(mask)
// /*AFLA*/ 
// /*AFLA*/ #define INIT_RADIX_TREE(root, mask)					\
// /*AFLA*/ do {									\
// /*AFLA*/ 	(root)->gfp_mask = (mask);					\
// /*AFLA*/ 	(root)->rnode = NULL;						\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ static inline bool radix_tree_empty(struct radix_tree_root *root)
// /*AFLA*/ {
// /*AFLA*/ 	return root->rnode == NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * Radix-tree synchronization
// /*AFLA*/  *
// /*AFLA*/  * The radix-tree API requires that users provide all synchronisation (with
// /*AFLA*/  * specific exceptions, noted below).
// /*AFLA*/  *
// /*AFLA*/  * Synchronization of access to the data items being stored in the tree, and
// /*AFLA*/  * management of their lifetimes must be completely managed by API users.
// /*AFLA*/  *
// /*AFLA*/  * For API usage, in general,
// /*AFLA*/  * - any function _modifying_ the tree or tags (inserting or deleting
// /*AFLA*/  *   items, setting or clearing tags) must exclude other modifications, and
// /*AFLA*/  *   exclude any functions reading the tree.
// /*AFLA*/  * - any function _reading_ the tree or tags (looking up items or tags,
// /*AFLA*/  *   gang lookups) must exclude modifications to the tree, but may occur
// /*AFLA*/  *   concurrently with other readers.
// /*AFLA*/  *
// /*AFLA*/  * The notable exceptions to this rule are the following functions:
// /*AFLA*/  * __radix_tree_lookup
// /*AFLA*/  * radix_tree_lookup
// /*AFLA*/  * radix_tree_lookup_slot
// /*AFLA*/  * radix_tree_tag_get
// /*AFLA*/  * radix_tree_gang_lookup
// /*AFLA*/  * radix_tree_gang_lookup_slot
// /*AFLA*/  * radix_tree_gang_lookup_tag
// /*AFLA*/  * radix_tree_gang_lookup_tag_slot
// /*AFLA*/  * radix_tree_tagged
// /*AFLA*/  *
// /*AFLA*/  * The first 8 functions are able to be called locklessly, using RCU. The
// /*AFLA*/  * caller must ensure calls to these functions are made within rcu_read_lock()
// /*AFLA*/  * regions. Other readers (lock-free or otherwise) and modifications may be
// /*AFLA*/  * running concurrently.
// /*AFLA*/  *
// /*AFLA*/  * It is still required that the caller manage the synchronization and lifetimes
// /*AFLA*/  * of the items. So if RCU lock-free lookups are used, typically this would mean
// /*AFLA*/  * that the items have their own locks, or are amenable to lock-free access; and
// /*AFLA*/  * that the items are freed by RCU (or only freed after having been deleted from
// /*AFLA*/  * the radix tree *and* a synchronize_rcu() grace period).
// /*AFLA*/  *
// /*AFLA*/  * (Note, rcu_assign_pointer and rcu_dereference are not needed to control
// /*AFLA*/  * access to data items when inserting into or looking up from the radix tree)
// /*AFLA*/  *
// /*AFLA*/  * Note that the value returned by radix_tree_tag_get() may not be relied upon
// /*AFLA*/  * if only the RCU read lock is held.  Functions to set/clear tags and to
// /*AFLA*/  * delete nodes running concurrently with it may affect its result such that
// /*AFLA*/  * two consecutive reads in the same locked section may return different
// /*AFLA*/  * values.  If reliability is required, modification functions must also be
// /*AFLA*/  * excluded from concurrency.
// /*AFLA*/  *
// /*AFLA*/  * radix_tree_tagged is able to be called without locking or RCU.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * radix_tree_deref_slot	- dereference a slot
// /*AFLA*/  * @pslot:	pointer to slot, returned by radix_tree_lookup_slot
// /*AFLA*/  * Returns:	item that was stored in that slot with any direct pointer flag
// /*AFLA*/  *		removed.
// /*AFLA*/  *
// /*AFLA*/  * For use with radix_tree_lookup_slot().  Caller must hold tree at least read
// /*AFLA*/  * locked across slot lookup and dereference. Not required if write lock is
// /*AFLA*/  * held (ie. items cannot be concurrently inserted).
// /*AFLA*/  *
// /*AFLA*/  * radix_tree_deref_retry must be used to confirm validity of the pointer if
// /*AFLA*/  * only the read lock is held.
// /*AFLA*/  */
// /*AFLA*/ static inline void *radix_tree_deref_slot(void **pslot)
// /*AFLA*/ {
// /*AFLA*/ 	return rcu_dereference(*pslot);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * radix_tree_deref_slot_protected	- dereference a slot without RCU lock but with tree lock held
// /*AFLA*/  * @pslot:	pointer to slot, returned by radix_tree_lookup_slot
// /*AFLA*/  * Returns:	item that was stored in that slot with any direct pointer flag
// /*AFLA*/  *		removed.
// /*AFLA*/  *
// /*AFLA*/  * Similar to radix_tree_deref_slot but only used during migration when a pages
// /*AFLA*/  * mapping is being moved. The caller does not hold the RCU read lock but it
// /*AFLA*/  * must hold the tree lock to prevent parallel updates.
// /*AFLA*/  */
// /*AFLA*/ static inline void *radix_tree_deref_slot_protected(void **pslot,
// /*AFLA*/ 							spinlock_t *treelock)
// /*AFLA*/ {
// /*AFLA*/ 	return rcu_dereference_protected(*pslot, lockdep_is_held(treelock));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * radix_tree_deref_retry	- check radix_tree_deref_slot
// /*AFLA*/  * @arg:	pointer returned by radix_tree_deref_slot
// /*AFLA*/  * Returns:	0 if retry is not required, otherwise retry is required
// /*AFLA*/  *
// /*AFLA*/  * radix_tree_deref_retry must be used with radix_tree_deref_slot.
// /*AFLA*/  */
// /*AFLA*/ static inline int radix_tree_deref_retry(void *arg)
// /*AFLA*/ {
// /*AFLA*/ 	return unlikely(radix_tree_is_internal_node(arg));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * radix_tree_exceptional_entry	- radix_tree_deref_slot gave exceptional entry?
// /*AFLA*/  * @arg:	value returned by radix_tree_deref_slot
// /*AFLA*/  * Returns:	0 if well-aligned pointer, non-0 if exceptional entry.
// /*AFLA*/  */
// /*AFLA*/ static inline int radix_tree_exceptional_entry(void *arg)
// /*AFLA*/ {
// /*AFLA*/ 	/* Not unlikely because radix_tree_exception often tested first */
// /*AFLA*/ 	return (unsigned long)arg & RADIX_TREE_EXCEPTIONAL_ENTRY;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * radix_tree_exception	- radix_tree_deref_slot returned either exception?
// /*AFLA*/  * @arg:	value returned by radix_tree_deref_slot
// /*AFLA*/  * Returns:	0 if well-aligned pointer, non-0 if either kind of exception.
// /*AFLA*/  */
// /*AFLA*/ static inline int radix_tree_exception(void *arg)
// /*AFLA*/ {
// /*AFLA*/ 	return unlikely((unsigned long)arg & RADIX_TREE_ENTRY_MASK);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * radix_tree_replace_slot	- replace item in a slot
// /*AFLA*/  * @pslot:	pointer to slot, returned by radix_tree_lookup_slot
// /*AFLA*/  * @item:	new item to store in the slot.
// /*AFLA*/  *
// /*AFLA*/  * For use with radix_tree_lookup_slot().  Caller must hold tree write locked
// /*AFLA*/  * across slot lookup and replacement.
// /*AFLA*/  */
// /*AFLA*/ static inline void radix_tree_replace_slot(void **pslot, void *item)
// /*AFLA*/ {
// /*AFLA*/ 	BUG_ON(radix_tree_is_internal_node(item));
// /*AFLA*/ 	rcu_assign_pointer(*pslot, item);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ int __radix_tree_create(struct radix_tree_root *root, unsigned long index,
// /*AFLA*/ 			unsigned order, struct radix_tree_node **nodep,
// /*AFLA*/ 			void ***slotp);
// /*AFLA*/ int __radix_tree_insert(struct radix_tree_root *, unsigned long index,
// /*AFLA*/ 			unsigned order, void *);
// /*AFLA*/ static inline int radix_tree_insert(struct radix_tree_root *root,
// /*AFLA*/ 			unsigned long index, void *entry)
// /*AFLA*/ {
// /*AFLA*/ 	return __radix_tree_insert(root, index, 0, entry);
// /*AFLA*/ }
// /*AFLA*/ void *__radix_tree_lookup(struct radix_tree_root *root, unsigned long index,
// /*AFLA*/ 			  struct radix_tree_node **nodep, void ***slotp);
// /*AFLA*/ void *radix_tree_lookup(struct radix_tree_root *, unsigned long);
// /*AFLA*/ void **radix_tree_lookup_slot(struct radix_tree_root *, unsigned long);
// /*AFLA*/ bool __radix_tree_delete_node(struct radix_tree_root *root,
// /*AFLA*/ 			      struct radix_tree_node *node);
// /*AFLA*/ void *radix_tree_delete_item(struct radix_tree_root *, unsigned long, void *);
// /*AFLA*/ void *radix_tree_delete(struct radix_tree_root *, unsigned long);
// /*AFLA*/ void radix_tree_clear_tags(struct radix_tree_root *root,
// /*AFLA*/ 			   struct radix_tree_node *node,
// /*AFLA*/ 			   void **slot);
// /*AFLA*/ unsigned int radix_tree_gang_lookup(struct radix_tree_root *root,
// /*AFLA*/ 			void **results, unsigned long first_index,
// /*AFLA*/ 			unsigned int max_items);
// /*AFLA*/ unsigned int radix_tree_gang_lookup_slot(struct radix_tree_root *root,
// /*AFLA*/ 			void ***results, unsigned long *indices,
// /*AFLA*/ 			unsigned long first_index, unsigned int max_items);
// /*AFLA*/ int radix_tree_preload(gfp_t gfp_mask);
// /*AFLA*/ int radix_tree_maybe_preload(gfp_t gfp_mask);
// /*AFLA*/ int radix_tree_maybe_preload_order(gfp_t gfp_mask, int order);
// /*AFLA*/ void radix_tree_init(void);
// /*AFLA*/ void *radix_tree_tag_set(struct radix_tree_root *root,
// /*AFLA*/ 			unsigned long index, unsigned int tag);
// /*AFLA*/ void *radix_tree_tag_clear(struct radix_tree_root *root,
// /*AFLA*/ 			unsigned long index, unsigned int tag);
// /*AFLA*/ int radix_tree_tag_get(struct radix_tree_root *root,
// /*AFLA*/ 			unsigned long index, unsigned int tag);
// /*AFLA*/ unsigned int
// /*AFLA*/ radix_tree_gang_lookup_tag(struct radix_tree_root *root, void **results,
// /*AFLA*/ 		unsigned long first_index, unsigned int max_items,
// /*AFLA*/ 		unsigned int tag);
// /*AFLA*/ unsigned int
// /*AFLA*/ radix_tree_gang_lookup_tag_slot(struct radix_tree_root *root, void ***results,
// /*AFLA*/ 		unsigned long first_index, unsigned int max_items,
// /*AFLA*/ 		unsigned int tag);
// /*AFLA*/ unsigned long radix_tree_range_tag_if_tagged(struct radix_tree_root *root,
// /*AFLA*/ 		unsigned long *first_indexp, unsigned long last_index,
// /*AFLA*/ 		unsigned long nr_to_tag,
// /*AFLA*/ 		unsigned int fromtag, unsigned int totag);
// /*AFLA*/ int radix_tree_tagged(struct radix_tree_root *root, unsigned int tag);
// /*AFLA*/ unsigned long radix_tree_locate_item(struct radix_tree_root *root, void *item);
// /*AFLA*/ 
// /*AFLA*/ static inline void radix_tree_preload_end(void)
// /*AFLA*/ {
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct radix_tree_iter - radix tree iterator state
// /*AFLA*/  *
// /*AFLA*/  * @index:	index of current slot
// /*AFLA*/  * @next_index:	one beyond the last index for this chunk
// /*AFLA*/  * @tags:	bit-mask for tag-iterating
// /*AFLA*/  * @shift:	shift for the node that holds our slots
// /*AFLA*/  *
// /*AFLA*/  * This radix tree iterator works in terms of "chunks" of slots.  A chunk is a
// /*AFLA*/  * subinterval of slots contained within one radix tree leaf node.  It is
// /*AFLA*/  * described by a pointer to its first slot and a struct radix_tree_iter
// /*AFLA*/  * which holds the chunk's position in the tree and its size.  For tagged
// /*AFLA*/  * iteration radix_tree_iter also holds the slots' bit-mask for one chosen
// /*AFLA*/  * radix tree tag.
// /*AFLA*/  */
// /*AFLA*/ struct radix_tree_iter {
// /*AFLA*/ 	unsigned long	index;
// /*AFLA*/ 	unsigned long	next_index;
// /*AFLA*/ 	unsigned long	tags;
// /*AFLA*/ #ifdef CONFIG_RADIX_TREE_MULTIORDER
// /*AFLA*/ 	unsigned int	shift;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int iter_shift(struct radix_tree_iter *iter)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_RADIX_TREE_MULTIORDER
// /*AFLA*/ 	return iter->shift;
// /*AFLA*/ #else
// /*AFLA*/ 	return 0;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define RADIX_TREE_ITER_TAG_MASK	0x00FF	/* tag index in lower byte */
// /*AFLA*/ #define RADIX_TREE_ITER_TAGGED		0x0100	/* lookup tagged slots */
// /*AFLA*/ #define RADIX_TREE_ITER_CONTIG		0x0200	/* stop at first hole */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * radix_tree_iter_init - initialize radix tree iterator
// /*AFLA*/  *
// /*AFLA*/  * @iter:	pointer to iterator state
// /*AFLA*/  * @start:	iteration starting index
// /*AFLA*/  * Returns:	NULL
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void **
// /*AFLA*/ radix_tree_iter_init(struct radix_tree_iter *iter, unsigned long start)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Leave iter->tags uninitialized. radix_tree_next_chunk() will fill it
// /*AFLA*/ 	 * in the case of a successful tagged chunk lookup.  If the lookup was
// /*AFLA*/ 	 * unsuccessful or non-tagged then nobody cares about ->tags.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Set index to zero to bypass next_index overflow protection.
// /*AFLA*/ 	 * See the comment in radix_tree_next_chunk() for details.
// /*AFLA*/ 	 */
// /*AFLA*/ 	iter->index = 0;
// /*AFLA*/ 	iter->next_index = start;
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * radix_tree_next_chunk - find next chunk of slots for iteration
// /*AFLA*/  *
// /*AFLA*/  * @root:	radix tree root
// /*AFLA*/  * @iter:	iterator state
// /*AFLA*/  * @flags:	RADIX_TREE_ITER_* flags and tag index
// /*AFLA*/  * Returns:	pointer to chunk first slot, or NULL if there no more left
// /*AFLA*/  *
// /*AFLA*/  * This function looks up the next chunk in the radix tree starting from
// /*AFLA*/  * @iter->next_index.  It returns a pointer to the chunk's first slot.
// /*AFLA*/  * Also it fills @iter with data about chunk: position in the tree (index),
// /*AFLA*/  * its end (next_index), and constructs a bit mask for tagged iterating (tags).
// /*AFLA*/  */
// /*AFLA*/ void **radix_tree_next_chunk(struct radix_tree_root *root,
// /*AFLA*/ 			     struct radix_tree_iter *iter, unsigned flags);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * radix_tree_iter_retry - retry this chunk of the iteration
// /*AFLA*/  * @iter:	iterator state
// /*AFLA*/  *
// /*AFLA*/  * If we iterate over a tree protected only by the RCU lock, a race
// /*AFLA*/  * against deletion or creation may result in seeing a slot for which
// /*AFLA*/  * radix_tree_deref_retry() returns true.  If so, call this function
// /*AFLA*/  * and continue the iteration.
// /*AFLA*/  */
// /*AFLA*/ static inline __must_check
// /*AFLA*/ void **radix_tree_iter_retry(struct radix_tree_iter *iter)
// /*AFLA*/ {
// /*AFLA*/ 	iter->next_index = iter->index;
// /*AFLA*/ 	iter->tags = 0;
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long
// /*AFLA*/ __radix_tree_iter_add(struct radix_tree_iter *iter, unsigned long slots)
// /*AFLA*/ {
// /*AFLA*/ 	return iter->index + (slots << iter_shift(iter));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * radix_tree_iter_next - resume iterating when the chunk may be invalid
// /*AFLA*/  * @iter:	iterator state
// /*AFLA*/  *
// /*AFLA*/  * If the iterator needs to release then reacquire a lock, the chunk may
// /*AFLA*/  * have been invalidated by an insertion or deletion.  Call this function
// /*AFLA*/  * to continue the iteration from the next index.
// /*AFLA*/  */
// /*AFLA*/ static inline __must_check
// /*AFLA*/ void **radix_tree_iter_next(struct radix_tree_iter *iter)
// /*AFLA*/ {
// /*AFLA*/ 	iter->next_index = __radix_tree_iter_add(iter, 1);
// /*AFLA*/ 	iter->tags = 0;
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * radix_tree_chunk_size - get current chunk size
// /*AFLA*/  *
// /*AFLA*/  * @iter:	pointer to radix tree iterator
// /*AFLA*/  * Returns:	current chunk size
// /*AFLA*/  */
// /*AFLA*/ static __always_inline long
// /*AFLA*/ radix_tree_chunk_size(struct radix_tree_iter *iter)
// /*AFLA*/ {
// /*AFLA*/ 	return (iter->next_index - iter->index) >> iter_shift(iter);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct radix_tree_node *entry_to_node(void *ptr)
// /*AFLA*/ {
// /*AFLA*/ 	return (void *)((unsigned long)ptr & ~RADIX_TREE_INTERNAL_NODE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * radix_tree_next_slot - find next slot in chunk
// /*AFLA*/  *
// /*AFLA*/  * @slot:	pointer to current slot
// /*AFLA*/  * @iter:	pointer to interator state
// /*AFLA*/  * @flags:	RADIX_TREE_ITER_*, should be constant
// /*AFLA*/  * Returns:	pointer to next slot, or NULL if there no more left
// /*AFLA*/  *
// /*AFLA*/  * This function updates @iter->index in the case of a successful lookup.
// /*AFLA*/  * For tagged lookup it also eats @iter->tags.
// /*AFLA*/  *
// /*AFLA*/  * There are several cases where 'slot' can be passed in as NULL to this
// /*AFLA*/  * function.  These cases result from the use of radix_tree_iter_next() or
// /*AFLA*/  * radix_tree_iter_retry().  In these cases we don't end up dereferencing
// /*AFLA*/  * 'slot' because either:
// /*AFLA*/  * a) we are doing tagged iteration and iter->tags has been set to 0, or
// /*AFLA*/  * b) we are doing non-tagged iteration, and iter->index and iter->next_index
// /*AFLA*/  *    have been set up so that radix_tree_chunk_size() returns 1 or 0.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void **
// /*AFLA*/ radix_tree_next_slot(void **slot, struct radix_tree_iter *iter, unsigned flags)
// /*AFLA*/ {
// /*AFLA*/ 	if (flags & RADIX_TREE_ITER_TAGGED) {
// /*AFLA*/ 		void *canon = slot;
// /*AFLA*/ 
// /*AFLA*/ 		iter->tags >>= 1;
// /*AFLA*/ 		if (unlikely(!iter->tags))
// /*AFLA*/ 			return NULL;
// /*AFLA*/ 		while (IS_ENABLED(CONFIG_RADIX_TREE_MULTIORDER) &&
// /*AFLA*/ 					radix_tree_is_internal_node(slot[1])) {
// /*AFLA*/ 			if (entry_to_node(slot[1]) == canon) {
// /*AFLA*/ 				iter->tags >>= 1;
// /*AFLA*/ 				iter->index = __radix_tree_iter_add(iter, 1);
// /*AFLA*/ 				slot++;
// /*AFLA*/ 				continue;
// /*AFLA*/ 			}
// /*AFLA*/ 			iter->next_index = __radix_tree_iter_add(iter, 1);
// /*AFLA*/ 			return NULL;
// /*AFLA*/ 		}
// /*AFLA*/ 		if (likely(iter->tags & 1ul)) {
// /*AFLA*/ 			iter->index = __radix_tree_iter_add(iter, 1);
// /*AFLA*/ 			return slot + 1;
// /*AFLA*/ 		}
// /*AFLA*/ 		if (!(flags & RADIX_TREE_ITER_CONTIG)) {
// /*AFLA*/ 			unsigned offset = __ffs(iter->tags);
// /*AFLA*/ 
// /*AFLA*/ 			iter->tags >>= offset;
// /*AFLA*/ 			iter->index = __radix_tree_iter_add(iter, offset + 1);
// /*AFLA*/ 			return slot + offset + 1;
// /*AFLA*/ 		}
// /*AFLA*/ 	} else {
// /*AFLA*/ 		long count = radix_tree_chunk_size(iter);
// /*AFLA*/ 		void *canon = slot;
// /*AFLA*/ 
// /*AFLA*/ 		while (--count > 0) {
// /*AFLA*/ 			slot++;
// /*AFLA*/ 			iter->index = __radix_tree_iter_add(iter, 1);
// /*AFLA*/ 
// /*AFLA*/ 			if (IS_ENABLED(CONFIG_RADIX_TREE_MULTIORDER) &&
// /*AFLA*/ 			    radix_tree_is_internal_node(*slot)) {
// /*AFLA*/ 				if (entry_to_node(*slot) == canon)
// /*AFLA*/ 					continue;
// /*AFLA*/ 				iter->next_index = iter->index;
// /*AFLA*/ 				break;
// /*AFLA*/ 			}
// /*AFLA*/ 
// /*AFLA*/ 			if (likely(*slot))
// /*AFLA*/ 				return slot;
// /*AFLA*/ 			if (flags & RADIX_TREE_ITER_CONTIG) {
// /*AFLA*/ 				/* forbid switching to the next chunk */
// /*AFLA*/ 				iter->next_index = 0;
// /*AFLA*/ 				break;
// /*AFLA*/ 			}
// /*AFLA*/ 		}
// /*AFLA*/ 	}
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * radix_tree_for_each_slot - iterate over non-empty slots
// /*AFLA*/  *
// /*AFLA*/  * @slot:	the void** variable for pointer to slot
// /*AFLA*/  * @root:	the struct radix_tree_root pointer
// /*AFLA*/  * @iter:	the struct radix_tree_iter pointer
// /*AFLA*/  * @start:	iteration starting index
// /*AFLA*/  *
// /*AFLA*/  * @slot points to radix tree slot, @iter->index contains its index.
// /*AFLA*/  */
// /*AFLA*/ #define radix_tree_for_each_slot(slot, root, iter, start)		\
// /*AFLA*/ 	for (slot = radix_tree_iter_init(iter, start) ;			\
// /*AFLA*/ 	     slot || (slot = radix_tree_next_chunk(root, iter, 0)) ;	\
// /*AFLA*/ 	     slot = radix_tree_next_slot(slot, iter, 0))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * radix_tree_for_each_contig - iterate over contiguous slots
// /*AFLA*/  *
// /*AFLA*/  * @slot:	the void** variable for pointer to slot
// /*AFLA*/  * @root:	the struct radix_tree_root pointer
// /*AFLA*/  * @iter:	the struct radix_tree_iter pointer
// /*AFLA*/  * @start:	iteration starting index
// /*AFLA*/  *
// /*AFLA*/  * @slot points to radix tree slot, @iter->index contains its index.
// /*AFLA*/  */
// /*AFLA*/ #define radix_tree_for_each_contig(slot, root, iter, start)		\
// /*AFLA*/ 	for (slot = radix_tree_iter_init(iter, start) ;			\
// /*AFLA*/ 	     slot || (slot = radix_tree_next_chunk(root, iter,		\
// /*AFLA*/ 				RADIX_TREE_ITER_CONTIG)) ;		\
// /*AFLA*/ 	     slot = radix_tree_next_slot(slot, iter,			\
// /*AFLA*/ 				RADIX_TREE_ITER_CONTIG))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * radix_tree_for_each_tagged - iterate over tagged slots
// /*AFLA*/  *
// /*AFLA*/  * @slot:	the void** variable for pointer to slot
// /*AFLA*/  * @root:	the struct radix_tree_root pointer
// /*AFLA*/  * @iter:	the struct radix_tree_iter pointer
// /*AFLA*/  * @start:	iteration starting index
// /*AFLA*/  * @tag:	tag index
// /*AFLA*/  *
// /*AFLA*/  * @slot points to radix tree slot, @iter->index contains its index.
// /*AFLA*/  */
// /*AFLA*/ #define radix_tree_for_each_tagged(slot, root, iter, start, tag)	\
// /*AFLA*/ 	for (slot = radix_tree_iter_init(iter, start) ;			\
// /*AFLA*/ 	     slot || (slot = radix_tree_next_chunk(root, iter,		\
// /*AFLA*/ 			      RADIX_TREE_ITER_TAGGED | tag)) ;		\
// /*AFLA*/ 	     slot = radix_tree_next_slot(slot, iter,			\
// /*AFLA*/ 				RADIX_TREE_ITER_TAGGED))
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_RADIX_TREE_H */
