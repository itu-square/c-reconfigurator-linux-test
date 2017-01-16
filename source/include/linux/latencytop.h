// /*AFLA*/ /*
// /*AFLA*/  * latencytop.h: Infrastructure for displaying latency
// /*AFLA*/  *
// /*AFLA*/  * (C) Copyright 2008 Intel Corporation
// /*AFLA*/  * Author: Arjan van de Ven <arjan@linux.intel.com>
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _INCLUDE_GUARD_LATENCYTOP_H_
// /*AFLA*/ #define _INCLUDE_GUARD_LATENCYTOP_H_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ struct task_struct;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_LATENCYTOP
// /*AFLA*/ 
// /*AFLA*/ #define LT_SAVECOUNT		32
// /*AFLA*/ #define LT_BACKTRACEDEPTH	12
// /*AFLA*/ 
// /*AFLA*/ struct latency_record {
// /*AFLA*/ 	unsigned long	backtrace[LT_BACKTRACEDEPTH];
// /*AFLA*/ 	unsigned int	count;
// /*AFLA*/ 	unsigned long	time;
// /*AFLA*/ 	unsigned long	max;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ extern int latencytop_enabled;
// /*AFLA*/ void __account_scheduler_latency(struct task_struct *task, int usecs, int inter);
// /*AFLA*/ static inline void
// /*AFLA*/ account_scheduler_latency(struct task_struct *task, int usecs, int inter)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(latencytop_enabled))
// /*AFLA*/ 		__account_scheduler_latency(task, usecs, inter);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void clear_all_latency_tracing(struct task_struct *p);
// /*AFLA*/ 
// /*AFLA*/ extern int sysctl_latencytop(struct ctl_table *table, int write,
// /*AFLA*/ 			void __user *buffer, size_t *lenp, loff_t *ppos);
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ account_scheduler_latency(struct task_struct *task, int usecs, int inter)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void clear_all_latency_tracing(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
