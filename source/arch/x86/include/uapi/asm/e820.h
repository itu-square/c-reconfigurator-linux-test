// /*AFLA*/ #ifndef _UAPI_ASM_X86_E820_H
// /*AFLA*/ #define _UAPI_ASM_X86_E820_H
// /*AFLA*/ #define E820MAP	0x2d0		/* our map */
// /*AFLA*/ #define E820MAX	128		/* number of entries in E820MAP */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Legacy E820 BIOS limits us to 128 (E820MAX) nodes due to the
// /*AFLA*/  * constrained space in the zeropage.  If we have more nodes than
// /*AFLA*/  * that, and if we've booted off EFI firmware, then the EFI tables
// /*AFLA*/  * passed us from the EFI firmware can list more nodes.  Size our
// /*AFLA*/  * internal memory map tables to have room for these additional
// /*AFLA*/  * nodes, based on up to three entries per node for which the
// /*AFLA*/  * kernel was built: MAX_NUMNODES == (1 << CONFIG_NODES_SHIFT),
// /*AFLA*/  * plus E820MAX, allowing space for the possible duplicate E820
// /*AFLA*/  * entries that might need room in the same arrays, prior to the
// /*AFLA*/  * call to sanitize_e820_map() to remove duplicates.  The allowance
// /*AFLA*/  * of three memory map entries per node is "enough" entries for
// /*AFLA*/  * the initial hardware platform motivating this mechanism to make
// /*AFLA*/  * use of additional EFI map entries.  Future platforms may want
// /*AFLA*/  * to allow more than three entries per node or otherwise refine
// /*AFLA*/  * this size.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __KERNEL__
// /*AFLA*/ #define E820_X_MAX E820MAX
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define E820NR	0x1e8		/* # entries in E820MAP */
// /*AFLA*/ 
// /*AFLA*/ #define E820_RAM	1
// /*AFLA*/ #define E820_RESERVED	2
// /*AFLA*/ #define E820_ACPI	3
// /*AFLA*/ #define E820_NVS	4
// /*AFLA*/ #define E820_UNUSABLE	5
// /*AFLA*/ #define E820_PMEM	7
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is a non-standardized way to represent ADR or NVDIMM regions that
// /*AFLA*/  * persist over a reboot.  The kernel will ignore their special capabilities
// /*AFLA*/  * unless the CONFIG_X86_PMEM_LEGACY option is set.
// /*AFLA*/  *
// /*AFLA*/  * ( Note that older platforms also used 6 for the same type of memory,
// /*AFLA*/  *   but newer versions switched to 12 as 6 was assigned differently.  Some
// /*AFLA*/  *   time they will learn... )
// /*AFLA*/  */
// /*AFLA*/ #define E820_PRAM	12
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * reserved RAM used by kernel itself
// /*AFLA*/  * if CONFIG_INTEL_TXT is enabled, memory of this type will be
// /*AFLA*/  * included in the S3 integrity calculation and so should not include
// /*AFLA*/  * any memory that BIOS might alter over the S3 transition
// /*AFLA*/  */
// /*AFLA*/ #define E820_RESERVED_KERN        128
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ struct e820entry {
// /*AFLA*/ 	__u64 addr;	/* start of memory segment */
// /*AFLA*/ 	__u64 size;	/* size of memory segment */
// /*AFLA*/ 	__u32 type;	/* type of memory segment */
// /*AFLA*/ } __attribute__((packed));
// /*AFLA*/ 
// /*AFLA*/ struct e820map {
// /*AFLA*/ 	__u32 nr_map;
// /*AFLA*/ 	struct e820entry map[E820_X_MAX];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define ISA_START_ADDRESS	0xa0000
// /*AFLA*/ #define ISA_END_ADDRESS		0x100000
// /*AFLA*/ 
// /*AFLA*/ #define BIOS_BEGIN		0x000a0000
// /*AFLA*/ #define BIOS_END		0x00100000
// /*AFLA*/ 
// /*AFLA*/ #define BIOS_ROM_BASE		0xffe00000
// /*AFLA*/ #define BIOS_ROM_END		0xffffffff
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_ASM_X86_E820_H */
