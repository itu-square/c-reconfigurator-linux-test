// /*AFLA*/ #ifndef _LINUX_UIDGID_H
// /*AFLA*/ #define _LINUX_UIDGID_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A set of types for the internal kernel types representing uids and gids.
// /*AFLA*/  *
// /*AFLA*/  * The types defined in this header allow distinguishing which uids and gids in
// /*AFLA*/  * the kernel are values used by userspace and which uid and gid values are
// /*AFLA*/  * the internal kernel values.  With the addition of user namespaces the values
// /*AFLA*/  * can be different.  Using the type system makes it possible for the compiler
// /*AFLA*/  * to detect when we overlook these differences.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/highuid.h>
// /*AFLA*/ 
// /*AFLA*/ struct user_namespace;
// /*AFLA*/ extern struct user_namespace init_user_ns;
// /*AFLA*/ 
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	uid_t val;
// /*AFLA*/ } kuid_t;
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	gid_t val;
// /*AFLA*/ } kgid_t;
// /*AFLA*/ 
// /*AFLA*/ #define KUIDT_INIT(value) (kuid_t){ value }
// /*AFLA*/ #define KGIDT_INIT(value) (kgid_t){ value }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MULTIUSER
// /*AFLA*/ static inline uid_t __kuid_val(kuid_t uid)
// /*AFLA*/ {
// /*AFLA*/ 	return uid.val;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline gid_t __kgid_val(kgid_t gid)
// /*AFLA*/ {
// /*AFLA*/ 	return gid.val;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline uid_t __kuid_val(kuid_t uid)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline gid_t __kgid_val(kgid_t gid)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define GLOBAL_ROOT_UID KUIDT_INIT(0)
// /*AFLA*/ #define GLOBAL_ROOT_GID KGIDT_INIT(0)
// /*AFLA*/ 
// /*AFLA*/ #define INVALID_UID KUIDT_INIT(-1)
// /*AFLA*/ #define INVALID_GID KGIDT_INIT(-1)
// /*AFLA*/ 
// /*AFLA*/ static inline bool uid_eq(kuid_t left, kuid_t right)
// /*AFLA*/ {
// /*AFLA*/ 	return __kuid_val(left) == __kuid_val(right);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool gid_eq(kgid_t left, kgid_t right)
// /*AFLA*/ {
// /*AFLA*/ 	return __kgid_val(left) == __kgid_val(right);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool uid_gt(kuid_t left, kuid_t right)
// /*AFLA*/ {
// /*AFLA*/ 	return __kuid_val(left) > __kuid_val(right);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool gid_gt(kgid_t left, kgid_t right)
// /*AFLA*/ {
// /*AFLA*/ 	return __kgid_val(left) > __kgid_val(right);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool uid_gte(kuid_t left, kuid_t right)
// /*AFLA*/ {
// /*AFLA*/ 	return __kuid_val(left) >= __kuid_val(right);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool gid_gte(kgid_t left, kgid_t right)
// /*AFLA*/ {
// /*AFLA*/ 	return __kgid_val(left) >= __kgid_val(right);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool uid_lt(kuid_t left, kuid_t right)
// /*AFLA*/ {
// /*AFLA*/ 	return __kuid_val(left) < __kuid_val(right);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool gid_lt(kgid_t left, kgid_t right)
// /*AFLA*/ {
// /*AFLA*/ 	return __kgid_val(left) < __kgid_val(right);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool uid_lte(kuid_t left, kuid_t right)
// /*AFLA*/ {
// /*AFLA*/ 	return __kuid_val(left) <= __kuid_val(right);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool gid_lte(kgid_t left, kgid_t right)
// /*AFLA*/ {
// /*AFLA*/ 	return __kgid_val(left) <= __kgid_val(right);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool uid_valid(kuid_t uid)
// /*AFLA*/ {
// /*AFLA*/ 	return __kuid_val(uid) != (uid_t) -1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool gid_valid(kgid_t gid)
// /*AFLA*/ {
// /*AFLA*/ 	return __kgid_val(gid) != (gid_t) -1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_USER_NS
// /*AFLA*/ 
// /*AFLA*/ extern kuid_t make_kuid(struct user_namespace *from, uid_t uid);
// /*AFLA*/ extern kgid_t make_kgid(struct user_namespace *from, gid_t gid);
// /*AFLA*/ 
// /*AFLA*/ extern uid_t from_kuid(struct user_namespace *to, kuid_t uid);
// /*AFLA*/ extern gid_t from_kgid(struct user_namespace *to, kgid_t gid);
// /*AFLA*/ extern uid_t from_kuid_munged(struct user_namespace *to, kuid_t uid);
// /*AFLA*/ extern gid_t from_kgid_munged(struct user_namespace *to, kgid_t gid);
// /*AFLA*/ 
// /*AFLA*/ static inline bool kuid_has_mapping(struct user_namespace *ns, kuid_t uid)
// /*AFLA*/ {
// /*AFLA*/ 	return from_kuid(ns, uid) != (uid_t) -1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool kgid_has_mapping(struct user_namespace *ns, kgid_t gid)
// /*AFLA*/ {
// /*AFLA*/ 	return from_kgid(ns, gid) != (gid_t) -1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline kuid_t make_kuid(struct user_namespace *from, uid_t uid)
// /*AFLA*/ {
// /*AFLA*/ 	return KUIDT_INIT(uid);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline kgid_t make_kgid(struct user_namespace *from, gid_t gid)
// /*AFLA*/ {
// /*AFLA*/ 	return KGIDT_INIT(gid);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline uid_t from_kuid(struct user_namespace *to, kuid_t kuid)
// /*AFLA*/ {
// /*AFLA*/ 	return __kuid_val(kuid);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline gid_t from_kgid(struct user_namespace *to, kgid_t kgid)
// /*AFLA*/ {
// /*AFLA*/ 	return __kgid_val(kgid);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline uid_t from_kuid_munged(struct user_namespace *to, kuid_t kuid)
// /*AFLA*/ {
// /*AFLA*/ 	uid_t uid = from_kuid(to, kuid);
// /*AFLA*/ 	if (uid == (uid_t)-1)
// /*AFLA*/ 		uid = overflowuid;
// /*AFLA*/ 	return uid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline gid_t from_kgid_munged(struct user_namespace *to, kgid_t kgid)
// /*AFLA*/ {
// /*AFLA*/ 	gid_t gid = from_kgid(to, kgid);
// /*AFLA*/ 	if (gid == (gid_t)-1)
// /*AFLA*/ 		gid = overflowgid;
// /*AFLA*/ 	return gid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool kuid_has_mapping(struct user_namespace *ns, kuid_t uid)
// /*AFLA*/ {
// /*AFLA*/ 	return uid_valid(uid);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool kgid_has_mapping(struct user_namespace *ns, kgid_t gid)
// /*AFLA*/ {
// /*AFLA*/ 	return gid_valid(gid);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_USER_NS */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_UIDGID_H */
