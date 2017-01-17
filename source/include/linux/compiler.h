#ifndef __LINUX_COMPILER_H
#define __LINUX_COMPILER_H

#ifndef __ASSEMBLY__

#ifdef __CHECKER__
# define __user		__attribute__((noderef, address_space(1)))
# define __kernel	__attribute__((address_space(0)))
# define __safe		__attribute__((safe))
# define __force	__attribute__((force))
# define __nocast	__attribute__((nocast))
# define __iomem	__attribute__((noderef, address_space(2)))
# define __must_hold(x)	__attribute__((context(x,1,1)))
# define __acquires(x)	__attribute__((context(x,0,1)))
# define __releases(x)	__attribute__((context(x,1,0)))
# define __acquire(x)	__context__(x,1)
# define __release(x)	__context__(x,-1)
# define __cond_lock(x,c)	((c) ? ({ __acquire(x); 1; }) : 0)
# define __percpu	__attribute__((noderef, address_space(3)))
#ifdef CONFIG_SPARSE_RCU_POINTER
# define __rcu		__attribute__((noderef, address_space(4)))
#else /* CONFIG_SPARSE_RCU_POINTER */
# define __rcu
#endif /* CONFIG_SPARSE_RCU_POINTER */
# define __private	__attribute__((noderef))
extern void __chk_user_ptr(const volatile void __user *);
extern void __chk_io_ptr(const volatile void __iomem *);
# define ACCESS_PRIVATE(p, member) (*((typeof((p)->member) __force *) &(p)->member))
#else /* __CHECKER__ */
# define __user
# define __kernel
# define __safe
# define __force
# define __nocast
# define __iomem
# define __chk_user_ptr(x) (void)0
# define __chk_io_ptr(x) (void)0
# define __builtin_warning(x, y...) (1)
# define __must_hold(x)
# define __acquires(x)
# define __releases(x)
# define __acquire(x) (void)0
# define __release(x) (void)0
# define __cond_lock(x,c) (c)
# define __percpu
# define __rcu
# define __private
# define ACCESS_PRIVATE(p, member) ((p)->member)
#endif /* __CHECKER__ */

/* Indirect macros required for expanded argument pasting, eg. __LINE__. */
#define ___PASTE(a,b) a##b
#define __PASTE(a,b) ___PASTE(a,b)

#ifdef __KERNEL__

