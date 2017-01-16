// /*AFLA*/ #ifndef _ASM_GENERIC_BITOPS_CONST_HWEIGHT_H_
// /*AFLA*/ #define _ASM_GENERIC_BITOPS_CONST_HWEIGHT_H_
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Compile time versions of __arch_hweightN()
// /*AFLA*/  */
// /*AFLA*/ #define __const_hweight8(w)		\
// /*AFLA*/ 	((unsigned int)			\
// /*AFLA*/ 	 ((!!((w) & (1ULL << 0))) +	\
// /*AFLA*/ 	  (!!((w) & (1ULL << 1))) +	\
// /*AFLA*/ 	  (!!((w) & (1ULL << 2))) +	\
// /*AFLA*/ 	  (!!((w) & (1ULL << 3))) +	\
// /*AFLA*/ 	  (!!((w) & (1ULL << 4))) +	\
// /*AFLA*/ 	  (!!((w) & (1ULL << 5))) +	\
// /*AFLA*/ 	  (!!((w) & (1ULL << 6))) +	\
// /*AFLA*/ 	  (!!((w) & (1ULL << 7)))))
// /*AFLA*/ 
// /*AFLA*/ #define __const_hweight16(w) (__const_hweight8(w)  + __const_hweight8((w)  >> 8 ))
// /*AFLA*/ #define __const_hweight32(w) (__const_hweight16(w) + __const_hweight16((w) >> 16))
// /*AFLA*/ #define __const_hweight64(w) (__const_hweight32(w) + __const_hweight32((w) >> 32))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Generic interface.
// /*AFLA*/  */
// /*AFLA*/ #define hweight8(w)  (__builtin_constant_p(w) ? __const_hweight8(w)  : __arch_hweight8(w))
// /*AFLA*/ #define hweight16(w) (__builtin_constant_p(w) ? __const_hweight16(w) : __arch_hweight16(w))
// /*AFLA*/ #define hweight32(w) (__builtin_constant_p(w) ? __const_hweight32(w) : __arch_hweight32(w))
// /*AFLA*/ #define hweight64(w) (__builtin_constant_p(w) ? __const_hweight64(w) : __arch_hweight64(w))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Interface for known constant arguments
// /*AFLA*/  */
// /*AFLA*/ #define HWEIGHT8(w)  (BUILD_BUG_ON_ZERO(!__builtin_constant_p(w)) + __const_hweight8(w))
// /*AFLA*/ #define HWEIGHT16(w) (BUILD_BUG_ON_ZERO(!__builtin_constant_p(w)) + __const_hweight16(w))
// /*AFLA*/ #define HWEIGHT32(w) (BUILD_BUG_ON_ZERO(!__builtin_constant_p(w)) + __const_hweight32(w))
// /*AFLA*/ #define HWEIGHT64(w) (BUILD_BUG_ON_ZERO(!__builtin_constant_p(w)) + __const_hweight64(w))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Type invariant interface to the compile time constant hweight functions.
// /*AFLA*/  */
// /*AFLA*/ #define HWEIGHT(w)   HWEIGHT64((u64)w)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_GENERIC_BITOPS_CONST_HWEIGHT_H_ */
