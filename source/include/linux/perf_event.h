// /*AFLA*/ /*
// /*AFLA*/  * Performance events:
// /*AFLA*/  *
// /*AFLA*/  *    Copyright (C) 2008-2009, Thomas Gleixner <tglx@linutronix.de>
// /*AFLA*/  *    Copyright (C) 2008-2011, Red Hat, Inc., Ingo Molnar
// /*AFLA*/  *    Copyright (C) 2008-2011, Red Hat, Inc., Peter Zijlstra
// /*AFLA*/  *
// /*AFLA*/  * Data type definitions, declarations, prototypes.
// /*AFLA*/  *
// /*AFLA*/  *    Started by: Thomas Gleixner and Ingo Molnar
// /*AFLA*/  *
// /*AFLA*/  * For licencing details see kernel-base/COPYING
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LINUX_PERF_EVENT_H
// /*AFLA*/ #define _LINUX_PERF_EVENT_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/perf_event.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Kernel-internal data types and definitions:
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PERF_EVENTS
// /*AFLA*/ # include <asm/perf_event.h>
// /*AFLA*/ # include <asm/local64.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct perf_guest_info_callbacks {
// /*AFLA*/ 	int				(*is_in_guest)(void);
// /*AFLA*/ 	int				(*is_user_mode)(void);
// /*AFLA*/ 	unsigned long			(*get_guest_ip)(void);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_HW_BREAKPOINT
// /*AFLA*/ #include <asm/hw_breakpoint.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/mutex.h>
// /*AFLA*/ #include <linux/rculist.h>
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/hrtimer.h>
// /*AFLA*/ #include <linux/fs.h>
// /*AFLA*/ #include <linux/pid_namespace.h>
// /*AFLA*/ #include <linux/workqueue.h>
// /*AFLA*/ #include <linux/ftrace.h>
// /*AFLA*/ #include <linux/cpu.h>
// /*AFLA*/ #include <linux/irq_work.h>
// /*AFLA*/ #include <linux/static_key.h>
// /*AFLA*/ #include <linux/jump_label_ratelimit.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/sysfs.h>
// /*AFLA*/ #include <linux/perf_regs.h>
// /*AFLA*/ #include <linux/workqueue.h>
// /*AFLA*/ #include <linux/cgroup.h>
// /*AFLA*/ #include <asm/local.h>
// /*AFLA*/ 
// /*AFLA*/ struct perf_callchain_entry {
// /*AFLA*/ 	__u64				nr;
// /*AFLA*/ 	__u64				ip[0]; /* /proc/sys/kernel/perf_event_max_stack */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct perf_callchain_entry_ctx {
// /*AFLA*/ 	struct perf_callchain_entry *entry;
// /*AFLA*/ 	u32			    max_stack;
// /*AFLA*/ 	u32			    nr;
// /*AFLA*/ 	short			    contexts;
// /*AFLA*/ 	bool			    contexts_maxed;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ typedef unsigned long (*perf_copy_f)(void *dst, const void *src,
// /*AFLA*/ 				     unsigned long off, unsigned long len);
// /*AFLA*/ 
// /*AFLA*/ struct perf_raw_frag {
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct perf_raw_frag	*next;
// /*AFLA*/ 		unsigned long		pad;
// /*AFLA*/ 	};
// /*AFLA*/ 	perf_copy_f			copy;
// /*AFLA*/ 	void				*data;
// /*AFLA*/ 	u32				size;
// /*AFLA*/ } __packed;
// /*AFLA*/ 
// /*AFLA*/ struct perf_raw_record {
// /*AFLA*/ 	struct perf_raw_frag		frag;
// /*AFLA*/ 	u32				size;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * branch stack layout:
// /*AFLA*/  *  nr: number of taken branches stored in entries[]
// /*AFLA*/  *
// /*AFLA*/  * Note that nr can vary from sample to sample
// /*AFLA*/  * branches (to, from) are stored from most recent
// /*AFLA*/  * to least recent, i.e., entries[0] contains the most
// /*AFLA*/  * recent branch.
// /*AFLA*/  */
// /*AFLA*/ struct perf_branch_stack {
// /*AFLA*/ 	__u64				nr;
// /*AFLA*/ 	struct perf_branch_entry	entries[0];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct task_struct;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * extra PMU register associated with an event
// /*AFLA*/  */
// /*AFLA*/ struct hw_perf_event_extra {
// /*AFLA*/ 	u64		config;	/* register value */
// /*AFLA*/ 	unsigned int	reg;	/* register address or index */
// /*AFLA*/ 	int		alloc;	/* extra register already allocated */
// /*AFLA*/ 	int		idx;	/* index in shared_regs->regs[] */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct hw_perf_event - performance event hardware details:
// /*AFLA*/  */
// /*AFLA*/ struct hw_perf_event {
// /*AFLA*/ #ifdef CONFIG_PERF_EVENTS
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct { /* hardware */
// /*AFLA*/ 			u64		config;
// /*AFLA*/ 			u64		last_tag;
// /*AFLA*/ 			unsigned long	config_base;
// /*AFLA*/ 			unsigned long	event_base;
// /*AFLA*/ 			int		event_base_rdpmc;
// /*AFLA*/ 			int		idx;
// /*AFLA*/ 			int		last_cpu;
// /*AFLA*/ 			int		flags;
// /*AFLA*/ 
// /*AFLA*/ 			struct hw_perf_event_extra extra_reg;
// /*AFLA*/ 			struct hw_perf_event_extra branch_reg;
// /*AFLA*/ 		};
// /*AFLA*/ 		struct { /* software */
// /*AFLA*/ 			struct hrtimer	hrtimer;
// /*AFLA*/ 		};
// /*AFLA*/ 		struct { /* tracepoint */
// /*AFLA*/ 			/* for tp_event->class */
// /*AFLA*/ 			struct list_head	tp_list;
// /*AFLA*/ 		};
// /*AFLA*/ 		struct { /* intel_cqm */
// /*AFLA*/ 			int			cqm_state;
// /*AFLA*/ 			u32			cqm_rmid;
// /*AFLA*/ 			int			is_group_event;
// /*AFLA*/ 			struct list_head	cqm_events_entry;
// /*AFLA*/ 			struct list_head	cqm_groups_entry;
// /*AFLA*/ 			struct list_head	cqm_group_entry;
// /*AFLA*/ 		};
// /*AFLA*/ 		struct { /* itrace */
// /*AFLA*/ 			int			itrace_started;
// /*AFLA*/ 		};
// /*AFLA*/ 		struct { /* amd_power */
// /*AFLA*/ 			u64	pwr_acc;
// /*AFLA*/ 			u64	ptsc;
// /*AFLA*/ 		};
// /*AFLA*/ #ifdef CONFIG_HAVE_HW_BREAKPOINT
// /*AFLA*/ 		struct { /* breakpoint */
// /*AFLA*/ 			/*
// /*AFLA*/ 			 * Crufty hack to avoid the chicken and egg
// /*AFLA*/ 			 * problem hw_breakpoint has with context
// /*AFLA*/ 			 * creation and event initalization.
// /*AFLA*/ 			 */
// /*AFLA*/ 			struct arch_hw_breakpoint	info;
// /*AFLA*/ 			struct list_head		bp_list;
// /*AFLA*/ 		};
// /*AFLA*/ #endif
// /*AFLA*/ 	};
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If the event is a per task event, this will point to the task in
// /*AFLA*/ 	 * question. See the comment in perf_event_alloc().
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct task_struct		*target;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * PMU would store hardware filter configuration
// /*AFLA*/ 	 * here.
// /*AFLA*/ 	 */
// /*AFLA*/ 	void				*addr_filters;
// /*AFLA*/ 
// /*AFLA*/ 	/* Last sync'ed generation of filters */
// /*AFLA*/ 	unsigned long			addr_filters_gen;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * hw_perf_event::state flags; used to track the PERF_EF_* state.
// /*AFLA*/  */
// /*AFLA*/ #define PERF_HES_STOPPED	0x01 /* the counter is stopped */
// /*AFLA*/ #define PERF_HES_UPTODATE	0x02 /* event->count up-to-date */
// /*AFLA*/ #define PERF_HES_ARCH		0x04
// /*AFLA*/ 
// /*AFLA*/ 	int				state;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The last observed hardware counter value, updated with a
// /*AFLA*/ 	 * local64_cmpxchg() such that pmu::read() can be called nested.
// /*AFLA*/ 	 */
// /*AFLA*/ 	local64_t			prev_count;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The period to start the next sample with.
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64				sample_period;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The period we started this sample with.
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64				last_period;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * However much is left of the current period; note that this is
// /*AFLA*/ 	 * a full 64bit value and allows for generation of periods longer
// /*AFLA*/ 	 * than hardware might allow.
// /*AFLA*/ 	 */
// /*AFLA*/ 	local64_t			period_left;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * State for throttling the event, see __perf_event_overflow() and
// /*AFLA*/ 	 * perf_adjust_freq_unthr_context().
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64                             interrupts_seq;
// /*AFLA*/ 	u64				interrupts;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * State for freq target events, see __perf_event_overflow() and
// /*AFLA*/ 	 * perf_adjust_freq_unthr_context().
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64				freq_time_stamp;
// /*AFLA*/ 	u64				freq_count_stamp;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct perf_event;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Common implementation detail of pmu::{start,commit,cancel}_txn
// /*AFLA*/  */
// /*AFLA*/ #define PERF_PMU_TXN_ADD  0x1		/* txn to add/schedule event on PMU */
// /*AFLA*/ #define PERF_PMU_TXN_READ 0x2		/* txn to read event group from PMU */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * pmu::capabilities flags
// /*AFLA*/  */
// /*AFLA*/ #define PERF_PMU_CAP_NO_INTERRUPT		0x01
// /*AFLA*/ #define PERF_PMU_CAP_NO_NMI			0x02
// /*AFLA*/ #define PERF_PMU_CAP_AUX_NO_SG			0x04
// /*AFLA*/ #define PERF_PMU_CAP_AUX_SW_DOUBLEBUF		0x08
// /*AFLA*/ #define PERF_PMU_CAP_EXCLUSIVE			0x10
// /*AFLA*/ #define PERF_PMU_CAP_ITRACE			0x20
// /*AFLA*/ #define PERF_PMU_CAP_HETEROGENEOUS_CPUS		0x40
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct pmu - generic performance monitoring unit
// /*AFLA*/  */
// /*AFLA*/ struct pmu {
// /*AFLA*/ 	struct list_head		entry;
// /*AFLA*/ 
// /*AFLA*/ 	struct module			*module;
// /*AFLA*/ 	struct device			*dev;
// /*AFLA*/ 	const struct attribute_group	**attr_groups;
// /*AFLA*/ 	const char			*name;
// /*AFLA*/ 	int				type;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * various common per-pmu feature flags
// /*AFLA*/ 	 */
// /*AFLA*/ 	int				capabilities;
// /*AFLA*/ 
// /*AFLA*/ 	int * __percpu			pmu_disable_count;
// /*AFLA*/ 	struct perf_cpu_context * __percpu pmu_cpu_context;
// /*AFLA*/ 	atomic_t			exclusive_cnt; /* < 0: cpu; > 0: tsk */
// /*AFLA*/ 	int				task_ctx_nr;
// /*AFLA*/ 	int				hrtimer_interval_ms;
// /*AFLA*/ 
// /*AFLA*/ 	/* number of address filters this PMU can do */
// /*AFLA*/ 	unsigned int			nr_addr_filters;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Fully disable/enable this PMU, can be used to protect from the PMI
// /*AFLA*/ 	 * as well as for lazy/batch writing of the MSRs.
// /*AFLA*/ 	 */
// /*AFLA*/ 	void (*pmu_enable)		(struct pmu *pmu); /* optional */
// /*AFLA*/ 	void (*pmu_disable)		(struct pmu *pmu); /* optional */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Try and initialize the event for this PMU.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Returns:
// /*AFLA*/ 	 *  -ENOENT	-- @event is not for this PMU
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *  -ENODEV	-- @event is for this PMU but PMU not present
// /*AFLA*/ 	 *  -EBUSY	-- @event is for this PMU but PMU temporarily unavailable
// /*AFLA*/ 	 *  -EINVAL	-- @event is for this PMU but @event is not valid
// /*AFLA*/ 	 *  -EOPNOTSUPP -- @event is for this PMU, @event is valid, but not supported
// /*AFLA*/ 	 *  -EACCESS	-- @event is for this PMU, @event is valid, but no privilidges
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *  0		-- @event is for this PMU and valid
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Other error return values are allowed.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int (*event_init)		(struct perf_event *event);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Notification that the event was mapped or unmapped.  Called
// /*AFLA*/ 	 * in the context of the mapping task.
// /*AFLA*/ 	 */
// /*AFLA*/ 	void (*event_mapped)		(struct perf_event *event); /*optional*/
// /*AFLA*/ 	void (*event_unmapped)		(struct perf_event *event); /*optional*/
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Flags for ->add()/->del()/ ->start()/->stop(). There are
// /*AFLA*/ 	 * matching hw_perf_event::state flags.
// /*AFLA*/ 	 */
// /*AFLA*/ #define PERF_EF_START	0x01		/* start the counter when adding    */
// /*AFLA*/ #define PERF_EF_RELOAD	0x02		/* reload the counter when starting */
// /*AFLA*/ #define PERF_EF_UPDATE	0x04		/* update the counter when stopping */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Adds/Removes a counter to/from the PMU, can be done inside a
// /*AFLA*/ 	 * transaction, see the ->*_txn() methods.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * The add/del callbacks will reserve all hardware resources required
// /*AFLA*/ 	 * to service the event, this includes any counter constraint
// /*AFLA*/ 	 * scheduling etc.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Called with IRQs disabled and the PMU disabled on the CPU the event
// /*AFLA*/ 	 * is on.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * ->add() called without PERF_EF_START should result in the same state
// /*AFLA*/ 	 *  as ->add() followed by ->stop().
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * ->del() must always PERF_EF_UPDATE stop an event. If it calls
// /*AFLA*/ 	 *  ->stop() that must deal with already being stopped without
// /*AFLA*/ 	 *  PERF_EF_UPDATE.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int  (*add)			(struct perf_event *event, int flags);
// /*AFLA*/ 	void (*del)			(struct perf_event *event, int flags);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Starts/Stops a counter present on the PMU.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * The PMI handler should stop the counter when perf_event_overflow()
// /*AFLA*/ 	 * returns !0. ->start() will be used to continue.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Also used to change the sample period.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Called with IRQs disabled and the PMU disabled on the CPU the event
// /*AFLA*/ 	 * is on -- will be called from NMI context with the PMU generates
// /*AFLA*/ 	 * NMIs.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * ->stop() with PERF_EF_UPDATE will read the counter and update
// /*AFLA*/ 	 *  period/count values like ->read() would.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * ->start() with PERF_EF_RELOAD will reprogram the the counter
// /*AFLA*/ 	 *  value, must be preceded by a ->stop() with PERF_EF_UPDATE.
// /*AFLA*/ 	 */
// /*AFLA*/ 	void (*start)			(struct perf_event *event, int flags);
// /*AFLA*/ 	void (*stop)			(struct perf_event *event, int flags);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Updates the counter value of the event.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * For sampling capable PMUs this will also update the software period
// /*AFLA*/ 	 * hw_perf_event::period_left field.
// /*AFLA*/ 	 */
// /*AFLA*/ 	void (*read)			(struct perf_event *event);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Group events scheduling is treated as a transaction, add
// /*AFLA*/ 	 * group events as a whole and perform one schedulability test.
// /*AFLA*/ 	 * If the test fails, roll back the whole group
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Start the transaction, after this ->add() doesn't need to
// /*AFLA*/ 	 * do schedulability tests.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Optional.
// /*AFLA*/ 	 */
// /*AFLA*/ 	void (*start_txn)		(struct pmu *pmu, unsigned int txn_flags);
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If ->start_txn() disabled the ->add() schedulability test
// /*AFLA*/ 	 * then ->commit_txn() is required to perform one. On success
// /*AFLA*/ 	 * the transaction is closed. On error the transaction is kept
// /*AFLA*/ 	 * open until ->cancel_txn() is called.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Optional.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int  (*commit_txn)		(struct pmu *pmu);
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Will cancel the transaction, assumes ->del() is called
// /*AFLA*/ 	 * for each successful ->add() during the transaction.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Optional.
// /*AFLA*/ 	 */
// /*AFLA*/ 	void (*cancel_txn)		(struct pmu *pmu);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Will return the value for perf_event_mmap_page::index for this event,
// /*AFLA*/ 	 * if no implementation is provided it will default to: event->hw.idx + 1.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int (*event_idx)		(struct perf_event *event); /*optional */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * context-switches callback
// /*AFLA*/ 	 */
// /*AFLA*/ 	void (*sched_task)		(struct perf_event_context *ctx,
// /*AFLA*/ 					bool sched_in);
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * PMU specific data size
// /*AFLA*/ 	 */
// /*AFLA*/ 	size_t				task_ctx_size;
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Return the count value for a counter.
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64 (*count)			(struct perf_event *event); /*optional*/
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Set up pmu-private data structures for an AUX area
// /*AFLA*/ 	 */
// /*AFLA*/ 	void *(*setup_aux)		(int cpu, void **pages,
// /*AFLA*/ 					 int nr_pages, bool overwrite);
// /*AFLA*/ 					/* optional */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Free pmu-private AUX data structures
// /*AFLA*/ 	 */
// /*AFLA*/ 	void (*free_aux)		(void *aux); /* optional */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Validate address range filters: make sure the HW supports the
// /*AFLA*/ 	 * requested configuration and number of filters; return 0 if the
// /*AFLA*/ 	 * supplied filters are valid, -errno otherwise.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Runs in the context of the ioctl()ing process and is not serialized
// /*AFLA*/ 	 * with the rest of the PMU callbacks.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int (*addr_filters_validate)	(struct list_head *filters);
// /*AFLA*/ 					/* optional */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Synchronize address range filter configuration:
// /*AFLA*/ 	 * translate hw-agnostic filters into hardware configuration in
// /*AFLA*/ 	 * event::hw::addr_filters.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Runs as a part of filter sync sequence that is done in ->start()
// /*AFLA*/ 	 * callback by calling perf_event_addr_filters_sync().
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * May (and should) traverse event::addr_filters::list, for which its
// /*AFLA*/ 	 * caller provides necessary serialization.
// /*AFLA*/ 	 */
// /*AFLA*/ 	void (*addr_filters_sync)	(struct perf_event *event);
// /*AFLA*/ 					/* optional */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Filter events for PMU-specific reasons.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int (*filter_match)		(struct perf_event *event); /* optional */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct perf_addr_filter - address range filter definition
// /*AFLA*/  * @entry:	event's filter list linkage
// /*AFLA*/  * @inode:	object file's inode for file-based filters
// /*AFLA*/  * @offset:	filter range offset
// /*AFLA*/  * @size:	filter range size
// /*AFLA*/  * @range:	1: range, 0: address
// /*AFLA*/  * @filter:	1: filter/start, 0: stop
// /*AFLA*/  *
// /*AFLA*/  * This is a hardware-agnostic filter configuration as specified by the user.
// /*AFLA*/  */
// /*AFLA*/ struct perf_addr_filter {
// /*AFLA*/ 	struct list_head	entry;
// /*AFLA*/ 	struct inode		*inode;
// /*AFLA*/ 	unsigned long		offset;
// /*AFLA*/ 	unsigned long		size;
// /*AFLA*/ 	unsigned int		range	: 1,
// /*AFLA*/ 				filter	: 1;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct perf_addr_filters_head - container for address range filters
// /*AFLA*/  * @list:	list of filters for this event
// /*AFLA*/  * @lock:	spinlock that serializes accesses to the @list and event's
// /*AFLA*/  *		(and its children's) filter generations.
// /*AFLA*/  *
// /*AFLA*/  * A child event will use parent's @list (and therefore @lock), so they are
// /*AFLA*/  * bundled together; see perf_event_addr_filters().
// /*AFLA*/  */
// /*AFLA*/ struct perf_addr_filters_head {
// /*AFLA*/ 	struct list_head	list;
// /*AFLA*/ 	raw_spinlock_t		lock;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * enum perf_event_active_state - the states of a event
// /*AFLA*/  */
// /*AFLA*/ enum perf_event_active_state {
// /*AFLA*/ 	PERF_EVENT_STATE_DEAD		= -4,
// /*AFLA*/ 	PERF_EVENT_STATE_EXIT		= -3,
// /*AFLA*/ 	PERF_EVENT_STATE_ERROR		= -2,
// /*AFLA*/ 	PERF_EVENT_STATE_OFF		= -1,
// /*AFLA*/ 	PERF_EVENT_STATE_INACTIVE	=  0,
// /*AFLA*/ 	PERF_EVENT_STATE_ACTIVE		=  1,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct file;
// /*AFLA*/ struct perf_sample_data;
// /*AFLA*/ 
// /*AFLA*/ typedef void (*perf_overflow_handler_t)(struct perf_event *,
// /*AFLA*/ 					struct perf_sample_data *,
// /*AFLA*/ 					struct pt_regs *regs);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Event capabilities. For event_caps and groups caps.
// /*AFLA*/  *
// /*AFLA*/  * PERF_EV_CAP_SOFTWARE: Is a software event.
// /*AFLA*/  * PERF_EV_CAP_READ_ACTIVE_PKG: A CPU event (or cgroup event) that can be read
// /*AFLA*/  * from any CPU in the package where it is active.
// /*AFLA*/  */
// /*AFLA*/ #define PERF_EV_CAP_SOFTWARE		BIT(0)
// /*AFLA*/ #define PERF_EV_CAP_READ_ACTIVE_PKG	BIT(1)
// /*AFLA*/ 
// /*AFLA*/ #define SWEVENT_HLIST_BITS		8
// /*AFLA*/ #define SWEVENT_HLIST_SIZE		(1 << SWEVENT_HLIST_BITS)
// /*AFLA*/ 
// /*AFLA*/ struct swevent_hlist {
// /*AFLA*/ 	struct hlist_head		heads[SWEVENT_HLIST_SIZE];
// /*AFLA*/ 	struct rcu_head			rcu_head;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define PERF_ATTACH_CONTEXT	0x01
// /*AFLA*/ #define PERF_ATTACH_GROUP	0x02
// /*AFLA*/ #define PERF_ATTACH_TASK	0x04
// /*AFLA*/ #define PERF_ATTACH_TASK_DATA	0x08
// /*AFLA*/ 
// /*AFLA*/ struct perf_cgroup;
// /*AFLA*/ struct ring_buffer;
// /*AFLA*/ 
// /*AFLA*/ struct pmu_event_list {
// /*AFLA*/ 	raw_spinlock_t		lock;
// /*AFLA*/ 	struct list_head	list;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct perf_event - performance event kernel representation:
// /*AFLA*/  */
// /*AFLA*/ struct perf_event {
// /*AFLA*/ #ifdef CONFIG_PERF_EVENTS
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * entry onto perf_event_context::event_list;
// /*AFLA*/ 	 *   modifications require ctx->lock
// /*AFLA*/ 	 *   RCU safe iterations.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_head		event_entry;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * XXX: group_entry and sibling_list should be mutually exclusive;
// /*AFLA*/ 	 * either you're a sibling on a group, or you're the group leader.
// /*AFLA*/ 	 * Rework the code to always use the same list element.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Locked for modification by both ctx->mutex and ctx->lock; holding
// /*AFLA*/ 	 * either sufficies for read.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_head		group_entry;
// /*AFLA*/ 	struct list_head		sibling_list;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * We need storage to track the entries in perf_pmu_migrate_context; we
// /*AFLA*/ 	 * cannot use the event_entry because of RCU and we want to keep the
// /*AFLA*/ 	 * group in tact which avoids us using the other two entries.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_head		migrate_entry;
// /*AFLA*/ 
// /*AFLA*/ 	struct hlist_node		hlist_entry;
// /*AFLA*/ 	struct list_head		active_entry;
// /*AFLA*/ 	int				nr_siblings;
// /*AFLA*/ 
// /*AFLA*/ 	/* Not serialized. Only written during event initialization. */
// /*AFLA*/ 	int				event_caps;
// /*AFLA*/ 	/* The cumulative AND of all event_caps for events in this group. */
// /*AFLA*/ 	int				group_caps;
// /*AFLA*/ 
// /*AFLA*/ 	struct perf_event		*group_leader;
// /*AFLA*/ 	struct pmu			*pmu;
// /*AFLA*/ 	void				*pmu_private;
// /*AFLA*/ 
// /*AFLA*/ 	enum perf_event_active_state	state;
// /*AFLA*/ 	unsigned int			attach_state;
// /*AFLA*/ 	local64_t			count;
// /*AFLA*/ 	atomic64_t			child_count;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * These are the total time in nanoseconds that the event
// /*AFLA*/ 	 * has been enabled (i.e. eligible to run, and the task has
// /*AFLA*/ 	 * been scheduled in, if this is a per-task event)
// /*AFLA*/ 	 * and running (scheduled onto the CPU), respectively.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * They are computed from tstamp_enabled, tstamp_running and
// /*AFLA*/ 	 * tstamp_stopped when the event is in INACTIVE or ACTIVE state.
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64				total_time_enabled;
// /*AFLA*/ 	u64				total_time_running;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * These are timestamps used for computing total_time_enabled
// /*AFLA*/ 	 * and total_time_running when the event is in INACTIVE or
// /*AFLA*/ 	 * ACTIVE state, measured in nanoseconds from an arbitrary point
// /*AFLA*/ 	 * in time.
// /*AFLA*/ 	 * tstamp_enabled: the notional time when the event was enabled
// /*AFLA*/ 	 * tstamp_running: the notional time when the event was scheduled on
// /*AFLA*/ 	 * tstamp_stopped: in INACTIVE state, the notional time when the
// /*AFLA*/ 	 *	event was scheduled off.
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64				tstamp_enabled;
// /*AFLA*/ 	u64				tstamp_running;
// /*AFLA*/ 	u64				tstamp_stopped;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * timestamp shadows the actual context timing but it can
// /*AFLA*/ 	 * be safely used in NMI interrupt context. It reflects the
// /*AFLA*/ 	 * context time as it was when the event was last scheduled in.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * ctx_time already accounts for ctx->timestamp. Therefore to
// /*AFLA*/ 	 * compute ctx_time for a sample, simply add perf_clock().
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64				shadow_ctx_time;
// /*AFLA*/ 
// /*AFLA*/ 	struct perf_event_attr		attr;
// /*AFLA*/ 	u16				header_size;
// /*AFLA*/ 	u16				id_header_size;
// /*AFLA*/ 	u16				read_size;
// /*AFLA*/ 	struct hw_perf_event		hw;
// /*AFLA*/ 
// /*AFLA*/ 	struct perf_event_context	*ctx;
// /*AFLA*/ 	atomic_long_t			refcount;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * These accumulate total time (in nanoseconds) that children
// /*AFLA*/ 	 * events have been enabled and running, respectively.
// /*AFLA*/ 	 */
// /*AFLA*/ 	atomic64_t			child_total_time_enabled;
// /*AFLA*/ 	atomic64_t			child_total_time_running;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Protect attach/detach and child_list:
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct mutex			child_mutex;
// /*AFLA*/ 	struct list_head		child_list;
// /*AFLA*/ 	struct perf_event		*parent;
// /*AFLA*/ 
// /*AFLA*/ 	int				oncpu;
// /*AFLA*/ 	int				cpu;
// /*AFLA*/ 
// /*AFLA*/ 	struct list_head		owner_entry;
// /*AFLA*/ 	struct task_struct		*owner;
// /*AFLA*/ 
// /*AFLA*/ 	/* mmap bits */
// /*AFLA*/ 	struct mutex			mmap_mutex;
// /*AFLA*/ 	atomic_t			mmap_count;
// /*AFLA*/ 
// /*AFLA*/ 	struct ring_buffer		*rb;
// /*AFLA*/ 	struct list_head		rb_entry;
// /*AFLA*/ 	unsigned long			rcu_batches;
// /*AFLA*/ 	int				rcu_pending;
// /*AFLA*/ 
// /*AFLA*/ 	/* poll related */
// /*AFLA*/ 	wait_queue_head_t		waitq;
// /*AFLA*/ 	struct fasync_struct		*fasync;
// /*AFLA*/ 
// /*AFLA*/ 	/* delayed work for NMIs and such */
// /*AFLA*/ 	int				pending_wakeup;
// /*AFLA*/ 	int				pending_kill;
// /*AFLA*/ 	int				pending_disable;
// /*AFLA*/ 	struct irq_work			pending;
// /*AFLA*/ 
// /*AFLA*/ 	atomic_t			event_limit;
// /*AFLA*/ 
// /*AFLA*/ 	/* address range filters */
// /*AFLA*/ 	struct perf_addr_filters_head	addr_filters;
// /*AFLA*/ 	/* vma address array for file-based filders */
// /*AFLA*/ 	unsigned long			*addr_filters_offs;
// /*AFLA*/ 	unsigned long			addr_filters_gen;
// /*AFLA*/ 
// /*AFLA*/ 	void (*destroy)(struct perf_event *);
// /*AFLA*/ 	struct rcu_head			rcu_head;
// /*AFLA*/ 
// /*AFLA*/ 	struct pid_namespace		*ns;
// /*AFLA*/ 	u64				id;
// /*AFLA*/ 
// /*AFLA*/ 	u64				(*clock)(void);
// /*AFLA*/ 	perf_overflow_handler_t		overflow_handler;
// /*AFLA*/ 	void				*overflow_handler_context;
// /*AFLA*/ #ifdef CONFIG_BPF_SYSCALL
// /*AFLA*/ 	perf_overflow_handler_t		orig_overflow_handler;
// /*AFLA*/ 	struct bpf_prog			*prog;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_EVENT_TRACING
// /*AFLA*/ 	struct trace_event_call		*tp_event;
// /*AFLA*/ 	struct event_filter		*filter;
// /*AFLA*/ #ifdef CONFIG_FUNCTION_TRACER
// /*AFLA*/ 	struct ftrace_ops               ftrace_ops;
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_CGROUP_PERF
// /*AFLA*/ 	struct perf_cgroup		*cgrp; /* cgroup event is attach to */
// /*AFLA*/ 	int				cgrp_defer_enabled;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	struct list_head		sb_list;
// /*AFLA*/ #endif /* CONFIG_PERF_EVENTS */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct perf_event_context - event context structure
// /*AFLA*/  *
// /*AFLA*/  * Used as a container for task events and CPU events as well:
// /*AFLA*/  */
// /*AFLA*/ struct perf_event_context {
// /*AFLA*/ 	struct pmu			*pmu;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Protect the states of the events in the list,
// /*AFLA*/ 	 * nr_active, and the list:
// /*AFLA*/ 	 */
// /*AFLA*/ 	raw_spinlock_t			lock;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Protect the list of events.  Locking either mutex or lock
// /*AFLA*/ 	 * is sufficient to ensure the list doesn't change; to change
// /*AFLA*/ 	 * the list you need to lock both the mutex and the spinlock.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct mutex			mutex;
// /*AFLA*/ 
// /*AFLA*/ 	struct list_head		active_ctx_list;
// /*AFLA*/ 	struct list_head		pinned_groups;
// /*AFLA*/ 	struct list_head		flexible_groups;
// /*AFLA*/ 	struct list_head		event_list;
// /*AFLA*/ 	int				nr_events;
// /*AFLA*/ 	int				nr_active;
// /*AFLA*/ 	int				is_active;
// /*AFLA*/ 	int				nr_stat;
// /*AFLA*/ 	int				nr_freq;
// /*AFLA*/ 	int				rotate_disable;
// /*AFLA*/ 	atomic_t			refcount;
// /*AFLA*/ 	struct task_struct		*task;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Context clock, runs when context enabled.
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64				time;
// /*AFLA*/ 	u64				timestamp;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * These fields let us detect when two contexts have both
// /*AFLA*/ 	 * been cloned (inherited) from a common ancestor.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct perf_event_context	*parent_ctx;
// /*AFLA*/ 	u64				parent_gen;
// /*AFLA*/ 	u64				generation;
// /*AFLA*/ 	int				pin_count;
// /*AFLA*/ #ifdef CONFIG_CGROUP_PERF
// /*AFLA*/ 	int				nr_cgroups;	 /* cgroup evts */
// /*AFLA*/ #endif
// /*AFLA*/ 	void				*task_ctx_data; /* pmu specific data */
// /*AFLA*/ 	struct rcu_head			rcu_head;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Number of contexts where an event can trigger:
// /*AFLA*/  *	task, softirq, hardirq, nmi.
// /*AFLA*/  */
// /*AFLA*/ #define PERF_NR_CONTEXTS	4
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct perf_event_cpu_context - per cpu event context structure
// /*AFLA*/  */
// /*AFLA*/ struct perf_cpu_context {
// /*AFLA*/ 	struct perf_event_context	ctx;
// /*AFLA*/ 	struct perf_event_context	*task_ctx;
// /*AFLA*/ 	int				active_oncpu;
// /*AFLA*/ 	int				exclusive;
// /*AFLA*/ 
// /*AFLA*/ 	raw_spinlock_t			hrtimer_lock;
// /*AFLA*/ 	struct hrtimer			hrtimer;
// /*AFLA*/ 	ktime_t				hrtimer_interval;
// /*AFLA*/ 	unsigned int			hrtimer_active;
// /*AFLA*/ 
// /*AFLA*/ 	struct pmu			*unique_pmu;
// /*AFLA*/ #ifdef CONFIG_CGROUP_PERF
// /*AFLA*/ 	struct perf_cgroup		*cgrp;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	struct list_head		sched_cb_entry;
// /*AFLA*/ 	int				sched_cb_usage;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct perf_output_handle {
// /*AFLA*/ 	struct perf_event		*event;
// /*AFLA*/ 	struct ring_buffer		*rb;
// /*AFLA*/ 	unsigned long			wakeup;
// /*AFLA*/ 	unsigned long			size;
// /*AFLA*/ 	union {
// /*AFLA*/ 		void			*addr;
// /*AFLA*/ 		unsigned long		head;
// /*AFLA*/ 	};
// /*AFLA*/ 	int				page;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct bpf_perf_event_data_kern {
// /*AFLA*/ 	struct pt_regs *regs;
// /*AFLA*/ 	struct perf_sample_data *data;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_CGROUP_PERF
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * perf_cgroup_info keeps track of time_enabled for a cgroup.
// /*AFLA*/  * This is a per-cpu dynamically allocated data structure.
// /*AFLA*/  */
// /*AFLA*/ struct perf_cgroup_info {
// /*AFLA*/ 	u64				time;
// /*AFLA*/ 	u64				timestamp;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct perf_cgroup {
// /*AFLA*/ 	struct cgroup_subsys_state	css;
// /*AFLA*/ 	struct perf_cgroup_info	__percpu *info;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Must ensure cgroup is pinned (css_get) before calling
// /*AFLA*/  * this function. In other words, we cannot call this function
// /*AFLA*/  * if there is no cgroup event for the current CPU context.
// /*AFLA*/  */
// /*AFLA*/ static inline struct perf_cgroup *
// /*AFLA*/ perf_cgroup_from_task(struct task_struct *task, struct perf_event_context *ctx)
// /*AFLA*/ {
// /*AFLA*/ 	return container_of(task_css_check(task, perf_event_cgrp_id,
// /*AFLA*/ 					   ctx ? lockdep_is_held(&ctx->lock)
// /*AFLA*/ 					       : true),
// /*AFLA*/ 			    struct perf_cgroup, css);
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_CGROUP_PERF */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PERF_EVENTS
// /*AFLA*/ 
// /*AFLA*/ extern void *perf_aux_output_begin(struct perf_output_handle *handle,
// /*AFLA*/ 				   struct perf_event *event);
// /*AFLA*/ extern void perf_aux_output_end(struct perf_output_handle *handle,
// /*AFLA*/ 				unsigned long size, bool truncated);
// /*AFLA*/ extern int perf_aux_output_skip(struct perf_output_handle *handle,
// /*AFLA*/ 				unsigned long size);
// /*AFLA*/ extern void *perf_get_aux(struct perf_output_handle *handle);
// /*AFLA*/ 
// /*AFLA*/ extern int perf_pmu_register(struct pmu *pmu, const char *name, int type);
// /*AFLA*/ extern void perf_pmu_unregister(struct pmu *pmu);
// /*AFLA*/ 
// /*AFLA*/ extern int perf_num_counters(void);
// /*AFLA*/ extern const char *perf_pmu_name(void);
// /*AFLA*/ extern void __perf_event_task_sched_in(struct task_struct *prev,
// /*AFLA*/ 				       struct task_struct *task);
// /*AFLA*/ extern void __perf_event_task_sched_out(struct task_struct *prev,
// /*AFLA*/ 					struct task_struct *next);
// /*AFLA*/ extern int perf_event_init_task(struct task_struct *child);
// /*AFLA*/ extern void perf_event_exit_task(struct task_struct *child);
// /*AFLA*/ extern void perf_event_free_task(struct task_struct *task);
// /*AFLA*/ extern void perf_event_delayed_put(struct task_struct *task);
// /*AFLA*/ extern struct file *perf_event_get(unsigned int fd);
// /*AFLA*/ extern const struct perf_event_attr *perf_event_attrs(struct perf_event *event);
// /*AFLA*/ extern void perf_event_print_debug(void);
// /*AFLA*/ extern void perf_pmu_disable(struct pmu *pmu);
// /*AFLA*/ extern void perf_pmu_enable(struct pmu *pmu);
// /*AFLA*/ extern void perf_sched_cb_dec(struct pmu *pmu);
// /*AFLA*/ extern void perf_sched_cb_inc(struct pmu *pmu);
// /*AFLA*/ extern int perf_event_task_disable(void);
// /*AFLA*/ extern int perf_event_task_enable(void);
// /*AFLA*/ extern int perf_event_refresh(struct perf_event *event, int refresh);
// /*AFLA*/ extern void perf_event_update_userpage(struct perf_event *event);
// /*AFLA*/ extern int perf_event_release_kernel(struct perf_event *event);
// /*AFLA*/ extern struct perf_event *
// /*AFLA*/ perf_event_create_kernel_counter(struct perf_event_attr *attr,
// /*AFLA*/ 				int cpu,
// /*AFLA*/ 				struct task_struct *task,
// /*AFLA*/ 				perf_overflow_handler_t callback,
// /*AFLA*/ 				void *context);
// /*AFLA*/ extern void perf_pmu_migrate_context(struct pmu *pmu,
// /*AFLA*/ 				int src_cpu, int dst_cpu);
// /*AFLA*/ extern u64 perf_event_read_local(struct perf_event *event);
// /*AFLA*/ extern u64 perf_event_read_value(struct perf_event *event,
// /*AFLA*/ 				 u64 *enabled, u64 *running);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ struct perf_sample_data {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Fields set by perf_sample_data_init(), group so as to
// /*AFLA*/ 	 * minimize the cachelines touched.
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64				addr;
// /*AFLA*/ 	struct perf_raw_record		*raw;
// /*AFLA*/ 	struct perf_branch_stack	*br_stack;
// /*AFLA*/ 	u64				period;
// /*AFLA*/ 	u64				weight;
// /*AFLA*/ 	u64				txn;
// /*AFLA*/ 	union  perf_mem_data_src	data_src;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The other fields, optionally {set,used} by
// /*AFLA*/ 	 * perf_{prepare,output}_sample().
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64				type;
// /*AFLA*/ 	u64				ip;
// /*AFLA*/ 	struct {
// /*AFLA*/ 		u32	pid;
// /*AFLA*/ 		u32	tid;
// /*AFLA*/ 	}				tid_entry;
// /*AFLA*/ 	u64				time;
// /*AFLA*/ 	u64				id;
// /*AFLA*/ 	u64				stream_id;
// /*AFLA*/ 	struct {
// /*AFLA*/ 		u32	cpu;
// /*AFLA*/ 		u32	reserved;
// /*AFLA*/ 	}				cpu_entry;
// /*AFLA*/ 	struct perf_callchain_entry	*callchain;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * regs_user may point to task_pt_regs or to regs_user_copy, depending
// /*AFLA*/ 	 * on arch details.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct perf_regs		regs_user;
// /*AFLA*/ 	struct pt_regs			regs_user_copy;
// /*AFLA*/ 
// /*AFLA*/ 	struct perf_regs		regs_intr;
// /*AFLA*/ 	u64				stack_user_size;
// /*AFLA*/ } ____cacheline_aligned;
// /*AFLA*/ 
// /*AFLA*/ /* default value for data source */
// /*AFLA*/ #define PERF_MEM_NA (PERF_MEM_S(OP, NA)   |\
// /*AFLA*/ 		    PERF_MEM_S(LVL, NA)   |\
// /*AFLA*/ 		    PERF_MEM_S(SNOOP, NA) |\
// /*AFLA*/ 		    PERF_MEM_S(LOCK, NA)  |\
// /*AFLA*/ 		    PERF_MEM_S(TLB, NA))
// /*AFLA*/ 
// /*AFLA*/ static inline void perf_sample_data_init(struct perf_sample_data *data,
// /*AFLA*/ 					 u64 addr, u64 period)
// /*AFLA*/ {
// /*AFLA*/ 	/* remaining struct members initialized in perf_prepare_sample() */
// /*AFLA*/ 	data->addr = addr;
// /*AFLA*/ 	data->raw  = NULL;
// /*AFLA*/ 	data->br_stack = NULL;
// /*AFLA*/ 	data->period = period;
// /*AFLA*/ 	data->weight = 0;
// /*AFLA*/ 	data->data_src.val = PERF_MEM_NA;
// /*AFLA*/ 	data->txn = 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void perf_output_sample(struct perf_output_handle *handle,
// /*AFLA*/ 			       struct perf_event_header *header,
// /*AFLA*/ 			       struct perf_sample_data *data,
// /*AFLA*/ 			       struct perf_event *event);
// /*AFLA*/ extern void perf_prepare_sample(struct perf_event_header *header,
// /*AFLA*/ 				struct perf_sample_data *data,
// /*AFLA*/ 				struct perf_event *event,
// /*AFLA*/ 				struct pt_regs *regs);
// /*AFLA*/ 
// /*AFLA*/ extern int perf_event_overflow(struct perf_event *event,
// /*AFLA*/ 				 struct perf_sample_data *data,
// /*AFLA*/ 				 struct pt_regs *regs);
// /*AFLA*/ 
// /*AFLA*/ extern void perf_event_output_forward(struct perf_event *event,
// /*AFLA*/ 				     struct perf_sample_data *data,
// /*AFLA*/ 				     struct pt_regs *regs);
// /*AFLA*/ extern void perf_event_output_backward(struct perf_event *event,
// /*AFLA*/ 				       struct perf_sample_data *data,
// /*AFLA*/ 				       struct pt_regs *regs);
// /*AFLA*/ extern void perf_event_output(struct perf_event *event,
// /*AFLA*/ 			      struct perf_sample_data *data,
// /*AFLA*/ 			      struct pt_regs *regs);
// /*AFLA*/ 
// /*AFLA*/ static inline bool
// /*AFLA*/ is_default_overflow_handler(struct perf_event *event)
// /*AFLA*/ {
// /*AFLA*/ 	if (likely(event->overflow_handler == perf_event_output_forward))
// /*AFLA*/ 		return true;
// /*AFLA*/ 	if (unlikely(event->overflow_handler == perf_event_output_backward))
// /*AFLA*/ 		return true;
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void
// /*AFLA*/ perf_event_header__init_id(struct perf_event_header *header,
// /*AFLA*/ 			   struct perf_sample_data *data,
// /*AFLA*/ 			   struct perf_event *event);
// /*AFLA*/ extern void
// /*AFLA*/ perf_event__output_id_sample(struct perf_event *event,
// /*AFLA*/ 			     struct perf_output_handle *handle,
// /*AFLA*/ 			     struct perf_sample_data *sample);
// /*AFLA*/ 
// /*AFLA*/ extern void
// /*AFLA*/ perf_log_lost_samples(struct perf_event *event, u64 lost);
// /*AFLA*/ 
// /*AFLA*/ static inline bool is_sampling_event(struct perf_event *event)
// /*AFLA*/ {
// /*AFLA*/ 	return event->attr.sample_period != 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Return 1 for a software event, 0 for a hardware event
// /*AFLA*/  */
// /*AFLA*/ static inline int is_software_event(struct perf_event *event)
// /*AFLA*/ {
// /*AFLA*/ 	return event->event_caps & PERF_EV_CAP_SOFTWARE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct static_key perf_swevent_enabled[PERF_COUNT_SW_MAX];
// /*AFLA*/ 
// /*AFLA*/ extern void ___perf_sw_event(u32, u64, struct pt_regs *, u64);
// /*AFLA*/ extern void __perf_sw_event(u32, u64, struct pt_regs *, u64);
// /*AFLA*/ 
// /*AFLA*/ #ifndef perf_arch_fetch_caller_regs
// /*AFLA*/ static inline void perf_arch_fetch_caller_regs(struct pt_regs *regs, unsigned long ip) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Take a snapshot of the regs. Skip ip and frame pointer to
// /*AFLA*/  * the nth caller. We only need a few of the regs:
// /*AFLA*/  * - ip for PERF_SAMPLE_IP
// /*AFLA*/  * - cs for user_mode() tests
// /*AFLA*/  * - bp for callchains
// /*AFLA*/  * - eflags, for future purposes, just in case
// /*AFLA*/  */
// /*AFLA*/ static inline void perf_fetch_caller_regs(struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ 	perf_arch_fetch_caller_regs(regs, CALLER_ADDR0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void
// /*AFLA*/ perf_sw_event(u32 event_id, u64 nr, struct pt_regs *regs, u64 addr)
// /*AFLA*/ {
// /*AFLA*/ 	if (static_key_false(&perf_swevent_enabled[event_id]))
// /*AFLA*/ 		__perf_sw_event(event_id, nr, regs, addr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ DECLARE_PER_CPU(struct pt_regs, __perf_regs[4]);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * 'Special' version for the scheduler, it hard assumes no recursion,
// /*AFLA*/  * which is guaranteed by us not actually scheduling inside other swevents
// /*AFLA*/  * because those disable preemption.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void
// /*AFLA*/ perf_sw_event_sched(u32 event_id, u64 nr, u64 addr)
// /*AFLA*/ {
// /*AFLA*/ 	if (static_key_false(&perf_swevent_enabled[event_id])) {
// /*AFLA*/ 		struct pt_regs *regs = this_cpu_ptr(&__perf_regs[0]);
// /*AFLA*/ 
// /*AFLA*/ 		perf_fetch_caller_regs(regs);
// /*AFLA*/ 		___perf_sw_event(event_id, nr, regs, addr);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct static_key_false perf_sched_events;
// /*AFLA*/ 
// /*AFLA*/ static __always_inline bool
// /*AFLA*/ perf_sw_migrate_enabled(void)
// /*AFLA*/ {
// /*AFLA*/ 	if (static_key_false(&perf_swevent_enabled[PERF_COUNT_SW_CPU_MIGRATIONS]))
// /*AFLA*/ 		return true;
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void perf_event_task_migrate(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	if (perf_sw_migrate_enabled())
// /*AFLA*/ 		task->sched_migrated = 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void perf_event_task_sched_in(struct task_struct *prev,
// /*AFLA*/ 					    struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	if (static_branch_unlikely(&perf_sched_events))
// /*AFLA*/ 		__perf_event_task_sched_in(prev, task);
// /*AFLA*/ 
// /*AFLA*/ 	if (perf_sw_migrate_enabled() && task->sched_migrated) {
// /*AFLA*/ 		struct pt_regs *regs = this_cpu_ptr(&__perf_regs[0]);
// /*AFLA*/ 
// /*AFLA*/ 		perf_fetch_caller_regs(regs);
// /*AFLA*/ 		___perf_sw_event(PERF_COUNT_SW_CPU_MIGRATIONS, 1, regs, 0);
// /*AFLA*/ 		task->sched_migrated = 0;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void perf_event_task_sched_out(struct task_struct *prev,
// /*AFLA*/ 					     struct task_struct *next)
// /*AFLA*/ {
// /*AFLA*/ 	perf_sw_event_sched(PERF_COUNT_SW_CONTEXT_SWITCHES, 1, 0);
// /*AFLA*/ 
// /*AFLA*/ 	if (static_branch_unlikely(&perf_sched_events))
// /*AFLA*/ 		__perf_event_task_sched_out(prev, next);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u64 __perf_event_count(struct perf_event *event)
// /*AFLA*/ {
// /*AFLA*/ 	return local64_read(&event->count) + atomic64_read(&event->child_count);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void perf_event_mmap(struct vm_area_struct *vma);
// /*AFLA*/ extern struct perf_guest_info_callbacks *perf_guest_cbs;
// /*AFLA*/ extern int perf_register_guest_info_callbacks(struct perf_guest_info_callbacks *callbacks);
// /*AFLA*/ extern int perf_unregister_guest_info_callbacks(struct perf_guest_info_callbacks *callbacks);
// /*AFLA*/ 
// /*AFLA*/ extern void perf_event_exec(void);
// /*AFLA*/ extern void perf_event_comm(struct task_struct *tsk, bool exec);
// /*AFLA*/ extern void perf_event_fork(struct task_struct *tsk);
// /*AFLA*/ 
// /*AFLA*/ /* Callchains */
// /*AFLA*/ DECLARE_PER_CPU(struct perf_callchain_entry, perf_callchain_entry);
// /*AFLA*/ 
// /*AFLA*/ extern void perf_callchain_user(struct perf_callchain_entry_ctx *entry, struct pt_regs *regs);
// /*AFLA*/ extern void perf_callchain_kernel(struct perf_callchain_entry_ctx *entry, struct pt_regs *regs);
// /*AFLA*/ extern struct perf_callchain_entry *
// /*AFLA*/ get_perf_callchain(struct pt_regs *regs, u32 init_nr, bool kernel, bool user,
// /*AFLA*/ 		   u32 max_stack, bool crosstask, bool add_mark);
// /*AFLA*/ extern int get_callchain_buffers(int max_stack);
// /*AFLA*/ extern void put_callchain_buffers(void);
// /*AFLA*/ 
// /*AFLA*/ extern int sysctl_perf_event_max_stack;
// /*AFLA*/ extern int sysctl_perf_event_max_contexts_per_stack;
// /*AFLA*/ 
// /*AFLA*/ static inline int perf_callchain_store_context(struct perf_callchain_entry_ctx *ctx, u64 ip)
// /*AFLA*/ {
// /*AFLA*/ 	if (ctx->contexts < sysctl_perf_event_max_contexts_per_stack) {
// /*AFLA*/ 		struct perf_callchain_entry *entry = ctx->entry;
// /*AFLA*/ 		entry->ip[entry->nr++] = ip;
// /*AFLA*/ 		++ctx->contexts;
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	} else {
// /*AFLA*/ 		ctx->contexts_maxed = true;
// /*AFLA*/ 		return -1; /* no more room, stop walking the stack */
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int perf_callchain_store(struct perf_callchain_entry_ctx *ctx, u64 ip)
// /*AFLA*/ {
// /*AFLA*/ 	if (ctx->nr < ctx->max_stack && !ctx->contexts_maxed) {
// /*AFLA*/ 		struct perf_callchain_entry *entry = ctx->entry;
// /*AFLA*/ 		entry->ip[entry->nr++] = ip;
// /*AFLA*/ 		++ctx->nr;
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	} else {
// /*AFLA*/ 		return -1; /* no more room, stop walking the stack */
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int sysctl_perf_event_paranoid;
// /*AFLA*/ extern int sysctl_perf_event_mlock;
// /*AFLA*/ extern int sysctl_perf_event_sample_rate;
// /*AFLA*/ extern int sysctl_perf_cpu_time_max_percent;
// /*AFLA*/ 
// /*AFLA*/ extern void perf_sample_event_took(u64 sample_len_ns);
// /*AFLA*/ 
// /*AFLA*/ extern int perf_proc_update_handler(struct ctl_table *table, int write,
// /*AFLA*/ 		void __user *buffer, size_t *lenp,
// /*AFLA*/ 		loff_t *ppos);
// /*AFLA*/ extern int perf_cpu_time_max_percent_handler(struct ctl_table *table, int write,
// /*AFLA*/ 		void __user *buffer, size_t *lenp,
// /*AFLA*/ 		loff_t *ppos);
// /*AFLA*/ 
// /*AFLA*/ int perf_event_max_stack_handler(struct ctl_table *table, int write,
// /*AFLA*/ 				 void __user *buffer, size_t *lenp, loff_t *ppos);
// /*AFLA*/ 
// /*AFLA*/ static inline bool perf_paranoid_tracepoint_raw(void)
// /*AFLA*/ {
// /*AFLA*/ 	return sysctl_perf_event_paranoid > -1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool perf_paranoid_cpu(void)
// /*AFLA*/ {
// /*AFLA*/ 	return sysctl_perf_event_paranoid > 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool perf_paranoid_kernel(void)
// /*AFLA*/ {
// /*AFLA*/ 	return sysctl_perf_event_paranoid > 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void perf_event_init(void);
// /*AFLA*/ extern void perf_tp_event(u16 event_type, u64 count, void *record,
// /*AFLA*/ 			  int entry_size, struct pt_regs *regs,
// /*AFLA*/ 			  struct hlist_head *head, int rctx,
// /*AFLA*/ 			  struct task_struct *task);
// /*AFLA*/ extern void perf_bp_event(struct perf_event *event, void *data);
// /*AFLA*/ 
// /*AFLA*/ #ifndef perf_misc_flags
// /*AFLA*/ # define perf_misc_flags(regs) \
// /*AFLA*/ 		(user_mode(regs) ? PERF_RECORD_MISC_USER : PERF_RECORD_MISC_KERNEL)
// /*AFLA*/ # define perf_instruction_pointer(regs)	instruction_pointer(regs)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline bool has_branch_stack(struct perf_event *event)
// /*AFLA*/ {
// /*AFLA*/ 	return event->attr.sample_type & PERF_SAMPLE_BRANCH_STACK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool needs_branch_stack(struct perf_event *event)
// /*AFLA*/ {
// /*AFLA*/ 	return event->attr.branch_sample_type != 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool has_aux(struct perf_event *event)
// /*AFLA*/ {
// /*AFLA*/ 	return event->pmu->setup_aux;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool is_write_backward(struct perf_event *event)
// /*AFLA*/ {
// /*AFLA*/ 	return !!event->attr.write_backward;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool has_addr_filter(struct perf_event *event)
// /*AFLA*/ {
// /*AFLA*/ 	return event->pmu->nr_addr_filters;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * An inherited event uses parent's filters
// /*AFLA*/  */
// /*AFLA*/ static inline struct perf_addr_filters_head *
// /*AFLA*/ perf_event_addr_filters(struct perf_event *event)
// /*AFLA*/ {
// /*AFLA*/ 	struct perf_addr_filters_head *ifh = &event->addr_filters;
// /*AFLA*/ 
// /*AFLA*/ 	if (event->parent)
// /*AFLA*/ 		ifh = &event->parent->addr_filters;
// /*AFLA*/ 
// /*AFLA*/ 	return ifh;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void perf_event_addr_filters_sync(struct perf_event *event);
// /*AFLA*/ 
// /*AFLA*/ extern int perf_output_begin(struct perf_output_handle *handle,
// /*AFLA*/ 			     struct perf_event *event, unsigned int size);
// /*AFLA*/ extern int perf_output_begin_forward(struct perf_output_handle *handle,
// /*AFLA*/ 				    struct perf_event *event,
// /*AFLA*/ 				    unsigned int size);
// /*AFLA*/ extern int perf_output_begin_backward(struct perf_output_handle *handle,
// /*AFLA*/ 				      struct perf_event *event,
// /*AFLA*/ 				      unsigned int size);
// /*AFLA*/ 
// /*AFLA*/ extern void perf_output_end(struct perf_output_handle *handle);
// /*AFLA*/ extern unsigned int perf_output_copy(struct perf_output_handle *handle,
// /*AFLA*/ 			     const void *buf, unsigned int len);
// /*AFLA*/ extern unsigned int perf_output_skip(struct perf_output_handle *handle,
// /*AFLA*/ 				     unsigned int len);
// /*AFLA*/ extern int perf_swevent_get_recursion_context(void);
// /*AFLA*/ extern void perf_swevent_put_recursion_context(int rctx);
// /*AFLA*/ extern u64 perf_swevent_set_period(struct perf_event *event);
// /*AFLA*/ extern void perf_event_enable(struct perf_event *event);
// /*AFLA*/ extern void perf_event_disable(struct perf_event *event);
// /*AFLA*/ extern void perf_event_disable_local(struct perf_event *event);
// /*AFLA*/ extern void perf_event_disable_inatomic(struct perf_event *event);
// /*AFLA*/ extern void perf_event_task_tick(void);
// /*AFLA*/ #else /* !CONFIG_PERF_EVENTS: */
// /*AFLA*/ static inline void *
// /*AFLA*/ perf_aux_output_begin(struct perf_output_handle *handle,
// /*AFLA*/ 		      struct perf_event *event)				{ return NULL; }
// /*AFLA*/ static inline void
// /*AFLA*/ perf_aux_output_end(struct perf_output_handle *handle, unsigned long size,
// /*AFLA*/ 		    bool truncated)					{ }
// /*AFLA*/ static inline int
// /*AFLA*/ perf_aux_output_skip(struct perf_output_handle *handle,
// /*AFLA*/ 		     unsigned long size)				{ return -EINVAL; }
// /*AFLA*/ static inline void *
// /*AFLA*/ perf_get_aux(struct perf_output_handle *handle)				{ return NULL; }
// /*AFLA*/ static inline void
// /*AFLA*/ perf_event_task_migrate(struct task_struct *task)			{ }
// /*AFLA*/ static inline void
// /*AFLA*/ perf_event_task_sched_in(struct task_struct *prev,
// /*AFLA*/ 			 struct task_struct *task)			{ }
// /*AFLA*/ static inline void
// /*AFLA*/ perf_event_task_sched_out(struct task_struct *prev,
// /*AFLA*/ 			  struct task_struct *next)			{ }
// /*AFLA*/ static inline int perf_event_init_task(struct task_struct *child)	{ return 0; }
// /*AFLA*/ static inline void perf_event_exit_task(struct task_struct *child)	{ }
// /*AFLA*/ static inline void perf_event_free_task(struct task_struct *task)	{ }
// /*AFLA*/ static inline void perf_event_delayed_put(struct task_struct *task)	{ }
// /*AFLA*/ static inline struct file *perf_event_get(unsigned int fd)	{ return ERR_PTR(-EINVAL); }
// /*AFLA*/ static inline const struct perf_event_attr *perf_event_attrs(struct perf_event *event)
// /*AFLA*/ {
// /*AFLA*/ 	return ERR_PTR(-EINVAL);
// /*AFLA*/ }
// /*AFLA*/ static inline u64 perf_event_read_local(struct perf_event *event)	{ return -EINVAL; }
// /*AFLA*/ static inline void perf_event_print_debug(void)				{ }
// /*AFLA*/ static inline int perf_event_task_disable(void)				{ return -EINVAL; }
// /*AFLA*/ static inline int perf_event_task_enable(void)				{ return -EINVAL; }
// /*AFLA*/ static inline int perf_event_refresh(struct perf_event *event, int refresh)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ perf_sw_event(u32 event_id, u64 nr, struct pt_regs *regs, u64 addr)	{ }
// /*AFLA*/ static inline void
// /*AFLA*/ perf_sw_event_sched(u32 event_id, u64 nr, u64 addr)			{ }
// /*AFLA*/ static inline void
// /*AFLA*/ perf_bp_event(struct perf_event *event, void *data)			{ }
// /*AFLA*/ 
// /*AFLA*/ static inline int perf_register_guest_info_callbacks
// /*AFLA*/ (struct perf_guest_info_callbacks *callbacks)				{ return 0; }
// /*AFLA*/ static inline int perf_unregister_guest_info_callbacks
// /*AFLA*/ (struct perf_guest_info_callbacks *callbacks)				{ return 0; }
// /*AFLA*/ 
// /*AFLA*/ static inline void perf_event_mmap(struct vm_area_struct *vma)		{ }
// /*AFLA*/ static inline void perf_event_exec(void)				{ }
// /*AFLA*/ static inline void perf_event_comm(struct task_struct *tsk, bool exec)	{ }
// /*AFLA*/ static inline void perf_event_fork(struct task_struct *tsk)		{ }
// /*AFLA*/ static inline void perf_event_init(void)				{ }
// /*AFLA*/ static inline int  perf_swevent_get_recursion_context(void)		{ return -1; }
// /*AFLA*/ static inline void perf_swevent_put_recursion_context(int rctx)		{ }
// /*AFLA*/ static inline u64 perf_swevent_set_period(struct perf_event *event)	{ return 0; }
// /*AFLA*/ static inline void perf_event_enable(struct perf_event *event)		{ }
// /*AFLA*/ static inline void perf_event_disable(struct perf_event *event)		{ }
// /*AFLA*/ static inline int __perf_event_disable(void *info)			{ return -1; }
// /*AFLA*/ static inline void perf_event_task_tick(void)				{ }
// /*AFLA*/ static inline int perf_event_release_kernel(struct perf_event *event)	{ return 0; }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_PERF_EVENTS) && defined(CONFIG_CPU_SUP_INTEL)
// /*AFLA*/ extern void perf_restore_debug_store(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void perf_restore_debug_store(void)			{ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static __always_inline bool perf_raw_frag_last(const struct perf_raw_frag *frag)
// /*AFLA*/ {
// /*AFLA*/ 	return frag->pad < sizeof(u64);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define perf_output_put(handle, x) perf_output_copy((handle), &(x), sizeof(x))
// /*AFLA*/ 
// /*AFLA*/ struct perf_pmu_events_attr {
// /*AFLA*/ 	struct device_attribute attr;
// /*AFLA*/ 	u64 id;
// /*AFLA*/ 	const char *event_str;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct perf_pmu_events_ht_attr {
// /*AFLA*/ 	struct device_attribute			attr;
// /*AFLA*/ 	u64					id;
// /*AFLA*/ 	const char				*event_str_ht;
// /*AFLA*/ 	const char				*event_str_noht;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ ssize_t perf_event_sysfs_show(struct device *dev, struct device_attribute *attr,
// /*AFLA*/ 			      char *page);
// /*AFLA*/ 
// /*AFLA*/ #define PMU_EVENT_ATTR(_name, _var, _id, _show)				\
// /*AFLA*/ static struct perf_pmu_events_attr _var = {				\
// /*AFLA*/ 	.attr = __ATTR(_name, 0444, _show, NULL),			\
// /*AFLA*/ 	.id   =  _id,							\
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define PMU_EVENT_ATTR_STRING(_name, _var, _str)			    \
// /*AFLA*/ static struct perf_pmu_events_attr _var = {				    \
// /*AFLA*/ 	.attr		= __ATTR(_name, 0444, perf_event_sysfs_show, NULL), \
// /*AFLA*/ 	.id		= 0,						    \
// /*AFLA*/ 	.event_str	= _str,						    \
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define PMU_FORMAT_ATTR(_name, _format)					\
// /*AFLA*/ static ssize_t								\
// /*AFLA*/ _name##_show(struct device *dev,					\
// /*AFLA*/ 			       struct device_attribute *attr,		\
// /*AFLA*/ 			       char *page)				\
// /*AFLA*/ {									\
// /*AFLA*/ 	BUILD_BUG_ON(sizeof(_format) >= PAGE_SIZE);			\
// /*AFLA*/ 	return sprintf(page, _format "\n");				\
// /*AFLA*/ }									\
// /*AFLA*/ 									\
// /*AFLA*/ static struct device_attribute format_attr_##_name = __ATTR_RO(_name)
// /*AFLA*/ 
// /*AFLA*/ /* Performance counter hotplug functions */
// /*AFLA*/ #ifdef CONFIG_PERF_EVENTS
// /*AFLA*/ int perf_event_init_cpu(unsigned int cpu);
// /*AFLA*/ int perf_event_exit_cpu(unsigned int cpu);
// /*AFLA*/ #else
// /*AFLA*/ #define perf_event_init_cpu	NULL
// /*AFLA*/ #define perf_event_exit_cpu	NULL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_PERF_EVENT_H */
