// /*AFLA*/ #ifndef _LINUX_STAT_H
// /*AFLA*/ #define _LINUX_STAT_H
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #include <asm/stat.h>
// /*AFLA*/ #include <uapi/linux/stat.h>
// /*AFLA*/ 
// /*AFLA*/ #define S_IRWXUGO	(S_IRWXU|S_IRWXG|S_IRWXO)
// /*AFLA*/ #define S_IALLUGO	(S_ISUID|S_ISGID|S_ISVTX|S_IRWXUGO)
// /*AFLA*/ #define S_IRUGO		(S_IRUSR|S_IRGRP|S_IROTH)
// /*AFLA*/ #define S_IWUGO		(S_IWUSR|S_IWGRP|S_IWOTH)
// /*AFLA*/ #define S_IXUGO		(S_IXUSR|S_IXGRP|S_IXOTH)
// /*AFLA*/ 
// /*AFLA*/ #define UTIME_NOW	((1l << 30) - 1l)
// /*AFLA*/ #define UTIME_OMIT	((1l << 30) - 2l)
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/time.h>
// /*AFLA*/ #include <linux/uidgid.h>
// /*AFLA*/ 
// /*AFLA*/ struct kstat {
// /*AFLA*/ 	u64		ino;
// /*AFLA*/ 	dev_t		dev;
// /*AFLA*/ 	umode_t		mode;
// /*AFLA*/ 	unsigned int	nlink;
// /*AFLA*/ 	kuid_t		uid;
// /*AFLA*/ 	kgid_t		gid;
// /*AFLA*/ 	dev_t		rdev;
// /*AFLA*/ 	loff_t		size;
// /*AFLA*/ 	struct timespec  atime;
// /*AFLA*/ 	struct timespec	mtime;
// /*AFLA*/ 	struct timespec	ctime;
// /*AFLA*/ 	unsigned long	blksize;
// /*AFLA*/ 	unsigned long long	blocks;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif
