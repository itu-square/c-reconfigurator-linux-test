// /*AFLA*/ #ifndef _LINUX_SIGNAL_H
// /*AFLA*/ #define _LINUX_SIGNAL_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ #include <uapi/linux/signal.h>
// /*AFLA*/ 
// /*AFLA*/ struct task_struct;
// /*AFLA*/ 
// /*AFLA*/ /* for sysctl */
// /*AFLA*/ extern int print_fatal_signals;
// /*AFLA*/ /*
// /*AFLA*/  * Real Time signals may be queued.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct sigqueue {
// /*AFLA*/ 	struct list_head list;
// /*AFLA*/ 	int flags;
// /*AFLA*/ 	siginfo_t info;
// /*AFLA*/ 	struct user_struct *user;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* flags values. */
// /*AFLA*/ #define SIGQUEUE_PREALLOC	1
// /*AFLA*/ 
// /*AFLA*/ struct sigpending {
// /*AFLA*/ 	struct list_head list;
// /*AFLA*/ 	sigset_t signal;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifndef HAVE_ARCH_COPY_SIGINFO
// /*AFLA*/ 
// /*AFLA*/ #include <linux/string.h>
// /*AFLA*/ 
// /*AFLA*/ static inline void copy_siginfo(struct siginfo *to, struct siginfo *from)
// /*AFLA*/ {
// /*AFLA*/ 	if (from->si_code < 0)
// /*AFLA*/ 		memcpy(to, from, sizeof(*to));
// /*AFLA*/ 	else
// /*AFLA*/ 		/* _sigchld is currently the largest know union member */
// /*AFLA*/ 		memcpy(to, from, __ARCH_SI_PREAMBLE_SIZE + sizeof(from->_sifields._sigchld));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Define some primitives to manipulate sigset_t.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_SIG_BITOPS
// /*AFLA*/ #include <linux/bitops.h>
// /*AFLA*/ 
// /*AFLA*/ /* We don't use <linux/bitops.h> for these because there is no need to
// /*AFLA*/    be atomic.  */
// /*AFLA*/ static inline void sigaddset(sigset_t *set, int _sig)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long sig = _sig - 1;
// /*AFLA*/ 	if (_NSIG_WORDS == 1)
// /*AFLA*/ 		set->sig[0] |= 1UL << sig;
// /*AFLA*/ 	else
// /*AFLA*/ 		set->sig[sig / _NSIG_BPW] |= 1UL << (sig % _NSIG_BPW);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sigdelset(sigset_t *set, int _sig)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long sig = _sig - 1;
// /*AFLA*/ 	if (_NSIG_WORDS == 1)
// /*AFLA*/ 		set->sig[0] &= ~(1UL << sig);
// /*AFLA*/ 	else
// /*AFLA*/ 		set->sig[sig / _NSIG_BPW] &= ~(1UL << (sig % _NSIG_BPW));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sigismember(sigset_t *set, int _sig)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long sig = _sig - 1;
// /*AFLA*/ 	if (_NSIG_WORDS == 1)
// /*AFLA*/ 		return 1 & (set->sig[0] >> sig);
// /*AFLA*/ 	else
// /*AFLA*/ 		return 1 & (set->sig[sig / _NSIG_BPW] >> (sig % _NSIG_BPW));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* __HAVE_ARCH_SIG_BITOPS */
// /*AFLA*/ 
// /*AFLA*/ static inline int sigisemptyset(sigset_t *set)
// /*AFLA*/ {
// /*AFLA*/ 	switch (_NSIG_WORDS) {
// /*AFLA*/ 	case 4:
// /*AFLA*/ 		return (set->sig[3] | set->sig[2] |
// /*AFLA*/ 			set->sig[1] | set->sig[0]) == 0;
// /*AFLA*/ 	case 2:
// /*AFLA*/ 		return (set->sig[1] | set->sig[0]) == 0;
// /*AFLA*/ 	case 1:
// /*AFLA*/ 		return set->sig[0] == 0;
// /*AFLA*/ 	default:
// /*AFLA*/ 		BUILD_BUG();
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define sigmask(sig)	(1UL << ((sig) - 1))
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_SIG_SETOPS
// /*AFLA*/ #include <linux/string.h>
// /*AFLA*/ 
// /*AFLA*/ #define _SIG_SET_BINOP(name, op)					\
// /*AFLA*/ static inline void name(sigset_t *r, const sigset_t *a, const sigset_t *b) \
// /*AFLA*/ {									\
// /*AFLA*/ 	unsigned long a0, a1, a2, a3, b0, b1, b2, b3;			\
// /*AFLA*/ 									\
// /*AFLA*/ 	switch (_NSIG_WORDS) {						\
// /*AFLA*/ 	case 4:								\
// /*AFLA*/ 		a3 = a->sig[3]; a2 = a->sig[2];				\
// /*AFLA*/ 		b3 = b->sig[3]; b2 = b->sig[2];				\
// /*AFLA*/ 		r->sig[3] = op(a3, b3);					\
// /*AFLA*/ 		r->sig[2] = op(a2, b2);					\
// /*AFLA*/ 	case 2:								\
// /*AFLA*/ 		a1 = a->sig[1]; b1 = b->sig[1];				\
// /*AFLA*/ 		r->sig[1] = op(a1, b1);					\
// /*AFLA*/ 	case 1:								\
// /*AFLA*/ 		a0 = a->sig[0]; b0 = b->sig[0];				\
// /*AFLA*/ 		r->sig[0] = op(a0, b0);					\
// /*AFLA*/ 		break;							\
// /*AFLA*/ 	default:							\
// /*AFLA*/ 		BUILD_BUG();						\
// /*AFLA*/ 	}								\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define _sig_or(x,y)	((x) | (y))
// /*AFLA*/ _SIG_SET_BINOP(sigorsets, _sig_or)
// /*AFLA*/ 
// /*AFLA*/ #define _sig_and(x,y)	((x) & (y))
// /*AFLA*/ _SIG_SET_BINOP(sigandsets, _sig_and)
// /*AFLA*/ 
// /*AFLA*/ #define _sig_andn(x,y)	((x) & ~(y))
// /*AFLA*/ _SIG_SET_BINOP(sigandnsets, _sig_andn)
// /*AFLA*/ 
// /*AFLA*/ #undef _SIG_SET_BINOP
// /*AFLA*/ #undef _sig_or
// /*AFLA*/ #undef _sig_and
// /*AFLA*/ #undef _sig_andn
// /*AFLA*/ 
// /*AFLA*/ #define _SIG_SET_OP(name, op)						\
// /*AFLA*/ static inline void name(sigset_t *set)					\
// /*AFLA*/ {									\
// /*AFLA*/ 	switch (_NSIG_WORDS) {						\
// /*AFLA*/ 	case 4:	set->sig[3] = op(set->sig[3]);				\
// /*AFLA*/ 		set->sig[2] = op(set->sig[2]);				\
// /*AFLA*/ 	case 2:	set->sig[1] = op(set->sig[1]);				\
// /*AFLA*/ 	case 1:	set->sig[0] = op(set->sig[0]);				\
// /*AFLA*/ 		    break;						\
// /*AFLA*/ 	default:							\
// /*AFLA*/ 		BUILD_BUG();						\
// /*AFLA*/ 	}								\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define _sig_not(x)	(~(x))
// /*AFLA*/ _SIG_SET_OP(signotset, _sig_not)
// /*AFLA*/ 
// /*AFLA*/ #undef _SIG_SET_OP
// /*AFLA*/ #undef _sig_not
// /*AFLA*/ 
// /*AFLA*/ static inline void sigemptyset(sigset_t *set)
// /*AFLA*/ {
// /*AFLA*/ 	switch (_NSIG_WORDS) {
// /*AFLA*/ 	default:
// /*AFLA*/ 		memset(set, 0, sizeof(sigset_t));
// /*AFLA*/ 		break;
// /*AFLA*/ 	case 2: set->sig[1] = 0;
// /*AFLA*/ 	case 1:	set->sig[0] = 0;
// /*AFLA*/ 		break;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sigfillset(sigset_t *set)
// /*AFLA*/ {
// /*AFLA*/ 	switch (_NSIG_WORDS) {
// /*AFLA*/ 	default:
// /*AFLA*/ 		memset(set, -1, sizeof(sigset_t));
// /*AFLA*/ 		break;
// /*AFLA*/ 	case 2: set->sig[1] = -1;
// /*AFLA*/ 	case 1:	set->sig[0] = -1;
// /*AFLA*/ 		break;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Some extensions for manipulating the low 32 signals in particular.  */
// /*AFLA*/ 
// /*AFLA*/ static inline void sigaddsetmask(sigset_t *set, unsigned long mask)
// /*AFLA*/ {
// /*AFLA*/ 	set->sig[0] |= mask;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sigdelsetmask(sigset_t *set, unsigned long mask)
// /*AFLA*/ {
// /*AFLA*/ 	set->sig[0] &= ~mask;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sigtestsetmask(sigset_t *set, unsigned long mask)
// /*AFLA*/ {
// /*AFLA*/ 	return (set->sig[0] & mask) != 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void siginitset(sigset_t *set, unsigned long mask)
// /*AFLA*/ {
// /*AFLA*/ 	set->sig[0] = mask;
// /*AFLA*/ 	switch (_NSIG_WORDS) {
// /*AFLA*/ 	default:
// /*AFLA*/ 		memset(&set->sig[1], 0, sizeof(long)*(_NSIG_WORDS-1));
// /*AFLA*/ 		break;
// /*AFLA*/ 	case 2: set->sig[1] = 0;
// /*AFLA*/ 	case 1: ;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void siginitsetinv(sigset_t *set, unsigned long mask)
// /*AFLA*/ {
// /*AFLA*/ 	set->sig[0] = ~mask;
// /*AFLA*/ 	switch (_NSIG_WORDS) {
// /*AFLA*/ 	default:
// /*AFLA*/ 		memset(&set->sig[1], -1, sizeof(long)*(_NSIG_WORDS-1));
// /*AFLA*/ 		break;
// /*AFLA*/ 	case 2: set->sig[1] = -1;
// /*AFLA*/ 	case 1: ;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* __HAVE_ARCH_SIG_SETOPS */
// /*AFLA*/ 
// /*AFLA*/ static inline void init_sigpending(struct sigpending *sig)
// /*AFLA*/ {
// /*AFLA*/ 	sigemptyset(&sig->signal);
// /*AFLA*/ 	INIT_LIST_HEAD(&sig->list);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void flush_sigqueue(struct sigpending *queue);
// /*AFLA*/ 
// /*AFLA*/ /* Test if 'sig' is valid signal. Use this instead of testing _NSIG directly */
// /*AFLA*/ static inline int valid_signal(unsigned long sig)
// /*AFLA*/ {
// /*AFLA*/ 	return sig <= _NSIG ? 1 : 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct timespec;
// /*AFLA*/ struct pt_regs;
// /*AFLA*/ 
// /*AFLA*/ extern int next_signal(struct sigpending *pending, sigset_t *mask);
// /*AFLA*/ extern int do_send_sig_info(int sig, struct siginfo *info,
// /*AFLA*/ 				struct task_struct *p, bool group);
// /*AFLA*/ extern int group_send_sig_info(int sig, struct siginfo *info, struct task_struct *p);
// /*AFLA*/ extern int __group_send_sig_info(int, struct siginfo *, struct task_struct *);
// /*AFLA*/ extern int do_sigtimedwait(const sigset_t *, siginfo_t *,
// /*AFLA*/ 				const struct timespec *);
// /*AFLA*/ extern int sigprocmask(int, sigset_t *, sigset_t *);
// /*AFLA*/ extern void set_current_blocked(sigset_t *);
// /*AFLA*/ extern void __set_current_blocked(const sigset_t *);
// /*AFLA*/ extern int show_unhandled_signals;
// /*AFLA*/ 
// /*AFLA*/ struct sigaction {
// /*AFLA*/ #ifndef __ARCH_HAS_IRIX_SIGACTION
// /*AFLA*/ 	__sighandler_t	sa_handler;
// /*AFLA*/ 	unsigned long	sa_flags;
// /*AFLA*/ #else
// /*AFLA*/ 	unsigned int	sa_flags;
// /*AFLA*/ 	__sighandler_t	sa_handler;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef __ARCH_HAS_SA_RESTORER
// /*AFLA*/ 	__sigrestore_t sa_restorer;
// /*AFLA*/ #endif
// /*AFLA*/ 	sigset_t	sa_mask;	/* mask last for extensibility */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct k_sigaction {
// /*AFLA*/ 	struct sigaction sa;
// /*AFLA*/ #ifdef __ARCH_HAS_KA_RESTORER
// /*AFLA*/ 	__sigrestore_t ka_restorer;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/  
// /*AFLA*/ #ifdef CONFIG_OLD_SIGACTION
// /*AFLA*/ struct old_sigaction {
// /*AFLA*/ 	__sighandler_t sa_handler;
// /*AFLA*/ 	old_sigset_t sa_mask;
// /*AFLA*/ 	unsigned long sa_flags;
// /*AFLA*/ 	__sigrestore_t sa_restorer;
// /*AFLA*/ };
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct ksignal {
// /*AFLA*/ 	struct k_sigaction ka;
// /*AFLA*/ 	siginfo_t info;
// /*AFLA*/ 	int sig;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern int get_signal(struct ksignal *ksig);
// /*AFLA*/ extern void signal_setup_done(int failed, struct ksignal *ksig, int stepping);
// /*AFLA*/ extern void exit_signals(struct task_struct *tsk);
// /*AFLA*/ extern void kernel_sigaction(int, __sighandler_t);
// /*AFLA*/ 
// /*AFLA*/ static inline void allow_signal(int sig)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Kernel threads handle their own signals. Let the signal code
// /*AFLA*/ 	 * know it'll be handled, so that they don't get converted to
// /*AFLA*/ 	 * SIGKILL or just silently dropped.
// /*AFLA*/ 	 */
// /*AFLA*/ 	kernel_sigaction(sig, (__force __sighandler_t)2);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void disallow_signal(int sig)
// /*AFLA*/ {
// /*AFLA*/ 	kernel_sigaction(sig, SIG_IGN);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct kmem_cache *sighand_cachep;
// /*AFLA*/ 
// /*AFLA*/ int unhandled_signal(struct task_struct *tsk, int sig);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * In POSIX a signal is sent either to a specific thread (Linux task)
// /*AFLA*/  * or to the process as a whole (Linux thread group).  How the signal
// /*AFLA*/  * is sent determines whether it's to one thread or the whole group,
// /*AFLA*/  * which determines which signal mask(s) are involved in blocking it
// /*AFLA*/  * from being delivered until later.  When the signal is delivered,
// /*AFLA*/  * either it's caught or ignored by a user handler or it has a default
// /*AFLA*/  * effect that applies to the whole thread group (POSIX process).
// /*AFLA*/  *
// /*AFLA*/  * The possible effects an unblocked signal set to SIG_DFL can have are:
// /*AFLA*/  *   ignore	- Nothing Happens
// /*AFLA*/  *   terminate	- kill the process, i.e. all threads in the group,
// /*AFLA*/  * 		  similar to exit_group.  The group leader (only) reports
// /*AFLA*/  *		  WIFSIGNALED status to its parent.
// /*AFLA*/  *   coredump	- write a core dump file describing all threads using
// /*AFLA*/  *		  the same mm and then kill all those threads
// /*AFLA*/  *   stop 	- stop all the threads in the group, i.e. TASK_STOPPED state
// /*AFLA*/  *
// /*AFLA*/  * SIGKILL and SIGSTOP cannot be caught, blocked, or ignored.
// /*AFLA*/  * Other signals when not blocked and set to SIG_DFL behaves as follows.
// /*AFLA*/  * The job control signals also have other special effects.
// /*AFLA*/  *
// /*AFLA*/  *	+--------------------+------------------+
// /*AFLA*/  *	|  POSIX signal      |  default action  |
// /*AFLA*/  *	+--------------------+------------------+
// /*AFLA*/  *	|  SIGHUP            |  terminate	|
// /*AFLA*/  *	|  SIGINT            |	terminate	|
// /*AFLA*/  *	|  SIGQUIT           |	coredump 	|
// /*AFLA*/  *	|  SIGILL            |	coredump 	|
// /*AFLA*/  *	|  SIGTRAP           |	coredump 	|
// /*AFLA*/  *	|  SIGABRT/SIGIOT    |	coredump 	|
// /*AFLA*/  *	|  SIGBUS            |	coredump 	|
// /*AFLA*/  *	|  SIGFPE            |	coredump 	|
// /*AFLA*/  *	|  SIGKILL           |	terminate(+)	|
// /*AFLA*/  *	|  SIGUSR1           |	terminate	|
// /*AFLA*/  *	|  SIGSEGV           |	coredump 	|
// /*AFLA*/  *	|  SIGUSR2           |	terminate	|
// /*AFLA*/  *	|  SIGPIPE           |	terminate	|
// /*AFLA*/  *	|  SIGALRM           |	terminate	|
// /*AFLA*/  *	|  SIGTERM           |	terminate	|
// /*AFLA*/  *	|  SIGCHLD           |	ignore   	|
// /*AFLA*/  *	|  SIGCONT           |	ignore(*)	|
// /*AFLA*/  *	|  SIGSTOP           |	stop(*)(+)  	|
// /*AFLA*/  *	|  SIGTSTP           |	stop(*)  	|
// /*AFLA*/  *	|  SIGTTIN           |	stop(*)  	|
// /*AFLA*/  *	|  SIGTTOU           |	stop(*)  	|
// /*AFLA*/  *	|  SIGURG            |	ignore   	|
// /*AFLA*/  *	|  SIGXCPU           |	coredump 	|
// /*AFLA*/  *	|  SIGXFSZ           |	coredump 	|
// /*AFLA*/  *	|  SIGVTALRM         |	terminate	|
// /*AFLA*/  *	|  SIGPROF           |	terminate	|
// /*AFLA*/  *	|  SIGPOLL/SIGIO     |	terminate	|
// /*AFLA*/  *	|  SIGSYS/SIGUNUSED  |	coredump 	|
// /*AFLA*/  *	|  SIGSTKFLT         |	terminate	|
// /*AFLA*/  *	|  SIGWINCH          |	ignore   	|
// /*AFLA*/  *	|  SIGPWR            |	terminate	|
// /*AFLA*/  *	|  SIGRTMIN-SIGRTMAX |	terminate       |
// /*AFLA*/  *	+--------------------+------------------+
// /*AFLA*/  *	|  non-POSIX signal  |  default action  |
// /*AFLA*/  *	+--------------------+------------------+
// /*AFLA*/  *	|  SIGEMT            |  coredump	|
// /*AFLA*/  *	+--------------------+------------------+
// /*AFLA*/  *
// /*AFLA*/  * (+) For SIGKILL and SIGSTOP the action is "always", not just "default".
// /*AFLA*/  * (*) Special job control effects:
// /*AFLA*/  * When SIGCONT is sent, it resumes the process (all threads in the group)
// /*AFLA*/  * from TASK_STOPPED state and also clears any pending/queued stop signals
// /*AFLA*/  * (any of those marked with "stop(*)").  This happens regardless of blocking,
// /*AFLA*/  * catching, or ignoring SIGCONT.  When any stop signal is sent, it clears
// /*AFLA*/  * any pending/queued SIGCONT signals; this happens regardless of blocking,
// /*AFLA*/  * catching, or ignored the stop signal, though (except for SIGSTOP) the
// /*AFLA*/  * default action of stopping the process may happen later or never.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifdef SIGEMT
// /*AFLA*/ #define SIGEMT_MASK	rt_sigmask(SIGEMT)
// /*AFLA*/ #else
// /*AFLA*/ #define SIGEMT_MASK	0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if SIGRTMIN > BITS_PER_LONG
// /*AFLA*/ #define rt_sigmask(sig)	(1ULL << ((sig)-1))
// /*AFLA*/ #else
// /*AFLA*/ #define rt_sigmask(sig)	sigmask(sig)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define siginmask(sig, mask) \
// /*AFLA*/ 	((sig) < SIGRTMIN && (rt_sigmask(sig) & (mask)))
// /*AFLA*/ 
// /*AFLA*/ #define SIG_KERNEL_ONLY_MASK (\
// /*AFLA*/ 	rt_sigmask(SIGKILL)   |  rt_sigmask(SIGSTOP))
// /*AFLA*/ 
// /*AFLA*/ #define SIG_KERNEL_STOP_MASK (\
// /*AFLA*/ 	rt_sigmask(SIGSTOP)   |  rt_sigmask(SIGTSTP)   | \
// /*AFLA*/ 	rt_sigmask(SIGTTIN)   |  rt_sigmask(SIGTTOU)   )
// /*AFLA*/ 
// /*AFLA*/ #define SIG_KERNEL_COREDUMP_MASK (\
// /*AFLA*/         rt_sigmask(SIGQUIT)   |  rt_sigmask(SIGILL)    | \
// /*AFLA*/ 	rt_sigmask(SIGTRAP)   |  rt_sigmask(SIGABRT)   | \
// /*AFLA*/         rt_sigmask(SIGFPE)    |  rt_sigmask(SIGSEGV)   | \
// /*AFLA*/ 	rt_sigmask(SIGBUS)    |  rt_sigmask(SIGSYS)    | \
// /*AFLA*/         rt_sigmask(SIGXCPU)   |  rt_sigmask(SIGXFSZ)   | \
// /*AFLA*/ 	SIGEMT_MASK				       )
// /*AFLA*/ 
// /*AFLA*/ #define SIG_KERNEL_IGNORE_MASK (\
// /*AFLA*/         rt_sigmask(SIGCONT)   |  rt_sigmask(SIGCHLD)   | \
// /*AFLA*/ 	rt_sigmask(SIGWINCH)  |  rt_sigmask(SIGURG)    )
// /*AFLA*/ 
// /*AFLA*/ #define sig_kernel_only(sig)		siginmask(sig, SIG_KERNEL_ONLY_MASK)
// /*AFLA*/ #define sig_kernel_coredump(sig)	siginmask(sig, SIG_KERNEL_COREDUMP_MASK)
// /*AFLA*/ #define sig_kernel_ignore(sig)		siginmask(sig, SIG_KERNEL_IGNORE_MASK)
// /*AFLA*/ #define sig_kernel_stop(sig)		siginmask(sig, SIG_KERNEL_STOP_MASK)
// /*AFLA*/ 
// /*AFLA*/ #define sig_user_defined(t, signr) \
// /*AFLA*/ 	(((t)->sighand->action[(signr)-1].sa.sa_handler != SIG_DFL) &&	\
// /*AFLA*/ 	 ((t)->sighand->action[(signr)-1].sa.sa_handler != SIG_IGN))
// /*AFLA*/ 
// /*AFLA*/ #define sig_fatal(t, signr) \
// /*AFLA*/ 	(!siginmask(signr, SIG_KERNEL_IGNORE_MASK|SIG_KERNEL_STOP_MASK) && \
// /*AFLA*/ 	 (t)->sighand->action[(signr)-1].sa.sa_handler == SIG_DFL)
// /*AFLA*/ 
// /*AFLA*/ void signals_init(void);
// /*AFLA*/ 
// /*AFLA*/ int restore_altstack(const stack_t __user *);
// /*AFLA*/ int __save_altstack(stack_t __user *, unsigned long);
// /*AFLA*/ 
// /*AFLA*/ #define save_altstack_ex(uss, sp) do { \
// /*AFLA*/ 	stack_t __user *__uss = uss; \
// /*AFLA*/ 	struct task_struct *t = current; \
// /*AFLA*/ 	put_user_ex((void __user *)t->sas_ss_sp, &__uss->ss_sp); \
// /*AFLA*/ 	put_user_ex(t->sas_ss_flags, &__uss->ss_flags); \
// /*AFLA*/ 	put_user_ex(t->sas_ss_size, &__uss->ss_size); \
// /*AFLA*/ 	if (t->sas_ss_flags & SS_AUTODISARM) \
// /*AFLA*/ 		sas_ss_reset(t); \
// /*AFLA*/ } while (0);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PROC_FS
// /*AFLA*/ struct seq_file;
// /*AFLA*/ extern void render_sigset_t(struct seq_file *, const char *, sigset_t *);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_SIGNAL_H */
