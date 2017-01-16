// /*AFLA*/ /*
// /*AFLA*/  *  include/linux/hrtimer.h
// /*AFLA*/  *
// /*AFLA*/  *  hrtimers - High-resolution kernel timers
// /*AFLA*/  *
// /*AFLA*/  *   Copyright(C) 2005, Thomas Gleixner <tglx@linutronix.de>
// /*AFLA*/  *   Copyright(C) 2005, Red Hat, Inc., Ingo Molnar
// /*AFLA*/  *
// /*AFLA*/  *  data type definitions, declarations, prototypes
// /*AFLA*/  *
// /*AFLA*/  *  Started by: Thomas Gleixner and Ingo Molnar
// /*AFLA*/  *
// /*AFLA*/  *  For licencing details see kernel-base/COPYING
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LINUX_HRTIMER_H
// /*AFLA*/ #define _LINUX_HRTIMER_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/rbtree.h>
// /*AFLA*/ #include <linux/ktime.h>
// /*AFLA*/ #include <linux/init.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/wait.h>
// /*AFLA*/ #include <linux/percpu.h>
// /*AFLA*/ #include <linux/timer.h>
// /*AFLA*/ #include <linux/timerqueue.h>
// /*AFLA*/ 
// /*AFLA*/ struct hrtimer_clock_base;
// /*AFLA*/ struct hrtimer_cpu_base;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Mode arguments of xxx_hrtimer functions:
// /*AFLA*/  */
// /*AFLA*/ enum hrtimer_mode {
// /*AFLA*/ 	HRTIMER_MODE_ABS = 0x0,		/* Time value is absolute */
// /*AFLA*/ 	HRTIMER_MODE_REL = 0x1,		/* Time value is relative to now */
// /*AFLA*/ 	HRTIMER_MODE_PINNED = 0x02,	/* Timer is bound to CPU */
// /*AFLA*/ 	HRTIMER_MODE_ABS_PINNED = 0x02,
// /*AFLA*/ 	HRTIMER_MODE_REL_PINNED = 0x03,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Return values for the callback function
// /*AFLA*/  */
// /*AFLA*/ enum hrtimer_restart {
// /*AFLA*/ 	HRTIMER_NORESTART,	/* Timer is not restarted */
// /*AFLA*/ 	HRTIMER_RESTART,	/* Timer must be restarted */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Values to track state of the timer
// /*AFLA*/  *
// /*AFLA*/  * Possible states:
// /*AFLA*/  *
// /*AFLA*/  * 0x00		inactive
// /*AFLA*/  * 0x01		enqueued into rbtree
// /*AFLA*/  *
// /*AFLA*/  * The callback state is not part of the timer->state because clearing it would
// /*AFLA*/  * mean touching the timer after the callback, this makes it impossible to free
// /*AFLA*/  * the timer from the callback function.
// /*AFLA*/  *
// /*AFLA*/  * Therefore we track the callback state in:
// /*AFLA*/  *
// /*AFLA*/  *	timer->base->cpu_base->running == timer
// /*AFLA*/  *
// /*AFLA*/  * On SMP it is possible to have a "callback function running and enqueued"
// /*AFLA*/  * status. It happens for example when a posix timer expired and the callback
// /*AFLA*/  * queued a signal. Between dropping the lock which protects the posix timer
// /*AFLA*/  * and reacquiring the base lock of the hrtimer, another CPU can deliver the
// /*AFLA*/  * signal and rearm the timer.
// /*AFLA*/  *
// /*AFLA*/  * All state transitions are protected by cpu_base->lock.
// /*AFLA*/  */
// /*AFLA*/ #define HRTIMER_STATE_INACTIVE	0x00
// /*AFLA*/ #define HRTIMER_STATE_ENQUEUED	0x01
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct hrtimer - the basic hrtimer structure
// /*AFLA*/  * @node:	timerqueue node, which also manages node.expires,
// /*AFLA*/  *		the absolute expiry time in the hrtimers internal
// /*AFLA*/  *		representation. The time is related to the clock on
// /*AFLA*/  *		which the timer is based. Is setup by adding
// /*AFLA*/  *		slack to the _softexpires value. For non range timers
// /*AFLA*/  *		identical to _softexpires.
// /*AFLA*/  * @_softexpires: the absolute earliest expiry time of the hrtimer.
// /*AFLA*/  *		The time which was given as expiry time when the timer
// /*AFLA*/  *		was armed.
// /*AFLA*/  * @function:	timer expiry callback function
// /*AFLA*/  * @base:	pointer to the timer base (per cpu and per clock)
// /*AFLA*/  * @state:	state information (See bit values above)
// /*AFLA*/  * @is_rel:	Set if the timer was armed relative
// /*AFLA*/  * @start_pid:  timer statistics field to store the pid of the task which
// /*AFLA*/  *		started the timer
// /*AFLA*/  * @start_site:	timer statistics field to store the site where the timer
// /*AFLA*/  *		was started
// /*AFLA*/  * @start_comm: timer statistics field to store the name of the process which
// /*AFLA*/  *		started the timer
// /*AFLA*/  *
// /*AFLA*/  * The hrtimer structure must be initialized by hrtimer_init()
// /*AFLA*/  */
// /*AFLA*/ struct hrtimer {
// /*AFLA*/ 	struct timerqueue_node		node;
// /*AFLA*/ 	ktime_t				_softexpires;
// /*AFLA*/ 	enum hrtimer_restart		(*function)(struct hrtimer *);
// /*AFLA*/ 	struct hrtimer_clock_base	*base;
// /*AFLA*/ 	u8				state;
// /*AFLA*/ 	u8				is_rel;
// /*AFLA*/ #ifdef CONFIG_TIMER_STATS
// /*AFLA*/ 	int				start_pid;
// /*AFLA*/ 	void				*start_site;
// /*AFLA*/ 	char				start_comm[16];
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct hrtimer_sleeper - simple sleeper structure
// /*AFLA*/  * @timer:	embedded timer structure
// /*AFLA*/  * @task:	task to wake up
// /*AFLA*/  *
// /*AFLA*/  * task is set to NULL, when the timer expires.
// /*AFLA*/  */
// /*AFLA*/ struct hrtimer_sleeper {
// /*AFLA*/ 	struct hrtimer timer;
// /*AFLA*/ 	struct task_struct *task;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_64BIT
// /*AFLA*/ # define HRTIMER_CLOCK_BASE_ALIGN	64
// /*AFLA*/ #else
// /*AFLA*/ # define HRTIMER_CLOCK_BASE_ALIGN	32
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct hrtimer_clock_base - the timer base for a specific clock
// /*AFLA*/  * @cpu_base:		per cpu clock base
// /*AFLA*/  * @index:		clock type index for per_cpu support when moving a
// /*AFLA*/  *			timer to a base on another cpu.
// /*AFLA*/  * @clockid:		clock id for per_cpu support
// /*AFLA*/  * @active:		red black tree root node for the active timers
// /*AFLA*/  * @get_time:		function to retrieve the current time of the clock
// /*AFLA*/  * @offset:		offset of this clock to the monotonic base
// /*AFLA*/  */
// /*AFLA*/ struct hrtimer_clock_base {
// /*AFLA*/ 	struct hrtimer_cpu_base	*cpu_base;
// /*AFLA*/ 	int			index;
// /*AFLA*/ 	clockid_t		clockid;
// /*AFLA*/ 	struct timerqueue_head	active;
// /*AFLA*/ 	ktime_t			(*get_time)(void);
// /*AFLA*/ 	ktime_t			offset;
// /*AFLA*/ } __attribute__((__aligned__(HRTIMER_CLOCK_BASE_ALIGN)));
// /*AFLA*/ 
// /*AFLA*/ enum  hrtimer_base_type {
// /*AFLA*/ 	HRTIMER_BASE_MONOTONIC,
// /*AFLA*/ 	HRTIMER_BASE_REALTIME,
// /*AFLA*/ 	HRTIMER_BASE_BOOTTIME,
// /*AFLA*/ 	HRTIMER_BASE_TAI,
// /*AFLA*/ 	HRTIMER_MAX_CLOCK_BASES,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * struct hrtimer_cpu_base - the per cpu clock bases
// /*AFLA*/  * @lock:		lock protecting the base and associated clock bases
// /*AFLA*/  *			and timers
// /*AFLA*/  * @seq:		seqcount around __run_hrtimer
// /*AFLA*/  * @running:		pointer to the currently running hrtimer
// /*AFLA*/  * @cpu:		cpu number
// /*AFLA*/  * @active_bases:	Bitfield to mark bases with active timers
// /*AFLA*/  * @clock_was_set_seq:	Sequence counter of clock was set events
// /*AFLA*/  * @migration_enabled:	The migration of hrtimers to other cpus is enabled
// /*AFLA*/  * @nohz_active:	The nohz functionality is enabled
// /*AFLA*/  * @expires_next:	absolute time of the next event which was scheduled
// /*AFLA*/  *			via clock_set_next_event()
// /*AFLA*/  * @next_timer:		Pointer to the first expiring timer
// /*AFLA*/  * @in_hrtirq:		hrtimer_interrupt() is currently executing
// /*AFLA*/  * @hres_active:	State of high resolution mode
// /*AFLA*/  * @hang_detected:	The last hrtimer interrupt detected a hang
// /*AFLA*/  * @nr_events:		Total number of hrtimer interrupt events
// /*AFLA*/  * @nr_retries:		Total number of hrtimer interrupt retries
// /*AFLA*/  * @nr_hangs:		Total number of hrtimer interrupt hangs
// /*AFLA*/  * @max_hang_time:	Maximum time spent in hrtimer_interrupt
// /*AFLA*/  * @clock_base:		array of clock bases for this cpu
// /*AFLA*/  *
// /*AFLA*/  * Note: next_timer is just an optimization for __remove_hrtimer().
// /*AFLA*/  *	 Do not dereference the pointer because it is not reliable on
// /*AFLA*/  *	 cross cpu removals.
// /*AFLA*/  */
// /*AFLA*/ struct hrtimer_cpu_base {
// /*AFLA*/ 	raw_spinlock_t			lock;
// /*AFLA*/ 	seqcount_t			seq;
// /*AFLA*/ 	struct hrtimer			*running;
// /*AFLA*/ 	unsigned int			cpu;
// /*AFLA*/ 	unsigned int			active_bases;
// /*AFLA*/ 	unsigned int			clock_was_set_seq;
// /*AFLA*/ 	bool				migration_enabled;
// /*AFLA*/ 	bool				nohz_active;
// /*AFLA*/ #ifdef CONFIG_HIGH_RES_TIMERS
// /*AFLA*/ 	unsigned int			in_hrtirq	: 1,
// /*AFLA*/ 					hres_active	: 1,
// /*AFLA*/ 					hang_detected	: 1;
// /*AFLA*/ 	ktime_t				expires_next;
// /*AFLA*/ 	struct hrtimer			*next_timer;
// /*AFLA*/ 	unsigned int			nr_events;
// /*AFLA*/ 	unsigned int			nr_retries;
// /*AFLA*/ 	unsigned int			nr_hangs;
// /*AFLA*/ 	unsigned int			max_hang_time;
// /*AFLA*/ #endif
// /*AFLA*/ 	struct hrtimer_clock_base	clock_base[HRTIMER_MAX_CLOCK_BASES];
// /*AFLA*/ } ____cacheline_aligned;
// /*AFLA*/ 
// /*AFLA*/ static inline void hrtimer_set_expires(struct hrtimer *timer, ktime_t time)
// /*AFLA*/ {
// /*AFLA*/ 	BUILD_BUG_ON(sizeof(struct hrtimer_clock_base) > HRTIMER_CLOCK_BASE_ALIGN);
// /*AFLA*/ 
// /*AFLA*/ 	timer->node.expires = time;
// /*AFLA*/ 	timer->_softexpires = time;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void hrtimer_set_expires_range(struct hrtimer *timer, ktime_t time, ktime_t delta)
// /*AFLA*/ {
// /*AFLA*/ 	timer->_softexpires = time;
// /*AFLA*/ 	timer->node.expires = ktime_add_safe(time, delta);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void hrtimer_set_expires_range_ns(struct hrtimer *timer, ktime_t time, u64 delta)
// /*AFLA*/ {
// /*AFLA*/ 	timer->_softexpires = time;
// /*AFLA*/ 	timer->node.expires = ktime_add_safe(time, ns_to_ktime(delta));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void hrtimer_set_expires_tv64(struct hrtimer *timer, s64 tv64)
// /*AFLA*/ {
// /*AFLA*/ 	timer->node.expires.tv64 = tv64;
// /*AFLA*/ 	timer->_softexpires.tv64 = tv64;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void hrtimer_add_expires(struct hrtimer *timer, ktime_t time)
// /*AFLA*/ {
// /*AFLA*/ 	timer->node.expires = ktime_add_safe(timer->node.expires, time);
// /*AFLA*/ 	timer->_softexpires = ktime_add_safe(timer->_softexpires, time);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void hrtimer_add_expires_ns(struct hrtimer *timer, u64 ns)
// /*AFLA*/ {
// /*AFLA*/ 	timer->node.expires = ktime_add_ns(timer->node.expires, ns);
// /*AFLA*/ 	timer->_softexpires = ktime_add_ns(timer->_softexpires, ns);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline ktime_t hrtimer_get_expires(const struct hrtimer *timer)
// /*AFLA*/ {
// /*AFLA*/ 	return timer->node.expires;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline ktime_t hrtimer_get_softexpires(const struct hrtimer *timer)
// /*AFLA*/ {
// /*AFLA*/ 	return timer->_softexpires;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline s64 hrtimer_get_expires_tv64(const struct hrtimer *timer)
// /*AFLA*/ {
// /*AFLA*/ 	return timer->node.expires.tv64;
// /*AFLA*/ }
// /*AFLA*/ static inline s64 hrtimer_get_softexpires_tv64(const struct hrtimer *timer)
// /*AFLA*/ {
// /*AFLA*/ 	return timer->_softexpires.tv64;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline s64 hrtimer_get_expires_ns(const struct hrtimer *timer)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_to_ns(timer->node.expires);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline ktime_t hrtimer_expires_remaining(const struct hrtimer *timer)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_sub(timer->node.expires, timer->base->get_time());
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline ktime_t hrtimer_cb_get_time(struct hrtimer *timer)
// /*AFLA*/ {
// /*AFLA*/ 	return timer->base->get_time();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HIGH_RES_TIMERS
// /*AFLA*/ struct clock_event_device;
// /*AFLA*/ 
// /*AFLA*/ extern void hrtimer_interrupt(struct clock_event_device *dev);
// /*AFLA*/ 
// /*AFLA*/ static inline int hrtimer_is_hres_active(struct hrtimer *timer)
// /*AFLA*/ {
// /*AFLA*/ 	return timer->base->cpu_base->hres_active;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void hrtimer_peek_ahead_timers(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The resolution of the clocks. The resolution value is returned in
// /*AFLA*/  * the clock_getres() system call to give application programmers an
// /*AFLA*/  * idea of the (in)accuracy of timers. Timer values are rounded up to
// /*AFLA*/  * this resolution values.
// /*AFLA*/  */
// /*AFLA*/ # define HIGH_RES_NSEC		1
// /*AFLA*/ # define KTIME_HIGH_RES		(ktime_t) { .tv64 = HIGH_RES_NSEC }
// /*AFLA*/ # define MONOTONIC_RES_NSEC	HIGH_RES_NSEC
// /*AFLA*/ # define KTIME_MONOTONIC_RES	KTIME_HIGH_RES
// /*AFLA*/ 
// /*AFLA*/ extern void clock_was_set_delayed(void);
// /*AFLA*/ 
// /*AFLA*/ extern unsigned int hrtimer_resolution;
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ # define MONOTONIC_RES_NSEC	LOW_RES_NSEC
// /*AFLA*/ # define KTIME_MONOTONIC_RES	KTIME_LOW_RES
// /*AFLA*/ 
// /*AFLA*/ #define hrtimer_resolution	(unsigned int)LOW_RES_NSEC
// /*AFLA*/ 
// /*AFLA*/ static inline void hrtimer_peek_ahead_timers(void) { }
// /*AFLA*/ 
// /*AFLA*/ static inline int hrtimer_is_hres_active(struct hrtimer *timer)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void clock_was_set_delayed(void) { }
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline ktime_t
// /*AFLA*/ __hrtimer_expires_remaining_adjusted(const struct hrtimer *timer, ktime_t now)
// /*AFLA*/ {
// /*AFLA*/ 	ktime_t rem = ktime_sub(timer->node.expires, now);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Adjust relative timers for the extra we added in
// /*AFLA*/ 	 * hrtimer_start_range_ns() to prevent short timeouts.
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (IS_ENABLED(CONFIG_TIME_LOW_RES) && timer->is_rel)
// /*AFLA*/ 		rem.tv64 -= hrtimer_resolution;
// /*AFLA*/ 	return rem;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline ktime_t
// /*AFLA*/ hrtimer_expires_remaining_adjusted(const struct hrtimer *timer)
// /*AFLA*/ {
// /*AFLA*/ 	return __hrtimer_expires_remaining_adjusted(timer,
// /*AFLA*/ 						    timer->base->get_time());
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void clock_was_set(void);
// /*AFLA*/ #ifdef CONFIG_TIMERFD
// /*AFLA*/ extern void timerfd_clock_was_set(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void timerfd_clock_was_set(void) { }
// /*AFLA*/ #endif
// /*AFLA*/ extern void hrtimers_resume(void);
// /*AFLA*/ 
// /*AFLA*/ DECLARE_PER_CPU(struct tick_device, tick_cpu_device);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* Exported timer functions: */
// /*AFLA*/ 
// /*AFLA*/ /* Initialize timers: */
// /*AFLA*/ extern void hrtimer_init(struct hrtimer *timer, clockid_t which_clock,
// /*AFLA*/ 			 enum hrtimer_mode mode);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_OBJECTS_TIMERS
// /*AFLA*/ extern void hrtimer_init_on_stack(struct hrtimer *timer, clockid_t which_clock,
// /*AFLA*/ 				  enum hrtimer_mode mode);
// /*AFLA*/ 
// /*AFLA*/ extern void destroy_hrtimer_on_stack(struct hrtimer *timer);
// /*AFLA*/ #else
// /*AFLA*/ static inline void hrtimer_init_on_stack(struct hrtimer *timer,
// /*AFLA*/ 					 clockid_t which_clock,
// /*AFLA*/ 					 enum hrtimer_mode mode)
// /*AFLA*/ {
// /*AFLA*/ 	hrtimer_init(timer, which_clock, mode);
// /*AFLA*/ }
// /*AFLA*/ static inline void destroy_hrtimer_on_stack(struct hrtimer *timer) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Basic timer operations: */
// /*AFLA*/ extern void hrtimer_start_range_ns(struct hrtimer *timer, ktime_t tim,
// /*AFLA*/ 				   u64 range_ns, const enum hrtimer_mode mode);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hrtimer_start - (re)start an hrtimer on the current CPU
// /*AFLA*/  * @timer:	the timer to be added
// /*AFLA*/  * @tim:	expiry time
// /*AFLA*/  * @mode:	expiry mode: absolute (HRTIMER_MODE_ABS) or
// /*AFLA*/  *		relative (HRTIMER_MODE_REL)
// /*AFLA*/  */
// /*AFLA*/ static inline void hrtimer_start(struct hrtimer *timer, ktime_t tim,
// /*AFLA*/ 				 const enum hrtimer_mode mode)
// /*AFLA*/ {
// /*AFLA*/ 	hrtimer_start_range_ns(timer, tim, 0, mode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int hrtimer_cancel(struct hrtimer *timer);
// /*AFLA*/ extern int hrtimer_try_to_cancel(struct hrtimer *timer);
// /*AFLA*/ 
// /*AFLA*/ static inline void hrtimer_start_expires(struct hrtimer *timer,
// /*AFLA*/ 					 enum hrtimer_mode mode)
// /*AFLA*/ {
// /*AFLA*/ 	u64 delta;
// /*AFLA*/ 	ktime_t soft, hard;
// /*AFLA*/ 	soft = hrtimer_get_softexpires(timer);
// /*AFLA*/ 	hard = hrtimer_get_expires(timer);
// /*AFLA*/ 	delta = ktime_to_ns(ktime_sub(hard, soft));
// /*AFLA*/ 	hrtimer_start_range_ns(timer, soft, delta, mode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void hrtimer_restart(struct hrtimer *timer)
// /*AFLA*/ {
// /*AFLA*/ 	hrtimer_start_expires(timer, HRTIMER_MODE_ABS);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Query timers: */
// /*AFLA*/ extern ktime_t __hrtimer_get_remaining(const struct hrtimer *timer, bool adjust);
// /*AFLA*/ 
// /*AFLA*/ static inline ktime_t hrtimer_get_remaining(const struct hrtimer *timer)
// /*AFLA*/ {
// /*AFLA*/ 	return __hrtimer_get_remaining(timer, false);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern u64 hrtimer_get_next_event(void);
// /*AFLA*/ 
// /*AFLA*/ extern bool hrtimer_active(const struct hrtimer *timer);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Helper function to check, whether the timer is on one of the queues
// /*AFLA*/  */
// /*AFLA*/ static inline int hrtimer_is_queued(struct hrtimer *timer)
// /*AFLA*/ {
// /*AFLA*/ 	return timer->state & HRTIMER_STATE_ENQUEUED;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Helper function to check, whether the timer is running the callback
// /*AFLA*/  * function
// /*AFLA*/  */
// /*AFLA*/ static inline int hrtimer_callback_running(struct hrtimer *timer)
// /*AFLA*/ {
// /*AFLA*/ 	return timer->base->cpu_base->running == timer;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Forward a hrtimer so it expires after now: */
// /*AFLA*/ extern u64
// /*AFLA*/ hrtimer_forward(struct hrtimer *timer, ktime_t now, ktime_t interval);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * hrtimer_forward_now - forward the timer expiry so it expires after now
// /*AFLA*/  * @timer:	hrtimer to forward
// /*AFLA*/  * @interval:	the interval to forward
// /*AFLA*/  *
// /*AFLA*/  * Forward the timer expiry so it will expire after the current time
// /*AFLA*/  * of the hrtimer clock base. Returns the number of overruns.
// /*AFLA*/  *
// /*AFLA*/  * Can be safely called from the callback function of @timer. If
// /*AFLA*/  * called from other contexts @timer must neither be enqueued nor
// /*AFLA*/  * running the callback and the caller needs to take care of
// /*AFLA*/  * serialization.
// /*AFLA*/  *
// /*AFLA*/  * Note: This only updates the timer expiry value and does not requeue
// /*AFLA*/  * the timer.
// /*AFLA*/  */
// /*AFLA*/ static inline u64 hrtimer_forward_now(struct hrtimer *timer,
// /*AFLA*/ 				      ktime_t interval)
// /*AFLA*/ {
// /*AFLA*/ 	return hrtimer_forward(timer, timer->base->get_time(), interval);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Precise sleep: */
// /*AFLA*/ extern long hrtimer_nanosleep(struct timespec *rqtp,
// /*AFLA*/ 			      struct timespec __user *rmtp,
// /*AFLA*/ 			      const enum hrtimer_mode mode,
// /*AFLA*/ 			      const clockid_t clockid);
// /*AFLA*/ extern long hrtimer_nanosleep_restart(struct restart_block *restart_block);
// /*AFLA*/ 
// /*AFLA*/ extern void hrtimer_init_sleeper(struct hrtimer_sleeper *sl,
// /*AFLA*/ 				 struct task_struct *tsk);
// /*AFLA*/ 
// /*AFLA*/ extern int schedule_hrtimeout_range(ktime_t *expires, u64 delta,
// /*AFLA*/ 						const enum hrtimer_mode mode);
// /*AFLA*/ extern int schedule_hrtimeout_range_clock(ktime_t *expires,
// /*AFLA*/ 					  u64 delta,
// /*AFLA*/ 					  const enum hrtimer_mode mode,
// /*AFLA*/ 					  int clock);
// /*AFLA*/ extern int schedule_hrtimeout(ktime_t *expires, const enum hrtimer_mode mode);
// /*AFLA*/ 
// /*AFLA*/ /* Soft interrupt function to run the hrtimer queues: */
// /*AFLA*/ extern void hrtimer_run_queues(void);
// /*AFLA*/ 
// /*AFLA*/ /* Bootup initialization: */
// /*AFLA*/ extern void __init hrtimers_init(void);
// /*AFLA*/ 
// /*AFLA*/ /* Show pending timers: */
// /*AFLA*/ extern void sysrq_timer_list_show(void);
// /*AFLA*/ 
// /*AFLA*/ int hrtimers_prepare_cpu(unsigned int cpu);
// /*AFLA*/ #ifdef CONFIG_HOTPLUG_CPU
// /*AFLA*/ int hrtimers_dead_cpu(unsigned int cpu);
// /*AFLA*/ #else
// /*AFLA*/ #define hrtimers_dead_cpu	NULL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
