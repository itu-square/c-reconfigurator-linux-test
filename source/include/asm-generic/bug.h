#ifndef _ASM_GENERIC_BUG_H
#define _ASM_GENERIC_BUG_H

#include <linux/compiler.h>

#ifdef CONFIG_GENERIC_BUG
#define BUGFLAG_WARNING		(1 << 0)
#define BUGFLAG_TAINT(taint)	(BUGFLAG_WARNING | ((taint) << 8))
#define BUG_GET_TAINT(bug)	((bug)->flags >> 8)
#endif

#ifndef __ASSEMBLY__
#include <linux/kernel.h>

#ifdef CONFIG_BUG

#ifdef CONFIG_GENERIC_BUG
struct bug_entry {
#ifndef CONFIG_GENERIC_BUG_RELATIVE_POINTERS
	unsigned long	bug_addr;
#else
	signed int	bug_addr_disp;
#endif
#ifdef CONFIG_DEBUG_BUGVERBOSE
#ifndef CONFIG_GENERIC_BUG_RELATIVE_POINTERS
	const char	*file;
#else
	signed int	file_disp;
#endif
	unsigned short	line;
#endif
	unsigned short	flags;
};
#endif	/* CONFIG_GENERIC_BUG */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Don't use BUG() or BUG_ON() unless there's really no way out; one
// /*AFLA*/  * example might be detecting data structure corruption in the middle
// /*AFLA*/  * of an operation that can't be backed out of.  If the (sub)system
// /*AFLA*/  * can somehow continue operating, perhaps with reduced functionality,
// /*AFLA*/  * it's probably not BUG-worthy.
// /*AFLA*/  *
// /*AFLA*/  * If you're tempted to BUG(), think again:  is completely giving up
// /*AFLA*/  * really the *only* solution?  There are usually better options, where
// /*AFLA*/  * users don't need to reboot ASAP and can mostly shut down cleanly.
// /*AFLA*/  */
// /*AFLA*/ #ifndef HAVE_ARCH_BUG
// /*AFLA*/ #define BUG() do { \
// /*AFLA*/ 	printk("BUG: failure at %s:%d/%s()!\n", __FILE__, __LINE__, __func__); \
// /*AFLA*/ 	panic("BUG!"); \
// /*AFLA*/ } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef HAVE_ARCH_BUG_ON
// /*AFLA*/ #define BUG_ON(condition) do { if (unlikely(condition)) BUG(); } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * WARN(), WARN_ON(), WARN_ON_ONCE, and so on can be used to report
// /*AFLA*/  * significant issues that need prompt attention if they should ever
// /*AFLA*/  * appear at runtime.  Use the versions with printk format strings
// /*AFLA*/  * to provide better diagnostics.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __WARN_TAINT
// /*AFLA*/ extern __printf(3, 4)
// /*AFLA*/ void warn_slowpath_fmt(const char *file, const int line,
// /*AFLA*/ 		       const char *fmt, ...);
// /*AFLA*/ extern __printf(4, 5)
// /*AFLA*/ void warn_slowpath_fmt_taint(const char *file, const int line, unsigned taint,
// /*AFLA*/ 			     const char *fmt, ...);
// /*AFLA*/ extern void warn_slowpath_null(const char *file, const int line);
// /*AFLA*/ #define WANT_WARN_ON_SLOWPATH
// /*AFLA*/ #define __WARN()		warn_slowpath_null(__FILE__, __LINE__)
// /*AFLA*/ #define __WARN_printf(arg...)	warn_slowpath_fmt(__FILE__, __LINE__, arg)
// /*AFLA*/ #define __WARN_printf_taint(taint, arg...)				\
// /*AFLA*/ 	warn_slowpath_fmt_taint(__FILE__, __LINE__, taint, arg)
// /*AFLA*/ #else
// /*AFLA*/ #define __WARN()		__WARN_TAINT(TAINT_WARN)
// /*AFLA*/ #define __WARN_printf(arg...)	do { printk(arg); __WARN(); } while (0)
// /*AFLA*/ #define __WARN_printf_taint(taint, arg...)				\
// /*AFLA*/ 	do { printk(arg); __WARN_TAINT(taint); } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* used internally by panic.c */
// /*AFLA*/ struct warn_args;
// /*AFLA*/ 
// /*AFLA*/ void __warn(const char *file, int line, void *caller, unsigned taint,
// /*AFLA*/ 	    struct pt_regs *regs, struct warn_args *args);
// /*AFLA*/ 
// /*AFLA*/ #ifndef WARN_ON
// /*AFLA*/ #define WARN_ON(condition) ({						\
// /*AFLA*/ 	int __ret_warn_on = !!(condition);				\
// /*AFLA*/ 	if (unlikely(__ret_warn_on))					\
// /*AFLA*/ 		__WARN();						\
// /*AFLA*/ 	unlikely(__ret_warn_on);					\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef WARN
// /*AFLA*/ #define WARN(condition, format...) ({						\
// /*AFLA*/ 	int __ret_warn_on = !!(condition);				\
// /*AFLA*/ 	if (unlikely(__ret_warn_on))					\
// /*AFLA*/ 		__WARN_printf(format);					\
// /*AFLA*/ 	unlikely(__ret_warn_on);					\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define WARN_TAINT(condition, taint, format...) ({			\
// /*AFLA*/ 	int __ret_warn_on = !!(condition);				\
// /*AFLA*/ 	if (unlikely(__ret_warn_on))					\
// /*AFLA*/ 		__WARN_printf_taint(taint, format);			\
// /*AFLA*/ 	unlikely(__ret_warn_on);					\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define WARN_ON_ONCE(condition)	({				\
// /*AFLA*/ 	static bool __section(.data.unlikely) __warned;		\
// /*AFLA*/ 	int __ret_warn_once = !!(condition);			\
// /*AFLA*/ 								\
// /*AFLA*/ 	if (unlikely(__ret_warn_once && !__warned)) {		\
// /*AFLA*/ 		__warned = true;				\
// /*AFLA*/ 		WARN_ON(1);					\
// /*AFLA*/ 	}							\
// /*AFLA*/ 	unlikely(__ret_warn_once);				\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define WARN_ONCE(condition, format...)	({			\
// /*AFLA*/ 	static bool __section(.data.unlikely) __warned;		\
// /*AFLA*/ 	int __ret_warn_once = !!(condition);			\
// /*AFLA*/ 								\
// /*AFLA*/ 	if (unlikely(__ret_warn_once && !__warned)) {		\
// /*AFLA*/ 		__warned = true;				\
// /*AFLA*/ 		WARN(1, format);				\
// /*AFLA*/ 	}							\
// /*AFLA*/ 	unlikely(__ret_warn_once);				\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define WARN_TAINT_ONCE(condition, taint, format...)	({	\
// /*AFLA*/ 	static bool __section(.data.unlikely) __warned;		\
// /*AFLA*/ 	int __ret_warn_once = !!(condition);			\
// /*AFLA*/ 								\
// /*AFLA*/ 	if (unlikely(__ret_warn_once && !__warned)) {		\
// /*AFLA*/ 		__warned = true;				\
// /*AFLA*/ 		WARN_TAINT(1, taint, format);			\
// /*AFLA*/ 	}							\
// /*AFLA*/ 	unlikely(__ret_warn_once);				\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_BUG */
// /*AFLA*/ #ifndef HAVE_ARCH_BUG
// /*AFLA*/ #define BUG() do {} while (1)
#endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef HAVE_ARCH_BUG_ON
// /*AFLA*/ #define BUG_ON(condition) do { if (condition) BUG(); } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef HAVE_ARCH_WARN_ON
// /*AFLA*/ #define WARN_ON(condition) ({						\
// /*AFLA*/ 	int __ret_warn_on = !!(condition);				\
// /*AFLA*/ 	unlikely(__ret_warn_on);					\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef WARN
// /*AFLA*/ #define WARN(condition, format...) ({					\
// /*AFLA*/ 	int __ret_warn_on = !!(condition);				\
// /*AFLA*/ 	no_printk(format);						\
// /*AFLA*/ 	unlikely(__ret_warn_on);					\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define WARN_ON_ONCE(condition) WARN_ON(condition)
// /*AFLA*/ #define WARN_ONCE(condition, format...) WARN(condition, format)
// /*AFLA*/ #define WARN_TAINT(condition, taint, format...) WARN(condition, format)
// /*AFLA*/ #define WARN_TAINT_ONCE(condition, taint, format...) WARN(condition, format)
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * WARN_ON_SMP() is for cases that the warning is either
// /*AFLA*/  * meaningless for !SMP or may even cause failures.
// /*AFLA*/  * This is usually used for cases that we have
// /*AFLA*/  * WARN_ON(!spin_is_locked(&lock)) checks, as spin_is_locked()
// /*AFLA*/  * returns 0 for uniprocessor settings.
// /*AFLA*/  * It can also be used with values that are only defined
// /*AFLA*/  * on SMP:
// /*AFLA*/  *
// /*AFLA*/  * struct foo {
// /*AFLA*/  *  [...]
// /*AFLA*/  * #ifdef CONFIG_SMP
// /*AFLA*/  *	int bar;
// /*AFLA*/  * #endif
// /*AFLA*/  * };
// /*AFLA*/  *
// /*AFLA*/  * void func(struct foo *zoot)
// /*AFLA*/  * {
// /*AFLA*/  *	WARN_ON_SMP(!zoot->bar);
// /*AFLA*/  *
// /*AFLA*/  * For CONFIG_SMP, WARN_ON_SMP() should act the same as WARN_ON(),
// /*AFLA*/  * and should be a nop and return false for uniprocessor.
// /*AFLA*/  *
// /*AFLA*/  * if (WARN_ON_SMP(x)) returns true only when CONFIG_SMP is set
// /*AFLA*/  * and x is true.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ # define WARN_ON_SMP(x)			WARN_ON(x)
// /*AFLA*/ #else
// /*AFLA*/ /*
// /*AFLA*/  * Use of ({0;}) because WARN_ON_SMP(x) may be used either as
// /*AFLA*/  * a stand alone line statement or as a condition in an if ()
// /*AFLA*/  * statement.
// /*AFLA*/  * A simple "0" would cause gcc to give a "statement has no effect"
// /*AFLA*/  * warning.
// /*AFLA*/  */
// /*AFLA*/ # define WARN_ON_SMP(x)			({0;})
// /*AFLA*/ #endif
// /*AFLA*/ 
#endif /* __ASSEMBLY__ */

#endif
