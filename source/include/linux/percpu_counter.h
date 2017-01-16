// /*AFLA*/ #ifndef _LINUX_PERCPU_COUNTER_H
// /*AFLA*/ #define _LINUX_PERCPU_COUNTER_H
// /*AFLA*/ /*
// /*AFLA*/  * A simple "approximate counter" for use in ext2 and ext3 superblocks.
// /*AFLA*/  *
// /*AFLA*/  * WARNING: these things are HUGE.  4 kbytes per counter on 32-way P4.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/smp.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/threads.h>
// /*AFLA*/ #include <linux/percpu.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/gfp.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 
// /*AFLA*/ struct percpu_counter {
// /*AFLA*/ 	raw_spinlock_t lock;
// /*AFLA*/ 	s64 count;
// /*AFLA*/ #ifdef CONFIG_HOTPLUG_CPU
// /*AFLA*/ 	struct list_head list;	/* All percpu_counters are on a list */
// /*AFLA*/ #endif
// /*AFLA*/ 	s32 __percpu *counters;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern int percpu_counter_batch;
// /*AFLA*/ 
// /*AFLA*/ int __percpu_counter_init(struct percpu_counter *fbc, s64 amount, gfp_t gfp,
// /*AFLA*/ 			  struct lock_class_key *key);
// /*AFLA*/ 
// /*AFLA*/ #define percpu_counter_init(fbc, value, gfp)				\
// /*AFLA*/ 	({								\
// /*AFLA*/ 		static struct lock_class_key __key;			\
// /*AFLA*/ 									\
// /*AFLA*/ 		__percpu_counter_init(fbc, value, gfp, &__key);		\
// /*AFLA*/ 	})
// /*AFLA*/ 
// /*AFLA*/ void percpu_counter_destroy(struct percpu_counter *fbc);
// /*AFLA*/ void percpu_counter_set(struct percpu_counter *fbc, s64 amount);
// /*AFLA*/ void __percpu_counter_add(struct percpu_counter *fbc, s64 amount, s32 batch);
// /*AFLA*/ s64 __percpu_counter_sum(struct percpu_counter *fbc);
// /*AFLA*/ int __percpu_counter_compare(struct percpu_counter *fbc, s64 rhs, s32 batch);
// /*AFLA*/ 
// /*AFLA*/ static inline int percpu_counter_compare(struct percpu_counter *fbc, s64 rhs)
// /*AFLA*/ {
// /*AFLA*/ 	return __percpu_counter_compare(fbc, rhs, percpu_counter_batch);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void percpu_counter_add(struct percpu_counter *fbc, s64 amount)
// /*AFLA*/ {
// /*AFLA*/ 	__percpu_counter_add(fbc, amount, percpu_counter_batch);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline s64 percpu_counter_sum_positive(struct percpu_counter *fbc)
// /*AFLA*/ {
// /*AFLA*/ 	s64 ret = __percpu_counter_sum(fbc);
// /*AFLA*/ 	return ret < 0 ? 0 : ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline s64 percpu_counter_sum(struct percpu_counter *fbc)
// /*AFLA*/ {
// /*AFLA*/ 	return __percpu_counter_sum(fbc);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline s64 percpu_counter_read(struct percpu_counter *fbc)
// /*AFLA*/ {
// /*AFLA*/ 	return fbc->count;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * It is possible for the percpu_counter_read() to return a small negative
// /*AFLA*/  * number for some counter which should never be negative.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ static inline s64 percpu_counter_read_positive(struct percpu_counter *fbc)
// /*AFLA*/ {
// /*AFLA*/ 	s64 ret = fbc->count;
// /*AFLA*/ 
// /*AFLA*/ 	barrier();		/* Prevent reloads of fbc->count */
// /*AFLA*/ 	if (ret >= 0)
// /*AFLA*/ 		return ret;
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int percpu_counter_initialized(struct percpu_counter *fbc)
// /*AFLA*/ {
// /*AFLA*/ 	return (fbc->counters != NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ struct percpu_counter {
// /*AFLA*/ 	s64 count;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline int percpu_counter_init(struct percpu_counter *fbc, s64 amount,
// /*AFLA*/ 				      gfp_t gfp)
// /*AFLA*/ {
// /*AFLA*/ 	fbc->count = amount;
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void percpu_counter_destroy(struct percpu_counter *fbc)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void percpu_counter_set(struct percpu_counter *fbc, s64 amount)
// /*AFLA*/ {
// /*AFLA*/ 	fbc->count = amount;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int percpu_counter_compare(struct percpu_counter *fbc, s64 rhs)
// /*AFLA*/ {
// /*AFLA*/ 	if (fbc->count > rhs)
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	else if (fbc->count < rhs)
// /*AFLA*/ 		return -1;
// /*AFLA*/ 	else
// /*AFLA*/ 		return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int
// /*AFLA*/ __percpu_counter_compare(struct percpu_counter *fbc, s64 rhs, s32 batch)
// /*AFLA*/ {
// /*AFLA*/ 	return percpu_counter_compare(fbc, rhs);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ percpu_counter_add(struct percpu_counter *fbc, s64 amount)
// /*AFLA*/ {
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	fbc->count += amount;
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ __percpu_counter_add(struct percpu_counter *fbc, s64 amount, s32 batch)
// /*AFLA*/ {
// /*AFLA*/ 	percpu_counter_add(fbc, amount);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline s64 percpu_counter_read(struct percpu_counter *fbc)
// /*AFLA*/ {
// /*AFLA*/ 	return fbc->count;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * percpu_counter is intended to track positive numbers. In the UP case the
// /*AFLA*/  * number should never be negative.
// /*AFLA*/  */
// /*AFLA*/ static inline s64 percpu_counter_read_positive(struct percpu_counter *fbc)
// /*AFLA*/ {
// /*AFLA*/ 	return fbc->count;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline s64 percpu_counter_sum_positive(struct percpu_counter *fbc)
// /*AFLA*/ {
// /*AFLA*/ 	return percpu_counter_read_positive(fbc);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline s64 percpu_counter_sum(struct percpu_counter *fbc)
// /*AFLA*/ {
// /*AFLA*/ 	return percpu_counter_read(fbc);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int percpu_counter_initialized(struct percpu_counter *fbc)
// /*AFLA*/ {
// /*AFLA*/ 	return 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif	/* CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ static inline void percpu_counter_inc(struct percpu_counter *fbc)
// /*AFLA*/ {
// /*AFLA*/ 	percpu_counter_add(fbc, 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void percpu_counter_dec(struct percpu_counter *fbc)
// /*AFLA*/ {
// /*AFLA*/ 	percpu_counter_add(fbc, -1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void percpu_counter_sub(struct percpu_counter *fbc, s64 amount)
// /*AFLA*/ {
// /*AFLA*/ 	percpu_counter_add(fbc, -amount);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_PERCPU_COUNTER_H */
