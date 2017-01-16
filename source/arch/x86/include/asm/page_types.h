// /*AFLA*/ #ifndef _ASM_X86_PAGE_DEFS_H
// /*AFLA*/ #define _ASM_X86_PAGE_DEFS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/const.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /* PAGE_SHIFT determines the page size */
// /*AFLA*/ #define PAGE_SHIFT		12
// /*AFLA*/ #define PAGE_SIZE		(_AC(1,UL) << PAGE_SHIFT)
// /*AFLA*/ #define PAGE_MASK		(~(PAGE_SIZE-1))
// /*AFLA*/ 
// /*AFLA*/ #define PMD_PAGE_SIZE		(_AC(1, UL) << PMD_SHIFT)
// /*AFLA*/ #define PMD_PAGE_MASK		(~(PMD_PAGE_SIZE-1))
// /*AFLA*/ 
// /*AFLA*/ #define PUD_PAGE_SIZE		(_AC(1, UL) << PUD_SHIFT)
// /*AFLA*/ #define PUD_PAGE_MASK		(~(PUD_PAGE_SIZE-1))
// /*AFLA*/ 
// /*AFLA*/ #define __PHYSICAL_MASK		((phys_addr_t)((1ULL << __PHYSICAL_MASK_SHIFT) - 1))
// /*AFLA*/ #define __VIRTUAL_MASK		((1UL << __VIRTUAL_MASK_SHIFT) - 1)
// /*AFLA*/ 
// /*AFLA*/ /* Cast *PAGE_MASK to a signed type so that it is sign-extended if
// /*AFLA*/    virtual addresses are 32-bits but physical addresses are larger
// /*AFLA*/    (ie, 32-bit PAE). */
// /*AFLA*/ #define PHYSICAL_PAGE_MASK	(((signed long)PAGE_MASK) & __PHYSICAL_MASK)
// /*AFLA*/ #define PHYSICAL_PMD_PAGE_MASK	(((signed long)PMD_PAGE_MASK) & __PHYSICAL_MASK)
// /*AFLA*/ #define PHYSICAL_PUD_PAGE_MASK	(((signed long)PUD_PAGE_MASK) & __PHYSICAL_MASK)
// /*AFLA*/ 
// /*AFLA*/ #define HPAGE_SHIFT		PMD_SHIFT
// /*AFLA*/ #define HPAGE_SIZE		(_AC(1,UL) << HPAGE_SHIFT)
// /*AFLA*/ #define HPAGE_MASK		(~(HPAGE_SIZE - 1))
// /*AFLA*/ #define HUGETLB_PAGE_ORDER	(HPAGE_SHIFT - PAGE_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ #define HUGE_MAX_HSTATE 2
// /*AFLA*/ 
// /*AFLA*/ #define PAGE_OFFSET		((unsigned long)__PAGE_OFFSET)
// /*AFLA*/ 
// /*AFLA*/ #define VM_DATA_DEFAULT_FLAGS \
// /*AFLA*/ 	(((current->personality & READ_IMPLIES_EXEC) ? VM_EXEC : 0 ) | \
// /*AFLA*/ 	 VM_READ | VM_WRITE | VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC)
// /*AFLA*/ 
// /*AFLA*/ #define __PHYSICAL_START	ALIGN(CONFIG_PHYSICAL_START, \
// /*AFLA*/ 				      CONFIG_PHYSICAL_ALIGN)
// /*AFLA*/ 
// /*AFLA*/ #define __START_KERNEL		(__START_KERNEL_map + __PHYSICAL_START)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ #include <asm/page_64_types.h>
// /*AFLA*/ #define IOREMAP_MAX_ORDER       (PUD_SHIFT)
// /*AFLA*/ #else
// /*AFLA*/ #include <asm/page_32_types.h>
// /*AFLA*/ #define IOREMAP_MAX_ORDER       (PMD_SHIFT)
// /*AFLA*/ #endif	/* CONFIG_X86_64 */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ extern int devmem_is_allowed(unsigned long pagenr);
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long max_low_pfn_mapped;
// /*AFLA*/ extern unsigned long max_pfn_mapped;
// /*AFLA*/ 
// /*AFLA*/ static inline phys_addr_t get_max_mapped(void)
// /*AFLA*/ {
// /*AFLA*/ 	return (phys_addr_t)max_pfn_mapped << PAGE_SHIFT;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ bool pfn_range_is_mapped(unsigned long start_pfn, unsigned long end_pfn);
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long init_memory_mapping(unsigned long start,
// /*AFLA*/ 					 unsigned long end);
// /*AFLA*/ 
// /*AFLA*/ extern void initmem_init(void);
// /*AFLA*/ 
// /*AFLA*/ #endif	/* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif	/* _ASM_X86_PAGE_DEFS_H */
