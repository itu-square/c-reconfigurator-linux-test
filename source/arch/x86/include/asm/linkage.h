// /*AFLA*/ #ifndef _ASM_X86_LINKAGE_H
// /*AFLA*/ #define _ASM_X86_LINKAGE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/stringify.h>
// /*AFLA*/ 
// /*AFLA*/ #undef notrace
// /*AFLA*/ #define notrace __attribute__((no_instrument_function))
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ #define asmlinkage CPP_ASMLINKAGE __attribute__((regparm(0)))
// /*AFLA*/ #endif /* CONFIG_X86_32 */
// /*AFLA*/ 
// /*AFLA*/ #ifdef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #define GLOBAL(name)	\
// /*AFLA*/ 	.globl name;	\
// /*AFLA*/ 	name:
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_X86_64) || defined(CONFIG_X86_ALIGNMENT_16)
// /*AFLA*/ #define __ALIGN		.p2align 4, 0x90
// /*AFLA*/ #define __ALIGN_STR	__stringify(__ALIGN)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_LINKAGE_H */
// /*AFLA*/ 
