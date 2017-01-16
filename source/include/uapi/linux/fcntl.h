// /*AFLA*/ #ifndef _UAPI_LINUX_FCNTL_H
// /*AFLA*/ #define _UAPI_LINUX_FCNTL_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/fcntl.h>
// /*AFLA*/ 
// /*AFLA*/ #define F_SETLEASE	(F_LINUX_SPECIFIC_BASE + 0)
// /*AFLA*/ #define F_GETLEASE	(F_LINUX_SPECIFIC_BASE + 1)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Cancel a blocking posix lock; internal use only until we expose an
// /*AFLA*/  * asynchronous lock api to userspace:
// /*AFLA*/  */
// /*AFLA*/ #define F_CANCELLK	(F_LINUX_SPECIFIC_BASE + 5)
// /*AFLA*/ 
// /*AFLA*/ /* Create a file descriptor with FD_CLOEXEC set. */
// /*AFLA*/ #define F_DUPFD_CLOEXEC	(F_LINUX_SPECIFIC_BASE + 6)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Request nofications on a directory.
// /*AFLA*/  * See below for events that may be notified.
// /*AFLA*/  */
// /*AFLA*/ #define F_NOTIFY	(F_LINUX_SPECIFIC_BASE+2)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Set and get of pipe page size array
// /*AFLA*/  */
// /*AFLA*/ #define F_SETPIPE_SZ	(F_LINUX_SPECIFIC_BASE + 7)
// /*AFLA*/ #define F_GETPIPE_SZ	(F_LINUX_SPECIFIC_BASE + 8)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Set/Get seals
// /*AFLA*/  */
// /*AFLA*/ #define F_ADD_SEALS	(F_LINUX_SPECIFIC_BASE + 9)
// /*AFLA*/ #define F_GET_SEALS	(F_LINUX_SPECIFIC_BASE + 10)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Types of seals
// /*AFLA*/  */
// /*AFLA*/ #define F_SEAL_SEAL	0x0001	/* prevent further seals from being set */
// /*AFLA*/ #define F_SEAL_SHRINK	0x0002	/* prevent file from shrinking */
// /*AFLA*/ #define F_SEAL_GROW	0x0004	/* prevent file from growing */
// /*AFLA*/ #define F_SEAL_WRITE	0x0008	/* prevent writes */
// /*AFLA*/ /* (1U << 31) is reserved for signed error codes */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Types of directory notifications that may be requested.
// /*AFLA*/  */
// /*AFLA*/ #define DN_ACCESS	0x00000001	/* File accessed */
// /*AFLA*/ #define DN_MODIFY	0x00000002	/* File modified */
// /*AFLA*/ #define DN_CREATE	0x00000004	/* File created */
// /*AFLA*/ #define DN_DELETE	0x00000008	/* File removed */
// /*AFLA*/ #define DN_RENAME	0x00000010	/* File renamed */
// /*AFLA*/ #define DN_ATTRIB	0x00000020	/* File changed attibutes */
// /*AFLA*/ #define DN_MULTISHOT	0x80000000	/* Don't remove notifier */
// /*AFLA*/ 
// /*AFLA*/ #define AT_FDCWD		-100    /* Special value used to indicate
// /*AFLA*/                                            openat should use the current
// /*AFLA*/                                            working directory. */
// /*AFLA*/ #define AT_SYMLINK_NOFOLLOW	0x100   /* Do not follow symbolic links.  */
// /*AFLA*/ #define AT_REMOVEDIR		0x200   /* Remove directory instead of
// /*AFLA*/                                            unlinking file.  */
// /*AFLA*/ #define AT_SYMLINK_FOLLOW	0x400   /* Follow symbolic links.  */
// /*AFLA*/ #define AT_NO_AUTOMOUNT		0x800	/* Suppress terminal automount traversal */
// /*AFLA*/ #define AT_EMPTY_PATH		0x1000	/* Allow empty relative pathname */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_FCNTL_H */
