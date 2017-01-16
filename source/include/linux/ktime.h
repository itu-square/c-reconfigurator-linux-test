// /*AFLA*/ /*
// /*AFLA*/  *  include/linux/ktime.h
// /*AFLA*/  *
// /*AFLA*/  *  ktime_t - nanosecond-resolution time format.
// /*AFLA*/  *
// /*AFLA*/  *   Copyright(C) 2005, Thomas Gleixner <tglx@linutronix.de>
// /*AFLA*/  *   Copyright(C) 2005, Red Hat, Inc., Ingo Molnar
// /*AFLA*/  *
// /*AFLA*/  *  data type definitions, declarations, prototypes and macros.
// /*AFLA*/  *
// /*AFLA*/  *  Started by: Thomas Gleixner and Ingo Molnar
// /*AFLA*/  *
// /*AFLA*/  *  Credits:
// /*AFLA*/  *
// /*AFLA*/  *  	Roman Zippel provided the ideas and primary code snippets of
// /*AFLA*/  *  	the ktime_t union and further simplifications of the original
// /*AFLA*/  *  	code.
// /*AFLA*/  *
// /*AFLA*/  *  For licencing details see kernel-base/COPYING
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LINUX_KTIME_H
// /*AFLA*/ #define _LINUX_KTIME_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/time.h>
// /*AFLA*/ #include <linux/jiffies.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * ktime_t:
// /*AFLA*/  *
// /*AFLA*/  * A single 64-bit variable is used to store the hrtimers
// /*AFLA*/  * internal representation of time values in scalar nanoseconds. The
// /*AFLA*/  * design plays out best on 64-bit CPUs, where most conversions are
// /*AFLA*/  * NOPs and most arithmetic ktime_t operations are plain arithmetic
// /*AFLA*/  * operations.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ union ktime {
// /*AFLA*/ 	s64	tv64;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ typedef union ktime ktime_t;		/* Kill this */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ktime_set - Set a ktime_t variable from a seconds/nanoseconds value
// /*AFLA*/  * @secs:	seconds to set
// /*AFLA*/  * @nsecs:	nanoseconds to set
// /*AFLA*/  *
// /*AFLA*/  * Return: The ktime_t representation of the value.
// /*AFLA*/  */
// /*AFLA*/ static inline ktime_t ktime_set(const s64 secs, const unsigned long nsecs)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(secs >= KTIME_SEC_MAX))
// /*AFLA*/ 		return (ktime_t){ .tv64 = KTIME_MAX };
// /*AFLA*/ 
// /*AFLA*/ 	return (ktime_t) { .tv64 = secs * NSEC_PER_SEC + (s64)nsecs };
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Subtract two ktime_t variables. rem = lhs -rhs: */
// /*AFLA*/ #define ktime_sub(lhs, rhs) \
// /*AFLA*/ 		({ (ktime_t){ .tv64 = (lhs).tv64 - (rhs).tv64 }; })
// /*AFLA*/ 
// /*AFLA*/ /* Add two ktime_t variables. res = lhs + rhs: */
// /*AFLA*/ #define ktime_add(lhs, rhs) \
// /*AFLA*/ 		({ (ktime_t){ .tv64 = (lhs).tv64 + (rhs).tv64 }; })
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Same as ktime_add(), but avoids undefined behaviour on overflow; however,
// /*AFLA*/  * this means that you must check the result for overflow yourself.
// /*AFLA*/  */
// /*AFLA*/ #define ktime_add_unsafe(lhs, rhs) \
// /*AFLA*/ 		({ (ktime_t){ .tv64 = (u64) (lhs).tv64 + (rhs).tv64 }; })
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Add a ktime_t variable and a scalar nanosecond value.
// /*AFLA*/  * res = kt + nsval:
// /*AFLA*/  */
// /*AFLA*/ #define ktime_add_ns(kt, nsval) \
// /*AFLA*/ 		({ (ktime_t){ .tv64 = (kt).tv64 + (nsval) }; })
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Subtract a scalar nanosecod from a ktime_t variable
// /*AFLA*/  * res = kt - nsval:
// /*AFLA*/  */
// /*AFLA*/ #define ktime_sub_ns(kt, nsval) \
// /*AFLA*/ 		({ (ktime_t){ .tv64 = (kt).tv64 - (nsval) }; })
// /*AFLA*/ 
// /*AFLA*/ /* convert a timespec to ktime_t format: */
// /*AFLA*/ static inline ktime_t timespec_to_ktime(struct timespec ts)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_set(ts.tv_sec, ts.tv_nsec);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* convert a timespec64 to ktime_t format: */
// /*AFLA*/ static inline ktime_t timespec64_to_ktime(struct timespec64 ts)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_set(ts.tv_sec, ts.tv_nsec);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* convert a timeval to ktime_t format: */
// /*AFLA*/ static inline ktime_t timeval_to_ktime(struct timeval tv)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_set(tv.tv_sec, tv.tv_usec * NSEC_PER_USEC);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Map the ktime_t to timespec conversion to ns_to_timespec function */
// /*AFLA*/ #define ktime_to_timespec(kt)		ns_to_timespec((kt).tv64)
// /*AFLA*/ 
// /*AFLA*/ /* Map the ktime_t to timespec conversion to ns_to_timespec function */
// /*AFLA*/ #define ktime_to_timespec64(kt)		ns_to_timespec64((kt).tv64)
// /*AFLA*/ 
// /*AFLA*/ /* Map the ktime_t to timeval conversion to ns_to_timeval function */
// /*AFLA*/ #define ktime_to_timeval(kt)		ns_to_timeval((kt).tv64)
// /*AFLA*/ 
// /*AFLA*/ /* Convert ktime_t to nanoseconds - NOP in the scalar storage format: */
// /*AFLA*/ #define ktime_to_ns(kt)			((kt).tv64)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ktime_equal - Compares two ktime_t variables to see if they are equal
// /*AFLA*/  * @cmp1:	comparable1
// /*AFLA*/  * @cmp2:	comparable2
// /*AFLA*/  *
// /*AFLA*/  * Compare two ktime_t variables.
// /*AFLA*/  *
// /*AFLA*/  * Return: 1 if equal.
// /*AFLA*/  */
// /*AFLA*/ static inline int ktime_equal(const ktime_t cmp1, const ktime_t cmp2)
// /*AFLA*/ {
// /*AFLA*/ 	return cmp1.tv64 == cmp2.tv64;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ktime_compare - Compares two ktime_t variables for less, greater or equal
// /*AFLA*/  * @cmp1:	comparable1
// /*AFLA*/  * @cmp2:	comparable2
// /*AFLA*/  *
// /*AFLA*/  * Return: ...
// /*AFLA*/  *   cmp1  < cmp2: return <0
// /*AFLA*/  *   cmp1 == cmp2: return 0
// /*AFLA*/  *   cmp1  > cmp2: return >0
// /*AFLA*/  */
// /*AFLA*/ static inline int ktime_compare(const ktime_t cmp1, const ktime_t cmp2)
// /*AFLA*/ {
// /*AFLA*/ 	if (cmp1.tv64 < cmp2.tv64)
// /*AFLA*/ 		return -1;
// /*AFLA*/ 	if (cmp1.tv64 > cmp2.tv64)
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ktime_after - Compare if a ktime_t value is bigger than another one.
// /*AFLA*/  * @cmp1:	comparable1
// /*AFLA*/  * @cmp2:	comparable2
// /*AFLA*/  *
// /*AFLA*/  * Return: true if cmp1 happened after cmp2.
// /*AFLA*/  */
// /*AFLA*/ static inline bool ktime_after(const ktime_t cmp1, const ktime_t cmp2)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_compare(cmp1, cmp2) > 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ktime_before - Compare if a ktime_t value is smaller than another one.
// /*AFLA*/  * @cmp1:	comparable1
// /*AFLA*/  * @cmp2:	comparable2
// /*AFLA*/  *
// /*AFLA*/  * Return: true if cmp1 happened before cmp2.
// /*AFLA*/  */
// /*AFLA*/ static inline bool ktime_before(const ktime_t cmp1, const ktime_t cmp2)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_compare(cmp1, cmp2) < 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #if BITS_PER_LONG < 64
// /*AFLA*/ extern s64 __ktime_divns(const ktime_t kt, s64 div);
// /*AFLA*/ static inline s64 ktime_divns(const ktime_t kt, s64 div)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Negative divisors could cause an inf loop,
// /*AFLA*/ 	 * so bug out here.
// /*AFLA*/ 	 */
// /*AFLA*/ 	BUG_ON(div < 0);
// /*AFLA*/ 	if (__builtin_constant_p(div) && !(div >> 32)) {
// /*AFLA*/ 		s64 ns = kt.tv64;
// /*AFLA*/ 		u64 tmp = ns < 0 ? -ns : ns;
// /*AFLA*/ 
// /*AFLA*/ 		do_div(tmp, div);
// /*AFLA*/ 		return ns < 0 ? -tmp : tmp;
// /*AFLA*/ 	} else {
// /*AFLA*/ 		return __ktime_divns(kt, div);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ #else /* BITS_PER_LONG < 64 */
// /*AFLA*/ static inline s64 ktime_divns(const ktime_t kt, s64 div)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * 32-bit implementation cannot handle negative divisors,
// /*AFLA*/ 	 * so catch them on 64bit as well.
// /*AFLA*/ 	 */
// /*AFLA*/ 	WARN_ON(div < 0);
// /*AFLA*/ 	return kt.tv64 / div;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline s64 ktime_to_us(const ktime_t kt)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_divns(kt, NSEC_PER_USEC);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline s64 ktime_to_ms(const ktime_t kt)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_divns(kt, NSEC_PER_MSEC);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline s64 ktime_us_delta(const ktime_t later, const ktime_t earlier)
// /*AFLA*/ {
// /*AFLA*/        return ktime_to_us(ktime_sub(later, earlier));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline s64 ktime_ms_delta(const ktime_t later, const ktime_t earlier)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_to_ms(ktime_sub(later, earlier));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline ktime_t ktime_add_us(const ktime_t kt, const u64 usec)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_add_ns(kt, usec * NSEC_PER_USEC);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline ktime_t ktime_add_ms(const ktime_t kt, const u64 msec)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_add_ns(kt, msec * NSEC_PER_MSEC);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline ktime_t ktime_sub_us(const ktime_t kt, const u64 usec)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_sub_ns(kt, usec * NSEC_PER_USEC);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline ktime_t ktime_sub_ms(const ktime_t kt, const u64 msec)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_sub_ns(kt, msec * NSEC_PER_MSEC);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern ktime_t ktime_add_safe(const ktime_t lhs, const ktime_t rhs);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ktime_to_timespec_cond - convert a ktime_t variable to timespec
// /*AFLA*/  *			    format only if the variable contains data
// /*AFLA*/  * @kt:		the ktime_t variable to convert
// /*AFLA*/  * @ts:		the timespec variable to store the result in
// /*AFLA*/  *
// /*AFLA*/  * Return: %true if there was a successful conversion, %false if kt was 0.
// /*AFLA*/  */
// /*AFLA*/ static inline __must_check bool ktime_to_timespec_cond(const ktime_t kt,
// /*AFLA*/ 						       struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	if (kt.tv64) {
// /*AFLA*/ 		*ts = ktime_to_timespec(kt);
// /*AFLA*/ 		return true;
// /*AFLA*/ 	} else {
// /*AFLA*/ 		return false;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ktime_to_timespec64_cond - convert a ktime_t variable to timespec64
// /*AFLA*/  *			    format only if the variable contains data
// /*AFLA*/  * @kt:		the ktime_t variable to convert
// /*AFLA*/  * @ts:		the timespec variable to store the result in
// /*AFLA*/  *
// /*AFLA*/  * Return: %true if there was a successful conversion, %false if kt was 0.
// /*AFLA*/  */
// /*AFLA*/ static inline __must_check bool ktime_to_timespec64_cond(const ktime_t kt,
// /*AFLA*/ 						       struct timespec64 *ts)
// /*AFLA*/ {
// /*AFLA*/ 	if (kt.tv64) {
// /*AFLA*/ 		*ts = ktime_to_timespec64(kt);
// /*AFLA*/ 		return true;
// /*AFLA*/ 	} else {
// /*AFLA*/ 		return false;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The resolution of the clocks. The resolution value is returned in
// /*AFLA*/  * the clock_getres() system call to give application programmers an
// /*AFLA*/  * idea of the (in)accuracy of timers. Timer values are rounded up to
// /*AFLA*/  * this resolution values.
// /*AFLA*/  */
// /*AFLA*/ #define LOW_RES_NSEC		TICK_NSEC
// /*AFLA*/ #define KTIME_LOW_RES		(ktime_t){ .tv64 = LOW_RES_NSEC }
// /*AFLA*/ 
// /*AFLA*/ static inline ktime_t ns_to_ktime(u64 ns)
// /*AFLA*/ {
// /*AFLA*/ 	static const ktime_t ktime_zero = { .tv64 = 0 };
// /*AFLA*/ 
// /*AFLA*/ 	return ktime_add_ns(ktime_zero, ns);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline ktime_t ms_to_ktime(u64 ms)
// /*AFLA*/ {
// /*AFLA*/ 	static const ktime_t ktime_zero = { .tv64 = 0 };
// /*AFLA*/ 
// /*AFLA*/ 	return ktime_add_ms(ktime_zero, ms);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ # include <linux/timekeeping.h>
// /*AFLA*/ 
// /*AFLA*/ #endif
