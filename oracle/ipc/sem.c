

// BEGIN #include <linux/slab.h>

// !defined(__ASSEMBLY__) && defined(__CHECKER__)
extern void __chk_user_ptr_V1 (const volatile void __attribute__ ((noderef , address_space (1)))*);

// !defined(__ASSEMBLY__) && defined(__CHECKER__)
extern void __chk_io_ptr_V1 (const volatile void __attribute__ ((noderef , address_space (2)))*);

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
struct ftrace_branch_data_V1
{
    const char* func;
    const char* file;
    unsigned line;
    union
    {
        struct
        {
            unsigned long correct;
            unsigned long incorrect;
        }
        ;
        struct
        {
            unsigned long miss;
            unsigned long hit;
        }
        ;
        unsigned long miss_hit [ 2 ];
    }
    ;
}
;

// !defined(__ASSEMBLY__) && !defined(__CHECKER__) && defined(__KERNEL__) && defined(CONFIG_TRACE_BRANCH_PROFILING) && !defined(DISABLE_BRANCH_PROFILING)
void ftrace_likely_update_V1 (struct ftrace_branch_data_V1* f , int val , int expect);

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef __signed__ char __s8_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef unsigned char __u8_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef __signed__ short __s16_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef unsigned short __u16_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef __signed__ int __s32_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef unsigned int __u32_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
__extension__
 typedef __signed__ long long __s64_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
__extension__
 typedef unsigned long long __u64_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef signed char s8_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef unsigned char u8_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef signed short s16_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef unsigned short u16_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef signed int s32_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef unsigned int u32_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef signed long long s64_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef unsigned long long u64_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
enum
{
    false_V1 = 0 , true_V1 = 1}
;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef struct
{
    unsigned long fds_bits [ 1024 / (8 * sizeof (long)) ];
}
__kernel_fd_set_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef void (* __kernel_sighandler_t_V1) (int);

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef int __kernel_key_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef int __kernel_mqd_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef unsigned short __kernel_old_uid_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef unsigned short __kernel_old_gid_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef unsigned long __kernel_old_dev_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__kernel_long_t)
typedef long __kernel_long_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__kernel_long_t)
typedef unsigned long __kernel_ulong_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__kernel_long_t) && !defined(__kernel_ino_t)
typedef __kernel_ulong_t_V1 __kernel_ino_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && defined(__kernel_long_t) && !defined(__kernel_ino_t)
typedef __kernel_ulong_t __kernel_ino_t_V2;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__kernel_mode_t)
typedef unsigned int __kernel_mode_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__kernel_pid_t)
typedef int __kernel_pid_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__kernel_ipc_pid_t)
typedef int __kernel_ipc_pid_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__kernel_uid_t)
typedef unsigned int __kernel_uid_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__kernel_uid_t)
typedef unsigned int __kernel_gid_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__kernel_long_t) && !defined(__kernel_suseconds_t)
typedef __kernel_long_t_V1 __kernel_suseconds_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && defined(__kernel_long_t) && !defined(__kernel_suseconds_t)
typedef __kernel_long_t __kernel_suseconds_t_V2;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__kernel_daddr_t)
typedef int __kernel_daddr_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__kernel_uid32_t)
typedef unsigned int __kernel_uid32_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__kernel_uid32_t)
typedef unsigned int __kernel_gid32_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && defined(__ILP32__) && !defined(__kernel_size_t)
typedef unsigned int __kernel_size_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && defined(__ILP32__) && !defined(__kernel_size_t)
typedef int __kernel_ssize_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__ILP32__) && !defined(__kernel_long_t) && !defined(__kernel_size_t)
typedef __kernel_ulong_t_V1 __kernel_size_t_V2;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__ILP32__) && defined(__kernel_long_t) && !defined(__kernel_size_t)
typedef __kernel_ulong_t __kernel_size_t_V3;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__ILP32__) && !defined(__kernel_long_t) && !defined(__kernel_size_t)
typedef __kernel_long_t_V1 __kernel_ssize_t_V2;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__ILP32__) && defined(__kernel_long_t) && !defined(__kernel_size_t)
typedef __kernel_long_t __kernel_ssize_t_V3;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && defined(__ILP32__) && !defined(__kernel_size_t)
typedef int __kernel_ptrdiff_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__ILP32__) && !defined(__kernel_long_t) && !defined(__kernel_size_t)
typedef __kernel_long_t_V1 __kernel_ptrdiff_t_V2;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__ILP32__) && defined(__kernel_long_t) && !defined(__kernel_size_t)
typedef __kernel_long_t __kernel_ptrdiff_t_V3;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__kernel_fsid_t)
typedef struct
{
    int val [ 2 ];
}
__kernel_fsid_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__kernel_long_t)
typedef __kernel_long_t_V1 __kernel_off_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && defined(__kernel_long_t)
typedef __kernel_long_t __kernel_off_t_V2;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef long long __kernel_loff_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__kernel_long_t)
typedef __kernel_long_t_V1 __kernel_time_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && defined(__kernel_long_t)
typedef __kernel_long_t __kernel_time_t_V2;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__kernel_long_t)
typedef __kernel_long_t_V1 __kernel_clock_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && defined(__kernel_long_t)
typedef __kernel_long_t __kernel_clock_t_V2;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef int __kernel_timer_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef int __kernel_clockid_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef char* __kernel_caddr_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef unsigned short __kernel_uid16_t_V1;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
typedef unsigned short __kernel_gid16_t_V1;

