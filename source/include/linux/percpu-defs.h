// /*AFLA*/ /*
// /*AFLA*/  * linux/percpu-defs.h - basic definitions for percpu areas
// /*AFLA*/  *
// /*AFLA*/  * DO NOT INCLUDE DIRECTLY OUTSIDE PERCPU IMPLEMENTATION PROPER.
// /*AFLA*/  *
// /*AFLA*/  * This file is separate from linux/percpu.h to avoid cyclic inclusion
// /*AFLA*/  * dependency from arch header files.  Only to be included from
// /*AFLA*/  * asm/percpu.h.
// /*AFLA*/  *
// /*AFLA*/  * This file includes macros necessary to declare percpu sections and
// /*AFLA*/  * variables, and definitions of percpu accessors and operations.  It
// /*AFLA*/  * should provide enough percpu features to arch header files even when
// /*AFLA*/  * they can only include asm/percpu.h to avoid cyclic inclusion dependency.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_PERCPU_DEFS_H
// /*AFLA*/ #define _LINUX_PERCPU_DEFS_H
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 
// /*AFLA*/ #ifdef MODULE
// /*AFLA*/ #define PER_CPU_SHARED_ALIGNED_SECTION ""
// /*AFLA*/ #define PER_CPU_ALIGNED_SECTION ""
// /*AFLA*/ #else
// /*AFLA*/ #define PER_CPU_SHARED_ALIGNED_SECTION "..shared_aligned"
// /*AFLA*/ #define PER_CPU_ALIGNED_SECTION "..shared_aligned"
// /*AFLA*/ #endif
// /*AFLA*/ #define PER_CPU_FIRST_SECTION "..first"
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ #define PER_CPU_SHARED_ALIGNED_SECTION ""
// /*AFLA*/ #define PER_CPU_ALIGNED_SECTION "..shared_aligned"
// /*AFLA*/ #define PER_CPU_FIRST_SECTION ""
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Base implementations of per-CPU variable declarations and definitions, where
// /*AFLA*/  * the section in which the variable is to be placed is provided by the
// /*AFLA*/  * 'sec' argument.  This may be used to affect the parameters governing the
// /*AFLA*/  * variable's storage.
// /*AFLA*/  *
// /*AFLA*/  * NOTE!  The sections for the DECLARE and for the DEFINE must match, lest
// /*AFLA*/  * linkage errors occur due the compiler generating the wrong code to access
// /*AFLA*/  * that section.
// /*AFLA*/  */
// /*AFLA*/ #define __PCPU_ATTRS(sec)						\
// /*AFLA*/ 	__percpu __attribute__((section(PER_CPU_BASE_SECTION sec)))	\
// /*AFLA*/ 	PER_CPU_ATTRIBUTES
// /*AFLA*/ 
// /*AFLA*/ #define __PCPU_DUMMY_ATTRS						\
// /*AFLA*/ 	__attribute__((section(".discard"), unused))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * s390 and alpha modules require percpu variables to be defined as
// /*AFLA*/  * weak to force the compiler to generate GOT based external
// /*AFLA*/  * references for them.  This is necessary because percpu sections
// /*AFLA*/  * will be located outside of the usually addressable area.
// /*AFLA*/  *
// /*AFLA*/  * This definition puts the following two extra restrictions when
// /*AFLA*/  * defining percpu variables.
// /*AFLA*/  *
// /*AFLA*/  * 1. The symbol must be globally unique, even the static ones.
// /*AFLA*/  * 2. Static percpu variables cannot be defined inside a function.
// /*AFLA*/  *
// /*AFLA*/  * Archs which need weak percpu definitions should define
// /*AFLA*/  * ARCH_NEEDS_WEAK_PER_CPU in asm/percpu.h when necessary.
// /*AFLA*/  *
// /*AFLA*/  * To ensure that the generic code observes the above two
// /*AFLA*/  * restrictions, if CONFIG_DEBUG_FORCE_WEAK_PER_CPU is set weak
// /*AFLA*/  * definition is used for all cases.
// /*AFLA*/  */
// /*AFLA*/ #if defined(ARCH_NEEDS_WEAK_PER_CPU) || defined(CONFIG_DEBUG_FORCE_WEAK_PER_CPU)
// /*AFLA*/ /*
// /*AFLA*/  * __pcpu_scope_* dummy variable is used to enforce scope.  It
// /*AFLA*/  * receives the static modifier when it's used in front of
// /*AFLA*/  * DEFINE_PER_CPU() and will trigger build failure if
// /*AFLA*/  * DECLARE_PER_CPU() is used for the same variable.
// /*AFLA*/  *
// /*AFLA*/  * __pcpu_unique_* dummy variable is used to enforce symbol uniqueness
// /*AFLA*/  * such that hidden weak symbol collision, which will cause unrelated
// /*AFLA*/  * variables to share the same address, can be detected during build.
// /*AFLA*/  */
// /*AFLA*/ #define DECLARE_PER_CPU_SECTION(type, name, sec)			\
// /*AFLA*/ 	extern __PCPU_DUMMY_ATTRS char __pcpu_scope_##name;		\
// /*AFLA*/ 	extern __PCPU_ATTRS(sec) __typeof__(type) name
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_PER_CPU_SECTION(type, name, sec)				\
// /*AFLA*/ 	__PCPU_DUMMY_ATTRS char __pcpu_scope_##name;			\
// /*AFLA*/ 	extern __PCPU_DUMMY_ATTRS char __pcpu_unique_##name;		\
// /*AFLA*/ 	__PCPU_DUMMY_ATTRS char __pcpu_unique_##name;			\
// /*AFLA*/ 	extern __PCPU_ATTRS(sec) __typeof__(type) name;			\
// /*AFLA*/ 	__PCPU_ATTRS(sec) PER_CPU_DEF_ATTRIBUTES __weak			\
// /*AFLA*/ 	__typeof__(type) name
// /*AFLA*/ #else
// /*AFLA*/ /*
// /*AFLA*/  * Normal declaration and definition macros.
// /*AFLA*/  */
// /*AFLA*/ #define DECLARE_PER_CPU_SECTION(type, name, sec)			\
// /*AFLA*/ 	extern __PCPU_ATTRS(sec) __typeof__(type) name
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_PER_CPU_SECTION(type, name, sec)				\
// /*AFLA*/ 	__PCPU_ATTRS(sec) PER_CPU_DEF_ATTRIBUTES			\
// /*AFLA*/ 	__typeof__(type) name
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Variant on the per-CPU variable declaration/definition theme used for
// /*AFLA*/  * ordinary per-CPU variables.
// /*AFLA*/  */
// /*AFLA*/ #define DECLARE_PER_CPU(type, name)					\
// /*AFLA*/ 	DECLARE_PER_CPU_SECTION(type, name, "")
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_PER_CPU(type, name)					\
// /*AFLA*/ 	DEFINE_PER_CPU_SECTION(type, name, "")
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Declaration/definition used for per-CPU variables that must come first in
// /*AFLA*/  * the set of variables.
// /*AFLA*/  */
// /*AFLA*/ #define DECLARE_PER_CPU_FIRST(type, name)				\
// /*AFLA*/ 	DECLARE_PER_CPU_SECTION(type, name, PER_CPU_FIRST_SECTION)
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_PER_CPU_FIRST(type, name)				\
// /*AFLA*/ 	DEFINE_PER_CPU_SECTION(type, name, PER_CPU_FIRST_SECTION)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Declaration/definition used for per-CPU variables that must be cacheline
// /*AFLA*/  * aligned under SMP conditions so that, whilst a particular instance of the
// /*AFLA*/  * data corresponds to a particular CPU, inefficiencies due to direct access by
// /*AFLA*/  * other CPUs are reduced by preventing the data from unnecessarily spanning
// /*AFLA*/  * cachelines.
// /*AFLA*/  *
// /*AFLA*/  * An example of this would be statistical data, where each CPU's set of data
// /*AFLA*/  * is updated by that CPU alone, but the data from across all CPUs is collated
// /*AFLA*/  * by a CPU processing a read from a proc file.
// /*AFLA*/  */
// /*AFLA*/ #define DECLARE_PER_CPU_SHARED_ALIGNED(type, name)			\
// /*AFLA*/ 	DECLARE_PER_CPU_SECTION(type, name, PER_CPU_SHARED_ALIGNED_SECTION) \
// /*AFLA*/ 	____cacheline_aligned_in_smp
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_PER_CPU_SHARED_ALIGNED(type, name)			\
// /*AFLA*/ 	DEFINE_PER_CPU_SECTION(type, name, PER_CPU_SHARED_ALIGNED_SECTION) \
// /*AFLA*/ 	____cacheline_aligned_in_smp
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_PER_CPU_ALIGNED(type, name)				\
// /*AFLA*/ 	DECLARE_PER_CPU_SECTION(type, name, PER_CPU_ALIGNED_SECTION)	\
// /*AFLA*/ 	____cacheline_aligned
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_PER_CPU_ALIGNED(type, name)				\
// /*AFLA*/ 	DEFINE_PER_CPU_SECTION(type, name, PER_CPU_ALIGNED_SECTION)	\
// /*AFLA*/ 	____cacheline_aligned
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Declaration/definition used for per-CPU variables that must be page aligned.
// /*AFLA*/  */
// /*AFLA*/ #define DECLARE_PER_CPU_PAGE_ALIGNED(type, name)			\
// /*AFLA*/ 	DECLARE_PER_CPU_SECTION(type, name, "..page_aligned")		\
// /*AFLA*/ 	__aligned(PAGE_SIZE)
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_PER_CPU_PAGE_ALIGNED(type, name)				\
// /*AFLA*/ 	DEFINE_PER_CPU_SECTION(type, name, "..page_aligned")		\
// /*AFLA*/ 	__aligned(PAGE_SIZE)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Declaration/definition used for per-CPU variables that must be read mostly.
// /*AFLA*/  */
// /*AFLA*/ #define DECLARE_PER_CPU_READ_MOSTLY(type, name)			\
// /*AFLA*/ 	DECLARE_PER_CPU_SECTION(type, name, "..read_mostly")
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_PER_CPU_READ_MOSTLY(type, name)				\
// /*AFLA*/ 	DEFINE_PER_CPU_SECTION(type, name, "..read_mostly")
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Intermodule exports for per-CPU variables.  sparse forgets about
// /*AFLA*/  * address space across EXPORT_SYMBOL(), change EXPORT_SYMBOL() to
// /*AFLA*/  * noop if __CHECKER__.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __CHECKER__
// /*AFLA*/ #define EXPORT_PER_CPU_SYMBOL(var) EXPORT_SYMBOL(var)
// /*AFLA*/ #define EXPORT_PER_CPU_SYMBOL_GPL(var) EXPORT_SYMBOL_GPL(var)
// /*AFLA*/ #else
// /*AFLA*/ #define EXPORT_PER_CPU_SYMBOL(var)
// /*AFLA*/ #define EXPORT_PER_CPU_SYMBOL_GPL(var)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Accessors and operations.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * __verify_pcpu_ptr() verifies @ptr is a percpu pointer without evaluating
// /*AFLA*/  * @ptr and is invoked once before a percpu area is accessed by all
// /*AFLA*/  * accessors and operations.  This is performed in the generic part of
// /*AFLA*/  * percpu and arch overrides don't need to worry about it; however, if an
// /*AFLA*/  * arch wants to implement an arch-specific percpu accessor or operation,
// /*AFLA*/  * it may use __verify_pcpu_ptr() to verify the parameters.
// /*AFLA*/  *
// /*AFLA*/  * + 0 is required in order to convert the pointer type from a
// /*AFLA*/  * potential array type to a pointer to a single item of the array.
// /*AFLA*/  */
// /*AFLA*/ #define __verify_pcpu_ptr(ptr)						\
// /*AFLA*/ do {									\
// /*AFLA*/ 	const void __percpu *__vpp_verify = (typeof((ptr) + 0))NULL;	\
// /*AFLA*/ 	(void)__vpp_verify;						\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Add an offset to a pointer but keep the pointer as-is.  Use RELOC_HIDE()
// /*AFLA*/  * to prevent the compiler from making incorrect assumptions about the
// /*AFLA*/  * pointer value.  The weird cast keeps both GCC and sparse happy.
// /*AFLA*/  */
// /*AFLA*/ #define SHIFT_PERCPU_PTR(__p, __offset)					\
// /*AFLA*/ 	RELOC_HIDE((typeof(*(__p)) __kernel __force *)(__p), (__offset))
// /*AFLA*/ 
// /*AFLA*/ #define per_cpu_ptr(ptr, cpu)						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	__verify_pcpu_ptr(ptr);						\
// /*AFLA*/ 	SHIFT_PERCPU_PTR((ptr), per_cpu_offset((cpu)));			\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define raw_cpu_ptr(ptr)						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	__verify_pcpu_ptr(ptr);						\
// /*AFLA*/ 	arch_raw_cpu_ptr(ptr);						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_PREEMPT
// /*AFLA*/ #define this_cpu_ptr(ptr)						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	__verify_pcpu_ptr(ptr);						\
// /*AFLA*/ 	SHIFT_PERCPU_PTR(ptr, my_cpu_offset);				\
// /*AFLA*/ })
// /*AFLA*/ #else
// /*AFLA*/ #define this_cpu_ptr(ptr) raw_cpu_ptr(ptr)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #else	/* CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ #define VERIFY_PERCPU_PTR(__p)						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	__verify_pcpu_ptr(__p);						\
// /*AFLA*/ 	(typeof(*(__p)) __kernel __force *)(__p);			\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define per_cpu_ptr(ptr, cpu)	({ (void)(cpu); VERIFY_PERCPU_PTR(ptr); })
// /*AFLA*/ #define raw_cpu_ptr(ptr)	per_cpu_ptr(ptr, 0)
// /*AFLA*/ #define this_cpu_ptr(ptr)	raw_cpu_ptr(ptr)
// /*AFLA*/ 
// /*AFLA*/ #endif	/* CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ #define per_cpu(var, cpu)	(*per_cpu_ptr(&(var), cpu))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Must be an lvalue. Since @var must be a simple identifier,
// /*AFLA*/  * we force a syntax error here if it isn't.
// /*AFLA*/  */
// /*AFLA*/ #define get_cpu_var(var)						\
// /*AFLA*/ (*({									\
// /*AFLA*/ 	preempt_disable();						\
// /*AFLA*/ 	this_cpu_ptr(&var);						\
// /*AFLA*/ }))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The weird & is necessary because sparse considers (void)(var) to be
// /*AFLA*/  * a direct dereference of percpu variable (var).
// /*AFLA*/  */
// /*AFLA*/ #define put_cpu_var(var)						\
// /*AFLA*/ do {									\
// /*AFLA*/ 	(void)&(var);							\
// /*AFLA*/ 	preempt_enable();						\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define get_cpu_ptr(var)						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	preempt_disable();						\
// /*AFLA*/ 	this_cpu_ptr(var);						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define put_cpu_ptr(var)						\
// /*AFLA*/ do {									\
// /*AFLA*/ 	(void)(var);							\
// /*AFLA*/ 	preempt_enable();						\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Branching function to split up a function into a set of functions that
// /*AFLA*/  * are called for different scalar sizes of the objects handled.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ extern void __bad_size_call_parameter(void);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_PREEMPT
// /*AFLA*/ extern void __this_cpu_preempt_check(const char *op);
// /*AFLA*/ #else
// /*AFLA*/ static inline void __this_cpu_preempt_check(const char *op) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __pcpu_size_call_return(stem, variable)				\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(variable) pscr_ret__;					\
// /*AFLA*/ 	__verify_pcpu_ptr(&(variable));					\
// /*AFLA*/ 	switch(sizeof(variable)) {					\
// /*AFLA*/ 	case 1: pscr_ret__ = stem##1(variable); break;			\
// /*AFLA*/ 	case 2: pscr_ret__ = stem##2(variable); break;			\
// /*AFLA*/ 	case 4: pscr_ret__ = stem##4(variable); break;			\
// /*AFLA*/ 	case 8: pscr_ret__ = stem##8(variable); break;			\
// /*AFLA*/ 	default:							\
// /*AFLA*/ 		__bad_size_call_parameter(); break;			\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	pscr_ret__;							\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __pcpu_size_call_return2(stem, variable, ...)			\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(variable) pscr2_ret__;					\
// /*AFLA*/ 	__verify_pcpu_ptr(&(variable));					\
// /*AFLA*/ 	switch(sizeof(variable)) {					\
// /*AFLA*/ 	case 1: pscr2_ret__ = stem##1(variable, __VA_ARGS__); break;	\
// /*AFLA*/ 	case 2: pscr2_ret__ = stem##2(variable, __VA_ARGS__); break;	\
// /*AFLA*/ 	case 4: pscr2_ret__ = stem##4(variable, __VA_ARGS__); break;	\
// /*AFLA*/ 	case 8: pscr2_ret__ = stem##8(variable, __VA_ARGS__); break;	\
// /*AFLA*/ 	default:							\
// /*AFLA*/ 		__bad_size_call_parameter(); break;			\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	pscr2_ret__;							\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Special handling for cmpxchg_double.  cmpxchg_double is passed two
// /*AFLA*/  * percpu variables.  The first has to be aligned to a double word
// /*AFLA*/  * boundary and the second has to follow directly thereafter.
// /*AFLA*/  * We enforce this on all architectures even if they don't support
// /*AFLA*/  * a double cmpxchg instruction, since it's a cheap requirement, and it
// /*AFLA*/  * avoids breaking the requirement for architectures with the instruction.
// /*AFLA*/  */
// /*AFLA*/ #define __pcpu_double_call_return_bool(stem, pcp1, pcp2, ...)		\
// /*AFLA*/ ({									\
// /*AFLA*/ 	bool pdcrb_ret__;						\
// /*AFLA*/ 	__verify_pcpu_ptr(&(pcp1));					\
// /*AFLA*/ 	BUILD_BUG_ON(sizeof(pcp1) != sizeof(pcp2));			\
// /*AFLA*/ 	VM_BUG_ON((unsigned long)(&(pcp1)) % (2 * sizeof(pcp1)));	\
// /*AFLA*/ 	VM_BUG_ON((unsigned long)(&(pcp2)) !=				\
// /*AFLA*/ 		  (unsigned long)(&(pcp1)) + sizeof(pcp1));		\
// /*AFLA*/ 	switch(sizeof(pcp1)) {						\
// /*AFLA*/ 	case 1: pdcrb_ret__ = stem##1(pcp1, pcp2, __VA_ARGS__); break;	\
// /*AFLA*/ 	case 2: pdcrb_ret__ = stem##2(pcp1, pcp2, __VA_ARGS__); break;	\
// /*AFLA*/ 	case 4: pdcrb_ret__ = stem##4(pcp1, pcp2, __VA_ARGS__); break;	\
// /*AFLA*/ 	case 8: pdcrb_ret__ = stem##8(pcp1, pcp2, __VA_ARGS__); break;	\
// /*AFLA*/ 	default:							\
// /*AFLA*/ 		__bad_size_call_parameter(); break;			\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	pdcrb_ret__;							\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __pcpu_size_call(stem, variable, ...)				\
// /*AFLA*/ do {									\
// /*AFLA*/ 	__verify_pcpu_ptr(&(variable));					\
// /*AFLA*/ 	switch(sizeof(variable)) {					\
// /*AFLA*/ 		case 1: stem##1(variable, __VA_ARGS__);break;		\
// /*AFLA*/ 		case 2: stem##2(variable, __VA_ARGS__);break;		\
// /*AFLA*/ 		case 4: stem##4(variable, __VA_ARGS__);break;		\
// /*AFLA*/ 		case 8: stem##8(variable, __VA_ARGS__);break;		\
// /*AFLA*/ 		default: 						\
// /*AFLA*/ 			__bad_size_call_parameter();break;		\
// /*AFLA*/ 	}								\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * this_cpu operations (C) 2008-2013 Christoph Lameter <cl@linux.com>
// /*AFLA*/  *
// /*AFLA*/  * Optimized manipulation for memory allocated through the per cpu
// /*AFLA*/  * allocator or for addresses of per cpu variables.
// /*AFLA*/  *
// /*AFLA*/  * These operation guarantee exclusivity of access for other operations
// /*AFLA*/  * on the *same* processor. The assumption is that per cpu data is only
// /*AFLA*/  * accessed by a single processor instance (the current one).
// /*AFLA*/  *
// /*AFLA*/  * The arch code can provide optimized implementation by defining macros
// /*AFLA*/  * for certain scalar sizes. F.e. provide this_cpu_add_2() to provide per
// /*AFLA*/  * cpu atomic operations for 2 byte sized RMW actions. If arch code does
// /*AFLA*/  * not provide operations for a scalar size then the fallback in the
// /*AFLA*/  * generic code will be used.
// /*AFLA*/  *
// /*AFLA*/  * cmpxchg_double replaces two adjacent scalars at once.  The first two
// /*AFLA*/  * parameters are per cpu variables which have to be of the same size.  A
// /*AFLA*/  * truth value is returned to indicate success or failure (since a double
// /*AFLA*/  * register result is difficult to handle).  There is very limited hardware
// /*AFLA*/  * support for these operations, so only certain sizes may work.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Operations for contexts where we do not want to do any checks for
// /*AFLA*/  * preemptions.  Unless strictly necessary, always use [__]this_cpu_*()
// /*AFLA*/  * instead.
// /*AFLA*/  *
// /*AFLA*/  * If there is no other protection through preempt disable and/or disabling
// /*AFLA*/  * interupts then one of these RMW operations can show unexpected behavior
// /*AFLA*/  * because the execution thread was rescheduled on another processor or an
// /*AFLA*/  * interrupt occurred and the same percpu variable was modified from the
// /*AFLA*/  * interrupt context.
// /*AFLA*/  */
// /*AFLA*/ #define raw_cpu_read(pcp)		__pcpu_size_call_return(raw_cpu_read_, pcp)
// /*AFLA*/ #define raw_cpu_write(pcp, val)		__pcpu_size_call(raw_cpu_write_, pcp, val)
// /*AFLA*/ #define raw_cpu_add(pcp, val)		__pcpu_size_call(raw_cpu_add_, pcp, val)
// /*AFLA*/ #define raw_cpu_and(pcp, val)		__pcpu_size_call(raw_cpu_and_, pcp, val)
// /*AFLA*/ #define raw_cpu_or(pcp, val)		__pcpu_size_call(raw_cpu_or_, pcp, val)
// /*AFLA*/ #define raw_cpu_add_return(pcp, val)	__pcpu_size_call_return2(raw_cpu_add_return_, pcp, val)
// /*AFLA*/ #define raw_cpu_xchg(pcp, nval)		__pcpu_size_call_return2(raw_cpu_xchg_, pcp, nval)
// /*AFLA*/ #define raw_cpu_cmpxchg(pcp, oval, nval) \
// /*AFLA*/ 	__pcpu_size_call_return2(raw_cpu_cmpxchg_, pcp, oval, nval)
// /*AFLA*/ #define raw_cpu_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2) \
// /*AFLA*/ 	__pcpu_double_call_return_bool(raw_cpu_cmpxchg_double_, pcp1, pcp2, oval1, oval2, nval1, nval2)
// /*AFLA*/ 
// /*AFLA*/ #define raw_cpu_sub(pcp, val)		raw_cpu_add(pcp, -(val))
// /*AFLA*/ #define raw_cpu_inc(pcp)		raw_cpu_add(pcp, 1)
// /*AFLA*/ #define raw_cpu_dec(pcp)		raw_cpu_sub(pcp, 1)
// /*AFLA*/ #define raw_cpu_sub_return(pcp, val)	raw_cpu_add_return(pcp, -(typeof(pcp))(val))
// /*AFLA*/ #define raw_cpu_inc_return(pcp)		raw_cpu_add_return(pcp, 1)
// /*AFLA*/ #define raw_cpu_dec_return(pcp)		raw_cpu_add_return(pcp, -1)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Operations for contexts that are safe from preemption/interrupts.  These
// /*AFLA*/  * operations verify that preemption is disabled.
// /*AFLA*/  */
// /*AFLA*/ #define __this_cpu_read(pcp)						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	__this_cpu_preempt_check("read");				\
// /*AFLA*/ 	raw_cpu_read(pcp);						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __this_cpu_write(pcp, val)					\
// /*AFLA*/ ({									\
// /*AFLA*/ 	__this_cpu_preempt_check("write");				\
// /*AFLA*/ 	raw_cpu_write(pcp, val);					\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __this_cpu_add(pcp, val)					\
// /*AFLA*/ ({									\
// /*AFLA*/ 	__this_cpu_preempt_check("add");				\
// /*AFLA*/ 	raw_cpu_add(pcp, val);						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __this_cpu_and(pcp, val)					\
// /*AFLA*/ ({									\
// /*AFLA*/ 	__this_cpu_preempt_check("and");				\
// /*AFLA*/ 	raw_cpu_and(pcp, val);						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __this_cpu_or(pcp, val)						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	__this_cpu_preempt_check("or");					\
// /*AFLA*/ 	raw_cpu_or(pcp, val);						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __this_cpu_add_return(pcp, val)					\
// /*AFLA*/ ({									\
// /*AFLA*/ 	__this_cpu_preempt_check("add_return");				\
// /*AFLA*/ 	raw_cpu_add_return(pcp, val);					\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __this_cpu_xchg(pcp, nval)					\
// /*AFLA*/ ({									\
// /*AFLA*/ 	__this_cpu_preempt_check("xchg");				\
// /*AFLA*/ 	raw_cpu_xchg(pcp, nval);					\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __this_cpu_cmpxchg(pcp, oval, nval)				\
// /*AFLA*/ ({									\
// /*AFLA*/ 	__this_cpu_preempt_check("cmpxchg");				\
// /*AFLA*/ 	raw_cpu_cmpxchg(pcp, oval, nval);				\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __this_cpu_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2) \
// /*AFLA*/ ({	__this_cpu_preempt_check("cmpxchg_double");			\
// /*AFLA*/ 	raw_cpu_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2);	\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __this_cpu_sub(pcp, val)	__this_cpu_add(pcp, -(typeof(pcp))(val))
// /*AFLA*/ #define __this_cpu_inc(pcp)		__this_cpu_add(pcp, 1)
// /*AFLA*/ #define __this_cpu_dec(pcp)		__this_cpu_sub(pcp, 1)
// /*AFLA*/ #define __this_cpu_sub_return(pcp, val)	__this_cpu_add_return(pcp, -(typeof(pcp))(val))
// /*AFLA*/ #define __this_cpu_inc_return(pcp)	__this_cpu_add_return(pcp, 1)
// /*AFLA*/ #define __this_cpu_dec_return(pcp)	__this_cpu_add_return(pcp, -1)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Operations with implied preemption/interrupt protection.  These
// /*AFLA*/  * operations can be used without worrying about preemption or interrupt.
// /*AFLA*/  */
// /*AFLA*/ #define this_cpu_read(pcp)		__pcpu_size_call_return(this_cpu_read_, pcp)
// /*AFLA*/ #define this_cpu_write(pcp, val)	__pcpu_size_call(this_cpu_write_, pcp, val)
// /*AFLA*/ #define this_cpu_add(pcp, val)		__pcpu_size_call(this_cpu_add_, pcp, val)
// /*AFLA*/ #define this_cpu_and(pcp, val)		__pcpu_size_call(this_cpu_and_, pcp, val)
// /*AFLA*/ #define this_cpu_or(pcp, val)		__pcpu_size_call(this_cpu_or_, pcp, val)
// /*AFLA*/ #define this_cpu_add_return(pcp, val)	__pcpu_size_call_return2(this_cpu_add_return_, pcp, val)
// /*AFLA*/ #define this_cpu_xchg(pcp, nval)	__pcpu_size_call_return2(this_cpu_xchg_, pcp, nval)
// /*AFLA*/ #define this_cpu_cmpxchg(pcp, oval, nval) \
// /*AFLA*/ 	__pcpu_size_call_return2(this_cpu_cmpxchg_, pcp, oval, nval)
// /*AFLA*/ #define this_cpu_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2) \
// /*AFLA*/ 	__pcpu_double_call_return_bool(this_cpu_cmpxchg_double_, pcp1, pcp2, oval1, oval2, nval1, nval2)
// /*AFLA*/ 
// /*AFLA*/ #define this_cpu_sub(pcp, val)		this_cpu_add(pcp, -(typeof(pcp))(val))
// /*AFLA*/ #define this_cpu_inc(pcp)		this_cpu_add(pcp, 1)
// /*AFLA*/ #define this_cpu_dec(pcp)		this_cpu_sub(pcp, 1)
// /*AFLA*/ #define this_cpu_sub_return(pcp, val)	this_cpu_add_return(pcp, -(typeof(pcp))(val))
// /*AFLA*/ #define this_cpu_inc_return(pcp)	this_cpu_add_return(pcp, 1)
// /*AFLA*/ #define this_cpu_dec_return(pcp)	this_cpu_add_return(pcp, -1)
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ #endif /* _LINUX_PERCPU_DEFS_H */
