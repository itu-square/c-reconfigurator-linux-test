// /*AFLA*/ #ifndef _LINUX_JIFFIES_H
// /*AFLA*/ #define _LINUX_JIFFIES_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/math64.h>
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/time.h>
// /*AFLA*/ #include <linux/timex.h>
// /*AFLA*/ #include <asm/param.h>			/* for HZ */
// /*AFLA*/ #include <generated/timeconst.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The following defines establish the engineering parameters of the PLL
// /*AFLA*/  * model. The HZ variable establishes the timer interrupt frequency, 100 Hz
// /*AFLA*/  * for the SunOS kernel, 256 Hz for the Ultrix kernel and 1024 Hz for the
// /*AFLA*/  * OSF/1 kernel. The SHIFT_HZ define expresses the same value as the
// /*AFLA*/  * nearest power of two in order to avoid hardware multiply operations.
// /*AFLA*/  */
// /*AFLA*/ #if HZ >= 12 && HZ < 24
// /*AFLA*/ # define SHIFT_HZ	4
// /*AFLA*/ #elif HZ >= 24 && HZ < 48
// /*AFLA*/ # define SHIFT_HZ	5
// /*AFLA*/ #elif HZ >= 48 && HZ < 96
// /*AFLA*/ # define SHIFT_HZ	6
// /*AFLA*/ #elif HZ >= 96 && HZ < 192
// /*AFLA*/ # define SHIFT_HZ	7
// /*AFLA*/ #elif HZ >= 192 && HZ < 384
// /*AFLA*/ # define SHIFT_HZ	8
// /*AFLA*/ #elif HZ >= 384 && HZ < 768
// /*AFLA*/ # define SHIFT_HZ	9
// /*AFLA*/ #elif HZ >= 768 && HZ < 1536
// /*AFLA*/ # define SHIFT_HZ	10
// /*AFLA*/ #elif HZ >= 1536 && HZ < 3072
// /*AFLA*/ # define SHIFT_HZ	11
// /*AFLA*/ #elif HZ >= 3072 && HZ < 6144
// /*AFLA*/ # define SHIFT_HZ	12
// /*AFLA*/ #elif HZ >= 6144 && HZ < 12288
// /*AFLA*/ # define SHIFT_HZ	13
// /*AFLA*/ #else
// /*AFLA*/ # error Invalid value of HZ.
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Suppose we want to divide two numbers NOM and DEN: NOM/DEN, then we can
// /*AFLA*/  * improve accuracy by shifting LSH bits, hence calculating:
// /*AFLA*/  *     (NOM << LSH) / DEN
// /*AFLA*/  * This however means trouble for large NOM, because (NOM << LSH) may no
// /*AFLA*/  * longer fit in 32 bits. The following way of calculating this gives us
// /*AFLA*/  * some slack, under the following conditions:
// /*AFLA*/  *   - (NOM / DEN) fits in (32 - LSH) bits.
// /*AFLA*/  *   - (NOM % DEN) fits in (32 - LSH) bits.
// /*AFLA*/  */
// /*AFLA*/ #define SH_DIV(NOM,DEN,LSH) (   (((NOM) / (DEN)) << (LSH))              \
// /*AFLA*/                              + ((((NOM) % (DEN)) << (LSH)) + (DEN) / 2) / (DEN))
// /*AFLA*/ 
// /*AFLA*/ /* LATCH is used in the interval timer and ftape setup. */
// /*AFLA*/ #define LATCH ((CLOCK_TICK_RATE + HZ/2) / HZ)	/* For divider */
// /*AFLA*/ 
// /*AFLA*/ extern int register_refined_jiffies(long clock_tick_rate);
// /*AFLA*/ 
// /*AFLA*/ /* TICK_NSEC is the time between ticks in nsec assuming SHIFTED_HZ */
// /*AFLA*/ #define TICK_NSEC ((NSEC_PER_SEC+HZ/2)/HZ)
// /*AFLA*/ 
// /*AFLA*/ /* TICK_USEC is the time between ticks in usec assuming fake USER_HZ */
// /*AFLA*/ #define TICK_USEC ((1000000UL + USER_HZ/2) / USER_HZ)
// /*AFLA*/ 
// /*AFLA*/ /* some arch's have a small-data section that can be accessed register-relative
// /*AFLA*/  * but that can only take up to, say, 4-byte variables. jiffies being part of
// /*AFLA*/  * an 8-byte variable may not be correctly accessed unless we force the issue
// /*AFLA*/  */
// /*AFLA*/ #define __jiffy_data  __attribute__((section(".data")))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The 64-bit value is not atomic - you MUST NOT read it
// /*AFLA*/  * without sampling the sequence number in jiffies_lock.
// /*AFLA*/  * get_jiffies_64() will do this for you as appropriate.
// /*AFLA*/  */
// /*AFLA*/ extern u64 __jiffy_data jiffies_64;
// /*AFLA*/ extern unsigned long volatile __jiffy_data jiffies;
// /*AFLA*/ 
// /*AFLA*/ #if (BITS_PER_LONG < 64)
// /*AFLA*/ u64 get_jiffies_64(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline u64 get_jiffies_64(void)
// /*AFLA*/ {
// /*AFLA*/ 	return (u64)jiffies;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  *	These inlines deal with timer wrapping correctly. You are 
// /*AFLA*/  *	strongly encouraged to use them
// /*AFLA*/  *	1. Because people otherwise forget
// /*AFLA*/  *	2. Because if the timer wrap changes in future you won't have to
// /*AFLA*/  *	   alter your driver code.
// /*AFLA*/  *
// /*AFLA*/  * time_after(a,b) returns true if the time a is after time b.
// /*AFLA*/  *
// /*AFLA*/  * Do this with "<0" and ">=0" to only test the sign of the result. A
// /*AFLA*/  * good compiler would generate better code (and a really good compiler
// /*AFLA*/  * wouldn't care). Gcc is currently neither.
// /*AFLA*/  */
// /*AFLA*/ #define time_after(a,b)		\
// /*AFLA*/ 	(typecheck(unsigned long, a) && \
// /*AFLA*/ 	 typecheck(unsigned long, b) && \
// /*AFLA*/ 	 ((long)((b) - (a)) < 0))
// /*AFLA*/ #define time_before(a,b)	time_after(b,a)
// /*AFLA*/ 
// /*AFLA*/ #define time_after_eq(a,b)	\
// /*AFLA*/ 	(typecheck(unsigned long, a) && \
// /*AFLA*/ 	 typecheck(unsigned long, b) && \
// /*AFLA*/ 	 ((long)((a) - (b)) >= 0))
// /*AFLA*/ #define time_before_eq(a,b)	time_after_eq(b,a)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Calculate whether a is in the range of [b, c].
// /*AFLA*/  */
// /*AFLA*/ #define time_in_range(a,b,c) \
// /*AFLA*/ 	(time_after_eq(a,b) && \
// /*AFLA*/ 	 time_before_eq(a,c))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Calculate whether a is in the range of [b, c).
// /*AFLA*/  */
// /*AFLA*/ #define time_in_range_open(a,b,c) \
// /*AFLA*/ 	(time_after_eq(a,b) && \
// /*AFLA*/ 	 time_before(a,c))
// /*AFLA*/ 
// /*AFLA*/ /* Same as above, but does so with platform independent 64bit types.
// /*AFLA*/  * These must be used when utilizing jiffies_64 (i.e. return value of
// /*AFLA*/  * get_jiffies_64() */
// /*AFLA*/ #define time_after64(a,b)	\
// /*AFLA*/ 	(typecheck(__u64, a) &&	\
// /*AFLA*/ 	 typecheck(__u64, b) && \
// /*AFLA*/ 	 ((__s64)((b) - (a)) < 0))
// /*AFLA*/ #define time_before64(a,b)	time_after64(b,a)
// /*AFLA*/ 
// /*AFLA*/ #define time_after_eq64(a,b)	\
// /*AFLA*/ 	(typecheck(__u64, a) && \
// /*AFLA*/ 	 typecheck(__u64, b) && \
// /*AFLA*/ 	 ((__s64)((a) - (b)) >= 0))
// /*AFLA*/ #define time_before_eq64(a,b)	time_after_eq64(b,a)
// /*AFLA*/ 
// /*AFLA*/ #define time_in_range64(a, b, c) \
// /*AFLA*/ 	(time_after_eq64(a, b) && \
// /*AFLA*/ 	 time_before_eq64(a, c))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These four macros compare jiffies and 'a' for convenience.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* time_is_before_jiffies(a) return true if a is before jiffies */
// /*AFLA*/ #define time_is_before_jiffies(a) time_after(jiffies, a)
// /*AFLA*/ #define time_is_before_jiffies64(a) time_after64(get_jiffies_64(), a)
// /*AFLA*/ 
// /*AFLA*/ /* time_is_after_jiffies(a) return true if a is after jiffies */
// /*AFLA*/ #define time_is_after_jiffies(a) time_before(jiffies, a)
// /*AFLA*/ #define time_is_after_jiffies64(a) time_before64(get_jiffies_64(), a)
// /*AFLA*/ 
// /*AFLA*/ /* time_is_before_eq_jiffies(a) return true if a is before or equal to jiffies*/
// /*AFLA*/ #define time_is_before_eq_jiffies(a) time_after_eq(jiffies, a)
// /*AFLA*/ #define time_is_before_eq_jiffies64(a) time_after_eq64(get_jiffies_64(), a)
// /*AFLA*/ 
// /*AFLA*/ /* time_is_after_eq_jiffies(a) return true if a is after or equal to jiffies*/
// /*AFLA*/ #define time_is_after_eq_jiffies(a) time_before_eq(jiffies, a)
// /*AFLA*/ #define time_is_after_eq_jiffies64(a) time_before_eq64(get_jiffies_64(), a)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Have the 32 bit jiffies value wrap 5 minutes after boot
// /*AFLA*/  * so jiffies wrap bugs show up earlier.
// /*AFLA*/  */
// /*AFLA*/ #define INITIAL_JIFFIES ((unsigned long)(unsigned int) (-300*HZ))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Change timeval to jiffies, trying to avoid the
// /*AFLA*/  * most obvious overflows..
// /*AFLA*/  *
// /*AFLA*/  * And some not so obvious.
// /*AFLA*/  *
// /*AFLA*/  * Note that we don't want to return LONG_MAX, because
// /*AFLA*/  * for various timeout reasons we often end up having
// /*AFLA*/  * to wait "jiffies+1" in order to guarantee that we wait
// /*AFLA*/  * at _least_ "jiffies" - so "jiffies+1" had better still
// /*AFLA*/  * be positive.
// /*AFLA*/  */
// /*AFLA*/ #define MAX_JIFFY_OFFSET ((LONG_MAX >> 1)-1)
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long preset_lpj;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We want to do realistic conversions of time so we need to use the same
// /*AFLA*/  * values the update wall clock code uses as the jiffies size.  This value
// /*AFLA*/  * is: TICK_NSEC (which is defined in timex.h).  This
// /*AFLA*/  * is a constant and is in nanoseconds.  We will use scaled math
// /*AFLA*/  * with a set of scales defined here as SEC_JIFFIE_SC,  USEC_JIFFIE_SC and
// /*AFLA*/  * NSEC_JIFFIE_SC.  Note that these defines contain nothing but
// /*AFLA*/  * constants and so are computed at compile time.  SHIFT_HZ (computed in
// /*AFLA*/  * timex.h) adjusts the scaling for different HZ values.
// /*AFLA*/ 
// /*AFLA*/  * Scaled math???  What is that?
// /*AFLA*/  *
// /*AFLA*/  * Scaled math is a way to do integer math on values that would,
// /*AFLA*/  * otherwise, either overflow, underflow, or cause undesired div
// /*AFLA*/  * instructions to appear in the execution path.  In short, we "scale"
// /*AFLA*/  * up the operands so they take more bits (more precision, less
// /*AFLA*/  * underflow), do the desired operation and then "scale" the result back
// /*AFLA*/  * by the same amount.  If we do the scaling by shifting we avoid the
// /*AFLA*/  * costly mpy and the dastardly div instructions.
// /*AFLA*/ 
// /*AFLA*/  * Suppose, for example, we want to convert from seconds to jiffies
// /*AFLA*/  * where jiffies is defined in nanoseconds as NSEC_PER_JIFFIE.  The
// /*AFLA*/  * simple math is: jiff = (sec * NSEC_PER_SEC) / NSEC_PER_JIFFIE; We
// /*AFLA*/  * observe that (NSEC_PER_SEC / NSEC_PER_JIFFIE) is a constant which we
// /*AFLA*/  * might calculate at compile time, however, the result will only have
// /*AFLA*/  * about 3-4 bits of precision (less for smaller values of HZ).
// /*AFLA*/  *
// /*AFLA*/  * So, we scale as follows:
// /*AFLA*/  * jiff = (sec) * (NSEC_PER_SEC / NSEC_PER_JIFFIE);
// /*AFLA*/  * jiff = ((sec) * ((NSEC_PER_SEC * SCALE)/ NSEC_PER_JIFFIE)) / SCALE;
// /*AFLA*/  * Then we make SCALE a power of two so:
// /*AFLA*/  * jiff = ((sec) * ((NSEC_PER_SEC << SCALE)/ NSEC_PER_JIFFIE)) >> SCALE;
// /*AFLA*/  * Now we define:
// /*AFLA*/  * #define SEC_CONV = ((NSEC_PER_SEC << SCALE)/ NSEC_PER_JIFFIE))
// /*AFLA*/  * jiff = (sec * SEC_CONV) >> SCALE;
// /*AFLA*/  *
// /*AFLA*/  * Often the math we use will expand beyond 32-bits so we tell C how to
// /*AFLA*/  * do this and pass the 64-bit result of the mpy through the ">> SCALE"
// /*AFLA*/  * which should take the result back to 32-bits.  We want this expansion
// /*AFLA*/  * to capture as much precision as possible.  At the same time we don't
// /*AFLA*/  * want to overflow so we pick the SCALE to avoid this.  In this file,
// /*AFLA*/  * that means using a different scale for each range of HZ values (as
// /*AFLA*/  * defined in timex.h).
// /*AFLA*/  *
// /*AFLA*/  * For those who want to know, gcc will give a 64-bit result from a "*"
// /*AFLA*/  * operator if the result is a long long AND at least one of the
// /*AFLA*/  * operands is cast to long long (usually just prior to the "*" so as
// /*AFLA*/  * not to confuse it into thinking it really has a 64-bit operand,
// /*AFLA*/  * which, buy the way, it can do, but it takes more code and at least 2
// /*AFLA*/  * mpys).
// /*AFLA*/ 
// /*AFLA*/  * We also need to be aware that one second in nanoseconds is only a
// /*AFLA*/  * couple of bits away from overflowing a 32-bit word, so we MUST use
// /*AFLA*/  * 64-bits to get the full range time in nanoseconds.
// /*AFLA*/ 
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Here are the scales we will use.  One for seconds, nanoseconds and
// /*AFLA*/  * microseconds.
// /*AFLA*/  *
// /*AFLA*/  * Within the limits of cpp we do a rough cut at the SEC_JIFFIE_SC and
// /*AFLA*/  * check if the sign bit is set.  If not, we bump the shift count by 1.
// /*AFLA*/  * (Gets an extra bit of precision where we can use it.)
// /*AFLA*/  * We know it is set for HZ = 1024 and HZ = 100 not for 1000.
// /*AFLA*/  * Haven't tested others.
// /*AFLA*/ 
// /*AFLA*/  * Limits of cpp (for #if expressions) only long (no long long), but
// /*AFLA*/  * then we only need the most signicant bit.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define SEC_JIFFIE_SC (31 - SHIFT_HZ)
// /*AFLA*/ #if !((((NSEC_PER_SEC << 2) / TICK_NSEC) << (SEC_JIFFIE_SC - 2)) & 0x80000000)
// /*AFLA*/ #undef SEC_JIFFIE_SC
// /*AFLA*/ #define SEC_JIFFIE_SC (32 - SHIFT_HZ)
// /*AFLA*/ #endif
// /*AFLA*/ #define NSEC_JIFFIE_SC (SEC_JIFFIE_SC + 29)
// /*AFLA*/ #define SEC_CONVERSION ((unsigned long)((((u64)NSEC_PER_SEC << SEC_JIFFIE_SC) +\
// /*AFLA*/                                 TICK_NSEC -1) / (u64)TICK_NSEC))
// /*AFLA*/ 
// /*AFLA*/ #define NSEC_CONVERSION ((unsigned long)((((u64)1 << NSEC_JIFFIE_SC) +\
// /*AFLA*/                                         TICK_NSEC -1) / (u64)TICK_NSEC))
// /*AFLA*/ /*
// /*AFLA*/  * The maximum jiffie value is (MAX_INT >> 1).  Here we translate that
// /*AFLA*/  * into seconds.  The 64-bit case will overflow if we are not careful,
// /*AFLA*/  * so use the messy SH_DIV macro to do it.  Still all constants.
// /*AFLA*/  */
// /*AFLA*/ #if BITS_PER_LONG < 64
// /*AFLA*/ # define MAX_SEC_IN_JIFFIES \
// /*AFLA*/ 	(long)((u64)((u64)MAX_JIFFY_OFFSET * TICK_NSEC) / NSEC_PER_SEC)
// /*AFLA*/ #else	/* take care of overflow on 64 bits machines */
// /*AFLA*/ # define MAX_SEC_IN_JIFFIES \
// /*AFLA*/ 	(SH_DIV((MAX_JIFFY_OFFSET >> SEC_JIFFIE_SC) * TICK_NSEC, NSEC_PER_SEC, 1) - 1)
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Convert various time units to each other:
// /*AFLA*/  */
// /*AFLA*/ extern unsigned int jiffies_to_msecs(const unsigned long j);
// /*AFLA*/ extern unsigned int jiffies_to_usecs(const unsigned long j);
// /*AFLA*/ 
// /*AFLA*/ static inline u64 jiffies_to_nsecs(const unsigned long j)
// /*AFLA*/ {
// /*AFLA*/ 	return (u64)jiffies_to_usecs(j) * NSEC_PER_USEC;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long __msecs_to_jiffies(const unsigned int m);
// /*AFLA*/ #if HZ <= MSEC_PER_SEC && !(MSEC_PER_SEC % HZ)
// /*AFLA*/ /*
// /*AFLA*/  * HZ is equal to or smaller than 1000, and 1000 is a nice round
// /*AFLA*/  * multiple of HZ, divide with the factor between them, but round
// /*AFLA*/  * upwards:
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long _msecs_to_jiffies(const unsigned int m)
// /*AFLA*/ {
// /*AFLA*/ 	return (m + (MSEC_PER_SEC / HZ) - 1) / (MSEC_PER_SEC / HZ);
// /*AFLA*/ }
// /*AFLA*/ #elif HZ > MSEC_PER_SEC && !(HZ % MSEC_PER_SEC)
// /*AFLA*/ /*
// /*AFLA*/  * HZ is larger than 1000, and HZ is a nice round multiple of 1000 -
// /*AFLA*/  * simply multiply with the factor between them.
// /*AFLA*/  *
// /*AFLA*/  * But first make sure the multiplication result cannot overflow:
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long _msecs_to_jiffies(const unsigned int m)
// /*AFLA*/ {
// /*AFLA*/ 	if (m > jiffies_to_msecs(MAX_JIFFY_OFFSET))
// /*AFLA*/ 		return MAX_JIFFY_OFFSET;
// /*AFLA*/ 	return m * (HZ / MSEC_PER_SEC);
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ /*
// /*AFLA*/  * Generic case - multiply, round and divide. But first check that if
// /*AFLA*/  * we are doing a net multiplication, that we wouldn't overflow:
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long _msecs_to_jiffies(const unsigned int m)
// /*AFLA*/ {
// /*AFLA*/ 	if (HZ > MSEC_PER_SEC && m > jiffies_to_msecs(MAX_JIFFY_OFFSET))
// /*AFLA*/ 		return MAX_JIFFY_OFFSET;
// /*AFLA*/ 
// /*AFLA*/ 	return (MSEC_TO_HZ_MUL32 * m + MSEC_TO_HZ_ADJ32) >> MSEC_TO_HZ_SHR32;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ /**
// /*AFLA*/  * msecs_to_jiffies: - convert milliseconds to jiffies
// /*AFLA*/  * @m:	time in milliseconds
// /*AFLA*/  *
// /*AFLA*/  * conversion is done as follows:
// /*AFLA*/  *
// /*AFLA*/  * - negative values mean 'infinite timeout' (MAX_JIFFY_OFFSET)
// /*AFLA*/  *
// /*AFLA*/  * - 'too large' values [that would result in larger than
// /*AFLA*/  *   MAX_JIFFY_OFFSET values] mean 'infinite timeout' too.
// /*AFLA*/  *
// /*AFLA*/  * - all other values are converted to jiffies by either multiplying
// /*AFLA*/  *   the input value by a factor or dividing it with a factor and
// /*AFLA*/  *   handling any 32-bit overflows.
// /*AFLA*/  *   for the details see __msecs_to_jiffies()
// /*AFLA*/  *
// /*AFLA*/  * msecs_to_jiffies() checks for the passed in value being a constant
// /*AFLA*/  * via __builtin_constant_p() allowing gcc to eliminate most of the
// /*AFLA*/  * code, __msecs_to_jiffies() is called if the value passed does not
// /*AFLA*/  * allow constant folding and the actual conversion must be done at
// /*AFLA*/  * runtime.
// /*AFLA*/  * the HZ range specific helpers _msecs_to_jiffies() are called both
// /*AFLA*/  * directly here and from __msecs_to_jiffies() in the case where
// /*AFLA*/  * constant folding is not possible.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline unsigned long msecs_to_jiffies(const unsigned int m)
// /*AFLA*/ {
// /*AFLA*/ 	if (__builtin_constant_p(m)) {
// /*AFLA*/ 		if ((int)m < 0)
// /*AFLA*/ 			return MAX_JIFFY_OFFSET;
// /*AFLA*/ 		return _msecs_to_jiffies(m);
// /*AFLA*/ 	} else {
// /*AFLA*/ 		return __msecs_to_jiffies(m);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long __usecs_to_jiffies(const unsigned int u);
// /*AFLA*/ #if !(USEC_PER_SEC % HZ)
// /*AFLA*/ static inline unsigned long _usecs_to_jiffies(const unsigned int u)
// /*AFLA*/ {
// /*AFLA*/ 	return (u + (USEC_PER_SEC / HZ) - 1) / (USEC_PER_SEC / HZ);
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline unsigned long _usecs_to_jiffies(const unsigned int u)
// /*AFLA*/ {
// /*AFLA*/ 	return (USEC_TO_HZ_MUL32 * u + USEC_TO_HZ_ADJ32)
// /*AFLA*/ 		>> USEC_TO_HZ_SHR32;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * usecs_to_jiffies: - convert microseconds to jiffies
// /*AFLA*/  * @u:	time in microseconds
// /*AFLA*/  *
// /*AFLA*/  * conversion is done as follows:
// /*AFLA*/  *
// /*AFLA*/  * - 'too large' values [that would result in larger than
// /*AFLA*/  *   MAX_JIFFY_OFFSET values] mean 'infinite timeout' too.
// /*AFLA*/  *
// /*AFLA*/  * - all other values are converted to jiffies by either multiplying
// /*AFLA*/  *   the input value by a factor or dividing it with a factor and
// /*AFLA*/  *   handling any 32-bit overflows as for msecs_to_jiffies.
// /*AFLA*/  *
// /*AFLA*/  * usecs_to_jiffies() checks for the passed in value being a constant
// /*AFLA*/  * via __builtin_constant_p() allowing gcc to eliminate most of the
// /*AFLA*/  * code, __usecs_to_jiffies() is called if the value passed does not
// /*AFLA*/  * allow constant folding and the actual conversion must be done at
// /*AFLA*/  * runtime.
// /*AFLA*/  * the HZ range specific helpers _usecs_to_jiffies() are called both
// /*AFLA*/  * directly here and from __msecs_to_jiffies() in the case where
// /*AFLA*/  * constant folding is not possible.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline unsigned long usecs_to_jiffies(const unsigned int u)
// /*AFLA*/ {
// /*AFLA*/ 	if (__builtin_constant_p(u)) {
// /*AFLA*/ 		if (u > jiffies_to_usecs(MAX_JIFFY_OFFSET))
// /*AFLA*/ 			return MAX_JIFFY_OFFSET;
// /*AFLA*/ 		return _usecs_to_jiffies(u);
// /*AFLA*/ 	} else {
// /*AFLA*/ 		return __usecs_to_jiffies(u);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long timespec64_to_jiffies(const struct timespec64 *value);
// /*AFLA*/ extern void jiffies_to_timespec64(const unsigned long jiffies,
// /*AFLA*/ 				  struct timespec64 *value);
// /*AFLA*/ static inline unsigned long timespec_to_jiffies(const struct timespec *value)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec64 ts = timespec_to_timespec64(*value);
// /*AFLA*/ 
// /*AFLA*/ 	return timespec64_to_jiffies(&ts);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void jiffies_to_timespec(const unsigned long jiffies,
// /*AFLA*/ 				       struct timespec *value)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec64 ts;
// /*AFLA*/ 
// /*AFLA*/ 	jiffies_to_timespec64(jiffies, &ts);
// /*AFLA*/ 	*value = timespec64_to_timespec(ts);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long timeval_to_jiffies(const struct timeval *value);
// /*AFLA*/ extern void jiffies_to_timeval(const unsigned long jiffies,
// /*AFLA*/ 			       struct timeval *value);
// /*AFLA*/ 
// /*AFLA*/ extern clock_t jiffies_to_clock_t(unsigned long x);
// /*AFLA*/ static inline clock_t jiffies_delta_to_clock_t(long delta)
// /*AFLA*/ {
// /*AFLA*/ 	return jiffies_to_clock_t(max(0L, delta));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long clock_t_to_jiffies(unsigned long x);
// /*AFLA*/ extern u64 jiffies_64_to_clock_t(u64 x);
// /*AFLA*/ extern u64 nsec_to_clock_t(u64 x);
// /*AFLA*/ extern u64 nsecs_to_jiffies64(u64 n);
// /*AFLA*/ extern unsigned long nsecs_to_jiffies(u64 n);
// /*AFLA*/ 
// /*AFLA*/ #define TIMESTAMP_SIZE	30
// /*AFLA*/ 
// /*AFLA*/ #endif
