// /*AFLA*/ #ifndef _LINUX_IPC_H
// /*AFLA*/ #define _LINUX_IPC_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/uidgid.h>
// /*AFLA*/ #include <uapi/linux/ipc.h>
// /*AFLA*/ 
// /*AFLA*/ #define IPCMNI 32768  /* <= MAX_INT limit for ipc arrays (including sysctl changes) */
// /*AFLA*/ 
// /*AFLA*/ /* used by in-kernel data structures */
// /*AFLA*/ struct kern_ipc_perm
// /*AFLA*/ {
// /*AFLA*/ 	spinlock_t	lock;
// /*AFLA*/ 	bool		deleted;
// /*AFLA*/ 	int		id;
// /*AFLA*/ 	key_t		key;
// /*AFLA*/ 	kuid_t		uid;
// /*AFLA*/ 	kgid_t		gid;
// /*AFLA*/ 	kuid_t		cuid;
// /*AFLA*/ 	kgid_t		cgid;
// /*AFLA*/ 	umode_t		mode; 
// /*AFLA*/ 	unsigned long	seq;
// /*AFLA*/ 	void		*security;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_IPC_H */
