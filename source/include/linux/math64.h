// /*AFLA*/ #ifndef _LINUX_MATH64_H
// /*AFLA*/ #define _LINUX_MATH64_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <asm/div64.h>
// /*AFLA*/ 
// /*AFLA*/ #if BITS_PER_LONG == 64
// /*AFLA*/ 
// /*AFLA*/ #define div64_long(x, y) div64_s64((x), (y))
// /*AFLA*/ #define div64_ul(x, y)   div64_u64((x), (y))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * div_u64_rem - unsigned 64bit divide with 32bit divisor with remainder
// /*AFLA*/  *
// /*AFLA*/  * This is commonly provided by 32bit archs to provide an optimized 64bit
// /*AFLA*/  * divide.
// /*AFLA*/  */
// /*AFLA*/ static inline u64 div_u64_rem(u64 dividend, u32 divisor, u32 *remainder)
// /*AFLA*/ {
// /*AFLA*/ 	*remainder = dividend % divisor;
// /*AFLA*/ 	return dividend / divisor;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * div_s64_rem - signed 64bit divide with 32bit divisor with remainder
// /*AFLA*/  */
// /*AFLA*/ static inline s64 div_s64_rem(s64 dividend, s32 divisor, s32 *remainder)
// /*AFLA*/ {
// /*AFLA*/ 	*remainder = dividend % divisor;
// /*AFLA*/ 	return dividend / divisor;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * div64_u64_rem - unsigned 64bit divide with 64bit divisor and remainder
// /*AFLA*/  */
// /*AFLA*/ static inline u64 div64_u64_rem(u64 dividend, u64 divisor, u64 *remainder)
// /*AFLA*/ {
// /*AFLA*/ 	*remainder = dividend % divisor;
// /*AFLA*/ 	return dividend / divisor;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * div64_u64 - unsigned 64bit divide with 64bit divisor
// /*AFLA*/  */
// /*AFLA*/ static inline u64 div64_u64(u64 dividend, u64 divisor)
// /*AFLA*/ {
// /*AFLA*/ 	return dividend / divisor;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * div64_s64 - signed 64bit divide with 64bit divisor
// /*AFLA*/  */
// /*AFLA*/ static inline s64 div64_s64(s64 dividend, s64 divisor)
// /*AFLA*/ {
// /*AFLA*/ 	return dividend / divisor;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #elif BITS_PER_LONG == 32
// /*AFLA*/ 
// /*AFLA*/ #define div64_long(x, y) div_s64((x), (y))
// /*AFLA*/ #define div64_ul(x, y)   div_u64((x), (y))
// /*AFLA*/ 
// /*AFLA*/ #ifndef div_u64_rem
// /*AFLA*/ static inline u64 div_u64_rem(u64 dividend, u32 divisor, u32 *remainder)
// /*AFLA*/ {
// /*AFLA*/ 	*remainder = do_div(dividend, divisor);
// /*AFLA*/ 	return dividend;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef div_s64_rem
// /*AFLA*/ extern s64 div_s64_rem(s64 dividend, s32 divisor, s32 *remainder);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef div64_u64_rem
// /*AFLA*/ extern u64 div64_u64_rem(u64 dividend, u64 divisor, u64 *remainder);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef div64_u64
// /*AFLA*/ extern u64 div64_u64(u64 dividend, u64 divisor);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef div64_s64
// /*AFLA*/ extern s64 div64_s64(s64 dividend, s64 divisor);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* BITS_PER_LONG */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * div_u64 - unsigned 64bit divide with 32bit divisor
// /*AFLA*/  *
// /*AFLA*/  * This is the most common 64bit divide and should be used if possible,
// /*AFLA*/  * as many 32bit archs can optimize this variant better than a full 64bit
// /*AFLA*/  * divide.
// /*AFLA*/  */
// /*AFLA*/ #ifndef div_u64
// /*AFLA*/ static inline u64 div_u64(u64 dividend, u32 divisor)
// /*AFLA*/ {
// /*AFLA*/ 	u32 remainder;
// /*AFLA*/ 	return div_u64_rem(dividend, divisor, &remainder);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * div_s64 - signed 64bit divide with 32bit divisor
// /*AFLA*/  */
// /*AFLA*/ #ifndef div_s64
// /*AFLA*/ static inline s64 div_s64(s64 dividend, s32 divisor)
// /*AFLA*/ {
// /*AFLA*/ 	s32 remainder;
// /*AFLA*/ 	return div_s64_rem(dividend, divisor, &remainder);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ u32 iter_div_u64_rem(u64 dividend, u32 divisor, u64 *remainder);
// /*AFLA*/ 
// /*AFLA*/ static __always_inline u32
// /*AFLA*/ __iter_div_u64_rem(u64 dividend, u32 divisor, u64 *remainder)
// /*AFLA*/ {
// /*AFLA*/ 	u32 ret = 0;
// /*AFLA*/ 
// /*AFLA*/ 	while (dividend >= divisor) {
// /*AFLA*/ 		/* The following asm() prevents the compiler from
// /*AFLA*/ 		   optimising this loop into a modulo operation.  */
// /*AFLA*/ 		asm("" : "+rm"(dividend));
// /*AFLA*/ 
// /*AFLA*/ 		dividend -= divisor;
// /*AFLA*/ 		ret++;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	*remainder = dividend;
// /*AFLA*/ 
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_ARCH_SUPPORTS_INT128) && defined(__SIZEOF_INT128__)
// /*AFLA*/ 
// /*AFLA*/ #ifndef mul_u64_u32_shr
// /*AFLA*/ static inline u64 mul_u64_u32_shr(u64 a, u32 mul, unsigned int shift)
// /*AFLA*/ {
// /*AFLA*/ 	return (u64)(((unsigned __int128)a * mul) >> shift);
// /*AFLA*/ }
// /*AFLA*/ #endif /* mul_u64_u32_shr */
// /*AFLA*/ 
// /*AFLA*/ #ifndef mul_u64_u64_shr
// /*AFLA*/ static inline u64 mul_u64_u64_shr(u64 a, u64 mul, unsigned int shift)
// /*AFLA*/ {
// /*AFLA*/ 	return (u64)(((unsigned __int128)a * mul) >> shift);
// /*AFLA*/ }
// /*AFLA*/ #endif /* mul_u64_u64_shr */
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ #ifndef mul_u64_u32_shr
// /*AFLA*/ static inline u64 mul_u64_u32_shr(u64 a, u32 mul, unsigned int shift)
// /*AFLA*/ {
// /*AFLA*/ 	u32 ah, al;
// /*AFLA*/ 	u64 ret;
// /*AFLA*/ 
// /*AFLA*/ 	al = a;
// /*AFLA*/ 	ah = a >> 32;
// /*AFLA*/ 
// /*AFLA*/ 	ret = ((u64)al * mul) >> shift;
// /*AFLA*/ 	if (ah)
// /*AFLA*/ 		ret += ((u64)ah * mul) << (32 - shift);
// /*AFLA*/ 
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ #endif /* mul_u64_u32_shr */
// /*AFLA*/ 
// /*AFLA*/ #ifndef mul_u64_u64_shr
// /*AFLA*/ static inline u64 mul_u64_u64_shr(u64 a, u64 b, unsigned int shift)
// /*AFLA*/ {
// /*AFLA*/ 	union {
// /*AFLA*/ 		u64 ll;
// /*AFLA*/ 		struct {
// /*AFLA*/ #ifdef __BIG_ENDIAN
// /*AFLA*/ 			u32 high, low;
// /*AFLA*/ #else
// /*AFLA*/ 			u32 low, high;
// /*AFLA*/ #endif
// /*AFLA*/ 		} l;
// /*AFLA*/ 	} rl, rm, rn, rh, a0, b0;
// /*AFLA*/ 	u64 c;
// /*AFLA*/ 
// /*AFLA*/ 	a0.ll = a;
// /*AFLA*/ 	b0.ll = b;
// /*AFLA*/ 
// /*AFLA*/ 	rl.ll = (u64)a0.l.low * b0.l.low;
// /*AFLA*/ 	rm.ll = (u64)a0.l.low * b0.l.high;
// /*AFLA*/ 	rn.ll = (u64)a0.l.high * b0.l.low;
// /*AFLA*/ 	rh.ll = (u64)a0.l.high * b0.l.high;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Each of these lines computes a 64-bit intermediate result into "c",
// /*AFLA*/ 	 * starting at bits 32-95.  The low 32-bits go into the result of the
// /*AFLA*/ 	 * multiplication, the high 32-bits are carried into the next step.
// /*AFLA*/ 	 */
// /*AFLA*/ 	rl.l.high = c = (u64)rl.l.high + rm.l.low + rn.l.low;
// /*AFLA*/ 	rh.l.low = c = (c >> 32) + rm.l.high + rn.l.high + rh.l.low;
// /*AFLA*/ 	rh.l.high = (c >> 32) + rh.l.high;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The 128-bit result of the multiplication is in rl.ll and rh.ll,
// /*AFLA*/ 	 * shift it right and throw away the high part of the result.
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (shift == 0)
// /*AFLA*/ 		return rl.ll;
// /*AFLA*/ 	if (shift < 64)
// /*AFLA*/ 		return (rl.ll >> shift) | (rh.ll << (64 - shift));
// /*AFLA*/ 	return rh.ll >> (shift & 63);
// /*AFLA*/ }
// /*AFLA*/ #endif /* mul_u64_u64_shr */
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef mul_u64_u32_div
// /*AFLA*/ static inline u64 mul_u64_u32_div(u64 a, u32 mul, u32 divisor)
// /*AFLA*/ {
// /*AFLA*/ 	union {
// /*AFLA*/ 		u64 ll;
// /*AFLA*/ 		struct {
// /*AFLA*/ #ifdef __BIG_ENDIAN
// /*AFLA*/ 			u32 high, low;
// /*AFLA*/ #else
// /*AFLA*/ 			u32 low, high;
// /*AFLA*/ #endif
// /*AFLA*/ 		} l;
// /*AFLA*/ 	} u, rl, rh;
// /*AFLA*/ 
// /*AFLA*/ 	u.ll = a;
// /*AFLA*/ 	rl.ll = (u64)u.l.low * mul;
// /*AFLA*/ 	rh.ll = (u64)u.l.high * mul + rl.l.high;
// /*AFLA*/ 
// /*AFLA*/ 	/* Bits 32-63 of the result will be in rh.l.low. */
// /*AFLA*/ 	rl.l.high = do_div(rh.ll, divisor);
// /*AFLA*/ 
// /*AFLA*/ 	/* Bits 0-31 of the result will be in rl.l.low.	*/
// /*AFLA*/ 	do_div(rl.ll, divisor);
// /*AFLA*/ 
// /*AFLA*/ 	rl.l.high = rh.l.low;
// /*AFLA*/ 	return rl.ll;
// /*AFLA*/ }
// /*AFLA*/ #endif /* mul_u64_u32_div */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_MATH64_H */