// !defined(__ASSEMBLY__) && !defined(__CHECKER__) && defined(__KERNEL__) || !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__CHECK_ENDIAN__)
typedef __u16_V1 __le16_V1;

// !defined(__ASSEMBLY__) && defined(__CHECKER__) && defined(__KERNEL__) && defined(__CHECK_ENDIAN__)
typedef __u16_V1 __attribute__ ((bitwise)) __le16_V2;

// !defined(__ASSEMBLY__) && !defined(__CHECKER__) && defined(__KERNEL__) || !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__CHECK_ENDIAN__)
typedef __u16_V1 __be16_V1;

// !defined(__ASSEMBLY__) && defined(__CHECKER__) && defined(__KERNEL__) && defined(__CHECK_ENDIAN__)
typedef __u16_V1 __attribute__ ((bitwise)) __be16_V2;

// !defined(__ASSEMBLY__) && !defined(__CHECKER__) && defined(__KERNEL__) || !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__CHECK_ENDIAN__)
typedef __u32_V1 __le32_V1;

// !defined(__ASSEMBLY__) && defined(__CHECKER__) && defined(__KERNEL__) && defined(__CHECK_ENDIAN__)
typedef __u32_V1 __attribute__ ((bitwise)) __le32_V2;

// !defined(__ASSEMBLY__) && !defined(__CHECKER__) && defined(__KERNEL__) || !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__CHECK_ENDIAN__)
typedef __u32_V1 __be32_V1;

// !defined(__ASSEMBLY__) && defined(__CHECKER__) && defined(__KERNEL__) && defined(__CHECK_ENDIAN__)
typedef __u32_V1 __attribute__ ((bitwise)) __be32_V2;

// !defined(__ASSEMBLY__) && !defined(__CHECKER__) && defined(__KERNEL__) || !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__CHECK_ENDIAN__)
typedef __u64_V1 __le64_V1;

// !defined(__ASSEMBLY__) && defined(__CHECKER__) && defined(__KERNEL__) && defined(__CHECK_ENDIAN__)
typedef __u64_V1 __attribute__ ((bitwise)) __le64_V2;

// !defined(__ASSEMBLY__) && !defined(__CHECKER__) && defined(__KERNEL__) || !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__CHECK_ENDIAN__)
typedef __u64_V1 __be64_V1;

