// /*AFLA*/ #ifndef _LINUX_TIME_H
// /*AFLA*/ #define _LINUX_TIME_H
// /*AFLA*/ 
// /*AFLA*/ # include <linux/cache.h>
// /*AFLA*/ # include <linux/seqlock.h>
// /*AFLA*/ # include <linux/math64.h>
// /*AFLA*/ # include <linux/time64.h>
// /*AFLA*/ 
// /*AFLA*/ extern struct timezone sys_tz;
// /*AFLA*/ 
// /*AFLA*/ #define TIME_T_MAX	(time_t)((1UL << ((sizeof(time_t) << 3) - 1)) - 1)
// /*AFLA*/ 
// /*AFLA*/ static inline int timespec_equal(const struct timespec *a,
// /*AFLA*/                                  const struct timespec *b)
// /*AFLA*/ {
// /*AFLA*/ 	return (a->tv_sec == b->tv_sec) && (a->tv_nsec == b->tv_nsec);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * lhs < rhs:  return <0
// /*AFLA*/  * lhs == rhs: return 0
// /*AFLA*/  * lhs > rhs:  return >0
// /*AFLA*/  */
// /*AFLA*/ static inline int timespec_compare(const struct timespec *lhs, const struct timespec *rhs)
// /*AFLA*/ {
// /*AFLA*/ 	if (lhs->tv_sec < rhs->tv_sec)
// /*AFLA*/ 		return -1;
// /*AFLA*/ 	if (lhs->tv_sec > rhs->tv_sec)
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	return lhs->tv_nsec - rhs->tv_nsec;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int timeval_compare(const struct timeval *lhs, const struct timeval *rhs)
// /*AFLA*/ {
// /*AFLA*/ 	if (lhs->tv_sec < rhs->tv_sec)
// /*AFLA*/ 		return -1;
// /*AFLA*/ 	if (lhs->tv_sec > rhs->tv_sec)
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	return lhs->tv_usec - rhs->tv_usec;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern time64_t mktime64(const unsigned int year, const unsigned int mon,
// /*AFLA*/ 			const unsigned int day, const unsigned int hour,
// /*AFLA*/ 			const unsigned int min, const unsigned int sec);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * Deprecated. Use mktime64().
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long mktime(const unsigned int year,
// /*AFLA*/ 			const unsigned int mon, const unsigned int day,
// /*AFLA*/ 			const unsigned int hour, const unsigned int min,
// /*AFLA*/ 			const unsigned int sec)
// /*AFLA*/ {
// /*AFLA*/ 	return mktime64(year, mon, day, hour, min, sec);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void set_normalized_timespec(struct timespec *ts, time_t sec, s64 nsec);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * timespec_add_safe assumes both values are positive and checks
// /*AFLA*/  * for overflow. It will return TIME_T_MAX if the reutrn would be
// /*AFLA*/  * smaller then either of the arguments.
// /*AFLA*/  */
// /*AFLA*/ extern struct timespec timespec_add_safe(const struct timespec lhs,
// /*AFLA*/ 					 const struct timespec rhs);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ static inline struct timespec timespec_add(struct timespec lhs,
// /*AFLA*/ 						struct timespec rhs)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec ts_delta;
// /*AFLA*/ 	set_normalized_timespec(&ts_delta, lhs.tv_sec + rhs.tv_sec,
// /*AFLA*/ 				lhs.tv_nsec + rhs.tv_nsec);
// /*AFLA*/ 	return ts_delta;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * sub = lhs - rhs, in normalized form
// /*AFLA*/  */
// /*AFLA*/ static inline struct timespec timespec_sub(struct timespec lhs,
// /*AFLA*/ 						struct timespec rhs)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec ts_delta;
// /*AFLA*/ 	set_normalized_timespec(&ts_delta, lhs.tv_sec - rhs.tv_sec,
// /*AFLA*/ 				lhs.tv_nsec - rhs.tv_nsec);
// /*AFLA*/ 	return ts_delta;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Returns true if the timespec is norm, false if denorm:
// /*AFLA*/  */
// /*AFLA*/ static inline bool timespec_valid(const struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	/* Dates before 1970 are bogus */
// /*AFLA*/ 	if (ts->tv_sec < 0)
// /*AFLA*/ 		return false;
// /*AFLA*/ 	/* Can't have more nanoseconds then a second */
// /*AFLA*/ 	if ((unsigned long)ts->tv_nsec >= NSEC_PER_SEC)
// /*AFLA*/ 		return false;
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool timespec_valid_strict(const struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	if (!timespec_valid(ts))
// /*AFLA*/ 		return false;
// /*AFLA*/ 	/* Disallow values that could overflow ktime_t */
// /*AFLA*/ 	if ((unsigned long long)ts->tv_sec >= KTIME_SEC_MAX)
// /*AFLA*/ 		return false;
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool timeval_valid(const struct timeval *tv)
// /*AFLA*/ {
// /*AFLA*/ 	/* Dates before 1970 are bogus */
// /*AFLA*/ 	if (tv->tv_sec < 0)
// /*AFLA*/ 		return false;
// /*AFLA*/ 
// /*AFLA*/ 	/* Can't have more microseconds then a second */
// /*AFLA*/ 	if (tv->tv_usec < 0 || tv->tv_usec >= USEC_PER_SEC)
// /*AFLA*/ 		return false;
// /*AFLA*/ 
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct timespec timespec_trunc(struct timespec t, unsigned gran);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Validates if a timespec/timeval used to inject a time offset is valid.
// /*AFLA*/  * Offsets can be postive or negative. The value of the timeval/timespec
// /*AFLA*/  * is the sum of its fields, but *NOTE*: the field tv_usec/tv_nsec must
// /*AFLA*/  * always be non-negative.
// /*AFLA*/  */
// /*AFLA*/ static inline bool timeval_inject_offset_valid(const struct timeval *tv)
// /*AFLA*/ {
// /*AFLA*/ 	/* We don't check the tv_sec as it can be positive or negative */
// /*AFLA*/ 
// /*AFLA*/ 	/* Can't have more microseconds then a second */
// /*AFLA*/ 	if (tv->tv_usec < 0 || tv->tv_usec >= USEC_PER_SEC)
// /*AFLA*/ 		return false;
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool timespec_inject_offset_valid(const struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	/* We don't check the tv_sec as it can be positive or negative */
// /*AFLA*/ 
// /*AFLA*/ 	/* Can't have more nanoseconds then a second */
// /*AFLA*/ 	if (ts->tv_nsec < 0 || ts->tv_nsec >= NSEC_PER_SEC)
// /*AFLA*/ 		return false;
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define CURRENT_TIME		(current_kernel_time())
// /*AFLA*/ #define CURRENT_TIME_SEC	((struct timespec) { get_seconds(), 0 })
// /*AFLA*/ 
// /*AFLA*/ /* Some architectures do not supply their own clocksource.
// /*AFLA*/  * This is mainly the case in architectures that get their
// /*AFLA*/  * inter-tick times by reading the counter on their interval
// /*AFLA*/  * timer. Since these timers wrap every tick, they're not really
// /*AFLA*/  * useful as clocksources. Wrapping them to act like one is possible
// /*AFLA*/  * but not very efficient. So we provide a callout these arches
// /*AFLA*/  * can implement for use with the jiffies clocksource to provide
// /*AFLA*/  * finer then tick granular time.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_ARCH_USES_GETTIMEOFFSET
// /*AFLA*/ extern u32 (*arch_gettimeoffset)(void);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct itimerval;
// /*AFLA*/ extern int do_setitimer(int which, struct itimerval *value,
// /*AFLA*/ 			struct itimerval *ovalue);
// /*AFLA*/ extern int do_getitimer(int which, struct itimerval *value);
// /*AFLA*/ 
// /*AFLA*/ extern unsigned int alarm_setitimer(unsigned int seconds);
// /*AFLA*/ 
// /*AFLA*/ extern long do_utimes(int dfd, const char __user *filename, struct timespec *times, int flags);
// /*AFLA*/ 
// /*AFLA*/ struct tms;
// /*AFLA*/ extern void do_sys_times(struct tms *);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Similar to the struct tm in userspace <time.h>, but it needs to be here so
// /*AFLA*/  * that the kernel source is self contained.
// /*AFLA*/  */
// /*AFLA*/ struct tm {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * the number of seconds after the minute, normally in the range
// /*AFLA*/ 	 * 0 to 59, but can be up to 60 to allow for leap seconds
// /*AFLA*/ 	 */
// /*AFLA*/ 	int tm_sec;
// /*AFLA*/ 	/* the number of minutes after the hour, in the range 0 to 59*/
// /*AFLA*/ 	int tm_min;
// /*AFLA*/ 	/* the number of hours past midnight, in the range 0 to 23 */
// /*AFLA*/ 	int tm_hour;
// /*AFLA*/ 	/* the day of the month, in the range 1 to 31 */
// /*AFLA*/ 	int tm_mday;
// /*AFLA*/ 	/* the number of months since January, in the range 0 to 11 */
// /*AFLA*/ 	int tm_mon;
// /*AFLA*/ 	/* the number of years since 1900 */
// /*AFLA*/ 	long tm_year;
// /*AFLA*/ 	/* the number of days since Sunday, in the range 0 to 6 */
// /*AFLA*/ 	int tm_wday;
// /*AFLA*/ 	/* the number of days since January 1, in the range 0 to 365 */
// /*AFLA*/ 	int tm_yday;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ void time64_to_tm(time64_t totalsecs, int offset, struct tm *result);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * time_to_tm - converts the calendar time to local broken-down time
// /*AFLA*/  *
// /*AFLA*/  * @totalsecs	the number of seconds elapsed since 00:00:00 on January 1, 1970,
// /*AFLA*/  *		Coordinated Universal Time (UTC).
// /*AFLA*/  * @offset	offset seconds adding to totalsecs.
// /*AFLA*/  * @result	pointer to struct tm variable to receive broken-down time
// /*AFLA*/  */
// /*AFLA*/ static inline void time_to_tm(time_t totalsecs, int offset, struct tm *result)
// /*AFLA*/ {
// /*AFLA*/ 	time64_to_tm(totalsecs, offset, result);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * timespec_to_ns - Convert timespec to nanoseconds
// /*AFLA*/  * @ts:		pointer to the timespec variable to be converted
// /*AFLA*/  *
// /*AFLA*/  * Returns the scalar nanosecond representation of the timespec
// /*AFLA*/  * parameter.
// /*AFLA*/  */
// /*AFLA*/ static inline s64 timespec_to_ns(const struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	return ((s64) ts->tv_sec * NSEC_PER_SEC) + ts->tv_nsec;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * timeval_to_ns - Convert timeval to nanoseconds
// /*AFLA*/  * @ts:		pointer to the timeval variable to be converted
// /*AFLA*/  *
// /*AFLA*/  * Returns the scalar nanosecond representation of the timeval
// /*AFLA*/  * parameter.
// /*AFLA*/  */
// /*AFLA*/ static inline s64 timeval_to_ns(const struct timeval *tv)
// /*AFLA*/ {
// /*AFLA*/ 	return ((s64) tv->tv_sec * NSEC_PER_SEC) +
// /*AFLA*/ 		tv->tv_usec * NSEC_PER_USEC;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ns_to_timespec - Convert nanoseconds to timespec
// /*AFLA*/  * @nsec:	the nanoseconds value to be converted
// /*AFLA*/  *
// /*AFLA*/  * Returns the timespec representation of the nsec parameter.
// /*AFLA*/  */
// /*AFLA*/ extern struct timespec ns_to_timespec(const s64 nsec);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ns_to_timeval - Convert nanoseconds to timeval
// /*AFLA*/  * @nsec:	the nanoseconds value to be converted
// /*AFLA*/  *
// /*AFLA*/  * Returns the timeval representation of the nsec parameter.
// /*AFLA*/  */
// /*AFLA*/ extern struct timeval ns_to_timeval(const s64 nsec);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * timespec_add_ns - Adds nanoseconds to a timespec
// /*AFLA*/  * @a:		pointer to timespec to be incremented
// /*AFLA*/  * @ns:		unsigned nanoseconds value to be added
// /*AFLA*/  *
// /*AFLA*/  * This must always be inlined because its used from the x86-64 vdso,
// /*AFLA*/  * which cannot call other kernel functions.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void timespec_add_ns(struct timespec *a, u64 ns)
// /*AFLA*/ {
// /*AFLA*/ 	a->tv_sec += __iter_div_u64_rem(a->tv_nsec + ns, NSEC_PER_SEC, &ns);
// /*AFLA*/ 	a->tv_nsec = ns;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
