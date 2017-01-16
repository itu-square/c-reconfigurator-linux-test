// /*AFLA*/ #ifndef _ASM_X86_RMWcc
// /*AFLA*/ #define _ASM_X86_RMWcc
// /*AFLA*/ 
// /*AFLA*/ #if !defined(__GCC_ASM_FLAG_OUTPUTS__) && defined(CC_HAVE_ASM_GOTO)
// /*AFLA*/ 
// /*AFLA*/ /* Use asm goto */
// /*AFLA*/ 
// /*AFLA*/ #define __GEN_RMWcc(fullop, var, cc, ...)				\
// /*AFLA*/ do {									\
// /*AFLA*/ 	asm_volatile_goto (fullop "; j" #cc " %l[cc_label]"		\
// /*AFLA*/ 			: : "m" (var), ## __VA_ARGS__ 			\
// /*AFLA*/ 			: "memory" : cc_label);				\
// /*AFLA*/ 	return 0;							\
// /*AFLA*/ cc_label:								\
// /*AFLA*/ 	return 1;							\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define GEN_UNARY_RMWcc(op, var, arg0, cc) 				\
// /*AFLA*/ 	__GEN_RMWcc(op " " arg0, var, cc)
// /*AFLA*/ 
// /*AFLA*/ #define GEN_BINARY_RMWcc(op, var, vcon, val, arg0, cc)			\
// /*AFLA*/ 	__GEN_RMWcc(op " %1, " arg0, var, cc, vcon (val))
// /*AFLA*/ 
// /*AFLA*/ #else /* defined(__GCC_ASM_FLAG_OUTPUTS__) || !defined(CC_HAVE_ASM_GOTO) */
// /*AFLA*/ 
// /*AFLA*/ /* Use flags output or a set instruction */
// /*AFLA*/ 
// /*AFLA*/ #define __GEN_RMWcc(fullop, var, cc, ...)				\
// /*AFLA*/ do {									\
// /*AFLA*/ 	bool c;								\
// /*AFLA*/ 	asm volatile (fullop ";" CC_SET(cc)				\
// /*AFLA*/ 			: "+m" (var), CC_OUT(cc) (c)			\
// /*AFLA*/ 			: __VA_ARGS__ : "memory");			\
// /*AFLA*/ 	return c;							\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define GEN_UNARY_RMWcc(op, var, arg0, cc)				\
// /*AFLA*/ 	__GEN_RMWcc(op " " arg0, var, cc)
// /*AFLA*/ 
// /*AFLA*/ #define GEN_BINARY_RMWcc(op, var, vcon, val, arg0, cc)			\
// /*AFLA*/ 	__GEN_RMWcc(op " %2, " arg0, var, cc, vcon (val))
// /*AFLA*/ 
// /*AFLA*/ #endif /* defined(__GCC_ASM_FLAG_OUTPUTS__) || !defined(CC_HAVE_ASM_GOTO) */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_RMWcc */
