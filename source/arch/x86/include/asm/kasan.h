// /*AFLA*/ #ifndef _ASM_X86_KASAN_H
// /*AFLA*/ #define _ASM_X86_KASAN_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/const.h>
// /*AFLA*/ #define KASAN_SHADOW_OFFSET _AC(CONFIG_KASAN_SHADOW_OFFSET, UL)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Compiler uses shadow offset assuming that addresses start
// /*AFLA*/  * from 0. Kernel addresses don't start from 0, so shadow
// /*AFLA*/  * for kernel really starts from compiler's shadow offset +
// /*AFLA*/  * 'kernel address space start' >> KASAN_SHADOW_SCALE_SHIFT
// /*AFLA*/  */
// /*AFLA*/ #define KASAN_SHADOW_START      (KASAN_SHADOW_OFFSET + \
// /*AFLA*/ 					(0xffff800000000000ULL >> 3))
// /*AFLA*/ /* 47 bits for kernel address -> (47 - 3) bits for shadow */
// /*AFLA*/ #define KASAN_SHADOW_END        (KASAN_SHADOW_START + (1ULL << (47 - 3)))
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KASAN
// /*AFLA*/ void __init kasan_early_init(void);
// /*AFLA*/ void __init kasan_init(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void kasan_early_init(void) { }
// /*AFLA*/ static inline void kasan_init(void) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
