// /*AFLA*/ #ifndef _LINUX_EXPORT_H
// /*AFLA*/ #define _LINUX_EXPORT_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Export symbols from the kernel to modules.  Forked from module.h
// /*AFLA*/  * to reduce the amount of pointless cruft we feed to gcc when only
// /*AFLA*/  * exporting a simple symbol or two.
// /*AFLA*/  *
// /*AFLA*/  * Try not to add #includes here.  It slows compilation and makes kernel
// /*AFLA*/  * hackers place grumpy comments in header files.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* Some toolchains use a `_' prefix for all user symbols. */
// /*AFLA*/ #ifdef CONFIG_HAVE_UNDERSCORE_SYMBOL_PREFIX
// /*AFLA*/ #define __VMLINUX_SYMBOL(x) _##x
// /*AFLA*/ #define __VMLINUX_SYMBOL_STR(x) "_" #x
// /*AFLA*/ #else
// /*AFLA*/ #define __VMLINUX_SYMBOL(x) x
// /*AFLA*/ #define __VMLINUX_SYMBOL_STR(x) #x
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Indirect, so macros are expanded before pasting. */
// /*AFLA*/ #define VMLINUX_SYMBOL(x) __VMLINUX_SYMBOL(x)
// /*AFLA*/ #define VMLINUX_SYMBOL_STR(x) __VMLINUX_SYMBOL_STR(x)
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ struct kernel_symbol
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long value;
// /*AFLA*/ 	const char *name;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef MODULE
// /*AFLA*/ extern struct module __this_module;
// /*AFLA*/ #define THIS_MODULE (&__this_module)
// /*AFLA*/ #else
// /*AFLA*/ #define THIS_MODULE ((struct module *)0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MODULES
// /*AFLA*/ 
// /*AFLA*/ #if defined(__KERNEL__) && !defined(__GENKSYMS__)
// /*AFLA*/ #ifdef CONFIG_MODVERSIONS
// /*AFLA*/ /* Mark the CRC weak since genksyms apparently decides not to
// /*AFLA*/  * generate a checksums for some symbols */
// /*AFLA*/ #define __CRC_SYMBOL(sym, sec)						\
// /*AFLA*/ 	extern __visible void *__crc_##sym __attribute__((weak));	\
// /*AFLA*/ 	static const unsigned long __kcrctab_##sym			\
// /*AFLA*/ 	__used								\
// /*AFLA*/ 	__attribute__((section("___kcrctab" sec "+" #sym), used))	\
// /*AFLA*/ 	= (unsigned long) &__crc_##sym;
// /*AFLA*/ #else
// /*AFLA*/ #define __CRC_SYMBOL(sym, sec)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* For every exported symbol, place a struct in the __ksymtab section */
// /*AFLA*/ #define ___EXPORT_SYMBOL(sym, sec)					\
// /*AFLA*/ 	extern typeof(sym) sym;						\
// /*AFLA*/ 	__CRC_SYMBOL(sym, sec)						\
// /*AFLA*/ 	static const char __kstrtab_##sym[]				\
// /*AFLA*/ 	__attribute__((section("__ksymtab_strings"), aligned(1)))	\
// /*AFLA*/ 	= VMLINUX_SYMBOL_STR(sym);					\
// /*AFLA*/ 	static const struct kernel_symbol __ksymtab_##sym		\
// /*AFLA*/ 	__used								\
// /*AFLA*/ 	__attribute__((section("___ksymtab" sec "+" #sym), used))	\
// /*AFLA*/ 	= { (unsigned long)&sym, __kstrtab_##sym }
// /*AFLA*/ 
// /*AFLA*/ #if defined(__KSYM_DEPS__)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * For fine grained build dependencies, we want to tell the build system
// /*AFLA*/  * about each possible exported symbol even if they're not actually exported.
// /*AFLA*/  * We use a string pattern that is unlikely to be valid code that the build
// /*AFLA*/  * system filters out from the preprocessor output (see ksym_dep_filter
// /*AFLA*/  * in scripts/Kbuild.include).
// /*AFLA*/  */
// /*AFLA*/ #define __EXPORT_SYMBOL(sym, sec)	=== __KSYM_##sym ===
// /*AFLA*/ 
// /*AFLA*/ #elif defined(CONFIG_TRIM_UNUSED_KSYMS)
// /*AFLA*/ 
// /*AFLA*/ #include <generated/autoksyms.h>
// /*AFLA*/ 
// /*AFLA*/ #define __EXPORT_SYMBOL(sym, sec)				\
// /*AFLA*/ 	__cond_export_sym(sym, sec, __is_defined(__KSYM_##sym))
// /*AFLA*/ #define __cond_export_sym(sym, sec, conf)			\
// /*AFLA*/ 	___cond_export_sym(sym, sec, conf)
// /*AFLA*/ #define ___cond_export_sym(sym, sec, enabled)			\
// /*AFLA*/ 	__cond_export_sym_##enabled(sym, sec)
// /*AFLA*/ #define __cond_export_sym_1(sym, sec) ___EXPORT_SYMBOL(sym, sec)
// /*AFLA*/ #define __cond_export_sym_0(sym, sec) /* nothing */
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ #define __EXPORT_SYMBOL ___EXPORT_SYMBOL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define EXPORT_SYMBOL(sym)					\
// /*AFLA*/ 	__EXPORT_SYMBOL(sym, "")
// /*AFLA*/ 
// /*AFLA*/ #define EXPORT_SYMBOL_GPL(sym)					\
// /*AFLA*/ 	__EXPORT_SYMBOL(sym, "_gpl")
// /*AFLA*/ 
// /*AFLA*/ #define EXPORT_SYMBOL_GPL_FUTURE(sym)				\
// /*AFLA*/ 	__EXPORT_SYMBOL(sym, "_gpl_future")
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_UNUSED_SYMBOLS
// /*AFLA*/ #define EXPORT_UNUSED_SYMBOL(sym) __EXPORT_SYMBOL(sym, "_unused")
// /*AFLA*/ #define EXPORT_UNUSED_SYMBOL_GPL(sym) __EXPORT_SYMBOL(sym, "_unused_gpl")
// /*AFLA*/ #else
// /*AFLA*/ #define EXPORT_UNUSED_SYMBOL(sym)
// /*AFLA*/ #define EXPORT_UNUSED_SYMBOL_GPL(sym)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif	/* __GENKSYMS__ */
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_MODULES... */
// /*AFLA*/ 
// /*AFLA*/ #define EXPORT_SYMBOL(sym)
// /*AFLA*/ #define EXPORT_SYMBOL_GPL(sym)
// /*AFLA*/ #define EXPORT_SYMBOL_GPL_FUTURE(sym)
// /*AFLA*/ #define EXPORT_UNUSED_SYMBOL(sym)
// /*AFLA*/ #define EXPORT_UNUSED_SYMBOL_GPL(sym)
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_MODULES */
// /*AFLA*/ #endif /* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_EXPORT_H */
