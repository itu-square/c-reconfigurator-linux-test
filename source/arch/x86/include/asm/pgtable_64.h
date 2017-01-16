// /*AFLA*/ #ifndef _ASM_X86_PGTABLE_64_H
// /*AFLA*/ #define _ASM_X86_PGTABLE_64_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/const.h>
// /*AFLA*/ #include <asm/pgtable_64_types.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This file contains the functions and defines necessary to modify and use
// /*AFLA*/  * the x86-64 page table tree.
// /*AFLA*/  */
// /*AFLA*/ #include <asm/processor.h>
// /*AFLA*/ #include <linux/bitops.h>
// /*AFLA*/ #include <linux/threads.h>
// /*AFLA*/ 
// /*AFLA*/ extern pud_t level3_kernel_pgt[512];
// /*AFLA*/ extern pud_t level3_ident_pgt[512];
// /*AFLA*/ extern pmd_t level2_kernel_pgt[512];
// /*AFLA*/ extern pmd_t level2_fixmap_pgt[512];
// /*AFLA*/ extern pmd_t level2_ident_pgt[512];
// /*AFLA*/ extern pte_t level1_fixmap_pgt[512];
// /*AFLA*/ extern pgd_t init_level4_pgt[];
// /*AFLA*/ 
// /*AFLA*/ #define swapper_pg_dir init_level4_pgt
// /*AFLA*/ 
// /*AFLA*/ extern void paging_init(void);
// /*AFLA*/ 
// /*AFLA*/ #define pte_ERROR(e)					\
// /*AFLA*/ 	pr_err("%s:%d: bad pte %p(%016lx)\n",		\
// /*AFLA*/ 	       __FILE__, __LINE__, &(e), pte_val(e))
// /*AFLA*/ #define pmd_ERROR(e)					\
// /*AFLA*/ 	pr_err("%s:%d: bad pmd %p(%016lx)\n",		\
// /*AFLA*/ 	       __FILE__, __LINE__, &(e), pmd_val(e))
// /*AFLA*/ #define pud_ERROR(e)					\
// /*AFLA*/ 	pr_err("%s:%d: bad pud %p(%016lx)\n",		\
// /*AFLA*/ 	       __FILE__, __LINE__, &(e), pud_val(e))
// /*AFLA*/ #define pgd_ERROR(e)					\
// /*AFLA*/ 	pr_err("%s:%d: bad pgd %p(%016lx)\n",		\
// /*AFLA*/ 	       __FILE__, __LINE__, &(e), pgd_val(e))
// /*AFLA*/ 
// /*AFLA*/ struct mm_struct;
// /*AFLA*/ 
// /*AFLA*/ void set_pte_vaddr_pud(pud_t *pud_page, unsigned long vaddr, pte_t new_pte);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ static inline void native_pte_clear(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 				    pte_t *ptep)
// /*AFLA*/ {
// /*AFLA*/ 	*ptep = native_make_pte(0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_set_pte(pte_t *ptep, pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	*ptep = pte;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_set_pte_atomic(pte_t *ptep, pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	native_set_pte(ptep, pte);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_set_pmd(pmd_t *pmdp, pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	*pmdp = pmd;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_pmd_clear(pmd_t *pmd)
// /*AFLA*/ {
// /*AFLA*/ 	native_set_pmd(pmd, native_make_pmd(0));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t native_ptep_get_and_clear(pte_t *xp)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 	return native_make_pte(xchg(&xp->pte, 0));
// /*AFLA*/ #else
// /*AFLA*/ 	/* native_local_ptep_get_and_clear,
// /*AFLA*/ 	   but duplicated because of cyclic dependency */
// /*AFLA*/ 	pte_t ret = *xp;
// /*AFLA*/ 	native_pte_clear(NULL, 0, xp);
// /*AFLA*/ 	return ret;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t native_pmdp_get_and_clear(pmd_t *xp)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 	return native_make_pmd(xchg(&xp->pmd, 0));
// /*AFLA*/ #else
// /*AFLA*/ 	/* native_local_pmdp_get_and_clear,
// /*AFLA*/ 	   but duplicated because of cyclic dependency */
// /*AFLA*/ 	pmd_t ret = *xp;
// /*AFLA*/ 	native_pmd_clear(xp);
// /*AFLA*/ 	return ret;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_set_pud(pud_t *pudp, pud_t pud)
// /*AFLA*/ {
// /*AFLA*/ 	*pudp = pud;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_pud_clear(pud_t *pud)
// /*AFLA*/ {
// /*AFLA*/ 	native_set_pud(pud, native_make_pud(0));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_set_pgd(pgd_t *pgdp, pgd_t pgd)
// /*AFLA*/ {
// /*AFLA*/ 	*pgdp = pgd;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_pgd_clear(pgd_t *pgd)
// /*AFLA*/ {
// /*AFLA*/ 	native_set_pgd(pgd, native_make_pgd(0));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void sync_global_pgds(unsigned long start, unsigned long end,
// /*AFLA*/ 			     int removed);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Conversion functions: convert a page and protection to a page entry,
// /*AFLA*/  * and a page entry and page directory to the page they refer to.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Level 4 access.
// /*AFLA*/  */
// /*AFLA*/ static inline int pgd_large(pgd_t pgd) { return 0; }
// /*AFLA*/ #define mk_kernel_pgd(address) __pgd((address) | _KERNPG_TABLE)
// /*AFLA*/ 
// /*AFLA*/ /* PUD - Level3 access */
// /*AFLA*/ 
// /*AFLA*/ /* PMD  - Level 2 access */
// /*AFLA*/ 
// /*AFLA*/ /* PTE - Level 1 access. */
// /*AFLA*/ 
// /*AFLA*/ /* x86-64 always has all page tables mapped. */
// /*AFLA*/ #define pte_offset_map(dir, address) pte_offset_kernel((dir), (address))
// /*AFLA*/ #define pte_unmap(pte) ((void)(pte))/* NOP */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Encode and de-code a swap entry
// /*AFLA*/  *
// /*AFLA*/  * |     ...            | 11| 10|  9|8|7|6|5| 4| 3|2|1|0| <- bit number
// /*AFLA*/  * |     ...            |SW3|SW2|SW1|G|L|D|A|CD|WT|U|W|P| <- bit names
// /*AFLA*/  * | OFFSET (14->63) | TYPE (9-13)  |0|X|X|X| X| X|X|X|0| <- swp entry
// /*AFLA*/  *
// /*AFLA*/  * G (8) is aliased and used as a PROT_NONE indicator for
// /*AFLA*/  * !present ptes.  We need to start storing swap entries above
// /*AFLA*/  * there.  We also need to avoid using A and D because of an
// /*AFLA*/  * erratum where they can be incorrectly set by hardware on
// /*AFLA*/  * non-present PTEs.
// /*AFLA*/  */
// /*AFLA*/ #define SWP_TYPE_FIRST_BIT (_PAGE_BIT_PROTNONE + 1)
// /*AFLA*/ #define SWP_TYPE_BITS 5
// /*AFLA*/ /* Place the offset above the type: */
// /*AFLA*/ #define SWP_OFFSET_FIRST_BIT (SWP_TYPE_FIRST_BIT + SWP_TYPE_BITS)
// /*AFLA*/ 
// /*AFLA*/ #define MAX_SWAPFILES_CHECK() BUILD_BUG_ON(MAX_SWAPFILES_SHIFT > SWP_TYPE_BITS)
// /*AFLA*/ 
// /*AFLA*/ #define __swp_type(x)			(((x).val >> (SWP_TYPE_FIRST_BIT)) \
// /*AFLA*/ 					 & ((1U << SWP_TYPE_BITS) - 1))
// /*AFLA*/ #define __swp_offset(x)			((x).val >> SWP_OFFSET_FIRST_BIT)
// /*AFLA*/ #define __swp_entry(type, offset)	((swp_entry_t) { \
// /*AFLA*/ 					 ((type) << (SWP_TYPE_FIRST_BIT)) \
// /*AFLA*/ 					 | ((offset) << SWP_OFFSET_FIRST_BIT) })
// /*AFLA*/ #define __pte_to_swp_entry(pte)		((swp_entry_t) { pte_val((pte)) })
// /*AFLA*/ #define __swp_entry_to_pte(x)		((pte_t) { .pte = (x).val })
// /*AFLA*/ 
// /*AFLA*/ extern int kern_addr_valid(unsigned long addr);
// /*AFLA*/ extern void cleanup_highmap(void);
// /*AFLA*/ 
// /*AFLA*/ #define HAVE_ARCH_UNMAPPED_AREA
// /*AFLA*/ #define HAVE_ARCH_UNMAPPED_AREA_TOPDOWN
// /*AFLA*/ 
// /*AFLA*/ #define pgtable_cache_init()   do { } while (0)
// /*AFLA*/ #define check_pgt_cache()      do { } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define PAGE_AGP    PAGE_KERNEL_NOCACHE
// /*AFLA*/ #define HAVE_PAGE_AGP 1
// /*AFLA*/ 
// /*AFLA*/ /* fs/proc/kcore.c */
// /*AFLA*/ #define	kc_vaddr_to_offset(v) ((v) & __VIRTUAL_MASK)
// /*AFLA*/ #define	kc_offset_to_vaddr(o) ((o) | ~__VIRTUAL_MASK)
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_PTE_SAME
// /*AFLA*/ 
// /*AFLA*/ #define vmemmap ((struct page *)VMEMMAP_START)
// /*AFLA*/ 
// /*AFLA*/ extern void init_extra_mapping_uc(unsigned long phys, unsigned long size);
// /*AFLA*/ extern void init_extra_mapping_wb(unsigned long phys, unsigned long size);
// /*AFLA*/ 
// /*AFLA*/ #endif /* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_PGTABLE_64_H */
