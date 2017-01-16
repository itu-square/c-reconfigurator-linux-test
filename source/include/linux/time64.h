// /*AFLA*/ #ifndef _LINUX_TIME64_H
// /*AFLA*/ #define _LINUX_TIME64_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/time.h>
// /*AFLA*/ #include <linux/math64.h>
// /*AFLA*/ 
// /*AFLA*/ typedef __s64 time64_t;
// /*AFLA*/ typedef __u64 timeu64_t;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This wants to go into uapi/linux/time.h once we agreed about the
// /*AFLA*/  * userspace interfaces.
// /*AFLA*/  */
// /*AFLA*/ #if __BITS_PER_LONG == 64
// /*AFLA*/ # define timespec64 timespec
// /*AFLA*/ #define itimerspec64 itimerspec
// /*AFLA*/ #else
// /*AFLA*/ struct timespec64 {
// /*AFLA*/ 	time64_t	tv_sec;			/* seconds */
// /*AFLA*/ 	long		tv_nsec;		/* nanoseconds */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct itimerspec64 {
// /*AFLA*/ 	struct timespec64 it_interval;
// /*AFLA*/ 	struct timespec64 it_value;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Parameters used to convert the timespec values: */
// /*AFLA*/ #define MSEC_PER_SEC	1000L
// /*AFLA*/ #define USEC_PER_MSEC	1000L
// /*AFLA*/ #define NSEC_PER_USEC	1000L
// /*AFLA*/ #define NSEC_PER_MSEC	1000000L
// /*AFLA*/ #define USEC_PER_SEC	1000000L
// /*AFLA*/ #define NSEC_PER_SEC	1000000000L
// /*AFLA*/ #define FSEC_PER_SEC	1000000000000000LL
// /*AFLA*/ 
// /*AFLA*/ /* Located here for timespec[64]_valid_strict */
// /*AFLA*/ #define TIME64_MAX			((s64)~((u64)1 << 63))
// /*AFLA*/ #define KTIME_MAX			((s64)~((u64)1 << 63))
// /*AFLA*/ #define KTIME_SEC_MAX			(KTIME_MAX / NSEC_PER_SEC)
// /*AFLA*/ 
// /*AFLA*/ #if __BITS_PER_LONG == 64
// /*AFLA*/ 
// /*AFLA*/ static inline struct timespec timespec64_to_timespec(const struct timespec64 ts64)
// /*AFLA*/ {
// /*AFLA*/ 	return ts64;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct timespec64 timespec_to_timespec64(const struct timespec ts)
// /*AFLA*/ {
// /*AFLA*/ 	return ts;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct itimerspec itimerspec64_to_itimerspec(struct itimerspec64 *its64)
// /*AFLA*/ {
// /*AFLA*/ 	return *its64;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct itimerspec64 itimerspec_to_itimerspec64(struct itimerspec *its)
// /*AFLA*/ {
// /*AFLA*/ 	return *its;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ # define timespec64_equal		timespec_equal
// /*AFLA*/ # define timespec64_compare		timespec_compare
// /*AFLA*/ # define set_normalized_timespec64	set_normalized_timespec
// /*AFLA*/ # define timespec64_add			timespec_add
// /*AFLA*/ # define timespec64_sub			timespec_sub
// /*AFLA*/ # define timespec64_valid		timespec_valid
// /*AFLA*/ # define timespec64_valid_strict	timespec_valid_strict
// /*AFLA*/ # define timespec64_to_ns		timespec_to_ns
// /*AFLA*/ # define ns_to_timespec64		ns_to_timespec
// /*AFLA*/ # define timespec64_add_ns		timespec_add_ns
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline struct timespec timespec64_to_timespec(const struct timespec64 ts64)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec ret;
// /*AFLA*/ 
// /*AFLA*/ 	ret.tv_sec = (time_t)ts64.tv_sec;
// /*AFLA*/ 	ret.tv_nsec = ts64.tv_nsec;
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct timespec64 timespec_to_timespec64(const struct timespec ts)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec64 ret;
// /*AFLA*/ 
// /*AFLA*/ 	ret.tv_sec = ts.tv_sec;
// /*AFLA*/ 	ret.tv_nsec = ts.tv_nsec;
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct itimerspec itimerspec64_to_itimerspec(struct itimerspec64 *its64)
// /*AFLA*/ {
// /*AFLA*/ 	struct itimerspec ret;
// /*AFLA*/ 
// /*AFLA*/ 	ret.it_interval = timespec64_to_timespec(its64->it_interval);
// /*AFLA*/ 	ret.it_value = timespec64_to_timespec(its64->it_value);
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct itimerspec64 itimerspec_to_itimerspec64(struct itimerspec *its)
// /*AFLA*/ {
// /*AFLA*/ 	struct itimerspec64 ret;
// /*AFLA*/ 
// /*AFLA*/ 	ret.it_interval = timespec_to_timespec64(its->it_interval);
// /*AFLA*/ 	ret.it_value = timespec_to_timespec64(its->it_value);
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int timespec64_equal(const struct timespec64 *a,
// /*AFLA*/ 				   const struct timespec64 *b)
// /*AFLA*/ {
// /*AFLA*/ 	return (a->tv_sec == b->tv_sec) && (a->tv_nsec == b->tv_nsec);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * lhs < rhs:  return <0
// /*AFLA*/  * lhs == rhs: return 0
// /*AFLA*/  * lhs > rhs:  return >0
// /*AFLA*/  */
// /*AFLA*/ static inline int timespec64_compare(const struct timespec64 *lhs, const struct timespec64 *rhs)
// /*AFLA*/ {
// /*AFLA*/ 	if (lhs->tv_sec < rhs->tv_sec)
// /*AFLA*/ 		return -1;
// /*AFLA*/ 	if (lhs->tv_sec > rhs->tv_sec)
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	return lhs->tv_nsec - rhs->tv_nsec;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void set_normalized_timespec64(struct timespec64 *ts, time64_t sec, s64 nsec);
// /*AFLA*/ 
// /*AFLA*/ static inline struct timespec64 timespec64_add(struct timespec64 lhs,
// /*AFLA*/ 						struct timespec64 rhs)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec64 ts_delta;
// /*AFLA*/ 	set_normalized_timespec64(&ts_delta, lhs.tv_sec + rhs.tv_sec,
// /*AFLA*/ 				lhs.tv_nsec + rhs.tv_nsec);
// /*AFLA*/ 	return ts_delta;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * sub = lhs - rhs, in normalized form
// /*AFLA*/  */
// /*AFLA*/ static inline struct timespec64 timespec64_sub(struct timespec64 lhs,
// /*AFLA*/ 						struct timespec64 rhs)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec64 ts_delta;
// /*AFLA*/ 	set_normalized_timespec64(&ts_delta, lhs.tv_sec - rhs.tv_sec,
// /*AFLA*/ 				lhs.tv_nsec - rhs.tv_nsec);
// /*AFLA*/ 	return ts_delta;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Returns true if the timespec64 is norm, false if denorm:
// /*AFLA*/  */
// /*AFLA*/ static inline bool timespec64_valid(const struct timespec64 *ts)
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
// /*AFLA*/ static inline bool timespec64_valid_strict(const struct timespec64 *ts)
// /*AFLA*/ {
// /*AFLA*/ 	if (!timespec64_valid(ts))
// /*AFLA*/ 		return false;
// /*AFLA*/ 	/* Disallow values that could overflow ktime_t */
// /*AFLA*/ 	if ((unsigned long long)ts->tv_sec >= KTIME_SEC_MAX)
// /*AFLA*/ 		return false;
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * timespec64_to_ns - Convert timespec64 to nanoseconds
// /*AFLA*/  * @ts:		pointer to the timespec64 variable to be converted
// /*AFLA*/  *
// /*AFLA*/  * Returns the scalar nanosecond representation of the timespec64
// /*AFLA*/  * parameter.
// /*AFLA*/  */
// /*AFLA*/ static inline s64 timespec64_to_ns(const struct timespec64 *ts)
// /*AFLA*/ {
// /*AFLA*/ 	return ((s64) ts->tv_sec * NSEC_PER_SEC) + ts->tv_nsec;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ns_to_timespec64 - Convert nanoseconds to timespec64
// /*AFLA*/  * @nsec:	the nanoseconds value to be converted
// /*AFLA*/  *
// /*AFLA*/  * Returns the timespec64 representation of the nsec parameter.
// /*AFLA*/  */
// /*AFLA*/ extern struct timespec64 ns_to_timespec64(const s64 nsec);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * timespec64_add_ns - Adds nanoseconds to a timespec64
// /*AFLA*/  * @a:		pointer to timespec64 to be incremented
// /*AFLA*/  * @ns:		unsigned nanoseconds value to be added
// /*AFLA*/  *
// /*AFLA*/  * This must always be inlined because its used from the x86-64 vdso,
// /*AFLA*/  * which cannot call other kernel functions.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void timespec64_add_ns(struct timespec64 *a, u64 ns)
// /*AFLA*/ {
// /*AFLA*/ 	a->tv_sec += __iter_div_u64_rem(a->tv_nsec + ns, NSEC_PER_SEC, &ns);
// /*AFLA*/ 	a->tv_nsec = ns;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * timespec64_add_safe assumes both values are positive and checks for
// /*AFLA*/  * overflow. It will return TIME64_MAX in case of overflow.
// /*AFLA*/  */
// /*AFLA*/ extern struct timespec64 timespec64_add_safe(const struct timespec64 lhs,
// /*AFLA*/ 					 const struct timespec64 rhs);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_TIME64_H */
