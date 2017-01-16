// /*AFLA*/ #ifndef __LINUX_LOCKREF_H
// /*AFLA*/ #define __LINUX_LOCKREF_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Locked reference counts.
// /*AFLA*/  *
// /*AFLA*/  * These are different from just plain atomic refcounts in that they
// /*AFLA*/  * are atomic with respect to the spinlock that goes with them.  In
// /*AFLA*/  * particular, there can be implementations that don't actually get
// /*AFLA*/  * the spinlock for the common decrement/increment operations, but they
// /*AFLA*/  * still have to check that the operation is done semantically as if
// /*AFLA*/  * the spinlock had been taken (using a cmpxchg operation that covers
// /*AFLA*/  * both the lock and the count word, or using memory transactions, for
// /*AFLA*/  * example).
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <generated/bounds.h>
// /*AFLA*/ 
// /*AFLA*/ #define USE_CMPXCHG_LOCKREF \
// /*AFLA*/ 	(IS_ENABLED(CONFIG_ARCH_USE_CMPXCHG_LOCKREF) && \
// /*AFLA*/ 	 IS_ENABLED(CONFIG_SMP) && SPINLOCK_SIZE <= 4)
// /*AFLA*/ 
// /*AFLA*/ struct lockref {
// /*AFLA*/ 	union {
// /*AFLA*/ #if USE_CMPXCHG_LOCKREF
// /*AFLA*/ 		aligned_u64 lock_count;
// /*AFLA*/ #endif
// /*AFLA*/ 		struct {
// /*AFLA*/ 			spinlock_t lock;
// /*AFLA*/ 			int count;
// /*AFLA*/ 		};
// /*AFLA*/ 	};
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern void lockref_get(struct lockref *);
// /*AFLA*/ extern int lockref_put_return(struct lockref *);
// /*AFLA*/ extern int lockref_get_not_zero(struct lockref *);
// /*AFLA*/ extern int lockref_get_or_lock(struct lockref *);
// /*AFLA*/ extern int lockref_put_or_lock(struct lockref *);
// /*AFLA*/ 
// /*AFLA*/ extern void lockref_mark_dead(struct lockref *);
// /*AFLA*/ extern int lockref_get_not_dead(struct lockref *);
// /*AFLA*/ 
// /*AFLA*/ /* Must be called under spinlock for reliable results */
// /*AFLA*/ static inline int __lockref_is_dead(const struct lockref *l)
// /*AFLA*/ {
// /*AFLA*/ 	return ((int)l->count < 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_LOCKREF_H */
