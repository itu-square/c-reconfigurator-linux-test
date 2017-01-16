// /*AFLA*/ #ifndef _TRACE_SYSCALL_H
// /*AFLA*/ #define _TRACE_SYSCALL_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/tracepoint.h>
// /*AFLA*/ #include <linux/unistd.h>
// /*AFLA*/ #include <linux/trace_events.h>
// /*AFLA*/ #include <linux/thread_info.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/ptrace.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A syscall entry in the ftrace syscalls array.
// /*AFLA*/  *
// /*AFLA*/  * @name: name of the syscall
// /*AFLA*/  * @syscall_nr: number of the syscall
// /*AFLA*/  * @nb_args: number of parameters it takes
// /*AFLA*/  * @types: list of types as strings
// /*AFLA*/  * @args: list of args as strings (args[i] matches types[i])
// /*AFLA*/  * @enter_fields: list of fields for syscall_enter trace event
// /*AFLA*/  * @enter_event: associated syscall_enter trace event
// /*AFLA*/  * @exit_event: associated syscall_exit trace event
// /*AFLA*/  */
// /*AFLA*/ struct syscall_metadata {
// /*AFLA*/ 	const char	*name;
// /*AFLA*/ 	int		syscall_nr;
// /*AFLA*/ 	int		nb_args;
// /*AFLA*/ 	const char	**types;
// /*AFLA*/ 	const char	**args;
// /*AFLA*/ 	struct list_head enter_fields;
// /*AFLA*/ 
// /*AFLA*/ 	struct trace_event_call *enter_event;
// /*AFLA*/ 	struct trace_event_call *exit_event;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_TRACEPOINTS) && defined(CONFIG_HAVE_SYSCALL_TRACEPOINTS)
// /*AFLA*/ static inline void syscall_tracepoint_update(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	if (test_thread_flag(TIF_SYSCALL_TRACEPOINT))
// /*AFLA*/ 		set_tsk_thread_flag(p, TIF_SYSCALL_TRACEPOINT);
// /*AFLA*/ 	else
// /*AFLA*/ 		clear_tsk_thread_flag(p, TIF_SYSCALL_TRACEPOINT);
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline void syscall_tracepoint_update(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _TRACE_SYSCALL_H */
