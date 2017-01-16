// /*AFLA*/ #ifndef __LINUX_STACKTRACE_H
// /*AFLA*/ #define __LINUX_STACKTRACE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ struct task_struct;
// /*AFLA*/ struct pt_regs;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_STACKTRACE
// /*AFLA*/ struct stack_trace {
// /*AFLA*/ 	unsigned int nr_entries, max_entries;
// /*AFLA*/ 	unsigned long *entries;
// /*AFLA*/ 	int skip;	/* input argument: How many entries to skip */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern void save_stack_trace(struct stack_trace *trace);
// /*AFLA*/ extern void save_stack_trace_regs(struct pt_regs *regs,
// /*AFLA*/ 				  struct stack_trace *trace);
// /*AFLA*/ extern void save_stack_trace_tsk(struct task_struct *tsk,
// /*AFLA*/ 				struct stack_trace *trace);
// /*AFLA*/ 
// /*AFLA*/ extern void print_stack_trace(struct stack_trace *trace, int spaces);
// /*AFLA*/ extern int snprint_stack_trace(char *buf, size_t size,
// /*AFLA*/ 			struct stack_trace *trace, int spaces);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_USER_STACKTRACE_SUPPORT
// /*AFLA*/ extern void save_stack_trace_user(struct stack_trace *trace);
// /*AFLA*/ #else
// /*AFLA*/ # define save_stack_trace_user(trace)              do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ # define save_stack_trace(trace)			do { } while (0)
// /*AFLA*/ # define save_stack_trace_tsk(tsk, trace)		do { } while (0)
// /*AFLA*/ # define save_stack_trace_user(trace)			do { } while (0)
// /*AFLA*/ # define print_stack_trace(trace, spaces)		do { } while (0)
// /*AFLA*/ # define snprint_stack_trace(buf, size, trace, spaces)	do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
