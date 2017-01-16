// /*AFLA*/ #ifndef __LINUX_MUTEX_DEBUG_H
// /*AFLA*/ #define __LINUX_MUTEX_DEBUG_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ #include <linux/lockdep.h>
// /*AFLA*/ #include <linux/debug_locks.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Mutexes - debugging helpers:
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define __DEBUG_MUTEX_INITIALIZER(lockname)				\
// /*AFLA*/ 	, .magic = &lockname
// /*AFLA*/ 
// /*AFLA*/ #define mutex_init(mutex)						\
// /*AFLA*/ do {									\
// /*AFLA*/ 	static struct lock_class_key __key;				\
// /*AFLA*/ 									\
// /*AFLA*/ 	__mutex_init((mutex), #mutex, &__key);				\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ extern void mutex_destroy(struct mutex *lock);
// /*AFLA*/ 
// /*AFLA*/ #endif
