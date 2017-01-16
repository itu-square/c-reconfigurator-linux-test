// /*AFLA*/ /* cgroupstats.h - exporting per-cgroup statistics
// /*AFLA*/  *
// /*AFLA*/  * Copyright IBM Corporation, 2007
// /*AFLA*/  * Author Balbir Singh <balbir@linux.vnet.ibm.com>
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
// /*AFLA*/ #ifndef _LINUX_CGROUPSTATS_H
// /*AFLA*/ #define _LINUX_CGROUPSTATS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/taskstats.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Data shared between user space and kernel space on a per cgroup
// /*AFLA*/  * basis. This data is shared using taskstats.
// /*AFLA*/  *
// /*AFLA*/  * Most of these states are derived by looking at the task->state value
// /*AFLA*/  * For the nr_io_wait state, a flag in the delay accounting structure
// /*AFLA*/  * indicates that the task is waiting on IO
// /*AFLA*/  *
// /*AFLA*/  * Each member is aligned to a 8 byte boundary.
// /*AFLA*/  */
// /*AFLA*/ struct cgroupstats {
// /*AFLA*/ 	__u64	nr_sleeping;		/* Number of tasks sleeping */
// /*AFLA*/ 	__u64	nr_running;		/* Number of tasks running */
// /*AFLA*/ 	__u64	nr_stopped;		/* Number of tasks in stopped state */
// /*AFLA*/ 	__u64	nr_uninterruptible;	/* Number of tasks in uninterruptible */
// /*AFLA*/ 					/* state */
// /*AFLA*/ 	__u64	nr_io_wait;		/* Number of tasks waiting on IO */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Commands sent from userspace
// /*AFLA*/  * Not versioned. New commands should only be inserted at the enum's end
// /*AFLA*/  * prior to __CGROUPSTATS_CMD_MAX
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	CGROUPSTATS_CMD_UNSPEC = __TASKSTATS_CMD_MAX,	/* Reserved */
// /*AFLA*/ 	CGROUPSTATS_CMD_GET,		/* user->kernel request/get-response */
// /*AFLA*/ 	CGROUPSTATS_CMD_NEW,		/* kernel->user event */
// /*AFLA*/ 	__CGROUPSTATS_CMD_MAX,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define CGROUPSTATS_CMD_MAX (__CGROUPSTATS_CMD_MAX - 1)
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	CGROUPSTATS_TYPE_UNSPEC = 0,	/* Reserved */
// /*AFLA*/ 	CGROUPSTATS_TYPE_CGROUP_STATS,	/* contains name + stats */
// /*AFLA*/ 	__CGROUPSTATS_TYPE_MAX,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define CGROUPSTATS_TYPE_MAX (__CGROUPSTATS_TYPE_MAX - 1)
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	CGROUPSTATS_CMD_ATTR_UNSPEC = 0,
// /*AFLA*/ 	CGROUPSTATS_CMD_ATTR_FD,
// /*AFLA*/ 	__CGROUPSTATS_CMD_ATTR_MAX,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define CGROUPSTATS_CMD_ATTR_MAX (__CGROUPSTATS_CMD_ATTR_MAX - 1)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_CGROUPSTATS_H */
