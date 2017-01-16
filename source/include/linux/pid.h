// /*AFLA*/ #ifndef _LINUX_PID_H
// /*AFLA*/ #define _LINUX_PID_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ 
// /*AFLA*/ enum pid_type
// /*AFLA*/ {
// /*AFLA*/ 	PIDTYPE_PID,
// /*AFLA*/ 	PIDTYPE_PGID,
// /*AFLA*/ 	PIDTYPE_SID,
// /*AFLA*/ 	PIDTYPE_MAX
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * What is struct pid?
// /*AFLA*/  *
// /*AFLA*/  * A struct pid is the kernel's internal notion of a process identifier.
// /*AFLA*/  * It refers to individual tasks, process groups, and sessions.  While
// /*AFLA*/  * there are processes attached to it the struct pid lives in a hash
// /*AFLA*/  * table, so it and then the processes that it refers to can be found
// /*AFLA*/  * quickly from the numeric pid value.  The attached processes may be
// /*AFLA*/  * quickly accessed by following pointers from struct pid.
// /*AFLA*/  *
// /*AFLA*/  * Storing pid_t values in the kernel and referring to them later has a
// /*AFLA*/  * problem.  The process originally with that pid may have exited and the
// /*AFLA*/  * pid allocator wrapped, and another process could have come along
// /*AFLA*/  * and been assigned that pid.
// /*AFLA*/  *
// /*AFLA*/  * Referring to user space processes by holding a reference to struct
// /*AFLA*/  * task_struct has a problem.  When the user space process exits
// /*AFLA*/  * the now useless task_struct is still kept.  A task_struct plus a
// /*AFLA*/  * stack consumes around 10K of low kernel memory.  More precisely
// /*AFLA*/  * this is THREAD_SIZE + sizeof(struct task_struct).  By comparison
// /*AFLA*/  * a struct pid is about 64 bytes.
// /*AFLA*/  *
// /*AFLA*/  * Holding a reference to struct pid solves both of these problems.
// /*AFLA*/  * It is small so holding a reference does not consume a lot of
// /*AFLA*/  * resources, and since a new struct pid is allocated when the numeric pid
// /*AFLA*/  * value is reused (when pids wrap around) we don't mistakenly refer to new
// /*AFLA*/  * processes.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * struct upid is used to get the id of the struct pid, as it is
// /*AFLA*/  * seen in particular namespace. Later the struct pid is found with
// /*AFLA*/  * find_pid_ns() using the int nr and struct pid_namespace *ns.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct upid {
// /*AFLA*/ 	/* Try to keep pid_chain in the same cacheline as nr for find_vpid */
// /*AFLA*/ 	int nr;
// /*AFLA*/ 	struct pid_namespace *ns;
// /*AFLA*/ 	struct hlist_node pid_chain;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct pid
// /*AFLA*/ {
// /*AFLA*/ 	atomic_t count;
// /*AFLA*/ 	unsigned int level;
// /*AFLA*/ 	/* lists of tasks that use this pid */
// /*AFLA*/ 	struct hlist_head tasks[PIDTYPE_MAX];
// /*AFLA*/ 	struct rcu_head rcu;
// /*AFLA*/ 	struct upid numbers[1];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct pid init_struct_pid;
// /*AFLA*/ 
// /*AFLA*/ struct pid_link
// /*AFLA*/ {
// /*AFLA*/ 	struct hlist_node node;
// /*AFLA*/ 	struct pid *pid;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline struct pid *get_pid(struct pid *pid)
// /*AFLA*/ {
// /*AFLA*/ 	if (pid)
// /*AFLA*/ 		atomic_inc(&pid->count);
// /*AFLA*/ 	return pid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void put_pid(struct pid *pid);
// /*AFLA*/ extern struct task_struct *pid_task(struct pid *pid, enum pid_type);
// /*AFLA*/ extern struct task_struct *get_pid_task(struct pid *pid, enum pid_type);
// /*AFLA*/ 
// /*AFLA*/ extern struct pid *get_task_pid(struct task_struct *task, enum pid_type type);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * these helpers must be called with the tasklist_lock write-held.
// /*AFLA*/  */
// /*AFLA*/ extern void attach_pid(struct task_struct *task, enum pid_type);
// /*AFLA*/ extern void detach_pid(struct task_struct *task, enum pid_type);
// /*AFLA*/ extern void change_pid(struct task_struct *task, enum pid_type,
// /*AFLA*/ 			struct pid *pid);
// /*AFLA*/ extern void transfer_pid(struct task_struct *old, struct task_struct *new,
// /*AFLA*/ 			 enum pid_type);
// /*AFLA*/ 
// /*AFLA*/ struct pid_namespace;
// /*AFLA*/ extern struct pid_namespace init_pid_ns;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * look up a PID in the hash table. Must be called with the tasklist_lock
// /*AFLA*/  * or rcu_read_lock() held.
// /*AFLA*/  *
// /*AFLA*/  * find_pid_ns() finds the pid in the namespace specified
// /*AFLA*/  * find_vpid() finds the pid by its virtual id, i.e. in the current namespace
// /*AFLA*/  *
// /*AFLA*/  * see also find_task_by_vpid() set in include/linux/sched.h
// /*AFLA*/  */
// /*AFLA*/ extern struct pid *find_pid_ns(int nr, struct pid_namespace *ns);
// /*AFLA*/ extern struct pid *find_vpid(int nr);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Lookup a PID in the hash table, and return with it's count elevated.
// /*AFLA*/  */
// /*AFLA*/ extern struct pid *find_get_pid(int nr);
// /*AFLA*/ extern struct pid *find_ge_pid(int nr, struct pid_namespace *);
// /*AFLA*/ int next_pidmap(struct pid_namespace *pid_ns, unsigned int last);
// /*AFLA*/ 
// /*AFLA*/ extern struct pid *alloc_pid(struct pid_namespace *ns);
// /*AFLA*/ extern void free_pid(struct pid *pid);
// /*AFLA*/ extern void disable_pid_allocation(struct pid_namespace *ns);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * ns_of_pid() returns the pid namespace in which the specified pid was
// /*AFLA*/  * allocated.
// /*AFLA*/  *
// /*AFLA*/  * NOTE:
// /*AFLA*/  * 	ns_of_pid() is expected to be called for a process (task) that has
// /*AFLA*/  * 	an attached 'struct pid' (see attach_pid(), detach_pid()) i.e @pid
// /*AFLA*/  * 	is expected to be non-NULL. If @pid is NULL, caller should handle
// /*AFLA*/  * 	the resulting NULL pid-ns.
// /*AFLA*/  */
// /*AFLA*/ static inline struct pid_namespace *ns_of_pid(struct pid *pid)
// /*AFLA*/ {
// /*AFLA*/ 	struct pid_namespace *ns = NULL;
// /*AFLA*/ 	if (pid)
// /*AFLA*/ 		ns = pid->numbers[pid->level].ns;
// /*AFLA*/ 	return ns;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * is_child_reaper returns true if the pid is the init process
// /*AFLA*/  * of the current namespace. As this one could be checked before
// /*AFLA*/  * pid_ns->child_reaper is assigned in copy_process, we check
// /*AFLA*/  * with the pid number.
// /*AFLA*/  */
// /*AFLA*/ static inline bool is_child_reaper(struct pid *pid)
// /*AFLA*/ {
// /*AFLA*/ 	return pid->numbers[pid->level].nr == 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * the helpers to get the pid's id seen from different namespaces
// /*AFLA*/  *
// /*AFLA*/  * pid_nr()    : global id, i.e. the id seen from the init namespace;
// /*AFLA*/  * pid_vnr()   : virtual id, i.e. the id seen from the pid namespace of
// /*AFLA*/  *               current.
// /*AFLA*/  * pid_nr_ns() : id seen from the ns specified.
// /*AFLA*/  *
// /*AFLA*/  * see also task_xid_nr() etc in include/linux/sched.h
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static inline pid_t pid_nr(struct pid *pid)
// /*AFLA*/ {
// /*AFLA*/ 	pid_t nr = 0;
// /*AFLA*/ 	if (pid)
// /*AFLA*/ 		nr = pid->numbers[0].nr;
// /*AFLA*/ 	return nr;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ pid_t pid_nr_ns(struct pid *pid, struct pid_namespace *ns);
// /*AFLA*/ pid_t pid_vnr(struct pid *pid);
// /*AFLA*/ 
// /*AFLA*/ #define do_each_pid_task(pid, type, task)				\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		if ((pid) != NULL)					\
// /*AFLA*/ 			hlist_for_each_entry_rcu((task),		\
// /*AFLA*/ 				&(pid)->tasks[type], pids[type].node) {
// /*AFLA*/ 
// /*AFLA*/ 			/*
// /*AFLA*/ 			 * Both old and new leaders may be attached to
// /*AFLA*/ 			 * the same pid in the middle of de_thread().
// /*AFLA*/ 			 */
// /*AFLA*/ #define while_each_pid_task(pid, type, task)				\
// /*AFLA*/ 				if (type == PIDTYPE_PID)		\
// /*AFLA*/ 					break;				\
// /*AFLA*/ 			}						\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #define do_each_pid_thread(pid, type, task)				\
// /*AFLA*/ 	do_each_pid_task(pid, type, task) {				\
// /*AFLA*/ 		struct task_struct *tg___ = task;			\
// /*AFLA*/ 		do {
// /*AFLA*/ 
// /*AFLA*/ #define while_each_pid_thread(pid, type, task)				\
// /*AFLA*/ 		} while_each_thread(tg___, task);			\
// /*AFLA*/ 		task = tg___;						\
// /*AFLA*/ 	} while_each_pid_task(pid, type, task)
// /*AFLA*/ #endif /* _LINUX_PID_H */
