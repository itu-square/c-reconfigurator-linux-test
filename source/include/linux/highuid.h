// /*AFLA*/ #ifndef _LINUX_HIGHUID_H
// /*AFLA*/ #define _LINUX_HIGHUID_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * general notes:
// /*AFLA*/  *
// /*AFLA*/  * CONFIG_UID16 is defined if the given architecture needs to
// /*AFLA*/  * support backwards compatibility for old system calls.
// /*AFLA*/  *
// /*AFLA*/  * kernel code should use uid_t and gid_t at all times when dealing with
// /*AFLA*/  * kernel-private data.
// /*AFLA*/  *
// /*AFLA*/  * old_uid_t and old_gid_t should only be different if CONFIG_UID16 is
// /*AFLA*/  * defined, else the platform should provide dummy typedefs for them
// /*AFLA*/  * such that they are equivalent to __kernel_{u,g}id_t.
// /*AFLA*/  *
// /*AFLA*/  * uid16_t and gid16_t are used on all architectures. (when dealing
// /*AFLA*/  * with structures hard coded to 16 bits, such as in filesystems)
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is the "overflow" UID and GID. They are used to signify uid/gid
// /*AFLA*/  * overflow to old programs when they request uid/gid information but are
// /*AFLA*/  * using the old 16 bit interfaces.
// /*AFLA*/  * When you run a libc5 program, it will think that all highuid files or
// /*AFLA*/  * processes are owned by this uid/gid.
// /*AFLA*/  * The idea is that it's better to do so than possibly return 0 in lieu of
// /*AFLA*/  * 65536, etc.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ extern int overflowuid;
// /*AFLA*/ extern int overflowgid;
// /*AFLA*/ 
// /*AFLA*/ extern void __bad_uid(void);
// /*AFLA*/ extern void __bad_gid(void);
// /*AFLA*/ 
// /*AFLA*/ #define DEFAULT_OVERFLOWUID	65534
// /*AFLA*/ #define DEFAULT_OVERFLOWGID	65534
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_UID16
// /*AFLA*/ 
// /*AFLA*/ /* prevent uid mod 65536 effect by returning a default value for high UIDs */
// /*AFLA*/ #define high2lowuid(uid) ((uid) & ~0xFFFF ? (old_uid_t)overflowuid : (old_uid_t)(uid))
// /*AFLA*/ #define high2lowgid(gid) ((gid) & ~0xFFFF ? (old_gid_t)overflowgid : (old_gid_t)(gid))
// /*AFLA*/ /*
// /*AFLA*/  * -1 is different in 16 bits than it is in 32 bits
// /*AFLA*/  * these macros are used by chown(), setreuid(), ...,
// /*AFLA*/  */
// /*AFLA*/ #define low2highuid(uid) ((uid) == (old_uid_t)-1 ? (uid_t)-1 : (uid_t)(uid))
// /*AFLA*/ #define low2highgid(gid) ((gid) == (old_gid_t)-1 ? (gid_t)-1 : (gid_t)(gid))
// /*AFLA*/ 
// /*AFLA*/ #define __convert_uid(size, uid) \
// /*AFLA*/ 	(size >= sizeof(uid) ? (uid) : high2lowuid(uid))
// /*AFLA*/ #define __convert_gid(size, gid) \
// /*AFLA*/ 	(size >= sizeof(gid) ? (gid) : high2lowgid(gid))
// /*AFLA*/ 	
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ #define __convert_uid(size, uid) (uid)
// /*AFLA*/ #define __convert_gid(size, gid) (gid)
// /*AFLA*/ 
// /*AFLA*/ #endif /* !CONFIG_UID16 */
// /*AFLA*/ 
// /*AFLA*/ /* uid/gid input should be always 32bit uid_t */
// /*AFLA*/ #define SET_UID(var, uid) do { (var) = __convert_uid(sizeof(var), (uid)); } while (0)
// /*AFLA*/ #define SET_GID(var, gid) do { (var) = __convert_gid(sizeof(var), (gid)); } while (0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Everything below this line is needed on all architectures, to deal with
// /*AFLA*/  * filesystems that only store 16 bits of the UID/GID, etc.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is the UID and GID that will get written to disk if a filesystem
// /*AFLA*/  * only supports 16-bit UIDs and the kernel has a high UID/GID to write
// /*AFLA*/  */
// /*AFLA*/ extern int fs_overflowuid;
// /*AFLA*/ extern int fs_overflowgid;
// /*AFLA*/ 
// /*AFLA*/ #define DEFAULT_FS_OVERFLOWUID	65534
// /*AFLA*/ #define DEFAULT_FS_OVERFLOWGID	65534
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Since these macros are used in architectures that only need limited
// /*AFLA*/  * 16-bit UID back compatibility, we won't use old_uid_t and old_gid_t
// /*AFLA*/  */
// /*AFLA*/ #define fs_high2lowuid(uid) ((uid) & ~0xFFFF ? (uid16_t)fs_overflowuid : (uid16_t)(uid))
// /*AFLA*/ #define fs_high2lowgid(gid) ((gid) & ~0xFFFF ? (gid16_t)fs_overflowgid : (gid16_t)(gid))
// /*AFLA*/ 
// /*AFLA*/ #define low_16_bits(x)	((x) & 0xFFFF)
// /*AFLA*/ #define high_16_bits(x)	(((x) & 0xFFFF0000) >> 16)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_HIGHUID_H */
