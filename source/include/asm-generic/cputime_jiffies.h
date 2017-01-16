// /*AFLA*/ #ifndef _ASM_GENERIC_CPUTIME_JIFFIES_H
// /*AFLA*/ #define _ASM_GENERIC_CPUTIME_JIFFIES_H
// /*AFLA*/ 
// /*AFLA*/ typedef unsigned long __nocast cputime_t;
// /*AFLA*/ 
// /*AFLA*/ #define cmpxchg_cputime(ptr, old, new) cmpxchg(ptr, old, new)
// /*AFLA*/ 
// /*AFLA*/ #define cputime_one_jiffy		jiffies_to_cputime(1)
// /*AFLA*/ #define cputime_to_jiffies(__ct)	(__force unsigned long)(__ct)
// /*AFLA*/ #define cputime_to_scaled(__ct)		(__ct)
// /*AFLA*/ #define jiffies_to_cputime(__hz)	(__force cputime_t)(__hz)
// /*AFLA*/ 
// /*AFLA*/ typedef u64 __nocast cputime64_t;
// /*AFLA*/ 
// /*AFLA*/ #define cputime64_to_jiffies64(__ct)	(__force u64)(__ct)
// /*AFLA*/ #define jiffies64_to_cputime64(__jif)	(__force cputime64_t)(__jif)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Convert nanoseconds <-> cputime
// /*AFLA*/  */
// /*AFLA*/ #define cputime_to_nsecs(__ct)		\
// /*AFLA*/ 	jiffies_to_nsecs(cputime_to_jiffies(__ct))
// /*AFLA*/ #define nsecs_to_cputime64(__nsec)	\
// /*AFLA*/ 	jiffies64_to_cputime64(nsecs_to_jiffies64(__nsec))
// /*AFLA*/ #define nsecs_to_cputime(__nsec)	\
// /*AFLA*/ 	jiffies_to_cputime(nsecs_to_jiffies(__nsec))
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Convert cputime to microseconds and back.
// /*AFLA*/  */
// /*AFLA*/ #define cputime_to_usecs(__ct)		\
// /*AFLA*/ 	jiffies_to_usecs(cputime_to_jiffies(__ct))
// /*AFLA*/ #define usecs_to_cputime(__usec)	\
// /*AFLA*/ 	jiffies_to_cputime(usecs_to_jiffies(__usec))
// /*AFLA*/ #define usecs_to_cputime64(__usec)	\
// /*AFLA*/ 	jiffies64_to_cputime64(nsecs_to_jiffies64((__usec) * 1000))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Convert cputime to seconds and back.
// /*AFLA*/  */
// /*AFLA*/ #define cputime_to_secs(jif)		(cputime_to_jiffies(jif) / HZ)
// /*AFLA*/ #define secs_to_cputime(sec)		jiffies_to_cputime((sec) * HZ)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Convert cputime to timespec and back.
// /*AFLA*/  */
// /*AFLA*/ #define timespec_to_cputime(__val)	\
// /*AFLA*/ 	jiffies_to_cputime(timespec_to_jiffies(__val))
// /*AFLA*/ #define cputime_to_timespec(__ct,__val)	\
// /*AFLA*/ 	jiffies_to_timespec(cputime_to_jiffies(__ct),__val)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Convert cputime to timeval and back.
// /*AFLA*/  */
// /*AFLA*/ #define timeval_to_cputime(__val)	\
// /*AFLA*/ 	jiffies_to_cputime(timeval_to_jiffies(__val))
// /*AFLA*/ #define cputime_to_timeval(__ct,__val)	\
// /*AFLA*/ 	jiffies_to_timeval(cputime_to_jiffies(__ct),__val)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Convert cputime to clock and back.
// /*AFLA*/  */
// /*AFLA*/ #define cputime_to_clock_t(__ct)	\
// /*AFLA*/ 	jiffies_to_clock_t(cputime_to_jiffies(__ct))
// /*AFLA*/ #define clock_t_to_cputime(__x)		\
// /*AFLA*/ 	jiffies_to_cputime(clock_t_to_jiffies(__x))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Convert cputime64 to clock.
// /*AFLA*/  */
// /*AFLA*/ #define cputime64_to_clock_t(__ct)	\
// /*AFLA*/ 	jiffies_64_to_clock_t(cputime64_to_jiffies64(__ct))
// /*AFLA*/ 
// /*AFLA*/ #endif
