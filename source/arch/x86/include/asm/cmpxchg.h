// /*AFLA*/ #ifndef ASM_X86_CMPXCHG_H
// /*AFLA*/ #define ASM_X86_CMPXCHG_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <asm/cpufeatures.h>
// /*AFLA*/ #include <asm/alternative.h> /* Provides LOCK_PREFIX */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Non-existant functions to indicate usage errors at link time
// /*AFLA*/  * (or compile-time if the compiler implements __compiletime_error().
// /*AFLA*/  */
// /*AFLA*/ extern void __xchg_wrong_size(void)
// /*AFLA*/ 	__compiletime_error("Bad argument size for xchg");
// /*AFLA*/ extern void __cmpxchg_wrong_size(void)
// /*AFLA*/ 	__compiletime_error("Bad argument size for cmpxchg");
// /*AFLA*/ extern void __xadd_wrong_size(void)
// /*AFLA*/ 	__compiletime_error("Bad argument size for xadd");
// /*AFLA*/ extern void __add_wrong_size(void)
// /*AFLA*/ 	__compiletime_error("Bad argument size for add");
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Constants for operation sizes. On 32-bit, the 64-bit size it set to
// /*AFLA*/  * -1 because sizeof will never return -1, thereby making those switch
// /*AFLA*/  * case statements guaranteeed dead code which the compiler will
// /*AFLA*/  * eliminate, and allowing the "missing symbol in the default case" to
// /*AFLA*/  * indicate a usage error.
// /*AFLA*/  */
// /*AFLA*/ #define __X86_CASE_B	1
// /*AFLA*/ #define __X86_CASE_W	2
// /*AFLA*/ #define __X86_CASE_L	4
// /*AFLA*/ #ifdef CONFIG_64BIT
// /*AFLA*/ #define __X86_CASE_Q	8
// /*AFLA*/ #else
// /*AFLA*/ #define	__X86_CASE_Q	-1		/* sizeof will never return -1 */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* 
// /*AFLA*/  * An exchange-type operation, which takes a value and a pointer, and
// /*AFLA*/  * returns the old value.
// /*AFLA*/  */
// /*AFLA*/ #define __xchg_op(ptr, arg, op, lock)					\
// /*AFLA*/ 	({								\
// /*AFLA*/ 	        __typeof__ (*(ptr)) __ret = (arg);			\
// /*AFLA*/ 		switch (sizeof(*(ptr))) {				\
// /*AFLA*/ 		case __X86_CASE_B:					\
// /*AFLA*/ 			asm volatile (lock #op "b %b0, %1\n"		\
// /*AFLA*/ 				      : "+q" (__ret), "+m" (*(ptr))	\
// /*AFLA*/ 				      : : "memory", "cc");		\
// /*AFLA*/ 			break;						\
// /*AFLA*/ 		case __X86_CASE_W:					\
// /*AFLA*/ 			asm volatile (lock #op "w %w0, %1\n"		\
// /*AFLA*/ 				      : "+r" (__ret), "+m" (*(ptr))	\
// /*AFLA*/ 				      : : "memory", "cc");		\
// /*AFLA*/ 			break;						\
// /*AFLA*/ 		case __X86_CASE_L:					\
// /*AFLA*/ 			asm volatile (lock #op "l %0, %1\n"		\
// /*AFLA*/ 				      : "+r" (__ret), "+m" (*(ptr))	\
// /*AFLA*/ 				      : : "memory", "cc");		\
// /*AFLA*/ 			break;						\
// /*AFLA*/ 		case __X86_CASE_Q:					\
// /*AFLA*/ 			asm volatile (lock #op "q %q0, %1\n"		\
// /*AFLA*/ 				      : "+r" (__ret), "+m" (*(ptr))	\
// /*AFLA*/ 				      : : "memory", "cc");		\
// /*AFLA*/ 			break;						\
// /*AFLA*/ 		default:						\
// /*AFLA*/ 			__ ## op ## _wrong_size();			\
// /*AFLA*/ 		}							\
// /*AFLA*/ 		__ret;							\
// /*AFLA*/ 	})
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Note: no "lock" prefix even on SMP: xchg always implies lock anyway.
// /*AFLA*/  * Since this is generally used to protect other memory information, we
// /*AFLA*/  * use "asm volatile" and "memory" clobbers to prevent gcc from moving
// /*AFLA*/  * information around.
// /*AFLA*/  */
// /*AFLA*/ #define xchg(ptr, v)	__xchg_op((ptr), (v), xchg, "")
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Atomic compare and exchange.  Compare OLD with MEM, if identical,
// /*AFLA*/  * store NEW in MEM.  Return the initial value in MEM.  Success is
// /*AFLA*/  * indicated by comparing RETURN with OLD.
// /*AFLA*/  */
// /*AFLA*/ #define __raw_cmpxchg(ptr, old, new, size, lock)			\
// /*AFLA*/ ({									\
// /*AFLA*/ 	__typeof__(*(ptr)) __ret;					\
// /*AFLA*/ 	__typeof__(*(ptr)) __old = (old);				\
// /*AFLA*/ 	__typeof__(*(ptr)) __new = (new);				\
// /*AFLA*/ 	switch (size) {							\
// /*AFLA*/ 	case __X86_CASE_B:						\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		volatile u8 *__ptr = (volatile u8 *)(ptr);		\
// /*AFLA*/ 		asm volatile(lock "cmpxchgb %2,%1"			\
// /*AFLA*/ 			     : "=a" (__ret), "+m" (*__ptr)		\
// /*AFLA*/ 			     : "q" (__new), "0" (__old)			\
// /*AFLA*/ 			     : "memory");				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	case __X86_CASE_W:						\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		volatile u16 *__ptr = (volatile u16 *)(ptr);		\
// /*AFLA*/ 		asm volatile(lock "cmpxchgw %2,%1"			\
// /*AFLA*/ 			     : "=a" (__ret), "+m" (*__ptr)		\
// /*AFLA*/ 			     : "r" (__new), "0" (__old)			\
// /*AFLA*/ 			     : "memory");				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	case __X86_CASE_L:						\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		volatile u32 *__ptr = (volatile u32 *)(ptr);		\
// /*AFLA*/ 		asm volatile(lock "cmpxchgl %2,%1"			\
// /*AFLA*/ 			     : "=a" (__ret), "+m" (*__ptr)		\
// /*AFLA*/ 			     : "r" (__new), "0" (__old)			\
// /*AFLA*/ 			     : "memory");				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	case __X86_CASE_Q:						\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		volatile u64 *__ptr = (volatile u64 *)(ptr);		\
// /*AFLA*/ 		asm volatile(lock "cmpxchgq %2,%1"			\
// /*AFLA*/ 			     : "=a" (__ret), "+m" (*__ptr)		\
// /*AFLA*/ 			     : "r" (__new), "0" (__old)			\
// /*AFLA*/ 			     : "memory");				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	default:							\
// /*AFLA*/ 		__cmpxchg_wrong_size();					\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __cmpxchg(ptr, old, new, size)					\
// /*AFLA*/ 	__raw_cmpxchg((ptr), (old), (new), (size), LOCK_PREFIX)
// /*AFLA*/ 
// /*AFLA*/ #define __sync_cmpxchg(ptr, old, new, size)				\
// /*AFLA*/ 	__raw_cmpxchg((ptr), (old), (new), (size), "lock; ")
// /*AFLA*/ 
// /*AFLA*/ #define __cmpxchg_local(ptr, old, new, size)				\
// /*AFLA*/ 	__raw_cmpxchg((ptr), (old), (new), (size), "")
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ # include <asm/cmpxchg_32.h>
// /*AFLA*/ #else
// /*AFLA*/ # include <asm/cmpxchg_64.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define cmpxchg(ptr, old, new)						\
// /*AFLA*/ 	__cmpxchg(ptr, old, new, sizeof(*(ptr)))
// /*AFLA*/ 
// /*AFLA*/ #define sync_cmpxchg(ptr, old, new)					\
// /*AFLA*/ 	__sync_cmpxchg(ptr, old, new, sizeof(*(ptr)))
// /*AFLA*/ 
// /*AFLA*/ #define cmpxchg_local(ptr, old, new)					\
// /*AFLA*/ 	__cmpxchg_local(ptr, old, new, sizeof(*(ptr)))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * xadd() adds "inc" to "*ptr" and atomically returns the previous
// /*AFLA*/  * value of "*ptr".
// /*AFLA*/  *
// /*AFLA*/  * xadd() is locked when multiple CPUs are online
// /*AFLA*/  */
// /*AFLA*/ #define __xadd(ptr, inc, lock)	__xchg_op((ptr), (inc), xadd, lock)
// /*AFLA*/ #define xadd(ptr, inc)		__xadd((ptr), (inc), LOCK_PREFIX)
// /*AFLA*/ 
// /*AFLA*/ #define __cmpxchg_double(pfx, p1, p2, o1, o2, n1, n2)			\
// /*AFLA*/ ({									\
// /*AFLA*/ 	bool __ret;							\
// /*AFLA*/ 	__typeof__(*(p1)) __old1 = (o1), __new1 = (n1);			\
// /*AFLA*/ 	__typeof__(*(p2)) __old2 = (o2), __new2 = (n2);			\
// /*AFLA*/ 	BUILD_BUG_ON(sizeof(*(p1)) != sizeof(long));			\
// /*AFLA*/ 	BUILD_BUG_ON(sizeof(*(p2)) != sizeof(long));			\
// /*AFLA*/ 	VM_BUG_ON((unsigned long)(p1) % (2 * sizeof(long)));		\
// /*AFLA*/ 	VM_BUG_ON((unsigned long)((p1) + 1) != (unsigned long)(p2));	\
// /*AFLA*/ 	asm volatile(pfx "cmpxchg%c4b %2; sete %0"			\
// /*AFLA*/ 		     : "=a" (__ret), "+d" (__old2),			\
// /*AFLA*/ 		       "+m" (*(p1)), "+m" (*(p2))			\
// /*AFLA*/ 		     : "i" (2 * sizeof(long)), "a" (__old1),		\
// /*AFLA*/ 		       "b" (__new1), "c" (__new2));			\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define cmpxchg_double(p1, p2, o1, o2, n1, n2) \
// /*AFLA*/ 	__cmpxchg_double(LOCK_PREFIX, p1, p2, o1, o2, n1, n2)
// /*AFLA*/ 
// /*AFLA*/ #define cmpxchg_double_local(p1, p2, o1, o2, n1, n2) \
// /*AFLA*/ 	__cmpxchg_double(, p1, p2, o1, o2, n1, n2)
// /*AFLA*/ 
// /*AFLA*/ #endif	/* ASM_X86_CMPXCHG_H */
