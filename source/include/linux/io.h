// /*AFLA*/ /*
// /*AFLA*/  * Copyright 2006 PathScale, Inc.  All Rights Reserved.
// /*AFLA*/  *
// /*AFLA*/  * This file is free software; you can redistribute it and/or modify
// /*AFLA*/  * it under the terms of version 2 of the GNU General Public License
// /*AFLA*/  * as published by the Free Software Foundation.
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it will be useful,
// /*AFLA*/  * but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/  * GNU General Public License for more details.
// /*AFLA*/  *
// /*AFLA*/  * You should have received a copy of the GNU General Public License
// /*AFLA*/  * along with this program; if not, write to the Free Software Foundation,
// /*AFLA*/  * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_IO_H
// /*AFLA*/ #define _LINUX_IO_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/init.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ #include <linux/err.h>
// /*AFLA*/ #include <asm/io.h>
// /*AFLA*/ #include <asm/page.h>
// /*AFLA*/ 
// /*AFLA*/ struct device;
// /*AFLA*/ struct resource;
// /*AFLA*/ 
// /*AFLA*/ __visible void __iowrite32_copy(void __iomem *to, const void *from, size_t count);
// /*AFLA*/ void __ioread32_copy(void *to, const void __iomem *from, size_t count);
// /*AFLA*/ void __iowrite64_copy(void __iomem *to, const void *from, size_t count);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ int ioremap_page_range(unsigned long addr, unsigned long end,
// /*AFLA*/ 		       phys_addr_t phys_addr, pgprot_t prot);
// /*AFLA*/ #else
// /*AFLA*/ static inline int ioremap_page_range(unsigned long addr, unsigned long end,
// /*AFLA*/ 				     phys_addr_t phys_addr, pgprot_t prot)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_ARCH_HUGE_VMAP
// /*AFLA*/ void __init ioremap_huge_init(void);
// /*AFLA*/ int arch_ioremap_pud_supported(void);
// /*AFLA*/ int arch_ioremap_pmd_supported(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void ioremap_huge_init(void) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Managed iomap interface
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_HAS_IOPORT_MAP
// /*AFLA*/ void __iomem * devm_ioport_map(struct device *dev, unsigned long port,
// /*AFLA*/ 			       unsigned int nr);
// /*AFLA*/ void devm_ioport_unmap(struct device *dev, void __iomem *addr);
// /*AFLA*/ #else
// /*AFLA*/ static inline void __iomem *devm_ioport_map(struct device *dev,
// /*AFLA*/ 					     unsigned long port,
// /*AFLA*/ 					     unsigned int nr)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void devm_ioport_unmap(struct device *dev, void __iomem *addr)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define IOMEM_ERR_PTR(err) (__force void __iomem *)ERR_PTR(err)
// /*AFLA*/ 
// /*AFLA*/ void __iomem *devm_ioremap(struct device *dev, resource_size_t offset,
// /*AFLA*/ 			   resource_size_t size);
// /*AFLA*/ void __iomem *devm_ioremap_nocache(struct device *dev, resource_size_t offset,
// /*AFLA*/ 				   resource_size_t size);
// /*AFLA*/ void __iomem *devm_ioremap_wc(struct device *dev, resource_size_t offset,
// /*AFLA*/ 				   resource_size_t size);
// /*AFLA*/ void devm_iounmap(struct device *dev, void __iomem *addr);
// /*AFLA*/ int check_signature(const volatile void __iomem *io_addr,
// /*AFLA*/ 			const unsigned char *signature, int length);
// /*AFLA*/ void devm_ioremap_release(struct device *dev, void *res);
// /*AFLA*/ 
// /*AFLA*/ void *devm_memremap(struct device *dev, resource_size_t offset,
// /*AFLA*/ 		size_t size, unsigned long flags);
// /*AFLA*/ void devm_memunmap(struct device *dev, void *addr);
// /*AFLA*/ 
// /*AFLA*/ void *__devm_memremap_pages(struct device *dev, struct resource *res);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Some systems do not have legacy ISA devices.
// /*AFLA*/  * /dev/port is not a valid interface on these systems.
// /*AFLA*/  * So for those archs, <asm/io.h> should define the following symbol.
// /*AFLA*/  */
// /*AFLA*/ #ifndef arch_has_dev_port
// /*AFLA*/ #define arch_has_dev_port()     (1)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Some systems (x86 without PAT) have a somewhat reliable way to mark a
// /*AFLA*/  * physical address range such that uncached mappings will actually
// /*AFLA*/  * end up write-combining.  This facility should be used in conjunction
// /*AFLA*/  * with pgprot_writecombine, ioremap-wc, or set_memory_wc, since it has
// /*AFLA*/  * no effect if the per-page mechanisms are functional.
// /*AFLA*/  * (On x86 without PAT, these functions manipulate MTRRs.)
// /*AFLA*/  *
// /*AFLA*/  * arch_phys_del_wc(0) or arch_phys_del_wc(any error code) is guaranteed
// /*AFLA*/  * to have no effect.
// /*AFLA*/  */
// /*AFLA*/ #ifndef arch_phys_wc_add
// /*AFLA*/ static inline int __must_check arch_phys_wc_add(unsigned long base,
// /*AFLA*/ 						unsigned long size)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;  /* It worked (i.e. did nothing). */
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void arch_phys_wc_del(int handle)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define arch_phys_wc_add arch_phys_wc_add
// /*AFLA*/ #ifndef arch_phys_wc_index
// /*AFLA*/ static inline int arch_phys_wc_index(int handle)
// /*AFLA*/ {
// /*AFLA*/ 	return -1;
// /*AFLA*/ }
// /*AFLA*/ #define arch_phys_wc_index arch_phys_wc_index
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	/* See memremap() kernel-doc for usage description... */
// /*AFLA*/ 	MEMREMAP_WB = 1 << 0,
// /*AFLA*/ 	MEMREMAP_WT = 1 << 1,
// /*AFLA*/ 	MEMREMAP_WC = 1 << 2,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ void *memremap(resource_size_t offset, size_t size, unsigned long flags);
// /*AFLA*/ void memunmap(void *addr);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * On x86 PAT systems we have memory tracking that keeps track of
// /*AFLA*/  * the allowed mappings on memory ranges. This tracking works for
// /*AFLA*/  * all the in-kernel mapping APIs (ioremap*), but where the user
// /*AFLA*/  * wishes to map a range from a physical device into user memory
// /*AFLA*/  * the tracking won't be updated. This API is to be used by
// /*AFLA*/  * drivers which remap physical device pages into userspace,
// /*AFLA*/  * and wants to make sure they are mapped WC and not UC.
// /*AFLA*/  */
// /*AFLA*/ #ifndef arch_io_reserve_memtype_wc
// /*AFLA*/ static inline int arch_io_reserve_memtype_wc(resource_size_t base,
// /*AFLA*/ 					     resource_size_t size)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void arch_io_free_memtype_wc(resource_size_t base,
// /*AFLA*/ 					   resource_size_t size)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_IO_H */