#ifdef __GNUC__
#include <linux/compiler-gcc.h>
#endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CC_USING_HOTPATCH) && !defined(__CHECKER__)
// /*AFLA*/ #define notrace __attribute__((hotpatch(0,0)))
// /*AFLA*/ #else
// /*AFLA*/ #define notrace __attribute__((no_instrument_function))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Intel compiler defines __GNUC__. So we will overwrite implementations
// /*AFLA*/  * coming from above header files here
// /*AFLA*/  */
// /*AFLA*/ #ifdef __INTEL_COMPILER
// /*AFLA*/ # include <linux/compiler-intel.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Clang compiler defines __GNUC__. So we will overwrite implementations
// /*AFLA*/  * coming from above header files here
// /*AFLA*/  */
// /*AFLA*/ #ifdef __clang__
// /*AFLA*/ #include <linux/compiler-clang.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Generic compiler-dependent macros required for kernel
// /*AFLA*/  * build go below this comment. Actual compiler/compiler version
// /*AFLA*/  * specific implementations come from the above header files
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct ftrace_branch_data {
// /*AFLA*/ 	const char *func;
// /*AFLA*/ 	const char *file;
// /*AFLA*/ 	unsigned line;
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct {
// /*AFLA*/ 			unsigned long correct;
// /*AFLA*/ 			unsigned long incorrect;
// /*AFLA*/ 		};
// /*AFLA*/ 		struct {
// /*AFLA*/ 			unsigned long miss;
// /*AFLA*/ 			unsigned long hit;
// /*AFLA*/ 		};
// /*AFLA*/ 		unsigned long miss_hit[2];
// /*AFLA*/ 	};
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Note: DISABLE_BRANCH_PROFILING can be used by special lowlevel code
// /*AFLA*/  * to disable branch tracing on a per file basis.
// /*AFLA*/  */
// /*AFLA*/ #if defined(CONFIG_TRACE_BRANCH_PROFILING) \
// /*AFLA*/     && !defined(DISABLE_BRANCH_PROFILING) && !defined(__CHECKER__)
// /*AFLA*/ void ftrace_likely_update(struct ftrace_branch_data *f, int val, int expect);
// /*AFLA*/ 
// /*AFLA*/ #define likely_notrace(x)	__builtin_expect(!!(x), 1)
// /*AFLA*/ #define unlikely_notrace(x)	__builtin_expect(!!(x), 0)
// /*AFLA*/ 
// /*AFLA*/ #define __branch_check__(x, expect) ({					\
// /*AFLA*/ 			int ______r;					\
// /*AFLA*/ 			static struct ftrace_branch_data		\
// /*AFLA*/ 				__attribute__((__aligned__(4)))		\
// /*AFLA*/ 				__attribute__((section("_ftrace_annotated_branch"))) \
// /*AFLA*/ 				______f = {				\
// /*AFLA*/ 				.func = __func__,			\
// /*AFLA*/ 				.file = __FILE__,			\
// /*AFLA*/ 				.line = __LINE__,			\
// /*AFLA*/ 			};						\
// /*AFLA*/ 			______r = likely_notrace(x);			\
// /*AFLA*/ 			ftrace_likely_update(&______f, ______r, expect); \
// /*AFLA*/ 			______r;					\
// /*AFLA*/ 		})
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Using __builtin_constant_p(x) to ignore cases where the return
// /*AFLA*/  * value is always the same.  This idea is taken from a similar patch
// /*AFLA*/  * written by Daniel Walker.
// /*AFLA*/  */
// /*AFLA*/ # ifndef likely
// /*AFLA*/ #  define likely(x)	(__builtin_constant_p(x) ? !!(x) : __branch_check__(x, 1))
// /*AFLA*/ # endif
// /*AFLA*/ # ifndef unlikely
// /*AFLA*/ #  define unlikely(x)	(__builtin_constant_p(x) ? !!(x) : __branch_check__(x, 0))
// /*AFLA*/ # endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PROFILE_ALL_BRANCHES
// /*AFLA*/ /*
// /*AFLA*/  * "Define 'is'", Bill Clinton
// /*AFLA*/  * "Define 'if'", Steven Rostedt
// /*AFLA*/  */
// /*AFLA*/ #define if(cond, ...) __trace_if( (cond , ## __VA_ARGS__) )
// /*AFLA*/ #define __trace_if(cond) \
// /*AFLA*/ 	if (__builtin_constant_p(!!(cond)) ? !!(cond) :			\
// /*AFLA*/ 	({								\
// /*AFLA*/ 		int ______r;						\
// /*AFLA*/ 		static struct ftrace_branch_data			\
// /*AFLA*/ 			__attribute__((__aligned__(4)))			\
// /*AFLA*/ 			__attribute__((section("_ftrace_branch")))	\
// /*AFLA*/ 			______f = {					\
// /*AFLA*/ 				.func = __func__,			\
// /*AFLA*/ 				.file = __FILE__,			\
// /*AFLA*/ 				.line = __LINE__,			\
// /*AFLA*/ 			};						\
// /*AFLA*/ 		______r = !!(cond);					\
// /*AFLA*/ 		______f.miss_hit[______r]++;					\
// /*AFLA*/ 		______r;						\
// /*AFLA*/ 	}))
// /*AFLA*/ #endif /* CONFIG_PROFILE_ALL_BRANCHES */
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ # define likely(x)	__builtin_expect(!!(x), 1)
// /*AFLA*/ # define unlikely(x)	__builtin_expect(!!(x), 0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Optimization barrier */
// /*AFLA*/ #ifndef barrier
// /*AFLA*/ # define barrier() __memory_barrier()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef barrier_data
// /*AFLA*/ # define barrier_data(ptr) barrier()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Unreachable code */
// /*AFLA*/ #ifndef unreachable
// /*AFLA*/ # define unreachable() do { } while (1)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * KENTRY - kernel entry point
// /*AFLA*/  * This can be used to annotate symbols (functions or data) that are used
// /*AFLA*/  * without their linker symbol being referenced explicitly. For example,
// /*AFLA*/  * interrupt vector handlers, or functions in the kernel image that are found
// /*AFLA*/  * programatically.
// /*AFLA*/  *
// /*AFLA*/  * Not required for symbols exported with EXPORT_SYMBOL, or initcalls. Those
// /*AFLA*/  * are handled in their own way (with KEEP() in linker scripts).
// /*AFLA*/  *
// /*AFLA*/  * KENTRY can be avoided if the symbols in question are marked as KEEP() in the
// /*AFLA*/  * linker script. For example an architecture could KEEP() its entire
// /*AFLA*/  * boot/exception vector code rather than annotate each function and data.
// /*AFLA*/  */
// /*AFLA*/ #ifndef KENTRY
// /*AFLA*/ # define KENTRY(sym)						\
// /*AFLA*/ 	extern typeof(sym) sym;					\
// /*AFLA*/ 	static const unsigned long __kentry_##sym		\
// /*AFLA*/ 	__used							\
// /*AFLA*/ 	__attribute__((section("___kentry" "+" #sym ), used))	\
// /*AFLA*/ 	= (unsigned long)&sym;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef RELOC_HIDE
// /*AFLA*/ # define RELOC_HIDE(ptr, off)					\
// /*AFLA*/   ({ unsigned long __ptr;					\
// /*AFLA*/      __ptr = (unsigned long) (ptr);				\
// /*AFLA*/     (typeof(ptr)) (__ptr + (off)); })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef OPTIMIZER_HIDE_VAR
// /*AFLA*/ #define OPTIMIZER_HIDE_VAR(var) barrier()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Not-quite-unique ID. */
// /*AFLA*/ #ifndef __UNIQUE_ID
// /*AFLA*/ # define __UNIQUE_ID(prefix) __PASTE(__PASTE(__UNIQUE_ID_, prefix), __LINE__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ #define __READ_ONCE_SIZE						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	switch (size) {							\
// /*AFLA*/ 	case 1: *(__u8 *)res = *(volatile __u8 *)p; break;		\
// /*AFLA*/ 	case 2: *(__u16 *)res = *(volatile __u16 *)p; break;		\
// /*AFLA*/ 	case 4: *(__u32 *)res = *(volatile __u32 *)p; break;		\
// /*AFLA*/ 	case 8: *(__u64 *)res = *(volatile __u64 *)p; break;		\
// /*AFLA*/ 	default:							\
// /*AFLA*/ 		barrier();						\
// /*AFLA*/ 		__builtin_memcpy((void *)res, (const void *)p, size);	\
// /*AFLA*/ 		barrier();						\
// /*AFLA*/ 	}								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ static __always_inline
// /*AFLA*/ void __read_once_size(const volatile void *p, void *res, int size)
// /*AFLA*/ {
// /*AFLA*/ 	__READ_ONCE_SIZE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KASAN
// /*AFLA*/ /*
// /*AFLA*/  * This function is not 'inline' because __no_sanitize_address confilcts
// /*AFLA*/  * with inlining. Attempt to inline it may cause a build failure.
// /*AFLA*/  * 	https://gcc.gnu.org/bugzilla/show_bug.cgi?id=67368
// /*AFLA*/  * '__maybe_unused' allows us to avoid defined-but-not-used warnings.
// /*AFLA*/  */
// /*AFLA*/ static __no_sanitize_address __maybe_unused
// /*AFLA*/ void __read_once_size_nocheck(const volatile void *p, void *res, int size)
// /*AFLA*/ {
// /*AFLA*/ 	__READ_ONCE_SIZE;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static __always_inline
// /*AFLA*/ void __read_once_size_nocheck(const volatile void *p, void *res, int size)
// /*AFLA*/ {
// /*AFLA*/ 	__READ_ONCE_SIZE;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void __write_once_size(volatile void *p, void *res, int size)
// /*AFLA*/ {
// /*AFLA*/ 	switch (size) {
// /*AFLA*/ 	case 1: *(volatile __u8 *)p = *(__u8 *)res; break;
// /*AFLA*/ 	case 2: *(volatile __u16 *)p = *(__u16 *)res; break;
// /*AFLA*/ 	case 4: *(volatile __u32 *)p = *(__u32 *)res; break;
// /*AFLA*/ 	case 8: *(volatile __u64 *)p = *(__u64 *)res; break;
// /*AFLA*/ 	default:
// /*AFLA*/ 		barrier();
// /*AFLA*/ 		__builtin_memcpy((void *)p, (const void *)res, size);
// /*AFLA*/ 		barrier();
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Prevent the compiler from merging or refetching reads or writes. The
// /*AFLA*/  * compiler is also forbidden from reordering successive instances of
// /*AFLA*/  * READ_ONCE, WRITE_ONCE and ACCESS_ONCE (see below), but only when the
// /*AFLA*/  * compiler is aware of some particular ordering.  One way to make the
// /*AFLA*/  * compiler aware of ordering is to put the two invocations of READ_ONCE,
// /*AFLA*/  * WRITE_ONCE or ACCESS_ONCE() in different C statements.
// /*AFLA*/  *
// /*AFLA*/  * In contrast to ACCESS_ONCE these two macros will also work on aggregate
// /*AFLA*/  * data types like structs or unions. If the size of the accessed data
// /*AFLA*/  * type exceeds the word size of the machine (e.g., 32 bits or 64 bits)
// /*AFLA*/  * READ_ONCE() and WRITE_ONCE() will fall back to memcpy(). There's at
// /*AFLA*/  * least two memcpy()s: one for the __builtin_memcpy() and then one for
// /*AFLA*/  * the macro doing the copy of variable - '__u' allocated on the stack.
// /*AFLA*/  *
// /*AFLA*/  * Their two major use cases are: (1) Mediating communication between
// /*AFLA*/  * process-level code and irq/NMI handlers, all running on the same CPU,
// /*AFLA*/  * and (2) Ensuring that the compiler does not  fold, spindle, or otherwise
// /*AFLA*/  * mutilate accesses that either do not require ordering or that interact
// /*AFLA*/  * with an explicit memory barrier or atomic instruction that provides the
// /*AFLA*/  * required ordering.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define __READ_ONCE(x, check)						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	union { typeof(x) __val; char __c[1]; } __u;			\
// /*AFLA*/ 	if (check)							\
// /*AFLA*/ 		__read_once_size(&(x), __u.__c, sizeof(x));		\
// /*AFLA*/ 	else								\
// /*AFLA*/ 		__read_once_size_nocheck(&(x), __u.__c, sizeof(x));	\
// /*AFLA*/ 	__u.__val;							\
// /*AFLA*/ })
// /*AFLA*/ #define READ_ONCE(x) __READ_ONCE(x, 1)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Use READ_ONCE_NOCHECK() instead of READ_ONCE() if you need
// /*AFLA*/  * to hide memory access from KASAN.
// /*AFLA*/  */
// /*AFLA*/ #define READ_ONCE_NOCHECK(x) __READ_ONCE(x, 0)
// /*AFLA*/ 
// /*AFLA*/ #define WRITE_ONCE(x, val) \
// /*AFLA*/ ({							\
// /*AFLA*/ 	union { typeof(x) __val; char __c[1]; } __u =	\
// /*AFLA*/ 		{ .__val = (__force typeof(x)) (val) }; \
// /*AFLA*/ 	__write_once_size(&(x), __u.__c, sizeof(x));	\
// /*AFLA*/ 	__u.__val;					\
// /*AFLA*/ })
// /*AFLA*/ 
#endif /* __KERNEL__ */

#endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #ifdef __KERNEL__
// /*AFLA*/ /*
// /*AFLA*/  * Allow us to mark functions as 'deprecated' and have gcc emit a nice
// /*AFLA*/  * warning for each use, in hopes of speeding the functions removal.
// /*AFLA*/  * Usage is:
// /*AFLA*/  * 		int __deprecated foo(void)
// /*AFLA*/  */
// /*AFLA*/ #ifndef __deprecated
// /*AFLA*/ # define __deprecated		/* unimplemented */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef MODULE
// /*AFLA*/ #define __deprecated_for_modules __deprecated
// /*AFLA*/ #else
// /*AFLA*/ #define __deprecated_for_modules
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __must_check
// /*AFLA*/ #define __must_check
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_ENABLE_MUST_CHECK
// /*AFLA*/ #undef __must_check
// /*AFLA*/ #define __must_check
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef CONFIG_ENABLE_WARN_DEPRECATED
// /*AFLA*/ #undef __deprecated
// /*AFLA*/ #undef __deprecated_for_modules
// /*AFLA*/ #define __deprecated
// /*AFLA*/ #define __deprecated_for_modules
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __malloc
// /*AFLA*/ #define __malloc
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Allow us to avoid 'defined but not used' warnings on functions and data,
// /*AFLA*/  * as well as force them to be emitted to the assembly file.
// /*AFLA*/  *
// /*AFLA*/  * As of gcc 3.4, static functions that are not marked with attribute((used))
// /*AFLA*/  * may be elided from the assembly file.  As of gcc 3.4, static data not so
// /*AFLA*/  * marked will not be elided, but this may change in a future gcc version.
// /*AFLA*/  *
// /*AFLA*/  * NOTE: Because distributions shipped with a backported unit-at-a-time
// /*AFLA*/  * compiler in gcc 3.3, we must define __used to be __attribute__((used))
// /*AFLA*/  * for gcc >=3.3 instead of 3.4.
// /*AFLA*/  *
// /*AFLA*/  * In prior versions of gcc, such functions and data would be emitted, but
// /*AFLA*/  * would be warned about except with attribute((unused)).
// /*AFLA*/  *
// /*AFLA*/  * Mark functions that are referenced only in inline assembly as __used so
// /*AFLA*/  * the code is emitted even though it appears to be unreferenced.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __used
// /*AFLA*/ # define __used			/* unimplemented */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __maybe_unused
// /*AFLA*/ # define __maybe_unused		/* unimplemented */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __always_unused
// /*AFLA*/ # define __always_unused	/* unimplemented */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef noinline
// /*AFLA*/ #define noinline
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Rather then using noinline to prevent stack consumption, use
// /*AFLA*/  * noinline_for_stack instead.  For documentation reasons.
// /*AFLA*/  */
// /*AFLA*/ #define noinline_for_stack noinline
// /*AFLA*/ 
// /*AFLA*/ #ifndef __always_inline
// /*AFLA*/ #define __always_inline inline
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* __KERNEL__ */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * From the GCC manual:
// /*AFLA*/  *
// /*AFLA*/  * Many functions do not examine any values except their arguments,
// /*AFLA*/  * and have no effects except the return value.  Basically this is
// /*AFLA*/  * just slightly more strict class than the `pure' attribute above,
// /*AFLA*/  * since function is not allowed to read global memory.
// /*AFLA*/  *
// /*AFLA*/  * Note that a function that has pointer arguments and examines the
// /*AFLA*/  * data pointed to must _not_ be declared `const'.  Likewise, a
// /*AFLA*/  * function that calls a non-`const' function usually must not be
// /*AFLA*/  * `const'.  It does not make sense for a `const' function to return
// /*AFLA*/  * `void'.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __attribute_const__
// /*AFLA*/ # define __attribute_const__	/* unimplemented */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __latent_entropy
// /*AFLA*/ # define __latent_entropy
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Tell gcc if a function is cold. The compiler will assume any path
// /*AFLA*/  * directly leading to the call is unlikely.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __cold
// /*AFLA*/ #define __cold
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Simple shorthand for a section definition */
// /*AFLA*/ #ifndef __section
// /*AFLA*/ # define __section(S) __attribute__ ((__section__(#S)))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __visible
// /*AFLA*/ #define __visible
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Assume alignment of return value.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __assume_aligned
// /*AFLA*/ #define __assume_aligned(a, ...)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* Are two types/vars the same type (ignoring qualifiers)? */
// /*AFLA*/ #ifndef __same_type
// /*AFLA*/ # define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Is this type a native word size -- useful for atomic operations */
// /*AFLA*/ #ifndef __native_word
// /*AFLA*/ # define __native_word(t) (sizeof(t) == sizeof(char) || sizeof(t) == sizeof(short) || sizeof(t) == sizeof(int) || sizeof(t) == sizeof(long))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Compile time object size, -1 for unknown */
// /*AFLA*/ #ifndef __compiletime_object_size
// /*AFLA*/ # define __compiletime_object_size(obj) -1
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __compiletime_warning
// /*AFLA*/ # define __compiletime_warning(message)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __compiletime_error
// /*AFLA*/ # define __compiletime_error(message)
// /*AFLA*/ /*
// /*AFLA*/  * Sparse complains of variable sized arrays due to the temporary variable in
// /*AFLA*/  * __compiletime_assert. Unfortunately we can't just expand it out to make
// /*AFLA*/  * sparse see a constant array size without breaking compiletime_assert on old
// /*AFLA*/  * versions of GCC (e.g. 4.2.4), so hide the array from sparse altogether.
// /*AFLA*/  */
// /*AFLA*/ # ifndef __CHECKER__
// /*AFLA*/ #  define __compiletime_error_fallback(condition) \
// /*AFLA*/ 	do { ((void)sizeof(char[1 - 2 * condition])); } while (0)
// /*AFLA*/ # endif
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __compiletime_error_fallback
// /*AFLA*/ # define __compiletime_error_fallback(condition) do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __compiletime_assert(condition, msg, prefix, suffix)		\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		bool __cond = !(condition);				\
// /*AFLA*/ 		extern void prefix ## suffix(void) __compiletime_error(msg); \
// /*AFLA*/ 		if (__cond)						\
// /*AFLA*/ 			prefix ## suffix();				\
// /*AFLA*/ 		__compiletime_error_fallback(__cond);			\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #define _compiletime_assert(condition, msg, prefix, suffix) \
// /*AFLA*/ 	__compiletime_assert(condition, msg, prefix, suffix)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * compiletime_assert - break build and emit msg if condition is false
// /*AFLA*/  * @condition: a compile-time constant condition to check
// /*AFLA*/  * @msg:       a message to emit if condition is false
// /*AFLA*/  *
// /*AFLA*/  * In tradition of POSIX assert, this macro will break the build if the
// /*AFLA*/  * supplied condition is *false*, emitting the supplied error message if the
// /*AFLA*/  * compiler has support to do so.
// /*AFLA*/  */
// /*AFLA*/ #define compiletime_assert(condition, msg) \
// /*AFLA*/ 	_compiletime_assert(condition, msg, __compiletime_assert_, __LINE__)
// /*AFLA*/ 
// /*AFLA*/ #define compiletime_assert_atomic_type(t)				\
// /*AFLA*/ 	compiletime_assert(__native_word(t),				\
// /*AFLA*/ 		"Need native word sized stores/loads for atomicity.")
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Prevent the compiler from merging or refetching accesses.  The compiler
// /*AFLA*/  * is also forbidden from reordering successive instances of ACCESS_ONCE(),
// /*AFLA*/  * but only when the compiler is aware of some particular ordering.  One way
// /*AFLA*/  * to make the compiler aware of ordering is to put the two invocations of
// /*AFLA*/  * ACCESS_ONCE() in different C statements.
// /*AFLA*/  *
// /*AFLA*/  * ACCESS_ONCE will only work on scalar types. For union types, ACCESS_ONCE
// /*AFLA*/  * on a union member will work as long as the size of the member matches the
// /*AFLA*/  * size of the union and the size is smaller than word size.
// /*AFLA*/  *
// /*AFLA*/  * The major use cases of ACCESS_ONCE used to be (1) Mediating communication
// /*AFLA*/  * between process-level code and irq/NMI handlers, all running on the same CPU,
// /*AFLA*/  * and (2) Ensuring that the compiler does not  fold, spindle, or otherwise
// /*AFLA*/  * mutilate accesses that either do not require ordering or that interact
// /*AFLA*/  * with an explicit memory barrier or atomic instruction that provides the
// /*AFLA*/  * required ordering.
// /*AFLA*/  *
// /*AFLA*/  * If possible use READ_ONCE()/WRITE_ONCE() instead.
// /*AFLA*/  */
// /*AFLA*/ #define __ACCESS_ONCE(x) ({ \
// /*AFLA*/ 	 __maybe_unused typeof(x) __var = (__force typeof(x)) 0; \
// /*AFLA*/ 	(volatile typeof(x) *)&(x); })
// /*AFLA*/ #define ACCESS_ONCE(x) (*__ACCESS_ONCE(x))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * lockless_dereference() - safely load a pointer for later dereference
// /*AFLA*/  * @p: The pointer to load
// /*AFLA*/  *
// /*AFLA*/  * Similar to rcu_dereference(), but for situations where the pointed-to
// /*AFLA*/  * object's lifetime is managed by something other than RCU.  That
// /*AFLA*/  * "something other" might be reference counting or simple immortality.
// /*AFLA*/  *
// /*AFLA*/  * The seemingly unused variable ___typecheck_p validates that @p is
// /*AFLA*/  * indeed a pointer type by using a pointer to typeof(*p) as the type.
// /*AFLA*/  * Taking a pointer to typeof(*p) again is needed in case p is void *.
// /*AFLA*/  */
// /*AFLA*/ #define lockless_dereference(p) \
// /*AFLA*/ ({ \
// /*AFLA*/ 	typeof(p) _________p1 = READ_ONCE(p); \
// /*AFLA*/ 	typeof(*(p)) *___typecheck_p __maybe_unused; \
// /*AFLA*/ 	smp_read_barrier_depends(); /* Dependency order vs. p above. */ \
// /*AFLA*/ 	(_________p1); \
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /* Ignore/forbid kprobes attach on very low level functions marked by this attribute: */
// /*AFLA*/ #ifdef CONFIG_KPROBES
// /*AFLA*/ # define __kprobes	__attribute__((__section__(".kprobes.text")))
// /*AFLA*/ # define nokprobe_inline	__always_inline
// /*AFLA*/ #else
// /*AFLA*/ # define __kprobes
// /*AFLA*/ # define nokprobe_inline	inline
// /*AFLA*/ #endif
#endif /* __LINUX_COMPILER_H */
