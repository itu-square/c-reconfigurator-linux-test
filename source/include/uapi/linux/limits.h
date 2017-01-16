// /*AFLA*/ #ifndef _LINUX_LIMITS_H
// /*AFLA*/ #define _LINUX_LIMITS_H
// /*AFLA*/ 
// /*AFLA*/ #define NR_OPEN	        1024
// /*AFLA*/ 
// /*AFLA*/ #define NGROUPS_MAX    65536	/* supplemental group IDs are available */
// /*AFLA*/ #define ARG_MAX       131072	/* # bytes of args + environ for exec() */
// /*AFLA*/ #define LINK_MAX         127	/* # links a file may have */
// /*AFLA*/ #define MAX_CANON        255	/* size of the canonical input queue */
// /*AFLA*/ #define MAX_INPUT        255	/* size of the type-ahead buffer */
// /*AFLA*/ #define NAME_MAX         255	/* # chars in a file name */
// /*AFLA*/ #define PATH_MAX        4096	/* # chars in a path name including nul */
// /*AFLA*/ #define PIPE_BUF        4096	/* # bytes in atomic write to a pipe */
// /*AFLA*/ #define XATTR_NAME_MAX   255	/* # chars in an extended attribute name */
// /*AFLA*/ #define XATTR_SIZE_MAX 65536	/* size of an extended attribute value (64k) */
// /*AFLA*/ #define XATTR_LIST_MAX 65536	/* size of extended attribute namelist (64k) */
// /*AFLA*/ 
// /*AFLA*/ #define RTSIG_MAX	  32
// /*AFLA*/ 
// /*AFLA*/ #endif
