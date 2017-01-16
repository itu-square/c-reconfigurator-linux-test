// /*AFLA*/ #ifndef __ASM_GENERIC_POSIX_TYPES_H
// /*AFLA*/ #define __ASM_GENERIC_POSIX_TYPES_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/bitsperlong.h>
// /*AFLA*/ /*
// /*AFLA*/  * This file is generally used by user-level software, so you need to
// /*AFLA*/  * be a little careful about namespace pollution etc.
// /*AFLA*/  *
// /*AFLA*/  * First the types that are often defined in different ways across
// /*AFLA*/  * architectures, so that you can override them.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __kernel_long_t
// /*AFLA*/ typedef long		__kernel_long_t;
// /*AFLA*/ typedef unsigned long	__kernel_ulong_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __kernel_ino_t
// /*AFLA*/ typedef __kernel_ulong_t __kernel_ino_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __kernel_mode_t
// /*AFLA*/ typedef unsigned int	__kernel_mode_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __kernel_pid_t
// /*AFLA*/ typedef int		__kernel_pid_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __kernel_ipc_pid_t
// /*AFLA*/ typedef int		__kernel_ipc_pid_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __kernel_uid_t
// /*AFLA*/ typedef unsigned int	__kernel_uid_t;
// /*AFLA*/ typedef unsigned int	__kernel_gid_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __kernel_suseconds_t
// /*AFLA*/ typedef __kernel_long_t		__kernel_suseconds_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __kernel_daddr_t
// /*AFLA*/ typedef int		__kernel_daddr_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __kernel_uid32_t
// /*AFLA*/ typedef unsigned int	__kernel_uid32_t;
// /*AFLA*/ typedef unsigned int	__kernel_gid32_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __kernel_old_uid_t
// /*AFLA*/ typedef __kernel_uid_t	__kernel_old_uid_t;
// /*AFLA*/ typedef __kernel_gid_t	__kernel_old_gid_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __kernel_old_dev_t
// /*AFLA*/ typedef unsigned int	__kernel_old_dev_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Most 32 bit architectures use "unsigned int" size_t,
// /*AFLA*/  * and all 64 bit architectures use "unsigned long" size_t.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __kernel_size_t
// /*AFLA*/ #if __BITS_PER_LONG != 64
// /*AFLA*/ typedef unsigned int	__kernel_size_t;
// /*AFLA*/ typedef int		__kernel_ssize_t;
// /*AFLA*/ typedef int		__kernel_ptrdiff_t;
// /*AFLA*/ #else
// /*AFLA*/ typedef __kernel_ulong_t __kernel_size_t;
// /*AFLA*/ typedef __kernel_long_t	__kernel_ssize_t;
// /*AFLA*/ typedef __kernel_long_t	__kernel_ptrdiff_t;
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __kernel_fsid_t
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	int	val[2];
// /*AFLA*/ } __kernel_fsid_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * anything below here should be completely generic
// /*AFLA*/  */
// /*AFLA*/ typedef __kernel_long_t	__kernel_off_t;
// /*AFLA*/ typedef long long	__kernel_loff_t;
// /*AFLA*/ typedef __kernel_long_t	__kernel_time_t;
// /*AFLA*/ typedef __kernel_long_t	__kernel_clock_t;
// /*AFLA*/ typedef int		__kernel_timer_t;
// /*AFLA*/ typedef int		__kernel_clockid_t;
// /*AFLA*/ typedef char *		__kernel_caddr_t;
// /*AFLA*/ typedef unsigned short	__kernel_uid16_t;
// /*AFLA*/ typedef unsigned short	__kernel_gid16_t;
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASM_GENERIC_POSIX_TYPES_H */
