// /*AFLA*/ #ifndef _UAPI_LINUX_SCHED_H
// /*AFLA*/ #define _UAPI_LINUX_SCHED_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * cloning flags:
// /*AFLA*/  */
// /*AFLA*/ #define CSIGNAL		0x000000ff	/* signal mask to be sent at exit */
// /*AFLA*/ #define CLONE_VM	0x00000100	/* set if VM shared between processes */
// /*AFLA*/ #define CLONE_FS	0x00000200	/* set if fs info shared between processes */
// /*AFLA*/ #define CLONE_FILES	0x00000400	/* set if open files shared between processes */
// /*AFLA*/ #define CLONE_SIGHAND	0x00000800	/* set if signal handlers and blocked signals shared */
// /*AFLA*/ #define CLONE_PTRACE	0x00002000	/* set if we want to let tracing continue on the child too */
// /*AFLA*/ #define CLONE_VFORK	0x00004000	/* set if the parent wants the child to wake it up on mm_release */
// /*AFLA*/ #define CLONE_PARENT	0x00008000	/* set if we want to have the same parent as the cloner */
// /*AFLA*/ #define CLONE_THREAD	0x00010000	/* Same thread group? */
// /*AFLA*/ #define CLONE_NEWNS	0x00020000	/* New mount namespace group */
// /*AFLA*/ #define CLONE_SYSVSEM	0x00040000	/* share system V SEM_UNDO semantics */
// /*AFLA*/ #define CLONE_SETTLS	0x00080000	/* create a new TLS for the child */
// /*AFLA*/ #define CLONE_PARENT_SETTID	0x00100000	/* set the TID in the parent */
// /*AFLA*/ #define CLONE_CHILD_CLEARTID	0x00200000	/* clear the TID in the child */
// /*AFLA*/ #define CLONE_DETACHED		0x00400000	/* Unused, ignored */
// /*AFLA*/ #define CLONE_UNTRACED		0x00800000	/* set if the tracing process can't force CLONE_PTRACE on this clone */
// /*AFLA*/ #define CLONE_CHILD_SETTID	0x01000000	/* set the TID in the child */
// /*AFLA*/ #define CLONE_NEWCGROUP		0x02000000	/* New cgroup namespace */
// /*AFLA*/ #define CLONE_NEWUTS		0x04000000	/* New utsname namespace */
// /*AFLA*/ #define CLONE_NEWIPC		0x08000000	/* New ipc namespace */
// /*AFLA*/ #define CLONE_NEWUSER		0x10000000	/* New user namespace */
// /*AFLA*/ #define CLONE_NEWPID		0x20000000	/* New pid namespace */
// /*AFLA*/ #define CLONE_NEWNET		0x40000000	/* New network namespace */
// /*AFLA*/ #define CLONE_IO		0x80000000	/* Clone io context */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Scheduling policies
// /*AFLA*/  */
// /*AFLA*/ #define SCHED_NORMAL		0
// /*AFLA*/ #define SCHED_FIFO		1
// /*AFLA*/ #define SCHED_RR		2
// /*AFLA*/ #define SCHED_BATCH		3
// /*AFLA*/ /* SCHED_ISO: reserved but not implemented yet */
// /*AFLA*/ #define SCHED_IDLE		5
// /*AFLA*/ #define SCHED_DEADLINE		6
// /*AFLA*/ 
// /*AFLA*/ /* Can be ORed in to make sure the process is reverted back to SCHED_NORMAL on fork */
// /*AFLA*/ #define SCHED_RESET_ON_FORK     0x40000000
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * For the sched_{set,get}attr() calls
// /*AFLA*/  */
// /*AFLA*/ #define SCHED_FLAG_RESET_ON_FORK	0x01
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_SCHED_H */
