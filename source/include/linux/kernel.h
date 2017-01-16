// /*AFLA*/ #ifndef _LINUX_KERNEL_H
// /*AFLA*/ #define _LINUX_KERNEL_H
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #include <stdarg.h>
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ #include <linux/stddef.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/bitops.h>
// /*AFLA*/ #include <linux/log2.h>
// /*AFLA*/ #include <linux/typecheck.h>
// /*AFLA*/ #include <linux/printk.h>
// /*AFLA*/ #include <asm/byteorder.h>
// /*AFLA*/ #include <uapi/linux/kernel.h>
// /*AFLA*/ 
// /*AFLA*/ #define USHRT_MAX	((u16)(~0U))
// /*AFLA*/ #define SHRT_MAX	((s16)(USHRT_MAX>>1))
// /*AFLA*/ #define SHRT_MIN	((s16)(-SHRT_MAX - 1))
// /*AFLA*/ #define INT_MAX		((int)(~0U>>1))
// /*AFLA*/ #define INT_MIN		(-INT_MAX - 1)
// /*AFLA*/ #define UINT_MAX	(~0U)
// /*AFLA*/ #define LONG_MAX	((long)(~0UL>>1))
// /*AFLA*/ #define LONG_MIN	(-LONG_MAX - 1)
// /*AFLA*/ #define ULONG_MAX	(~0UL)
// /*AFLA*/ #define LLONG_MAX	((long long)(~0ULL>>1))
// /*AFLA*/ #define LLONG_MIN	(-LLONG_MAX - 1)
// /*AFLA*/ #define ULLONG_MAX	(~0ULL)
// /*AFLA*/ #define SIZE_MAX	(~(size_t)0)
// /*AFLA*/ 
// /*AFLA*/ #define U8_MAX		((u8)~0U)
// /*AFLA*/ #define S8_MAX		((s8)(U8_MAX>>1))
// /*AFLA*/ #define S8_MIN		((s8)(-S8_MAX - 1))
// /*AFLA*/ #define U16_MAX		((u16)~0U)
// /*AFLA*/ #define S16_MAX		((s16)(U16_MAX>>1))
// /*AFLA*/ #define S16_MIN		((s16)(-S16_MAX - 1))
// /*AFLA*/ #define U32_MAX		((u32)~0U)
// /*AFLA*/ #define S32_MAX		((s32)(U32_MAX>>1))
// /*AFLA*/ #define S32_MIN		((s32)(-S32_MAX - 1))
// /*AFLA*/ #define U64_MAX		((u64)~0ULL)
// /*AFLA*/ #define S64_MAX		((s64)(U64_MAX>>1))
// /*AFLA*/ #define S64_MIN		((s64)(-S64_MAX - 1))
// /*AFLA*/ 
// /*AFLA*/ #define STACK_MAGIC	0xdeadbeef
// /*AFLA*/ 
// /*AFLA*/ #define REPEAT_BYTE(x)	((~0ul / 0xff) * (x))
// /*AFLA*/ 
// /*AFLA*/ #define ALIGN(x, a)		__ALIGN_KERNEL((x), (a))
// /*AFLA*/ #define __ALIGN_MASK(x, mask)	__ALIGN_KERNEL_MASK((x), (mask))
// /*AFLA*/ #define PTR_ALIGN(p, a)		((typeof(p))ALIGN((unsigned long)(p), (a)))
// /*AFLA*/ #define IS_ALIGNED(x, a)		(((x) & ((typeof(x))(a) - 1)) == 0)
// /*AFLA*/ 
// /*AFLA*/ #define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))
// /*AFLA*/ 
// /*AFLA*/ #define u64_to_user_ptr(x) (		\
// /*AFLA*/ {					\
// /*AFLA*/ 	typecheck(u64, x);		\
// /*AFLA*/ 	(void __user *)(uintptr_t)x;	\
// /*AFLA*/ }					\
// /*AFLA*/ )
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This looks more complex than it should be. But we need to
// /*AFLA*/  * get the type for the ~ right in round_down (it needs to be
// /*AFLA*/  * as wide as the result!), and we want to evaluate the macro
// /*AFLA*/  * arguments just once each.
// /*AFLA*/  */
// /*AFLA*/ #define __round_mask(x, y) ((__typeof__(x))((y)-1))
// /*AFLA*/ #define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
// /*AFLA*/ #define round_down(x, y) ((x) & ~__round_mask(x, y))
// /*AFLA*/ 
// /*AFLA*/ #define FIELD_SIZEOF(t, f) (sizeof(((t*)0)->f))
// /*AFLA*/ #define DIV_ROUND_UP __KERNEL_DIV_ROUND_UP
// /*AFLA*/ #define DIV_ROUND_UP_ULL(ll,d) \
// /*AFLA*/ 	({ unsigned long long _tmp = (ll)+(d)-1; do_div(_tmp, d); _tmp; })
// /*AFLA*/ 
// /*AFLA*/ #if BITS_PER_LONG == 32
// /*AFLA*/ # define DIV_ROUND_UP_SECTOR_T(ll,d) DIV_ROUND_UP_ULL(ll, d)
// /*AFLA*/ #else
// /*AFLA*/ # define DIV_ROUND_UP_SECTOR_T(ll,d) DIV_ROUND_UP(ll,d)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* The `const' in roundup() prevents gcc-3.3 from calling __divdi3 */
// /*AFLA*/ #define roundup(x, y) (					\
// /*AFLA*/ {							\
// /*AFLA*/ 	const typeof(y) __y = y;			\
// /*AFLA*/ 	(((x) + (__y - 1)) / __y) * __y;		\
// /*AFLA*/ }							\
// /*AFLA*/ )
// /*AFLA*/ #define rounddown(x, y) (				\
// /*AFLA*/ {							\
// /*AFLA*/ 	typeof(x) __x = (x);				\
// /*AFLA*/ 	__x - (__x % (y));				\
// /*AFLA*/ }							\
// /*AFLA*/ )
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Divide positive or negative dividend by positive divisor and round
// /*AFLA*/  * to closest integer. Result is undefined for negative divisors and
// /*AFLA*/  * for negative dividends if the divisor variable type is unsigned.
// /*AFLA*/  */
// /*AFLA*/ #define DIV_ROUND_CLOSEST(x, divisor)(			\
// /*AFLA*/ {							\
// /*AFLA*/ 	typeof(x) __x = x;				\
// /*AFLA*/ 	typeof(divisor) __d = divisor;			\
// /*AFLA*/ 	(((typeof(x))-1) > 0 ||				\
// /*AFLA*/ 	 ((typeof(divisor))-1) > 0 || (__x) > 0) ?	\
// /*AFLA*/ 		(((__x) + ((__d) / 2)) / (__d)) :	\
// /*AFLA*/ 		(((__x) - ((__d) / 2)) / (__d));	\
// /*AFLA*/ }							\
// /*AFLA*/ )
// /*AFLA*/ /*
// /*AFLA*/  * Same as above but for u64 dividends. divisor must be a 32-bit
// /*AFLA*/  * number.
// /*AFLA*/  */
// /*AFLA*/ #define DIV_ROUND_CLOSEST_ULL(x, divisor)(		\
// /*AFLA*/ {							\
// /*AFLA*/ 	typeof(divisor) __d = divisor;			\
// /*AFLA*/ 	unsigned long long _tmp = (x) + (__d) / 2;	\
// /*AFLA*/ 	do_div(_tmp, __d);				\
// /*AFLA*/ 	_tmp;						\
// /*AFLA*/ }							\
// /*AFLA*/ )
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Multiplies an integer by a fraction, while avoiding unnecessary
// /*AFLA*/  * overflow or loss of precision.
// /*AFLA*/  */
// /*AFLA*/ #define mult_frac(x, numer, denom)(			\
// /*AFLA*/ {							\
// /*AFLA*/ 	typeof(x) quot = (x) / (denom);			\
// /*AFLA*/ 	typeof(x) rem  = (x) % (denom);			\
// /*AFLA*/ 	(quot * (numer)) + ((rem * (numer)) / (denom));	\
// /*AFLA*/ }							\
// /*AFLA*/ )
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define _RET_IP_		(unsigned long)__builtin_return_address(0)
// /*AFLA*/ #define _THIS_IP_  ({ __label__ __here; __here: (unsigned long)&&__here; })
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_LBDAF
// /*AFLA*/ # include <asm/div64.h>
// /*AFLA*/ # define sector_div(a, b) do_div(a, b)
// /*AFLA*/ #else
// /*AFLA*/ # define sector_div(n, b)( \
// /*AFLA*/ { \
// /*AFLA*/ 	int _res; \
// /*AFLA*/ 	_res = (n) % (b); \
// /*AFLA*/ 	(n) /= (b); \
// /*AFLA*/ 	_res; \
// /*AFLA*/ } \
// /*AFLA*/ )
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * upper_32_bits - return bits 32-63 of a number
// /*AFLA*/  * @n: the number we're accessing
// /*AFLA*/  *
// /*AFLA*/  * A basic shift-right of a 64- or 32-bit quantity.  Use this to suppress
// /*AFLA*/  * the "right shift count >= width of type" warning when that quantity is
// /*AFLA*/  * 32-bits.
// /*AFLA*/  */
// /*AFLA*/ #define upper_32_bits(n) ((u32)(((n) >> 16) >> 16))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * lower_32_bits - return bits 0-31 of a number
// /*AFLA*/  * @n: the number we're accessing
// /*AFLA*/  */
// /*AFLA*/ #define lower_32_bits(n) ((u32)(n))
// /*AFLA*/ 
// /*AFLA*/ struct completion;
// /*AFLA*/ struct pt_regs;
// /*AFLA*/ struct user;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PREEMPT_VOLUNTARY
// /*AFLA*/ extern int _cond_resched(void);
// /*AFLA*/ # define might_resched() _cond_resched()
// /*AFLA*/ #else
// /*AFLA*/ # define might_resched() do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_ATOMIC_SLEEP
// /*AFLA*/   void ___might_sleep(const char *file, int line, int preempt_offset);
// /*AFLA*/   void __might_sleep(const char *file, int line, int preempt_offset);
// /*AFLA*/ /**
// /*AFLA*/  * might_sleep - annotation for functions that can sleep
// /*AFLA*/  *
// /*AFLA*/  * this macro will print a stack trace if it is executed in an atomic
// /*AFLA*/  * context (spinlock, irq-handler, ...).
// /*AFLA*/  *
// /*AFLA*/  * This is a useful debugging help to be able to catch problems early and not
// /*AFLA*/  * be bitten later when the calling function happens to sleep when it is not
// /*AFLA*/  * supposed to.
// /*AFLA*/  */
// /*AFLA*/ # define might_sleep() \
// /*AFLA*/ 	do { __might_sleep(__FILE__, __LINE__, 0); might_resched(); } while (0)
// /*AFLA*/ # define sched_annotate_sleep()	(current->task_state_change = 0)
// /*AFLA*/ #else
// /*AFLA*/   static inline void ___might_sleep(const char *file, int line,
// /*AFLA*/ 				   int preempt_offset) { }
// /*AFLA*/   static inline void __might_sleep(const char *file, int line,
// /*AFLA*/ 				   int preempt_offset) { }
// /*AFLA*/ # define might_sleep() do { might_resched(); } while (0)
// /*AFLA*/ # define sched_annotate_sleep() do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define might_sleep_if(cond) do { if (cond) might_sleep(); } while (0)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * abs - return absolute value of an argument
// /*AFLA*/  * @x: the value.  If it is unsigned type, it is converted to signed type first.
// /*AFLA*/  *     char is treated as if it was signed (regardless of whether it really is)
// /*AFLA*/  *     but the macro's return type is preserved as char.
// /*AFLA*/  *
// /*AFLA*/  * Return: an absolute value of x.
// /*AFLA*/  */
// /*AFLA*/ #define abs(x)	__abs_choose_expr(x, long long,				\
// /*AFLA*/ 		__abs_choose_expr(x, long,				\
// /*AFLA*/ 		__abs_choose_expr(x, int,				\
// /*AFLA*/ 		__abs_choose_expr(x, short,				\
// /*AFLA*/ 		__abs_choose_expr(x, char,				\
// /*AFLA*/ 		__builtin_choose_expr(					\
// /*AFLA*/ 			__builtin_types_compatible_p(typeof(x), char),	\
// /*AFLA*/ 			(char)({ signed char __x = (x); __x<0?-__x:__x; }), \
// /*AFLA*/ 			((void)0)))))))
// /*AFLA*/ 
// /*AFLA*/ #define __abs_choose_expr(x, type, other) __builtin_choose_expr(	\
// /*AFLA*/ 	__builtin_types_compatible_p(typeof(x),   signed type) ||	\
// /*AFLA*/ 	__builtin_types_compatible_p(typeof(x), unsigned type),		\
// /*AFLA*/ 	({ signed type __x = (x); __x < 0 ? -__x : __x; }), other)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * reciprocal_scale - "scale" a value into range [0, ep_ro)
// /*AFLA*/  * @val: value
// /*AFLA*/  * @ep_ro: right open interval endpoint
// /*AFLA*/  *
// /*AFLA*/  * Perform a "reciprocal multiplication" in order to "scale" a value into
// /*AFLA*/  * range [0, ep_ro), where the upper interval endpoint is right-open.
// /*AFLA*/  * This is useful, e.g. for accessing a index of an array containing
// /*AFLA*/  * ep_ro elements, for example. Think of it as sort of modulus, only that
// /*AFLA*/  * the result isn't that of modulo. ;) Note that if initial input is a
// /*AFLA*/  * small value, then result will return 0.
// /*AFLA*/  *
// /*AFLA*/  * Return: a result based on val in interval [0, ep_ro).
// /*AFLA*/  */
// /*AFLA*/ static inline u32 reciprocal_scale(u32 val, u32 ep_ro)
// /*AFLA*/ {
// /*AFLA*/ 	return (u32)(((u64) val * ep_ro) >> 32);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_MMU) && \
// /*AFLA*/ 	(defined(CONFIG_PROVE_LOCKING) || defined(CONFIG_DEBUG_ATOMIC_SLEEP))
// /*AFLA*/ #define might_fault() __might_fault(__FILE__, __LINE__)
// /*AFLA*/ void __might_fault(const char *file, int line);
// /*AFLA*/ #else
// /*AFLA*/ static inline void might_fault(void) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern struct atomic_notifier_head panic_notifier_list;
// /*AFLA*/ extern long (*panic_blink)(int state);
// /*AFLA*/ __printf(1, 2)
// /*AFLA*/ void panic(const char *fmt, ...) __noreturn __cold;
// /*AFLA*/ void nmi_panic(struct pt_regs *regs, const char *msg);
// /*AFLA*/ extern void oops_enter(void);
// /*AFLA*/ extern void oops_exit(void);
// /*AFLA*/ void print_oops_end_marker(void);
// /*AFLA*/ extern int oops_may_print(void);
// /*AFLA*/ void do_exit(long error_code) __noreturn;
// /*AFLA*/ void complete_and_exit(struct completion *, long) __noreturn;
// /*AFLA*/ 
// /*AFLA*/ /* Internal, do not use. */
// /*AFLA*/ int __must_check _kstrtoul(const char *s, unsigned int base, unsigned long *res);
// /*AFLA*/ int __must_check _kstrtol(const char *s, unsigned int base, long *res);
// /*AFLA*/ 
// /*AFLA*/ int __must_check kstrtoull(const char *s, unsigned int base, unsigned long long *res);
// /*AFLA*/ int __must_check kstrtoll(const char *s, unsigned int base, long long *res);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kstrtoul - convert a string to an unsigned long
// /*AFLA*/  * @s: The start of the string. The string must be null-terminated, and may also
// /*AFLA*/  *  include a single newline before its terminating null. The first character
// /*AFLA*/  *  may also be a plus sign, but not a minus sign.
// /*AFLA*/  * @base: The number base to use. The maximum supported base is 16. If base is
// /*AFLA*/  *  given as 0, then the base of the string is automatically detected with the
// /*AFLA*/  *  conventional semantics - If it begins with 0x the number will be parsed as a
// /*AFLA*/  *  hexadecimal (case insensitive), if it otherwise begins with 0, it will be
// /*AFLA*/  *  parsed as an octal number. Otherwise it will be parsed as a decimal.
// /*AFLA*/  * @res: Where to write the result of the conversion on success.
// /*AFLA*/  *
// /*AFLA*/  * Returns 0 on success, -ERANGE on overflow and -EINVAL on parsing error.
// /*AFLA*/  * Used as a replacement for the obsolete simple_strtoull. Return code must
// /*AFLA*/  * be checked.
// /*AFLA*/ */
// /*AFLA*/ static inline int __must_check kstrtoul(const char *s, unsigned int base, unsigned long *res)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * We want to shortcut function call, but
// /*AFLA*/ 	 * __builtin_types_compatible_p(unsigned long, unsigned long long) = 0.
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (sizeof(unsigned long) == sizeof(unsigned long long) &&
// /*AFLA*/ 	    __alignof__(unsigned long) == __alignof__(unsigned long long))
// /*AFLA*/ 		return kstrtoull(s, base, (unsigned long long *)res);
// /*AFLA*/ 	else
// /*AFLA*/ 		return _kstrtoul(s, base, res);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kstrtol - convert a string to a long
// /*AFLA*/  * @s: The start of the string. The string must be null-terminated, and may also
// /*AFLA*/  *  include a single newline before its terminating null. The first character
// /*AFLA*/  *  may also be a plus sign or a minus sign.
// /*AFLA*/  * @base: The number base to use. The maximum supported base is 16. If base is
// /*AFLA*/  *  given as 0, then the base of the string is automatically detected with the
// /*AFLA*/  *  conventional semantics - If it begins with 0x the number will be parsed as a
// /*AFLA*/  *  hexadecimal (case insensitive), if it otherwise begins with 0, it will be
// /*AFLA*/  *  parsed as an octal number. Otherwise it will be parsed as a decimal.
// /*AFLA*/  * @res: Where to write the result of the conversion on success.
// /*AFLA*/  *
// /*AFLA*/  * Returns 0 on success, -ERANGE on overflow and -EINVAL on parsing error.
// /*AFLA*/  * Used as a replacement for the obsolete simple_strtoull. Return code must
// /*AFLA*/  * be checked.
// /*AFLA*/  */
// /*AFLA*/ static inline int __must_check kstrtol(const char *s, unsigned int base, long *res)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * We want to shortcut function call, but
// /*AFLA*/ 	 * __builtin_types_compatible_p(long, long long) = 0.
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (sizeof(long) == sizeof(long long) &&
// /*AFLA*/ 	    __alignof__(long) == __alignof__(long long))
// /*AFLA*/ 		return kstrtoll(s, base, (long long *)res);
// /*AFLA*/ 	else
// /*AFLA*/ 		return _kstrtol(s, base, res);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ int __must_check kstrtouint(const char *s, unsigned int base, unsigned int *res);
// /*AFLA*/ int __must_check kstrtoint(const char *s, unsigned int base, int *res);
// /*AFLA*/ 
// /*AFLA*/ static inline int __must_check kstrtou64(const char *s, unsigned int base, u64 *res)
// /*AFLA*/ {
// /*AFLA*/ 	return kstrtoull(s, base, res);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __must_check kstrtos64(const char *s, unsigned int base, s64 *res)
// /*AFLA*/ {
// /*AFLA*/ 	return kstrtoll(s, base, res);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __must_check kstrtou32(const char *s, unsigned int base, u32 *res)
// /*AFLA*/ {
// /*AFLA*/ 	return kstrtouint(s, base, res);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __must_check kstrtos32(const char *s, unsigned int base, s32 *res)
// /*AFLA*/ {
// /*AFLA*/ 	return kstrtoint(s, base, res);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ int __must_check kstrtou16(const char *s, unsigned int base, u16 *res);
// /*AFLA*/ int __must_check kstrtos16(const char *s, unsigned int base, s16 *res);
// /*AFLA*/ int __must_check kstrtou8(const char *s, unsigned int base, u8 *res);
// /*AFLA*/ int __must_check kstrtos8(const char *s, unsigned int base, s8 *res);
// /*AFLA*/ int __must_check kstrtobool(const char *s, bool *res);
// /*AFLA*/ 
// /*AFLA*/ int __must_check kstrtoull_from_user(const char __user *s, size_t count, unsigned int base, unsigned long long *res);
// /*AFLA*/ int __must_check kstrtoll_from_user(const char __user *s, size_t count, unsigned int base, long long *res);
// /*AFLA*/ int __must_check kstrtoul_from_user(const char __user *s, size_t count, unsigned int base, unsigned long *res);
// /*AFLA*/ int __must_check kstrtol_from_user(const char __user *s, size_t count, unsigned int base, long *res);
// /*AFLA*/ int __must_check kstrtouint_from_user(const char __user *s, size_t count, unsigned int base, unsigned int *res);
// /*AFLA*/ int __must_check kstrtoint_from_user(const char __user *s, size_t count, unsigned int base, int *res);
// /*AFLA*/ int __must_check kstrtou16_from_user(const char __user *s, size_t count, unsigned int base, u16 *res);
// /*AFLA*/ int __must_check kstrtos16_from_user(const char __user *s, size_t count, unsigned int base, s16 *res);
// /*AFLA*/ int __must_check kstrtou8_from_user(const char __user *s, size_t count, unsigned int base, u8 *res);
// /*AFLA*/ int __must_check kstrtos8_from_user(const char __user *s, size_t count, unsigned int base, s8 *res);
// /*AFLA*/ int __must_check kstrtobool_from_user(const char __user *s, size_t count, bool *res);
// /*AFLA*/ 
// /*AFLA*/ static inline int __must_check kstrtou64_from_user(const char __user *s, size_t count, unsigned int base, u64 *res)
// /*AFLA*/ {
// /*AFLA*/ 	return kstrtoull_from_user(s, count, base, res);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __must_check kstrtos64_from_user(const char __user *s, size_t count, unsigned int base, s64 *res)
// /*AFLA*/ {
// /*AFLA*/ 	return kstrtoll_from_user(s, count, base, res);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __must_check kstrtou32_from_user(const char __user *s, size_t count, unsigned int base, u32 *res)
// /*AFLA*/ {
// /*AFLA*/ 	return kstrtouint_from_user(s, count, base, res);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __must_check kstrtos32_from_user(const char __user *s, size_t count, unsigned int base, s32 *res)
// /*AFLA*/ {
// /*AFLA*/ 	return kstrtoint_from_user(s, count, base, res);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Obsolete, do not use.  Use kstrto<foo> instead */
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long simple_strtoul(const char *,char **,unsigned int);
// /*AFLA*/ extern long simple_strtol(const char *,char **,unsigned int);
// /*AFLA*/ extern unsigned long long simple_strtoull(const char *,char **,unsigned int);
// /*AFLA*/ extern long long simple_strtoll(const char *,char **,unsigned int);
// /*AFLA*/ 
// /*AFLA*/ extern int num_to_str(char *buf, int size, unsigned long long num);
// /*AFLA*/ 
// /*AFLA*/ /* lib/printf utilities */
// /*AFLA*/ 
// /*AFLA*/ extern __printf(2, 3) int sprintf(char *buf, const char * fmt, ...);
// /*AFLA*/ extern __printf(2, 0) int vsprintf(char *buf, const char *, va_list);
// /*AFLA*/ extern __printf(3, 4)
// /*AFLA*/ int snprintf(char *buf, size_t size, const char *fmt, ...);
// /*AFLA*/ extern __printf(3, 0)
// /*AFLA*/ int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
// /*AFLA*/ extern __printf(3, 4)
// /*AFLA*/ int scnprintf(char *buf, size_t size, const char *fmt, ...);
// /*AFLA*/ extern __printf(3, 0)
// /*AFLA*/ int vscnprintf(char *buf, size_t size, const char *fmt, va_list args);
// /*AFLA*/ extern __printf(2, 3) __malloc
// /*AFLA*/ char *kasprintf(gfp_t gfp, const char *fmt, ...);
// /*AFLA*/ extern __printf(2, 0) __malloc
// /*AFLA*/ char *kvasprintf(gfp_t gfp, const char *fmt, va_list args);
// /*AFLA*/ extern __printf(2, 0)
// /*AFLA*/ const char *kvasprintf_const(gfp_t gfp, const char *fmt, va_list args);
// /*AFLA*/ 
// /*AFLA*/ extern __scanf(2, 3)
// /*AFLA*/ int sscanf(const char *, const char *, ...);
// /*AFLA*/ extern __scanf(2, 0)
// /*AFLA*/ int vsscanf(const char *, const char *, va_list);
// /*AFLA*/ 
// /*AFLA*/ extern int get_option(char **str, int *pint);
// /*AFLA*/ extern char *get_options(const char *str, int nints, int *ints);
// /*AFLA*/ extern unsigned long long memparse(const char *ptr, char **retptr);
// /*AFLA*/ extern bool parse_option_str(const char *str, const char *option);
// /*AFLA*/ 
// /*AFLA*/ extern int core_kernel_text(unsigned long addr);
// /*AFLA*/ extern int core_kernel_data(unsigned long addr);
// /*AFLA*/ extern int __kernel_text_address(unsigned long addr);
// /*AFLA*/ extern int kernel_text_address(unsigned long addr);
// /*AFLA*/ extern int func_ptr_is_kernel_text(void *ptr);
// /*AFLA*/ 
// /*AFLA*/ unsigned long int_sqrt(unsigned long);
// /*AFLA*/ 
// /*AFLA*/ extern void bust_spinlocks(int yes);
// /*AFLA*/ extern int oops_in_progress;		/* If set, an oops, panic(), BUG() or die() is in progress */
// /*AFLA*/ extern int panic_timeout;
// /*AFLA*/ extern int panic_on_oops;
// /*AFLA*/ extern int panic_on_unrecovered_nmi;
// /*AFLA*/ extern int panic_on_io_nmi;
// /*AFLA*/ extern int panic_on_warn;
// /*AFLA*/ extern int sysctl_panic_on_rcu_stall;
// /*AFLA*/ extern int sysctl_panic_on_stackoverflow;
// /*AFLA*/ 
// /*AFLA*/ extern bool crash_kexec_post_notifiers;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * panic_cpu is used for synchronizing panic() and crash_kexec() execution. It
// /*AFLA*/  * holds a CPU number which is executing panic() currently. A value of
// /*AFLA*/  * PANIC_CPU_INVALID means no CPU has entered panic() or crash_kexec().
// /*AFLA*/  */
// /*AFLA*/ extern atomic_t panic_cpu;
// /*AFLA*/ #define PANIC_CPU_INVALID	-1
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Only to be used by arch init code. If the user over-wrote the default
// /*AFLA*/  * CONFIG_PANIC_TIMEOUT, honor it.
// /*AFLA*/  */
// /*AFLA*/ static inline void set_arch_panic_timeout(int timeout, int arch_default_timeout)
// /*AFLA*/ {
// /*AFLA*/ 	if (panic_timeout == arch_default_timeout)
// /*AFLA*/ 		panic_timeout = timeout;
// /*AFLA*/ }
// /*AFLA*/ extern const char *print_tainted(void);
// /*AFLA*/ enum lockdep_ok {
// /*AFLA*/ 	LOCKDEP_STILL_OK,
// /*AFLA*/ 	LOCKDEP_NOW_UNRELIABLE
// /*AFLA*/ };
// /*AFLA*/ extern void add_taint(unsigned flag, enum lockdep_ok);
// /*AFLA*/ extern int test_taint(unsigned flag);
// /*AFLA*/ extern unsigned long get_taint(void);
// /*AFLA*/ extern int root_mountflags;
// /*AFLA*/ 
// /*AFLA*/ extern bool early_boot_irqs_disabled;
// /*AFLA*/ 
// /*AFLA*/ /* Values used for system_state */
// /*AFLA*/ extern enum system_states {
// /*AFLA*/ 	SYSTEM_BOOTING,
// /*AFLA*/ 	SYSTEM_RUNNING,
// /*AFLA*/ 	SYSTEM_HALT,
// /*AFLA*/ 	SYSTEM_POWER_OFF,
// /*AFLA*/ 	SYSTEM_RESTART,
// /*AFLA*/ } system_state;
// /*AFLA*/ 
// /*AFLA*/ #define TAINT_PROPRIETARY_MODULE	0
// /*AFLA*/ #define TAINT_FORCED_MODULE		1
// /*AFLA*/ #define TAINT_CPU_OUT_OF_SPEC		2
// /*AFLA*/ #define TAINT_FORCED_RMMOD		3
// /*AFLA*/ #define TAINT_MACHINE_CHECK		4
// /*AFLA*/ #define TAINT_BAD_PAGE			5
// /*AFLA*/ #define TAINT_USER			6
// /*AFLA*/ #define TAINT_DIE			7
// /*AFLA*/ #define TAINT_OVERRIDDEN_ACPI_TABLE	8
// /*AFLA*/ #define TAINT_WARN			9
// /*AFLA*/ #define TAINT_CRAP			10
// /*AFLA*/ #define TAINT_FIRMWARE_WORKAROUND	11
// /*AFLA*/ #define TAINT_OOT_MODULE		12
// /*AFLA*/ #define TAINT_UNSIGNED_MODULE		13
// /*AFLA*/ #define TAINT_SOFTLOCKUP		14
// /*AFLA*/ #define TAINT_LIVEPATCH			15
// /*AFLA*/ 
// /*AFLA*/ extern const char hex_asc[];
// /*AFLA*/ #define hex_asc_lo(x)	hex_asc[((x) & 0x0f)]
// /*AFLA*/ #define hex_asc_hi(x)	hex_asc[((x) & 0xf0) >> 4]
// /*AFLA*/ 
// /*AFLA*/ static inline char *hex_byte_pack(char *buf, u8 byte)
// /*AFLA*/ {
// /*AFLA*/ 	*buf++ = hex_asc_hi(byte);
// /*AFLA*/ 	*buf++ = hex_asc_lo(byte);
// /*AFLA*/ 	return buf;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern const char hex_asc_upper[];
// /*AFLA*/ #define hex_asc_upper_lo(x)	hex_asc_upper[((x) & 0x0f)]
// /*AFLA*/ #define hex_asc_upper_hi(x)	hex_asc_upper[((x) & 0xf0) >> 4]
// /*AFLA*/ 
// /*AFLA*/ static inline char *hex_byte_pack_upper(char *buf, u8 byte)
// /*AFLA*/ {
// /*AFLA*/ 	*buf++ = hex_asc_upper_hi(byte);
// /*AFLA*/ 	*buf++ = hex_asc_upper_lo(byte);
// /*AFLA*/ 	return buf;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int hex_to_bin(char ch);
// /*AFLA*/ extern int __must_check hex2bin(u8 *dst, const char *src, size_t count);
// /*AFLA*/ extern char *bin2hex(char *dst, const void *src, size_t count);
// /*AFLA*/ 
// /*AFLA*/ bool mac_pton(const char *s, u8 *mac);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * General tracing related utility functions - trace_printk(),
// /*AFLA*/  * tracing_on/tracing_off and tracing_start()/tracing_stop
// /*AFLA*/  *
// /*AFLA*/  * Use tracing_on/tracing_off when you want to quickly turn on or off
// /*AFLA*/  * tracing. It simply enables or disables the recording of the trace events.
// /*AFLA*/  * This also corresponds to the user space /sys/kernel/debug/tracing/tracing_on
// /*AFLA*/  * file, which gives a means for the kernel and userspace to interact.
// /*AFLA*/  * Place a tracing_off() in the kernel where you want tracing to end.
// /*AFLA*/  * From user space, examine the trace, and then echo 1 > tracing_on
// /*AFLA*/  * to continue tracing.
// /*AFLA*/  *
// /*AFLA*/  * tracing_stop/tracing_start has slightly more overhead. It is used
// /*AFLA*/  * by things like suspend to ram where disabling the recording of the
// /*AFLA*/  * trace is not enough, but tracing must actually stop because things
// /*AFLA*/  * like calling smp_processor_id() may crash the system.
// /*AFLA*/  *
// /*AFLA*/  * Most likely, you want to use tracing_on/tracing_off.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ enum ftrace_dump_mode {
// /*AFLA*/ 	DUMP_NONE,
// /*AFLA*/ 	DUMP_ALL,
// /*AFLA*/ 	DUMP_ORIG,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TRACING
// /*AFLA*/ void tracing_on(void);
// /*AFLA*/ void tracing_off(void);
// /*AFLA*/ int tracing_is_on(void);
// /*AFLA*/ void tracing_snapshot(void);
// /*AFLA*/ void tracing_snapshot_alloc(void);
// /*AFLA*/ 
// /*AFLA*/ extern void tracing_start(void);
// /*AFLA*/ extern void tracing_stop(void);
// /*AFLA*/ 
// /*AFLA*/ static inline __printf(1, 2)
// /*AFLA*/ void ____trace_printk_check_format(const char *fmt, ...)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #define __trace_printk_check_format(fmt, args...)			\
// /*AFLA*/ do {									\
// /*AFLA*/ 	if (0)								\
// /*AFLA*/ 		____trace_printk_check_format(fmt, ##args);		\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * trace_printk - printf formatting in the ftrace buffer
// /*AFLA*/  * @fmt: the printf format for printing
// /*AFLA*/  *
// /*AFLA*/  * Note: __trace_printk is an internal function for trace_printk and
// /*AFLA*/  *       the @ip is passed in via the trace_printk macro.
// /*AFLA*/  *
// /*AFLA*/  * This function allows a kernel developer to debug fast path sections
// /*AFLA*/  * that printk is not appropriate for. By scattering in various
// /*AFLA*/  * printk like tracing in the code, a developer can quickly see
// /*AFLA*/  * where problems are occurring.
// /*AFLA*/  *
// /*AFLA*/  * This is intended as a debugging tool for the developer only.
// /*AFLA*/  * Please refrain from leaving trace_printks scattered around in
// /*AFLA*/  * your code. (Extra memory is used for special buffers that are
// /*AFLA*/  * allocated when trace_printk() is used)
// /*AFLA*/  *
// /*AFLA*/  * A little optization trick is done here. If there's only one
// /*AFLA*/  * argument, there's no need to scan the string for printf formats.
// /*AFLA*/  * The trace_puts() will suffice. But how can we take advantage of
// /*AFLA*/  * using trace_puts() when trace_printk() has only one argument?
// /*AFLA*/  * By stringifying the args and checking the size we can tell
// /*AFLA*/  * whether or not there are args. __stringify((__VA_ARGS__)) will
// /*AFLA*/  * turn into "()\0" with a size of 3 when there are no args, anything
// /*AFLA*/  * else will be bigger. All we need to do is define a string to this,
// /*AFLA*/  * and then take its size and compare to 3. If it's bigger, use
// /*AFLA*/  * do_trace_printk() otherwise, optimize it to trace_puts(). Then just
// /*AFLA*/  * let gcc optimize the rest.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define trace_printk(fmt, ...)				\
// /*AFLA*/ do {							\
// /*AFLA*/ 	char _______STR[] = __stringify((__VA_ARGS__));	\
// /*AFLA*/ 	if (sizeof(_______STR) > 3)			\
// /*AFLA*/ 		do_trace_printk(fmt, ##__VA_ARGS__);	\
// /*AFLA*/ 	else						\
// /*AFLA*/ 		trace_puts(fmt);			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define do_trace_printk(fmt, args...)					\
// /*AFLA*/ do {									\
// /*AFLA*/ 	static const char *trace_printk_fmt __used			\
// /*AFLA*/ 		__attribute__((section("__trace_printk_fmt"))) =	\
// /*AFLA*/ 		__builtin_constant_p(fmt) ? fmt : NULL;			\
// /*AFLA*/ 									\
// /*AFLA*/ 	__trace_printk_check_format(fmt, ##args);			\
// /*AFLA*/ 									\
// /*AFLA*/ 	if (__builtin_constant_p(fmt))					\
// /*AFLA*/ 		__trace_bprintk(_THIS_IP_, trace_printk_fmt, ##args);	\
// /*AFLA*/ 	else								\
// /*AFLA*/ 		__trace_printk(_THIS_IP_, fmt, ##args);			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ extern __printf(2, 3)
// /*AFLA*/ int __trace_bprintk(unsigned long ip, const char *fmt, ...);
// /*AFLA*/ 
// /*AFLA*/ extern __printf(2, 3)
// /*AFLA*/ int __trace_printk(unsigned long ip, const char *fmt, ...);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * trace_puts - write a string into the ftrace buffer
// /*AFLA*/  * @str: the string to record
// /*AFLA*/  *
// /*AFLA*/  * Note: __trace_bputs is an internal function for trace_puts and
// /*AFLA*/  *       the @ip is passed in via the trace_puts macro.
// /*AFLA*/  *
// /*AFLA*/  * This is similar to trace_printk() but is made for those really fast
// /*AFLA*/  * paths that a developer wants the least amount of "Heisenbug" affects,
// /*AFLA*/  * where the processing of the print format is still too much.
// /*AFLA*/  *
// /*AFLA*/  * This function allows a kernel developer to debug fast path sections
// /*AFLA*/  * that printk is not appropriate for. By scattering in various
// /*AFLA*/  * printk like tracing in the code, a developer can quickly see
// /*AFLA*/  * where problems are occurring.
// /*AFLA*/  *
// /*AFLA*/  * This is intended as a debugging tool for the developer only.
// /*AFLA*/  * Please refrain from leaving trace_puts scattered around in
// /*AFLA*/  * your code. (Extra memory is used for special buffers that are
// /*AFLA*/  * allocated when trace_puts() is used)
// /*AFLA*/  *
// /*AFLA*/  * Returns: 0 if nothing was written, positive # if string was.
// /*AFLA*/  *  (1 when __trace_bputs is used, strlen(str) when __trace_puts is used)
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define trace_puts(str) ({						\
// /*AFLA*/ 	static const char *trace_printk_fmt __used			\
// /*AFLA*/ 		__attribute__((section("__trace_printk_fmt"))) =	\
// /*AFLA*/ 		__builtin_constant_p(str) ? str : NULL;			\
// /*AFLA*/ 									\
// /*AFLA*/ 	if (__builtin_constant_p(str))					\
// /*AFLA*/ 		__trace_bputs(_THIS_IP_, trace_printk_fmt);		\
// /*AFLA*/ 	else								\
// /*AFLA*/ 		__trace_puts(_THIS_IP_, str, strlen(str));		\
// /*AFLA*/ })
// /*AFLA*/ extern int __trace_bputs(unsigned long ip, const char *str);
// /*AFLA*/ extern int __trace_puts(unsigned long ip, const char *str, int size);
// /*AFLA*/ 
// /*AFLA*/ extern void trace_dump_stack(int skip);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The double __builtin_constant_p is because gcc will give us an error
// /*AFLA*/  * if we try to allocate the static variable to fmt if it is not a
// /*AFLA*/  * constant. Even with the outer if statement.
// /*AFLA*/  */
// /*AFLA*/ #define ftrace_vprintk(fmt, vargs)					\
// /*AFLA*/ do {									\
// /*AFLA*/ 	if (__builtin_constant_p(fmt)) {				\
// /*AFLA*/ 		static const char *trace_printk_fmt __used		\
// /*AFLA*/ 		  __attribute__((section("__trace_printk_fmt"))) =	\
// /*AFLA*/ 			__builtin_constant_p(fmt) ? fmt : NULL;		\
// /*AFLA*/ 									\
// /*AFLA*/ 		__ftrace_vbprintk(_THIS_IP_, trace_printk_fmt, vargs);	\
// /*AFLA*/ 	} else								\
// /*AFLA*/ 		__ftrace_vprintk(_THIS_IP_, fmt, vargs);		\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ extern __printf(2, 0) int
// /*AFLA*/ __ftrace_vbprintk(unsigned long ip, const char *fmt, va_list ap);
// /*AFLA*/ 
// /*AFLA*/ extern __printf(2, 0) int
// /*AFLA*/ __ftrace_vprintk(unsigned long ip, const char *fmt, va_list ap);
// /*AFLA*/ 
// /*AFLA*/ extern void ftrace_dump(enum ftrace_dump_mode oops_dump_mode);
// /*AFLA*/ #else
// /*AFLA*/ static inline void tracing_start(void) { }
// /*AFLA*/ static inline void tracing_stop(void) { }
// /*AFLA*/ static inline void trace_dump_stack(int skip) { }
// /*AFLA*/ 
// /*AFLA*/ static inline void tracing_on(void) { }
// /*AFLA*/ static inline void tracing_off(void) { }
// /*AFLA*/ static inline int tracing_is_on(void) { return 0; }
// /*AFLA*/ static inline void tracing_snapshot(void) { }
// /*AFLA*/ static inline void tracing_snapshot_alloc(void) { }
// /*AFLA*/ 
// /*AFLA*/ static inline __printf(1, 2)
// /*AFLA*/ int trace_printk(const char *fmt, ...)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static __printf(1, 0) inline int
// /*AFLA*/ ftrace_vprintk(const char *fmt, va_list ap)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void ftrace_dump(enum ftrace_dump_mode oops_dump_mode) { }
// /*AFLA*/ #endif /* CONFIG_TRACING */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * min()/max()/clamp() macros that also do
// /*AFLA*/  * strict type-checking.. See the
// /*AFLA*/  * "unnecessary" pointer comparison.
// /*AFLA*/  */
// /*AFLA*/ #define __min(t1, t2, min1, min2, x, y) ({		\
// /*AFLA*/ 	t1 min1 = (x);					\
// /*AFLA*/ 	t2 min2 = (y);					\
// /*AFLA*/ 	(void) (&min1 == &min2);			\
// /*AFLA*/ 	min1 < min2 ? min1 : min2; })
// /*AFLA*/ #define min(x, y)					\
// /*AFLA*/ 	__min(typeof(x), typeof(y),			\
// /*AFLA*/ 	      __UNIQUE_ID(min1_), __UNIQUE_ID(min2_),	\
// /*AFLA*/ 	      x, y)
// /*AFLA*/ 
// /*AFLA*/ #define __max(t1, t2, max1, max2, x, y) ({		\
// /*AFLA*/ 	t1 max1 = (x);					\
// /*AFLA*/ 	t2 max2 = (y);					\
// /*AFLA*/ 	(void) (&max1 == &max2);			\
// /*AFLA*/ 	max1 > max2 ? max1 : max2; })
// /*AFLA*/ #define max(x, y)					\
// /*AFLA*/ 	__max(typeof(x), typeof(y),			\
// /*AFLA*/ 	      __UNIQUE_ID(max1_), __UNIQUE_ID(max2_),	\
// /*AFLA*/ 	      x, y)
// /*AFLA*/ 
// /*AFLA*/ #define min3(x, y, z) min((typeof(x))min(x, y), z)
// /*AFLA*/ #define max3(x, y, z) max((typeof(x))max(x, y), z)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * min_not_zero - return the minimum that is _not_ zero, unless both are zero
// /*AFLA*/  * @x: value1
// /*AFLA*/  * @y: value2
// /*AFLA*/  */
// /*AFLA*/ #define min_not_zero(x, y) ({			\
// /*AFLA*/ 	typeof(x) __x = (x);			\
// /*AFLA*/ 	typeof(y) __y = (y);			\
// /*AFLA*/ 	__x == 0 ? __y : ((__y == 0) ? __x : min(__x, __y)); })
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * clamp - return a value clamped to a given range with strict typechecking
// /*AFLA*/  * @val: current value
// /*AFLA*/  * @lo: lowest allowable value
// /*AFLA*/  * @hi: highest allowable value
// /*AFLA*/  *
// /*AFLA*/  * This macro does strict typechecking of lo/hi to make sure they are of the
// /*AFLA*/  * same type as val.  See the unnecessary pointer comparisons.
// /*AFLA*/  */
// /*AFLA*/ #define clamp(val, lo, hi) min((typeof(val))max(val, lo), hi)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * ..and if you can't take the strict
// /*AFLA*/  * types, you can specify one yourself.
// /*AFLA*/  *
// /*AFLA*/  * Or not use min/max/clamp at all, of course.
// /*AFLA*/  */
// /*AFLA*/ #define min_t(type, x, y)				\
// /*AFLA*/ 	__min(type, type,				\
// /*AFLA*/ 	      __UNIQUE_ID(min1_), __UNIQUE_ID(min2_),	\
// /*AFLA*/ 	      x, y)
// /*AFLA*/ 
// /*AFLA*/ #define max_t(type, x, y)				\
// /*AFLA*/ 	__max(type, type,				\
// /*AFLA*/ 	      __UNIQUE_ID(min1_), __UNIQUE_ID(min2_),	\
// /*AFLA*/ 	      x, y)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * clamp_t - return a value clamped to a given range using a given type
// /*AFLA*/  * @type: the type of variable to use
// /*AFLA*/  * @val: current value
// /*AFLA*/  * @lo: minimum allowable value
// /*AFLA*/  * @hi: maximum allowable value
// /*AFLA*/  *
// /*AFLA*/  * This macro does no typechecking and uses temporary variables of type
// /*AFLA*/  * 'type' to make all the comparisons.
// /*AFLA*/  */
// /*AFLA*/ #define clamp_t(type, val, lo, hi) min_t(type, max_t(type, val, lo), hi)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * clamp_val - return a value clamped to a given range using val's type
// /*AFLA*/  * @val: current value
// /*AFLA*/  * @lo: minimum allowable value
// /*AFLA*/  * @hi: maximum allowable value
// /*AFLA*/  *
// /*AFLA*/  * This macro does no typechecking and uses temporary variables of whatever
// /*AFLA*/  * type the input argument 'val' is.  This is useful when val is an unsigned
// /*AFLA*/  * type and min and max are literals that will otherwise be assigned a signed
// /*AFLA*/  * integer type.
// /*AFLA*/  */
// /*AFLA*/ #define clamp_val(val, lo, hi) clamp_t(typeof(val), val, lo, hi)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * swap - swap value of @a and @b
// /*AFLA*/  */
// /*AFLA*/ #define swap(a, b) \
// /*AFLA*/ 	do { typeof(a) __tmp = (a); (a) = (b); (b) = __tmp; } while (0)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * container_of - cast a member of a structure out to the containing structure
// /*AFLA*/  * @ptr:	the pointer to the member.
// /*AFLA*/  * @type:	the type of the container struct this is embedded in.
// /*AFLA*/  * @member:	the name of the member within the struct.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ #define container_of(ptr, type, member) ({			\
// /*AFLA*/ 	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
// /*AFLA*/ 	(type *)( (char *)__mptr - offsetof(type,member) );})
// /*AFLA*/ 
// /*AFLA*/ /* Rebuild everything on CONFIG_FTRACE_MCOUNT_RECORD */
// /*AFLA*/ #ifdef CONFIG_FTRACE_MCOUNT_RECORD
// /*AFLA*/ # define REBUILD_DUE_TO_FTRACE_MCOUNT_RECORD
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Permissions on a sysfs file: you didn't miss the 0 prefix did you? */
// /*AFLA*/ #define VERIFY_OCTAL_PERMISSIONS(perms)						\
// /*AFLA*/ 	(BUILD_BUG_ON_ZERO((perms) < 0) +					\
// /*AFLA*/ 	 BUILD_BUG_ON_ZERO((perms) > 0777) +					\
// /*AFLA*/ 	 /* USER_READABLE >= GROUP_READABLE >= OTHER_READABLE */		\
// /*AFLA*/ 	 BUILD_BUG_ON_ZERO((((perms) >> 6) & 4) < (((perms) >> 3) & 4)) +	\
// /*AFLA*/ 	 BUILD_BUG_ON_ZERO((((perms) >> 3) & 4) < ((perms) & 4)) +		\
// /*AFLA*/ 	 /* USER_WRITABLE >= GROUP_WRITABLE */					\
// /*AFLA*/ 	 BUILD_BUG_ON_ZERO((((perms) >> 6) & 2) < (((perms) >> 3) & 2)) +	\
// /*AFLA*/ 	 /* OTHER_WRITABLE?  Generally considered a bad idea. */		\
// /*AFLA*/ 	 BUILD_BUG_ON_ZERO((perms) & 2) +					\
// /*AFLA*/ 	 (perms))
// /*AFLA*/ #endif
