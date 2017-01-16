// /*AFLA*/ #ifndef _LINUX_WAIT_H
// /*AFLA*/ #define _LINUX_WAIT_H
// /*AFLA*/ /*
// /*AFLA*/  * Linux wait queue related types and methods
// /*AFLA*/  */
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/stddef.h>
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <asm/current.h>
// /*AFLA*/ #include <uapi/linux/wait.h>
// /*AFLA*/ 
// /*AFLA*/ typedef struct __wait_queue wait_queue_t;
// /*AFLA*/ typedef int (*wait_queue_func_t)(wait_queue_t *wait, unsigned mode, int flags, void *key);
// /*AFLA*/ int default_wake_function(wait_queue_t *wait, unsigned mode, int flags, void *key);
// /*AFLA*/ 
// /*AFLA*/ /* __wait_queue::flags */
// /*AFLA*/ #define WQ_FLAG_EXCLUSIVE	0x01
// /*AFLA*/ #define WQ_FLAG_WOKEN		0x02
// /*AFLA*/ 
// /*AFLA*/ struct __wait_queue {
// /*AFLA*/ 	unsigned int		flags;
// /*AFLA*/ 	void			*private;
// /*AFLA*/ 	wait_queue_func_t	func;
// /*AFLA*/ 	struct list_head	task_list;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct wait_bit_key {
// /*AFLA*/ 	void			*flags;
// /*AFLA*/ 	int			bit_nr;
// /*AFLA*/ #define WAIT_ATOMIC_T_BIT_NR	-1
// /*AFLA*/ 	unsigned long		timeout;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct wait_bit_queue {
// /*AFLA*/ 	struct wait_bit_key	key;
// /*AFLA*/ 	wait_queue_t		wait;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct __wait_queue_head {
// /*AFLA*/ 	spinlock_t		lock;
// /*AFLA*/ 	struct list_head	task_list;
// /*AFLA*/ };
// /*AFLA*/ typedef struct __wait_queue_head wait_queue_head_t;
// /*AFLA*/ 
// /*AFLA*/ struct task_struct;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Macros for declaration and initialisaton of the datatypes
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define __WAITQUEUE_INITIALIZER(name, tsk) {				\
// /*AFLA*/ 	.private	= tsk,						\
// /*AFLA*/ 	.func		= default_wake_function,			\
// /*AFLA*/ 	.task_list	= { NULL, NULL } }
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_WAITQUEUE(name, tsk)					\
// /*AFLA*/ 	wait_queue_t name = __WAITQUEUE_INITIALIZER(name, tsk)
// /*AFLA*/ 
// /*AFLA*/ #define __WAIT_QUEUE_HEAD_INITIALIZER(name) {				\
// /*AFLA*/ 	.lock		= __SPIN_LOCK_UNLOCKED(name.lock),		\
// /*AFLA*/ 	.task_list	= { &(name).task_list, &(name).task_list } }
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_WAIT_QUEUE_HEAD(name) \
// /*AFLA*/ 	wait_queue_head_t name = __WAIT_QUEUE_HEAD_INITIALIZER(name)
// /*AFLA*/ 
// /*AFLA*/ #define __WAIT_BIT_KEY_INITIALIZER(word, bit)				\
// /*AFLA*/ 	{ .flags = word, .bit_nr = bit, }
// /*AFLA*/ 
// /*AFLA*/ #define __WAIT_ATOMIC_T_KEY_INITIALIZER(p)				\
// /*AFLA*/ 	{ .flags = p, .bit_nr = WAIT_ATOMIC_T_BIT_NR, }
// /*AFLA*/ 
// /*AFLA*/ extern void __init_waitqueue_head(wait_queue_head_t *q, const char *name, struct lock_class_key *);
// /*AFLA*/ 
// /*AFLA*/ #define init_waitqueue_head(q)				\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		static struct lock_class_key __key;	\
// /*AFLA*/ 							\
// /*AFLA*/ 		__init_waitqueue_head((q), #q, &__key);	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_LOCKDEP
// /*AFLA*/ # define __WAIT_QUEUE_HEAD_INIT_ONSTACK(name) \
// /*AFLA*/ 	({ init_waitqueue_head(&name); name; })
// /*AFLA*/ # define DECLARE_WAIT_QUEUE_HEAD_ONSTACK(name) \
// /*AFLA*/ 	wait_queue_head_t name = __WAIT_QUEUE_HEAD_INIT_ONSTACK(name)
// /*AFLA*/ #else
// /*AFLA*/ # define DECLARE_WAIT_QUEUE_HEAD_ONSTACK(name) DECLARE_WAIT_QUEUE_HEAD(name)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline void init_waitqueue_entry(wait_queue_t *q, struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	q->flags	= 0;
// /*AFLA*/ 	q->private	= p;
// /*AFLA*/ 	q->func		= default_wake_function;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ init_waitqueue_func_entry(wait_queue_t *q, wait_queue_func_t func)
// /*AFLA*/ {
// /*AFLA*/ 	q->flags	= 0;
// /*AFLA*/ 	q->private	= NULL;
// /*AFLA*/ 	q->func		= func;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * waitqueue_active -- locklessly test for waiters on the queue
// /*AFLA*/  * @q: the waitqueue to test for waiters
// /*AFLA*/  *
// /*AFLA*/  * returns true if the wait list is not empty
// /*AFLA*/  *
// /*AFLA*/  * NOTE: this function is lockless and requires care, incorrect usage _will_
// /*AFLA*/  * lead to sporadic and non-obvious failure.
// /*AFLA*/  *
// /*AFLA*/  * Use either while holding wait_queue_head_t::lock or when used for wakeups
// /*AFLA*/  * with an extra smp_mb() like:
// /*AFLA*/  *
// /*AFLA*/  *      CPU0 - waker                    CPU1 - waiter
// /*AFLA*/  *
// /*AFLA*/  *                                      for (;;) {
// /*AFLA*/  *      @cond = true;                     prepare_to_wait(&wq, &wait, state);
// /*AFLA*/  *      smp_mb();                         // smp_mb() from set_current_state()
// /*AFLA*/  *      if (waitqueue_active(wq))         if (@cond)
// /*AFLA*/  *        wake_up(wq);                      break;
// /*AFLA*/  *                                        schedule();
// /*AFLA*/  *                                      }
// /*AFLA*/  *                                      finish_wait(&wq, &wait);
// /*AFLA*/  *
// /*AFLA*/  * Because without the explicit smp_mb() it's possible for the
// /*AFLA*/  * waitqueue_active() load to get hoisted over the @cond store such that we'll
// /*AFLA*/  * observe an empty wait list while the waiter might not observe @cond.
// /*AFLA*/  *
// /*AFLA*/  * Also note that this 'optimization' trades a spin_lock() for an smp_mb(),
// /*AFLA*/  * which (when the lock is uncontended) are of roughly equal cost.
// /*AFLA*/  */
// /*AFLA*/ static inline int waitqueue_active(wait_queue_head_t *q)
// /*AFLA*/ {
// /*AFLA*/ 	return !list_empty(&q->task_list);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wq_has_sleeper - check if there are any waiting processes
// /*AFLA*/  * @wq: wait queue head
// /*AFLA*/  *
// /*AFLA*/  * Returns true if wq has waiting processes
// /*AFLA*/  *
// /*AFLA*/  * Please refer to the comment for waitqueue_active.
// /*AFLA*/  */
// /*AFLA*/ static inline bool wq_has_sleeper(wait_queue_head_t *wq)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * We need to be sure we are in sync with the
// /*AFLA*/ 	 * add_wait_queue modifications to the wait queue.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * This memory barrier should be paired with one on the
// /*AFLA*/ 	 * waiting side.
// /*AFLA*/ 	 */
// /*AFLA*/ 	smp_mb();
// /*AFLA*/ 	return waitqueue_active(wq);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void add_wait_queue(wait_queue_head_t *q, wait_queue_t *wait);
// /*AFLA*/ extern void add_wait_queue_exclusive(wait_queue_head_t *q, wait_queue_t *wait);
// /*AFLA*/ extern void remove_wait_queue(wait_queue_head_t *q, wait_queue_t *wait);
// /*AFLA*/ 
// /*AFLA*/ static inline void __add_wait_queue(wait_queue_head_t *head, wait_queue_t *new)
// /*AFLA*/ {
// /*AFLA*/ 	list_add(&new->task_list, &head->task_list);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Used for wake-one threads:
// /*AFLA*/  */
// /*AFLA*/ static inline void
// /*AFLA*/ __add_wait_queue_exclusive(wait_queue_head_t *q, wait_queue_t *wait)
// /*AFLA*/ {
// /*AFLA*/ 	wait->flags |= WQ_FLAG_EXCLUSIVE;
// /*AFLA*/ 	__add_wait_queue(q, wait);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __add_wait_queue_tail(wait_queue_head_t *head,
// /*AFLA*/ 					 wait_queue_t *new)
// /*AFLA*/ {
// /*AFLA*/ 	list_add_tail(&new->task_list, &head->task_list);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ __add_wait_queue_tail_exclusive(wait_queue_head_t *q, wait_queue_t *wait)
// /*AFLA*/ {
// /*AFLA*/ 	wait->flags |= WQ_FLAG_EXCLUSIVE;
// /*AFLA*/ 	__add_wait_queue_tail(q, wait);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ __remove_wait_queue(wait_queue_head_t *head, wait_queue_t *old)
// /*AFLA*/ {
// /*AFLA*/ 	list_del(&old->task_list);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ typedef int wait_bit_action_f(struct wait_bit_key *, int mode);
// /*AFLA*/ void __wake_up(wait_queue_head_t *q, unsigned int mode, int nr, void *key);
// /*AFLA*/ void __wake_up_locked_key(wait_queue_head_t *q, unsigned int mode, void *key);
// /*AFLA*/ void __wake_up_sync_key(wait_queue_head_t *q, unsigned int mode, int nr, void *key);
// /*AFLA*/ void __wake_up_locked(wait_queue_head_t *q, unsigned int mode, int nr);
// /*AFLA*/ void __wake_up_sync(wait_queue_head_t *q, unsigned int mode, int nr);
// /*AFLA*/ void __wake_up_bit(wait_queue_head_t *, void *, int);
// /*AFLA*/ int __wait_on_bit(wait_queue_head_t *, struct wait_bit_queue *, wait_bit_action_f *, unsigned);
// /*AFLA*/ int __wait_on_bit_lock(wait_queue_head_t *, struct wait_bit_queue *, wait_bit_action_f *, unsigned);
// /*AFLA*/ void wake_up_bit(void *, int);
// /*AFLA*/ void wake_up_atomic_t(atomic_t *);
// /*AFLA*/ int out_of_line_wait_on_bit(void *, int, wait_bit_action_f *, unsigned);
// /*AFLA*/ int out_of_line_wait_on_bit_timeout(void *, int, wait_bit_action_f *, unsigned, unsigned long);
// /*AFLA*/ int out_of_line_wait_on_bit_lock(void *, int, wait_bit_action_f *, unsigned);
// /*AFLA*/ int out_of_line_wait_on_atomic_t(atomic_t *, int (*)(atomic_t *), unsigned);
// /*AFLA*/ wait_queue_head_t *bit_waitqueue(void *, int);
// /*AFLA*/ 
// /*AFLA*/ #define wake_up(x)			__wake_up(x, TASK_NORMAL, 1, NULL)
// /*AFLA*/ #define wake_up_nr(x, nr)		__wake_up(x, TASK_NORMAL, nr, NULL)
// /*AFLA*/ #define wake_up_all(x)			__wake_up(x, TASK_NORMAL, 0, NULL)
// /*AFLA*/ #define wake_up_locked(x)		__wake_up_locked((x), TASK_NORMAL, 1)
// /*AFLA*/ #define wake_up_all_locked(x)		__wake_up_locked((x), TASK_NORMAL, 0)
// /*AFLA*/ 
// /*AFLA*/ #define wake_up_interruptible(x)	__wake_up(x, TASK_INTERRUPTIBLE, 1, NULL)
// /*AFLA*/ #define wake_up_interruptible_nr(x, nr)	__wake_up(x, TASK_INTERRUPTIBLE, nr, NULL)
// /*AFLA*/ #define wake_up_interruptible_all(x)	__wake_up(x, TASK_INTERRUPTIBLE, 0, NULL)
// /*AFLA*/ #define wake_up_interruptible_sync(x)	__wake_up_sync((x), TASK_INTERRUPTIBLE, 1)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Wakeup macros to be used to report events to the targets.
// /*AFLA*/  */
// /*AFLA*/ #define wake_up_poll(x, m)						\
// /*AFLA*/ 	__wake_up(x, TASK_NORMAL, 1, (void *) (m))
// /*AFLA*/ #define wake_up_locked_poll(x, m)					\
// /*AFLA*/ 	__wake_up_locked_key((x), TASK_NORMAL, (void *) (m))
// /*AFLA*/ #define wake_up_interruptible_poll(x, m)				\
// /*AFLA*/ 	__wake_up(x, TASK_INTERRUPTIBLE, 1, (void *) (m))
// /*AFLA*/ #define wake_up_interruptible_sync_poll(x, m)				\
// /*AFLA*/ 	__wake_up_sync_key((x), TASK_INTERRUPTIBLE, 1, (void *) (m))
// /*AFLA*/ 
// /*AFLA*/ #define ___wait_cond_timeout(condition)					\
// /*AFLA*/ ({									\
// /*AFLA*/ 	bool __cond = (condition);					\
// /*AFLA*/ 	if (__cond && !__ret)						\
// /*AFLA*/ 		__ret = 1;						\
// /*AFLA*/ 	__cond || !__ret;						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define ___wait_is_interruptible(state)					\
// /*AFLA*/ 	(!__builtin_constant_p(state) ||				\
// /*AFLA*/ 		state == TASK_INTERRUPTIBLE || state == TASK_KILLABLE)	\
// /*AFLA*/ 
// /*AFLA*/ extern void init_wait_entry(wait_queue_t *__wait, int flags);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The below macro ___wait_event() has an explicit shadow of the __ret
// /*AFLA*/  * variable when used from the wait_event_*() macros.
// /*AFLA*/  *
// /*AFLA*/  * This is so that both can use the ___wait_cond_timeout() construct
// /*AFLA*/  * to wrap the condition.
// /*AFLA*/  *
// /*AFLA*/  * The type inconsistency of the wait_event_*() __ret variable is also
// /*AFLA*/  * on purpose; we use long where we can return timeout values and int
// /*AFLA*/  * otherwise.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define ___wait_event(wq, condition, state, exclusive, ret, cmd)	\
// /*AFLA*/ ({									\
// /*AFLA*/ 	__label__ __out;						\
// /*AFLA*/ 	wait_queue_t __wait;						\
// /*AFLA*/ 	long __ret = ret;	/* explicit shadow */			\
// /*AFLA*/ 									\
// /*AFLA*/ 	init_wait_entry(&__wait, exclusive ? WQ_FLAG_EXCLUSIVE : 0);	\
// /*AFLA*/ 	for (;;) {							\
// /*AFLA*/ 		long __int = prepare_to_wait_event(&wq, &__wait, state);\
// /*AFLA*/ 									\
// /*AFLA*/ 		if (condition)						\
// /*AFLA*/ 			break;						\
// /*AFLA*/ 									\
// /*AFLA*/ 		if (___wait_is_interruptible(state) && __int) {		\
// /*AFLA*/ 			__ret = __int;					\
// /*AFLA*/ 			goto __out;					\
// /*AFLA*/ 		}							\
// /*AFLA*/ 									\
// /*AFLA*/ 		cmd;							\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	finish_wait(&wq, &__wait);					\
// /*AFLA*/ __out:	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event(wq, condition)					\
// /*AFLA*/ 	(void)___wait_event(wq, condition, TASK_UNINTERRUPTIBLE, 0, 0,	\
// /*AFLA*/ 			    schedule())
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event - sleep until a condition gets true
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_UNINTERRUPTIBLE) until the
// /*AFLA*/  * @condition evaluates to true. The @condition is checked each time
// /*AFLA*/  * the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * wake_up() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event(wq, condition)					\
// /*AFLA*/ do {									\
// /*AFLA*/ 	might_sleep();							\
// /*AFLA*/ 	if (condition)							\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	__wait_event(wq, condition);					\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define __io_wait_event(wq, condition)					\
// /*AFLA*/ 	(void)___wait_event(wq, condition, TASK_UNINTERRUPTIBLE, 0, 0,	\
// /*AFLA*/ 			    io_schedule())
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * io_wait_event() -- like wait_event() but with io_schedule()
// /*AFLA*/  */
// /*AFLA*/ #define io_wait_event(wq, condition)					\
// /*AFLA*/ do {									\
// /*AFLA*/ 	might_sleep();							\
// /*AFLA*/ 	if (condition)							\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	__io_wait_event(wq, condition);					\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event_freezable(wq, condition)				\
// /*AFLA*/ 	___wait_event(wq, condition, TASK_INTERRUPTIBLE, 0, 0,		\
// /*AFLA*/ 			    schedule(); try_to_freeze())
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_freezable - sleep (or freeze) until a condition gets true
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_INTERRUPTIBLE -- so as not to contribute
// /*AFLA*/  * to system load) until the @condition evaluates to true. The
// /*AFLA*/  * @condition is checked each time the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * wake_up() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_freezable(wq, condition)				\
// /*AFLA*/ ({									\
// /*AFLA*/ 	int __ret = 0;							\
// /*AFLA*/ 	might_sleep();							\
// /*AFLA*/ 	if (!(condition))						\
// /*AFLA*/ 		__ret = __wait_event_freezable(wq, condition);		\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event_timeout(wq, condition, timeout)			\
// /*AFLA*/ 	___wait_event(wq, ___wait_cond_timeout(condition),		\
// /*AFLA*/ 		      TASK_UNINTERRUPTIBLE, 0, timeout,			\
// /*AFLA*/ 		      __ret = schedule_timeout(__ret))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_timeout - sleep until a condition gets true or a timeout elapses
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  * @timeout: timeout, in jiffies
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_UNINTERRUPTIBLE) until the
// /*AFLA*/  * @condition evaluates to true. The @condition is checked each time
// /*AFLA*/  * the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * wake_up() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  *
// /*AFLA*/  * Returns:
// /*AFLA*/  * 0 if the @condition evaluated to %false after the @timeout elapsed,
// /*AFLA*/  * 1 if the @condition evaluated to %true after the @timeout elapsed,
// /*AFLA*/  * or the remaining jiffies (at least 1) if the @condition evaluated
// /*AFLA*/  * to %true before the @timeout elapsed.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_timeout(wq, condition, timeout)			\
// /*AFLA*/ ({									\
// /*AFLA*/ 	long __ret = timeout;						\
// /*AFLA*/ 	might_sleep();							\
// /*AFLA*/ 	if (!___wait_cond_timeout(condition))				\
// /*AFLA*/ 		__ret = __wait_event_timeout(wq, condition, timeout);	\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event_freezable_timeout(wq, condition, timeout)		\
// /*AFLA*/ 	___wait_event(wq, ___wait_cond_timeout(condition),		\
// /*AFLA*/ 		      TASK_INTERRUPTIBLE, 0, timeout,			\
// /*AFLA*/ 		      __ret = schedule_timeout(__ret); try_to_freeze())
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * like wait_event_timeout() -- except it uses TASK_INTERRUPTIBLE to avoid
// /*AFLA*/  * increasing load and is freezable.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_freezable_timeout(wq, condition, timeout)		\
// /*AFLA*/ ({									\
// /*AFLA*/ 	long __ret = timeout;						\
// /*AFLA*/ 	might_sleep();							\
// /*AFLA*/ 	if (!___wait_cond_timeout(condition))				\
// /*AFLA*/ 		__ret = __wait_event_freezable_timeout(wq, condition, timeout);	\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event_exclusive_cmd(wq, condition, cmd1, cmd2)		\
// /*AFLA*/ 	(void)___wait_event(wq, condition, TASK_UNINTERRUPTIBLE, 1, 0,	\
// /*AFLA*/ 			    cmd1; schedule(); cmd2)
// /*AFLA*/ /*
// /*AFLA*/  * Just like wait_event_cmd(), except it sets exclusive flag
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_exclusive_cmd(wq, condition, cmd1, cmd2)		\
// /*AFLA*/ do {									\
// /*AFLA*/ 	if (condition)							\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	__wait_event_exclusive_cmd(wq, condition, cmd1, cmd2);		\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event_cmd(wq, condition, cmd1, cmd2)			\
// /*AFLA*/ 	(void)___wait_event(wq, condition, TASK_UNINTERRUPTIBLE, 0, 0,	\
// /*AFLA*/ 			    cmd1; schedule(); cmd2)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_cmd - sleep until a condition gets true
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  * @cmd1: the command will be executed before sleep
// /*AFLA*/  * @cmd2: the command will be executed after sleep
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_UNINTERRUPTIBLE) until the
// /*AFLA*/  * @condition evaluates to true. The @condition is checked each time
// /*AFLA*/  * the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * wake_up() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_cmd(wq, condition, cmd1, cmd2)			\
// /*AFLA*/ do {									\
// /*AFLA*/ 	if (condition)							\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	__wait_event_cmd(wq, condition, cmd1, cmd2);			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event_interruptible(wq, condition)			\
// /*AFLA*/ 	___wait_event(wq, condition, TASK_INTERRUPTIBLE, 0, 0,		\
// /*AFLA*/ 		      schedule())
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_interruptible - sleep until a condition gets true
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_INTERRUPTIBLE) until the
// /*AFLA*/  * @condition evaluates to true or a signal is received.
// /*AFLA*/  * The @condition is checked each time the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * wake_up() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  *
// /*AFLA*/  * The function will return -ERESTARTSYS if it was interrupted by a
// /*AFLA*/  * signal and 0 if @condition evaluated to true.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_interruptible(wq, condition)				\
// /*AFLA*/ ({									\
// /*AFLA*/ 	int __ret = 0;							\
// /*AFLA*/ 	might_sleep();							\
// /*AFLA*/ 	if (!(condition))						\
// /*AFLA*/ 		__ret = __wait_event_interruptible(wq, condition);	\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event_interruptible_timeout(wq, condition, timeout)	\
// /*AFLA*/ 	___wait_event(wq, ___wait_cond_timeout(condition),		\
// /*AFLA*/ 		      TASK_INTERRUPTIBLE, 0, timeout,			\
// /*AFLA*/ 		      __ret = schedule_timeout(__ret))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_interruptible_timeout - sleep until a condition gets true or a timeout elapses
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  * @timeout: timeout, in jiffies
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_INTERRUPTIBLE) until the
// /*AFLA*/  * @condition evaluates to true or a signal is received.
// /*AFLA*/  * The @condition is checked each time the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * wake_up() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  *
// /*AFLA*/  * Returns:
// /*AFLA*/  * 0 if the @condition evaluated to %false after the @timeout elapsed,
// /*AFLA*/  * 1 if the @condition evaluated to %true after the @timeout elapsed,
// /*AFLA*/  * the remaining jiffies (at least 1) if the @condition evaluated
// /*AFLA*/  * to %true before the @timeout elapsed, or -%ERESTARTSYS if it was
// /*AFLA*/  * interrupted by a signal.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_interruptible_timeout(wq, condition, timeout)	\
// /*AFLA*/ ({									\
// /*AFLA*/ 	long __ret = timeout;						\
// /*AFLA*/ 	might_sleep();							\
// /*AFLA*/ 	if (!___wait_cond_timeout(condition))				\
// /*AFLA*/ 		__ret = __wait_event_interruptible_timeout(wq,		\
// /*AFLA*/ 						condition, timeout);	\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event_hrtimeout(wq, condition, timeout, state)		\
// /*AFLA*/ ({									\
// /*AFLA*/ 	int __ret = 0;							\
// /*AFLA*/ 	struct hrtimer_sleeper __t;					\
// /*AFLA*/ 									\
// /*AFLA*/ 	hrtimer_init_on_stack(&__t.timer, CLOCK_MONOTONIC,		\
// /*AFLA*/ 			      HRTIMER_MODE_REL);			\
// /*AFLA*/ 	hrtimer_init_sleeper(&__t, current);				\
// /*AFLA*/ 	if ((timeout).tv64 != KTIME_MAX)				\
// /*AFLA*/ 		hrtimer_start_range_ns(&__t.timer, timeout,		\
// /*AFLA*/ 				       current->timer_slack_ns,		\
// /*AFLA*/ 				       HRTIMER_MODE_REL);		\
// /*AFLA*/ 									\
// /*AFLA*/ 	__ret = ___wait_event(wq, condition, state, 0, 0,		\
// /*AFLA*/ 		if (!__t.task) {					\
// /*AFLA*/ 			__ret = -ETIME;					\
// /*AFLA*/ 			break;						\
// /*AFLA*/ 		}							\
// /*AFLA*/ 		schedule());						\
// /*AFLA*/ 									\
// /*AFLA*/ 	hrtimer_cancel(&__t.timer);					\
// /*AFLA*/ 	destroy_hrtimer_on_stack(&__t.timer);				\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_hrtimeout - sleep until a condition gets true or a timeout elapses
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  * @timeout: timeout, as a ktime_t
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_UNINTERRUPTIBLE) until the
// /*AFLA*/  * @condition evaluates to true or a signal is received.
// /*AFLA*/  * The @condition is checked each time the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * wake_up() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  *
// /*AFLA*/  * The function returns 0 if @condition became true, or -ETIME if the timeout
// /*AFLA*/  * elapsed.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_hrtimeout(wq, condition, timeout)			\
// /*AFLA*/ ({									\
// /*AFLA*/ 	int __ret = 0;							\
// /*AFLA*/ 	might_sleep();							\
// /*AFLA*/ 	if (!(condition))						\
// /*AFLA*/ 		__ret = __wait_event_hrtimeout(wq, condition, timeout,	\
// /*AFLA*/ 					       TASK_UNINTERRUPTIBLE);	\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_interruptible_hrtimeout - sleep until a condition gets true or a timeout elapses
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  * @timeout: timeout, as a ktime_t
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_INTERRUPTIBLE) until the
// /*AFLA*/  * @condition evaluates to true or a signal is received.
// /*AFLA*/  * The @condition is checked each time the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * wake_up() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  *
// /*AFLA*/  * The function returns 0 if @condition became true, -ERESTARTSYS if it was
// /*AFLA*/  * interrupted by a signal, or -ETIME if the timeout elapsed.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_interruptible_hrtimeout(wq, condition, timeout)	\
// /*AFLA*/ ({									\
// /*AFLA*/ 	long __ret = 0;							\
// /*AFLA*/ 	might_sleep();							\
// /*AFLA*/ 	if (!(condition))						\
// /*AFLA*/ 		__ret = __wait_event_hrtimeout(wq, condition, timeout,	\
// /*AFLA*/ 					       TASK_INTERRUPTIBLE);	\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event_interruptible_exclusive(wq, condition)		\
// /*AFLA*/ 	___wait_event(wq, condition, TASK_INTERRUPTIBLE, 1, 0,		\
// /*AFLA*/ 		      schedule())
// /*AFLA*/ 
// /*AFLA*/ #define wait_event_interruptible_exclusive(wq, condition)		\
// /*AFLA*/ ({									\
// /*AFLA*/ 	int __ret = 0;							\
// /*AFLA*/ 	might_sleep();							\
// /*AFLA*/ 	if (!(condition))						\
// /*AFLA*/ 		__ret = __wait_event_interruptible_exclusive(wq, condition);\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event_killable_exclusive(wq, condition)			\
// /*AFLA*/ 	___wait_event(wq, condition, TASK_KILLABLE, 1, 0,		\
// /*AFLA*/ 		      schedule())
// /*AFLA*/ 
// /*AFLA*/ #define wait_event_killable_exclusive(wq, condition)			\
// /*AFLA*/ ({									\
// /*AFLA*/ 	int __ret = 0;							\
// /*AFLA*/ 	might_sleep();							\
// /*AFLA*/ 	if (!(condition))						\
// /*AFLA*/ 		__ret = __wait_event_killable_exclusive(wq, condition);	\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event_freezable_exclusive(wq, condition)			\
// /*AFLA*/ 	___wait_event(wq, condition, TASK_INTERRUPTIBLE, 1, 0,		\
// /*AFLA*/ 			schedule(); try_to_freeze())
// /*AFLA*/ 
// /*AFLA*/ #define wait_event_freezable_exclusive(wq, condition)			\
// /*AFLA*/ ({									\
// /*AFLA*/ 	int __ret = 0;							\
// /*AFLA*/ 	might_sleep();							\
// /*AFLA*/ 	if (!(condition))						\
// /*AFLA*/ 		__ret = __wait_event_freezable_exclusive(wq, condition);\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event_interruptible_locked(wq, condition, exclusive, irq) \
// /*AFLA*/ ({									\
// /*AFLA*/ 	int __ret = 0;							\
// /*AFLA*/ 	DEFINE_WAIT(__wait);						\
// /*AFLA*/ 	if (exclusive)							\
// /*AFLA*/ 		__wait.flags |= WQ_FLAG_EXCLUSIVE;			\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		if (likely(list_empty(&__wait.task_list)))		\
// /*AFLA*/ 			__add_wait_queue_tail(&(wq), &__wait);		\
// /*AFLA*/ 		set_current_state(TASK_INTERRUPTIBLE);			\
// /*AFLA*/ 		if (signal_pending(current)) {				\
// /*AFLA*/ 			__ret = -ERESTARTSYS;				\
// /*AFLA*/ 			break;						\
// /*AFLA*/ 		}							\
// /*AFLA*/ 		if (irq)						\
// /*AFLA*/ 			spin_unlock_irq(&(wq).lock);			\
// /*AFLA*/ 		else							\
// /*AFLA*/ 			spin_unlock(&(wq).lock);			\
// /*AFLA*/ 		schedule();						\
// /*AFLA*/ 		if (irq)						\
// /*AFLA*/ 			spin_lock_irq(&(wq).lock);			\
// /*AFLA*/ 		else							\
// /*AFLA*/ 			spin_lock(&(wq).lock);				\
// /*AFLA*/ 	} while (!(condition));						\
// /*AFLA*/ 	__remove_wait_queue(&(wq), &__wait);				\
// /*AFLA*/ 	__set_current_state(TASK_RUNNING);				\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_interruptible_locked - sleep until a condition gets true
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_INTERRUPTIBLE) until the
// /*AFLA*/  * @condition evaluates to true or a signal is received.
// /*AFLA*/  * The @condition is checked each time the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * It must be called with wq.lock being held.  This spinlock is
// /*AFLA*/  * unlocked while sleeping but @condition testing is done while lock
// /*AFLA*/  * is held and when this macro exits the lock is held.
// /*AFLA*/  *
// /*AFLA*/  * The lock is locked/unlocked using spin_lock()/spin_unlock()
// /*AFLA*/  * functions which must match the way they are locked/unlocked outside
// /*AFLA*/  * of this macro.
// /*AFLA*/  *
// /*AFLA*/  * wake_up_locked() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  *
// /*AFLA*/  * The function will return -ERESTARTSYS if it was interrupted by a
// /*AFLA*/  * signal and 0 if @condition evaluated to true.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_interruptible_locked(wq, condition)			\
// /*AFLA*/ 	((condition)							\
// /*AFLA*/ 	 ? 0 : __wait_event_interruptible_locked(wq, condition, 0, 0))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_interruptible_locked_irq - sleep until a condition gets true
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_INTERRUPTIBLE) until the
// /*AFLA*/  * @condition evaluates to true or a signal is received.
// /*AFLA*/  * The @condition is checked each time the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * It must be called with wq.lock being held.  This spinlock is
// /*AFLA*/  * unlocked while sleeping but @condition testing is done while lock
// /*AFLA*/  * is held and when this macro exits the lock is held.
// /*AFLA*/  *
// /*AFLA*/  * The lock is locked/unlocked using spin_lock_irq()/spin_unlock_irq()
// /*AFLA*/  * functions which must match the way they are locked/unlocked outside
// /*AFLA*/  * of this macro.
// /*AFLA*/  *
// /*AFLA*/  * wake_up_locked() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  *
// /*AFLA*/  * The function will return -ERESTARTSYS if it was interrupted by a
// /*AFLA*/  * signal and 0 if @condition evaluated to true.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_interruptible_locked_irq(wq, condition)		\
// /*AFLA*/ 	((condition)							\
// /*AFLA*/ 	 ? 0 : __wait_event_interruptible_locked(wq, condition, 0, 1))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_interruptible_exclusive_locked - sleep exclusively until a condition gets true
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_INTERRUPTIBLE) until the
// /*AFLA*/  * @condition evaluates to true or a signal is received.
// /*AFLA*/  * The @condition is checked each time the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * It must be called with wq.lock being held.  This spinlock is
// /*AFLA*/  * unlocked while sleeping but @condition testing is done while lock
// /*AFLA*/  * is held and when this macro exits the lock is held.
// /*AFLA*/  *
// /*AFLA*/  * The lock is locked/unlocked using spin_lock()/spin_unlock()
// /*AFLA*/  * functions which must match the way they are locked/unlocked outside
// /*AFLA*/  * of this macro.
// /*AFLA*/  *
// /*AFLA*/  * The process is put on the wait queue with an WQ_FLAG_EXCLUSIVE flag
// /*AFLA*/  * set thus when other process waits process on the list if this
// /*AFLA*/  * process is awaken further processes are not considered.
// /*AFLA*/  *
// /*AFLA*/  * wake_up_locked() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  *
// /*AFLA*/  * The function will return -ERESTARTSYS if it was interrupted by a
// /*AFLA*/  * signal and 0 if @condition evaluated to true.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_interruptible_exclusive_locked(wq, condition)	\
// /*AFLA*/ 	((condition)							\
// /*AFLA*/ 	 ? 0 : __wait_event_interruptible_locked(wq, condition, 1, 0))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_interruptible_exclusive_locked_irq - sleep until a condition gets true
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_INTERRUPTIBLE) until the
// /*AFLA*/  * @condition evaluates to true or a signal is received.
// /*AFLA*/  * The @condition is checked each time the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * It must be called with wq.lock being held.  This spinlock is
// /*AFLA*/  * unlocked while sleeping but @condition testing is done while lock
// /*AFLA*/  * is held and when this macro exits the lock is held.
// /*AFLA*/  *
// /*AFLA*/  * The lock is locked/unlocked using spin_lock_irq()/spin_unlock_irq()
// /*AFLA*/  * functions which must match the way they are locked/unlocked outside
// /*AFLA*/  * of this macro.
// /*AFLA*/  *
// /*AFLA*/  * The process is put on the wait queue with an WQ_FLAG_EXCLUSIVE flag
// /*AFLA*/  * set thus when other process waits process on the list if this
// /*AFLA*/  * process is awaken further processes are not considered.
// /*AFLA*/  *
// /*AFLA*/  * wake_up_locked() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  *
// /*AFLA*/  * The function will return -ERESTARTSYS if it was interrupted by a
// /*AFLA*/  * signal and 0 if @condition evaluated to true.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_interruptible_exclusive_locked_irq(wq, condition)	\
// /*AFLA*/ 	((condition)							\
// /*AFLA*/ 	 ? 0 : __wait_event_interruptible_locked(wq, condition, 1, 1))
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event_killable(wq, condition)				\
// /*AFLA*/ 	___wait_event(wq, condition, TASK_KILLABLE, 0, 0, schedule())
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_killable - sleep until a condition gets true
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_KILLABLE) until the
// /*AFLA*/  * @condition evaluates to true or a signal is received.
// /*AFLA*/  * The @condition is checked each time the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * wake_up() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  *
// /*AFLA*/  * The function will return -ERESTARTSYS if it was interrupted by a
// /*AFLA*/  * signal and 0 if @condition evaluated to true.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_killable(wq, condition)				\
// /*AFLA*/ ({									\
// /*AFLA*/ 	int __ret = 0;							\
// /*AFLA*/ 	might_sleep();							\
// /*AFLA*/ 	if (!(condition))						\
// /*AFLA*/ 		__ret = __wait_event_killable(wq, condition);		\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event_lock_irq(wq, condition, lock, cmd)			\
// /*AFLA*/ 	(void)___wait_event(wq, condition, TASK_UNINTERRUPTIBLE, 0, 0,	\
// /*AFLA*/ 			    spin_unlock_irq(&lock);			\
// /*AFLA*/ 			    cmd;					\
// /*AFLA*/ 			    schedule();					\
// /*AFLA*/ 			    spin_lock_irq(&lock))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_lock_irq_cmd - sleep until a condition gets true. The
// /*AFLA*/  *			     condition is checked under the lock. This
// /*AFLA*/  *			     is expected to be called with the lock
// /*AFLA*/  *			     taken.
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  * @lock: a locked spinlock_t, which will be released before cmd
// /*AFLA*/  *	  and schedule() and reacquired afterwards.
// /*AFLA*/  * @cmd: a command which is invoked outside the critical section before
// /*AFLA*/  *	 sleep
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_UNINTERRUPTIBLE) until the
// /*AFLA*/  * @condition evaluates to true. The @condition is checked each time
// /*AFLA*/  * the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * wake_up() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  *
// /*AFLA*/  * This is supposed to be called while holding the lock. The lock is
// /*AFLA*/  * dropped before invoking the cmd and going to sleep and is reacquired
// /*AFLA*/  * afterwards.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_lock_irq_cmd(wq, condition, lock, cmd)		\
// /*AFLA*/ do {									\
// /*AFLA*/ 	if (condition)							\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	__wait_event_lock_irq(wq, condition, lock, cmd);		\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_lock_irq - sleep until a condition gets true. The
// /*AFLA*/  *			 condition is checked under the lock. This
// /*AFLA*/  *			 is expected to be called with the lock
// /*AFLA*/  *			 taken.
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  * @lock: a locked spinlock_t, which will be released before schedule()
// /*AFLA*/  *	  and reacquired afterwards.
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_UNINTERRUPTIBLE) until the
// /*AFLA*/  * @condition evaluates to true. The @condition is checked each time
// /*AFLA*/  * the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * wake_up() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  *
// /*AFLA*/  * This is supposed to be called while holding the lock. The lock is
// /*AFLA*/  * dropped before going to sleep and is reacquired afterwards.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_lock_irq(wq, condition, lock)			\
// /*AFLA*/ do {									\
// /*AFLA*/ 	if (condition)							\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	__wait_event_lock_irq(wq, condition, lock, );			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event_interruptible_lock_irq(wq, condition, lock, cmd)	\
// /*AFLA*/ 	___wait_event(wq, condition, TASK_INTERRUPTIBLE, 0, 0,		\
// /*AFLA*/ 		      spin_unlock_irq(&lock);				\
// /*AFLA*/ 		      cmd;						\
// /*AFLA*/ 		      schedule();					\
// /*AFLA*/ 		      spin_lock_irq(&lock))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_interruptible_lock_irq_cmd - sleep until a condition gets true.
// /*AFLA*/  *		The condition is checked under the lock. This is expected to
// /*AFLA*/  *		be called with the lock taken.
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  * @lock: a locked spinlock_t, which will be released before cmd and
// /*AFLA*/  *	  schedule() and reacquired afterwards.
// /*AFLA*/  * @cmd: a command which is invoked outside the critical section before
// /*AFLA*/  *	 sleep
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_INTERRUPTIBLE) until the
// /*AFLA*/  * @condition evaluates to true or a signal is received. The @condition is
// /*AFLA*/  * checked each time the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * wake_up() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  *
// /*AFLA*/  * This is supposed to be called while holding the lock. The lock is
// /*AFLA*/  * dropped before invoking the cmd and going to sleep and is reacquired
// /*AFLA*/  * afterwards.
// /*AFLA*/  *
// /*AFLA*/  * The macro will return -ERESTARTSYS if it was interrupted by a signal
// /*AFLA*/  * and 0 if @condition evaluated to true.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_interruptible_lock_irq_cmd(wq, condition, lock, cmd)	\
// /*AFLA*/ ({									\
// /*AFLA*/ 	int __ret = 0;							\
// /*AFLA*/ 	if (!(condition))						\
// /*AFLA*/ 		__ret = __wait_event_interruptible_lock_irq(wq,		\
// /*AFLA*/ 						condition, lock, cmd);	\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_interruptible_lock_irq - sleep until a condition gets true.
// /*AFLA*/  *		The condition is checked under the lock. This is expected
// /*AFLA*/  *		to be called with the lock taken.
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  * @lock: a locked spinlock_t, which will be released before schedule()
// /*AFLA*/  *	  and reacquired afterwards.
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_INTERRUPTIBLE) until the
// /*AFLA*/  * @condition evaluates to true or signal is received. The @condition is
// /*AFLA*/  * checked each time the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * wake_up() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  *
// /*AFLA*/  * This is supposed to be called while holding the lock. The lock is
// /*AFLA*/  * dropped before going to sleep and is reacquired afterwards.
// /*AFLA*/  *
// /*AFLA*/  * The macro will return -ERESTARTSYS if it was interrupted by a signal
// /*AFLA*/  * and 0 if @condition evaluated to true.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_interruptible_lock_irq(wq, condition, lock)		\
// /*AFLA*/ ({									\
// /*AFLA*/ 	int __ret = 0;							\
// /*AFLA*/ 	if (!(condition))						\
// /*AFLA*/ 		__ret = __wait_event_interruptible_lock_irq(wq,		\
// /*AFLA*/ 						condition, lock,);	\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define __wait_event_interruptible_lock_irq_timeout(wq, condition,	\
// /*AFLA*/ 						    lock, timeout)	\
// /*AFLA*/ 	___wait_event(wq, ___wait_cond_timeout(condition),		\
// /*AFLA*/ 		      TASK_INTERRUPTIBLE, 0, timeout,			\
// /*AFLA*/ 		      spin_unlock_irq(&lock);				\
// /*AFLA*/ 		      __ret = schedule_timeout(__ret);			\
// /*AFLA*/ 		      spin_lock_irq(&lock));
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_event_interruptible_lock_irq_timeout - sleep until a condition gets
// /*AFLA*/  *		true or a timeout elapses. The condition is checked under
// /*AFLA*/  *		the lock. This is expected to be called with the lock taken.
// /*AFLA*/  * @wq: the waitqueue to wait on
// /*AFLA*/  * @condition: a C expression for the event to wait for
// /*AFLA*/  * @lock: a locked spinlock_t, which will be released before schedule()
// /*AFLA*/  *	  and reacquired afterwards.
// /*AFLA*/  * @timeout: timeout, in jiffies
// /*AFLA*/  *
// /*AFLA*/  * The process is put to sleep (TASK_INTERRUPTIBLE) until the
// /*AFLA*/  * @condition evaluates to true or signal is received. The @condition is
// /*AFLA*/  * checked each time the waitqueue @wq is woken up.
// /*AFLA*/  *
// /*AFLA*/  * wake_up() has to be called after changing any variable that could
// /*AFLA*/  * change the result of the wait condition.
// /*AFLA*/  *
// /*AFLA*/  * This is supposed to be called while holding the lock. The lock is
// /*AFLA*/  * dropped before going to sleep and is reacquired afterwards.
// /*AFLA*/  *
// /*AFLA*/  * The function returns 0 if the @timeout elapsed, -ERESTARTSYS if it
// /*AFLA*/  * was interrupted by a signal, and the remaining jiffies otherwise
// /*AFLA*/  * if the condition evaluated to true before the timeout elapsed.
// /*AFLA*/  */
// /*AFLA*/ #define wait_event_interruptible_lock_irq_timeout(wq, condition, lock,	\
// /*AFLA*/ 						  timeout)		\
// /*AFLA*/ ({									\
// /*AFLA*/ 	long __ret = timeout;						\
// /*AFLA*/ 	if (!___wait_cond_timeout(condition))				\
// /*AFLA*/ 		__ret = __wait_event_interruptible_lock_irq_timeout(	\
// /*AFLA*/ 					wq, condition, lock, timeout);	\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Waitqueues which are removed from the waitqueue_head at wakeup time
// /*AFLA*/  */
// /*AFLA*/ void prepare_to_wait(wait_queue_head_t *q, wait_queue_t *wait, int state);
// /*AFLA*/ void prepare_to_wait_exclusive(wait_queue_head_t *q, wait_queue_t *wait, int state);
// /*AFLA*/ long prepare_to_wait_event(wait_queue_head_t *q, wait_queue_t *wait, int state);
// /*AFLA*/ void finish_wait(wait_queue_head_t *q, wait_queue_t *wait);
// /*AFLA*/ long wait_woken(wait_queue_t *wait, unsigned mode, long timeout);
// /*AFLA*/ int woken_wake_function(wait_queue_t *wait, unsigned mode, int sync, void *key);
// /*AFLA*/ int autoremove_wake_function(wait_queue_t *wait, unsigned mode, int sync, void *key);
// /*AFLA*/ int wake_bit_function(wait_queue_t *wait, unsigned mode, int sync, void *key);
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_WAIT_FUNC(name, function)				\
// /*AFLA*/ 	wait_queue_t name = {						\
// /*AFLA*/ 		.private	= current,				\
// /*AFLA*/ 		.func		= function,				\
// /*AFLA*/ 		.task_list	= LIST_HEAD_INIT((name).task_list),	\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_WAIT(name) DEFINE_WAIT_FUNC(name, autoremove_wake_function)
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_WAIT_BIT(name, word, bit)				\
// /*AFLA*/ 	struct wait_bit_queue name = {					\
// /*AFLA*/ 		.key = __WAIT_BIT_KEY_INITIALIZER(word, bit),		\
// /*AFLA*/ 		.wait	= {						\
// /*AFLA*/ 			.private	= current,			\
// /*AFLA*/ 			.func		= wake_bit_function,		\
// /*AFLA*/ 			.task_list	=				\
// /*AFLA*/ 				LIST_HEAD_INIT((name).wait.task_list),	\
// /*AFLA*/ 		},							\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ #define init_wait(wait)							\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		(wait)->private = current;				\
// /*AFLA*/ 		(wait)->func = autoremove_wake_function;		\
// /*AFLA*/ 		INIT_LIST_HEAD(&(wait)->task_list);			\
// /*AFLA*/ 		(wait)->flags = 0;					\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ extern int bit_wait(struct wait_bit_key *, int);
// /*AFLA*/ extern int bit_wait_io(struct wait_bit_key *, int);
// /*AFLA*/ extern int bit_wait_timeout(struct wait_bit_key *, int);
// /*AFLA*/ extern int bit_wait_io_timeout(struct wait_bit_key *, int);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_on_bit - wait for a bit to be cleared
// /*AFLA*/  * @word: the word being waited on, a kernel virtual address
// /*AFLA*/  * @bit: the bit of the word being waited on
// /*AFLA*/  * @mode: the task state to sleep in
// /*AFLA*/  *
// /*AFLA*/  * There is a standard hashed waitqueue table for generic use. This
// /*AFLA*/  * is the part of the hashtable's accessor API that waits on a bit.
// /*AFLA*/  * For instance, if one were to have waiters on a bitflag, one would
// /*AFLA*/  * call wait_on_bit() in threads waiting for the bit to clear.
// /*AFLA*/  * One uses wait_on_bit() where one is waiting for the bit to clear,
// /*AFLA*/  * but has no intention of setting it.
// /*AFLA*/  * Returned value will be zero if the bit was cleared, or non-zero
// /*AFLA*/  * if the process received a signal and the mode permitted wakeup
// /*AFLA*/  * on that signal.
// /*AFLA*/  */
// /*AFLA*/ static inline int
// /*AFLA*/ wait_on_bit(unsigned long *word, int bit, unsigned mode)
// /*AFLA*/ {
// /*AFLA*/ 	might_sleep();
// /*AFLA*/ 	if (!test_bit(bit, word))
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	return out_of_line_wait_on_bit(word, bit,
// /*AFLA*/ 				       bit_wait,
// /*AFLA*/ 				       mode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_on_bit_io - wait for a bit to be cleared
// /*AFLA*/  * @word: the word being waited on, a kernel virtual address
// /*AFLA*/  * @bit: the bit of the word being waited on
// /*AFLA*/  * @mode: the task state to sleep in
// /*AFLA*/  *
// /*AFLA*/  * Use the standard hashed waitqueue table to wait for a bit
// /*AFLA*/  * to be cleared.  This is similar to wait_on_bit(), but calls
// /*AFLA*/  * io_schedule() instead of schedule() for the actual waiting.
// /*AFLA*/  *
// /*AFLA*/  * Returned value will be zero if the bit was cleared, or non-zero
// /*AFLA*/  * if the process received a signal and the mode permitted wakeup
// /*AFLA*/  * on that signal.
// /*AFLA*/  */
// /*AFLA*/ static inline int
// /*AFLA*/ wait_on_bit_io(unsigned long *word, int bit, unsigned mode)
// /*AFLA*/ {
// /*AFLA*/ 	might_sleep();
// /*AFLA*/ 	if (!test_bit(bit, word))
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	return out_of_line_wait_on_bit(word, bit,
// /*AFLA*/ 				       bit_wait_io,
// /*AFLA*/ 				       mode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_on_bit_timeout - wait for a bit to be cleared or a timeout elapses
// /*AFLA*/  * @word: the word being waited on, a kernel virtual address
// /*AFLA*/  * @bit: the bit of the word being waited on
// /*AFLA*/  * @mode: the task state to sleep in
// /*AFLA*/  * @timeout: timeout, in jiffies
// /*AFLA*/  *
// /*AFLA*/  * Use the standard hashed waitqueue table to wait for a bit
// /*AFLA*/  * to be cleared. This is similar to wait_on_bit(), except also takes a
// /*AFLA*/  * timeout parameter.
// /*AFLA*/  *
// /*AFLA*/  * Returned value will be zero if the bit was cleared before the
// /*AFLA*/  * @timeout elapsed, or non-zero if the @timeout elapsed or process
// /*AFLA*/  * received a signal and the mode permitted wakeup on that signal.
// /*AFLA*/  */
// /*AFLA*/ static inline int
// /*AFLA*/ wait_on_bit_timeout(unsigned long *word, int bit, unsigned mode,
// /*AFLA*/ 		    unsigned long timeout)
// /*AFLA*/ {
// /*AFLA*/ 	might_sleep();
// /*AFLA*/ 	if (!test_bit(bit, word))
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	return out_of_line_wait_on_bit_timeout(word, bit,
// /*AFLA*/ 					       bit_wait_timeout,
// /*AFLA*/ 					       mode, timeout);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_on_bit_action - wait for a bit to be cleared
// /*AFLA*/  * @word: the word being waited on, a kernel virtual address
// /*AFLA*/  * @bit: the bit of the word being waited on
// /*AFLA*/  * @action: the function used to sleep, which may take special actions
// /*AFLA*/  * @mode: the task state to sleep in
// /*AFLA*/  *
// /*AFLA*/  * Use the standard hashed waitqueue table to wait for a bit
// /*AFLA*/  * to be cleared, and allow the waiting action to be specified.
// /*AFLA*/  * This is like wait_on_bit() but allows fine control of how the waiting
// /*AFLA*/  * is done.
// /*AFLA*/  *
// /*AFLA*/  * Returned value will be zero if the bit was cleared, or non-zero
// /*AFLA*/  * if the process received a signal and the mode permitted wakeup
// /*AFLA*/  * on that signal.
// /*AFLA*/  */
// /*AFLA*/ static inline int
// /*AFLA*/ wait_on_bit_action(unsigned long *word, int bit, wait_bit_action_f *action,
// /*AFLA*/ 		   unsigned mode)
// /*AFLA*/ {
// /*AFLA*/ 	might_sleep();
// /*AFLA*/ 	if (!test_bit(bit, word))
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	return out_of_line_wait_on_bit(word, bit, action, mode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_on_bit_lock - wait for a bit to be cleared, when wanting to set it
// /*AFLA*/  * @word: the word being waited on, a kernel virtual address
// /*AFLA*/  * @bit: the bit of the word being waited on
// /*AFLA*/  * @mode: the task state to sleep in
// /*AFLA*/  *
// /*AFLA*/  * There is a standard hashed waitqueue table for generic use. This
// /*AFLA*/  * is the part of the hashtable's accessor API that waits on a bit
// /*AFLA*/  * when one intends to set it, for instance, trying to lock bitflags.
// /*AFLA*/  * For instance, if one were to have waiters trying to set bitflag
// /*AFLA*/  * and waiting for it to clear before setting it, one would call
// /*AFLA*/  * wait_on_bit() in threads waiting to be able to set the bit.
// /*AFLA*/  * One uses wait_on_bit_lock() where one is waiting for the bit to
// /*AFLA*/  * clear with the intention of setting it, and when done, clearing it.
// /*AFLA*/  *
// /*AFLA*/  * Returns zero if the bit was (eventually) found to be clear and was
// /*AFLA*/  * set.  Returns non-zero if a signal was delivered to the process and
// /*AFLA*/  * the @mode allows that signal to wake the process.
// /*AFLA*/  */
// /*AFLA*/ static inline int
// /*AFLA*/ wait_on_bit_lock(unsigned long *word, int bit, unsigned mode)
// /*AFLA*/ {
// /*AFLA*/ 	might_sleep();
// /*AFLA*/ 	if (!test_and_set_bit(bit, word))
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	return out_of_line_wait_on_bit_lock(word, bit, bit_wait, mode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_on_bit_lock_io - wait for a bit to be cleared, when wanting to set it
// /*AFLA*/  * @word: the word being waited on, a kernel virtual address
// /*AFLA*/  * @bit: the bit of the word being waited on
// /*AFLA*/  * @mode: the task state to sleep in
// /*AFLA*/  *
// /*AFLA*/  * Use the standard hashed waitqueue table to wait for a bit
// /*AFLA*/  * to be cleared and then to atomically set it.  This is similar
// /*AFLA*/  * to wait_on_bit(), but calls io_schedule() instead of schedule()
// /*AFLA*/  * for the actual waiting.
// /*AFLA*/  *
// /*AFLA*/  * Returns zero if the bit was (eventually) found to be clear and was
// /*AFLA*/  * set.  Returns non-zero if a signal was delivered to the process and
// /*AFLA*/  * the @mode allows that signal to wake the process.
// /*AFLA*/  */
// /*AFLA*/ static inline int
// /*AFLA*/ wait_on_bit_lock_io(unsigned long *word, int bit, unsigned mode)
// /*AFLA*/ {
// /*AFLA*/ 	might_sleep();
// /*AFLA*/ 	if (!test_and_set_bit(bit, word))
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	return out_of_line_wait_on_bit_lock(word, bit, bit_wait_io, mode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_on_bit_lock_action - wait for a bit to be cleared, when wanting to set it
// /*AFLA*/  * @word: the word being waited on, a kernel virtual address
// /*AFLA*/  * @bit: the bit of the word being waited on
// /*AFLA*/  * @action: the function used to sleep, which may take special actions
// /*AFLA*/  * @mode: the task state to sleep in
// /*AFLA*/  *
// /*AFLA*/  * Use the standard hashed waitqueue table to wait for a bit
// /*AFLA*/  * to be cleared and then to set it, and allow the waiting action
// /*AFLA*/  * to be specified.
// /*AFLA*/  * This is like wait_on_bit() but allows fine control of how the waiting
// /*AFLA*/  * is done.
// /*AFLA*/  *
// /*AFLA*/  * Returns zero if the bit was (eventually) found to be clear and was
// /*AFLA*/  * set.  Returns non-zero if a signal was delivered to the process and
// /*AFLA*/  * the @mode allows that signal to wake the process.
// /*AFLA*/  */
// /*AFLA*/ static inline int
// /*AFLA*/ wait_on_bit_lock_action(unsigned long *word, int bit, wait_bit_action_f *action,
// /*AFLA*/ 			unsigned mode)
// /*AFLA*/ {
// /*AFLA*/ 	might_sleep();
// /*AFLA*/ 	if (!test_and_set_bit(bit, word))
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	return out_of_line_wait_on_bit_lock(word, bit, action, mode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wait_on_atomic_t - Wait for an atomic_t to become 0
// /*AFLA*/  * @val: The atomic value being waited on, a kernel virtual address
// /*AFLA*/  * @action: the function used to sleep, which may take special actions
// /*AFLA*/  * @mode: the task state to sleep in
// /*AFLA*/  *
// /*AFLA*/  * Wait for an atomic_t to become 0.  We abuse the bit-wait waitqueue table for
// /*AFLA*/  * the purpose of getting a waitqueue, but we set the key to a bit number
// /*AFLA*/  * outside of the target 'word'.
// /*AFLA*/  */
// /*AFLA*/ static inline
// /*AFLA*/ int wait_on_atomic_t(atomic_t *val, int (*action)(atomic_t *), unsigned mode)
// /*AFLA*/ {
// /*AFLA*/ 	might_sleep();
// /*AFLA*/ 	if (atomic_read(val) == 0)
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	return out_of_line_wait_on_atomic_t(val, action, mode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_WAIT_H */
