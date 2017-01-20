#ifndef _UAPI_LINUX_TYPES_H
#define _UAPI_LINUX_TYPES_H

#include <asm/types.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #ifndef	__KERNEL__
// /*AFLA*/ #ifndef __EXPORTED_HEADERS__
// /*AFLA*/ #warning "Attempt to use kernel headers from user space, see http://kernelnewbies.org/KernelHeaders"
// /*AFLA*/ #endif /* __EXPORTED_HEADERS__ */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <linux/posix_types.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Below are truly Linux-specific types that should never collide with
// /*AFLA*/  * any application/library that wants linux/types.h.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifdef __CHECKER__
// /*AFLA*/ #define __bitwise__ __attribute__((bitwise))
// /*AFLA*/ #else
// /*AFLA*/ #define __bitwise__
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef __CHECK_ENDIAN__
// /*AFLA*/ #define __bitwise __bitwise__
// /*AFLA*/ #else
// /*AFLA*/ #define __bitwise
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ typedef __u16 __bitwise __le16;
// /*AFLA*/ typedef __u16 __bitwise __be16;
// /*AFLA*/ typedef __u32 __bitwise __le32;
// /*AFLA*/ typedef __u32 __bitwise __be32;
// /*AFLA*/ typedef __u64 __bitwise __le64;
// /*AFLA*/ typedef __u64 __bitwise __be64;
// /*AFLA*/ 
// /*AFLA*/ typedef __u16 __bitwise __sum16;
// /*AFLA*/ typedef __u32 __bitwise __wsum;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * aligned_u64 should be used in defining kernel<->userspace ABIs to avoid
// /*AFLA*/  * common 32/64-bit compat problems.
// /*AFLA*/  * 64-bit values align to 4-byte boundaries on x86_32 (and possibly other
// /*AFLA*/  * architectures) and to 8-byte boundaries on 64-bit architectures.  The new
// /*AFLA*/  * aligned_64 type enforces 8-byte alignment so that structs containing
// /*AFLA*/  * aligned_64 values have the same alignment on 32-bit and 64-bit architectures.
// /*AFLA*/  * No conversions are necessary between 32-bit user-space and a 64-bit kernel.
// /*AFLA*/  */
// /*AFLA*/ #define __aligned_u64 __u64 __attribute__((aligned(8)))
// /*AFLA*/ #define __aligned_be64 __be64 __attribute__((aligned(8)))
// /*AFLA*/ #define __aligned_le64 __le64 __attribute__((aligned(8)))
// /*AFLA*/ 
// /*AFLA*/ #endif /*  __ASSEMBLY__ */
#endif /* _UAPI_LINUX_TYPES_H */
