// /*AFLA*/ #ifndef _LINUX_TRACE_CLOCK_H
// /*AFLA*/ #define _LINUX_TRACE_CLOCK_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * 3 trace clock variants, with differing scalability/precision
// /*AFLA*/  * tradeoffs:
// /*AFLA*/  *
// /*AFLA*/  *  -   local: CPU-local trace clock
// /*AFLA*/  *  -  medium: scalable global clock with some jitter
// /*AFLA*/  *  -  global: globally monotonic, serialized clock
// /*AFLA*/  */
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/trace_clock.h>
// /*AFLA*/ 
// /*AFLA*/ extern u64 notrace trace_clock_local(void);
// /*AFLA*/ extern u64 notrace trace_clock(void);
// /*AFLA*/ extern u64 notrace trace_clock_jiffies(void);
// /*AFLA*/ extern u64 notrace trace_clock_global(void);
// /*AFLA*/ extern u64 notrace trace_clock_counter(void);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_TRACE_CLOCK_H */
