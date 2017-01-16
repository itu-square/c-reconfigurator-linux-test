// /*AFLA*/ #ifndef TRACEPOINT_DEFS_H
// /*AFLA*/ #define TRACEPOINT_DEFS_H 1
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * File can be included directly by headers who only want to access
// /*AFLA*/  * tracepoint->key to guard out of line trace calls, or the definition of
// /*AFLA*/  * trace_print_flags{_u64}. Otherwise linux/tracepoint.h should be used.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/static_key.h>
// /*AFLA*/ 
// /*AFLA*/ struct trace_print_flags {
// /*AFLA*/ 	unsigned long		mask;
// /*AFLA*/ 	const char		*name;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct trace_print_flags_u64 {
// /*AFLA*/ 	unsigned long long	mask;
// /*AFLA*/ 	const char		*name;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct tracepoint_func {
// /*AFLA*/ 	void *func;
// /*AFLA*/ 	void *data;
// /*AFLA*/ 	int prio;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct tracepoint {
// /*AFLA*/ 	const char *name;		/* Tracepoint name */
// /*AFLA*/ 	struct static_key key;
// /*AFLA*/ 	void (*regfunc)(void);
// /*AFLA*/ 	void (*unregfunc)(void);
// /*AFLA*/ 	struct tracepoint_func __rcu *funcs;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif
