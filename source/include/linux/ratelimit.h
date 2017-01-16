// /*AFLA*/ #ifndef _LINUX_RATELIMIT_H
// /*AFLA*/ #define _LINUX_RATELIMIT_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/param.h>
// /*AFLA*/ #include <linux/sched.h>
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ 
// /*AFLA*/ #define DEFAULT_RATELIMIT_INTERVAL	(5 * HZ)
// /*AFLA*/ #define DEFAULT_RATELIMIT_BURST		10
// /*AFLA*/ 
// /*AFLA*/ /* issue num suppressed message on exit */
// /*AFLA*/ #define RATELIMIT_MSG_ON_RELEASE	BIT(0)
// /*AFLA*/ 
// /*AFLA*/ struct ratelimit_state {
// /*AFLA*/ 	raw_spinlock_t	lock;		/* protect the state */
// /*AFLA*/ 
// /*AFLA*/ 	int		interval;
// /*AFLA*/ 	int		burst;
// /*AFLA*/ 	int		printed;
// /*AFLA*/ 	int		missed;
// /*AFLA*/ 	unsigned long	begin;
// /*AFLA*/ 	unsigned long	flags;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define RATELIMIT_STATE_INIT(name, interval_init, burst_init) {		\
// /*AFLA*/ 		.lock		= __RAW_SPIN_LOCK_UNLOCKED(name.lock),	\
// /*AFLA*/ 		.interval	= interval_init,			\
// /*AFLA*/ 		.burst		= burst_init,				\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ #define RATELIMIT_STATE_INIT_DISABLED					\
// /*AFLA*/ 	RATELIMIT_STATE_INIT(ratelimit_state, 0, DEFAULT_RATELIMIT_BURST)
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_RATELIMIT_STATE(name, interval_init, burst_init)		\
// /*AFLA*/ 									\
// /*AFLA*/ 	struct ratelimit_state name =					\
// /*AFLA*/ 		RATELIMIT_STATE_INIT(name, interval_init, burst_init)	\
// /*AFLA*/ 
// /*AFLA*/ static inline void ratelimit_state_init(struct ratelimit_state *rs,
// /*AFLA*/ 					int interval, int burst)
// /*AFLA*/ {
// /*AFLA*/ 	memset(rs, 0, sizeof(*rs));
// /*AFLA*/ 
// /*AFLA*/ 	raw_spin_lock_init(&rs->lock);
// /*AFLA*/ 	rs->interval	= interval;
// /*AFLA*/ 	rs->burst	= burst;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ratelimit_default_init(struct ratelimit_state *rs)
// /*AFLA*/ {
// /*AFLA*/ 	return ratelimit_state_init(rs, DEFAULT_RATELIMIT_INTERVAL,
// /*AFLA*/ 					DEFAULT_RATELIMIT_BURST);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ratelimit_state_exit(struct ratelimit_state *rs)
// /*AFLA*/ {
// /*AFLA*/ 	if (!(rs->flags & RATELIMIT_MSG_ON_RELEASE))
// /*AFLA*/ 		return;
// /*AFLA*/ 
// /*AFLA*/ 	if (rs->missed) {
// /*AFLA*/ 		pr_warn("%s: %d output lines suppressed due to ratelimiting\n",
// /*AFLA*/ 			current->comm, rs->missed);
// /*AFLA*/ 		rs->missed = 0;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ ratelimit_set_flags(struct ratelimit_state *rs, unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ 	rs->flags = flags;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct ratelimit_state printk_ratelimit_state;
// /*AFLA*/ 
// /*AFLA*/ extern int ___ratelimit(struct ratelimit_state *rs, const char *func);
// /*AFLA*/ #define __ratelimit(state) ___ratelimit(state, __func__)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PRINTK
// /*AFLA*/ 
// /*AFLA*/ #define WARN_ON_RATELIMIT(condition, state)			\
// /*AFLA*/ 		WARN_ON((condition) && __ratelimit(state))
// /*AFLA*/ 
// /*AFLA*/ #define WARN_RATELIMIT(condition, format, ...)			\
// /*AFLA*/ ({								\
// /*AFLA*/ 	static DEFINE_RATELIMIT_STATE(_rs,			\
// /*AFLA*/ 				      DEFAULT_RATELIMIT_INTERVAL,	\
// /*AFLA*/ 				      DEFAULT_RATELIMIT_BURST);	\
// /*AFLA*/ 	int rtn = !!(condition);				\
// /*AFLA*/ 								\
// /*AFLA*/ 	if (unlikely(rtn && __ratelimit(&_rs)))			\
// /*AFLA*/ 		WARN(rtn, format, ##__VA_ARGS__);		\
// /*AFLA*/ 								\
// /*AFLA*/ 	rtn;							\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ #define WARN_ON_RATELIMIT(condition, state)			\
// /*AFLA*/ 	WARN_ON(condition)
// /*AFLA*/ 
// /*AFLA*/ #define WARN_RATELIMIT(condition, format, ...)			\
// /*AFLA*/ ({								\
// /*AFLA*/ 	int rtn = WARN(condition, format, ##__VA_ARGS__);	\
// /*AFLA*/ 	rtn;							\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_RATELIMIT_H */
