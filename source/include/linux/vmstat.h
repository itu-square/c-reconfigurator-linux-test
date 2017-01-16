// /*AFLA*/ #ifndef _LINUX_VMSTAT_H
// /*AFLA*/ #define _LINUX_VMSTAT_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/percpu.h>
// /*AFLA*/ #include <linux/mm.h>
// /*AFLA*/ #include <linux/mmzone.h>
// /*AFLA*/ #include <linux/vm_event_item.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ 
// /*AFLA*/ extern int sysctl_stat_interval;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_VM_EVENT_COUNTERS
// /*AFLA*/ /*
// /*AFLA*/  * Light weight per cpu counter implementation.
// /*AFLA*/  *
// /*AFLA*/  * Counters should only be incremented and no critical kernel component
// /*AFLA*/  * should rely on the counter values.
// /*AFLA*/  *
// /*AFLA*/  * Counters are handled completely inline. On many platforms the code
// /*AFLA*/  * generated will simply be the increment of a global address.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct vm_event_state {
// /*AFLA*/ 	unsigned long event[NR_VM_EVENT_ITEMS];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ DECLARE_PER_CPU(struct vm_event_state, vm_event_states);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * vm counters are allowed to be racy. Use raw_cpu_ops to avoid the
// /*AFLA*/  * local_irq_disable overhead.
// /*AFLA*/  */
// /*AFLA*/ static inline void __count_vm_event(enum vm_event_item item)
// /*AFLA*/ {
// /*AFLA*/ 	raw_cpu_inc(vm_event_states.event[item]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void count_vm_event(enum vm_event_item item)
// /*AFLA*/ {
// /*AFLA*/ 	this_cpu_inc(vm_event_states.event[item]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __count_vm_events(enum vm_event_item item, long delta)
// /*AFLA*/ {
// /*AFLA*/ 	raw_cpu_add(vm_event_states.event[item], delta);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void count_vm_events(enum vm_event_item item, long delta)
// /*AFLA*/ {
// /*AFLA*/ 	this_cpu_add(vm_event_states.event[item], delta);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void all_vm_events(unsigned long *);
// /*AFLA*/ 
// /*AFLA*/ extern void vm_events_fold_cpu(int cpu);
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ /* Disable counters */
// /*AFLA*/ static inline void count_vm_event(enum vm_event_item item)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void count_vm_events(enum vm_event_item item, long delta)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void __count_vm_event(enum vm_event_item item)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void __count_vm_events(enum vm_event_item item, long delta)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void all_vm_events(unsigned long *ret)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void vm_events_fold_cpu(int cpu)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_VM_EVENT_COUNTERS */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA_BALANCING
// /*AFLA*/ #define count_vm_numa_event(x)     count_vm_event(x)
// /*AFLA*/ #define count_vm_numa_events(x, y) count_vm_events(x, y)
// /*AFLA*/ #else
// /*AFLA*/ #define count_vm_numa_event(x) do {} while (0)
// /*AFLA*/ #define count_vm_numa_events(x, y) do { (void)(y); } while (0)
// /*AFLA*/ #endif /* CONFIG_NUMA_BALANCING */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_TLBFLUSH
// /*AFLA*/ #define count_vm_tlb_event(x)	   count_vm_event(x)
// /*AFLA*/ #define count_vm_tlb_events(x, y)  count_vm_events(x, y)
// /*AFLA*/ #else
// /*AFLA*/ #define count_vm_tlb_event(x)     do {} while (0)
// /*AFLA*/ #define count_vm_tlb_events(x, y) do { (void)(y); } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_VM_VMACACHE
// /*AFLA*/ #define count_vm_vmacache_event(x) count_vm_event(x)
// /*AFLA*/ #else
// /*AFLA*/ #define count_vm_vmacache_event(x) do {} while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __count_zid_vm_events(item, zid, delta) \
// /*AFLA*/ 	__count_vm_events(item##_NORMAL - ZONE_NORMAL + zid, delta)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Zone and node-based page accounting with per cpu differentials.
// /*AFLA*/  */
// /*AFLA*/ extern atomic_long_t vm_zone_stat[NR_VM_ZONE_STAT_ITEMS];
// /*AFLA*/ extern atomic_long_t vm_node_stat[NR_VM_NODE_STAT_ITEMS];
// /*AFLA*/ 
// /*AFLA*/ static inline void zone_page_state_add(long x, struct zone *zone,
// /*AFLA*/ 				 enum zone_stat_item item)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_long_add(x, &zone->vm_stat[item]);
// /*AFLA*/ 	atomic_long_add(x, &vm_zone_stat[item]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void node_page_state_add(long x, struct pglist_data *pgdat,
// /*AFLA*/ 				 enum node_stat_item item)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_long_add(x, &pgdat->vm_stat[item]);
// /*AFLA*/ 	atomic_long_add(x, &vm_node_stat[item]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long global_page_state(enum zone_stat_item item)
// /*AFLA*/ {
// /*AFLA*/ 	long x = atomic_long_read(&vm_zone_stat[item]);
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 	if (x < 0)
// /*AFLA*/ 		x = 0;
// /*AFLA*/ #endif
// /*AFLA*/ 	return x;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long global_node_page_state(enum node_stat_item item)
// /*AFLA*/ {
// /*AFLA*/ 	long x = atomic_long_read(&vm_node_stat[item]);
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 	if (x < 0)
// /*AFLA*/ 		x = 0;
// /*AFLA*/ #endif
// /*AFLA*/ 	return x;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long zone_page_state(struct zone *zone,
// /*AFLA*/ 					enum zone_stat_item item)
// /*AFLA*/ {
// /*AFLA*/ 	long x = atomic_long_read(&zone->vm_stat[item]);
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 	if (x < 0)
// /*AFLA*/ 		x = 0;
// /*AFLA*/ #endif
// /*AFLA*/ 	return x;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * More accurate version that also considers the currently pending
// /*AFLA*/  * deltas. For that we need to loop over all cpus to find the current
// /*AFLA*/  * deltas. There is no synchronization so the result cannot be
// /*AFLA*/  * exactly accurate either.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long zone_page_state_snapshot(struct zone *zone,
// /*AFLA*/ 					enum zone_stat_item item)
// /*AFLA*/ {
// /*AFLA*/ 	long x = atomic_long_read(&zone->vm_stat[item]);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 	int cpu;
// /*AFLA*/ 	for_each_online_cpu(cpu)
// /*AFLA*/ 		x += per_cpu_ptr(zone->pageset, cpu)->vm_stat_diff[item];
// /*AFLA*/ 
// /*AFLA*/ 	if (x < 0)
// /*AFLA*/ 		x = 0;
// /*AFLA*/ #endif
// /*AFLA*/ 	return x;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long node_page_state_snapshot(pg_data_t *pgdat,
// /*AFLA*/ 					enum node_stat_item item)
// /*AFLA*/ {
// /*AFLA*/ 	long x = atomic_long_read(&pgdat->vm_stat[item]);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 	int cpu;
// /*AFLA*/ 	for_each_online_cpu(cpu)
// /*AFLA*/ 		x += per_cpu_ptr(pgdat->per_cpu_nodestats, cpu)->vm_node_stat_diff[item];
// /*AFLA*/ 
// /*AFLA*/ 	if (x < 0)
// /*AFLA*/ 		x = 0;
// /*AFLA*/ #endif
// /*AFLA*/ 	return x;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ extern unsigned long sum_zone_node_page_state(int node,
// /*AFLA*/ 						enum zone_stat_item item);
// /*AFLA*/ extern unsigned long node_page_state(struct pglist_data *pgdat,
// /*AFLA*/ 						enum node_stat_item item);
// /*AFLA*/ #else
// /*AFLA*/ #define sum_zone_node_page_state(node, item) global_page_state(item)
// /*AFLA*/ #define node_page_state(node, item) global_node_page_state(item)
// /*AFLA*/ #endif /* CONFIG_NUMA */
// /*AFLA*/ 
// /*AFLA*/ #define add_zone_page_state(__z, __i, __d) mod_zone_page_state(__z, __i, __d)
// /*AFLA*/ #define sub_zone_page_state(__z, __i, __d) mod_zone_page_state(__z, __i, -(__d))
// /*AFLA*/ #define add_node_page_state(__p, __i, __d) mod_node_page_state(__p, __i, __d)
// /*AFLA*/ #define sub_node_page_state(__p, __i, __d) mod_node_page_state(__p, __i, -(__d))
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ void __mod_zone_page_state(struct zone *, enum zone_stat_item item, long);
// /*AFLA*/ void __inc_zone_page_state(struct page *, enum zone_stat_item);
// /*AFLA*/ void __dec_zone_page_state(struct page *, enum zone_stat_item);
// /*AFLA*/ 
// /*AFLA*/ void __mod_node_page_state(struct pglist_data *, enum node_stat_item item, long);
// /*AFLA*/ void __inc_node_page_state(struct page *, enum node_stat_item);
// /*AFLA*/ void __dec_node_page_state(struct page *, enum node_stat_item);
// /*AFLA*/ 
// /*AFLA*/ void mod_zone_page_state(struct zone *, enum zone_stat_item, long);
// /*AFLA*/ void inc_zone_page_state(struct page *, enum zone_stat_item);
// /*AFLA*/ void dec_zone_page_state(struct page *, enum zone_stat_item);
// /*AFLA*/ 
// /*AFLA*/ void mod_node_page_state(struct pglist_data *, enum node_stat_item, long);
// /*AFLA*/ void inc_node_page_state(struct page *, enum node_stat_item);
// /*AFLA*/ void dec_node_page_state(struct page *, enum node_stat_item);
// /*AFLA*/ 
// /*AFLA*/ extern void inc_node_state(struct pglist_data *, enum node_stat_item);
// /*AFLA*/ extern void __inc_zone_state(struct zone *, enum zone_stat_item);
// /*AFLA*/ extern void __inc_node_state(struct pglist_data *, enum node_stat_item);
// /*AFLA*/ extern void dec_zone_state(struct zone *, enum zone_stat_item);
// /*AFLA*/ extern void __dec_zone_state(struct zone *, enum zone_stat_item);
// /*AFLA*/ extern void __dec_node_state(struct pglist_data *, enum node_stat_item);
// /*AFLA*/ 
// /*AFLA*/ void quiet_vmstat(void);
// /*AFLA*/ void cpu_vm_stats_fold(int cpu);
// /*AFLA*/ void refresh_zone_stat_thresholds(void);
// /*AFLA*/ 
// /*AFLA*/ struct ctl_table;
// /*AFLA*/ int vmstat_refresh(struct ctl_table *, int write,
// /*AFLA*/ 		   void __user *buffer, size_t *lenp, loff_t *ppos);
// /*AFLA*/ 
// /*AFLA*/ void drain_zonestat(struct zone *zone, struct per_cpu_pageset *);
// /*AFLA*/ 
// /*AFLA*/ int calculate_pressure_threshold(struct zone *zone);
// /*AFLA*/ int calculate_normal_threshold(struct zone *zone);
// /*AFLA*/ void set_pgdat_percpu_threshold(pg_data_t *pgdat,
// /*AFLA*/ 				int (*calculate_pressure)(struct zone *));
// /*AFLA*/ #else /* CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We do not maintain differentials in a single processor configuration.
// /*AFLA*/  * The functions directly modify the zone and global counters.
// /*AFLA*/  */
// /*AFLA*/ static inline void __mod_zone_page_state(struct zone *zone,
// /*AFLA*/ 			enum zone_stat_item item, long delta)
// /*AFLA*/ {
// /*AFLA*/ 	zone_page_state_add(delta, zone, item);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __mod_node_page_state(struct pglist_data *pgdat,
// /*AFLA*/ 			enum node_stat_item item, int delta)
// /*AFLA*/ {
// /*AFLA*/ 	node_page_state_add(delta, pgdat, item);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __inc_zone_state(struct zone *zone, enum zone_stat_item item)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_long_inc(&zone->vm_stat[item]);
// /*AFLA*/ 	atomic_long_inc(&vm_zone_stat[item]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __inc_node_state(struct pglist_data *pgdat, enum node_stat_item item)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_long_inc(&pgdat->vm_stat[item]);
// /*AFLA*/ 	atomic_long_inc(&vm_node_stat[item]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __dec_zone_state(struct zone *zone, enum zone_stat_item item)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_long_dec(&zone->vm_stat[item]);
// /*AFLA*/ 	atomic_long_dec(&vm_zone_stat[item]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __dec_node_state(struct pglist_data *pgdat, enum node_stat_item item)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_long_dec(&pgdat->vm_stat[item]);
// /*AFLA*/ 	atomic_long_dec(&vm_node_stat[item]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __inc_zone_page_state(struct page *page,
// /*AFLA*/ 			enum zone_stat_item item)
// /*AFLA*/ {
// /*AFLA*/ 	__inc_zone_state(page_zone(page), item);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __inc_node_page_state(struct page *page,
// /*AFLA*/ 			enum node_stat_item item)
// /*AFLA*/ {
// /*AFLA*/ 	__inc_node_state(page_pgdat(page), item);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ static inline void __dec_zone_page_state(struct page *page,
// /*AFLA*/ 			enum zone_stat_item item)
// /*AFLA*/ {
// /*AFLA*/ 	__dec_zone_state(page_zone(page), item);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __dec_node_page_state(struct page *page,
// /*AFLA*/ 			enum node_stat_item item)
// /*AFLA*/ {
// /*AFLA*/ 	__dec_node_state(page_pgdat(page), item);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We only use atomic operations to update counters. So there is no need to
// /*AFLA*/  * disable interrupts.
// /*AFLA*/  */
// /*AFLA*/ #define inc_zone_page_state __inc_zone_page_state
// /*AFLA*/ #define dec_zone_page_state __dec_zone_page_state
// /*AFLA*/ #define mod_zone_page_state __mod_zone_page_state
// /*AFLA*/ 
// /*AFLA*/ #define inc_node_page_state __inc_node_page_state
// /*AFLA*/ #define dec_node_page_state __dec_node_page_state
// /*AFLA*/ #define mod_node_page_state __mod_node_page_state
// /*AFLA*/ 
// /*AFLA*/ #define inc_zone_state __inc_zone_state
// /*AFLA*/ #define inc_node_state __inc_node_state
// /*AFLA*/ #define dec_zone_state __dec_zone_state
// /*AFLA*/ 
// /*AFLA*/ #define set_pgdat_percpu_threshold(pgdat, callback) { }
// /*AFLA*/ 
// /*AFLA*/ static inline void refresh_zone_stat_thresholds(void) { }
// /*AFLA*/ static inline void cpu_vm_stats_fold(int cpu) { }
// /*AFLA*/ static inline void quiet_vmstat(void) { }
// /*AFLA*/ 
// /*AFLA*/ static inline void drain_zonestat(struct zone *zone,
// /*AFLA*/ 			struct per_cpu_pageset *pset) { }
// /*AFLA*/ #endif		/* CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ static inline void __mod_zone_freepage_state(struct zone *zone, int nr_pages,
// /*AFLA*/ 					     int migratetype)
// /*AFLA*/ {
// /*AFLA*/ 	__mod_zone_page_state(zone, NR_FREE_PAGES, nr_pages);
// /*AFLA*/ 	if (is_migrate_cma(migratetype))
// /*AFLA*/ 		__mod_zone_page_state(zone, NR_FREE_CMA_PAGES, nr_pages);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern const char * const vmstat_text[];
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_VMSTAT_H */
