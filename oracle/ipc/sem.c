

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

// END #include <linux/slab.h>