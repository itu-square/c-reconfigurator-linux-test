// /*AFLA*/ #ifndef _LINUX_PTRACE_H
// /*AFLA*/ #define _LINUX_PTRACE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>		/* For unlikely.  */
// /*AFLA*/ #include <linux/sched.h>		/* For struct task_struct.  */
// /*AFLA*/ #include <linux/err.h>			/* for IS_ERR_VALUE */
// /*AFLA*/ #include <linux/bug.h>			/* For BUG_ON.  */
// /*AFLA*/ #include <linux/pid_namespace.h>	/* For task_active_pid_ns.  */
// /*AFLA*/ #include <uapi/linux/ptrace.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Ptrace flags
// /*AFLA*/  *
// /*AFLA*/  * The owner ship rules for task->ptrace which holds the ptrace
// /*AFLA*/  * flags is simple.  When a task is running it owns it's task->ptrace
// /*AFLA*/  * flags.  When the a task is stopped the ptracer owns task->ptrace.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define PT_SEIZED	0x00010000	/* SEIZE used, enable new behavior */
// /*AFLA*/ #define PT_PTRACED	0x00000001
// /*AFLA*/ #define PT_DTRACE	0x00000002	/* delayed trace (used on m68k, i386) */
// /*AFLA*/ #define PT_PTRACE_CAP	0x00000004	/* ptracer can follow suid-exec */
// /*AFLA*/ 
// /*AFLA*/ #define PT_OPT_FLAG_SHIFT	3
// /*AFLA*/ /* PT_TRACE_* event enable flags */
// /*AFLA*/ #define PT_EVENT_FLAG(event)	(1 << (PT_OPT_FLAG_SHIFT + (event)))
// /*AFLA*/ #define PT_TRACESYSGOOD		PT_EVENT_FLAG(0)
// /*AFLA*/ #define PT_TRACE_FORK		PT_EVENT_FLAG(PTRACE_EVENT_FORK)
// /*AFLA*/ #define PT_TRACE_VFORK		PT_EVENT_FLAG(PTRACE_EVENT_VFORK)
// /*AFLA*/ #define PT_TRACE_CLONE		PT_EVENT_FLAG(PTRACE_EVENT_CLONE)
// /*AFLA*/ #define PT_TRACE_EXEC		PT_EVENT_FLAG(PTRACE_EVENT_EXEC)
// /*AFLA*/ #define PT_TRACE_VFORK_DONE	PT_EVENT_FLAG(PTRACE_EVENT_VFORK_DONE)
// /*AFLA*/ #define PT_TRACE_EXIT		PT_EVENT_FLAG(PTRACE_EVENT_EXIT)
// /*AFLA*/ #define PT_TRACE_SECCOMP	PT_EVENT_FLAG(PTRACE_EVENT_SECCOMP)
// /*AFLA*/ 
// /*AFLA*/ #define PT_EXITKILL		(PTRACE_O_EXITKILL << PT_OPT_FLAG_SHIFT)
// /*AFLA*/ #define PT_SUSPEND_SECCOMP	(PTRACE_O_SUSPEND_SECCOMP << PT_OPT_FLAG_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ /* single stepping state bits (used on ARM and PA-RISC) */
// /*AFLA*/ #define PT_SINGLESTEP_BIT	31
// /*AFLA*/ #define PT_SINGLESTEP		(1<<PT_SINGLESTEP_BIT)
// /*AFLA*/ #define PT_BLOCKSTEP_BIT	30
// /*AFLA*/ #define PT_BLOCKSTEP		(1<<PT_BLOCKSTEP_BIT)
// /*AFLA*/ 
// /*AFLA*/ extern long arch_ptrace(struct task_struct *child, long request,
// /*AFLA*/ 			unsigned long addr, unsigned long data);
// /*AFLA*/ extern int ptrace_readdata(struct task_struct *tsk, unsigned long src, char __user *dst, int len);
// /*AFLA*/ extern int ptrace_writedata(struct task_struct *tsk, char __user *src, unsigned long dst, int len);
// /*AFLA*/ extern void ptrace_disable(struct task_struct *);
// /*AFLA*/ extern int ptrace_request(struct task_struct *child, long request,
// /*AFLA*/ 			  unsigned long addr, unsigned long data);
// /*AFLA*/ extern void ptrace_notify(int exit_code);
// /*AFLA*/ extern void __ptrace_link(struct task_struct *child,
// /*AFLA*/ 			  struct task_struct *new_parent);
// /*AFLA*/ extern void __ptrace_unlink(struct task_struct *child);
// /*AFLA*/ extern void exit_ptrace(struct task_struct *tracer, struct list_head *dead);
// /*AFLA*/ #define PTRACE_MODE_READ	0x01
// /*AFLA*/ #define PTRACE_MODE_ATTACH	0x02
// /*AFLA*/ #define PTRACE_MODE_NOAUDIT	0x04
// /*AFLA*/ #define PTRACE_MODE_FSCREDS 0x08
// /*AFLA*/ #define PTRACE_MODE_REALCREDS 0x10
// /*AFLA*/ 
// /*AFLA*/ /* shorthands for READ/ATTACH and FSCREDS/REALCREDS combinations */
// /*AFLA*/ #define PTRACE_MODE_READ_FSCREDS (PTRACE_MODE_READ | PTRACE_MODE_FSCREDS)
// /*AFLA*/ #define PTRACE_MODE_READ_REALCREDS (PTRACE_MODE_READ | PTRACE_MODE_REALCREDS)
// /*AFLA*/ #define PTRACE_MODE_ATTACH_FSCREDS (PTRACE_MODE_ATTACH | PTRACE_MODE_FSCREDS)
// /*AFLA*/ #define PTRACE_MODE_ATTACH_REALCREDS (PTRACE_MODE_ATTACH | PTRACE_MODE_REALCREDS)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ptrace_may_access - check whether the caller is permitted to access
// /*AFLA*/  * a target task.
// /*AFLA*/  * @task: target task
// /*AFLA*/  * @mode: selects type of access and caller credentials
// /*AFLA*/  *
// /*AFLA*/  * Returns true on success, false on denial.
// /*AFLA*/  *
// /*AFLA*/  * One of the flags PTRACE_MODE_FSCREDS and PTRACE_MODE_REALCREDS must
// /*AFLA*/  * be set in @mode to specify whether the access was requested through
// /*AFLA*/  * a filesystem syscall (should use effective capabilities and fsuid
// /*AFLA*/  * of the caller) or through an explicit syscall such as
// /*AFLA*/  * process_vm_writev or ptrace (and should use the real credentials).
// /*AFLA*/  */
// /*AFLA*/ extern bool ptrace_may_access(struct task_struct *task, unsigned int mode);
// /*AFLA*/ 
// /*AFLA*/ static inline int ptrace_reparented(struct task_struct *child)
// /*AFLA*/ {
// /*AFLA*/ 	return !same_thread_group(child->real_parent, child->parent);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ptrace_unlink(struct task_struct *child)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(child->ptrace))
// /*AFLA*/ 		__ptrace_unlink(child);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ int generic_ptrace_peekdata(struct task_struct *tsk, unsigned long addr,
// /*AFLA*/ 			    unsigned long data);
// /*AFLA*/ int generic_ptrace_pokedata(struct task_struct *tsk, unsigned long addr,
// /*AFLA*/ 			    unsigned long data);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ptrace_parent - return the task that is tracing the given task
// /*AFLA*/  * @task: task to consider
// /*AFLA*/  *
// /*AFLA*/  * Returns %NULL if no one is tracing @task, or the &struct task_struct
// /*AFLA*/  * pointer to its tracer.
// /*AFLA*/  *
// /*AFLA*/  * Must called under rcu_read_lock().  The pointer returned might be kept
// /*AFLA*/  * live only by RCU.  During exec, this may be called with task_lock() held
// /*AFLA*/  * on @task, still held from when check_unsafe_exec() was called.
// /*AFLA*/  */
// /*AFLA*/ static inline struct task_struct *ptrace_parent(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(task->ptrace))
// /*AFLA*/ 		return rcu_dereference(task->parent);
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ptrace_event_enabled - test whether a ptrace event is enabled
// /*AFLA*/  * @task: ptracee of interest
// /*AFLA*/  * @event: %PTRACE_EVENT_* to test
// /*AFLA*/  *
// /*AFLA*/  * Test whether @event is enabled for ptracee @task.
// /*AFLA*/  *
// /*AFLA*/  * Returns %true if @event is enabled, %false otherwise.
// /*AFLA*/  */
// /*AFLA*/ static inline bool ptrace_event_enabled(struct task_struct *task, int event)
// /*AFLA*/ {
// /*AFLA*/ 	return task->ptrace & PT_EVENT_FLAG(event);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ptrace_event - possibly stop for a ptrace event notification
// /*AFLA*/  * @event:	%PTRACE_EVENT_* value to report
// /*AFLA*/  * @message:	value for %PTRACE_GETEVENTMSG to return
// /*AFLA*/  *
// /*AFLA*/  * Check whether @event is enabled and, if so, report @event and @message
// /*AFLA*/  * to the ptrace parent.
// /*AFLA*/  *
// /*AFLA*/  * Called without locks.
// /*AFLA*/  */
// /*AFLA*/ static inline void ptrace_event(int event, unsigned long message)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(ptrace_event_enabled(current, event))) {
// /*AFLA*/ 		current->ptrace_message = message;
// /*AFLA*/ 		ptrace_notify((event << 8) | SIGTRAP);
// /*AFLA*/ 	} else if (event == PTRACE_EVENT_EXEC) {
// /*AFLA*/ 		/* legacy EXEC report via SIGTRAP */
// /*AFLA*/ 		if ((current->ptrace & (PT_PTRACED|PT_SEIZED)) == PT_PTRACED)
// /*AFLA*/ 			send_sig(SIGTRAP, current, 0);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ptrace_event_pid - possibly stop for a ptrace event notification
// /*AFLA*/  * @event:	%PTRACE_EVENT_* value to report
// /*AFLA*/  * @pid:	process identifier for %PTRACE_GETEVENTMSG to return
// /*AFLA*/  *
// /*AFLA*/  * Check whether @event is enabled and, if so, report @event and @pid
// /*AFLA*/  * to the ptrace parent.  @pid is reported as the pid_t seen from the
// /*AFLA*/  * the ptrace parent's pid namespace.
// /*AFLA*/  *
// /*AFLA*/  * Called without locks.
// /*AFLA*/  */
// /*AFLA*/ static inline void ptrace_event_pid(int event, struct pid *pid)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * FIXME: There's a potential race if a ptracer in a different pid
// /*AFLA*/ 	 * namespace than parent attaches between computing message below and
// /*AFLA*/ 	 * when we acquire tasklist_lock in ptrace_stop().  If this happens,
// /*AFLA*/ 	 * the ptracer will get a bogus pid from PTRACE_GETEVENTMSG.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long message = 0;
// /*AFLA*/ 	struct pid_namespace *ns;
// /*AFLA*/ 
// /*AFLA*/ 	rcu_read_lock();
// /*AFLA*/ 	ns = task_active_pid_ns(rcu_dereference(current->parent));
// /*AFLA*/ 	if (ns)
// /*AFLA*/ 		message = pid_nr_ns(pid, ns);
// /*AFLA*/ 	rcu_read_unlock();
// /*AFLA*/ 
// /*AFLA*/ 	ptrace_event(event, message);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ptrace_init_task - initialize ptrace state for a new child
// /*AFLA*/  * @child:		new child task
// /*AFLA*/  * @ptrace:		true if child should be ptrace'd by parent's tracer
// /*AFLA*/  *
// /*AFLA*/  * This is called immediately after adding @child to its parent's children
// /*AFLA*/  * list.  @ptrace is false in the normal case, and true to ptrace @child.
// /*AFLA*/  *
// /*AFLA*/  * Called with current's siglock and write_lock_irq(&tasklist_lock) held.
// /*AFLA*/  */
// /*AFLA*/ static inline void ptrace_init_task(struct task_struct *child, bool ptrace)
// /*AFLA*/ {
// /*AFLA*/ 	INIT_LIST_HEAD(&child->ptrace_entry);
// /*AFLA*/ 	INIT_LIST_HEAD(&child->ptraced);
// /*AFLA*/ 	child->jobctl = 0;
// /*AFLA*/ 	child->ptrace = 0;
// /*AFLA*/ 	child->parent = child->real_parent;
// /*AFLA*/ 
// /*AFLA*/ 	if (unlikely(ptrace) && current->ptrace) {
// /*AFLA*/ 		child->ptrace = current->ptrace;
// /*AFLA*/ 		__ptrace_link(child, current->parent);
// /*AFLA*/ 
// /*AFLA*/ 		if (child->ptrace & PT_SEIZED)
// /*AFLA*/ 			task_set_jobctl_pending(child, JOBCTL_TRAP_STOP);
// /*AFLA*/ 		else
// /*AFLA*/ 			sigaddset(&child->pending.signal, SIGSTOP);
// /*AFLA*/ 
// /*AFLA*/ 		set_tsk_thread_flag(child, TIF_SIGPENDING);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * ptrace_release_task - final ptrace-related cleanup of a zombie being reaped
// /*AFLA*/  * @task:	task in %EXIT_DEAD state
// /*AFLA*/  *
// /*AFLA*/  * Called with write_lock(&tasklist_lock) held.
// /*AFLA*/  */
// /*AFLA*/ static inline void ptrace_release_task(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	BUG_ON(!list_empty(&task->ptraced));
// /*AFLA*/ 	ptrace_unlink(task);
// /*AFLA*/ 	BUG_ON(!list_empty(&task->ptrace_entry));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifndef force_successful_syscall_return
// /*AFLA*/ /*
// /*AFLA*/  * System call handlers that, upon successful completion, need to return a
// /*AFLA*/  * negative value should call force_successful_syscall_return() right before
// /*AFLA*/  * returning.  On architectures where the syscall convention provides for a
// /*AFLA*/  * separate error flag (e.g., alpha, ia64, ppc{,64}, sparc{,64}, possibly
// /*AFLA*/  * others), this macro can be used to ensure that the error flag will not get
// /*AFLA*/  * set.  On architectures which do not support a separate error flag, the macro
// /*AFLA*/  * is a no-op and the spurious error condition needs to be filtered out by some
// /*AFLA*/  * other means (e.g., in user-level, by passing an extra argument to the
// /*AFLA*/  * syscall handler, or something along those lines).
// /*AFLA*/  */
// /*AFLA*/ #define force_successful_syscall_return() do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef is_syscall_success
// /*AFLA*/ /*
// /*AFLA*/  * On most systems we can tell if a syscall is a success based on if the retval
// /*AFLA*/  * is an error value.  On some systems like ia64 and powerpc they have different
// /*AFLA*/  * indicators of success/failure and must define their own.
// /*AFLA*/  */
// /*AFLA*/ #define is_syscall_success(regs) (!IS_ERR_VALUE((unsigned long)(regs_return_value(regs))))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * <asm/ptrace.h> should define the following things inside #ifdef __KERNEL__.
// /*AFLA*/  *
// /*AFLA*/  * These do-nothing inlines are used when the arch does not
// /*AFLA*/  * implement single-step.  The kerneldoc comments are here
// /*AFLA*/  * to document the interface for all arch definitions.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef arch_has_single_step
// /*AFLA*/ /**
// /*AFLA*/  * arch_has_single_step - does this CPU support user-mode single-step?
// /*AFLA*/  *
// /*AFLA*/  * If this is defined, then there must be function declarations or
// /*AFLA*/  * inlines for user_enable_single_step() and user_disable_single_step().
// /*AFLA*/  * arch_has_single_step() should evaluate to nonzero iff the machine
// /*AFLA*/  * supports instruction single-step for user mode.
// /*AFLA*/  * It can be a constant or it can test a CPU feature bit.
// /*AFLA*/  */
// /*AFLA*/ #define arch_has_single_step()		(0)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * user_enable_single_step - single-step in user-mode task
// /*AFLA*/  * @task: either current or a task stopped in %TASK_TRACED
// /*AFLA*/  *
// /*AFLA*/  * This can only be called when arch_has_single_step() has returned nonzero.
// /*AFLA*/  * Set @task so that when it returns to user mode, it will trap after the
// /*AFLA*/  * next single instruction executes.  If arch_has_block_step() is defined,
// /*AFLA*/  * this must clear the effects of user_enable_block_step() too.
// /*AFLA*/  */
// /*AFLA*/ static inline void user_enable_single_step(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	BUG();			/* This can never be called.  */
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * user_disable_single_step - cancel user-mode single-step
// /*AFLA*/  * @task: either current or a task stopped in %TASK_TRACED
// /*AFLA*/  *
// /*AFLA*/  * Clear @task of the effects of user_enable_single_step() and
// /*AFLA*/  * user_enable_block_step().  This can be called whether or not either
// /*AFLA*/  * of those was ever called on @task, and even if arch_has_single_step()
// /*AFLA*/  * returned zero.
// /*AFLA*/  */
// /*AFLA*/ static inline void user_disable_single_step(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ extern void user_enable_single_step(struct task_struct *);
// /*AFLA*/ extern void user_disable_single_step(struct task_struct *);
// /*AFLA*/ #endif	/* arch_has_single_step */
// /*AFLA*/ 
// /*AFLA*/ #ifndef arch_has_block_step
// /*AFLA*/ /**
// /*AFLA*/  * arch_has_block_step - does this CPU support user-mode block-step?
// /*AFLA*/  *
// /*AFLA*/  * If this is defined, then there must be a function declaration or inline
// /*AFLA*/  * for user_enable_block_step(), and arch_has_single_step() must be defined
// /*AFLA*/  * too.  arch_has_block_step() should evaluate to nonzero iff the machine
// /*AFLA*/  * supports step-until-branch for user mode.  It can be a constant or it
// /*AFLA*/  * can test a CPU feature bit.
// /*AFLA*/  */
// /*AFLA*/ #define arch_has_block_step()		(0)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * user_enable_block_step - step until branch in user-mode task
// /*AFLA*/  * @task: either current or a task stopped in %TASK_TRACED
// /*AFLA*/  *
// /*AFLA*/  * This can only be called when arch_has_block_step() has returned nonzero,
// /*AFLA*/  * and will never be called when single-instruction stepping is being used.
// /*AFLA*/  * Set @task so that when it returns to user mode, it will trap after the
// /*AFLA*/  * next branch or trap taken.
// /*AFLA*/  */
// /*AFLA*/ static inline void user_enable_block_step(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	BUG();			/* This can never be called.  */
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ extern void user_enable_block_step(struct task_struct *);
// /*AFLA*/ #endif	/* arch_has_block_step */
// /*AFLA*/ 
// /*AFLA*/ #ifdef ARCH_HAS_USER_SINGLE_STEP_INFO
// /*AFLA*/ extern void user_single_step_siginfo(struct task_struct *tsk,
// /*AFLA*/ 				struct pt_regs *regs, siginfo_t *info);
// /*AFLA*/ #else
// /*AFLA*/ static inline void user_single_step_siginfo(struct task_struct *tsk,
// /*AFLA*/ 				struct pt_regs *regs, siginfo_t *info)
// /*AFLA*/ {
// /*AFLA*/ 	memset(info, 0, sizeof(*info));
// /*AFLA*/ 	info->si_signo = SIGTRAP;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef arch_ptrace_stop_needed
// /*AFLA*/ /**
// /*AFLA*/  * arch_ptrace_stop_needed - Decide whether arch_ptrace_stop() should be called
// /*AFLA*/  * @code:	current->exit_code value ptrace will stop with
// /*AFLA*/  * @info:	siginfo_t pointer (or %NULL) for signal ptrace will stop with
// /*AFLA*/  *
// /*AFLA*/  * This is called with the siglock held, to decide whether or not it's
// /*AFLA*/  * necessary to release the siglock and call arch_ptrace_stop() with the
// /*AFLA*/  * same @code and @info arguments.  It can be defined to a constant if
// /*AFLA*/  * arch_ptrace_stop() is never required, or always is.  On machines where
// /*AFLA*/  * this makes sense, it should be defined to a quick test to optimize out
// /*AFLA*/  * calling arch_ptrace_stop() when it would be superfluous.  For example,
// /*AFLA*/  * if the thread has not been back to user mode since the last stop, the
// /*AFLA*/  * thread state might indicate that nothing needs to be done.
// /*AFLA*/  *
// /*AFLA*/  * This is guaranteed to be invoked once before a task stops for ptrace and
// /*AFLA*/  * may include arch-specific operations necessary prior to a ptrace stop.
// /*AFLA*/  */
// /*AFLA*/ #define arch_ptrace_stop_needed(code, info)	(0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef arch_ptrace_stop
// /*AFLA*/ /**
// /*AFLA*/  * arch_ptrace_stop - Do machine-specific work before stopping for ptrace
// /*AFLA*/  * @code:	current->exit_code value ptrace will stop with
// /*AFLA*/  * @info:	siginfo_t pointer (or %NULL) for signal ptrace will stop with
// /*AFLA*/  *
// /*AFLA*/  * This is called with no locks held when arch_ptrace_stop_needed() has
// /*AFLA*/  * just returned nonzero.  It is allowed to block, e.g. for user memory
// /*AFLA*/  * access.  The arch can have machine-specific work to be done before
// /*AFLA*/  * ptrace stops.  On ia64, register backing store gets written back to user
// /*AFLA*/  * memory here.  Since this can be costly (requires dropping the siglock),
// /*AFLA*/  * we only do it when the arch requires it for this particular stop, as
// /*AFLA*/  * indicated by arch_ptrace_stop_needed().
// /*AFLA*/  */
// /*AFLA*/ #define arch_ptrace_stop(code, info)		do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef current_pt_regs
// /*AFLA*/ #define current_pt_regs() task_pt_regs(current)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef ptrace_signal_deliver
// /*AFLA*/ #define ptrace_signal_deliver() ((void)0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * unlike current_pt_regs(), this one is equal to task_pt_regs(current)
// /*AFLA*/  * on *all* architectures; the only reason to have a per-arch definition
// /*AFLA*/  * is optimisation.
// /*AFLA*/  */
// /*AFLA*/ #ifndef signal_pt_regs
// /*AFLA*/ #define signal_pt_regs() task_pt_regs(current)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef current_user_stack_pointer
// /*AFLA*/ #define current_user_stack_pointer() user_stack_pointer(current_pt_regs())
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern int task_current_syscall(struct task_struct *target, long *callno,
// /*AFLA*/ 				unsigned long args[6], unsigned int maxargs,
// /*AFLA*/ 				unsigned long *sp, unsigned long *pc);
// /*AFLA*/ 
// /*AFLA*/ #endif
