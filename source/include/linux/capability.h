// /*AFLA*/ /*
// /*AFLA*/  * This is <linux/capability.h>
// /*AFLA*/  *
// /*AFLA*/  * Andrew G. Morgan <morgan@kernel.org>
// /*AFLA*/  * Alexander Kjeldaas <astor@guardian.no>
// /*AFLA*/  * with help from Aleph1, Roland Buresund and Andrew Main.
// /*AFLA*/  *
// /*AFLA*/  * See here for the libcap library ("POSIX draft" compliance):
// /*AFLA*/  *
// /*AFLA*/  * ftp://www.kernel.org/pub/linux/libs/security/linux-privs/kernel-2.6/
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LINUX_CAPABILITY_H
// /*AFLA*/ #define _LINUX_CAPABILITY_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/capability.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define _KERNEL_CAPABILITY_VERSION _LINUX_CAPABILITY_VERSION_3
// /*AFLA*/ #define _KERNEL_CAPABILITY_U32S    _LINUX_CAPABILITY_U32S_3
// /*AFLA*/ 
// /*AFLA*/ extern int file_caps_enabled;
// /*AFLA*/ 
// /*AFLA*/ typedef struct kernel_cap_struct {
// /*AFLA*/ 	__u32 cap[_KERNEL_CAPABILITY_U32S];
// /*AFLA*/ } kernel_cap_t;
// /*AFLA*/ 
// /*AFLA*/ /* exact same as vfs_cap_data but in cpu endian and always filled completely */
// /*AFLA*/ struct cpu_vfs_cap_data {
// /*AFLA*/ 	__u32 magic_etc;
// /*AFLA*/ 	kernel_cap_t permitted;
// /*AFLA*/ 	kernel_cap_t inheritable;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define _USER_CAP_HEADER_SIZE  (sizeof(struct __user_cap_header_struct))
// /*AFLA*/ #define _KERNEL_CAP_T_SIZE     (sizeof(kernel_cap_t))
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ struct file;
// /*AFLA*/ struct inode;
// /*AFLA*/ struct dentry;
// /*AFLA*/ struct task_struct;
// /*AFLA*/ struct user_namespace;
// /*AFLA*/ 
// /*AFLA*/ extern const kernel_cap_t __cap_empty_set;
// /*AFLA*/ extern const kernel_cap_t __cap_init_eff_set;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Internal kernel functions only
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_FOR_EACH_U32(__capi)  \
// /*AFLA*/ 	for (__capi = 0; __capi < _KERNEL_CAPABILITY_U32S; ++__capi)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * CAP_FS_MASK and CAP_NFSD_MASKS:
// /*AFLA*/  *
// /*AFLA*/  * The fs mask is all the privileges that fsuid==0 historically meant.
// /*AFLA*/  * At one time in the past, that included CAP_MKNOD and CAP_LINUX_IMMUTABLE.
// /*AFLA*/  *
// /*AFLA*/  * It has never meant setting security.* and trusted.* xattrs.
// /*AFLA*/  *
// /*AFLA*/  * We could also define fsmask as follows:
// /*AFLA*/  *   1. CAP_FS_MASK is the privilege to bypass all fs-related DAC permissions
// /*AFLA*/  *   2. The security.* and trusted.* xattrs are fs-related MAC permissions
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ # define CAP_FS_MASK_B0     (CAP_TO_MASK(CAP_CHOWN)		\
// /*AFLA*/ 			    | CAP_TO_MASK(CAP_MKNOD)		\
// /*AFLA*/ 			    | CAP_TO_MASK(CAP_DAC_OVERRIDE)	\
// /*AFLA*/ 			    | CAP_TO_MASK(CAP_DAC_READ_SEARCH)	\
// /*AFLA*/ 			    | CAP_TO_MASK(CAP_FOWNER)		\
// /*AFLA*/ 			    | CAP_TO_MASK(CAP_FSETID))
// /*AFLA*/ 
// /*AFLA*/ # define CAP_FS_MASK_B1     (CAP_TO_MASK(CAP_MAC_OVERRIDE))
// /*AFLA*/ 
// /*AFLA*/ #if _KERNEL_CAPABILITY_U32S != 2
// /*AFLA*/ # error Fix up hand-coded capability macro initializers
// /*AFLA*/ #else /* HAND-CODED capability initializers */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_LAST_U32			((_KERNEL_CAPABILITY_U32S) - 1)
// /*AFLA*/ #define CAP_LAST_U32_VALID_MASK		(CAP_TO_MASK(CAP_LAST_CAP + 1) -1)
// /*AFLA*/ 
// /*AFLA*/ # define CAP_EMPTY_SET    ((kernel_cap_t){{ 0, 0 }})
// /*AFLA*/ # define CAP_FULL_SET     ((kernel_cap_t){{ ~0, CAP_LAST_U32_VALID_MASK }})
// /*AFLA*/ # define CAP_FS_SET       ((kernel_cap_t){{ CAP_FS_MASK_B0 \
// /*AFLA*/ 				    | CAP_TO_MASK(CAP_LINUX_IMMUTABLE), \
// /*AFLA*/ 				    CAP_FS_MASK_B1 } })
// /*AFLA*/ # define CAP_NFSD_SET     ((kernel_cap_t){{ CAP_FS_MASK_B0 \
// /*AFLA*/ 				    | CAP_TO_MASK(CAP_SYS_RESOURCE), \
// /*AFLA*/ 				    CAP_FS_MASK_B1 } })
// /*AFLA*/ 
// /*AFLA*/ #endif /* _KERNEL_CAPABILITY_U32S != 2 */
// /*AFLA*/ 
// /*AFLA*/ # define cap_clear(c)         do { (c) = __cap_empty_set; } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define cap_raise(c, flag)  ((c).cap[CAP_TO_INDEX(flag)] |= CAP_TO_MASK(flag))
// /*AFLA*/ #define cap_lower(c, flag)  ((c).cap[CAP_TO_INDEX(flag)] &= ~CAP_TO_MASK(flag))
// /*AFLA*/ #define cap_raised(c, flag) ((c).cap[CAP_TO_INDEX(flag)] & CAP_TO_MASK(flag))
// /*AFLA*/ 
// /*AFLA*/ #define CAP_BOP_ALL(c, a, b, OP)                                    \
// /*AFLA*/ do {                                                                \
// /*AFLA*/ 	unsigned __capi;                                            \
// /*AFLA*/ 	CAP_FOR_EACH_U32(__capi) {                                  \
// /*AFLA*/ 		c.cap[__capi] = a.cap[__capi] OP b.cap[__capi];     \
// /*AFLA*/ 	}                                                           \
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define CAP_UOP_ALL(c, a, OP)                                       \
// /*AFLA*/ do {                                                                \
// /*AFLA*/ 	unsigned __capi;                                            \
// /*AFLA*/ 	CAP_FOR_EACH_U32(__capi) {                                  \
// /*AFLA*/ 		c.cap[__capi] = OP a.cap[__capi];                   \
// /*AFLA*/ 	}                                                           \
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ static inline kernel_cap_t cap_combine(const kernel_cap_t a,
// /*AFLA*/ 				       const kernel_cap_t b)
// /*AFLA*/ {
// /*AFLA*/ 	kernel_cap_t dest;
// /*AFLA*/ 	CAP_BOP_ALL(dest, a, b, |);
// /*AFLA*/ 	return dest;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline kernel_cap_t cap_intersect(const kernel_cap_t a,
// /*AFLA*/ 					 const kernel_cap_t b)
// /*AFLA*/ {
// /*AFLA*/ 	kernel_cap_t dest;
// /*AFLA*/ 	CAP_BOP_ALL(dest, a, b, &);
// /*AFLA*/ 	return dest;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline kernel_cap_t cap_drop(const kernel_cap_t a,
// /*AFLA*/ 				    const kernel_cap_t drop)
// /*AFLA*/ {
// /*AFLA*/ 	kernel_cap_t dest;
// /*AFLA*/ 	CAP_BOP_ALL(dest, a, drop, &~);
// /*AFLA*/ 	return dest;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline kernel_cap_t cap_invert(const kernel_cap_t c)
// /*AFLA*/ {
// /*AFLA*/ 	kernel_cap_t dest;
// /*AFLA*/ 	CAP_UOP_ALL(dest, c, ~);
// /*AFLA*/ 	return dest;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool cap_isclear(const kernel_cap_t a)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned __capi;
// /*AFLA*/ 	CAP_FOR_EACH_U32(__capi) {
// /*AFLA*/ 		if (a.cap[__capi] != 0)
// /*AFLA*/ 			return false;
// /*AFLA*/ 	}
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Check if "a" is a subset of "set".
// /*AFLA*/  * return true if ALL of the capabilities in "a" are also in "set"
// /*AFLA*/  *	cap_issubset(0101, 1111) will return true
// /*AFLA*/  * return false if ANY of the capabilities in "a" are not in "set"
// /*AFLA*/  *	cap_issubset(1111, 0101) will return false
// /*AFLA*/  */
// /*AFLA*/ static inline bool cap_issubset(const kernel_cap_t a, const kernel_cap_t set)
// /*AFLA*/ {
// /*AFLA*/ 	kernel_cap_t dest;
// /*AFLA*/ 	dest = cap_drop(a, set);
// /*AFLA*/ 	return cap_isclear(dest);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Used to decide between falling back on the old suser() or fsuser(). */
// /*AFLA*/ 
// /*AFLA*/ static inline kernel_cap_t cap_drop_fs_set(const kernel_cap_t a)
// /*AFLA*/ {
// /*AFLA*/ 	const kernel_cap_t __cap_fs_set = CAP_FS_SET;
// /*AFLA*/ 	return cap_drop(a, __cap_fs_set);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline kernel_cap_t cap_raise_fs_set(const kernel_cap_t a,
// /*AFLA*/ 					    const kernel_cap_t permitted)
// /*AFLA*/ {
// /*AFLA*/ 	const kernel_cap_t __cap_fs_set = CAP_FS_SET;
// /*AFLA*/ 	return cap_combine(a,
// /*AFLA*/ 			   cap_intersect(permitted, __cap_fs_set));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline kernel_cap_t cap_drop_nfsd_set(const kernel_cap_t a)
// /*AFLA*/ {
// /*AFLA*/ 	const kernel_cap_t __cap_fs_set = CAP_NFSD_SET;
// /*AFLA*/ 	return cap_drop(a, __cap_fs_set);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline kernel_cap_t cap_raise_nfsd_set(const kernel_cap_t a,
// /*AFLA*/ 					      const kernel_cap_t permitted)
// /*AFLA*/ {
// /*AFLA*/ 	const kernel_cap_t __cap_nfsd_set = CAP_NFSD_SET;
// /*AFLA*/ 	return cap_combine(a,
// /*AFLA*/ 			   cap_intersect(permitted, __cap_nfsd_set));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MULTIUSER
// /*AFLA*/ extern bool has_capability(struct task_struct *t, int cap);
// /*AFLA*/ extern bool has_ns_capability(struct task_struct *t,
// /*AFLA*/ 			      struct user_namespace *ns, int cap);
// /*AFLA*/ extern bool has_capability_noaudit(struct task_struct *t, int cap);
// /*AFLA*/ extern bool has_ns_capability_noaudit(struct task_struct *t,
// /*AFLA*/ 				      struct user_namespace *ns, int cap);
// /*AFLA*/ extern bool capable(int cap);
// /*AFLA*/ extern bool ns_capable(struct user_namespace *ns, int cap);
// /*AFLA*/ extern bool ns_capable_noaudit(struct user_namespace *ns, int cap);
// /*AFLA*/ #else
// /*AFLA*/ static inline bool has_capability(struct task_struct *t, int cap)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ static inline bool has_ns_capability(struct task_struct *t,
// /*AFLA*/ 			      struct user_namespace *ns, int cap)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ static inline bool has_capability_noaudit(struct task_struct *t, int cap)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ static inline bool has_ns_capability_noaudit(struct task_struct *t,
// /*AFLA*/ 				      struct user_namespace *ns, int cap)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ static inline bool capable(int cap)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ static inline bool ns_capable(struct user_namespace *ns, int cap)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ static inline bool ns_capable_noaudit(struct user_namespace *ns, int cap)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_MULTIUSER */
// /*AFLA*/ extern bool capable_wrt_inode_uidgid(const struct inode *inode, int cap);
// /*AFLA*/ extern bool file_ns_capable(const struct file *file, struct user_namespace *ns, int cap);
// /*AFLA*/ 
// /*AFLA*/ /* audit system wants to get cap info from files as well */
// /*AFLA*/ extern int get_vfs_caps_from_disk(const struct dentry *dentry, struct cpu_vfs_cap_data *cpu_caps);
// /*AFLA*/ 
// /*AFLA*/ #endif /* !_LINUX_CAPABILITY_H */
