// /*AFLA*/ #ifndef _LINUX_PFN_H_
// /*AFLA*/ #define _LINUX_PFN_H_
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * pfn_t: encapsulates a page-frame number that is optionally backed
// /*AFLA*/  * by memmap (struct page).  Whether a pfn_t has a 'struct page'
// /*AFLA*/  * backing is indicated by flags in the high bits of the value.
// /*AFLA*/  */
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	u64 val;
// /*AFLA*/ } pfn_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define PFN_ALIGN(x)	(((unsigned long)(x) + (PAGE_SIZE - 1)) & PAGE_MASK)
// /*AFLA*/ #define PFN_UP(x)	(((x) + PAGE_SIZE-1) >> PAGE_SHIFT)
// /*AFLA*/ #define PFN_DOWN(x)	((x) >> PAGE_SHIFT)
// /*AFLA*/ #define PFN_PHYS(x)	((phys_addr_t)(x) << PAGE_SHIFT)
// /*AFLA*/ #define PHYS_PFN(x)	((unsigned long)((x) >> PAGE_SHIFT))
// /*AFLA*/ 
// /*AFLA*/ #endif
