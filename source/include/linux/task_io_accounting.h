// /*AFLA*/ /*
// /*AFLA*/  * task_io_accounting: a structure which is used for recording a single task's
// /*AFLA*/  * IO statistics.
// /*AFLA*/  *
// /*AFLA*/  * Don't include this header file directly - it is designed to be dragged in via
// /*AFLA*/  * sched.h.
// /*AFLA*/  *
// /*AFLA*/  * Blame Andrew Morton for all this.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct task_io_accounting {
// /*AFLA*/ #ifdef CONFIG_TASK_XACCT
// /*AFLA*/ 	/* bytes read */
// /*AFLA*/ 	u64 rchar;
// /*AFLA*/ 	/*  bytes written */
// /*AFLA*/ 	u64 wchar;
// /*AFLA*/ 	/* # of read syscalls */
// /*AFLA*/ 	u64 syscr;
// /*AFLA*/ 	/* # of write syscalls */
// /*AFLA*/ 	u64 syscw;
// /*AFLA*/ #endif /* CONFIG_TASK_XACCT */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TASK_IO_ACCOUNTING
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The number of bytes which this task has caused to be read from
// /*AFLA*/ 	 * storage.
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64 read_bytes;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The number of bytes which this task has caused, or shall cause to be
// /*AFLA*/ 	 * written to disk.
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64 write_bytes;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * A task can cause "negative" IO too.  If this task truncates some
// /*AFLA*/ 	 * dirty pagecache, some IO which another task has been accounted for
// /*AFLA*/ 	 * (in its write_bytes) will not be happening.  We _could_ just
// /*AFLA*/ 	 * subtract that from the truncating task's write_bytes, but there is
// /*AFLA*/ 	 * information loss in doing that.
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64 cancelled_write_bytes;
// /*AFLA*/ #endif /* CONFIG_TASK_IO_ACCOUNTING */
// /*AFLA*/ };
