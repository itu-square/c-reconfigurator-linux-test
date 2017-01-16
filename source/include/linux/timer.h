// /*AFLA*/ #ifndef _LINUX_TIMER_H
// /*AFLA*/ #define _LINUX_TIMER_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/ktime.h>
// /*AFLA*/ #include <linux/stddef.h>
// /*AFLA*/ #include <linux/debugobjects.h>
// /*AFLA*/ #include <linux/stringify.h>
// /*AFLA*/ 
// /*AFLA*/ struct tvec_base;
// /*AFLA*/ 
// /*AFLA*/ struct timer_list {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * All fields that change during normal runtime grouped to the
// /*AFLA*/ 	 * same cacheline
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct hlist_node	entry;
// /*AFLA*/ 	unsigned long		expires;
// /*AFLA*/ 	void			(*function)(unsigned long);
// /*AFLA*/ 	unsigned long		data;
// /*AFLA*/ 	u32			flags;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TIMER_STATS
// /*AFLA*/ 	int			start_pid;
// /*AFLA*/ 	void			*start_site;
// /*AFLA*/ 	char			start_comm[16];
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_LOCKDEP
// /*AFLA*/ 	struct lockdep_map	lockdep_map;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_LOCKDEP
// /*AFLA*/ /*
// /*AFLA*/  * NB: because we have to copy the lockdep_map, setting the lockdep_map key
// /*AFLA*/  * (second argument) here is required, otherwise it could be initialised to
// /*AFLA*/  * the copy of the lockdep_map later! We use the pointer to and the string
// /*AFLA*/  * "<file>:<line>" as the key resp. the name of the lockdep_map.
// /*AFLA*/  */
// /*AFLA*/ #define __TIMER_LOCKDEP_MAP_INITIALIZER(_kn)				\
// /*AFLA*/ 	.lockdep_map = STATIC_LOCKDEP_MAP_INIT(_kn, &_kn),
// /*AFLA*/ #else
// /*AFLA*/ #define __TIMER_LOCKDEP_MAP_INITIALIZER(_kn)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A deferrable timer will work normally when the system is busy, but
// /*AFLA*/  * will not cause a CPU to come out of idle just to service it; instead,
// /*AFLA*/  * the timer will be serviced when the CPU eventually wakes up with a
// /*AFLA*/  * subsequent non-deferrable timer.
// /*AFLA*/  *
// /*AFLA*/  * An irqsafe timer is executed with IRQ disabled and it's safe to wait for
// /*AFLA*/  * the completion of the running instance from IRQ handlers, for example,
// /*AFLA*/  * by calling del_timer_sync().
// /*AFLA*/  *
// /*AFLA*/  * Note: The irq disabled callback execution is a special case for
// /*AFLA*/  * workqueue locking issues. It's not meant for executing random crap
// /*AFLA*/  * with interrupts disabled. Abuse is monitored!
// /*AFLA*/  */
// /*AFLA*/ #define TIMER_CPUMASK		0x0003FFFF
// /*AFLA*/ #define TIMER_MIGRATING		0x00040000
// /*AFLA*/ #define TIMER_BASEMASK		(TIMER_CPUMASK | TIMER_MIGRATING)
// /*AFLA*/ #define TIMER_DEFERRABLE	0x00080000
// /*AFLA*/ #define TIMER_PINNED		0x00100000
// /*AFLA*/ #define TIMER_IRQSAFE		0x00200000
// /*AFLA*/ #define TIMER_ARRAYSHIFT	22
// /*AFLA*/ #define TIMER_ARRAYMASK		0xFFC00000
// /*AFLA*/ 
// /*AFLA*/ #define __TIMER_INITIALIZER(_function, _expires, _data, _flags) { \
// /*AFLA*/ 		.entry = { .next = TIMER_ENTRY_STATIC },	\
// /*AFLA*/ 		.function = (_function),			\
// /*AFLA*/ 		.expires = (_expires),				\
// /*AFLA*/ 		.data = (_data),				\
// /*AFLA*/ 		.flags = (_flags),				\
// /*AFLA*/ 		__TIMER_LOCKDEP_MAP_INITIALIZER(		\
// /*AFLA*/ 			__FILE__ ":" __stringify(__LINE__))	\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ #define TIMER_INITIALIZER(_function, _expires, _data)		\
// /*AFLA*/ 	__TIMER_INITIALIZER((_function), (_expires), (_data), 0)
// /*AFLA*/ 
// /*AFLA*/ #define TIMER_PINNED_INITIALIZER(_function, _expires, _data)	\
// /*AFLA*/ 	__TIMER_INITIALIZER((_function), (_expires), (_data), TIMER_PINNED)
// /*AFLA*/ 
// /*AFLA*/ #define TIMER_DEFERRED_INITIALIZER(_function, _expires, _data)	\
// /*AFLA*/ 	__TIMER_INITIALIZER((_function), (_expires), (_data), TIMER_DEFERRABLE)
// /*AFLA*/ 
// /*AFLA*/ #define TIMER_PINNED_DEFERRED_INITIALIZER(_function, _expires, _data)	\
// /*AFLA*/ 	__TIMER_INITIALIZER((_function), (_expires), (_data), TIMER_DEFERRABLE | TIMER_PINNED)
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_TIMER(_name, _function, _expires, _data)		\
// /*AFLA*/ 	struct timer_list _name =				\
// /*AFLA*/ 		TIMER_INITIALIZER(_function, _expires, _data)
// /*AFLA*/ 
// /*AFLA*/ void init_timer_key(struct timer_list *timer, unsigned int flags,
// /*AFLA*/ 		    const char *name, struct lock_class_key *key);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_OBJECTS_TIMERS
// /*AFLA*/ extern void init_timer_on_stack_key(struct timer_list *timer,
// /*AFLA*/ 				    unsigned int flags, const char *name,
// /*AFLA*/ 				    struct lock_class_key *key);
// /*AFLA*/ extern void destroy_timer_on_stack(struct timer_list *timer);
// /*AFLA*/ #else
// /*AFLA*/ static inline void destroy_timer_on_stack(struct timer_list *timer) { }
// /*AFLA*/ static inline void init_timer_on_stack_key(struct timer_list *timer,
// /*AFLA*/ 					   unsigned int flags, const char *name,
// /*AFLA*/ 					   struct lock_class_key *key)
// /*AFLA*/ {
// /*AFLA*/ 	init_timer_key(timer, flags, name, key);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_LOCKDEP
// /*AFLA*/ #define __init_timer(_timer, _flags)					\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		static struct lock_class_key __key;			\
// /*AFLA*/ 		init_timer_key((_timer), (_flags), #_timer, &__key);	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #define __init_timer_on_stack(_timer, _flags)				\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		static struct lock_class_key __key;			\
// /*AFLA*/ 		init_timer_on_stack_key((_timer), (_flags), #_timer, &__key); \
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #else
// /*AFLA*/ #define __init_timer(_timer, _flags)					\
// /*AFLA*/ 	init_timer_key((_timer), (_flags), NULL, NULL)
// /*AFLA*/ #define __init_timer_on_stack(_timer, _flags)				\
// /*AFLA*/ 	init_timer_on_stack_key((_timer), (_flags), NULL, NULL)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define init_timer(timer)						\
// /*AFLA*/ 	__init_timer((timer), 0)
// /*AFLA*/ #define init_timer_pinned(timer)					\
// /*AFLA*/ 	__init_timer((timer), TIMER_PINNED)
// /*AFLA*/ #define init_timer_deferrable(timer)					\
// /*AFLA*/ 	__init_timer((timer), TIMER_DEFERRABLE)
// /*AFLA*/ #define init_timer_pinned_deferrable(timer)				\
// /*AFLA*/ 	__init_timer((timer), TIMER_DEFERRABLE | TIMER_PINNED)
// /*AFLA*/ #define init_timer_on_stack(timer)					\
// /*AFLA*/ 	__init_timer_on_stack((timer), 0)
// /*AFLA*/ 
// /*AFLA*/ #define __setup_timer(_timer, _fn, _data, _flags)			\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		__init_timer((_timer), (_flags));			\
// /*AFLA*/ 		(_timer)->function = (_fn);				\
// /*AFLA*/ 		(_timer)->data = (_data);				\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #define __setup_timer_on_stack(_timer, _fn, _data, _flags)		\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		__init_timer_on_stack((_timer), (_flags));		\
// /*AFLA*/ 		(_timer)->function = (_fn);				\
// /*AFLA*/ 		(_timer)->data = (_data);				\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #define setup_timer(timer, fn, data)					\
// /*AFLA*/ 	__setup_timer((timer), (fn), (data), 0)
// /*AFLA*/ #define setup_pinned_timer(timer, fn, data)				\
// /*AFLA*/ 	__setup_timer((timer), (fn), (data), TIMER_PINNED)
// /*AFLA*/ #define setup_deferrable_timer(timer, fn, data)				\
// /*AFLA*/ 	__setup_timer((timer), (fn), (data), TIMER_DEFERRABLE)
// /*AFLA*/ #define setup_pinned_deferrable_timer(timer, fn, data)			\
// /*AFLA*/ 	__setup_timer((timer), (fn), (data), TIMER_DEFERRABLE | TIMER_PINNED)
// /*AFLA*/ #define setup_timer_on_stack(timer, fn, data)				\
// /*AFLA*/ 	__setup_timer_on_stack((timer), (fn), (data), 0)
// /*AFLA*/ #define setup_pinned_timer_on_stack(timer, fn, data)			\
// /*AFLA*/ 	__setup_timer_on_stack((timer), (fn), (data), TIMER_PINNED)
// /*AFLA*/ #define setup_deferrable_timer_on_stack(timer, fn, data)		\
// /*AFLA*/ 	__setup_timer_on_stack((timer), (fn), (data), TIMER_DEFERRABLE)
// /*AFLA*/ #define setup_pinned_deferrable_timer_on_stack(timer, fn, data)		\
// /*AFLA*/ 	__setup_timer_on_stack((timer), (fn), (data), TIMER_DEFERRABLE | TIMER_PINNED)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * timer_pending - is a timer pending?
// /*AFLA*/  * @timer: the timer in question
// /*AFLA*/  *
// /*AFLA*/  * timer_pending will tell whether a given timer is currently pending,
// /*AFLA*/  * or not. Callers must ensure serialization wrt. other operations done
// /*AFLA*/  * to this timer, eg. interrupt contexts, or other CPUs on SMP.
// /*AFLA*/  *
// /*AFLA*/  * return value: 1 if the timer is pending, 0 if not.
// /*AFLA*/  */
// /*AFLA*/ static inline int timer_pending(const struct timer_list * timer)
// /*AFLA*/ {
// /*AFLA*/ 	return timer->entry.pprev != NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void add_timer_on(struct timer_list *timer, int cpu);
// /*AFLA*/ extern int del_timer(struct timer_list * timer);
// /*AFLA*/ extern int mod_timer(struct timer_list *timer, unsigned long expires);
// /*AFLA*/ extern int mod_timer_pending(struct timer_list *timer, unsigned long expires);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The jiffies value which is added to now, when there is no timer
// /*AFLA*/  * in the timer wheel:
// /*AFLA*/  */
// /*AFLA*/ #define NEXT_TIMER_MAX_DELTA	((1UL << 30) - 1)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Timer-statistics info:
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_TIMER_STATS
// /*AFLA*/ 
// /*AFLA*/ extern int timer_stats_active;
// /*AFLA*/ 
// /*AFLA*/ extern void init_timer_stats(void);
// /*AFLA*/ 
// /*AFLA*/ extern void timer_stats_update_stats(void *timer, pid_t pid, void *startf,
// /*AFLA*/ 				     void *timerf, char *comm, u32 flags);
// /*AFLA*/ 
// /*AFLA*/ extern void __timer_stats_timer_set_start_info(struct timer_list *timer,
// /*AFLA*/ 					       void *addr);
// /*AFLA*/ 
// /*AFLA*/ static inline void timer_stats_timer_set_start_info(struct timer_list *timer)
// /*AFLA*/ {
// /*AFLA*/ 	if (likely(!timer_stats_active))
// /*AFLA*/ 		return;
// /*AFLA*/ 	__timer_stats_timer_set_start_info(timer, __builtin_return_address(0));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void timer_stats_timer_clear_start_info(struct timer_list *timer)
// /*AFLA*/ {
// /*AFLA*/ 	timer->start_site = NULL;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline void init_timer_stats(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void timer_stats_timer_set_start_info(struct timer_list *timer)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void timer_stats_timer_clear_start_info(struct timer_list *timer)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void add_timer(struct timer_list *timer);
// /*AFLA*/ 
// /*AFLA*/ extern int try_to_del_timer_sync(struct timer_list *timer);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/   extern int del_timer_sync(struct timer_list *timer);
// /*AFLA*/ #else
// /*AFLA*/ # define del_timer_sync(t)		del_timer(t)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define del_singleshot_timer_sync(t) del_timer_sync(t)
// /*AFLA*/ 
// /*AFLA*/ extern void init_timers(void);
// /*AFLA*/ extern void run_local_timers(void);
// /*AFLA*/ struct hrtimer;
// /*AFLA*/ extern enum hrtimer_restart it_real_fn(struct hrtimer *);
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_SMP) && defined(CONFIG_NO_HZ_COMMON)
// /*AFLA*/ #include <linux/sysctl.h>
// /*AFLA*/ 
// /*AFLA*/ extern unsigned int sysctl_timer_migration;
// /*AFLA*/ int timer_migration_handler(struct ctl_table *table, int write,
// /*AFLA*/ 			    void __user *buffer, size_t *lenp,
// /*AFLA*/ 			    loff_t *ppos);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ unsigned long __round_jiffies(unsigned long j, int cpu);
// /*AFLA*/ unsigned long __round_jiffies_relative(unsigned long j, int cpu);
// /*AFLA*/ unsigned long round_jiffies(unsigned long j);
// /*AFLA*/ unsigned long round_jiffies_relative(unsigned long j);
// /*AFLA*/ 
// /*AFLA*/ unsigned long __round_jiffies_up(unsigned long j, int cpu);
// /*AFLA*/ unsigned long __round_jiffies_up_relative(unsigned long j, int cpu);
// /*AFLA*/ unsigned long round_jiffies_up(unsigned long j);
// /*AFLA*/ unsigned long round_jiffies_up_relative(unsigned long j);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HOTPLUG_CPU
// /*AFLA*/ int timers_dead_cpu(unsigned int cpu);
// /*AFLA*/ #else
// /*AFLA*/ #define timers_dead_cpu NULL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
