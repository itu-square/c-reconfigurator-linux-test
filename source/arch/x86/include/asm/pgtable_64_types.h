// /*AFLA*/ #ifndef _ASM_X86_PGTABLE_64_DEFS_H
// /*AFLA*/ #define _ASM_X86_PGTABLE_64_DEFS_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/sparsemem.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <asm/kaslr.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These are used to make use of C type-checking..
// /*AFLA*/  */
// /*AFLA*/ typedef unsigned long	pteval_t;
// /*AFLA*/ typedef unsigned long	pmdval_t;
// /*AFLA*/ typedef unsigned long	pudval_t;
// /*AFLA*/ typedef unsigned long	pgdval_t;
// /*AFLA*/ typedef unsigned long	pgprotval_t;
// /*AFLA*/ 
// /*AFLA*/ typedef struct { pteval_t pte; } pte_t;
// /*AFLA*/ 
// /*AFLA*/ #endif	/* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #define SHARED_KERNEL_PMD	0
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * PGDIR_SHIFT determines what a top-level page table entry can map
// /*AFLA*/  */
// /*AFLA*/ #define PGDIR_SHIFT	39
// /*AFLA*/ #define PTRS_PER_PGD	512
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * 3rd level page
// /*AFLA*/  */
// /*AFLA*/ #define PUD_SHIFT	30
// /*AFLA*/ #define PTRS_PER_PUD	512
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * PMD_SHIFT determines the size of the area a middle-level
// /*AFLA*/  * page table can map
// /*AFLA*/  */
// /*AFLA*/ #define PMD_SHIFT	21
// /*AFLA*/ #define PTRS_PER_PMD	512
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * entries per page directory level
// /*AFLA*/  */
// /*AFLA*/ #define PTRS_PER_PTE	512
// /*AFLA*/ 
// /*AFLA*/ #define PMD_SIZE	(_AC(1, UL) << PMD_SHIFT)
// /*AFLA*/ #define PMD_MASK	(~(PMD_SIZE - 1))
// /*AFLA*/ #define PUD_SIZE	(_AC(1, UL) << PUD_SHIFT)
// /*AFLA*/ #define PUD_MASK	(~(PUD_SIZE - 1))
// /*AFLA*/ #define PGDIR_SIZE	(_AC(1, UL) << PGDIR_SHIFT)
// /*AFLA*/ #define PGDIR_MASK	(~(PGDIR_SIZE - 1))
// /*AFLA*/ 
// /*AFLA*/ /* See Documentation/x86/x86_64/mm.txt for a description of the memory map. */
// /*AFLA*/ #define MAXMEM		_AC(__AC(1, UL) << MAX_PHYSMEM_BITS, UL)
// /*AFLA*/ #define VMALLOC_SIZE_TB	_AC(32, UL)
// /*AFLA*/ #define __VMALLOC_BASE	_AC(0xffffc90000000000, UL)
// /*AFLA*/ #define __VMEMMAP_BASE	_AC(0xffffea0000000000, UL)
// /*AFLA*/ #ifdef CONFIG_RANDOMIZE_MEMORY
// /*AFLA*/ #define VMALLOC_START	vmalloc_base
// /*AFLA*/ #define VMEMMAP_START	vmemmap_base
// /*AFLA*/ #else
// /*AFLA*/ #define VMALLOC_START	__VMALLOC_BASE
// /*AFLA*/ #define VMEMMAP_START	__VMEMMAP_BASE
// /*AFLA*/ #endif /* CONFIG_RANDOMIZE_MEMORY */
// /*AFLA*/ #define VMALLOC_END	(VMALLOC_START + _AC((VMALLOC_SIZE_TB << 40) - 1, UL))
// /*AFLA*/ #define MODULES_VADDR    (__START_KERNEL_map + KERNEL_IMAGE_SIZE)
// /*AFLA*/ #define MODULES_END      _AC(0xffffffffff000000, UL)
// /*AFLA*/ #define MODULES_LEN   (MODULES_END - MODULES_VADDR)
// /*AFLA*/ #define ESPFIX_PGD_ENTRY _AC(-2, UL)
// /*AFLA*/ #define ESPFIX_BASE_ADDR (ESPFIX_PGD_ENTRY << PGDIR_SHIFT)
// /*AFLA*/ #define EFI_VA_START	 ( -4 * (_AC(1, UL) << 30))
// /*AFLA*/ #define EFI_VA_END	 (-68 * (_AC(1, UL) << 30))
// /*AFLA*/ 
// /*AFLA*/ #define EARLY_DYNAMIC_PAGE_TABLES	64
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_PGTABLE_64_DEFS_H */
