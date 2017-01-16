// /*AFLA*/ /*
// /*AFLA*/  * Supervisor Mode Access Prevention support
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2012 Intel Corporation
// /*AFLA*/  * Author: H. Peter Anvin <hpa@linux.intel.com>
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or
// /*AFLA*/  * modify it under the terms of the GNU General Public License
// /*AFLA*/  * as published by the Free Software Foundation; version 2
// /*AFLA*/  * of the License.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _ASM_X86_SMAP_H
// /*AFLA*/ #define _ASM_X86_SMAP_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/stringify.h>
// /*AFLA*/ #include <asm/nops.h>
// /*AFLA*/ #include <asm/cpufeatures.h>
// /*AFLA*/ 
// /*AFLA*/ /* "Raw" instruction opcodes */
// /*AFLA*/ #define __ASM_CLAC	.byte 0x0f,0x01,0xca
// /*AFLA*/ #define __ASM_STAC	.byte 0x0f,0x01,0xcb
// /*AFLA*/ 
// /*AFLA*/ #ifdef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #include <asm/alternative-asm.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_SMAP
// /*AFLA*/ 
// /*AFLA*/ #define ASM_CLAC \
// /*AFLA*/ 	ALTERNATIVE "", __stringify(__ASM_CLAC), X86_FEATURE_SMAP
// /*AFLA*/ 
// /*AFLA*/ #define ASM_STAC \
// /*AFLA*/ 	ALTERNATIVE "", __stringify(__ASM_STAC), X86_FEATURE_SMAP
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_X86_SMAP */
// /*AFLA*/ 
// /*AFLA*/ #define ASM_CLAC
// /*AFLA*/ #define ASM_STAC
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_X86_SMAP */
// /*AFLA*/ 
// /*AFLA*/ #else /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #include <asm/alternative.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_SMAP
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void clac(void)
// /*AFLA*/ {
// /*AFLA*/ 	/* Note: a barrier is implicit in alternative() */
// /*AFLA*/ 	alternative("", __stringify(__ASM_CLAC), X86_FEATURE_SMAP);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void stac(void)
// /*AFLA*/ {
// /*AFLA*/ 	/* Note: a barrier is implicit in alternative() */
// /*AFLA*/ 	alternative("", __stringify(__ASM_STAC), X86_FEATURE_SMAP);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* These macros can be used in asm() statements */
// /*AFLA*/ #define ASM_CLAC \
// /*AFLA*/ 	ALTERNATIVE("", __stringify(__ASM_CLAC), X86_FEATURE_SMAP)
// /*AFLA*/ #define ASM_STAC \
// /*AFLA*/ 	ALTERNATIVE("", __stringify(__ASM_STAC), X86_FEATURE_SMAP)
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_X86_SMAP */
// /*AFLA*/ 
// /*AFLA*/ static inline void clac(void) { }
// /*AFLA*/ static inline void stac(void) { }
// /*AFLA*/ 
// /*AFLA*/ #define ASM_CLAC
// /*AFLA*/ #define ASM_STAC
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_X86_SMAP */
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_SMAP_H */
