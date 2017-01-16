// /*AFLA*/ /*
// /*AFLA*/  * kref.h - library routines for handling generic reference counted objects
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2004 Greg Kroah-Hartman <greg@kroah.com>
// /*AFLA*/  * Copyright (C) 2004 IBM Corp.
// /*AFLA*/  *
// /*AFLA*/  * based on kobject.h which was:
// /*AFLA*/  * Copyright (C) 2002-2003 Patrick Mochel <mochel@osdl.org>
// /*AFLA*/  * Copyright (C) 2002-2003 Open Source Development Labs
// /*AFLA*/  *
// /*AFLA*/  * This file is released under the GPLv2.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _KREF_H_
// /*AFLA*/ #define _KREF_H_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/mutex.h>
// /*AFLA*/ 
// /*AFLA*/ struct kref {
// /*AFLA*/ 	atomic_t refcount;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kref_init - initialize object.
// /*AFLA*/  * @kref: object in question.
// /*AFLA*/  */
// /*AFLA*/ static inline void kref_init(struct kref *kref)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_set(&kref->refcount, 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kref_get - increment refcount for object.
// /*AFLA*/  * @kref: object.
// /*AFLA*/  */
// /*AFLA*/ static inline void kref_get(struct kref *kref)
// /*AFLA*/ {
// /*AFLA*/ 	/* If refcount was 0 before incrementing then we have a race
// /*AFLA*/ 	 * condition when this kref is freeing by some other thread right now.
// /*AFLA*/ 	 * In this case one should use kref_get_unless_zero()
// /*AFLA*/ 	 */
// /*AFLA*/ 	WARN_ON_ONCE(atomic_inc_return(&kref->refcount) < 2);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kref_sub - subtract a number of refcounts for object.
// /*AFLA*/  * @kref: object.
// /*AFLA*/  * @count: Number of recounts to subtract.
// /*AFLA*/  * @release: pointer to the function that will clean up the object when the
// /*AFLA*/  *	     last reference to the object is released.
// /*AFLA*/  *	     This pointer is required, and it is not acceptable to pass kfree
// /*AFLA*/  *	     in as this function.  If the caller does pass kfree to this
// /*AFLA*/  *	     function, you will be publicly mocked mercilessly by the kref
// /*AFLA*/  *	     maintainer, and anyone else who happens to notice it.  You have
// /*AFLA*/  *	     been warned.
// /*AFLA*/  *
// /*AFLA*/  * Subtract @count from the refcount, and if 0, call release().
// /*AFLA*/  * Return 1 if the object was removed, otherwise return 0.  Beware, if this
// /*AFLA*/  * function returns 0, you still can not count on the kref from remaining in
// /*AFLA*/  * memory.  Only use the return value if you want to see if the kref is now
// /*AFLA*/  * gone, not present.
// /*AFLA*/  */
// /*AFLA*/ static inline int kref_sub(struct kref *kref, unsigned int count,
// /*AFLA*/ 	     void (*release)(struct kref *kref))
// /*AFLA*/ {
// /*AFLA*/ 	WARN_ON(release == NULL);
// /*AFLA*/ 
// /*AFLA*/ 	if (atomic_sub_and_test((int) count, &kref->refcount)) {
// /*AFLA*/ 		release(kref);
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	}
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kref_put - decrement refcount for object.
// /*AFLA*/  * @kref: object.
// /*AFLA*/  * @release: pointer to the function that will clean up the object when the
// /*AFLA*/  *	     last reference to the object is released.
// /*AFLA*/  *	     This pointer is required, and it is not acceptable to pass kfree
// /*AFLA*/  *	     in as this function.  If the caller does pass kfree to this
// /*AFLA*/  *	     function, you will be publicly mocked mercilessly by the kref
// /*AFLA*/  *	     maintainer, and anyone else who happens to notice it.  You have
// /*AFLA*/  *	     been warned.
// /*AFLA*/  *
// /*AFLA*/  * Decrement the refcount, and if 0, call release().
// /*AFLA*/  * Return 1 if the object was removed, otherwise return 0.  Beware, if this
// /*AFLA*/  * function returns 0, you still can not count on the kref from remaining in
// /*AFLA*/  * memory.  Only use the return value if you want to see if the kref is now
// /*AFLA*/  * gone, not present.
// /*AFLA*/  */
// /*AFLA*/ static inline int kref_put(struct kref *kref, void (*release)(struct kref *kref))
// /*AFLA*/ {
// /*AFLA*/ 	return kref_sub(kref, 1, release);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int kref_put_mutex(struct kref *kref,
// /*AFLA*/ 				 void (*release)(struct kref *kref),
// /*AFLA*/ 				 struct mutex *lock)
// /*AFLA*/ {
// /*AFLA*/ 	WARN_ON(release == NULL);
// /*AFLA*/ 	if (unlikely(!atomic_add_unless(&kref->refcount, -1, 1))) {
// /*AFLA*/ 		mutex_lock(lock);
// /*AFLA*/ 		if (unlikely(!atomic_dec_and_test(&kref->refcount))) {
// /*AFLA*/ 			mutex_unlock(lock);
// /*AFLA*/ 			return 0;
// /*AFLA*/ 		}
// /*AFLA*/ 		release(kref);
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	}
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kref_get_unless_zero - Increment refcount for object unless it is zero.
// /*AFLA*/  * @kref: object.
// /*AFLA*/  *
// /*AFLA*/  * Return non-zero if the increment succeeded. Otherwise return 0.
// /*AFLA*/  *
// /*AFLA*/  * This function is intended to simplify locking around refcounting for
// /*AFLA*/  * objects that can be looked up from a lookup structure, and which are
// /*AFLA*/  * removed from that lookup structure in the object destructor.
// /*AFLA*/  * Operations on such objects require at least a read lock around
// /*AFLA*/  * lookup + kref_get, and a write lock around kref_put + remove from lookup
// /*AFLA*/  * structure. Furthermore, RCU implementations become extremely tricky.
// /*AFLA*/  * With a lookup followed by a kref_get_unless_zero *with return value check*
// /*AFLA*/  * locking in the kref_put path can be deferred to the actual removal from
// /*AFLA*/  * the lookup structure and RCU lookups become trivial.
// /*AFLA*/  */
// /*AFLA*/ static inline int __must_check kref_get_unless_zero(struct kref *kref)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_add_unless(&kref->refcount, 1, 0);
// /*AFLA*/ }
// /*AFLA*/ #endif /* _KREF_H_ */
