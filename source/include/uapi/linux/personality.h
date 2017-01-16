// /*AFLA*/ #ifndef _UAPI_LINUX_PERSONALITY_H
// /*AFLA*/ #define _UAPI_LINUX_PERSONALITY_H
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Flags for bug emulation.
// /*AFLA*/  *
// /*AFLA*/  * These occupy the top three bytes.
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	UNAME26	=               0x0020000,
// /*AFLA*/ 	ADDR_NO_RANDOMIZE = 	0x0040000,	/* disable randomization of VA space */
// /*AFLA*/ 	FDPIC_FUNCPTRS =	0x0080000,	/* userspace function ptrs point to descriptors
// /*AFLA*/ 						 * (signal handling)
// /*AFLA*/ 						 */
// /*AFLA*/ 	MMAP_PAGE_ZERO =	0x0100000,
// /*AFLA*/ 	ADDR_COMPAT_LAYOUT =	0x0200000,
// /*AFLA*/ 	READ_IMPLIES_EXEC =	0x0400000,
// /*AFLA*/ 	ADDR_LIMIT_32BIT =	0x0800000,
// /*AFLA*/ 	SHORT_INODE =		0x1000000,
// /*AFLA*/ 	WHOLE_SECONDS =		0x2000000,
// /*AFLA*/ 	STICKY_TIMEOUTS	=	0x4000000,
// /*AFLA*/ 	ADDR_LIMIT_3GB = 	0x8000000,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Security-relevant compatibility flags that must be
// /*AFLA*/  * cleared upon setuid or setgid exec:
// /*AFLA*/  */
// /*AFLA*/ #define PER_CLEAR_ON_SETID (READ_IMPLIES_EXEC  | \
// /*AFLA*/ 			    ADDR_NO_RANDOMIZE  | \
// /*AFLA*/ 			    ADDR_COMPAT_LAYOUT | \
// /*AFLA*/ 			    MMAP_PAGE_ZERO)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Personality types.
// /*AFLA*/  *
// /*AFLA*/  * These go in the low byte.  Avoid using the top bit, it will
// /*AFLA*/  * conflict with error returns.
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	PER_LINUX =		0x0000,
// /*AFLA*/ 	PER_LINUX_32BIT =	0x0000 | ADDR_LIMIT_32BIT,
// /*AFLA*/ 	PER_LINUX_FDPIC =	0x0000 | FDPIC_FUNCPTRS,
// /*AFLA*/ 	PER_SVR4 =		0x0001 | STICKY_TIMEOUTS | MMAP_PAGE_ZERO,
// /*AFLA*/ 	PER_SVR3 =		0x0002 | STICKY_TIMEOUTS | SHORT_INODE,
// /*AFLA*/ 	PER_SCOSVR3 =		0x0003 | STICKY_TIMEOUTS |
// /*AFLA*/ 					 WHOLE_SECONDS | SHORT_INODE,
// /*AFLA*/ 	PER_OSR5 =		0x0003 | STICKY_TIMEOUTS | WHOLE_SECONDS,
// /*AFLA*/ 	PER_WYSEV386 =		0x0004 | STICKY_TIMEOUTS | SHORT_INODE,
// /*AFLA*/ 	PER_ISCR4 =		0x0005 | STICKY_TIMEOUTS,
// /*AFLA*/ 	PER_BSD =		0x0006,
// /*AFLA*/ 	PER_SUNOS =		0x0006 | STICKY_TIMEOUTS,
// /*AFLA*/ 	PER_XENIX =		0x0007 | STICKY_TIMEOUTS | SHORT_INODE,
// /*AFLA*/ 	PER_LINUX32 =		0x0008,
// /*AFLA*/ 	PER_LINUX32_3GB =	0x0008 | ADDR_LIMIT_3GB,
// /*AFLA*/ 	PER_IRIX32 =		0x0009 | STICKY_TIMEOUTS,/* IRIX5 32-bit */
// /*AFLA*/ 	PER_IRIXN32 =		0x000a | STICKY_TIMEOUTS,/* IRIX6 new 32-bit */
// /*AFLA*/ 	PER_IRIX64 =		0x000b | STICKY_TIMEOUTS,/* IRIX6 64-bit */
// /*AFLA*/ 	PER_RISCOS =		0x000c,
// /*AFLA*/ 	PER_SOLARIS =		0x000d | STICKY_TIMEOUTS,
// /*AFLA*/ 	PER_UW7 =		0x000e | STICKY_TIMEOUTS | MMAP_PAGE_ZERO,
// /*AFLA*/ 	PER_OSF4 =		0x000f,			 /* OSF/1 v4 */
// /*AFLA*/ 	PER_HPUX =		0x0010,
// /*AFLA*/ 	PER_MASK =		0x00ff,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_PERSONALITY_H */
