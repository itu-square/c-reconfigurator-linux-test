// /*AFLA*/ #ifndef _ASM_X86_STRING_64_H
// /*AFLA*/ #define _ASM_X86_STRING_64_H
// /*AFLA*/ 
// /*AFLA*/ #ifdef __KERNEL__
// /*AFLA*/ #include <linux/jump_label.h>
// /*AFLA*/ 
// /*AFLA*/ /* Written 2002 by Andi Kleen */
// /*AFLA*/ 
// /*AFLA*/ /* Only used for special circumstances. Stolen from i386/string.h */
// /*AFLA*/ static __always_inline void *__inline_memcpy(void *to, const void *from, size_t n)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long d0, d1, d2;
// /*AFLA*/ 	asm volatile("rep ; movsl\n\t"
// /*AFLA*/ 		     "testb $2,%b4\n\t"
// /*AFLA*/ 		     "je 1f\n\t"
// /*AFLA*/ 		     "movsw\n"
// /*AFLA*/ 		     "1:\ttestb $1,%b4\n\t"
// /*AFLA*/ 		     "je 2f\n\t"
// /*AFLA*/ 		     "movsb\n"
// /*AFLA*/ 		     "2:"
// /*AFLA*/ 		     : "=&c" (d0), "=&D" (d1), "=&S" (d2)
// /*AFLA*/ 		     : "0" (n / 4), "q" (n), "1" ((long)to), "2" ((long)from)
// /*AFLA*/ 		     : "memory");
// /*AFLA*/ 	return to;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Even with __builtin_ the compiler may decide to use the out of line
// /*AFLA*/    function. */
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_MEMCPY 1
// /*AFLA*/ extern void *memcpy(void *to, const void *from, size_t len);
// /*AFLA*/ extern void *__memcpy(void *to, const void *from, size_t len);
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_KMEMCHECK
// /*AFLA*/ #if (__GNUC__ == 4 && __GNUC_MINOR__ < 3) || __GNUC__ < 4
// /*AFLA*/ #define memcpy(dst, src, len)					\
// /*AFLA*/ ({								\
// /*AFLA*/ 	size_t __len = (len);					\
// /*AFLA*/ 	void *__ret;						\
// /*AFLA*/ 	if (__builtin_constant_p(len) && __len >= 64)		\
// /*AFLA*/ 		__ret = __memcpy((dst), (src), __len);		\
// /*AFLA*/ 	else							\
// /*AFLA*/ 		__ret = __builtin_memcpy((dst), (src), __len);	\
// /*AFLA*/ 	__ret;							\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ #else
// /*AFLA*/ /*
// /*AFLA*/  * kmemcheck becomes very happy if we use the REP instructions unconditionally,
// /*AFLA*/  * because it means that we know both memory operands in advance.
// /*AFLA*/  */
// /*AFLA*/ #define memcpy(dst, src, len) __inline_memcpy((dst), (src), (len))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_MEMSET
// /*AFLA*/ void *memset(void *s, int c, size_t n);
// /*AFLA*/ void *__memset(void *s, int c, size_t n);
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_MEMMOVE
// /*AFLA*/ void *memmove(void *dest, const void *src, size_t count);
// /*AFLA*/ void *__memmove(void *dest, const void *src, size_t count);
// /*AFLA*/ 
// /*AFLA*/ int memcmp(const void *cs, const void *ct, size_t count);
// /*AFLA*/ size_t strlen(const char *s);
// /*AFLA*/ char *strcpy(char *dest, const char *src);
// /*AFLA*/ char *strcat(char *dest, const char *src);
// /*AFLA*/ int strcmp(const char *cs, const char *ct);
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_KASAN) && !defined(__SANITIZE_ADDRESS__)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * For files that not instrumented (e.g. mm/slub.c) we
// /*AFLA*/  * should use not instrumented version of mem* functions.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #undef memcpy
// /*AFLA*/ #define memcpy(dst, src, len) __memcpy(dst, src, len)
// /*AFLA*/ #define memmove(dst, src, len) __memmove(dst, src, len)
// /*AFLA*/ #define memset(s, c, n) __memset(s, c, n)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ __must_check int memcpy_mcsafe_unrolled(void *dst, const void *src, size_t cnt);
// /*AFLA*/ DECLARE_STATIC_KEY_FALSE(mcsafe_key);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * memcpy_mcsafe - copy memory with indication if a machine check happened
// /*AFLA*/  *
// /*AFLA*/  * @dst:	destination address
// /*AFLA*/  * @src:	source address
// /*AFLA*/  * @cnt:	number of bytes to copy
// /*AFLA*/  *
// /*AFLA*/  * Low level memory copy function that catches machine checks
// /*AFLA*/  * We only call into the "safe" function on systems that can
// /*AFLA*/  * actually do machine check recovery. Everyone else can just
// /*AFLA*/  * use memcpy().
// /*AFLA*/  *
// /*AFLA*/  * Return 0 for success, -EFAULT for fail
// /*AFLA*/  */
// /*AFLA*/ static __always_inline __must_check int
// /*AFLA*/ memcpy_mcsafe(void *dst, const void *src, size_t cnt)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_X86_MCE
// /*AFLA*/ 	if (static_branch_unlikely(&mcsafe_key))
// /*AFLA*/ 		return memcpy_mcsafe_unrolled(dst, src, cnt);
// /*AFLA*/ 	else
// /*AFLA*/ #endif
// /*AFLA*/ 		memcpy(dst, src, cnt);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* __KERNEL__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_STRING_64_H */
