// /*AFLA*/ #ifndef PAGE_FLAGS_LAYOUT_H
// /*AFLA*/ #define PAGE_FLAGS_LAYOUT_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/numa.h>
// /*AFLA*/ #include <generated/bounds.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * When a memory allocation must conform to specific limitations (such
// /*AFLA*/  * as being suitable for DMA) the caller will pass in hints to the
// /*AFLA*/  * allocator in the gfp_mask, in the zone modifier bits.  These bits
// /*AFLA*/  * are used to select a priority ordered list of memory zones which
// /*AFLA*/  * match the requested limits. See gfp_zone() in include/linux/gfp.h
// /*AFLA*/  */
// /*AFLA*/ #if MAX_NR_ZONES < 2
// /*AFLA*/ #define ZONES_SHIFT 0
// /*AFLA*/ #elif MAX_NR_ZONES <= 2
// /*AFLA*/ #define ZONES_SHIFT 1
// /*AFLA*/ #elif MAX_NR_ZONES <= 4
// /*AFLA*/ #define ZONES_SHIFT 2
// /*AFLA*/ #elif MAX_NR_ZONES <= 8
// /*AFLA*/ #define ZONES_SHIFT 3
// /*AFLA*/ #else
// /*AFLA*/ #error ZONES_SHIFT -- too many zones configured adjust calculation
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SPARSEMEM
// /*AFLA*/ #include <asm/sparsemem.h>
// /*AFLA*/ 
// /*AFLA*/ /* SECTION_SHIFT	#bits space required to store a section # */
// /*AFLA*/ #define SECTIONS_SHIFT	(MAX_PHYSMEM_BITS - SECTION_SIZE_BITS)
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_SPARSEMEM */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * page->flags layout:
// /*AFLA*/  *
// /*AFLA*/  * There are five possibilities for how page->flags get laid out.  The first
// /*AFLA*/  * pair is for the normal case without sparsemem. The second pair is for
// /*AFLA*/  * sparsemem when there is plenty of space for node and section information.
// /*AFLA*/  * The last is when there is insufficient space in page->flags and a separate
// /*AFLA*/  * lookup is necessary.
// /*AFLA*/  *
// /*AFLA*/  * No sparsemem or sparsemem vmemmap: |       NODE     | ZONE |             ... | FLAGS |
// /*AFLA*/  *      " plus space for last_cpupid: |       NODE     | ZONE | LAST_CPUPID ... | FLAGS |
// /*AFLA*/  * classic sparse with space for node:| SECTION | NODE | ZONE |             ... | FLAGS |
// /*AFLA*/  *      " plus space for last_cpupid: | SECTION | NODE | ZONE | LAST_CPUPID ... | FLAGS |
// /*AFLA*/  * classic sparse no space for node:  | SECTION |     ZONE    | ... | FLAGS |
// /*AFLA*/  */
// /*AFLA*/ #if defined(CONFIG_SPARSEMEM) && !defined(CONFIG_SPARSEMEM_VMEMMAP)
// /*AFLA*/ #define SECTIONS_WIDTH		SECTIONS_SHIFT
// /*AFLA*/ #else
// /*AFLA*/ #define SECTIONS_WIDTH		0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define ZONES_WIDTH		ZONES_SHIFT
// /*AFLA*/ 
// /*AFLA*/ #if SECTIONS_WIDTH+ZONES_WIDTH+NODES_SHIFT <= BITS_PER_LONG - NR_PAGEFLAGS
// /*AFLA*/ #define NODES_WIDTH		NODES_SHIFT
// /*AFLA*/ #else
// /*AFLA*/ #ifdef CONFIG_SPARSEMEM_VMEMMAP
// /*AFLA*/ #error "Vmemmap: No space for nodes field in page flags"
// /*AFLA*/ #endif
// /*AFLA*/ #define NODES_WIDTH		0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA_BALANCING
// /*AFLA*/ #define LAST__PID_SHIFT 8
// /*AFLA*/ #define LAST__PID_MASK  ((1 << LAST__PID_SHIFT)-1)
// /*AFLA*/ 
// /*AFLA*/ #define LAST__CPU_SHIFT NR_CPUS_BITS
// /*AFLA*/ #define LAST__CPU_MASK  ((1 << LAST__CPU_SHIFT)-1)
// /*AFLA*/ 
// /*AFLA*/ #define LAST_CPUPID_SHIFT (LAST__PID_SHIFT+LAST__CPU_SHIFT)
// /*AFLA*/ #else
// /*AFLA*/ #define LAST_CPUPID_SHIFT 0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if SECTIONS_WIDTH+ZONES_WIDTH+NODES_SHIFT+LAST_CPUPID_SHIFT <= BITS_PER_LONG - NR_PAGEFLAGS
// /*AFLA*/ #define LAST_CPUPID_WIDTH LAST_CPUPID_SHIFT
// /*AFLA*/ #else
// /*AFLA*/ #define LAST_CPUPID_WIDTH 0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We are going to use the flags for the page to node mapping if its in
// /*AFLA*/  * there.  This includes the case where there is no node, so it is implicit.
// /*AFLA*/  */
// /*AFLA*/ #if !(NODES_WIDTH > 0 || NODES_SHIFT == 0)
// /*AFLA*/ #define NODE_NOT_IN_PAGE_FLAGS
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_NUMA_BALANCING) && LAST_CPUPID_WIDTH == 0
// /*AFLA*/ #define LAST_CPUPID_NOT_IN_PAGE_FLAGS
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_PAGE_FLAGS_LAYOUT */
