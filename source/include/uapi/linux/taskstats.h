// /*AFLA*/ /* taskstats.h - exporting per-task statistics
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) Shailabh Nagar, IBM Corp. 2006
// /*AFLA*/  *           (C) Balbir Singh,   IBM Corp. 2006
// /*AFLA*/  *           (C) Jay Lan,        SGI, 2006
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify it
// /*AFLA*/  * under the terms of version 2.1 of the GNU Lesser General Public License
// /*AFLA*/  * as published by the Free Software Foundation.
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it would be useful, but
// /*AFLA*/  * WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_TASKSTATS_H
// /*AFLA*/ #define _LINUX_TASKSTATS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /* Format for per-task data returned to userland when
// /*AFLA*/  *	- a task exits
// /*AFLA*/  *	- listener requests stats for a task
// /*AFLA*/  *
// /*AFLA*/  * The struct is versioned. Newer versions should only add fields to
// /*AFLA*/  * the bottom of the struct to maintain backward compatibility.
// /*AFLA*/  *
// /*AFLA*/  *
// /*AFLA*/  * To add new fields
// /*AFLA*/  *	a) bump up TASKSTATS_VERSION
// /*AFLA*/  *	b) add comment indicating new version number at end of struct
// /*AFLA*/  *	c) add new fields after version comment; maintain 64-bit alignment
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define TASKSTATS_VERSION	8
// /*AFLA*/ #define TS_COMM_LEN		32	/* should be >= TASK_COMM_LEN
// /*AFLA*/ 					 * in linux/sched.h */
// /*AFLA*/ 
// /*AFLA*/ struct taskstats {
// /*AFLA*/ 
// /*AFLA*/ 	/* The version number of this struct. This field is always set to
// /*AFLA*/ 	 * TAKSTATS_VERSION, which is defined in <linux/taskstats.h>.
// /*AFLA*/ 	 * Each time the struct is changed, the value should be incremented.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u16	version;
// /*AFLA*/ 	__u32	ac_exitcode;		/* Exit status */
// /*AFLA*/ 
// /*AFLA*/ 	/* The accounting flags of a task as defined in <linux/acct.h>
// /*AFLA*/ 	 * Defined values are AFORK, ASU, ACOMPAT, ACORE, and AXSIG.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u8	ac_flag;		/* Record flags */
// /*AFLA*/ 	__u8	ac_nice;		/* task_nice */
// /*AFLA*/ 
// /*AFLA*/ 	/* Delay accounting fields start
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * All values, until comment "Delay accounting fields end" are
// /*AFLA*/ 	 * available only if delay accounting is enabled, even though the last
// /*AFLA*/ 	 * few fields are not delays
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * xxx_count is the number of delay values recorded
// /*AFLA*/ 	 * xxx_delay_total is the corresponding cumulative delay in nanoseconds
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * xxx_delay_total wraps around to zero on overflow
// /*AFLA*/ 	 * xxx_count incremented regardless of overflow
// /*AFLA*/ 	 */
// /*AFLA*/ 
// /*AFLA*/ 	/* Delay waiting for cpu, while runnable
// /*AFLA*/ 	 * count, delay_total NOT updated atomically
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u64	cpu_count __attribute__((aligned(8)));
// /*AFLA*/ 	__u64	cpu_delay_total;
// /*AFLA*/ 
// /*AFLA*/ 	/* Following four fields atomically updated using task->delays->lock */
// /*AFLA*/ 
// /*AFLA*/ 	/* Delay waiting for synchronous block I/O to complete
// /*AFLA*/ 	 * does not account for delays in I/O submission
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u64	blkio_count;
// /*AFLA*/ 	__u64	blkio_delay_total;
// /*AFLA*/ 
// /*AFLA*/ 	/* Delay waiting for page fault I/O (swap in only) */
// /*AFLA*/ 	__u64	swapin_count;
// /*AFLA*/ 	__u64	swapin_delay_total;
// /*AFLA*/ 
// /*AFLA*/ 	/* cpu "wall-clock" running time
// /*AFLA*/ 	 * On some architectures, value will adjust for cpu time stolen
// /*AFLA*/ 	 * from the kernel in involuntary waits due to virtualization.
// /*AFLA*/ 	 * Value is cumulative, in nanoseconds, without a corresponding count
// /*AFLA*/ 	 * and wraps around to zero silently on overflow
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u64	cpu_run_real_total;
// /*AFLA*/ 
// /*AFLA*/ 	/* cpu "virtual" running time
// /*AFLA*/ 	 * Uses time intervals seen by the kernel i.e. no adjustment
// /*AFLA*/ 	 * for kernel's involuntary waits due to virtualization.
// /*AFLA*/ 	 * Value is cumulative, in nanoseconds, without a corresponding count
// /*AFLA*/ 	 * and wraps around to zero silently on overflow
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u64	cpu_run_virtual_total;
// /*AFLA*/ 	/* Delay accounting fields end */
// /*AFLA*/ 	/* version 1 ends here */
// /*AFLA*/ 
// /*AFLA*/ 	/* Basic Accounting Fields start */
// /*AFLA*/ 	char	ac_comm[TS_COMM_LEN];	/* Command name */
// /*AFLA*/ 	__u8	ac_sched __attribute__((aligned(8)));
// /*AFLA*/ 					/* Scheduling discipline */
// /*AFLA*/ 	__u8	ac_pad[3];
// /*AFLA*/ 	__u32	ac_uid __attribute__((aligned(8)));
// /*AFLA*/ 					/* User ID */
// /*AFLA*/ 	__u32	ac_gid;			/* Group ID */
// /*AFLA*/ 	__u32	ac_pid;			/* Process ID */
// /*AFLA*/ 	__u32	ac_ppid;		/* Parent process ID */
// /*AFLA*/ 	__u32	ac_btime;		/* Begin time [sec since 1970] */
// /*AFLA*/ 	__u64	ac_etime __attribute__((aligned(8)));
// /*AFLA*/ 					/* Elapsed time [usec] */
// /*AFLA*/ 	__u64	ac_utime;		/* User CPU time [usec] */
// /*AFLA*/ 	__u64	ac_stime;		/* SYstem CPU time [usec] */
// /*AFLA*/ 	__u64	ac_minflt;		/* Minor Page Fault Count */
// /*AFLA*/ 	__u64	ac_majflt;		/* Major Page Fault Count */
// /*AFLA*/ 	/* Basic Accounting Fields end */
// /*AFLA*/ 
// /*AFLA*/ 	/* Extended accounting fields start */
// /*AFLA*/ 	/* Accumulated RSS usage in duration of a task, in MBytes-usecs.
// /*AFLA*/ 	 * The current rss usage is added to this counter every time
// /*AFLA*/ 	 * a tick is charged to a task's system time. So, at the end we
// /*AFLA*/ 	 * will have memory usage multiplied by system time. Thus an
// /*AFLA*/ 	 * average usage per system time unit can be calculated.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u64	coremem;		/* accumulated RSS usage in MB-usec */
// /*AFLA*/ 	/* Accumulated virtual memory usage in duration of a task.
// /*AFLA*/ 	 * Same as acct_rss_mem1 above except that we keep track of VM usage.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u64	virtmem;		/* accumulated VM  usage in MB-usec */
// /*AFLA*/ 
// /*AFLA*/ 	/* High watermark of RSS and virtual memory usage in duration of
// /*AFLA*/ 	 * a task, in KBytes.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u64	hiwater_rss;		/* High-watermark of RSS usage, in KB */
// /*AFLA*/ 	__u64	hiwater_vm;		/* High-water VM usage, in KB */
// /*AFLA*/ 
// /*AFLA*/ 	/* The following four fields are I/O statistics of a task. */
// /*AFLA*/ 	__u64	read_char;		/* bytes read */
// /*AFLA*/ 	__u64	write_char;		/* bytes written */
// /*AFLA*/ 	__u64	read_syscalls;		/* read syscalls */
// /*AFLA*/ 	__u64	write_syscalls;		/* write syscalls */
// /*AFLA*/ 	/* Extended accounting fields end */
// /*AFLA*/ 
// /*AFLA*/ #define TASKSTATS_HAS_IO_ACCOUNTING
// /*AFLA*/ 	/* Per-task storage I/O accounting starts */
// /*AFLA*/ 	__u64	read_bytes;		/* bytes of read I/O */
// /*AFLA*/ 	__u64	write_bytes;		/* bytes of write I/O */
// /*AFLA*/ 	__u64	cancelled_write_bytes;	/* bytes of cancelled write I/O */
// /*AFLA*/ 
// /*AFLA*/ 	__u64  nvcsw;			/* voluntary_ctxt_switches */
// /*AFLA*/ 	__u64  nivcsw;			/* nonvoluntary_ctxt_switches */
// /*AFLA*/ 
// /*AFLA*/ 	/* time accounting for SMT machines */
// /*AFLA*/ 	__u64	ac_utimescaled;		/* utime scaled on frequency etc */
// /*AFLA*/ 	__u64	ac_stimescaled;		/* stime scaled on frequency etc */
// /*AFLA*/ 	__u64	cpu_scaled_run_real_total; /* scaled cpu_run_real_total */
// /*AFLA*/ 
// /*AFLA*/ 	/* Delay waiting for memory reclaim */
// /*AFLA*/ 	__u64	freepages_count;
// /*AFLA*/ 	__u64	freepages_delay_total;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Commands sent from userspace
// /*AFLA*/  * Not versioned. New commands should only be inserted at the enum's end
// /*AFLA*/  * prior to __TASKSTATS_CMD_MAX
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	TASKSTATS_CMD_UNSPEC = 0,	/* Reserved */
// /*AFLA*/ 	TASKSTATS_CMD_GET,		/* user->kernel request/get-response */
// /*AFLA*/ 	TASKSTATS_CMD_NEW,		/* kernel->user event */
// /*AFLA*/ 	__TASKSTATS_CMD_MAX,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define TASKSTATS_CMD_MAX (__TASKSTATS_CMD_MAX - 1)
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	TASKSTATS_TYPE_UNSPEC = 0,	/* Reserved */
// /*AFLA*/ 	TASKSTATS_TYPE_PID,		/* Process id */
// /*AFLA*/ 	TASKSTATS_TYPE_TGID,		/* Thread group id */
// /*AFLA*/ 	TASKSTATS_TYPE_STATS,		/* taskstats structure */
// /*AFLA*/ 	TASKSTATS_TYPE_AGGR_PID,	/* contains pid + stats */
// /*AFLA*/ 	TASKSTATS_TYPE_AGGR_TGID,	/* contains tgid + stats */
// /*AFLA*/ 	TASKSTATS_TYPE_NULL,		/* contains nothing */
// /*AFLA*/ 	__TASKSTATS_TYPE_MAX,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define TASKSTATS_TYPE_MAX (__TASKSTATS_TYPE_MAX - 1)
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	TASKSTATS_CMD_ATTR_UNSPEC = 0,
// /*AFLA*/ 	TASKSTATS_CMD_ATTR_PID,
// /*AFLA*/ 	TASKSTATS_CMD_ATTR_TGID,
// /*AFLA*/ 	TASKSTATS_CMD_ATTR_REGISTER_CPUMASK,
// /*AFLA*/ 	TASKSTATS_CMD_ATTR_DEREGISTER_CPUMASK,
// /*AFLA*/ 	__TASKSTATS_CMD_ATTR_MAX,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define TASKSTATS_CMD_ATTR_MAX (__TASKSTATS_CMD_ATTR_MAX - 1)
// /*AFLA*/ 
// /*AFLA*/ /* NETLINK_GENERIC related info */
// /*AFLA*/ 
// /*AFLA*/ #define TASKSTATS_GENL_NAME	"TASKSTATS"
// /*AFLA*/ #define TASKSTATS_GENL_VERSION	0x1
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_TASKSTATS_H */
