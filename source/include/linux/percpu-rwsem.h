// /*AFLA*/ #ifndef _LINUX_PERCPU_RWSEM_H
// /*AFLA*/ #define _LINUX_PERCPU_RWSEM_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/rwsem.h>
// /*AFLA*/ #include <linux/percpu.h>
// /*AFLA*/ #include <linux/wait.h>
// /*AFLA*/ #include <linux/rcu_sync.h>
// /*AFLA*/ #include <linux/lockdep.h>
// /*AFLA*/ 
// /*AFLA*/ struct percpu_rw_semaphore {
// /*AFLA*/ 	struct rcu_sync		rss;
// /*AFLA*/ 	unsigned int __percpu	*read_count;
// /*AFLA*/ 	struct rw_semaphore	rw_sem;
// /*AFLA*/ 	wait_queue_head_t	writer;
// /*AFLA*/ 	int			readers_block;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_STATIC_PERCPU_RWSEM(name)				\
// /*AFLA*/ static DEFINE_PER_CPU(unsigned int, __percpu_rwsem_rc_##name);		\
// /*AFLA*/ static struct percpu_rw_semaphore name = {				\
// /*AFLA*/ 	.rss = __RCU_SYNC_INITIALIZER(name.rss, RCU_SCHED_SYNC),	\
// /*AFLA*/ 	.read_count = &__percpu_rwsem_rc_##name,			\
// /*AFLA*/ 	.rw_sem = __RWSEM_INITIALIZER(name.rw_sem),			\
// /*AFLA*/ 	.writer = __WAIT_QUEUE_HEAD_INITIALIZER(name.writer),		\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int __percpu_down_read(struct percpu_rw_semaphore *, int);
// /*AFLA*/ extern void __percpu_up_read(struct percpu_rw_semaphore *);
// /*AFLA*/ 
// /*AFLA*/ static inline void percpu_down_read_preempt_disable(struct percpu_rw_semaphore *sem)
// /*AFLA*/ {
// /*AFLA*/ 	might_sleep();
// /*AFLA*/ 
// /*AFLA*/ 	rwsem_acquire_read(&sem->rw_sem.dep_map, 0, 0, _RET_IP_);
// /*AFLA*/ 
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * We are in an RCU-sched read-side critical section, so the writer
// /*AFLA*/ 	 * cannot both change sem->state from readers_fast and start checking
// /*AFLA*/ 	 * counters while we are here. So if we see !sem->state, we know that
// /*AFLA*/ 	 * the writer won't be checking until we're past the preempt_enable()
// /*AFLA*/ 	 * and that one the synchronize_sched() is done, the writer will see
// /*AFLA*/ 	 * anything we did within this RCU-sched read-size critical section.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__this_cpu_inc(*sem->read_count);
// /*AFLA*/ 	if (unlikely(!rcu_sync_is_idle(&sem->rss)))
// /*AFLA*/ 		__percpu_down_read(sem, false); /* Unconditional memory barrier */
// /*AFLA*/ 	barrier();
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The barrier() prevents the compiler from
// /*AFLA*/ 	 * bleeding the critical section out.
// /*AFLA*/ 	 */
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void percpu_down_read(struct percpu_rw_semaphore *sem)
// /*AFLA*/ {
// /*AFLA*/ 	percpu_down_read_preempt_disable(sem);
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int percpu_down_read_trylock(struct percpu_rw_semaphore *sem)
// /*AFLA*/ {
// /*AFLA*/ 	int ret = 1;
// /*AFLA*/ 
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Same as in percpu_down_read().
// /*AFLA*/ 	 */
// /*AFLA*/ 	__this_cpu_inc(*sem->read_count);
// /*AFLA*/ 	if (unlikely(!rcu_sync_is_idle(&sem->rss)))
// /*AFLA*/ 		ret = __percpu_down_read(sem, true); /* Unconditional memory barrier */
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The barrier() from preempt_enable() prevents the compiler from
// /*AFLA*/ 	 * bleeding the critical section out.
// /*AFLA*/ 	 */
// /*AFLA*/ 
// /*AFLA*/ 	if (ret)
// /*AFLA*/ 		rwsem_acquire_read(&sem->rw_sem.dep_map, 0, 1, _RET_IP_);
// /*AFLA*/ 
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void percpu_up_read_preempt_enable(struct percpu_rw_semaphore *sem)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The barrier() prevents the compiler from
// /*AFLA*/ 	 * bleeding the critical section out.
// /*AFLA*/ 	 */
// /*AFLA*/ 	barrier();
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Same as in percpu_down_read().
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (likely(rcu_sync_is_idle(&sem->rss)))
// /*AFLA*/ 		__this_cpu_dec(*sem->read_count);
// /*AFLA*/ 	else
// /*AFLA*/ 		__percpu_up_read(sem); /* Unconditional memory barrier */
// /*AFLA*/ 	preempt_enable();
// /*AFLA*/ 
// /*AFLA*/ 	rwsem_release(&sem->rw_sem.dep_map, 1, _RET_IP_);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void percpu_up_read(struct percpu_rw_semaphore *sem)
// /*AFLA*/ {
// /*AFLA*/ 	preempt_disable();
// /*AFLA*/ 	percpu_up_read_preempt_enable(sem);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void percpu_down_write(struct percpu_rw_semaphore *);
// /*AFLA*/ extern void percpu_up_write(struct percpu_rw_semaphore *);
// /*AFLA*/ 
// /*AFLA*/ extern int __percpu_init_rwsem(struct percpu_rw_semaphore *,
// /*AFLA*/ 				const char *, struct lock_class_key *);
// /*AFLA*/ 
// /*AFLA*/ extern void percpu_free_rwsem(struct percpu_rw_semaphore *);
// /*AFLA*/ 
// /*AFLA*/ #define percpu_init_rwsem(sem)					\
// /*AFLA*/ ({								\
// /*AFLA*/ 	static struct lock_class_key rwsem_key;			\
// /*AFLA*/ 	__percpu_init_rwsem(sem, #sem, &rwsem_key);		\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define percpu_rwsem_is_held(sem) lockdep_is_held(&(sem)->rw_sem)
// /*AFLA*/ 
// /*AFLA*/ #define percpu_rwsem_assert_held(sem)				\
// /*AFLA*/ 	lockdep_assert_held(&(sem)->rw_sem)
// /*AFLA*/ 
// /*AFLA*/ static inline void percpu_rwsem_release(struct percpu_rw_semaphore *sem,
// /*AFLA*/ 					bool read, unsigned long ip)
// /*AFLA*/ {
// /*AFLA*/ 	lock_release(&sem->rw_sem.dep_map, 1, ip);
// /*AFLA*/ #ifdef CONFIG_RWSEM_SPIN_ON_OWNER
// /*AFLA*/ 	if (!read)
// /*AFLA*/ 		sem->rw_sem.owner = NULL;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void percpu_rwsem_acquire(struct percpu_rw_semaphore *sem,
// /*AFLA*/ 					bool read, unsigned long ip)
// /*AFLA*/ {
// /*AFLA*/ 	lock_acquire(&sem->rw_sem.dep_map, 0, 1, read, 1, NULL, ip);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
