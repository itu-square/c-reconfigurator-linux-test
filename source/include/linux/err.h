// /*AFLA*/ #ifndef _LINUX_ERR_H
// /*AFLA*/ #define _LINUX_ERR_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/errno.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Kernel pointers have redundant information, so we can use a
// /*AFLA*/  * scheme where we can return either an error code or a normal
// /*AFLA*/  * pointer with the same return value.
// /*AFLA*/  *
// /*AFLA*/  * This should be a per-architecture thing, to allow different
// /*AFLA*/  * error and pointer decisions.
// /*AFLA*/  */
// /*AFLA*/ #define MAX_ERRNO	4095
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #define IS_ERR_VALUE(x) unlikely((unsigned long)(void *)(x) >= (unsigned long)-MAX_ERRNO)
// /*AFLA*/ 
// /*AFLA*/ static inline void * __must_check ERR_PTR(long error)
// /*AFLA*/ {
// /*AFLA*/ 	return (void *) error;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline long __must_check PTR_ERR(__force const void *ptr)
// /*AFLA*/ {
// /*AFLA*/ 	return (long) ptr;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool __must_check IS_ERR(__force const void *ptr)
// /*AFLA*/ {
// /*AFLA*/ 	return IS_ERR_VALUE((unsigned long)ptr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool __must_check IS_ERR_OR_NULL(__force const void *ptr)
// /*AFLA*/ {
// /*AFLA*/ 	return unlikely(!ptr) || IS_ERR_VALUE((unsigned long)ptr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ERR_CAST - Explicitly cast an error-valued pointer to another pointer type
// /*AFLA*/  * @ptr: The pointer to cast.
// /*AFLA*/  *
// /*AFLA*/  * Explicitly cast an error-valued pointer to another pointer type in such a
// /*AFLA*/  * way as to make it clear that's what's going on.
// /*AFLA*/  */
// /*AFLA*/ static inline void * __must_check ERR_CAST(__force const void *ptr)
// /*AFLA*/ {
// /*AFLA*/ 	/* cast away the const */
// /*AFLA*/ 	return (void *) ptr;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __must_check PTR_ERR_OR_ZERO(__force const void *ptr)
// /*AFLA*/ {
// /*AFLA*/ 	if (IS_ERR(ptr))
// /*AFLA*/ 		return PTR_ERR(ptr);
// /*AFLA*/ 	else
// /*AFLA*/ 		return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Deprecated */
// /*AFLA*/ #define PTR_RET(p) PTR_ERR_OR_ZERO(p)
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_ERR_H */
