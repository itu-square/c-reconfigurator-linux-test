// /*AFLA*/ #ifndef _ASM_GENERIC_ERRNO_H
// /*AFLA*/ #define _ASM_GENERIC_ERRNO_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/errno-base.h>
// /*AFLA*/ 
// /*AFLA*/ #define	EDEADLK		35	/* Resource deadlock would occur */
// /*AFLA*/ #define	ENAMETOOLONG	36	/* File name too long */
// /*AFLA*/ #define	ENOLCK		37	/* No record locks available */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This error code is special: arch syscall entry code will return
// /*AFLA*/  * -ENOSYS if users try to call a syscall that doesn't exist.  To keep
// /*AFLA*/  * failures of syscalls that really do exist distinguishable from
// /*AFLA*/  * failures due to attempts to use a nonexistent syscall, syscall
// /*AFLA*/  * implementations should refrain from returning -ENOSYS.
// /*AFLA*/  */
// /*AFLA*/ #define	ENOSYS		38	/* Invalid system call number */
// /*AFLA*/ 
// /*AFLA*/ #define	ENOTEMPTY	39	/* Directory not empty */
// /*AFLA*/ #define	ELOOP		40	/* Too many symbolic links encountered */
// /*AFLA*/ #define	EWOULDBLOCK	EAGAIN	/* Operation would block */
// /*AFLA*/ #define	ENOMSG		42	/* No message of desired type */
// /*AFLA*/ #define	EIDRM		43	/* Identifier removed */
// /*AFLA*/ #define	ECHRNG		44	/* Channel number out of range */
// /*AFLA*/ #define	EL2NSYNC	45	/* Level 2 not synchronized */
// /*AFLA*/ #define	EL3HLT		46	/* Level 3 halted */
// /*AFLA*/ #define	EL3RST		47	/* Level 3 reset */
// /*AFLA*/ #define	ELNRNG		48	/* Link number out of range */
// /*AFLA*/ #define	EUNATCH		49	/* Protocol driver not attached */
// /*AFLA*/ #define	ENOCSI		50	/* No CSI structure available */
// /*AFLA*/ #define	EL2HLT		51	/* Level 2 halted */
// /*AFLA*/ #define	EBADE		52	/* Invalid exchange */
// /*AFLA*/ #define	EBADR		53	/* Invalid request descriptor */
// /*AFLA*/ #define	EXFULL		54	/* Exchange full */
// /*AFLA*/ #define	ENOANO		55	/* No anode */
// /*AFLA*/ #define	EBADRQC		56	/* Invalid request code */
// /*AFLA*/ #define	EBADSLT		57	/* Invalid slot */
// /*AFLA*/ 
// /*AFLA*/ #define	EDEADLOCK	EDEADLK
// /*AFLA*/ 
// /*AFLA*/ #define	EBFONT		59	/* Bad font file format */
// /*AFLA*/ #define	ENOSTR		60	/* Device not a stream */
// /*AFLA*/ #define	ENODATA		61	/* No data available */
// /*AFLA*/ #define	ETIME		62	/* Timer expired */
// /*AFLA*/ #define	ENOSR		63	/* Out of streams resources */
// /*AFLA*/ #define	ENONET		64	/* Machine is not on the network */
// /*AFLA*/ #define	ENOPKG		65	/* Package not installed */
// /*AFLA*/ #define	EREMOTE		66	/* Object is remote */
// /*AFLA*/ #define	ENOLINK		67	/* Link has been severed */
// /*AFLA*/ #define	EADV		68	/* Advertise error */
// /*AFLA*/ #define	ESRMNT		69	/* Srmount error */
// /*AFLA*/ #define	ECOMM		70	/* Communication error on send */
// /*AFLA*/ #define	EPROTO		71	/* Protocol error */
// /*AFLA*/ #define	EMULTIHOP	72	/* Multihop attempted */
// /*AFLA*/ #define	EDOTDOT		73	/* RFS specific error */
// /*AFLA*/ #define	EBADMSG		74	/* Not a data message */
// /*AFLA*/ #define	EOVERFLOW	75	/* Value too large for defined data type */
// /*AFLA*/ #define	ENOTUNIQ	76	/* Name not unique on network */
// /*AFLA*/ #define	EBADFD		77	/* File descriptor in bad state */
// /*AFLA*/ #define	EREMCHG		78	/* Remote address changed */
// /*AFLA*/ #define	ELIBACC		79	/* Can not access a needed shared library */
// /*AFLA*/ #define	ELIBBAD		80	/* Accessing a corrupted shared library */
// /*AFLA*/ #define	ELIBSCN		81	/* .lib section in a.out corrupted */
// /*AFLA*/ #define	ELIBMAX		82	/* Attempting to link in too many shared libraries */
// /*AFLA*/ #define	ELIBEXEC	83	/* Cannot exec a shared library directly */
// /*AFLA*/ #define	EILSEQ		84	/* Illegal byte sequence */
// /*AFLA*/ #define	ERESTART	85	/* Interrupted system call should be restarted */
// /*AFLA*/ #define	ESTRPIPE	86	/* Streams pipe error */
// /*AFLA*/ #define	EUSERS		87	/* Too many users */
// /*AFLA*/ #define	ENOTSOCK	88	/* Socket operation on non-socket */
// /*AFLA*/ #define	EDESTADDRREQ	89	/* Destination address required */
// /*AFLA*/ #define	EMSGSIZE	90	/* Message too long */
// /*AFLA*/ #define	EPROTOTYPE	91	/* Protocol wrong type for socket */
// /*AFLA*/ #define	ENOPROTOOPT	92	/* Protocol not available */
// /*AFLA*/ #define	EPROTONOSUPPORT	93	/* Protocol not supported */
// /*AFLA*/ #define	ESOCKTNOSUPPORT	94	/* Socket type not supported */
// /*AFLA*/ #define	EOPNOTSUPP	95	/* Operation not supported on transport endpoint */
// /*AFLA*/ #define	EPFNOSUPPORT	96	/* Protocol family not supported */
// /*AFLA*/ #define	EAFNOSUPPORT	97	/* Address family not supported by protocol */
// /*AFLA*/ #define	EADDRINUSE	98	/* Address already in use */
// /*AFLA*/ #define	EADDRNOTAVAIL	99	/* Cannot assign requested address */
// /*AFLA*/ #define	ENETDOWN	100	/* Network is down */
// /*AFLA*/ #define	ENETUNREACH	101	/* Network is unreachable */
// /*AFLA*/ #define	ENETRESET	102	/* Network dropped connection because of reset */
// /*AFLA*/ #define	ECONNABORTED	103	/* Software caused connection abort */
// /*AFLA*/ #define	ECONNRESET	104	/* Connection reset by peer */
// /*AFLA*/ #define	ENOBUFS		105	/* No buffer space available */
// /*AFLA*/ #define	EISCONN		106	/* Transport endpoint is already connected */
// /*AFLA*/ #define	ENOTCONN	107	/* Transport endpoint is not connected */
// /*AFLA*/ #define	ESHUTDOWN	108	/* Cannot send after transport endpoint shutdown */
// /*AFLA*/ #define	ETOOMANYREFS	109	/* Too many references: cannot splice */
// /*AFLA*/ #define	ETIMEDOUT	110	/* Connection timed out */
// /*AFLA*/ #define	ECONNREFUSED	111	/* Connection refused */
// /*AFLA*/ #define	EHOSTDOWN	112	/* Host is down */
// /*AFLA*/ #define	EHOSTUNREACH	113	/* No route to host */
// /*AFLA*/ #define	EALREADY	114	/* Operation already in progress */
// /*AFLA*/ #define	EINPROGRESS	115	/* Operation now in progress */
// /*AFLA*/ #define	ESTALE		116	/* Stale file handle */
// /*AFLA*/ #define	EUCLEAN		117	/* Structure needs cleaning */
// /*AFLA*/ #define	ENOTNAM		118	/* Not a XENIX named type file */
// /*AFLA*/ #define	ENAVAIL		119	/* No XENIX semaphores available */
// /*AFLA*/ #define	EISNAM		120	/* Is a named type file */
// /*AFLA*/ #define	EREMOTEIO	121	/* Remote I/O error */
// /*AFLA*/ #define	EDQUOT		122	/* Quota exceeded */
// /*AFLA*/ 
// /*AFLA*/ #define	ENOMEDIUM	123	/* No medium found */
// /*AFLA*/ #define	EMEDIUMTYPE	124	/* Wrong medium type */
// /*AFLA*/ #define	ECANCELED	125	/* Operation Canceled */
// /*AFLA*/ #define	ENOKEY		126	/* Required key not available */
// /*AFLA*/ #define	EKEYEXPIRED	127	/* Key has expired */
// /*AFLA*/ #define	EKEYREVOKED	128	/* Key has been revoked */
// /*AFLA*/ #define	EKEYREJECTED	129	/* Key was rejected by service */
// /*AFLA*/ 
// /*AFLA*/ /* for robust mutexes */
// /*AFLA*/ #define	EOWNERDEAD	130	/* Owner died */
// /*AFLA*/ #define	ENOTRECOVERABLE	131	/* State not recoverable */
// /*AFLA*/ 
// /*AFLA*/ #define ERFKILL		132	/* Operation not possible due to RF-kill */
// /*AFLA*/ 
// /*AFLA*/ #define EHWPOISON	133	/* Memory page has hardware error */
// /*AFLA*/ 
// /*AFLA*/ #endif
