// /*AFLA*/ /*
// /*AFLA*/  * fixmap.h: compile-time virtual memory allocation
// /*AFLA*/  *
// /*AFLA*/  * This file is subject to the terms and conditions of the GNU General Public
// /*AFLA*/  * License.  See the file "COPYING" in the main directory of this archive
// /*AFLA*/  * for more details.
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 1998 Ingo Molnar
// /*AFLA*/  *
// /*AFLA*/  * Support of BIGMEM added by Gerhard Wichert, Siemens AG, July 1999
// /*AFLA*/  * x86_32 and x86_64 integration by Gustavo F. Padovan, February 2009
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _ASM_X86_FIXMAP_H
// /*AFLA*/ #define _ASM_X86_FIXMAP_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <asm/acpi.h>
// /*AFLA*/ #include <asm/apicdef.h>
// /*AFLA*/ #include <asm/page.h>
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ #include <linux/threads.h>
// /*AFLA*/ #include <asm/kmap_types.h>
// /*AFLA*/ #else
// /*AFLA*/ #include <uapi/asm/vsyscall.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We can't declare FIXADDR_TOP as variable for x86_64 because vsyscall
// /*AFLA*/  * uses fixmaps that relies on FIXADDR_TOP for proper address calculation.
// /*AFLA*/  * Because of this, FIXADDR_TOP x86 integration was left as later work.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ /* used by vmalloc.c, vsyscall.lds.S.
// /*AFLA*/  *
// /*AFLA*/  * Leave one empty page between vmalloc'ed areas and
// /*AFLA*/  * the start of the fixmap.
// /*AFLA*/  */
// /*AFLA*/ extern unsigned long __FIXADDR_TOP;
// /*AFLA*/ #define FIXADDR_TOP	((unsigned long)__FIXADDR_TOP)
// /*AFLA*/ #else
// /*AFLA*/ #define FIXADDR_TOP	(round_up(VSYSCALL_ADDR + PAGE_SIZE, 1<<PMD_SHIFT) - \
// /*AFLA*/ 			 PAGE_SIZE)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Here we define all the compile-time 'special' virtual
// /*AFLA*/  * addresses. The point is to have a constant address at
// /*AFLA*/  * compile time, but to set the physical address only
// /*AFLA*/  * in the boot process.
// /*AFLA*/  * for x86_32: We allocate these special addresses
// /*AFLA*/  * from the end of virtual memory (0xfffff000) backwards.
// /*AFLA*/  * Also this lets us do fail-safe vmalloc(), we
// /*AFLA*/  * can guarantee that these special addresses and
// /*AFLA*/  * vmalloc()-ed addresses never overlap.
// /*AFLA*/  *
// /*AFLA*/  * These 'compile-time allocated' memory buffers are
// /*AFLA*/  * fixed-size 4k pages (or larger if used with an increment
// /*AFLA*/  * higher than 1). Use set_fixmap(idx,phys) to associate
// /*AFLA*/  * physical memory with fixmap indices.
// /*AFLA*/  *
// /*AFLA*/  * TLB entries of such buffers will not be flushed across
// /*AFLA*/  * task switches.
// /*AFLA*/  */
// /*AFLA*/ enum fixed_addresses {
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 	FIX_HOLE,
// /*AFLA*/ #else
// /*AFLA*/ #ifdef CONFIG_X86_VSYSCALL_EMULATION
// /*AFLA*/ 	VSYSCALL_PAGE = (FIXADDR_TOP - VSYSCALL_ADDR) >> PAGE_SHIFT,
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 	FIX_DBGP_BASE,
// /*AFLA*/ 	FIX_EARLYCON_MEM_BASE,
// /*AFLA*/ #ifdef CONFIG_PROVIDE_OHCI1394_DMA_INIT
// /*AFLA*/ 	FIX_OHCI1394_BASE,
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_X86_LOCAL_APIC
// /*AFLA*/ 	FIX_APIC_BASE,	/* local (CPU) APIC) -- required for SMP or not */
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_X86_IO_APIC
// /*AFLA*/ 	FIX_IO_APIC_BASE_0,
// /*AFLA*/ 	FIX_IO_APIC_BASE_END = FIX_IO_APIC_BASE_0 + MAX_IO_APICS - 1,
// /*AFLA*/ #endif
// /*AFLA*/ 	FIX_RO_IDT,	/* Virtual mapping for read-only IDT */
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 	FIX_KMAP_BEGIN,	/* reserved pte's for temporary kernel mappings */
// /*AFLA*/ 	FIX_KMAP_END = FIX_KMAP_BEGIN+(KM_TYPE_NR*NR_CPUS)-1,
// /*AFLA*/ #ifdef CONFIG_PCI_MMCONFIG
// /*AFLA*/ 	FIX_PCIE_MCFG,
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_PARAVIRT
// /*AFLA*/ 	FIX_PARAVIRT_BOOTMAP,
// /*AFLA*/ #endif
// /*AFLA*/ 	FIX_TEXT_POKE1,	/* reserve 2 pages for text_poke() */
// /*AFLA*/ 	FIX_TEXT_POKE0, /* first page is last, because allocation is backward */
// /*AFLA*/ #ifdef	CONFIG_X86_INTEL_MID
// /*AFLA*/ 	FIX_LNW_VRTC,
// /*AFLA*/ #endif
// /*AFLA*/ 	__end_of_permanent_fixed_addresses,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * 512 temporary boot-time mappings, used by early_ioremap(),
// /*AFLA*/ 	 * before ioremap() is functional.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * If necessary we round it up to the next 512 pages boundary so
// /*AFLA*/ 	 * that we can have a single pgd entry and a single pte table:
// /*AFLA*/ 	 */
// /*AFLA*/ #define NR_FIX_BTMAPS		64
// /*AFLA*/ #define FIX_BTMAPS_SLOTS	8
// /*AFLA*/ #define TOTAL_FIX_BTMAPS	(NR_FIX_BTMAPS * FIX_BTMAPS_SLOTS)
// /*AFLA*/ 	FIX_BTMAP_END =
// /*AFLA*/ 	 (__end_of_permanent_fixed_addresses ^
// /*AFLA*/ 	  (__end_of_permanent_fixed_addresses + TOTAL_FIX_BTMAPS - 1)) &
// /*AFLA*/ 	 -PTRS_PER_PTE
// /*AFLA*/ 	 ? __end_of_permanent_fixed_addresses + TOTAL_FIX_BTMAPS -
// /*AFLA*/ 	   (__end_of_permanent_fixed_addresses & (TOTAL_FIX_BTMAPS - 1))
// /*AFLA*/ 	 : __end_of_permanent_fixed_addresses,
// /*AFLA*/ 	FIX_BTMAP_BEGIN = FIX_BTMAP_END + TOTAL_FIX_BTMAPS - 1,
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 	FIX_WP_TEST,
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_INTEL_TXT
// /*AFLA*/ 	FIX_TBOOT_BASE,
// /*AFLA*/ #endif
// /*AFLA*/ 	__end_of_fixed_addresses
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ extern void reserve_top_address(unsigned long reserve);
// /*AFLA*/ 
// /*AFLA*/ #define FIXADDR_SIZE	(__end_of_permanent_fixed_addresses << PAGE_SHIFT)
// /*AFLA*/ #define FIXADDR_START		(FIXADDR_TOP - FIXADDR_SIZE)
// /*AFLA*/ 
// /*AFLA*/ extern int fixmaps_set;
// /*AFLA*/ 
// /*AFLA*/ extern pte_t *kmap_pte;
// /*AFLA*/ #define kmap_prot PAGE_KERNEL
// /*AFLA*/ extern pte_t *pkmap_page_table;
// /*AFLA*/ 
// /*AFLA*/ void __native_set_fixmap(enum fixed_addresses idx, pte_t pte);
// /*AFLA*/ void native_set_fixmap(enum fixed_addresses idx,
// /*AFLA*/ 		       phys_addr_t phys, pgprot_t flags);
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_PARAVIRT
// /*AFLA*/ static inline void __set_fixmap(enum fixed_addresses idx,
// /*AFLA*/ 				phys_addr_t phys, pgprot_t flags)
// /*AFLA*/ {
// /*AFLA*/ 	native_set_fixmap(idx, phys, flags);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/fixmap.h>
// /*AFLA*/ 
// /*AFLA*/ #define __late_set_fixmap(idx, phys, flags) __set_fixmap(idx, phys, flags)
// /*AFLA*/ #define __late_clear_fixmap(idx) __set_fixmap(idx, 0, __pgprot(0))
// /*AFLA*/ 
// /*AFLA*/ void __early_set_fixmap(enum fixed_addresses idx,
// /*AFLA*/ 			phys_addr_t phys, pgprot_t flags);
// /*AFLA*/ 
// /*AFLA*/ #endif /* !__ASSEMBLY__ */
// /*AFLA*/ #endif /* _ASM_X86_FIXMAP_H */
