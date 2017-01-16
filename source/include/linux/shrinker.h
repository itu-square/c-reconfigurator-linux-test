// /*AFLA*/ #ifndef _LINUX_SHRINKER_H
// /*AFLA*/ #define _LINUX_SHRINKER_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This struct is used to pass information from page reclaim to the shrinkers.
// /*AFLA*/  * We consolidate the values for easier extention later.
// /*AFLA*/  *
// /*AFLA*/  * The 'gfpmask' refers to the allocation we are currently trying to
// /*AFLA*/  * fulfil.
// /*AFLA*/  */
// /*AFLA*/ struct shrink_control {
// /*AFLA*/ 	gfp_t gfp_mask;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * How many objects scan_objects should scan and try to reclaim.
// /*AFLA*/ 	 * This is reset before every call, so it is safe for callees
// /*AFLA*/ 	 * to modify.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long nr_to_scan;
// /*AFLA*/ 
// /*AFLA*/ 	/* current node being shrunk (for NUMA aware shrinkers) */
// /*AFLA*/ 	int nid;
// /*AFLA*/ 
// /*AFLA*/ 	/* current memcg being shrunk (for memcg aware shrinkers) */
// /*AFLA*/ 	struct mem_cgroup *memcg;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define SHRINK_STOP (~0UL)
// /*AFLA*/ /*
// /*AFLA*/  * A callback you can register to apply pressure to ageable caches.
// /*AFLA*/  *
// /*AFLA*/  * @count_objects should return the number of freeable items in the cache. If
// /*AFLA*/  * there are no objects to free or the number of freeable items cannot be
// /*AFLA*/  * determined, it should return 0. No deadlock checks should be done during the
// /*AFLA*/  * count callback - the shrinker relies on aggregating scan counts that couldn't
// /*AFLA*/  * be executed due to potential deadlocks to be run at a later call when the
// /*AFLA*/  * deadlock condition is no longer pending.
// /*AFLA*/  *
// /*AFLA*/  * @scan_objects will only be called if @count_objects returned a non-zero
// /*AFLA*/  * value for the number of freeable objects. The callout should scan the cache
// /*AFLA*/  * and attempt to free items from the cache. It should then return the number
// /*AFLA*/  * of objects freed during the scan, or SHRINK_STOP if progress cannot be made
// /*AFLA*/  * due to potential deadlocks. If SHRINK_STOP is returned, then no further
// /*AFLA*/  * attempts to call the @scan_objects will be made from the current reclaim
// /*AFLA*/  * context.
// /*AFLA*/  *
// /*AFLA*/  * @flags determine the shrinker abilities, like numa awareness
// /*AFLA*/  */
// /*AFLA*/ struct shrinker {
// /*AFLA*/ 	unsigned long (*count_objects)(struct shrinker *,
// /*AFLA*/ 				       struct shrink_control *sc);
// /*AFLA*/ 	unsigned long (*scan_objects)(struct shrinker *,
// /*AFLA*/ 				      struct shrink_control *sc);
// /*AFLA*/ 
// /*AFLA*/ 	int seeks;	/* seeks to recreate an obj */
// /*AFLA*/ 	long batch;	/* reclaim batch size, 0 = default */
// /*AFLA*/ 	unsigned long flags;
// /*AFLA*/ 
// /*AFLA*/ 	/* These are for internal use */
// /*AFLA*/ 	struct list_head list;
// /*AFLA*/ 	/* objs pending delete, per node */
// /*AFLA*/ 	atomic_long_t *nr_deferred;
// /*AFLA*/ };
// /*AFLA*/ #define DEFAULT_SEEKS 2 /* A good number if you don't know better. */
// /*AFLA*/ 
// /*AFLA*/ /* Flags */
// /*AFLA*/ #define SHRINKER_NUMA_AWARE	(1 << 0)
// /*AFLA*/ #define SHRINKER_MEMCG_AWARE	(1 << 1)
// /*AFLA*/ 
// /*AFLA*/ extern int register_shrinker(struct shrinker *);
// /*AFLA*/ extern void unregister_shrinker(struct shrinker *);
// /*AFLA*/ #endif
