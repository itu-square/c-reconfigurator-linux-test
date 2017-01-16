// /*AFLA*/ #ifndef __LINUX_CACHE_H
// /*AFLA*/ #define __LINUX_CACHE_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/kernel.h>
// /*AFLA*/ #include <asm/cache.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef L1_CACHE_ALIGN
// /*AFLA*/ #define L1_CACHE_ALIGN(x) __ALIGN_KERNEL(x, L1_CACHE_BYTES)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef SMP_CACHE_BYTES
// /*AFLA*/ #define SMP_CACHE_BYTES L1_CACHE_BYTES
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * __read_mostly is used to keep rarely changing variables out of frequently
// /*AFLA*/  * updated cachelines. If an architecture doesn't support it, ignore the
// /*AFLA*/  * hint.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __read_mostly
// /*AFLA*/ #define __read_mostly
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * __ro_after_init is used to mark things that are read-only after init (i.e.
// /*AFLA*/  * after mark_rodata_ro() has been called). These are effectively read-only,
// /*AFLA*/  * but may get written to during init, so can't live in .rodata (via "const").
// /*AFLA*/  */
// /*AFLA*/ #ifndef __ro_after_init
// /*AFLA*/ #define __ro_after_init __attribute__((__section__(".data..ro_after_init")))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef ____cacheline_aligned
// /*AFLA*/ #define ____cacheline_aligned __attribute__((__aligned__(SMP_CACHE_BYTES)))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef ____cacheline_aligned_in_smp
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ #define ____cacheline_aligned_in_smp ____cacheline_aligned
// /*AFLA*/ #else
// /*AFLA*/ #define ____cacheline_aligned_in_smp
// /*AFLA*/ #endif /* CONFIG_SMP */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __cacheline_aligned
// /*AFLA*/ #define __cacheline_aligned					\
// /*AFLA*/   __attribute__((__aligned__(SMP_CACHE_BYTES),			\
// /*AFLA*/ 		 __section__(".data..cacheline_aligned")))
// /*AFLA*/ #endif /* __cacheline_aligned */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __cacheline_aligned_in_smp
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ #define __cacheline_aligned_in_smp __cacheline_aligned
// /*AFLA*/ #else
// /*AFLA*/ #define __cacheline_aligned_in_smp
// /*AFLA*/ #endif /* CONFIG_SMP */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The maximum alignment needed for some critical structures
// /*AFLA*/  * These could be inter-node cacheline sizes/L3 cacheline
// /*AFLA*/  * size etc.  Define this in asm/cache.h for your arch
// /*AFLA*/  */
// /*AFLA*/ #ifndef INTERNODE_CACHE_SHIFT
// /*AFLA*/ #define INTERNODE_CACHE_SHIFT L1_CACHE_SHIFT
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if !defined(____cacheline_internodealigned_in_smp)
// /*AFLA*/ #if defined(CONFIG_SMP)
// /*AFLA*/ #define ____cacheline_internodealigned_in_smp \
// /*AFLA*/ 	__attribute__((__aligned__(1 << (INTERNODE_CACHE_SHIFT))))
// /*AFLA*/ #else
// /*AFLA*/ #define ____cacheline_internodealigned_in_smp
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_ARCH_HAS_CACHE_LINE_SIZE
// /*AFLA*/ #define cache_line_size()	L1_CACHE_BYTES
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_CACHE_H */
