// /*AFLA*/ #ifndef _ASM_GENERIC_IOCTL_H
// /*AFLA*/ #define _ASM_GENERIC_IOCTL_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/asm-generic/ioctl.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef __CHECKER__
// /*AFLA*/ #define _IOC_TYPECHECK(t) (sizeof(t))
// /*AFLA*/ #else
// /*AFLA*/ /* provoke compile error for invalid uses of size argument */
// /*AFLA*/ extern unsigned int __invalid_size_argument_for_IOC;
// /*AFLA*/ #define _IOC_TYPECHECK(t) \
// /*AFLA*/ 	((sizeof(t) == sizeof(t[1]) && \
// /*AFLA*/ 	  sizeof(t) < (1 << _IOC_SIZEBITS)) ? \
// /*AFLA*/ 	  sizeof(t) : __invalid_size_argument_for_IOC)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_GENERIC_IOCTL_H */
