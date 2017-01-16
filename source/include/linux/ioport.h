// /*AFLA*/ /*
// /*AFLA*/  * ioport.h	Definitions of routines for detecting, reserving and
// /*AFLA*/  *		allocating system resources.
// /*AFLA*/  *
// /*AFLA*/  * Authors:	Linus Torvalds
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_IOPORT_H
// /*AFLA*/ #define _LINUX_IOPORT_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ /*
// /*AFLA*/  * Resources are tree-like, allowing
// /*AFLA*/  * nesting etc..
// /*AFLA*/  */
// /*AFLA*/ struct resource {
// /*AFLA*/ 	resource_size_t start;
// /*AFLA*/ 	resource_size_t end;
// /*AFLA*/ 	const char *name;
// /*AFLA*/ 	unsigned long flags;
// /*AFLA*/ 	unsigned long desc;
// /*AFLA*/ 	struct resource *parent, *sibling, *child;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * IO resources have these defined flags.
// /*AFLA*/  *
// /*AFLA*/  * PCI devices expose these flags to userspace in the "resource" sysfs file,
// /*AFLA*/  * so don't move them.
// /*AFLA*/  */
// /*AFLA*/ #define IORESOURCE_BITS		0x000000ff	/* Bus-specific bits */
// /*AFLA*/ 
// /*AFLA*/ #define IORESOURCE_TYPE_BITS	0x00001f00	/* Resource type */
// /*AFLA*/ #define IORESOURCE_IO		0x00000100	/* PCI/ISA I/O ports */
// /*AFLA*/ #define IORESOURCE_MEM		0x00000200
// /*AFLA*/ #define IORESOURCE_REG		0x00000300	/* Register offsets */
// /*AFLA*/ #define IORESOURCE_IRQ		0x00000400
// /*AFLA*/ #define IORESOURCE_DMA		0x00000800
// /*AFLA*/ #define IORESOURCE_BUS		0x00001000
// /*AFLA*/ 
// /*AFLA*/ #define IORESOURCE_PREFETCH	0x00002000	/* No side effects */
// /*AFLA*/ #define IORESOURCE_READONLY	0x00004000
// /*AFLA*/ #define IORESOURCE_CACHEABLE	0x00008000
// /*AFLA*/ #define IORESOURCE_RANGELENGTH	0x00010000
// /*AFLA*/ #define IORESOURCE_SHADOWABLE	0x00020000
// /*AFLA*/ 
// /*AFLA*/ #define IORESOURCE_SIZEALIGN	0x00040000	/* size indicates alignment */
// /*AFLA*/ #define IORESOURCE_STARTALIGN	0x00080000	/* start field is alignment */
// /*AFLA*/ 
// /*AFLA*/ #define IORESOURCE_MEM_64	0x00100000
// /*AFLA*/ #define IORESOURCE_WINDOW	0x00200000	/* forwarded by bridge */
// /*AFLA*/ #define IORESOURCE_MUXED	0x00400000	/* Resource is software muxed */
// /*AFLA*/ 
// /*AFLA*/ #define IORESOURCE_EXT_TYPE_BITS 0x01000000	/* Resource extended types */
// /*AFLA*/ #define IORESOURCE_SYSRAM	0x01000000	/* System RAM (modifier) */
// /*AFLA*/ 
// /*AFLA*/ #define IORESOURCE_EXCLUSIVE	0x08000000	/* Userland may not map this resource */
// /*AFLA*/ 
// /*AFLA*/ #define IORESOURCE_DISABLED	0x10000000
// /*AFLA*/ #define IORESOURCE_UNSET	0x20000000	/* No address assigned yet */
// /*AFLA*/ #define IORESOURCE_AUTO		0x40000000
// /*AFLA*/ #define IORESOURCE_BUSY		0x80000000	/* Driver has marked this resource busy */
// /*AFLA*/ 
// /*AFLA*/ /* I/O resource extended types */
// /*AFLA*/ #define IORESOURCE_SYSTEM_RAM		(IORESOURCE_MEM|IORESOURCE_SYSRAM)
// /*AFLA*/ 
// /*AFLA*/ /* PnP IRQ specific bits (IORESOURCE_BITS) */
// /*AFLA*/ #define IORESOURCE_IRQ_HIGHEDGE		(1<<0)
// /*AFLA*/ #define IORESOURCE_IRQ_LOWEDGE		(1<<1)
// /*AFLA*/ #define IORESOURCE_IRQ_HIGHLEVEL	(1<<2)
// /*AFLA*/ #define IORESOURCE_IRQ_LOWLEVEL		(1<<3)
// /*AFLA*/ #define IORESOURCE_IRQ_SHAREABLE	(1<<4)
// /*AFLA*/ #define IORESOURCE_IRQ_OPTIONAL 	(1<<5)
// /*AFLA*/ 
// /*AFLA*/ /* PnP DMA specific bits (IORESOURCE_BITS) */
// /*AFLA*/ #define IORESOURCE_DMA_TYPE_MASK	(3<<0)
// /*AFLA*/ #define IORESOURCE_DMA_8BIT		(0<<0)
// /*AFLA*/ #define IORESOURCE_DMA_8AND16BIT	(1<<0)
// /*AFLA*/ #define IORESOURCE_DMA_16BIT		(2<<0)
// /*AFLA*/ 
// /*AFLA*/ #define IORESOURCE_DMA_MASTER		(1<<2)
// /*AFLA*/ #define IORESOURCE_DMA_BYTE		(1<<3)
// /*AFLA*/ #define IORESOURCE_DMA_WORD		(1<<4)
// /*AFLA*/ 
// /*AFLA*/ #define IORESOURCE_DMA_SPEED_MASK	(3<<6)
// /*AFLA*/ #define IORESOURCE_DMA_COMPATIBLE	(0<<6)
// /*AFLA*/ #define IORESOURCE_DMA_TYPEA		(1<<6)
// /*AFLA*/ #define IORESOURCE_DMA_TYPEB		(2<<6)
// /*AFLA*/ #define IORESOURCE_DMA_TYPEF		(3<<6)
// /*AFLA*/ 
// /*AFLA*/ /* PnP memory I/O specific bits (IORESOURCE_BITS) */
// /*AFLA*/ #define IORESOURCE_MEM_WRITEABLE	(1<<0)	/* dup: IORESOURCE_READONLY */
// /*AFLA*/ #define IORESOURCE_MEM_CACHEABLE	(1<<1)	/* dup: IORESOURCE_CACHEABLE */
// /*AFLA*/ #define IORESOURCE_MEM_RANGELENGTH	(1<<2)	/* dup: IORESOURCE_RANGELENGTH */
// /*AFLA*/ #define IORESOURCE_MEM_TYPE_MASK	(3<<3)
// /*AFLA*/ #define IORESOURCE_MEM_8BIT		(0<<3)
// /*AFLA*/ #define IORESOURCE_MEM_16BIT		(1<<3)
// /*AFLA*/ #define IORESOURCE_MEM_8AND16BIT	(2<<3)
// /*AFLA*/ #define IORESOURCE_MEM_32BIT		(3<<3)
// /*AFLA*/ #define IORESOURCE_MEM_SHADOWABLE	(1<<5)	/* dup: IORESOURCE_SHADOWABLE */
// /*AFLA*/ #define IORESOURCE_MEM_EXPANSIONROM	(1<<6)
// /*AFLA*/ 
// /*AFLA*/ /* PnP I/O specific bits (IORESOURCE_BITS) */
// /*AFLA*/ #define IORESOURCE_IO_16BIT_ADDR	(1<<0)
// /*AFLA*/ #define IORESOURCE_IO_FIXED		(1<<1)
// /*AFLA*/ #define IORESOURCE_IO_SPARSE		(1<<2)
// /*AFLA*/ 
// /*AFLA*/ /* PCI ROM control bits (IORESOURCE_BITS) */
// /*AFLA*/ #define IORESOURCE_ROM_ENABLE		(1<<0)	/* ROM is enabled, same as PCI_ROM_ADDRESS_ENABLE */
// /*AFLA*/ #define IORESOURCE_ROM_SHADOW		(1<<1)	/* Use RAM image, not ROM BAR */
// /*AFLA*/ 
// /*AFLA*/ /* PCI control bits.  Shares IORESOURCE_BITS with above PCI ROM.  */
// /*AFLA*/ #define IORESOURCE_PCI_FIXED		(1<<4)	/* Do not move resource */
// /*AFLA*/ #define IORESOURCE_PCI_EA_BEI		(1<<5)	/* BAR Equivalent Indicator */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * I/O Resource Descriptors
// /*AFLA*/  *
// /*AFLA*/  * Descriptors are used by walk_iomem_res_desc() and region_intersects()
// /*AFLA*/  * for searching a specific resource range in the iomem table.  Assign
// /*AFLA*/  * a new descriptor when a resource range supports the search interfaces.
// /*AFLA*/  * Otherwise, resource.desc must be set to IORES_DESC_NONE (0).
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	IORES_DESC_NONE				= 0,
// /*AFLA*/ 	IORES_DESC_CRASH_KERNEL			= 1,
// /*AFLA*/ 	IORES_DESC_ACPI_TABLES			= 2,
// /*AFLA*/ 	IORES_DESC_ACPI_NV_STORAGE		= 3,
// /*AFLA*/ 	IORES_DESC_PERSISTENT_MEMORY		= 4,
// /*AFLA*/ 	IORES_DESC_PERSISTENT_MEMORY_LEGACY	= 5,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* helpers to define resources */
// /*AFLA*/ #define DEFINE_RES_NAMED(_start, _size, _name, _flags)			\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		.start = (_start),					\
// /*AFLA*/ 		.end = (_start) + (_size) - 1,				\
// /*AFLA*/ 		.name = (_name),					\
// /*AFLA*/ 		.flags = (_flags),					\
// /*AFLA*/ 		.desc = IORES_DESC_NONE,				\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_RES_IO_NAMED(_start, _size, _name)			\
// /*AFLA*/ 	DEFINE_RES_NAMED((_start), (_size), (_name), IORESOURCE_IO)
// /*AFLA*/ #define DEFINE_RES_IO(_start, _size)					\
// /*AFLA*/ 	DEFINE_RES_IO_NAMED((_start), (_size), NULL)
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_RES_MEM_NAMED(_start, _size, _name)			\
// /*AFLA*/ 	DEFINE_RES_NAMED((_start), (_size), (_name), IORESOURCE_MEM)
// /*AFLA*/ #define DEFINE_RES_MEM(_start, _size)					\
// /*AFLA*/ 	DEFINE_RES_MEM_NAMED((_start), (_size), NULL)
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_RES_IRQ_NAMED(_irq, _name)				\
// /*AFLA*/ 	DEFINE_RES_NAMED((_irq), 1, (_name), IORESOURCE_IRQ)
// /*AFLA*/ #define DEFINE_RES_IRQ(_irq)						\
// /*AFLA*/ 	DEFINE_RES_IRQ_NAMED((_irq), NULL)
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_RES_DMA_NAMED(_dma, _name)				\
// /*AFLA*/ 	DEFINE_RES_NAMED((_dma), 1, (_name), IORESOURCE_DMA)
// /*AFLA*/ #define DEFINE_RES_DMA(_dma)						\
// /*AFLA*/ 	DEFINE_RES_DMA_NAMED((_dma), NULL)
// /*AFLA*/ 
// /*AFLA*/ /* PC/ISA/whatever - the normal PC address spaces: IO and memory */
// /*AFLA*/ extern struct resource ioport_resource;
// /*AFLA*/ extern struct resource iomem_resource;
// /*AFLA*/ 
// /*AFLA*/ extern struct resource *request_resource_conflict(struct resource *root, struct resource *new);
// /*AFLA*/ extern int request_resource(struct resource *root, struct resource *new);
// /*AFLA*/ extern int release_resource(struct resource *new);
// /*AFLA*/ void release_child_resources(struct resource *new);
// /*AFLA*/ extern void reserve_region_with_split(struct resource *root,
// /*AFLA*/ 			     resource_size_t start, resource_size_t end,
// /*AFLA*/ 			     const char *name);
// /*AFLA*/ extern struct resource *insert_resource_conflict(struct resource *parent, struct resource *new);
// /*AFLA*/ extern int insert_resource(struct resource *parent, struct resource *new);
// /*AFLA*/ extern void insert_resource_expand_to_fit(struct resource *root, struct resource *new);
// /*AFLA*/ extern int remove_resource(struct resource *old);
// /*AFLA*/ extern void arch_remove_reservations(struct resource *avail);
// /*AFLA*/ extern int allocate_resource(struct resource *root, struct resource *new,
// /*AFLA*/ 			     resource_size_t size, resource_size_t min,
// /*AFLA*/ 			     resource_size_t max, resource_size_t align,
// /*AFLA*/ 			     resource_size_t (*alignf)(void *,
// /*AFLA*/ 						       const struct resource *,
// /*AFLA*/ 						       resource_size_t,
// /*AFLA*/ 						       resource_size_t),
// /*AFLA*/ 			     void *alignf_data);
// /*AFLA*/ struct resource *lookup_resource(struct resource *root, resource_size_t start);
// /*AFLA*/ int adjust_resource(struct resource *res, resource_size_t start,
// /*AFLA*/ 		    resource_size_t size);
// /*AFLA*/ resource_size_t resource_alignment(struct resource *res);
// /*AFLA*/ static inline resource_size_t resource_size(const struct resource *res)
// /*AFLA*/ {
// /*AFLA*/ 	return res->end - res->start + 1;
// /*AFLA*/ }
// /*AFLA*/ static inline unsigned long resource_type(const struct resource *res)
// /*AFLA*/ {
// /*AFLA*/ 	return res->flags & IORESOURCE_TYPE_BITS;
// /*AFLA*/ }
// /*AFLA*/ static inline unsigned long resource_ext_type(const struct resource *res)
// /*AFLA*/ {
// /*AFLA*/ 	return res->flags & IORESOURCE_EXT_TYPE_BITS;
// /*AFLA*/ }
// /*AFLA*/ /* True iff r1 completely contains r2 */
// /*AFLA*/ static inline bool resource_contains(struct resource *r1, struct resource *r2)
// /*AFLA*/ {
// /*AFLA*/ 	if (resource_type(r1) != resource_type(r2))
// /*AFLA*/ 		return false;
// /*AFLA*/ 	if (r1->flags & IORESOURCE_UNSET || r2->flags & IORESOURCE_UNSET)
// /*AFLA*/ 		return false;
// /*AFLA*/ 	return r1->start <= r2->start && r1->end >= r2->end;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* Convenience shorthand with allocation */
// /*AFLA*/ #define request_region(start,n,name)		__request_region(&ioport_resource, (start), (n), (name), 0)
// /*AFLA*/ #define request_muxed_region(start,n,name)	__request_region(&ioport_resource, (start), (n), (name), IORESOURCE_MUXED)
// /*AFLA*/ #define __request_mem_region(start,n,name, excl) __request_region(&iomem_resource, (start), (n), (name), excl)
// /*AFLA*/ #define request_mem_region(start,n,name) __request_region(&iomem_resource, (start), (n), (name), 0)
// /*AFLA*/ #define request_mem_region_exclusive(start,n,name) \
// /*AFLA*/ 	__request_region(&iomem_resource, (start), (n), (name), IORESOURCE_EXCLUSIVE)
// /*AFLA*/ #define rename_region(region, newname) do { (region)->name = (newname); } while (0)
// /*AFLA*/ 
// /*AFLA*/ extern struct resource * __request_region(struct resource *,
// /*AFLA*/ 					resource_size_t start,
// /*AFLA*/ 					resource_size_t n,
// /*AFLA*/ 					const char *name, int flags);
// /*AFLA*/ 
// /*AFLA*/ /* Compatibility cruft */
// /*AFLA*/ #define release_region(start,n)	__release_region(&ioport_resource, (start), (n))
// /*AFLA*/ #define release_mem_region(start,n)	__release_region(&iomem_resource, (start), (n))
// /*AFLA*/ 
// /*AFLA*/ extern void __release_region(struct resource *, resource_size_t,
// /*AFLA*/ 				resource_size_t);
// /*AFLA*/ #ifdef CONFIG_MEMORY_HOTREMOVE
// /*AFLA*/ extern int release_mem_region_adjustable(struct resource *, resource_size_t,
// /*AFLA*/ 				resource_size_t);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Wrappers for managed devices */
// /*AFLA*/ struct device;
// /*AFLA*/ 
// /*AFLA*/ extern int devm_request_resource(struct device *dev, struct resource *root,
// /*AFLA*/ 				 struct resource *new);
// /*AFLA*/ extern void devm_release_resource(struct device *dev, struct resource *new);
// /*AFLA*/ 
// /*AFLA*/ #define devm_request_region(dev,start,n,name) \
// /*AFLA*/ 	__devm_request_region(dev, &ioport_resource, (start), (n), (name))
// /*AFLA*/ #define devm_request_mem_region(dev,start,n,name) \
// /*AFLA*/ 	__devm_request_region(dev, &iomem_resource, (start), (n), (name))
// /*AFLA*/ 
// /*AFLA*/ extern struct resource * __devm_request_region(struct device *dev,
// /*AFLA*/ 				struct resource *parent, resource_size_t start,
// /*AFLA*/ 				resource_size_t n, const char *name);
// /*AFLA*/ 
// /*AFLA*/ #define devm_release_region(dev, start, n) \
// /*AFLA*/ 	__devm_release_region(dev, &ioport_resource, (start), (n))
// /*AFLA*/ #define devm_release_mem_region(dev, start, n) \
// /*AFLA*/ 	__devm_release_region(dev, &iomem_resource, (start), (n))
// /*AFLA*/ 
// /*AFLA*/ extern void __devm_release_region(struct device *dev, struct resource *parent,
// /*AFLA*/ 				  resource_size_t start, resource_size_t n);
// /*AFLA*/ extern int iomem_map_sanity_check(resource_size_t addr, unsigned long size);
// /*AFLA*/ extern int iomem_is_exclusive(u64 addr);
// /*AFLA*/ 
// /*AFLA*/ extern int
// /*AFLA*/ walk_system_ram_range(unsigned long start_pfn, unsigned long nr_pages,
// /*AFLA*/ 		void *arg, int (*func)(unsigned long, unsigned long, void *));
// /*AFLA*/ extern int
// /*AFLA*/ walk_system_ram_res(u64 start, u64 end, void *arg,
// /*AFLA*/ 		    int (*func)(u64, u64, void *));
// /*AFLA*/ extern int
// /*AFLA*/ walk_iomem_res_desc(unsigned long desc, unsigned long flags, u64 start, u64 end,
// /*AFLA*/ 		    void *arg, int (*func)(u64, u64, void *));
// /*AFLA*/ 
// /*AFLA*/ /* True if any part of r1 overlaps r2 */
// /*AFLA*/ static inline bool resource_overlaps(struct resource *r1, struct resource *r2)
// /*AFLA*/ {
// /*AFLA*/        return (r1->start <= r2->end && r1->end >= r2->start);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ #endif	/* _LINUX_IOPORT_H */
