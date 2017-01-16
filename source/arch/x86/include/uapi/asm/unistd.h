// /*AFLA*/ #ifndef _UAPI_ASM_X86_UNISTD_H
// /*AFLA*/ #define _UAPI_ASM_X86_UNISTD_H
// /*AFLA*/ 
// /*AFLA*/ /* x32 syscall flag bit */
// /*AFLA*/ #define __X32_SYSCALL_BIT	0x40000000
// /*AFLA*/ 
// /*AFLA*/ #ifndef __KERNEL__
// /*AFLA*/ # ifdef __i386__
// /*AFLA*/ #  include <asm/unistd_32.h>
// /*AFLA*/ # elif defined(__ILP32__)
// /*AFLA*/ #  include <asm/unistd_x32.h>
// /*AFLA*/ # else
// /*AFLA*/ #  include <asm/unistd_64.h>
// /*AFLA*/ # endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_ASM_X86_UNISTD_H */
