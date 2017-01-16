// /*AFLA*/ #ifndef _UAPI_ASM_X86_MSR_H
// /*AFLA*/ #define _UAPI_ASM_X86_MSR_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/ioctl.h>
// /*AFLA*/ 
// /*AFLA*/ #define X86_IOC_RDMSR_REGS	_IOWR('c', 0xA0, __u32[8])
// /*AFLA*/ #define X86_IOC_WRMSR_REGS	_IOWR('c', 0xA1, __u32[8])
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ #endif /* _UAPI_ASM_X86_MSR_H */
