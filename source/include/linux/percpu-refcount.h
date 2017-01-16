// /*AFLA*/ /*
// /*AFLA*/  * Percpu refcounts:
// /*AFLA*/  * (C) 2012 Google, Inc.
// /*AFLA*/  * Author: Kent Overstreet <koverstreet@google.com>
// /*AFLA*/  *
// /*AFLA*/  * This implements a refcount with similar semantics to atomic_t - atomic_inc(),
// /*AFLA*/  * atomic_dec_and_test() - but percpu.
// /*AFLA*/  *
// /*AFLA*/  * There's one important difference between percpu refs and normal atomic_t
// /*AFLA*/  * refcounts; you have to keep track of your initial refcount, and then when you
// /*AFLA*/  * start shutting down you call percpu_ref_kill() _before_ dropping the initial
// /*AFLA*/  * refcount.
// /*AFLA*/  *
// /*AFLA*/  * The refcount will have a range of 0 to ((1U << 31) - 1), i.e. one bit less
// /*AFLA*/  * than an atomic_t - this is because of the way shutdown works, see
// /*AFLA*/  * percpu_ref_kill()/PERCPU_COUNT_BIAS.
// /*AFLA*/  *
// /*AFLA*/  * Before you call percpu_ref_kill(), percpu_ref_put() does not check for the
// /*AFLA*/  * refcount hitting 0 - it can't, if it was in percpu mode. percpu_ref_kill()
// /*AFLA*/  * puts the ref back in single atomic_t mode, collecting the per cpu refs and
// /*AFLA*/  * issuing the appropriate barriers, and then marks the ref as shutting down so
// /*AFLA*/  * that percpu_ref_put() will check for the ref hitting 0.  After it returns,
// /*AFLA*/  * it's safe to drop the initial ref.
// /*AFLA*/  *
// /*AFLA*/  * USAGE:
// /*AFLA*/  *
// /*AFLA*/  * See fs/aio.c for some example usage; it's used there for struct kioctx, which
// /*AFLA*/  * is created when userspaces calls io_setup(), and destroyed when userspace
// /*AFLA*/  * calls io_destroy() or the process exits.
// /*AFLA*/  *
// /*AFLA*/  * In the aio code, kill_ioctx() is called when we wish to destroy a kioctx; it
// /*AFLA*/  * calls percpu_ref_kill(), then hlist_del_rcu() and synchronize_rcu() to remove
// /*AFLA*/  * the kioctx from the proccess's list of kioctxs - after that, there can't be
// /*AFLA*/  * any new users of the kioctx (from lookup_ioctx()) and it's then safe to drop
// /*AFLA*/  * the initial ref with percpu_ref_put().
// /*AFLA*/  *
// /*AFLA*/  * Code that does a two stage shutdown like this often needs some kind of
// /*AFLA*/  * explicit synchronization to ensure the initial refcount can only be dropped
// /*AFLA*/  * once - percpu_ref_kill() does this for you, it returns true once and false if
// /*AFLA*/  * someone else already called it. The aio code uses it this way, but it's not
// /*AFLA*/  * necessary if the code has some other mechanism to synchronize teardown.
// /*AFLA*/  * around.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_PERCPU_REFCOUNT_H
// /*AFLA*/ #define _LINUX_PERCPU_REFCOUNT_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/percpu.h>
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ #include <linux/gfp.h>
// /*AFLA*/ 
// /*AFLA*/ struct percpu_ref;
// /*AFLA*/ typedef void (percpu_ref_func_t)(struct percpu_ref *);
// /*AFLA*/ 
// /*AFLA*/ /* flags set in the lower bits of percpu_ref->percpu_count_ptr */
// /*AFLA*/ enum {
// /*AFLA*/ 	__PERCPU_REF_ATOMIC	= 1LU << 0,	/* operating in atomic mode */
// /*AFLA*/ 	__PERCPU_REF_DEAD	= 1LU << 1,	/* (being) killed */
// /*AFLA*/ 	__PERCPU_REF_ATOMIC_DEAD = __PERCPU_REF_ATOMIC | __PERCPU_REF_DEAD,
// /*AFLA*/ 
// /*AFLA*/ 	__PERCPU_REF_FLAG_BITS	= 2,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* @flags for percpu_ref_init() */
// /*AFLA*/ enum {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Start w/ ref == 1 in atomic mode.  Can be switched to percpu
// /*AFLA*/ 	 * operation using percpu_ref_switch_to_percpu().  If initialized
// /*AFLA*/ 	 * with this flag, the ref will stay in atomic mode until
// /*AFLA*/ 	 * percpu_ref_switch_to_percpu() is invoked on it.
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERCPU_REF_INIT_ATOMIC	= 1 << 0,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Start dead w/ ref == 0 in atomic mode.  Must be revived with
// /*AFLA*/ 	 * percpu_ref_reinit() before used.  Implies INIT_ATOMIC.
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERCPU_REF_INIT_DEAD	= 1 << 1,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct percpu_ref {
// /*AFLA*/ 	atomic_long_t		count;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The low bit of the pointer indicates whether the ref is in percpu
// /*AFLA*/ 	 * mode; if set, then get/put will manipulate the atomic_t.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long		percpu_count_ptr;
// /*AFLA*/ 	percpu_ref_func_t	*release;
// /*AFLA*/ 	percpu_ref_func_t	*confirm_switch;
// /*AFLA*/ 	bool			force_atomic:1;
// /*AFLA*/ 	struct rcu_head		rcu;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ int __must_check percpu_ref_init(struct percpu_ref *ref,
// /*AFLA*/ 				 percpu_ref_func_t *release, unsigned int flags,
// /*AFLA*/ 				 gfp_t gfp);
// /*AFLA*/ void percpu_ref_exit(struct percpu_ref *ref);
// /*AFLA*/ void percpu_ref_switch_to_atomic(struct percpu_ref *ref,
// /*AFLA*/ 				 percpu_ref_func_t *confirm_switch);
// /*AFLA*/ void percpu_ref_switch_to_percpu(struct percpu_ref *ref);
// /*AFLA*/ void percpu_ref_kill_and_confirm(struct percpu_ref *ref,
// /*AFLA*/ 				 percpu_ref_func_t *confirm_kill);
// /*AFLA*/ void percpu_ref_reinit(struct percpu_ref *ref);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * percpu_ref_kill - drop the initial ref
// /*AFLA*/  * @ref: percpu_ref to kill
// /*AFLA*/  *
// /*AFLA*/  * Must be used to drop the initial ref on a percpu refcount; must be called
// /*AFLA*/  * precisely once before shutdown.
// /*AFLA*/  *
// /*AFLA*/  * Puts @ref in non percpu mode, then does a call_rcu() before gathering up the
// /*AFLA*/  * percpu counters and dropping the initial ref.
// /*AFLA*/  */
// /*AFLA*/ static inline void percpu_ref_kill(struct percpu_ref *ref)
// /*AFLA*/ {
// /*AFLA*/ 	percpu_ref_kill_and_confirm(ref, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Internal helper.  Don't use outside percpu-refcount proper.  The
// /*AFLA*/  * function doesn't return the pointer and let the caller test it for NULL
// /*AFLA*/  * because doing so forces the compiler to generate two conditional
// /*AFLA*/  * branches as it can't assume that @ref->percpu_count is not NULL.
// /*AFLA*/  */
// /*AFLA*/ static inline bool __ref_is_percpu(struct percpu_ref *ref,
// /*AFLA*/ 					  unsigned long __percpu **percpu_countp)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long percpu_ptr;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The value of @ref->percpu_count_ptr is tested for
// /*AFLA*/ 	 * !__PERCPU_REF_ATOMIC, which may be set asynchronously, and then
// /*AFLA*/ 	 * used as a pointer.  If the compiler generates a separate fetch
// /*AFLA*/ 	 * when using it as a pointer, __PERCPU_REF_ATOMIC may be set in
// /*AFLA*/ 	 * between contaminating the pointer value, meaning that
// /*AFLA*/ 	 * READ_ONCE() is required when fetching it.
// /*AFLA*/ 	 */
// /*AFLA*/ 	percpu_ptr = READ_ONCE(ref->percpu_count_ptr);
// /*AFLA*/ 
// /*AFLA*/ 	/* paired with smp_store_release() in __percpu_ref_switch_to_percpu() */
// /*AFLA*/ 	smp_read_barrier_depends();
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Theoretically, the following could test just ATOMIC; however,
// /*AFLA*/ 	 * then we'd have to mask off DEAD separately as DEAD may be
// /*AFLA*/ 	 * visible without ATOMIC if we race with percpu_ref_kill().  DEAD
// /*AFLA*/ 	 * implies ATOMIC anyway.  Test them together.
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (unlikely(percpu_ptr & __PERCPU_REF_ATOMIC_DEAD))
// /*AFLA*/ 		return false;
// /*AFLA*/ 
// /*AFLA*/ 	*percpu_countp = (unsigned long __percpu *)percpu_ptr;
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * percpu_ref_get_many - increment a percpu refcount
// /*AFLA*/  * @ref: percpu_ref to get
// /*AFLA*/  * @nr: number of references to get
// /*AFLA*/  *
// /*AFLA*/  * Analogous to atomic_long_add().
// /*AFLA*/  *
// /*AFLA*/  * This function is safe to call as long as @ref is between init and exit.
// /*AFLA*/  */
// /*AFLA*/ static inline void percpu_ref_get_many(struct percpu_ref *ref, unsigned long nr)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long __percpu *percpu_count;
// /*AFLA*/ 
// /*AFLA*/ 	rcu_read_lock_sched();
// /*AFLA*/ 
// /*AFLA*/ 	if (__ref_is_percpu(ref, &percpu_count))
// /*AFLA*/ 		this_cpu_add(*percpu_count, nr);
// /*AFLA*/ 	else
// /*AFLA*/ 		atomic_long_add(nr, &ref->count);
// /*AFLA*/ 
// /*AFLA*/ 	rcu_read_unlock_sched();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * percpu_ref_get - increment a percpu refcount
// /*AFLA*/  * @ref: percpu_ref to get
// /*AFLA*/  *
// /*AFLA*/  * Analagous to atomic_long_inc().
// /*AFLA*/  *
// /*AFLA*/  * This function is safe to call as long as @ref is between init and exit.
// /*AFLA*/  */
// /*AFLA*/ static inline void percpu_ref_get(struct percpu_ref *ref)
// /*AFLA*/ {
// /*AFLA*/ 	percpu_ref_get_many(ref, 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * percpu_ref_tryget - try to increment a percpu refcount
// /*AFLA*/  * @ref: percpu_ref to try-get
// /*AFLA*/  *
// /*AFLA*/  * Increment a percpu refcount unless its count already reached zero.
// /*AFLA*/  * Returns %true on success; %false on failure.
// /*AFLA*/  *
// /*AFLA*/  * This function is safe to call as long as @ref is between init and exit.
// /*AFLA*/  */
// /*AFLA*/ static inline bool percpu_ref_tryget(struct percpu_ref *ref)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long __percpu *percpu_count;
// /*AFLA*/ 	int ret;
// /*AFLA*/ 
// /*AFLA*/ 	rcu_read_lock_sched();
// /*AFLA*/ 
// /*AFLA*/ 	if (__ref_is_percpu(ref, &percpu_count)) {
// /*AFLA*/ 		this_cpu_inc(*percpu_count);
// /*AFLA*/ 		ret = true;
// /*AFLA*/ 	} else {
// /*AFLA*/ 		ret = atomic_long_inc_not_zero(&ref->count);
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	rcu_read_unlock_sched();
// /*AFLA*/ 
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * percpu_ref_tryget_live - try to increment a live percpu refcount
// /*AFLA*/  * @ref: percpu_ref to try-get
// /*AFLA*/  *
// /*AFLA*/  * Increment a percpu refcount unless it has already been killed.  Returns
// /*AFLA*/  * %true on success; %false on failure.
// /*AFLA*/  *
// /*AFLA*/  * Completion of percpu_ref_kill() in itself doesn't guarantee that this
// /*AFLA*/  * function will fail.  For such guarantee, percpu_ref_kill_and_confirm()
// /*AFLA*/  * should be used.  After the confirm_kill callback is invoked, it's
// /*AFLA*/  * guaranteed that no new reference will be given out by
// /*AFLA*/  * percpu_ref_tryget_live().
// /*AFLA*/  *
// /*AFLA*/  * This function is safe to call as long as @ref is between init and exit.
// /*AFLA*/  */
// /*AFLA*/ static inline bool percpu_ref_tryget_live(struct percpu_ref *ref)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long __percpu *percpu_count;
// /*AFLA*/ 	int ret = false;
// /*AFLA*/ 
// /*AFLA*/ 	rcu_read_lock_sched();
// /*AFLA*/ 
// /*AFLA*/ 	if (__ref_is_percpu(ref, &percpu_count)) {
// /*AFLA*/ 		this_cpu_inc(*percpu_count);
// /*AFLA*/ 		ret = true;
// /*AFLA*/ 	} else if (!(ref->percpu_count_ptr & __PERCPU_REF_DEAD)) {
// /*AFLA*/ 		ret = atomic_long_inc_not_zero(&ref->count);
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	rcu_read_unlock_sched();
// /*AFLA*/ 
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * percpu_ref_put_many - decrement a percpu refcount
// /*AFLA*/  * @ref: percpu_ref to put
// /*AFLA*/  * @nr: number of references to put
// /*AFLA*/  *
// /*AFLA*/  * Decrement the refcount, and if 0, call the release function (which was passed
// /*AFLA*/  * to percpu_ref_init())
// /*AFLA*/  *
// /*AFLA*/  * This function is safe to call as long as @ref is between init and exit.
// /*AFLA*/  */
// /*AFLA*/ static inline void percpu_ref_put_many(struct percpu_ref *ref, unsigned long nr)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long __percpu *percpu_count;
// /*AFLA*/ 
// /*AFLA*/ 	rcu_read_lock_sched();
// /*AFLA*/ 
// /*AFLA*/ 	if (__ref_is_percpu(ref, &percpu_count))
// /*AFLA*/ 		this_cpu_sub(*percpu_count, nr);
// /*AFLA*/ 	else if (unlikely(atomic_long_sub_and_test(nr, &ref->count)))
// /*AFLA*/ 		ref->release(ref);
// /*AFLA*/ 
// /*AFLA*/ 	rcu_read_unlock_sched();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * percpu_ref_put - decrement a percpu refcount
// /*AFLA*/  * @ref: percpu_ref to put
// /*AFLA*/  *
// /*AFLA*/  * Decrement the refcount, and if 0, call the release function (which was passed
// /*AFLA*/  * to percpu_ref_init())
// /*AFLA*/  *
// /*AFLA*/  * This function is safe to call as long as @ref is between init and exit.
// /*AFLA*/  */
// /*AFLA*/ static inline void percpu_ref_put(struct percpu_ref *ref)
// /*AFLA*/ {
// /*AFLA*/ 	percpu_ref_put_many(ref, 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * percpu_ref_is_dying - test whether a percpu refcount is dying or dead
// /*AFLA*/  * @ref: percpu_ref to test
// /*AFLA*/  *
// /*AFLA*/  * Returns %true if @ref is dying or dead.
// /*AFLA*/  *
// /*AFLA*/  * This function is safe to call as long as @ref is between init and exit
// /*AFLA*/  * and the caller is responsible for synchronizing against state changes.
// /*AFLA*/  */
// /*AFLA*/ static inline bool percpu_ref_is_dying(struct percpu_ref *ref)
// /*AFLA*/ {
// /*AFLA*/ 	return ref->percpu_count_ptr & __PERCPU_REF_DEAD;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * percpu_ref_is_zero - test whether a percpu refcount reached zero
// /*AFLA*/  * @ref: percpu_ref to test
// /*AFLA*/  *
// /*AFLA*/  * Returns %true if @ref reached zero.
// /*AFLA*/  *
// /*AFLA*/  * This function is safe to call as long as @ref is between init and exit.
// /*AFLA*/  */
// /*AFLA*/ static inline bool percpu_ref_is_zero(struct percpu_ref *ref)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long __percpu *percpu_count;
// /*AFLA*/ 
// /*AFLA*/ 	if (__ref_is_percpu(ref, &percpu_count))
// /*AFLA*/ 		return false;
// /*AFLA*/ 	return !atomic_long_read(&ref->count);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
