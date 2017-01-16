// /*AFLA*/ #ifndef _UAPI_LINUX_IPC_H
// /*AFLA*/ #define _UAPI_LINUX_IPC_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ #define IPC_PRIVATE ((__kernel_key_t) 0)  
// /*AFLA*/ 
// /*AFLA*/ /* Obsolete, used only for backwards compatibility and libc5 compiles */
// /*AFLA*/ struct ipc_perm
// /*AFLA*/ {
// /*AFLA*/ 	__kernel_key_t	key;
// /*AFLA*/ 	__kernel_uid_t	uid;
// /*AFLA*/ 	__kernel_gid_t	gid;
// /*AFLA*/ 	__kernel_uid_t	cuid;
// /*AFLA*/ 	__kernel_gid_t	cgid;
// /*AFLA*/ 	__kernel_mode_t	mode; 
// /*AFLA*/ 	unsigned short	seq;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Include the definition of ipc64_perm */
// /*AFLA*/ #include <asm/ipcbuf.h>
// /*AFLA*/ 
// /*AFLA*/ /* resource get request flags */
// /*AFLA*/ #define IPC_CREAT  00001000   /* create if key is nonexistent */
// /*AFLA*/ #define IPC_EXCL   00002000   /* fail if key exists */
// /*AFLA*/ #define IPC_NOWAIT 00004000   /* return error on wait */
// /*AFLA*/ 
// /*AFLA*/ /* these fields are used by the DIPC package so the kernel as standard
// /*AFLA*/    should avoid using them if possible */
// /*AFLA*/    
// /*AFLA*/ #define IPC_DIPC 00010000  /* make it distributed */
// /*AFLA*/ #define IPC_OWN  00020000  /* this machine is the DIPC owner */
// /*AFLA*/ 
// /*AFLA*/ /* 
// /*AFLA*/  * Control commands used with semctl, msgctl and shmctl 
// /*AFLA*/  * see also specific commands in sem.h, msg.h and shm.h
// /*AFLA*/  */
// /*AFLA*/ #define IPC_RMID 0     /* remove resource */
// /*AFLA*/ #define IPC_SET  1     /* set ipc_perm options */
// /*AFLA*/ #define IPC_STAT 2     /* get ipc_perm options */
// /*AFLA*/ #define IPC_INFO 3     /* see ipcs */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Version flags for semctl, msgctl, and shmctl commands
// /*AFLA*/  * These are passed as bitflags or-ed with the actual command
// /*AFLA*/  */
// /*AFLA*/ #define IPC_OLD 0	/* Old version (no 32-bit UID support on many
// /*AFLA*/ 			   architectures) */
// /*AFLA*/ #define IPC_64  0x0100  /* New version (support 32-bit UIDs, bigger
// /*AFLA*/ 			   message sizes, etc. */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These are used to wrap system calls.
// /*AFLA*/  *
// /*AFLA*/  * See architecture code for ugly details..
// /*AFLA*/  */
// /*AFLA*/ struct ipc_kludge {
// /*AFLA*/ 	struct msgbuf __user *msgp;
// /*AFLA*/ 	long msgtyp;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define SEMOP		 1
// /*AFLA*/ #define SEMGET		 2
// /*AFLA*/ #define SEMCTL		 3
// /*AFLA*/ #define SEMTIMEDOP	 4
// /*AFLA*/ #define MSGSND		11
// /*AFLA*/ #define MSGRCV		12
// /*AFLA*/ #define MSGGET		13
// /*AFLA*/ #define MSGCTL		14
// /*AFLA*/ #define SHMAT		21
// /*AFLA*/ #define SHMDT		22
// /*AFLA*/ #define SHMGET		23
// /*AFLA*/ #define SHMCTL		24
// /*AFLA*/ 
// /*AFLA*/ /* Used by the DIPC package, try and avoid reusing it */
// /*AFLA*/ #define DIPC            25
// /*AFLA*/ 
// /*AFLA*/ #define IPCCALL(version,op)	((version)<<16 | (op))
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_IPC_H */
