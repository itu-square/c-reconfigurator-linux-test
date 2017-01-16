// /*AFLA*/ #ifndef _ASM_X86_UACCESS_H
// /*AFLA*/ #define _ASM_X86_UACCESS_H
// /*AFLA*/ /*
// /*AFLA*/  * User space memory access functions
// /*AFLA*/  */
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/kasan-checks.h>
// /*AFLA*/ #include <linux/thread_info.h>
// /*AFLA*/ #include <linux/string.h>
// /*AFLA*/ #include <asm/asm.h>
// /*AFLA*/ #include <asm/page.h>
// /*AFLA*/ #include <asm/smap.h>
// /*AFLA*/ #include <asm/extable.h>
// /*AFLA*/ 
// /*AFLA*/ #define VERIFY_READ 0
// /*AFLA*/ #define VERIFY_WRITE 1
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The fs value determines whether argument validity checking should be
// /*AFLA*/  * performed or not.  If get_fs() == USER_DS, checking is performed, with
// /*AFLA*/  * get_fs() == KERNEL_DS, checking is bypassed.
// /*AFLA*/  *
// /*AFLA*/  * For historical reasons, these macros are grossly misnamed.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define MAKE_MM_SEG(s)	((mm_segment_t) { (s) })
// /*AFLA*/ 
// /*AFLA*/ #define KERNEL_DS	MAKE_MM_SEG(-1UL)
// /*AFLA*/ #define USER_DS 	MAKE_MM_SEG(TASK_SIZE_MAX)
// /*AFLA*/ 
// /*AFLA*/ #define get_ds()	(KERNEL_DS)
// /*AFLA*/ #define get_fs()	(current->thread.addr_limit)
// /*AFLA*/ #define set_fs(x)	(current->thread.addr_limit = (x))
// /*AFLA*/ 
// /*AFLA*/ #define segment_eq(a, b)	((a).seg == (b).seg)
// /*AFLA*/ 
// /*AFLA*/ #define user_addr_max() (current->thread.addr_limit.seg)
// /*AFLA*/ #define __addr_ok(addr) 	\
// /*AFLA*/ 	((unsigned long __force)(addr) < user_addr_max())
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Test whether a block of memory is a valid user space address.
// /*AFLA*/  * Returns 0 if the range is valid, nonzero otherwise.
// /*AFLA*/  */
// /*AFLA*/ static inline bool __chk_range_not_ok(unsigned long addr, unsigned long size, unsigned long limit)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If we have used "sizeof()" for the size,
// /*AFLA*/ 	 * we know it won't overflow the limit (but
// /*AFLA*/ 	 * it might overflow the 'addr', so it's
// /*AFLA*/ 	 * important to subtract the size from the
// /*AFLA*/ 	 * limit, not add it to the address).
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (__builtin_constant_p(size))
// /*AFLA*/ 		return unlikely(addr > limit - size);
// /*AFLA*/ 
// /*AFLA*/ 	/* Arbitrary sizes? Be careful about overflow */
// /*AFLA*/ 	addr += size;
// /*AFLA*/ 	if (unlikely(addr < size))
// /*AFLA*/ 		return true;
// /*AFLA*/ 	return unlikely(addr > limit);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __range_not_ok(addr, size, limit)				\
// /*AFLA*/ ({									\
// /*AFLA*/ 	__chk_user_ptr(addr);						\
// /*AFLA*/ 	__chk_range_not_ok((unsigned long __force)(addr), size, limit); \
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * access_ok: - Checks if a user space pointer is valid
// /*AFLA*/  * @type: Type of access: %VERIFY_READ or %VERIFY_WRITE.  Note that
// /*AFLA*/  *        %VERIFY_WRITE is a superset of %VERIFY_READ - if it is safe
// /*AFLA*/  *        to write to a block, it is always safe to read from it.
// /*AFLA*/  * @addr: User space pointer to start of block to check
// /*AFLA*/  * @size: Size of block to check
// /*AFLA*/  *
// /*AFLA*/  * Context: User context only. This function may sleep if pagefaults are
// /*AFLA*/  *          enabled.
// /*AFLA*/  *
// /*AFLA*/  * Checks if a pointer to a block of memory in user space is valid.
// /*AFLA*/  *
// /*AFLA*/  * Returns true (nonzero) if the memory block may be valid, false (zero)
// /*AFLA*/  * if it is definitely invalid.
// /*AFLA*/  *
// /*AFLA*/  * Note that, depending on architecture, this function probably just
// /*AFLA*/  * checks that the pointer is in the user space range - after calling
// /*AFLA*/  * this function, memory access functions may still return -EFAULT.
// /*AFLA*/  */
// /*AFLA*/ #define access_ok(type, addr, size) \
// /*AFLA*/ 	likely(!__range_not_ok(addr, size, user_addr_max()))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These are the main single-value transfer routines.  They automatically
// /*AFLA*/  * use the right size if we just have the right pointer type.
// /*AFLA*/  *
// /*AFLA*/  * This gets kind of ugly. We want to return _two_ values in "get_user()"
// /*AFLA*/  * and yet we don't want to do any pointers, because that is too much
// /*AFLA*/  * of a performance impact. Thus we have a few rather ugly macros here,
// /*AFLA*/  * and hide all the ugliness from the user.
// /*AFLA*/  *
// /*AFLA*/  * The "__xxx" versions of the user access functions are versions that
// /*AFLA*/  * do not verify the address space, that must have been done previously
// /*AFLA*/  * with a separate "access_ok()" call (this is used when we do multiple
// /*AFLA*/  * accesses to the same area of user memory).
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ extern int __get_user_1(void);
// /*AFLA*/ extern int __get_user_2(void);
// /*AFLA*/ extern int __get_user_4(void);
// /*AFLA*/ extern int __get_user_8(void);
// /*AFLA*/ extern int __get_user_bad(void);
// /*AFLA*/ 
// /*AFLA*/ #define __uaccess_begin() stac()
// /*AFLA*/ #define __uaccess_end()   clac()
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is a type: either unsigned long, if the argument fits into
// /*AFLA*/  * that type, or otherwise unsigned long long.
// /*AFLA*/  */
// /*AFLA*/ #define __inttype(x) \
// /*AFLA*/ __typeof__(__builtin_choose_expr(sizeof(x) > sizeof(0UL), 0ULL, 0UL))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * get_user: - Get a simple variable from user space.
// /*AFLA*/  * @x:   Variable to store result.
// /*AFLA*/  * @ptr: Source address, in user space.
// /*AFLA*/  *
// /*AFLA*/  * Context: User context only. This function may sleep if pagefaults are
// /*AFLA*/  *          enabled.
// /*AFLA*/  *
// /*AFLA*/  * This macro copies a single simple variable from user space to kernel
// /*AFLA*/  * space.  It supports simple types like char and int, but not larger
// /*AFLA*/  * data types like structures or arrays.
// /*AFLA*/  *
// /*AFLA*/  * @ptr must have pointer-to-simple-variable type, and the result of
// /*AFLA*/  * dereferencing @ptr must be assignable to @x without a cast.
// /*AFLA*/  *
// /*AFLA*/  * Returns zero on success, or -EFAULT on error.
// /*AFLA*/  * On error, the variable @x is set to zero.
// /*AFLA*/  */
// /*AFLA*/ /*
// /*AFLA*/  * Careful: we have to cast the result to the type of the pointer
// /*AFLA*/  * for sign reasons.
// /*AFLA*/  *
// /*AFLA*/  * The use of _ASM_DX as the register specifier is a bit of a
// /*AFLA*/  * simplification, as gcc only cares about it as the starting point
// /*AFLA*/  * and not size: for a 64-bit value it will use %ecx:%edx on 32 bits
// /*AFLA*/  * (%ecx being the next register in gcc's x86 register sequence), and
// /*AFLA*/  * %rdx on 64 bits.
// /*AFLA*/  *
// /*AFLA*/  * Clang/LLVM cares about the size of the register, but still wants
// /*AFLA*/  * the base register for something that ends up being a pair.
// /*AFLA*/  */
// /*AFLA*/ #define get_user(x, ptr)						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	int __ret_gu;							\
// /*AFLA*/ 	register __inttype(*(ptr)) __val_gu asm("%"_ASM_DX);		\
// /*AFLA*/ 	register void *__sp asm(_ASM_SP);				\
// /*AFLA*/ 	__chk_user_ptr(ptr);						\
// /*AFLA*/ 	might_fault();							\
// /*AFLA*/ 	asm volatile("call __get_user_%P4"				\
// /*AFLA*/ 		     : "=a" (__ret_gu), "=r" (__val_gu), "+r" (__sp)	\
// /*AFLA*/ 		     : "0" (ptr), "i" (sizeof(*(ptr))));		\
// /*AFLA*/ 	(x) = (__force __typeof__(*(ptr))) __val_gu;			\
// /*AFLA*/ 	__builtin_expect(__ret_gu, 0);					\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __put_user_x(size, x, ptr, __ret_pu)			\
// /*AFLA*/ 	asm volatile("call __put_user_" #size : "=a" (__ret_pu)	\
// /*AFLA*/ 		     : "0" ((typeof(*(ptr)))(x)), "c" (ptr) : "ebx")
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ #define __put_user_asm_u64(x, addr, err, errret)			\
// /*AFLA*/ 	asm volatile("\n"						\
// /*AFLA*/ 		     "1:	movl %%eax,0(%2)\n"			\
// /*AFLA*/ 		     "2:	movl %%edx,4(%2)\n"			\
// /*AFLA*/ 		     "3:"						\
// /*AFLA*/ 		     ".section .fixup,\"ax\"\n"				\
// /*AFLA*/ 		     "4:	movl %3,%0\n"				\
// /*AFLA*/ 		     "	jmp 3b\n"					\
// /*AFLA*/ 		     ".previous\n"					\
// /*AFLA*/ 		     _ASM_EXTABLE(1b, 4b)				\
// /*AFLA*/ 		     _ASM_EXTABLE(2b, 4b)				\
// /*AFLA*/ 		     : "=r" (err)					\
// /*AFLA*/ 		     : "A" (x), "r" (addr), "i" (errret), "0" (err))
// /*AFLA*/ 
// /*AFLA*/ #define __put_user_asm_ex_u64(x, addr)					\
// /*AFLA*/ 	asm volatile("\n"						\
// /*AFLA*/ 		     "1:	movl %%eax,0(%1)\n"			\
// /*AFLA*/ 		     "2:	movl %%edx,4(%1)\n"			\
// /*AFLA*/ 		     "3:"						\
// /*AFLA*/ 		     _ASM_EXTABLE_EX(1b, 2b)				\
// /*AFLA*/ 		     _ASM_EXTABLE_EX(2b, 3b)				\
// /*AFLA*/ 		     : : "A" (x), "r" (addr))
// /*AFLA*/ 
// /*AFLA*/ #define __put_user_x8(x, ptr, __ret_pu)				\
// /*AFLA*/ 	asm volatile("call __put_user_8" : "=a" (__ret_pu)	\
// /*AFLA*/ 		     : "A" ((typeof(*(ptr)))(x)), "c" (ptr) : "ebx")
// /*AFLA*/ #else
// /*AFLA*/ #define __put_user_asm_u64(x, ptr, retval, errret) \
// /*AFLA*/ 	__put_user_asm(x, ptr, retval, "q", "", "er", errret)
// /*AFLA*/ #define __put_user_asm_ex_u64(x, addr)	\
// /*AFLA*/ 	__put_user_asm_ex(x, addr, "q", "", "er")
// /*AFLA*/ #define __put_user_x8(x, ptr, __ret_pu) __put_user_x(8, x, ptr, __ret_pu)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void __put_user_bad(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Strange magic calling convention: pointer in %ecx,
// /*AFLA*/  * value in %eax(:%edx), return value in %eax. clobbers %rbx
// /*AFLA*/  */
// /*AFLA*/ extern void __put_user_1(void);
// /*AFLA*/ extern void __put_user_2(void);
// /*AFLA*/ extern void __put_user_4(void);
// /*AFLA*/ extern void __put_user_8(void);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * put_user: - Write a simple value into user space.
// /*AFLA*/  * @x:   Value to copy to user space.
// /*AFLA*/  * @ptr: Destination address, in user space.
// /*AFLA*/  *
// /*AFLA*/  * Context: User context only. This function may sleep if pagefaults are
// /*AFLA*/  *          enabled.
// /*AFLA*/  *
// /*AFLA*/  * This macro copies a single simple value from kernel space to user
// /*AFLA*/  * space.  It supports simple types like char and int, but not larger
// /*AFLA*/  * data types like structures or arrays.
// /*AFLA*/  *
// /*AFLA*/  * @ptr must have pointer-to-simple-variable type, and @x must be assignable
// /*AFLA*/  * to the result of dereferencing @ptr.
// /*AFLA*/  *
// /*AFLA*/  * Returns zero on success, or -EFAULT on error.
// /*AFLA*/  */
// /*AFLA*/ #define put_user(x, ptr)					\
// /*AFLA*/ ({								\
// /*AFLA*/ 	int __ret_pu;						\
// /*AFLA*/ 	__typeof__(*(ptr)) __pu_val;				\
// /*AFLA*/ 	__chk_user_ptr(ptr);					\
// /*AFLA*/ 	might_fault();						\
// /*AFLA*/ 	__pu_val = x;						\
// /*AFLA*/ 	switch (sizeof(*(ptr))) {				\
// /*AFLA*/ 	case 1:							\
// /*AFLA*/ 		__put_user_x(1, __pu_val, ptr, __ret_pu);	\
// /*AFLA*/ 		break;						\
// /*AFLA*/ 	case 2:							\
// /*AFLA*/ 		__put_user_x(2, __pu_val, ptr, __ret_pu);	\
// /*AFLA*/ 		break;						\
// /*AFLA*/ 	case 4:							\
// /*AFLA*/ 		__put_user_x(4, __pu_val, ptr, __ret_pu);	\
// /*AFLA*/ 		break;						\
// /*AFLA*/ 	case 8:							\
// /*AFLA*/ 		__put_user_x8(__pu_val, ptr, __ret_pu);		\
// /*AFLA*/ 		break;						\
// /*AFLA*/ 	default:						\
// /*AFLA*/ 		__put_user_x(X, __pu_val, ptr, __ret_pu);	\
// /*AFLA*/ 		break;						\
// /*AFLA*/ 	}							\
// /*AFLA*/ 	__builtin_expect(__ret_pu, 0);				\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __put_user_size(x, ptr, size, retval, errret)			\
// /*AFLA*/ do {									\
// /*AFLA*/ 	retval = 0;							\
// /*AFLA*/ 	__chk_user_ptr(ptr);						\
// /*AFLA*/ 	switch (size) {							\
// /*AFLA*/ 	case 1:								\
// /*AFLA*/ 		__put_user_asm(x, ptr, retval, "b", "b", "iq", errret);	\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 2:								\
// /*AFLA*/ 		__put_user_asm(x, ptr, retval, "w", "w", "ir", errret);	\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 4:								\
// /*AFLA*/ 		__put_user_asm(x, ptr, retval, "l", "k", "ir", errret);	\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 8:								\
// /*AFLA*/ 		__put_user_asm_u64((__typeof__(*ptr))(x), ptr, retval,	\
// /*AFLA*/ 				   errret);				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	default:							\
// /*AFLA*/ 		__put_user_bad();					\
// /*AFLA*/ 	}								\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This doesn't do __uaccess_begin/end - the exception handling
// /*AFLA*/  * around it must do that.
// /*AFLA*/  */
// /*AFLA*/ #define __put_user_size_ex(x, ptr, size)				\
// /*AFLA*/ do {									\
// /*AFLA*/ 	__chk_user_ptr(ptr);						\
// /*AFLA*/ 	switch (size) {							\
// /*AFLA*/ 	case 1:								\
// /*AFLA*/ 		__put_user_asm_ex(x, ptr, "b", "b", "iq");		\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 2:								\
// /*AFLA*/ 		__put_user_asm_ex(x, ptr, "w", "w", "ir");		\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 4:								\
// /*AFLA*/ 		__put_user_asm_ex(x, ptr, "l", "k", "ir");		\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 8:								\
// /*AFLA*/ 		__put_user_asm_ex_u64((__typeof__(*ptr))(x), ptr);	\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	default:							\
// /*AFLA*/ 		__put_user_bad();					\
// /*AFLA*/ 	}								\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ #define __get_user_asm_u64(x, ptr, retval, errret)			\
// /*AFLA*/ ({									\
// /*AFLA*/ 	__typeof__(ptr) __ptr = (ptr);					\
// /*AFLA*/ 	asm volatile(ASM_STAC "\n"					\
// /*AFLA*/ 		     "1:	movl %2,%%eax\n"			\
// /*AFLA*/ 		     "2:	movl %3,%%edx\n"			\
// /*AFLA*/ 		     "3: " ASM_CLAC "\n"				\
// /*AFLA*/ 		     ".section .fixup,\"ax\"\n"				\
// /*AFLA*/ 		     "4:	mov %4,%0\n"				\
// /*AFLA*/ 		     "	xorl %%eax,%%eax\n"				\
// /*AFLA*/ 		     "	xorl %%edx,%%edx\n"				\
// /*AFLA*/ 		     "	jmp 3b\n"					\
// /*AFLA*/ 		     ".previous\n"					\
// /*AFLA*/ 		     _ASM_EXTABLE(1b, 4b)				\
// /*AFLA*/ 		     _ASM_EXTABLE(2b, 4b)				\
// /*AFLA*/ 		     : "=r" (retval), "=A"(x)				\
// /*AFLA*/ 		     : "m" (__m(__ptr)), "m" __m(((u32 *)(__ptr)) + 1),	\
// /*AFLA*/ 		       "i" (errret), "0" (retval));			\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __get_user_asm_ex_u64(x, ptr)			(x) = __get_user_bad()
// /*AFLA*/ #else
// /*AFLA*/ #define __get_user_asm_u64(x, ptr, retval, errret) \
// /*AFLA*/ 	 __get_user_asm(x, ptr, retval, "q", "", "=r", errret)
// /*AFLA*/ #define __get_user_asm_ex_u64(x, ptr) \
// /*AFLA*/ 	 __get_user_asm_ex(x, ptr, "q", "", "=r")
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __get_user_size(x, ptr, size, retval, errret)			\
// /*AFLA*/ do {									\
// /*AFLA*/ 	retval = 0;							\
// /*AFLA*/ 	__chk_user_ptr(ptr);						\
// /*AFLA*/ 	switch (size) {							\
// /*AFLA*/ 	case 1:								\
// /*AFLA*/ 		__get_user_asm(x, ptr, retval, "b", "b", "=q", errret);	\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 2:								\
// /*AFLA*/ 		__get_user_asm(x, ptr, retval, "w", "w", "=r", errret);	\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 4:								\
// /*AFLA*/ 		__get_user_asm(x, ptr, retval, "l", "k", "=r", errret);	\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 8:								\
// /*AFLA*/ 		__get_user_asm_u64(x, ptr, retval, errret);		\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	default:							\
// /*AFLA*/ 		(x) = __get_user_bad();					\
// /*AFLA*/ 	}								\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define __get_user_asm(x, addr, err, itype, rtype, ltype, errret)	\
// /*AFLA*/ 	asm volatile("\n"						\
// /*AFLA*/ 		     "1:	mov"itype" %2,%"rtype"1\n"		\
// /*AFLA*/ 		     "2:\n"						\
// /*AFLA*/ 		     ".section .fixup,\"ax\"\n"				\
// /*AFLA*/ 		     "3:	mov %3,%0\n"				\
// /*AFLA*/ 		     "	xor"itype" %"rtype"1,%"rtype"1\n"		\
// /*AFLA*/ 		     "	jmp 2b\n"					\
// /*AFLA*/ 		     ".previous\n"					\
// /*AFLA*/ 		     _ASM_EXTABLE(1b, 3b)				\
// /*AFLA*/ 		     : "=r" (err), ltype(x)				\
// /*AFLA*/ 		     : "m" (__m(addr)), "i" (errret), "0" (err))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This doesn't do __uaccess_begin/end - the exception handling
// /*AFLA*/  * around it must do that.
// /*AFLA*/  */
// /*AFLA*/ #define __get_user_size_ex(x, ptr, size)				\
// /*AFLA*/ do {									\
// /*AFLA*/ 	__chk_user_ptr(ptr);						\
// /*AFLA*/ 	switch (size) {							\
// /*AFLA*/ 	case 1:								\
// /*AFLA*/ 		__get_user_asm_ex(x, ptr, "b", "b", "=q");		\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 2:								\
// /*AFLA*/ 		__get_user_asm_ex(x, ptr, "w", "w", "=r");		\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 4:								\
// /*AFLA*/ 		__get_user_asm_ex(x, ptr, "l", "k", "=r");		\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	case 8:								\
// /*AFLA*/ 		__get_user_asm_ex_u64(x, ptr);				\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	default:							\
// /*AFLA*/ 		(x) = __get_user_bad();					\
// /*AFLA*/ 	}								\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define __get_user_asm_ex(x, addr, itype, rtype, ltype)			\
// /*AFLA*/ 	asm volatile("1:	mov"itype" %1,%"rtype"0\n"		\
// /*AFLA*/ 		     "2:\n"						\
// /*AFLA*/ 		     ".section .fixup,\"ax\"\n"				\
// /*AFLA*/                      "3:xor"itype" %"rtype"0,%"rtype"0\n"		\
// /*AFLA*/ 		     "  jmp 2b\n"					\
// /*AFLA*/ 		     ".previous\n"					\
// /*AFLA*/ 		     _ASM_EXTABLE_EX(1b, 3b)				\
// /*AFLA*/ 		     : ltype(x) : "m" (__m(addr)))
// /*AFLA*/ 
// /*AFLA*/ #define __put_user_nocheck(x, ptr, size)			\
// /*AFLA*/ ({								\
// /*AFLA*/ 	int __pu_err;						\
// /*AFLA*/ 	__uaccess_begin();					\
// /*AFLA*/ 	__put_user_size((x), (ptr), (size), __pu_err, -EFAULT);	\
// /*AFLA*/ 	__uaccess_end();					\
// /*AFLA*/ 	__builtin_expect(__pu_err, 0);				\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __get_user_nocheck(x, ptr, size)				\
// /*AFLA*/ ({									\
// /*AFLA*/ 	int __gu_err;							\
// /*AFLA*/ 	__inttype(*(ptr)) __gu_val;					\
// /*AFLA*/ 	__uaccess_begin();						\
// /*AFLA*/ 	__get_user_size(__gu_val, (ptr), (size), __gu_err, -EFAULT);	\
// /*AFLA*/ 	__uaccess_end();						\
// /*AFLA*/ 	(x) = (__force __typeof__(*(ptr)))__gu_val;			\
// /*AFLA*/ 	__builtin_expect(__gu_err, 0);					\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /* FIXME: this hack is definitely wrong -AK */
// /*AFLA*/ struct __large_struct { unsigned long buf[100]; };
// /*AFLA*/ #define __m(x) (*(struct __large_struct __user *)(x))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Tell gcc we read from memory instead of writing: this is because
// /*AFLA*/  * we do not write to any memory gcc knows about, so there are no
// /*AFLA*/  * aliasing issues.
// /*AFLA*/  */
// /*AFLA*/ #define __put_user_asm(x, addr, err, itype, rtype, ltype, errret)	\
// /*AFLA*/ 	asm volatile("\n"						\
// /*AFLA*/ 		     "1:	mov"itype" %"rtype"1,%2\n"		\
// /*AFLA*/ 		     "2:\n"						\
// /*AFLA*/ 		     ".section .fixup,\"ax\"\n"				\
// /*AFLA*/ 		     "3:	mov %3,%0\n"				\
// /*AFLA*/ 		     "	jmp 2b\n"					\
// /*AFLA*/ 		     ".previous\n"					\
// /*AFLA*/ 		     _ASM_EXTABLE(1b, 3b)				\
// /*AFLA*/ 		     : "=r"(err)					\
// /*AFLA*/ 		     : ltype(x), "m" (__m(addr)), "i" (errret), "0" (err))
// /*AFLA*/ 
// /*AFLA*/ #define __put_user_asm_ex(x, addr, itype, rtype, ltype)			\
// /*AFLA*/ 	asm volatile("1:	mov"itype" %"rtype"0,%1\n"		\
// /*AFLA*/ 		     "2:\n"						\
// /*AFLA*/ 		     _ASM_EXTABLE_EX(1b, 2b)				\
// /*AFLA*/ 		     : : ltype(x), "m" (__m(addr)))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * uaccess_try and catch
// /*AFLA*/  */
// /*AFLA*/ #define uaccess_try	do {						\
// /*AFLA*/ 	current->thread.uaccess_err = 0;				\
// /*AFLA*/ 	__uaccess_begin();						\
// /*AFLA*/ 	barrier();
// /*AFLA*/ 
// /*AFLA*/ #define uaccess_catch(err)						\
// /*AFLA*/ 	__uaccess_end();						\
// /*AFLA*/ 	(err) |= (current->thread.uaccess_err ? -EFAULT : 0);		\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __get_user: - Get a simple variable from user space, with less checking.
// /*AFLA*/  * @x:   Variable to store result.
// /*AFLA*/  * @ptr: Source address, in user space.
// /*AFLA*/  *
// /*AFLA*/  * Context: User context only. This function may sleep if pagefaults are
// /*AFLA*/  *          enabled.
// /*AFLA*/  *
// /*AFLA*/  * This macro copies a single simple variable from user space to kernel
// /*AFLA*/  * space.  It supports simple types like char and int, but not larger
// /*AFLA*/  * data types like structures or arrays.
// /*AFLA*/  *
// /*AFLA*/  * @ptr must have pointer-to-simple-variable type, and the result of
// /*AFLA*/  * dereferencing @ptr must be assignable to @x without a cast.
// /*AFLA*/  *
// /*AFLA*/  * Caller must check the pointer with access_ok() before calling this
// /*AFLA*/  * function.
// /*AFLA*/  *
// /*AFLA*/  * Returns zero on success, or -EFAULT on error.
// /*AFLA*/  * On error, the variable @x is set to zero.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define __get_user(x, ptr)						\
// /*AFLA*/ 	__get_user_nocheck((x), (ptr), sizeof(*(ptr)))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __put_user: - Write a simple value into user space, with less checking.
// /*AFLA*/  * @x:   Value to copy to user space.
// /*AFLA*/  * @ptr: Destination address, in user space.
// /*AFLA*/  *
// /*AFLA*/  * Context: User context only. This function may sleep if pagefaults are
// /*AFLA*/  *          enabled.
// /*AFLA*/  *
// /*AFLA*/  * This macro copies a single simple value from kernel space to user
// /*AFLA*/  * space.  It supports simple types like char and int, but not larger
// /*AFLA*/  * data types like structures or arrays.
// /*AFLA*/  *
// /*AFLA*/  * @ptr must have pointer-to-simple-variable type, and @x must be assignable
// /*AFLA*/  * to the result of dereferencing @ptr.
// /*AFLA*/  *
// /*AFLA*/  * Caller must check the pointer with access_ok() before calling this
// /*AFLA*/  * function.
// /*AFLA*/  *
// /*AFLA*/  * Returns zero on success, or -EFAULT on error.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define __put_user(x, ptr)						\
// /*AFLA*/ 	__put_user_nocheck((__typeof__(*(ptr)))(x), (ptr), sizeof(*(ptr)))
// /*AFLA*/ 
// /*AFLA*/ #define __get_user_unaligned __get_user
// /*AFLA*/ #define __put_user_unaligned __put_user
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * {get|put}_user_try and catch
// /*AFLA*/  *
// /*AFLA*/  * get_user_try {
// /*AFLA*/  *	get_user_ex(...);
// /*AFLA*/  * } get_user_catch(err)
// /*AFLA*/  */
// /*AFLA*/ #define get_user_try		uaccess_try
// /*AFLA*/ #define get_user_catch(err)	uaccess_catch(err)
// /*AFLA*/ 
// /*AFLA*/ #define get_user_ex(x, ptr)	do {					\
// /*AFLA*/ 	unsigned long __gue_val;					\
// /*AFLA*/ 	__get_user_size_ex((__gue_val), (ptr), (sizeof(*(ptr))));	\
// /*AFLA*/ 	(x) = (__force __typeof__(*(ptr)))__gue_val;			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define put_user_try		uaccess_try
// /*AFLA*/ #define put_user_catch(err)	uaccess_catch(err)
// /*AFLA*/ 
// /*AFLA*/ #define put_user_ex(x, ptr)						\
// /*AFLA*/ 	__put_user_size_ex((__typeof__(*(ptr)))(x), (ptr), sizeof(*(ptr)))
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long
// /*AFLA*/ copy_from_user_nmi(void *to, const void __user *from, unsigned long n);
// /*AFLA*/ extern __must_check long
// /*AFLA*/ strncpy_from_user(char *dst, const char __user *src, long count);
// /*AFLA*/ 
// /*AFLA*/ extern __must_check long strlen_user(const char __user *str);
// /*AFLA*/ extern __must_check long strnlen_user(const char __user *str, long n);
// /*AFLA*/ 
// /*AFLA*/ unsigned long __must_check clear_user(void __user *mem, unsigned long len);
// /*AFLA*/ unsigned long __must_check __clear_user(void __user *mem, unsigned long len);
// /*AFLA*/ 
// /*AFLA*/ extern void __cmpxchg_wrong_size(void)
// /*AFLA*/ 	__compiletime_error("Bad argument size for cmpxchg");
// /*AFLA*/ 
// /*AFLA*/ #define __user_atomic_cmpxchg_inatomic(uval, ptr, old, new, size)	\
// /*AFLA*/ ({									\
// /*AFLA*/ 	int __ret = 0;							\
// /*AFLA*/ 	__typeof__(ptr) __uval = (uval);				\
// /*AFLA*/ 	__typeof__(*(ptr)) __old = (old);				\
// /*AFLA*/ 	__typeof__(*(ptr)) __new = (new);				\
// /*AFLA*/ 	__uaccess_begin();						\
// /*AFLA*/ 	switch (size) {							\
// /*AFLA*/ 	case 1:								\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		asm volatile("\n"					\
// /*AFLA*/ 			"1:\t" LOCK_PREFIX "cmpxchgb %4, %2\n"		\
// /*AFLA*/ 			"2:\n"						\
// /*AFLA*/ 			"\t.section .fixup, \"ax\"\n"			\
// /*AFLA*/ 			"3:\tmov     %3, %0\n"				\
// /*AFLA*/ 			"\tjmp     2b\n"				\
// /*AFLA*/ 			"\t.previous\n"					\
// /*AFLA*/ 			_ASM_EXTABLE(1b, 3b)				\
// /*AFLA*/ 			: "+r" (__ret), "=a" (__old), "+m" (*(ptr))	\
// /*AFLA*/ 			: "i" (-EFAULT), "q" (__new), "1" (__old)	\
// /*AFLA*/ 			: "memory"					\
// /*AFLA*/ 		);							\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	case 2:								\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		asm volatile("\n"					\
// /*AFLA*/ 			"1:\t" LOCK_PREFIX "cmpxchgw %4, %2\n"		\
// /*AFLA*/ 			"2:\n"						\
// /*AFLA*/ 			"\t.section .fixup, \"ax\"\n"			\
// /*AFLA*/ 			"3:\tmov     %3, %0\n"				\
// /*AFLA*/ 			"\tjmp     2b\n"				\
// /*AFLA*/ 			"\t.previous\n"					\
// /*AFLA*/ 			_ASM_EXTABLE(1b, 3b)				\
// /*AFLA*/ 			: "+r" (__ret), "=a" (__old), "+m" (*(ptr))	\
// /*AFLA*/ 			: "i" (-EFAULT), "r" (__new), "1" (__old)	\
// /*AFLA*/ 			: "memory"					\
// /*AFLA*/ 		);							\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	case 4:								\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		asm volatile("\n"					\
// /*AFLA*/ 			"1:\t" LOCK_PREFIX "cmpxchgl %4, %2\n"		\
// /*AFLA*/ 			"2:\n"						\
// /*AFLA*/ 			"\t.section .fixup, \"ax\"\n"			\
// /*AFLA*/ 			"3:\tmov     %3, %0\n"				\
// /*AFLA*/ 			"\tjmp     2b\n"				\
// /*AFLA*/ 			"\t.previous\n"					\
// /*AFLA*/ 			_ASM_EXTABLE(1b, 3b)				\
// /*AFLA*/ 			: "+r" (__ret), "=a" (__old), "+m" (*(ptr))	\
// /*AFLA*/ 			: "i" (-EFAULT), "r" (__new), "1" (__old)	\
// /*AFLA*/ 			: "memory"					\
// /*AFLA*/ 		);							\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	case 8:								\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		if (!IS_ENABLED(CONFIG_X86_64))				\
// /*AFLA*/ 			__cmpxchg_wrong_size();				\
// /*AFLA*/ 									\
// /*AFLA*/ 		asm volatile("\n"					\
// /*AFLA*/ 			"1:\t" LOCK_PREFIX "cmpxchgq %4, %2\n"		\
// /*AFLA*/ 			"2:\n"						\
// /*AFLA*/ 			"\t.section .fixup, \"ax\"\n"			\
// /*AFLA*/ 			"3:\tmov     %3, %0\n"				\
// /*AFLA*/ 			"\tjmp     2b\n"				\
// /*AFLA*/ 			"\t.previous\n"					\
// /*AFLA*/ 			_ASM_EXTABLE(1b, 3b)				\
// /*AFLA*/ 			: "+r" (__ret), "=a" (__old), "+m" (*(ptr))	\
// /*AFLA*/ 			: "i" (-EFAULT), "r" (__new), "1" (__old)	\
// /*AFLA*/ 			: "memory"					\
// /*AFLA*/ 		);							\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	default:							\
// /*AFLA*/ 		__cmpxchg_wrong_size();					\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	__uaccess_end();						\
// /*AFLA*/ 	*__uval = __old;						\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define user_atomic_cmpxchg_inatomic(uval, ptr, old, new)		\
// /*AFLA*/ ({									\
// /*AFLA*/ 	access_ok(VERIFY_WRITE, (ptr), sizeof(*(ptr))) ?		\
// /*AFLA*/ 		__user_atomic_cmpxchg_inatomic((uval), (ptr),		\
// /*AFLA*/ 				(old), (new), sizeof(*(ptr))) :		\
// /*AFLA*/ 		-EFAULT;						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * movsl can be slow when source and dest are not both 8-byte aligned
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_X86_INTEL_USERCOPY
// /*AFLA*/ extern struct movsl_mask {
// /*AFLA*/ 	int mask;
// /*AFLA*/ } ____cacheline_aligned_in_smp movsl_mask;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define ARCH_HAS_NOCACHE_UACCESS 1
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ # include <asm/uaccess_32.h>
// /*AFLA*/ #else
// /*AFLA*/ # include <asm/uaccess_64.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ unsigned long __must_check _copy_from_user(void *to, const void __user *from,
// /*AFLA*/ 					   unsigned n);
// /*AFLA*/ unsigned long __must_check _copy_to_user(void __user *to, const void *from,
// /*AFLA*/ 					 unsigned n);
// /*AFLA*/ 
// /*AFLA*/ extern void __compiletime_error("usercopy buffer size is too small")
// /*AFLA*/ __bad_copy_user(void);
// /*AFLA*/ 
// /*AFLA*/ static inline void copy_user_overflow(int size, unsigned long count)
// /*AFLA*/ {
// /*AFLA*/ 	WARN(1, "Buffer overflow detected (%d < %lu)!\n", size, count);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline unsigned long __must_check
// /*AFLA*/ copy_from_user(void *to, const void __user *from, unsigned long n)
// /*AFLA*/ {
// /*AFLA*/ 	int sz = __compiletime_object_size(to);
// /*AFLA*/ 
// /*AFLA*/ 	might_fault();
// /*AFLA*/ 
// /*AFLA*/ 	kasan_check_write(to, n);
// /*AFLA*/ 
// /*AFLA*/ 	if (likely(sz < 0 || sz >= n)) {
// /*AFLA*/ 		check_object_size(to, n, false);
// /*AFLA*/ 		n = _copy_from_user(to, from, n);
// /*AFLA*/ 	} else if (!__builtin_constant_p(n))
// /*AFLA*/ 		copy_user_overflow(sz, n);
// /*AFLA*/ 	else
// /*AFLA*/ 		__bad_copy_user();
// /*AFLA*/ 
// /*AFLA*/ 	return n;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline unsigned long __must_check
// /*AFLA*/ copy_to_user(void __user *to, const void *from, unsigned long n)
// /*AFLA*/ {
// /*AFLA*/ 	int sz = __compiletime_object_size(from);
// /*AFLA*/ 
// /*AFLA*/ 	kasan_check_read(from, n);
// /*AFLA*/ 
// /*AFLA*/ 	might_fault();
// /*AFLA*/ 
// /*AFLA*/ 	if (likely(sz < 0 || sz >= n)) {
// /*AFLA*/ 		check_object_size(from, n, true);
// /*AFLA*/ 		n = _copy_to_user(to, from, n);
// /*AFLA*/ 	} else if (!__builtin_constant_p(n))
// /*AFLA*/ 		copy_user_overflow(sz, n);
// /*AFLA*/ 	else
// /*AFLA*/ 		__bad_copy_user();
// /*AFLA*/ 
// /*AFLA*/ 	return n;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We rely on the nested NMI work to allow atomic faults from the NMI path; the
// /*AFLA*/  * nested NMI paths are careful to preserve CR2.
// /*AFLA*/  *
// /*AFLA*/  * Caller must use pagefault_enable/disable, or run in interrupt context,
// /*AFLA*/  * and also do a uaccess_ok() check
// /*AFLA*/  */
// /*AFLA*/ #define __copy_from_user_nmi __copy_from_user_inatomic
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The "unsafe" user accesses aren't really "unsafe", but the naming
// /*AFLA*/  * is a big fat warning: you have to not only do the access_ok()
// /*AFLA*/  * checking before using them, but you have to surround them with the
// /*AFLA*/  * user_access_begin/end() pair.
// /*AFLA*/  */
// /*AFLA*/ #define user_access_begin()	__uaccess_begin()
// /*AFLA*/ #define user_access_end()	__uaccess_end()
// /*AFLA*/ 
// /*AFLA*/ #define unsafe_put_user(x, ptr, err_label)					\
// /*AFLA*/ do {										\
// /*AFLA*/ 	int __pu_err;								\
// /*AFLA*/ 	__put_user_size((x), (ptr), sizeof(*(ptr)), __pu_err, -EFAULT);		\
// /*AFLA*/ 	if (unlikely(__pu_err)) goto err_label;					\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define unsafe_get_user(x, ptr, err_label)					\
// /*AFLA*/ do {										\
// /*AFLA*/ 	int __gu_err;								\
// /*AFLA*/ 	unsigned long __gu_val;							\
// /*AFLA*/ 	__get_user_size(__gu_val, (ptr), sizeof(*(ptr)), __gu_err, -EFAULT);	\
// /*AFLA*/ 	(x) = (__force __typeof__(*(ptr)))__gu_val;				\
// /*AFLA*/ 	if (unlikely(__gu_err)) goto err_label;					\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_UACCESS_H */
// /*AFLA*/ 
