// /*AFLA*/ /*
// /*AFLA*/  * x86 TSC related functions
// /*AFLA*/  */
// /*AFLA*/ #ifndef _ASM_X86_TSC_H
// /*AFLA*/ #define _ASM_X86_TSC_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/processor.h>
// /*AFLA*/ 
// /*AFLA*/ #define NS_SCALE	10 /* 2^10, carefully chosen */
// /*AFLA*/ #define US_SCALE	32 /* 2^32, arbitralrily chosen */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Standard way to access the cycle counter.
// /*AFLA*/  */
// /*AFLA*/ typedef unsigned long long cycles_t;
// /*AFLA*/ 
// /*AFLA*/ extern unsigned int cpu_khz;
// /*AFLA*/ extern unsigned int tsc_khz;
// /*AFLA*/ 
// /*AFLA*/ extern void disable_TSC(void);
// /*AFLA*/ 
// /*AFLA*/ static inline cycles_t get_cycles(void)
// /*AFLA*/ {
// /*AFLA*/ #ifndef CONFIG_X86_TSC
// /*AFLA*/ 	if (!boot_cpu_has(X86_FEATURE_TSC))
// /*AFLA*/ 		return 0;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	return rdtsc();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct system_counterval_t convert_art_to_tsc(cycle_t art);
// /*AFLA*/ 
// /*AFLA*/ extern void tsc_init(void);
// /*AFLA*/ extern void mark_tsc_unstable(char *reason);
// /*AFLA*/ extern int unsynchronized_tsc(void);
// /*AFLA*/ extern int check_tsc_unstable(void);
// /*AFLA*/ extern unsigned long native_calibrate_cpu(void);
// /*AFLA*/ extern unsigned long native_calibrate_tsc(void);
// /*AFLA*/ extern unsigned long long native_sched_clock_from_tsc(u64 tsc);
// /*AFLA*/ 
// /*AFLA*/ extern int tsc_clocksource_reliable;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Boot-time check whether the TSCs are synchronized across
// /*AFLA*/  * all CPUs/cores:
// /*AFLA*/  */
// /*AFLA*/ extern void check_tsc_sync_source(int cpu);
// /*AFLA*/ extern void check_tsc_sync_target(void);
// /*AFLA*/ 
// /*AFLA*/ extern int notsc_setup(char *);
// /*AFLA*/ extern void tsc_save_sched_clock_state(void);
// /*AFLA*/ extern void tsc_restore_sched_clock_state(void);
// /*AFLA*/ 
// /*AFLA*/ unsigned long cpu_khz_from_msr(void);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_TSC_H */
