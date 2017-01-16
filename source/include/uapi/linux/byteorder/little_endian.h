// /*AFLA*/ #ifndef _UAPI_LINUX_BYTEORDER_LITTLE_ENDIAN_H
// /*AFLA*/ #define _UAPI_LINUX_BYTEORDER_LITTLE_ENDIAN_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef __LITTLE_ENDIAN
// /*AFLA*/ #define __LITTLE_ENDIAN 1234
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __LITTLE_ENDIAN_BITFIELD
// /*AFLA*/ #define __LITTLE_ENDIAN_BITFIELD
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/swab.h>
// /*AFLA*/ 
// /*AFLA*/ #define __constant_htonl(x) ((__force __be32)___constant_swab32((x)))
// /*AFLA*/ #define __constant_ntohl(x) ___constant_swab32((__force __be32)(x))
// /*AFLA*/ #define __constant_htons(x) ((__force __be16)___constant_swab16((x)))
// /*AFLA*/ #define __constant_ntohs(x) ___constant_swab16((__force __be16)(x))
// /*AFLA*/ #define __constant_cpu_to_le64(x) ((__force __le64)(__u64)(x))
// /*AFLA*/ #define __constant_le64_to_cpu(x) ((__force __u64)(__le64)(x))
// /*AFLA*/ #define __constant_cpu_to_le32(x) ((__force __le32)(__u32)(x))
// /*AFLA*/ #define __constant_le32_to_cpu(x) ((__force __u32)(__le32)(x))
// /*AFLA*/ #define __constant_cpu_to_le16(x) ((__force __le16)(__u16)(x))
// /*AFLA*/ #define __constant_le16_to_cpu(x) ((__force __u16)(__le16)(x))
// /*AFLA*/ #define __constant_cpu_to_be64(x) ((__force __be64)___constant_swab64((x)))
// /*AFLA*/ #define __constant_be64_to_cpu(x) ___constant_swab64((__force __u64)(__be64)(x))
// /*AFLA*/ #define __constant_cpu_to_be32(x) ((__force __be32)___constant_swab32((x)))
// /*AFLA*/ #define __constant_be32_to_cpu(x) ___constant_swab32((__force __u32)(__be32)(x))
// /*AFLA*/ #define __constant_cpu_to_be16(x) ((__force __be16)___constant_swab16((x)))
// /*AFLA*/ #define __constant_be16_to_cpu(x) ___constant_swab16((__force __u16)(__be16)(x))
// /*AFLA*/ #define __cpu_to_le64(x) ((__force __le64)(__u64)(x))
// /*AFLA*/ #define __le64_to_cpu(x) ((__force __u64)(__le64)(x))
// /*AFLA*/ #define __cpu_to_le32(x) ((__force __le32)(__u32)(x))
// /*AFLA*/ #define __le32_to_cpu(x) ((__force __u32)(__le32)(x))
// /*AFLA*/ #define __cpu_to_le16(x) ((__force __le16)(__u16)(x))
// /*AFLA*/ #define __le16_to_cpu(x) ((__force __u16)(__le16)(x))
// /*AFLA*/ #define __cpu_to_be64(x) ((__force __be64)__swab64((x)))
// /*AFLA*/ #define __be64_to_cpu(x) __swab64((__force __u64)(__be64)(x))
// /*AFLA*/ #define __cpu_to_be32(x) ((__force __be32)__swab32((x)))
// /*AFLA*/ #define __be32_to_cpu(x) __swab32((__force __u32)(__be32)(x))
// /*AFLA*/ #define __cpu_to_be16(x) ((__force __be16)__swab16((x)))
// /*AFLA*/ #define __be16_to_cpu(x) __swab16((__force __u16)(__be16)(x))
// /*AFLA*/ 
// /*AFLA*/ static __always_inline __le64 __cpu_to_le64p(const __u64 *p)
// /*AFLA*/ {
// /*AFLA*/ 	return (__force __le64)*p;
// /*AFLA*/ }
// /*AFLA*/ static __always_inline __u64 __le64_to_cpup(const __le64 *p)
// /*AFLA*/ {
// /*AFLA*/ 	return (__force __u64)*p;
// /*AFLA*/ }
// /*AFLA*/ static __always_inline __le32 __cpu_to_le32p(const __u32 *p)
// /*AFLA*/ {
// /*AFLA*/ 	return (__force __le32)*p;
// /*AFLA*/ }
// /*AFLA*/ static __always_inline __u32 __le32_to_cpup(const __le32 *p)
// /*AFLA*/ {
// /*AFLA*/ 	return (__force __u32)*p;
// /*AFLA*/ }
// /*AFLA*/ static __always_inline __le16 __cpu_to_le16p(const __u16 *p)
// /*AFLA*/ {
// /*AFLA*/ 	return (__force __le16)*p;
// /*AFLA*/ }
// /*AFLA*/ static __always_inline __u16 __le16_to_cpup(const __le16 *p)
// /*AFLA*/ {
// /*AFLA*/ 	return (__force __u16)*p;
// /*AFLA*/ }
// /*AFLA*/ static __always_inline __be64 __cpu_to_be64p(const __u64 *p)
// /*AFLA*/ {
// /*AFLA*/ 	return (__force __be64)__swab64p(p);
// /*AFLA*/ }
// /*AFLA*/ static __always_inline __u64 __be64_to_cpup(const __be64 *p)
// /*AFLA*/ {
// /*AFLA*/ 	return __swab64p((__u64 *)p);
// /*AFLA*/ }
// /*AFLA*/ static __always_inline __be32 __cpu_to_be32p(const __u32 *p)
// /*AFLA*/ {
// /*AFLA*/ 	return (__force __be32)__swab32p(p);
// /*AFLA*/ }
// /*AFLA*/ static __always_inline __u32 __be32_to_cpup(const __be32 *p)
// /*AFLA*/ {
// /*AFLA*/ 	return __swab32p((__u32 *)p);
// /*AFLA*/ }
// /*AFLA*/ static __always_inline __be16 __cpu_to_be16p(const __u16 *p)
// /*AFLA*/ {
// /*AFLA*/ 	return (__force __be16)__swab16p(p);
// /*AFLA*/ }
// /*AFLA*/ static __always_inline __u16 __be16_to_cpup(const __be16 *p)
// /*AFLA*/ {
// /*AFLA*/ 	return __swab16p((__u16 *)p);
// /*AFLA*/ }
// /*AFLA*/ #define __cpu_to_le64s(x) do { (void)(x); } while (0)
// /*AFLA*/ #define __le64_to_cpus(x) do { (void)(x); } while (0)
// /*AFLA*/ #define __cpu_to_le32s(x) do { (void)(x); } while (0)
// /*AFLA*/ #define __le32_to_cpus(x) do { (void)(x); } while (0)
// /*AFLA*/ #define __cpu_to_le16s(x) do { (void)(x); } while (0)
// /*AFLA*/ #define __le16_to_cpus(x) do { (void)(x); } while (0)
// /*AFLA*/ #define __cpu_to_be64s(x) __swab64s((x))
// /*AFLA*/ #define __be64_to_cpus(x) __swab64s((x))
// /*AFLA*/ #define __cpu_to_be32s(x) __swab32s((x))
// /*AFLA*/ #define __be32_to_cpus(x) __swab32s((x))
// /*AFLA*/ #define __cpu_to_be16s(x) __swab16s((x))
// /*AFLA*/ #define __be16_to_cpus(x) __swab16s((x))
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_BYTEORDER_LITTLE_ENDIAN_H */
