// /*AFLA*/ #ifndef _UAPI_LINUX_KERNEL_H
// /*AFLA*/ #define _UAPI_LINUX_KERNEL_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/sysinfo.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * 'kernel.h' contains some often-used function prototypes etc
// /*AFLA*/  */
// /*AFLA*/ #define __ALIGN_KERNEL(x, a)		__ALIGN_KERNEL_MASK(x, (typeof(x))(a) - 1)
// /*AFLA*/ #define __ALIGN_KERNEL_MASK(x, mask)	(((x) + (mask)) & ~(mask))
// /*AFLA*/ 
// /*AFLA*/ #define __KERNEL_DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_KERNEL_H */
