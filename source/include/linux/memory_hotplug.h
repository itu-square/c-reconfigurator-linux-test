// /*AFLA*/ #ifndef __LINUX_MEMORY_HOTPLUG_H
// /*AFLA*/ #define __LINUX_MEMORY_HOTPLUG_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/mmzone.h>
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/notifier.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ 
// /*AFLA*/ struct page;
// /*AFLA*/ struct zone;
// /*AFLA*/ struct pglist_data;
// /*AFLA*/ struct mem_section;
// /*AFLA*/ struct memory_block;
// /*AFLA*/ struct resource;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MEMORY_HOTPLUG
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Types for free bootmem stored in page->lru.next. These have to be in
// /*AFLA*/  * some random range in unsigned long space for debugging purposes.
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	MEMORY_HOTPLUG_MIN_BOOTMEM_TYPE = 12,
// /*AFLA*/ 	SECTION_INFO = MEMORY_HOTPLUG_MIN_BOOTMEM_TYPE,
// /*AFLA*/ 	MIX_SECTION_INFO,
// /*AFLA*/ 	NODE_INFO,
// /*AFLA*/ 	MEMORY_HOTPLUG_MAX_BOOTMEM_TYPE = NODE_INFO,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Types for control the zone type of onlined and offlined memory */
// /*AFLA*/ enum {
// /*AFLA*/ 	MMOP_OFFLINE = -1,
// /*AFLA*/ 	MMOP_ONLINE_KEEP,
// /*AFLA*/ 	MMOP_ONLINE_KERNEL,
// /*AFLA*/ 	MMOP_ONLINE_MOVABLE,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * pgdat resizing functions
// /*AFLA*/  */
// /*AFLA*/ static inline
// /*AFLA*/ void pgdat_resize_lock(struct pglist_data *pgdat, unsigned long *flags)
// /*AFLA*/ {
// /*AFLA*/ 	spin_lock_irqsave(&pgdat->node_size_lock, *flags);
// /*AFLA*/ }
// /*AFLA*/ static inline
// /*AFLA*/ void pgdat_resize_unlock(struct pglist_data *pgdat, unsigned long *flags)
// /*AFLA*/ {
// /*AFLA*/ 	spin_unlock_irqrestore(&pgdat->node_size_lock, *flags);
// /*AFLA*/ }
// /*AFLA*/ static inline
// /*AFLA*/ void pgdat_resize_init(struct pglist_data *pgdat)
// /*AFLA*/ {
// /*AFLA*/ 	spin_lock_init(&pgdat->node_size_lock);
// /*AFLA*/ }
// /*AFLA*/ /*
// /*AFLA*/  * Zone resizing functions
// /*AFLA*/  *
// /*AFLA*/  * Note: any attempt to resize a zone should has pgdat_resize_lock()
// /*AFLA*/  * zone_span_writelock() both held. This ensure the size of a zone
// /*AFLA*/  * can't be changed while pgdat_resize_lock() held.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned zone_span_seqbegin(struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ 	return read_seqbegin(&zone->span_seqlock);
// /*AFLA*/ }
// /*AFLA*/ static inline int zone_span_seqretry(struct zone *zone, unsigned iv)
// /*AFLA*/ {
// /*AFLA*/ 	return read_seqretry(&zone->span_seqlock, iv);
// /*AFLA*/ }
// /*AFLA*/ static inline void zone_span_writelock(struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ 	write_seqlock(&zone->span_seqlock);
// /*AFLA*/ }
// /*AFLA*/ static inline void zone_span_writeunlock(struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ 	write_sequnlock(&zone->span_seqlock);
// /*AFLA*/ }
// /*AFLA*/ static inline void zone_seqlock_init(struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ 	seqlock_init(&zone->span_seqlock);
// /*AFLA*/ }
// /*AFLA*/ extern int zone_grow_free_lists(struct zone *zone, unsigned long new_nr_pages);
// /*AFLA*/ extern int zone_grow_waitqueues(struct zone *zone, unsigned long nr_pages);
// /*AFLA*/ extern int add_one_highpage(struct page *page, int pfn, int bad_ppro);
// /*AFLA*/ /* VM interface that may be used by firmware interface */
// /*AFLA*/ extern int online_pages(unsigned long, unsigned long, int);
// /*AFLA*/ extern int test_pages_in_a_zone(unsigned long, unsigned long);
// /*AFLA*/ extern void __offline_isolated_pages(unsigned long, unsigned long);
// /*AFLA*/ 
// /*AFLA*/ typedef void (*online_page_callback_t)(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ extern int set_online_page_callback(online_page_callback_t callback);
// /*AFLA*/ extern int restore_online_page_callback(online_page_callback_t callback);
// /*AFLA*/ 
// /*AFLA*/ extern void __online_page_set_limits(struct page *page);
// /*AFLA*/ extern void __online_page_increment_counters(struct page *page);
// /*AFLA*/ extern void __online_page_free(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ extern int try_online_node(int nid);
// /*AFLA*/ 
// /*AFLA*/ extern bool memhp_auto_online;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MEMORY_HOTREMOVE
// /*AFLA*/ extern bool is_pageblock_removable_nolock(struct page *page);
// /*AFLA*/ extern int arch_remove_memory(u64 start, u64 size);
// /*AFLA*/ extern int __remove_pages(struct zone *zone, unsigned long start_pfn,
// /*AFLA*/ 	unsigned long nr_pages);
// /*AFLA*/ #endif /* CONFIG_MEMORY_HOTREMOVE */
// /*AFLA*/ 
// /*AFLA*/ /* reasonably generic interface to expand the physical pages in a zone  */
// /*AFLA*/ extern int __add_pages(int nid, struct zone *zone, unsigned long start_pfn,
// /*AFLA*/ 	unsigned long nr_pages);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ extern int memory_add_physaddr_to_nid(u64 start);
// /*AFLA*/ #else
// /*AFLA*/ static inline int memory_add_physaddr_to_nid(u64 start)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_ARCH_NODEDATA_EXTENSION
// /*AFLA*/ /*
// /*AFLA*/  * For supporting node-hotadd, we have to allocate a new pgdat.
// /*AFLA*/  *
// /*AFLA*/  * If an arch has generic style NODE_DATA(),
// /*AFLA*/  * node_data[nid] = kzalloc() works well. But it depends on the architecture.
// /*AFLA*/  *
// /*AFLA*/  * In general, generic_alloc_nodedata() is used.
// /*AFLA*/  * Now, arch_free_nodedata() is just defined for error path of node_hot_add.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ extern pg_data_t *arch_alloc_nodedata(int nid);
// /*AFLA*/ extern void arch_free_nodedata(pg_data_t *pgdat);
// /*AFLA*/ extern void arch_refresh_nodedata(int nid, pg_data_t *pgdat);
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_HAVE_ARCH_NODEDATA_EXTENSION */
// /*AFLA*/ 
// /*AFLA*/ #define arch_alloc_nodedata(nid)	generic_alloc_nodedata(nid)
// /*AFLA*/ #define arch_free_nodedata(pgdat)	generic_free_nodedata(pgdat)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ /*
// /*AFLA*/  * If ARCH_HAS_NODEDATA_EXTENSION=n, this func is used to allocate pgdat.
// /*AFLA*/  * XXX: kmalloc_node() can't work well to get new node's memory at this time.
// /*AFLA*/  *	Because, pgdat for the new node is not allocated/initialized yet itself.
// /*AFLA*/  *	To use new node's memory, more consideration will be necessary.
// /*AFLA*/  */
// /*AFLA*/ #define generic_alloc_nodedata(nid)				\
// /*AFLA*/ ({								\
// /*AFLA*/ 	kzalloc(sizeof(pg_data_t), GFP_KERNEL);			\
// /*AFLA*/ })
// /*AFLA*/ /*
// /*AFLA*/  * This definition is just for error path in node hotadd.
// /*AFLA*/  * For node hotremove, we have to replace this.
// /*AFLA*/  */
// /*AFLA*/ #define generic_free_nodedata(pgdat)	kfree(pgdat)
// /*AFLA*/ 
// /*AFLA*/ extern pg_data_t *node_data[];
// /*AFLA*/ static inline void arch_refresh_nodedata(int nid, pg_data_t *pgdat)
// /*AFLA*/ {
// /*AFLA*/ 	node_data[nid] = pgdat;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_NUMA */
// /*AFLA*/ 
// /*AFLA*/ /* never called */
// /*AFLA*/ static inline pg_data_t *generic_alloc_nodedata(int nid)
// /*AFLA*/ {
// /*AFLA*/ 	BUG();
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ static inline void generic_free_nodedata(pg_data_t *pgdat)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void arch_refresh_nodedata(int nid, pg_data_t *pgdat)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_NUMA */
// /*AFLA*/ #endif /* CONFIG_HAVE_ARCH_NODEDATA_EXTENSION */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_BOOTMEM_INFO_NODE
// /*AFLA*/ extern void __init register_page_bootmem_info_node(struct pglist_data *pgdat);
// /*AFLA*/ #else
// /*AFLA*/ static inline void register_page_bootmem_info_node(struct pglist_data *pgdat)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ extern void put_page_bootmem(struct page *page);
// /*AFLA*/ extern void get_page_bootmem(unsigned long ingo, struct page *page,
// /*AFLA*/ 			     unsigned long type);
// /*AFLA*/ 
// /*AFLA*/ void get_online_mems(void);
// /*AFLA*/ void put_online_mems(void);
// /*AFLA*/ 
// /*AFLA*/ void mem_hotplug_begin(void);
// /*AFLA*/ void mem_hotplug_done(void);
// /*AFLA*/ 
// /*AFLA*/ extern void set_zone_contiguous(struct zone *zone);
// /*AFLA*/ extern void clear_zone_contiguous(struct zone *zone);
// /*AFLA*/ 
// /*AFLA*/ #else /* ! CONFIG_MEMORY_HOTPLUG */
// /*AFLA*/ /*
// /*AFLA*/  * Stub functions for when hotplug is off
// /*AFLA*/  */
// /*AFLA*/ static inline void pgdat_resize_lock(struct pglist_data *p, unsigned long *f) {}
// /*AFLA*/ static inline void pgdat_resize_unlock(struct pglist_data *p, unsigned long *f) {}
// /*AFLA*/ static inline void pgdat_resize_init(struct pglist_data *pgdat) {}
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned zone_span_seqbegin(struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int zone_span_seqretry(struct zone *zone, unsigned iv)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void zone_span_writelock(struct zone *zone) {}
// /*AFLA*/ static inline void zone_span_writeunlock(struct zone *zone) {}
// /*AFLA*/ static inline void zone_seqlock_init(struct zone *zone) {}
// /*AFLA*/ 
// /*AFLA*/ static inline int mhp_notimplemented(const char *func)
// /*AFLA*/ {
// /*AFLA*/ 	printk(KERN_WARNING "%s() called, with CONFIG_MEMORY_HOTPLUG disabled\n", func);
// /*AFLA*/ 	dump_stack();
// /*AFLA*/ 	return -ENOSYS;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void register_page_bootmem_info_node(struct pglist_data *pgdat)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int try_online_node(int nid)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void get_online_mems(void) {}
// /*AFLA*/ static inline void put_online_mems(void) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void mem_hotplug_begin(void) {}
// /*AFLA*/ static inline void mem_hotplug_done(void) {}
// /*AFLA*/ 
// /*AFLA*/ #endif /* ! CONFIG_MEMORY_HOTPLUG */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MEMORY_HOTREMOVE
// /*AFLA*/ 
// /*AFLA*/ extern bool is_mem_section_removable(unsigned long pfn, unsigned long nr_pages);
// /*AFLA*/ extern void try_offline_node(int nid);
// /*AFLA*/ extern int offline_pages(unsigned long start_pfn, unsigned long nr_pages);
// /*AFLA*/ extern void remove_memory(int nid, u64 start, u64 size);
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ static inline bool is_mem_section_removable(unsigned long pfn,
// /*AFLA*/ 					unsigned long nr_pages)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void try_offline_node(int nid) {}
// /*AFLA*/ 
// /*AFLA*/ static inline int offline_pages(unsigned long start_pfn, unsigned long nr_pages)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void remove_memory(int nid, u64 start, u64 size) {}
// /*AFLA*/ #endif /* CONFIG_MEMORY_HOTREMOVE */
// /*AFLA*/ 
// /*AFLA*/ extern int walk_memory_range(unsigned long start_pfn, unsigned long end_pfn,
// /*AFLA*/ 		void *arg, int (*func)(struct memory_block *, void *));
// /*AFLA*/ extern int add_memory(int nid, u64 start, u64 size);
// /*AFLA*/ extern int add_memory_resource(int nid, struct resource *resource, bool online);
// /*AFLA*/ extern int zone_for_memory(int nid, u64 start, u64 size, int zone_default,
// /*AFLA*/ 		bool for_device);
// /*AFLA*/ extern int arch_add_memory(int nid, u64 start, u64 size, bool for_device);
// /*AFLA*/ extern int offline_pages(unsigned long start_pfn, unsigned long nr_pages);
// /*AFLA*/ extern bool is_memblock_offlined(struct memory_block *mem);
// /*AFLA*/ extern void remove_memory(int nid, u64 start, u64 size);
// /*AFLA*/ extern int sparse_add_one_section(struct zone *zone, unsigned long start_pfn);
// /*AFLA*/ extern void sparse_remove_one_section(struct zone *zone, struct mem_section *ms,
// /*AFLA*/ 		unsigned long map_offset);
// /*AFLA*/ extern struct page *sparse_decode_mem_map(unsigned long coded_mem_map,
// /*AFLA*/ 					  unsigned long pnum);
// /*AFLA*/ extern int zone_can_shift(unsigned long pfn, unsigned long nr_pages,
// /*AFLA*/ 			  enum zone_type target);
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_MEMORY_HOTPLUG_H */
