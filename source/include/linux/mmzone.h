// /*AFLA*/ #ifndef _LINUX_MMZONE_H
// /*AFLA*/ #define _LINUX_MMZONE_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #ifndef __GENERATING_BOUNDS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/wait.h>
// /*AFLA*/ #include <linux/bitops.h>
// /*AFLA*/ #include <linux/cache.h>
// /*AFLA*/ #include <linux/threads.h>
// /*AFLA*/ #include <linux/numa.h>
// /*AFLA*/ #include <linux/init.h>
// /*AFLA*/ #include <linux/seqlock.h>
// /*AFLA*/ #include <linux/nodemask.h>
// /*AFLA*/ #include <linux/pageblock-flags.h>
// /*AFLA*/ #include <linux/page-flags-layout.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <asm/page.h>
// /*AFLA*/ 
// /*AFLA*/ /* Free memory management - zoned buddy allocator.  */
// /*AFLA*/ #ifndef CONFIG_FORCE_MAX_ZONEORDER
// /*AFLA*/ #define MAX_ORDER 11
// /*AFLA*/ #else
// /*AFLA*/ #define MAX_ORDER CONFIG_FORCE_MAX_ZONEORDER
// /*AFLA*/ #endif
// /*AFLA*/ #define MAX_ORDER_NR_PAGES (1 << (MAX_ORDER - 1))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * PAGE_ALLOC_COSTLY_ORDER is the order at which allocations are deemed
// /*AFLA*/  * costly to service.  That is between allocation orders which should
// /*AFLA*/  * coalesce naturally under reasonable reclaim pressure and those which
// /*AFLA*/  * will not.
// /*AFLA*/  */
// /*AFLA*/ #define PAGE_ALLOC_COSTLY_ORDER 3
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	MIGRATE_UNMOVABLE,
// /*AFLA*/ 	MIGRATE_MOVABLE,
// /*AFLA*/ 	MIGRATE_RECLAIMABLE,
// /*AFLA*/ 	MIGRATE_PCPTYPES,	/* the number of types on the pcp lists */
// /*AFLA*/ 	MIGRATE_HIGHATOMIC = MIGRATE_PCPTYPES,
// /*AFLA*/ #ifdef CONFIG_CMA
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * MIGRATE_CMA migration type is designed to mimic the way
// /*AFLA*/ 	 * ZONE_MOVABLE works.  Only movable pages can be allocated
// /*AFLA*/ 	 * from MIGRATE_CMA pageblocks and page allocator never
// /*AFLA*/ 	 * implicitly change migration type of MIGRATE_CMA pageblock.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * The way to use it is to change migratetype of a range of
// /*AFLA*/ 	 * pageblocks to MIGRATE_CMA which can be done by
// /*AFLA*/ 	 * __free_pageblock_cma() function.  What is important though
// /*AFLA*/ 	 * is that a range of pageblocks must be aligned to
// /*AFLA*/ 	 * MAX_ORDER_NR_PAGES should biggest page be bigger then
// /*AFLA*/ 	 * a single pageblock.
// /*AFLA*/ 	 */
// /*AFLA*/ 	MIGRATE_CMA,
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_MEMORY_ISOLATION
// /*AFLA*/ 	MIGRATE_ISOLATE,	/* can't allocate from here */
// /*AFLA*/ #endif
// /*AFLA*/ 	MIGRATE_TYPES
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* In mm/page_alloc.c; keep in sync also with show_migration_types() there */
// /*AFLA*/ extern char * const migratetype_names[MIGRATE_TYPES];
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_CMA
// /*AFLA*/ #  define is_migrate_cma(migratetype) unlikely((migratetype) == MIGRATE_CMA)
// /*AFLA*/ #  define is_migrate_cma_page(_page) (get_pageblock_migratetype(_page) == MIGRATE_CMA)
// /*AFLA*/ #else
// /*AFLA*/ #  define is_migrate_cma(migratetype) false
// /*AFLA*/ #  define is_migrate_cma_page(_page) false
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define for_each_migratetype_order(order, type) \
// /*AFLA*/ 	for (order = 0; order < MAX_ORDER; order++) \
// /*AFLA*/ 		for (type = 0; type < MIGRATE_TYPES; type++)
// /*AFLA*/ 
// /*AFLA*/ extern int page_group_by_mobility_disabled;
// /*AFLA*/ 
// /*AFLA*/ #define NR_MIGRATETYPE_BITS (PB_migrate_end - PB_migrate + 1)
// /*AFLA*/ #define MIGRATETYPE_MASK ((1UL << NR_MIGRATETYPE_BITS) - 1)
// /*AFLA*/ 
// /*AFLA*/ #define get_pageblock_migratetype(page)					\
// /*AFLA*/ 	get_pfnblock_flags_mask(page, page_to_pfn(page),		\
// /*AFLA*/ 			PB_migrate_end, MIGRATETYPE_MASK)
// /*AFLA*/ 
// /*AFLA*/ struct free_area {
// /*AFLA*/ 	struct list_head	free_list[MIGRATE_TYPES];
// /*AFLA*/ 	unsigned long		nr_free;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct pglist_data;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * zone->lock and the zone lru_lock are two of the hottest locks in the kernel.
// /*AFLA*/  * So add a wild amount of padding here to ensure that they fall into separate
// /*AFLA*/  * cachelines.  There are very few zone structures in the machine, so space
// /*AFLA*/  * consumption is not a concern here.
// /*AFLA*/  */
// /*AFLA*/ #if defined(CONFIG_SMP)
// /*AFLA*/ struct zone_padding {
// /*AFLA*/ 	char x[0];
// /*AFLA*/ } ____cacheline_internodealigned_in_smp;
// /*AFLA*/ #define ZONE_PADDING(name)	struct zone_padding name;
// /*AFLA*/ #else
// /*AFLA*/ #define ZONE_PADDING(name)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ enum zone_stat_item {
// /*AFLA*/ 	/* First 128 byte cacheline (assuming 64 bit words) */
// /*AFLA*/ 	NR_FREE_PAGES,
// /*AFLA*/ 	NR_ZONE_LRU_BASE, /* Used only for compaction and reclaim retry */
// /*AFLA*/ 	NR_ZONE_INACTIVE_ANON = NR_ZONE_LRU_BASE,
// /*AFLA*/ 	NR_ZONE_ACTIVE_ANON,
// /*AFLA*/ 	NR_ZONE_INACTIVE_FILE,
// /*AFLA*/ 	NR_ZONE_ACTIVE_FILE,
// /*AFLA*/ 	NR_ZONE_UNEVICTABLE,
// /*AFLA*/ 	NR_ZONE_WRITE_PENDING,	/* Count of dirty, writeback and unstable pages */
// /*AFLA*/ 	NR_MLOCK,		/* mlock()ed pages found and moved off LRU */
// /*AFLA*/ 	NR_SLAB_RECLAIMABLE,
// /*AFLA*/ 	NR_SLAB_UNRECLAIMABLE,
// /*AFLA*/ 	NR_PAGETABLE,		/* used for pagetables */
// /*AFLA*/ 	NR_KERNEL_STACK_KB,	/* measured in KiB */
// /*AFLA*/ 	/* Second 128 byte cacheline */
// /*AFLA*/ 	NR_BOUNCE,
// /*AFLA*/ #if IS_ENABLED(CONFIG_ZSMALLOC)
// /*AFLA*/ 	NR_ZSPAGES,		/* allocated in zsmalloc */
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ 	NUMA_HIT,		/* allocated in intended node */
// /*AFLA*/ 	NUMA_MISS,		/* allocated in non intended node */
// /*AFLA*/ 	NUMA_FOREIGN,		/* was intended here, hit elsewhere */
// /*AFLA*/ 	NUMA_INTERLEAVE_HIT,	/* interleaver preferred this zone */
// /*AFLA*/ 	NUMA_LOCAL,		/* allocation from local node */
// /*AFLA*/ 	NUMA_OTHER,		/* allocation from other node */
// /*AFLA*/ #endif
// /*AFLA*/ 	NR_FREE_CMA_PAGES,
// /*AFLA*/ 	NR_VM_ZONE_STAT_ITEMS };
// /*AFLA*/ 
// /*AFLA*/ enum node_stat_item {
// /*AFLA*/ 	NR_LRU_BASE,
// /*AFLA*/ 	NR_INACTIVE_ANON = NR_LRU_BASE, /* must match order of LRU_[IN]ACTIVE */
// /*AFLA*/ 	NR_ACTIVE_ANON,		/*  "     "     "   "       "         */
// /*AFLA*/ 	NR_INACTIVE_FILE,	/*  "     "     "   "       "         */
// /*AFLA*/ 	NR_ACTIVE_FILE,		/*  "     "     "   "       "         */
// /*AFLA*/ 	NR_UNEVICTABLE,		/*  "     "     "   "       "         */
// /*AFLA*/ 	NR_ISOLATED_ANON,	/* Temporary isolated pages from anon lru */
// /*AFLA*/ 	NR_ISOLATED_FILE,	/* Temporary isolated pages from file lru */
// /*AFLA*/ 	NR_PAGES_SCANNED,	/* pages scanned since last reclaim */
// /*AFLA*/ 	WORKINGSET_REFAULT,
// /*AFLA*/ 	WORKINGSET_ACTIVATE,
// /*AFLA*/ 	WORKINGSET_NODERECLAIM,
// /*AFLA*/ 	NR_ANON_MAPPED,	/* Mapped anonymous pages */
// /*AFLA*/ 	NR_FILE_MAPPED,	/* pagecache pages mapped into pagetables.
// /*AFLA*/ 			   only modified from process context */
// /*AFLA*/ 	NR_FILE_PAGES,
// /*AFLA*/ 	NR_FILE_DIRTY,
// /*AFLA*/ 	NR_WRITEBACK,
// /*AFLA*/ 	NR_WRITEBACK_TEMP,	/* Writeback using temporary buffers */
// /*AFLA*/ 	NR_SHMEM,		/* shmem pages (included tmpfs/GEM pages) */
// /*AFLA*/ 	NR_SHMEM_THPS,
// /*AFLA*/ 	NR_SHMEM_PMDMAPPED,
// /*AFLA*/ 	NR_ANON_THPS,
// /*AFLA*/ 	NR_UNSTABLE_NFS,	/* NFS unstable pages */
// /*AFLA*/ 	NR_VMSCAN_WRITE,
// /*AFLA*/ 	NR_VMSCAN_IMMEDIATE,	/* Prioritise for reclaim when writeback ends */
// /*AFLA*/ 	NR_DIRTIED,		/* page dirtyings since bootup */
// /*AFLA*/ 	NR_WRITTEN,		/* page writings since bootup */
// /*AFLA*/ 	NR_VM_NODE_STAT_ITEMS
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We do arithmetic on the LRU lists in various places in the code,
// /*AFLA*/  * so it is important to keep the active lists LRU_ACTIVE higher in
// /*AFLA*/  * the array than the corresponding inactive lists, and to keep
// /*AFLA*/  * the *_FILE lists LRU_FILE higher than the corresponding _ANON lists.
// /*AFLA*/  *
// /*AFLA*/  * This has to be kept in sync with the statistics in zone_stat_item
// /*AFLA*/  * above and the descriptions in vmstat_text in mm/vmstat.c
// /*AFLA*/  */
// /*AFLA*/ #define LRU_BASE 0
// /*AFLA*/ #define LRU_ACTIVE 1
// /*AFLA*/ #define LRU_FILE 2
// /*AFLA*/ 
// /*AFLA*/ enum lru_list {
// /*AFLA*/ 	LRU_INACTIVE_ANON = LRU_BASE,
// /*AFLA*/ 	LRU_ACTIVE_ANON = LRU_BASE + LRU_ACTIVE,
// /*AFLA*/ 	LRU_INACTIVE_FILE = LRU_BASE + LRU_FILE,
// /*AFLA*/ 	LRU_ACTIVE_FILE = LRU_BASE + LRU_FILE + LRU_ACTIVE,
// /*AFLA*/ 	LRU_UNEVICTABLE,
// /*AFLA*/ 	NR_LRU_LISTS
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define for_each_lru(lru) for (lru = 0; lru < NR_LRU_LISTS; lru++)
// /*AFLA*/ 
// /*AFLA*/ #define for_each_evictable_lru(lru) for (lru = 0; lru <= LRU_ACTIVE_FILE; lru++)
// /*AFLA*/ 
// /*AFLA*/ static inline int is_file_lru(enum lru_list lru)
// /*AFLA*/ {
// /*AFLA*/ 	return (lru == LRU_INACTIVE_FILE || lru == LRU_ACTIVE_FILE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int is_active_lru(enum lru_list lru)
// /*AFLA*/ {
// /*AFLA*/ 	return (lru == LRU_ACTIVE_ANON || lru == LRU_ACTIVE_FILE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct zone_reclaim_stat {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The pageout code in vmscan.c keeps track of how many of the
// /*AFLA*/ 	 * mem/swap backed and file backed pages are referenced.
// /*AFLA*/ 	 * The higher the rotated/scanned ratio, the more valuable
// /*AFLA*/ 	 * that cache is.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * The anon LRU stats live in [0], file LRU stats in [1]
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long		recent_rotated[2];
// /*AFLA*/ 	unsigned long		recent_scanned[2];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct lruvec {
// /*AFLA*/ 	struct list_head		lists[NR_LRU_LISTS];
// /*AFLA*/ 	struct zone_reclaim_stat	reclaim_stat;
// /*AFLA*/ 	/* Evictions & activations on the inactive file list */
// /*AFLA*/ 	atomic_long_t			inactive_age;
// /*AFLA*/ #ifdef CONFIG_MEMCG
// /*AFLA*/ 	struct pglist_data *pgdat;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Mask used at gathering information at once (see memcontrol.c) */
// /*AFLA*/ #define LRU_ALL_FILE (BIT(LRU_INACTIVE_FILE) | BIT(LRU_ACTIVE_FILE))
// /*AFLA*/ #define LRU_ALL_ANON (BIT(LRU_INACTIVE_ANON) | BIT(LRU_ACTIVE_ANON))
// /*AFLA*/ #define LRU_ALL	     ((1 << NR_LRU_LISTS) - 1)
// /*AFLA*/ 
// /*AFLA*/ /* Isolate clean file */
// /*AFLA*/ #define ISOLATE_CLEAN		((__force isolate_mode_t)0x1)
// /*AFLA*/ /* Isolate unmapped file */
// /*AFLA*/ #define ISOLATE_UNMAPPED	((__force isolate_mode_t)0x2)
// /*AFLA*/ /* Isolate for asynchronous migration */
// /*AFLA*/ #define ISOLATE_ASYNC_MIGRATE	((__force isolate_mode_t)0x4)
// /*AFLA*/ /* Isolate unevictable pages */
// /*AFLA*/ #define ISOLATE_UNEVICTABLE	((__force isolate_mode_t)0x8)
// /*AFLA*/ 
// /*AFLA*/ /* LRU Isolation modes. */
// /*AFLA*/ typedef unsigned __bitwise__ isolate_mode_t;
// /*AFLA*/ 
// /*AFLA*/ enum zone_watermarks {
// /*AFLA*/ 	WMARK_MIN,
// /*AFLA*/ 	WMARK_LOW,
// /*AFLA*/ 	WMARK_HIGH,
// /*AFLA*/ 	NR_WMARK
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define min_wmark_pages(z) (z->watermark[WMARK_MIN])
// /*AFLA*/ #define low_wmark_pages(z) (z->watermark[WMARK_LOW])
// /*AFLA*/ #define high_wmark_pages(z) (z->watermark[WMARK_HIGH])
// /*AFLA*/ 
// /*AFLA*/ struct per_cpu_pages {
// /*AFLA*/ 	int count;		/* number of pages in the list */
// /*AFLA*/ 	int high;		/* high watermark, emptying needed */
// /*AFLA*/ 	int batch;		/* chunk size for buddy add/remove */
// /*AFLA*/ 
// /*AFLA*/ 	/* Lists of pages, one per migrate type stored on the pcp-lists */
// /*AFLA*/ 	struct list_head lists[MIGRATE_PCPTYPES];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct per_cpu_pageset {
// /*AFLA*/ 	struct per_cpu_pages pcp;
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ 	s8 expire;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 	s8 stat_threshold;
// /*AFLA*/ 	s8 vm_stat_diff[NR_VM_ZONE_STAT_ITEMS];
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct per_cpu_nodestat {
// /*AFLA*/ 	s8 stat_threshold;
// /*AFLA*/ 	s8 vm_node_stat_diff[NR_VM_NODE_STAT_ITEMS];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* !__GENERATING_BOUNDS.H */
// /*AFLA*/ 
// /*AFLA*/ enum zone_type {
// /*AFLA*/ #ifdef CONFIG_ZONE_DMA
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * ZONE_DMA is used when there are devices that are not able
// /*AFLA*/ 	 * to do DMA to all of addressable memory (ZONE_NORMAL). Then we
// /*AFLA*/ 	 * carve out the portion of memory that is needed for these devices.
// /*AFLA*/ 	 * The range is arch specific.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Some examples
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Architecture		Limit
// /*AFLA*/ 	 * ---------------------------
// /*AFLA*/ 	 * parisc, ia64, sparc	<4G
// /*AFLA*/ 	 * s390			<2G
// /*AFLA*/ 	 * arm			Various
// /*AFLA*/ 	 * alpha		Unlimited or 0-16MB.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * i386, x86_64 and multiple other arches
// /*AFLA*/ 	 * 			<16M.
// /*AFLA*/ 	 */
// /*AFLA*/ 	ZONE_DMA,
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_ZONE_DMA32
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * x86_64 needs two ZONE_DMAs because it supports devices that are
// /*AFLA*/ 	 * only able to do DMA to the lower 16M but also 32 bit devices that
// /*AFLA*/ 	 * can only do DMA areas below 4G.
// /*AFLA*/ 	 */
// /*AFLA*/ 	ZONE_DMA32,
// /*AFLA*/ #endif
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Normal addressable memory is in ZONE_NORMAL. DMA operations can be
// /*AFLA*/ 	 * performed on pages in ZONE_NORMAL if the DMA devices support
// /*AFLA*/ 	 * transfers to all addressable memory.
// /*AFLA*/ 	 */
// /*AFLA*/ 	ZONE_NORMAL,
// /*AFLA*/ #ifdef CONFIG_HIGHMEM
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * A memory area that is only addressable by the kernel through
// /*AFLA*/ 	 * mapping portions into its own address space. This is for example
// /*AFLA*/ 	 * used by i386 to allow the kernel to address the memory beyond
// /*AFLA*/ 	 * 900MB. The kernel will set up special mappings (page
// /*AFLA*/ 	 * table entries on i386) for each page that the kernel needs to
// /*AFLA*/ 	 * access.
// /*AFLA*/ 	 */
// /*AFLA*/ 	ZONE_HIGHMEM,
// /*AFLA*/ #endif
// /*AFLA*/ 	ZONE_MOVABLE,
// /*AFLA*/ #ifdef CONFIG_ZONE_DEVICE
// /*AFLA*/ 	ZONE_DEVICE,
// /*AFLA*/ #endif
// /*AFLA*/ 	__MAX_NR_ZONES
// /*AFLA*/ 
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifndef __GENERATING_BOUNDS_H
// /*AFLA*/ 
// /*AFLA*/ struct zone {
// /*AFLA*/ 	/* Read-mostly fields */
// /*AFLA*/ 
// /*AFLA*/ 	/* zone watermarks, access with *_wmark_pages(zone) macros */
// /*AFLA*/ 	unsigned long watermark[NR_WMARK];
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long nr_reserved_highatomic;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * We don't know if the memory that we're going to allocate will be
// /*AFLA*/ 	 * freeable or/and it will be released eventually, so to avoid totally
// /*AFLA*/ 	 * wasting several GB of ram we must reserve some of the lower zone
// /*AFLA*/ 	 * memory (otherwise we risk to run OOM on the lower zones despite
// /*AFLA*/ 	 * there being tons of freeable ram on the higher zones).  This array is
// /*AFLA*/ 	 * recalculated at runtime if the sysctl_lowmem_reserve_ratio sysctl
// /*AFLA*/ 	 * changes.
// /*AFLA*/ 	 */
// /*AFLA*/ 	long lowmem_reserve[MAX_NR_ZONES];
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ 	int node;
// /*AFLA*/ #endif
// /*AFLA*/ 	struct pglist_data	*zone_pgdat;
// /*AFLA*/ 	struct per_cpu_pageset __percpu *pageset;
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_SPARSEMEM
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Flags for a pageblock_nr_pages block. See pageblock-flags.h.
// /*AFLA*/ 	 * In SPARSEMEM, this map is stored in struct mem_section
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long		*pageblock_flags;
// /*AFLA*/ #endif /* CONFIG_SPARSEMEM */
// /*AFLA*/ 
// /*AFLA*/ 	/* zone_start_pfn == zone_start_paddr >> PAGE_SHIFT */
// /*AFLA*/ 	unsigned long		zone_start_pfn;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * spanned_pages is the total pages spanned by the zone, including
// /*AFLA*/ 	 * holes, which is calculated as:
// /*AFLA*/ 	 * 	spanned_pages = zone_end_pfn - zone_start_pfn;
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * present_pages is physical pages existing within the zone, which
// /*AFLA*/ 	 * is calculated as:
// /*AFLA*/ 	 *	present_pages = spanned_pages - absent_pages(pages in holes);
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * managed_pages is present pages managed by the buddy system, which
// /*AFLA*/ 	 * is calculated as (reserved_pages includes pages allocated by the
// /*AFLA*/ 	 * bootmem allocator):
// /*AFLA*/ 	 *	managed_pages = present_pages - reserved_pages;
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * So present_pages may be used by memory hotplug or memory power
// /*AFLA*/ 	 * management logic to figure out unmanaged pages by checking
// /*AFLA*/ 	 * (present_pages - managed_pages). And managed_pages should be used
// /*AFLA*/ 	 * by page allocator and vm scanner to calculate all kinds of watermarks
// /*AFLA*/ 	 * and thresholds.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Locking rules:
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * zone_start_pfn and spanned_pages are protected by span_seqlock.
// /*AFLA*/ 	 * It is a seqlock because it has to be read outside of zone->lock,
// /*AFLA*/ 	 * and it is done in the main allocator path.  But, it is written
// /*AFLA*/ 	 * quite infrequently.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * The span_seq lock is declared along with zone->lock because it is
// /*AFLA*/ 	 * frequently read in proximity to zone->lock.  It's good to
// /*AFLA*/ 	 * give them a chance of being in the same cacheline.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Write access to present_pages at runtime should be protected by
// /*AFLA*/ 	 * mem_hotplug_begin/end(). Any reader who can't tolerant drift of
// /*AFLA*/ 	 * present_pages should get_online_mems() to get a stable value.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Read access to managed_pages should be safe because it's unsigned
// /*AFLA*/ 	 * long. Write access to zone->managed_pages and totalram_pages are
// /*AFLA*/ 	 * protected by managed_page_count_lock at runtime. Idealy only
// /*AFLA*/ 	 * adjust_managed_page_count() should be used instead of directly
// /*AFLA*/ 	 * touching zone->managed_pages and totalram_pages.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long		managed_pages;
// /*AFLA*/ 	unsigned long		spanned_pages;
// /*AFLA*/ 	unsigned long		present_pages;
// /*AFLA*/ 
// /*AFLA*/ 	const char		*name;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MEMORY_ISOLATION
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Number of isolated pageblock. It is used to solve incorrect
// /*AFLA*/ 	 * freepage counting problem due to racy retrieving migratetype
// /*AFLA*/ 	 * of pageblock. Protected by zone->lock.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long		nr_isolate_pageblock;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MEMORY_HOTPLUG
// /*AFLA*/ 	/* see spanned/present_pages for more description */
// /*AFLA*/ 	seqlock_t		span_seqlock;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	int initialized;
// /*AFLA*/ 
// /*AFLA*/ 	/* Write-intensive fields used from the page allocator */
// /*AFLA*/ 	ZONE_PADDING(_pad1_)
// /*AFLA*/ 
// /*AFLA*/ 	/* free areas of different sizes */
// /*AFLA*/ 	struct free_area	free_area[MAX_ORDER];
// /*AFLA*/ 
// /*AFLA*/ 	/* zone flags, see below */
// /*AFLA*/ 	unsigned long		flags;
// /*AFLA*/ 
// /*AFLA*/ 	/* Primarily protects free_area */
// /*AFLA*/ 	spinlock_t		lock;
// /*AFLA*/ 
// /*AFLA*/ 	/* Write-intensive fields used by compaction and vmstats. */
// /*AFLA*/ 	ZONE_PADDING(_pad2_)
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * When free pages are below this point, additional steps are taken
// /*AFLA*/ 	 * when reading the number of free pages to avoid per-cpu counter
// /*AFLA*/ 	 * drift allowing watermarks to be breached
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long percpu_drift_mark;
// /*AFLA*/ 
// /*AFLA*/ #if defined CONFIG_COMPACTION || defined CONFIG_CMA
// /*AFLA*/ 	/* pfn where compaction free scanner should start */
// /*AFLA*/ 	unsigned long		compact_cached_free_pfn;
// /*AFLA*/ 	/* pfn where async and sync compaction migration scanner should start */
// /*AFLA*/ 	unsigned long		compact_cached_migrate_pfn[2];
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_COMPACTION
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * On compaction failure, 1<<compact_defer_shift compactions
// /*AFLA*/ 	 * are skipped before trying again. The number attempted since
// /*AFLA*/ 	 * last failure is tracked with compact_considered.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned int		compact_considered;
// /*AFLA*/ 	unsigned int		compact_defer_shift;
// /*AFLA*/ 	int			compact_order_failed;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined CONFIG_COMPACTION || defined CONFIG_CMA
// /*AFLA*/ 	/* Set to true when the PG_migrate_skip bits should be cleared */
// /*AFLA*/ 	bool			compact_blockskip_flush;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	bool			contiguous;
// /*AFLA*/ 
// /*AFLA*/ 	ZONE_PADDING(_pad3_)
// /*AFLA*/ 	/* Zone statistics */
// /*AFLA*/ 	atomic_long_t		vm_stat[NR_VM_ZONE_STAT_ITEMS];
// /*AFLA*/ } ____cacheline_internodealigned_in_smp;
// /*AFLA*/ 
// /*AFLA*/ enum pgdat_flags {
// /*AFLA*/ 	PGDAT_CONGESTED,		/* pgdat has many dirty pages backed by
// /*AFLA*/ 					 * a congested BDI
// /*AFLA*/ 					 */
// /*AFLA*/ 	PGDAT_DIRTY,			/* reclaim scanning has recently found
// /*AFLA*/ 					 * many dirty file pages at the tail
// /*AFLA*/ 					 * of the LRU.
// /*AFLA*/ 					 */
// /*AFLA*/ 	PGDAT_WRITEBACK,		/* reclaim scanning has recently found
// /*AFLA*/ 					 * many pages under writeback
// /*AFLA*/ 					 */
// /*AFLA*/ 	PGDAT_RECLAIM_LOCKED,		/* prevents concurrent reclaim */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long zone_end_pfn(const struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ 	return zone->zone_start_pfn + zone->spanned_pages;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool zone_spans_pfn(const struct zone *zone, unsigned long pfn)
// /*AFLA*/ {
// /*AFLA*/ 	return zone->zone_start_pfn <= pfn && pfn < zone_end_pfn(zone);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool zone_is_initialized(struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ 	return zone->initialized;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool zone_is_empty(struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ 	return zone->spanned_pages == 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The "priority" of VM scanning is how much of the queues we will scan in one
// /*AFLA*/  * go. A value of 12 for DEF_PRIORITY implies that we will scan 1/4096th of the
// /*AFLA*/  * queues ("queue_length >> 12") during an aging round.
// /*AFLA*/  */
// /*AFLA*/ #define DEF_PRIORITY 12
// /*AFLA*/ 
// /*AFLA*/ /* Maximum number of zones on a zonelist */
// /*AFLA*/ #define MAX_ZONES_PER_ZONELIST (MAX_NUMNODES * MAX_NR_ZONES)
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	ZONELIST_FALLBACK,	/* zonelist with fallback */
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The NUMA zonelists are doubled because we need zonelists that
// /*AFLA*/ 	 * restrict the allocations to a single node for __GFP_THISNODE.
// /*AFLA*/ 	 */
// /*AFLA*/ 	ZONELIST_NOFALLBACK,	/* zonelist without fallback (__GFP_THISNODE) */
// /*AFLA*/ #endif
// /*AFLA*/ 	MAX_ZONELISTS
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This struct contains information about a zone in a zonelist. It is stored
// /*AFLA*/  * here to avoid dereferences into large structures and lookups of tables
// /*AFLA*/  */
// /*AFLA*/ struct zoneref {
// /*AFLA*/ 	struct zone *zone;	/* Pointer to actual zone */
// /*AFLA*/ 	int zone_idx;		/* zone_idx(zoneref->zone) */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * One allocation request operates on a zonelist. A zonelist
// /*AFLA*/  * is a list of zones, the first one is the 'goal' of the
// /*AFLA*/  * allocation, the other zones are fallback zones, in decreasing
// /*AFLA*/  * priority.
// /*AFLA*/  *
// /*AFLA*/  * To speed the reading of the zonelist, the zonerefs contain the zone index
// /*AFLA*/  * of the entry being read. Helper functions to access information given
// /*AFLA*/  * a struct zoneref are
// /*AFLA*/  *
// /*AFLA*/  * zonelist_zone()	- Return the struct zone * for an entry in _zonerefs
// /*AFLA*/  * zonelist_zone_idx()	- Return the index of the zone for an entry
// /*AFLA*/  * zonelist_node_idx()	- Return the index of the node for an entry
// /*AFLA*/  */
// /*AFLA*/ struct zonelist {
// /*AFLA*/ 	struct zoneref _zonerefs[MAX_ZONES_PER_ZONELIST + 1];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_DISCONTIGMEM
// /*AFLA*/ /* The array of struct pages - for discontigmem use pgdat->lmem_map */
// /*AFLA*/ extern struct page *mem_map;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The pg_data_t structure is used in machines with CONFIG_DISCONTIGMEM
// /*AFLA*/  * (mostly NUMA machines?) to denote a higher-level memory zone than the
// /*AFLA*/  * zone denotes.
// /*AFLA*/  *
// /*AFLA*/  * On NUMA machines, each NUMA node would have a pg_data_t to describe
// /*AFLA*/  * it's memory layout.
// /*AFLA*/  *
// /*AFLA*/  * Memory statistics and page replacement data structures are maintained on a
// /*AFLA*/  * per-zone basis.
// /*AFLA*/  */
// /*AFLA*/ struct bootmem_data;
// /*AFLA*/ typedef struct pglist_data {
// /*AFLA*/ 	struct zone node_zones[MAX_NR_ZONES];
// /*AFLA*/ 	struct zonelist node_zonelists[MAX_ZONELISTS];
// /*AFLA*/ 	int nr_zones;
// /*AFLA*/ #ifdef CONFIG_FLAT_NODE_MEM_MAP	/* means !SPARSEMEM */
// /*AFLA*/ 	struct page *node_mem_map;
// /*AFLA*/ #ifdef CONFIG_PAGE_EXTENSION
// /*AFLA*/ 	struct page_ext *node_page_ext;
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef CONFIG_NO_BOOTMEM
// /*AFLA*/ 	struct bootmem_data *bdata;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_MEMORY_HOTPLUG
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Must be held any time you expect node_start_pfn, node_present_pages
// /*AFLA*/ 	 * or node_spanned_pages stay constant.  Holding this will also
// /*AFLA*/ 	 * guarantee that any pfn_valid() stays that way.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * pgdat_resize_lock() and pgdat_resize_unlock() are provided to
// /*AFLA*/ 	 * manipulate node_size_lock without checking for CONFIG_MEMORY_HOTPLUG.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Nests above zone->lock and zone->span_seqlock
// /*AFLA*/ 	 */
// /*AFLA*/ 	spinlock_t node_size_lock;
// /*AFLA*/ #endif
// /*AFLA*/ 	unsigned long node_start_pfn;
// /*AFLA*/ 	unsigned long node_present_pages; /* total number of physical pages */
// /*AFLA*/ 	unsigned long node_spanned_pages; /* total size of physical page
// /*AFLA*/ 					     range, including holes */
// /*AFLA*/ 	int node_id;
// /*AFLA*/ 	wait_queue_head_t kswapd_wait;
// /*AFLA*/ 	wait_queue_head_t pfmemalloc_wait;
// /*AFLA*/ 	struct task_struct *kswapd;	/* Protected by
// /*AFLA*/ 					   mem_hotplug_begin/end() */
// /*AFLA*/ 	int kswapd_order;
// /*AFLA*/ 	enum zone_type kswapd_classzone_idx;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_COMPACTION
// /*AFLA*/ 	int kcompactd_max_order;
// /*AFLA*/ 	enum zone_type kcompactd_classzone_idx;
// /*AFLA*/ 	wait_queue_head_t kcompactd_wait;
// /*AFLA*/ 	struct task_struct *kcompactd;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_NUMA_BALANCING
// /*AFLA*/ 	/* Lock serializing the migrate rate limiting window */
// /*AFLA*/ 	spinlock_t numabalancing_migrate_lock;
// /*AFLA*/ 
// /*AFLA*/ 	/* Rate limiting time interval */
// /*AFLA*/ 	unsigned long numabalancing_migrate_next_window;
// /*AFLA*/ 
// /*AFLA*/ 	/* Number of pages migrated during the rate limiting time interval */
// /*AFLA*/ 	unsigned long numabalancing_migrate_nr_pages;
// /*AFLA*/ #endif
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * This is a per-node reserve of pages that are not available
// /*AFLA*/ 	 * to userspace allocations.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long		totalreserve_pages;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * zone reclaim becomes active if more unmapped pages exist.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long		min_unmapped_pages;
// /*AFLA*/ 	unsigned long		min_slab_pages;
// /*AFLA*/ #endif /* CONFIG_NUMA */
// /*AFLA*/ 
// /*AFLA*/ 	/* Write-intensive fields used by page reclaim */
// /*AFLA*/ 	ZONE_PADDING(_pad1_)
// /*AFLA*/ 	spinlock_t		lru_lock;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEFERRED_STRUCT_PAGE_INIT
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If memory initialisation on large machines is deferred then this
// /*AFLA*/ 	 * is the first PFN that needs to be initialised.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long first_deferred_pfn;
// /*AFLA*/ #endif /* CONFIG_DEFERRED_STRUCT_PAGE_INIT */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ 	spinlock_t split_queue_lock;
// /*AFLA*/ 	struct list_head split_queue;
// /*AFLA*/ 	unsigned long split_queue_len;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	/* Fields commonly accessed by the page reclaim scanner */
// /*AFLA*/ 	struct lruvec		lruvec;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The target ratio of ACTIVE_ANON to INACTIVE_ANON pages on
// /*AFLA*/ 	 * this node's LRU.  Maintained by the pageout code.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned int inactive_ratio;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long		flags;
// /*AFLA*/ 
// /*AFLA*/ 	ZONE_PADDING(_pad2_)
// /*AFLA*/ 
// /*AFLA*/ 	/* Per-node vmstats */
// /*AFLA*/ 	struct per_cpu_nodestat __percpu *per_cpu_nodestats;
// /*AFLA*/ 	atomic_long_t		vm_stat[NR_VM_NODE_STAT_ITEMS];
// /*AFLA*/ } pg_data_t;
// /*AFLA*/ 
// /*AFLA*/ #define node_present_pages(nid)	(NODE_DATA(nid)->node_present_pages)
// /*AFLA*/ #define node_spanned_pages(nid)	(NODE_DATA(nid)->node_spanned_pages)
// /*AFLA*/ #ifdef CONFIG_FLAT_NODE_MEM_MAP
// /*AFLA*/ #define pgdat_page_nr(pgdat, pagenr)	((pgdat)->node_mem_map + (pagenr))
// /*AFLA*/ #else
// /*AFLA*/ #define pgdat_page_nr(pgdat, pagenr)	pfn_to_page((pgdat)->node_start_pfn + (pagenr))
// /*AFLA*/ #endif
// /*AFLA*/ #define nid_page_nr(nid, pagenr) 	pgdat_page_nr(NODE_DATA(nid),(pagenr))
// /*AFLA*/ 
// /*AFLA*/ #define node_start_pfn(nid)	(NODE_DATA(nid)->node_start_pfn)
// /*AFLA*/ #define node_end_pfn(nid) pgdat_end_pfn(NODE_DATA(nid))
// /*AFLA*/ static inline spinlock_t *zone_lru_lock(struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ 	return &zone->zone_pgdat->lru_lock;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct lruvec *node_lruvec(struct pglist_data *pgdat)
// /*AFLA*/ {
// /*AFLA*/ 	return &pgdat->lruvec;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long pgdat_end_pfn(pg_data_t *pgdat)
// /*AFLA*/ {
// /*AFLA*/ 	return pgdat->node_start_pfn + pgdat->node_spanned_pages;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool pgdat_is_empty(pg_data_t *pgdat)
// /*AFLA*/ {
// /*AFLA*/ 	return !pgdat->node_start_pfn && !pgdat->node_spanned_pages;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int zone_id(const struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ 	struct pglist_data *pgdat = zone->zone_pgdat;
// /*AFLA*/ 
// /*AFLA*/ 	return zone - pgdat->node_zones;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_ZONE_DEVICE
// /*AFLA*/ static inline bool is_dev_zone(const struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ 	return zone_id(zone) == ZONE_DEVICE;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline bool is_dev_zone(const struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <linux/memory_hotplug.h>
// /*AFLA*/ 
// /*AFLA*/ extern struct mutex zonelists_mutex;
// /*AFLA*/ void build_all_zonelists(pg_data_t *pgdat, struct zone *zone);
// /*AFLA*/ void wakeup_kswapd(struct zone *zone, int order, enum zone_type classzone_idx);
// /*AFLA*/ bool __zone_watermark_ok(struct zone *z, unsigned int order, unsigned long mark,
// /*AFLA*/ 			 int classzone_idx, unsigned int alloc_flags,
// /*AFLA*/ 			 long free_pages);
// /*AFLA*/ bool zone_watermark_ok(struct zone *z, unsigned int order,
// /*AFLA*/ 		unsigned long mark, int classzone_idx,
// /*AFLA*/ 		unsigned int alloc_flags);
// /*AFLA*/ bool zone_watermark_ok_safe(struct zone *z, unsigned int order,
// /*AFLA*/ 		unsigned long mark, int classzone_idx);
// /*AFLA*/ enum memmap_context {
// /*AFLA*/ 	MEMMAP_EARLY,
// /*AFLA*/ 	MEMMAP_HOTPLUG,
// /*AFLA*/ };
// /*AFLA*/ extern int init_currently_empty_zone(struct zone *zone, unsigned long start_pfn,
// /*AFLA*/ 				     unsigned long size);
// /*AFLA*/ 
// /*AFLA*/ extern void lruvec_init(struct lruvec *lruvec);
// /*AFLA*/ 
// /*AFLA*/ static inline struct pglist_data *lruvec_pgdat(struct lruvec *lruvec)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_MEMCG
// /*AFLA*/ 	return lruvec->pgdat;
// /*AFLA*/ #else
// /*AFLA*/ 	return container_of(lruvec, struct pglist_data, lruvec);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long lruvec_lru_size(struct lruvec *lruvec, enum lru_list lru);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_MEMORY_PRESENT
// /*AFLA*/ void memory_present(int nid, unsigned long start, unsigned long end);
// /*AFLA*/ #else
// /*AFLA*/ static inline void memory_present(int nid, unsigned long start, unsigned long end) {}
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_MEMORYLESS_NODES
// /*AFLA*/ int local_memory_node(int node_id);
// /*AFLA*/ #else
// /*AFLA*/ static inline int local_memory_node(int node_id) { return node_id; };
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NEED_NODE_MEMMAP_SIZE
// /*AFLA*/ unsigned long __init node_memmap_size_bytes(int, unsigned long, unsigned long);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * zone_idx() returns 0 for the ZONE_DMA zone, 1 for the ZONE_NORMAL zone, etc.
// /*AFLA*/  */
// /*AFLA*/ #define zone_idx(zone)		((zone) - (zone)->zone_pgdat->node_zones)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Returns true if a zone has pages managed by the buddy allocator.
// /*AFLA*/  * All the reclaim decisions have to use this function rather than
// /*AFLA*/  * populated_zone(). If the whole zone is reserved then we can easily
// /*AFLA*/  * end up with populated_zone() && !managed_zone().
// /*AFLA*/  */
// /*AFLA*/ static inline bool managed_zone(struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ 	return zone->managed_pages;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Returns true if a zone has memory */
// /*AFLA*/ static inline bool populated_zone(struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ 	return zone->present_pages;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int movable_zone;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HIGHMEM
// /*AFLA*/ static inline int zone_movable_is_highmem(void)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_HAVE_MEMBLOCK_NODE_MAP
// /*AFLA*/ 	return movable_zone == ZONE_HIGHMEM;
// /*AFLA*/ #else
// /*AFLA*/ 	return (ZONE_MOVABLE - 1) == ZONE_HIGHMEM;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline int is_highmem_idx(enum zone_type idx)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_HIGHMEM
// /*AFLA*/ 	return (idx == ZONE_HIGHMEM ||
// /*AFLA*/ 		(idx == ZONE_MOVABLE && zone_movable_is_highmem()));
// /*AFLA*/ #else
// /*AFLA*/ 	return 0;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * is_highmem - helper function to quickly check if a struct zone is a 
// /*AFLA*/  *              highmem zone or not.  This is an attempt to keep references
// /*AFLA*/  *              to ZONE_{DMA/NORMAL/HIGHMEM/etc} in general code to a minimum.
// /*AFLA*/  * @zone - pointer to struct zone variable
// /*AFLA*/  */
// /*AFLA*/ static inline int is_highmem(struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_HIGHMEM
// /*AFLA*/ 	return is_highmem_idx(zone_idx(zone));
// /*AFLA*/ #else
// /*AFLA*/ 	return 0;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* These two functions are used to setup the per zone pages min values */
// /*AFLA*/ struct ctl_table;
// /*AFLA*/ int min_free_kbytes_sysctl_handler(struct ctl_table *, int,
// /*AFLA*/ 					void __user *, size_t *, loff_t *);
// /*AFLA*/ int watermark_scale_factor_sysctl_handler(struct ctl_table *, int,
// /*AFLA*/ 					void __user *, size_t *, loff_t *);
// /*AFLA*/ extern int sysctl_lowmem_reserve_ratio[MAX_NR_ZONES-1];
// /*AFLA*/ int lowmem_reserve_ratio_sysctl_handler(struct ctl_table *, int,
// /*AFLA*/ 					void __user *, size_t *, loff_t *);
// /*AFLA*/ int percpu_pagelist_fraction_sysctl_handler(struct ctl_table *, int,
// /*AFLA*/ 					void __user *, size_t *, loff_t *);
// /*AFLA*/ int sysctl_min_unmapped_ratio_sysctl_handler(struct ctl_table *, int,
// /*AFLA*/ 			void __user *, size_t *, loff_t *);
// /*AFLA*/ int sysctl_min_slab_ratio_sysctl_handler(struct ctl_table *, int,
// /*AFLA*/ 			void __user *, size_t *, loff_t *);
// /*AFLA*/ 
// /*AFLA*/ extern int numa_zonelist_order_handler(struct ctl_table *, int,
// /*AFLA*/ 			void __user *, size_t *, loff_t *);
// /*AFLA*/ extern char numa_zonelist_order[];
// /*AFLA*/ #define NUMA_ZONELIST_ORDER_LEN 16	/* string buffer size */
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_NEED_MULTIPLE_NODES
// /*AFLA*/ 
// /*AFLA*/ extern struct pglist_data contig_page_data;
// /*AFLA*/ #define NODE_DATA(nid)		(&contig_page_data)
// /*AFLA*/ #define NODE_MEM_MAP(nid)	mem_map
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_NEED_MULTIPLE_NODES */
// /*AFLA*/ 
// /*AFLA*/ #include <asm/mmzone.h>
// /*AFLA*/ 
// /*AFLA*/ #endif /* !CONFIG_NEED_MULTIPLE_NODES */
// /*AFLA*/ 
// /*AFLA*/ extern struct pglist_data *first_online_pgdat(void);
// /*AFLA*/ extern struct pglist_data *next_online_pgdat(struct pglist_data *pgdat);
// /*AFLA*/ extern struct zone *next_zone(struct zone *zone);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * for_each_online_pgdat - helper macro to iterate over all online nodes
// /*AFLA*/  * @pgdat - pointer to a pg_data_t variable
// /*AFLA*/  */
// /*AFLA*/ #define for_each_online_pgdat(pgdat)			\
// /*AFLA*/ 	for (pgdat = first_online_pgdat();		\
// /*AFLA*/ 	     pgdat;					\
// /*AFLA*/ 	     pgdat = next_online_pgdat(pgdat))
// /*AFLA*/ /**
// /*AFLA*/  * for_each_zone - helper macro to iterate over all memory zones
// /*AFLA*/  * @zone - pointer to struct zone variable
// /*AFLA*/  *
// /*AFLA*/  * The user only needs to declare the zone variable, for_each_zone
// /*AFLA*/  * fills it in.
// /*AFLA*/  */
// /*AFLA*/ #define for_each_zone(zone)			        \
// /*AFLA*/ 	for (zone = (first_online_pgdat())->node_zones; \
// /*AFLA*/ 	     zone;					\
// /*AFLA*/ 	     zone = next_zone(zone))
// /*AFLA*/ 
// /*AFLA*/ #define for_each_populated_zone(zone)		        \
// /*AFLA*/ 	for (zone = (first_online_pgdat())->node_zones; \
// /*AFLA*/ 	     zone;					\
// /*AFLA*/ 	     zone = next_zone(zone))			\
// /*AFLA*/ 		if (!populated_zone(zone))		\
// /*AFLA*/ 			; /* do nothing */		\
// /*AFLA*/ 		else
// /*AFLA*/ 
// /*AFLA*/ static inline struct zone *zonelist_zone(struct zoneref *zoneref)
// /*AFLA*/ {
// /*AFLA*/ 	return zoneref->zone;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int zonelist_zone_idx(struct zoneref *zoneref)
// /*AFLA*/ {
// /*AFLA*/ 	return zoneref->zone_idx;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int zonelist_node_idx(struct zoneref *zoneref)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ 	/* zone_to_nid not available in this context */
// /*AFLA*/ 	return zoneref->zone->node;
// /*AFLA*/ #else
// /*AFLA*/ 	return 0;
// /*AFLA*/ #endif /* CONFIG_NUMA */
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct zoneref *__next_zones_zonelist(struct zoneref *z,
// /*AFLA*/ 					enum zone_type highest_zoneidx,
// /*AFLA*/ 					nodemask_t *nodes);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * next_zones_zonelist - Returns the next zone at or below highest_zoneidx within the allowed nodemask using a cursor within a zonelist as a starting point
// /*AFLA*/  * @z - The cursor used as a starting point for the search
// /*AFLA*/  * @highest_zoneidx - The zone index of the highest zone to return
// /*AFLA*/  * @nodes - An optional nodemask to filter the zonelist with
// /*AFLA*/  *
// /*AFLA*/  * This function returns the next zone at or below a given zone index that is
// /*AFLA*/  * within the allowed nodemask using a cursor as the starting point for the
// /*AFLA*/  * search. The zoneref returned is a cursor that represents the current zone
// /*AFLA*/  * being examined. It should be advanced by one before calling
// /*AFLA*/  * next_zones_zonelist again.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline struct zoneref *next_zones_zonelist(struct zoneref *z,
// /*AFLA*/ 					enum zone_type highest_zoneidx,
// /*AFLA*/ 					nodemask_t *nodes)
// /*AFLA*/ {
// /*AFLA*/ 	if (likely(!nodes && zonelist_zone_idx(z) <= highest_zoneidx))
// /*AFLA*/ 		return z;
// /*AFLA*/ 	return __next_zones_zonelist(z, highest_zoneidx, nodes);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * first_zones_zonelist - Returns the first zone at or below highest_zoneidx within the allowed nodemask in a zonelist
// /*AFLA*/  * @zonelist - The zonelist to search for a suitable zone
// /*AFLA*/  * @highest_zoneidx - The zone index of the highest zone to return
// /*AFLA*/  * @nodes - An optional nodemask to filter the zonelist with
// /*AFLA*/  * @zone - The first suitable zone found is returned via this parameter
// /*AFLA*/  *
// /*AFLA*/  * This function returns the first zone at or below a given zone index that is
// /*AFLA*/  * within the allowed nodemask. The zoneref returned is a cursor that can be
// /*AFLA*/  * used to iterate the zonelist with next_zones_zonelist by advancing it by
// /*AFLA*/  * one before calling.
// /*AFLA*/  */
// /*AFLA*/ static inline struct zoneref *first_zones_zonelist(struct zonelist *zonelist,
// /*AFLA*/ 					enum zone_type highest_zoneidx,
// /*AFLA*/ 					nodemask_t *nodes)
// /*AFLA*/ {
// /*AFLA*/ 	return next_zones_zonelist(zonelist->_zonerefs,
// /*AFLA*/ 							highest_zoneidx, nodes);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * for_each_zone_zonelist_nodemask - helper macro to iterate over valid zones in a zonelist at or below a given zone index and within a nodemask
// /*AFLA*/  * @zone - The current zone in the iterator
// /*AFLA*/  * @z - The current pointer within zonelist->zones being iterated
// /*AFLA*/  * @zlist - The zonelist being iterated
// /*AFLA*/  * @highidx - The zone index of the highest zone to return
// /*AFLA*/  * @nodemask - Nodemask allowed by the allocator
// /*AFLA*/  *
// /*AFLA*/  * This iterator iterates though all zones at or below a given zone index and
// /*AFLA*/  * within a given nodemask
// /*AFLA*/  */
// /*AFLA*/ #define for_each_zone_zonelist_nodemask(zone, z, zlist, highidx, nodemask) \
// /*AFLA*/ 	for (z = first_zones_zonelist(zlist, highidx, nodemask), zone = zonelist_zone(z);	\
// /*AFLA*/ 		zone;							\
// /*AFLA*/ 		z = next_zones_zonelist(++z, highidx, nodemask),	\
// /*AFLA*/ 			zone = zonelist_zone(z))
// /*AFLA*/ 
// /*AFLA*/ #define for_next_zone_zonelist_nodemask(zone, z, zlist, highidx, nodemask) \
// /*AFLA*/ 	for (zone = z->zone;	\
// /*AFLA*/ 		zone;							\
// /*AFLA*/ 		z = next_zones_zonelist(++z, highidx, nodemask),	\
// /*AFLA*/ 			zone = zonelist_zone(z))
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * for_each_zone_zonelist - helper macro to iterate over valid zones in a zonelist at or below a given zone index
// /*AFLA*/  * @zone - The current zone in the iterator
// /*AFLA*/  * @z - The current pointer within zonelist->zones being iterated
// /*AFLA*/  * @zlist - The zonelist being iterated
// /*AFLA*/  * @highidx - The zone index of the highest zone to return
// /*AFLA*/  *
// /*AFLA*/  * This iterator iterates though all zones at or below a given zone index.
// /*AFLA*/  */
// /*AFLA*/ #define for_each_zone_zonelist(zone, z, zlist, highidx) \
// /*AFLA*/ 	for_each_zone_zonelist_nodemask(zone, z, zlist, highidx, NULL)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SPARSEMEM
// /*AFLA*/ #include <asm/sparsemem.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if !defined(CONFIG_HAVE_ARCH_EARLY_PFN_TO_NID) && \
// /*AFLA*/ 	!defined(CONFIG_HAVE_MEMBLOCK_NODE_MAP)
// /*AFLA*/ static inline unsigned long early_pfn_to_nid(unsigned long pfn)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FLATMEM
// /*AFLA*/ #define pfn_to_nid(pfn)		(0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SPARSEMEM
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * SECTION_SHIFT    		#bits space required to store a section #
// /*AFLA*/  *
// /*AFLA*/  * PA_SECTION_SHIFT		physical address to/from section number
// /*AFLA*/  * PFN_SECTION_SHIFT		pfn to/from section number
// /*AFLA*/  */
// /*AFLA*/ #define PA_SECTION_SHIFT	(SECTION_SIZE_BITS)
// /*AFLA*/ #define PFN_SECTION_SHIFT	(SECTION_SIZE_BITS - PAGE_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ #define NR_MEM_SECTIONS		(1UL << SECTIONS_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ #define PAGES_PER_SECTION       (1UL << PFN_SECTION_SHIFT)
// /*AFLA*/ #define PAGE_SECTION_MASK	(~(PAGES_PER_SECTION-1))
// /*AFLA*/ 
// /*AFLA*/ #define SECTION_BLOCKFLAGS_BITS \
// /*AFLA*/ 	((1UL << (PFN_SECTION_SHIFT - pageblock_order)) * NR_PAGEBLOCK_BITS)
// /*AFLA*/ 
// /*AFLA*/ #if (MAX_ORDER - 1 + PAGE_SHIFT) > SECTION_SIZE_BITS
// /*AFLA*/ #error Allocator MAX_ORDER exceeds SECTION_SIZE
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define pfn_to_section_nr(pfn) ((pfn) >> PFN_SECTION_SHIFT)
// /*AFLA*/ #define section_nr_to_pfn(sec) ((sec) << PFN_SECTION_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ #define SECTION_ALIGN_UP(pfn)	(((pfn) + PAGES_PER_SECTION - 1) & PAGE_SECTION_MASK)
// /*AFLA*/ #define SECTION_ALIGN_DOWN(pfn)	((pfn) & PAGE_SECTION_MASK)
// /*AFLA*/ 
// /*AFLA*/ struct page;
// /*AFLA*/ struct page_ext;
// /*AFLA*/ struct mem_section {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * This is, logically, a pointer to an array of struct
// /*AFLA*/ 	 * pages.  However, it is stored with some other magic.
// /*AFLA*/ 	 * (see sparse.c::sparse_init_one_section())
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Additionally during early boot we encode node id of
// /*AFLA*/ 	 * the location of the section here to guide allocation.
// /*AFLA*/ 	 * (see sparse.c::memory_present())
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Making it a UL at least makes someone do a cast
// /*AFLA*/ 	 * before using it wrong.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long section_mem_map;
// /*AFLA*/ 
// /*AFLA*/ 	/* See declaration of similar field in struct zone */
// /*AFLA*/ 	unsigned long *pageblock_flags;
// /*AFLA*/ #ifdef CONFIG_PAGE_EXTENSION
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If SPARSEMEM, pgdat doesn't have page_ext pointer. We use
// /*AFLA*/ 	 * section. (see page_ext.h about this.)
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct page_ext *page_ext;
// /*AFLA*/ 	unsigned long pad;
// /*AFLA*/ #endif
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * WARNING: mem_section must be a power-of-2 in size for the
// /*AFLA*/ 	 * calculation and use of SECTION_ROOT_MASK to make sense.
// /*AFLA*/ 	 */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SPARSEMEM_EXTREME
// /*AFLA*/ #define SECTIONS_PER_ROOT       (PAGE_SIZE / sizeof (struct mem_section))
// /*AFLA*/ #else
// /*AFLA*/ #define SECTIONS_PER_ROOT	1
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define SECTION_NR_TO_ROOT(sec)	((sec) / SECTIONS_PER_ROOT)
// /*AFLA*/ #define NR_SECTION_ROOTS	DIV_ROUND_UP(NR_MEM_SECTIONS, SECTIONS_PER_ROOT)
// /*AFLA*/ #define SECTION_ROOT_MASK	(SECTIONS_PER_ROOT - 1)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SPARSEMEM_EXTREME
// /*AFLA*/ extern struct mem_section *mem_section[NR_SECTION_ROOTS];
// /*AFLA*/ #else
// /*AFLA*/ extern struct mem_section mem_section[NR_SECTION_ROOTS][SECTIONS_PER_ROOT];
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline struct mem_section *__nr_to_section(unsigned long nr)
// /*AFLA*/ {
// /*AFLA*/ 	if (!mem_section[SECTION_NR_TO_ROOT(nr)])
// /*AFLA*/ 		return NULL;
// /*AFLA*/ 	return &mem_section[SECTION_NR_TO_ROOT(nr)][nr & SECTION_ROOT_MASK];
// /*AFLA*/ }
// /*AFLA*/ extern int __section_nr(struct mem_section* ms);
// /*AFLA*/ extern unsigned long usemap_size(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We use the lower bits of the mem_map pointer to store
// /*AFLA*/  * a little bit of information.  There should be at least
// /*AFLA*/  * 3 bits here due to 32-bit alignment.
// /*AFLA*/  */
// /*AFLA*/ #define	SECTION_MARKED_PRESENT	(1UL<<0)
// /*AFLA*/ #define SECTION_HAS_MEM_MAP	(1UL<<1)
// /*AFLA*/ #define SECTION_MAP_LAST_BIT	(1UL<<2)
// /*AFLA*/ #define SECTION_MAP_MASK	(~(SECTION_MAP_LAST_BIT-1))
// /*AFLA*/ #define SECTION_NID_SHIFT	2
// /*AFLA*/ 
// /*AFLA*/ static inline struct page *__section_mem_map_addr(struct mem_section *section)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long map = section->section_mem_map;
// /*AFLA*/ 	map &= SECTION_MAP_MASK;
// /*AFLA*/ 	return (struct page *)map;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int present_section(struct mem_section *section)
// /*AFLA*/ {
// /*AFLA*/ 	return (section && (section->section_mem_map & SECTION_MARKED_PRESENT));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int present_section_nr(unsigned long nr)
// /*AFLA*/ {
// /*AFLA*/ 	return present_section(__nr_to_section(nr));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int valid_section(struct mem_section *section)
// /*AFLA*/ {
// /*AFLA*/ 	return (section && (section->section_mem_map & SECTION_HAS_MEM_MAP));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int valid_section_nr(unsigned long nr)
// /*AFLA*/ {
// /*AFLA*/ 	return valid_section(__nr_to_section(nr));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct mem_section *__pfn_to_section(unsigned long pfn)
// /*AFLA*/ {
// /*AFLA*/ 	return __nr_to_section(pfn_to_section_nr(pfn));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_HAVE_ARCH_PFN_VALID
// /*AFLA*/ static inline int pfn_valid(unsigned long pfn)
// /*AFLA*/ {
// /*AFLA*/ 	if (pfn_to_section_nr(pfn) >= NR_MEM_SECTIONS)
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	return valid_section(__nr_to_section(pfn_to_section_nr(pfn)));
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline int pfn_present(unsigned long pfn)
// /*AFLA*/ {
// /*AFLA*/ 	if (pfn_to_section_nr(pfn) >= NR_MEM_SECTIONS)
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	return present_section(__nr_to_section(pfn_to_section_nr(pfn)));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These are _only_ used during initialisation, therefore they
// /*AFLA*/  * can use __initdata ...  They could have names to indicate
// /*AFLA*/  * this restriction.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ #define pfn_to_nid(pfn)							\
// /*AFLA*/ ({									\
// /*AFLA*/ 	unsigned long __pfn_to_nid_pfn = (pfn);				\
// /*AFLA*/ 	page_to_nid(pfn_to_page(__pfn_to_nid_pfn));			\
// /*AFLA*/ })
// /*AFLA*/ #else
// /*AFLA*/ #define pfn_to_nid(pfn)		(0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define early_pfn_valid(pfn)	pfn_valid(pfn)
// /*AFLA*/ void sparse_init(void);
// /*AFLA*/ #else
// /*AFLA*/ #define sparse_init()	do {} while (0)
// /*AFLA*/ #define sparse_index_init(_sec, _nid)  do {} while (0)
// /*AFLA*/ #endif /* CONFIG_SPARSEMEM */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * During memory init memblocks map pfns to nids. The search is expensive and
// /*AFLA*/  * this caches recent lookups. The implementation of __early_pfn_to_nid
// /*AFLA*/  * may treat start/end as pfns or sections.
// /*AFLA*/  */
// /*AFLA*/ struct mminit_pfnnid_cache {
// /*AFLA*/ 	unsigned long last_start;
// /*AFLA*/ 	unsigned long last_end;
// /*AFLA*/ 	int last_nid;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifndef early_pfn_valid
// /*AFLA*/ #define early_pfn_valid(pfn)	(1)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ void memory_present(int nid, unsigned long start, unsigned long end);
// /*AFLA*/ unsigned long __init node_memmap_size_bytes(int, unsigned long, unsigned long);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * If it is possible to have holes within a MAX_ORDER_NR_PAGES, then we
// /*AFLA*/  * need to check pfn validility within that MAX_ORDER_NR_PAGES block.
// /*AFLA*/  * pfn_valid_within() should be used in this case; we optimise this away
// /*AFLA*/  * when we have no holes within a MAX_ORDER_NR_PAGES block.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_HOLES_IN_ZONE
// /*AFLA*/ #define pfn_valid_within(pfn) pfn_valid(pfn)
// /*AFLA*/ #else
// /*AFLA*/ #define pfn_valid_within(pfn) (1)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_ARCH_HAS_HOLES_MEMORYMODEL
// /*AFLA*/ /*
// /*AFLA*/  * pfn_valid() is meant to be able to tell if a given PFN has valid memmap
// /*AFLA*/  * associated with it or not. In FLATMEM, it is expected that holes always
// /*AFLA*/  * have valid memmap as long as there is valid PFNs either side of the hole.
// /*AFLA*/  * In SPARSEMEM, it is assumed that a valid section has a memmap for the
// /*AFLA*/  * entire section.
// /*AFLA*/  *
// /*AFLA*/  * However, an ARM, and maybe other embedded architectures in the future
// /*AFLA*/  * free memmap backing holes to save memory on the assumption the memmap is
// /*AFLA*/  * never used. The page_zone linkages are then broken even though pfn_valid()
// /*AFLA*/  * returns true. A walker of the full memmap must then do this additional
// /*AFLA*/  * check to ensure the memmap they are looking at is sane by making sure
// /*AFLA*/  * the zone and PFN linkages are still valid. This is expensive, but walkers
// /*AFLA*/  * of the full memmap are extremely rare.
// /*AFLA*/  */
// /*AFLA*/ bool memmap_valid_within(unsigned long pfn,
// /*AFLA*/ 					struct page *page, struct zone *zone);
// /*AFLA*/ #else
// /*AFLA*/ static inline bool memmap_valid_within(unsigned long pfn,
// /*AFLA*/ 					struct page *page, struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_ARCH_HAS_HOLES_MEMORYMODEL */
// /*AFLA*/ 
// /*AFLA*/ #endif /* !__GENERATING_BOUNDS.H */
// /*AFLA*/ #endif /* !__ASSEMBLY__ */
// /*AFLA*/ #endif /* _LINUX_MMZONE_H */
