// /*AFLA*/ #ifndef _ASM_EARLY_IOREMAP_H_
// /*AFLA*/ #define _ASM_EARLY_IOREMAP_H_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * early_ioremap() and early_iounmap() are for temporary early boot-time
// /*AFLA*/  * mappings, before the real ioremap() is functional.
// /*AFLA*/  */
// /*AFLA*/ extern void __iomem *early_ioremap(resource_size_t phys_addr,
// /*AFLA*/ 				   unsigned long size);
// /*AFLA*/ extern void *early_memremap(resource_size_t phys_addr,
// /*AFLA*/ 			    unsigned long size);
// /*AFLA*/ extern void *early_memremap_ro(resource_size_t phys_addr,
// /*AFLA*/ 			       unsigned long size);
// /*AFLA*/ extern void early_iounmap(void __iomem *addr, unsigned long size);
// /*AFLA*/ extern void early_memunmap(void *addr, unsigned long size);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Weak function called by early_ioremap_reset(). It does nothing, but
// /*AFLA*/  * architectures may provide their own version to do any needed cleanups.
// /*AFLA*/  */
// /*AFLA*/ extern void early_ioremap_shutdown(void);
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_GENERIC_EARLY_IOREMAP) && defined(CONFIG_MMU)
// /*AFLA*/ /* Arch-specific initialization */
// /*AFLA*/ extern void early_ioremap_init(void);
// /*AFLA*/ 
// /*AFLA*/ /* Generic initialization called by architecture code */
// /*AFLA*/ extern void early_ioremap_setup(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Called as last step in paging_init() so library can act
// /*AFLA*/  * accordingly for subsequent map/unmap requests.
// /*AFLA*/  */
// /*AFLA*/ extern void early_ioremap_reset(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Early copy from unmapped memory to kernel mapped memory.
// /*AFLA*/  */
// /*AFLA*/ extern void copy_from_early_mem(void *dest, phys_addr_t src,
// /*AFLA*/ 				unsigned long size);
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ static inline void early_ioremap_init(void) { }
// /*AFLA*/ static inline void early_ioremap_setup(void) { }
// /*AFLA*/ static inline void early_ioremap_reset(void) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_EARLY_IOREMAP_H_ */
