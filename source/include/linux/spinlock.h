// /*AFLA*/ #ifndef __LINUX_SPINLOCK_H
// /*AFLA*/ #define __LINUX_SPINLOCK_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * include/linux/spinlock.h - generic spinlock/rwlock declarations
// /*AFLA*/  *
// /*AFLA*/  * here's the role of the various spinlock/rwlock related include files:
// /*AFLA*/  *
// /*AFLA*/  * on SMP builds:
// /*AFLA*/  *
// /*AFLA*/  *  asm/spinlock_types.h: contains the arch_spinlock_t/arch_rwlock_t and the
// /*AFLA*/  *                        initializers
// /*AFLA*/  *
// /*AFLA*/  *  linux/spinlock_types.h:
// /*AFLA*/  *                        defines the generic type and initializers
// /*AFLA*/  *
// /*AFLA*/  *  asm/spinlock.h:       contains the arch_spin_*()/etc. lowlevel
// /*AFLA*/  *                        implementations, mostly inline assembly code
// /*AFLA*/  *
// /*AFLA*/  *   (also included on UP-debug builds:)
// /*AFLA*/  *
// /*AFLA*/  *  linux/spinlock_api_smp.h:
// /*AFLA*/  *                        contains the prototypes for the _spin_*() APIs.
// /*AFLA*/  *
// /*AFLA*/  *  linux/spinlock.h:     builds the final spin_*() APIs.
// /*AFLA*/  *
// /*AFLA*/  * on UP builds:
// /*AFLA*/  *
// /*AFLA*/  *  linux/spinlock_type_up.h:
// /*AFLA*/  *                        contains the generic, simplified UP spinlock type.
// /*AFLA*/  *                        (which is an empty structure on non-debug builds)
// /*AFLA*/  *
// /*AFLA*/  *  linux/spinlock_types.h:
// /*AFLA*/  *                        defines the generic type and initializers
// /*AFLA*/  *
// /*AFLA*/  *  linux/spinlock_up.h:
// /*AFLA*/  *                        contains the arch_spin_*()/etc. version of UP
// /*AFLA*/  *                        builds. (which are NOPs on non-debug, non-preempt
// /*AFLA*/  *                        builds)
// /*AFLA*/  *
// /*AFLA*/  *   (included on UP-non-debug builds:)
// /*AFLA*/  *
// /*AFLA*/  *  linux/spinlock_api_up.h:
// /*AFLA*/  *                        builds the _spin_*() APIs.
// /*AFLA*/  *
// /*AFLA*/  *  linux/spinlock.h:     builds the final spin_*() APIs.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/typecheck.h>
// /*AFLA*/ #include <linux/preempt.h>
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/irqflags.h>
// /*AFLA*/ #include <linux/thread_info.h>
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/stringify.h>
// /*AFLA*/ #include <linux/bottom_half.h>
// /*AFLA*/ #include <asm/barrier.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Must define these before including other files, inline functions need them
// /*AFLA*/  */
// /*AFLA*/ #define LOCK_SECTION_NAME ".text..lock."KBUILD_BASENAME
// /*AFLA*/ 
// /*AFLA*/ #define LOCK_SECTION_START(extra)               \
// /*AFLA*/         ".subsection 1\n\t"                     \
// /*AFLA*/         extra                                   \
// /*AFLA*/         ".ifndef " LOCK_SECTION_NAME "\n\t"     \
// /*AFLA*/         LOCK_SECTION_NAME ":\n\t"               \
// /*AFLA*/         ".endif\n"
// /*AFLA*/ 
// /*AFLA*/ #define LOCK_SECTION_END                        \
// /*AFLA*/         ".previous\n\t"
// /*AFLA*/ 
// /*AFLA*/ #define __lockfunc __attribute__((section(".spinlock.text")))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Pull the arch_spinlock_t and arch_rwlock_t definitions:
// /*AFLA*/  */
// /*AFLA*/ #include <linux/spinlock_types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Pull the arch_spin*() functions/declarations (UP-nondebug doesn't need them):
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ # include <asm/spinlock.h>
// /*AFLA*/ #else
// /*AFLA*/ # include <linux/spinlock_up.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_SPINLOCK
// /*AFLA*/   extern void __raw_spin_lock_init(raw_spinlock_t *lock, const char *name,
// /*AFLA*/ 				   struct lock_class_key *key);
// /*AFLA*/ # define raw_spin_lock_init(lock)				\
// /*AFLA*/ do {								\
// /*AFLA*/ 	static struct lock_class_key __key;			\
// /*AFLA*/ 								\
// /*AFLA*/ 	__raw_spin_lock_init((lock), #lock, &__key);		\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ # define raw_spin_lock_init(lock)				\
// /*AFLA*/ 	do { *(lock) = __RAW_SPIN_LOCK_UNLOCKED(lock); } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define raw_spin_is_locked(lock)	arch_spin_is_locked(&(lock)->raw_lock)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_GENERIC_LOCKBREAK
// /*AFLA*/ #define raw_spin_is_contended(lock) ((lock)->break_lock)
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ #ifdef arch_spin_is_contended
// /*AFLA*/ #define raw_spin_is_contended(lock)	arch_spin_is_contended(&(lock)->raw_lock)
// /*AFLA*/ #else
// /*AFLA*/ #define raw_spin_is_contended(lock)	(((void)(lock), 0))
// /*AFLA*/ #endif /*arch_spin_is_contended*/
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Despite its name it doesn't necessarily has to be a full barrier.
// /*AFLA*/  * It should only guarantee that a STORE before the critical section
// /*AFLA*/  * can not be reordered with LOADs and STOREs inside this section.
// /*AFLA*/  * spin_lock() is the one-way barrier, this LOAD can not escape out
// /*AFLA*/  * of the region. So the default implementation simply ensures that
// /*AFLA*/  * a STORE can not move into the critical section, smp_wmb() should
// /*AFLA*/  * serialize it with another STORE done by spin_lock().
// /*AFLA*/  */
// /*AFLA*/ #ifndef smp_mb__before_spinlock
// /*AFLA*/ #define smp_mb__before_spinlock()	smp_wmb()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * raw_spin_unlock_wait - wait until the spinlock gets unlocked
// /*AFLA*/  * @lock: the spinlock in question.
// /*AFLA*/  */
// /*AFLA*/ #define raw_spin_unlock_wait(lock)	arch_spin_unlock_wait(&(lock)->raw_lock)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_SPINLOCK
// /*AFLA*/  extern void do_raw_spin_lock(raw_spinlock_t *lock) __acquires(lock);
// /*AFLA*/ #define do_raw_spin_lock_flags(lock, flags) do_raw_spin_lock(lock)
// /*AFLA*/  extern int do_raw_spin_trylock(raw_spinlock_t *lock);
// /*AFLA*/  extern void do_raw_spin_unlock(raw_spinlock_t *lock) __releases(lock);
// /*AFLA*/ #else
// /*AFLA*/ static inline void do_raw_spin_lock(raw_spinlock_t *lock) __acquires(lock)
// /*AFLA*/ {
// /*AFLA*/ 	__acquire(lock);
// /*AFLA*/ 	arch_spin_lock(&lock->raw_lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ do_raw_spin_lock_flags(raw_spinlock_t *lock, unsigned long *flags) __acquires(lock)
// /*AFLA*/ {
// /*AFLA*/ 	__acquire(lock);
// /*AFLA*/ 	arch_spin_lock_flags(&lock->raw_lock, *flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int do_raw_spin_trylock(raw_spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	return arch_spin_trylock(&(lock)->raw_lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void do_raw_spin_unlock(raw_spinlock_t *lock) __releases(lock)
// /*AFLA*/ {
// /*AFLA*/ 	arch_spin_unlock(&lock->raw_lock);
// /*AFLA*/ 	__release(lock);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Define the various spin_lock methods.  Note we define these
// /*AFLA*/  * regardless of whether CONFIG_SMP or CONFIG_PREEMPT are set. The
// /*AFLA*/  * various methods are defined as nops in the case they are not
// /*AFLA*/  * required.
// /*AFLA*/  */
// /*AFLA*/ #define raw_spin_trylock(lock)	__cond_lock(lock, _raw_spin_trylock(lock))
// /*AFLA*/ 
// /*AFLA*/ #define raw_spin_lock(lock)	_raw_spin_lock(lock)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ # define raw_spin_lock_nested(lock, subclass) \
// /*AFLA*/ 	_raw_spin_lock_nested(lock, subclass)
// /*AFLA*/ # define raw_spin_lock_bh_nested(lock, subclass) \
// /*AFLA*/ 	_raw_spin_lock_bh_nested(lock, subclass)
// /*AFLA*/ 
// /*AFLA*/ # define raw_spin_lock_nest_lock(lock, nest_lock)			\
// /*AFLA*/ 	 do {								\
// /*AFLA*/ 		 typecheck(struct lockdep_map *, &(nest_lock)->dep_map);\
// /*AFLA*/ 		 _raw_spin_lock_nest_lock(lock, &(nest_lock)->dep_map);	\
// /*AFLA*/ 	 } while (0)
// /*AFLA*/ #else
// /*AFLA*/ /*
// /*AFLA*/  * Always evaluate the 'subclass' argument to avoid that the compiler
// /*AFLA*/  * warns about set-but-not-used variables when building with
// /*AFLA*/  * CONFIG_DEBUG_LOCK_ALLOC=n and with W=1.
// /*AFLA*/  */
// /*AFLA*/ # define raw_spin_lock_nested(lock, subclass)		\
// /*AFLA*/ 	_raw_spin_lock(((void)(subclass), (lock)))
// /*AFLA*/ # define raw_spin_lock_nest_lock(lock, nest_lock)	_raw_spin_lock(lock)
// /*AFLA*/ # define raw_spin_lock_bh_nested(lock, subclass)	_raw_spin_lock_bh(lock)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK)
// /*AFLA*/ 
// /*AFLA*/ #define raw_spin_lock_irqsave(lock, flags)			\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		typecheck(unsigned long, flags);	\
// /*AFLA*/ 		flags = _raw_spin_lock_irqsave(lock);	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ #define raw_spin_lock_irqsave_nested(lock, flags, subclass)		\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		typecheck(unsigned long, flags);			\
// /*AFLA*/ 		flags = _raw_spin_lock_irqsave_nested(lock, subclass);	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #else
// /*AFLA*/ #define raw_spin_lock_irqsave_nested(lock, flags, subclass)		\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		typecheck(unsigned long, flags);			\
// /*AFLA*/ 		flags = _raw_spin_lock_irqsave(lock);			\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ #define raw_spin_lock_irqsave(lock, flags)		\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		typecheck(unsigned long, flags);	\
// /*AFLA*/ 		_raw_spin_lock_irqsave(lock, flags);	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #define raw_spin_lock_irqsave_nested(lock, flags, subclass)	\
// /*AFLA*/ 	raw_spin_lock_irqsave(lock, flags)
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define raw_spin_lock_irq(lock)		_raw_spin_lock_irq(lock)
// /*AFLA*/ #define raw_spin_lock_bh(lock)		_raw_spin_lock_bh(lock)
// /*AFLA*/ #define raw_spin_unlock(lock)		_raw_spin_unlock(lock)
// /*AFLA*/ #define raw_spin_unlock_irq(lock)	_raw_spin_unlock_irq(lock)
// /*AFLA*/ 
// /*AFLA*/ #define raw_spin_unlock_irqrestore(lock, flags)		\
// /*AFLA*/ 	do {							\
// /*AFLA*/ 		typecheck(unsigned long, flags);		\
// /*AFLA*/ 		_raw_spin_unlock_irqrestore(lock, flags);	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #define raw_spin_unlock_bh(lock)	_raw_spin_unlock_bh(lock)
// /*AFLA*/ 
// /*AFLA*/ #define raw_spin_trylock_bh(lock) \
// /*AFLA*/ 	__cond_lock(lock, _raw_spin_trylock_bh(lock))
// /*AFLA*/ 
// /*AFLA*/ #define raw_spin_trylock_irq(lock) \
// /*AFLA*/ ({ \
// /*AFLA*/ 	local_irq_disable(); \
// /*AFLA*/ 	raw_spin_trylock(lock) ? \
// /*AFLA*/ 	1 : ({ local_irq_enable(); 0;  }); \
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define raw_spin_trylock_irqsave(lock, flags) \
// /*AFLA*/ ({ \
// /*AFLA*/ 	local_irq_save(flags); \
// /*AFLA*/ 	raw_spin_trylock(lock) ? \
// /*AFLA*/ 	1 : ({ local_irq_restore(flags); 0; }); \
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * raw_spin_can_lock - would raw_spin_trylock() succeed?
// /*AFLA*/  * @lock: the spinlock in question.
// /*AFLA*/  */
// /*AFLA*/ #define raw_spin_can_lock(lock)	(!raw_spin_is_locked(lock))
// /*AFLA*/ 
// /*AFLA*/ /* Include rwlock functions */
// /*AFLA*/ #include <linux/rwlock.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Pull the _spin_*()/_read_*()/_write_*() functions/declarations:
// /*AFLA*/  */
// /*AFLA*/ #if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK)
// /*AFLA*/ # include <linux/spinlock_api_smp.h>
// /*AFLA*/ #else
// /*AFLA*/ # include <linux/spinlock_api_up.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Map the spin_lock functions to the raw variants for PREEMPT_RT=n
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static __always_inline raw_spinlock_t *spinlock_check(spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	return &lock->rlock;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define spin_lock_init(_lock)				\
// /*AFLA*/ do {							\
// /*AFLA*/ 	spinlock_check(_lock);				\
// /*AFLA*/ 	raw_spin_lock_init(&(_lock)->rlock);		\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void spin_lock(spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	raw_spin_lock(&lock->rlock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void spin_lock_bh(spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	raw_spin_lock_bh(&lock->rlock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline int spin_trylock(spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	return raw_spin_trylock(&lock->rlock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define spin_lock_nested(lock, subclass)			\
// /*AFLA*/ do {								\
// /*AFLA*/ 	raw_spin_lock_nested(spinlock_check(lock), subclass);	\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define spin_lock_bh_nested(lock, subclass)			\
// /*AFLA*/ do {								\
// /*AFLA*/ 	raw_spin_lock_bh_nested(spinlock_check(lock), subclass);\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define spin_lock_nest_lock(lock, nest_lock)				\
// /*AFLA*/ do {									\
// /*AFLA*/ 	raw_spin_lock_nest_lock(spinlock_check(lock), nest_lock);	\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void spin_lock_irq(spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	raw_spin_lock_irq(&lock->rlock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define spin_lock_irqsave(lock, flags)				\
// /*AFLA*/ do {								\
// /*AFLA*/ 	raw_spin_lock_irqsave(spinlock_check(lock), flags);	\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define spin_lock_irqsave_nested(lock, flags, subclass)			\
// /*AFLA*/ do {									\
// /*AFLA*/ 	raw_spin_lock_irqsave_nested(spinlock_check(lock), flags, subclass); \
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void spin_unlock(spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	raw_spin_unlock(&lock->rlock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void spin_unlock_bh(spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	raw_spin_unlock_bh(&lock->rlock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void spin_unlock_irq(spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	raw_spin_unlock_irq(&lock->rlock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ 	raw_spin_unlock_irqrestore(&lock->rlock, flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline int spin_trylock_bh(spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	return raw_spin_trylock_bh(&lock->rlock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline int spin_trylock_irq(spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	return raw_spin_trylock_irq(&lock->rlock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define spin_trylock_irqsave(lock, flags)			\
// /*AFLA*/ ({								\
// /*AFLA*/ 	raw_spin_trylock_irqsave(spinlock_check(lock), flags); \
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void spin_unlock_wait(spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	raw_spin_unlock_wait(&lock->rlock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline int spin_is_locked(spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	return raw_spin_is_locked(&lock->rlock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline int spin_is_contended(spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	return raw_spin_is_contended(&lock->rlock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline int spin_can_lock(spinlock_t *lock)
// /*AFLA*/ {
// /*AFLA*/ 	return raw_spin_can_lock(&lock->rlock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define assert_spin_locked(lock)	assert_raw_spin_locked(&(lock)->rlock)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Pull the atomic_t declaration:
// /*AFLA*/  * (asm-mips/atomic.h needs above definitions)
// /*AFLA*/  */
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ /**
// /*AFLA*/  * atomic_dec_and_lock - lock on reaching reference count zero
// /*AFLA*/  * @atomic: the atomic counter
// /*AFLA*/  * @lock: the spinlock in question
// /*AFLA*/  *
// /*AFLA*/  * Decrements @atomic by 1.  If the result is 0, returns true and locks
// /*AFLA*/  * @lock.  Returns false for all other cases.
// /*AFLA*/  */
// /*AFLA*/ extern int _atomic_dec_and_lock(atomic_t *atomic, spinlock_t *lock);
// /*AFLA*/ #define atomic_dec_and_lock(atomic, lock) \
// /*AFLA*/ 		__cond_lock(lock, _atomic_dec_and_lock(atomic, lock))
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_SPINLOCK_H */
