// /*AFLA*/ #ifndef _ASM_GENERIC_PGTABLE_H
// /*AFLA*/ #define _ASM_GENERIC_PGTABLE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/pfn.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ 
// /*AFLA*/ #include <linux/mm_types.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ 
// /*AFLA*/ #if 4 - defined(__PAGETABLE_PUD_FOLDED) - defined(__PAGETABLE_PMD_FOLDED) != \
// /*AFLA*/ 	CONFIG_PGTABLE_LEVELS
// /*AFLA*/ #error CONFIG_PGTABLE_LEVELS is not consistent with __PAGETABLE_{PUD,PMD}_FOLDED
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * On almost all architectures and configurations, 0 can be used as the
// /*AFLA*/  * upper ceiling to free_pgtables(): on many architectures it has the same
// /*AFLA*/  * effect as using TASK_SIZE.  However, there is one configuration which
// /*AFLA*/  * must impose a more careful limit, to avoid freeing kernel pgtables.
// /*AFLA*/  */
// /*AFLA*/ #ifndef USER_PGTABLES_CEILING
// /*AFLA*/ #define USER_PGTABLES_CEILING	0UL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PTEP_SET_ACCESS_FLAGS
// /*AFLA*/ extern int ptep_set_access_flags(struct vm_area_struct *vma,
// /*AFLA*/ 				 unsigned long address, pte_t *ptep,
// /*AFLA*/ 				 pte_t entry, int dirty);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PMDP_SET_ACCESS_FLAGS
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ extern int pmdp_set_access_flags(struct vm_area_struct *vma,
// /*AFLA*/ 				 unsigned long address, pmd_t *pmdp,
// /*AFLA*/ 				 pmd_t entry, int dirty);
// /*AFLA*/ #else
// /*AFLA*/ static inline int pmdp_set_access_flags(struct vm_area_struct *vma,
// /*AFLA*/ 					unsigned long address, pmd_t *pmdp,
// /*AFLA*/ 					pmd_t entry, int dirty)
// /*AFLA*/ {
// /*AFLA*/ 	BUILD_BUG();
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_TRANSPARENT_HUGEPAGE */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PTEP_TEST_AND_CLEAR_YOUNG
// /*AFLA*/ static inline int ptep_test_and_clear_young(struct vm_area_struct *vma,
// /*AFLA*/ 					    unsigned long address,
// /*AFLA*/ 					    pte_t *ptep)
// /*AFLA*/ {
// /*AFLA*/ 	pte_t pte = *ptep;
// /*AFLA*/ 	int r = 1;
// /*AFLA*/ 	if (!pte_young(pte))
// /*AFLA*/ 		r = 0;
// /*AFLA*/ 	else
// /*AFLA*/ 		set_pte_at(vma->vm_mm, address, ptep, pte_mkold(pte));
// /*AFLA*/ 	return r;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PMDP_TEST_AND_CLEAR_YOUNG
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ static inline int pmdp_test_and_clear_young(struct vm_area_struct *vma,
// /*AFLA*/ 					    unsigned long address,
// /*AFLA*/ 					    pmd_t *pmdp)
// /*AFLA*/ {
// /*AFLA*/ 	pmd_t pmd = *pmdp;
// /*AFLA*/ 	int r = 1;
// /*AFLA*/ 	if (!pmd_young(pmd))
// /*AFLA*/ 		r = 0;
// /*AFLA*/ 	else
// /*AFLA*/ 		set_pmd_at(vma->vm_mm, address, pmdp, pmd_mkold(pmd));
// /*AFLA*/ 	return r;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline int pmdp_test_and_clear_young(struct vm_area_struct *vma,
// /*AFLA*/ 					    unsigned long address,
// /*AFLA*/ 					    pmd_t *pmdp)
// /*AFLA*/ {
// /*AFLA*/ 	BUILD_BUG();
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_TRANSPARENT_HUGEPAGE */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PTEP_CLEAR_YOUNG_FLUSH
// /*AFLA*/ int ptep_clear_flush_young(struct vm_area_struct *vma,
// /*AFLA*/ 			   unsigned long address, pte_t *ptep);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PMDP_CLEAR_YOUNG_FLUSH
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ extern int pmdp_clear_flush_young(struct vm_area_struct *vma,
// /*AFLA*/ 				  unsigned long address, pmd_t *pmdp);
// /*AFLA*/ #else
// /*AFLA*/ /*
// /*AFLA*/  * Despite relevant to THP only, this API is called from generic rmap code
// /*AFLA*/  * under PageTransHuge(), hence needs a dummy implementation for !THP
// /*AFLA*/  */
// /*AFLA*/ static inline int pmdp_clear_flush_young(struct vm_area_struct *vma,
// /*AFLA*/ 					 unsigned long address, pmd_t *pmdp)
// /*AFLA*/ {
// /*AFLA*/ 	BUILD_BUG();
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_TRANSPARENT_HUGEPAGE */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PTEP_GET_AND_CLEAR
// /*AFLA*/ static inline pte_t ptep_get_and_clear(struct mm_struct *mm,
// /*AFLA*/ 				       unsigned long address,
// /*AFLA*/ 				       pte_t *ptep)
// /*AFLA*/ {
// /*AFLA*/ 	pte_t pte = *ptep;
// /*AFLA*/ 	pte_clear(mm, address, ptep);
// /*AFLA*/ 	return pte;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PMDP_HUGE_GET_AND_CLEAR
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ static inline pmd_t pmdp_huge_get_and_clear(struct mm_struct *mm,
// /*AFLA*/ 					    unsigned long address,
// /*AFLA*/ 					    pmd_t *pmdp)
// /*AFLA*/ {
// /*AFLA*/ 	pmd_t pmd = *pmdp;
// /*AFLA*/ 	pmd_clear(pmdp);
// /*AFLA*/ 	return pmd;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_TRANSPARENT_HUGEPAGE */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PMDP_HUGE_GET_AND_CLEAR_FULL
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ static inline pmd_t pmdp_huge_get_and_clear_full(struct mm_struct *mm,
// /*AFLA*/ 					    unsigned long address, pmd_t *pmdp,
// /*AFLA*/ 					    int full)
// /*AFLA*/ {
// /*AFLA*/ 	return pmdp_huge_get_and_clear(mm, address, pmdp);
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_TRANSPARENT_HUGEPAGE */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PTEP_GET_AND_CLEAR_FULL
// /*AFLA*/ static inline pte_t ptep_get_and_clear_full(struct mm_struct *mm,
// /*AFLA*/ 					    unsigned long address, pte_t *ptep,
// /*AFLA*/ 					    int full)
// /*AFLA*/ {
// /*AFLA*/ 	pte_t pte;
// /*AFLA*/ 	pte = ptep_get_and_clear(mm, address, ptep);
// /*AFLA*/ 	return pte;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Some architectures may be able to avoid expensive synchronization
// /*AFLA*/  * primitives when modifications are made to PTE's which are already
// /*AFLA*/  * not present, or in the process of an address space destruction.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __HAVE_ARCH_PTE_CLEAR_NOT_PRESENT_FULL
// /*AFLA*/ static inline void pte_clear_not_present_full(struct mm_struct *mm,
// /*AFLA*/ 					      unsigned long address,
// /*AFLA*/ 					      pte_t *ptep,
// /*AFLA*/ 					      int full)
// /*AFLA*/ {
// /*AFLA*/ 	pte_clear(mm, address, ptep);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PTEP_CLEAR_FLUSH
// /*AFLA*/ extern pte_t ptep_clear_flush(struct vm_area_struct *vma,
// /*AFLA*/ 			      unsigned long address,
// /*AFLA*/ 			      pte_t *ptep);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PMDP_HUGE_CLEAR_FLUSH
// /*AFLA*/ extern pmd_t pmdp_huge_clear_flush(struct vm_area_struct *vma,
// /*AFLA*/ 			      unsigned long address,
// /*AFLA*/ 			      pmd_t *pmdp);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PTEP_SET_WRPROTECT
// /*AFLA*/ struct mm_struct;
// /*AFLA*/ static inline void ptep_set_wrprotect(struct mm_struct *mm, unsigned long address, pte_t *ptep)
// /*AFLA*/ {
// /*AFLA*/ 	pte_t old_pte = *ptep;
// /*AFLA*/ 	set_pte_at(mm, address, ptep, pte_wrprotect(old_pte));
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PMDP_SET_WRPROTECT
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ static inline void pmdp_set_wrprotect(struct mm_struct *mm,
// /*AFLA*/ 				      unsigned long address, pmd_t *pmdp)
// /*AFLA*/ {
// /*AFLA*/ 	pmd_t old_pmd = *pmdp;
// /*AFLA*/ 	set_pmd_at(mm, address, pmdp, pmd_wrprotect(old_pmd));
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline void pmdp_set_wrprotect(struct mm_struct *mm,
// /*AFLA*/ 				      unsigned long address, pmd_t *pmdp)
// /*AFLA*/ {
// /*AFLA*/ 	BUILD_BUG();
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_TRANSPARENT_HUGEPAGE */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef pmdp_collapse_flush
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ extern pmd_t pmdp_collapse_flush(struct vm_area_struct *vma,
// /*AFLA*/ 				 unsigned long address, pmd_t *pmdp);
// /*AFLA*/ #else
// /*AFLA*/ static inline pmd_t pmdp_collapse_flush(struct vm_area_struct *vma,
// /*AFLA*/ 					unsigned long address,
// /*AFLA*/ 					pmd_t *pmdp)
// /*AFLA*/ {
// /*AFLA*/ 	BUILD_BUG();
// /*AFLA*/ 	return *pmdp;
// /*AFLA*/ }
// /*AFLA*/ #define pmdp_collapse_flush pmdp_collapse_flush
// /*AFLA*/ #endif /* CONFIG_TRANSPARENT_HUGEPAGE */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PGTABLE_DEPOSIT
// /*AFLA*/ extern void pgtable_trans_huge_deposit(struct mm_struct *mm, pmd_t *pmdp,
// /*AFLA*/ 				       pgtable_t pgtable);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PGTABLE_WITHDRAW
// /*AFLA*/ extern pgtable_t pgtable_trans_huge_withdraw(struct mm_struct *mm, pmd_t *pmdp);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PMDP_INVALIDATE
// /*AFLA*/ extern void pmdp_invalidate(struct vm_area_struct *vma, unsigned long address,
// /*AFLA*/ 			    pmd_t *pmdp);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PMDP_HUGE_SPLIT_PREPARE
// /*AFLA*/ static inline void pmdp_huge_split_prepare(struct vm_area_struct *vma,
// /*AFLA*/ 					   unsigned long address, pmd_t *pmdp)
// /*AFLA*/ {
// /*AFLA*/ 
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PTE_SAME
// /*AFLA*/ static inline int pte_same(pte_t pte_a, pte_t pte_b)
// /*AFLA*/ {
// /*AFLA*/ 	return pte_val(pte_a) == pte_val(pte_b);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PTE_UNUSED
// /*AFLA*/ /*
// /*AFLA*/  * Some architectures provide facilities to virtualization guests
// /*AFLA*/  * so that they can flag allocated pages as unused. This allows the
// /*AFLA*/  * host to transparently reclaim unused pages. This function returns
// /*AFLA*/  * whether the pte's page is unused.
// /*AFLA*/  */
// /*AFLA*/ static inline int pte_unused(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PMD_SAME
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ static inline int pmd_same(pmd_t pmd_a, pmd_t pmd_b)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd_val(pmd_a) == pmd_val(pmd_b);
// /*AFLA*/ }
// /*AFLA*/ #else /* CONFIG_TRANSPARENT_HUGEPAGE */
// /*AFLA*/ static inline int pmd_same(pmd_t pmd_a, pmd_t pmd_b)
// /*AFLA*/ {
// /*AFLA*/ 	BUILD_BUG();
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_TRANSPARENT_HUGEPAGE */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PGD_OFFSET_GATE
// /*AFLA*/ #define pgd_offset_gate(mm, addr)	pgd_offset(mm, addr)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_MOVE_PTE
// /*AFLA*/ #define move_pte(pte, prot, old_addr, new_addr)	(pte)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef pte_accessible
// /*AFLA*/ # define pte_accessible(mm, pte)	((void)(pte), 1)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef flush_tlb_fix_spurious_fault
// /*AFLA*/ #define flush_tlb_fix_spurious_fault(vma, address) flush_tlb_page(vma, address)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef pgprot_noncached
// /*AFLA*/ #define pgprot_noncached(prot)	(prot)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef pgprot_writecombine
// /*AFLA*/ #define pgprot_writecombine pgprot_noncached
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef pgprot_writethrough
// /*AFLA*/ #define pgprot_writethrough pgprot_noncached
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef pgprot_device
// /*AFLA*/ #define pgprot_device pgprot_noncached
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef pgprot_modify
// /*AFLA*/ #define pgprot_modify pgprot_modify
// /*AFLA*/ static inline pgprot_t pgprot_modify(pgprot_t oldprot, pgprot_t newprot)
// /*AFLA*/ {
// /*AFLA*/ 	if (pgprot_val(oldprot) == pgprot_val(pgprot_noncached(oldprot)))
// /*AFLA*/ 		newprot = pgprot_noncached(newprot);
// /*AFLA*/ 	if (pgprot_val(oldprot) == pgprot_val(pgprot_writecombine(oldprot)))
// /*AFLA*/ 		newprot = pgprot_writecombine(newprot);
// /*AFLA*/ 	if (pgprot_val(oldprot) == pgprot_val(pgprot_device(oldprot)))
// /*AFLA*/ 		newprot = pgprot_device(newprot);
// /*AFLA*/ 	return newprot;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * When walking page tables, get the address of the next boundary,
// /*AFLA*/  * or the end address of the range if that comes earlier.  Although no
// /*AFLA*/  * vma end wraps to 0, rounded up __boundary may wrap to 0 throughout.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define pgd_addr_end(addr, end)						\
// /*AFLA*/ ({	unsigned long __boundary = ((addr) + PGDIR_SIZE) & PGDIR_MASK;	\
// /*AFLA*/ 	(__boundary - 1 < (end) - 1)? __boundary: (end);		\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #ifndef pud_addr_end
// /*AFLA*/ #define pud_addr_end(addr, end)						\
// /*AFLA*/ ({	unsigned long __boundary = ((addr) + PUD_SIZE) & PUD_MASK;	\
// /*AFLA*/ 	(__boundary - 1 < (end) - 1)? __boundary: (end);		\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef pmd_addr_end
// /*AFLA*/ #define pmd_addr_end(addr, end)						\
// /*AFLA*/ ({	unsigned long __boundary = ((addr) + PMD_SIZE) & PMD_MASK;	\
// /*AFLA*/ 	(__boundary - 1 < (end) - 1)? __boundary: (end);		\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * When walking page tables, we usually want to skip any p?d_none entries;
// /*AFLA*/  * and any p?d_bad entries - reporting the error before resetting to none.
// /*AFLA*/  * Do the tests inline, but report and clear the bad entry in mm/memory.c.
// /*AFLA*/  */
// /*AFLA*/ void pgd_clear_bad(pgd_t *);
// /*AFLA*/ void pud_clear_bad(pud_t *);
// /*AFLA*/ void pmd_clear_bad(pmd_t *);
// /*AFLA*/ 
// /*AFLA*/ static inline int pgd_none_or_clear_bad(pgd_t *pgd)
// /*AFLA*/ {
// /*AFLA*/ 	if (pgd_none(*pgd))
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	if (unlikely(pgd_bad(*pgd))) {
// /*AFLA*/ 		pgd_clear_bad(pgd);
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	}
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pud_none_or_clear_bad(pud_t *pud)
// /*AFLA*/ {
// /*AFLA*/ 	if (pud_none(*pud))
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	if (unlikely(pud_bad(*pud))) {
// /*AFLA*/ 		pud_clear_bad(pud);
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	}
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pmd_none_or_clear_bad(pmd_t *pmd)
// /*AFLA*/ {
// /*AFLA*/ 	if (pmd_none(*pmd))
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	if (unlikely(pmd_bad(*pmd))) {
// /*AFLA*/ 		pmd_clear_bad(pmd);
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	}
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t __ptep_modify_prot_start(struct mm_struct *mm,
// /*AFLA*/ 					     unsigned long addr,
// /*AFLA*/ 					     pte_t *ptep)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Get the current pte state, but zero it out to make it
// /*AFLA*/ 	 * non-present, preventing the hardware from asynchronously
// /*AFLA*/ 	 * updating it.
// /*AFLA*/ 	 */
// /*AFLA*/ 	return ptep_get_and_clear(mm, addr, ptep);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __ptep_modify_prot_commit(struct mm_struct *mm,
// /*AFLA*/ 					     unsigned long addr,
// /*AFLA*/ 					     pte_t *ptep, pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The pte is non-present, so there's no hardware state to
// /*AFLA*/ 	 * preserve.
// /*AFLA*/ 	 */
// /*AFLA*/ 	set_pte_at(mm, addr, ptep, pte);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PTEP_MODIFY_PROT_TRANSACTION
// /*AFLA*/ /*
// /*AFLA*/  * Start a pte protection read-modify-write transaction, which
// /*AFLA*/  * protects against asynchronous hardware modifications to the pte.
// /*AFLA*/  * The intention is not to prevent the hardware from making pte
// /*AFLA*/  * updates, but to prevent any updates it may make from being lost.
// /*AFLA*/  *
// /*AFLA*/  * This does not protect against other software modifications of the
// /*AFLA*/  * pte; the appropriate pte lock must be held over the transation.
// /*AFLA*/  *
// /*AFLA*/  * Note that this interface is intended to be batchable, meaning that
// /*AFLA*/  * ptep_modify_prot_commit may not actually update the pte, but merely
// /*AFLA*/  * queue the update to be done at some later time.  The update must be
// /*AFLA*/  * actually committed before the pte lock is released, however.
// /*AFLA*/  */
// /*AFLA*/ static inline pte_t ptep_modify_prot_start(struct mm_struct *mm,
// /*AFLA*/ 					   unsigned long addr,
// /*AFLA*/ 					   pte_t *ptep)
// /*AFLA*/ {
// /*AFLA*/ 	return __ptep_modify_prot_start(mm, addr, ptep);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Commit an update to a pte, leaving any hardware-controlled bits in
// /*AFLA*/  * the PTE unmodified.
// /*AFLA*/  */
// /*AFLA*/ static inline void ptep_modify_prot_commit(struct mm_struct *mm,
// /*AFLA*/ 					   unsigned long addr,
// /*AFLA*/ 					   pte_t *ptep, pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	__ptep_modify_prot_commit(mm, addr, ptep, pte);
// /*AFLA*/ }
// /*AFLA*/ #endif /* __HAVE_ARCH_PTEP_MODIFY_PROT_TRANSACTION */
// /*AFLA*/ #endif /* CONFIG_MMU */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A facility to provide lazy MMU batching.  This allows PTE updates and
// /*AFLA*/  * page invalidations to be delayed until a call to leave lazy MMU mode
// /*AFLA*/  * is issued.  Some architectures may benefit from doing this, and it is
// /*AFLA*/  * beneficial for both shadow and direct mode hypervisors, which may batch
// /*AFLA*/  * the PTE updates which happen during this window.  Note that using this
// /*AFLA*/  * interface requires that read hazards be removed from the code.  A read
// /*AFLA*/  * hazard could result in the direct mode hypervisor case, since the actual
// /*AFLA*/  * write to the page tables may not yet have taken place, so reads though
// /*AFLA*/  * a raw PTE pointer after it has been modified are not guaranteed to be
// /*AFLA*/  * up to date.  This mode can only be entered and left under the protection of
// /*AFLA*/  * the page table locks for all page tables which may be modified.  In the UP
// /*AFLA*/  * case, this is required so that preemption is disabled, and in the SMP case,
// /*AFLA*/  * it must synchronize the delayed page table writes properly on other CPUs.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __HAVE_ARCH_ENTER_LAZY_MMU_MODE
// /*AFLA*/ #define arch_enter_lazy_mmu_mode()	do {} while (0)
// /*AFLA*/ #define arch_leave_lazy_mmu_mode()	do {} while (0)
// /*AFLA*/ #define arch_flush_lazy_mmu_mode()	do {} while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A facility to provide batching of the reload of page tables and
// /*AFLA*/  * other process state with the actual context switch code for
// /*AFLA*/  * paravirtualized guests.  By convention, only one of the batched
// /*AFLA*/  * update (lazy) modes (CPU, MMU) should be active at any given time,
// /*AFLA*/  * entry should never be nested, and entry and exits should always be
// /*AFLA*/  * paired.  This is for sanity of maintaining and reasoning about the
// /*AFLA*/  * kernel code.  In this case, the exit (end of the context switch) is
// /*AFLA*/  * in architecture-specific code, and so doesn't need a generic
// /*AFLA*/  * definition.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __HAVE_ARCH_START_CONTEXT_SWITCH
// /*AFLA*/ #define arch_start_context_switch(prev)	do {} while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_HAVE_ARCH_SOFT_DIRTY
// /*AFLA*/ static inline int pte_soft_dirty(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pmd_soft_dirty(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_mksoft_dirty(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pmd_mksoft_dirty(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_clear_soft_dirty(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t pmd_clear_soft_dirty(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_swp_mksoft_dirty(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pte_swp_soft_dirty(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t pte_swp_clear_soft_dirty(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_PFNMAP_TRACKING
// /*AFLA*/ /*
// /*AFLA*/  * Interfaces that can be used by architecture code to keep track of
// /*AFLA*/  * memory type of pfn mappings specified by the remap_pfn_range,
// /*AFLA*/  * vm_insert_pfn.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * track_pfn_remap is called when a _new_ pfn mapping is being established
// /*AFLA*/  * by remap_pfn_range() for physical range indicated by pfn and size.
// /*AFLA*/  */
// /*AFLA*/ static inline int track_pfn_remap(struct vm_area_struct *vma, pgprot_t *prot,
// /*AFLA*/ 				  unsigned long pfn, unsigned long addr,
// /*AFLA*/ 				  unsigned long size)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * track_pfn_insert is called when a _new_ single pfn is established
// /*AFLA*/  * by vm_insert_pfn().
// /*AFLA*/  */
// /*AFLA*/ static inline int track_pfn_insert(struct vm_area_struct *vma, pgprot_t *prot,
// /*AFLA*/ 				   pfn_t pfn)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * track_pfn_copy is called when vma that is covering the pfnmap gets
// /*AFLA*/  * copied through copy_page_range().
// /*AFLA*/  */
// /*AFLA*/ static inline int track_pfn_copy(struct vm_area_struct *vma)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * untrack_pfn is called while unmapping a pfnmap for a region.
// /*AFLA*/  * untrack can be called for a specific region indicated by pfn and size or
// /*AFLA*/  * can be for the entire vma (in which case pfn, size are zero).
// /*AFLA*/  */
// /*AFLA*/ static inline void untrack_pfn(struct vm_area_struct *vma,
// /*AFLA*/ 			       unsigned long pfn, unsigned long size)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * untrack_pfn_moved is called while mremapping a pfnmap for a new region.
// /*AFLA*/  */
// /*AFLA*/ static inline void untrack_pfn_moved(struct vm_area_struct *vma)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ extern int track_pfn_remap(struct vm_area_struct *vma, pgprot_t *prot,
// /*AFLA*/ 			   unsigned long pfn, unsigned long addr,
// /*AFLA*/ 			   unsigned long size);
// /*AFLA*/ extern int track_pfn_insert(struct vm_area_struct *vma, pgprot_t *prot,
// /*AFLA*/ 			    pfn_t pfn);
// /*AFLA*/ extern int track_pfn_copy(struct vm_area_struct *vma);
// /*AFLA*/ extern void untrack_pfn(struct vm_area_struct *vma, unsigned long pfn,
// /*AFLA*/ 			unsigned long size);
// /*AFLA*/ extern void untrack_pfn_moved(struct vm_area_struct *vma);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef __HAVE_COLOR_ZERO_PAGE
// /*AFLA*/ static inline int is_zero_pfn(unsigned long pfn)
// /*AFLA*/ {
// /*AFLA*/ 	extern unsigned long zero_pfn;
// /*AFLA*/ 	unsigned long offset_from_zero_pfn = pfn - zero_pfn;
// /*AFLA*/ 	return offset_from_zero_pfn <= (zero_page_mask >> PAGE_SHIFT);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define my_zero_pfn(addr)	page_to_pfn(ZERO_PAGE(addr))
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ static inline int is_zero_pfn(unsigned long pfn)
// /*AFLA*/ {
// /*AFLA*/ 	extern unsigned long zero_pfn;
// /*AFLA*/ 	return pfn == zero_pfn;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long my_zero_pfn(unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	extern unsigned long zero_pfn;
// /*AFLA*/ 	return zero_pfn;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ static inline int pmd_trans_huge(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #ifndef __HAVE_ARCH_PMD_WRITE
// /*AFLA*/ static inline int pmd_write(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	BUG();
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif /* __HAVE_ARCH_PMD_WRITE */
// /*AFLA*/ #endif /* CONFIG_TRANSPARENT_HUGEPAGE */
// /*AFLA*/ 
// /*AFLA*/ #ifndef pmd_read_atomic
// /*AFLA*/ static inline pmd_t pmd_read_atomic(pmd_t *pmdp)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Depend on compiler for an atomic pmd read. NOTE: this is
// /*AFLA*/ 	 * only going to work, if the pmdval_t isn't larger than
// /*AFLA*/ 	 * an unsigned long.
// /*AFLA*/ 	 */
// /*AFLA*/ 	return *pmdp;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef pmd_move_must_withdraw
// /*AFLA*/ static inline int pmd_move_must_withdraw(spinlock_t *new_pmd_ptl,
// /*AFLA*/ 					 spinlock_t *old_pmd_ptl)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * With split pmd lock we also need to move preallocated
// /*AFLA*/ 	 * PTE page table if new_pmd is on different PMD page table.
// /*AFLA*/ 	 */
// /*AFLA*/ 	return new_pmd_ptl != old_pmd_ptl;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This function is meant to be used by sites walking pagetables with
// /*AFLA*/  * the mmap_sem hold in read mode to protect against MADV_DONTNEED and
// /*AFLA*/  * transhuge page faults. MADV_DONTNEED can convert a transhuge pmd
// /*AFLA*/  * into a null pmd and the transhuge page fault can convert a null pmd
// /*AFLA*/  * into an hugepmd or into a regular pmd (if the hugepage allocation
// /*AFLA*/  * fails). While holding the mmap_sem in read mode the pmd becomes
// /*AFLA*/  * stable and stops changing under us only if it's not null and not a
// /*AFLA*/  * transhuge pmd. When those races occurs and this function makes a
// /*AFLA*/  * difference vs the standard pmd_none_or_clear_bad, the result is
// /*AFLA*/  * undefined so behaving like if the pmd was none is safe (because it
// /*AFLA*/  * can return none anyway). The compiler level barrier() is critically
// /*AFLA*/  * important to compute the two checks atomically on the same pmdval.
// /*AFLA*/  *
// /*AFLA*/  * For 32bit kernels with a 64bit large pmd_t this automatically takes
// /*AFLA*/  * care of reading the pmd atomically to avoid SMP race conditions
// /*AFLA*/  * against pmd_populate() when the mmap_sem is hold for reading by the
// /*AFLA*/  * caller (a special atomic read not done by "gcc" as in the generic
// /*AFLA*/  * version above, is also needed when THP is disabled because the page
// /*AFLA*/  * fault can populate the pmd from under us).
// /*AFLA*/  */
// /*AFLA*/ static inline int pmd_none_or_trans_huge_or_clear_bad(pmd_t *pmd)
// /*AFLA*/ {
// /*AFLA*/ 	pmd_t pmdval = pmd_read_atomic(pmd);
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The barrier will stabilize the pmdval in a register or on
// /*AFLA*/ 	 * the stack so that it will stop changing under the code.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * When CONFIG_TRANSPARENT_HUGEPAGE=y on x86 32bit PAE,
// /*AFLA*/ 	 * pmd_read_atomic is allowed to return a not atomic pmdval
// /*AFLA*/ 	 * (for example pointing to an hugepage that has never been
// /*AFLA*/ 	 * mapped in the pmd). The below checks will only care about
// /*AFLA*/ 	 * the low part of the pmd with 32bit PAE x86 anyway, with the
// /*AFLA*/ 	 * exception of pmd_none(). So the important thing is that if
// /*AFLA*/ 	 * the low part of the pmd is found null, the high part will
// /*AFLA*/ 	 * be also null or the pmd_none() check below would be
// /*AFLA*/ 	 * confused.
// /*AFLA*/ 	 */
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ 	barrier();
// /*AFLA*/ #endif
// /*AFLA*/ 	if (pmd_none(pmdval) || pmd_trans_huge(pmdval))
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	if (unlikely(pmd_bad(pmdval))) {
// /*AFLA*/ 		pmd_clear_bad(pmd);
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	}
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is a noop if Transparent Hugepage Support is not built into
// /*AFLA*/  * the kernel. Otherwise it is equivalent to
// /*AFLA*/  * pmd_none_or_trans_huge_or_clear_bad(), and shall only be called in
// /*AFLA*/  * places that already verified the pmd is not none and they want to
// /*AFLA*/  * walk ptes while holding the mmap sem in read mode (write mode don't
// /*AFLA*/  * need this). If THP is not enabled, the pmd can't go away under the
// /*AFLA*/  * code even if MADV_DONTNEED runs, but if THP is enabled we need to
// /*AFLA*/  * run a pmd_trans_unstable before walking the ptes after
// /*AFLA*/  * split_huge_page_pmd returns (because it may have run when the pmd
// /*AFLA*/  * become null, but then a page fault can map in a THP and not a
// /*AFLA*/  * regular page).
// /*AFLA*/  */
// /*AFLA*/ static inline int pmd_trans_unstable(pmd_t *pmd)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ 	return pmd_none_or_trans_huge_or_clear_bad(pmd);
// /*AFLA*/ #else
// /*AFLA*/ 	return 0;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_NUMA_BALANCING
// /*AFLA*/ /*
// /*AFLA*/  * Technically a PTE can be PROTNONE even when not doing NUMA balancing but
// /*AFLA*/  * the only case the kernel cares is for NUMA balancing and is only ever set
// /*AFLA*/  * when the VMA is accessible. For PROT_NONE VMAs, the PTEs are not marked
// /*AFLA*/  * _PAGE_PROTNONE so by by default, implement the helper as "always no". It
// /*AFLA*/  * is the responsibility of the caller to distinguish between PROT_NONE
// /*AFLA*/  * protections and NUMA hinting fault protections.
// /*AFLA*/  */
// /*AFLA*/ static inline int pte_protnone(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pmd_protnone(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_NUMA_BALANCING */
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_MMU */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_ARCH_HUGE_VMAP
// /*AFLA*/ int pud_set_huge(pud_t *pud, phys_addr_t addr, pgprot_t prot);
// /*AFLA*/ int pmd_set_huge(pmd_t *pmd, phys_addr_t addr, pgprot_t prot);
// /*AFLA*/ int pud_clear_huge(pud_t *pud);
// /*AFLA*/ int pmd_clear_huge(pmd_t *pmd);
// /*AFLA*/ #else	/* !CONFIG_HAVE_ARCH_HUGE_VMAP */
// /*AFLA*/ static inline int pud_set_huge(pud_t *pud, phys_addr_t addr, pgprot_t prot)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int pmd_set_huge(pmd_t *pmd, phys_addr_t addr, pgprot_t prot)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int pud_clear_huge(pud_t *pud)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int pmd_clear_huge(pmd_t *pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif	/* CONFIG_HAVE_ARCH_HUGE_VMAP */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_FLUSH_PMD_TLB_RANGE
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ /*
// /*AFLA*/  * ARCHes with special requirements for evicting THP backing TLB entries can
// /*AFLA*/  * implement this. Otherwise also, it can help optimize normal TLB flush in
// /*AFLA*/  * THP regime. stock flush_tlb_range() typically has optimization to nuke the
// /*AFLA*/  * entire TLB TLB if flush span is greater than a threshold, which will
// /*AFLA*/  * likely be true for a single huge page. Thus a single thp flush will
// /*AFLA*/  * invalidate the entire TLB which is not desitable.
// /*AFLA*/  * e.g. see arch/arc: flush_pmd_tlb_range
// /*AFLA*/  */
// /*AFLA*/ #define flush_pmd_tlb_range(vma, addr, end)	flush_tlb_range(vma, addr, end)
// /*AFLA*/ #else
// /*AFLA*/ #define flush_pmd_tlb_range(vma, addr, end)	BUILD_BUG()
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct file;
// /*AFLA*/ int phys_mem_access_prot_allowed(struct file *file, unsigned long pfn,
// /*AFLA*/ 			unsigned long size, pgprot_t *vma_prot);
// /*AFLA*/ #endif /* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #ifndef io_remap_pfn_range
// /*AFLA*/ #define io_remap_pfn_range remap_pfn_range
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef has_transparent_hugepage
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ #define has_transparent_hugepage() 1
// /*AFLA*/ #else
// /*AFLA*/ #define has_transparent_hugepage() 0
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_GENERIC_PGTABLE_H */
