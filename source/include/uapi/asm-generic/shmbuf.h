// /*AFLA*/ #ifndef __ASM_GENERIC_SHMBUF_H
// /*AFLA*/ #define __ASM_GENERIC_SHMBUF_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/bitsperlong.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The shmid64_ds structure for x86 architecture.
// /*AFLA*/  * Note extra padding because this structure is passed back and forth
// /*AFLA*/  * between kernel and user space.
// /*AFLA*/  *
// /*AFLA*/  * shmid64_ds was originally meant to be architecture specific, but
// /*AFLA*/  * everyone just ended up making identical copies without specific
// /*AFLA*/  * optimizations, so we may just as well all use the same one.
// /*AFLA*/  *
// /*AFLA*/  * 64 bit architectures typically define a 64 bit __kernel_time_t,
// /*AFLA*/  * so they do not need the first two padding words.
// /*AFLA*/  * On big-endian systems, the padding is in the wrong place.
// /*AFLA*/  *
// /*AFLA*/  *
// /*AFLA*/  * Pad space is left for:
// /*AFLA*/  * - 64-bit time_t to solve y2038 problem
// /*AFLA*/  * - 2 miscellaneous 32-bit values
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct shmid64_ds {
// /*AFLA*/ 	struct ipc64_perm	shm_perm;	/* operation perms */
// /*AFLA*/ 	size_t			shm_segsz;	/* size of segment (bytes) */
// /*AFLA*/ 	__kernel_time_t		shm_atime;	/* last attach time */
// /*AFLA*/ #if __BITS_PER_LONG != 64
// /*AFLA*/ 	unsigned long		__unused1;
// /*AFLA*/ #endif
// /*AFLA*/ 	__kernel_time_t		shm_dtime;	/* last detach time */
// /*AFLA*/ #if __BITS_PER_LONG != 64
// /*AFLA*/ 	unsigned long		__unused2;
// /*AFLA*/ #endif
// /*AFLA*/ 	__kernel_time_t		shm_ctime;	/* last change time */
// /*AFLA*/ #if __BITS_PER_LONG != 64
// /*AFLA*/ 	unsigned long		__unused3;
// /*AFLA*/ #endif
// /*AFLA*/ 	__kernel_pid_t		shm_cpid;	/* pid of creator */
// /*AFLA*/ 	__kernel_pid_t		shm_lpid;	/* pid of last operator */
// /*AFLA*/ 	__kernel_ulong_t	shm_nattch;	/* no. of current attaches */
// /*AFLA*/ 	__kernel_ulong_t	__unused4;
// /*AFLA*/ 	__kernel_ulong_t	__unused5;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct shminfo64 {
// /*AFLA*/ 	__kernel_ulong_t	shmmax;
// /*AFLA*/ 	__kernel_ulong_t	shmmin;
// /*AFLA*/ 	__kernel_ulong_t	shmmni;
// /*AFLA*/ 	__kernel_ulong_t	shmseg;
// /*AFLA*/ 	__kernel_ulong_t	shmall;
// /*AFLA*/ 	__kernel_ulong_t	__unused1;
// /*AFLA*/ 	__kernel_ulong_t	__unused2;
// /*AFLA*/ 	__kernel_ulong_t	__unused3;
// /*AFLA*/ 	__kernel_ulong_t	__unused4;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASM_GENERIC_SHMBUF_H */
