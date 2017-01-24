

// BEGIN #include <linux/slab.h>

// !defined(__ASSEMBLY__) && defined(__CHECKER__)
extern void __chk_user_ptr_V0 (const volatile void __attribute__ ((noderef , address_space (1)))*);

// !defined(__ASSEMBLY__) && defined(__CHECKER__)
extern void __chk_io_ptr_V0 (const volatile void __attribute__ ((noderef , address_space (2)))*);

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
void ftrace_likely_update_V2 (struct ftrace_branch_data_V1* f , int val , int expect);

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

// END #include <linux/slab.h>