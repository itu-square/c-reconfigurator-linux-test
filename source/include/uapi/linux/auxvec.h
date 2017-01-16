// /*AFLA*/ #ifndef _UAPI_LINUX_AUXVEC_H
// /*AFLA*/ #define _UAPI_LINUX_AUXVEC_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/auxvec.h>
// /*AFLA*/ 
// /*AFLA*/ /* Symbolic values for the entries in the auxiliary table
// /*AFLA*/    put on the initial stack */
// /*AFLA*/ #define AT_NULL   0	/* end of vector */
// /*AFLA*/ #define AT_IGNORE 1	/* entry should be ignored */
// /*AFLA*/ #define AT_EXECFD 2	/* file descriptor of program */
// /*AFLA*/ #define AT_PHDR   3	/* program headers for program */
// /*AFLA*/ #define AT_PHENT  4	/* size of program header entry */
// /*AFLA*/ #define AT_PHNUM  5	/* number of program headers */
// /*AFLA*/ #define AT_PAGESZ 6	/* system page size */
// /*AFLA*/ #define AT_BASE   7	/* base address of interpreter */
// /*AFLA*/ #define AT_FLAGS  8	/* flags */
// /*AFLA*/ #define AT_ENTRY  9	/* entry point of program */
// /*AFLA*/ #define AT_NOTELF 10	/* program is not ELF */
// /*AFLA*/ #define AT_UID    11	/* real uid */
// /*AFLA*/ #define AT_EUID   12	/* effective uid */
// /*AFLA*/ #define AT_GID    13	/* real gid */
// /*AFLA*/ #define AT_EGID   14	/* effective gid */
// /*AFLA*/ #define AT_PLATFORM 15  /* string identifying CPU for optimizations */
// /*AFLA*/ #define AT_HWCAP  16    /* arch dependent hints at CPU capabilities */
// /*AFLA*/ #define AT_CLKTCK 17	/* frequency at which times() increments */
// /*AFLA*/ /* AT_* values 18 through 22 are reserved */
// /*AFLA*/ #define AT_SECURE 23   /* secure mode boolean */
// /*AFLA*/ #define AT_BASE_PLATFORM 24	/* string identifying real platform, may
// /*AFLA*/ 				 * differ from AT_PLATFORM. */
// /*AFLA*/ #define AT_RANDOM 25	/* address of 16 random bytes */
// /*AFLA*/ #define AT_HWCAP2 26	/* extension of AT_HWCAP */
// /*AFLA*/ 
// /*AFLA*/ #define AT_EXECFN  31	/* filename of program */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_AUXVEC_H */
