// /*AFLA*/ #ifndef _ASM_X86_PGTABLE_H
// /*AFLA*/ #define _ASM_X86_PGTABLE_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/page.h>
// /*AFLA*/ #include <asm/e820.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/pgtable_types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Macro to mark a page protection value as UC-
// /*AFLA*/  */
// /*AFLA*/ #define pgprot_noncached(prot)						\
// /*AFLA*/ 	((boot_cpu_data.x86 > 3)					\
// /*AFLA*/ 	 ? (__pgprot(pgprot_val(prot) |					\
// /*AFLA*/ 		     cachemode2protval(_PAGE_CACHE_MODE_UC_MINUS)))	\
// /*AFLA*/ 	 : (prot))
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #include <asm/x86_init.h>
// /*AFLA*/ 
// /*AFLA*/ void ptdump_walk_pgd_level(struct seq_file *m, pgd_t *pgd);
// /*AFLA*/ void ptdump_walk_pgd_level_checkwx(void);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_WX
// /*AFLA*/ #define debug_checkwx() ptdump_walk_pgd_level_checkwx()
// /*AFLA*/ #else
// /*AFLA*/ #define debug_checkwx() do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * ZERO_PAGE is a global shared page that is always zero: used
// /*AFLA*/  * for zero-mapped memory areas etc..
// /*AFLA*/  */
// /*AFLA*/ extern unsigned long empty_zero_page[PAGE_SIZE / sizeof(unsigned long)]
// /*AFLA*/ 	__visible;
// /*AFLA*/ #define ZERO_PAGE(vaddr) (virt_to_page(empty_zero_page))
// /*AFLA*/ 
// /*AFLA*/ extern spinlock_t pgd_lock;
// /*AFLA*/ extern struct list_head pgd_list;
// /*AFLA*/ 
// /*AFLA*/ extern struct mm_struct *pgd_page_get_mm(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PARAVIRT
// /*AFLA*/ #include <asm/paravirt.h>
// /*AFLA*/ #else  /* !CONFIG_PARAVIRT */
// /*AFLA*/ #define set_pte(ptep, pte)		native_set_pte(ptep, pte)
// /*AFLA*/ #define set_pte_at(mm, addr, ptep, pte)	native_set_pte_at(mm, addr, ptep, pte)
// /*AFLA*/ #define set_pmd_at(mm, addr, pmdp, pmd)	native_set_pmd_at(mm, addr, pmdp, pmd)
// /*AFLA*/ 
// /*AFLA*/ #define set_pte_atomic(ptep, pte)					\
// /*AFLA*/ 	native_set_pte_atomic(ptep, pte)
// /*AFLA*/ 
// /*AFLA*/ #define set_pmd(pmdp, pmd)		native_set_pmd(pmdp, pmd)
// /*AFLA*/ 
// /*AFLA*/ #ifndef __PAGETABLE_PUD_FOLDED
// /*AFLA*/ #define set_pgd(pgdp, pgd)		native_set_pgd(pgdp, pgd)
// /*AFLA*/ #define pgd_clear(pgd)			native_pgd_clear(pgd)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef set_pud
// /*AFLA*/ # define set_pud(pudp, pud)		native_set_pud(pudp, pud)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __PAGETABLE_PMD_FOLDED
// /*AFLA*/ #define pud_clear(pud)			native_pud_clear(pud)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define pte_clear(mm, addr, ptep)	native_pte_clear(mm, addr, ptep)
// /*AFLA*/ #define pmd_clear(pmd)			native_pmd_clear(pmd)
// /*AFLA*/ 
// /*AFLA*/ #define pte_update(mm, addr, ptep)              do { } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define pgd_val(x)	native_pgd_val(x)
// /*AFLA*/ #define __pgd(x)	native_make_pgd(x)
// /*AFLA*/ 
// /*AFLA*/ #ifndef __PAGETABLE_PUD_FOLDED
// /*AFLA*/ #define pud_val(x)	native_pud_val(x)
// /*AFLA*/ #define __pud(x)	native_make_pud(x)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __PAGETABLE_PMD_FOLDED
// /*AFLA*/ #define pmd_val(x)	native_pmd_val(x)
// /*AFLA*/ #define __pmd(x)	native_make_pmd(x)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define pte_val(x)	native_pte_val(x)
// /*AFLA*/ #define __pte(x)	native_make_pte(x)
// /*AFLA*/ 
// /*AFLA*/ #define arch_end_context_switch(prev)	do {} while(0)
// /*AFLA*/ 
// /*AFLA*/ #endif	/* CONFIG_PARAVIRT */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The following only work if pte_present() is true.
// /*AFLA*/  * Undefined behaviour if not..
// /*AFLA*/  */
// /*AFLA*/ static inline int pte_dirty(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_flags(pte) & _PAGE_DIRTY;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ static inline u32 read_pkru(void)
// /*AFLA*/ {
// /*AFLA*/ 	if (boot_cpu_has(X86_FEATURE_OSPKE))
// /*AFLA*/ 		return __read_pkru();
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_pkru(u32 pkru)
// /*AFLA*/ {
// /*AFLA*/ 	if (boot_cpu_has(X86_FEATURE_OSPKE))
// /*AFLA*/ 		__write_pkru(pkru);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pte_young(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_flags(pte) & _PAGE_ACCESSED;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pmd_dirty(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_flags(pmd) & _PAGE_DIRTY;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pmd_young(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_flags(pmd) & _PAGE_ACCESSED;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pte_write(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_flags(pte) & _PAGE_RW;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pte_huge(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_flags(pte) & _PAGE_PSE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pte_global(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_flags(pte) & _PAGE_GLOBAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pte_exec(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return !(pte_flags(pte) & _PAGE_NX);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pte_special(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_flags(pte) & _PAGE_SPECIAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long pte_pfn(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return (pte_val(pte) & PTE_PFN_MASK) >> PAGE_SHIFT;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long pmd_pfn(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return (pmd_val(pmd) & pmd_pfn_mask(pmd)) >> PAGE_SHIFT;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long pud_pfn(pud_t pud)
// /*AFLA*/ {
// /*AFLA*/ 	return (pud_val(pud) & pud_pfn_mask(pud)) >> PAGE_SHIFT;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define pte_page(pte)	pfn_to_page(pte_pfn(pte))
// /*AFLA*/ 
// /*AFLA*/ static inline int pmd_large(pmd_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_flags(pte) & _PAGE_PSE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ static inline int pmd_trans_huge(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return (pmd_val(pmd) & (_PAGE_PSE|_PAGE_DEVMAP)) == _PAGE_PSE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define has_transparent_hugepage has_transparent_hugepage
// /*AFLA*/ static inline int has_transparent_hugepage(void)
// /*AFLA*/ {
// /*AFLA*/ 	return boot_cpu_has(X86_FEATURE_PSE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef __HAVE_ARCH_PTE_DEVMAP
// /*AFLA*/ static inline int pmd_devmap(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return !!(pmd_val(pmd) & _PAGE_DEVMAP);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* CONFIG_TRANSPARENT_HUGEPAGE */
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_set_flags(pte_t pte, pteval_t set)
// /*AFLA*/ {
// /*AFLA*/ 	pteval_t v = native_pte_val(pte);
// /*AFLA*/ 
// /*AFLA*/ 	return native_make_pte(v | set);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_clear_flags(pte_t pte, pteval_t clear)
// /*AFLA*/ {
// /*AFLA*/ 	pteval_t v = native_pte_val(pte);
// /*AFLA*/ 
// /*AFLA*/ 	return native_make_pte(v & ~clear);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_mkclean(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_clear_flags(pte, _PAGE_DIRTY);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_mkold(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_clear_flags(pte, _PAGE_ACCESSED);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_wrprotect(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_clear_flags(pte, _PAGE_RW);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_mkexec(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_clear_flags(pte, _PAGE_NX);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_mkdirty(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_set_flags(pte, _PAGE_DIRTY | _PAGE_SOFT_DIRTY);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_mkyoung(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_set_flags(pte, _PAGE_ACCESSED);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_mkwrite(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_set_flags(pte, _PAGE_RW);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_mkhuge(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_set_flags(pte, _PAGE_PSE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_clrhuge(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_clear_flags(pte, _PAGE_PSE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_mkglobal(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_set_flags(pte, _PAGE_GLOBAL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_clrglobal(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_clear_flags(pte, _PAGE_GLOBAL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_mkspecial(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_set_flags(pte, _PAGE_SPECIAL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_mkdevmap(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_set_flags(pte, _PAGE_SPECIAL|_PAGE_DEVMAP);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pmd_set_flags(pmd_t pmd, pmdval_t set)
// /*AFLA*/ {
// /*AFLA*/ 	pmdval_t v = native_pmd_val(pmd);
// /*AFLA*/ 
// /*AFLA*/ 	return __pmd(v | set);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pmd_clear_flags(pmd_t pmd, pmdval_t clear)
// /*AFLA*/ {
// /*AFLA*/ 	pmdval_t v = native_pmd_val(pmd);
// /*AFLA*/ 
// /*AFLA*/ 	return __pmd(v & ~clear);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pmd_mkold(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_clear_flags(pmd, _PAGE_ACCESSED);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pmd_mkclean(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_clear_flags(pmd, _PAGE_DIRTY);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pmd_wrprotect(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_clear_flags(pmd, _PAGE_RW);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pmd_mkdirty(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_set_flags(pmd, _PAGE_DIRTY | _PAGE_SOFT_DIRTY);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pmd_mkdevmap(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_set_flags(pmd, _PAGE_DEVMAP);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pmd_mkhuge(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_set_flags(pmd, _PAGE_PSE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pmd_mkyoung(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_set_flags(pmd, _PAGE_ACCESSED);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pmd_mkwrite(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_set_flags(pmd, _PAGE_RW);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pmd_mknotpresent(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_clear_flags(pmd, _PAGE_PRESENT | _PAGE_PROTNONE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_ARCH_SOFT_DIRTY
// /*AFLA*/ static inline int pte_soft_dirty(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_flags(pte) & _PAGE_SOFT_DIRTY;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pmd_soft_dirty(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_flags(pmd) & _PAGE_SOFT_DIRTY;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_mksoft_dirty(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_set_flags(pte, _PAGE_SOFT_DIRTY);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pmd_mksoft_dirty(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_set_flags(pmd, _PAGE_SOFT_DIRTY);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_clear_soft_dirty(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_clear_flags(pte, _PAGE_SOFT_DIRTY);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pmd_clear_soft_dirty(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_clear_flags(pmd, _PAGE_SOFT_DIRTY);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_HAVE_ARCH_SOFT_DIRTY */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Mask out unsupported bits in a present pgprot.  Non-present pgprots
// /*AFLA*/  * can use those bits for other purposes, so leave them be.
// /*AFLA*/  */
// /*AFLA*/ static inline pgprotval_t massage_pgprot(pgprot_t pgprot)
// /*AFLA*/ {
// /*AFLA*/ 	pgprotval_t protval = pgprot_val(pgprot);
// /*AFLA*/ 
// /*AFLA*/ 	if (protval & _PAGE_PRESENT)
// /*AFLA*/ 		protval &= __supported_pte_mask;
// /*AFLA*/ 
// /*AFLA*/ 	return protval;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pfn_pte(unsigned long page_nr, pgprot_t pgprot)
// /*AFLA*/ {
// /*AFLA*/ 	return __pte(((phys_addr_t)page_nr << PAGE_SHIFT) |
// /*AFLA*/ 		     massage_pgprot(pgprot));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pfn_pmd(unsigned long page_nr, pgprot_t pgprot)
// /*AFLA*/ {
// /*AFLA*/ 	return __pmd(((phys_addr_t)page_nr << PAGE_SHIFT) |
// /*AFLA*/ 		     massage_pgprot(pgprot));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_modify(pte_t pte, pgprot_t newprot)
// /*AFLA*/ {
// /*AFLA*/ 	pteval_t val = pte_val(pte);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Chop off the NX bit (if present), and add the NX portion of
// /*AFLA*/ 	 * the newprot (if present):
// /*AFLA*/ 	 */
// /*AFLA*/ 	val &= _PAGE_CHG_MASK;
// /*AFLA*/ 	val |= massage_pgprot(newprot) & ~_PAGE_CHG_MASK;
// /*AFLA*/ 
// /*AFLA*/ 	return __pte(val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pmd_modify(pmd_t pmd, pgprot_t newprot)
// /*AFLA*/ {
// /*AFLA*/ 	pmdval_t val = pmd_val(pmd);
// /*AFLA*/ 
// /*AFLA*/ 	val &= _HPAGE_CHG_MASK;
// /*AFLA*/ 	val |= massage_pgprot(newprot) & ~_HPAGE_CHG_MASK;
// /*AFLA*/ 
// /*AFLA*/ 	return __pmd(val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* mprotect needs to preserve PAT bits when updating vm_page_prot */
// /*AFLA*/ #define pgprot_modify pgprot_modify
// /*AFLA*/ static inline pgprot_t pgprot_modify(pgprot_t oldprot, pgprot_t newprot)
// /*AFLA*/ {
// /*AFLA*/ 	pgprotval_t preservebits = pgprot_val(oldprot) & _PAGE_CHG_MASK;
// /*AFLA*/ 	pgprotval_t addbits = pgprot_val(newprot);
// /*AFLA*/ 	return __pgprot(preservebits | addbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define pte_pgprot(x) __pgprot(pte_flags(x))
// /*AFLA*/ #define pmd_pgprot(x) __pgprot(pmd_flags(x))
// /*AFLA*/ #define pud_pgprot(x) __pgprot(pud_flags(x))
// /*AFLA*/ 
// /*AFLA*/ #define canon_pgprot(p) __pgprot(massage_pgprot(p))
// /*AFLA*/ 
// /*AFLA*/ static inline int is_new_memtype_allowed(u64 paddr, unsigned long size,
// /*AFLA*/ 					 enum page_cache_mode pcm,
// /*AFLA*/ 					 enum page_cache_mode new_pcm)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * PAT type is always WB for untracked ranges, so no need to check.
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (x86_platform.is_untracked_pat_range(paddr, paddr + size))
// /*AFLA*/ 		return 1;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Certain new memtypes are not allowed with certain
// /*AFLA*/ 	 * requested memtype:
// /*AFLA*/ 	 * - request is uncached, return cannot be write-back
// /*AFLA*/ 	 * - request is write-combine, return cannot be write-back
// /*AFLA*/ 	 * - request is write-through, return cannot be write-back
// /*AFLA*/ 	 * - request is write-through, return cannot be write-combine
// /*AFLA*/ 	 */
// /*AFLA*/ 	if ((pcm == _PAGE_CACHE_MODE_UC_MINUS &&
// /*AFLA*/ 	     new_pcm == _PAGE_CACHE_MODE_WB) ||
// /*AFLA*/ 	    (pcm == _PAGE_CACHE_MODE_WC &&
// /*AFLA*/ 	     new_pcm == _PAGE_CACHE_MODE_WB) ||
// /*AFLA*/ 	    (pcm == _PAGE_CACHE_MODE_WT &&
// /*AFLA*/ 	     new_pcm == _PAGE_CACHE_MODE_WB) ||
// /*AFLA*/ 	    (pcm == _PAGE_CACHE_MODE_WT &&
// /*AFLA*/ 	     new_pcm == _PAGE_CACHE_MODE_WC)) {
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	return 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ pmd_t *populate_extra_pmd(unsigned long vaddr);
// /*AFLA*/ pte_t *populate_extra_pte(unsigned long vaddr);
// /*AFLA*/ #endif	/* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ # include <asm/pgtable_32.h>
// /*AFLA*/ #else
// /*AFLA*/ # include <asm/pgtable_64.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #include <linux/mm_types.h>
// /*AFLA*/ #include <linux/mmdebug.h>
// /*AFLA*/ #include <linux/log2.h>
// /*AFLA*/ 
// /*AFLA*/ static inline int pte_none(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return !(pte.pte & ~(_PAGE_KNL_ERRATUM_MASK));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_PTE_SAME
// /*AFLA*/ static inline int pte_same(pte_t a, pte_t b)
// /*AFLA*/ {
// /*AFLA*/ 	return a.pte == b.pte;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pte_present(pte_t a)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_flags(a) & (_PAGE_PRESENT | _PAGE_PROTNONE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef __HAVE_ARCH_PTE_DEVMAP
// /*AFLA*/ static inline int pte_devmap(pte_t a)
// /*AFLA*/ {
// /*AFLA*/ 	return (pte_flags(a) & _PAGE_DEVMAP) == _PAGE_DEVMAP;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define pte_accessible pte_accessible
// /*AFLA*/ static inline bool pte_accessible(struct mm_struct *mm, pte_t a)
// /*AFLA*/ {
// /*AFLA*/ 	if (pte_flags(a) & _PAGE_PRESENT)
// /*AFLA*/ 		return true;
// /*AFLA*/ 
// /*AFLA*/ 	if ((pte_flags(a) & _PAGE_PROTNONE) &&
// /*AFLA*/ 			mm_tlb_flush_pending(mm))
// /*AFLA*/ 		return true;
// /*AFLA*/ 
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pte_hidden(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_flags(pte) & _PAGE_HIDDEN;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pmd_present(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Checking for _PAGE_PSE is needed too because
// /*AFLA*/ 	 * split_huge_page will temporarily clear the present bit (but
// /*AFLA*/ 	 * the _PAGE_PSE flag will remain set at all times while the
// /*AFLA*/ 	 * _PAGE_PRESENT bit is clear).
// /*AFLA*/ 	 */
// /*AFLA*/ 	return pmd_flags(pmd) & (_PAGE_PRESENT | _PAGE_PROTNONE | _PAGE_PSE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA_BALANCING
// /*AFLA*/ /*
// /*AFLA*/  * These work without NUMA balancing but the kernel does not care. See the
// /*AFLA*/  * comment in include/asm-generic/pgtable.h
// /*AFLA*/  */
// /*AFLA*/ static inline int pte_protnone(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return (pte_flags(pte) & (_PAGE_PROTNONE | _PAGE_PRESENT))
// /*AFLA*/ 		== _PAGE_PROTNONE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pmd_protnone(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return (pmd_flags(pmd) & (_PAGE_PROTNONE | _PAGE_PRESENT))
// /*AFLA*/ 		== _PAGE_PROTNONE;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_NUMA_BALANCING */
// /*AFLA*/ 
// /*AFLA*/ static inline int pmd_none(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	/* Only check low word on 32-bit platforms, since it might be
// /*AFLA*/ 	   out of sync with upper half. */
// /*AFLA*/ 	unsigned long val = native_pmd_val(pmd);
// /*AFLA*/ 	return (val & ~_PAGE_KNL_ERRATUM_MASK) == 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long pmd_page_vaddr(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return (unsigned long)__va(pmd_val(pmd) & pmd_pfn_mask(pmd));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Currently stuck as a macro due to indirect forward reference to
// /*AFLA*/  * linux/mmzone.h's __section_mem_map_addr() definition:
// /*AFLA*/  */
// /*AFLA*/ #define pmd_page(pmd)		\
// /*AFLA*/ 	pfn_to_page((pmd_val(pmd) & pmd_pfn_mask(pmd)) >> PAGE_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * the pmd page can be thought of an array like this: pmd_t[PTRS_PER_PMD]
// /*AFLA*/  *
// /*AFLA*/  * this macro returns the index of the entry in the pmd page which would
// /*AFLA*/  * control the given virtual address
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long pmd_index(unsigned long address)
// /*AFLA*/ {
// /*AFLA*/ 	return (address >> PMD_SHIFT) & (PTRS_PER_PMD - 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Conversion functions: convert a page and protection to a page entry,
// /*AFLA*/  * and a page entry and page directory to the page they refer to.
// /*AFLA*/  *
// /*AFLA*/  * (Currently stuck as a macro because of indirect forward reference
// /*AFLA*/  * to linux/mm.h:page_to_nid())
// /*AFLA*/  */
// /*AFLA*/ #define mk_pte(page, pgprot)   pfn_pte(page_to_pfn(page), (pgprot))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * the pte page can be thought of an array like this: pte_t[PTRS_PER_PTE]
// /*AFLA*/  *
// /*AFLA*/  * this function returns the index of the entry in the pte page which would
// /*AFLA*/  * control the given virtual address
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long pte_index(unsigned long address)
// /*AFLA*/ {
// /*AFLA*/ 	return (address >> PAGE_SHIFT) & (PTRS_PER_PTE - 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t *pte_offset_kernel(pmd_t *pmd, unsigned long address)
// /*AFLA*/ {
// /*AFLA*/ 	return (pte_t *)pmd_page_vaddr(*pmd) + pte_index(address);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pmd_bad(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return (pmd_flags(pmd) & ~_PAGE_USER) != _KERNPG_TABLE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long pages_to_mb(unsigned long npg)
// /*AFLA*/ {
// /*AFLA*/ 	return npg >> (20 - PAGE_SHIFT);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #if CONFIG_PGTABLE_LEVELS > 2
// /*AFLA*/ static inline int pud_none(pud_t pud)
// /*AFLA*/ {
// /*AFLA*/ 	return (native_pud_val(pud) & ~(_PAGE_KNL_ERRATUM_MASK)) == 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pud_present(pud_t pud)
// /*AFLA*/ {
// /*AFLA*/ 	return pud_flags(pud) & _PAGE_PRESENT;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long pud_page_vaddr(pud_t pud)
// /*AFLA*/ {
// /*AFLA*/ 	return (unsigned long)__va(pud_val(pud) & pud_pfn_mask(pud));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Currently stuck as a macro due to indirect forward reference to
// /*AFLA*/  * linux/mmzone.h's __section_mem_map_addr() definition:
// /*AFLA*/  */
// /*AFLA*/ #define pud_page(pud)		\
// /*AFLA*/ 	pfn_to_page((pud_val(pud) & pud_pfn_mask(pud)) >> PAGE_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ /* Find an entry in the second-level page table.. */
// /*AFLA*/ static inline pmd_t *pmd_offset(pud_t *pud, unsigned long address)
// /*AFLA*/ {
// /*AFLA*/ 	return (pmd_t *)pud_page_vaddr(*pud) + pmd_index(address);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pud_large(pud_t pud)
// /*AFLA*/ {
// /*AFLA*/ 	return (pud_val(pud) & (_PAGE_PSE | _PAGE_PRESENT)) ==
// /*AFLA*/ 		(_PAGE_PSE | _PAGE_PRESENT);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pud_bad(pud_t pud)
// /*AFLA*/ {
// /*AFLA*/ 	return (pud_flags(pud) & ~(_KERNPG_TABLE | _PAGE_USER)) != 0;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline int pud_large(pud_t pud)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif	/* CONFIG_PGTABLE_LEVELS > 2 */
// /*AFLA*/ 
// /*AFLA*/ #if CONFIG_PGTABLE_LEVELS > 3
// /*AFLA*/ static inline int pgd_present(pgd_t pgd)
// /*AFLA*/ {
// /*AFLA*/ 	return pgd_flags(pgd) & _PAGE_PRESENT;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long pgd_page_vaddr(pgd_t pgd)
// /*AFLA*/ {
// /*AFLA*/ 	return (unsigned long)__va((unsigned long)pgd_val(pgd) & PTE_PFN_MASK);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Currently stuck as a macro due to indirect forward reference to
// /*AFLA*/  * linux/mmzone.h's __section_mem_map_addr() definition:
// /*AFLA*/  */
// /*AFLA*/ #define pgd_page(pgd)		pfn_to_page(pgd_val(pgd) >> PAGE_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ /* to find an entry in a page-table-directory. */
// /*AFLA*/ static inline unsigned long pud_index(unsigned long address)
// /*AFLA*/ {
// /*AFLA*/ 	return (address >> PUD_SHIFT) & (PTRS_PER_PUD - 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pud_t *pud_offset(pgd_t *pgd, unsigned long address)
// /*AFLA*/ {
// /*AFLA*/ 	return (pud_t *)pgd_page_vaddr(*pgd) + pud_index(address);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pgd_bad(pgd_t pgd)
// /*AFLA*/ {
// /*AFLA*/ 	return (pgd_flags(pgd) & ~_PAGE_USER) != _KERNPG_TABLE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pgd_none(pgd_t pgd)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * There is no need to do a workaround for the KNL stray
// /*AFLA*/ 	 * A/D bit erratum here.  PGDs only point to page tables
// /*AFLA*/ 	 * except on 32-bit non-PAE which is not supported on
// /*AFLA*/ 	 * KNL.
// /*AFLA*/ 	 */
// /*AFLA*/ 	return !native_pgd_val(pgd);
// /*AFLA*/ }
// /*AFLA*/ #endif	/* CONFIG_PGTABLE_LEVELS > 3 */
// /*AFLA*/ 
// /*AFLA*/ #endif	/* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * the pgd page can be thought of an array like this: pgd_t[PTRS_PER_PGD]
// /*AFLA*/  *
// /*AFLA*/  * this macro returns the index of the entry in the pgd page which would
// /*AFLA*/  * control the given virtual address
// /*AFLA*/  */
// /*AFLA*/ #define pgd_index(address) (((address) >> PGDIR_SHIFT) & (PTRS_PER_PGD - 1))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * pgd_offset() returns a (pgd_t *)
// /*AFLA*/  * pgd_index() is used get the offset into the pgd page's array of pgd_t's;
// /*AFLA*/  */
// /*AFLA*/ #define pgd_offset(mm, address) ((mm)->pgd + pgd_index((address)))
// /*AFLA*/ /*
// /*AFLA*/  * a shortcut which implies the use of the kernel's pgd, instead
// /*AFLA*/  * of a process's
// /*AFLA*/  */
// /*AFLA*/ #define pgd_offset_k(address) pgd_offset(&init_mm, (address))
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define KERNEL_PGD_BOUNDARY	pgd_index(PAGE_OFFSET)
// /*AFLA*/ #define KERNEL_PGD_PTRS		(PTRS_PER_PGD - KERNEL_PGD_BOUNDARY)
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ extern int direct_gbpages;
// /*AFLA*/ void init_mem_mapping(void);
// /*AFLA*/ void early_alloc_pgt_buf(void);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ /* Realmode trampoline initialization. */
// /*AFLA*/ extern pgd_t trampoline_pgd_entry;
// /*AFLA*/ static inline void __meminit init_trampoline_default(void)
// /*AFLA*/ {
// /*AFLA*/ 	/* Default trampoline pgd value */
// /*AFLA*/ 	trampoline_pgd_entry = init_level4_pgt[pgd_index(__PAGE_OFFSET)];
// /*AFLA*/ }
// /*AFLA*/ # ifdef CONFIG_RANDOMIZE_MEMORY
// /*AFLA*/ void __meminit init_trampoline(void);
// /*AFLA*/ # else
// /*AFLA*/ #  define init_trampoline init_trampoline_default
// /*AFLA*/ # endif
// /*AFLA*/ #else
// /*AFLA*/ static inline void init_trampoline(void) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* local pte updates need not use xchg for locking */
// /*AFLA*/ static inline pte_t native_local_ptep_get_and_clear(pte_t *ptep)
// /*AFLA*/ {
// /*AFLA*/ 	pte_t res = *ptep;
// /*AFLA*/ 
// /*AFLA*/ 	/* Pure native function needs no input for mm, addr */
// /*AFLA*/ 	native_pte_clear(NULL, 0, ptep);
// /*AFLA*/ 	return res;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t native_local_pmdp_get_and_clear(pmd_t *pmdp)
// /*AFLA*/ {
// /*AFLA*/ 	pmd_t res = *pmdp;
// /*AFLA*/ 
// /*AFLA*/ 	native_pmd_clear(pmdp);
// /*AFLA*/ 	return res;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_set_pte_at(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 				     pte_t *ptep , pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	native_set_pte(ptep, pte);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_set_pmd_at(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 				     pmd_t *pmdp , pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	native_set_pmd(pmdp, pmd);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_PARAVIRT
// /*AFLA*/ /*
// /*AFLA*/  * Rules for using pte_update - it must be called after any PTE update which
// /*AFLA*/  * has not been done using the set_pte / clear_pte interfaces.  It is used by
// /*AFLA*/  * shadow mode hypervisors to resynchronize the shadow page tables.  Kernel PTE
// /*AFLA*/  * updates should either be sets, clears, or set_pte_atomic for P->P
// /*AFLA*/  * transitions, which means this hook should only be called for user PTEs.
// /*AFLA*/  * This hook implies a P->P protection or access change has taken place, which
// /*AFLA*/  * requires a subsequent TLB flush.
// /*AFLA*/  */
// /*AFLA*/ #define pte_update(mm, addr, ptep)		do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We only update the dirty/accessed state if we set
// /*AFLA*/  * the dirty bit by hand in the kernel, since the hardware
// /*AFLA*/  * will do the accessed bit for us, and we don't want to
// /*AFLA*/  * race with other CPU's that might be updating the dirty
// /*AFLA*/  * bit at the same time.
// /*AFLA*/  */
// /*AFLA*/ struct vm_area_struct;
// /*AFLA*/ 
// /*AFLA*/ #define  __HAVE_ARCH_PTEP_SET_ACCESS_FLAGS
// /*AFLA*/ extern int ptep_set_access_flags(struct vm_area_struct *vma,
// /*AFLA*/ 				 unsigned long address, pte_t *ptep,
// /*AFLA*/ 				 pte_t entry, int dirty);
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_PTEP_TEST_AND_CLEAR_YOUNG
// /*AFLA*/ extern int ptep_test_and_clear_young(struct vm_area_struct *vma,
// /*AFLA*/ 				     unsigned long addr, pte_t *ptep);
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_PTEP_CLEAR_YOUNG_FLUSH
// /*AFLA*/ extern int ptep_clear_flush_young(struct vm_area_struct *vma,
// /*AFLA*/ 				  unsigned long address, pte_t *ptep);
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_PTEP_GET_AND_CLEAR
// /*AFLA*/ static inline pte_t ptep_get_and_clear(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 				       pte_t *ptep)
// /*AFLA*/ {
// /*AFLA*/ 	pte_t pte = native_ptep_get_and_clear(ptep);
// /*AFLA*/ 	pte_update(mm, addr, ptep);
// /*AFLA*/ 	return pte;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_PTEP_GET_AND_CLEAR_FULL
// /*AFLA*/ static inline pte_t ptep_get_and_clear_full(struct mm_struct *mm,
// /*AFLA*/ 					    unsigned long addr, pte_t *ptep,
// /*AFLA*/ 					    int full)
// /*AFLA*/ {
// /*AFLA*/ 	pte_t pte;
// /*AFLA*/ 	if (full) {
// /*AFLA*/ 		/*
// /*AFLA*/ 		 * Full address destruction in progress; paravirt does not
// /*AFLA*/ 		 * care about updates and native needs no locking
// /*AFLA*/ 		 */
// /*AFLA*/ 		pte = native_local_ptep_get_and_clear(ptep);
// /*AFLA*/ 	} else {
// /*AFLA*/ 		pte = ptep_get_and_clear(mm, addr, ptep);
// /*AFLA*/ 	}
// /*AFLA*/ 	return pte;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_PTEP_SET_WRPROTECT
// /*AFLA*/ static inline void ptep_set_wrprotect(struct mm_struct *mm,
// /*AFLA*/ 				      unsigned long addr, pte_t *ptep)
// /*AFLA*/ {
// /*AFLA*/ 	clear_bit(_PAGE_BIT_RW, (unsigned long *)&ptep->pte);
// /*AFLA*/ 	pte_update(mm, addr, ptep);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define flush_tlb_fix_spurious_fault(vma, address) do { } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define mk_pmd(page, pgprot)   pfn_pmd(page_to_pfn(page), (pgprot))
// /*AFLA*/ 
// /*AFLA*/ #define  __HAVE_ARCH_PMDP_SET_ACCESS_FLAGS
// /*AFLA*/ extern int pmdp_set_access_flags(struct vm_area_struct *vma,
// /*AFLA*/ 				 unsigned long address, pmd_t *pmdp,
// /*AFLA*/ 				 pmd_t entry, int dirty);
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_PMDP_TEST_AND_CLEAR_YOUNG
// /*AFLA*/ extern int pmdp_test_and_clear_young(struct vm_area_struct *vma,
// /*AFLA*/ 				     unsigned long addr, pmd_t *pmdp);
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_PMDP_CLEAR_YOUNG_FLUSH
// /*AFLA*/ extern int pmdp_clear_flush_young(struct vm_area_struct *vma,
// /*AFLA*/ 				  unsigned long address, pmd_t *pmdp);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_PMD_WRITE
// /*AFLA*/ static inline int pmd_write(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_flags(pmd) & _PAGE_RW;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_PMDP_HUGE_GET_AND_CLEAR
// /*AFLA*/ static inline pmd_t pmdp_huge_get_and_clear(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 				       pmd_t *pmdp)
// /*AFLA*/ {
// /*AFLA*/ 	return native_pmdp_get_and_clear(pmdp);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_PMDP_SET_WRPROTECT
// /*AFLA*/ static inline void pmdp_set_wrprotect(struct mm_struct *mm,
// /*AFLA*/ 				      unsigned long addr, pmd_t *pmdp)
// /*AFLA*/ {
// /*AFLA*/ 	clear_bit(_PAGE_BIT_RW, (unsigned long *)pmdp);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * clone_pgd_range(pgd_t *dst, pgd_t *src, int count);
// /*AFLA*/  *
// /*AFLA*/  *  dst - pointer to pgd range anwhere on a pgd page
// /*AFLA*/  *  src - ""
// /*AFLA*/  *  count - the number of pgds to copy.
// /*AFLA*/  *
// /*AFLA*/  * dst and src can be on the same page, but the range must not overlap,
// /*AFLA*/  * and must not cross a page boundary.
// /*AFLA*/  */
// /*AFLA*/ static inline void clone_pgd_range(pgd_t *dst, pgd_t *src, int count)
// /*AFLA*/ {
// /*AFLA*/        memcpy(dst, src, count * sizeof(pgd_t));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define PTE_SHIFT ilog2(PTRS_PER_PTE)
// /*AFLA*/ static inline int page_level_shift(enum pg_level level)
// /*AFLA*/ {
// /*AFLA*/ 	return (PAGE_SHIFT - PTE_SHIFT) + level * PTE_SHIFT;
// /*AFLA*/ }
// /*AFLA*/ static inline unsigned long page_level_size(enum pg_level level)
// /*AFLA*/ {
// /*AFLA*/ 	return 1UL << page_level_shift(level);
// /*AFLA*/ }
// /*AFLA*/ static inline unsigned long page_level_mask(enum pg_level level)
// /*AFLA*/ {
// /*AFLA*/ 	return ~(page_level_size(level) - 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The x86 doesn't have any external MMU info: the kernel page
// /*AFLA*/  * tables contain all the necessary information.
// /*AFLA*/  */
// /*AFLA*/ static inline void update_mmu_cache(struct vm_area_struct *vma,
// /*AFLA*/ 		unsigned long addr, pte_t *ptep)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void update_mmu_cache_pmd(struct vm_area_struct *vma,
// /*AFLA*/ 		unsigned long addr, pmd_t *pmd)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_ARCH_SOFT_DIRTY
// /*AFLA*/ static inline pte_t pte_swp_mksoft_dirty(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_set_flags(pte, _PAGE_SWP_SOFT_DIRTY);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pte_swp_soft_dirty(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_flags(pte) & _PAGE_SWP_SOFT_DIRTY;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_swp_clear_soft_dirty(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_clear_flags(pte, _PAGE_SWP_SOFT_DIRTY);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define PKRU_AD_BIT 0x1
// /*AFLA*/ #define PKRU_WD_BIT 0x2
// /*AFLA*/ #define PKRU_BITS_PER_PKEY 2
// /*AFLA*/ 
// /*AFLA*/ static inline bool __pkru_allows_read(u32 pkru, u16 pkey)
// /*AFLA*/ {
// /*AFLA*/ 	int pkru_pkey_bits = pkey * PKRU_BITS_PER_PKEY;
// /*AFLA*/ 	return !(pkru & (PKRU_AD_BIT << pkru_pkey_bits));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool __pkru_allows_write(u32 pkru, u16 pkey)
// /*AFLA*/ {
// /*AFLA*/ 	int pkru_pkey_bits = pkey * PKRU_BITS_PER_PKEY;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Access-disable disables writes too so we need to check
// /*AFLA*/ 	 * both bits here.
// /*AFLA*/ 	 */
// /*AFLA*/ 	return !(pkru & ((PKRU_AD_BIT|PKRU_WD_BIT) << pkru_pkey_bits));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u16 pte_flags_pkey(unsigned long pte_flags)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS
// /*AFLA*/ 	/* ifdef to avoid doing 59-bit shift on 32-bit values */
// /*AFLA*/ 	return (pte_flags & _PAGE_PKEY_MASK) >> _PAGE_BIT_PKEY_BIT0;
// /*AFLA*/ #else
// /*AFLA*/ 	return 0;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/pgtable.h>
// /*AFLA*/ #endif	/* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_PGTABLE_H */
