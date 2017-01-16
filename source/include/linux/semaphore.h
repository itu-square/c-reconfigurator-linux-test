// /*AFLA*/ /*
// /*AFLA*/  * Copyright (c) 2008 Intel Corporation
// /*AFLA*/  * Author: Matthew Wilcox <willy@linux.intel.com>
// /*AFLA*/  *
// /*AFLA*/  * Distributed under the terms of the GNU GPL, version 2
// /*AFLA*/  *
// /*AFLA*/  * Please see kernel/semaphore.c for documentation of these functions
// /*AFLA*/  */
// /*AFLA*/ #ifndef __LINUX_SEMAPHORE_H
// /*AFLA*/ #define __LINUX_SEMAPHORE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ 
// /*AFLA*/ /* Please don't access any members of this structure directly */
// /*AFLA*/ struct semaphore {
// /*AFLA*/ 	raw_spinlock_t		lock;
// /*AFLA*/ 	unsigned int		count;
// /*AFLA*/ 	struct list_head	wait_list;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define __SEMAPHORE_INITIALIZER(name, n)				\
// /*AFLA*/ {									\
// /*AFLA*/ 	.lock		= __RAW_SPIN_LOCK_UNLOCKED((name).lock),	\
// /*AFLA*/ 	.count		= n,						\
// /*AFLA*/ 	.wait_list	= LIST_HEAD_INIT((name).wait_list),		\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_SEMAPHORE(name)	\
// /*AFLA*/ 	struct semaphore name = __SEMAPHORE_INITIALIZER(name, 1)
// /*AFLA*/ 
// /*AFLA*/ static inline void sema_init(struct semaphore *sem, int val)
// /*AFLA*/ {
// /*AFLA*/ 	static struct lock_class_key __key;
// /*AFLA*/ 	*sem = (struct semaphore) __SEMAPHORE_INITIALIZER(*sem, val);
// /*AFLA*/ 	lockdep_init_map(&sem->lock.dep_map, "semaphore->lock", &__key, 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void down(struct semaphore *sem);
// /*AFLA*/ extern int __must_check down_interruptible(struct semaphore *sem);
// /*AFLA*/ extern int __must_check down_killable(struct semaphore *sem);
// /*AFLA*/ extern int __must_check down_trylock(struct semaphore *sem);
// /*AFLA*/ extern int __must_check down_timeout(struct semaphore *sem, long jiffies);
// /*AFLA*/ extern void up(struct semaphore *sem);
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_SEMAPHORE_H */
