// /*AFLA*/ #ifndef _LINUX_TIMEKEEPING_H
// /*AFLA*/ #define _LINUX_TIMEKEEPING_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ 
// /*AFLA*/ /* Included from linux/ktime.h */
// /*AFLA*/ 
// /*AFLA*/ void timekeeping_init(void);
// /*AFLA*/ extern int timekeeping_suspended;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Get and set timeofday
// /*AFLA*/  */
// /*AFLA*/ extern void do_gettimeofday(struct timeval *tv);
// /*AFLA*/ extern int do_settimeofday64(const struct timespec64 *ts);
// /*AFLA*/ extern int do_sys_settimeofday64(const struct timespec64 *tv,
// /*AFLA*/ 				 const struct timezone *tz);
// /*AFLA*/ static inline int do_sys_settimeofday(const struct timespec *tv,
// /*AFLA*/ 				      const struct timezone *tz)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec64 ts64;
// /*AFLA*/ 
// /*AFLA*/ 	if (!tv)
// /*AFLA*/ 		return do_sys_settimeofday64(NULL, tz);
// /*AFLA*/ 
// /*AFLA*/ 	if (!timespec_valid(tv))
// /*AFLA*/ 		return -EINVAL;
// /*AFLA*/ 
// /*AFLA*/ 	ts64 = timespec_to_timespec64(*tv);
// /*AFLA*/ 	return do_sys_settimeofday64(&ts64, tz);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Kernel time accessors
// /*AFLA*/  */
// /*AFLA*/ unsigned long get_seconds(void);
// /*AFLA*/ struct timespec64 current_kernel_time64(void);
// /*AFLA*/ /* does not take xtime_lock */
// /*AFLA*/ struct timespec __current_kernel_time(void);
// /*AFLA*/ 
// /*AFLA*/ static inline struct timespec current_kernel_time(void)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec64 now = current_kernel_time64();
// /*AFLA*/ 
// /*AFLA*/ 	return timespec64_to_timespec(now);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * timespec based interfaces
// /*AFLA*/  */
// /*AFLA*/ struct timespec64 get_monotonic_coarse64(void);
// /*AFLA*/ extern void getrawmonotonic64(struct timespec64 *ts);
// /*AFLA*/ extern void ktime_get_ts64(struct timespec64 *ts);
// /*AFLA*/ extern time64_t ktime_get_seconds(void);
// /*AFLA*/ extern time64_t ktime_get_real_seconds(void);
// /*AFLA*/ 
// /*AFLA*/ extern int __getnstimeofday64(struct timespec64 *tv);
// /*AFLA*/ extern void getnstimeofday64(struct timespec64 *tv);
// /*AFLA*/ extern void getboottime64(struct timespec64 *ts);
// /*AFLA*/ 
// /*AFLA*/ #if BITS_PER_LONG == 64
// /*AFLA*/ /**
// /*AFLA*/  * Deprecated. Use do_settimeofday64().
// /*AFLA*/  */
// /*AFLA*/ static inline int do_settimeofday(const struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	return do_settimeofday64(ts);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __getnstimeofday(struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	return __getnstimeofday64(ts);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void getnstimeofday(struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	getnstimeofday64(ts);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ktime_get_ts(struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	ktime_get_ts64(ts);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ktime_get_real_ts(struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	getnstimeofday64(ts);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void getrawmonotonic(struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	getrawmonotonic64(ts);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct timespec get_monotonic_coarse(void)
// /*AFLA*/ {
// /*AFLA*/ 	return get_monotonic_coarse64();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void getboottime(struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	return getboottime64(ts);
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ /**
// /*AFLA*/  * Deprecated. Use do_settimeofday64().
// /*AFLA*/  */
// /*AFLA*/ static inline int do_settimeofday(const struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec64 ts64;
// /*AFLA*/ 
// /*AFLA*/ 	ts64 = timespec_to_timespec64(*ts);
// /*AFLA*/ 	return do_settimeofday64(&ts64);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __getnstimeofday(struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec64 ts64;
// /*AFLA*/ 	int ret = __getnstimeofday64(&ts64);
// /*AFLA*/ 
// /*AFLA*/ 	*ts = timespec64_to_timespec(ts64);
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void getnstimeofday(struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec64 ts64;
// /*AFLA*/ 
// /*AFLA*/ 	getnstimeofday64(&ts64);
// /*AFLA*/ 	*ts = timespec64_to_timespec(ts64);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ktime_get_ts(struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec64 ts64;
// /*AFLA*/ 
// /*AFLA*/ 	ktime_get_ts64(&ts64);
// /*AFLA*/ 	*ts = timespec64_to_timespec(ts64);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ktime_get_real_ts(struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec64 ts64;
// /*AFLA*/ 
// /*AFLA*/ 	getnstimeofday64(&ts64);
// /*AFLA*/ 	*ts = timespec64_to_timespec(ts64);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void getrawmonotonic(struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec64 ts64;
// /*AFLA*/ 
// /*AFLA*/ 	getrawmonotonic64(&ts64);
// /*AFLA*/ 	*ts = timespec64_to_timespec(ts64);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct timespec get_monotonic_coarse(void)
// /*AFLA*/ {
// /*AFLA*/ 	return timespec64_to_timespec(get_monotonic_coarse64());
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void getboottime(struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec64 ts64;
// /*AFLA*/ 
// /*AFLA*/ 	getboottime64(&ts64);
// /*AFLA*/ 	*ts = timespec64_to_timespec(ts64);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define ktime_get_real_ts64(ts)	getnstimeofday64(ts)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * ktime_t based interfaces
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ enum tk_offsets {
// /*AFLA*/ 	TK_OFFS_REAL,
// /*AFLA*/ 	TK_OFFS_BOOT,
// /*AFLA*/ 	TK_OFFS_TAI,
// /*AFLA*/ 	TK_OFFS_MAX,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern ktime_t ktime_get(void);
// /*AFLA*/ extern ktime_t ktime_get_with_offset(enum tk_offsets offs);
// /*AFLA*/ extern ktime_t ktime_mono_to_any(ktime_t tmono, enum tk_offsets offs);
// /*AFLA*/ extern ktime_t ktime_get_raw(void);
// /*AFLA*/ extern u32 ktime_get_resolution_ns(void);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ktime_get_real - get the real (wall-) time in ktime_t format
// /*AFLA*/  */
// /*AFLA*/ static inline ktime_t ktime_get_real(void)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_get_with_offset(TK_OFFS_REAL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ktime_get_boottime - Returns monotonic time since boot in ktime_t format
// /*AFLA*/  *
// /*AFLA*/  * This is similar to CLOCK_MONTONIC/ktime_get, but also includes the
// /*AFLA*/  * time spent in suspend.
// /*AFLA*/  */
// /*AFLA*/ static inline ktime_t ktime_get_boottime(void)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_get_with_offset(TK_OFFS_BOOT);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ktime_get_clocktai - Returns the TAI time of day in ktime_t format
// /*AFLA*/  */
// /*AFLA*/ static inline ktime_t ktime_get_clocktai(void)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_get_with_offset(TK_OFFS_TAI);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ktime_mono_to_real - Convert monotonic time to clock realtime
// /*AFLA*/  */
// /*AFLA*/ static inline ktime_t ktime_mono_to_real(ktime_t mono)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_mono_to_any(mono, TK_OFFS_REAL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u64 ktime_get_ns(void)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_to_ns(ktime_get());
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u64 ktime_get_real_ns(void)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_to_ns(ktime_get_real());
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u64 ktime_get_boot_ns(void)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_to_ns(ktime_get_boottime());
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u64 ktime_get_tai_ns(void)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_to_ns(ktime_get_clocktai());
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u64 ktime_get_raw_ns(void)
// /*AFLA*/ {
// /*AFLA*/ 	return ktime_to_ns(ktime_get_raw());
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern u64 ktime_get_mono_fast_ns(void);
// /*AFLA*/ extern u64 ktime_get_raw_fast_ns(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Timespec interfaces utilizing the ktime based ones
// /*AFLA*/  */
// /*AFLA*/ static inline void get_monotonic_boottime(struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	*ts = ktime_to_timespec(ktime_get_boottime());
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void get_monotonic_boottime64(struct timespec64 *ts)
// /*AFLA*/ {
// /*AFLA*/ 	*ts = ktime_to_timespec64(ktime_get_boottime());
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void timekeeping_clocktai(struct timespec *ts)
// /*AFLA*/ {
// /*AFLA*/ 	*ts = ktime_to_timespec(ktime_get_clocktai());
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * RTC specific
// /*AFLA*/  */
// /*AFLA*/ extern bool timekeeping_rtc_skipsuspend(void);
// /*AFLA*/ extern bool timekeeping_rtc_skipresume(void);
// /*AFLA*/ 
// /*AFLA*/ extern void timekeeping_inject_sleeptime64(struct timespec64 *delta);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * PPS accessor
// /*AFLA*/  */
// /*AFLA*/ extern void ktime_get_raw_and_real_ts64(struct timespec64 *ts_raw,
// /*AFLA*/ 				        struct timespec64 *ts_real);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * struct system_time_snapshot - simultaneous raw/real time capture with
// /*AFLA*/  *	counter value
// /*AFLA*/  * @cycles:	Clocksource counter value to produce the system times
// /*AFLA*/  * @real:	Realtime system time
// /*AFLA*/  * @raw:	Monotonic raw system time
// /*AFLA*/  * @clock_was_set_seq:	The sequence number of clock was set events
// /*AFLA*/  * @cs_was_changed_seq:	The sequence number of clocksource change events
// /*AFLA*/  */
// /*AFLA*/ struct system_time_snapshot {
// /*AFLA*/ 	cycle_t		cycles;
// /*AFLA*/ 	ktime_t		real;
// /*AFLA*/ 	ktime_t		raw;
// /*AFLA*/ 	unsigned int	clock_was_set_seq;
// /*AFLA*/ 	u8		cs_was_changed_seq;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * struct system_device_crosststamp - system/device cross-timestamp
// /*AFLA*/  *	(syncronized capture)
// /*AFLA*/  * @device:		Device time
// /*AFLA*/  * @sys_realtime:	Realtime simultaneous with device time
// /*AFLA*/  * @sys_monoraw:	Monotonic raw simultaneous with device time
// /*AFLA*/  */
// /*AFLA*/ struct system_device_crosststamp {
// /*AFLA*/ 	ktime_t device;
// /*AFLA*/ 	ktime_t sys_realtime;
// /*AFLA*/ 	ktime_t sys_monoraw;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * struct system_counterval_t - system counter value with the pointer to the
// /*AFLA*/  *	corresponding clocksource
// /*AFLA*/  * @cycles:	System counter value
// /*AFLA*/  * @cs:		Clocksource corresponding to system counter value. Used by
// /*AFLA*/  *	timekeeping code to verify comparibility of two cycle values
// /*AFLA*/  */
// /*AFLA*/ struct system_counterval_t {
// /*AFLA*/ 	cycle_t			cycles;
// /*AFLA*/ 	struct clocksource	*cs;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Get cross timestamp between system clock and device clock
// /*AFLA*/  */
// /*AFLA*/ extern int get_device_system_crosststamp(
// /*AFLA*/ 			int (*get_time_fn)(ktime_t *device_time,
// /*AFLA*/ 				struct system_counterval_t *system_counterval,
// /*AFLA*/ 				void *ctx),
// /*AFLA*/ 			void *ctx,
// /*AFLA*/ 			struct system_time_snapshot *history,
// /*AFLA*/ 			struct system_device_crosststamp *xtstamp);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Simultaneously snapshot realtime and monotonic raw clocks
// /*AFLA*/  */
// /*AFLA*/ extern void ktime_get_snapshot(struct system_time_snapshot *systime_snapshot);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Persistent clock related interfaces
// /*AFLA*/  */
// /*AFLA*/ extern int persistent_clock_is_local;
// /*AFLA*/ 
// /*AFLA*/ extern void read_persistent_clock(struct timespec *ts);
// /*AFLA*/ extern void read_persistent_clock64(struct timespec64 *ts);
// /*AFLA*/ extern void read_boot_clock64(struct timespec64 *ts);
// /*AFLA*/ extern int update_persistent_clock(struct timespec now);
// /*AFLA*/ extern int update_persistent_clock64(struct timespec64 now);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif
