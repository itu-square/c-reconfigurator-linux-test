// /*AFLA*/ #ifndef __LINUX_SEQLOCK_H
// /*AFLA*/ #define __LINUX_SEQLOCK_H
// /*AFLA*/ /*
// /*AFLA*/  * Reader/writer consistent mechanism without starving writers. This type of
// /*AFLA*/  * lock for data where the reader wants a consistent set of information
// /*AFLA*/  * and is willing to retry if the information changes. There are two types
// /*AFLA*/  * of readers:
// /*AFLA*/  * 1. Sequence readers which never block a writer but they may have to retry
// /*AFLA*/  *    if a writer is in progress by detecting change in sequence number.
// /*AFLA*/  *    Writers do not wait for a sequence reader.
// /*AFLA*/  * 2. Locking readers which will wait if a writer or another locking reader
// /*AFLA*/  *    is in progress. A locking reader in progress will also block a writer
// /*AFLA*/  *    from going forward. Unlike the regular rwlock, the read lock here is
// /*AFLA*/  *    exclusive so that only one locking reader can get it.
// /*AFLA*/  *
// /*AFLA*/  * This is not as cache friendly as brlock. Also, this may not work well
// /*AFLA*/  * for data that contains pointers, because any writer could
// /*AFLA*/  * invalidate a pointer that a reader was following.
// /*AFLA*/  *
// /*AFLA*/  * Expected non-blocking reader usage:
// /*AFLA*/  * 	do {
// /*AFLA*/  *	    seq = read_seqbegin(&foo);
// /*AFLA*/  * 	...
// /*AFLA*/  *      } while (read_seqretry(&foo, seq));
// /*AFLA*/  *
// /*AFLA*/  *
// /*AFLA*/  * On non-SMP the spin locks disappear but the writer still needs
// /*AFLA*/  * to increment the sequence variables because an interrupt routine could
// /*AFLA*/  * change the state of the data.
// /*AFLA*/  *
// /*AFLA*/  * Based on x86_64 vsyscall gettimeofday 
// /*AFLA*/  * by Keith Owens and Andrea Arcangeli
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/preempt.h>
// /*AFLA*/ #include <linux/lockdep.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <asm/processor.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Version using sequence counter only.
// /*AFLA*/  * This can be used when code has its own mutex protecting the
// /*AFLA*/  * updating starting before the write_seqcountbeqin() and ending
// /*AFLA*/  * after the write_seqcount_end().
// /*AFLA*/  */
// /*AFLA*/ typedef struct seqcount {
// /*AFLA*/ 	unsigned sequence;
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ 	struct lockdep_map dep_map;
// /*AFLA*/ #endif
// /*AFLA*/ } seqcount_t;
// /*AFLA*/ 
// /*AFLA*/ static inline void __seqcount_init(seqcount_t *s, const char *name,
// /*AFLA*/ 					  struct lock_class_key *key)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Make sure we are not reinitializing a held lock:
// /*AFLA*/ 	 */
// /*AFLA*/ 	lockdep_init_map(&s->dep_map, name, key, 0);
// /*AFLA*/ 	s->sequence = 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ # define SEQCOUNT_DEP_MAP_INIT(lockname) \
// /*AFLA*/ 		.dep_map = { .name = #lockname } \
// /*AFLA*/ 
// /*AFLA*/ # define seqcount_init(s)				\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		static struct lock_class_key __key;	\
// /*AFLA*/ 		__seqcount_init((s), #s, &__key);	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ static inline void seqcount_lockdep_reader_access(const seqcount_t *s)
// /*AFLA*/ {
// /*AFLA*/ 	seqcount_t *l = (seqcount_t *)s;
// /*AFLA*/ 	unsigned long flags;
// /*AFLA*/ 
// /*AFLA*/ 	local_irq_save(flags);
// /*AFLA*/ 	seqcount_acquire_read(&l->dep_map, 0, 0, _RET_IP_);
// /*AFLA*/ 	seqcount_release(&l->dep_map, 1, _RET_IP_);
// /*AFLA*/ 	local_irq_restore(flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ # define SEQCOUNT_DEP_MAP_INIT(lockname)
// /*AFLA*/ # define seqcount_init(s) __seqcount_init(s, NULL, NULL)
// /*AFLA*/ # define seqcount_lockdep_reader_access(x)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define SEQCNT_ZERO(lockname) { .sequence = 0, SEQCOUNT_DEP_MAP_INIT(lockname)}
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __read_seqcount_begin - begin a seq-read critical section (without barrier)
// /*AFLA*/  * @s: pointer to seqcount_t
// /*AFLA*/  * Returns: count to be passed to read_seqcount_retry
// /*AFLA*/  *
// /*AFLA*/  * __read_seqcount_begin is like read_seqcount_begin, but has no smp_rmb()
// /*AFLA*/  * barrier. Callers should ensure that smp_rmb() or equivalent ordering is
// /*AFLA*/  * provided before actually loading any of the variables that are to be
// /*AFLA*/  * protected in this critical section.
// /*AFLA*/  *
// /*AFLA*/  * Use carefully, only in critical code, and comment how the barrier is
// /*AFLA*/  * provided.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned __read_seqcount_begin(const seqcount_t *s)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned ret;
// /*AFLA*/ 
// /*AFLA*/ repeat:
// /*AFLA*/ 	ret = READ_ONCE(s->sequence);
// /*AFLA*/ 	if (unlikely(ret & 1)) {
// /*AFLA*/ 		cpu_relax();
// /*AFLA*/ 		goto repeat;
// /*AFLA*/ 	}
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * raw_read_seqcount - Read the raw seqcount
// /*AFLA*/  * @s: pointer to seqcount_t
// /*AFLA*/  * Returns: count to be passed to read_seqcount_retry
// /*AFLA*/  *
// /*AFLA*/  * raw_read_seqcount opens a read critical section of the given
// /*AFLA*/  * seqcount without any lockdep checking and without checking or
// /*AFLA*/  * masking the LSB. Calling code is responsible for handling that.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned raw_read_seqcount(const seqcount_t *s)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned ret = READ_ONCE(s->sequence);
// /*AFLA*/ 	smp_rmb();
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * raw_read_seqcount_begin - start seq-read critical section w/o lockdep
// /*AFLA*/  * @s: pointer to seqcount_t
// /*AFLA*/  * Returns: count to be passed to read_seqcount_retry
// /*AFLA*/  *
// /*AFLA*/  * raw_read_seqcount_begin opens a read critical section of the given
// /*AFLA*/  * seqcount, but without any lockdep checking. Validity of the critical
// /*AFLA*/  * section is tested by checking read_seqcount_retry function.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned raw_read_seqcount_begin(const seqcount_t *s)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned ret = __read_seqcount_begin(s);
// /*AFLA*/ 	smp_rmb();
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * read_seqcount_begin - begin a seq-read critical section
// /*AFLA*/  * @s: pointer to seqcount_t
// /*AFLA*/  * Returns: count to be passed to read_seqcount_retry
// /*AFLA*/  *
// /*AFLA*/  * read_seqcount_begin opens a read critical section of the given seqcount.
// /*AFLA*/  * Validity of the critical section is tested by checking read_seqcount_retry
// /*AFLA*/  * function.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned read_seqcount_begin(const seqcount_t *s)
// /*AFLA*/ {
// /*AFLA*/ 	seqcount_lockdep_reader_access(s);
// /*AFLA*/ 	return raw_read_seqcount_begin(s);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * raw_seqcount_begin - begin a seq-read critical section
// /*AFLA*/  * @s: pointer to seqcount_t
// /*AFLA*/  * Returns: count to be passed to read_seqcount_retry
// /*AFLA*/  *
// /*AFLA*/  * raw_seqcount_begin opens a read critical section of the given seqcount.
// /*AFLA*/  * Validity of the critical section is tested by checking read_seqcount_retry
// /*AFLA*/  * function.
// /*AFLA*/  *
// /*AFLA*/  * Unlike read_seqcount_begin(), this function will not wait for the count
// /*AFLA*/  * to stabilize. If a writer is active when we begin, we will fail the
// /*AFLA*/  * read_seqcount_retry() instead of stabilizing at the beginning of the
// /*AFLA*/  * critical section.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned raw_seqcount_begin(const seqcount_t *s)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned ret = READ_ONCE(s->sequence);
// /*AFLA*/ 	smp_rmb();
// /*AFLA*/ 	return ret & ~1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __read_seqcount_retry - end a seq-read critical section (without barrier)
// /*AFLA*/  * @s: pointer to seqcount_t
// /*AFLA*/  * @start: count, from read_seqcount_begin
// /*AFLA*/  * Returns: 1 if retry is required, else 0
// /*AFLA*/  *
// /*AFLA*/  * __read_seqcount_retry is like read_seqcount_retry, but has no smp_rmb()
// /*AFLA*/  * barrier. Callers should ensure that smp_rmb() or equivalent ordering is
// /*AFLA*/  * provided before actually loading any of the variables that are to be
// /*AFLA*/  * protected in this critical section.
// /*AFLA*/  *
// /*AFLA*/  * Use carefully, only in critical code, and comment how the barrier is
// /*AFLA*/  * provided.
// /*AFLA*/  */
// /*AFLA*/ static inline int __read_seqcount_retry(const seqcount_t *s, unsigned start)
// /*AFLA*/ {
// /*AFLA*/ 	return unlikely(s->sequence != start);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * read_seqcount_retry - end a seq-read critical section
// /*AFLA*/  * @s: pointer to seqcount_t
// /*AFLA*/  * @start: count, from read_seqcount_begin
// /*AFLA*/  * Returns: 1 if retry is required, else 0
// /*AFLA*/  *
// /*AFLA*/  * read_seqcount_retry closes a read critical section of the given seqcount.
// /*AFLA*/  * If the critical section was invalid, it must be ignored (and typically
// /*AFLA*/  * retried).
// /*AFLA*/  */
// /*AFLA*/ static inline int read_seqcount_retry(const seqcount_t *s, unsigned start)
// /*AFLA*/ {
// /*AFLA*/ 	smp_rmb();
// /*AFLA*/ 	return __read_seqcount_retry(s, start);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ static inline void raw_write_seqcount_begin(seqcount_t *s)
// /*AFLA*/ {
// /*AFLA*/ 	s->sequence++;
// /*AFLA*/ 	smp_wmb();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void raw_write_seqcount_end(seqcount_t *s)
// /*AFLA*/ {
// /*AFLA*/ 	smp_wmb();
// /*AFLA*/ 	s->sequence++;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * raw_write_seqcount_barrier - do a seq write barrier
// /*AFLA*/  * @s: pointer to seqcount_t
// /*AFLA*/  *
// /*AFLA*/  * This can be used to provide an ordering guarantee instead of the
// /*AFLA*/  * usual consistency guarantee. It is one wmb cheaper, because we can
// /*AFLA*/  * collapse the two back-to-back wmb()s.
// /*AFLA*/  *
// /*AFLA*/  *      seqcount_t seq;
// /*AFLA*/  *      bool X = true, Y = false;
// /*AFLA*/  *
// /*AFLA*/  *      void read(void)
// /*AFLA*/  *      {
// /*AFLA*/  *              bool x, y;
// /*AFLA*/  *
// /*AFLA*/  *              do {
// /*AFLA*/  *                      int s = read_seqcount_begin(&seq);
// /*AFLA*/  *
// /*AFLA*/  *                      x = X; y = Y;
// /*AFLA*/  *
// /*AFLA*/  *              } while (read_seqcount_retry(&seq, s));
// /*AFLA*/  *
// /*AFLA*/  *              BUG_ON(!x && !y);
// /*AFLA*/  *      }
// /*AFLA*/  *
// /*AFLA*/  *      void write(void)
// /*AFLA*/  *      {
// /*AFLA*/  *              Y = true;
// /*AFLA*/  *
// /*AFLA*/  *              raw_write_seqcount_barrier(seq);
// /*AFLA*/  *
// /*AFLA*/  *              X = false;
// /*AFLA*/  *      }
// /*AFLA*/  */
// /*AFLA*/ static inline void raw_write_seqcount_barrier(seqcount_t *s)
// /*AFLA*/ {
// /*AFLA*/ 	s->sequence++;
// /*AFLA*/ 	smp_wmb();
// /*AFLA*/ 	s->sequence++;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int raw_read_seqcount_latch(seqcount_t *s)
// /*AFLA*/ {
// /*AFLA*/ 	int seq = READ_ONCE(s->sequence);
// /*AFLA*/ 	/* Pairs with the first smp_wmb() in raw_write_seqcount_latch() */
// /*AFLA*/ 	smp_read_barrier_depends();
// /*AFLA*/ 	return seq;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * raw_write_seqcount_latch - redirect readers to even/odd copy
// /*AFLA*/  * @s: pointer to seqcount_t
// /*AFLA*/  *
// /*AFLA*/  * The latch technique is a multiversion concurrency control method that allows
// /*AFLA*/  * queries during non-atomic modifications. If you can guarantee queries never
// /*AFLA*/  * interrupt the modification -- e.g. the concurrency is strictly between CPUs
// /*AFLA*/  * -- you most likely do not need this.
// /*AFLA*/  *
// /*AFLA*/  * Where the traditional RCU/lockless data structures rely on atomic
// /*AFLA*/  * modifications to ensure queries observe either the old or the new state the
// /*AFLA*/  * latch allows the same for non-atomic updates. The trade-off is doubling the
// /*AFLA*/  * cost of storage; we have to maintain two copies of the entire data
// /*AFLA*/  * structure.
// /*AFLA*/  *
// /*AFLA*/  * Very simply put: we first modify one copy and then the other. This ensures
// /*AFLA*/  * there is always one copy in a stable state, ready to give us an answer.
// /*AFLA*/  *
// /*AFLA*/  * The basic form is a data structure like:
// /*AFLA*/  *
// /*AFLA*/  * struct latch_struct {
// /*AFLA*/  *	seqcount_t		seq;
// /*AFLA*/  *	struct data_struct	data[2];
// /*AFLA*/  * };
// /*AFLA*/  *
// /*AFLA*/  * Where a modification, which is assumed to be externally serialized, does the
// /*AFLA*/  * following:
// /*AFLA*/  *
// /*AFLA*/  * void latch_modify(struct latch_struct *latch, ...)
// /*AFLA*/  * {
// /*AFLA*/  *	smp_wmb();	<- Ensure that the last data[1] update is visible
// /*AFLA*/  *	latch->seq++;
// /*AFLA*/  *	smp_wmb();	<- Ensure that the seqcount update is visible
// /*AFLA*/  *
// /*AFLA*/  *	modify(latch->data[0], ...);
// /*AFLA*/  *
// /*AFLA*/  *	smp_wmb();	<- Ensure that the data[0] update is visible
// /*AFLA*/  *	latch->seq++;
// /*AFLA*/  *	smp_wmb();	<- Ensure that the seqcount update is visible
// /*AFLA*/  *
// /*AFLA*/  *	modify(latch->data[1], ...);
// /*AFLA*/  * }
// /*AFLA*/  *
// /*AFLA*/  * The query will have a form like:
// /*AFLA*/  *
// /*AFLA*/  * struct entry *latch_query(struct latch_struct *latch, ...)
// /*AFLA*/  * {
// /*AFLA*/  *	struct entry *entry;
// /*AFLA*/  *	unsigned seq, idx;
// /*AFLA*/  *
// /*AFLA*/  *	do {
// /*AFLA*/  *		seq = raw_read_seqcount_latch(&latch->seq);
// /*AFLA*/  *
// /*AFLA*/  *		idx = seq & 0x01;
// /*AFLA*/  *		entry = data_query(latch->data[idx], ...);
// /*AFLA*/  *
// /*AFLA*/  *		smp_rmb();
// /*AFLA*/  *	} while (seq != latch->seq);
// /*AFLA*/  *
// /*AFLA*/  *	return entry;
// /*AFLA*/  * }
// /*AFLA*/  *
// /*AFLA*/  * So during the modification, queries are first redirected to data[1]. Then we
// /*AFLA*/  * modify data[0]. When that is complete, we redirect queries back to data[0]
// /*AFLA*/  * and we can modify data[1].
// /*AFLA*/  *
// /*AFLA*/  * NOTE: The non-requirement for atomic modifications does _NOT_ include
// /*AFLA*/  *       the publishing of new entries in the case where data is a dynamic
// /*AFLA*/  *       data structure.
// /*AFLA*/  *
// /*AFLA*/  *       An iteration might start in data[0] and get suspended long enough
// /*AFLA*/  *       to miss an entire modification sequence, once it resumes it might
// /*AFLA*/  *       observe the new entry.
// /*AFLA*/  *
// /*AFLA*/  * NOTE: When data is a dynamic data structure; one should use regular RCU
// /*AFLA*/  *       patterns to manage the lifetimes of the objects within.
// /*AFLA*/  */
// /*AFLA*/ static inline void raw_write_seqcount_latch(seqcount_t *s)
// /*AFLA*/ {
// /*AFLA*/        smp_wmb();      /* prior stores before incrementing "sequence" */
// /*AFLA*/        s->sequence++;
// /*AFLA*/        smp_wmb();      /* increment "sequence" before following stores */
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Sequence counter only version assumes that callers are using their
// /*AFLA*/  * own mutexing.
// /*AFLA*/  */
// /*AFLA*/ static inline void write_seqcount_begin_nested(seqcount_t *s, int subclass)
// /*AFLA*/ {
// /*AFLA*/ 	raw_write_seqcount_begin(s);
// /*AFLA*/ 	seqcount_acquire(&s->dep_map, subclass, 0, _RET_IP_);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_seqcount_begin(seqcount_t *s)
// /*AFLA*/ {
// /*AFLA*/ 	write_seqcount_begin_nested(s, 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_seqcount_end(seqcount_t *s)
// /*AFLA*/ {
// /*AFLA*/ 	seqcount_release(&s->dep_map, 1, _RET_IP_);
// /*AFLA*/ 	raw_write_seqcount_end(s);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * write_seqcount_invalidate - invalidate in-progress read-side seq operations
// /*AFLA*/  * @s: pointer to seqcount_t
// /*AFLA*/  *
// /*AFLA*/  * After write_seqcount_invalidate, no read-side seq operations will complete
// /*AFLA*/  * successfully and see data older than this.
// /*AFLA*/  */
// /*AFLA*/ static inline void write_seqcount_invalidate(seqcount_t *s)
// /*AFLA*/ {
// /*AFLA*/ 	smp_wmb();
// /*AFLA*/ 	s->sequence+=2;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	struct seqcount seqcount;
// /*AFLA*/ 	spinlock_t lock;
// /*AFLA*/ } seqlock_t;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These macros triggered gcc-3.x compile-time problems.  We think these are
// /*AFLA*/  * OK now.  Be cautious.
// /*AFLA*/  */
// /*AFLA*/ #define __SEQLOCK_UNLOCKED(lockname)			\
// /*AFLA*/ 	{						\
// /*AFLA*/ 		.seqcount = SEQCNT_ZERO(lockname),	\
// /*AFLA*/ 		.lock =	__SPIN_LOCK_UNLOCKED(lockname)	\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ #define seqlock_init(x)					\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		seqcount_init(&(x)->seqcount);		\
// /*AFLA*/ 		spin_lock_init(&(x)->lock);		\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_SEQLOCK(x) \
// /*AFLA*/ 		seqlock_t x = __SEQLOCK_UNLOCKED(x)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Read side functions for starting and finalizing a read side section.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned read_seqbegin(const seqlock_t *sl)
// /*AFLA*/ {
// /*AFLA*/ 	return read_seqcount_begin(&sl->seqcount);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned read_seqretry(const seqlock_t *sl, unsigned start)
// /*AFLA*/ {
// /*AFLA*/ 	return read_seqcount_retry(&sl->seqcount, start);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Lock out other writers and update the count.
// /*AFLA*/  * Acts like a normal spin_lock/unlock.
// /*AFLA*/  * Don't need preempt_disable() because that is in the spin_lock already.
// /*AFLA*/  */
// /*AFLA*/ static inline void write_seqlock(seqlock_t *sl)
// /*AFLA*/ {
// /*AFLA*/ 	spin_lock(&sl->lock);
// /*AFLA*/ 	write_seqcount_begin(&sl->seqcount);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_sequnlock(seqlock_t *sl)
// /*AFLA*/ {
// /*AFLA*/ 	write_seqcount_end(&sl->seqcount);
// /*AFLA*/ 	spin_unlock(&sl->lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_seqlock_bh(seqlock_t *sl)
// /*AFLA*/ {
// /*AFLA*/ 	spin_lock_bh(&sl->lock);
// /*AFLA*/ 	write_seqcount_begin(&sl->seqcount);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_sequnlock_bh(seqlock_t *sl)
// /*AFLA*/ {
// /*AFLA*/ 	write_seqcount_end(&sl->seqcount);
// /*AFLA*/ 	spin_unlock_bh(&sl->lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_seqlock_irq(seqlock_t *sl)
// /*AFLA*/ {
// /*AFLA*/ 	spin_lock_irq(&sl->lock);
// /*AFLA*/ 	write_seqcount_begin(&sl->seqcount);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_sequnlock_irq(seqlock_t *sl)
// /*AFLA*/ {
// /*AFLA*/ 	write_seqcount_end(&sl->seqcount);
// /*AFLA*/ 	spin_unlock_irq(&sl->lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long __write_seqlock_irqsave(seqlock_t *sl)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long flags;
// /*AFLA*/ 
// /*AFLA*/ 	spin_lock_irqsave(&sl->lock, flags);
// /*AFLA*/ 	write_seqcount_begin(&sl->seqcount);
// /*AFLA*/ 	return flags;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define write_seqlock_irqsave(lock, flags)				\
// /*AFLA*/ 	do { flags = __write_seqlock_irqsave(lock); } while (0)
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ write_sequnlock_irqrestore(seqlock_t *sl, unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ 	write_seqcount_end(&sl->seqcount);
// /*AFLA*/ 	spin_unlock_irqrestore(&sl->lock, flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A locking reader exclusively locks out other writers and locking readers,
// /*AFLA*/  * but doesn't update the sequence number. Acts like a normal spin_lock/unlock.
// /*AFLA*/  * Don't need preempt_disable() because that is in the spin_lock already.
// /*AFLA*/  */
// /*AFLA*/ static inline void read_seqlock_excl(seqlock_t *sl)
// /*AFLA*/ {
// /*AFLA*/ 	spin_lock(&sl->lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void read_sequnlock_excl(seqlock_t *sl)
// /*AFLA*/ {
// /*AFLA*/ 	spin_unlock(&sl->lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * read_seqbegin_or_lock - begin a sequence number check or locking block
// /*AFLA*/  * @lock: sequence lock
// /*AFLA*/  * @seq : sequence number to be checked
// /*AFLA*/  *
// /*AFLA*/  * First try it once optimistically without taking the lock. If that fails,
// /*AFLA*/  * take the lock. The sequence number is also used as a marker for deciding
// /*AFLA*/  * whether to be a reader (even) or writer (odd).
// /*AFLA*/  * N.B. seq must be initialized to an even number to begin with.
// /*AFLA*/  */
// /*AFLA*/ static inline void read_seqbegin_or_lock(seqlock_t *lock, int *seq)
// /*AFLA*/ {
// /*AFLA*/ 	if (!(*seq & 1))	/* Even */
// /*AFLA*/ 		*seq = read_seqbegin(lock);
// /*AFLA*/ 	else			/* Odd */
// /*AFLA*/ 		read_seqlock_excl(lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int need_seqretry(seqlock_t *lock, int seq)
// /*AFLA*/ {
// /*AFLA*/ 	return !(seq & 1) && read_seqretry(lock, seq);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void done_seqretry(seqlock_t *lock, int seq)
// /*AFLA*/ {
// /*AFLA*/ 	if (seq & 1)
// /*AFLA*/ 		read_sequnlock_excl(lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void read_seqlock_excl_bh(seqlock_t *sl)
// /*AFLA*/ {
// /*AFLA*/ 	spin_lock_bh(&sl->lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void read_sequnlock_excl_bh(seqlock_t *sl)
// /*AFLA*/ {
// /*AFLA*/ 	spin_unlock_bh(&sl->lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void read_seqlock_excl_irq(seqlock_t *sl)
// /*AFLA*/ {
// /*AFLA*/ 	spin_lock_irq(&sl->lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void read_sequnlock_excl_irq(seqlock_t *sl)
// /*AFLA*/ {
// /*AFLA*/ 	spin_unlock_irq(&sl->lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long __read_seqlock_excl_irqsave(seqlock_t *sl)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long flags;
// /*AFLA*/ 
// /*AFLA*/ 	spin_lock_irqsave(&sl->lock, flags);
// /*AFLA*/ 	return flags;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define read_seqlock_excl_irqsave(lock, flags)				\
// /*AFLA*/ 	do { flags = __read_seqlock_excl_irqsave(lock); } while (0)
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ read_sequnlock_excl_irqrestore(seqlock_t *sl, unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ 	spin_unlock_irqrestore(&sl->lock, flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long
// /*AFLA*/ read_seqbegin_or_lock_irqsave(seqlock_t *lock, int *seq)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long flags = 0;
// /*AFLA*/ 
// /*AFLA*/ 	if (!(*seq & 1))	/* Even */
// /*AFLA*/ 		*seq = read_seqbegin(lock);
// /*AFLA*/ 	else			/* Odd */
// /*AFLA*/ 		read_seqlock_excl_irqsave(lock, flags);
// /*AFLA*/ 
// /*AFLA*/ 	return flags;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ done_seqretry_irqrestore(seqlock_t *lock, int seq, unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ 	if (seq & 1)
// /*AFLA*/ 		read_sequnlock_excl_irqrestore(lock, flags);
// /*AFLA*/ }
// /*AFLA*/ #endif /* __LINUX_SEQLOCK_H */
