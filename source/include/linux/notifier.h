// /*AFLA*/ /*
// /*AFLA*/  *	Routines to manage notifier chains for passing status changes to any
// /*AFLA*/  *	interested routines. We need this instead of hard coded call lists so
// /*AFLA*/  *	that modules can poke their nose into the innards. The network devices
// /*AFLA*/  *	needed them so here they are for the rest of you.
// /*AFLA*/  *
// /*AFLA*/  *				Alan Cox <Alan.Cox@linux.org>
// /*AFLA*/  */
// /*AFLA*/  
// /*AFLA*/ #ifndef _LINUX_NOTIFIER_H
// /*AFLA*/ #define _LINUX_NOTIFIER_H
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ #include <linux/mutex.h>
// /*AFLA*/ #include <linux/rwsem.h>
// /*AFLA*/ #include <linux/srcu.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Notifier chains are of four types:
// /*AFLA*/  *
// /*AFLA*/  *	Atomic notifier chains: Chain callbacks run in interrupt/atomic
// /*AFLA*/  *		context. Callouts are not allowed to block.
// /*AFLA*/  *	Blocking notifier chains: Chain callbacks run in process context.
// /*AFLA*/  *		Callouts are allowed to block.
// /*AFLA*/  *	Raw notifier chains: There are no restrictions on callbacks,
// /*AFLA*/  *		registration, or unregistration.  All locking and protection
// /*AFLA*/  *		must be provided by the caller.
// /*AFLA*/  *	SRCU notifier chains: A variant of blocking notifier chains, with
// /*AFLA*/  *		the same restrictions.
// /*AFLA*/  *
// /*AFLA*/  * atomic_notifier_chain_register() may be called from an atomic context,
// /*AFLA*/  * but blocking_notifier_chain_register() and srcu_notifier_chain_register()
// /*AFLA*/  * must be called from a process context.  Ditto for the corresponding
// /*AFLA*/  * _unregister() routines.
// /*AFLA*/  *
// /*AFLA*/  * atomic_notifier_chain_unregister(), blocking_notifier_chain_unregister(),
// /*AFLA*/  * and srcu_notifier_chain_unregister() _must not_ be called from within
// /*AFLA*/  * the call chain.
// /*AFLA*/  *
// /*AFLA*/  * SRCU notifier chains are an alternative form of blocking notifier chains.
// /*AFLA*/  * They use SRCU (Sleepable Read-Copy Update) instead of rw-semaphores for
// /*AFLA*/  * protection of the chain links.  This means there is _very_ low overhead
// /*AFLA*/  * in srcu_notifier_call_chain(): no cache bounces and no memory barriers.
// /*AFLA*/  * As compensation, srcu_notifier_chain_unregister() is rather expensive.
// /*AFLA*/  * SRCU notifier chains should be used when the chain will be called very
// /*AFLA*/  * often but notifier_blocks will seldom be removed.  Also, SRCU notifier
// /*AFLA*/  * chains are slightly more difficult to use because they require special
// /*AFLA*/  * runtime initialization.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct notifier_block;
// /*AFLA*/ 
// /*AFLA*/ typedef	int (*notifier_fn_t)(struct notifier_block *nb,
// /*AFLA*/ 			unsigned long action, void *data);
// /*AFLA*/ 
// /*AFLA*/ struct notifier_block {
// /*AFLA*/ 	notifier_fn_t notifier_call;
// /*AFLA*/ 	struct notifier_block __rcu *next;
// /*AFLA*/ 	int priority;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct atomic_notifier_head {
// /*AFLA*/ 	spinlock_t lock;
// /*AFLA*/ 	struct notifier_block __rcu *head;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct blocking_notifier_head {
// /*AFLA*/ 	struct rw_semaphore rwsem;
// /*AFLA*/ 	struct notifier_block __rcu *head;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct raw_notifier_head {
// /*AFLA*/ 	struct notifier_block __rcu *head;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct srcu_notifier_head {
// /*AFLA*/ 	struct mutex mutex;
// /*AFLA*/ 	struct srcu_struct srcu;
// /*AFLA*/ 	struct notifier_block __rcu *head;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC_INIT_NOTIFIER_HEAD(name) do {	\
// /*AFLA*/ 		spin_lock_init(&(name)->lock);	\
// /*AFLA*/ 		(name)->head = NULL;		\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #define BLOCKING_INIT_NOTIFIER_HEAD(name) do {	\
// /*AFLA*/ 		init_rwsem(&(name)->rwsem);	\
// /*AFLA*/ 		(name)->head = NULL;		\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #define RAW_INIT_NOTIFIER_HEAD(name) do {	\
// /*AFLA*/ 		(name)->head = NULL;		\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ /* srcu_notifier_heads must be initialized and cleaned up dynamically */
// /*AFLA*/ extern void srcu_init_notifier_head(struct srcu_notifier_head *nh);
// /*AFLA*/ #define srcu_cleanup_notifier_head(name)	\
// /*AFLA*/ 		cleanup_srcu_struct(&(name)->srcu);
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC_NOTIFIER_INIT(name) {				\
// /*AFLA*/ 		.lock = __SPIN_LOCK_UNLOCKED(name.lock),	\
// /*AFLA*/ 		.head = NULL }
// /*AFLA*/ #define BLOCKING_NOTIFIER_INIT(name) {				\
// /*AFLA*/ 		.rwsem = __RWSEM_INITIALIZER((name).rwsem),	\
// /*AFLA*/ 		.head = NULL }
// /*AFLA*/ #define RAW_NOTIFIER_INIT(name)	{				\
// /*AFLA*/ 		.head = NULL }
// /*AFLA*/ /* srcu_notifier_heads cannot be initialized statically */
// /*AFLA*/ 
// /*AFLA*/ #define ATOMIC_NOTIFIER_HEAD(name)				\
// /*AFLA*/ 	struct atomic_notifier_head name =			\
// /*AFLA*/ 		ATOMIC_NOTIFIER_INIT(name)
// /*AFLA*/ #define BLOCKING_NOTIFIER_HEAD(name)				\
// /*AFLA*/ 	struct blocking_notifier_head name =			\
// /*AFLA*/ 		BLOCKING_NOTIFIER_INIT(name)
// /*AFLA*/ #define RAW_NOTIFIER_HEAD(name)					\
// /*AFLA*/ 	struct raw_notifier_head name =				\
// /*AFLA*/ 		RAW_NOTIFIER_INIT(name)
// /*AFLA*/ 
// /*AFLA*/ #ifdef __KERNEL__
// /*AFLA*/ 
// /*AFLA*/ extern int atomic_notifier_chain_register(struct atomic_notifier_head *nh,
// /*AFLA*/ 		struct notifier_block *nb);
// /*AFLA*/ extern int blocking_notifier_chain_register(struct blocking_notifier_head *nh,
// /*AFLA*/ 		struct notifier_block *nb);
// /*AFLA*/ extern int raw_notifier_chain_register(struct raw_notifier_head *nh,
// /*AFLA*/ 		struct notifier_block *nb);
// /*AFLA*/ extern int srcu_notifier_chain_register(struct srcu_notifier_head *nh,
// /*AFLA*/ 		struct notifier_block *nb);
// /*AFLA*/ 
// /*AFLA*/ extern int blocking_notifier_chain_cond_register(
// /*AFLA*/ 		struct blocking_notifier_head *nh,
// /*AFLA*/ 		struct notifier_block *nb);
// /*AFLA*/ 
// /*AFLA*/ extern int atomic_notifier_chain_unregister(struct atomic_notifier_head *nh,
// /*AFLA*/ 		struct notifier_block *nb);
// /*AFLA*/ extern int blocking_notifier_chain_unregister(struct blocking_notifier_head *nh,
// /*AFLA*/ 		struct notifier_block *nb);
// /*AFLA*/ extern int raw_notifier_chain_unregister(struct raw_notifier_head *nh,
// /*AFLA*/ 		struct notifier_block *nb);
// /*AFLA*/ extern int srcu_notifier_chain_unregister(struct srcu_notifier_head *nh,
// /*AFLA*/ 		struct notifier_block *nb);
// /*AFLA*/ 
// /*AFLA*/ extern int atomic_notifier_call_chain(struct atomic_notifier_head *nh,
// /*AFLA*/ 		unsigned long val, void *v);
// /*AFLA*/ extern int __atomic_notifier_call_chain(struct atomic_notifier_head *nh,
// /*AFLA*/ 	unsigned long val, void *v, int nr_to_call, int *nr_calls);
// /*AFLA*/ extern int blocking_notifier_call_chain(struct blocking_notifier_head *nh,
// /*AFLA*/ 		unsigned long val, void *v);
// /*AFLA*/ extern int __blocking_notifier_call_chain(struct blocking_notifier_head *nh,
// /*AFLA*/ 	unsigned long val, void *v, int nr_to_call, int *nr_calls);
// /*AFLA*/ extern int raw_notifier_call_chain(struct raw_notifier_head *nh,
// /*AFLA*/ 		unsigned long val, void *v);
// /*AFLA*/ extern int __raw_notifier_call_chain(struct raw_notifier_head *nh,
// /*AFLA*/ 	unsigned long val, void *v, int nr_to_call, int *nr_calls);
// /*AFLA*/ extern int srcu_notifier_call_chain(struct srcu_notifier_head *nh,
// /*AFLA*/ 		unsigned long val, void *v);
// /*AFLA*/ extern int __srcu_notifier_call_chain(struct srcu_notifier_head *nh,
// /*AFLA*/ 	unsigned long val, void *v, int nr_to_call, int *nr_calls);
// /*AFLA*/ 
// /*AFLA*/ #define NOTIFY_DONE		0x0000		/* Don't care */
// /*AFLA*/ #define NOTIFY_OK		0x0001		/* Suits me */
// /*AFLA*/ #define NOTIFY_STOP_MASK	0x8000		/* Don't call further */
// /*AFLA*/ #define NOTIFY_BAD		(NOTIFY_STOP_MASK|0x0002)
// /*AFLA*/ 						/* Bad/Veto action */
// /*AFLA*/ /*
// /*AFLA*/  * Clean way to return from the notifier and stop further calls.
// /*AFLA*/  */
// /*AFLA*/ #define NOTIFY_STOP		(NOTIFY_OK|NOTIFY_STOP_MASK)
// /*AFLA*/ 
// /*AFLA*/ /* Encapsulate (negative) errno value (in particular, NOTIFY_BAD <=> EPERM). */
// /*AFLA*/ static inline int notifier_from_errno(int err)
// /*AFLA*/ {
// /*AFLA*/ 	if (err)
// /*AFLA*/ 		return NOTIFY_STOP_MASK | (NOTIFY_OK - err);
// /*AFLA*/ 
// /*AFLA*/ 	return NOTIFY_OK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Restore (negative) errno value from notify return value. */
// /*AFLA*/ static inline int notifier_to_errno(int ret)
// /*AFLA*/ {
// /*AFLA*/ 	ret &= ~NOTIFY_STOP_MASK;
// /*AFLA*/ 	return ret > NOTIFY_OK ? NOTIFY_OK - ret : 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  *	Declared notifiers so far. I can imagine quite a few more chains
// /*AFLA*/  *	over time (eg laptop power reset chains, reboot chain (to clean 
// /*AFLA*/  *	device units up), device [un]mount chain, module load/unload chain,
// /*AFLA*/  *	low memory chain, screenblank chain (for plug in modular screenblankers) 
// /*AFLA*/  *	VC switch chains (for loadable kernel svgalib VC switch helpers) etc...
// /*AFLA*/  */
// /*AFLA*/  
// /*AFLA*/ /* CPU notfiers are defined in include/linux/cpu.h. */
// /*AFLA*/ 
// /*AFLA*/ /* netdevice notifiers are defined in include/linux/netdevice.h */
// /*AFLA*/ 
// /*AFLA*/ /* reboot notifiers are defined in include/linux/reboot.h. */
// /*AFLA*/ 
// /*AFLA*/ /* Hibernation and suspend events are defined in include/linux/suspend.h. */
// /*AFLA*/ 
// /*AFLA*/ /* Virtual Terminal events are defined in include/linux/vt.h. */
// /*AFLA*/ 
// /*AFLA*/ #define NETLINK_URELEASE	0x0001	/* Unicast netlink socket released */
// /*AFLA*/ 
// /*AFLA*/ /* Console keyboard events.
// /*AFLA*/  * Note: KBD_KEYCODE is always sent before KBD_UNBOUND_KEYCODE, KBD_UNICODE and
// /*AFLA*/  * KBD_KEYSYM. */
// /*AFLA*/ #define KBD_KEYCODE		0x0001 /* Keyboard keycode, called before any other */
// /*AFLA*/ #define KBD_UNBOUND_KEYCODE	0x0002 /* Keyboard keycode which is not bound to any other */
// /*AFLA*/ #define KBD_UNICODE		0x0003 /* Keyboard unicode */
// /*AFLA*/ #define KBD_KEYSYM		0x0004 /* Keyboard keysym */
// /*AFLA*/ #define KBD_POST_KEYSYM		0x0005 /* Called after keyboard keysym interpretation */
// /*AFLA*/ 
// /*AFLA*/ extern struct blocking_notifier_head reboot_notifier_list;
// /*AFLA*/ 
// /*AFLA*/ #endif /* __KERNEL__ */
// /*AFLA*/ #endif /* _LINUX_NOTIFIER_H */
