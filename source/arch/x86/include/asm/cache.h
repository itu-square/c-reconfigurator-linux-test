// /*AFLA*/ #ifndef _ASM_X86_CACHE_H
// /*AFLA*/ #define _ASM_X86_CACHE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ 
// /*AFLA*/ /* L1 cache line size */
// /*AFLA*/ #define L1_CACHE_SHIFT	(CONFIG_X86_L1_CACHE_SHIFT)
// /*AFLA*/ #define L1_CACHE_BYTES	(1 << L1_CACHE_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ #define __read_mostly __attribute__((__section__(".data..read_mostly")))
// /*AFLA*/ 
// /*AFLA*/ #define INTERNODE_CACHE_SHIFT CONFIG_X86_INTERNODE_CACHE_SHIFT
// /*AFLA*/ #define INTERNODE_CACHE_BYTES (1 << INTERNODE_CACHE_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_VSMP
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ #define __cacheline_aligned_in_smp					\
// /*AFLA*/ 	__attribute__((__aligned__(INTERNODE_CACHE_BYTES)))		\
// /*AFLA*/ 	__page_aligned_data
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_CACHE_H */
