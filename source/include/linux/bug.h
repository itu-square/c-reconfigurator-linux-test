// /*AFLA*/ #ifndef _LINUX_BUG_H
// /*AFLA*/ #define _LINUX_BUG_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/bug.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ 
// /*AFLA*/ enum bug_trap_type {
// /*AFLA*/ 	BUG_TRAP_TYPE_NONE = 0,
// /*AFLA*/ 	BUG_TRAP_TYPE_WARN = 1,
// /*AFLA*/ 	BUG_TRAP_TYPE_BUG = 2,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct pt_regs;
// /*AFLA*/ 
// /*AFLA*/ #ifdef __CHECKER__
// /*AFLA*/ #define __BUILD_BUG_ON_NOT_POWER_OF_2(n) (0)
// /*AFLA*/ #define BUILD_BUG_ON_NOT_POWER_OF_2(n) (0)
// /*AFLA*/ #define BUILD_BUG_ON_ZERO(e) (0)
// /*AFLA*/ #define BUILD_BUG_ON_NULL(e) ((void*)0)
// /*AFLA*/ #define BUILD_BUG_ON_INVALID(e) (0)
// /*AFLA*/ #define BUILD_BUG_ON_MSG(cond, msg) (0)
// /*AFLA*/ #define BUILD_BUG_ON(condition) (0)
// /*AFLA*/ #define BUILD_BUG() (0)
// /*AFLA*/ #define MAYBE_BUILD_BUG_ON(cond) (0)
// /*AFLA*/ #else /* __CHECKER__ */
// /*AFLA*/ 
// /*AFLA*/ /* Force a compilation error if a constant expression is not a power of 2 */
// /*AFLA*/ #define __BUILD_BUG_ON_NOT_POWER_OF_2(n)	\
// /*AFLA*/ 	BUILD_BUG_ON(((n) & ((n) - 1)) != 0)
// /*AFLA*/ #define BUILD_BUG_ON_NOT_POWER_OF_2(n)			\
// /*AFLA*/ 	BUILD_BUG_ON((n) == 0 || (((n) & ((n) - 1)) != 0))
// /*AFLA*/ 
// /*AFLA*/ /* Force a compilation error if condition is true, but also produce a
// /*AFLA*/    result (of value 0 and type size_t), so the expression can be used
// /*AFLA*/    e.g. in a structure initializer (or where-ever else comma expressions
// /*AFLA*/    aren't permitted). */
// /*AFLA*/ #define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))
// /*AFLA*/ #define BUILD_BUG_ON_NULL(e) ((void *)sizeof(struct { int:-!!(e); }))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * BUILD_BUG_ON_INVALID() permits the compiler to check the validity of the
// /*AFLA*/  * expression but avoids the generation of any code, even if that expression
// /*AFLA*/  * has side-effects.
// /*AFLA*/  */
// /*AFLA*/ #define BUILD_BUG_ON_INVALID(e) ((void)(sizeof((__force long)(e))))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * BUILD_BUG_ON_MSG - break compile if a condition is true & emit supplied
// /*AFLA*/  *		      error message.
// /*AFLA*/  * @condition: the condition which the compiler should know is false.
// /*AFLA*/  *
// /*AFLA*/  * See BUILD_BUG_ON for description.
// /*AFLA*/  */
// /*AFLA*/ #define BUILD_BUG_ON_MSG(cond, msg) compiletime_assert(!(cond), msg)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * BUILD_BUG_ON - break compile if a condition is true.
// /*AFLA*/  * @condition: the condition which the compiler should know is false.
// /*AFLA*/  *
// /*AFLA*/  * If you have some code which relies on certain constants being equal, or
// /*AFLA*/  * some other compile-time-evaluated condition, you should use BUILD_BUG_ON to
// /*AFLA*/  * detect if someone changes it.
// /*AFLA*/  *
// /*AFLA*/  * The implementation uses gcc's reluctance to create a negative array, but gcc
// /*AFLA*/  * (as of 4.4) only emits that error for obvious cases (e.g. not arguments to
// /*AFLA*/  * inline functions).  Luckily, in 4.3 they added the "error" function
// /*AFLA*/  * attribute just for this type of case.  Thus, we use a negative sized array
// /*AFLA*/  * (should always create an error on gcc versions older than 4.4) and then call
// /*AFLA*/  * an undefined function with the error attribute (should always create an
// /*AFLA*/  * error on gcc 4.3 and later).  If for some reason, neither creates a
// /*AFLA*/  * compile-time error, we'll still have a link-time error, which is harder to
// /*AFLA*/  * track down.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __OPTIMIZE__
// /*AFLA*/ #define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))
// /*AFLA*/ #else
// /*AFLA*/ #define BUILD_BUG_ON(condition) \
// /*AFLA*/ 	BUILD_BUG_ON_MSG(condition, "BUILD_BUG_ON failed: " #condition)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * BUILD_BUG - break compile if used.
// /*AFLA*/  *
// /*AFLA*/  * If you have some code that you expect the compiler to eliminate at
// /*AFLA*/  * build time, you should use BUILD_BUG to detect if it is
// /*AFLA*/  * unexpectedly used.
// /*AFLA*/  */
// /*AFLA*/ #define BUILD_BUG() BUILD_BUG_ON_MSG(1, "BUILD_BUG failed")
// /*AFLA*/ 
// /*AFLA*/ #define MAYBE_BUILD_BUG_ON(cond)			\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		if (__builtin_constant_p((cond)))       \
// /*AFLA*/ 			BUILD_BUG_ON(cond);             \
// /*AFLA*/ 		else                                    \
// /*AFLA*/ 			BUG_ON(cond);                   \
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #endif	/* __CHECKER__ */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_GENERIC_BUG
// /*AFLA*/ #include <asm-generic/bug.h>
// /*AFLA*/ 
// /*AFLA*/ static inline int is_warning_bug(const struct bug_entry *bug)
// /*AFLA*/ {
// /*AFLA*/ 	return bug->flags & BUGFLAG_WARNING;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ const struct bug_entry *find_bug(unsigned long bugaddr);
// /*AFLA*/ 
// /*AFLA*/ enum bug_trap_type report_bug(unsigned long bug_addr, struct pt_regs *regs);
// /*AFLA*/ 
// /*AFLA*/ /* These are defined by the architecture */
// /*AFLA*/ int is_valid_bugaddr(unsigned long addr);
// /*AFLA*/ 
// /*AFLA*/ #else	/* !CONFIG_GENERIC_BUG */
// /*AFLA*/ 
// /*AFLA*/ static inline enum bug_trap_type report_bug(unsigned long bug_addr,
// /*AFLA*/ 					    struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ 	return BUG_TRAP_TYPE_BUG;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif	/* CONFIG_GENERIC_BUG */
// /*AFLA*/ #endif	/* _LINUX_BUG_H */
