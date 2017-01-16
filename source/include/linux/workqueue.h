// /*AFLA*/ /*
// /*AFLA*/  * workqueue.h --- work queue handling for Linux.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_WORKQUEUE_H
// /*AFLA*/ #define _LINUX_WORKQUEUE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/timer.h>
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ #include <linux/bitops.h>
// /*AFLA*/ #include <linux/lockdep.h>
// /*AFLA*/ #include <linux/threads.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ 
// /*AFLA*/ struct workqueue_struct;
// /*AFLA*/ 
// /*AFLA*/ struct work_struct;
// /*AFLA*/ typedef void (*work_func_t)(struct work_struct *work);
// /*AFLA*/ void delayed_work_timer_fn(unsigned long __data);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The first word is the work queue pointer and the flags rolled into
// /*AFLA*/  * one
// /*AFLA*/  */
// /*AFLA*/ #define work_data_bits(work) ((unsigned long *)(&(work)->data))
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	WORK_STRUCT_PENDING_BIT	= 0,	/* work item is pending execution */
// /*AFLA*/ 	WORK_STRUCT_DELAYED_BIT	= 1,	/* work item is delayed */
// /*AFLA*/ 	WORK_STRUCT_PWQ_BIT	= 2,	/* data points to pwq */
// /*AFLA*/ 	WORK_STRUCT_LINKED_BIT	= 3,	/* next work is linked to this one */
// /*AFLA*/ #ifdef CONFIG_DEBUG_OBJECTS_WORK
// /*AFLA*/ 	WORK_STRUCT_STATIC_BIT	= 4,	/* static initializer (debugobjects) */
// /*AFLA*/ 	WORK_STRUCT_COLOR_SHIFT	= 5,	/* color for workqueue flushing */
// /*AFLA*/ #else
// /*AFLA*/ 	WORK_STRUCT_COLOR_SHIFT	= 4,	/* color for workqueue flushing */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	WORK_STRUCT_COLOR_BITS	= 4,
// /*AFLA*/ 
// /*AFLA*/ 	WORK_STRUCT_PENDING	= 1 << WORK_STRUCT_PENDING_BIT,
// /*AFLA*/ 	WORK_STRUCT_DELAYED	= 1 << WORK_STRUCT_DELAYED_BIT,
// /*AFLA*/ 	WORK_STRUCT_PWQ		= 1 << WORK_STRUCT_PWQ_BIT,
// /*AFLA*/ 	WORK_STRUCT_LINKED	= 1 << WORK_STRUCT_LINKED_BIT,
// /*AFLA*/ #ifdef CONFIG_DEBUG_OBJECTS_WORK
// /*AFLA*/ 	WORK_STRUCT_STATIC	= 1 << WORK_STRUCT_STATIC_BIT,
// /*AFLA*/ #else
// /*AFLA*/ 	WORK_STRUCT_STATIC	= 0,
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The last color is no color used for works which don't
// /*AFLA*/ 	 * participate in workqueue flushing.
// /*AFLA*/ 	 */
// /*AFLA*/ 	WORK_NR_COLORS		= (1 << WORK_STRUCT_COLOR_BITS) - 1,
// /*AFLA*/ 	WORK_NO_COLOR		= WORK_NR_COLORS,
// /*AFLA*/ 
// /*AFLA*/ 	/* not bound to any CPU, prefer the local CPU */
// /*AFLA*/ 	WORK_CPU_UNBOUND	= NR_CPUS,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Reserve 7 bits off of pwq pointer w/ debugobjects turned off.
// /*AFLA*/ 	 * This makes pwqs aligned to 256 bytes and allows 15 workqueue
// /*AFLA*/ 	 * flush colors.
// /*AFLA*/ 	 */
// /*AFLA*/ 	WORK_STRUCT_FLAG_BITS	= WORK_STRUCT_COLOR_SHIFT +
// /*AFLA*/ 				  WORK_STRUCT_COLOR_BITS,
// /*AFLA*/ 
// /*AFLA*/ 	/* data contains off-queue information when !WORK_STRUCT_PWQ */
// /*AFLA*/ 	WORK_OFFQ_FLAG_BASE	= WORK_STRUCT_COLOR_SHIFT,
// /*AFLA*/ 
// /*AFLA*/ 	__WORK_OFFQ_CANCELING	= WORK_OFFQ_FLAG_BASE,
// /*AFLA*/ 	WORK_OFFQ_CANCELING	= (1 << __WORK_OFFQ_CANCELING),
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * When a work item is off queue, its high bits point to the last
// /*AFLA*/ 	 * pool it was on.  Cap at 31 bits and use the highest number to
// /*AFLA*/ 	 * indicate that no pool is associated.
// /*AFLA*/ 	 */
// /*AFLA*/ 	WORK_OFFQ_FLAG_BITS	= 1,
// /*AFLA*/ 	WORK_OFFQ_POOL_SHIFT	= WORK_OFFQ_FLAG_BASE + WORK_OFFQ_FLAG_BITS,
// /*AFLA*/ 	WORK_OFFQ_LEFT		= BITS_PER_LONG - WORK_OFFQ_POOL_SHIFT,
// /*AFLA*/ 	WORK_OFFQ_POOL_BITS	= WORK_OFFQ_LEFT <= 31 ? WORK_OFFQ_LEFT : 31,
// /*AFLA*/ 	WORK_OFFQ_POOL_NONE	= (1LU << WORK_OFFQ_POOL_BITS) - 1,
// /*AFLA*/ 
// /*AFLA*/ 	/* convenience constants */
// /*AFLA*/ 	WORK_STRUCT_FLAG_MASK	= (1UL << WORK_STRUCT_FLAG_BITS) - 1,
// /*AFLA*/ 	WORK_STRUCT_WQ_DATA_MASK = ~WORK_STRUCT_FLAG_MASK,
// /*AFLA*/ 	WORK_STRUCT_NO_POOL	= (unsigned long)WORK_OFFQ_POOL_NONE << WORK_OFFQ_POOL_SHIFT,
// /*AFLA*/ 
// /*AFLA*/ 	/* bit mask for work_busy() return values */
// /*AFLA*/ 	WORK_BUSY_PENDING	= 1 << 0,
// /*AFLA*/ 	WORK_BUSY_RUNNING	= 1 << 1,
// /*AFLA*/ 
// /*AFLA*/ 	/* maximum string length for set_worker_desc() */
// /*AFLA*/ 	WORKER_DESC_LEN		= 24,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct work_struct {
// /*AFLA*/ 	atomic_long_t data;
// /*AFLA*/ 	struct list_head entry;
// /*AFLA*/ 	work_func_t func;
// /*AFLA*/ #ifdef CONFIG_LOCKDEP
// /*AFLA*/ 	struct lockdep_map lockdep_map;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define WORK_DATA_INIT()	ATOMIC_LONG_INIT(WORK_STRUCT_NO_POOL)
// /*AFLA*/ #define WORK_DATA_STATIC_INIT()	\
// /*AFLA*/ 	ATOMIC_LONG_INIT(WORK_STRUCT_NO_POOL | WORK_STRUCT_STATIC)
// /*AFLA*/ 
// /*AFLA*/ struct delayed_work {
// /*AFLA*/ 	struct work_struct work;
// /*AFLA*/ 	struct timer_list timer;
// /*AFLA*/ 
// /*AFLA*/ 	/* target workqueue and CPU ->timer uses to queue ->work */
// /*AFLA*/ 	struct workqueue_struct *wq;
// /*AFLA*/ 	int cpu;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A struct for workqueue attributes.  This can be used to change
// /*AFLA*/  * attributes of an unbound workqueue.
// /*AFLA*/  *
// /*AFLA*/  * Unlike other fields, ->no_numa isn't a property of a worker_pool.  It
// /*AFLA*/  * only modifies how apply_workqueue_attrs() select pools and thus doesn't
// /*AFLA*/  * participate in pool hash calculations or equality comparisons.
// /*AFLA*/  */
// /*AFLA*/ struct workqueue_attrs {
// /*AFLA*/ 	int			nice;		/* nice level */
// /*AFLA*/ 	cpumask_var_t		cpumask;	/* allowed CPUs */
// /*AFLA*/ 	bool			no_numa;	/* disable NUMA affinity */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline struct delayed_work *to_delayed_work(struct work_struct *work)
// /*AFLA*/ {
// /*AFLA*/ 	return container_of(work, struct delayed_work, work);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct execute_work {
// /*AFLA*/ 	struct work_struct work;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_LOCKDEP
// /*AFLA*/ /*
// /*AFLA*/  * NB: because we have to copy the lockdep_map, setting _key
// /*AFLA*/  * here is required, otherwise it could get initialised to the
// /*AFLA*/  * copy of the lockdep_map!
// /*AFLA*/  */
// /*AFLA*/ #define __WORK_INIT_LOCKDEP_MAP(n, k) \
// /*AFLA*/ 	.lockdep_map = STATIC_LOCKDEP_MAP_INIT(n, k),
// /*AFLA*/ #else
// /*AFLA*/ #define __WORK_INIT_LOCKDEP_MAP(n, k)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __WORK_INITIALIZER(n, f) {					\
// /*AFLA*/ 	.data = WORK_DATA_STATIC_INIT(),				\
// /*AFLA*/ 	.entry	= { &(n).entry, &(n).entry },				\
// /*AFLA*/ 	.func = (f),							\
// /*AFLA*/ 	__WORK_INIT_LOCKDEP_MAP(#n, &(n))				\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ #define __DELAYED_WORK_INITIALIZER(n, f, tflags) {			\
// /*AFLA*/ 	.work = __WORK_INITIALIZER((n).work, (f)),			\
// /*AFLA*/ 	.timer = __TIMER_INITIALIZER(delayed_work_timer_fn,		\
// /*AFLA*/ 				     0, (unsigned long)&(n),		\
// /*AFLA*/ 				     (tflags) | TIMER_IRQSAFE),		\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_WORK(n, f)						\
// /*AFLA*/ 	struct work_struct n = __WORK_INITIALIZER(n, f)
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_DELAYED_WORK(n, f)					\
// /*AFLA*/ 	struct delayed_work n = __DELAYED_WORK_INITIALIZER(n, f, 0)
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_DEFERRABLE_WORK(n, f)					\
// /*AFLA*/ 	struct delayed_work n = __DELAYED_WORK_INITIALIZER(n, f, TIMER_DEFERRABLE)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_OBJECTS_WORK
// /*AFLA*/ extern void __init_work(struct work_struct *work, int onstack);
// /*AFLA*/ extern void destroy_work_on_stack(struct work_struct *work);
// /*AFLA*/ extern void destroy_delayed_work_on_stack(struct delayed_work *work);
// /*AFLA*/ static inline unsigned int work_static(struct work_struct *work)
// /*AFLA*/ {
// /*AFLA*/ 	return *work_data_bits(work) & WORK_STRUCT_STATIC;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline void __init_work(struct work_struct *work, int onstack) { }
// /*AFLA*/ static inline void destroy_work_on_stack(struct work_struct *work) { }
// /*AFLA*/ static inline void destroy_delayed_work_on_stack(struct delayed_work *work) { }
// /*AFLA*/ static inline unsigned int work_static(struct work_struct *work) { return 0; }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * initialize all of a work item in one go
// /*AFLA*/  *
// /*AFLA*/  * NOTE! No point in using "atomic_long_set()": using a direct
// /*AFLA*/  * assignment of the work data initializer allows the compiler
// /*AFLA*/  * to generate better code.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_LOCKDEP
// /*AFLA*/ #define __INIT_WORK(_work, _func, _onstack)				\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		static struct lock_class_key __key;			\
// /*AFLA*/ 									\
// /*AFLA*/ 		__init_work((_work), _onstack);				\
// /*AFLA*/ 		(_work)->data = (atomic_long_t) WORK_DATA_INIT();	\
// /*AFLA*/ 		lockdep_init_map(&(_work)->lockdep_map, #_work, &__key, 0); \
// /*AFLA*/ 		INIT_LIST_HEAD(&(_work)->entry);			\
// /*AFLA*/ 		(_work)->func = (_func);				\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #else
// /*AFLA*/ #define __INIT_WORK(_work, _func, _onstack)				\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		__init_work((_work), _onstack);				\
// /*AFLA*/ 		(_work)->data = (atomic_long_t) WORK_DATA_INIT();	\
// /*AFLA*/ 		INIT_LIST_HEAD(&(_work)->entry);			\
// /*AFLA*/ 		(_work)->func = (_func);				\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define INIT_WORK(_work, _func)						\
// /*AFLA*/ 	__INIT_WORK((_work), (_func), 0)
// /*AFLA*/ 
// /*AFLA*/ #define INIT_WORK_ONSTACK(_work, _func)					\
// /*AFLA*/ 	__INIT_WORK((_work), (_func), 1)
// /*AFLA*/ 
// /*AFLA*/ #define __INIT_DELAYED_WORK(_work, _func, _tflags)			\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		INIT_WORK(&(_work)->work, (_func));			\
// /*AFLA*/ 		__setup_timer(&(_work)->timer, delayed_work_timer_fn,	\
// /*AFLA*/ 			      (unsigned long)(_work),			\
// /*AFLA*/ 			      (_tflags) | TIMER_IRQSAFE);		\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #define __INIT_DELAYED_WORK_ONSTACK(_work, _func, _tflags)		\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		INIT_WORK_ONSTACK(&(_work)->work, (_func));		\
// /*AFLA*/ 		__setup_timer_on_stack(&(_work)->timer,			\
// /*AFLA*/ 				       delayed_work_timer_fn,		\
// /*AFLA*/ 				       (unsigned long)(_work),		\
// /*AFLA*/ 				       (_tflags) | TIMER_IRQSAFE);	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #define INIT_DELAYED_WORK(_work, _func)					\
// /*AFLA*/ 	__INIT_DELAYED_WORK(_work, _func, 0)
// /*AFLA*/ 
// /*AFLA*/ #define INIT_DELAYED_WORK_ONSTACK(_work, _func)				\
// /*AFLA*/ 	__INIT_DELAYED_WORK_ONSTACK(_work, _func, 0)
// /*AFLA*/ 
// /*AFLA*/ #define INIT_DEFERRABLE_WORK(_work, _func)				\
// /*AFLA*/ 	__INIT_DELAYED_WORK(_work, _func, TIMER_DEFERRABLE)
// /*AFLA*/ 
// /*AFLA*/ #define INIT_DEFERRABLE_WORK_ONSTACK(_work, _func)			\
// /*AFLA*/ 	__INIT_DELAYED_WORK_ONSTACK(_work, _func, TIMER_DEFERRABLE)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * work_pending - Find out whether a work item is currently pending
// /*AFLA*/  * @work: The work item in question
// /*AFLA*/  */
// /*AFLA*/ #define work_pending(work) \
// /*AFLA*/ 	test_bit(WORK_STRUCT_PENDING_BIT, work_data_bits(work))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * delayed_work_pending - Find out whether a delayable work item is currently
// /*AFLA*/  * pending
// /*AFLA*/  * @w: The work item in question
// /*AFLA*/  */
// /*AFLA*/ #define delayed_work_pending(w) \
// /*AFLA*/ 	work_pending(&(w)->work)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Workqueue flags and constants.  For details, please refer to
// /*AFLA*/  * Documentation/workqueue.txt.
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	WQ_UNBOUND		= 1 << 1, /* not bound to any cpu */
// /*AFLA*/ 	WQ_FREEZABLE		= 1 << 2, /* freeze during suspend */
// /*AFLA*/ 	WQ_MEM_RECLAIM		= 1 << 3, /* may be used for memory reclaim */
// /*AFLA*/ 	WQ_HIGHPRI		= 1 << 4, /* high priority */
// /*AFLA*/ 	WQ_CPU_INTENSIVE	= 1 << 5, /* cpu intensive workqueue */
// /*AFLA*/ 	WQ_SYSFS		= 1 << 6, /* visible in sysfs, see wq_sysfs_register() */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Per-cpu workqueues are generally preferred because they tend to
// /*AFLA*/ 	 * show better performance thanks to cache locality.  Per-cpu
// /*AFLA*/ 	 * workqueues exclude the scheduler from choosing the CPU to
// /*AFLA*/ 	 * execute the worker threads, which has an unfortunate side effect
// /*AFLA*/ 	 * of increasing power consumption.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * The scheduler considers a CPU idle if it doesn't have any task
// /*AFLA*/ 	 * to execute and tries to keep idle cores idle to conserve power;
// /*AFLA*/ 	 * however, for example, a per-cpu work item scheduled from an
// /*AFLA*/ 	 * interrupt handler on an idle CPU will force the scheduler to
// /*AFLA*/ 	 * excute the work item on that CPU breaking the idleness, which in
// /*AFLA*/ 	 * turn may lead to more scheduling choices which are sub-optimal
// /*AFLA*/ 	 * in terms of power consumption.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Workqueues marked with WQ_POWER_EFFICIENT are per-cpu by default
// /*AFLA*/ 	 * but become unbound if workqueue.power_efficient kernel param is
// /*AFLA*/ 	 * specified.  Per-cpu workqueues which are identified to
// /*AFLA*/ 	 * contribute significantly to power-consumption are identified and
// /*AFLA*/ 	 * marked with this flag and enabling the power_efficient mode
// /*AFLA*/ 	 * leads to noticeable power saving at the cost of small
// /*AFLA*/ 	 * performance disadvantage.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * http://thread.gmane.org/gmane.linux.kernel/1480396
// /*AFLA*/ 	 */
// /*AFLA*/ 	WQ_POWER_EFFICIENT	= 1 << 7,
// /*AFLA*/ 
// /*AFLA*/ 	__WQ_DRAINING		= 1 << 16, /* internal: workqueue is draining */
// /*AFLA*/ 	__WQ_ORDERED		= 1 << 17, /* internal: workqueue is ordered */
// /*AFLA*/ 	__WQ_LEGACY		= 1 << 18, /* internal: create*_workqueue() */
// /*AFLA*/ 
// /*AFLA*/ 	WQ_MAX_ACTIVE		= 512,	  /* I like 512, better ideas? */
// /*AFLA*/ 	WQ_MAX_UNBOUND_PER_CPU	= 4,	  /* 4 * #cpus for unbound wq */
// /*AFLA*/ 	WQ_DFL_ACTIVE		= WQ_MAX_ACTIVE / 2,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* unbound wq's aren't per-cpu, scale max_active according to #cpus */
// /*AFLA*/ #define WQ_UNBOUND_MAX_ACTIVE	\
// /*AFLA*/ 	max_t(int, WQ_MAX_ACTIVE, num_possible_cpus() * WQ_MAX_UNBOUND_PER_CPU)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * System-wide workqueues which are always present.
// /*AFLA*/  *
// /*AFLA*/  * system_wq is the one used by schedule[_delayed]_work[_on]().
// /*AFLA*/  * Multi-CPU multi-threaded.  There are users which expect relatively
// /*AFLA*/  * short queue flush time.  Don't queue works which can run for too
// /*AFLA*/  * long.
// /*AFLA*/  *
// /*AFLA*/  * system_highpri_wq is similar to system_wq but for work items which
// /*AFLA*/  * require WQ_HIGHPRI.
// /*AFLA*/  *
// /*AFLA*/  * system_long_wq is similar to system_wq but may host long running
// /*AFLA*/  * works.  Queue flushing might take relatively long.
// /*AFLA*/  *
// /*AFLA*/  * system_unbound_wq is unbound workqueue.  Workers are not bound to
// /*AFLA*/  * any specific CPU, not concurrency managed, and all queued works are
// /*AFLA*/  * executed immediately as long as max_active limit is not reached and
// /*AFLA*/  * resources are available.
// /*AFLA*/  *
// /*AFLA*/  * system_freezable_wq is equivalent to system_wq except that it's
// /*AFLA*/  * freezable.
// /*AFLA*/  *
// /*AFLA*/  * *_power_efficient_wq are inclined towards saving power and converted
// /*AFLA*/  * into WQ_UNBOUND variants if 'wq_power_efficient' is enabled; otherwise,
// /*AFLA*/  * they are same as their non-power-efficient counterparts - e.g.
// /*AFLA*/  * system_power_efficient_wq is identical to system_wq if
// /*AFLA*/  * 'wq_power_efficient' is disabled.  See WQ_POWER_EFFICIENT for more info.
// /*AFLA*/  */
// /*AFLA*/ extern struct workqueue_struct *system_wq;
// /*AFLA*/ extern struct workqueue_struct *system_highpri_wq;
// /*AFLA*/ extern struct workqueue_struct *system_long_wq;
// /*AFLA*/ extern struct workqueue_struct *system_unbound_wq;
// /*AFLA*/ extern struct workqueue_struct *system_freezable_wq;
// /*AFLA*/ extern struct workqueue_struct *system_power_efficient_wq;
// /*AFLA*/ extern struct workqueue_struct *system_freezable_power_efficient_wq;
// /*AFLA*/ 
// /*AFLA*/ extern struct workqueue_struct *
// /*AFLA*/ __alloc_workqueue_key(const char *fmt, unsigned int flags, int max_active,
// /*AFLA*/ 	struct lock_class_key *key, const char *lock_name, ...) __printf(1, 6);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * alloc_workqueue - allocate a workqueue
// /*AFLA*/  * @fmt: printf format for the name of the workqueue
// /*AFLA*/  * @flags: WQ_* flags
// /*AFLA*/  * @max_active: max in-flight work items, 0 for default
// /*AFLA*/  * @args...: args for @fmt
// /*AFLA*/  *
// /*AFLA*/  * Allocate a workqueue with the specified parameters.  For detailed
// /*AFLA*/  * information on WQ_* flags, please refer to Documentation/workqueue.txt.
// /*AFLA*/  *
// /*AFLA*/  * The __lock_name macro dance is to guarantee that single lock_class_key
// /*AFLA*/  * doesn't end up with different namesm, which isn't allowed by lockdep.
// /*AFLA*/  *
// /*AFLA*/  * RETURNS:
// /*AFLA*/  * Pointer to the allocated workqueue on success, %NULL on failure.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_LOCKDEP
// /*AFLA*/ #define alloc_workqueue(fmt, flags, max_active, args...)		\
// /*AFLA*/ ({									\
// /*AFLA*/ 	static struct lock_class_key __key;				\
// /*AFLA*/ 	const char *__lock_name;					\
// /*AFLA*/ 									\
// /*AFLA*/ 	__lock_name = #fmt#args;					\
// /*AFLA*/ 									\
// /*AFLA*/ 	__alloc_workqueue_key((fmt), (flags), (max_active),		\
// /*AFLA*/ 			      &__key, __lock_name, ##args);		\
// /*AFLA*/ })
// /*AFLA*/ #else
// /*AFLA*/ #define alloc_workqueue(fmt, flags, max_active, args...)		\
// /*AFLA*/ 	__alloc_workqueue_key((fmt), (flags), (max_active),		\
// /*AFLA*/ 			      NULL, NULL, ##args)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * alloc_ordered_workqueue - allocate an ordered workqueue
// /*AFLA*/  * @fmt: printf format for the name of the workqueue
// /*AFLA*/  * @flags: WQ_* flags (only WQ_FREEZABLE and WQ_MEM_RECLAIM are meaningful)
// /*AFLA*/  * @args...: args for @fmt
// /*AFLA*/  *
// /*AFLA*/  * Allocate an ordered workqueue.  An ordered workqueue executes at
// /*AFLA*/  * most one work item at any given time in the queued order.  They are
// /*AFLA*/  * implemented as unbound workqueues with @max_active of one.
// /*AFLA*/  *
// /*AFLA*/  * RETURNS:
// /*AFLA*/  * Pointer to the allocated workqueue on success, %NULL on failure.
// /*AFLA*/  */
// /*AFLA*/ #define alloc_ordered_workqueue(fmt, flags, args...)			\
// /*AFLA*/ 	alloc_workqueue(fmt, WQ_UNBOUND | __WQ_ORDERED | (flags), 1, ##args)
// /*AFLA*/ 
// /*AFLA*/ #define create_workqueue(name)						\
// /*AFLA*/ 	alloc_workqueue("%s", __WQ_LEGACY | WQ_MEM_RECLAIM, 1, (name))
// /*AFLA*/ #define create_freezable_workqueue(name)				\
// /*AFLA*/ 	alloc_workqueue("%s", __WQ_LEGACY | WQ_FREEZABLE | WQ_UNBOUND |	\
// /*AFLA*/ 			WQ_MEM_RECLAIM, 1, (name))
// /*AFLA*/ #define create_singlethread_workqueue(name)				\
// /*AFLA*/ 	alloc_ordered_workqueue("%s", __WQ_LEGACY | WQ_MEM_RECLAIM, name)
// /*AFLA*/ 
// /*AFLA*/ extern void destroy_workqueue(struct workqueue_struct *wq);
// /*AFLA*/ 
// /*AFLA*/ struct workqueue_attrs *alloc_workqueue_attrs(gfp_t gfp_mask);
// /*AFLA*/ void free_workqueue_attrs(struct workqueue_attrs *attrs);
// /*AFLA*/ int apply_workqueue_attrs(struct workqueue_struct *wq,
// /*AFLA*/ 			  const struct workqueue_attrs *attrs);
// /*AFLA*/ int workqueue_set_unbound_cpumask(cpumask_var_t cpumask);
// /*AFLA*/ 
// /*AFLA*/ extern bool queue_work_on(int cpu, struct workqueue_struct *wq,
// /*AFLA*/ 			struct work_struct *work);
// /*AFLA*/ extern bool queue_delayed_work_on(int cpu, struct workqueue_struct *wq,
// /*AFLA*/ 			struct delayed_work *work, unsigned long delay);
// /*AFLA*/ extern bool mod_delayed_work_on(int cpu, struct workqueue_struct *wq,
// /*AFLA*/ 			struct delayed_work *dwork, unsigned long delay);
// /*AFLA*/ 
// /*AFLA*/ extern void flush_workqueue(struct workqueue_struct *wq);
// /*AFLA*/ extern void drain_workqueue(struct workqueue_struct *wq);
// /*AFLA*/ 
// /*AFLA*/ extern int schedule_on_each_cpu(work_func_t func);
// /*AFLA*/ 
// /*AFLA*/ int execute_in_process_context(work_func_t fn, struct execute_work *);
// /*AFLA*/ 
// /*AFLA*/ extern bool flush_work(struct work_struct *work);
// /*AFLA*/ extern bool cancel_work(struct work_struct *work);
// /*AFLA*/ extern bool cancel_work_sync(struct work_struct *work);
// /*AFLA*/ 
// /*AFLA*/ extern bool flush_delayed_work(struct delayed_work *dwork);
// /*AFLA*/ extern bool cancel_delayed_work(struct delayed_work *dwork);
// /*AFLA*/ extern bool cancel_delayed_work_sync(struct delayed_work *dwork);
// /*AFLA*/ 
// /*AFLA*/ extern void workqueue_set_max_active(struct workqueue_struct *wq,
// /*AFLA*/ 				     int max_active);
// /*AFLA*/ extern bool current_is_workqueue_rescuer(void);
// /*AFLA*/ extern bool workqueue_congested(int cpu, struct workqueue_struct *wq);
// /*AFLA*/ extern unsigned int work_busy(struct work_struct *work);
// /*AFLA*/ extern __printf(1, 2) void set_worker_desc(const char *fmt, ...);
// /*AFLA*/ extern void print_worker_info(const char *log_lvl, struct task_struct *task);
// /*AFLA*/ extern void show_workqueue_state(void);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * queue_work - queue work on a workqueue
// /*AFLA*/  * @wq: workqueue to use
// /*AFLA*/  * @work: work to queue
// /*AFLA*/  *
// /*AFLA*/  * Returns %false if @work was already on a queue, %true otherwise.
// /*AFLA*/  *
// /*AFLA*/  * We queue the work to the CPU on which it was submitted, but if the CPU dies
// /*AFLA*/  * it can be processed by another CPU.
// /*AFLA*/  */
// /*AFLA*/ static inline bool queue_work(struct workqueue_struct *wq,
// /*AFLA*/ 			      struct work_struct *work)
// /*AFLA*/ {
// /*AFLA*/ 	return queue_work_on(WORK_CPU_UNBOUND, wq, work);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * queue_delayed_work - queue work on a workqueue after delay
// /*AFLA*/  * @wq: workqueue to use
// /*AFLA*/  * @dwork: delayable work to queue
// /*AFLA*/  * @delay: number of jiffies to wait before queueing
// /*AFLA*/  *
// /*AFLA*/  * Equivalent to queue_delayed_work_on() but tries to use the local CPU.
// /*AFLA*/  */
// /*AFLA*/ static inline bool queue_delayed_work(struct workqueue_struct *wq,
// /*AFLA*/ 				      struct delayed_work *dwork,
// /*AFLA*/ 				      unsigned long delay)
// /*AFLA*/ {
// /*AFLA*/ 	return queue_delayed_work_on(WORK_CPU_UNBOUND, wq, dwork, delay);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * mod_delayed_work - modify delay of or queue a delayed work
// /*AFLA*/  * @wq: workqueue to use
// /*AFLA*/  * @dwork: work to queue
// /*AFLA*/  * @delay: number of jiffies to wait before queueing
// /*AFLA*/  *
// /*AFLA*/  * mod_delayed_work_on() on local CPU.
// /*AFLA*/  */
// /*AFLA*/ static inline bool mod_delayed_work(struct workqueue_struct *wq,
// /*AFLA*/ 				    struct delayed_work *dwork,
// /*AFLA*/ 				    unsigned long delay)
// /*AFLA*/ {
// /*AFLA*/ 	return mod_delayed_work_on(WORK_CPU_UNBOUND, wq, dwork, delay);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * schedule_work_on - put work task on a specific cpu
// /*AFLA*/  * @cpu: cpu to put the work task on
// /*AFLA*/  * @work: job to be done
// /*AFLA*/  *
// /*AFLA*/  * This puts a job on a specific cpu
// /*AFLA*/  */
// /*AFLA*/ static inline bool schedule_work_on(int cpu, struct work_struct *work)
// /*AFLA*/ {
// /*AFLA*/ 	return queue_work_on(cpu, system_wq, work);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * schedule_work - put work task in global workqueue
// /*AFLA*/  * @work: job to be done
// /*AFLA*/  *
// /*AFLA*/  * Returns %false if @work was already on the kernel-global workqueue and
// /*AFLA*/  * %true otherwise.
// /*AFLA*/  *
// /*AFLA*/  * This puts a job in the kernel-global workqueue if it was not already
// /*AFLA*/  * queued and leaves it in the same position on the kernel-global
// /*AFLA*/  * workqueue otherwise.
// /*AFLA*/  */
// /*AFLA*/ static inline bool schedule_work(struct work_struct *work)
// /*AFLA*/ {
// /*AFLA*/ 	return queue_work(system_wq, work);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * flush_scheduled_work - ensure that any scheduled work has run to completion.
// /*AFLA*/  *
// /*AFLA*/  * Forces execution of the kernel-global workqueue and blocks until its
// /*AFLA*/  * completion.
// /*AFLA*/  *
// /*AFLA*/  * Think twice before calling this function!  It's very easy to get into
// /*AFLA*/  * trouble if you don't take great care.  Either of the following situations
// /*AFLA*/  * will lead to deadlock:
// /*AFLA*/  *
// /*AFLA*/  *	One of the work items currently on the workqueue needs to acquire
// /*AFLA*/  *	a lock held by your code or its caller.
// /*AFLA*/  *
// /*AFLA*/  *	Your code is running in the context of a work routine.
// /*AFLA*/  *
// /*AFLA*/  * They will be detected by lockdep when they occur, but the first might not
// /*AFLA*/  * occur very often.  It depends on what work items are on the workqueue and
// /*AFLA*/  * what locks they need, which you have no control over.
// /*AFLA*/  *
// /*AFLA*/  * In most situations flushing the entire workqueue is overkill; you merely
// /*AFLA*/  * need to know that a particular work item isn't queued and isn't running.
// /*AFLA*/  * In such cases you should use cancel_delayed_work_sync() or
// /*AFLA*/  * cancel_work_sync() instead.
// /*AFLA*/  */
// /*AFLA*/ static inline void flush_scheduled_work(void)
// /*AFLA*/ {
// /*AFLA*/ 	flush_workqueue(system_wq);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * schedule_delayed_work_on - queue work in global workqueue on CPU after delay
// /*AFLA*/  * @cpu: cpu to use
// /*AFLA*/  * @dwork: job to be done
// /*AFLA*/  * @delay: number of jiffies to wait
// /*AFLA*/  *
// /*AFLA*/  * After waiting for a given time this puts a job in the kernel-global
// /*AFLA*/  * workqueue on the specified CPU.
// /*AFLA*/  */
// /*AFLA*/ static inline bool schedule_delayed_work_on(int cpu, struct delayed_work *dwork,
// /*AFLA*/ 					    unsigned long delay)
// /*AFLA*/ {
// /*AFLA*/ 	return queue_delayed_work_on(cpu, system_wq, dwork, delay);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * schedule_delayed_work - put work task in global workqueue after delay
// /*AFLA*/  * @dwork: job to be done
// /*AFLA*/  * @delay: number of jiffies to wait or 0 for immediate execution
// /*AFLA*/  *
// /*AFLA*/  * After waiting for a given time this puts a job in the kernel-global
// /*AFLA*/  * workqueue.
// /*AFLA*/  */
// /*AFLA*/ static inline bool schedule_delayed_work(struct delayed_work *dwork,
// /*AFLA*/ 					 unsigned long delay)
// /*AFLA*/ {
// /*AFLA*/ 	return queue_delayed_work(system_wq, dwork, delay);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * keventd_up - is workqueue initialized yet?
// /*AFLA*/  */
// /*AFLA*/ static inline bool keventd_up(void)
// /*AFLA*/ {
// /*AFLA*/ 	return system_wq != NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_SMP
// /*AFLA*/ static inline long work_on_cpu(int cpu, long (*fn)(void *), void *arg)
// /*AFLA*/ {
// /*AFLA*/ 	return fn(arg);
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ long work_on_cpu(int cpu, long (*fn)(void *), void *arg);
// /*AFLA*/ #endif /* CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FREEZER
// /*AFLA*/ extern void freeze_workqueues_begin(void);
// /*AFLA*/ extern bool freeze_workqueues_busy(void);
// /*AFLA*/ extern void thaw_workqueues(void);
// /*AFLA*/ #endif /* CONFIG_FREEZER */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SYSFS
// /*AFLA*/ int workqueue_sysfs_register(struct workqueue_struct *wq);
// /*AFLA*/ #else	/* CONFIG_SYSFS */
// /*AFLA*/ static inline int workqueue_sysfs_register(struct workqueue_struct *wq)
// /*AFLA*/ { return 0; }
// /*AFLA*/ #endif	/* CONFIG_SYSFS */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_WQ_WATCHDOG
// /*AFLA*/ void wq_watchdog_touch(int cpu);
// /*AFLA*/ #else	/* CONFIG_WQ_WATCHDOG */
// /*AFLA*/ static inline void wq_watchdog_touch(int cpu) { }
// /*AFLA*/ #endif	/* CONFIG_WQ_WATCHDOG */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ int workqueue_prepare_cpu(unsigned int cpu);
// /*AFLA*/ int workqueue_online_cpu(unsigned int cpu);
// /*AFLA*/ int workqueue_offline_cpu(unsigned int cpu);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
