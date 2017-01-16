// /*AFLA*/ #ifndef _ASM_GENERIC_PERCPU_H_
// /*AFLA*/ #define _ASM_GENERIC_PERCPU_H_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/threads.h>
// /*AFLA*/ #include <linux/percpu-defs.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * per_cpu_offset() is the offset that has to be added to a
// /*AFLA*/  * percpu variable to get to the instance for a certain processor.
// /*AFLA*/  *
// /*AFLA*/  * Most arches use the __per_cpu_offset array for those offsets but
// /*AFLA*/  * some arches have their own ways of determining the offset (x86_64, s390).
// /*AFLA*/  */
// /*AFLA*/ #ifndef __per_cpu_offset
// /*AFLA*/ extern unsigned long __per_cpu_offset[NR_CPUS];
// /*AFLA*/ 
// /*AFLA*/ #define per_cpu_offset(x) (__per_cpu_offset[x])
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Determine the offset for the currently active processor.
// /*AFLA*/  * An arch may define __my_cpu_offset to provide a more effective
// /*AFLA*/  * means of obtaining the offset to the per cpu variables of the
// /*AFLA*/  * current processor.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __my_cpu_offset
// /*AFLA*/ #define __my_cpu_offset per_cpu_offset(raw_smp_processor_id())
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_DEBUG_PREEMPT
// /*AFLA*/ #define my_cpu_offset per_cpu_offset(smp_processor_id())
// /*AFLA*/ #else
// /*AFLA*/ #define my_cpu_offset __my_cpu_offset
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Arch may define arch_raw_cpu_ptr() to provide more efficient address
// /*AFLA*/  * translations for raw_cpu_ptr().
// /*AFLA*/  */
// /*AFLA*/ #ifndef arch_raw_cpu_ptr
// /*AFLA*/ #define arch_raw_cpu_ptr(ptr) SHIFT_PERCPU_PTR(ptr, __my_cpu_offset)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_SETUP_PER_CPU_AREA
// /*AFLA*/ extern void setup_per_cpu_areas(void);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif	/* SMP */
// /*AFLA*/ 
// /*AFLA*/ #ifndef PER_CPU_BASE_SECTION
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ #define PER_CPU_BASE_SECTION ".data..percpu"
// /*AFLA*/ #else
// /*AFLA*/ #define PER_CPU_BASE_SECTION ".data"
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef PER_CPU_ATTRIBUTES
// /*AFLA*/ #define PER_CPU_ATTRIBUTES
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef PER_CPU_DEF_ATTRIBUTES
// /*AFLA*/ #define PER_CPU_DEF_ATTRIBUTES
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define raw_cpu_generic_read(pcp)					\
// /*AFLA*/ ({									\
// /*AFLA*/ 	*raw_cpu_ptr(&(pcp));						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define raw_cpu_generic_to_op(pcp, val, op)				\
// /*AFLA*/ do {									\
// /*AFLA*/ 	*raw_cpu_ptr(&(pcp)) op val;					\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define raw_cpu_generic_add_return(pcp, val)				\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(&(pcp)) __p = raw_cpu_ptr(&(pcp));			\
// /*AFLA*/ 									\
// /*AFLA*/ 	*__p += val;							\
// /*AFLA*/ 	*__p;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define raw_cpu_generic_xchg(pcp, nval)					\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(&(pcp)) __p = raw_cpu_ptr(&(pcp));			\
// /*AFLA*/ 	typeof(pcp) __ret;						\
// /*AFLA*/ 	__ret = *__p;							\
// /*AFLA*/ 	*__p = nval;							\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define raw_cpu_generic_cmpxchg(pcp, oval, nval)			\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(&(pcp)) __p = raw_cpu_ptr(&(pcp));			\
// /*AFLA*/ 	typeof(pcp) __ret;						\
// /*AFLA*/ 	__ret = *__p;							\
// /*AFLA*/ 	if (__ret == (oval))						\
// /*AFLA*/ 		*__p = nval;						\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define raw_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2) \
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(&(pcp1)) __p1 = raw_cpu_ptr(&(pcp1));			\
// /*AFLA*/ 	typeof(&(pcp2)) __p2 = raw_cpu_ptr(&(pcp2));			\
// /*AFLA*/ 	int __ret = 0;							\
// /*AFLA*/ 	if (*__p1 == (oval1) && *__p2  == (oval2)) {			\
// /*AFLA*/ 		*__p1 = nval1;						\
// /*AFLA*/ 		*__p2 = nval2;						\
// /*AFLA*/ 		__ret = 1;						\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	(__ret);							\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define this_cpu_generic_read(pcp)					\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(pcp) __ret;						\
// /*AFLA*/ 	preempt_disable_notrace();					\
// /*AFLA*/ 	__ret = raw_cpu_generic_read(pcp);				\
// /*AFLA*/ 	preempt_enable_notrace();					\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define this_cpu_generic_to_op(pcp, val, op)				\
// /*AFLA*/ do {									\
// /*AFLA*/ 	unsigned long __flags;						\
// /*AFLA*/ 	raw_local_irq_save(__flags);					\
// /*AFLA*/ 	raw_cpu_generic_to_op(pcp, val, op);				\
// /*AFLA*/ 	raw_local_irq_restore(__flags);					\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define this_cpu_generic_add_return(pcp, val)				\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(pcp) __ret;						\
// /*AFLA*/ 	unsigned long __flags;						\
// /*AFLA*/ 	raw_local_irq_save(__flags);					\
// /*AFLA*/ 	__ret = raw_cpu_generic_add_return(pcp, val);			\
// /*AFLA*/ 	raw_local_irq_restore(__flags);					\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define this_cpu_generic_xchg(pcp, nval)				\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(pcp) __ret;						\
// /*AFLA*/ 	unsigned long __flags;						\
// /*AFLA*/ 	raw_local_irq_save(__flags);					\
// /*AFLA*/ 	__ret = raw_cpu_generic_xchg(pcp, nval);			\
// /*AFLA*/ 	raw_local_irq_restore(__flags);					\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define this_cpu_generic_cmpxchg(pcp, oval, nval)			\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(pcp) __ret;						\
// /*AFLA*/ 	unsigned long __flags;						\
// /*AFLA*/ 	raw_local_irq_save(__flags);					\
// /*AFLA*/ 	__ret = raw_cpu_generic_cmpxchg(pcp, oval, nval);		\
// /*AFLA*/ 	raw_local_irq_restore(__flags);					\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define this_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)	\
// /*AFLA*/ ({									\
// /*AFLA*/ 	int __ret;							\
// /*AFLA*/ 	unsigned long __flags;						\
// /*AFLA*/ 	raw_local_irq_save(__flags);					\
// /*AFLA*/ 	__ret = raw_cpu_generic_cmpxchg_double(pcp1, pcp2,		\
// /*AFLA*/ 			oval1, oval2, nval1, nval2);			\
// /*AFLA*/ 	raw_local_irq_restore(__flags);					\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #ifndef raw_cpu_read_1
// /*AFLA*/ #define raw_cpu_read_1(pcp)		raw_cpu_generic_read(pcp)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_read_2
// /*AFLA*/ #define raw_cpu_read_2(pcp)		raw_cpu_generic_read(pcp)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_read_4
// /*AFLA*/ #define raw_cpu_read_4(pcp)		raw_cpu_generic_read(pcp)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_read_8
// /*AFLA*/ #define raw_cpu_read_8(pcp)		raw_cpu_generic_read(pcp)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef raw_cpu_write_1
// /*AFLA*/ #define raw_cpu_write_1(pcp, val)	raw_cpu_generic_to_op(pcp, val, =)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_write_2
// /*AFLA*/ #define raw_cpu_write_2(pcp, val)	raw_cpu_generic_to_op(pcp, val, =)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_write_4
// /*AFLA*/ #define raw_cpu_write_4(pcp, val)	raw_cpu_generic_to_op(pcp, val, =)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_write_8
// /*AFLA*/ #define raw_cpu_write_8(pcp, val)	raw_cpu_generic_to_op(pcp, val, =)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef raw_cpu_add_1
// /*AFLA*/ #define raw_cpu_add_1(pcp, val)		raw_cpu_generic_to_op(pcp, val, +=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_add_2
// /*AFLA*/ #define raw_cpu_add_2(pcp, val)		raw_cpu_generic_to_op(pcp, val, +=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_add_4
// /*AFLA*/ #define raw_cpu_add_4(pcp, val)		raw_cpu_generic_to_op(pcp, val, +=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_add_8
// /*AFLA*/ #define raw_cpu_add_8(pcp, val)		raw_cpu_generic_to_op(pcp, val, +=)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef raw_cpu_and_1
// /*AFLA*/ #define raw_cpu_and_1(pcp, val)		raw_cpu_generic_to_op(pcp, val, &=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_and_2
// /*AFLA*/ #define raw_cpu_and_2(pcp, val)		raw_cpu_generic_to_op(pcp, val, &=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_and_4
// /*AFLA*/ #define raw_cpu_and_4(pcp, val)		raw_cpu_generic_to_op(pcp, val, &=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_and_8
// /*AFLA*/ #define raw_cpu_and_8(pcp, val)		raw_cpu_generic_to_op(pcp, val, &=)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef raw_cpu_or_1
// /*AFLA*/ #define raw_cpu_or_1(pcp, val)		raw_cpu_generic_to_op(pcp, val, |=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_or_2
// /*AFLA*/ #define raw_cpu_or_2(pcp, val)		raw_cpu_generic_to_op(pcp, val, |=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_or_4
// /*AFLA*/ #define raw_cpu_or_4(pcp, val)		raw_cpu_generic_to_op(pcp, val, |=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_or_8
// /*AFLA*/ #define raw_cpu_or_8(pcp, val)		raw_cpu_generic_to_op(pcp, val, |=)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef raw_cpu_add_return_1
// /*AFLA*/ #define raw_cpu_add_return_1(pcp, val)	raw_cpu_generic_add_return(pcp, val)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_add_return_2
// /*AFLA*/ #define raw_cpu_add_return_2(pcp, val)	raw_cpu_generic_add_return(pcp, val)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_add_return_4
// /*AFLA*/ #define raw_cpu_add_return_4(pcp, val)	raw_cpu_generic_add_return(pcp, val)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_add_return_8
// /*AFLA*/ #define raw_cpu_add_return_8(pcp, val)	raw_cpu_generic_add_return(pcp, val)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef raw_cpu_xchg_1
// /*AFLA*/ #define raw_cpu_xchg_1(pcp, nval)	raw_cpu_generic_xchg(pcp, nval)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_xchg_2
// /*AFLA*/ #define raw_cpu_xchg_2(pcp, nval)	raw_cpu_generic_xchg(pcp, nval)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_xchg_4
// /*AFLA*/ #define raw_cpu_xchg_4(pcp, nval)	raw_cpu_generic_xchg(pcp, nval)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_xchg_8
// /*AFLA*/ #define raw_cpu_xchg_8(pcp, nval)	raw_cpu_generic_xchg(pcp, nval)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef raw_cpu_cmpxchg_1
// /*AFLA*/ #define raw_cpu_cmpxchg_1(pcp, oval, nval) \
// /*AFLA*/ 	raw_cpu_generic_cmpxchg(pcp, oval, nval)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_cmpxchg_2
// /*AFLA*/ #define raw_cpu_cmpxchg_2(pcp, oval, nval) \
// /*AFLA*/ 	raw_cpu_generic_cmpxchg(pcp, oval, nval)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_cmpxchg_4
// /*AFLA*/ #define raw_cpu_cmpxchg_4(pcp, oval, nval) \
// /*AFLA*/ 	raw_cpu_generic_cmpxchg(pcp, oval, nval)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_cmpxchg_8
// /*AFLA*/ #define raw_cpu_cmpxchg_8(pcp, oval, nval) \
// /*AFLA*/ 	raw_cpu_generic_cmpxchg(pcp, oval, nval)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef raw_cpu_cmpxchg_double_1
// /*AFLA*/ #define raw_cpu_cmpxchg_double_1(pcp1, pcp2, oval1, oval2, nval1, nval2) \
// /*AFLA*/ 	raw_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_cmpxchg_double_2
// /*AFLA*/ #define raw_cpu_cmpxchg_double_2(pcp1, pcp2, oval1, oval2, nval1, nval2) \
// /*AFLA*/ 	raw_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_cmpxchg_double_4
// /*AFLA*/ #define raw_cpu_cmpxchg_double_4(pcp1, pcp2, oval1, oval2, nval1, nval2) \
// /*AFLA*/ 	raw_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef raw_cpu_cmpxchg_double_8
// /*AFLA*/ #define raw_cpu_cmpxchg_double_8(pcp1, pcp2, oval1, oval2, nval1, nval2) \
// /*AFLA*/ 	raw_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef this_cpu_read_1
// /*AFLA*/ #define this_cpu_read_1(pcp)		this_cpu_generic_read(pcp)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_read_2
// /*AFLA*/ #define this_cpu_read_2(pcp)		this_cpu_generic_read(pcp)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_read_4
// /*AFLA*/ #define this_cpu_read_4(pcp)		this_cpu_generic_read(pcp)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_read_8
// /*AFLA*/ #define this_cpu_read_8(pcp)		this_cpu_generic_read(pcp)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef this_cpu_write_1
// /*AFLA*/ #define this_cpu_write_1(pcp, val)	this_cpu_generic_to_op(pcp, val, =)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_write_2
// /*AFLA*/ #define this_cpu_write_2(pcp, val)	this_cpu_generic_to_op(pcp, val, =)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_write_4
// /*AFLA*/ #define this_cpu_write_4(pcp, val)	this_cpu_generic_to_op(pcp, val, =)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_write_8
// /*AFLA*/ #define this_cpu_write_8(pcp, val)	this_cpu_generic_to_op(pcp, val, =)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef this_cpu_add_1
// /*AFLA*/ #define this_cpu_add_1(pcp, val)	this_cpu_generic_to_op(pcp, val, +=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_add_2
// /*AFLA*/ #define this_cpu_add_2(pcp, val)	this_cpu_generic_to_op(pcp, val, +=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_add_4
// /*AFLA*/ #define this_cpu_add_4(pcp, val)	this_cpu_generic_to_op(pcp, val, +=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_add_8
// /*AFLA*/ #define this_cpu_add_8(pcp, val)	this_cpu_generic_to_op(pcp, val, +=)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef this_cpu_and_1
// /*AFLA*/ #define this_cpu_and_1(pcp, val)	this_cpu_generic_to_op(pcp, val, &=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_and_2
// /*AFLA*/ #define this_cpu_and_2(pcp, val)	this_cpu_generic_to_op(pcp, val, &=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_and_4
// /*AFLA*/ #define this_cpu_and_4(pcp, val)	this_cpu_generic_to_op(pcp, val, &=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_and_8
// /*AFLA*/ #define this_cpu_and_8(pcp, val)	this_cpu_generic_to_op(pcp, val, &=)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef this_cpu_or_1
// /*AFLA*/ #define this_cpu_or_1(pcp, val)		this_cpu_generic_to_op(pcp, val, |=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_or_2
// /*AFLA*/ #define this_cpu_or_2(pcp, val)		this_cpu_generic_to_op(pcp, val, |=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_or_4
// /*AFLA*/ #define this_cpu_or_4(pcp, val)		this_cpu_generic_to_op(pcp, val, |=)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_or_8
// /*AFLA*/ #define this_cpu_or_8(pcp, val)		this_cpu_generic_to_op(pcp, val, |=)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef this_cpu_add_return_1
// /*AFLA*/ #define this_cpu_add_return_1(pcp, val)	this_cpu_generic_add_return(pcp, val)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_add_return_2
// /*AFLA*/ #define this_cpu_add_return_2(pcp, val)	this_cpu_generic_add_return(pcp, val)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_add_return_4
// /*AFLA*/ #define this_cpu_add_return_4(pcp, val)	this_cpu_generic_add_return(pcp, val)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_add_return_8
// /*AFLA*/ #define this_cpu_add_return_8(pcp, val)	this_cpu_generic_add_return(pcp, val)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef this_cpu_xchg_1
// /*AFLA*/ #define this_cpu_xchg_1(pcp, nval)	this_cpu_generic_xchg(pcp, nval)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_xchg_2
// /*AFLA*/ #define this_cpu_xchg_2(pcp, nval)	this_cpu_generic_xchg(pcp, nval)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_xchg_4
// /*AFLA*/ #define this_cpu_xchg_4(pcp, nval)	this_cpu_generic_xchg(pcp, nval)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_xchg_8
// /*AFLA*/ #define this_cpu_xchg_8(pcp, nval)	this_cpu_generic_xchg(pcp, nval)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef this_cpu_cmpxchg_1
// /*AFLA*/ #define this_cpu_cmpxchg_1(pcp, oval, nval) \
// /*AFLA*/ 	this_cpu_generic_cmpxchg(pcp, oval, nval)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_cmpxchg_2
// /*AFLA*/ #define this_cpu_cmpxchg_2(pcp, oval, nval) \
// /*AFLA*/ 	this_cpu_generic_cmpxchg(pcp, oval, nval)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_cmpxchg_4
// /*AFLA*/ #define this_cpu_cmpxchg_4(pcp, oval, nval) \
// /*AFLA*/ 	this_cpu_generic_cmpxchg(pcp, oval, nval)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_cmpxchg_8
// /*AFLA*/ #define this_cpu_cmpxchg_8(pcp, oval, nval) \
// /*AFLA*/ 	this_cpu_generic_cmpxchg(pcp, oval, nval)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef this_cpu_cmpxchg_double_1
// /*AFLA*/ #define this_cpu_cmpxchg_double_1(pcp1, pcp2, oval1, oval2, nval1, nval2) \
// /*AFLA*/ 	this_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_cmpxchg_double_2
// /*AFLA*/ #define this_cpu_cmpxchg_double_2(pcp1, pcp2, oval1, oval2, nval1, nval2) \
// /*AFLA*/ 	this_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_cmpxchg_double_4
// /*AFLA*/ #define this_cpu_cmpxchg_double_4(pcp1, pcp2, oval1, oval2, nval1, nval2) \
// /*AFLA*/ 	this_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef this_cpu_cmpxchg_double_8
// /*AFLA*/ #define this_cpu_cmpxchg_double_8(pcp1, pcp2, oval1, oval2, nval1, nval2) \
// /*AFLA*/ 	this_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_GENERIC_PERCPU_H_ */
