// /*AFLA*/ #ifndef __ASM_MEMORY_MODEL_H
// /*AFLA*/ #define __ASM_MEMORY_MODEL_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/pfn.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_FLATMEM)
// /*AFLA*/ 
// /*AFLA*/ #ifndef ARCH_PFN_OFFSET
// /*AFLA*/ #define ARCH_PFN_OFFSET		(0UL)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #elif defined(CONFIG_DISCONTIGMEM)
// /*AFLA*/ 
// /*AFLA*/ #ifndef arch_pfn_to_nid
// /*AFLA*/ #define arch_pfn_to_nid(pfn)	pfn_to_nid(pfn)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef arch_local_page_offset
// /*AFLA*/ #define arch_local_page_offset(pfn, nid)	\
// /*AFLA*/ 	((pfn) - NODE_DATA(nid)->node_start_pfn)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_DISCONTIGMEM */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * supports 3 memory models.
// /*AFLA*/  */
// /*AFLA*/ #if defined(CONFIG_FLATMEM)
// /*AFLA*/ 
// /*AFLA*/ #define __pfn_to_page(pfn)	(mem_map + ((pfn) - ARCH_PFN_OFFSET))
// /*AFLA*/ #define __page_to_pfn(page)	((unsigned long)((page) - mem_map) + \
// /*AFLA*/ 				 ARCH_PFN_OFFSET)
// /*AFLA*/ #elif defined(CONFIG_DISCONTIGMEM)
// /*AFLA*/ 
// /*AFLA*/ #define __pfn_to_page(pfn)			\
// /*AFLA*/ ({	unsigned long __pfn = (pfn);		\
// /*AFLA*/ 	unsigned long __nid = arch_pfn_to_nid(__pfn);  \
// /*AFLA*/ 	NODE_DATA(__nid)->node_mem_map + arch_local_page_offset(__pfn, __nid);\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __page_to_pfn(pg)						\
// /*AFLA*/ ({	const struct page *__pg = (pg);					\
// /*AFLA*/ 	struct pglist_data *__pgdat = NODE_DATA(page_to_nid(__pg));	\
// /*AFLA*/ 	(unsigned long)(__pg - __pgdat->node_mem_map) +			\
// /*AFLA*/ 	 __pgdat->node_start_pfn;					\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #elif defined(CONFIG_SPARSEMEM_VMEMMAP)
// /*AFLA*/ 
// /*AFLA*/ /* memmap is virtually contiguous.  */
// /*AFLA*/ #define __pfn_to_page(pfn)	(vmemmap + (pfn))
// /*AFLA*/ #define __page_to_pfn(page)	(unsigned long)((page) - vmemmap)
// /*AFLA*/ 
// /*AFLA*/ #elif defined(CONFIG_SPARSEMEM)
// /*AFLA*/ /*
// /*AFLA*/  * Note: section's mem_map is encoded to reflect its start_pfn.
// /*AFLA*/  * section[i].section_mem_map == mem_map's address - start_pfn;
// /*AFLA*/  */
// /*AFLA*/ #define __page_to_pfn(pg)					\
// /*AFLA*/ ({	const struct page *__pg = (pg);				\
// /*AFLA*/ 	int __sec = page_to_section(__pg);			\
// /*AFLA*/ 	(unsigned long)(__pg - __section_mem_map_addr(__nr_to_section(__sec)));	\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __pfn_to_page(pfn)				\
// /*AFLA*/ ({	unsigned long __pfn = (pfn);			\
// /*AFLA*/ 	struct mem_section *__sec = __pfn_to_section(__pfn);	\
// /*AFLA*/ 	__section_mem_map_addr(__sec) + __pfn;		\
// /*AFLA*/ })
// /*AFLA*/ #endif /* CONFIG_FLATMEM/DISCONTIGMEM/SPARSEMEM */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Convert a physical address to a Page Frame Number and back
// /*AFLA*/  */
// /*AFLA*/ #define	__phys_to_pfn(paddr)	PHYS_PFN(paddr)
// /*AFLA*/ #define	__pfn_to_phys(pfn)	PFN_PHYS(pfn)
// /*AFLA*/ 
// /*AFLA*/ #define page_to_pfn __page_to_pfn
// /*AFLA*/ #define pfn_to_page __pfn_to_page
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif
