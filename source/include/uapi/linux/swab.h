// /*AFLA*/ #ifndef _UAPI_LINUX_SWAB_H
// /*AFLA*/ #define _UAPI_LINUX_SWAB_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <asm/swab.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * casts are necessary for constants, because we never know how for sure
// /*AFLA*/  * how U/UL/ULL map to __u16, __u32, __u64. At least not in a portable way.
// /*AFLA*/  */
// /*AFLA*/ #define ___constant_swab16(x) ((__u16)(				\
// /*AFLA*/ 	(((__u16)(x) & (__u16)0x00ffU) << 8) |			\
// /*AFLA*/ 	(((__u16)(x) & (__u16)0xff00U) >> 8)))
// /*AFLA*/ 
// /*AFLA*/ #define ___constant_swab32(x) ((__u32)(				\
// /*AFLA*/ 	(((__u32)(x) & (__u32)0x000000ffUL) << 24) |		\
// /*AFLA*/ 	(((__u32)(x) & (__u32)0x0000ff00UL) <<  8) |		\
// /*AFLA*/ 	(((__u32)(x) & (__u32)0x00ff0000UL) >>  8) |		\
// /*AFLA*/ 	(((__u32)(x) & (__u32)0xff000000UL) >> 24)))
// /*AFLA*/ 
// /*AFLA*/ #define ___constant_swab64(x) ((__u64)(				\
// /*AFLA*/ 	(((__u64)(x) & (__u64)0x00000000000000ffULL) << 56) |	\
// /*AFLA*/ 	(((__u64)(x) & (__u64)0x000000000000ff00ULL) << 40) |	\
// /*AFLA*/ 	(((__u64)(x) & (__u64)0x0000000000ff0000ULL) << 24) |	\
// /*AFLA*/ 	(((__u64)(x) & (__u64)0x00000000ff000000ULL) <<  8) |	\
// /*AFLA*/ 	(((__u64)(x) & (__u64)0x000000ff00000000ULL) >>  8) |	\
// /*AFLA*/ 	(((__u64)(x) & (__u64)0x0000ff0000000000ULL) >> 24) |	\
// /*AFLA*/ 	(((__u64)(x) & (__u64)0x00ff000000000000ULL) >> 40) |	\
// /*AFLA*/ 	(((__u64)(x) & (__u64)0xff00000000000000ULL) >> 56)))
// /*AFLA*/ 
// /*AFLA*/ #define ___constant_swahw32(x) ((__u32)(			\
// /*AFLA*/ 	(((__u32)(x) & (__u32)0x0000ffffUL) << 16) |		\
// /*AFLA*/ 	(((__u32)(x) & (__u32)0xffff0000UL) >> 16)))
// /*AFLA*/ 
// /*AFLA*/ #define ___constant_swahb32(x) ((__u32)(			\
// /*AFLA*/ 	(((__u32)(x) & (__u32)0x00ff00ffUL) << 8) |		\
// /*AFLA*/ 	(((__u32)(x) & (__u32)0xff00ff00UL) >> 8)))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Implement the following as inlines, but define the interface using
// /*AFLA*/  * macros to allow constant folding when possible:
// /*AFLA*/  * ___swab16, ___swab32, ___swab64, ___swahw32, ___swahb32
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static inline __attribute_const__ __u16 __fswab16(__u16 val)
// /*AFLA*/ {
// /*AFLA*/ #if defined (__arch_swab16)
// /*AFLA*/ 	return __arch_swab16(val);
// /*AFLA*/ #else
// /*AFLA*/ 	return ___constant_swab16(val);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline __attribute_const__ __u32 __fswab32(__u32 val)
// /*AFLA*/ {
// /*AFLA*/ #if defined(__arch_swab32)
// /*AFLA*/ 	return __arch_swab32(val);
// /*AFLA*/ #else
// /*AFLA*/ 	return ___constant_swab32(val);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline __attribute_const__ __u64 __fswab64(__u64 val)
// /*AFLA*/ {
// /*AFLA*/ #if defined (__arch_swab64)
// /*AFLA*/ 	return __arch_swab64(val);
// /*AFLA*/ #elif defined(__SWAB_64_THRU_32__)
// /*AFLA*/ 	__u32 h = val >> 32;
// /*AFLA*/ 	__u32 l = val & ((1ULL << 32) - 1);
// /*AFLA*/ 	return (((__u64)__fswab32(l)) << 32) | ((__u64)(__fswab32(h)));
// /*AFLA*/ #else
// /*AFLA*/ 	return ___constant_swab64(val);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline __attribute_const__ __u32 __fswahw32(__u32 val)
// /*AFLA*/ {
// /*AFLA*/ #ifdef __arch_swahw32
// /*AFLA*/ 	return __arch_swahw32(val);
// /*AFLA*/ #else
// /*AFLA*/ 	return ___constant_swahw32(val);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline __attribute_const__ __u32 __fswahb32(__u32 val)
// /*AFLA*/ {
// /*AFLA*/ #ifdef __arch_swahb32
// /*AFLA*/ 	return __arch_swahb32(val);
// /*AFLA*/ #else
// /*AFLA*/ 	return ___constant_swahb32(val);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __swab16 - return a byteswapped 16-bit value
// /*AFLA*/  * @x: value to byteswap
// /*AFLA*/  */
// /*AFLA*/ #ifdef __HAVE_BUILTIN_BSWAP16__
// /*AFLA*/ #define __swab16(x) (__u16)__builtin_bswap16((__u16)(x))
// /*AFLA*/ #else
// /*AFLA*/ #define __swab16(x)				\
// /*AFLA*/ 	(__builtin_constant_p((__u16)(x)) ?	\
// /*AFLA*/ 	___constant_swab16(x) :			\
// /*AFLA*/ 	__fswab16(x))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __swab32 - return a byteswapped 32-bit value
// /*AFLA*/  * @x: value to byteswap
// /*AFLA*/  */
// /*AFLA*/ #ifdef __HAVE_BUILTIN_BSWAP32__
// /*AFLA*/ #define __swab32(x) (__u32)__builtin_bswap32((__u32)(x))
// /*AFLA*/ #else
// /*AFLA*/ #define __swab32(x)				\
// /*AFLA*/ 	(__builtin_constant_p((__u32)(x)) ?	\
// /*AFLA*/ 	___constant_swab32(x) :			\
// /*AFLA*/ 	__fswab32(x))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __swab64 - return a byteswapped 64-bit value
// /*AFLA*/  * @x: value to byteswap
// /*AFLA*/  */
// /*AFLA*/ #ifdef __HAVE_BUILTIN_BSWAP64__
// /*AFLA*/ #define __swab64(x) (__u64)__builtin_bswap64((__u64)(x))
// /*AFLA*/ #else
// /*AFLA*/ #define __swab64(x)				\
// /*AFLA*/ 	(__builtin_constant_p((__u64)(x)) ?	\
// /*AFLA*/ 	___constant_swab64(x) :			\
// /*AFLA*/ 	__fswab64(x))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __swahw32 - return a word-swapped 32-bit value
// /*AFLA*/  * @x: value to wordswap
// /*AFLA*/  *
// /*AFLA*/  * __swahw32(0x12340000) is 0x00001234
// /*AFLA*/  */
// /*AFLA*/ #define __swahw32(x)				\
// /*AFLA*/ 	(__builtin_constant_p((__u32)(x)) ?	\
// /*AFLA*/ 	___constant_swahw32(x) :		\
// /*AFLA*/ 	__fswahw32(x))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __swahb32 - return a high and low byte-swapped 32-bit value
// /*AFLA*/  * @x: value to byteswap
// /*AFLA*/  *
// /*AFLA*/  * __swahb32(0x12345678) is 0x34127856
// /*AFLA*/  */
// /*AFLA*/ #define __swahb32(x)				\
// /*AFLA*/ 	(__builtin_constant_p((__u32)(x)) ?	\
// /*AFLA*/ 	___constant_swahb32(x) :		\
// /*AFLA*/ 	__fswahb32(x))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __swab16p - return a byteswapped 16-bit value from a pointer
// /*AFLA*/  * @p: pointer to a naturally-aligned 16-bit value
// /*AFLA*/  */
// /*AFLA*/ static __always_inline __u16 __swab16p(const __u16 *p)
// /*AFLA*/ {
// /*AFLA*/ #ifdef __arch_swab16p
// /*AFLA*/ 	return __arch_swab16p(p);
// /*AFLA*/ #else
// /*AFLA*/ 	return __swab16(*p);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __swab32p - return a byteswapped 32-bit value from a pointer
// /*AFLA*/  * @p: pointer to a naturally-aligned 32-bit value
// /*AFLA*/  */
// /*AFLA*/ static __always_inline __u32 __swab32p(const __u32 *p)
// /*AFLA*/ {
// /*AFLA*/ #ifdef __arch_swab32p
// /*AFLA*/ 	return __arch_swab32p(p);
// /*AFLA*/ #else
// /*AFLA*/ 	return __swab32(*p);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __swab64p - return a byteswapped 64-bit value from a pointer
// /*AFLA*/  * @p: pointer to a naturally-aligned 64-bit value
// /*AFLA*/  */
// /*AFLA*/ static __always_inline __u64 __swab64p(const __u64 *p)
// /*AFLA*/ {
// /*AFLA*/ #ifdef __arch_swab64p
// /*AFLA*/ 	return __arch_swab64p(p);
// /*AFLA*/ #else
// /*AFLA*/ 	return __swab64(*p);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __swahw32p - return a wordswapped 32-bit value from a pointer
// /*AFLA*/  * @p: pointer to a naturally-aligned 32-bit value
// /*AFLA*/  *
// /*AFLA*/  * See __swahw32() for details of wordswapping.
// /*AFLA*/  */
// /*AFLA*/ static inline __u32 __swahw32p(const __u32 *p)
// /*AFLA*/ {
// /*AFLA*/ #ifdef __arch_swahw32p
// /*AFLA*/ 	return __arch_swahw32p(p);
// /*AFLA*/ #else
// /*AFLA*/ 	return __swahw32(*p);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __swahb32p - return a high and low byteswapped 32-bit value from a pointer
// /*AFLA*/  * @p: pointer to a naturally-aligned 32-bit value
// /*AFLA*/  *
// /*AFLA*/  * See __swahb32() for details of high/low byteswapping.
// /*AFLA*/  */
// /*AFLA*/ static inline __u32 __swahb32p(const __u32 *p)
// /*AFLA*/ {
// /*AFLA*/ #ifdef __arch_swahb32p
// /*AFLA*/ 	return __arch_swahb32p(p);
// /*AFLA*/ #else
// /*AFLA*/ 	return __swahb32(*p);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __swab16s - byteswap a 16-bit value in-place
// /*AFLA*/  * @p: pointer to a naturally-aligned 16-bit value
// /*AFLA*/  */
// /*AFLA*/ static inline void __swab16s(__u16 *p)
// /*AFLA*/ {
// /*AFLA*/ #ifdef __arch_swab16s
// /*AFLA*/ 	__arch_swab16s(p);
// /*AFLA*/ #else
// /*AFLA*/ 	*p = __swab16p(p);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ /**
// /*AFLA*/  * __swab32s - byteswap a 32-bit value in-place
// /*AFLA*/  * @p: pointer to a naturally-aligned 32-bit value
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void __swab32s(__u32 *p)
// /*AFLA*/ {
// /*AFLA*/ #ifdef __arch_swab32s
// /*AFLA*/ 	__arch_swab32s(p);
// /*AFLA*/ #else
// /*AFLA*/ 	*p = __swab32p(p);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __swab64s - byteswap a 64-bit value in-place
// /*AFLA*/  * @p: pointer to a naturally-aligned 64-bit value
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void __swab64s(__u64 *p)
// /*AFLA*/ {
// /*AFLA*/ #ifdef __arch_swab64s
// /*AFLA*/ 	__arch_swab64s(p);
// /*AFLA*/ #else
// /*AFLA*/ 	*p = __swab64p(p);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __swahw32s - wordswap a 32-bit value in-place
// /*AFLA*/  * @p: pointer to a naturally-aligned 32-bit value
// /*AFLA*/  *
// /*AFLA*/  * See __swahw32() for details of wordswapping
// /*AFLA*/  */
// /*AFLA*/ static inline void __swahw32s(__u32 *p)
// /*AFLA*/ {
// /*AFLA*/ #ifdef __arch_swahw32s
// /*AFLA*/ 	__arch_swahw32s(p);
// /*AFLA*/ #else
// /*AFLA*/ 	*p = __swahw32p(p);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __swahb32s - high and low byteswap a 32-bit value in-place
// /*AFLA*/  * @p: pointer to a naturally-aligned 32-bit value
// /*AFLA*/  *
// /*AFLA*/  * See __swahb32() for details of high and low byte swapping
// /*AFLA*/  */
// /*AFLA*/ static inline void __swahb32s(__u32 *p)
// /*AFLA*/ {
// /*AFLA*/ #ifdef __arch_swahb32s
// /*AFLA*/ 	__arch_swahb32s(p);
// /*AFLA*/ #else
// /*AFLA*/ 	*p = __swahb32p(p);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_SWAB_H */
