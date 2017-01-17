#ifndef __LINUX_COMPILER_H
#error "Please don't include <linux/compiler-gcc.h> directly, include <linux/compiler.h> instead."
#endif

/*
 * Common definitions for all gcc versions go here.
 */
#define GCC_VERSION (__GNUC__ * 10000		\
		     + __GNUC_MINOR__ * 100	\
		     + __GNUC_PATCHLEVEL__)

/* Optimization barrier */

/* The "volatile" is due to gcc bugs */
#define barrier() __asm__ __volatile__("": : :"memory")
/*
 * This version is i.e. to prevent dead stores elimination on @ptr
 * where gcc and llvm may behave differently when otherwise using
 * normal barrier(): while gcc behavior gets along with a normal
 * barrier(), llvm needs an explicit input variable to be assumed
 * clobbered. The issue is as follows: while the inline asm might
 * access any memory it wants, the compiler could have fit all of
 * @ptr into memory registers instead, and since @ptr never escaped
 * from that, it proofed that the inline asm wasn't touching any of
 * it. This version works well with both compilers, i.e. we're telling
 * the compiler that the inline asm absolutely may see the contents
 * of @ptr. See also: https://llvm.org/bugs/show_bug.cgi?id=15495
 */
#define barrier_data(ptr) __asm__ __volatile__("": :"r"(ptr) :"memory")

/*
 * This macro obfuscates arithmetic on a variable address so that gcc
 * shouldn't recognize the original var, and make assumptions about it.
 *
 * This is needed because the C standard makes it undefined to do
 * pointer arithmetic on "objects" outside their boundaries and the
 * gcc optimizers assume this is the case. In particular they
 * assume such arithmetic does not wrap.
 *
 * A miscompilation has been observed because of this on PPC.
 * To work around it we hide the relationship of the pointer and the object
 * using this macro.
 *
 * Versions of the ppc64 compiler before 4.1 had a bug where use of
 * RELOC_HIDE could trash r30. The bug can be worked around by changing
 * the inline assembly constraint from =g to =r, in this particular
 * case either is valid.
 */
#define RELOC_HIDE(ptr, off)						\
({									\
	unsigned long __ptr;						\
	__asm__ ("" : "=r"(__ptr) : "0"(ptr));				\
	(typeof(ptr)) (__ptr + (off));					\
})

/* Make the optimizer believe the variable can be manipulated arbitrarily. */
#define OPTIMIZER_HIDE_VAR(var)						\
	__asm__ ("" : "=r" (var) : "0" (var))

#ifdef __CHECKER__
#define __must_be_array(a)	0
#else
/* &a[0] degrades to a pointer: a different type from an array */
#define __must_be_array(a)	BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#endif

/*
 * Force always-inline if the user requests it so via the .config,
 * or if gcc is too old:
 */
#if !defined(CONFIG_ARCH_SUPPORTS_OPTIMIZED_INLINING) ||		\
    !defined(CONFIG_OPTIMIZE_INLINING) || (__GNUC__ < 4)
#define inline		inline		__attribute__((always_inline)) notrace
#define __inline__	__inline__	__attribute__((always_inline)) notrace
#define __inline	__inline	__attribute__((always_inline)) notrace
#else
/* A lot of inline functions can cause havoc with function tracing */
#define inline		inline		notrace
#define __inline__	__inline__	notrace
#define __inline	__inline	notrace
#endif

#define __always_inline	inline __attribute__((always_inline))
#define  noinline	__attribute__((noinline))

#define __deprecated	__attribute__((deprecated))
#define __packed	__attribute__((packed))
#define __weak		__attribute__((weak))
#define __alias(symbol)	__attribute__((alias(#symbol)))

/*
 * it doesn't make sense on ARM (currently the only user of __naked)
 * to trace naked functions because then mcount is called without
 * stack and frame pointer being set up and there is no chance to
 * restore the lr register to the value before mcount was called.
 *
 * The asm() bodies of naked functions often depend on standard calling
 * conventions, therefore they must be noinline and noclone.
 *
 * GCC 4.[56] currently fail to enforce this, so we must do so ourselves.
 * See GCC PR44290.
 */
#define __naked		__attribute__((naked)) noinline __noclone notrace

#define __noreturn	__attribute__((noreturn))

/*
 * From the GCC manual:
 *
 * Many functions have no effects except the return value and their
 * return value depends only on the parameters and/or global
 * variables.  Such a function can be subject to common subexpression
 * elimination and loop optimization just as an arithmetic operator
 * would be.
 * [...]
 */
