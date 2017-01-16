// /*AFLA*/ #ifndef __LINUX_PERCPU_H
// /*AFLA*/ #define __LINUX_PERCPU_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/mmdebug.h>
// /*AFLA*/ #include <linux/preempt.h>
// /*AFLA*/ #include <linux/smp.h>
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ #include <linux/printk.h>
// /*AFLA*/ #include <linux/pfn.h>
// /*AFLA*/ #include <linux/init.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/percpu.h>
// /*AFLA*/ 
// /*AFLA*/ /* enough to cover all DEFINE_PER_CPUs in modules */
// /*AFLA*/ #ifdef CONFIG_MODULES
// /*AFLA*/ #define PERCPU_MODULE_RESERVE		(8 << 10)
// /*AFLA*/ #else
// /*AFLA*/ #define PERCPU_MODULE_RESERVE		0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* minimum unit size, also is the maximum supported allocation size */
// /*AFLA*/ #define PCPU_MIN_UNIT_SIZE		PFN_ALIGN(32 << 10)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Percpu allocator can serve percpu allocations before slab is
// /*AFLA*/  * initialized which allows slab to depend on the percpu allocator.
// /*AFLA*/  * The following two parameters decide how much resource to
// /*AFLA*/  * preallocate for this.  Keep PERCPU_DYNAMIC_RESERVE equal to or
// /*AFLA*/  * larger than PERCPU_DYNAMIC_EARLY_SIZE.
// /*AFLA*/  */
// /*AFLA*/ #define PERCPU_DYNAMIC_EARLY_SLOTS	128
// /*AFLA*/ #define PERCPU_DYNAMIC_EARLY_SIZE	(12 << 10)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * PERCPU_DYNAMIC_RESERVE indicates the amount of free area to piggy
// /*AFLA*/  * back on the first chunk for dynamic percpu allocation if arch is
// /*AFLA*/  * manually allocating and mapping it for faster access (as a part of
// /*AFLA*/  * large page mapping for example).
// /*AFLA*/  *
// /*AFLA*/  * The following values give between one and two pages of free space
// /*AFLA*/  * after typical minimal boot (2-way SMP, single disk and NIC) with
// /*AFLA*/  * both defconfig and a distro config on x86_64 and 32.  More
// /*AFLA*/  * intelligent way to determine this would be nice.
// /*AFLA*/  */
// /*AFLA*/ #if BITS_PER_LONG > 32
// /*AFLA*/ #define PERCPU_DYNAMIC_RESERVE		(28 << 10)
// /*AFLA*/ #else
// /*AFLA*/ #define PERCPU_DYNAMIC_RESERVE		(20 << 10)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void *pcpu_base_addr;
// /*AFLA*/ extern const unsigned long *pcpu_unit_offsets;
// /*AFLA*/ 
// /*AFLA*/ struct pcpu_group_info {
// /*AFLA*/ 	int			nr_units;	/* aligned # of units */
// /*AFLA*/ 	unsigned long		base_offset;	/* base address offset */
// /*AFLA*/ 	unsigned int		*cpu_map;	/* unit->cpu map, empty
// /*AFLA*/ 						 * entries contain NR_CPUS */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct pcpu_alloc_info {
// /*AFLA*/ 	size_t			static_size;
// /*AFLA*/ 	size_t			reserved_size;
// /*AFLA*/ 	size_t			dyn_size;
// /*AFLA*/ 	size_t			unit_size;
// /*AFLA*/ 	size_t			atom_size;
// /*AFLA*/ 	size_t			alloc_size;
// /*AFLA*/ 	size_t			__ai_size;	/* internal, don't use */
// /*AFLA*/ 	int			nr_groups;	/* 0 if grouping unnecessary */
// /*AFLA*/ 	struct pcpu_group_info	groups[];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum pcpu_fc {
// /*AFLA*/ 	PCPU_FC_AUTO,
// /*AFLA*/ 	PCPU_FC_EMBED,
// /*AFLA*/ 	PCPU_FC_PAGE,
// /*AFLA*/ 
// /*AFLA*/ 	PCPU_FC_NR,
// /*AFLA*/ };
// /*AFLA*/ extern const char * const pcpu_fc_names[PCPU_FC_NR];
// /*AFLA*/ 
// /*AFLA*/ extern enum pcpu_fc pcpu_chosen_fc;
// /*AFLA*/ 
// /*AFLA*/ typedef void * (*pcpu_fc_alloc_fn_t)(unsigned int cpu, size_t size,
// /*AFLA*/ 				     size_t align);
// /*AFLA*/ typedef void (*pcpu_fc_free_fn_t)(void *ptr, size_t size);
// /*AFLA*/ typedef void (*pcpu_fc_populate_pte_fn_t)(unsigned long addr);
// /*AFLA*/ typedef int (pcpu_fc_cpu_distance_fn_t)(unsigned int from, unsigned int to);
// /*AFLA*/ 
// /*AFLA*/ extern struct pcpu_alloc_info * __init pcpu_alloc_alloc_info(int nr_groups,
// /*AFLA*/ 							     int nr_units);
// /*AFLA*/ extern void __init pcpu_free_alloc_info(struct pcpu_alloc_info *ai);
// /*AFLA*/ 
// /*AFLA*/ extern int __init pcpu_setup_first_chunk(const struct pcpu_alloc_info *ai,
// /*AFLA*/ 					 void *base_addr);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NEED_PER_CPU_EMBED_FIRST_CHUNK
// /*AFLA*/ extern int __init pcpu_embed_first_chunk(size_t reserved_size, size_t dyn_size,
// /*AFLA*/ 				size_t atom_size,
// /*AFLA*/ 				pcpu_fc_cpu_distance_fn_t cpu_distance_fn,
// /*AFLA*/ 				pcpu_fc_alloc_fn_t alloc_fn,
// /*AFLA*/ 				pcpu_fc_free_fn_t free_fn);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NEED_PER_CPU_PAGE_FIRST_CHUNK
// /*AFLA*/ extern int __init pcpu_page_first_chunk(size_t reserved_size,
// /*AFLA*/ 				pcpu_fc_alloc_fn_t alloc_fn,
// /*AFLA*/ 				pcpu_fc_free_fn_t free_fn,
// /*AFLA*/ 				pcpu_fc_populate_pte_fn_t populate_pte_fn);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void __percpu *__alloc_reserved_percpu(size_t size, size_t align);
// /*AFLA*/ extern bool is_kernel_percpu_address(unsigned long addr);
// /*AFLA*/ 
// /*AFLA*/ #if !defined(CONFIG_SMP) || !defined(CONFIG_HAVE_SETUP_PER_CPU_AREA)
// /*AFLA*/ extern void __init setup_per_cpu_areas(void);
// /*AFLA*/ #endif
// /*AFLA*/ extern void __init percpu_init_late(void);
// /*AFLA*/ 
// /*AFLA*/ extern void __percpu *__alloc_percpu_gfp(size_t size, size_t align, gfp_t gfp);
// /*AFLA*/ extern void __percpu *__alloc_percpu(size_t size, size_t align);
// /*AFLA*/ extern void free_percpu(void __percpu *__pdata);
// /*AFLA*/ extern phys_addr_t per_cpu_ptr_to_phys(void *addr);
// /*AFLA*/ 
// /*AFLA*/ #define alloc_percpu_gfp(type, gfp)					\
// /*AFLA*/ 	(typeof(type) __percpu *)__alloc_percpu_gfp(sizeof(type),	\
// /*AFLA*/ 						__alignof__(type), gfp)
// /*AFLA*/ #define alloc_percpu(type)						\
// /*AFLA*/ 	(typeof(type) __percpu *)__alloc_percpu(sizeof(type),		\
// /*AFLA*/ 						__alignof__(type))
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_PERCPU_H */
