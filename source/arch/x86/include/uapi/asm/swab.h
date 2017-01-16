// /*AFLA*/ #ifndef _ASM_X86_SWAB_H
// /*AFLA*/ #define _ASM_X86_SWAB_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ 
// /*AFLA*/ static inline __attribute_const__ __u32 __arch_swab32(__u32 val)
// /*AFLA*/ {
// /*AFLA*/ 	asm("bswapl %0" : "=r" (val) : "0" (val));
// /*AFLA*/ 	return val;
// /*AFLA*/ }
// /*AFLA*/ #define __arch_swab32 __arch_swab32
// /*AFLA*/ 
// /*AFLA*/ static inline __attribute_const__ __u64 __arch_swab64(__u64 val)
// /*AFLA*/ {
// /*AFLA*/ #ifdef __i386__
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u32 a;
// /*AFLA*/ 			__u32 b;
// /*AFLA*/ 		} s;
// /*AFLA*/ 		__u64 u;
// /*AFLA*/ 	} v;
// /*AFLA*/ 	v.u = val;
// /*AFLA*/ 	asm("bswapl %0 ; bswapl %1 ; xchgl %0,%1"
// /*AFLA*/ 	    : "=r" (v.s.a), "=r" (v.s.b)
// /*AFLA*/ 	    : "0" (v.s.a), "1" (v.s.b));
// /*AFLA*/ 	return v.u;
// /*AFLA*/ #else /* __i386__ */
// /*AFLA*/ 	asm("bswapq %0" : "=r" (val) : "0" (val));
// /*AFLA*/ 	return val;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ #define __arch_swab64 __arch_swab64
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_SWAB_H */
