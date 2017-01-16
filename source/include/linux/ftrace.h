// /*AFLA*/ /*
// /*AFLA*/  * Ftrace header.  For implementation details beyond the random comments
// /*AFLA*/  * scattered below, see: Documentation/trace/ftrace-design.txt
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_FTRACE_H
// /*AFLA*/ #define _LINUX_FTRACE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/trace_clock.h>
// /*AFLA*/ #include <linux/kallsyms.h>
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ #include <linux/bitops.h>
// /*AFLA*/ #include <linux/ptrace.h>
// /*AFLA*/ #include <linux/ktime.h>
// /*AFLA*/ #include <linux/sched.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/init.h>
// /*AFLA*/ #include <linux/fs.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/ftrace.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * If the arch supports passing the variable contents of
// /*AFLA*/  * function_trace_op as the third parameter back from the
// /*AFLA*/  * mcount call, then the arch should define this as 1.
// /*AFLA*/  */
// /*AFLA*/ #ifndef ARCH_SUPPORTS_FTRACE_OPS
// /*AFLA*/ #define ARCH_SUPPORTS_FTRACE_OPS 0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * If the arch's mcount caller does not support all of ftrace's
// /*AFLA*/  * features, then it must call an indirect function that
// /*AFLA*/  * does. Or at least does enough to prevent any unwelcomed side effects.
// /*AFLA*/  */
// /*AFLA*/ #if !ARCH_SUPPORTS_FTRACE_OPS
// /*AFLA*/ # define FTRACE_FORCE_LIST_FUNC 1
// /*AFLA*/ #else
// /*AFLA*/ # define FTRACE_FORCE_LIST_FUNC 0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Main tracing buffer and events set up */
// /*AFLA*/ #ifdef CONFIG_TRACING
// /*AFLA*/ void trace_init(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void trace_init(void) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct module;
// /*AFLA*/ struct ftrace_hash;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FUNCTION_TRACER
// /*AFLA*/ 
// /*AFLA*/ extern int ftrace_enabled;
// /*AFLA*/ extern int
// /*AFLA*/ ftrace_enable_sysctl(struct ctl_table *table, int write,
// /*AFLA*/ 		     void __user *buffer, size_t *lenp,
// /*AFLA*/ 		     loff_t *ppos);
// /*AFLA*/ 
// /*AFLA*/ struct ftrace_ops;
// /*AFLA*/ 
// /*AFLA*/ typedef void (*ftrace_func_t)(unsigned long ip, unsigned long parent_ip,
// /*AFLA*/ 			      struct ftrace_ops *op, struct pt_regs *regs);
// /*AFLA*/ 
// /*AFLA*/ ftrace_func_t ftrace_ops_get_func(struct ftrace_ops *ops);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * FTRACE_OPS_FL_* bits denote the state of ftrace_ops struct and are
// /*AFLA*/  * set in the flags member.
// /*AFLA*/  * CONTROL, SAVE_REGS, SAVE_REGS_IF_SUPPORTED, RECURSION_SAFE, STUB and
// /*AFLA*/  * IPMODIFY are a kind of attribute flags which can be set only before
// /*AFLA*/  * registering the ftrace_ops, and can not be modified while registered.
// /*AFLA*/  * Changing those attribute flags after regsitering ftrace_ops will
// /*AFLA*/  * cause unexpected results.
// /*AFLA*/  *
// /*AFLA*/  * ENABLED - set/unset when ftrace_ops is registered/unregistered
// /*AFLA*/  * DYNAMIC - set when ftrace_ops is registered to denote dynamically
// /*AFLA*/  *           allocated ftrace_ops which need special care
// /*AFLA*/  * PER_CPU - set manualy by ftrace_ops user to denote the ftrace_ops
// /*AFLA*/  *           could be controlled by following calls:
// /*AFLA*/  *             ftrace_function_local_enable
// /*AFLA*/  *             ftrace_function_local_disable
// /*AFLA*/  * SAVE_REGS - The ftrace_ops wants regs saved at each function called
// /*AFLA*/  *            and passed to the callback. If this flag is set, but the
// /*AFLA*/  *            architecture does not support passing regs
// /*AFLA*/  *            (CONFIG_DYNAMIC_FTRACE_WITH_REGS is not defined), then the
// /*AFLA*/  *            ftrace_ops will fail to register, unless the next flag
// /*AFLA*/  *            is set.
// /*AFLA*/  * SAVE_REGS_IF_SUPPORTED - This is the same as SAVE_REGS, but if the
// /*AFLA*/  *            handler can handle an arch that does not save regs
// /*AFLA*/  *            (the handler tests if regs == NULL), then it can set
// /*AFLA*/  *            this flag instead. It will not fail registering the ftrace_ops
// /*AFLA*/  *            but, the regs field will be NULL if the arch does not support
// /*AFLA*/  *            passing regs to the handler.
// /*AFLA*/  *            Note, if this flag is set, the SAVE_REGS flag will automatically
// /*AFLA*/  *            get set upon registering the ftrace_ops, if the arch supports it.
// /*AFLA*/  * RECURSION_SAFE - The ftrace_ops can set this to tell the ftrace infrastructure
// /*AFLA*/  *            that the call back has its own recursion protection. If it does
// /*AFLA*/  *            not set this, then the ftrace infrastructure will add recursion
// /*AFLA*/  *            protection for the caller.
// /*AFLA*/  * STUB   - The ftrace_ops is just a place holder.
// /*AFLA*/  * INITIALIZED - The ftrace_ops has already been initialized (first use time
// /*AFLA*/  *            register_ftrace_function() is called, it will initialized the ops)
// /*AFLA*/  * DELETED - The ops are being deleted, do not let them be registered again.
// /*AFLA*/  * ADDING  - The ops is in the process of being added.
// /*AFLA*/  * REMOVING - The ops is in the process of being removed.
// /*AFLA*/  * MODIFYING - The ops is in the process of changing its filter functions.
// /*AFLA*/  * ALLOC_TRAMP - A dynamic trampoline was allocated by the core code.
// /*AFLA*/  *            The arch specific code sets this flag when it allocated a
// /*AFLA*/  *            trampoline. This lets the arch know that it can update the
// /*AFLA*/  *            trampoline in case the callback function changes.
// /*AFLA*/  *            The ftrace_ops trampoline can be set by the ftrace users, and
// /*AFLA*/  *            in such cases the arch must not modify it. Only the arch ftrace
// /*AFLA*/  *            core code should set this flag.
// /*AFLA*/  * IPMODIFY - The ops can modify the IP register. This can only be set with
// /*AFLA*/  *            SAVE_REGS. If another ops with this flag set is already registered
// /*AFLA*/  *            for any of the functions that this ops will be registered for, then
// /*AFLA*/  *            this ops will fail to register or set_filter_ip.
// /*AFLA*/  * PID     - Is affected by set_ftrace_pid (allows filtering on those pids)
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	FTRACE_OPS_FL_ENABLED			= 1 << 0,
// /*AFLA*/ 	FTRACE_OPS_FL_DYNAMIC			= 1 << 1,
// /*AFLA*/ 	FTRACE_OPS_FL_PER_CPU			= 1 << 2,
// /*AFLA*/ 	FTRACE_OPS_FL_SAVE_REGS			= 1 << 3,
// /*AFLA*/ 	FTRACE_OPS_FL_SAVE_REGS_IF_SUPPORTED	= 1 << 4,
// /*AFLA*/ 	FTRACE_OPS_FL_RECURSION_SAFE		= 1 << 5,
// /*AFLA*/ 	FTRACE_OPS_FL_STUB			= 1 << 6,
// /*AFLA*/ 	FTRACE_OPS_FL_INITIALIZED		= 1 << 7,
// /*AFLA*/ 	FTRACE_OPS_FL_DELETED			= 1 << 8,
// /*AFLA*/ 	FTRACE_OPS_FL_ADDING			= 1 << 9,
// /*AFLA*/ 	FTRACE_OPS_FL_REMOVING			= 1 << 10,
// /*AFLA*/ 	FTRACE_OPS_FL_MODIFYING			= 1 << 11,
// /*AFLA*/ 	FTRACE_OPS_FL_ALLOC_TRAMP		= 1 << 12,
// /*AFLA*/ 	FTRACE_OPS_FL_IPMODIFY			= 1 << 13,
// /*AFLA*/ 	FTRACE_OPS_FL_PID			= 1 << 14,
// /*AFLA*/ 	FTRACE_OPS_FL_RCU			= 1 << 15,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DYNAMIC_FTRACE
// /*AFLA*/ /* The hash used to know what functions callbacks trace */
// /*AFLA*/ struct ftrace_ops_hash {
// /*AFLA*/ 	struct ftrace_hash		*notrace_hash;
// /*AFLA*/ 	struct ftrace_hash		*filter_hash;
// /*AFLA*/ 	struct mutex			regex_lock;
// /*AFLA*/ };
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Note, ftrace_ops can be referenced outside of RCU protection, unless
// /*AFLA*/  * the RCU flag is set. If ftrace_ops is allocated and not part of kernel
// /*AFLA*/  * core data, the unregistering of it will perform a scheduling on all CPUs
// /*AFLA*/  * to make sure that there are no more users. Depending on the load of the
// /*AFLA*/  * system that may take a bit of time.
// /*AFLA*/  *
// /*AFLA*/  * Any private data added must also take care not to be freed and if private
// /*AFLA*/  * data is added to a ftrace_ops that is in core code, the user of the
// /*AFLA*/  * ftrace_ops must perform a schedule_on_each_cpu() before freeing it.
// /*AFLA*/  */
// /*AFLA*/ struct ftrace_ops {
// /*AFLA*/ 	ftrace_func_t			func;
// /*AFLA*/ 	struct ftrace_ops		*next;
// /*AFLA*/ 	unsigned long			flags;
// /*AFLA*/ 	void				*private;
// /*AFLA*/ 	ftrace_func_t			saved_func;
// /*AFLA*/ 	int __percpu			*disabled;
// /*AFLA*/ #ifdef CONFIG_DYNAMIC_FTRACE
// /*AFLA*/ 	struct ftrace_ops_hash		local_hash;
// /*AFLA*/ 	struct ftrace_ops_hash		*func_hash;
// /*AFLA*/ 	struct ftrace_ops_hash		old_hash;
// /*AFLA*/ 	unsigned long			trampoline;
// /*AFLA*/ 	unsigned long			trampoline_size;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Type of the current tracing.
// /*AFLA*/  */
// /*AFLA*/ enum ftrace_tracing_type_t {
// /*AFLA*/ 	FTRACE_TYPE_ENTER = 0, /* Hook the call of the function */
// /*AFLA*/ 	FTRACE_TYPE_RETURN,	/* Hook the return of the function */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Current tracing type, default is FTRACE_TYPE_ENTER */
// /*AFLA*/ extern enum ftrace_tracing_type_t ftrace_tracing_type;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The ftrace_ops must be a static and should also
// /*AFLA*/  * be read_mostly.  These functions do modify read_mostly variables
// /*AFLA*/  * so use them sparely. Never free an ftrace_op or modify the
// /*AFLA*/  * next pointer after it has been registered. Even after unregistering
// /*AFLA*/  * it, the next pointer may still be used internally.
// /*AFLA*/  */
// /*AFLA*/ int register_ftrace_function(struct ftrace_ops *ops);
// /*AFLA*/ int unregister_ftrace_function(struct ftrace_ops *ops);
// /*AFLA*/ void clear_ftrace_function(void);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ftrace_function_local_enable - enable ftrace_ops on current cpu
// /*AFLA*/  *
// /*AFLA*/  * This function enables tracing on current cpu by decreasing
// /*AFLA*/  * the per cpu control variable.
// /*AFLA*/  * It must be called with preemption disabled and only on ftrace_ops
// /*AFLA*/  * registered with FTRACE_OPS_FL_PER_CPU. If called without preemption
// /*AFLA*/  * disabled, this_cpu_ptr will complain when CONFIG_DEBUG_PREEMPT is enabled.
// /*AFLA*/  */
// /*AFLA*/ static inline void ftrace_function_local_enable(struct ftrace_ops *ops)
// /*AFLA*/ {
// /*AFLA*/ 	if (WARN_ON_ONCE(!(ops->flags & FTRACE_OPS_FL_PER_CPU)))
// /*AFLA*/ 		return;
// /*AFLA*/ 
// /*AFLA*/ 	(*this_cpu_ptr(ops->disabled))--;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ftrace_function_local_disable - disable ftrace_ops on current cpu
// /*AFLA*/  *
// /*AFLA*/  * This function disables tracing on current cpu by increasing
// /*AFLA*/  * the per cpu control variable.
// /*AFLA*/  * It must be called with preemption disabled and only on ftrace_ops
// /*AFLA*/  * registered with FTRACE_OPS_FL_PER_CPU. If called without preemption
// /*AFLA*/  * disabled, this_cpu_ptr will complain when CONFIG_DEBUG_PREEMPT is enabled.
// /*AFLA*/  */
// /*AFLA*/ static inline void ftrace_function_local_disable(struct ftrace_ops *ops)
// /*AFLA*/ {
// /*AFLA*/ 	if (WARN_ON_ONCE(!(ops->flags & FTRACE_OPS_FL_PER_CPU)))
// /*AFLA*/ 		return;
// /*AFLA*/ 
// /*AFLA*/ 	(*this_cpu_ptr(ops->disabled))++;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ftrace_function_local_disabled - returns ftrace_ops disabled value
// /*AFLA*/  *                                  on current cpu
// /*AFLA*/  *
// /*AFLA*/  * This function returns value of ftrace_ops::disabled on current cpu.
// /*AFLA*/  * It must be called with preemption disabled and only on ftrace_ops
// /*AFLA*/  * registered with FTRACE_OPS_FL_PER_CPU. If called without preemption
// /*AFLA*/  * disabled, this_cpu_ptr will complain when CONFIG_DEBUG_PREEMPT is enabled.
// /*AFLA*/  */
// /*AFLA*/ static inline int ftrace_function_local_disabled(struct ftrace_ops *ops)
// /*AFLA*/ {
// /*AFLA*/ 	WARN_ON_ONCE(!(ops->flags & FTRACE_OPS_FL_PER_CPU));
// /*AFLA*/ 	return *this_cpu_ptr(ops->disabled);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void ftrace_stub(unsigned long a0, unsigned long a1,
// /*AFLA*/ 			struct ftrace_ops *op, struct pt_regs *regs);
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_FUNCTION_TRACER */
// /*AFLA*/ /*
// /*AFLA*/  * (un)register_ftrace_function must be a macro since the ops parameter
// /*AFLA*/  * must not be evaluated.
// /*AFLA*/  */
// /*AFLA*/ #define register_ftrace_function(ops) ({ 0; })
// /*AFLA*/ #define unregister_ftrace_function(ops) ({ 0; })
// /*AFLA*/ static inline int ftrace_nr_registered_ops(void)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void clear_ftrace_function(void) { }
// /*AFLA*/ static inline void ftrace_kill(void) { }
// /*AFLA*/ #endif /* CONFIG_FUNCTION_TRACER */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_STACK_TRACER
// /*AFLA*/ 
// /*AFLA*/ #define STACK_TRACE_ENTRIES 500
// /*AFLA*/ 
// /*AFLA*/ struct stack_trace;
// /*AFLA*/ 
// /*AFLA*/ extern unsigned stack_trace_index[];
// /*AFLA*/ extern struct stack_trace stack_trace_max;
// /*AFLA*/ extern unsigned long stack_trace_max_size;
// /*AFLA*/ extern arch_spinlock_t stack_trace_max_lock;
// /*AFLA*/ 
// /*AFLA*/ extern int stack_tracer_enabled;
// /*AFLA*/ void stack_trace_print(void);
// /*AFLA*/ int
// /*AFLA*/ stack_trace_sysctl(struct ctl_table *table, int write,
// /*AFLA*/ 		   void __user *buffer, size_t *lenp,
// /*AFLA*/ 		   loff_t *ppos);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct ftrace_func_command {
// /*AFLA*/ 	struct list_head	list;
// /*AFLA*/ 	char			*name;
// /*AFLA*/ 	int			(*func)(struct ftrace_hash *hash,
// /*AFLA*/ 					char *func, char *cmd,
// /*AFLA*/ 					char *params, int enable);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DYNAMIC_FTRACE
// /*AFLA*/ 
// /*AFLA*/ int ftrace_arch_code_modify_prepare(void);
// /*AFLA*/ int ftrace_arch_code_modify_post_process(void);
// /*AFLA*/ 
// /*AFLA*/ struct dyn_ftrace;
// /*AFLA*/ 
// /*AFLA*/ enum ftrace_bug_type {
// /*AFLA*/ 	FTRACE_BUG_UNKNOWN,
// /*AFLA*/ 	FTRACE_BUG_INIT,
// /*AFLA*/ 	FTRACE_BUG_NOP,
// /*AFLA*/ 	FTRACE_BUG_CALL,
// /*AFLA*/ 	FTRACE_BUG_UPDATE,
// /*AFLA*/ };
// /*AFLA*/ extern enum ftrace_bug_type ftrace_bug_type;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Archs can set this to point to a variable that holds the value that was
// /*AFLA*/  * expected at the call site before calling ftrace_bug().
// /*AFLA*/  */
// /*AFLA*/ extern const void *ftrace_expected;
// /*AFLA*/ 
// /*AFLA*/ void ftrace_bug(int err, struct dyn_ftrace *rec);
// /*AFLA*/ 
// /*AFLA*/ struct seq_file;
// /*AFLA*/ 
// /*AFLA*/ struct ftrace_probe_ops {
// /*AFLA*/ 	void			(*func)(unsigned long ip,
// /*AFLA*/ 					unsigned long parent_ip,
// /*AFLA*/ 					void **data);
// /*AFLA*/ 	int			(*init)(struct ftrace_probe_ops *ops,
// /*AFLA*/ 					unsigned long ip, void **data);
// /*AFLA*/ 	void			(*free)(struct ftrace_probe_ops *ops,
// /*AFLA*/ 					unsigned long ip, void **data);
// /*AFLA*/ 	int			(*print)(struct seq_file *m,
// /*AFLA*/ 					 unsigned long ip,
// /*AFLA*/ 					 struct ftrace_probe_ops *ops,
// /*AFLA*/ 					 void *data);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern int
// /*AFLA*/ register_ftrace_function_probe(char *glob, struct ftrace_probe_ops *ops,
// /*AFLA*/ 			      void *data);
// /*AFLA*/ extern void
// /*AFLA*/ unregister_ftrace_function_probe(char *glob, struct ftrace_probe_ops *ops,
// /*AFLA*/ 				void *data);
// /*AFLA*/ extern void
// /*AFLA*/ unregister_ftrace_function_probe_func(char *glob, struct ftrace_probe_ops *ops);
// /*AFLA*/ extern void unregister_ftrace_function_probe_all(char *glob);
// /*AFLA*/ 
// /*AFLA*/ extern int ftrace_text_reserved(const void *start, const void *end);
// /*AFLA*/ 
// /*AFLA*/ extern int ftrace_nr_registered_ops(void);
// /*AFLA*/ 
// /*AFLA*/ bool is_ftrace_trampoline(unsigned long addr);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The dyn_ftrace record's flags field is split into two parts.
// /*AFLA*/  * the first part which is '0-FTRACE_REF_MAX' is a counter of
// /*AFLA*/  * the number of callbacks that have registered the function that
// /*AFLA*/  * the dyn_ftrace descriptor represents.
// /*AFLA*/  *
// /*AFLA*/  * The second part is a mask:
// /*AFLA*/  *  ENABLED - the function is being traced
// /*AFLA*/  *  REGS    - the record wants the function to save regs
// /*AFLA*/  *  REGS_EN - the function is set up to save regs.
// /*AFLA*/  *  IPMODIFY - the record allows for the IP address to be changed.
// /*AFLA*/  *  DISABLED - the record is not ready to be touched yet
// /*AFLA*/  *
// /*AFLA*/  * When a new ftrace_ops is registered and wants a function to save
// /*AFLA*/  * pt_regs, the rec->flag REGS is set. When the function has been
// /*AFLA*/  * set up to save regs, the REG_EN flag is set. Once a function
// /*AFLA*/  * starts saving regs it will do so until all ftrace_ops are removed
// /*AFLA*/  * from tracing that function.
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	FTRACE_FL_ENABLED	= (1UL << 31),
// /*AFLA*/ 	FTRACE_FL_REGS		= (1UL << 30),
// /*AFLA*/ 	FTRACE_FL_REGS_EN	= (1UL << 29),
// /*AFLA*/ 	FTRACE_FL_TRAMP		= (1UL << 28),
// /*AFLA*/ 	FTRACE_FL_TRAMP_EN	= (1UL << 27),
// /*AFLA*/ 	FTRACE_FL_IPMODIFY	= (1UL << 26),
// /*AFLA*/ 	FTRACE_FL_DISABLED	= (1UL << 25),
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define FTRACE_REF_MAX_SHIFT	25
// /*AFLA*/ #define FTRACE_FL_BITS		7
// /*AFLA*/ #define FTRACE_FL_MASKED_BITS	((1UL << FTRACE_FL_BITS) - 1)
// /*AFLA*/ #define FTRACE_FL_MASK		(FTRACE_FL_MASKED_BITS << FTRACE_REF_MAX_SHIFT)
// /*AFLA*/ #define FTRACE_REF_MAX		((1UL << FTRACE_REF_MAX_SHIFT) - 1)
// /*AFLA*/ 
// /*AFLA*/ #define ftrace_rec_count(rec)	((rec)->flags & ~FTRACE_FL_MASK)
// /*AFLA*/ 
// /*AFLA*/ struct dyn_ftrace {
// /*AFLA*/ 	unsigned long		ip; /* address of mcount call-site */
// /*AFLA*/ 	unsigned long		flags;
// /*AFLA*/ 	struct dyn_arch_ftrace	arch;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ int ftrace_force_update(void);
// /*AFLA*/ int ftrace_set_filter_ip(struct ftrace_ops *ops, unsigned long ip,
// /*AFLA*/ 			 int remove, int reset);
// /*AFLA*/ int ftrace_set_filter(struct ftrace_ops *ops, unsigned char *buf,
// /*AFLA*/ 		       int len, int reset);
// /*AFLA*/ int ftrace_set_notrace(struct ftrace_ops *ops, unsigned char *buf,
// /*AFLA*/ 			int len, int reset);
// /*AFLA*/ void ftrace_set_global_filter(unsigned char *buf, int len, int reset);
// /*AFLA*/ void ftrace_set_global_notrace(unsigned char *buf, int len, int reset);
// /*AFLA*/ void ftrace_free_filter(struct ftrace_ops *ops);
// /*AFLA*/ 
// /*AFLA*/ int register_ftrace_command(struct ftrace_func_command *cmd);
// /*AFLA*/ int unregister_ftrace_command(struct ftrace_func_command *cmd);
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	FTRACE_UPDATE_CALLS		= (1 << 0),
// /*AFLA*/ 	FTRACE_DISABLE_CALLS		= (1 << 1),
// /*AFLA*/ 	FTRACE_UPDATE_TRACE_FUNC	= (1 << 2),
// /*AFLA*/ 	FTRACE_START_FUNC_RET		= (1 << 3),
// /*AFLA*/ 	FTRACE_STOP_FUNC_RET		= (1 << 4),
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The FTRACE_UPDATE_* enum is used to pass information back
// /*AFLA*/  * from the ftrace_update_record() and ftrace_test_record()
// /*AFLA*/  * functions. These are called by the code update routines
// /*AFLA*/  * to find out what is to be done for a given function.
// /*AFLA*/  *
// /*AFLA*/  *  IGNORE           - The function is already what we want it to be
// /*AFLA*/  *  MAKE_CALL        - Start tracing the function
// /*AFLA*/  *  MODIFY_CALL      - Stop saving regs for the function
// /*AFLA*/  *  MAKE_NOP         - Stop tracing the function
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	FTRACE_UPDATE_IGNORE,
// /*AFLA*/ 	FTRACE_UPDATE_MAKE_CALL,
// /*AFLA*/ 	FTRACE_UPDATE_MODIFY_CALL,
// /*AFLA*/ 	FTRACE_UPDATE_MAKE_NOP,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	FTRACE_ITER_FILTER	= (1 << 0),
// /*AFLA*/ 	FTRACE_ITER_NOTRACE	= (1 << 1),
// /*AFLA*/ 	FTRACE_ITER_PRINTALL	= (1 << 2),
// /*AFLA*/ 	FTRACE_ITER_DO_HASH	= (1 << 3),
// /*AFLA*/ 	FTRACE_ITER_HASH	= (1 << 4),
// /*AFLA*/ 	FTRACE_ITER_ENABLED	= (1 << 5),
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ void arch_ftrace_update_code(int command);
// /*AFLA*/ 
// /*AFLA*/ struct ftrace_rec_iter;
// /*AFLA*/ 
// /*AFLA*/ struct ftrace_rec_iter *ftrace_rec_iter_start(void);
// /*AFLA*/ struct ftrace_rec_iter *ftrace_rec_iter_next(struct ftrace_rec_iter *iter);
// /*AFLA*/ struct dyn_ftrace *ftrace_rec_iter_record(struct ftrace_rec_iter *iter);
// /*AFLA*/ 
// /*AFLA*/ #define for_ftrace_rec_iter(iter)		\
// /*AFLA*/ 	for (iter = ftrace_rec_iter_start();	\
// /*AFLA*/ 	     iter;				\
// /*AFLA*/ 	     iter = ftrace_rec_iter_next(iter))
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ int ftrace_update_record(struct dyn_ftrace *rec, int enable);
// /*AFLA*/ int ftrace_test_record(struct dyn_ftrace *rec, int enable);
// /*AFLA*/ void ftrace_run_stop_machine(int command);
// /*AFLA*/ unsigned long ftrace_location(unsigned long ip);
// /*AFLA*/ unsigned long ftrace_location_range(unsigned long start, unsigned long end);
// /*AFLA*/ unsigned long ftrace_get_addr_new(struct dyn_ftrace *rec);
// /*AFLA*/ unsigned long ftrace_get_addr_curr(struct dyn_ftrace *rec);
// /*AFLA*/ 
// /*AFLA*/ extern ftrace_func_t ftrace_trace_function;
// /*AFLA*/ 
// /*AFLA*/ int ftrace_regex_open(struct ftrace_ops *ops, int flag,
// /*AFLA*/ 		  struct inode *inode, struct file *file);
// /*AFLA*/ ssize_t ftrace_filter_write(struct file *file, const char __user *ubuf,
// /*AFLA*/ 			    size_t cnt, loff_t *ppos);
// /*AFLA*/ ssize_t ftrace_notrace_write(struct file *file, const char __user *ubuf,
// /*AFLA*/ 			     size_t cnt, loff_t *ppos);
// /*AFLA*/ int ftrace_regex_release(struct inode *inode, struct file *file);
// /*AFLA*/ 
// /*AFLA*/ void __init
// /*AFLA*/ ftrace_set_early_filter(struct ftrace_ops *ops, char *buf, int enable);
// /*AFLA*/ 
// /*AFLA*/ /* defined in arch */
// /*AFLA*/ extern int ftrace_ip_converted(unsigned long ip);
// /*AFLA*/ extern int ftrace_dyn_arch_init(void);
// /*AFLA*/ extern void ftrace_replace_code(int enable);
// /*AFLA*/ extern int ftrace_update_ftrace_func(ftrace_func_t func);
// /*AFLA*/ extern void ftrace_caller(void);
// /*AFLA*/ extern void ftrace_regs_caller(void);
// /*AFLA*/ extern void ftrace_call(void);
// /*AFLA*/ extern void ftrace_regs_call(void);
// /*AFLA*/ extern void mcount_call(void);
// /*AFLA*/ 
// /*AFLA*/ void ftrace_modify_all_code(int command);
// /*AFLA*/ 
// /*AFLA*/ #ifndef FTRACE_ADDR
// /*AFLA*/ #define FTRACE_ADDR ((unsigned long)ftrace_caller)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef FTRACE_GRAPH_ADDR
// /*AFLA*/ #define FTRACE_GRAPH_ADDR ((unsigned long)ftrace_graph_caller)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef FTRACE_REGS_ADDR
// /*AFLA*/ #ifdef CONFIG_DYNAMIC_FTRACE_WITH_REGS
// /*AFLA*/ # define FTRACE_REGS_ADDR ((unsigned long)ftrace_regs_caller)
// /*AFLA*/ #else
// /*AFLA*/ # define FTRACE_REGS_ADDR FTRACE_ADDR
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * If an arch would like functions that are only traced
// /*AFLA*/  * by the function graph tracer to jump directly to its own
// /*AFLA*/  * trampoline, then they can define FTRACE_GRAPH_TRAMP_ADDR
// /*AFLA*/  * to be that address to jump to.
// /*AFLA*/  */
// /*AFLA*/ #ifndef FTRACE_GRAPH_TRAMP_ADDR
// /*AFLA*/ #define FTRACE_GRAPH_TRAMP_ADDR ((unsigned long) 0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FUNCTION_GRAPH_TRACER
// /*AFLA*/ extern void ftrace_graph_caller(void);
// /*AFLA*/ extern int ftrace_enable_ftrace_graph_caller(void);
// /*AFLA*/ extern int ftrace_disable_ftrace_graph_caller(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline int ftrace_enable_ftrace_graph_caller(void) { return 0; }
// /*AFLA*/ static inline int ftrace_disable_ftrace_graph_caller(void) { return 0; }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ftrace_make_nop - convert code into nop
// /*AFLA*/  * @mod: module structure if called by module load initialization
// /*AFLA*/  * @rec: the mcount call site record
// /*AFLA*/  * @addr: the address that the call site should be calling
// /*AFLA*/  *
// /*AFLA*/  * This is a very sensitive operation and great care needs
// /*AFLA*/  * to be taken by the arch.  The operation should carefully
// /*AFLA*/  * read the location, check to see if what is read is indeed
// /*AFLA*/  * what we expect it to be, and then on success of the compare,
// /*AFLA*/  * it should write to the location.
// /*AFLA*/  *
// /*AFLA*/  * The code segment at @rec->ip should be a caller to @addr
// /*AFLA*/  *
// /*AFLA*/  * Return must be:
// /*AFLA*/  *  0 on success
// /*AFLA*/  *  -EFAULT on error reading the location
// /*AFLA*/  *  -EINVAL on a failed compare of the contents
// /*AFLA*/  *  -EPERM  on error writing to the location
// /*AFLA*/  * Any other value will be considered a failure.
// /*AFLA*/  */
// /*AFLA*/ extern int ftrace_make_nop(struct module *mod,
// /*AFLA*/ 			   struct dyn_ftrace *rec, unsigned long addr);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ftrace_make_call - convert a nop call site into a call to addr
// /*AFLA*/  * @rec: the mcount call site record
// /*AFLA*/  * @addr: the address that the call site should call
// /*AFLA*/  *
// /*AFLA*/  * This is a very sensitive operation and great care needs
// /*AFLA*/  * to be taken by the arch.  The operation should carefully
// /*AFLA*/  * read the location, check to see if what is read is indeed
// /*AFLA*/  * what we expect it to be, and then on success of the compare,
// /*AFLA*/  * it should write to the location.
// /*AFLA*/  *
// /*AFLA*/  * The code segment at @rec->ip should be a nop
// /*AFLA*/  *
// /*AFLA*/  * Return must be:
// /*AFLA*/  *  0 on success
// /*AFLA*/  *  -EFAULT on error reading the location
// /*AFLA*/  *  -EINVAL on a failed compare of the contents
// /*AFLA*/  *  -EPERM  on error writing to the location
// /*AFLA*/  * Any other value will be considered a failure.
// /*AFLA*/  */
// /*AFLA*/ extern int ftrace_make_call(struct dyn_ftrace *rec, unsigned long addr);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DYNAMIC_FTRACE_WITH_REGS
// /*AFLA*/ /**
// /*AFLA*/  * ftrace_modify_call - convert from one addr to another (no nop)
// /*AFLA*/  * @rec: the mcount call site record
// /*AFLA*/  * @old_addr: the address expected to be currently called to
// /*AFLA*/  * @addr: the address to change to
// /*AFLA*/  *
// /*AFLA*/  * This is a very sensitive operation and great care needs
// /*AFLA*/  * to be taken by the arch.  The operation should carefully
// /*AFLA*/  * read the location, check to see if what is read is indeed
// /*AFLA*/  * what we expect it to be, and then on success of the compare,
// /*AFLA*/  * it should write to the location.
// /*AFLA*/  *
// /*AFLA*/  * The code segment at @rec->ip should be a caller to @old_addr
// /*AFLA*/  *
// /*AFLA*/  * Return must be:
// /*AFLA*/  *  0 on success
// /*AFLA*/  *  -EFAULT on error reading the location
// /*AFLA*/  *  -EINVAL on a failed compare of the contents
// /*AFLA*/  *  -EPERM  on error writing to the location
// /*AFLA*/  * Any other value will be considered a failure.
// /*AFLA*/  */
// /*AFLA*/ extern int ftrace_modify_call(struct dyn_ftrace *rec, unsigned long old_addr,
// /*AFLA*/ 			      unsigned long addr);
// /*AFLA*/ #else
// /*AFLA*/ /* Should never be called */
// /*AFLA*/ static inline int ftrace_modify_call(struct dyn_ftrace *rec, unsigned long old_addr,
// /*AFLA*/ 				     unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* May be defined in arch */
// /*AFLA*/ extern int ftrace_arch_read_dyn_info(char *buf, int size);
// /*AFLA*/ 
// /*AFLA*/ extern int skip_trace(unsigned long ip);
// /*AFLA*/ extern void ftrace_module_init(struct module *mod);
// /*AFLA*/ extern void ftrace_module_enable(struct module *mod);
// /*AFLA*/ extern void ftrace_release_mod(struct module *mod);
// /*AFLA*/ 
// /*AFLA*/ extern void ftrace_disable_daemon(void);
// /*AFLA*/ extern void ftrace_enable_daemon(void);
// /*AFLA*/ #else /* CONFIG_DYNAMIC_FTRACE */
// /*AFLA*/ static inline int skip_trace(unsigned long ip) { return 0; }
// /*AFLA*/ static inline int ftrace_force_update(void) { return 0; }
// /*AFLA*/ static inline void ftrace_disable_daemon(void) { }
// /*AFLA*/ static inline void ftrace_enable_daemon(void) { }
// /*AFLA*/ static inline void ftrace_module_init(struct module *mod) { }
// /*AFLA*/ static inline void ftrace_module_enable(struct module *mod) { }
// /*AFLA*/ static inline void ftrace_release_mod(struct module *mod) { }
// /*AFLA*/ static inline __init int register_ftrace_command(struct ftrace_func_command *cmd)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ static inline __init int unregister_ftrace_command(char *cmd_name)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ static inline int ftrace_text_reserved(const void *start, const void *end)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline unsigned long ftrace_location(unsigned long ip)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Again users of functions that have ftrace_ops may not
// /*AFLA*/  * have them defined when ftrace is not enabled, but these
// /*AFLA*/  * functions may still be called. Use a macro instead of inline.
// /*AFLA*/  */
// /*AFLA*/ #define ftrace_regex_open(ops, flag, inod, file) ({ -ENODEV; })
// /*AFLA*/ #define ftrace_set_early_filter(ops, buf, enable) do { } while (0)
// /*AFLA*/ #define ftrace_set_filter_ip(ops, ip, remove, reset) ({ -ENODEV; })
// /*AFLA*/ #define ftrace_set_filter(ops, buf, len, reset) ({ -ENODEV; })
// /*AFLA*/ #define ftrace_set_notrace(ops, buf, len, reset) ({ -ENODEV; })
// /*AFLA*/ #define ftrace_free_filter(ops) do { } while (0)
// /*AFLA*/ 
// /*AFLA*/ static inline ssize_t ftrace_filter_write(struct file *file, const char __user *ubuf,
// /*AFLA*/ 			    size_t cnt, loff_t *ppos) { return -ENODEV; }
// /*AFLA*/ static inline ssize_t ftrace_notrace_write(struct file *file, const char __user *ubuf,
// /*AFLA*/ 			     size_t cnt, loff_t *ppos) { return -ENODEV; }
// /*AFLA*/ static inline int
// /*AFLA*/ ftrace_regex_release(struct inode *inode, struct file *file) { return -ENODEV; }
// /*AFLA*/ 
// /*AFLA*/ static inline bool is_ftrace_trampoline(unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_DYNAMIC_FTRACE */
// /*AFLA*/ 
// /*AFLA*/ /* totally disable ftrace - can not re-enable after this */
// /*AFLA*/ void ftrace_kill(void);
// /*AFLA*/ 
// /*AFLA*/ static inline void tracer_disable(void)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_FUNCTION_TRACER
// /*AFLA*/ 	ftrace_enabled = 0;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Ftrace disable/restore without lock. Some synchronization mechanism
// /*AFLA*/  * must be used to prevent ftrace_enabled to be changed between
// /*AFLA*/  * disable/restore.
// /*AFLA*/  */
// /*AFLA*/ static inline int __ftrace_enabled_save(void)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_FUNCTION_TRACER
// /*AFLA*/ 	int saved_ftrace_enabled = ftrace_enabled;
// /*AFLA*/ 	ftrace_enabled = 0;
// /*AFLA*/ 	return saved_ftrace_enabled;
// /*AFLA*/ #else
// /*AFLA*/ 	return 0;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __ftrace_enabled_restore(int enabled)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_FUNCTION_TRACER
// /*AFLA*/ 	ftrace_enabled = enabled;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* All archs should have this, but we define it for consistency */
// /*AFLA*/ #ifndef ftrace_return_address0
// /*AFLA*/ # define ftrace_return_address0 __builtin_return_address(0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Archs may use other ways for ADDR1 and beyond */
// /*AFLA*/ #ifndef ftrace_return_address
// /*AFLA*/ # ifdef CONFIG_FRAME_POINTER
// /*AFLA*/ #  define ftrace_return_address(n) __builtin_return_address(n)
// /*AFLA*/ # else
// /*AFLA*/ #  define ftrace_return_address(n) 0UL
// /*AFLA*/ # endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define CALLER_ADDR0 ((unsigned long)ftrace_return_address0)
// /*AFLA*/ #define CALLER_ADDR1 ((unsigned long)ftrace_return_address(1))
// /*AFLA*/ #define CALLER_ADDR2 ((unsigned long)ftrace_return_address(2))
// /*AFLA*/ #define CALLER_ADDR3 ((unsigned long)ftrace_return_address(3))
// /*AFLA*/ #define CALLER_ADDR4 ((unsigned long)ftrace_return_address(4))
// /*AFLA*/ #define CALLER_ADDR5 ((unsigned long)ftrace_return_address(5))
// /*AFLA*/ #define CALLER_ADDR6 ((unsigned long)ftrace_return_address(6))
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long get_lock_parent_ip(void)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long addr = CALLER_ADDR0;
// /*AFLA*/ 
// /*AFLA*/ 	if (!in_lock_functions(addr))
// /*AFLA*/ 		return addr;
// /*AFLA*/ 	addr = CALLER_ADDR1;
// /*AFLA*/ 	if (!in_lock_functions(addr))
// /*AFLA*/ 		return addr;
// /*AFLA*/ 	return CALLER_ADDR2;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_IRQSOFF_TRACER
// /*AFLA*/   extern void time_hardirqs_on(unsigned long a0, unsigned long a1);
// /*AFLA*/   extern void time_hardirqs_off(unsigned long a0, unsigned long a1);
// /*AFLA*/ #else
// /*AFLA*/   static inline void time_hardirqs_on(unsigned long a0, unsigned long a1) { }
// /*AFLA*/   static inline void time_hardirqs_off(unsigned long a0, unsigned long a1) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PREEMPT_TRACER
// /*AFLA*/   extern void trace_preempt_on(unsigned long a0, unsigned long a1);
// /*AFLA*/   extern void trace_preempt_off(unsigned long a0, unsigned long a1);
// /*AFLA*/ #else
// /*AFLA*/ /*
// /*AFLA*/  * Use defines instead of static inlines because some arches will make code out
// /*AFLA*/  * of the CALLER_ADDR, when we really want these to be a real nop.
// /*AFLA*/  */
// /*AFLA*/ # define trace_preempt_on(a0, a1) do { } while (0)
// /*AFLA*/ # define trace_preempt_off(a0, a1) do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FTRACE_MCOUNT_RECORD
// /*AFLA*/ extern void ftrace_init(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void ftrace_init(void) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Structure that defines an entry function trace.
// /*AFLA*/  * It's already packed but the attribute "packed" is needed
// /*AFLA*/  * to remove extra padding at the end.
// /*AFLA*/  */
// /*AFLA*/ struct ftrace_graph_ent {
// /*AFLA*/ 	unsigned long func; /* Current function */
// /*AFLA*/ 	int depth;
// /*AFLA*/ } __packed;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Structure that defines a return function trace.
// /*AFLA*/  * It's already packed but the attribute "packed" is needed
// /*AFLA*/  * to remove extra padding at the end.
// /*AFLA*/  */
// /*AFLA*/ struct ftrace_graph_ret {
// /*AFLA*/ 	unsigned long func; /* Current function */
// /*AFLA*/ 	/* Number of functions that overran the depth limit for current task */
// /*AFLA*/ 	unsigned long overrun;
// /*AFLA*/ 	unsigned long long calltime;
// /*AFLA*/ 	unsigned long long rettime;
// /*AFLA*/ 	int depth;
// /*AFLA*/ } __packed;
// /*AFLA*/ 
// /*AFLA*/ /* Type of the callback handlers for tracing function graph*/
// /*AFLA*/ typedef void (*trace_func_graph_ret_t)(struct ftrace_graph_ret *); /* return */
// /*AFLA*/ typedef int (*trace_func_graph_ent_t)(struct ftrace_graph_ent *); /* entry */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FUNCTION_GRAPH_TRACER
// /*AFLA*/ 
// /*AFLA*/ /* for init task */
// /*AFLA*/ #define INIT_FTRACE_GRAPH		.ret_stack = NULL,
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Stack of return addresses for functions
// /*AFLA*/  * of a thread.
// /*AFLA*/  * Used in struct thread_info
// /*AFLA*/  */
// /*AFLA*/ struct ftrace_ret_stack {
// /*AFLA*/ 	unsigned long ret;
// /*AFLA*/ 	unsigned long func;
// /*AFLA*/ 	unsigned long long calltime;
// /*AFLA*/ #ifdef CONFIG_FUNCTION_PROFILER
// /*AFLA*/ 	unsigned long long subtime;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef HAVE_FUNCTION_GRAPH_FP_TEST
// /*AFLA*/ 	unsigned long fp;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef HAVE_FUNCTION_GRAPH_RET_ADDR_PTR
// /*AFLA*/ 	unsigned long *retp;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Primary handler of a function return.
// /*AFLA*/  * It relays on ftrace_return_to_handler.
// /*AFLA*/  * Defined in entry_32/64.S
// /*AFLA*/  */
// /*AFLA*/ extern void return_to_handler(void);
// /*AFLA*/ 
// /*AFLA*/ extern int
// /*AFLA*/ ftrace_push_return_trace(unsigned long ret, unsigned long func, int *depth,
// /*AFLA*/ 			 unsigned long frame_pointer, unsigned long *retp);
// /*AFLA*/ 
// /*AFLA*/ unsigned long ftrace_graph_ret_addr(struct task_struct *task, int *idx,
// /*AFLA*/ 				    unsigned long ret, unsigned long *retp);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Sometimes we don't want to trace a function with the function
// /*AFLA*/  * graph tracer but we want them to keep traced by the usual function
// /*AFLA*/  * tracer if the function graph tracer is not configured.
// /*AFLA*/  */
// /*AFLA*/ #define __notrace_funcgraph		notrace
// /*AFLA*/ 
// /*AFLA*/ #define FTRACE_NOTRACE_DEPTH 65536
// /*AFLA*/ #define FTRACE_RETFUNC_DEPTH 50
// /*AFLA*/ #define FTRACE_RETSTACK_ALLOC_SIZE 32
// /*AFLA*/ extern int register_ftrace_graph(trace_func_graph_ret_t retfunc,
// /*AFLA*/ 				trace_func_graph_ent_t entryfunc);
// /*AFLA*/ 
// /*AFLA*/ extern bool ftrace_graph_is_dead(void);
// /*AFLA*/ extern void ftrace_graph_stop(void);
// /*AFLA*/ 
// /*AFLA*/ /* The current handlers in use */
// /*AFLA*/ extern trace_func_graph_ret_t ftrace_graph_return;
// /*AFLA*/ extern trace_func_graph_ent_t ftrace_graph_entry;
// /*AFLA*/ 
// /*AFLA*/ extern void unregister_ftrace_graph(void);
// /*AFLA*/ 
// /*AFLA*/ extern void ftrace_graph_init_task(struct task_struct *t);
// /*AFLA*/ extern void ftrace_graph_exit_task(struct task_struct *t);
// /*AFLA*/ extern void ftrace_graph_init_idle_task(struct task_struct *t, int cpu);
// /*AFLA*/ 
// /*AFLA*/ static inline int task_curr_ret_stack(struct task_struct *t)
// /*AFLA*/ {
// /*AFLA*/ 	return t->curr_ret_stack;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void pause_graph_tracing(void)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_inc(&current->tracing_graph_pause);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void unpause_graph_tracing(void)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_dec(&current->tracing_graph_pause);
// /*AFLA*/ }
// /*AFLA*/ #else /* !CONFIG_FUNCTION_GRAPH_TRACER */
// /*AFLA*/ 
// /*AFLA*/ #define __notrace_funcgraph
// /*AFLA*/ #define INIT_FTRACE_GRAPH
// /*AFLA*/ 
// /*AFLA*/ static inline void ftrace_graph_init_task(struct task_struct *t) { }
// /*AFLA*/ static inline void ftrace_graph_exit_task(struct task_struct *t) { }
// /*AFLA*/ static inline void ftrace_graph_init_idle_task(struct task_struct *t, int cpu) { }
// /*AFLA*/ 
// /*AFLA*/ static inline int register_ftrace_graph(trace_func_graph_ret_t retfunc,
// /*AFLA*/ 			  trace_func_graph_ent_t entryfunc)
// /*AFLA*/ {
// /*AFLA*/ 	return -1;
// /*AFLA*/ }
// /*AFLA*/ static inline void unregister_ftrace_graph(void) { }
// /*AFLA*/ 
// /*AFLA*/ static inline int task_curr_ret_stack(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return -1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long
// /*AFLA*/ ftrace_graph_ret_addr(struct task_struct *task, int *idx, unsigned long ret,
// /*AFLA*/ 		      unsigned long *retp)
// /*AFLA*/ {
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void pause_graph_tracing(void) { }
// /*AFLA*/ static inline void unpause_graph_tracing(void) { }
// /*AFLA*/ #endif /* CONFIG_FUNCTION_GRAPH_TRACER */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TRACING
// /*AFLA*/ 
// /*AFLA*/ /* flags for current->trace */
// /*AFLA*/ enum {
// /*AFLA*/ 	TSK_TRACE_FL_TRACE_BIT	= 0,
// /*AFLA*/ 	TSK_TRACE_FL_GRAPH_BIT	= 1,
// /*AFLA*/ };
// /*AFLA*/ enum {
// /*AFLA*/ 	TSK_TRACE_FL_TRACE	= 1 << TSK_TRACE_FL_TRACE_BIT,
// /*AFLA*/ 	TSK_TRACE_FL_GRAPH	= 1 << TSK_TRACE_FL_GRAPH_BIT,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline void set_tsk_trace_trace(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	set_bit(TSK_TRACE_FL_TRACE_BIT, &tsk->trace);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void clear_tsk_trace_trace(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	clear_bit(TSK_TRACE_FL_TRACE_BIT, &tsk->trace);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int test_tsk_trace_trace(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return tsk->trace & TSK_TRACE_FL_TRACE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void set_tsk_trace_graph(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	set_bit(TSK_TRACE_FL_GRAPH_BIT, &tsk->trace);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void clear_tsk_trace_graph(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	clear_bit(TSK_TRACE_FL_GRAPH_BIT, &tsk->trace);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int test_tsk_trace_graph(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return tsk->trace & TSK_TRACE_FL_GRAPH;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ enum ftrace_dump_mode;
// /*AFLA*/ 
// /*AFLA*/ extern enum ftrace_dump_mode ftrace_dump_on_oops;
// /*AFLA*/ extern int tracepoint_printk;
// /*AFLA*/ 
// /*AFLA*/ extern void disable_trace_on_warning(void);
// /*AFLA*/ extern int __disable_trace_on_warning;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PREEMPT
// /*AFLA*/ #define INIT_TRACE_RECURSION		.trace_recursion = 0,
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_TRACING */
// /*AFLA*/ static inline void  disable_trace_on_warning(void) { }
// /*AFLA*/ #endif /* CONFIG_TRACING */
// /*AFLA*/ 
// /*AFLA*/ #ifndef INIT_TRACE_RECURSION
// /*AFLA*/ #define INIT_TRACE_RECURSION
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FTRACE_SYSCALLS
// /*AFLA*/ 
// /*AFLA*/ unsigned long arch_syscall_addr(int nr);
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_FTRACE_SYSCALLS */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_FTRACE_H */
