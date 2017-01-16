// /*AFLA*/ #ifndef _ASM_GENERIC_FCNTL_H
// /*AFLA*/ #define _ASM_GENERIC_FCNTL_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * FMODE_EXEC is 0x20
// /*AFLA*/  * FMODE_NONOTIFY is 0x4000000
// /*AFLA*/  * These cannot be used by userspace O_* until internal and external open
// /*AFLA*/  * flags are split.
// /*AFLA*/  * -Eric Paris
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * When introducing new O_* bits, please check its uniqueness in fcntl_init().
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define O_ACCMODE	00000003
// /*AFLA*/ #define O_RDONLY	00000000
// /*AFLA*/ #define O_WRONLY	00000001
// /*AFLA*/ #define O_RDWR		00000002
// /*AFLA*/ #ifndef O_CREAT
// /*AFLA*/ #define O_CREAT		00000100	/* not fcntl */
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef O_EXCL
// /*AFLA*/ #define O_EXCL		00000200	/* not fcntl */
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef O_NOCTTY
// /*AFLA*/ #define O_NOCTTY	00000400	/* not fcntl */
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef O_TRUNC
// /*AFLA*/ #define O_TRUNC		00001000	/* not fcntl */
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef O_APPEND
// /*AFLA*/ #define O_APPEND	00002000
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef O_NONBLOCK
// /*AFLA*/ #define O_NONBLOCK	00004000
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef O_DSYNC
// /*AFLA*/ #define O_DSYNC		00010000	/* used to be O_SYNC, see below */
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef FASYNC
// /*AFLA*/ #define FASYNC		00020000	/* fcntl, for BSD compatibility */
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef O_DIRECT
// /*AFLA*/ #define O_DIRECT	00040000	/* direct disk access hint */
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef O_LARGEFILE
// /*AFLA*/ #define O_LARGEFILE	00100000
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef O_DIRECTORY
// /*AFLA*/ #define O_DIRECTORY	00200000	/* must be a directory */
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef O_NOFOLLOW
// /*AFLA*/ #define O_NOFOLLOW	00400000	/* don't follow links */
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef O_NOATIME
// /*AFLA*/ #define O_NOATIME	01000000
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef O_CLOEXEC
// /*AFLA*/ #define O_CLOEXEC	02000000	/* set close_on_exec */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Before Linux 2.6.33 only O_DSYNC semantics were implemented, but using
// /*AFLA*/  * the O_SYNC flag.  We continue to use the existing numerical value
// /*AFLA*/  * for O_DSYNC semantics now, but using the correct symbolic name for it.
// /*AFLA*/  * This new value is used to request true Posix O_SYNC semantics.  It is
// /*AFLA*/  * defined in this strange way to make sure applications compiled against
// /*AFLA*/  * new headers get at least O_DSYNC semantics on older kernels.
// /*AFLA*/  *
// /*AFLA*/  * This has the nice side-effect that we can simply test for O_DSYNC
// /*AFLA*/  * wherever we do not care if O_DSYNC or O_SYNC is used.
// /*AFLA*/  *
// /*AFLA*/  * Note: __O_SYNC must never be used directly.
// /*AFLA*/  */
// /*AFLA*/ #ifndef O_SYNC
// /*AFLA*/ #define __O_SYNC	04000000
// /*AFLA*/ #define O_SYNC		(__O_SYNC|O_DSYNC)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef O_PATH
// /*AFLA*/ #define O_PATH		010000000
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __O_TMPFILE
// /*AFLA*/ #define __O_TMPFILE	020000000
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* a horrid kludge trying to make sure that this will fail on old kernels */
// /*AFLA*/ #define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
// /*AFLA*/ #define O_TMPFILE_MASK (__O_TMPFILE | O_DIRECTORY | O_CREAT)      
// /*AFLA*/ 
// /*AFLA*/ #ifndef O_NDELAY
// /*AFLA*/ #define O_NDELAY	O_NONBLOCK
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define F_DUPFD		0	/* dup */
// /*AFLA*/ #define F_GETFD		1	/* get close_on_exec */
// /*AFLA*/ #define F_SETFD		2	/* set/clear close_on_exec */
// /*AFLA*/ #define F_GETFL		3	/* get file->f_flags */
// /*AFLA*/ #define F_SETFL		4	/* set file->f_flags */
// /*AFLA*/ #ifndef F_GETLK
// /*AFLA*/ #define F_GETLK		5
// /*AFLA*/ #define F_SETLK		6
// /*AFLA*/ #define F_SETLKW	7
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef F_SETOWN
// /*AFLA*/ #define F_SETOWN	8	/* for sockets. */
// /*AFLA*/ #define F_GETOWN	9	/* for sockets. */
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef F_SETSIG
// /*AFLA*/ #define F_SETSIG	10	/* for sockets. */
// /*AFLA*/ #define F_GETSIG	11	/* for sockets. */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_64BIT
// /*AFLA*/ #ifndef F_GETLK64
// /*AFLA*/ #define F_GETLK64	12	/*  using 'struct flock64' */
// /*AFLA*/ #define F_SETLK64	13
// /*AFLA*/ #define F_SETLKW64	14
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef F_SETOWN_EX
// /*AFLA*/ #define F_SETOWN_EX	15
// /*AFLA*/ #define F_GETOWN_EX	16
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef F_GETOWNER_UIDS
// /*AFLA*/ #define F_GETOWNER_UIDS	17
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Open File Description Locks
// /*AFLA*/  *
// /*AFLA*/  * Usually record locks held by a process are released on *any* close and are
// /*AFLA*/  * not inherited across a fork().
// /*AFLA*/  *
// /*AFLA*/  * These cmd values will set locks that conflict with process-associated
// /*AFLA*/  * record  locks, but are "owned" by the open file description, not the
// /*AFLA*/  * process. This means that they are inherited across fork() like BSD (flock)
// /*AFLA*/  * locks, and they are only released automatically when the last reference to
// /*AFLA*/  * the the open file against which they were acquired is put.
// /*AFLA*/  */
// /*AFLA*/ #define F_OFD_GETLK	36
// /*AFLA*/ #define F_OFD_SETLK	37
// /*AFLA*/ #define F_OFD_SETLKW	38
// /*AFLA*/ 
// /*AFLA*/ #define F_OWNER_TID	0
// /*AFLA*/ #define F_OWNER_PID	1
// /*AFLA*/ #define F_OWNER_PGRP	2
// /*AFLA*/ 
// /*AFLA*/ struct f_owner_ex {
// /*AFLA*/ 	int	type;
// /*AFLA*/ 	__kernel_pid_t	pid;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* for F_[GET|SET]FL */
// /*AFLA*/ #define FD_CLOEXEC	1	/* actually anything with low bit set goes */
// /*AFLA*/ 
// /*AFLA*/ /* for posix fcntl() and lockf() */
// /*AFLA*/ #ifndef F_RDLCK
// /*AFLA*/ #define F_RDLCK		0
// /*AFLA*/ #define F_WRLCK		1
// /*AFLA*/ #define F_UNLCK		2
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* for old implementation of bsd flock () */
// /*AFLA*/ #ifndef F_EXLCK
// /*AFLA*/ #define F_EXLCK		4	/* or 3 */
// /*AFLA*/ #define F_SHLCK		8	/* or 4 */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* operations for bsd flock(), also used by the kernel implementation */
// /*AFLA*/ #define LOCK_SH		1	/* shared lock */
// /*AFLA*/ #define LOCK_EX		2	/* exclusive lock */
// /*AFLA*/ #define LOCK_NB		4	/* or'd with one of the above to prevent
// /*AFLA*/ 				   blocking */
// /*AFLA*/ #define LOCK_UN		8	/* remove lock */
// /*AFLA*/ 
// /*AFLA*/ #define LOCK_MAND	32	/* This is a mandatory flock ... */
// /*AFLA*/ #define LOCK_READ	64	/* which allows concurrent read operations */
// /*AFLA*/ #define LOCK_WRITE	128	/* which allows concurrent write operations */
// /*AFLA*/ #define LOCK_RW		192	/* which allows concurrent read & write ops */
// /*AFLA*/ 
// /*AFLA*/ #define F_LINUX_SPECIFIC_BASE	1024
// /*AFLA*/ 
// /*AFLA*/ #ifndef HAVE_ARCH_STRUCT_FLOCK
// /*AFLA*/ #ifndef __ARCH_FLOCK_PAD
// /*AFLA*/ #define __ARCH_FLOCK_PAD
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct flock {
// /*AFLA*/ 	short	l_type;
// /*AFLA*/ 	short	l_whence;
// /*AFLA*/ 	__kernel_off_t	l_start;
// /*AFLA*/ 	__kernel_off_t	l_len;
// /*AFLA*/ 	__kernel_pid_t	l_pid;
// /*AFLA*/ 	__ARCH_FLOCK_PAD
// /*AFLA*/ };
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef HAVE_ARCH_STRUCT_FLOCK64
// /*AFLA*/ #ifndef __ARCH_FLOCK64_PAD
// /*AFLA*/ #define __ARCH_FLOCK64_PAD
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct flock64 {
// /*AFLA*/ 	short  l_type;
// /*AFLA*/ 	short  l_whence;
// /*AFLA*/ 	__kernel_loff_t l_start;
// /*AFLA*/ 	__kernel_loff_t l_len;
// /*AFLA*/ 	__kernel_pid_t  l_pid;
// /*AFLA*/ 	__ARCH_FLOCK64_PAD
// /*AFLA*/ };
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_GENERIC_FCNTL_H */
