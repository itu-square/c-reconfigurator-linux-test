// /*AFLA*/ #ifndef _ASM_X86_CMPXCHG_64_H
// /*AFLA*/ #define _ASM_X86_CMPXCHG_64_H
// /*AFLA*/ 
// /*AFLA*/ static inline void set_64bit(volatile u64 *ptr, u64 val)
// /*AFLA*/ {
// /*AFLA*/ 	*ptr = val;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define cmpxchg64(ptr, o, n)						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
// /*AFLA*/ 	cmpxchg((ptr), (o), (n));					\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define cmpxchg64_local(ptr, o, n)					\
// /*AFLA*/ ({									\
// /*AFLA*/ 	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
// /*AFLA*/ 	cmpxchg_local((ptr), (o), (n));					\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define system_has_cmpxchg_double() boot_cpu_has(X86_FEATURE_CX16)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_CMPXCHG_64_H */
