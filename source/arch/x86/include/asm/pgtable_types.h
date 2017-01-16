// /*AFLA*/ #ifndef _ASM_X86_PGTABLE_DEFS_H
// /*AFLA*/ #define _ASM_X86_PGTABLE_DEFS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/const.h>
// /*AFLA*/ #include <asm/page_types.h>
// /*AFLA*/ 
// /*AFLA*/ #define FIRST_USER_ADDRESS	0UL
// /*AFLA*/ 
// /*AFLA*/ #define _PAGE_BIT_PRESENT	0	/* is present */
// /*AFLA*/ #define _PAGE_BIT_RW		1	/* writeable */
// /*AFLA*/ #define _PAGE_BIT_USER		2	/* userspace addressable */
// /*AFLA*/ #define _PAGE_BIT_PWT		3	/* page write through */
// /*AFLA*/ #define _PAGE_BIT_PCD		4	/* page cache disabled */
// /*AFLA*/ #define _PAGE_BIT_ACCESSED	5	/* was accessed (raised by CPU) */
// /*AFLA*/ #define _PAGE_BIT_DIRTY		6	/* was written to (raised by CPU) */
// /*AFLA*/ #define _PAGE_BIT_PSE		7	/* 4 MB (or 2MB) page */
// /*AFLA*/ #define _PAGE_BIT_PAT		7	/* on 4KB pages */
// /*AFLA*/ #define _PAGE_BIT_GLOBAL	8	/* Global TLB entry PPro+ */
// /*AFLA*/ #define _PAGE_BIT_SOFTW1	9	/* available for programmer */
// /*AFLA*/ #define _PAGE_BIT_SOFTW2	10	/* " */
// /*AFLA*/ #define _PAGE_BIT_SOFTW3	11	/* " */
// /*AFLA*/ #define _PAGE_BIT_PAT_LARGE	12	/* On 2MB or 1GB pages */
// /*AFLA*/ #define _PAGE_BIT_SOFTW4	58	/* available for programmer */
// /*AFLA*/ #define _PAGE_BIT_PKEY_BIT0	59	/* Protection Keys, bit 1/4 */
// /*AFLA*/ #define _PAGE_BIT_PKEY_BIT1	60	/* Protection Keys, bit 2/4 */
// /*AFLA*/ #define _PAGE_BIT_PKEY_BIT2	61	/* Protection Keys, bit 3/4 */
// /*AFLA*/ #define _PAGE_BIT_PKEY_BIT3	62	/* Protection Keys, bit 4/4 */
// /*AFLA*/ #define _PAGE_BIT_NX		63	/* No execute: only valid after cpuid check */
// /*AFLA*/ 
// /*AFLA*/ #define _PAGE_BIT_SPECIAL	_PAGE_BIT_SOFTW1
// /*AFLA*/ #define _PAGE_BIT_CPA_TEST	_PAGE_BIT_SOFTW1
// /*AFLA*/ #define _PAGE_BIT_HIDDEN	_PAGE_BIT_SOFTW3 /* hidden by kmemcheck */
// /*AFLA*/ #define _PAGE_BIT_SOFT_DIRTY	_PAGE_BIT_SOFTW3 /* software dirty tracking */
// /*AFLA*/ #define _PAGE_BIT_DEVMAP	_PAGE_BIT_SOFTW4
// /*AFLA*/ 
// /*AFLA*/ /* If _PAGE_BIT_PRESENT is clear, we use these: */
// /*AFLA*/ /* - if the user mapped it with PROT_NONE; pte_present gives true */
// /*AFLA*/ #define _PAGE_BIT_PROTNONE	_PAGE_BIT_GLOBAL
// /*AFLA*/ 
// /*AFLA*/ #define _PAGE_PRESENT	(_AT(pteval_t, 1) << _PAGE_BIT_PRESENT)
// /*AFLA*/ #define _PAGE_RW	(_AT(pteval_t, 1) << _PAGE_BIT_RW)
// /*AFLA*/ #define _PAGE_USER	(_AT(pteval_t, 1) << _PAGE_BIT_USER)
// /*AFLA*/ #define _PAGE_PWT	(_AT(pteval_t, 1) << _PAGE_BIT_PWT)
// /*AFLA*/ #define _PAGE_PCD	(_AT(pteval_t, 1) << _PAGE_BIT_PCD)
// /*AFLA*/ #define _PAGE_ACCESSED	(_AT(pteval_t, 1) << _PAGE_BIT_ACCESSED)
// /*AFLA*/ #define _PAGE_DIRTY	(_AT(pteval_t, 1) << _PAGE_BIT_DIRTY)
// /*AFLA*/ #define _PAGE_PSE	(_AT(pteval_t, 1) << _PAGE_BIT_PSE)
// /*AFLA*/ #define _PAGE_GLOBAL	(_AT(pteval_t, 1) << _PAGE_BIT_GLOBAL)
// /*AFLA*/ #define _PAGE_SOFTW1	(_AT(pteval_t, 1) << _PAGE_BIT_SOFTW1)
// /*AFLA*/ #define _PAGE_SOFTW2	(_AT(pteval_t, 1) << _PAGE_BIT_SOFTW2)
// /*AFLA*/ #define _PAGE_PAT	(_AT(pteval_t, 1) << _PAGE_BIT_PAT)
// /*AFLA*/ #define _PAGE_PAT_LARGE (_AT(pteval_t, 1) << _PAGE_BIT_PAT_LARGE)
// /*AFLA*/ #define _PAGE_SPECIAL	(_AT(pteval_t, 1) << _PAGE_BIT_SPECIAL)
// /*AFLA*/ #define _PAGE_CPA_TEST	(_AT(pteval_t, 1) << _PAGE_BIT_CPA_TEST)
// /*AFLA*/ #ifdef CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS
// /*AFLA*/ #define _PAGE_PKEY_BIT0	(_AT(pteval_t, 1) << _PAGE_BIT_PKEY_BIT0)
// /*AFLA*/ #define _PAGE_PKEY_BIT1	(_AT(pteval_t, 1) << _PAGE_BIT_PKEY_BIT1)
// /*AFLA*/ #define _PAGE_PKEY_BIT2	(_AT(pteval_t, 1) << _PAGE_BIT_PKEY_BIT2)
// /*AFLA*/ #define _PAGE_PKEY_BIT3	(_AT(pteval_t, 1) << _PAGE_BIT_PKEY_BIT3)
// /*AFLA*/ #else
// /*AFLA*/ #define _PAGE_PKEY_BIT0	(_AT(pteval_t, 0))
// /*AFLA*/ #define _PAGE_PKEY_BIT1	(_AT(pteval_t, 0))
// /*AFLA*/ #define _PAGE_PKEY_BIT2	(_AT(pteval_t, 0))
// /*AFLA*/ #define _PAGE_PKEY_BIT3	(_AT(pteval_t, 0))
// /*AFLA*/ #endif
// /*AFLA*/ #define __HAVE_ARCH_PTE_SPECIAL
// /*AFLA*/ 
// /*AFLA*/ #define _PAGE_PKEY_MASK (_PAGE_PKEY_BIT0 | \
// /*AFLA*/ 			 _PAGE_PKEY_BIT1 | \
// /*AFLA*/ 			 _PAGE_PKEY_BIT2 | \
// /*AFLA*/ 			 _PAGE_PKEY_BIT3)
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_X86_64) || defined(CONFIG_X86_PAE)
// /*AFLA*/ #define _PAGE_KNL_ERRATUM_MASK (_PAGE_DIRTY | _PAGE_ACCESSED)
// /*AFLA*/ #else
// /*AFLA*/ #define _PAGE_KNL_ERRATUM_MASK 0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KMEMCHECK
// /*AFLA*/ #define _PAGE_HIDDEN	(_AT(pteval_t, 1) << _PAGE_BIT_HIDDEN)
// /*AFLA*/ #else
// /*AFLA*/ #define _PAGE_HIDDEN	(_AT(pteval_t, 0))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The same hidden bit is used by kmemcheck, but since kmemcheck
// /*AFLA*/  * works on kernel pages while soft-dirty engine on user space,
// /*AFLA*/  * they do not conflict with each other.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MEM_SOFT_DIRTY
// /*AFLA*/ #define _PAGE_SOFT_DIRTY	(_AT(pteval_t, 1) << _PAGE_BIT_SOFT_DIRTY)
// /*AFLA*/ #else
// /*AFLA*/ #define _PAGE_SOFT_DIRTY	(_AT(pteval_t, 0))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Tracking soft dirty bit when a page goes to a swap is tricky.
// /*AFLA*/  * We need a bit which can be stored in pte _and_ not conflict
// /*AFLA*/  * with swap entry format. On x86 bits 6 and 7 are *not* involved
// /*AFLA*/  * into swap entry computation, but bit 6 is used for nonlinear
// /*AFLA*/  * file mapping, so we borrow bit 7 for soft dirty tracking.
// /*AFLA*/  *
// /*AFLA*/  * Please note that this bit must be treated as swap dirty page
// /*AFLA*/  * mark if and only if the PTE has present bit clear!
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_MEM_SOFT_DIRTY
// /*AFLA*/ #define _PAGE_SWP_SOFT_DIRTY	_PAGE_PSE
// /*AFLA*/ #else
// /*AFLA*/ #define _PAGE_SWP_SOFT_DIRTY	(_AT(pteval_t, 0))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_X86_64) || defined(CONFIG_X86_PAE)
// /*AFLA*/ #define _PAGE_NX	(_AT(pteval_t, 1) << _PAGE_BIT_NX)
// /*AFLA*/ #define _PAGE_DEVMAP	(_AT(u64, 1) << _PAGE_BIT_DEVMAP)
// /*AFLA*/ #define __HAVE_ARCH_PTE_DEVMAP
// /*AFLA*/ #else
// /*AFLA*/ #define _PAGE_NX	(_AT(pteval_t, 0))
// /*AFLA*/ #define _PAGE_DEVMAP	(_AT(pteval_t, 0))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define _PAGE_PROTNONE	(_AT(pteval_t, 1) << _PAGE_BIT_PROTNONE)
// /*AFLA*/ 
// /*AFLA*/ #define _PAGE_TABLE	(_PAGE_PRESENT | _PAGE_RW | _PAGE_USER |	\
// /*AFLA*/ 			 _PAGE_ACCESSED | _PAGE_DIRTY)
// /*AFLA*/ #define _KERNPG_TABLE	(_PAGE_PRESENT | _PAGE_RW | _PAGE_ACCESSED |	\
// /*AFLA*/ 			 _PAGE_DIRTY)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Set of bits not changed in pte_modify.  The pte's
// /*AFLA*/  * protection key is treated like _PAGE_RW, for
// /*AFLA*/  * instance, and is *not* included in this mask since
// /*AFLA*/  * pte_modify() does modify it.
// /*AFLA*/  */
// /*AFLA*/ #define _PAGE_CHG_MASK	(PTE_PFN_MASK | _PAGE_PCD | _PAGE_PWT |		\
// /*AFLA*/ 			 _PAGE_SPECIAL | _PAGE_ACCESSED | _PAGE_DIRTY |	\
// /*AFLA*/ 			 _PAGE_SOFT_DIRTY)
// /*AFLA*/ #define _HPAGE_CHG_MASK (_PAGE_CHG_MASK | _PAGE_PSE)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The cache modes defined here are used to translate between pure SW usage
// /*AFLA*/  * and the HW defined cache mode bits and/or PAT entries.
// /*AFLA*/  *
// /*AFLA*/  * The resulting bits for PWT, PCD and PAT should be chosen in a way
// /*AFLA*/  * to have the WB mode at index 0 (all bits clear). This is the default
// /*AFLA*/  * right now and likely would break too much if changed.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ enum page_cache_mode {
// /*AFLA*/ 	_PAGE_CACHE_MODE_WB = 0,
// /*AFLA*/ 	_PAGE_CACHE_MODE_WC = 1,
// /*AFLA*/ 	_PAGE_CACHE_MODE_UC_MINUS = 2,
// /*AFLA*/ 	_PAGE_CACHE_MODE_UC = 3,
// /*AFLA*/ 	_PAGE_CACHE_MODE_WT = 4,
// /*AFLA*/ 	_PAGE_CACHE_MODE_WP = 5,
// /*AFLA*/ 	_PAGE_CACHE_MODE_NUM = 8
// /*AFLA*/ };
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define _PAGE_CACHE_MASK	(_PAGE_PAT | _PAGE_PCD | _PAGE_PWT)
// /*AFLA*/ #define _PAGE_NOCACHE		(cachemode2protval(_PAGE_CACHE_MODE_UC))
// /*AFLA*/ 
// /*AFLA*/ #define PAGE_NONE	__pgprot(_PAGE_PROTNONE | _PAGE_ACCESSED)
// /*AFLA*/ #define PAGE_SHARED	__pgprot(_PAGE_PRESENT | _PAGE_RW | _PAGE_USER | \
// /*AFLA*/ 				 _PAGE_ACCESSED | _PAGE_NX)
// /*AFLA*/ 
// /*AFLA*/ #define PAGE_SHARED_EXEC	__pgprot(_PAGE_PRESENT | _PAGE_RW |	\
// /*AFLA*/ 					 _PAGE_USER | _PAGE_ACCESSED)
// /*AFLA*/ #define PAGE_COPY_NOEXEC	__pgprot(_PAGE_PRESENT | _PAGE_USER |	\
// /*AFLA*/ 					 _PAGE_ACCESSED | _PAGE_NX)
// /*AFLA*/ #define PAGE_COPY_EXEC		__pgprot(_PAGE_PRESENT | _PAGE_USER |	\
// /*AFLA*/ 					 _PAGE_ACCESSED)
// /*AFLA*/ #define PAGE_COPY		PAGE_COPY_NOEXEC
// /*AFLA*/ #define PAGE_READONLY		__pgprot(_PAGE_PRESENT | _PAGE_USER |	\
// /*AFLA*/ 					 _PAGE_ACCESSED | _PAGE_NX)
// /*AFLA*/ #define PAGE_READONLY_EXEC	__pgprot(_PAGE_PRESENT | _PAGE_USER |	\
// /*AFLA*/ 					 _PAGE_ACCESSED)
// /*AFLA*/ 
// /*AFLA*/ #define __PAGE_KERNEL_EXEC						\
// /*AFLA*/ 	(_PAGE_PRESENT | _PAGE_RW | _PAGE_DIRTY | _PAGE_ACCESSED | _PAGE_GLOBAL)
// /*AFLA*/ #define __PAGE_KERNEL		(__PAGE_KERNEL_EXEC | _PAGE_NX)
// /*AFLA*/ 
// /*AFLA*/ #define __PAGE_KERNEL_RO		(__PAGE_KERNEL & ~_PAGE_RW)
// /*AFLA*/ #define __PAGE_KERNEL_RX		(__PAGE_KERNEL_EXEC & ~_PAGE_RW)
// /*AFLA*/ #define __PAGE_KERNEL_NOCACHE		(__PAGE_KERNEL | _PAGE_NOCACHE)
// /*AFLA*/ #define __PAGE_KERNEL_VSYSCALL		(__PAGE_KERNEL_RX | _PAGE_USER)
// /*AFLA*/ #define __PAGE_KERNEL_VVAR		(__PAGE_KERNEL_RO | _PAGE_USER)
// /*AFLA*/ #define __PAGE_KERNEL_LARGE		(__PAGE_KERNEL | _PAGE_PSE)
// /*AFLA*/ #define __PAGE_KERNEL_LARGE_EXEC	(__PAGE_KERNEL_EXEC | _PAGE_PSE)
// /*AFLA*/ 
// /*AFLA*/ #define __PAGE_KERNEL_IO		(__PAGE_KERNEL)
// /*AFLA*/ #define __PAGE_KERNEL_IO_NOCACHE	(__PAGE_KERNEL_NOCACHE)
// /*AFLA*/ 
// /*AFLA*/ #define PAGE_KERNEL			__pgprot(__PAGE_KERNEL)
// /*AFLA*/ #define PAGE_KERNEL_RO			__pgprot(__PAGE_KERNEL_RO)
// /*AFLA*/ #define PAGE_KERNEL_EXEC		__pgprot(__PAGE_KERNEL_EXEC)
// /*AFLA*/ #define PAGE_KERNEL_RX			__pgprot(__PAGE_KERNEL_RX)
// /*AFLA*/ #define PAGE_KERNEL_NOCACHE		__pgprot(__PAGE_KERNEL_NOCACHE)
// /*AFLA*/ #define PAGE_KERNEL_LARGE		__pgprot(__PAGE_KERNEL_LARGE)
// /*AFLA*/ #define PAGE_KERNEL_LARGE_EXEC		__pgprot(__PAGE_KERNEL_LARGE_EXEC)
// /*AFLA*/ #define PAGE_KERNEL_VSYSCALL		__pgprot(__PAGE_KERNEL_VSYSCALL)
// /*AFLA*/ #define PAGE_KERNEL_VVAR		__pgprot(__PAGE_KERNEL_VVAR)
// /*AFLA*/ 
// /*AFLA*/ #define PAGE_KERNEL_IO			__pgprot(__PAGE_KERNEL_IO)
// /*AFLA*/ #define PAGE_KERNEL_IO_NOCACHE		__pgprot(__PAGE_KERNEL_IO_NOCACHE)
// /*AFLA*/ 
// /*AFLA*/ /*         xwr */
// /*AFLA*/ #define __P000	PAGE_NONE
// /*AFLA*/ #define __P001	PAGE_READONLY
// /*AFLA*/ #define __P010	PAGE_COPY
// /*AFLA*/ #define __P011	PAGE_COPY
// /*AFLA*/ #define __P100	PAGE_READONLY_EXEC
// /*AFLA*/ #define __P101	PAGE_READONLY_EXEC
// /*AFLA*/ #define __P110	PAGE_COPY_EXEC
// /*AFLA*/ #define __P111	PAGE_COPY_EXEC
// /*AFLA*/ 
// /*AFLA*/ #define __S000	PAGE_NONE
// /*AFLA*/ #define __S001	PAGE_READONLY
// /*AFLA*/ #define __S010	PAGE_SHARED
// /*AFLA*/ #define __S011	PAGE_SHARED
// /*AFLA*/ #define __S100	PAGE_READONLY_EXEC
// /*AFLA*/ #define __S101	PAGE_READONLY_EXEC
// /*AFLA*/ #define __S110	PAGE_SHARED_EXEC
// /*AFLA*/ #define __S111	PAGE_SHARED_EXEC
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * early identity mapping  pte attrib macros.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ #define __PAGE_KERNEL_IDENT_LARGE_EXEC	__PAGE_KERNEL_LARGE_EXEC
// /*AFLA*/ #else
// /*AFLA*/ #define PTE_IDENT_ATTR	 0x003		/* PRESENT+RW */
// /*AFLA*/ #define PDE_IDENT_ATTR	 0x063		/* PRESENT+RW+DIRTY+ACCESSED */
// /*AFLA*/ #define PGD_IDENT_ATTR	 0x001		/* PRESENT (no other attributes) */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ # include <asm/pgtable_32_types.h>
// /*AFLA*/ #else
// /*AFLA*/ # include <asm/pgtable_64_types.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /* Extracts the PFN from a (pte|pmd|pud|pgd)val_t of a 4KB page */
// /*AFLA*/ #define PTE_PFN_MASK		((pteval_t)PHYSICAL_PAGE_MASK)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  *  Extracts the flags from a (pte|pmd|pud|pgd)val_t
// /*AFLA*/  *  This includes the protection key value.
// /*AFLA*/  */
// /*AFLA*/ #define PTE_FLAGS_MASK		(~PTE_PFN_MASK)
// /*AFLA*/ 
// /*AFLA*/ typedef struct pgprot { pgprotval_t pgprot; } pgprot_t;
// /*AFLA*/ 
// /*AFLA*/ typedef struct { pgdval_t pgd; } pgd_t;
// /*AFLA*/ 
// /*AFLA*/ static inline pgd_t native_make_pgd(pgdval_t val)
// /*AFLA*/ {
// /*AFLA*/ 	return (pgd_t) { val };
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pgdval_t native_pgd_val(pgd_t pgd)
// /*AFLA*/ {
// /*AFLA*/ 	return pgd.pgd;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pgdval_t pgd_flags(pgd_t pgd)
// /*AFLA*/ {
// /*AFLA*/ 	return native_pgd_val(pgd) & PTE_FLAGS_MASK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #if CONFIG_PGTABLE_LEVELS > 3
// /*AFLA*/ typedef struct { pudval_t pud; } pud_t;
// /*AFLA*/ 
// /*AFLA*/ static inline pud_t native_make_pud(pmdval_t val)
// /*AFLA*/ {
// /*AFLA*/ 	return (pud_t) { val };
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pudval_t native_pud_val(pud_t pud)
// /*AFLA*/ {
// /*AFLA*/ 	return pud.pud;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ #include <asm-generic/pgtable-nopud.h>
// /*AFLA*/ 
// /*AFLA*/ static inline pudval_t native_pud_val(pud_t pud)
// /*AFLA*/ {
// /*AFLA*/ 	return native_pgd_val(pud.pgd);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if CONFIG_PGTABLE_LEVELS > 2
// /*AFLA*/ typedef struct { pmdval_t pmd; } pmd_t;
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t native_make_pmd(pmdval_t val)
// /*AFLA*/ {
// /*AFLA*/ 	return (pmd_t) { val };
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmdval_t native_pmd_val(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return pmd.pmd;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ #include <asm-generic/pgtable-nopmd.h>
// /*AFLA*/ 
// /*AFLA*/ static inline pmdval_t native_pmd_val(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return native_pgd_val(pmd.pud.pgd);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline pudval_t pud_pfn_mask(pud_t pud)
// /*AFLA*/ {
// /*AFLA*/ 	if (native_pud_val(pud) & _PAGE_PSE)
// /*AFLA*/ 		return PHYSICAL_PUD_PAGE_MASK;
// /*AFLA*/ 	else
// /*AFLA*/ 		return PTE_PFN_MASK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pudval_t pud_flags_mask(pud_t pud)
// /*AFLA*/ {
// /*AFLA*/ 	return ~pud_pfn_mask(pud);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pudval_t pud_flags(pud_t pud)
// /*AFLA*/ {
// /*AFLA*/ 	return native_pud_val(pud) & pud_flags_mask(pud);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmdval_t pmd_pfn_mask(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	if (native_pmd_val(pmd) & _PAGE_PSE)
// /*AFLA*/ 		return PHYSICAL_PMD_PAGE_MASK;
// /*AFLA*/ 	else
// /*AFLA*/ 		return PTE_PFN_MASK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmdval_t pmd_flags_mask(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return ~pmd_pfn_mask(pmd);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmdval_t pmd_flags(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return native_pmd_val(pmd) & pmd_flags_mask(pmd);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t native_make_pte(pteval_t val)
// /*AFLA*/ {
// /*AFLA*/ 	return (pte_t) { .pte = val };
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pteval_t native_pte_val(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return pte.pte;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pteval_t pte_flags(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return native_pte_val(pte) & PTE_FLAGS_MASK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define pgprot_val(x)	((x).pgprot)
// /*AFLA*/ #define __pgprot(x)	((pgprot_t) { (x) } )
// /*AFLA*/ 
// /*AFLA*/ extern uint16_t __cachemode2pte_tbl[_PAGE_CACHE_MODE_NUM];
// /*AFLA*/ extern uint8_t __pte2cachemode_tbl[8];
// /*AFLA*/ 
// /*AFLA*/ #define __pte2cm_idx(cb)				\
// /*AFLA*/ 	((((cb) >> (_PAGE_BIT_PAT - 2)) & 4) |		\
// /*AFLA*/ 	 (((cb) >> (_PAGE_BIT_PCD - 1)) & 2) |		\
// /*AFLA*/ 	 (((cb) >> _PAGE_BIT_PWT) & 1))
// /*AFLA*/ #define __cm_idx2pte(i)					\
// /*AFLA*/ 	((((i) & 4) << (_PAGE_BIT_PAT - 2)) |		\
// /*AFLA*/ 	 (((i) & 2) << (_PAGE_BIT_PCD - 1)) |		\
// /*AFLA*/ 	 (((i) & 1) << _PAGE_BIT_PWT))
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long cachemode2protval(enum page_cache_mode pcm)
// /*AFLA*/ {
// /*AFLA*/ 	if (likely(pcm == 0))
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	return __cachemode2pte_tbl[pcm];
// /*AFLA*/ }
// /*AFLA*/ static inline pgprot_t cachemode2pgprot(enum page_cache_mode pcm)
// /*AFLA*/ {
// /*AFLA*/ 	return __pgprot(cachemode2protval(pcm));
// /*AFLA*/ }
// /*AFLA*/ static inline enum page_cache_mode pgprot2cachemode(pgprot_t pgprot)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long masked;
// /*AFLA*/ 
// /*AFLA*/ 	masked = pgprot_val(pgprot) & _PAGE_CACHE_MASK;
// /*AFLA*/ 	if (likely(masked == 0))
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	return __pte2cachemode_tbl[__pte2cm_idx(masked)];
// /*AFLA*/ }
// /*AFLA*/ static inline pgprot_t pgprot_4k_2_large(pgprot_t pgprot)
// /*AFLA*/ {
// /*AFLA*/ 	pgprotval_t val = pgprot_val(pgprot);
// /*AFLA*/ 	pgprot_t new;
// /*AFLA*/ 
// /*AFLA*/ 	pgprot_val(new) = (val & ~(_PAGE_PAT | _PAGE_PAT_LARGE)) |
// /*AFLA*/ 		((val & _PAGE_PAT) << (_PAGE_BIT_PAT_LARGE - _PAGE_BIT_PAT));
// /*AFLA*/ 	return new;
// /*AFLA*/ }
// /*AFLA*/ static inline pgprot_t pgprot_large_2_4k(pgprot_t pgprot)
// /*AFLA*/ {
// /*AFLA*/ 	pgprotval_t val = pgprot_val(pgprot);
// /*AFLA*/ 	pgprot_t new;
// /*AFLA*/ 
// /*AFLA*/ 	pgprot_val(new) = (val & ~(_PAGE_PAT | _PAGE_PAT_LARGE)) |
// /*AFLA*/ 			  ((val & _PAGE_PAT_LARGE) >>
// /*AFLA*/ 			   (_PAGE_BIT_PAT_LARGE - _PAGE_BIT_PAT));
// /*AFLA*/ 	return new;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ typedef struct page *pgtable_t;
// /*AFLA*/ 
// /*AFLA*/ extern pteval_t __supported_pte_mask;
// /*AFLA*/ extern void set_nx(void);
// /*AFLA*/ extern int nx_enabled;
// /*AFLA*/ 
// /*AFLA*/ #define pgprot_writecombine	pgprot_writecombine
// /*AFLA*/ extern pgprot_t pgprot_writecombine(pgprot_t prot);
// /*AFLA*/ 
// /*AFLA*/ #define pgprot_writethrough	pgprot_writethrough
// /*AFLA*/ extern pgprot_t pgprot_writethrough(pgprot_t prot);
// /*AFLA*/ 
// /*AFLA*/ /* Indicate that x86 has its own track and untrack pfn vma functions */
// /*AFLA*/ #define __HAVE_PFNMAP_TRACKING
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_PHYS_MEM_ACCESS_PROT
// /*AFLA*/ struct file;
// /*AFLA*/ pgprot_t phys_mem_access_prot(struct file *file, unsigned long pfn,
// /*AFLA*/                               unsigned long size, pgprot_t vma_prot);
// /*AFLA*/ 
// /*AFLA*/ /* Install a pte for a particular vaddr in kernel space. */
// /*AFLA*/ void set_pte_vaddr(unsigned long vaddr, pte_t pte);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ extern void native_pagetable_init(void);
// /*AFLA*/ #else
// /*AFLA*/ #define native_pagetable_init        paging_init
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct seq_file;
// /*AFLA*/ extern void arch_report_meminfo(struct seq_file *m);
// /*AFLA*/ 
// /*AFLA*/ enum pg_level {
// /*AFLA*/ 	PG_LEVEL_NONE,
// /*AFLA*/ 	PG_LEVEL_4K,
// /*AFLA*/ 	PG_LEVEL_2M,
// /*AFLA*/ 	PG_LEVEL_1G,
// /*AFLA*/ 	PG_LEVEL_NUM
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PROC_FS
// /*AFLA*/ extern void update_page_count(int level, unsigned long pages);
// /*AFLA*/ #else
// /*AFLA*/ static inline void update_page_count(int level, unsigned long pages) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Helper function that returns the kernel pagetable entry controlling
// /*AFLA*/  * the virtual address 'address'. NULL means no pagetable entry present.
// /*AFLA*/  * NOTE: the return type is pte_t but if the pmd is PSE then we return it
// /*AFLA*/  * as a pte too.
// /*AFLA*/  */
// /*AFLA*/ extern pte_t *lookup_address(unsigned long address, unsigned int *level);
// /*AFLA*/ extern pte_t *lookup_address_in_pgd(pgd_t *pgd, unsigned long address,
// /*AFLA*/ 				    unsigned int *level);
// /*AFLA*/ extern pmd_t *lookup_pmd_address(unsigned long address);
// /*AFLA*/ extern phys_addr_t slow_virt_to_phys(void *__address);
// /*AFLA*/ extern int kernel_map_pages_in_pgd(pgd_t *pgd, u64 pfn, unsigned long address,
// /*AFLA*/ 				   unsigned numpages, unsigned long page_flags);
// /*AFLA*/ #endif	/* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_PGTABLE_DEFS_H */
