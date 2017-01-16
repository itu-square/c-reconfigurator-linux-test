// /*AFLA*/ #ifndef _LINUX_HUGE_MM_H
// /*AFLA*/ #define _LINUX_HUGE_MM_H
// /*AFLA*/ 
// /*AFLA*/ extern int do_huge_pmd_anonymous_page(struct fault_env *fe);
// /*AFLA*/ extern int copy_huge_pmd(struct mm_struct *dst_mm, struct mm_struct *src_mm,
// /*AFLA*/ 			 pmd_t *dst_pmd, pmd_t *src_pmd, unsigned long addr,
// /*AFLA*/ 			 struct vm_area_struct *vma);
// /*AFLA*/ extern void huge_pmd_set_accessed(struct fault_env *fe, pmd_t orig_pmd);
// /*AFLA*/ extern int do_huge_pmd_wp_page(struct fault_env *fe, pmd_t orig_pmd);
// /*AFLA*/ extern struct page *follow_trans_huge_pmd(struct vm_area_struct *vma,
// /*AFLA*/ 					  unsigned long addr,
// /*AFLA*/ 					  pmd_t *pmd,
// /*AFLA*/ 					  unsigned int flags);
// /*AFLA*/ extern bool madvise_free_huge_pmd(struct mmu_gather *tlb,
// /*AFLA*/ 			struct vm_area_struct *vma,
// /*AFLA*/ 			pmd_t *pmd, unsigned long addr, unsigned long next);
// /*AFLA*/ extern int zap_huge_pmd(struct mmu_gather *tlb,
// /*AFLA*/ 			struct vm_area_struct *vma,
// /*AFLA*/ 			pmd_t *pmd, unsigned long addr);
// /*AFLA*/ extern int mincore_huge_pmd(struct vm_area_struct *vma, pmd_t *pmd,
// /*AFLA*/ 			unsigned long addr, unsigned long end,
// /*AFLA*/ 			unsigned char *vec);
// /*AFLA*/ extern bool move_huge_pmd(struct vm_area_struct *vma, unsigned long old_addr,
// /*AFLA*/ 			 unsigned long new_addr, unsigned long old_end,
// /*AFLA*/ 			 pmd_t *old_pmd, pmd_t *new_pmd, bool *need_flush);
// /*AFLA*/ extern int change_huge_pmd(struct vm_area_struct *vma, pmd_t *pmd,
// /*AFLA*/ 			unsigned long addr, pgprot_t newprot,
// /*AFLA*/ 			int prot_numa);
// /*AFLA*/ int vmf_insert_pfn_pmd(struct vm_area_struct *, unsigned long addr, pmd_t *,
// /*AFLA*/ 			pfn_t pfn, bool write);
// /*AFLA*/ enum transparent_hugepage_flag {
// /*AFLA*/ 	TRANSPARENT_HUGEPAGE_FLAG,
// /*AFLA*/ 	TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG,
// /*AFLA*/ 	TRANSPARENT_HUGEPAGE_DEFRAG_DIRECT_FLAG,
// /*AFLA*/ 	TRANSPARENT_HUGEPAGE_DEFRAG_KSWAPD_FLAG,
// /*AFLA*/ 	TRANSPARENT_HUGEPAGE_DEFRAG_REQ_MADV_FLAG,
// /*AFLA*/ 	TRANSPARENT_HUGEPAGE_DEFRAG_KHUGEPAGED_FLAG,
// /*AFLA*/ 	TRANSPARENT_HUGEPAGE_USE_ZERO_PAGE_FLAG,
// /*AFLA*/ #ifdef CONFIG_DEBUG_VM
// /*AFLA*/ 	TRANSPARENT_HUGEPAGE_DEBUG_COW_FLAG,
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct kobject;
// /*AFLA*/ struct kobj_attribute;
// /*AFLA*/ 
// /*AFLA*/ extern ssize_t single_hugepage_flag_store(struct kobject *kobj,
// /*AFLA*/ 				 struct kobj_attribute *attr,
// /*AFLA*/ 				 const char *buf, size_t count,
// /*AFLA*/ 				 enum transparent_hugepage_flag flag);
// /*AFLA*/ extern ssize_t single_hugepage_flag_show(struct kobject *kobj,
// /*AFLA*/ 				struct kobj_attribute *attr, char *buf,
// /*AFLA*/ 				enum transparent_hugepage_flag flag);
// /*AFLA*/ extern struct kobj_attribute shmem_enabled_attr;
// /*AFLA*/ 
// /*AFLA*/ #define HPAGE_PMD_ORDER (HPAGE_PMD_SHIFT-PAGE_SHIFT)
// /*AFLA*/ #define HPAGE_PMD_NR (1<<HPAGE_PMD_ORDER)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ struct page *follow_devmap_pmd(struct vm_area_struct *vma, unsigned long addr,
// /*AFLA*/ 		pmd_t *pmd, int flags);
// /*AFLA*/ 
// /*AFLA*/ #define HPAGE_PMD_SHIFT PMD_SHIFT
// /*AFLA*/ #define HPAGE_PMD_SIZE	((1UL) << HPAGE_PMD_SHIFT)
// /*AFLA*/ #define HPAGE_PMD_MASK	(~(HPAGE_PMD_SIZE - 1))
// /*AFLA*/ 
// /*AFLA*/ extern bool is_vma_temporary_stack(struct vm_area_struct *vma);
// /*AFLA*/ 
// /*AFLA*/ #define transparent_hugepage_enabled(__vma)				\
// /*AFLA*/ 	((transparent_hugepage_flags &					\
// /*AFLA*/ 	  (1<<TRANSPARENT_HUGEPAGE_FLAG) ||				\
// /*AFLA*/ 	  (transparent_hugepage_flags &					\
// /*AFLA*/ 	   (1<<TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG) &&			\
// /*AFLA*/ 	   ((__vma)->vm_flags & VM_HUGEPAGE))) &&			\
// /*AFLA*/ 	 !((__vma)->vm_flags & VM_NOHUGEPAGE) &&			\
// /*AFLA*/ 	 !is_vma_temporary_stack(__vma))
// /*AFLA*/ #define transparent_hugepage_use_zero_page()				\
// /*AFLA*/ 	(transparent_hugepage_flags &					\
// /*AFLA*/ 	 (1<<TRANSPARENT_HUGEPAGE_USE_ZERO_PAGE_FLAG))
// /*AFLA*/ #ifdef CONFIG_DEBUG_VM
// /*AFLA*/ #define transparent_hugepage_debug_cow()				\
// /*AFLA*/ 	(transparent_hugepage_flags &					\
// /*AFLA*/ 	 (1<<TRANSPARENT_HUGEPAGE_DEBUG_COW_FLAG))
// /*AFLA*/ #else /* CONFIG_DEBUG_VM */
// /*AFLA*/ #define transparent_hugepage_debug_cow() 0
// /*AFLA*/ #endif /* CONFIG_DEBUG_VM */
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long transparent_hugepage_flags;
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long thp_get_unmapped_area(struct file *filp,
// /*AFLA*/ 		unsigned long addr, unsigned long len, unsigned long pgoff,
// /*AFLA*/ 		unsigned long flags);
// /*AFLA*/ 
// /*AFLA*/ extern void prep_transhuge_page(struct page *page);
// /*AFLA*/ extern void free_transhuge_page(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ int split_huge_page_to_list(struct page *page, struct list_head *list);
// /*AFLA*/ static inline int split_huge_page(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return split_huge_page_to_list(page, NULL);
// /*AFLA*/ }
// /*AFLA*/ void deferred_split_huge_page(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ void __split_huge_pmd(struct vm_area_struct *vma, pmd_t *pmd,
// /*AFLA*/ 		unsigned long address, bool freeze, struct page *page);
// /*AFLA*/ 
// /*AFLA*/ #define split_huge_pmd(__vma, __pmd, __address)				\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		pmd_t *____pmd = (__pmd);				\
// /*AFLA*/ 		if (pmd_trans_huge(*____pmd)				\
// /*AFLA*/ 					|| pmd_devmap(*____pmd))	\
// /*AFLA*/ 			__split_huge_pmd(__vma, __pmd, __address,	\
// /*AFLA*/ 						false, NULL);		\
// /*AFLA*/ 	}  while (0)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ void split_huge_pmd_address(struct vm_area_struct *vma, unsigned long address,
// /*AFLA*/ 		bool freeze, struct page *page);
// /*AFLA*/ 
// /*AFLA*/ extern int hugepage_madvise(struct vm_area_struct *vma,
// /*AFLA*/ 			    unsigned long *vm_flags, int advice);
// /*AFLA*/ extern void vma_adjust_trans_huge(struct vm_area_struct *vma,
// /*AFLA*/ 				    unsigned long start,
// /*AFLA*/ 				    unsigned long end,
// /*AFLA*/ 				    long adjust_next);
// /*AFLA*/ extern spinlock_t *__pmd_trans_huge_lock(pmd_t *pmd,
// /*AFLA*/ 		struct vm_area_struct *vma);
// /*AFLA*/ /* mmap_sem must be held on entry */
// /*AFLA*/ static inline spinlock_t *pmd_trans_huge_lock(pmd_t *pmd,
// /*AFLA*/ 		struct vm_area_struct *vma)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_VMA(!rwsem_is_locked(&vma->vm_mm->mmap_sem), vma);
// /*AFLA*/ 	if (pmd_trans_huge(*pmd) || pmd_devmap(*pmd))
// /*AFLA*/ 		return __pmd_trans_huge_lock(pmd, vma);
// /*AFLA*/ 	else
// /*AFLA*/ 		return NULL;
// /*AFLA*/ }
// /*AFLA*/ static inline int hpage_nr_pages(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(PageTransHuge(page)))
// /*AFLA*/ 		return HPAGE_PMD_NR;
// /*AFLA*/ 	return 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int do_huge_pmd_numa_page(struct fault_env *fe, pmd_t orig_pmd);
// /*AFLA*/ 
// /*AFLA*/ extern struct page *huge_zero_page;
// /*AFLA*/ 
// /*AFLA*/ static inline bool is_huge_zero_page(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return ACCESS_ONCE(huge_zero_page) == page;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool is_huge_zero_pmd(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return is_huge_zero_page(pmd_page(pmd));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct page *mm_get_huge_zero_page(struct mm_struct *mm);
// /*AFLA*/ void mm_put_huge_zero_page(struct mm_struct *mm);
// /*AFLA*/ 
// /*AFLA*/ #define mk_huge_pmd(page, prot) pmd_mkhuge(mk_pmd(page, prot))
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_TRANSPARENT_HUGEPAGE */
// /*AFLA*/ #define HPAGE_PMD_SHIFT ({ BUILD_BUG(); 0; })
// /*AFLA*/ #define HPAGE_PMD_MASK ({ BUILD_BUG(); 0; })
// /*AFLA*/ #define HPAGE_PMD_SIZE ({ BUILD_BUG(); 0; })
// /*AFLA*/ 
// /*AFLA*/ #define hpage_nr_pages(x) 1
// /*AFLA*/ 
// /*AFLA*/ #define transparent_hugepage_enabled(__vma) 0
// /*AFLA*/ 
// /*AFLA*/ static inline void prep_transhuge_page(struct page *page) {}
// /*AFLA*/ 
// /*AFLA*/ #define transparent_hugepage_flags 0UL
// /*AFLA*/ 
// /*AFLA*/ #define thp_get_unmapped_area	NULL
// /*AFLA*/ 
// /*AFLA*/ static inline int
// /*AFLA*/ split_huge_page_to_list(struct page *page, struct list_head *list)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int split_huge_page(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void deferred_split_huge_page(struct page *page) {}
// /*AFLA*/ #define split_huge_pmd(__vma, __pmd, __address)	\
// /*AFLA*/ 	do { } while (0)
// /*AFLA*/ 
// /*AFLA*/ static inline void split_huge_pmd_address(struct vm_area_struct *vma,
// /*AFLA*/ 		unsigned long address, bool freeze, struct page *page) {}
// /*AFLA*/ 
// /*AFLA*/ static inline int hugepage_madvise(struct vm_area_struct *vma,
// /*AFLA*/ 				   unsigned long *vm_flags, int advice)
// /*AFLA*/ {
// /*AFLA*/ 	BUG();
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void vma_adjust_trans_huge(struct vm_area_struct *vma,
// /*AFLA*/ 					 unsigned long start,
// /*AFLA*/ 					 unsigned long end,
// /*AFLA*/ 					 long adjust_next)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline spinlock_t *pmd_trans_huge_lock(pmd_t *pmd,
// /*AFLA*/ 		struct vm_area_struct *vma)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int do_huge_pmd_numa_page(struct fault_env *fe, pmd_t orig_pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool is_huge_zero_page(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void mm_put_huge_zero_page(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct page *follow_devmap_pmd(struct vm_area_struct *vma,
// /*AFLA*/ 		unsigned long addr, pmd_t *pmd, int flags)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_TRANSPARENT_HUGEPAGE */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_HUGE_MM_H */
