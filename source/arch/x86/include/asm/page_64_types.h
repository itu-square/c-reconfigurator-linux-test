// /*AFLA*/ #ifndef _ASM_X86_PAGE_64_DEFS_H
// /*AFLA*/ #define _ASM_X86_PAGE_64_DEFS_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #include <asm/kaslr.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KASAN
// /*AFLA*/ #define KASAN_STACK_ORDER 1
// /*AFLA*/ #else
// /*AFLA*/ #define KASAN_STACK_ORDER 0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define THREAD_SIZE_ORDER	(2 + KASAN_STACK_ORDER)
// /*AFLA*/ #define THREAD_SIZE  (PAGE_SIZE << THREAD_SIZE_ORDER)
// /*AFLA*/ #define CURRENT_MASK (~(THREAD_SIZE - 1))
// /*AFLA*/ 
// /*AFLA*/ #define EXCEPTION_STACK_ORDER (0 + KASAN_STACK_ORDER)
// /*AFLA*/ #define EXCEPTION_STKSZ (PAGE_SIZE << EXCEPTION_STACK_ORDER)
// /*AFLA*/ 
// /*AFLA*/ #define DEBUG_STACK_ORDER (EXCEPTION_STACK_ORDER + 1)
// /*AFLA*/ #define DEBUG_STKSZ (PAGE_SIZE << DEBUG_STACK_ORDER)
// /*AFLA*/ 
// /*AFLA*/ #define IRQ_STACK_ORDER (2 + KASAN_STACK_ORDER)
// /*AFLA*/ #define IRQ_STACK_SIZE (PAGE_SIZE << IRQ_STACK_ORDER)
// /*AFLA*/ 
// /*AFLA*/ #define DOUBLEFAULT_STACK 1
// /*AFLA*/ #define NMI_STACK 2
// /*AFLA*/ #define DEBUG_STACK 3
// /*AFLA*/ #define MCE_STACK 4
// /*AFLA*/ #define N_EXCEPTION_STACKS 4  /* hw limit: 7 */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Set __PAGE_OFFSET to the most negative possible address +
// /*AFLA*/  * PGDIR_SIZE*16 (pgd slot 272).  The gap is to allow a space for a
// /*AFLA*/  * hypervisor to fit.  Choosing 16 slots here is arbitrary, but it's
// /*AFLA*/  * what Xen requires.
// /*AFLA*/  */
// /*AFLA*/ #define __PAGE_OFFSET_BASE      _AC(0xffff880000000000, UL)
// /*AFLA*/ #ifdef CONFIG_RANDOMIZE_MEMORY
// /*AFLA*/ #define __PAGE_OFFSET           page_offset_base
// /*AFLA*/ #else
// /*AFLA*/ #define __PAGE_OFFSET           __PAGE_OFFSET_BASE
// /*AFLA*/ #endif /* CONFIG_RANDOMIZE_MEMORY */
// /*AFLA*/ 
// /*AFLA*/ #define __START_KERNEL_map	_AC(0xffffffff80000000, UL)
// /*AFLA*/ 
// /*AFLA*/ /* See Documentation/x86/x86_64/mm.txt for a description of the memory map. */
// /*AFLA*/ #define __PHYSICAL_MASK_SHIFT	46
// /*AFLA*/ #define __VIRTUAL_MASK_SHIFT	47
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Kernel image size is limited to 1GiB due to the fixmap living in the
// /*AFLA*/  * next 1GiB (see level2_kernel_pgt in arch/x86/kernel/head_64.S). Use
// /*AFLA*/  * 512MiB by default, leaving 1.5GiB for modules once the page tables
// /*AFLA*/  * are fully set up. If kernel ASLR is configured, it can extend the
// /*AFLA*/  * kernel page table mapping, reducing the size of the modules area.
// /*AFLA*/  */
// /*AFLA*/ #if defined(CONFIG_RANDOMIZE_BASE)
// /*AFLA*/ #define KERNEL_IMAGE_SIZE	(1024 * 1024 * 1024)
// /*AFLA*/ #else
// /*AFLA*/ #define KERNEL_IMAGE_SIZE	(512 * 1024 * 1024)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_PAGE_64_DEFS_H */