#define __pure			__attribute__((pure))
#define __aligned(x)		__attribute__((aligned(x)))
#define __printf(a, b)		__attribute__((format(printf, a, b)))
#define __scanf(a, b)		__attribute__((format(scanf, a, b)))
#define __attribute_const__	__attribute__((__const__))
#define __maybe_unused		__attribute__((unused))
#define __always_unused		__attribute__((unused))

/* gcc version specific checks */

#if GCC_VERSION < 30200
# error Sorry, your compiler is too old - please upgrade it.
#endif

#if GCC_VERSION < 30300
# define __used			__attribute__((__unused__))
#else
# define __used			__attribute__((__used__))
#endif

// /*AFLA*/ #ifdef CONFIG_GCOV_KERNEL
// /*AFLA*/ # if GCC_VERSION < 30400
// /*AFLA*/ #   error "GCOV profiling support for gcc versions below 3.4 not included"
// /*AFLA*/ # endif /* __GNUC_MINOR__ */
// /*AFLA*/ #endif /* CONFIG_GCOV_KERNEL */
// /*AFLA*/ 
// /*AFLA*/ #if GCC_VERSION >= 30400
// /*AFLA*/ #define __must_check		__attribute__((warn_unused_result))
// /*AFLA*/ #define __malloc		__attribute__((__malloc__))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if GCC_VERSION >= 40000
// /*AFLA*/ 
// /*AFLA*/ /* GCC 4.1.[01] miscompiles __weak */
// /*AFLA*/ #ifdef __KERNEL__
// /*AFLA*/ # if GCC_VERSION >= 40100 &&  GCC_VERSION <= 40101
// /*AFLA*/ #  error Your version of gcc miscompiles the __weak directive
// /*AFLA*/ # endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __used			__attribute__((__used__))
// /*AFLA*/ #define __compiler_offsetof(a, b)					\
// /*AFLA*/ 	__builtin_offsetof(a, b)
// /*AFLA*/ 
// /*AFLA*/ #if GCC_VERSION >= 40100
// /*AFLA*/ # define __compiletime_object_size(obj) __builtin_object_size(obj, 0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if GCC_VERSION >= 40300
// /*AFLA*/ /* Mark functions as cold. gcc will assume any path leading to a call
// /*AFLA*/  * to them will be unlikely.  This means a lot of manual unlikely()s
// /*AFLA*/  * are unnecessary now for any paths leading to the usual suspects
// /*AFLA*/  * like BUG(), printk(), panic() etc. [but let's keep them for now for
// /*AFLA*/  * older compilers]
// /*AFLA*/  *
// /*AFLA*/  * Early snapshots of gcc 4.3 don't support this and we can't detect this
// /*AFLA*/  * in the preprocessor, but we can live with this because they're unreleased.
// /*AFLA*/  * Maketime probing would be overkill here.
// /*AFLA*/  *
// /*AFLA*/  * gcc also has a __attribute__((__hot__)) to move hot functions into
// /*AFLA*/  * a special section, but I don't see any sense in this right now in
// /*AFLA*/  * the kernel context
// /*AFLA*/  */
// /*AFLA*/ #define __cold			__attribute__((__cold__))
// /*AFLA*/ 
// /*AFLA*/ #define __UNIQUE_ID(prefix) __PASTE(__PASTE(__UNIQUE_ID_, prefix), __COUNTER__)
// /*AFLA*/ 
// /*AFLA*/ #ifndef __CHECKER__
// /*AFLA*/ # define __compiletime_warning(message) __attribute__((warning(message)))
// /*AFLA*/ # define __compiletime_error(message) __attribute__((error(message)))
// /*AFLA*/ #endif /* __CHECKER__ */
// /*AFLA*/ #endif /* GCC_VERSION >= 40300 */
// /*AFLA*/ 
// /*AFLA*/ #if GCC_VERSION >= 40500
// /*AFLA*/ 
// /*AFLA*/ #ifndef __CHECKER__
// /*AFLA*/ #ifdef LATENT_ENTROPY_PLUGIN
// /*AFLA*/ #define __latent_entropy __attribute__((latent_entropy))
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Mark a position in code as unreachable.  This can be used to
// /*AFLA*/  * suppress control flow warnings after asm blocks that transfer
// /*AFLA*/  * control elsewhere.
// /*AFLA*/  *
// /*AFLA*/  * Early snapshots of gcc 4.5 don't support this and we can't detect
// /*AFLA*/  * this in the preprocessor, but we can live with this because they're
// /*AFLA*/  * unreleased.  Really, we need to have autoconf for the kernel.
// /*AFLA*/  */
// /*AFLA*/ #define unreachable() __builtin_unreachable()
// /*AFLA*/ 
// /*AFLA*/ /* Mark a function definition as prohibited from being cloned. */
// /*AFLA*/ #define __noclone	__attribute__((__noclone__, __optimize__("no-tracer")))
// /*AFLA*/ 
// /*AFLA*/ #endif /* GCC_VERSION >= 40500 */
// /*AFLA*/ 
// /*AFLA*/ #if GCC_VERSION >= 40600
// /*AFLA*/ /*
// /*AFLA*/  * When used with Link Time Optimization, gcc can optimize away C functions or
// /*AFLA*/  * variables which are referenced only from assembly code.  __visible tells the
// /*AFLA*/  * optimizer that something else uses this function or variable, thus preventing
// /*AFLA*/  * this.
// /*AFLA*/  */
// /*AFLA*/ #define __visible	__attribute__((externally_visible))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #if GCC_VERSION >= 40900 && !defined(__CHECKER__)
// /*AFLA*/ /*
// /*AFLA*/  * __assume_aligned(n, k): Tell the optimizer that the returned
// /*AFLA*/  * pointer can be assumed to be k modulo n. The second argument is
// /*AFLA*/  * optional (default 0), so we use a variadic macro to make the
// /*AFLA*/  * shorthand.
// /*AFLA*/  *
// /*AFLA*/  * Beware: Do not apply this to functions which may return
// /*AFLA*/  * ERR_PTRs. Also, it is probably unwise to apply it to functions
// /*AFLA*/  * returning extra information in the low bits (but in that case the
// /*AFLA*/  * compiler should see some alignment anyway, when the return value is
// /*AFLA*/  * massaged by 'flags = ptr & 3; ptr &= ~3;').
// /*AFLA*/  */
// /*AFLA*/ #define __assume_aligned(a, ...) __attribute__((__assume_aligned__(a, ## __VA_ARGS__)))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * GCC 'asm goto' miscompiles certain code sequences:
// /*AFLA*/  *
// /*AFLA*/  *   http://gcc.gnu.org/bugzilla/show_bug.cgi?id=58670
// /*AFLA*/  *
// /*AFLA*/  * Work it around via a compiler barrier quirk suggested by Jakub Jelinek.
// /*AFLA*/  *
// /*AFLA*/  * (asm goto is automatically volatile - the naming reflects this.)
// /*AFLA*/  */
// /*AFLA*/ #define asm_volatile_goto(x...)	do { asm goto(x); asm (""); } while (0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * sparse (__CHECKER__) pretends to be gcc, but can't do constant
// /*AFLA*/  * folding in __builtin_bswap*() (yet), so don't set these for it.
// /*AFLA*/  */
// /*AFLA*/ #if defined(CONFIG_ARCH_USE_BUILTIN_BSWAP) && !defined(__CHECKER__)
// /*AFLA*/ #if GCC_VERSION >= 40400
// /*AFLA*/ #define __HAVE_BUILTIN_BSWAP32__
// /*AFLA*/ #define __HAVE_BUILTIN_BSWAP64__
// /*AFLA*/ #endif
// /*AFLA*/ #if GCC_VERSION >= 40800
// /*AFLA*/ #define __HAVE_BUILTIN_BSWAP16__
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* CONFIG_ARCH_USE_BUILTIN_BSWAP && !__CHECKER__ */
// /*AFLA*/ 
// /*AFLA*/ #if GCC_VERSION >= 70000
// /*AFLA*/ #define KASAN_ABI_VERSION 5
// /*AFLA*/ #elif GCC_VERSION >= 50000
// /*AFLA*/ #define KASAN_ABI_VERSION 4
// /*AFLA*/ #elif GCC_VERSION >= 40902
// /*AFLA*/ #define KASAN_ABI_VERSION 3
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if GCC_VERSION >= 40902
// /*AFLA*/ /*
// /*AFLA*/  * Tell the compiler that address safety instrumentation (KASAN)
// /*AFLA*/  * should not be applied to that function.
// /*AFLA*/  * Conflicts with inlining: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=67368
// /*AFLA*/  */
// /*AFLA*/ #define __no_sanitize_address __attribute__((no_sanitize_address))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif	/* gcc version >= 40000 specific checks */
// /*AFLA*/ 
// /*AFLA*/ #if !defined(__noclone)
// /*AFLA*/ #define __noclone	/* not needed */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if !defined(__no_sanitize_address)
// /*AFLA*/ #define __no_sanitize_address
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A trick to suppress uninitialized variable warning without generating any
// /*AFLA*/  * code
// /*AFLA*/  */
// /*AFLA*/ #define uninitialized_var(x) x = x
