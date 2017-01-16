// /*AFLA*/ #ifndef __GENERIC_IO_H
// /*AFLA*/ #define __GENERIC_IO_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ #include <asm/byteorder.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These are the "generic" interfaces for doing new-style
// /*AFLA*/  * memory-mapped or PIO accesses. Architectures may do
// /*AFLA*/  * their own arch-optimized versions, these just act as
// /*AFLA*/  * wrappers around the old-style IO register access functions:
// /*AFLA*/  * read[bwl]/write[bwl]/in[bwl]/out[bwl]
// /*AFLA*/  *
// /*AFLA*/  * Don't include this directly, include it from <asm/io.h>.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Read/write from/to an (offsettable) iomem cookie. It might be a PIO
// /*AFLA*/  * access or a MMIO access, these functions don't care. The info is
// /*AFLA*/  * encoded in the hardware mapping set up by the mapping functions
// /*AFLA*/  * (or the cookie itself, depending on implementation and hw).
// /*AFLA*/  *
// /*AFLA*/  * The generic routines just encode the PIO/MMIO as part of the
// /*AFLA*/  * cookie, and coldly assume that the MMIO IO mappings are not
// /*AFLA*/  * in the low address range. Architectures for which this is not
// /*AFLA*/  * true can't use this generic implementation.
// /*AFLA*/  */
// /*AFLA*/ extern unsigned int ioread8(void __iomem *);
// /*AFLA*/ extern unsigned int ioread16(void __iomem *);
// /*AFLA*/ extern unsigned int ioread16be(void __iomem *);
// /*AFLA*/ extern unsigned int ioread32(void __iomem *);
// /*AFLA*/ extern unsigned int ioread32be(void __iomem *);
// /*AFLA*/ #ifdef CONFIG_64BIT
// /*AFLA*/ extern u64 ioread64(void __iomem *);
// /*AFLA*/ extern u64 ioread64be(void __iomem *);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void iowrite8(u8, void __iomem *);
// /*AFLA*/ extern void iowrite16(u16, void __iomem *);
// /*AFLA*/ extern void iowrite16be(u16, void __iomem *);
// /*AFLA*/ extern void iowrite32(u32, void __iomem *);
// /*AFLA*/ extern void iowrite32be(u32, void __iomem *);
// /*AFLA*/ #ifdef CONFIG_64BIT
// /*AFLA*/ extern void iowrite64(u64, void __iomem *);
// /*AFLA*/ extern void iowrite64be(u64, void __iomem *);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * "string" versions of the above. Note that they
// /*AFLA*/  * use native byte ordering for the accesses (on
// /*AFLA*/  * the assumption that IO and memory agree on a
// /*AFLA*/  * byte order, and CPU byteorder is irrelevant).
// /*AFLA*/  *
// /*AFLA*/  * They do _not_ update the port address. If you
// /*AFLA*/  * want MMIO that copies stuff laid out in MMIO
// /*AFLA*/  * memory across multiple ports, use "memcpy_toio()"
// /*AFLA*/  * and friends.
// /*AFLA*/  */
// /*AFLA*/ extern void ioread8_rep(void __iomem *port, void *buf, unsigned long count);
// /*AFLA*/ extern void ioread16_rep(void __iomem *port, void *buf, unsigned long count);
// /*AFLA*/ extern void ioread32_rep(void __iomem *port, void *buf, unsigned long count);
// /*AFLA*/ 
// /*AFLA*/ extern void iowrite8_rep(void __iomem *port, const void *buf, unsigned long count);
// /*AFLA*/ extern void iowrite16_rep(void __iomem *port, const void *buf, unsigned long count);
// /*AFLA*/ extern void iowrite32_rep(void __iomem *port, const void *buf, unsigned long count);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAS_IOPORT_MAP
// /*AFLA*/ /* Create a virtual mapping cookie for an IO port range */
// /*AFLA*/ extern void __iomem *ioport_map(unsigned long port, unsigned int nr);
// /*AFLA*/ extern void ioport_unmap(void __iomem *);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef ARCH_HAS_IOREMAP_WC
// /*AFLA*/ #define ioremap_wc ioremap_nocache
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef ARCH_HAS_IOREMAP_WT
// /*AFLA*/ #define ioremap_wt ioremap_nocache
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PCI
// /*AFLA*/ /* Destroy a virtual mapping cookie for a PCI BAR (memory or IO) */
// /*AFLA*/ struct pci_dev;
// /*AFLA*/ extern void pci_iounmap(struct pci_dev *dev, void __iomem *);
// /*AFLA*/ #elif defined(CONFIG_GENERIC_IOMAP)
// /*AFLA*/ struct pci_dev;
// /*AFLA*/ static inline void pci_iounmap(struct pci_dev *dev, void __iomem *addr)
// /*AFLA*/ { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/pci_iomap.h>
// /*AFLA*/ 
// /*AFLA*/ #endif
