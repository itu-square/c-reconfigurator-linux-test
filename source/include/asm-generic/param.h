// /*AFLA*/ #ifndef __ASM_GENERIC_PARAM_H
// /*AFLA*/ #define __ASM_GENERIC_PARAM_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/asm-generic/param.h>
// /*AFLA*/ 
// /*AFLA*/ # undef HZ
// /*AFLA*/ # define HZ		CONFIG_HZ	/* Internal kernel timer frequency */
// /*AFLA*/ # define USER_HZ	100		/* some user interfaces are */
// /*AFLA*/ # define CLOCKS_PER_SEC	(USER_HZ)       /* in "ticks" like times() */
// /*AFLA*/ #endif /* __ASM_GENERIC_PARAM_H */
