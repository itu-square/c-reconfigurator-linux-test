// /*AFLA*/ #ifndef _ASM_X86_BUG_H
// /*AFLA*/ #define _ASM_X86_BUG_H
// /*AFLA*/ 
// /*AFLA*/ #define HAVE_ARCH_BUG
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_BUGVERBOSE
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ # define __BUG_C0	"2:\t.long 1b, %c0\n"
// /*AFLA*/ #else
// /*AFLA*/ # define __BUG_C0	"2:\t.long 1b - 2b, %c0 - 2b\n"
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define BUG()							\
// /*AFLA*/ do {								\
// /*AFLA*/ 	asm volatile("1:\tud2\n"				\
// /*AFLA*/ 		     ".pushsection __bug_table,\"a\"\n"		\
// /*AFLA*/ 		     __BUG_C0					\
// /*AFLA*/ 		     "\t.word %c1, 0\n"				\
// /*AFLA*/ 		     "\t.org 2b+%c2\n"				\
// /*AFLA*/ 		     ".popsection"				\
// /*AFLA*/ 		     : : "i" (__FILE__), "i" (__LINE__),	\
// /*AFLA*/ 		     "i" (sizeof(struct bug_entry)));		\
// /*AFLA*/ 	unreachable();						\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ #define BUG()							\
// /*AFLA*/ do {								\
// /*AFLA*/ 	asm volatile("ud2");					\
// /*AFLA*/ 	unreachable();						\
// /*AFLA*/ } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/bug.h>
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_BUG_H */
