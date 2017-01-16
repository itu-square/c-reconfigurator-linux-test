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
// /*AFLA*/ #ifndef _UAPI_LINUX_PERF_EVENT_H
// /*AFLA*/ #define _UAPI_LINUX_PERF_EVENT_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/ioctl.h>
// /*AFLA*/ #include <asm/byteorder.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * User-space ABI bits:
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * attr.type
// /*AFLA*/  */
// /*AFLA*/ enum perf_type_id {
// /*AFLA*/ 	PERF_TYPE_HARDWARE			= 0,
// /*AFLA*/ 	PERF_TYPE_SOFTWARE			= 1,
// /*AFLA*/ 	PERF_TYPE_TRACEPOINT			= 2,
// /*AFLA*/ 	PERF_TYPE_HW_CACHE			= 3,
// /*AFLA*/ 	PERF_TYPE_RAW				= 4,
// /*AFLA*/ 	PERF_TYPE_BREAKPOINT			= 5,
// /*AFLA*/ 
// /*AFLA*/ 	PERF_TYPE_MAX,				/* non-ABI */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Generalized performance event event_id types, used by the
// /*AFLA*/  * attr.event_id parameter of the sys_perf_event_open()
// /*AFLA*/  * syscall:
// /*AFLA*/  */
// /*AFLA*/ enum perf_hw_id {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Common hardware events, generalized by the kernel:
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERF_COUNT_HW_CPU_CYCLES		= 0,
// /*AFLA*/ 	PERF_COUNT_HW_INSTRUCTIONS		= 1,
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_REFERENCES		= 2,
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_MISSES		= 3,
// /*AFLA*/ 	PERF_COUNT_HW_BRANCH_INSTRUCTIONS	= 4,
// /*AFLA*/ 	PERF_COUNT_HW_BRANCH_MISSES		= 5,
// /*AFLA*/ 	PERF_COUNT_HW_BUS_CYCLES		= 6,
// /*AFLA*/ 	PERF_COUNT_HW_STALLED_CYCLES_FRONTEND	= 7,
// /*AFLA*/ 	PERF_COUNT_HW_STALLED_CYCLES_BACKEND	= 8,
// /*AFLA*/ 	PERF_COUNT_HW_REF_CPU_CYCLES		= 9,
// /*AFLA*/ 
// /*AFLA*/ 	PERF_COUNT_HW_MAX,			/* non-ABI */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Generalized hardware cache events:
// /*AFLA*/  *
// /*AFLA*/  *       { L1-D, L1-I, LLC, ITLB, DTLB, BPU, NODE } x
// /*AFLA*/  *       { read, write, prefetch } x
// /*AFLA*/  *       { accesses, misses }
// /*AFLA*/  */
// /*AFLA*/ enum perf_hw_cache_id {
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_L1D			= 0,
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_L1I			= 1,
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_LL			= 2,
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_DTLB		= 3,
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_ITLB		= 4,
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_BPU			= 5,
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_NODE		= 6,
// /*AFLA*/ 
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_MAX,		/* non-ABI */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum perf_hw_cache_op_id {
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_OP_READ		= 0,
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_OP_WRITE		= 1,
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_OP_PREFETCH		= 2,
// /*AFLA*/ 
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_OP_MAX,		/* non-ABI */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum perf_hw_cache_op_result_id {
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_RESULT_ACCESS	= 0,
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_RESULT_MISS		= 1,
// /*AFLA*/ 
// /*AFLA*/ 	PERF_COUNT_HW_CACHE_RESULT_MAX,		/* non-ABI */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Special "software" events provided by the kernel, even if the hardware
// /*AFLA*/  * does not support performance events. These events measure various
// /*AFLA*/  * physical and sw events of the kernel (and allow the profiling of them as
// /*AFLA*/  * well):
// /*AFLA*/  */
// /*AFLA*/ enum perf_sw_ids {
// /*AFLA*/ 	PERF_COUNT_SW_CPU_CLOCK			= 0,
// /*AFLA*/ 	PERF_COUNT_SW_TASK_CLOCK		= 1,
// /*AFLA*/ 	PERF_COUNT_SW_PAGE_FAULTS		= 2,
// /*AFLA*/ 	PERF_COUNT_SW_CONTEXT_SWITCHES		= 3,
// /*AFLA*/ 	PERF_COUNT_SW_CPU_MIGRATIONS		= 4,
// /*AFLA*/ 	PERF_COUNT_SW_PAGE_FAULTS_MIN		= 5,
// /*AFLA*/ 	PERF_COUNT_SW_PAGE_FAULTS_MAJ		= 6,
// /*AFLA*/ 	PERF_COUNT_SW_ALIGNMENT_FAULTS		= 7,
// /*AFLA*/ 	PERF_COUNT_SW_EMULATION_FAULTS		= 8,
// /*AFLA*/ 	PERF_COUNT_SW_DUMMY			= 9,
// /*AFLA*/ 	PERF_COUNT_SW_BPF_OUTPUT		= 10,
// /*AFLA*/ 
// /*AFLA*/ 	PERF_COUNT_SW_MAX,			/* non-ABI */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Bits that can be set in attr.sample_type to request information
// /*AFLA*/  * in the overflow packets.
// /*AFLA*/  */
// /*AFLA*/ enum perf_event_sample_format {
// /*AFLA*/ 	PERF_SAMPLE_IP				= 1U << 0,
// /*AFLA*/ 	PERF_SAMPLE_TID				= 1U << 1,
// /*AFLA*/ 	PERF_SAMPLE_TIME			= 1U << 2,
// /*AFLA*/ 	PERF_SAMPLE_ADDR			= 1U << 3,
// /*AFLA*/ 	PERF_SAMPLE_READ			= 1U << 4,
// /*AFLA*/ 	PERF_SAMPLE_CALLCHAIN			= 1U << 5,
// /*AFLA*/ 	PERF_SAMPLE_ID				= 1U << 6,
// /*AFLA*/ 	PERF_SAMPLE_CPU				= 1U << 7,
// /*AFLA*/ 	PERF_SAMPLE_PERIOD			= 1U << 8,
// /*AFLA*/ 	PERF_SAMPLE_STREAM_ID			= 1U << 9,
// /*AFLA*/ 	PERF_SAMPLE_RAW				= 1U << 10,
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_STACK		= 1U << 11,
// /*AFLA*/ 	PERF_SAMPLE_REGS_USER			= 1U << 12,
// /*AFLA*/ 	PERF_SAMPLE_STACK_USER			= 1U << 13,
// /*AFLA*/ 	PERF_SAMPLE_WEIGHT			= 1U << 14,
// /*AFLA*/ 	PERF_SAMPLE_DATA_SRC			= 1U << 15,
// /*AFLA*/ 	PERF_SAMPLE_IDENTIFIER			= 1U << 16,
// /*AFLA*/ 	PERF_SAMPLE_TRANSACTION			= 1U << 17,
// /*AFLA*/ 	PERF_SAMPLE_REGS_INTR			= 1U << 18,
// /*AFLA*/ 
// /*AFLA*/ 	PERF_SAMPLE_MAX = 1U << 19,		/* non-ABI */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * values to program into branch_sample_type when PERF_SAMPLE_BRANCH is set
// /*AFLA*/  *
// /*AFLA*/  * If the user does not pass priv level information via branch_sample_type,
// /*AFLA*/  * the kernel uses the event's priv level. Branch and event priv levels do
// /*AFLA*/  * not have to match. Branch priv level is checked for permissions.
// /*AFLA*/  *
// /*AFLA*/  * The branch types can be combined, however BRANCH_ANY covers all types
// /*AFLA*/  * of branches and therefore it supersedes all the other types.
// /*AFLA*/  */
// /*AFLA*/ enum perf_branch_sample_type_shift {
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_USER_SHIFT		= 0, /* user branches */
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_KERNEL_SHIFT		= 1, /* kernel branches */
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_HV_SHIFT		= 2, /* hypervisor branches */
// /*AFLA*/ 
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_ANY_SHIFT		= 3, /* any branch types */
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_ANY_CALL_SHIFT	= 4, /* any call branch */
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_ANY_RETURN_SHIFT	= 5, /* any return branch */
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_IND_CALL_SHIFT	= 6, /* indirect calls */
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_ABORT_TX_SHIFT	= 7, /* transaction aborts */
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_IN_TX_SHIFT		= 8, /* in transaction */
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_NO_TX_SHIFT		= 9, /* not in transaction */
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_COND_SHIFT		= 10, /* conditional branches */
// /*AFLA*/ 
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_CALL_STACK_SHIFT	= 11, /* call/ret stack */
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_IND_JUMP_SHIFT	= 12, /* indirect jumps */
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_CALL_SHIFT		= 13, /* direct call */
// /*AFLA*/ 
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_NO_FLAGS_SHIFT	= 14, /* no flags */
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_NO_CYCLES_SHIFT	= 15, /* no cycles */
// /*AFLA*/ 
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_MAX_SHIFT		/* non-ABI */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum perf_branch_sample_type {
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_USER		= 1U << PERF_SAMPLE_BRANCH_USER_SHIFT,
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_KERNEL	= 1U << PERF_SAMPLE_BRANCH_KERNEL_SHIFT,
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_HV		= 1U << PERF_SAMPLE_BRANCH_HV_SHIFT,
// /*AFLA*/ 
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_ANY		= 1U << PERF_SAMPLE_BRANCH_ANY_SHIFT,
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_ANY_CALL	= 1U << PERF_SAMPLE_BRANCH_ANY_CALL_SHIFT,
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_ANY_RETURN	= 1U << PERF_SAMPLE_BRANCH_ANY_RETURN_SHIFT,
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_IND_CALL	= 1U << PERF_SAMPLE_BRANCH_IND_CALL_SHIFT,
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_ABORT_TX	= 1U << PERF_SAMPLE_BRANCH_ABORT_TX_SHIFT,
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_IN_TX	= 1U << PERF_SAMPLE_BRANCH_IN_TX_SHIFT,
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_NO_TX	= 1U << PERF_SAMPLE_BRANCH_NO_TX_SHIFT,
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_COND		= 1U << PERF_SAMPLE_BRANCH_COND_SHIFT,
// /*AFLA*/ 
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_CALL_STACK	= 1U << PERF_SAMPLE_BRANCH_CALL_STACK_SHIFT,
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_IND_JUMP	= 1U << PERF_SAMPLE_BRANCH_IND_JUMP_SHIFT,
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_CALL		= 1U << PERF_SAMPLE_BRANCH_CALL_SHIFT,
// /*AFLA*/ 
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_NO_FLAGS	= 1U << PERF_SAMPLE_BRANCH_NO_FLAGS_SHIFT,
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_NO_CYCLES	= 1U << PERF_SAMPLE_BRANCH_NO_CYCLES_SHIFT,
// /*AFLA*/ 
// /*AFLA*/ 	PERF_SAMPLE_BRANCH_MAX		= 1U << PERF_SAMPLE_BRANCH_MAX_SHIFT,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define PERF_SAMPLE_BRANCH_PLM_ALL \
// /*AFLA*/ 	(PERF_SAMPLE_BRANCH_USER|\
// /*AFLA*/ 	 PERF_SAMPLE_BRANCH_KERNEL|\
// /*AFLA*/ 	 PERF_SAMPLE_BRANCH_HV)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Values to determine ABI of the registers dump.
// /*AFLA*/  */
// /*AFLA*/ enum perf_sample_regs_abi {
// /*AFLA*/ 	PERF_SAMPLE_REGS_ABI_NONE	= 0,
// /*AFLA*/ 	PERF_SAMPLE_REGS_ABI_32		= 1,
// /*AFLA*/ 	PERF_SAMPLE_REGS_ABI_64		= 2,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Values for the memory transaction event qualifier, mostly for
// /*AFLA*/  * abort events. Multiple bits can be set.
// /*AFLA*/  */
// /*AFLA*/ enum {
// /*AFLA*/ 	PERF_TXN_ELISION        = (1 << 0), /* From elision */
// /*AFLA*/ 	PERF_TXN_TRANSACTION    = (1 << 1), /* From transaction */
// /*AFLA*/ 	PERF_TXN_SYNC           = (1 << 2), /* Instruction is related */
// /*AFLA*/ 	PERF_TXN_ASYNC          = (1 << 3), /* Instruction not related */
// /*AFLA*/ 	PERF_TXN_RETRY          = (1 << 4), /* Retry possible */
// /*AFLA*/ 	PERF_TXN_CONFLICT       = (1 << 5), /* Conflict abort */
// /*AFLA*/ 	PERF_TXN_CAPACITY_WRITE = (1 << 6), /* Capacity write abort */
// /*AFLA*/ 	PERF_TXN_CAPACITY_READ  = (1 << 7), /* Capacity read abort */
// /*AFLA*/ 
// /*AFLA*/ 	PERF_TXN_MAX	        = (1 << 8), /* non-ABI */
// /*AFLA*/ 
// /*AFLA*/ 	/* bits 32..63 are reserved for the abort code */
// /*AFLA*/ 
// /*AFLA*/ 	PERF_TXN_ABORT_MASK  = (0xffffffffULL << 32),
// /*AFLA*/ 	PERF_TXN_ABORT_SHIFT = 32,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The format of the data returned by read() on a perf event fd,
// /*AFLA*/  * as specified by attr.read_format:
// /*AFLA*/  *
// /*AFLA*/  * struct read_format {
// /*AFLA*/  *	{ u64		value;
// /*AFLA*/  *	  { u64		time_enabled; } && PERF_FORMAT_TOTAL_TIME_ENABLED
// /*AFLA*/  *	  { u64		time_running; } && PERF_FORMAT_TOTAL_TIME_RUNNING
// /*AFLA*/  *	  { u64		id;           } && PERF_FORMAT_ID
// /*AFLA*/  *	} && !PERF_FORMAT_GROUP
// /*AFLA*/  *
// /*AFLA*/  *	{ u64		nr;
// /*AFLA*/  *	  { u64		time_enabled; } && PERF_FORMAT_TOTAL_TIME_ENABLED
// /*AFLA*/  *	  { u64		time_running; } && PERF_FORMAT_TOTAL_TIME_RUNNING
// /*AFLA*/  *	  { u64		value;
// /*AFLA*/  *	    { u64	id;           } && PERF_FORMAT_ID
// /*AFLA*/  *	  }		cntr[nr];
// /*AFLA*/  *	} && PERF_FORMAT_GROUP
// /*AFLA*/  * };
// /*AFLA*/  */
// /*AFLA*/ enum perf_event_read_format {
// /*AFLA*/ 	PERF_FORMAT_TOTAL_TIME_ENABLED		= 1U << 0,
// /*AFLA*/ 	PERF_FORMAT_TOTAL_TIME_RUNNING		= 1U << 1,
// /*AFLA*/ 	PERF_FORMAT_ID				= 1U << 2,
// /*AFLA*/ 	PERF_FORMAT_GROUP			= 1U << 3,
// /*AFLA*/ 
// /*AFLA*/ 	PERF_FORMAT_MAX = 1U << 4,		/* non-ABI */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define PERF_ATTR_SIZE_VER0	64	/* sizeof first published struct */
// /*AFLA*/ #define PERF_ATTR_SIZE_VER1	72	/* add: config2 */
// /*AFLA*/ #define PERF_ATTR_SIZE_VER2	80	/* add: branch_sample_type */
// /*AFLA*/ #define PERF_ATTR_SIZE_VER3	96	/* add: sample_regs_user */
// /*AFLA*/ 					/* add: sample_stack_user */
// /*AFLA*/ #define PERF_ATTR_SIZE_VER4	104	/* add: sample_regs_intr */
// /*AFLA*/ #define PERF_ATTR_SIZE_VER5	112	/* add: aux_watermark */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Hardware event_id to monitor via a performance monitoring event:
// /*AFLA*/  *
// /*AFLA*/  * @sample_max_stack: Max number of frame pointers in a callchain,
// /*AFLA*/  *		      should be < /proc/sys/kernel/perf_event_max_stack
// /*AFLA*/  */
// /*AFLA*/ struct perf_event_attr {
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Major type: hardware/software/tracepoint/etc.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u32			type;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Size of the attr structure, for fwd/bwd compat.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u32			size;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Type specific configuration information.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u64			config;
// /*AFLA*/ 
// /*AFLA*/ 	union {
// /*AFLA*/ 		__u64		sample_period;
// /*AFLA*/ 		__u64		sample_freq;
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ 	__u64			sample_type;
// /*AFLA*/ 	__u64			read_format;
// /*AFLA*/ 
// /*AFLA*/ 	__u64			disabled       :  1, /* off by default        */
// /*AFLA*/ 				inherit	       :  1, /* children inherit it   */
// /*AFLA*/ 				pinned	       :  1, /* must always be on PMU */
// /*AFLA*/ 				exclusive      :  1, /* only group on PMU     */
// /*AFLA*/ 				exclude_user   :  1, /* don't count user      */
// /*AFLA*/ 				exclude_kernel :  1, /* ditto kernel          */
// /*AFLA*/ 				exclude_hv     :  1, /* ditto hypervisor      */
// /*AFLA*/ 				exclude_idle   :  1, /* don't count when idle */
// /*AFLA*/ 				mmap           :  1, /* include mmap data     */
// /*AFLA*/ 				comm	       :  1, /* include comm data     */
// /*AFLA*/ 				freq           :  1, /* use freq, not period  */
// /*AFLA*/ 				inherit_stat   :  1, /* per task counts       */
// /*AFLA*/ 				enable_on_exec :  1, /* next exec enables     */
// /*AFLA*/ 				task           :  1, /* trace fork/exit       */
// /*AFLA*/ 				watermark      :  1, /* wakeup_watermark      */
// /*AFLA*/ 				/*
// /*AFLA*/ 				 * precise_ip:
// /*AFLA*/ 				 *
// /*AFLA*/ 				 *  0 - SAMPLE_IP can have arbitrary skid
// /*AFLA*/ 				 *  1 - SAMPLE_IP must have constant skid
// /*AFLA*/ 				 *  2 - SAMPLE_IP requested to have 0 skid
// /*AFLA*/ 				 *  3 - SAMPLE_IP must have 0 skid
// /*AFLA*/ 				 *
// /*AFLA*/ 				 *  See also PERF_RECORD_MISC_EXACT_IP
// /*AFLA*/ 				 */
// /*AFLA*/ 				precise_ip     :  2, /* skid constraint       */
// /*AFLA*/ 				mmap_data      :  1, /* non-exec mmap data    */
// /*AFLA*/ 				sample_id_all  :  1, /* sample_type all events */
// /*AFLA*/ 
// /*AFLA*/ 				exclude_host   :  1, /* don't count in host   */
// /*AFLA*/ 				exclude_guest  :  1, /* don't count in guest  */
// /*AFLA*/ 
// /*AFLA*/ 				exclude_callchain_kernel : 1, /* exclude kernel callchains */
// /*AFLA*/ 				exclude_callchain_user   : 1, /* exclude user callchains */
// /*AFLA*/ 				mmap2          :  1, /* include mmap with inode data     */
// /*AFLA*/ 				comm_exec      :  1, /* flag comm events that are due to an exec */
// /*AFLA*/ 				use_clockid    :  1, /* use @clockid for time fields */
// /*AFLA*/ 				context_switch :  1, /* context switch data */
// /*AFLA*/ 				write_backward :  1, /* Write ring buffer from end to beginning */
// /*AFLA*/ 				__reserved_1   : 36;
// /*AFLA*/ 
// /*AFLA*/ 	union {
// /*AFLA*/ 		__u32		wakeup_events;	  /* wakeup every n events */
// /*AFLA*/ 		__u32		wakeup_watermark; /* bytes before wakeup   */
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ 	__u32			bp_type;
// /*AFLA*/ 	union {
// /*AFLA*/ 		__u64		bp_addr;
// /*AFLA*/ 		__u64		config1; /* extension of config */
// /*AFLA*/ 	};
// /*AFLA*/ 	union {
// /*AFLA*/ 		__u64		bp_len;
// /*AFLA*/ 		__u64		config2; /* extension of config1 */
// /*AFLA*/ 	};
// /*AFLA*/ 	__u64	branch_sample_type; /* enum perf_branch_sample_type */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Defines set of user regs to dump on samples.
// /*AFLA*/ 	 * See asm/perf_regs.h for details.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u64	sample_regs_user;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Defines size of the user stack to dump on samples.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u32	sample_stack_user;
// /*AFLA*/ 
// /*AFLA*/ 	__s32	clockid;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Defines set of regs to dump for each sample
// /*AFLA*/ 	 * state captured on:
// /*AFLA*/ 	 *  - precise = 0: PMU interrupt
// /*AFLA*/ 	 *  - precise > 0: sampled instruction
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * See asm/perf_regs.h for details.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u64	sample_regs_intr;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Wakeup watermark for AUX area
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u32	aux_watermark;
// /*AFLA*/ 	__u16	sample_max_stack;
// /*AFLA*/ 	__u16	__reserved_2;	/* align to __u64 */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define perf_flags(attr)	(*(&(attr)->read_format + 1))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Ioctls that can be done on a perf event fd:
// /*AFLA*/  */
// /*AFLA*/ #define PERF_EVENT_IOC_ENABLE		_IO ('$', 0)
// /*AFLA*/ #define PERF_EVENT_IOC_DISABLE		_IO ('$', 1)
// /*AFLA*/ #define PERF_EVENT_IOC_REFRESH		_IO ('$', 2)
// /*AFLA*/ #define PERF_EVENT_IOC_RESET		_IO ('$', 3)
// /*AFLA*/ #define PERF_EVENT_IOC_PERIOD		_IOW('$', 4, __u64)
// /*AFLA*/ #define PERF_EVENT_IOC_SET_OUTPUT	_IO ('$', 5)
// /*AFLA*/ #define PERF_EVENT_IOC_SET_FILTER	_IOW('$', 6, char *)
// /*AFLA*/ #define PERF_EVENT_IOC_ID		_IOR('$', 7, __u64 *)
// /*AFLA*/ #define PERF_EVENT_IOC_SET_BPF		_IOW('$', 8, __u32)
// /*AFLA*/ #define PERF_EVENT_IOC_PAUSE_OUTPUT	_IOW('$', 9, __u32)
// /*AFLA*/ 
// /*AFLA*/ enum perf_event_ioc_flags {
// /*AFLA*/ 	PERF_IOC_FLAG_GROUP		= 1U << 0,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Structure of the page that can be mapped via mmap
// /*AFLA*/  */
// /*AFLA*/ struct perf_event_mmap_page {
// /*AFLA*/ 	__u32	version;		/* version number of this structure */
// /*AFLA*/ 	__u32	compat_version;		/* lowest version this is compat with */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Bits needed to read the hw events in user-space.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *   u32 seq, time_mult, time_shift, index, width;
// /*AFLA*/ 	 *   u64 count, enabled, running;
// /*AFLA*/ 	 *   u64 cyc, time_offset;
// /*AFLA*/ 	 *   s64 pmc = 0;
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *   do {
// /*AFLA*/ 	 *     seq = pc->lock;
// /*AFLA*/ 	 *     barrier()
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *     enabled = pc->time_enabled;
// /*AFLA*/ 	 *     running = pc->time_running;
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *     if (pc->cap_usr_time && enabled != running) {
// /*AFLA*/ 	 *       cyc = rdtsc();
// /*AFLA*/ 	 *       time_offset = pc->time_offset;
// /*AFLA*/ 	 *       time_mult   = pc->time_mult;
// /*AFLA*/ 	 *       time_shift  = pc->time_shift;
// /*AFLA*/ 	 *     }
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *     index = pc->index;
// /*AFLA*/ 	 *     count = pc->offset;
// /*AFLA*/ 	 *     if (pc->cap_user_rdpmc && index) {
// /*AFLA*/ 	 *       width = pc->pmc_width;
// /*AFLA*/ 	 *       pmc = rdpmc(index - 1);
// /*AFLA*/ 	 *     }
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *     barrier();
// /*AFLA*/ 	 *   } while (pc->lock != seq);
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * NOTE: for obvious reason this only works on self-monitoring
// /*AFLA*/ 	 *       processes.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u32	lock;			/* seqlock for synchronization */
// /*AFLA*/ 	__u32	index;			/* hardware event identifier */
// /*AFLA*/ 	__s64	offset;			/* add to hardware event value */
// /*AFLA*/ 	__u64	time_enabled;		/* time event active */
// /*AFLA*/ 	__u64	time_running;		/* time event on cpu */
// /*AFLA*/ 	union {
// /*AFLA*/ 		__u64	capabilities;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u64	cap_bit0		: 1, /* Always 0, deprecated, see commit 860f085b74e9 */
// /*AFLA*/ 				cap_bit0_is_deprecated	: 1, /* Always 1, signals that bit 0 is zero */
// /*AFLA*/ 
// /*AFLA*/ 				cap_user_rdpmc		: 1, /* The RDPMC instruction can be used to read counts */
// /*AFLA*/ 				cap_user_time		: 1, /* The time_* fields are used */
// /*AFLA*/ 				cap_user_time_zero	: 1, /* The time_zero field is used */
// /*AFLA*/ 				cap_____res		: 59;
// /*AFLA*/ 		};
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If cap_user_rdpmc this field provides the bit-width of the value
// /*AFLA*/ 	 * read using the rdpmc() or equivalent instruction. This can be used
// /*AFLA*/ 	 * to sign extend the result like:
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *   pmc <<= 64 - width;
// /*AFLA*/ 	 *   pmc >>= 64 - width; // signed shift right
// /*AFLA*/ 	 *   count += pmc;
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u16	pmc_width;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If cap_usr_time the below fields can be used to compute the time
// /*AFLA*/ 	 * delta since time_enabled (in ns) using rdtsc or similar.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *   u64 quot, rem;
// /*AFLA*/ 	 *   u64 delta;
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *   quot = (cyc >> time_shift);
// /*AFLA*/ 	 *   rem = cyc & (((u64)1 << time_shift) - 1);
// /*AFLA*/ 	 *   delta = time_offset + quot * time_mult +
// /*AFLA*/ 	 *              ((rem * time_mult) >> time_shift);
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Where time_offset,time_mult,time_shift and cyc are read in the
// /*AFLA*/ 	 * seqcount loop described above. This delta can then be added to
// /*AFLA*/ 	 * enabled and possible running (if index), improving the scaling:
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *   enabled += delta;
// /*AFLA*/ 	 *   if (index)
// /*AFLA*/ 	 *     running += delta;
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *   quot = count / running;
// /*AFLA*/ 	 *   rem  = count % running;
// /*AFLA*/ 	 *   count = quot * enabled + (rem * enabled) / running;
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u16	time_shift;
// /*AFLA*/ 	__u32	time_mult;
// /*AFLA*/ 	__u64	time_offset;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If cap_usr_time_zero, the hardware clock (e.g. TSC) can be calculated
// /*AFLA*/ 	 * from sample timestamps.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *   time = timestamp - time_zero;
// /*AFLA*/ 	 *   quot = time / time_mult;
// /*AFLA*/ 	 *   rem  = time % time_mult;
// /*AFLA*/ 	 *   cyc = (quot << time_shift) + (rem << time_shift) / time_mult;
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * And vice versa:
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *   quot = cyc >> time_shift;
// /*AFLA*/ 	 *   rem  = cyc & (((u64)1 << time_shift) - 1);
// /*AFLA*/ 	 *   timestamp = time_zero + quot * time_mult +
// /*AFLA*/ 	 *               ((rem * time_mult) >> time_shift);
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u64	time_zero;
// /*AFLA*/ 	__u32	size;			/* Header size up to __reserved[] fields. */
// /*AFLA*/ 
// /*AFLA*/ 		/*
// /*AFLA*/ 		 * Hole for extension of the self monitor capabilities
// /*AFLA*/ 		 */
// /*AFLA*/ 
// /*AFLA*/ 	__u8	__reserved[118*8+4];	/* align to 1k. */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Control data for the mmap() data buffer.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * User-space reading the @data_head value should issue an smp_rmb(),
// /*AFLA*/ 	 * after reading this value.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * When the mapping is PROT_WRITE the @data_tail value should be
// /*AFLA*/ 	 * written by userspace to reflect the last read data, after issueing
// /*AFLA*/ 	 * an smp_mb() to separate the data read from the ->data_tail store.
// /*AFLA*/ 	 * In this case the kernel will not over-write unread data.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * See perf_output_put_handle() for the data ordering.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * data_{offset,size} indicate the location and size of the perf record
// /*AFLA*/ 	 * buffer within the mmapped area.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u64   data_head;		/* head in the data section */
// /*AFLA*/ 	__u64	data_tail;		/* user-space written tail */
// /*AFLA*/ 	__u64	data_offset;		/* where the buffer starts */
// /*AFLA*/ 	__u64	data_size;		/* data buffer size */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * AUX area is defined by aux_{offset,size} fields that should be set
// /*AFLA*/ 	 * by the userspace, so that
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *   aux_offset >= data_offset + data_size
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * prior to mmap()ing it. Size of the mmap()ed area should be aux_size.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Ring buffer pointers aux_{head,tail} have the same semantics as
// /*AFLA*/ 	 * data_{head,tail} and same ordering rules apply.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u64	aux_head;
// /*AFLA*/ 	__u64	aux_tail;
// /*AFLA*/ 	__u64	aux_offset;
// /*AFLA*/ 	__u64	aux_size;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define PERF_RECORD_MISC_CPUMODE_MASK		(7 << 0)
// /*AFLA*/ #define PERF_RECORD_MISC_CPUMODE_UNKNOWN	(0 << 0)
// /*AFLA*/ #define PERF_RECORD_MISC_KERNEL			(1 << 0)
// /*AFLA*/ #define PERF_RECORD_MISC_USER			(2 << 0)
// /*AFLA*/ #define PERF_RECORD_MISC_HYPERVISOR		(3 << 0)
// /*AFLA*/ #define PERF_RECORD_MISC_GUEST_KERNEL		(4 << 0)
// /*AFLA*/ #define PERF_RECORD_MISC_GUEST_USER		(5 << 0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Indicates that /proc/PID/maps parsing are truncated by time out.
// /*AFLA*/  */
// /*AFLA*/ #define PERF_RECORD_MISC_PROC_MAP_PARSE_TIMEOUT	(1 << 12)
// /*AFLA*/ /*
// /*AFLA*/  * PERF_RECORD_MISC_MMAP_DATA and PERF_RECORD_MISC_COMM_EXEC are used on
// /*AFLA*/  * different events so can reuse the same bit position.
// /*AFLA*/  * Ditto PERF_RECORD_MISC_SWITCH_OUT.
// /*AFLA*/  */
// /*AFLA*/ #define PERF_RECORD_MISC_MMAP_DATA		(1 << 13)
// /*AFLA*/ #define PERF_RECORD_MISC_COMM_EXEC		(1 << 13)
// /*AFLA*/ #define PERF_RECORD_MISC_SWITCH_OUT		(1 << 13)
// /*AFLA*/ /*
// /*AFLA*/  * Indicates that the content of PERF_SAMPLE_IP points to
// /*AFLA*/  * the actual instruction that triggered the event. See also
// /*AFLA*/  * perf_event_attr::precise_ip.
// /*AFLA*/  */
// /*AFLA*/ #define PERF_RECORD_MISC_EXACT_IP		(1 << 14)
// /*AFLA*/ /*
// /*AFLA*/  * Reserve the last bit to indicate some extended misc field
// /*AFLA*/  */
// /*AFLA*/ #define PERF_RECORD_MISC_EXT_RESERVED		(1 << 15)
// /*AFLA*/ 
// /*AFLA*/ struct perf_event_header {
// /*AFLA*/ 	__u32	type;
// /*AFLA*/ 	__u16	misc;
// /*AFLA*/ 	__u16	size;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum perf_event_type {
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If perf_event_attr.sample_id_all is set then all event types will
// /*AFLA*/ 	 * have the sample_type selected fields related to where/when
// /*AFLA*/ 	 * (identity) an event took place (TID, TIME, ID, STREAM_ID, CPU,
// /*AFLA*/ 	 * IDENTIFIER) described in PERF_RECORD_SAMPLE below, it will be stashed
// /*AFLA*/ 	 * just after the perf_event_header and the fields already present for
// /*AFLA*/ 	 * the existing fields, i.e. at the end of the payload. That way a newer
// /*AFLA*/ 	 * perf.data file will be supported by older perf tools, with these new
// /*AFLA*/ 	 * optional fields being ignored.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * struct sample_id {
// /*AFLA*/ 	 * 	{ u32			pid, tid; } && PERF_SAMPLE_TID
// /*AFLA*/ 	 * 	{ u64			time;     } && PERF_SAMPLE_TIME
// /*AFLA*/ 	 * 	{ u64			id;       } && PERF_SAMPLE_ID
// /*AFLA*/ 	 * 	{ u64			stream_id;} && PERF_SAMPLE_STREAM_ID
// /*AFLA*/ 	 * 	{ u32			cpu, res; } && PERF_SAMPLE_CPU
// /*AFLA*/ 	 *	{ u64			id;	  } && PERF_SAMPLE_IDENTIFIER
// /*AFLA*/ 	 * } && perf_event_attr::sample_id_all
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Note that PERF_SAMPLE_IDENTIFIER duplicates PERF_SAMPLE_ID.  The
// /*AFLA*/ 	 * advantage of PERF_SAMPLE_IDENTIFIER is that its position is fixed
// /*AFLA*/ 	 * relative to header.size.
// /*AFLA*/ 	 */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The MMAP events record the PROT_EXEC mappings so that we can
// /*AFLA*/ 	 * correlate userspace IPs to code. They have the following structure:
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * struct {
// /*AFLA*/ 	 *	struct perf_event_header	header;
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *	u32				pid, tid;
// /*AFLA*/ 	 *	u64				addr;
// /*AFLA*/ 	 *	u64				len;
// /*AFLA*/ 	 *	u64				pgoff;
// /*AFLA*/ 	 *	char				filename[];
// /*AFLA*/ 	 * 	struct sample_id		sample_id;
// /*AFLA*/ 	 * };
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERF_RECORD_MMAP			= 1,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * struct {
// /*AFLA*/ 	 *	struct perf_event_header	header;
// /*AFLA*/ 	 *	u64				id;
// /*AFLA*/ 	 *	u64				lost;
// /*AFLA*/ 	 * 	struct sample_id		sample_id;
// /*AFLA*/ 	 * };
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERF_RECORD_LOST			= 2,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * struct {
// /*AFLA*/ 	 *	struct perf_event_header	header;
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *	u32				pid, tid;
// /*AFLA*/ 	 *	char				comm[];
// /*AFLA*/ 	 * 	struct sample_id		sample_id;
// /*AFLA*/ 	 * };
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERF_RECORD_COMM			= 3,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * struct {
// /*AFLA*/ 	 *	struct perf_event_header	header;
// /*AFLA*/ 	 *	u32				pid, ppid;
// /*AFLA*/ 	 *	u32				tid, ptid;
// /*AFLA*/ 	 *	u64				time;
// /*AFLA*/ 	 * 	struct sample_id		sample_id;
// /*AFLA*/ 	 * };
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERF_RECORD_EXIT			= 4,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * struct {
// /*AFLA*/ 	 *	struct perf_event_header	header;
// /*AFLA*/ 	 *	u64				time;
// /*AFLA*/ 	 *	u64				id;
// /*AFLA*/ 	 *	u64				stream_id;
// /*AFLA*/ 	 * 	struct sample_id		sample_id;
// /*AFLA*/ 	 * };
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERF_RECORD_THROTTLE			= 5,
// /*AFLA*/ 	PERF_RECORD_UNTHROTTLE			= 6,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * struct {
// /*AFLA*/ 	 *	struct perf_event_header	header;
// /*AFLA*/ 	 *	u32				pid, ppid;
// /*AFLA*/ 	 *	u32				tid, ptid;
// /*AFLA*/ 	 *	u64				time;
// /*AFLA*/ 	 * 	struct sample_id		sample_id;
// /*AFLA*/ 	 * };
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERF_RECORD_FORK			= 7,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * struct {
// /*AFLA*/ 	 *	struct perf_event_header	header;
// /*AFLA*/ 	 *	u32				pid, tid;
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *	struct read_format		values;
// /*AFLA*/ 	 * 	struct sample_id		sample_id;
// /*AFLA*/ 	 * };
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERF_RECORD_READ			= 8,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * struct {
// /*AFLA*/ 	 *	struct perf_event_header	header;
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *	#
// /*AFLA*/ 	 *	# Note that PERF_SAMPLE_IDENTIFIER duplicates PERF_SAMPLE_ID.
// /*AFLA*/ 	 *	# The advantage of PERF_SAMPLE_IDENTIFIER is that its position
// /*AFLA*/ 	 *	# is fixed relative to header.
// /*AFLA*/ 	 *	#
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *	{ u64			id;	  } && PERF_SAMPLE_IDENTIFIER
// /*AFLA*/ 	 *	{ u64			ip;	  } && PERF_SAMPLE_IP
// /*AFLA*/ 	 *	{ u32			pid, tid; } && PERF_SAMPLE_TID
// /*AFLA*/ 	 *	{ u64			time;     } && PERF_SAMPLE_TIME
// /*AFLA*/ 	 *	{ u64			addr;     } && PERF_SAMPLE_ADDR
// /*AFLA*/ 	 *	{ u64			id;	  } && PERF_SAMPLE_ID
// /*AFLA*/ 	 *	{ u64			stream_id;} && PERF_SAMPLE_STREAM_ID
// /*AFLA*/ 	 *	{ u32			cpu, res; } && PERF_SAMPLE_CPU
// /*AFLA*/ 	 *	{ u64			period;   } && PERF_SAMPLE_PERIOD
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *	{ struct read_format	values;	  } && PERF_SAMPLE_READ
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *	{ u64			nr,
// /*AFLA*/ 	 *	  u64			ips[nr];  } && PERF_SAMPLE_CALLCHAIN
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *	#
// /*AFLA*/ 	 *	# The RAW record below is opaque data wrt the ABI
// /*AFLA*/ 	 *	#
// /*AFLA*/ 	 *	# That is, the ABI doesn't make any promises wrt to
// /*AFLA*/ 	 *	# the stability of its content, it may vary depending
// /*AFLA*/ 	 *	# on event, hardware, kernel version and phase of
// /*AFLA*/ 	 *	# the moon.
// /*AFLA*/ 	 *	#
// /*AFLA*/ 	 *	# In other words, PERF_SAMPLE_RAW contents are not an ABI.
// /*AFLA*/ 	 *	#
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *	{ u32			size;
// /*AFLA*/ 	 *	  char                  data[size];}&& PERF_SAMPLE_RAW
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *	{ u64                   nr;
// /*AFLA*/ 	 *        { u64 from, to, flags } lbr[nr];} && PERF_SAMPLE_BRANCH_STACK
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * 	{ u64			abi; # enum perf_sample_regs_abi
// /*AFLA*/ 	 * 	  u64			regs[weight(mask)]; } && PERF_SAMPLE_REGS_USER
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * 	{ u64			size;
// /*AFLA*/ 	 * 	  char			data[size];
// /*AFLA*/ 	 * 	  u64			dyn_size; } && PERF_SAMPLE_STACK_USER
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *	{ u64			weight;   } && PERF_SAMPLE_WEIGHT
// /*AFLA*/ 	 *	{ u64			data_src; } && PERF_SAMPLE_DATA_SRC
// /*AFLA*/ 	 *	{ u64			transaction; } && PERF_SAMPLE_TRANSACTION
// /*AFLA*/ 	 *	{ u64			abi; # enum perf_sample_regs_abi
// /*AFLA*/ 	 *	  u64			regs[weight(mask)]; } && PERF_SAMPLE_REGS_INTR
// /*AFLA*/ 	 * };
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERF_RECORD_SAMPLE			= 9,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The MMAP2 records are an augmented version of MMAP, they add
// /*AFLA*/ 	 * maj, min, ino numbers to be used to uniquely identify each mapping
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * struct {
// /*AFLA*/ 	 *	struct perf_event_header	header;
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *	u32				pid, tid;
// /*AFLA*/ 	 *	u64				addr;
// /*AFLA*/ 	 *	u64				len;
// /*AFLA*/ 	 *	u64				pgoff;
// /*AFLA*/ 	 *	u32				maj;
// /*AFLA*/ 	 *	u32				min;
// /*AFLA*/ 	 *	u64				ino;
// /*AFLA*/ 	 *	u64				ino_generation;
// /*AFLA*/ 	 *	u32				prot, flags;
// /*AFLA*/ 	 *	char				filename[];
// /*AFLA*/ 	 * 	struct sample_id		sample_id;
// /*AFLA*/ 	 * };
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERF_RECORD_MMAP2			= 10,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Records that new data landed in the AUX buffer part.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * struct {
// /*AFLA*/ 	 * 	struct perf_event_header	header;
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * 	u64				aux_offset;
// /*AFLA*/ 	 * 	u64				aux_size;
// /*AFLA*/ 	 *	u64				flags;
// /*AFLA*/ 	 * 	struct sample_id		sample_id;
// /*AFLA*/ 	 * };
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERF_RECORD_AUX				= 11,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Indicates that instruction trace has started
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * struct {
// /*AFLA*/ 	 *	struct perf_event_header	header;
// /*AFLA*/ 	 *	u32				pid;
// /*AFLA*/ 	 *	u32				tid;
// /*AFLA*/ 	 * };
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERF_RECORD_ITRACE_START		= 12,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Records the dropped/lost sample number.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * struct {
// /*AFLA*/ 	 *	struct perf_event_header	header;
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *	u64				lost;
// /*AFLA*/ 	 *	struct sample_id		sample_id;
// /*AFLA*/ 	 * };
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERF_RECORD_LOST_SAMPLES		= 13,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Records a context switch in or out (flagged by
// /*AFLA*/ 	 * PERF_RECORD_MISC_SWITCH_OUT). See also
// /*AFLA*/ 	 * PERF_RECORD_SWITCH_CPU_WIDE.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * struct {
// /*AFLA*/ 	 *	struct perf_event_header	header;
// /*AFLA*/ 	 *	struct sample_id		sample_id;
// /*AFLA*/ 	 * };
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERF_RECORD_SWITCH			= 14,
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * CPU-wide version of PERF_RECORD_SWITCH with next_prev_pid and
// /*AFLA*/ 	 * next_prev_tid that are the next (switching out) or previous
// /*AFLA*/ 	 * (switching in) pid/tid.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * struct {
// /*AFLA*/ 	 *	struct perf_event_header	header;
// /*AFLA*/ 	 *	u32				next_prev_pid;
// /*AFLA*/ 	 *	u32				next_prev_tid;
// /*AFLA*/ 	 *	struct sample_id		sample_id;
// /*AFLA*/ 	 * };
// /*AFLA*/ 	 */
// /*AFLA*/ 	PERF_RECORD_SWITCH_CPU_WIDE		= 15,
// /*AFLA*/ 
// /*AFLA*/ 	PERF_RECORD_MAX,			/* non-ABI */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define PERF_MAX_STACK_DEPTH		127
// /*AFLA*/ #define PERF_MAX_CONTEXTS_PER_STACK	  8
// /*AFLA*/ 
// /*AFLA*/ enum perf_callchain_context {
// /*AFLA*/ 	PERF_CONTEXT_HV			= (__u64)-32,
// /*AFLA*/ 	PERF_CONTEXT_KERNEL		= (__u64)-128,
// /*AFLA*/ 	PERF_CONTEXT_USER		= (__u64)-512,
// /*AFLA*/ 
// /*AFLA*/ 	PERF_CONTEXT_GUEST		= (__u64)-2048,
// /*AFLA*/ 	PERF_CONTEXT_GUEST_KERNEL	= (__u64)-2176,
// /*AFLA*/ 	PERF_CONTEXT_GUEST_USER		= (__u64)-2560,
// /*AFLA*/ 
// /*AFLA*/ 	PERF_CONTEXT_MAX		= (__u64)-4095,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * PERF_RECORD_AUX::flags bits
// /*AFLA*/  */
// /*AFLA*/ #define PERF_AUX_FLAG_TRUNCATED		0x01	/* record was truncated to fit */
// /*AFLA*/ #define PERF_AUX_FLAG_OVERWRITE		0x02	/* snapshot from overwrite mode */
// /*AFLA*/ 
// /*AFLA*/ #define PERF_FLAG_FD_NO_GROUP		(1UL << 0)
// /*AFLA*/ #define PERF_FLAG_FD_OUTPUT		(1UL << 1)
// /*AFLA*/ #define PERF_FLAG_PID_CGROUP		(1UL << 2) /* pid=cgroup id, per-cpu mode only */
// /*AFLA*/ #define PERF_FLAG_FD_CLOEXEC		(1UL << 3) /* O_CLOEXEC */
// /*AFLA*/ 
// /*AFLA*/ union perf_mem_data_src {
// /*AFLA*/ 	__u64 val;
// /*AFLA*/ 	struct {
// /*AFLA*/ 		__u64   mem_op:5,	/* type of opcode */
// /*AFLA*/ 			mem_lvl:14,	/* memory hierarchy level */
// /*AFLA*/ 			mem_snoop:5,	/* snoop mode */
// /*AFLA*/ 			mem_lock:2,	/* lock instr */
// /*AFLA*/ 			mem_dtlb:7,	/* tlb access */
// /*AFLA*/ 			mem_rsvd:31;
// /*AFLA*/ 	};
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* type of opcode (load/store/prefetch,code) */
// /*AFLA*/ #define PERF_MEM_OP_NA		0x01 /* not available */
// /*AFLA*/ #define PERF_MEM_OP_LOAD	0x02 /* load instruction */
// /*AFLA*/ #define PERF_MEM_OP_STORE	0x04 /* store instruction */
// /*AFLA*/ #define PERF_MEM_OP_PFETCH	0x08 /* prefetch */
// /*AFLA*/ #define PERF_MEM_OP_EXEC	0x10 /* code (execution) */
// /*AFLA*/ #define PERF_MEM_OP_SHIFT	0
// /*AFLA*/ 
// /*AFLA*/ /* memory hierarchy (memory level, hit or miss) */
// /*AFLA*/ #define PERF_MEM_LVL_NA		0x01  /* not available */
// /*AFLA*/ #define PERF_MEM_LVL_HIT	0x02  /* hit level */
// /*AFLA*/ #define PERF_MEM_LVL_MISS	0x04  /* miss level  */
// /*AFLA*/ #define PERF_MEM_LVL_L1		0x08  /* L1 */
// /*AFLA*/ #define PERF_MEM_LVL_LFB	0x10  /* Line Fill Buffer */
// /*AFLA*/ #define PERF_MEM_LVL_L2		0x20  /* L2 */
// /*AFLA*/ #define PERF_MEM_LVL_L3		0x40  /* L3 */
// /*AFLA*/ #define PERF_MEM_LVL_LOC_RAM	0x80  /* Local DRAM */
// /*AFLA*/ #define PERF_MEM_LVL_REM_RAM1	0x100 /* Remote DRAM (1 hop) */
// /*AFLA*/ #define PERF_MEM_LVL_REM_RAM2	0x200 /* Remote DRAM (2 hops) */
// /*AFLA*/ #define PERF_MEM_LVL_REM_CCE1	0x400 /* Remote Cache (1 hop) */
// /*AFLA*/ #define PERF_MEM_LVL_REM_CCE2	0x800 /* Remote Cache (2 hops) */
// /*AFLA*/ #define PERF_MEM_LVL_IO		0x1000 /* I/O memory */
// /*AFLA*/ #define PERF_MEM_LVL_UNC	0x2000 /* Uncached memory */
// /*AFLA*/ #define PERF_MEM_LVL_SHIFT	5
// /*AFLA*/ 
// /*AFLA*/ /* snoop mode */
// /*AFLA*/ #define PERF_MEM_SNOOP_NA	0x01 /* not available */
// /*AFLA*/ #define PERF_MEM_SNOOP_NONE	0x02 /* no snoop */
// /*AFLA*/ #define PERF_MEM_SNOOP_HIT	0x04 /* snoop hit */
// /*AFLA*/ #define PERF_MEM_SNOOP_MISS	0x08 /* snoop miss */
// /*AFLA*/ #define PERF_MEM_SNOOP_HITM	0x10 /* snoop hit modified */
// /*AFLA*/ #define PERF_MEM_SNOOP_SHIFT	19
// /*AFLA*/ 
// /*AFLA*/ /* locked instruction */
// /*AFLA*/ #define PERF_MEM_LOCK_NA	0x01 /* not available */
// /*AFLA*/ #define PERF_MEM_LOCK_LOCKED	0x02 /* locked transaction */
// /*AFLA*/ #define PERF_MEM_LOCK_SHIFT	24
// /*AFLA*/ 
// /*AFLA*/ /* TLB access */
// /*AFLA*/ #define PERF_MEM_TLB_NA		0x01 /* not available */
// /*AFLA*/ #define PERF_MEM_TLB_HIT	0x02 /* hit level */
// /*AFLA*/ #define PERF_MEM_TLB_MISS	0x04 /* miss level */
// /*AFLA*/ #define PERF_MEM_TLB_L1		0x08 /* L1 */
// /*AFLA*/ #define PERF_MEM_TLB_L2		0x10 /* L2 */
// /*AFLA*/ #define PERF_MEM_TLB_WK		0x20 /* Hardware Walker*/
// /*AFLA*/ #define PERF_MEM_TLB_OS		0x40 /* OS fault handler */
// /*AFLA*/ #define PERF_MEM_TLB_SHIFT	26
// /*AFLA*/ 
// /*AFLA*/ #define PERF_MEM_S(a, s) \
// /*AFLA*/ 	(((__u64)PERF_MEM_##a##_##s) << PERF_MEM_##a##_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * single taken branch record layout:
// /*AFLA*/  *
// /*AFLA*/  *      from: source instruction (may not always be a branch insn)
// /*AFLA*/  *        to: branch target
// /*AFLA*/  *   mispred: branch target was mispredicted
// /*AFLA*/  * predicted: branch target was predicted
// /*AFLA*/  *
// /*AFLA*/  * support for mispred, predicted is optional. In case it
// /*AFLA*/  * is not supported mispred = predicted = 0.
// /*AFLA*/  *
// /*AFLA*/  *     in_tx: running in a hardware transaction
// /*AFLA*/  *     abort: aborting a hardware transaction
// /*AFLA*/  *    cycles: cycles from last branch (or 0 if not supported)
// /*AFLA*/  */
// /*AFLA*/ struct perf_branch_entry {
// /*AFLA*/ 	__u64	from;
// /*AFLA*/ 	__u64	to;
// /*AFLA*/ 	__u64	mispred:1,  /* target mispredicted */
// /*AFLA*/ 		predicted:1,/* target predicted */
// /*AFLA*/ 		in_tx:1,    /* in transaction */
// /*AFLA*/ 		abort:1,    /* transaction abort */
// /*AFLA*/ 		cycles:16,  /* cycle count to last branch */
// /*AFLA*/ 		reserved:44;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_PERF_EVENT_H */
