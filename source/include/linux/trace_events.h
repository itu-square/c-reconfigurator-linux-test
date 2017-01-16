// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_TRACE_EVENT_H
// /*AFLA*/ #define _LINUX_TRACE_EVENT_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/ring_buffer.h>
// /*AFLA*/ #include <linux/trace_seq.h>
// /*AFLA*/ #include <linux/percpu.h>
// /*AFLA*/ #include <linux/hardirq.h>
// /*AFLA*/ #include <linux/perf_event.h>
// /*AFLA*/ #include <linux/tracepoint.h>
// /*AFLA*/ 
// /*AFLA*/ struct trace_array;
// /*AFLA*/ struct trace_buffer;
// /*AFLA*/ struct tracer;
// /*AFLA*/ struct dentry;
// /*AFLA*/ struct bpf_prog;
// /*AFLA*/ 
// /*AFLA*/ const char *trace_print_flags_seq(struct trace_seq *p, const char *delim,
// /*AFLA*/ 				  unsigned long flags,
// /*AFLA*/ 				  const struct trace_print_flags *flag_array);
// /*AFLA*/ 
// /*AFLA*/ const char *trace_print_symbols_seq(struct trace_seq *p, unsigned long val,
// /*AFLA*/ 				    const struct trace_print_flags *symbol_array);
// /*AFLA*/ 
// /*AFLA*/ #if BITS_PER_LONG == 32
// /*AFLA*/ const char *trace_print_symbols_seq_u64(struct trace_seq *p,
// /*AFLA*/ 					unsigned long long val,
// /*AFLA*/ 					const struct trace_print_flags_u64
// /*AFLA*/ 								 *symbol_array);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ const char *trace_print_bitmask_seq(struct trace_seq *p, void *bitmask_ptr,
// /*AFLA*/ 				    unsigned int bitmask_size);
// /*AFLA*/ 
// /*AFLA*/ const char *trace_print_hex_seq(struct trace_seq *p,
// /*AFLA*/ 				const unsigned char *buf, int len);
// /*AFLA*/ 
// /*AFLA*/ const char *trace_print_array_seq(struct trace_seq *p,
// /*AFLA*/ 				   const void *buf, int count,
// /*AFLA*/ 				   size_t el_size);
// /*AFLA*/ 
// /*AFLA*/ struct trace_iterator;
// /*AFLA*/ struct trace_event;
// /*AFLA*/ 
// /*AFLA*/ int trace_raw_output_prep(struct trace_iterator *iter,
// /*AFLA*/ 			  struct trace_event *event);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The trace entry - the most basic unit of tracing. This is what
// /*AFLA*/  * is printed in the end as a single line in the trace output, such as:
// /*AFLA*/  *
// /*AFLA*/  *     bash-15816 [01]   235.197585: idle_cpu <- irq_enter
// /*AFLA*/  */
// /*AFLA*/ struct trace_entry {
// /*AFLA*/ 	unsigned short		type;
// /*AFLA*/ 	unsigned char		flags;
// /*AFLA*/ 	unsigned char		preempt_count;
// /*AFLA*/ 	int			pid;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define TRACE_EVENT_TYPE_MAX						\
// /*AFLA*/ 	((1 << (sizeof(((struct trace_entry *)0)->type) * 8)) - 1)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Trace iterator - used by printout routines who present trace
// /*AFLA*/  * results to users and which routines might sleep, etc:
// /*AFLA*/  */
// /*AFLA*/ struct trace_iterator {
// /*AFLA*/ 	struct trace_array	*tr;
// /*AFLA*/ 	struct tracer		*trace;
// /*AFLA*/ 	struct trace_buffer	*trace_buffer;
// /*AFLA*/ 	void			*private;
// /*AFLA*/ 	int			cpu_file;
// /*AFLA*/ 	struct mutex		mutex;
// /*AFLA*/ 	struct ring_buffer_iter	**buffer_iter;
// /*AFLA*/ 	unsigned long		iter_flags;
// /*AFLA*/ 
// /*AFLA*/ 	/* trace_seq for __print_flags() and __print_symbolic() etc. */
// /*AFLA*/ 	struct trace_seq	tmp_seq;
// /*AFLA*/ 
// /*AFLA*/ 	cpumask_var_t		started;
// /*AFLA*/ 
// /*AFLA*/ 	/* it's true when current open file is snapshot */
// /*AFLA*/ 	bool			snapshot;
// /*AFLA*/ 
// /*AFLA*/ 	/* The below is zeroed out in pipe_read */
// /*AFLA*/ 	struct trace_seq	seq;
// /*AFLA*/ 	struct trace_entry	*ent;
// /*AFLA*/ 	unsigned long		lost_events;
// /*AFLA*/ 	int			leftover;
// /*AFLA*/ 	int			ent_size;
// /*AFLA*/ 	int			cpu;
// /*AFLA*/ 	u64			ts;
// /*AFLA*/ 
// /*AFLA*/ 	loff_t			pos;
// /*AFLA*/ 	long			idx;
// /*AFLA*/ 
// /*AFLA*/ 	/* All new field here will be zeroed out in pipe_read */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum trace_iter_flags {
// /*AFLA*/ 	TRACE_FILE_LAT_FMT	= 1,
// /*AFLA*/ 	TRACE_FILE_ANNOTATE	= 2,
// /*AFLA*/ 	TRACE_FILE_TIME_IN_NS	= 4,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ typedef enum print_line_t (*trace_print_func)(struct trace_iterator *iter,
// /*AFLA*/ 				      int flags, struct trace_event *event);
// /*AFLA*/ 
// /*AFLA*/ struct trace_event_functions {
// /*AFLA*/ 	trace_print_func	trace;
// /*AFLA*/ 	trace_print_func	raw;
// /*AFLA*/ 	trace_print_func	hex;
// /*AFLA*/ 	trace_print_func	binary;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct trace_event {
// /*AFLA*/ 	struct hlist_node		node;
// /*AFLA*/ 	struct list_head		list;
// /*AFLA*/ 	int				type;
// /*AFLA*/ 	struct trace_event_functions	*funcs;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern int register_trace_event(struct trace_event *event);
// /*AFLA*/ extern int unregister_trace_event(struct trace_event *event);
// /*AFLA*/ 
// /*AFLA*/ /* Return values for print_line callback */
// /*AFLA*/ enum print_line_t {
// /*AFLA*/ 	TRACE_TYPE_PARTIAL_LINE	= 0,	/* Retry after flushing the seq */
// /*AFLA*/ 	TRACE_TYPE_HANDLED	= 1,
// /*AFLA*/ 	TRACE_TYPE_UNHANDLED	= 2,	/* Relay to other output functions */
// /*AFLA*/ 	TRACE_TYPE_NO_CONSUME	= 3	/* Handled but ask to not consume */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Several functions return TRACE_TYPE_PARTIAL_LINE if the trace_seq
// /*AFLA*/  * overflowed, and TRACE_TYPE_HANDLED otherwise. This helper function
// /*AFLA*/  * simplifies those functions and keeps them in sync.
// /*AFLA*/  */
// /*AFLA*/ static inline enum print_line_t trace_handle_return(struct trace_seq *s)
// /*AFLA*/ {
// /*AFLA*/ 	return trace_seq_has_overflowed(s) ?
// /*AFLA*/ 		TRACE_TYPE_PARTIAL_LINE : TRACE_TYPE_HANDLED;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void tracing_generic_entry_update(struct trace_entry *entry,
// /*AFLA*/ 				  unsigned long flags,
// /*AFLA*/ 				  int pc);
// /*AFLA*/ struct trace_event_file;
// /*AFLA*/ 
// /*AFLA*/ struct ring_buffer_event *
// /*AFLA*/ trace_event_buffer_lock_reserve(struct ring_buffer **current_buffer,
// /*AFLA*/ 				struct trace_event_file *trace_file,
// /*AFLA*/ 				int type, unsigned long len,
// /*AFLA*/ 				unsigned long flags, int pc);
// /*AFLA*/ 
// /*AFLA*/ void tracing_record_cmdline(struct task_struct *tsk);
// /*AFLA*/ 
// /*AFLA*/ int trace_output_call(struct trace_iterator *iter, char *name, char *fmt, ...);
// /*AFLA*/ 
// /*AFLA*/ struct event_filter;
// /*AFLA*/ 
// /*AFLA*/ enum trace_reg {
// /*AFLA*/ 	TRACE_REG_REGISTER,
// /*AFLA*/ 	TRACE_REG_UNREGISTER,
// /*AFLA*/ #ifdef CONFIG_PERF_EVENTS
// /*AFLA*/ 	TRACE_REG_PERF_REGISTER,
// /*AFLA*/ 	TRACE_REG_PERF_UNREGISTER,
// /*AFLA*/ 	TRACE_REG_PERF_OPEN,
// /*AFLA*/ 	TRACE_REG_PERF_CLOSE,
// /*AFLA*/ 	TRACE_REG_PERF_ADD,
// /*AFLA*/ 	TRACE_REG_PERF_DEL,
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct trace_event_call;
// /*AFLA*/ 
// /*AFLA*/ struct trace_event_class {
// /*AFLA*/ 	const char		*system;
// /*AFLA*/ 	void			*probe;
// /*AFLA*/ #ifdef CONFIG_PERF_EVENTS
// /*AFLA*/ 	void			*perf_probe;
// /*AFLA*/ #endif
// /*AFLA*/ 	int			(*reg)(struct trace_event_call *event,
// /*AFLA*/ 				       enum trace_reg type, void *data);
// /*AFLA*/ 	int			(*define_fields)(struct trace_event_call *);
// /*AFLA*/ 	struct list_head	*(*get_fields)(struct trace_event_call *);
// /*AFLA*/ 	struct list_head	fields;
// /*AFLA*/ 	int			(*raw_init)(struct trace_event_call *);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern int trace_event_reg(struct trace_event_call *event,
// /*AFLA*/ 			    enum trace_reg type, void *data);
// /*AFLA*/ 
// /*AFLA*/ struct trace_event_buffer {
// /*AFLA*/ 	struct ring_buffer		*buffer;
// /*AFLA*/ 	struct ring_buffer_event	*event;
// /*AFLA*/ 	struct trace_event_file		*trace_file;
// /*AFLA*/ 	void				*entry;
// /*AFLA*/ 	unsigned long			flags;
// /*AFLA*/ 	int				pc;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ void *trace_event_buffer_reserve(struct trace_event_buffer *fbuffer,
// /*AFLA*/ 				  struct trace_event_file *trace_file,
// /*AFLA*/ 				  unsigned long len);
// /*AFLA*/ 
// /*AFLA*/ void trace_event_buffer_commit(struct trace_event_buffer *fbuffer);
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	TRACE_EVENT_FL_FILTERED_BIT,
// /*AFLA*/ 	TRACE_EVENT_FL_CAP_ANY_BIT,
// /*AFLA*/ 	TRACE_EVENT_FL_NO_SET_FILTER_BIT,
// /*AFLA*/ 	TRACE_EVENT_FL_IGNORE_ENABLE_BIT,
// /*AFLA*/ 	TRACE_EVENT_FL_WAS_ENABLED_BIT,
// /*AFLA*/ 	TRACE_EVENT_FL_TRACEPOINT_BIT,
// /*AFLA*/ 	TRACE_EVENT_FL_KPROBE_BIT,
// /*AFLA*/ 	TRACE_EVENT_FL_UPROBE_BIT,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Event flags:
// /*AFLA*/  *  FILTERED	  - The event has a filter attached
// /*AFLA*/  *  CAP_ANY	  - Any user can enable for perf
// /*AFLA*/  *  NO_SET_FILTER - Set when filter has error and is to be ignored
// /*AFLA*/  *  IGNORE_ENABLE - For trace internal events, do not enable with debugfs file
// /*AFLA*/  *  WAS_ENABLED   - Set and stays set when an event was ever enabled
// /*AFLA*/  *                    (used for module unloading, if a module event is enabled,
// /*AFLA*/  *                     it is best to clear the buffers that used it).
// /*AFLA*/  *  TRACEPOINT    - Event is a tracepoint
// /*AFLA*/  *  KPROBE        - Event is a kprobe
// /*AFLA*/  *  UPROBE        - Event is a uprobe
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	TRACE_EVENT_FL_FILTERED		= (1 << TRACE_EVENT_FL_FILTERED_BIT),
// /*AFLA*/ 	TRACE_EVENT_FL_CAP_ANY		= (1 << TRACE_EVENT_FL_CAP_ANY_BIT),
// /*AFLA*/ 	TRACE_EVENT_FL_NO_SET_FILTER	= (1 << TRACE_EVENT_FL_NO_SET_FILTER_BIT),
// /*AFLA*/ 	TRACE_EVENT_FL_IGNORE_ENABLE	= (1 << TRACE_EVENT_FL_IGNORE_ENABLE_BIT),
// /*AFLA*/ 	TRACE_EVENT_FL_WAS_ENABLED	= (1 << TRACE_EVENT_FL_WAS_ENABLED_BIT),
// /*AFLA*/ 	TRACE_EVENT_FL_TRACEPOINT	= (1 << TRACE_EVENT_FL_TRACEPOINT_BIT),
// /*AFLA*/ 	TRACE_EVENT_FL_KPROBE		= (1 << TRACE_EVENT_FL_KPROBE_BIT),
// /*AFLA*/ 	TRACE_EVENT_FL_UPROBE		= (1 << TRACE_EVENT_FL_UPROBE_BIT),
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define TRACE_EVENT_FL_UKPROBE (TRACE_EVENT_FL_KPROBE | TRACE_EVENT_FL_UPROBE)
// /*AFLA*/ 
// /*AFLA*/ struct trace_event_call {
// /*AFLA*/ 	struct list_head	list;
// /*AFLA*/ 	struct trace_event_class *class;
// /*AFLA*/ 	union {
// /*AFLA*/ 		char			*name;
// /*AFLA*/ 		/* Set TRACE_EVENT_FL_TRACEPOINT flag when using "tp" */
// /*AFLA*/ 		struct tracepoint	*tp;
// /*AFLA*/ 	};
// /*AFLA*/ 	struct trace_event	event;
// /*AFLA*/ 	char			*print_fmt;
// /*AFLA*/ 	struct event_filter	*filter;
// /*AFLA*/ 	void			*mod;
// /*AFLA*/ 	void			*data;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 *   bit 0:		filter_active
// /*AFLA*/ 	 *   bit 1:		allow trace by non root (cap any)
// /*AFLA*/ 	 *   bit 2:		failed to apply filter
// /*AFLA*/ 	 *   bit 3:		trace internal event (do not enable)
// /*AFLA*/ 	 *   bit 4:		Event was enabled by module
// /*AFLA*/ 	 *   bit 5:		use call filter rather than file filter
// /*AFLA*/ 	 *   bit 6:		Event is a tracepoint
// /*AFLA*/ 	 */
// /*AFLA*/ 	int			flags; /* static flags of different events */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PERF_EVENTS
// /*AFLA*/ 	int				perf_refcount;
// /*AFLA*/ 	struct hlist_head __percpu	*perf_events;
// /*AFLA*/ 	struct bpf_prog			*prog;
// /*AFLA*/ 
// /*AFLA*/ 	int	(*perf_perm)(struct trace_event_call *,
// /*AFLA*/ 			     struct perf_event *);
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline const char *
// /*AFLA*/ trace_event_name(struct trace_event_call *call)
// /*AFLA*/ {
// /*AFLA*/ 	if (call->flags & TRACE_EVENT_FL_TRACEPOINT)
// /*AFLA*/ 		return call->tp ? call->tp->name : NULL;
// /*AFLA*/ 	else
// /*AFLA*/ 		return call->name;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct trace_array;
// /*AFLA*/ struct trace_subsystem_dir;
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	EVENT_FILE_FL_ENABLED_BIT,
// /*AFLA*/ 	EVENT_FILE_FL_RECORDED_CMD_BIT,
// /*AFLA*/ 	EVENT_FILE_FL_FILTERED_BIT,
// /*AFLA*/ 	EVENT_FILE_FL_NO_SET_FILTER_BIT,
// /*AFLA*/ 	EVENT_FILE_FL_SOFT_MODE_BIT,
// /*AFLA*/ 	EVENT_FILE_FL_SOFT_DISABLED_BIT,
// /*AFLA*/ 	EVENT_FILE_FL_TRIGGER_MODE_BIT,
// /*AFLA*/ 	EVENT_FILE_FL_TRIGGER_COND_BIT,
// /*AFLA*/ 	EVENT_FILE_FL_PID_FILTER_BIT,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Event file flags:
// /*AFLA*/  *  ENABLED	  - The event is enabled
// /*AFLA*/  *  RECORDED_CMD  - The comms should be recorded at sched_switch
// /*AFLA*/  *  FILTERED	  - The event has a filter attached
// /*AFLA*/  *  NO_SET_FILTER - Set when filter has error and is to be ignored
// /*AFLA*/  *  SOFT_MODE     - The event is enabled/disabled by SOFT_DISABLED
// /*AFLA*/  *  SOFT_DISABLED - When set, do not trace the event (even though its
// /*AFLA*/  *                   tracepoint may be enabled)
// /*AFLA*/  *  TRIGGER_MODE  - When set, invoke the triggers associated with the event
// /*AFLA*/  *  TRIGGER_COND  - When set, one or more triggers has an associated filter
// /*AFLA*/  *  PID_FILTER    - When set, the event is filtered based on pid
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	EVENT_FILE_FL_ENABLED		= (1 << EVENT_FILE_FL_ENABLED_BIT),
// /*AFLA*/ 	EVENT_FILE_FL_RECORDED_CMD	= (1 << EVENT_FILE_FL_RECORDED_CMD_BIT),
// /*AFLA*/ 	EVENT_FILE_FL_FILTERED		= (1 << EVENT_FILE_FL_FILTERED_BIT),
// /*AFLA*/ 	EVENT_FILE_FL_NO_SET_FILTER	= (1 << EVENT_FILE_FL_NO_SET_FILTER_BIT),
// /*AFLA*/ 	EVENT_FILE_FL_SOFT_MODE		= (1 << EVENT_FILE_FL_SOFT_MODE_BIT),
// /*AFLA*/ 	EVENT_FILE_FL_SOFT_DISABLED	= (1 << EVENT_FILE_FL_SOFT_DISABLED_BIT),
// /*AFLA*/ 	EVENT_FILE_FL_TRIGGER_MODE	= (1 << EVENT_FILE_FL_TRIGGER_MODE_BIT),
// /*AFLA*/ 	EVENT_FILE_FL_TRIGGER_COND	= (1 << EVENT_FILE_FL_TRIGGER_COND_BIT),
// /*AFLA*/ 	EVENT_FILE_FL_PID_FILTER	= (1 << EVENT_FILE_FL_PID_FILTER_BIT),
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct trace_event_file {
// /*AFLA*/ 	struct list_head		list;
// /*AFLA*/ 	struct trace_event_call		*event_call;
// /*AFLA*/ 	struct event_filter		*filter;
// /*AFLA*/ 	struct dentry			*dir;
// /*AFLA*/ 	struct trace_array		*tr;
// /*AFLA*/ 	struct trace_subsystem_dir	*system;
// /*AFLA*/ 	struct list_head		triggers;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * 32 bit flags:
// /*AFLA*/ 	 *   bit 0:		enabled
// /*AFLA*/ 	 *   bit 1:		enabled cmd record
// /*AFLA*/ 	 *   bit 2:		enable/disable with the soft disable bit
// /*AFLA*/ 	 *   bit 3:		soft disabled
// /*AFLA*/ 	 *   bit 4:		trigger enabled
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Note: The bits must be set atomically to prevent races
// /*AFLA*/ 	 * from other writers. Reads of flags do not need to be in
// /*AFLA*/ 	 * sync as they occur in critical sections. But the way flags
// /*AFLA*/ 	 * is currently used, these changes do not affect the code
// /*AFLA*/ 	 * except that when a change is made, it may have a slight
// /*AFLA*/ 	 * delay in propagating the changes to other CPUs due to
// /*AFLA*/ 	 * caching and such. Which is mostly OK ;-)
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long		flags;
// /*AFLA*/ 	atomic_t		sm_ref;	/* soft-mode reference counter */
// /*AFLA*/ 	atomic_t		tm_ref;	/* trigger-mode reference counter */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define __TRACE_EVENT_FLAGS(name, value)				\
// /*AFLA*/ 	static int __init trace_init_flags_##name(void)			\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		event_##name.flags |= value;				\
// /*AFLA*/ 		return 0;						\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	early_initcall(trace_init_flags_##name);
// /*AFLA*/ 
// /*AFLA*/ #define __TRACE_EVENT_PERF_PERM(name, expr...)				\
// /*AFLA*/ 	static int perf_perm_##name(struct trace_event_call *tp_event, \
// /*AFLA*/ 				    struct perf_event *p_event)		\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		return ({ expr; });					\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	static int __init trace_init_perf_perm_##name(void)		\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		event_##name.perf_perm = &perf_perm_##name;		\
// /*AFLA*/ 		return 0;						\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	early_initcall(trace_init_perf_perm_##name);
// /*AFLA*/ 
// /*AFLA*/ #define PERF_MAX_TRACE_SIZE	2048
// /*AFLA*/ 
// /*AFLA*/ #define MAX_FILTER_STR_VAL	256	/* Should handle KSYM_SYMBOL_LEN */
// /*AFLA*/ 
// /*AFLA*/ enum event_trigger_type {
// /*AFLA*/ 	ETT_NONE		= (0),
// /*AFLA*/ 	ETT_TRACE_ONOFF		= (1 << 0),
// /*AFLA*/ 	ETT_SNAPSHOT		= (1 << 1),
// /*AFLA*/ 	ETT_STACKTRACE		= (1 << 2),
// /*AFLA*/ 	ETT_EVENT_ENABLE	= (1 << 3),
// /*AFLA*/ 	ETT_EVENT_HIST		= (1 << 4),
// /*AFLA*/ 	ETT_HIST_ENABLE		= (1 << 5),
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern int filter_match_preds(struct event_filter *filter, void *rec);
// /*AFLA*/ 
// /*AFLA*/ extern enum event_trigger_type event_triggers_call(struct trace_event_file *file,
// /*AFLA*/ 						   void *rec);
// /*AFLA*/ extern void event_triggers_post_call(struct trace_event_file *file,
// /*AFLA*/ 				     enum event_trigger_type tt,
// /*AFLA*/ 				     void *rec);
// /*AFLA*/ 
// /*AFLA*/ bool trace_event_ignore_this_pid(struct trace_event_file *trace_file);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * trace_trigger_soft_disabled - do triggers and test if soft disabled
// /*AFLA*/  * @file: The file pointer of the event to test
// /*AFLA*/  *
// /*AFLA*/  * If any triggers without filters are attached to this event, they
// /*AFLA*/  * will be called here. If the event is soft disabled and has no
// /*AFLA*/  * triggers that require testing the fields, it will return true,
// /*AFLA*/  * otherwise false.
// /*AFLA*/  */
// /*AFLA*/ static inline bool
// /*AFLA*/ trace_trigger_soft_disabled(struct trace_event_file *file)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long eflags = file->flags;
// /*AFLA*/ 
// /*AFLA*/ 	if (!(eflags & EVENT_FILE_FL_TRIGGER_COND)) {
// /*AFLA*/ 		if (eflags & EVENT_FILE_FL_TRIGGER_MODE)
// /*AFLA*/ 			event_triggers_call(file, NULL);
// /*AFLA*/ 		if (eflags & EVENT_FILE_FL_SOFT_DISABLED)
// /*AFLA*/ 			return true;
// /*AFLA*/ 		if (eflags & EVENT_FILE_FL_PID_FILTER)
// /*AFLA*/ 			return trace_event_ignore_this_pid(file);
// /*AFLA*/ 	}
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_BPF_EVENTS
// /*AFLA*/ unsigned int trace_call_bpf(struct bpf_prog *prog, void *ctx);
// /*AFLA*/ #else
// /*AFLA*/ static inline unsigned int trace_call_bpf(struct bpf_prog *prog, void *ctx)
// /*AFLA*/ {
// /*AFLA*/ 	return 1;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	FILTER_OTHER = 0,
// /*AFLA*/ 	FILTER_STATIC_STRING,
// /*AFLA*/ 	FILTER_DYN_STRING,
// /*AFLA*/ 	FILTER_PTR_STRING,
// /*AFLA*/ 	FILTER_TRACE_FN,
// /*AFLA*/ 	FILTER_COMM,
// /*AFLA*/ 	FILTER_CPU,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern int trace_event_raw_init(struct trace_event_call *call);
// /*AFLA*/ extern int trace_define_field(struct trace_event_call *call, const char *type,
// /*AFLA*/ 			      const char *name, int offset, int size,
// /*AFLA*/ 			      int is_signed, int filter_type);
// /*AFLA*/ extern int trace_add_event_call(struct trace_event_call *call);
// /*AFLA*/ extern int trace_remove_event_call(struct trace_event_call *call);
// /*AFLA*/ extern int trace_event_get_offsets(struct trace_event_call *call);
// /*AFLA*/ 
// /*AFLA*/ #define is_signed_type(type)	(((type)(-1)) < (type)1)
// /*AFLA*/ 
// /*AFLA*/ int trace_set_clr_event(const char *system, const char *event, int set);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The double __builtin_constant_p is because gcc will give us an error
// /*AFLA*/  * if we try to allocate the static variable to fmt if it is not a
// /*AFLA*/  * constant. Even with the outer if statement optimizing out.
// /*AFLA*/  */
// /*AFLA*/ #define event_trace_printk(ip, fmt, args...)				\
// /*AFLA*/ do {									\
// /*AFLA*/ 	__trace_printk_check_format(fmt, ##args);			\
// /*AFLA*/ 	tracing_record_cmdline(current);				\
// /*AFLA*/ 	if (__builtin_constant_p(fmt)) {				\
// /*AFLA*/ 		static const char *trace_printk_fmt			\
// /*AFLA*/ 		  __attribute__((section("__trace_printk_fmt"))) =	\
// /*AFLA*/ 			__builtin_constant_p(fmt) ? fmt : NULL;		\
// /*AFLA*/ 									\
// /*AFLA*/ 		__trace_bprintk(ip, trace_printk_fmt, ##args);		\
// /*AFLA*/ 	} else								\
// /*AFLA*/ 		__trace_printk(ip, fmt, ##args);			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PERF_EVENTS
// /*AFLA*/ struct perf_event;
// /*AFLA*/ 
// /*AFLA*/ DECLARE_PER_CPU(struct pt_regs, perf_trace_regs);
// /*AFLA*/ 
// /*AFLA*/ extern int  perf_trace_init(struct perf_event *event);
// /*AFLA*/ extern void perf_trace_destroy(struct perf_event *event);
// /*AFLA*/ extern int  perf_trace_add(struct perf_event *event, int flags);
// /*AFLA*/ extern void perf_trace_del(struct perf_event *event, int flags);
// /*AFLA*/ extern int  ftrace_profile_set_filter(struct perf_event *event, int event_id,
// /*AFLA*/ 				     char *filter_str);
// /*AFLA*/ extern void ftrace_profile_free_filter(struct perf_event *event);
// /*AFLA*/ void perf_trace_buf_update(void *record, u16 type);
// /*AFLA*/ void *perf_trace_buf_alloc(int size, struct pt_regs **regs, int *rctxp);
// /*AFLA*/ 
// /*AFLA*/ void perf_trace_run_bpf_submit(void *raw_data, int size, int rctx,
// /*AFLA*/ 			       struct trace_event_call *call, u64 count,
// /*AFLA*/ 			       struct pt_regs *regs, struct hlist_head *head,
// /*AFLA*/ 			       struct task_struct *task);
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ perf_trace_buf_submit(void *raw_data, int size, int rctx, u16 type,
// /*AFLA*/ 		       u64 count, struct pt_regs *regs, void *head,
// /*AFLA*/ 		       struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	perf_tp_event(type, count, raw_data, size, regs, head, rctx, task);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_TRACE_EVENT_H */
