// /*AFLA*/ #ifndef _LINUX_TRACEPOINT_H
// /*AFLA*/ #define _LINUX_TRACEPOINT_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Kernel Tracepoint API.
// /*AFLA*/  *
// /*AFLA*/  * See Documentation/trace/tracepoints.txt.
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2008-2014 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
// /*AFLA*/  *
// /*AFLA*/  * Heavily inspired from the Linux Kernel Markers.
// /*AFLA*/  *
// /*AFLA*/  * This file is released under the GPLv2.
// /*AFLA*/  * See the file COPYING for more details.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/smp.h>
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ #include <linux/tracepoint-defs.h>
// /*AFLA*/ 
// /*AFLA*/ struct module;
// /*AFLA*/ struct tracepoint;
// /*AFLA*/ struct notifier_block;
// /*AFLA*/ 
// /*AFLA*/ struct trace_enum_map {
// /*AFLA*/ 	const char		*system;
// /*AFLA*/ 	const char		*enum_string;
// /*AFLA*/ 	unsigned long		enum_value;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define TRACEPOINT_DEFAULT_PRIO	10
// /*AFLA*/ 
// /*AFLA*/ extern int
// /*AFLA*/ tracepoint_probe_register(struct tracepoint *tp, void *probe, void *data);
// /*AFLA*/ extern int
// /*AFLA*/ tracepoint_probe_register_prio(struct tracepoint *tp, void *probe, void *data,
// /*AFLA*/ 			       int prio);
// /*AFLA*/ extern int
// /*AFLA*/ tracepoint_probe_unregister(struct tracepoint *tp, void *probe, void *data);
// /*AFLA*/ extern void
// /*AFLA*/ for_each_kernel_tracepoint(void (*fct)(struct tracepoint *tp, void *priv),
// /*AFLA*/ 		void *priv);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MODULES
// /*AFLA*/ struct tp_module {
// /*AFLA*/ 	struct list_head list;
// /*AFLA*/ 	struct module *mod;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ bool trace_module_has_bad_taint(struct module *mod);
// /*AFLA*/ extern int register_tracepoint_module_notifier(struct notifier_block *nb);
// /*AFLA*/ extern int unregister_tracepoint_module_notifier(struct notifier_block *nb);
// /*AFLA*/ #else
// /*AFLA*/ static inline bool trace_module_has_bad_taint(struct module *mod)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ static inline
// /*AFLA*/ int register_tracepoint_module_notifier(struct notifier_block *nb)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline
// /*AFLA*/ int unregister_tracepoint_module_notifier(struct notifier_block *nb)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_MODULES */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * tracepoint_synchronize_unregister must be called between the last tracepoint
// /*AFLA*/  * probe unregistration and the end of module exit to make sure there is no
// /*AFLA*/  * caller executing a probe when it is freed.
// /*AFLA*/  */
// /*AFLA*/ static inline void tracepoint_synchronize_unregister(void)
// /*AFLA*/ {
// /*AFLA*/ 	synchronize_sched();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_SYSCALL_TRACEPOINTS
// /*AFLA*/ extern void syscall_regfunc(void);
// /*AFLA*/ extern void syscall_unregfunc(void);
// /*AFLA*/ #endif /* CONFIG_HAVE_SYSCALL_TRACEPOINTS */
// /*AFLA*/ 
// /*AFLA*/ #define PARAMS(args...) args
// /*AFLA*/ 
// /*AFLA*/ #define TRACE_DEFINE_ENUM(x)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_TRACEPOINT_H */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Note: we keep the TRACE_EVENT and DECLARE_TRACE outside the include
// /*AFLA*/  *  file ifdef protection.
// /*AFLA*/  *  This is due to the way trace events work. If a file includes two
// /*AFLA*/  *  trace event headers under one "CREATE_TRACE_POINTS" the first include
// /*AFLA*/  *  will override the TRACE_EVENT and break the second include.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef DECLARE_TRACE
// /*AFLA*/ 
// /*AFLA*/ #define TP_PROTO(args...)	args
// /*AFLA*/ #define TP_ARGS(args...)	args
// /*AFLA*/ #define TP_CONDITION(args...)	args
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Individual subsystem my have a separate configuration to
// /*AFLA*/  * enable their tracepoints. By default, this file will create
// /*AFLA*/  * the tracepoints if CONFIG_TRACEPOINT is defined. If a subsystem
// /*AFLA*/  * wants to be able to disable its tracepoints from being created
// /*AFLA*/  * it can define NOTRACE before including the tracepoint headers.
// /*AFLA*/  */
// /*AFLA*/ #if defined(CONFIG_TRACEPOINTS) && !defined(NOTRACE)
// /*AFLA*/ #define TRACEPOINTS_ENABLED
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef TRACEPOINTS_ENABLED
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * it_func[0] is never NULL because there is at least one element in the array
// /*AFLA*/  * when the array itself is non NULL.
// /*AFLA*/  *
// /*AFLA*/  * Note, the proto and args passed in includes "__data" as the first parameter.
// /*AFLA*/  * The reason for this is to handle the "void" prototype. If a tracepoint
// /*AFLA*/  * has a "void" prototype, then it is invalid to declare a function
// /*AFLA*/  * as "(void *, void)". The DECLARE_TRACE_NOARGS() will pass in just
// /*AFLA*/  * "void *data", where as the DECLARE_TRACE() will pass in "void *data, proto".
// /*AFLA*/  */
// /*AFLA*/ #define __DO_TRACE(tp, proto, args, cond, prercu, postrcu)		\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		struct tracepoint_func *it_func_ptr;			\
// /*AFLA*/ 		void *it_func;						\
// /*AFLA*/ 		void *__data;						\
// /*AFLA*/ 									\
// /*AFLA*/ 		if (!(cond))						\
// /*AFLA*/ 			return;						\
// /*AFLA*/ 		prercu;							\
// /*AFLA*/ 		rcu_read_lock_sched_notrace();				\
// /*AFLA*/ 		it_func_ptr = rcu_dereference_sched((tp)->funcs);	\
// /*AFLA*/ 		if (it_func_ptr) {					\
// /*AFLA*/ 			do {						\
// /*AFLA*/ 				it_func = (it_func_ptr)->func;		\
// /*AFLA*/ 				__data = (it_func_ptr)->data;		\
// /*AFLA*/ 				((void(*)(proto))(it_func))(args);	\
// /*AFLA*/ 			} while ((++it_func_ptr)->func);		\
// /*AFLA*/ 		}							\
// /*AFLA*/ 		rcu_read_unlock_sched_notrace();			\
// /*AFLA*/ 		postrcu;						\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #ifndef MODULE
// /*AFLA*/ #define __DECLARE_TRACE_RCU(name, proto, args, cond, data_proto, data_args)	\
// /*AFLA*/ 	static inline void trace_##name##_rcuidle(proto)		\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		if (static_key_false(&__tracepoint_##name.key))		\
// /*AFLA*/ 			__DO_TRACE(&__tracepoint_##name,		\
// /*AFLA*/ 				TP_PROTO(data_proto),			\
// /*AFLA*/ 				TP_ARGS(data_args),			\
// /*AFLA*/ 				TP_CONDITION(cond),			\
// /*AFLA*/ 				rcu_irq_enter_irqson(),			\
// /*AFLA*/ 				rcu_irq_exit_irqson());			\
// /*AFLA*/ 	}
// /*AFLA*/ #else
// /*AFLA*/ #define __DECLARE_TRACE_RCU(name, proto, args, cond, data_proto, data_args)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Make sure the alignment of the structure in the __tracepoints section will
// /*AFLA*/  * not add unwanted padding between the beginning of the section and the
// /*AFLA*/  * structure. Force alignment to the same alignment as the section start.
// /*AFLA*/  *
// /*AFLA*/  * When lockdep is enabled, we make sure to always do the RCU portions of
// /*AFLA*/  * the tracepoint code, regardless of whether tracing is on. However,
// /*AFLA*/  * don't check if the condition is false, due to interaction with idle
// /*AFLA*/  * instrumentation. This lets us find RCU issues triggered with tracepoints
// /*AFLA*/  * even when this tracepoint is off. This code has no purpose other than
// /*AFLA*/  * poking RCU a bit.
// /*AFLA*/  */
// /*AFLA*/ #define __DECLARE_TRACE(name, proto, args, cond, data_proto, data_args) \
// /*AFLA*/ 	extern struct tracepoint __tracepoint_##name;			\
// /*AFLA*/ 	static inline void trace_##name(proto)				\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		if (static_key_false(&__tracepoint_##name.key))		\
// /*AFLA*/ 			__DO_TRACE(&__tracepoint_##name,		\
// /*AFLA*/ 				TP_PROTO(data_proto),			\
// /*AFLA*/ 				TP_ARGS(data_args),			\
// /*AFLA*/ 				TP_CONDITION(cond),,);			\
// /*AFLA*/ 		if (IS_ENABLED(CONFIG_LOCKDEP) && (cond)) {		\
// /*AFLA*/ 			rcu_read_lock_sched_notrace();			\
// /*AFLA*/ 			rcu_dereference_sched(__tracepoint_##name.funcs);\
// /*AFLA*/ 			rcu_read_unlock_sched_notrace();		\
// /*AFLA*/ 		}							\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	__DECLARE_TRACE_RCU(name, PARAMS(proto), PARAMS(args),		\
// /*AFLA*/ 		PARAMS(cond), PARAMS(data_proto), PARAMS(data_args))	\
// /*AFLA*/ 	static inline int						\
// /*AFLA*/ 	register_trace_##name(void (*probe)(data_proto), void *data)	\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		return tracepoint_probe_register(&__tracepoint_##name,	\
// /*AFLA*/ 						(void *)probe, data);	\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	static inline int						\
// /*AFLA*/ 	register_trace_prio_##name(void (*probe)(data_proto), void *data,\
// /*AFLA*/ 				   int prio)				\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		return tracepoint_probe_register_prio(&__tracepoint_##name, \
// /*AFLA*/ 					      (void *)probe, data, prio); \
// /*AFLA*/ 	}								\
// /*AFLA*/ 	static inline int						\
// /*AFLA*/ 	unregister_trace_##name(void (*probe)(data_proto), void *data)	\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		return tracepoint_probe_unregister(&__tracepoint_##name,\
// /*AFLA*/ 						(void *)probe, data);	\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	static inline void						\
// /*AFLA*/ 	check_trace_callback_type_##name(void (*cb)(data_proto))	\
// /*AFLA*/ 	{								\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	static inline bool						\
// /*AFLA*/ 	trace_##name##_enabled(void)					\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		return static_key_false(&__tracepoint_##name.key);	\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We have no guarantee that gcc and the linker won't up-align the tracepoint
// /*AFLA*/  * structures, so we create an array of pointers that will be used for iteration
// /*AFLA*/  * on the tracepoints.
// /*AFLA*/  */
// /*AFLA*/ #define DEFINE_TRACE_FN(name, reg, unreg)				 \
// /*AFLA*/ 	static const char __tpstrtab_##name[]				 \
// /*AFLA*/ 	__attribute__((section("__tracepoints_strings"))) = #name;	 \
// /*AFLA*/ 	struct tracepoint __tracepoint_##name				 \
// /*AFLA*/ 	__attribute__((section("__tracepoints"))) =			 \
// /*AFLA*/ 		{ __tpstrtab_##name, STATIC_KEY_INIT_FALSE, reg, unreg, NULL };\
// /*AFLA*/ 	static struct tracepoint * const __tracepoint_ptr_##name __used	 \
// /*AFLA*/ 	__attribute__((section("__tracepoints_ptrs"))) =		 \
// /*AFLA*/ 		&__tracepoint_##name;
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_TRACE(name)						\
// /*AFLA*/ 	DEFINE_TRACE_FN(name, NULL, NULL);
// /*AFLA*/ 
// /*AFLA*/ #define EXPORT_TRACEPOINT_SYMBOL_GPL(name)				\
// /*AFLA*/ 	EXPORT_SYMBOL_GPL(__tracepoint_##name)
// /*AFLA*/ #define EXPORT_TRACEPOINT_SYMBOL(name)					\
// /*AFLA*/ 	EXPORT_SYMBOL(__tracepoint_##name)
// /*AFLA*/ 
// /*AFLA*/ #else /* !TRACEPOINTS_ENABLED */
// /*AFLA*/ #define __DECLARE_TRACE(name, proto, args, cond, data_proto, data_args) \
// /*AFLA*/ 	static inline void trace_##name(proto)				\
// /*AFLA*/ 	{ }								\
// /*AFLA*/ 	static inline void trace_##name##_rcuidle(proto)		\
// /*AFLA*/ 	{ }								\
// /*AFLA*/ 	static inline int						\
// /*AFLA*/ 	register_trace_##name(void (*probe)(data_proto),		\
// /*AFLA*/ 			      void *data)				\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		return -ENOSYS;						\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	static inline int						\
// /*AFLA*/ 	unregister_trace_##name(void (*probe)(data_proto),		\
// /*AFLA*/ 				void *data)				\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		return -ENOSYS;						\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	static inline void check_trace_callback_type_##name(void (*cb)(data_proto)) \
// /*AFLA*/ 	{								\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	static inline bool						\
// /*AFLA*/ 	trace_##name##_enabled(void)					\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		return false;						\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_TRACE_FN(name, reg, unreg)
// /*AFLA*/ #define DEFINE_TRACE(name)
// /*AFLA*/ #define EXPORT_TRACEPOINT_SYMBOL_GPL(name)
// /*AFLA*/ #define EXPORT_TRACEPOINT_SYMBOL(name)
// /*AFLA*/ 
// /*AFLA*/ #endif /* TRACEPOINTS_ENABLED */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TRACING
// /*AFLA*/ /**
// /*AFLA*/  * tracepoint_string - register constant persistent string to trace system
// /*AFLA*/  * @str - a constant persistent string that will be referenced in tracepoints
// /*AFLA*/  *
// /*AFLA*/  * If constant strings are being used in tracepoints, it is faster and
// /*AFLA*/  * more efficient to just save the pointer to the string and reference
// /*AFLA*/  * that with a printf "%s" instead of saving the string in the ring buffer
// /*AFLA*/  * and wasting space and time.
// /*AFLA*/  *
// /*AFLA*/  * The problem with the above approach is that userspace tools that read
// /*AFLA*/  * the binary output of the trace buffers do not have access to the string.
// /*AFLA*/  * Instead they just show the address of the string which is not very
// /*AFLA*/  * useful to users.
// /*AFLA*/  *
// /*AFLA*/  * With tracepoint_string(), the string will be registered to the tracing
// /*AFLA*/  * system and exported to userspace via the debugfs/tracing/printk_formats
// /*AFLA*/  * file that maps the string address to the string text. This way userspace
// /*AFLA*/  * tools that read the binary buffers have a way to map the pointers to
// /*AFLA*/  * the ASCII strings they represent.
// /*AFLA*/  *
// /*AFLA*/  * The @str used must be a constant string and persistent as it would not
// /*AFLA*/  * make sense to show a string that no longer exists. But it is still fine
// /*AFLA*/  * to be used with modules, because when modules are unloaded, if they
// /*AFLA*/  * had tracepoints, the ring buffers are cleared too. As long as the string
// /*AFLA*/  * does not change during the life of the module, it is fine to use
// /*AFLA*/  * tracepoint_string() within a module.
// /*AFLA*/  */
// /*AFLA*/ #define tracepoint_string(str)						\
// /*AFLA*/ 	({								\
// /*AFLA*/ 		static const char *___tp_str __tracepoint_string = str; \
// /*AFLA*/ 		___tp_str;						\
// /*AFLA*/ 	})
// /*AFLA*/ #define __tracepoint_string	__attribute__((section("__tracepoint_str")))
// /*AFLA*/ #else
// /*AFLA*/ /*
// /*AFLA*/  * tracepoint_string() is used to save the string address for userspace
// /*AFLA*/  * tracing tools. When tracing isn't configured, there's no need to save
// /*AFLA*/  * anything.
// /*AFLA*/  */
// /*AFLA*/ # define tracepoint_string(str) str
// /*AFLA*/ # define __tracepoint_string
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The need for the DECLARE_TRACE_NOARGS() is to handle the prototype
// /*AFLA*/  * (void). "void" is a special value in a function prototype and can
// /*AFLA*/  * not be combined with other arguments. Since the DECLARE_TRACE()
// /*AFLA*/  * macro adds a data element at the beginning of the prototype,
// /*AFLA*/  * we need a way to differentiate "(void *data, proto)" from
// /*AFLA*/  * "(void *data, void)". The second prototype is invalid.
// /*AFLA*/  *
// /*AFLA*/  * DECLARE_TRACE_NOARGS() passes "void" as the tracepoint prototype
// /*AFLA*/  * and "void *__data" as the callback prototype.
// /*AFLA*/  *
// /*AFLA*/  * DECLARE_TRACE() passes "proto" as the tracepoint protoype and
// /*AFLA*/  * "void *__data, proto" as the callback prototype.
// /*AFLA*/  */
// /*AFLA*/ #define DECLARE_TRACE_NOARGS(name)					\
// /*AFLA*/ 	__DECLARE_TRACE(name, void, ,					\
// /*AFLA*/ 			cpu_online(raw_smp_processor_id()),		\
// /*AFLA*/ 			void *__data, __data)
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_TRACE(name, proto, args)				\
// /*AFLA*/ 	__DECLARE_TRACE(name, PARAMS(proto), PARAMS(args),		\
// /*AFLA*/ 			cpu_online(raw_smp_processor_id()),		\
// /*AFLA*/ 			PARAMS(void *__data, proto),			\
// /*AFLA*/ 			PARAMS(__data, args))
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_TRACE_CONDITION(name, proto, args, cond)		\
// /*AFLA*/ 	__DECLARE_TRACE(name, PARAMS(proto), PARAMS(args),		\
// /*AFLA*/ 			cpu_online(raw_smp_processor_id()) && (PARAMS(cond)), \
// /*AFLA*/ 			PARAMS(void *__data, proto),			\
// /*AFLA*/ 			PARAMS(__data, args))
// /*AFLA*/ 
// /*AFLA*/ #define TRACE_EVENT_FLAGS(event, flag)
// /*AFLA*/ 
// /*AFLA*/ #define TRACE_EVENT_PERF_PERM(event, expr...)
// /*AFLA*/ 
// /*AFLA*/ #endif /* DECLARE_TRACE */
// /*AFLA*/ 
// /*AFLA*/ #ifndef TRACE_EVENT
// /*AFLA*/ /*
// /*AFLA*/  * For use with the TRACE_EVENT macro:
// /*AFLA*/  *
// /*AFLA*/  * We define a tracepoint, its arguments, its printk format
// /*AFLA*/  * and its 'fast binary record' layout.
// /*AFLA*/  *
// /*AFLA*/  * Firstly, name your tracepoint via TRACE_EVENT(name : the
// /*AFLA*/  * 'subsystem_event' notation is fine.
// /*AFLA*/  *
// /*AFLA*/  * Think about this whole construct as the
// /*AFLA*/  * 'trace_sched_switch() function' from now on.
// /*AFLA*/  *
// /*AFLA*/  *
// /*AFLA*/  *  TRACE_EVENT(sched_switch,
// /*AFLA*/  *
// /*AFLA*/  *	*
// /*AFLA*/  *	* A function has a regular function arguments
// /*AFLA*/  *	* prototype, declare it via TP_PROTO():
// /*AFLA*/  *	*
// /*AFLA*/  *
// /*AFLA*/  *	TP_PROTO(struct rq *rq, struct task_struct *prev,
// /*AFLA*/  *		 struct task_struct *next),
// /*AFLA*/  *
// /*AFLA*/  *	*
// /*AFLA*/  *	* Define the call signature of the 'function'.
// /*AFLA*/  *	* (Design sidenote: we use this instead of a
// /*AFLA*/  *	*  TP_PROTO1/TP_PROTO2/TP_PROTO3 ugliness.)
// /*AFLA*/  *	*
// /*AFLA*/  *
// /*AFLA*/  *	TP_ARGS(rq, prev, next),
// /*AFLA*/  *
// /*AFLA*/  *	*
// /*AFLA*/  *	* Fast binary tracing: define the trace record via
// /*AFLA*/  *	* TP_STRUCT__entry(). You can think about it like a
// /*AFLA*/  *	* regular C structure local variable definition.
// /*AFLA*/  *	*
// /*AFLA*/  *	* This is how the trace record is structured and will
// /*AFLA*/  *	* be saved into the ring buffer. These are the fields
// /*AFLA*/  *	* that will be exposed to user-space in
// /*AFLA*/  *	* /sys/kernel/debug/tracing/events/<*>/format.
// /*AFLA*/  *	*
// /*AFLA*/  *	* The declared 'local variable' is called '__entry'
// /*AFLA*/  *	*
// /*AFLA*/  *	* __field(pid_t, prev_prid) is equivalent to a standard declariton:
// /*AFLA*/  *	*
// /*AFLA*/  *	*	pid_t	prev_pid;
// /*AFLA*/  *	*
// /*AFLA*/  *	* __array(char, prev_comm, TASK_COMM_LEN) is equivalent to:
// /*AFLA*/  *	*
// /*AFLA*/  *	*	char	prev_comm[TASK_COMM_LEN];
// /*AFLA*/  *	*
// /*AFLA*/  *
// /*AFLA*/  *	TP_STRUCT__entry(
// /*AFLA*/  *		__array(	char,	prev_comm,	TASK_COMM_LEN	)
// /*AFLA*/  *		__field(	pid_t,	prev_pid			)
// /*AFLA*/  *		__field(	int,	prev_prio			)
// /*AFLA*/  *		__array(	char,	next_comm,	TASK_COMM_LEN	)
// /*AFLA*/  *		__field(	pid_t,	next_pid			)
// /*AFLA*/  *		__field(	int,	next_prio			)
// /*AFLA*/  *	),
// /*AFLA*/  *
// /*AFLA*/  *	*
// /*AFLA*/  *	* Assign the entry into the trace record, by embedding
// /*AFLA*/  *	* a full C statement block into TP_fast_assign(). You
// /*AFLA*/  *	* can refer to the trace record as '__entry' -
// /*AFLA*/  *	* otherwise you can put arbitrary C code in here.
// /*AFLA*/  *	*
// /*AFLA*/  *	* Note: this C code will execute every time a trace event
// /*AFLA*/  *	* happens, on an active tracepoint.
// /*AFLA*/  *	*
// /*AFLA*/  *
// /*AFLA*/  *	TP_fast_assign(
// /*AFLA*/  *		memcpy(__entry->next_comm, next->comm, TASK_COMM_LEN);
// /*AFLA*/  *		__entry->prev_pid	= prev->pid;
// /*AFLA*/  *		__entry->prev_prio	= prev->prio;
// /*AFLA*/  *		memcpy(__entry->prev_comm, prev->comm, TASK_COMM_LEN);
// /*AFLA*/  *		__entry->next_pid	= next->pid;
// /*AFLA*/  *		__entry->next_prio	= next->prio;
// /*AFLA*/  *	),
// /*AFLA*/  *
// /*AFLA*/  *	*
// /*AFLA*/  *	* Formatted output of a trace record via TP_printk().
// /*AFLA*/  *	* This is how the tracepoint will appear under ftrace
// /*AFLA*/  *	* plugins that make use of this tracepoint.
// /*AFLA*/  *	*
// /*AFLA*/  *	* (raw-binary tracing wont actually perform this step.)
// /*AFLA*/  *	*
// /*AFLA*/  *
// /*AFLA*/  *	TP_printk("task %s:%d [%d] ==> %s:%d [%d]",
// /*AFLA*/  *		__entry->prev_comm, __entry->prev_pid, __entry->prev_prio,
// /*AFLA*/  *		__entry->next_comm, __entry->next_pid, __entry->next_prio),
// /*AFLA*/  *
// /*AFLA*/  * );
// /*AFLA*/  *
// /*AFLA*/  * This macro construct is thus used for the regular printk format
// /*AFLA*/  * tracing setup, it is used to construct a function pointer based
// /*AFLA*/  * tracepoint callback (this is used by programmatic plugins and
// /*AFLA*/  * can also by used by generic instrumentation like SystemTap), and
// /*AFLA*/  * it is also used to expose a structured trace record in
// /*AFLA*/  * /sys/kernel/debug/tracing/events/.
// /*AFLA*/  *
// /*AFLA*/  * A set of (un)registration functions can be passed to the variant
// /*AFLA*/  * TRACE_EVENT_FN to perform any (un)registration work.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_EVENT_CLASS(name, proto, args, tstruct, assign, print)
// /*AFLA*/ #define DEFINE_EVENT(template, name, proto, args)		\
// /*AFLA*/ 	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))
// /*AFLA*/ #define DEFINE_EVENT_FN(template, name, proto, args, reg, unreg)\
// /*AFLA*/ 	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))
// /*AFLA*/ #define DEFINE_EVENT_PRINT(template, name, proto, args, print)	\
// /*AFLA*/ 	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))
// /*AFLA*/ #define DEFINE_EVENT_CONDITION(template, name, proto,		\
// /*AFLA*/ 			       args, cond)			\
// /*AFLA*/ 	DECLARE_TRACE_CONDITION(name, PARAMS(proto),		\
// /*AFLA*/ 				PARAMS(args), PARAMS(cond))
// /*AFLA*/ 
// /*AFLA*/ #define TRACE_EVENT(name, proto, args, struct, assign, print)	\
// /*AFLA*/ 	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))
// /*AFLA*/ #define TRACE_EVENT_FN(name, proto, args, struct,		\
// /*AFLA*/ 		assign, print, reg, unreg)			\
// /*AFLA*/ 	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))
// /*AFLA*/ #define TRACE_EVENT_FN_COND(name, proto, args, cond, struct,		\
// /*AFLA*/ 		assign, print, reg, unreg)			\
// /*AFLA*/ 	DECLARE_TRACE_CONDITION(name, PARAMS(proto),	\
// /*AFLA*/ 			PARAMS(args), PARAMS(cond))
// /*AFLA*/ #define TRACE_EVENT_CONDITION(name, proto, args, cond,		\
// /*AFLA*/ 			      struct, assign, print)		\
// /*AFLA*/ 	DECLARE_TRACE_CONDITION(name, PARAMS(proto),		\
// /*AFLA*/ 				PARAMS(args), PARAMS(cond))
// /*AFLA*/ 
// /*AFLA*/ #define TRACE_EVENT_FLAGS(event, flag)
// /*AFLA*/ 
// /*AFLA*/ #define TRACE_EVENT_PERF_PERM(event, expr...)
// /*AFLA*/ 
// /*AFLA*/ #endif /* ifdef TRACE_EVENT (see note above) */
