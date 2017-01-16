// /*AFLA*/ #ifndef _ASM_X86_POSIX_TYPES_64_H
// /*AFLA*/ #define _ASM_X86_POSIX_TYPES_64_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This file is generally used by user-level software, so you need to
// /*AFLA*/  * be a little careful about namespace pollution etc.  Also, we cannot
// /*AFLA*/  * assume GCC is being used.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ typedef unsigned short __kernel_old_uid_t;
// /*AFLA*/ typedef unsigned short __kernel_old_gid_t;
// /*AFLA*/ #define __kernel_old_uid_t __kernel_old_uid_t
// /*AFLA*/ 
// /*AFLA*/ typedef unsigned long	__kernel_old_dev_t;
// /*AFLA*/ #define __kernel_old_dev_t __kernel_old_dev_t
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/posix_types.h>
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_POSIX_TYPES_64_H */
