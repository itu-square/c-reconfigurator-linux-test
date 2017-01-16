// /*AFLA*/ #ifndef _UAPI_LINUX_STAT_H
// /*AFLA*/ #define _UAPI_LINUX_STAT_H
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #if defined(__KERNEL__) || !defined(__GLIBC__) || (__GLIBC__ < 2)
// /*AFLA*/ 
// /*AFLA*/ #define S_IFMT  00170000
// /*AFLA*/ #define S_IFSOCK 0140000
// /*AFLA*/ #define S_IFLNK	 0120000
// /*AFLA*/ #define S_IFREG  0100000
// /*AFLA*/ #define S_IFBLK  0060000
// /*AFLA*/ #define S_IFDIR  0040000
// /*AFLA*/ #define S_IFCHR  0020000
// /*AFLA*/ #define S_IFIFO  0010000
// /*AFLA*/ #define S_ISUID  0004000
// /*AFLA*/ #define S_ISGID  0002000
// /*AFLA*/ #define S_ISVTX  0001000
// /*AFLA*/ 
// /*AFLA*/ #define S_ISLNK(m)	(((m) & S_IFMT) == S_IFLNK)
// /*AFLA*/ #define S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
// /*AFLA*/ #define S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
// /*AFLA*/ #define S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
// /*AFLA*/ #define S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
// /*AFLA*/ #define S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)
// /*AFLA*/ #define S_ISSOCK(m)	(((m) & S_IFMT) == S_IFSOCK)
// /*AFLA*/ 
// /*AFLA*/ #define S_IRWXU 00700
// /*AFLA*/ #define S_IRUSR 00400
// /*AFLA*/ #define S_IWUSR 00200
// /*AFLA*/ #define S_IXUSR 00100
// /*AFLA*/ 
// /*AFLA*/ #define S_IRWXG 00070
// /*AFLA*/ #define S_IRGRP 00040
// /*AFLA*/ #define S_IWGRP 00020
// /*AFLA*/ #define S_IXGRP 00010
// /*AFLA*/ 
// /*AFLA*/ #define S_IRWXO 00007
// /*AFLA*/ #define S_IROTH 00004
// /*AFLA*/ #define S_IWOTH 00002
// /*AFLA*/ #define S_IXOTH 00001
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_STAT_H */
