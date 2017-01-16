// /*AFLA*/ #ifndef _ASM_X86_UACCESS_64_H
// /*AFLA*/ #define _ASM_X86_UACCESS_64_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * User space memory access functions
// /*AFLA*/  */
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ #include <linux/lockdep.h>
// /*AFLA*/ #include <linux/kasan-checks.h>
// /*AFLA*/ #include <asm/alternative.h>
// /*AFLA*/ #include <asm/cpufeatures.h>
// /*AFLA*/ #include <asm/page.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Copy To/From Userspace
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* Handles exceptions in both to and from, but doesn't do access_ok */
// /*AFLA*/ __must_check unsigned long
// /*AFLA*/ copy_user_enhanced_fast_string(void *to, const void *from, unsigned len);
// /*AFLA*/ __must_check unsigned long
// /*AFLA*/ copy_user_generic_string(void *to, const void *from, unsigned len);
// /*AFLA*/ __must_check unsigned long
// /*AFLA*/ copy_user_generic_unrolled(void *to, const void *from, unsigned len);
// /*AFLA*/ 
// /*AFLA*/ static __always_inline __must_check unsigned long
// /*AFLA*/ copy_user_generic(void *to, const void *from, unsigned len)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned ret;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If CPU has ERMS feature, use copy_user_enhanced_fast_string.
// /*AFLA*/ 	 * Otherwise, if CPU has rep_good feature, use copy_user_generic_string.
// /*AFLA*/ 	 * Otherwise, use copy_user_generic_unrolled.
// /*AFLA*/ 	 */
// /*AFLA*/ 	alternative_call_2(copy_user_generic_unrolled,
// /*AFLA*/ 			 copy_user_generic_string,
// /*AFLA*/ 			 X86_FEATURE_REP_GOOD,
// /*AFLA*/ 			 copy_user_enhanced_fast_string,
// /*AFLA*/ 			 X86_FEATURE_ERMS,
// /*AFLA*/ 			 ASM_OUTPUT2("=a" (ret), "=D" (to), "=S" (from),
// /*AFLA*/ 				     "=d" (len)),
// /*AFLA*/ 			 "1" (to), "2" (from), "3" (len)
// /*AFLA*/ 			 : "memory", "rcx", "r8", "r9", "r10", "r11");
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ __must_check unsigned long
// /*AFLA*/ copy_in_user(void __user *to, const void __user *from, unsigned len);
// /*AFLA*/ 
// /*AFLA*/ static __always_inline __must_check
// /*AFLA*/ int __copy_from_user_nocheck(void *dst, const void __user *src, unsigned size)
// /*AFLA*/ {
// /*AFLA*/ 	int ret = 0;
// /*AFLA*/ 
// /*AFLA*/ 	check_object_size(dst, size, false);
// /*AFLA*/ 	if (!__builtin_constant_p(size))
// /*AFLA*/ 		return copy_user_generic(dst, (__force void *)src, size);
// /*AFLA*/ 	switch (size) {
// /*AFLA*/ 	case 1:
// /*AFLA*/ 		__uaccess_begin();
// /*AFLA*/ 		__get_user_asm(*(u8 *)dst, (u8 __user *)src,
// /*AFLA*/ 			      ret, "b", "b", "=q", 1);
// /*AFLA*/ 		__uaccess_end();
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	case 2:
// /*AFLA*/ 		__uaccess_begin();
// /*AFLA*/ 		__get_user_asm(*(u16 *)dst, (u16 __user *)src,
// /*AFLA*/ 			      ret, "w", "w", "=r", 2);
// /*AFLA*/ 		__uaccess_end();
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	case 4:
// /*AFLA*/ 		__uaccess_begin();
// /*AFLA*/ 		__get_user_asm(*(u32 *)dst, (u32 __user *)src,
// /*AFLA*/ 			      ret, "l", "k", "=r", 4);
// /*AFLA*/ 		__uaccess_end();
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	case 8:
// /*AFLA*/ 		__uaccess_begin();
// /*AFLA*/ 		__get_user_asm(*(u64 *)dst, (u64 __user *)src,
// /*AFLA*/ 			      ret, "q", "", "=r", 8);
// /*AFLA*/ 		__uaccess_end();
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	case 10:
// /*AFLA*/ 		__uaccess_begin();
// /*AFLA*/ 		__get_user_asm(*(u64 *)dst, (u64 __user *)src,
// /*AFLA*/ 			       ret, "q", "", "=r", 10);
// /*AFLA*/ 		if (likely(!ret))
// /*AFLA*/ 			__get_user_asm(*(u16 *)(8 + (char *)dst),
// /*AFLA*/ 				       (u16 __user *)(8 + (char __user *)src),
// /*AFLA*/ 				       ret, "w", "w", "=r", 2);
// /*AFLA*/ 		__uaccess_end();
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	case 16:
// /*AFLA*/ 		__uaccess_begin();
// /*AFLA*/ 		__get_user_asm(*(u64 *)dst, (u64 __user *)src,
// /*AFLA*/ 			       ret, "q", "", "=r", 16);
// /*AFLA*/ 		if (likely(!ret))
// /*AFLA*/ 			__get_user_asm(*(u64 *)(8 + (char *)dst),
// /*AFLA*/ 				       (u64 __user *)(8 + (char __user *)src),
// /*AFLA*/ 				       ret, "q", "", "=r", 8);
// /*AFLA*/ 		__uaccess_end();
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	default:
// /*AFLA*/ 		return copy_user_generic(dst, (__force void *)src, size);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline __must_check
// /*AFLA*/ int __copy_from_user(void *dst, const void __user *src, unsigned size)
// /*AFLA*/ {
// /*AFLA*/ 	might_fault();
// /*AFLA*/ 	kasan_check_write(dst, size);
// /*AFLA*/ 	return __copy_from_user_nocheck(dst, src, size);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline __must_check
// /*AFLA*/ int __copy_to_user_nocheck(void __user *dst, const void *src, unsigned size)
// /*AFLA*/ {
// /*AFLA*/ 	int ret = 0;
// /*AFLA*/ 
// /*AFLA*/ 	check_object_size(src, size, true);
// /*AFLA*/ 	if (!__builtin_constant_p(size))
// /*AFLA*/ 		return copy_user_generic((__force void *)dst, src, size);
// /*AFLA*/ 	switch (size) {
// /*AFLA*/ 	case 1:
// /*AFLA*/ 		__uaccess_begin();
// /*AFLA*/ 		__put_user_asm(*(u8 *)src, (u8 __user *)dst,
// /*AFLA*/ 			      ret, "b", "b", "iq", 1);
// /*AFLA*/ 		__uaccess_end();
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	case 2:
// /*AFLA*/ 		__uaccess_begin();
// /*AFLA*/ 		__put_user_asm(*(u16 *)src, (u16 __user *)dst,
// /*AFLA*/ 			      ret, "w", "w", "ir", 2);
// /*AFLA*/ 		__uaccess_end();
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	case 4:
// /*AFLA*/ 		__uaccess_begin();
// /*AFLA*/ 		__put_user_asm(*(u32 *)src, (u32 __user *)dst,
// /*AFLA*/ 			      ret, "l", "k", "ir", 4);
// /*AFLA*/ 		__uaccess_end();
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	case 8:
// /*AFLA*/ 		__uaccess_begin();
// /*AFLA*/ 		__put_user_asm(*(u64 *)src, (u64 __user *)dst,
// /*AFLA*/ 			      ret, "q", "", "er", 8);
// /*AFLA*/ 		__uaccess_end();
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	case 10:
// /*AFLA*/ 		__uaccess_begin();
// /*AFLA*/ 		__put_user_asm(*(u64 *)src, (u64 __user *)dst,
// /*AFLA*/ 			       ret, "q", "", "er", 10);
// /*AFLA*/ 		if (likely(!ret)) {
// /*AFLA*/ 			asm("":::"memory");
// /*AFLA*/ 			__put_user_asm(4[(u16 *)src], 4 + (u16 __user *)dst,
// /*AFLA*/ 				       ret, "w", "w", "ir", 2);
// /*AFLA*/ 		}
// /*AFLA*/ 		__uaccess_end();
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	case 16:
// /*AFLA*/ 		__uaccess_begin();
// /*AFLA*/ 		__put_user_asm(*(u64 *)src, (u64 __user *)dst,
// /*AFLA*/ 			       ret, "q", "", "er", 16);
// /*AFLA*/ 		if (likely(!ret)) {
// /*AFLA*/ 			asm("":::"memory");
// /*AFLA*/ 			__put_user_asm(1[(u64 *)src], 1 + (u64 __user *)dst,
// /*AFLA*/ 				       ret, "q", "", "er", 8);
// /*AFLA*/ 		}
// /*AFLA*/ 		__uaccess_end();
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	default:
// /*AFLA*/ 		return copy_user_generic((__force void *)dst, src, size);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline __must_check
// /*AFLA*/ int __copy_to_user(void __user *dst, const void *src, unsigned size)
// /*AFLA*/ {
// /*AFLA*/ 	might_fault();
// /*AFLA*/ 	kasan_check_read(src, size);
// /*AFLA*/ 	return __copy_to_user_nocheck(dst, src, size);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline __must_check
// /*AFLA*/ int __copy_in_user(void __user *dst, const void __user *src, unsigned size)
// /*AFLA*/ {
// /*AFLA*/ 	int ret = 0;
// /*AFLA*/ 
// /*AFLA*/ 	might_fault();
// /*AFLA*/ 	if (!__builtin_constant_p(size))
// /*AFLA*/ 		return copy_user_generic((__force void *)dst,
// /*AFLA*/ 					 (__force void *)src, size);
// /*AFLA*/ 	switch (size) {
// /*AFLA*/ 	case 1: {
// /*AFLA*/ 		u8 tmp;
// /*AFLA*/ 		__uaccess_begin();
// /*AFLA*/ 		__get_user_asm(tmp, (u8 __user *)src,
// /*AFLA*/ 			       ret, "b", "b", "=q", 1);
// /*AFLA*/ 		if (likely(!ret))
// /*AFLA*/ 			__put_user_asm(tmp, (u8 __user *)dst,
// /*AFLA*/ 				       ret, "b", "b", "iq", 1);
// /*AFLA*/ 		__uaccess_end();
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	}
// /*AFLA*/ 	case 2: {
// /*AFLA*/ 		u16 tmp;
// /*AFLA*/ 		__uaccess_begin();
// /*AFLA*/ 		__get_user_asm(tmp, (u16 __user *)src,
// /*AFLA*/ 			       ret, "w", "w", "=r", 2);
// /*AFLA*/ 		if (likely(!ret))
// /*AFLA*/ 			__put_user_asm(tmp, (u16 __user *)dst,
// /*AFLA*/ 				       ret, "w", "w", "ir", 2);
// /*AFLA*/ 		__uaccess_end();
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	case 4: {
// /*AFLA*/ 		u32 tmp;
// /*AFLA*/ 		__uaccess_begin();
// /*AFLA*/ 		__get_user_asm(tmp, (u32 __user *)src,
// /*AFLA*/ 			       ret, "l", "k", "=r", 4);
// /*AFLA*/ 		if (likely(!ret))
// /*AFLA*/ 			__put_user_asm(tmp, (u32 __user *)dst,
// /*AFLA*/ 				       ret, "l", "k", "ir", 4);
// /*AFLA*/ 		__uaccess_end();
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	}
// /*AFLA*/ 	case 8: {
// /*AFLA*/ 		u64 tmp;
// /*AFLA*/ 		__uaccess_begin();
// /*AFLA*/ 		__get_user_asm(tmp, (u64 __user *)src,
// /*AFLA*/ 			       ret, "q", "", "=r", 8);
// /*AFLA*/ 		if (likely(!ret))
// /*AFLA*/ 			__put_user_asm(tmp, (u64 __user *)dst,
// /*AFLA*/ 				       ret, "q", "", "er", 8);
// /*AFLA*/ 		__uaccess_end();
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	}
// /*AFLA*/ 	default:
// /*AFLA*/ 		return copy_user_generic((__force void *)dst,
// /*AFLA*/ 					 (__force void *)src, size);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __must_check __always_inline int
// /*AFLA*/ __copy_from_user_inatomic(void *dst, const void __user *src, unsigned size)
// /*AFLA*/ {
// /*AFLA*/ 	kasan_check_write(dst, size);
// /*AFLA*/ 	return __copy_from_user_nocheck(dst, src, size);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __must_check __always_inline int
// /*AFLA*/ __copy_to_user_inatomic(void __user *dst, const void *src, unsigned size)
// /*AFLA*/ {
// /*AFLA*/ 	kasan_check_read(src, size);
// /*AFLA*/ 	return __copy_to_user_nocheck(dst, src, size);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern long __copy_user_nocache(void *dst, const void __user *src,
// /*AFLA*/ 				unsigned size, int zerorest);
// /*AFLA*/ 
// /*AFLA*/ static inline int
// /*AFLA*/ __copy_from_user_nocache(void *dst, const void __user *src, unsigned size)
// /*AFLA*/ {
// /*AFLA*/ 	might_fault();
// /*AFLA*/ 	kasan_check_write(dst, size);
// /*AFLA*/ 	return __copy_user_nocache(dst, src, size, 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int
// /*AFLA*/ __copy_from_user_inatomic_nocache(void *dst, const void __user *src,
// /*AFLA*/ 				  unsigned size)
// /*AFLA*/ {
// /*AFLA*/ 	kasan_check_write(dst, size);
// /*AFLA*/ 	return __copy_user_nocache(dst, src, size, 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ unsigned long
// /*AFLA*/ copy_user_handle_tail(char *to, char *from, unsigned len);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_UACCESS_64_H */
