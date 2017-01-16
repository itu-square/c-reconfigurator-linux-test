// /*AFLA*/ /*
// /*AFLA*/  * Copyright (c) 2013 Red Hat, Inc. and Parallels Inc. All rights reserved.
// /*AFLA*/  * Authors: David Chinner and Glauber Costa
// /*AFLA*/  *
// /*AFLA*/  * Generic LRU infrastructure
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LRU_LIST_H
// /*AFLA*/ #define _LRU_LIST_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/nodemask.h>
// /*AFLA*/ #include <linux/shrinker.h>
// /*AFLA*/ 
// /*AFLA*/ struct mem_cgroup;
// /*AFLA*/ 
// /*AFLA*/ /* list_lru_walk_cb has to always return one of those */
// /*AFLA*/ enum lru_status {
// /*AFLA*/ 	LRU_REMOVED,		/* item removed from list */
// /*AFLA*/ 	LRU_REMOVED_RETRY,	/* item removed, but lock has been
// /*AFLA*/ 				   dropped and reacquired */
// /*AFLA*/ 	LRU_ROTATE,		/* item referenced, give another pass */
// /*AFLA*/ 	LRU_SKIP,		/* item cannot be locked, skip */
// /*AFLA*/ 	LRU_RETRY,		/* item not freeable. May drop the lock
// /*AFLA*/ 				   internally, but has to return locked. */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct list_lru_one {
// /*AFLA*/ 	struct list_head	list;
// /*AFLA*/ 	/* may become negative during memcg reparenting */
// /*AFLA*/ 	long			nr_items;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct list_lru_memcg {
// /*AFLA*/ 	/* array of per cgroup lists, indexed by memcg_cache_id */
// /*AFLA*/ 	struct list_lru_one	*lru[0];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct list_lru_node {
// /*AFLA*/ 	/* protects all lists on the node, including per cgroup */
// /*AFLA*/ 	spinlock_t		lock;
// /*AFLA*/ 	/* global list, used for the root cgroup in cgroup aware lrus */
// /*AFLA*/ 	struct list_lru_one	lru;
// /*AFLA*/ #if defined(CONFIG_MEMCG) && !defined(CONFIG_SLOB)
// /*AFLA*/ 	/* for cgroup aware lrus points to per cgroup lists, otherwise NULL */
// /*AFLA*/ 	struct list_lru_memcg	*memcg_lrus;
// /*AFLA*/ #endif
// /*AFLA*/ } ____cacheline_aligned_in_smp;
// /*AFLA*/ 
// /*AFLA*/ struct list_lru {
// /*AFLA*/ 	struct list_lru_node	*node;
// /*AFLA*/ #if defined(CONFIG_MEMCG) && !defined(CONFIG_SLOB)
// /*AFLA*/ 	struct list_head	list;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ void list_lru_destroy(struct list_lru *lru);
// /*AFLA*/ int __list_lru_init(struct list_lru *lru, bool memcg_aware,
// /*AFLA*/ 		    struct lock_class_key *key);
// /*AFLA*/ 
// /*AFLA*/ #define list_lru_init(lru)		__list_lru_init((lru), false, NULL)
// /*AFLA*/ #define list_lru_init_key(lru, key)	__list_lru_init((lru), false, (key))
// /*AFLA*/ #define list_lru_init_memcg(lru)	__list_lru_init((lru), true, NULL)
// /*AFLA*/ 
// /*AFLA*/ int memcg_update_all_list_lrus(int num_memcgs);
// /*AFLA*/ void memcg_drain_all_list_lrus(int src_idx, int dst_idx);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_lru_add: add an element to the lru list's tail
// /*AFLA*/  * @list_lru: the lru pointer
// /*AFLA*/  * @item: the item to be added.
// /*AFLA*/  *
// /*AFLA*/  * If the element is already part of a list, this function returns doing
// /*AFLA*/  * nothing. Therefore the caller does not need to keep state about whether or
// /*AFLA*/  * not the element already belongs in the list and is allowed to lazy update
// /*AFLA*/  * it. Note however that this is valid for *a* list, not *this* list. If
// /*AFLA*/  * the caller organize itself in a way that elements can be in more than
// /*AFLA*/  * one type of list, it is up to the caller to fully remove the item from
// /*AFLA*/  * the previous list (with list_lru_del() for instance) before moving it
// /*AFLA*/  * to @list_lru
// /*AFLA*/  *
// /*AFLA*/  * Return value: true if the list was updated, false otherwise
// /*AFLA*/  */
// /*AFLA*/ bool list_lru_add(struct list_lru *lru, struct list_head *item);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_lru_del: delete an element to the lru list
// /*AFLA*/  * @list_lru: the lru pointer
// /*AFLA*/  * @item: the item to be deleted.
// /*AFLA*/  *
// /*AFLA*/  * This function works analogously as list_lru_add in terms of list
// /*AFLA*/  * manipulation. The comments about an element already pertaining to
// /*AFLA*/  * a list are also valid for list_lru_del.
// /*AFLA*/  *
// /*AFLA*/  * Return value: true if the list was updated, false otherwise
// /*AFLA*/  */
// /*AFLA*/ bool list_lru_del(struct list_lru *lru, struct list_head *item);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_lru_count_one: return the number of objects currently held by @lru
// /*AFLA*/  * @lru: the lru pointer.
// /*AFLA*/  * @nid: the node id to count from.
// /*AFLA*/  * @memcg: the cgroup to count from.
// /*AFLA*/  *
// /*AFLA*/  * Always return a non-negative number, 0 for empty lists. There is no
// /*AFLA*/  * guarantee that the list is not updated while the count is being computed.
// /*AFLA*/  * Callers that want such a guarantee need to provide an outer lock.
// /*AFLA*/  */
// /*AFLA*/ unsigned long list_lru_count_one(struct list_lru *lru,
// /*AFLA*/ 				 int nid, struct mem_cgroup *memcg);
// /*AFLA*/ unsigned long list_lru_count_node(struct list_lru *lru, int nid);
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long list_lru_shrink_count(struct list_lru *lru,
// /*AFLA*/ 						  struct shrink_control *sc)
// /*AFLA*/ {
// /*AFLA*/ 	return list_lru_count_one(lru, sc->nid, sc->memcg);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long list_lru_count(struct list_lru *lru)
// /*AFLA*/ {
// /*AFLA*/ 	long count = 0;
// /*AFLA*/ 	int nid;
// /*AFLA*/ 
// /*AFLA*/ 	for_each_node_state(nid, N_NORMAL_MEMORY)
// /*AFLA*/ 		count += list_lru_count_node(lru, nid);
// /*AFLA*/ 
// /*AFLA*/ 	return count;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void list_lru_isolate(struct list_lru_one *list, struct list_head *item);
// /*AFLA*/ void list_lru_isolate_move(struct list_lru_one *list, struct list_head *item,
// /*AFLA*/ 			   struct list_head *head);
// /*AFLA*/ 
// /*AFLA*/ typedef enum lru_status (*list_lru_walk_cb)(struct list_head *item,
// /*AFLA*/ 		struct list_lru_one *list, spinlock_t *lock, void *cb_arg);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * list_lru_walk_one: walk a list_lru, isolating and disposing freeable items.
// /*AFLA*/  * @lru: the lru pointer.
// /*AFLA*/  * @nid: the node id to scan from.
// /*AFLA*/  * @memcg: the cgroup to scan from.
// /*AFLA*/  * @isolate: callback function that is resposible for deciding what to do with
// /*AFLA*/  *  the item currently being scanned
// /*AFLA*/  * @cb_arg: opaque type that will be passed to @isolate
// /*AFLA*/  * @nr_to_walk: how many items to scan.
// /*AFLA*/  *
// /*AFLA*/  * This function will scan all elements in a particular list_lru, calling the
// /*AFLA*/  * @isolate callback for each of those items, along with the current list
// /*AFLA*/  * spinlock and a caller-provided opaque. The @isolate callback can choose to
// /*AFLA*/  * drop the lock internally, but *must* return with the lock held. The callback
// /*AFLA*/  * will return an enum lru_status telling the list_lru infrastructure what to
// /*AFLA*/  * do with the object being scanned.
// /*AFLA*/  *
// /*AFLA*/  * Please note that nr_to_walk does not mean how many objects will be freed,
// /*AFLA*/  * just how many objects will be scanned.
// /*AFLA*/  *
// /*AFLA*/  * Return value: the number of objects effectively removed from the LRU.
// /*AFLA*/  */
// /*AFLA*/ unsigned long list_lru_walk_one(struct list_lru *lru,
// /*AFLA*/ 				int nid, struct mem_cgroup *memcg,
// /*AFLA*/ 				list_lru_walk_cb isolate, void *cb_arg,
// /*AFLA*/ 				unsigned long *nr_to_walk);
// /*AFLA*/ unsigned long list_lru_walk_node(struct list_lru *lru, int nid,
// /*AFLA*/ 				 list_lru_walk_cb isolate, void *cb_arg,
// /*AFLA*/ 				 unsigned long *nr_to_walk);
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long
// /*AFLA*/ list_lru_shrink_walk(struct list_lru *lru, struct shrink_control *sc,
// /*AFLA*/ 		     list_lru_walk_cb isolate, void *cb_arg)
// /*AFLA*/ {
// /*AFLA*/ 	return list_lru_walk_one(lru, sc->nid, sc->memcg, isolate, cb_arg,
// /*AFLA*/ 				 &sc->nr_to_scan);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long
// /*AFLA*/ list_lru_walk(struct list_lru *lru, list_lru_walk_cb isolate,
// /*AFLA*/ 	      void *cb_arg, unsigned long nr_to_walk)
// /*AFLA*/ {
// /*AFLA*/ 	long isolated = 0;
// /*AFLA*/ 	int nid;
// /*AFLA*/ 
// /*AFLA*/ 	for_each_node_state(nid, N_NORMAL_MEMORY) {
// /*AFLA*/ 		isolated += list_lru_walk_node(lru, nid, isolate,
// /*AFLA*/ 					       cb_arg, &nr_to_walk);
// /*AFLA*/ 		if (nr_to_walk <= 0)
// /*AFLA*/ 			break;
// /*AFLA*/ 	}
// /*AFLA*/ 	return isolated;
// /*AFLA*/ }
// /*AFLA*/ #endif /* _LRU_LIST_H */
