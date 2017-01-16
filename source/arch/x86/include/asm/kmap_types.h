// /*AFLA*/ #ifndef _ASM_X86_KMAP_TYPES_H
// /*AFLA*/ #define _ASM_X86_KMAP_TYPES_H
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_X86_32) && defined(CONFIG_DEBUG_HIGHMEM)
// /*AFLA*/ #define  __WITH_KM_FENCE
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/kmap_types.h>
// /*AFLA*/ 
// /*AFLA*/ #undef __WITH_KM_FENCE
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_KMAP_TYPES_H */