// !defined(__ASSEMBLY__) && defined(__CHECKER__) && defined(__KERNEL__) && defined(__CHECK_ENDIAN__)
typedef __u64_V1 __attribute__ ((bitwise)) __be64_V2;

// !defined(__ASSEMBLY__) && !defined(__CHECKER__) && defined(__KERNEL__) || !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__CHECK_ENDIAN__)
typedef __u16_V1 __sum16_V1;

// !defined(__ASSEMBLY__) && defined(__CHECKER__) && defined(__KERNEL__) && defined(__CHECK_ENDIAN__)
typedef __u16_V1 __attribute__ ((bitwise)) __sum16_V2;

// !defined(__ASSEMBLY__) && !defined(__CHECKER__) && defined(__KERNEL__) || !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(__CHECK_ENDIAN__)
typedef __u32_V1 __wsum_V1;

// !defined(__ASSEMBLY__) && defined(__CHECKER__) && defined(__KERNEL__) && defined(__CHECK_ENDIAN__)
typedef __u32_V1 __attribute__ ((bitwise)) __wsum_V2;

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
static void __read_once_size_V1 (const volatile void* p , void* res , int size)
{
    (
    {
        switch (size)
        {
            case 1 : * (__u8*) res = * (volatile __u8*) p;
            break;
            case 2 : * (__u16*) res = * (volatile __u16*) p;
            break;
            case 4 : * (__u32*) res = * (volatile __u32*) p;
            break;
            case 8 : * (__u64*) res = * (volatile __u64*) p;
            break;
            default : __asm__ __volatile__ ("" : : : "memory");
            __builtin_memcpy ((void*) res , (const void*) p , size);
            __asm__ __volatile__ ("" : : : "memory");
        }
    }
    );
}

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && defined(CONFIG_KASAN)
static __attribute__ ((no_sanitize_address)) __attribute__ ((unused)) void __read_once_size_nocheck_V1 (const volatile void* p , void* res , int size)
{
    (
    {
        switch (size)
        {
            case 1 : * (__u8*) res = * (volatile __u8*) p;
            break;
            case 2 : * (__u16*) res = * (volatile __u16*) p;
            break;
            case 4 : * (__u32*) res = * (volatile __u32*) p;
            break;
            case 8 : * (__u64*) res = * (volatile __u64*) p;
            break;
            default : __asm__ __volatile__ ("" : : : "memory");
            __builtin_memcpy ((void*) res , (const void*) p , size);
            __asm__ __volatile__ ("" : : : "memory");
        }
    }
    );
}

// !defined(__ASSEMBLY__) && defined(__KERNEL__) && !defined(CONFIG_KASAN)
static void __read_once_size_nocheck_V2 (const volatile void* p , void* res , int size)
{
    (
    {
        switch (size)
        {
            case 1 : * (__u8*) res = * (volatile __u8*) p;
            break;
            case 2 : * (__u16*) res = * (volatile __u16*) p;
            break;
            case 4 : * (__u32*) res = * (volatile __u32*) p;
            break;
            case 8 : * (__u64*) res = * (volatile __u64*) p;
            break;
            default : __asm__ __volatile__ ("" : : : "memory");
            __builtin_memcpy ((void*) res , (const void*) p , size);
            __asm__ __volatile__ ("" : : : "memory");
        }
    }
    );
}

// !defined(__ASSEMBLY__) && defined(__KERNEL__)
static void __write_once_size_V1 (volatile void* p , void* res , int size)
{
    switch (size)
    {
        case 1 : * (volatile __u8*) p = * (__u8*) res;
        break;
        case 2 : * (volatile __u16*) p = * (__u16*) res;
        break;
        case 4 : * (volatile __u32*) p = * (__u32*) res;
        break;
        case 8 : * (volatile __u64*) p = * (__u64*) res;
        break;
        default : __asm__ __volatile__ ("" : : : "memory");
        __builtin_memcpy ((void*) p , (const void*) res , size);
        __asm__ __volatile__ ("" : : : "memory");
    }
}

// END #include <linux/slab.h>