// /*AFLA*/ #ifndef _ASM_X86_SEMBUF_H
// /*AFLA*/ #define _ASM_X86_SEMBUF_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The semid64_ds structure for x86 architecture.
// /*AFLA*/  * Note extra padding because this structure is passed back and forth
// /*AFLA*/  * between kernel and user space.
// /*AFLA*/  *
// /*AFLA*/  * Pad space is left for:
// /*AFLA*/  * - 64-bit time_t to solve y2038 problem
// /*AFLA*/  * - 2 miscellaneous 32-bit values
// /*AFLA*/  */
// /*AFLA*/ struct semid64_ds {
// /*AFLA*/ 	struct ipc64_perm sem_perm;	/* permissions .. see ipc.h */
// /*AFLA*/ 	__kernel_time_t	sem_otime;	/* last semop time */
// /*AFLA*/ 	__kernel_ulong_t __unused1;
// /*AFLA*/ 	__kernel_time_t	sem_ctime;	/* last change time */
// /*AFLA*/ 	__kernel_ulong_t __unused2;
// /*AFLA*/ 	__kernel_ulong_t sem_nsems;	/* no. of semaphores in array */
// /*AFLA*/ 	__kernel_ulong_t __unused3;
// /*AFLA*/ 	__kernel_ulong_t __unused4;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_SEMBUF_H */
