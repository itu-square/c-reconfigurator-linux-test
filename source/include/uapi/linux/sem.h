// /*AFLA*/ #ifndef _UAPI_LINUX_SEM_H
// /*AFLA*/ #define _UAPI_LINUX_SEM_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/ipc.h>
// /*AFLA*/ 
// /*AFLA*/ /* semop flags */
// /*AFLA*/ #define SEM_UNDO        0x1000  /* undo the operation on exit */
// /*AFLA*/ 
// /*AFLA*/ /* semctl Command Definitions. */
// /*AFLA*/ #define GETPID  11       /* get sempid */
// /*AFLA*/ #define GETVAL  12       /* get semval */
// /*AFLA*/ #define GETALL  13       /* get all semval's */
// /*AFLA*/ #define GETNCNT 14       /* get semncnt */
// /*AFLA*/ #define GETZCNT 15       /* get semzcnt */
// /*AFLA*/ #define SETVAL  16       /* set semval */
// /*AFLA*/ #define SETALL  17       /* set all semval's */
// /*AFLA*/ 
// /*AFLA*/ /* ipcs ctl cmds */
// /*AFLA*/ #define SEM_STAT 18
// /*AFLA*/ #define SEM_INFO 19
// /*AFLA*/ 
// /*AFLA*/ /* Obsolete, used only for backwards compatibility and libc5 compiles */
// /*AFLA*/ struct semid_ds {
// /*AFLA*/ 	struct ipc_perm	sem_perm;		/* permissions .. see ipc.h */
// /*AFLA*/ 	__kernel_time_t	sem_otime;		/* last semop time */
// /*AFLA*/ 	__kernel_time_t	sem_ctime;		/* last change time */
// /*AFLA*/ 	struct sem	*sem_base;		/* ptr to first semaphore in array */
// /*AFLA*/ 	struct sem_queue *sem_pending;		/* pending operations to be processed */
// /*AFLA*/ 	struct sem_queue **sem_pending_last;	/* last pending operation */
// /*AFLA*/ 	struct sem_undo	*undo;			/* undo requests on this array */
// /*AFLA*/ 	unsigned short	sem_nsems;		/* no. of semaphores in array */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Include the definition of semid64_ds */
// /*AFLA*/ #include <asm/sembuf.h>
// /*AFLA*/ 
// /*AFLA*/ /* semop system calls takes an array of these. */
// /*AFLA*/ struct sembuf {
// /*AFLA*/ 	unsigned short  sem_num;	/* semaphore index in array */
// /*AFLA*/ 	short		sem_op;		/* semaphore operation */
// /*AFLA*/ 	short		sem_flg;	/* operation flags */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* arg for semctl system calls. */
// /*AFLA*/ union semun {
// /*AFLA*/ 	int val;			/* value for SETVAL */
// /*AFLA*/ 	struct semid_ds __user *buf;	/* buffer for IPC_STAT & IPC_SET */
// /*AFLA*/ 	unsigned short __user *array;	/* array for GETALL & SETALL */
// /*AFLA*/ 	struct seminfo __user *__buf;	/* buffer for IPC_INFO */
// /*AFLA*/ 	void __user *__pad;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct  seminfo {
// /*AFLA*/ 	int semmap;
// /*AFLA*/ 	int semmni;
// /*AFLA*/ 	int semmns;
// /*AFLA*/ 	int semmnu;
// /*AFLA*/ 	int semmsl;
// /*AFLA*/ 	int semopm;
// /*AFLA*/ 	int semume;
// /*AFLA*/ 	int semusz;
// /*AFLA*/ 	int semvmx;
// /*AFLA*/ 	int semaem;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * SEMMNI, SEMMSL and SEMMNS are default values which can be
// /*AFLA*/  * modified by sysctl.
// /*AFLA*/  * The values has been chosen to be larger than necessary for any
// /*AFLA*/  * known configuration.
// /*AFLA*/  *
// /*AFLA*/  * SEMOPM should not be increased beyond 1000, otherwise there is the
// /*AFLA*/  * risk that semop()/semtimedop() fails due to kernel memory fragmentation when
// /*AFLA*/  * allocating the sop array.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define SEMMNI  32000           /* <= IPCMNI  max # of semaphore identifiers */
// /*AFLA*/ #define SEMMSL  32000           /* <= INT_MAX max num of semaphores per id */
// /*AFLA*/ #define SEMMNS  (SEMMNI*SEMMSL) /* <= INT_MAX max # of semaphores in system */
// /*AFLA*/ #define SEMOPM  500	        /* <= 1 000 max num of ops per semop call */
// /*AFLA*/ #define SEMVMX  32767           /* <= 32767 semaphore maximum value */
// /*AFLA*/ #define SEMAEM  SEMVMX          /* adjust on exit max value */
// /*AFLA*/ 
// /*AFLA*/ /* unused */
// /*AFLA*/ #define SEMUME  SEMOPM          /* max num of undo entries per process */
// /*AFLA*/ #define SEMMNU  SEMMNS          /* num of undo structures system wide */
// /*AFLA*/ #define SEMMAP  SEMMNS          /* # of entries in semaphore map */
// /*AFLA*/ #define SEMUSZ  20		/* sizeof struct sem_undo */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_SEM_H */
