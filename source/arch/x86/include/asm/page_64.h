// /*AFLA*/ #ifndef _ASM_X86_PAGE_64_H
// /*AFLA*/ #define _ASM_X86_PAGE_64_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/page_64_types.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ /* duplicated to the one in bootmem.h */
// /*AFLA*/ extern unsigned long max_pfn;
// /*AFLA*/ extern unsigned long phys_base;
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long __phys_addr_nodebug(unsigned long x)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long y = x - __START_KERNEL_map;
// /*AFLA*/ 
// /*AFLA*/ 	/* use the carry flag to determine if x was < __START_KERNEL_map */
// /*AFLA*/ 	x = y + ((x > y) ? phys_base : (__START_KERNEL_map - PAGE_OFFSET));
// /*AFLA*/ 
// /*AFLA*/ 	return x;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_VIRTUAL
// /*AFLA*/ extern unsigned long __phys_addr(unsigned long);
// /*AFLA*/ extern unsigned long __phys_addr_symbol(unsigned long);
// /*AFLA*/ #else
// /*AFLA*/ #define __phys_addr(x)		__phys_addr_nodebug(x)
// /*AFLA*/ #define __phys_addr_symbol(x) \
// /*AFLA*/ 	((unsigned long)(x) - __START_KERNEL_map + phys_base)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __phys_reloc_hide(x)	(x)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FLATMEM
// /*AFLA*/ #define pfn_valid(pfn)          ((pfn) < max_pfn)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ void clear_page(void *page);
// /*AFLA*/ void copy_page(void *to, void *from);
// /*AFLA*/ 
// /*AFLA*/ #endif	/* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_VSYSCALL_EMULATION
// /*AFLA*/ # define __HAVE_ARCH_GATE_AREA 1
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_PAGE_64_H */
