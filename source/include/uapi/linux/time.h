// /*AFLA*/ #ifndef _UAPI_LINUX_TIME_H
// /*AFLA*/ #define _UAPI_LINUX_TIME_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifndef _STRUCT_TIMESPEC
// /*AFLA*/ #define _STRUCT_TIMESPEC
// /*AFLA*/ struct timespec {
// /*AFLA*/ 	__kernel_time_t	tv_sec;			/* seconds */
// /*AFLA*/ 	long		tv_nsec;		/* nanoseconds */
// /*AFLA*/ };
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct timeval {
// /*AFLA*/ 	__kernel_time_t		tv_sec;		/* seconds */
// /*AFLA*/ 	__kernel_suseconds_t	tv_usec;	/* microseconds */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct timezone {
// /*AFLA*/ 	int	tz_minuteswest;	/* minutes west of Greenwich */
// /*AFLA*/ 	int	tz_dsttime;	/* type of dst correction */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Names of the interval timers, and structure
// /*AFLA*/  * defining a timer setting:
// /*AFLA*/  */
// /*AFLA*/ #define	ITIMER_REAL		0
// /*AFLA*/ #define	ITIMER_VIRTUAL		1
// /*AFLA*/ #define	ITIMER_PROF		2
// /*AFLA*/ 
// /*AFLA*/ struct itimerspec {
// /*AFLA*/ 	struct timespec it_interval;	/* timer period */
// /*AFLA*/ 	struct timespec it_value;	/* timer expiration */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct itimerval {
// /*AFLA*/ 	struct timeval it_interval;	/* timer interval */
// /*AFLA*/ 	struct timeval it_value;	/* current value */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The IDs of the various system clocks (for POSIX.1b interval timers):
// /*AFLA*/  */
// /*AFLA*/ #define CLOCK_REALTIME			0
// /*AFLA*/ #define CLOCK_MONOTONIC			1
// /*AFLA*/ #define CLOCK_PROCESS_CPUTIME_ID	2
// /*AFLA*/ #define CLOCK_THREAD_CPUTIME_ID		3
// /*AFLA*/ #define CLOCK_MONOTONIC_RAW		4
// /*AFLA*/ #define CLOCK_REALTIME_COARSE		5
// /*AFLA*/ #define CLOCK_MONOTONIC_COARSE		6
// /*AFLA*/ #define CLOCK_BOOTTIME			7
// /*AFLA*/ #define CLOCK_REALTIME_ALARM		8
// /*AFLA*/ #define CLOCK_BOOTTIME_ALARM		9
// /*AFLA*/ #define CLOCK_SGI_CYCLE			10	/* Hardware specific */
// /*AFLA*/ #define CLOCK_TAI			11
// /*AFLA*/ 
// /*AFLA*/ #define MAX_CLOCKS			16
// /*AFLA*/ #define CLOCKS_MASK			(CLOCK_REALTIME | CLOCK_MONOTONIC)
// /*AFLA*/ #define CLOCKS_MONO			CLOCK_MONOTONIC
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The various flags for setting POSIX.1b interval timers:
// /*AFLA*/  */
// /*AFLA*/ #define TIMER_ABSTIME			0x01
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_TIME_H */
