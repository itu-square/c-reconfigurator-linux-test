// /*AFLA*/ #ifndef _SCHED_PRIO_H
// /*AFLA*/ #define _SCHED_PRIO_H
// /*AFLA*/ 
// /*AFLA*/ #define MAX_NICE	19
// /*AFLA*/ #define MIN_NICE	-20
// /*AFLA*/ #define NICE_WIDTH	(MAX_NICE - MIN_NICE + 1)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Priority of a process goes from 0..MAX_PRIO-1, valid RT
// /*AFLA*/  * priority is 0..MAX_RT_PRIO-1, and SCHED_NORMAL/SCHED_BATCH
// /*AFLA*/  * tasks are in the range MAX_RT_PRIO..MAX_PRIO-1. Priority
// /*AFLA*/  * values are inverted: lower p->prio value means higher priority.
// /*AFLA*/  *
// /*AFLA*/  * The MAX_USER_RT_PRIO value allows the actual maximum
// /*AFLA*/  * RT priority to be separate from the value exported to
// /*AFLA*/  * user-space.  This allows kernel threads to set their
// /*AFLA*/  * priority to a value higher than any user task. Note:
// /*AFLA*/  * MAX_RT_PRIO must not be smaller than MAX_USER_RT_PRIO.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define MAX_USER_RT_PRIO	100
// /*AFLA*/ #define MAX_RT_PRIO		MAX_USER_RT_PRIO
// /*AFLA*/ 
// /*AFLA*/ #define MAX_PRIO		(MAX_RT_PRIO + NICE_WIDTH)
// /*AFLA*/ #define DEFAULT_PRIO		(MAX_RT_PRIO + NICE_WIDTH / 2)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Convert user-nice values [ -20 ... 0 ... 19 ]
// /*AFLA*/  * to static priority [ MAX_RT_PRIO..MAX_PRIO-1 ],
// /*AFLA*/  * and back.
// /*AFLA*/  */
// /*AFLA*/ #define NICE_TO_PRIO(nice)	((nice) + DEFAULT_PRIO)
// /*AFLA*/ #define PRIO_TO_NICE(prio)	((prio) - DEFAULT_PRIO)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * 'User priority' is the nice value converted to something we
// /*AFLA*/  * can work with better when scaling various scheduler parameters,
// /*AFLA*/  * it's a [ 0 ... 39 ] range.
// /*AFLA*/  */
// /*AFLA*/ #define USER_PRIO(p)		((p)-MAX_RT_PRIO)
// /*AFLA*/ #define TASK_USER_PRIO(p)	USER_PRIO((p)->static_prio)
// /*AFLA*/ #define MAX_USER_PRIO		(USER_PRIO(MAX_PRIO))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Convert nice value [19,-20] to rlimit style value [1,40].
// /*AFLA*/  */
// /*AFLA*/ static inline long nice_to_rlimit(long nice)
// /*AFLA*/ {
// /*AFLA*/ 	return (MAX_NICE - nice + 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Convert rlimit style value [1,40] to nice value [-20, 19].
// /*AFLA*/  */
// /*AFLA*/ static inline long rlimit_to_nice(long prio)
// /*AFLA*/ {
// /*AFLA*/ 	return (MAX_NICE - prio + 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _SCHED_PRIO_H */
