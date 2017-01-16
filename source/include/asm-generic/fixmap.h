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
// /*AFLA*/  * Break out common bits to asm-generic by Mark Salter, November 2013
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASM_GENERIC_FIXMAP_H
// /*AFLA*/ #define __ASM_GENERIC_FIXMAP_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ 
// /*AFLA*/ #define __fix_to_virt(x)	(FIXADDR_TOP - ((x) << PAGE_SHIFT))
// /*AFLA*/ #define __virt_to_fix(x)	((FIXADDR_TOP - ((x)&PAGE_MASK)) >> PAGE_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ /*
// /*AFLA*/  * 'index to address' translation. If anyone tries to use the idx
// /*AFLA*/  * directly without translation, we catch the bug with a NULL-deference
// /*AFLA*/  * kernel oops. Illegal ranges of incoming indices are caught too.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline unsigned long fix_to_virt(const unsigned int idx)
// /*AFLA*/ {
// /*AFLA*/ 	BUILD_BUG_ON(idx >= __end_of_fixed_addresses);
// /*AFLA*/ 	return __fix_to_virt(idx);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long virt_to_fix(const unsigned long vaddr)
// /*AFLA*/ {
// /*AFLA*/ 	BUG_ON(vaddr >= FIXADDR_TOP || vaddr < FIXADDR_START);
// /*AFLA*/ 	return __virt_to_fix(vaddr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Provide some reasonable defaults for page flags.
// /*AFLA*/  * Not all architectures use all of these different types and some
// /*AFLA*/  * architectures use different names.
// /*AFLA*/  */
// /*AFLA*/ #ifndef FIXMAP_PAGE_NORMAL
// /*AFLA*/ #define FIXMAP_PAGE_NORMAL PAGE_KERNEL
// /*AFLA*/ #endif
// /*AFLA*/ #if !defined(FIXMAP_PAGE_RO) && defined(PAGE_KERNEL_RO)
// /*AFLA*/ #define FIXMAP_PAGE_RO PAGE_KERNEL_RO
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef FIXMAP_PAGE_NOCACHE
// /*AFLA*/ #define FIXMAP_PAGE_NOCACHE PAGE_KERNEL_NOCACHE
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef FIXMAP_PAGE_IO
// /*AFLA*/ #define FIXMAP_PAGE_IO PAGE_KERNEL_IO
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef FIXMAP_PAGE_CLEAR
// /*AFLA*/ #define FIXMAP_PAGE_CLEAR __pgprot(0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef set_fixmap
// /*AFLA*/ #define set_fixmap(idx, phys)				\
// /*AFLA*/ 	__set_fixmap(idx, phys, FIXMAP_PAGE_NORMAL)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef clear_fixmap
// /*AFLA*/ #define clear_fixmap(idx)			\
// /*AFLA*/ 	__set_fixmap(idx, 0, FIXMAP_PAGE_CLEAR)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Return a pointer with offset calculated */
// /*AFLA*/ #define __set_fixmap_offset(idx, phys, flags)				\
// /*AFLA*/ ({									\
// /*AFLA*/ 	unsigned long ________addr;					\
// /*AFLA*/ 	__set_fixmap(idx, phys, flags);					\
// /*AFLA*/ 	________addr = fix_to_virt(idx) + ((phys) & (PAGE_SIZE - 1));	\
// /*AFLA*/ 	________addr;							\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define set_fixmap_offset(idx, phys) \
// /*AFLA*/ 	__set_fixmap_offset(idx, phys, FIXMAP_PAGE_NORMAL)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Some hardware wants to get fixmapped without caching.
// /*AFLA*/  */
// /*AFLA*/ #define set_fixmap_nocache(idx, phys) \
// /*AFLA*/ 	__set_fixmap(idx, phys, FIXMAP_PAGE_NOCACHE)
// /*AFLA*/ 
// /*AFLA*/ #define set_fixmap_offset_nocache(idx, phys) \
// /*AFLA*/ 	__set_fixmap_offset(idx, phys, FIXMAP_PAGE_NOCACHE)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Some fixmaps are for IO
// /*AFLA*/  */
// /*AFLA*/ #define set_fixmap_io(idx, phys) \
// /*AFLA*/ 	__set_fixmap(idx, phys, FIXMAP_PAGE_IO)
// /*AFLA*/ 
// /*AFLA*/ #define set_fixmap_offset_io(idx, phys) \
// /*AFLA*/ 	__set_fixmap_offset(idx, phys, FIXMAP_PAGE_IO)
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ #endif /* __ASM_GENERIC_FIXMAP_H */
