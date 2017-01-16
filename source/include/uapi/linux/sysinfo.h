// /*AFLA*/ #ifndef _LINUX_SYSINFO_H
// /*AFLA*/ #define _LINUX_SYSINFO_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ #define SI_LOAD_SHIFT	16
// /*AFLA*/ struct sysinfo {
// /*AFLA*/ 	__kernel_long_t uptime;		/* Seconds since boot */
// /*AFLA*/ 	__kernel_ulong_t loads[3];	/* 1, 5, and 15 minute load averages */
// /*AFLA*/ 	__kernel_ulong_t totalram;	/* Total usable main memory size */
// /*AFLA*/ 	__kernel_ulong_t freeram;	/* Available memory size */
// /*AFLA*/ 	__kernel_ulong_t sharedram;	/* Amount of shared memory */
// /*AFLA*/ 	__kernel_ulong_t bufferram;	/* Memory used by buffers */
// /*AFLA*/ 	__kernel_ulong_t totalswap;	/* Total swap space size */
// /*AFLA*/ 	__kernel_ulong_t freeswap;	/* swap space still available */
// /*AFLA*/ 	__u16 procs;		   	/* Number of current processes */
// /*AFLA*/ 	__u16 pad;		   	/* Explicit padding for m68k */
// /*AFLA*/ 	__kernel_ulong_t totalhigh;	/* Total high memory size */
// /*AFLA*/ 	__kernel_ulong_t freehigh;	/* Available high memory size */
// /*AFLA*/ 	__u32 mem_unit;			/* Memory unit size in bytes */
// /*AFLA*/ 	char _f[20-2*sizeof(__kernel_ulong_t)-sizeof(__u32)];	/* Padding: libc5 uses this.. */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_SYSINFO_H */
