// /*AFLA*/ #ifndef _LINUX_THREADS_H
// /*AFLA*/ #define _LINUX_THREADS_H
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The default limit for the nr of threads is now in
// /*AFLA*/  * /proc/sys/kernel/threads-max.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Maximum supported processors.  Setting this smaller saves quite a
// /*AFLA*/  * bit of memory.  Use nr_cpu_ids instead of this except for static bitmaps.
// /*AFLA*/  */
// /*AFLA*/ #ifndef CONFIG_NR_CPUS
// /*AFLA*/ /* FIXME: This should be fixed in the arch's Kconfig */
// /*AFLA*/ #define CONFIG_NR_CPUS	1
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Places which use this should consider cpumask_var_t. */
// /*AFLA*/ #define NR_CPUS		CONFIG_NR_CPUS
// /*AFLA*/ 
// /*AFLA*/ #define MIN_THREADS_LEFT_FOR_ROOT 4
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This controls the default maximum pid allocated to a process
// /*AFLA*/  */
// /*AFLA*/ #define PID_MAX_DEFAULT (CONFIG_BASE_SMALL ? 0x1000 : 0x8000)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A maximum of 4 million PIDs should be enough for a while.
// /*AFLA*/  * [NOTE: PID/TIDs are limited to 2^29 ~= 500+ million, see futex.h.]
// /*AFLA*/  */
// /*AFLA*/ #define PID_MAX_LIMIT (CONFIG_BASE_SMALL ? PAGE_SIZE * 8 : \
// /*AFLA*/ 	(sizeof(long) > 4 ? 4 * 1024 * 1024 : PID_MAX_DEFAULT))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Define a minimum number of pids per cpu.  Heuristically based
// /*AFLA*/  * on original pid max of 32k for 32 cpus.  Also, increase the
// /*AFLA*/  * minimum settable value for pid_max on the running system based
// /*AFLA*/  * on similar defaults.  See kernel/pid.c:pidmap_init() for details.
// /*AFLA*/  */
// /*AFLA*/ #define PIDS_PER_CPU_DEFAULT	1024
// /*AFLA*/ #define PIDS_PER_CPU_MIN	8
// /*AFLA*/ 
// /*AFLA*/ #endif
