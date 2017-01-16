// /*AFLA*/ #ifndef _ASM_X86_PAGE_H
// /*AFLA*/ #define _ASM_X86_PAGE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef __KERNEL__
// /*AFLA*/ 
// /*AFLA*/ #include <asm/page_types.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ #include <asm/page_64.h>
// /*AFLA*/ #else
// /*AFLA*/ #include <asm/page_32.h>
// /*AFLA*/ #endif	/* CONFIG_X86_64 */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ struct page;
// /*AFLA*/ 
// /*AFLA*/ #include <linux/range.h>
// /*AFLA*/ extern struct range pfn_mapped[];
// /*AFLA*/ extern int nr_pfn_mapped;
// /*AFLA*/ 
// /*AFLA*/ static inline void clear_user_page(void *page, unsigned long vaddr,
// /*AFLA*/ 				   struct page *pg)
// /*AFLA*/ {
// /*AFLA*/ 	clear_page(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void copy_user_page(void *to, void *from, unsigned long vaddr,
// /*AFLA*/ 				  struct page *topage)
// /*AFLA*/ {
// /*AFLA*/ 	copy_page(to, from);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __alloc_zeroed_user_highpage(movableflags, vma, vaddr) \
// /*AFLA*/ 	alloc_page_vma(GFP_HIGHUSER | __GFP_ZERO | movableflags, vma, vaddr)
// /*AFLA*/ #define __HAVE_ARCH_ALLOC_ZEROED_USER_HIGHPAGE
// /*AFLA*/ 
// /*AFLA*/ #ifndef __pa
// /*AFLA*/ #define __pa(x)		__phys_addr((unsigned long)(x))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __pa_nodebug(x)	__phys_addr_nodebug((unsigned long)(x))
// /*AFLA*/ /* __pa_symbol should be used for C visible symbols.
// /*AFLA*/    This seems to be the official gcc blessed way to do such arithmetic. */
// /*AFLA*/ /*
// /*AFLA*/  * We need __phys_reloc_hide() here because gcc may assume that there is no
// /*AFLA*/  * overflow during __pa() calculation and can optimize it unexpectedly.
// /*AFLA*/  * Newer versions of gcc provide -fno-strict-overflow switch to handle this
// /*AFLA*/  * case properly. Once all supported versions of gcc understand it, we can
// /*AFLA*/  * remove this Voodoo magic stuff. (i.e. once gcc3.x is deprecated)
// /*AFLA*/  */
// /*AFLA*/ #define __pa_symbol(x) \
// /*AFLA*/ 	__phys_addr_symbol(__phys_reloc_hide((unsigned long)(x)))
// /*AFLA*/ 
// /*AFLA*/ #ifndef __va
// /*AFLA*/ #define __va(x)			((void *)((unsigned long)(x)+PAGE_OFFSET))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __boot_va(x)		__va(x)
// /*AFLA*/ #define __boot_pa(x)		__pa(x)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * virt_to_page(kaddr) returns a valid pointer if and only if
// /*AFLA*/  * virt_addr_valid(kaddr) returns true.
// /*AFLA*/  */
// /*AFLA*/ #define virt_to_page(kaddr)	pfn_to_page(__pa(kaddr) >> PAGE_SHIFT)
// /*AFLA*/ #define pfn_to_kaddr(pfn)      __va((pfn) << PAGE_SHIFT)
// /*AFLA*/ extern bool __virt_addr_valid(unsigned long kaddr);
// /*AFLA*/ #define virt_addr_valid(kaddr)	__virt_addr_valid((unsigned long) (kaddr))
// /*AFLA*/ 
// /*AFLA*/ #endif	/* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/memory_model.h>
// /*AFLA*/ #include <asm-generic/getorder.h>
// /*AFLA*/ 
// /*AFLA*/ #define HAVE_ARCH_HUGETLB_UNMAPPED_AREA
// /*AFLA*/ 
// /*AFLA*/ #endif	/* __KERNEL__ */
// /*AFLA*/ #endif /* _ASM_X86_PAGE_H */
