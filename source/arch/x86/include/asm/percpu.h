// /*AFLA*/ #ifndef _ASM_X86_PERCPU_H
// /*AFLA*/ #define _ASM_X86_PERCPU_H
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ #define __percpu_seg		gs
// /*AFLA*/ #define __percpu_mov_op		movq
// /*AFLA*/ #else
// /*AFLA*/ #define __percpu_seg		fs
// /*AFLA*/ #define __percpu_mov_op		movl
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * PER_CPU finds an address of a per-cpu variable.
// /*AFLA*/  *
// /*AFLA*/  * Args:
// /*AFLA*/  *    var - variable name
// /*AFLA*/  *    reg - 32bit register
// /*AFLA*/  *
// /*AFLA*/  * The resulting address is stored in the "reg" argument.
// /*AFLA*/  *
// /*AFLA*/  * Example:
// /*AFLA*/  *    PER_CPU(cpu_gdt_descr, %ebx)
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ #define PER_CPU(var, reg)						\
// /*AFLA*/ 	__percpu_mov_op %__percpu_seg:this_cpu_off, reg;		\
// /*AFLA*/ 	lea var(reg), reg
// /*AFLA*/ #define PER_CPU_VAR(var)	%__percpu_seg:var
// /*AFLA*/ #else /* ! SMP */
// /*AFLA*/ #define PER_CPU(var, reg)	__percpu_mov_op $var, reg
// /*AFLA*/ #define PER_CPU_VAR(var)	var
// /*AFLA*/ #endif	/* SMP */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64_SMP
// /*AFLA*/ #define INIT_PER_CPU_VAR(var)  init_per_cpu__##var
// /*AFLA*/ #else
// /*AFLA*/ #define INIT_PER_CPU_VAR(var)  var
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #else /* ...!ASSEMBLY */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/stringify.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ #define __percpu_prefix		"%%"__stringify(__percpu_seg)":"
// /*AFLA*/ #define __my_cpu_offset		this_cpu_read(this_cpu_off)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Compared to the generic __my_cpu_offset version, the following
// /*AFLA*/  * saves one instruction and avoids clobbering a temp register.
// /*AFLA*/  */
// /*AFLA*/ #define arch_raw_cpu_ptr(ptr)				\
// /*AFLA*/ ({							\
// /*AFLA*/ 	unsigned long tcp_ptr__;			\
// /*AFLA*/ 	asm volatile("add " __percpu_arg(1) ", %0"	\
// /*AFLA*/ 		     : "=r" (tcp_ptr__)			\
// /*AFLA*/ 		     : "m" (this_cpu_off), "0" (ptr));	\
// /*AFLA*/ 	(typeof(*(ptr)) __kernel __force *)tcp_ptr__;	\
// /*AFLA*/ })
// /*AFLA*/ #else
// /*AFLA*/ #define __percpu_prefix		""
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __percpu_arg(x)		__percpu_prefix "%" #x
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Initialized pointers to per-cpu variables needed for the boot
// /*AFLA*/  * processor need to use these macros to get the proper address
// /*AFLA*/  * offset from __per_cpu_load on SMP.
// /*AFLA*/  *
// /*AFLA*/  * There also must be an entry in vmlinux_64.lds.S
// /*AFLA*/  */
// /*AFLA*/ #define DECLARE_INIT_PER_CPU(var) \
// /*AFLA*/        extern typeof(var) init_per_cpu_var(var)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64_SMP
// /*AFLA*/ #define init_per_cpu_var(var)  init_per_cpu__##var
// /*AFLA*/ #else
// /*AFLA*/ #define init_per_cpu_var(var)  var
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* For arch-specific code, we can use direct single-insn ops (they
// /*AFLA*/  * don't give an lvalue though). */
// /*AFLA*/ extern void __bad_percpu_size(void);
// /*AFLA*/ 
// /*AFLA*/ #define percpu_to_op(op, var, val)			\
// /*AFLA*/ do {							\
// /*AFLA*/ 	typedef typeof(var) pto_T__;			\
// /*AFLA*/ 	if (0) {					\
// /*AFLA*/ 		pto_T__ pto_tmp__;			\
// /*AFLA*/ 		pto_tmp__ = (val);			\
// /*AFLA*/ 		(void)pto_tmp__;			\
// /*AFLA*/ 	}						\
// /*AFLA*/ 	switch (sizeof(var)) {				\
// /*AFLA*/ 	case 1:						\
// /*AFLA*/ 		asm(op "b %1,"__percpu_arg(0)		\
// /*AFLA*/ 		    : "+m" (var)			\
// /*AFLA*/ 		    : "qi" ((pto_T__)(val)));		\
// /*AFLA*/ 		break;					\
// /*AFLA*/ 	case 2:						\
// /*AFLA*/ 		asm(op "w %1,"__percpu_arg(0)		\
// /*AFLA*/ 		    : "+m" (var)			\
// /*AFLA*/ 		    : "ri" ((pto_T__)(val)));		\
// /*AFLA*/ 		break;					\
// /*AFLA*/ 	case 4:						\
// /*AFLA*/ 		asm(op "l %1,"__percpu_arg(0)		\
// /*AFLA*/ 		    : "+m" (var)			\
// /*AFLA*/ 		    : "ri" ((pto_T__)(val)));		\
// /*AFLA*/ 		break;					\
// /*AFLA*/ 	case 8:						\
// /*AFLA*/ 		asm(op "q %1,"__percpu_arg(0)		\
// /*AFLA*/ 		    : "+m" (var)			\
// /*AFLA*/ 		    : "re" ((pto_T__)(val)));		\
// /*AFLA*/ 		break;					\
// /*AFLA*/ 	default: __bad_percpu_size();			\
// /*AFLA*/ 	}						\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Generate a percpu add to memory instruction and optimize code
// /*AFLA*/  * if one is added or subtracted.
// /*AFLA*/  */
// /*AFLA*/ #define percpu_add_op(var, val)						\
// /*AFLA*/ do {									\
// /*AFLA*/ 	typedef typeof(var) pao_T__;					\
// /*AFLA*/ 	const int pao_ID__ = (__builtin_constant_p(val) &&		\
// /*AFLA*/ 			      ((val) == 1 || (val) == -1)) ?		\
// /*AFLA*/ 				(int)(val) : 0;				\
// /*AFLA*/ 	if (0) {							\
// /*AFLA*/ 		pao_T__ pao_tmp__;					\
// /*AFLA*/ 		pao_tmp__ = (val);					\
// /*AFLA*/ 		(void)pao_tmp__;					\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	switch (sizeof(var)) {						\
// /*AFLA*/ 	case 1:								\
// /*AFLA*/ 		if (pao_ID__ == 1)					\
// /*AFLA*/ 			asm("incb "__percpu_arg(0) : "+m" (var));	\
// /*AFLA*/ 		else if (pao_ID__ == -1)				\
// /*AFLA*/ 			asm("decb "__percpu_arg(0) : "+m" (var));	\
// /*AFLA*/ 		else							\
// /*AFLA*/ 			asm("addb %1, "__percpu_arg(0)			\
// /*AFLA*/ 			    : "+m" (var)				\
// /*AFLA*/ 			    : "qi" ((pao_T__)(val)));			\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 2:								\
// /*AFLA*/ 		if (pao_ID__ == 1)					\
// /*AFLA*/ 			asm("incw "__percpu_arg(0) : "+m" (var));	\
// /*AFLA*/ 		else if (pao_ID__ == -1)				\
// /*AFLA*/ 			asm("decw "__percpu_arg(0) : "+m" (var));	\
// /*AFLA*/ 		else							\
// /*AFLA*/ 			asm("addw %1, "__percpu_arg(0)			\
// /*AFLA*/ 			    : "+m" (var)				\
// /*AFLA*/ 			    : "ri" ((pao_T__)(val)));			\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 4:								\
// /*AFLA*/ 		if (pao_ID__ == 1)					\
// /*AFLA*/ 			asm("incl "__percpu_arg(0) : "+m" (var));	\
// /*AFLA*/ 		else if (pao_ID__ == -1)				\
// /*AFLA*/ 			asm("decl "__percpu_arg(0) : "+m" (var));	\
// /*AFLA*/ 		else							\
// /*AFLA*/ 			asm("addl %1, "__percpu_arg(0)			\
// /*AFLA*/ 			    : "+m" (var)				\
// /*AFLA*/ 			    : "ri" ((pao_T__)(val)));			\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 8:								\
// /*AFLA*/ 		if (pao_ID__ == 1)					\
// /*AFLA*/ 			asm("incq "__percpu_arg(0) : "+m" (var));	\
// /*AFLA*/ 		else if (pao_ID__ == -1)				\
// /*AFLA*/ 			asm("decq "__percpu_arg(0) : "+m" (var));	\
// /*AFLA*/ 		else							\
// /*AFLA*/ 			asm("addq %1, "__percpu_arg(0)			\
// /*AFLA*/ 			    : "+m" (var)				\
// /*AFLA*/ 			    : "re" ((pao_T__)(val)));			\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	default: __bad_percpu_size();					\
// /*AFLA*/ 	}								\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define percpu_from_op(op, var)				\
// /*AFLA*/ ({							\
// /*AFLA*/ 	typeof(var) pfo_ret__;				\
// /*AFLA*/ 	switch (sizeof(var)) {				\
// /*AFLA*/ 	case 1:						\
// /*AFLA*/ 		asm(op "b "__percpu_arg(1)",%0"		\
// /*AFLA*/ 		    : "=q" (pfo_ret__)			\
// /*AFLA*/ 		    : "m" (var));			\
// /*AFLA*/ 		break;					\
// /*AFLA*/ 	case 2:						\
// /*AFLA*/ 		asm(op "w "__percpu_arg(1)",%0"		\
// /*AFLA*/ 		    : "=r" (pfo_ret__)			\
// /*AFLA*/ 		    : "m" (var));			\
// /*AFLA*/ 		break;					\
// /*AFLA*/ 	case 4:						\
// /*AFLA*/ 		asm(op "l "__percpu_arg(1)",%0"		\
// /*AFLA*/ 		    : "=r" (pfo_ret__)			\
// /*AFLA*/ 		    : "m" (var));			\
// /*AFLA*/ 		break;					\
// /*AFLA*/ 	case 8:						\
// /*AFLA*/ 		asm(op "q "__percpu_arg(1)",%0"		\
// /*AFLA*/ 		    : "=r" (pfo_ret__)			\
// /*AFLA*/ 		    : "m" (var));			\
// /*AFLA*/ 		break;					\
// /*AFLA*/ 	default: __bad_percpu_size();			\
// /*AFLA*/ 	}						\
// /*AFLA*/ 	pfo_ret__;					\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define percpu_stable_op(op, var)			\
// /*AFLA*/ ({							\
// /*AFLA*/ 	typeof(var) pfo_ret__;				\
// /*AFLA*/ 	switch (sizeof(var)) {				\
// /*AFLA*/ 	case 1:						\
// /*AFLA*/ 		asm(op "b "__percpu_arg(P1)",%0"	\
// /*AFLA*/ 		    : "=q" (pfo_ret__)			\
// /*AFLA*/ 		    : "p" (&(var)));			\
// /*AFLA*/ 		break;					\
// /*AFLA*/ 	case 2:						\
// /*AFLA*/ 		asm(op "w "__percpu_arg(P1)",%0"	\
// /*AFLA*/ 		    : "=r" (pfo_ret__)			\
// /*AFLA*/ 		    : "p" (&(var)));			\
// /*AFLA*/ 		break;					\
// /*AFLA*/ 	case 4:						\
// /*AFLA*/ 		asm(op "l "__percpu_arg(P1)",%0"	\
// /*AFLA*/ 		    : "=r" (pfo_ret__)			\
// /*AFLA*/ 		    : "p" (&(var)));			\
// /*AFLA*/ 		break;					\
// /*AFLA*/ 	case 8:						\
// /*AFLA*/ 		asm(op "q "__percpu_arg(P1)",%0"	\
// /*AFLA*/ 		    : "=r" (pfo_ret__)			\
// /*AFLA*/ 		    : "p" (&(var)));			\
// /*AFLA*/ 		break;					\
// /*AFLA*/ 	default: __bad_percpu_size();			\
// /*AFLA*/ 	}						\
// /*AFLA*/ 	pfo_ret__;					\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define percpu_unary_op(op, var)			\
// /*AFLA*/ ({							\
// /*AFLA*/ 	switch (sizeof(var)) {				\
// /*AFLA*/ 	case 1:						\
// /*AFLA*/ 		asm(op "b "__percpu_arg(0)		\
// /*AFLA*/ 		    : "+m" (var));			\
// /*AFLA*/ 		break;					\
// /*AFLA*/ 	case 2:						\
// /*AFLA*/ 		asm(op "w "__percpu_arg(0)		\
// /*AFLA*/ 		    : "+m" (var));			\
// /*AFLA*/ 		break;					\
// /*AFLA*/ 	case 4:						\
// /*AFLA*/ 		asm(op "l "__percpu_arg(0)		\
// /*AFLA*/ 		    : "+m" (var));			\
// /*AFLA*/ 		break;					\
// /*AFLA*/ 	case 8:						\
// /*AFLA*/ 		asm(op "q "__percpu_arg(0)		\
// /*AFLA*/ 		    : "+m" (var));			\
// /*AFLA*/ 		break;					\
// /*AFLA*/ 	default: __bad_percpu_size();			\
// /*AFLA*/ 	}						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Add return operation
// /*AFLA*/  */
// /*AFLA*/ #define percpu_add_return_op(var, val)					\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(var) paro_ret__ = val;					\
// /*AFLA*/ 	switch (sizeof(var)) {						\
// /*AFLA*/ 	case 1:								\
// /*AFLA*/ 		asm("xaddb %0, "__percpu_arg(1)				\
// /*AFLA*/ 			    : "+q" (paro_ret__), "+m" (var)		\
// /*AFLA*/ 			    : : "memory");				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 2:								\
// /*AFLA*/ 		asm("xaddw %0, "__percpu_arg(1)				\
// /*AFLA*/ 			    : "+r" (paro_ret__), "+m" (var)		\
// /*AFLA*/ 			    : : "memory");				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 4:								\
// /*AFLA*/ 		asm("xaddl %0, "__percpu_arg(1)				\
// /*AFLA*/ 			    : "+r" (paro_ret__), "+m" (var)		\
// /*AFLA*/ 			    : : "memory");				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 8:								\
// /*AFLA*/ 		asm("xaddq %0, "__percpu_arg(1)				\
// /*AFLA*/ 			    : "+re" (paro_ret__), "+m" (var)		\
// /*AFLA*/ 			    : : "memory");				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	default: __bad_percpu_size();					\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	paro_ret__ += val;						\
// /*AFLA*/ 	paro_ret__;							\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * xchg is implemented using cmpxchg without a lock prefix. xchg is
// /*AFLA*/  * expensive due to the implied lock prefix.  The processor cannot prefetch
// /*AFLA*/  * cachelines if xchg is used.
// /*AFLA*/  */
// /*AFLA*/ #define percpu_xchg_op(var, nval)					\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(var) pxo_ret__;						\
// /*AFLA*/ 	typeof(var) pxo_new__ = (nval);					\
// /*AFLA*/ 	switch (sizeof(var)) {						\
// /*AFLA*/ 	case 1:								\
// /*AFLA*/ 		asm("\n\tmov "__percpu_arg(1)",%%al"			\
// /*AFLA*/ 		    "\n1:\tcmpxchgb %2, "__percpu_arg(1)		\
// /*AFLA*/ 		    "\n\tjnz 1b"					\
// /*AFLA*/ 			    : "=&a" (pxo_ret__), "+m" (var)		\
// /*AFLA*/ 			    : "q" (pxo_new__)				\
// /*AFLA*/ 			    : "memory");				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 2:								\
// /*AFLA*/ 		asm("\n\tmov "__percpu_arg(1)",%%ax"			\
// /*AFLA*/ 		    "\n1:\tcmpxchgw %2, "__percpu_arg(1)		\
// /*AFLA*/ 		    "\n\tjnz 1b"					\
// /*AFLA*/ 			    : "=&a" (pxo_ret__), "+m" (var)		\
// /*AFLA*/ 			    : "r" (pxo_new__)				\
// /*AFLA*/ 			    : "memory");				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 4:								\
// /*AFLA*/ 		asm("\n\tmov "__percpu_arg(1)",%%eax"			\
// /*AFLA*/ 		    "\n1:\tcmpxchgl %2, "__percpu_arg(1)		\
// /*AFLA*/ 		    "\n\tjnz 1b"					\
// /*AFLA*/ 			    : "=&a" (pxo_ret__), "+m" (var)		\
// /*AFLA*/ 			    : "r" (pxo_new__)				\
// /*AFLA*/ 			    : "memory");				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 8:								\
// /*AFLA*/ 		asm("\n\tmov "__percpu_arg(1)",%%rax"			\
// /*AFLA*/ 		    "\n1:\tcmpxchgq %2, "__percpu_arg(1)		\
// /*AFLA*/ 		    "\n\tjnz 1b"					\
// /*AFLA*/ 			    : "=&a" (pxo_ret__), "+m" (var)		\
// /*AFLA*/ 			    : "r" (pxo_new__)				\
// /*AFLA*/ 			    : "memory");				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	default: __bad_percpu_size();					\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	pxo_ret__;							\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * cmpxchg has no such implied lock semantics as a result it is much
// /*AFLA*/  * more efficient for cpu local operations.
// /*AFLA*/  */
// /*AFLA*/ #define percpu_cmpxchg_op(var, oval, nval)				\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(var) pco_ret__;						\
// /*AFLA*/ 	typeof(var) pco_old__ = (oval);					\
// /*AFLA*/ 	typeof(var) pco_new__ = (nval);					\
// /*AFLA*/ 	switch (sizeof(var)) {						\
// /*AFLA*/ 	case 1:								\
// /*AFLA*/ 		asm("cmpxchgb %2, "__percpu_arg(1)			\
// /*AFLA*/ 			    : "=a" (pco_ret__), "+m" (var)		\
// /*AFLA*/ 			    : "q" (pco_new__), "0" (pco_old__)		\
// /*AFLA*/ 			    : "memory");				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 2:								\
// /*AFLA*/ 		asm("cmpxchgw %2, "__percpu_arg(1)			\
// /*AFLA*/ 			    : "=a" (pco_ret__), "+m" (var)		\
// /*AFLA*/ 			    : "r" (pco_new__), "0" (pco_old__)		\
// /*AFLA*/ 			    : "memory");				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 4:								\
// /*AFLA*/ 		asm("cmpxchgl %2, "__percpu_arg(1)			\
// /*AFLA*/ 			    : "=a" (pco_ret__), "+m" (var)		\
// /*AFLA*/ 			    : "r" (pco_new__), "0" (pco_old__)		\
// /*AFLA*/ 			    : "memory");				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 8:								\
// /*AFLA*/ 		asm("cmpxchgq %2, "__percpu_arg(1)			\
// /*AFLA*/ 			    : "=a" (pco_ret__), "+m" (var)		\
// /*AFLA*/ 			    : "r" (pco_new__), "0" (pco_old__)		\
// /*AFLA*/ 			    : "memory");				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	default: __bad_percpu_size();					\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	pco_ret__;							\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * this_cpu_read() makes gcc load the percpu variable every time it is
// /*AFLA*/  * accessed while this_cpu_read_stable() allows the value to be cached.
// /*AFLA*/  * this_cpu_read_stable() is more efficient and can be used if its value
// /*AFLA*/  * is guaranteed to be valid across cpus.  The current users include
// /*AFLA*/  * get_current() and get_thread_info() both of which are actually
// /*AFLA*/  * per-thread variables implemented as per-cpu variables and thus
// /*AFLA*/  * stable for the duration of the respective task.
// /*AFLA*/  */
// /*AFLA*/ #define this_cpu_read_stable(var)	percpu_stable_op("mov", var)
// /*AFLA*/ 
// /*AFLA*/ #define raw_cpu_read_1(pcp)		percpu_from_op("mov", pcp)
// /*AFLA*/ #define raw_cpu_read_2(pcp)		percpu_from_op("mov", pcp)
// /*AFLA*/ #define raw_cpu_read_4(pcp)		percpu_from_op("mov", pcp)
// /*AFLA*/ 
// /*AFLA*/ #define raw_cpu_write_1(pcp, val)	percpu_to_op("mov", (pcp), val)
// /*AFLA*/ #define raw_cpu_write_2(pcp, val)	percpu_to_op("mov", (pcp), val)
// /*AFLA*/ #define raw_cpu_write_4(pcp, val)	percpu_to_op("mov", (pcp), val)
// /*AFLA*/ #define raw_cpu_add_1(pcp, val)		percpu_add_op((pcp), val)
// /*AFLA*/ #define raw_cpu_add_2(pcp, val)		percpu_add_op((pcp), val)
// /*AFLA*/ #define raw_cpu_add_4(pcp, val)		percpu_add_op((pcp), val)
// /*AFLA*/ #define raw_cpu_and_1(pcp, val)		percpu_to_op("and", (pcp), val)
// /*AFLA*/ #define raw_cpu_and_2(pcp, val)		percpu_to_op("and", (pcp), val)
// /*AFLA*/ #define raw_cpu_and_4(pcp, val)		percpu_to_op("and", (pcp), val)
// /*AFLA*/ #define raw_cpu_or_1(pcp, val)		percpu_to_op("or", (pcp), val)
// /*AFLA*/ #define raw_cpu_or_2(pcp, val)		percpu_to_op("or", (pcp), val)
// /*AFLA*/ #define raw_cpu_or_4(pcp, val)		percpu_to_op("or", (pcp), val)
// /*AFLA*/ #define raw_cpu_xchg_1(pcp, val)	percpu_xchg_op(pcp, val)
// /*AFLA*/ #define raw_cpu_xchg_2(pcp, val)	percpu_xchg_op(pcp, val)
// /*AFLA*/ #define raw_cpu_xchg_4(pcp, val)	percpu_xchg_op(pcp, val)
// /*AFLA*/ 
// /*AFLA*/ #define this_cpu_read_1(pcp)		percpu_from_op("mov", pcp)
// /*AFLA*/ #define this_cpu_read_2(pcp)		percpu_from_op("mov", pcp)
// /*AFLA*/ #define this_cpu_read_4(pcp)		percpu_from_op("mov", pcp)
// /*AFLA*/ #define this_cpu_write_1(pcp, val)	percpu_to_op("mov", (pcp), val)
// /*AFLA*/ #define this_cpu_write_2(pcp, val)	percpu_to_op("mov", (pcp), val)
// /*AFLA*/ #define this_cpu_write_4(pcp, val)	percpu_to_op("mov", (pcp), val)
// /*AFLA*/ #define this_cpu_add_1(pcp, val)	percpu_add_op((pcp), val)
// /*AFLA*/ #define this_cpu_add_2(pcp, val)	percpu_add_op((pcp), val)
// /*AFLA*/ #define this_cpu_add_4(pcp, val)	percpu_add_op((pcp), val)
// /*AFLA*/ #define this_cpu_and_1(pcp, val)	percpu_to_op("and", (pcp), val)
// /*AFLA*/ #define this_cpu_and_2(pcp, val)	percpu_to_op("and", (pcp), val)
// /*AFLA*/ #define this_cpu_and_4(pcp, val)	percpu_to_op("and", (pcp), val)
// /*AFLA*/ #define this_cpu_or_1(pcp, val)		percpu_to_op("or", (pcp), val)
// /*AFLA*/ #define this_cpu_or_2(pcp, val)		percpu_to_op("or", (pcp), val)
// /*AFLA*/ #define this_cpu_or_4(pcp, val)		percpu_to_op("or", (pcp), val)
// /*AFLA*/ #define this_cpu_xchg_1(pcp, nval)	percpu_xchg_op(pcp, nval)
// /*AFLA*/ #define this_cpu_xchg_2(pcp, nval)	percpu_xchg_op(pcp, nval)
// /*AFLA*/ #define this_cpu_xchg_4(pcp, nval)	percpu_xchg_op(pcp, nval)
// /*AFLA*/ 
// /*AFLA*/ #define raw_cpu_add_return_1(pcp, val)		percpu_add_return_op(pcp, val)
// /*AFLA*/ #define raw_cpu_add_return_2(pcp, val)		percpu_add_return_op(pcp, val)
// /*AFLA*/ #define raw_cpu_add_return_4(pcp, val)		percpu_add_return_op(pcp, val)
// /*AFLA*/ #define raw_cpu_cmpxchg_1(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)
// /*AFLA*/ #define raw_cpu_cmpxchg_2(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)
// /*AFLA*/ #define raw_cpu_cmpxchg_4(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)
// /*AFLA*/ 
// /*AFLA*/ #define this_cpu_add_return_1(pcp, val)		percpu_add_return_op(pcp, val)
// /*AFLA*/ #define this_cpu_add_return_2(pcp, val)		percpu_add_return_op(pcp, val)
// /*AFLA*/ #define this_cpu_add_return_4(pcp, val)		percpu_add_return_op(pcp, val)
// /*AFLA*/ #define this_cpu_cmpxchg_1(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)
// /*AFLA*/ #define this_cpu_cmpxchg_2(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)
// /*AFLA*/ #define this_cpu_cmpxchg_4(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_CMPXCHG64
// /*AFLA*/ #define percpu_cmpxchg8b_double(pcp1, pcp2, o1, o2, n1, n2)		\
// /*AFLA*/ ({									\
// /*AFLA*/ 	bool __ret;							\
// /*AFLA*/ 	typeof(pcp1) __o1 = (o1), __n1 = (n1);				\
// /*AFLA*/ 	typeof(pcp2) __o2 = (o2), __n2 = (n2);				\
// /*AFLA*/ 	asm volatile("cmpxchg8b "__percpu_arg(1)"\n\tsetz %0\n\t"	\
// /*AFLA*/ 		    : "=a" (__ret), "+m" (pcp1), "+m" (pcp2), "+d" (__o2) \
// /*AFLA*/ 		    :  "b" (__n1), "c" (__n2), "a" (__o1));		\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define raw_cpu_cmpxchg_double_4	percpu_cmpxchg8b_double
// /*AFLA*/ #define this_cpu_cmpxchg_double_4	percpu_cmpxchg8b_double
// /*AFLA*/ #endif /* CONFIG_X86_CMPXCHG64 */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Per cpu atomic 64 bit operations are only available under 64 bit.
// /*AFLA*/  * 32 bit must fall back to generic operations.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ #define raw_cpu_read_8(pcp)			percpu_from_op("mov", pcp)
// /*AFLA*/ #define raw_cpu_write_8(pcp, val)		percpu_to_op("mov", (pcp), val)
// /*AFLA*/ #define raw_cpu_add_8(pcp, val)			percpu_add_op((pcp), val)
// /*AFLA*/ #define raw_cpu_and_8(pcp, val)			percpu_to_op("and", (pcp), val)
// /*AFLA*/ #define raw_cpu_or_8(pcp, val)			percpu_to_op("or", (pcp), val)
// /*AFLA*/ #define raw_cpu_add_return_8(pcp, val)		percpu_add_return_op(pcp, val)
// /*AFLA*/ #define raw_cpu_xchg_8(pcp, nval)		percpu_xchg_op(pcp, nval)
// /*AFLA*/ #define raw_cpu_cmpxchg_8(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)
// /*AFLA*/ 
// /*AFLA*/ #define this_cpu_read_8(pcp)			percpu_from_op("mov", pcp)
// /*AFLA*/ #define this_cpu_write_8(pcp, val)		percpu_to_op("mov", (pcp), val)
// /*AFLA*/ #define this_cpu_add_8(pcp, val)		percpu_add_op((pcp), val)
// /*AFLA*/ #define this_cpu_and_8(pcp, val)		percpu_to_op("and", (pcp), val)
// /*AFLA*/ #define this_cpu_or_8(pcp, val)			percpu_to_op("or", (pcp), val)
// /*AFLA*/ #define this_cpu_add_return_8(pcp, val)		percpu_add_return_op(pcp, val)
// /*AFLA*/ #define this_cpu_xchg_8(pcp, nval)		percpu_xchg_op(pcp, nval)
// /*AFLA*/ #define this_cpu_cmpxchg_8(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Pretty complex macro to generate cmpxchg16 instruction.  The instruction
// /*AFLA*/  * is not supported on early AMD64 processors so we must be able to emulate
// /*AFLA*/  * it in software.  The address used in the cmpxchg16 instruction must be
// /*AFLA*/  * aligned to a 16 byte boundary.
// /*AFLA*/  */
// /*AFLA*/ #define percpu_cmpxchg16b_double(pcp1, pcp2, o1, o2, n1, n2)		\
// /*AFLA*/ ({									\
// /*AFLA*/ 	bool __ret;							\
// /*AFLA*/ 	typeof(pcp1) __o1 = (o1), __n1 = (n1);				\
// /*AFLA*/ 	typeof(pcp2) __o2 = (o2), __n2 = (n2);				\
// /*AFLA*/ 	alternative_io("leaq %P1,%%rsi\n\tcall this_cpu_cmpxchg16b_emu\n\t", \
// /*AFLA*/ 		       "cmpxchg16b " __percpu_arg(1) "\n\tsetz %0\n\t",	\
// /*AFLA*/ 		       X86_FEATURE_CX16,				\
// /*AFLA*/ 		       ASM_OUTPUT2("=a" (__ret), "+m" (pcp1),		\
// /*AFLA*/ 				   "+m" (pcp2), "+d" (__o2)),		\
// /*AFLA*/ 		       "b" (__n1), "c" (__n2), "a" (__o1) : "rsi");	\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define raw_cpu_cmpxchg_double_8	percpu_cmpxchg16b_double
// /*AFLA*/ #define this_cpu_cmpxchg_double_8	percpu_cmpxchg16b_double
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* This is not atomic against other CPUs -- CPU preemption needs to be off */
// /*AFLA*/ #define x86_test_and_clear_bit_percpu(bit, var)				\
// /*AFLA*/ ({									\
// /*AFLA*/ 	bool old__;							\
// /*AFLA*/ 	asm volatile("btr %2,"__percpu_arg(1)"\n\t"			\
// /*AFLA*/ 		     CC_SET(c)						\
// /*AFLA*/ 		     : CC_OUT(c) (old__), "+m" (var)			\
// /*AFLA*/ 		     : "dIr" (bit));					\
// /*AFLA*/ 	old__;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ static __always_inline bool x86_this_cpu_constant_test_bit(unsigned int nr,
// /*AFLA*/                         const unsigned long __percpu *addr)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long __percpu *a =
// /*AFLA*/ 		(unsigned long __percpu *)addr + nr / BITS_PER_LONG;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 	return ((1UL << (nr % BITS_PER_LONG)) & raw_cpu_read_8(*a)) != 0;
// /*AFLA*/ #else
// /*AFLA*/ 	return ((1UL << (nr % BITS_PER_LONG)) & raw_cpu_read_4(*a)) != 0;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool x86_this_cpu_variable_test_bit(int nr,
// /*AFLA*/                         const unsigned long __percpu *addr)
// /*AFLA*/ {
// /*AFLA*/ 	bool oldbit;
// /*AFLA*/ 
// /*AFLA*/ 	asm volatile("bt "__percpu_arg(2)",%1\n\t"
// /*AFLA*/ 			CC_SET(c)
// /*AFLA*/ 			: CC_OUT(c) (oldbit)
// /*AFLA*/ 			: "m" (*(unsigned long __percpu *)addr), "Ir" (nr));
// /*AFLA*/ 
// /*AFLA*/ 	return oldbit;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define x86_this_cpu_test_bit(nr, addr)			\
// /*AFLA*/ 	(__builtin_constant_p((nr))			\
// /*AFLA*/ 	 ? x86_this_cpu_constant_test_bit((nr), (addr))	\
// /*AFLA*/ 	 : x86_this_cpu_variable_test_bit((nr), (addr)))
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/percpu.h>
// /*AFLA*/ 
// /*AFLA*/ /* We can use this directly for local CPU (faster). */
// /*AFLA*/ DECLARE_PER_CPU_READ_MOSTLY(unsigned long, this_cpu_off);
// /*AFLA*/ 
// /*AFLA*/ #endif /* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Define the "EARLY_PER_CPU" macros.  These are used for some per_cpu
// /*AFLA*/  * variables that are initialized and accessed before there are per_cpu
// /*AFLA*/  * areas allocated.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define	DEFINE_EARLY_PER_CPU(_type, _name, _initvalue)			\
// /*AFLA*/ 	DEFINE_PER_CPU(_type, _name) = _initvalue;			\
// /*AFLA*/ 	__typeof__(_type) _name##_early_map[NR_CPUS] __initdata =	\
// /*AFLA*/ 				{ [0 ... NR_CPUS-1] = _initvalue };	\
// /*AFLA*/ 	__typeof__(_type) *_name##_early_ptr __refdata = _name##_early_map
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_EARLY_PER_CPU_READ_MOSTLY(_type, _name, _initvalue)	\
// /*AFLA*/ 	DEFINE_PER_CPU_READ_MOSTLY(_type, _name) = _initvalue;		\
// /*AFLA*/ 	__typeof__(_type) _name##_early_map[NR_CPUS] __initdata =	\
// /*AFLA*/ 				{ [0 ... NR_CPUS-1] = _initvalue };	\
// /*AFLA*/ 	__typeof__(_type) *_name##_early_ptr __refdata = _name##_early_map
// /*AFLA*/ 
// /*AFLA*/ #define EXPORT_EARLY_PER_CPU_SYMBOL(_name)			\
// /*AFLA*/ 	EXPORT_PER_CPU_SYMBOL(_name)
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_EARLY_PER_CPU(_type, _name)			\
// /*AFLA*/ 	DECLARE_PER_CPU(_type, _name);				\
// /*AFLA*/ 	extern __typeof__(_type) *_name##_early_ptr;		\
// /*AFLA*/ 	extern __typeof__(_type)  _name##_early_map[]
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_EARLY_PER_CPU_READ_MOSTLY(_type, _name)		\
// /*AFLA*/ 	DECLARE_PER_CPU_READ_MOSTLY(_type, _name);		\
// /*AFLA*/ 	extern __typeof__(_type) *_name##_early_ptr;		\
// /*AFLA*/ 	extern __typeof__(_type)  _name##_early_map[]
// /*AFLA*/ 
// /*AFLA*/ #define	early_per_cpu_ptr(_name) (_name##_early_ptr)
// /*AFLA*/ #define	early_per_cpu_map(_name, _idx) (_name##_early_map[_idx])
// /*AFLA*/ #define	early_per_cpu(_name, _cpu) 				\
// /*AFLA*/ 	*(early_per_cpu_ptr(_name) ?				\
// /*AFLA*/ 		&early_per_cpu_ptr(_name)[_cpu] :		\
// /*AFLA*/ 		&per_cpu(_name, _cpu))
// /*AFLA*/ 
// /*AFLA*/ #else	/* !CONFIG_SMP */
// /*AFLA*/ #define	DEFINE_EARLY_PER_CPU(_type, _name, _initvalue)		\
// /*AFLA*/ 	DEFINE_PER_CPU(_type, _name) = _initvalue
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_EARLY_PER_CPU_READ_MOSTLY(_type, _name, _initvalue)	\
// /*AFLA*/ 	DEFINE_PER_CPU_READ_MOSTLY(_type, _name) = _initvalue
// /*AFLA*/ 
// /*AFLA*/ #define EXPORT_EARLY_PER_CPU_SYMBOL(_name)			\
// /*AFLA*/ 	EXPORT_PER_CPU_SYMBOL(_name)
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_EARLY_PER_CPU(_type, _name)			\
// /*AFLA*/ 	DECLARE_PER_CPU(_type, _name)
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_EARLY_PER_CPU_READ_MOSTLY(_type, _name)		\
// /*AFLA*/ 	DECLARE_PER_CPU_READ_MOSTLY(_type, _name)
// /*AFLA*/ 
// /*AFLA*/ #define	early_per_cpu(_name, _cpu) per_cpu(_name, _cpu)
// /*AFLA*/ #define	early_per_cpu_ptr(_name) NULL
// /*AFLA*/ /* no early_per_cpu_map() */
// /*AFLA*/ 
// /*AFLA*/ #endif	/* !CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_PERCPU_H */
