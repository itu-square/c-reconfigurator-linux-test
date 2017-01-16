// /*AFLA*/ #ifndef __ASM_GENERIC_POLL_H
// /*AFLA*/ #define __ASM_GENERIC_POLL_H
// /*AFLA*/ 
// /*AFLA*/ /* These are specified by iBCS2 */
// /*AFLA*/ #define POLLIN		0x0001
// /*AFLA*/ #define POLLPRI		0x0002
// /*AFLA*/ #define POLLOUT		0x0004
// /*AFLA*/ #define POLLERR		0x0008
// /*AFLA*/ #define POLLHUP		0x0010
// /*AFLA*/ #define POLLNVAL	0x0020
// /*AFLA*/ 
// /*AFLA*/ /* The rest seem to be more-or-less nonstandard. Check them! */
// /*AFLA*/ #define POLLRDNORM	0x0040
// /*AFLA*/ #define POLLRDBAND	0x0080
// /*AFLA*/ #ifndef POLLWRNORM
// /*AFLA*/ #define POLLWRNORM	0x0100
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef POLLWRBAND
// /*AFLA*/ #define POLLWRBAND	0x0200
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef POLLMSG
// /*AFLA*/ #define POLLMSG		0x0400
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef POLLREMOVE
// /*AFLA*/ #define POLLREMOVE	0x1000
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef POLLRDHUP
// /*AFLA*/ #define POLLRDHUP       0x2000
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define POLLFREE	0x4000	/* currently only for epoll */
// /*AFLA*/ 
// /*AFLA*/ #define POLL_BUSY_LOOP	0x8000
// /*AFLA*/ 
// /*AFLA*/ struct pollfd {
// /*AFLA*/ 	int fd;
// /*AFLA*/ 	short events;
// /*AFLA*/ 	short revents;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif	/* __ASM_GENERIC_POLL_H */
