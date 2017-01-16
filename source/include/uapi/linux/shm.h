// /*AFLA*/ #ifndef _UAPI_LINUX_SHM_H_
// /*AFLA*/ #define _UAPI_LINUX_SHM_H_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/ipc.h>
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ #ifndef __KERNEL__
// /*AFLA*/ #include <unistd.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * SHMMNI, SHMMAX and SHMALL are default upper limits which can be
// /*AFLA*/  * modified by sysctl. The SHMMAX and SHMALL values have been chosen to
// /*AFLA*/  * be as large possible without facilitating scenarios where userspace
// /*AFLA*/  * causes overflows when adjusting the limits via operations of the form
// /*AFLA*/  * "retrieve current limit; add X; update limit". It is therefore not
// /*AFLA*/  * advised to make SHMMAX and SHMALL any larger. These limits are
// /*AFLA*/  * suitable for both 32 and 64-bit systems.
// /*AFLA*/  */
// /*AFLA*/ #define SHMMIN 1			 /* min shared seg size (bytes) */
// /*AFLA*/ #define SHMMNI 4096			 /* max num of segs system wide */
// /*AFLA*/ #define SHMMAX (ULONG_MAX - (1UL << 24)) /* max shared seg size (bytes) */
// /*AFLA*/ #define SHMALL (ULONG_MAX - (1UL << 24)) /* max shm system wide (pages) */
// /*AFLA*/ #define SHMSEG SHMMNI			 /* max shared segs per process */
// /*AFLA*/ 
// /*AFLA*/ /* Obsolete, used only for backwards compatibility and libc5 compiles */
// /*AFLA*/ struct shmid_ds {
// /*AFLA*/ 	struct ipc_perm		shm_perm;	/* operation perms */
// /*AFLA*/ 	int			shm_segsz;	/* size of segment (bytes) */
// /*AFLA*/ 	__kernel_time_t		shm_atime;	/* last attach time */
// /*AFLA*/ 	__kernel_time_t		shm_dtime;	/* last detach time */
// /*AFLA*/ 	__kernel_time_t		shm_ctime;	/* last change time */
// /*AFLA*/ 	__kernel_ipc_pid_t	shm_cpid;	/* pid of creator */
// /*AFLA*/ 	__kernel_ipc_pid_t	shm_lpid;	/* pid of last operator */
// /*AFLA*/ 	unsigned short		shm_nattch;	/* no. of current attaches */
// /*AFLA*/ 	unsigned short 		shm_unused;	/* compatibility */
// /*AFLA*/ 	void 			*shm_unused2;	/* ditto - used by DIPC */
// /*AFLA*/ 	void			*shm_unused3;	/* unused */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Include the definition of shmid64_ds and shminfo64 */
// /*AFLA*/ #include <asm/shmbuf.h>
// /*AFLA*/ 
// /*AFLA*/ /* permission flag for shmget */
// /*AFLA*/ #define SHM_R		0400	/* or S_IRUGO from <linux/stat.h> */
// /*AFLA*/ #define SHM_W		0200	/* or S_IWUGO from <linux/stat.h> */
// /*AFLA*/ 
// /*AFLA*/ /* mode for attach */
// /*AFLA*/ #define	SHM_RDONLY	010000	/* read-only access */
// /*AFLA*/ #define	SHM_RND		020000	/* round attach address to SHMLBA boundary */
// /*AFLA*/ #define	SHM_REMAP	040000	/* take-over region on attach */
// /*AFLA*/ #define	SHM_EXEC	0100000	/* execution access */
// /*AFLA*/ 
// /*AFLA*/ /* super user shmctl commands */
// /*AFLA*/ #define SHM_LOCK 	11
// /*AFLA*/ #define SHM_UNLOCK 	12
// /*AFLA*/ 
// /*AFLA*/ /* ipcs ctl commands */
// /*AFLA*/ #define SHM_STAT 	13
// /*AFLA*/ #define SHM_INFO 	14
// /*AFLA*/ 
// /*AFLA*/ /* Obsolete, used only for backwards compatibility */
// /*AFLA*/ struct	shminfo {
// /*AFLA*/ 	int shmmax;
// /*AFLA*/ 	int shmmin;
// /*AFLA*/ 	int shmmni;
// /*AFLA*/ 	int shmseg;
// /*AFLA*/ 	int shmall;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct shm_info {
// /*AFLA*/ 	int used_ids;
// /*AFLA*/ 	__kernel_ulong_t shm_tot;	/* total allocated shm */
// /*AFLA*/ 	__kernel_ulong_t shm_rss;	/* total resident shm */
// /*AFLA*/ 	__kernel_ulong_t shm_swp;	/* total swapped shm */
// /*AFLA*/ 	__kernel_ulong_t swap_attempts;
// /*AFLA*/ 	__kernel_ulong_t swap_successes;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_SHM_H_ */
