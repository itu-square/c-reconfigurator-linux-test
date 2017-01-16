// /*AFLA*/ #ifndef _ASM_X86_IO_H
// /*AFLA*/ #define _ASM_X86_IO_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This file contains the definitions for the x86 IO instructions
// /*AFLA*/  * inb/inw/inl/outb/outw/outl and the "string versions" of the same
// /*AFLA*/  * (insb/insw/insl/outsb/outsw/outsl). You can also use "pausing"
// /*AFLA*/  * versions of the single-IO instructions (inb_p/inw_p/..).
// /*AFLA*/  *
// /*AFLA*/  * This file is not meant to be obfuscating: it's just complicated
// /*AFLA*/  * to (a) handle it all in a way that makes gcc able to optimize it
// /*AFLA*/  * as well as possible and (b) trying to avoid writing the same thing
// /*AFLA*/  * over and over again with slight variations and possibly making a
// /*AFLA*/  * mistake somewhere.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Thanks to James van Artsdalen for a better timing-fix than
// /*AFLA*/  * the two short jumps: using outb's to a nonexistent port seems
// /*AFLA*/  * to guarantee better timings even on fast machines.
// /*AFLA*/  *
// /*AFLA*/  * On the other hand, I'd like to be sure of a non-existent port:
// /*AFLA*/  * I feel a bit unsafe about using 0x80 (should be safe, though)
// /*AFLA*/  *
// /*AFLA*/  *		Linus
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/  /*
// /*AFLA*/   *  Bit simplified and optimized by Jan Hubicka
// /*AFLA*/   *  Support of BIGMEM added by Gerhard Wichert, Siemens AG, July 1999.
// /*AFLA*/   *
// /*AFLA*/   *  isa_memset_io, isa_memcpy_fromio, isa_memcpy_toio added,
// /*AFLA*/   *  isa_read[wl] and isa_write[wl] fixed
// /*AFLA*/   *  - Arnaldo Carvalho de Melo <acme@conectiva.com.br>
// /*AFLA*/   */
// /*AFLA*/ 
// /*AFLA*/ #define ARCH_HAS_IOREMAP_WC
// /*AFLA*/ #define ARCH_HAS_IOREMAP_WT
// /*AFLA*/ 
// /*AFLA*/ #include <linux/string.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <asm/page.h>
// /*AFLA*/ #include <asm/early_ioremap.h>
// /*AFLA*/ #include <asm/pgtable_types.h>
// /*AFLA*/ 
// /*AFLA*/ #define build_mmio_read(name, size, type, reg, barrier) \
// /*AFLA*/ static inline type name(const volatile void __iomem *addr) \
// /*AFLA*/ { type ret; asm volatile("mov" size " %1,%0":reg (ret) \
// /*AFLA*/ :"m" (*(volatile type __force *)addr) barrier); return ret; }
// /*AFLA*/ 
// /*AFLA*/ #define build_mmio_write(name, size, type, reg, barrier) \
// /*AFLA*/ static inline void name(type val, volatile void __iomem *addr) \
// /*AFLA*/ { asm volatile("mov" size " %0,%1": :reg (val), \
// /*AFLA*/ "m" (*(volatile type __force *)addr) barrier); }
// /*AFLA*/ 
// /*AFLA*/ build_mmio_read(readb, "b", unsigned char, "=q", :"memory")
// /*AFLA*/ build_mmio_read(readw, "w", unsigned short, "=r", :"memory")
// /*AFLA*/ build_mmio_read(readl, "l", unsigned int, "=r", :"memory")
// /*AFLA*/ 
// /*AFLA*/ build_mmio_read(__readb, "b", unsigned char, "=q", )
// /*AFLA*/ build_mmio_read(__readw, "w", unsigned short, "=r", )
// /*AFLA*/ build_mmio_read(__readl, "l", unsigned int, "=r", )
// /*AFLA*/ 
// /*AFLA*/ build_mmio_write(writeb, "b", unsigned char, "q", :"memory")
// /*AFLA*/ build_mmio_write(writew, "w", unsigned short, "r", :"memory")
// /*AFLA*/ build_mmio_write(writel, "l", unsigned int, "r", :"memory")
// /*AFLA*/ 
// /*AFLA*/ build_mmio_write(__writeb, "b", unsigned char, "q", )
// /*AFLA*/ build_mmio_write(__writew, "w", unsigned short, "r", )
// /*AFLA*/ build_mmio_write(__writel, "l", unsigned int, "r", )
// /*AFLA*/ 
// /*AFLA*/ #define readb_relaxed(a) __readb(a)
// /*AFLA*/ #define readw_relaxed(a) __readw(a)
// /*AFLA*/ #define readl_relaxed(a) __readl(a)
// /*AFLA*/ #define __raw_readb __readb
// /*AFLA*/ #define __raw_readw __readw
// /*AFLA*/ #define __raw_readl __readl
// /*AFLA*/ 
// /*AFLA*/ #define writeb_relaxed(v, a) __writeb(v, a)
// /*AFLA*/ #define writew_relaxed(v, a) __writew(v, a)
// /*AFLA*/ #define writel_relaxed(v, a) __writel(v, a)
// /*AFLA*/ #define __raw_writeb __writeb
// /*AFLA*/ #define __raw_writew __writew
// /*AFLA*/ #define __raw_writel __writel
// /*AFLA*/ 
// /*AFLA*/ #define mmiowb() barrier()
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 
// /*AFLA*/ build_mmio_read(readq, "q", unsigned long, "=r", :"memory")
// /*AFLA*/ build_mmio_write(writeq, "q", unsigned long, "r", :"memory")
// /*AFLA*/ 
// /*AFLA*/ #define readq_relaxed(a)	readq(a)
// /*AFLA*/ #define writeq_relaxed(v, a)	writeq(v, a)
// /*AFLA*/ 
// /*AFLA*/ #define __raw_readq(a)		readq(a)
// /*AFLA*/ #define __raw_writeq(val, addr)	writeq(val, addr)
// /*AFLA*/ 
// /*AFLA*/ /* Let people know that we have them */
// /*AFLA*/ #define readq			readq
// /*AFLA*/ #define writeq			writeq
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  *	virt_to_phys	-	map virtual addresses to physical
// /*AFLA*/  *	@address: address to remap
// /*AFLA*/  *
// /*AFLA*/  *	The returned physical address is the physical (CPU) mapping for
// /*AFLA*/  *	the memory address given. It is only valid to use this function on
// /*AFLA*/  *	addresses directly mapped or allocated via kmalloc.
// /*AFLA*/  *
// /*AFLA*/  *	This function does not give bus mappings for DMA transfers. In
// /*AFLA*/  *	almost all conceivable cases a device driver should not be using
// /*AFLA*/  *	this function
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static inline phys_addr_t virt_to_phys(volatile void *address)
// /*AFLA*/ {
// /*AFLA*/ 	return __pa(address);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  *	phys_to_virt	-	map physical address to virtual
// /*AFLA*/  *	@address: address to remap
// /*AFLA*/  *
// /*AFLA*/  *	The returned virtual address is a current CPU mapping for
// /*AFLA*/  *	the memory address given. It is only valid to use this function on
// /*AFLA*/  *	addresses that have a kernel mapping
// /*AFLA*/  *
// /*AFLA*/  *	This function does not handle bus mappings for DMA transfers. In
// /*AFLA*/  *	almost all conceivable cases a device driver should not be using
// /*AFLA*/  *	this function
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static inline void *phys_to_virt(phys_addr_t address)
// /*AFLA*/ {
// /*AFLA*/ 	return __va(address);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Change "struct page" to physical address.
// /*AFLA*/  */
// /*AFLA*/ #define page_to_phys(page)    ((dma_addr_t)page_to_pfn(page) << PAGE_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * ISA I/O bus memory addresses are 1:1 with the physical address.
// /*AFLA*/  * However, we truncate the address to unsigned int to avoid undesirable
// /*AFLA*/  * promitions in legacy drivers.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned int isa_virt_to_bus(volatile void *address)
// /*AFLA*/ {
// /*AFLA*/ 	return (unsigned int)virt_to_phys(address);
// /*AFLA*/ }
// /*AFLA*/ #define isa_page_to_bus(page)	((unsigned int)page_to_phys(page))
// /*AFLA*/ #define isa_bus_to_virt		phys_to_virt
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * However PCI ones are not necessarily 1:1 and therefore these interfaces
// /*AFLA*/  * are forbidden in portable PCI drivers.
// /*AFLA*/  *
// /*AFLA*/  * Allow them on x86 for legacy drivers, though.
// /*AFLA*/  */
// /*AFLA*/ #define virt_to_bus virt_to_phys
// /*AFLA*/ #define bus_to_virt phys_to_virt
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ioremap     -   map bus memory into CPU space
// /*AFLA*/  * @offset:    bus address of the memory
// /*AFLA*/  * @size:      size of the resource to map
// /*AFLA*/  *
// /*AFLA*/  * ioremap performs a platform specific sequence of operations to
// /*AFLA*/  * make bus memory CPU accessible via the readb/readw/readl/writeb/
// /*AFLA*/  * writew/writel functions and the other mmio helpers. The returned
// /*AFLA*/  * address is not guaranteed to be usable directly as a virtual
// /*AFLA*/  * address.
// /*AFLA*/  *
// /*AFLA*/  * If the area you are trying to map is a PCI BAR you should have a
// /*AFLA*/  * look at pci_iomap().
// /*AFLA*/  */
// /*AFLA*/ extern void __iomem *ioremap_nocache(resource_size_t offset, unsigned long size);
// /*AFLA*/ extern void __iomem *ioremap_uc(resource_size_t offset, unsigned long size);
// /*AFLA*/ #define ioremap_uc ioremap_uc
// /*AFLA*/ 
// /*AFLA*/ extern void __iomem *ioremap_cache(resource_size_t offset, unsigned long size);
// /*AFLA*/ extern void __iomem *ioremap_prot(resource_size_t offset, unsigned long size,
// /*AFLA*/ 				unsigned long prot_val);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The default ioremap() behavior is non-cached:
// /*AFLA*/  */
// /*AFLA*/ static inline void __iomem *ioremap(resource_size_t offset, unsigned long size)
// /*AFLA*/ {
// /*AFLA*/ 	return ioremap_nocache(offset, size);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void iounmap(volatile void __iomem *addr);
// /*AFLA*/ 
// /*AFLA*/ extern void set_iounmap_nonlazy(void);
// /*AFLA*/ 
// /*AFLA*/ #ifdef __KERNEL__
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/iomap.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Convert a virtual cached pointer to an uncached pointer
// /*AFLA*/  */
// /*AFLA*/ #define xlate_dev_kmem_ptr(p)	p
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ memset_io(volatile void __iomem *addr, unsigned char val, size_t count)
// /*AFLA*/ {
// /*AFLA*/ 	memset((void __force *)addr, val, count);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ memcpy_fromio(void *dst, const volatile void __iomem *src, size_t count)
// /*AFLA*/ {
// /*AFLA*/ 	memcpy(dst, (const void __force *)src, count);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ memcpy_toio(volatile void __iomem *dst, const void *src, size_t count)
// /*AFLA*/ {
// /*AFLA*/ 	memcpy((void __force *)dst, src, count);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * ISA space is 'always mapped' on a typical x86 system, no need to
// /*AFLA*/  * explicitly ioremap() it. The fact that the ISA IO space is mapped
// /*AFLA*/  * to PAGE_OFFSET is pure coincidence - it does not mean ISA values
// /*AFLA*/  * are physical addresses. The following constant pointer can be
// /*AFLA*/  * used as the IO-area pointer (it can be iounmapped as well, so the
// /*AFLA*/  * analogy with PCI is quite large):
// /*AFLA*/  */
// /*AFLA*/ #define __ISA_IO_base ((char __iomem *)(PAGE_OFFSET))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  *	Cache management
// /*AFLA*/  *
// /*AFLA*/  *	This needed for two cases
// /*AFLA*/  *	1. Out of order aware processors
// /*AFLA*/  *	2. Accidentally out of order processors (PPro errata #51)
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static inline void flush_write_buffers(void)
// /*AFLA*/ {
// /*AFLA*/ #if defined(CONFIG_X86_PPRO_FENCE)
// /*AFLA*/ 	asm volatile("lock; addl $0,0(%%esp)": : :"memory");
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* __KERNEL__ */
// /*AFLA*/ 
// /*AFLA*/ extern void native_io_delay(void);
// /*AFLA*/ 
// /*AFLA*/ extern int io_delay_type;
// /*AFLA*/ extern void io_delay_init(void);
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_PARAVIRT)
// /*AFLA*/ #include <asm/paravirt.h>
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline void slow_down_io(void)
// /*AFLA*/ {
// /*AFLA*/ 	native_io_delay();
// /*AFLA*/ #ifdef REALLY_SLOW_IO
// /*AFLA*/ 	native_io_delay();
// /*AFLA*/ 	native_io_delay();
// /*AFLA*/ 	native_io_delay();
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define BUILDIO(bwl, bw, type)						\
// /*AFLA*/ static inline void out##bwl(unsigned type value, int port)		\
// /*AFLA*/ {									\
// /*AFLA*/ 	asm volatile("out" #bwl " %" #bw "0, %w1"			\
// /*AFLA*/ 		     : : "a"(value), "Nd"(port));			\
// /*AFLA*/ }									\
// /*AFLA*/ 									\
// /*AFLA*/ static inline unsigned type in##bwl(int port)				\
// /*AFLA*/ {									\
// /*AFLA*/ 	unsigned type value;						\
// /*AFLA*/ 	asm volatile("in" #bwl " %w1, %" #bw "0"			\
// /*AFLA*/ 		     : "=a"(value) : "Nd"(port));			\
// /*AFLA*/ 	return value;							\
// /*AFLA*/ }									\
// /*AFLA*/ 									\
// /*AFLA*/ static inline void out##bwl##_p(unsigned type value, int port)		\
// /*AFLA*/ {									\
// /*AFLA*/ 	out##bwl(value, port);						\
// /*AFLA*/ 	slow_down_io();							\
// /*AFLA*/ }									\
// /*AFLA*/ 									\
// /*AFLA*/ static inline unsigned type in##bwl##_p(int port)			\
// /*AFLA*/ {									\
// /*AFLA*/ 	unsigned type value = in##bwl(port);				\
// /*AFLA*/ 	slow_down_io();							\
// /*AFLA*/ 	return value;							\
// /*AFLA*/ }									\
// /*AFLA*/ 									\
// /*AFLA*/ static inline void outs##bwl(int port, const void *addr, unsigned long count) \
// /*AFLA*/ {									\
// /*AFLA*/ 	asm volatile("rep; outs" #bwl					\
// /*AFLA*/ 		     : "+S"(addr), "+c"(count) : "d"(port));		\
// /*AFLA*/ }									\
// /*AFLA*/ 									\
// /*AFLA*/ static inline void ins##bwl(int port, void *addr, unsigned long count)	\
// /*AFLA*/ {									\
// /*AFLA*/ 	asm volatile("rep; ins" #bwl					\
// /*AFLA*/ 		     : "+D"(addr), "+c"(count) : "d"(port));		\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ BUILDIO(b, b, char)
// /*AFLA*/ BUILDIO(w, w, short)
// /*AFLA*/ BUILDIO(l, , int)
// /*AFLA*/ 
// /*AFLA*/ extern void *xlate_dev_mem_ptr(phys_addr_t phys);
// /*AFLA*/ extern void unxlate_dev_mem_ptr(phys_addr_t phys, void *addr);
// /*AFLA*/ 
// /*AFLA*/ extern int ioremap_change_attr(unsigned long vaddr, unsigned long size,
// /*AFLA*/ 				enum page_cache_mode pcm);
// /*AFLA*/ extern void __iomem *ioremap_wc(resource_size_t offset, unsigned long size);
// /*AFLA*/ extern void __iomem *ioremap_wt(resource_size_t offset, unsigned long size);
// /*AFLA*/ 
// /*AFLA*/ extern bool is_early_ioremap_ptep(pte_t *ptep);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_XEN
// /*AFLA*/ #include <xen/xen.h>
// /*AFLA*/ struct bio_vec;
// /*AFLA*/ 
// /*AFLA*/ extern bool xen_biovec_phys_mergeable(const struct bio_vec *vec1,
// /*AFLA*/ 				      const struct bio_vec *vec2);
// /*AFLA*/ 
// /*AFLA*/ #define BIOVEC_PHYS_MERGEABLE(vec1, vec2)				\
// /*AFLA*/ 	(__BIOVEC_PHYS_MERGEABLE(vec1, vec2) &&				\
// /*AFLA*/ 	 (!xen_domain() || xen_biovec_phys_mergeable(vec1, vec2)))
// /*AFLA*/ #endif	/* CONFIG_XEN */
// /*AFLA*/ 
// /*AFLA*/ #define IO_SPACE_LIMIT 0xffff
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MTRR
// /*AFLA*/ extern int __must_check arch_phys_wc_index(int handle);
// /*AFLA*/ #define arch_phys_wc_index arch_phys_wc_index
// /*AFLA*/ 
// /*AFLA*/ extern int __must_check arch_phys_wc_add(unsigned long base,
// /*AFLA*/ 					 unsigned long size);
// /*AFLA*/ extern void arch_phys_wc_del(int handle);
// /*AFLA*/ #define arch_phys_wc_add arch_phys_wc_add
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_PAT
// /*AFLA*/ extern int arch_io_reserve_memtype_wc(resource_size_t start, resource_size_t size);
// /*AFLA*/ extern void arch_io_free_memtype_wc(resource_size_t start, resource_size_t size);
// /*AFLA*/ #define arch_io_reserve_memtype_wc arch_io_reserve_memtype_wc
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_IO_H */
