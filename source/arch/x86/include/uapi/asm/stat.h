// /*AFLA*/ #ifndef _ASM_X86_STAT_H
// /*AFLA*/ #define _ASM_X86_STAT_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/posix_types.h>
// /*AFLA*/ 
// /*AFLA*/ #define STAT_HAVE_NSEC 1
// /*AFLA*/ 
// /*AFLA*/ #ifdef __i386__
// /*AFLA*/ struct stat {
// /*AFLA*/ 	unsigned long  st_dev;
// /*AFLA*/ 	unsigned long  st_ino;
// /*AFLA*/ 	unsigned short st_mode;
// /*AFLA*/ 	unsigned short st_nlink;
// /*AFLA*/ 	unsigned short st_uid;
// /*AFLA*/ 	unsigned short st_gid;
// /*AFLA*/ 	unsigned long  st_rdev;
// /*AFLA*/ 	unsigned long  st_size;
// /*AFLA*/ 	unsigned long  st_blksize;
// /*AFLA*/ 	unsigned long  st_blocks;
// /*AFLA*/ 	unsigned long  st_atime;
// /*AFLA*/ 	unsigned long  st_atime_nsec;
// /*AFLA*/ 	unsigned long  st_mtime;
// /*AFLA*/ 	unsigned long  st_mtime_nsec;
// /*AFLA*/ 	unsigned long  st_ctime;
// /*AFLA*/ 	unsigned long  st_ctime_nsec;
// /*AFLA*/ 	unsigned long  __unused4;
// /*AFLA*/ 	unsigned long  __unused5;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* We don't need to memset the whole thing just to initialize the padding */
// /*AFLA*/ #define INIT_STRUCT_STAT_PADDING(st) do {	\
// /*AFLA*/ 	st.__unused4 = 0;			\
// /*AFLA*/ 	st.__unused5 = 0;			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define STAT64_HAS_BROKEN_ST_INO	1
// /*AFLA*/ 
// /*AFLA*/ /* This matches struct stat64 in glibc2.1, hence the absolutely
// /*AFLA*/  * insane amounts of padding around dev_t's.
// /*AFLA*/  */
// /*AFLA*/ struct stat64 {
// /*AFLA*/ 	unsigned long long	st_dev;
// /*AFLA*/ 	unsigned char	__pad0[4];
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long	__st_ino;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned int	st_mode;
// /*AFLA*/ 	unsigned int	st_nlink;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long	st_uid;
// /*AFLA*/ 	unsigned long	st_gid;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long long	st_rdev;
// /*AFLA*/ 	unsigned char	__pad3[4];
// /*AFLA*/ 
// /*AFLA*/ 	long long	st_size;
// /*AFLA*/ 	unsigned long	st_blksize;
// /*AFLA*/ 
// /*AFLA*/ 	/* Number 512-byte blocks allocated. */
// /*AFLA*/ 	unsigned long long	st_blocks;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long	st_atime;
// /*AFLA*/ 	unsigned long	st_atime_nsec;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long	st_mtime;
// /*AFLA*/ 	unsigned int	st_mtime_nsec;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long	st_ctime;
// /*AFLA*/ 	unsigned long	st_ctime_nsec;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long long	st_ino;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* We don't need to memset the whole thing just to initialize the padding */
// /*AFLA*/ #define INIT_STRUCT_STAT64_PADDING(st) do {		\
// /*AFLA*/ 	memset(&st.__pad0, 0, sizeof(st.__pad0));	\
// /*AFLA*/ 	memset(&st.__pad3, 0, sizeof(st.__pad3));	\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #else /* __i386__ */
// /*AFLA*/ 
// /*AFLA*/ struct stat {
// /*AFLA*/ 	__kernel_ulong_t	st_dev;
// /*AFLA*/ 	__kernel_ulong_t	st_ino;
// /*AFLA*/ 	__kernel_ulong_t	st_nlink;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned int		st_mode;
// /*AFLA*/ 	unsigned int		st_uid;
// /*AFLA*/ 	unsigned int		st_gid;
// /*AFLA*/ 	unsigned int		__pad0;
// /*AFLA*/ 	__kernel_ulong_t	st_rdev;
// /*AFLA*/ 	__kernel_long_t		st_size;
// /*AFLA*/ 	__kernel_long_t		st_blksize;
// /*AFLA*/ 	__kernel_long_t		st_blocks;	/* Number 512-byte blocks allocated. */
// /*AFLA*/ 
// /*AFLA*/ 	__kernel_ulong_t	st_atime;
// /*AFLA*/ 	__kernel_ulong_t	st_atime_nsec;
// /*AFLA*/ 	__kernel_ulong_t	st_mtime;
// /*AFLA*/ 	__kernel_ulong_t	st_mtime_nsec;
// /*AFLA*/ 	__kernel_ulong_t	st_ctime;
// /*AFLA*/ 	__kernel_ulong_t	st_ctime_nsec;
// /*AFLA*/ 	__kernel_long_t		__unused[3];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* We don't need to memset the whole thing just to initialize the padding */
// /*AFLA*/ #define INIT_STRUCT_STAT_PADDING(st) do {	\
// /*AFLA*/ 	st.__pad0 = 0;				\
// /*AFLA*/ 	st.__unused[0] = 0;			\
// /*AFLA*/ 	st.__unused[1] = 0;			\
// /*AFLA*/ 	st.__unused[2] = 0;			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* for 32bit emulation and 32 bit kernels */
// /*AFLA*/ struct __old_kernel_stat {
// /*AFLA*/ 	unsigned short st_dev;
// /*AFLA*/ 	unsigned short st_ino;
// /*AFLA*/ 	unsigned short st_mode;
// /*AFLA*/ 	unsigned short st_nlink;
// /*AFLA*/ 	unsigned short st_uid;
// /*AFLA*/ 	unsigned short st_gid;
// /*AFLA*/ 	unsigned short st_rdev;
// /*AFLA*/ #ifdef __i386__
// /*AFLA*/ 	unsigned long  st_size;
// /*AFLA*/ 	unsigned long  st_atime;
// /*AFLA*/ 	unsigned long  st_mtime;
// /*AFLA*/ 	unsigned long  st_ctime;
// /*AFLA*/ #else
// /*AFLA*/ 	unsigned int  st_size;
// /*AFLA*/ 	unsigned int  st_atime;
// /*AFLA*/ 	unsigned int  st_mtime;
// /*AFLA*/ 	unsigned int  st_ctime;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_STAT_H */
