// /*AFLA*/ #ifndef _LINUX_PROJID_H
// /*AFLA*/ #define _LINUX_PROJID_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A set of types for the internal kernel types representing project ids.
// /*AFLA*/  *
// /*AFLA*/  * The types defined in this header allow distinguishing which project ids in
// /*AFLA*/  * the kernel are values used by userspace and which project id values are
// /*AFLA*/  * the internal kernel values.  With the addition of user namespaces the values
// /*AFLA*/  * can be different.  Using the type system makes it possible for the compiler
// /*AFLA*/  * to detect when we overlook these differences.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ struct user_namespace;
// /*AFLA*/ extern struct user_namespace init_user_ns;
// /*AFLA*/ 
// /*AFLA*/ typedef __kernel_uid32_t projid_t;
// /*AFLA*/ 
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	projid_t val;
// /*AFLA*/ } kprojid_t;
// /*AFLA*/ 
// /*AFLA*/ static inline projid_t __kprojid_val(kprojid_t projid)
// /*AFLA*/ {
// /*AFLA*/ 	return projid.val;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define KPROJIDT_INIT(value) (kprojid_t){ value }
// /*AFLA*/ 
// /*AFLA*/ #define INVALID_PROJID KPROJIDT_INIT(-1)
// /*AFLA*/ #define OVERFLOW_PROJID 65534
// /*AFLA*/ 
// /*AFLA*/ static inline bool projid_eq(kprojid_t left, kprojid_t right)
// /*AFLA*/ {
// /*AFLA*/ 	return __kprojid_val(left) == __kprojid_val(right);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool projid_lt(kprojid_t left, kprojid_t right)
// /*AFLA*/ {
// /*AFLA*/ 	return __kprojid_val(left) < __kprojid_val(right);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool projid_valid(kprojid_t projid)
// /*AFLA*/ {
// /*AFLA*/ 	return !projid_eq(projid, INVALID_PROJID);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_USER_NS
// /*AFLA*/ 
// /*AFLA*/ extern kprojid_t make_kprojid(struct user_namespace *from, projid_t projid);
// /*AFLA*/ 
// /*AFLA*/ extern projid_t from_kprojid(struct user_namespace *to, kprojid_t projid);
// /*AFLA*/ extern projid_t from_kprojid_munged(struct user_namespace *to, kprojid_t projid);
// /*AFLA*/ 
// /*AFLA*/ static inline bool kprojid_has_mapping(struct user_namespace *ns, kprojid_t projid)
// /*AFLA*/ {
// /*AFLA*/ 	return from_kprojid(ns, projid) != (projid_t)-1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline kprojid_t make_kprojid(struct user_namespace *from, projid_t projid)
// /*AFLA*/ {
// /*AFLA*/ 	return KPROJIDT_INIT(projid);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline projid_t from_kprojid(struct user_namespace *to, kprojid_t kprojid)
// /*AFLA*/ {
// /*AFLA*/ 	return __kprojid_val(kprojid);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline projid_t from_kprojid_munged(struct user_namespace *to, kprojid_t kprojid)
// /*AFLA*/ {
// /*AFLA*/ 	projid_t projid = from_kprojid(to, kprojid);
// /*AFLA*/ 	if (projid == (projid_t)-1)
// /*AFLA*/ 		projid = OVERFLOW_PROJID;
// /*AFLA*/ 	return projid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool kprojid_has_mapping(struct user_namespace *ns, kprojid_t projid)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_USER_NS */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_PROJID_H */
