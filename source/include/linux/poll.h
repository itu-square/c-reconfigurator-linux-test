// /*AFLA*/ #ifndef _LINUX_POLL_H
// /*AFLA*/ #define _LINUX_POLL_H
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/ktime.h>
// /*AFLA*/ #include <linux/wait.h>
// /*AFLA*/ #include <linux/string.h>
// /*AFLA*/ #include <linux/fs.h>
// /*AFLA*/ #include <linux/sysctl.h>
// /*AFLA*/ #include <asm/uaccess.h>
// /*AFLA*/ #include <uapi/linux/poll.h>
// /*AFLA*/ 
// /*AFLA*/ extern struct ctl_table epoll_table[]; /* for sysctl */
// /*AFLA*/ /* ~832 bytes of stack space used max in sys_select/sys_poll before allocating
// /*AFLA*/    additional memory. */
// /*AFLA*/ #define MAX_STACK_ALLOC 832
// /*AFLA*/ #define FRONTEND_STACK_ALLOC	256
// /*AFLA*/ #define SELECT_STACK_ALLOC	FRONTEND_STACK_ALLOC
// /*AFLA*/ #define POLL_STACK_ALLOC	FRONTEND_STACK_ALLOC
// /*AFLA*/ #define WQUEUES_STACK_ALLOC	(MAX_STACK_ALLOC - FRONTEND_STACK_ALLOC)
// /*AFLA*/ #define N_INLINE_POLL_ENTRIES	(WQUEUES_STACK_ALLOC / sizeof(struct poll_table_entry))
// /*AFLA*/ 
// /*AFLA*/ #define DEFAULT_POLLMASK (POLLIN | POLLOUT | POLLRDNORM | POLLWRNORM)
// /*AFLA*/ 
// /*AFLA*/ struct poll_table_struct;
// /*AFLA*/ 
// /*AFLA*/ /* 
// /*AFLA*/  * structures and helpers for f_op->poll implementations
// /*AFLA*/  */
// /*AFLA*/ typedef void (*poll_queue_proc)(struct file *, wait_queue_head_t *, struct poll_table_struct *);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Do not touch the structure directly, use the access functions
// /*AFLA*/  * poll_does_not_wait() and poll_requested_events() instead.
// /*AFLA*/  */
// /*AFLA*/ typedef struct poll_table_struct {
// /*AFLA*/ 	poll_queue_proc _qproc;
// /*AFLA*/ 	unsigned long _key;
// /*AFLA*/ } poll_table;
// /*AFLA*/ 
// /*AFLA*/ static inline void poll_wait(struct file * filp, wait_queue_head_t * wait_address, poll_table *p)
// /*AFLA*/ {
// /*AFLA*/ 	if (p && p->_qproc && wait_address)
// /*AFLA*/ 		p->_qproc(filp, wait_address, p);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Return true if it is guaranteed that poll will not wait. This is the case
// /*AFLA*/  * if the poll() of another file descriptor in the set got an event, so there
// /*AFLA*/  * is no need for waiting.
// /*AFLA*/  */
// /*AFLA*/ static inline bool poll_does_not_wait(const poll_table *p)
// /*AFLA*/ {
// /*AFLA*/ 	return p == NULL || p->_qproc == NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Return the set of events that the application wants to poll for.
// /*AFLA*/  * This is useful for drivers that need to know whether a DMA transfer has
// /*AFLA*/  * to be started implicitly on poll(). You typically only want to do that
// /*AFLA*/  * if the application is actually polling for POLLIN and/or POLLOUT.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long poll_requested_events(const poll_table *p)
// /*AFLA*/ {
// /*AFLA*/ 	return p ? p->_key : ~0UL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void init_poll_funcptr(poll_table *pt, poll_queue_proc qproc)
// /*AFLA*/ {
// /*AFLA*/ 	pt->_qproc = qproc;
// /*AFLA*/ 	pt->_key   = ~0UL; /* all events enabled */
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct poll_table_entry {
// /*AFLA*/ 	struct file *filp;
// /*AFLA*/ 	unsigned long key;
// /*AFLA*/ 	wait_queue_t wait;
// /*AFLA*/ 	wait_queue_head_t *wait_address;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Structures and helpers for select/poll syscall
// /*AFLA*/  */
// /*AFLA*/ struct poll_wqueues {
// /*AFLA*/ 	poll_table pt;
// /*AFLA*/ 	struct poll_table_page *table;
// /*AFLA*/ 	struct task_struct *polling_task;
// /*AFLA*/ 	int triggered;
// /*AFLA*/ 	int error;
// /*AFLA*/ 	int inline_index;
// /*AFLA*/ 	struct poll_table_entry inline_entries[N_INLINE_POLL_ENTRIES];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern void poll_initwait(struct poll_wqueues *pwq);
// /*AFLA*/ extern void poll_freewait(struct poll_wqueues *pwq);
// /*AFLA*/ extern int poll_schedule_timeout(struct poll_wqueues *pwq, int state,
// /*AFLA*/ 				 ktime_t *expires, unsigned long slack);
// /*AFLA*/ extern u64 select_estimate_accuracy(struct timespec64 *tv);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ static inline int poll_schedule(struct poll_wqueues *pwq, int state)
// /*AFLA*/ {
// /*AFLA*/ 	return poll_schedule_timeout(pwq, state, NULL, 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Scalable version of the fd_set.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	unsigned long *in, *out, *ex;
// /*AFLA*/ 	unsigned long *res_in, *res_out, *res_ex;
// /*AFLA*/ } fd_set_bits;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * How many longwords for "nr" bits?
// /*AFLA*/  */
// /*AFLA*/ #define FDS_BITPERLONG	(8*sizeof(long))
// /*AFLA*/ #define FDS_LONGS(nr)	(((nr)+FDS_BITPERLONG-1)/FDS_BITPERLONG)
// /*AFLA*/ #define FDS_BYTES(nr)	(FDS_LONGS(nr)*sizeof(long))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We do a VERIFY_WRITE here even though we are only reading this time:
// /*AFLA*/  * we'll write to it eventually..
// /*AFLA*/  *
// /*AFLA*/  * Use "unsigned long" accesses to let user-mode fd_set's be long-aligned.
// /*AFLA*/  */
// /*AFLA*/ static inline
// /*AFLA*/ int get_fd_set(unsigned long nr, void __user *ufdset, unsigned long *fdset)
// /*AFLA*/ {
// /*AFLA*/ 	nr = FDS_BYTES(nr);
// /*AFLA*/ 	if (ufdset)
// /*AFLA*/ 		return copy_from_user(fdset, ufdset, nr) ? -EFAULT : 0;
// /*AFLA*/ 
// /*AFLA*/ 	memset(fdset, 0, nr);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long __must_check
// /*AFLA*/ set_fd_set(unsigned long nr, void __user *ufdset, unsigned long *fdset)
// /*AFLA*/ {
// /*AFLA*/ 	if (ufdset)
// /*AFLA*/ 		return __copy_to_user(ufdset, fdset, FDS_BYTES(nr));
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline
// /*AFLA*/ void zero_fd_set(unsigned long nr, unsigned long *fdset)
// /*AFLA*/ {
// /*AFLA*/ 	memset(fdset, 0, FDS_BYTES(nr));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define MAX_INT64_SECONDS (((s64)(~((u64)0)>>1)/HZ)-1)
// /*AFLA*/ 
// /*AFLA*/ extern int do_select(int n, fd_set_bits *fds, struct timespec64 *end_time);
// /*AFLA*/ extern int do_sys_poll(struct pollfd __user * ufds, unsigned int nfds,
// /*AFLA*/ 		       struct timespec64 *end_time);
// /*AFLA*/ extern int core_sys_select(int n, fd_set __user *inp, fd_set __user *outp,
// /*AFLA*/ 			   fd_set __user *exp, struct timespec64 *end_time);
// /*AFLA*/ 
// /*AFLA*/ extern int poll_select_set_timeout(struct timespec64 *to, time64_t sec,
// /*AFLA*/ 				   long nsec);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_POLL_H */
