// /*AFLA*/ #ifndef __LINUX_COMPLETION_H
// /*AFLA*/ #define __LINUX_COMPLETION_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * (C) Copyright 2001 Linus Torvalds
// /*AFLA*/  *
// /*AFLA*/  * Atomic wait-for-completion handler data structures.
// /*AFLA*/  * See kernel/sched/completion.c for details.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/wait.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * struct completion - structure used to maintain state for a "completion"
// /*AFLA*/  *
// /*AFLA*/  * This is the opaque structure used to maintain the state for a "completion".
// /*AFLA*/  * Completions currently use a FIFO to queue threads that have to wait for
// /*AFLA*/  * the "completion" event.
// /*AFLA*/  *
// /*AFLA*/  * See also:  complete(), wait_for_completion() (and friends _timeout,
// /*AFLA*/  * _interruptible, _interruptible_timeout, and _killable), init_completion(),
// /*AFLA*/  * reinit_completion(), and macros DECLARE_COMPLETION(),
// /*AFLA*/  * DECLARE_COMPLETION_ONSTACK().
// /*AFLA*/  */
// /*AFLA*/ struct completion {
// /*AFLA*/ 	unsigned int done;
// /*AFLA*/ 	wait_queue_head_t wait;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define COMPLETION_INITIALIZER(work) \
// /*AFLA*/ 	{ 0, __WAIT_QUEUE_HEAD_INITIALIZER((work).wait) }
// /*AFLA*/ 
// /*AFLA*/ #define COMPLETION_INITIALIZER_ONSTACK(work) \
// /*AFLA*/ 	({ init_completion(&work); work; })
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * DECLARE_COMPLETION - declare and initialize a completion structure
// /*AFLA*/  * @work:  identifier for the completion structure
// /*AFLA*/  *
// /*AFLA*/  * This macro declares and initializes a completion structure. Generally used
// /*AFLA*/  * for static declarations. You should use the _ONSTACK variant for automatic
// /*AFLA*/  * variables.
// /*AFLA*/  */
// /*AFLA*/ #define DECLARE_COMPLETION(work) \
// /*AFLA*/ 	struct completion work = COMPLETION_INITIALIZER(work)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Lockdep needs to run a non-constant initializer for on-stack
// /*AFLA*/  * completions - so we use the _ONSTACK() variant for those that
// /*AFLA*/  * are on the kernel stack:
// /*AFLA*/  */
// /*AFLA*/ /**
// /*AFLA*/  * DECLARE_COMPLETION_ONSTACK - declare and initialize a completion structure
// /*AFLA*/  * @work:  identifier for the completion structure
// /*AFLA*/  *
// /*AFLA*/  * This macro declares and initializes a completion structure on the kernel
// /*AFLA*/  * stack.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_LOCKDEP
// /*AFLA*/ # define DECLARE_COMPLETION_ONSTACK(work) \
// /*AFLA*/ 	struct completion work = COMPLETION_INITIALIZER_ONSTACK(work)
// /*AFLA*/ #else
// /*AFLA*/ # define DECLARE_COMPLETION_ONSTACK(work) DECLARE_COMPLETION(work)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * init_completion - Initialize a dynamically allocated completion
// /*AFLA*/  * @x:  pointer to completion structure that is to be initialized
// /*AFLA*/  *
// /*AFLA*/  * This inline function will initialize a dynamically created completion
// /*AFLA*/  * structure.
// /*AFLA*/  */
// /*AFLA*/ static inline void init_completion(struct completion *x)
// /*AFLA*/ {
// /*AFLA*/ 	x->done = 0;
// /*AFLA*/ 	init_waitqueue_head(&x->wait);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * reinit_completion - reinitialize a completion structure
// /*AFLA*/  * @x:  pointer to completion structure that is to be reinitialized
// /*AFLA*/  *
// /*AFLA*/  * This inline function should be used to reinitialize a completion structure so it can
// /*AFLA*/  * be reused. This is especially important after complete_all() is used.
// /*AFLA*/  */
// /*AFLA*/ static inline void reinit_completion(struct completion *x)
// /*AFLA*/ {
// /*AFLA*/ 	x->done = 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void wait_for_completion(struct completion *);
// /*AFLA*/ extern void wait_for_completion_io(struct completion *);
// /*AFLA*/ extern int wait_for_completion_interruptible(struct completion *x);
// /*AFLA*/ extern int wait_for_completion_killable(struct completion *x);
// /*AFLA*/ extern unsigned long wait_for_completion_timeout(struct completion *x,
// /*AFLA*/ 						   unsigned long timeout);
// /*AFLA*/ extern unsigned long wait_for_completion_io_timeout(struct completion *x,
// /*AFLA*/ 						    unsigned long timeout);
// /*AFLA*/ extern long wait_for_completion_interruptible_timeout(
// /*AFLA*/ 	struct completion *x, unsigned long timeout);
// /*AFLA*/ extern long wait_for_completion_killable_timeout(
// /*AFLA*/ 	struct completion *x, unsigned long timeout);
// /*AFLA*/ extern bool try_wait_for_completion(struct completion *x);
// /*AFLA*/ extern bool completion_done(struct completion *x);
// /*AFLA*/ 
// /*AFLA*/ extern void complete(struct completion *);
// /*AFLA*/ extern void complete_all(struct completion *);
// /*AFLA*/ 
// /*AFLA*/ #endif
