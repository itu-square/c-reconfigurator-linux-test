// /*AFLA*/ #ifndef _UAPI_LINUX_WAIT_H
// /*AFLA*/ #define _UAPI_LINUX_WAIT_H
// /*AFLA*/ 
// /*AFLA*/ #define WNOHANG		0x00000001
// /*AFLA*/ #define WUNTRACED	0x00000002
// /*AFLA*/ #define WSTOPPED	WUNTRACED
// /*AFLA*/ #define WEXITED		0x00000004
// /*AFLA*/ #define WCONTINUED	0x00000008
// /*AFLA*/ #define WNOWAIT		0x01000000	/* Don't reap, just poll status.  */
// /*AFLA*/ 
// /*AFLA*/ #define __WNOTHREAD	0x20000000	/* Don't wait on children of other threads in this group */
// /*AFLA*/ #define __WALL		0x40000000	/* Wait on all children, regardless of type */
// /*AFLA*/ #define __WCLONE	0x80000000	/* Wait only on non-SIGCHLD children */
// /*AFLA*/ 
// /*AFLA*/ /* First argument to waitid: */
// /*AFLA*/ #define P_ALL		0
// /*AFLA*/ #define P_PID		1
// /*AFLA*/ #define P_PGID		2
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_WAIT_H */
