// /*AFLA*/ #ifndef _UAPI_LINUX_KDEV_T_H
// /*AFLA*/ #define _UAPI_LINUX_KDEV_T_H
// /*AFLA*/ #ifndef __KERNEL__
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/ Some programs want their definitions of MAJOR and MINOR and MKDEV
// /*AFLA*/ from the kernel sources. These must be the externally visible ones.
// /*AFLA*/ */
// /*AFLA*/ #define MAJOR(dev)	((dev)>>8)
// /*AFLA*/ #define MINOR(dev)	((dev) & 0xff)
// /*AFLA*/ #define MKDEV(ma,mi)	((ma)<<8 | (mi))
// /*AFLA*/ #endif /* __KERNEL__ */
// /*AFLA*/ #endif /* _UAPI_LINUX_KDEV_T_H */
