/*
 * linux/ipc/sem.c
 * Copyright (C) 1992 Krishna Balasubramanian
 * Copyright (C) 1995 Eric Schenk, Bruno Haible
 *
 * /proc/sysvipc/sem support (c) 1999 Dragos Acostachioaie <dragos@iname.com>
 *
 * SMP-threaded, sysctl's added
 * (c) 1999 Manfred Spraul <manfred@colorfullife.com>
 * Enforced range limit on SEM_UNDO
 * (c) 2001 Red Hat Inc
 * Lockless wakeup
 * (c) 2003 Manfred Spraul <manfred@colorfullife.com>
 * Further wakeup optimizations, documentation
 * (c) 2010 Manfred Spraul <manfred@colorfullife.com>
 *
 * support for audit of ipc object properties and permission changes
 * Dustin Kirkland <dustin.kirkland@us.ibm.com>
 *
 * namespaces support
 * OpenVZ, SWsoft Inc.
 * Pavel Emelianov <xemul@openvz.org>
 *
 * Implementation notes: (May 2010)
 * This file implements System V semaphores.
 *
 * User space visible behavior:
 * - FIFO ordering for semop() operations (just FIFO, not starvation
 *   protection)
 * - multiple semaphore operations that alter the same semaphore in
 *   one semop() are handled.
 * - sem_ctime (time of last semctl()) is updated in the IPC_SET, SETVAL and
 *   SETALL calls.
 * - two Linux specific semctl() commands: SEM_STAT, SEM_INFO.
 * - undo adjustments at process exit are limited to 0..SEMVMX.
 * - namespace are supported.
 * - SEMMSL, SEMMNS, SEMOPM and SEMMNI can be configured at runtine by writing
 *   to /proc/sys/kernel/sem.
 * - statistics about the usage are reported in /proc/sysvipc/sem.
 *
 * Internals:
 * - scalability:
 *   - all global variables are read-mostly.
 *   - semop() calls and semctl(RMID) are synchronized by RCU.
 *   - most operations do write operations (actually: spin_lock calls) to
 *     the per-semaphore array structure.
 *   Thus: Perfect SMP scaling between independent semaphore arrays.
 *         If multiple semaphores in one array are used, then cache line
 *         trashing on the semaphore array spinlock will limit the scaling.
 * - semncnt and semzcnt are calculated on demand in count_semcnt()
 * - the task that performs a successful semop() scans the list of all
 *   sleeping tasks and completes any pending operations that can be fulfilled.
 *   Semaphores are actively given to waiting tasks (necessary for FIFO).
 *   (see update_queue())
 * - To improve the scalability, the actual wake-up calls are performed after
 *   dropping all locks. (see wake_up_sem_queue_prepare(),
 *   wake_up_sem_queue_do())
 * - All work is done by the waker, the woken up task does not have to do
 *   anything - not even acquiring a lock or dropping a refcount.
 * - A woken up task may not even touch the semaphore array anymore, it may
 *   have been destroyed already by a semctl(RMID).
 * - The synchronizations between wake-ups due to a timeout/signal and a
 *   wake-up due to a completed semaphore operation is achieved by using an
 *   intermediate state (IN_WAKEUP).
 * - UNDO values are stored in an array (one per process and per
 *   semaphore array, lazily allocated). For backwards compatibility, multiple
 *   modes for the UNDO variables are supported (per process, per thread)
 *   (see copy_semundo, CLONE_SYSVSEM)
 * - There are two lists of the pending operations: a per-array list
 *   and per-semaphore list (stored in the array). This allows to achieve FIFO
 *   ordering without always scanning all pending operations.
 *   The worst-case behavior is nevertheless O(N^2) for N wakeups.
 */

#include <linux/slab.h>
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/init.h>
// /*AFLA*/ #include <linux/proc_fs.h>
// /*AFLA*/ #include <linux/time.h>
// /*AFLA*/ #include <linux/security.h>
// /*AFLA*/ #include <linux/syscalls.h>
// /*AFLA*/ #include <linux/audit.h>
// /*AFLA*/ #include <linux/capability.h>
// /*AFLA*/ #include <linux/seq_file.h>
// /*AFLA*/ #include <linux/rwsem.h>
// /*AFLA*/ #include <linux/nsproxy.h>
// /*AFLA*/ #include <linux/ipc_namespace.h>
// /*AFLA*/ 
// /*AFLA*/ #include <linux/uaccess.h>
// /*AFLA*/ #include "util.h"
// /*AFLA*/ 
// /*AFLA*/ /* One semaphore structure for each semaphore in the system. */
// /*AFLA*/ struct sem {
// /*AFLA*/ 	int	semval;		/* current value */
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * PID of the process that last modified the semaphore. For
// /*AFLA*/ 	 * Linux, specifically these are:
// /*AFLA*/ 	 *  - semop
// /*AFLA*/ 	 *  - semctl, via SETVAL and SETALL.
// /*AFLA*/ 	 *  - at task exit when performing undo adjustments (see exit_sem).
// /*AFLA*/ 	 */
// /*AFLA*/ 	int	sempid;
// /*AFLA*/ 	spinlock_t	lock;	/* spinlock for fine-grained semtimedop */
// /*AFLA*/ 	struct list_head pending_alter; /* pending single-sop operations */
// /*AFLA*/ 					/* that alter the semaphore */
// /*AFLA*/ 	struct list_head pending_const; /* pending single-sop operations */
// /*AFLA*/ 					/* that do not alter the semaphore*/
// /*AFLA*/ 	time_t	sem_otime;	/* candidate for sem_otime */
// /*AFLA*/ } ____cacheline_aligned_in_smp;
// /*AFLA*/ 
// /*AFLA*/ /* One queue for each sleeping process in the system. */
// /*AFLA*/ struct sem_queue {
// /*AFLA*/ 	struct list_head	list;	 /* queue of pending operations */
// /*AFLA*/ 	struct task_struct	*sleeper; /* this process */
// /*AFLA*/ 	struct sem_undo		*undo;	 /* undo structure */
// /*AFLA*/ 	int			pid;	 /* process id of requesting process */
// /*AFLA*/ 	int			status;	 /* completion status of operation */
// /*AFLA*/ 	struct sembuf		*sops;	 /* array of pending operations */
// /*AFLA*/ 	struct sembuf		*blocking; /* the operation that blocked */
// /*AFLA*/ 	int			nsops;	 /* number of operations */
// /*AFLA*/ 	int			alter;	 /* does *sops alter the array? */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Each task has a list of undo requests. They are executed automatically
// /*AFLA*/  * when the process exits.
// /*AFLA*/  */
// /*AFLA*/ struct sem_undo {
// /*AFLA*/ 	struct list_head	list_proc;	/* per-process list: *
// /*AFLA*/ 						 * all undos from one process
// /*AFLA*/ 						 * rcu protected */
// /*AFLA*/ 	struct rcu_head		rcu;		/* rcu struct for sem_undo */
// /*AFLA*/ 	struct sem_undo_list	*ulp;		/* back ptr to sem_undo_list */
// /*AFLA*/ 	struct list_head	list_id;	/* per semaphore array list:
// /*AFLA*/ 						 * all undos for one array */
// /*AFLA*/ 	int			semid;		/* semaphore set identifier */
// /*AFLA*/ 	short			*semadj;	/* array of adjustments */
// /*AFLA*/ 						/* one per semaphore */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* sem_undo_list controls shared access to the list of sem_undo structures
// /*AFLA*/  * that may be shared among all a CLONE_SYSVSEM task group.
// /*AFLA*/  */
// /*AFLA*/ struct sem_undo_list {
// /*AFLA*/ 	atomic_t		refcnt;
// /*AFLA*/ 	spinlock_t		lock;
// /*AFLA*/ 	struct list_head	list_proc;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define sem_ids(ns)	((ns)->ids[IPC_SEM_IDS])
// /*AFLA*/ 
// /*AFLA*/ #define sem_checkid(sma, semid)	ipc_checkid(&sma->sem_perm, semid)
// /*AFLA*/ 
// /*AFLA*/ static int newary(struct ipc_namespace *, struct ipc_params *);
// /*AFLA*/ static void freeary(struct ipc_namespace *, struct kern_ipc_perm *);
// /*AFLA*/ #ifdef CONFIG_PROC_FS
// /*AFLA*/ static int sysvipc_sem_proc_show(struct seq_file *s, void *it);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define SEMMSL_FAST	256 /* 512 bytes on stack */
// /*AFLA*/ #define SEMOPM_FAST	64  /* ~ 372 bytes on stack */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Locking:
// /*AFLA*/  * a) global sem_lock() for read/write
// /*AFLA*/  *	sem_undo.id_next,
// /*AFLA*/  *	sem_array.complex_count,
// /*AFLA*/  *	sem_array.complex_mode
// /*AFLA*/  *	sem_array.pending{_alter,_const},
// /*AFLA*/  *	sem_array.sem_undo
// /*AFLA*/  *
// /*AFLA*/  * b) global or semaphore sem_lock() for read/write:
// /*AFLA*/  *	sem_array.sem_base[i].pending_{const,alter}:
// /*AFLA*/  *	sem_array.complex_mode (for read)
// /*AFLA*/  *
// /*AFLA*/  * c) special:
// /*AFLA*/  *	sem_undo_list.list_proc:
// /*AFLA*/  *	* undo_list->lock for write
// /*AFLA*/  *	* rcu for read
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define sc_semmsl	sem_ctls[0]
// /*AFLA*/ #define sc_semmns	sem_ctls[1]
// /*AFLA*/ #define sc_semopm	sem_ctls[2]
// /*AFLA*/ #define sc_semmni	sem_ctls[3]
// /*AFLA*/ 
// /*AFLA*/ void sem_init_ns(struct ipc_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	ns->sc_semmsl = SEMMSL;
// /*AFLA*/ 	ns->sc_semmns = SEMMNS;
// /*AFLA*/ 	ns->sc_semopm = SEMOPM;
// /*AFLA*/ 	ns->sc_semmni = SEMMNI;
// /*AFLA*/ 	ns->used_sems = 0;
// /*AFLA*/ 	ipc_init_ids(&ns->ids[IPC_SEM_IDS]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_IPC_NS
// /*AFLA*/ void sem_exit_ns(struct ipc_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	free_ipcs(ns, &sem_ids(ns), freeary);
// /*AFLA*/ 	idr_destroy(&ns->ids[IPC_SEM_IDS].ipcs_idr);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ void __init sem_init(void)
// /*AFLA*/ {
// /*AFLA*/ 	sem_init_ns(&init_ipc_ns);
// /*AFLA*/ 	ipc_init_proc_interface("sysvipc/sem",
// /*AFLA*/ 				"       key      semid perms      nsems   uid   gid  cuid  cgid      otime      ctime\n",
// /*AFLA*/ 				IPC_SEM_IDS, sysvipc_sem_proc_show);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * unmerge_queues - unmerge queues, if possible.
// /*AFLA*/  * @sma: semaphore array
// /*AFLA*/  *
// /*AFLA*/  * The function unmerges the wait queues if complex_count is 0.
// /*AFLA*/  * It must be called prior to dropping the global semaphore array lock.
// /*AFLA*/  */
// /*AFLA*/ static void unmerge_queues(struct sem_array *sma)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem_queue *q, *tq;
// /*AFLA*/ 
// /*AFLA*/ 	/* complex operations still around? */
// /*AFLA*/ 	if (sma->complex_count)
// /*AFLA*/ 		return;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * We will switch back to simple mode.
// /*AFLA*/ 	 * Move all pending operation back into the per-semaphore
// /*AFLA*/ 	 * queues.
// /*AFLA*/ 	 */
// /*AFLA*/ 	list_for_each_entry_safe(q, tq, &sma->pending_alter, list) {
// /*AFLA*/ 		struct sem *curr;
// /*AFLA*/ 		curr = &sma->sem_base[q->sops[0].sem_num];
// /*AFLA*/ 
// /*AFLA*/ 		list_add_tail(&q->list, &curr->pending_alter);
// /*AFLA*/ 	}
// /*AFLA*/ 	INIT_LIST_HEAD(&sma->pending_alter);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * merge_queues - merge single semop queues into global queue
// /*AFLA*/  * @sma: semaphore array
// /*AFLA*/  *
// /*AFLA*/  * This function merges all per-semaphore queues into the global queue.
// /*AFLA*/  * It is necessary to achieve FIFO ordering for the pending single-sop
// /*AFLA*/  * operations when a multi-semop operation must sleep.
// /*AFLA*/  * Only the alter operations must be moved, the const operations can stay.
// /*AFLA*/  */
// /*AFLA*/ static void merge_queues(struct sem_array *sma)
// /*AFLA*/ {
// /*AFLA*/ 	int i;
// /*AFLA*/ 	for (i = 0; i < sma->sem_nsems; i++) {
// /*AFLA*/ 		struct sem *sem = sma->sem_base + i;
// /*AFLA*/ 
// /*AFLA*/ 		list_splice_init(&sem->pending_alter, &sma->pending_alter);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static void sem_rcu_free(struct rcu_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	struct ipc_rcu *p = container_of(head, struct ipc_rcu, rcu);
// /*AFLA*/ 	struct sem_array *sma = ipc_rcu_to_struct(p);
// /*AFLA*/ 
// /*AFLA*/ 	security_sem_free(sma);
// /*AFLA*/ 	ipc_rcu_free(head);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Enter the mode suitable for non-simple operations:
// /*AFLA*/  * Caller must own sem_perm.lock.
// /*AFLA*/  */
// /*AFLA*/ static void complexmode_enter(struct sem_array *sma)
// /*AFLA*/ {
// /*AFLA*/ 	int i;
// /*AFLA*/ 	struct sem *sem;
// /*AFLA*/ 
// /*AFLA*/ 	if (sma->complex_mode)  {
// /*AFLA*/ 		/* We are already in complex_mode. Nothing to do */
// /*AFLA*/ 		return;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	/* We need a full barrier after seting complex_mode:
// /*AFLA*/ 	 * The write to complex_mode must be visible
// /*AFLA*/ 	 * before we read the first sem->lock spinlock state.
// /*AFLA*/ 	 */
// /*AFLA*/ 	smp_store_mb(sma->complex_mode, true);
// /*AFLA*/ 
// /*AFLA*/ 	for (i = 0; i < sma->sem_nsems; i++) {
// /*AFLA*/ 		sem = sma->sem_base + i;
// /*AFLA*/ 		spin_unlock_wait(&sem->lock);
// /*AFLA*/ 	}
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * spin_unlock_wait() is not a memory barriers, it is only a
// /*AFLA*/ 	 * control barrier. The code must pair with spin_unlock(&sem->lock),
// /*AFLA*/ 	 * thus just the control barrier is insufficient.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * smp_rmb() is sufficient, as writes cannot pass the control barrier.
// /*AFLA*/ 	 */
// /*AFLA*/ 	smp_rmb();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Try to leave the mode that disallows simple operations:
// /*AFLA*/  * Caller must own sem_perm.lock.
// /*AFLA*/  */
// /*AFLA*/ static void complexmode_tryleave(struct sem_array *sma)
// /*AFLA*/ {
// /*AFLA*/ 	if (sma->complex_count)  {
// /*AFLA*/ 		/* Complex ops are sleeping.
// /*AFLA*/ 		 * We must stay in complex mode
// /*AFLA*/ 		 */
// /*AFLA*/ 		return;
// /*AFLA*/ 	}
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Immediately after setting complex_mode to false,
// /*AFLA*/ 	 * a simple op can start. Thus: all memory writes
// /*AFLA*/ 	 * performed by the current operation must be visible
// /*AFLA*/ 	 * before we set complex_mode to false.
// /*AFLA*/ 	 */
// /*AFLA*/ 	smp_store_release(&sma->complex_mode, false);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define SEM_GLOBAL_LOCK	(-1)
// /*AFLA*/ /*
// /*AFLA*/  * If the request contains only one semaphore operation, and there are
// /*AFLA*/  * no complex transactions pending, lock only the semaphore involved.
// /*AFLA*/  * Otherwise, lock the entire semaphore array, since we either have
// /*AFLA*/  * multiple semaphores in our own semops, or we need to look at
// /*AFLA*/  * semaphores from other pending complex operations.
// /*AFLA*/  */
// /*AFLA*/ static inline int sem_lock(struct sem_array *sma, struct sembuf *sops,
// /*AFLA*/ 			      int nsops)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem *sem;
// /*AFLA*/ 
// /*AFLA*/ 	if (nsops != 1) {
// /*AFLA*/ 		/* Complex operation - acquire a full lock */
// /*AFLA*/ 		ipc_lock_object(&sma->sem_perm);
// /*AFLA*/ 
// /*AFLA*/ 		/* Prevent parallel simple ops */
// /*AFLA*/ 		complexmode_enter(sma);
// /*AFLA*/ 		return SEM_GLOBAL_LOCK;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Only one semaphore affected - try to optimize locking.
// /*AFLA*/ 	 * Optimized locking is possible if no complex operation
// /*AFLA*/ 	 * is either enqueued or processed right now.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Both facts are tracked by complex_mode.
// /*AFLA*/ 	 */
// /*AFLA*/ 	sem = sma->sem_base + sops->sem_num;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Initial check for complex_mode. Just an optimization,
// /*AFLA*/ 	 * no locking, no memory barrier.
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (!sma->complex_mode) {
// /*AFLA*/ 		/*
// /*AFLA*/ 		 * It appears that no complex operation is around.
// /*AFLA*/ 		 * Acquire the per-semaphore lock.
// /*AFLA*/ 		 */
// /*AFLA*/ 		spin_lock(&sem->lock);
// /*AFLA*/ 
// /*AFLA*/ 		/*
// /*AFLA*/ 		 * See 51d7d5205d33
// /*AFLA*/ 		 * ("powerpc: Add smp_mb() to arch_spin_is_locked()"):
// /*AFLA*/ 		 * A full barrier is required: the write of sem->lock
// /*AFLA*/ 		 * must be visible before the read is executed
// /*AFLA*/ 		 */
// /*AFLA*/ 		smp_mb();
// /*AFLA*/ 
// /*AFLA*/ 		if (!smp_load_acquire(&sma->complex_mode)) {
// /*AFLA*/ 			/* fast path successful! */
// /*AFLA*/ 			return sops->sem_num;
// /*AFLA*/ 		}
// /*AFLA*/ 		spin_unlock(&sem->lock);
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	/* slow path: acquire the full lock */
// /*AFLA*/ 	ipc_lock_object(&sma->sem_perm);
// /*AFLA*/ 
// /*AFLA*/ 	if (sma->complex_count == 0) {
// /*AFLA*/ 		/* False alarm:
// /*AFLA*/ 		 * There is no complex operation, thus we can switch
// /*AFLA*/ 		 * back to the fast path.
// /*AFLA*/ 		 */
// /*AFLA*/ 		spin_lock(&sem->lock);
// /*AFLA*/ 		ipc_unlock_object(&sma->sem_perm);
// /*AFLA*/ 		return sops->sem_num;
// /*AFLA*/ 	} else {
// /*AFLA*/ 		/* Not a false alarm, thus complete the sequence for a
// /*AFLA*/ 		 * full lock.
// /*AFLA*/ 		 */
// /*AFLA*/ 		complexmode_enter(sma);
// /*AFLA*/ 		return SEM_GLOBAL_LOCK;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sem_unlock(struct sem_array *sma, int locknum)
// /*AFLA*/ {
// /*AFLA*/ 	if (locknum == SEM_GLOBAL_LOCK) {
// /*AFLA*/ 		unmerge_queues(sma);
// /*AFLA*/ 		complexmode_tryleave(sma);
// /*AFLA*/ 		ipc_unlock_object(&sma->sem_perm);
// /*AFLA*/ 	} else {
// /*AFLA*/ 		struct sem *sem = sma->sem_base + locknum;
// /*AFLA*/ 		spin_unlock(&sem->lock);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * sem_lock_(check_) routines are called in the paths where the rwsem
// /*AFLA*/  * is not held.
// /*AFLA*/  *
// /*AFLA*/  * The caller holds the RCU read lock.
// /*AFLA*/  */
// /*AFLA*/ static inline struct sem_array *sem_obtain_lock(struct ipc_namespace *ns,
// /*AFLA*/ 			int id, struct sembuf *sops, int nsops, int *locknum)
// /*AFLA*/ {
// /*AFLA*/ 	struct kern_ipc_perm *ipcp;
// /*AFLA*/ 	struct sem_array *sma;
// /*AFLA*/ 
// /*AFLA*/ 	ipcp = ipc_obtain_object_idr(&sem_ids(ns), id);
// /*AFLA*/ 	if (IS_ERR(ipcp))
// /*AFLA*/ 		return ERR_CAST(ipcp);
// /*AFLA*/ 
// /*AFLA*/ 	sma = container_of(ipcp, struct sem_array, sem_perm);
// /*AFLA*/ 	*locknum = sem_lock(sma, sops, nsops);
// /*AFLA*/ 
// /*AFLA*/ 	/* ipc_rmid() may have already freed the ID while sem_lock
// /*AFLA*/ 	 * was spinning: verify that the structure is still valid
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (ipc_valid_object(ipcp))
// /*AFLA*/ 		return container_of(ipcp, struct sem_array, sem_perm);
// /*AFLA*/ 
// /*AFLA*/ 	sem_unlock(sma, *locknum);
// /*AFLA*/ 	return ERR_PTR(-EINVAL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct sem_array *sem_obtain_object(struct ipc_namespace *ns, int id)
// /*AFLA*/ {
// /*AFLA*/ 	struct kern_ipc_perm *ipcp = ipc_obtain_object_idr(&sem_ids(ns), id);
// /*AFLA*/ 
// /*AFLA*/ 	if (IS_ERR(ipcp))
// /*AFLA*/ 		return ERR_CAST(ipcp);
// /*AFLA*/ 
// /*AFLA*/ 	return container_of(ipcp, struct sem_array, sem_perm);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct sem_array *sem_obtain_object_check(struct ipc_namespace *ns,
// /*AFLA*/ 							int id)
// /*AFLA*/ {
// /*AFLA*/ 	struct kern_ipc_perm *ipcp = ipc_obtain_object_check(&sem_ids(ns), id);
// /*AFLA*/ 
// /*AFLA*/ 	if (IS_ERR(ipcp))
// /*AFLA*/ 		return ERR_CAST(ipcp);
// /*AFLA*/ 
// /*AFLA*/ 	return container_of(ipcp, struct sem_array, sem_perm);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sem_lock_and_putref(struct sem_array *sma)
// /*AFLA*/ {
// /*AFLA*/ 	sem_lock(sma, NULL, -1);
// /*AFLA*/ 	ipc_rcu_putref(sma, sem_rcu_free);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sem_rmid(struct ipc_namespace *ns, struct sem_array *s)
// /*AFLA*/ {
// /*AFLA*/ 	ipc_rmid(&sem_ids(ns), &s->sem_perm);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Lockless wakeup algorithm:
// /*AFLA*/  * Without the check/retry algorithm a lockless wakeup is possible:
// /*AFLA*/  * - queue.status is initialized to -EINTR before blocking.
// /*AFLA*/  * - wakeup is performed by
// /*AFLA*/  *	* unlinking the queue entry from the pending list
// /*AFLA*/  *	* setting queue.status to IN_WAKEUP
// /*AFLA*/  *	  This is the notification for the blocked thread that a
// /*AFLA*/  *	  result value is imminent.
// /*AFLA*/  *	* call wake_up_process
// /*AFLA*/  *	* set queue.status to the final value.
// /*AFLA*/  * - the previously blocked thread checks queue.status:
// /*AFLA*/  *	* if it's IN_WAKEUP, then it must wait until the value changes
// /*AFLA*/  *	* if it's not -EINTR, then the operation was completed by
// /*AFLA*/  *	  update_queue. semtimedop can return queue.status without
// /*AFLA*/  *	  performing any operation on the sem array.
// /*AFLA*/  *	* otherwise it must acquire the spinlock and check what's up.
// /*AFLA*/  *
// /*AFLA*/  * The two-stage algorithm is necessary to protect against the following
// /*AFLA*/  * races:
// /*AFLA*/  * - if queue.status is set after wake_up_process, then the woken up idle
// /*AFLA*/  *   thread could race forward and try (and fail) to acquire sma->lock
// /*AFLA*/  *   before update_queue had a chance to set queue.status
// /*AFLA*/  * - if queue.status is written before wake_up_process and if the
// /*AFLA*/  *   blocked process is woken up by a signal between writing
// /*AFLA*/  *   queue.status and the wake_up_process, then the woken up
// /*AFLA*/  *   process could return from semtimedop and die by calling
// /*AFLA*/  *   sys_exit before wake_up_process is called. Then wake_up_process
// /*AFLA*/  *   will oops, because the task structure is already invalid.
// /*AFLA*/  *   (yes, this happened on s390 with sysv msg).
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ #define IN_WAKEUP	1
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * newary - Create a new semaphore set
// /*AFLA*/  * @ns: namespace
// /*AFLA*/  * @params: ptr to the structure that contains key, semflg and nsems
// /*AFLA*/  *
// /*AFLA*/  * Called with sem_ids.rwsem held (as a writer)
// /*AFLA*/  */
// /*AFLA*/ static int newary(struct ipc_namespace *ns, struct ipc_params *params)
// /*AFLA*/ {
// /*AFLA*/ 	int id;
// /*AFLA*/ 	int retval;
// /*AFLA*/ 	struct sem_array *sma;
// /*AFLA*/ 	int size;
// /*AFLA*/ 	key_t key = params->key;
// /*AFLA*/ 	int nsems = params->u.nsems;
// /*AFLA*/ 	int semflg = params->flg;
// /*AFLA*/ 	int i;
// /*AFLA*/ 
// /*AFLA*/ 	if (!nsems)
// /*AFLA*/ 		return -EINVAL;
// /*AFLA*/ 	if (ns->used_sems + nsems > ns->sc_semmns)
// /*AFLA*/ 		return -ENOSPC;
// /*AFLA*/ 
// /*AFLA*/ 	size = sizeof(*sma) + nsems * sizeof(struct sem);
// /*AFLA*/ 	sma = ipc_rcu_alloc(size);
// /*AFLA*/ 	if (!sma)
// /*AFLA*/ 		return -ENOMEM;
// /*AFLA*/ 
// /*AFLA*/ 	memset(sma, 0, size);
// /*AFLA*/ 
// /*AFLA*/ 	sma->sem_perm.mode = (semflg & S_IRWXUGO);
// /*AFLA*/ 	sma->sem_perm.key = key;
// /*AFLA*/ 
// /*AFLA*/ 	sma->sem_perm.security = NULL;
// /*AFLA*/ 	retval = security_sem_alloc(sma);
// /*AFLA*/ 	if (retval) {
// /*AFLA*/ 		ipc_rcu_putref(sma, ipc_rcu_free);
// /*AFLA*/ 		return retval;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	sma->sem_base = (struct sem *) &sma[1];
// /*AFLA*/ 
// /*AFLA*/ 	for (i = 0; i < nsems; i++) {
// /*AFLA*/ 		INIT_LIST_HEAD(&sma->sem_base[i].pending_alter);
// /*AFLA*/ 		INIT_LIST_HEAD(&sma->sem_base[i].pending_const);
// /*AFLA*/ 		spin_lock_init(&sma->sem_base[i].lock);
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	sma->complex_count = 0;
// /*AFLA*/ 	sma->complex_mode = true; /* dropped by sem_unlock below */
// /*AFLA*/ 	INIT_LIST_HEAD(&sma->pending_alter);
// /*AFLA*/ 	INIT_LIST_HEAD(&sma->pending_const);
// /*AFLA*/ 	INIT_LIST_HEAD(&sma->list_id);
// /*AFLA*/ 	sma->sem_nsems = nsems;
// /*AFLA*/ 	sma->sem_ctime = get_seconds();
// /*AFLA*/ 
// /*AFLA*/ 	id = ipc_addid(&sem_ids(ns), &sma->sem_perm, ns->sc_semmni);
// /*AFLA*/ 	if (id < 0) {
// /*AFLA*/ 		ipc_rcu_putref(sma, sem_rcu_free);
// /*AFLA*/ 		return id;
// /*AFLA*/ 	}
// /*AFLA*/ 	ns->used_sems += nsems;
// /*AFLA*/ 
// /*AFLA*/ 	sem_unlock(sma, -1);
// /*AFLA*/ 	rcu_read_unlock();
// /*AFLA*/ 
// /*AFLA*/ 	return sma->sem_perm.id;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Called with sem_ids.rwsem and ipcp locked.
// /*AFLA*/  */
// /*AFLA*/ static inline int sem_security(struct kern_ipc_perm *ipcp, int semflg)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem_array *sma;
// /*AFLA*/ 
// /*AFLA*/ 	sma = container_of(ipcp, struct sem_array, sem_perm);
// /*AFLA*/ 	return security_sem_associate(sma, semflg);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Called with sem_ids.rwsem and ipcp locked.
// /*AFLA*/  */
// /*AFLA*/ static inline int sem_more_checks(struct kern_ipc_perm *ipcp,
// /*AFLA*/ 				struct ipc_params *params)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem_array *sma;
// /*AFLA*/ 
// /*AFLA*/ 	sma = container_of(ipcp, struct sem_array, sem_perm);
// /*AFLA*/ 	if (params->u.nsems > sma->sem_nsems)
// /*AFLA*/ 		return -EINVAL;
// /*AFLA*/ 
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ SYSCALL_DEFINE3(semget, key_t, key, int, nsems, int, semflg)
// /*AFLA*/ {
// /*AFLA*/ 	struct ipc_namespace *ns;
// /*AFLA*/ 	static const struct ipc_ops sem_ops = {
// /*AFLA*/ 		.getnew = newary,
// /*AFLA*/ 		.associate = sem_security,
// /*AFLA*/ 		.more_checks = sem_more_checks,
// /*AFLA*/ 	};
// /*AFLA*/ 	struct ipc_params sem_params;
// /*AFLA*/ 
// /*AFLA*/ 	ns = current->nsproxy->ipc_ns;
// /*AFLA*/ 
// /*AFLA*/ 	if (nsems < 0 || nsems > ns->sc_semmsl)
// /*AFLA*/ 		return -EINVAL;
// /*AFLA*/ 
// /*AFLA*/ 	sem_params.key = key;
// /*AFLA*/ 	sem_params.flg = semflg;
// /*AFLA*/ 	sem_params.u.nsems = nsems;
// /*AFLA*/ 
// /*AFLA*/ 	return ipcget(ns, &sem_ids(ns), &sem_ops, &sem_params);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * perform_atomic_semop - Perform (if possible) a semaphore operation
// /*AFLA*/  * @sma: semaphore array
// /*AFLA*/  * @q: struct sem_queue that describes the operation
// /*AFLA*/  *
// /*AFLA*/  * Returns 0 if the operation was possible.
// /*AFLA*/  * Returns 1 if the operation is impossible, the caller must sleep.
// /*AFLA*/  * Negative values are error codes.
// /*AFLA*/  */
// /*AFLA*/ static int perform_atomic_semop(struct sem_array *sma, struct sem_queue *q)
// /*AFLA*/ {
// /*AFLA*/ 	int result, sem_op, nsops, pid;
// /*AFLA*/ 	struct sembuf *sop;
// /*AFLA*/ 	struct sem *curr;
// /*AFLA*/ 	struct sembuf *sops;
// /*AFLA*/ 	struct sem_undo *un;
// /*AFLA*/ 
// /*AFLA*/ 	sops = q->sops;
// /*AFLA*/ 	nsops = q->nsops;
// /*AFLA*/ 	un = q->undo;
// /*AFLA*/ 
// /*AFLA*/ 	for (sop = sops; sop < sops + nsops; sop++) {
// /*AFLA*/ 		curr = sma->sem_base + sop->sem_num;
// /*AFLA*/ 		sem_op = sop->sem_op;
// /*AFLA*/ 		result = curr->semval;
// /*AFLA*/ 
// /*AFLA*/ 		if (!sem_op && result)
// /*AFLA*/ 			goto would_block;
// /*AFLA*/ 
// /*AFLA*/ 		result += sem_op;
// /*AFLA*/ 		if (result < 0)
// /*AFLA*/ 			goto would_block;
// /*AFLA*/ 		if (result > SEMVMX)
// /*AFLA*/ 			goto out_of_range;
// /*AFLA*/ 
// /*AFLA*/ 		if (sop->sem_flg & SEM_UNDO) {
// /*AFLA*/ 			int undo = un->semadj[sop->sem_num] - sem_op;
// /*AFLA*/ 			/* Exceeding the undo range is an error. */
// /*AFLA*/ 			if (undo < (-SEMAEM - 1) || undo > SEMAEM)
// /*AFLA*/ 				goto out_of_range;
// /*AFLA*/ 			un->semadj[sop->sem_num] = undo;
// /*AFLA*/ 		}
// /*AFLA*/ 
// /*AFLA*/ 		curr->semval = result;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	sop--;
// /*AFLA*/ 	pid = q->pid;
// /*AFLA*/ 	while (sop >= sops) {
// /*AFLA*/ 		sma->sem_base[sop->sem_num].sempid = pid;
// /*AFLA*/ 		sop--;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	return 0;
// /*AFLA*/ 
// /*AFLA*/ out_of_range:
// /*AFLA*/ 	result = -ERANGE;
// /*AFLA*/ 	goto undo;
// /*AFLA*/ 
// /*AFLA*/ would_block:
// /*AFLA*/ 	q->blocking = sop;
// /*AFLA*/ 
// /*AFLA*/ 	if (sop->sem_flg & IPC_NOWAIT)
// /*AFLA*/ 		result = -EAGAIN;
// /*AFLA*/ 	else
// /*AFLA*/ 		result = 1;
// /*AFLA*/ 
// /*AFLA*/ undo:
// /*AFLA*/ 	sop--;
// /*AFLA*/ 	while (sop >= sops) {
// /*AFLA*/ 		sem_op = sop->sem_op;
// /*AFLA*/ 		sma->sem_base[sop->sem_num].semval -= sem_op;
// /*AFLA*/ 		if (sop->sem_flg & SEM_UNDO)
// /*AFLA*/ 			un->semadj[sop->sem_num] += sem_op;
// /*AFLA*/ 		sop--;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	return result;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /** wake_up_sem_queue_prepare(q, error): Prepare wake-up
// /*AFLA*/  * @q: queue entry that must be signaled
// /*AFLA*/  * @error: Error value for the signal
// /*AFLA*/  *
// /*AFLA*/  * Prepare the wake-up of the queue entry q.
// /*AFLA*/  */
// /*AFLA*/ static void wake_up_sem_queue_prepare(struct list_head *pt,
// /*AFLA*/ 				struct sem_queue *q, int error)
// /*AFLA*/ {
// /*AFLA*/ 	if (list_empty(pt)) {
// /*AFLA*/ 		/*
// /*AFLA*/ 		 * Hold preempt off so that we don't get preempted and have the
// /*AFLA*/ 		 * wakee busy-wait until we're scheduled back on.
// /*AFLA*/ 		 */
// /*AFLA*/ 		preempt_disable();
// /*AFLA*/ 	}
// /*AFLA*/ 	q->status = IN_WAKEUP;
// /*AFLA*/ 	q->pid = error;
// /*AFLA*/ 
// /*AFLA*/ 	list_add_tail(&q->list, pt);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wake_up_sem_queue_do - do the actual wake-up
// /*AFLA*/  * @pt: list of tasks to be woken up
// /*AFLA*/  *
// /*AFLA*/  * Do the actual wake-up.
// /*AFLA*/  * The function is called without any locks held, thus the semaphore array
// /*AFLA*/  * could be destroyed already and the tasks can disappear as soon as the
// /*AFLA*/  * status is set to the actual return code.
// /*AFLA*/  */
// /*AFLA*/ static void wake_up_sem_queue_do(struct list_head *pt)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem_queue *q, *t;
// /*AFLA*/ 	int did_something;
// /*AFLA*/ 
// /*AFLA*/ 	did_something = !list_empty(pt);
// /*AFLA*/ 	list_for_each_entry_safe(q, t, pt, list) {
// /*AFLA*/ 		wake_up_process(q->sleeper);
// /*AFLA*/ 		/* q can disappear immediately after writing q->status. */
// /*AFLA*/ 		smp_wmb();
// /*AFLA*/ 		q->status = q->pid;
// /*AFLA*/ 	}
// /*AFLA*/ 	if (did_something)
// /*AFLA*/ 		preempt_enable();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static void unlink_queue(struct sem_array *sma, struct sem_queue *q)
// /*AFLA*/ {
// /*AFLA*/ 	list_del(&q->list);
// /*AFLA*/ 	if (q->nsops > 1)
// /*AFLA*/ 		sma->complex_count--;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /** check_restart(sma, q)
// /*AFLA*/  * @sma: semaphore array
// /*AFLA*/  * @q: the operation that just completed
// /*AFLA*/  *
// /*AFLA*/  * update_queue is O(N^2) when it restarts scanning the whole queue of
// /*AFLA*/  * waiting operations. Therefore this function checks if the restart is
// /*AFLA*/  * really necessary. It is called after a previously waiting operation
// /*AFLA*/  * modified the array.
// /*AFLA*/  * Note that wait-for-zero operations are handled without restart.
// /*AFLA*/  */
// /*AFLA*/ static int check_restart(struct sem_array *sma, struct sem_queue *q)
// /*AFLA*/ {
// /*AFLA*/ 	/* pending complex alter operations are too difficult to analyse */
// /*AFLA*/ 	if (!list_empty(&sma->pending_alter))
// /*AFLA*/ 		return 1;
// /*AFLA*/ 
// /*AFLA*/ 	/* we were a sleeping complex operation. Too difficult */
// /*AFLA*/ 	if (q->nsops > 1)
// /*AFLA*/ 		return 1;
// /*AFLA*/ 
// /*AFLA*/ 	/* It is impossible that someone waits for the new value:
// /*AFLA*/ 	 * - complex operations always restart.
// /*AFLA*/ 	 * - wait-for-zero are handled seperately.
// /*AFLA*/ 	 * - q is a previously sleeping simple operation that
// /*AFLA*/ 	 *   altered the array. It must be a decrement, because
// /*AFLA*/ 	 *   simple increments never sleep.
// /*AFLA*/ 	 * - If there are older (higher priority) decrements
// /*AFLA*/ 	 *   in the queue, then they have observed the original
// /*AFLA*/ 	 *   semval value and couldn't proceed. The operation
// /*AFLA*/ 	 *   decremented to value - thus they won't proceed either.
// /*AFLA*/ 	 */
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * wake_const_ops - wake up non-alter tasks
// /*AFLA*/  * @sma: semaphore array.
// /*AFLA*/  * @semnum: semaphore that was modified.
// /*AFLA*/  * @pt: list head for the tasks that must be woken up.
// /*AFLA*/  *
// /*AFLA*/  * wake_const_ops must be called after a semaphore in a semaphore array
// /*AFLA*/  * was set to 0. If complex const operations are pending, wake_const_ops must
// /*AFLA*/  * be called with semnum = -1, as well as with the number of each modified
// /*AFLA*/  * semaphore.
// /*AFLA*/  * The tasks that must be woken up are added to @pt. The return code
// /*AFLA*/  * is stored in q->pid.
// /*AFLA*/  * The function returns 1 if at least one operation was completed successfully.
// /*AFLA*/  */
// /*AFLA*/ static int wake_const_ops(struct sem_array *sma, int semnum,
// /*AFLA*/ 				struct list_head *pt)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem_queue *q;
// /*AFLA*/ 	struct list_head *walk;
// /*AFLA*/ 	struct list_head *pending_list;
// /*AFLA*/ 	int semop_completed = 0;
// /*AFLA*/ 
// /*AFLA*/ 	if (semnum == -1)
// /*AFLA*/ 		pending_list = &sma->pending_const;
// /*AFLA*/ 	else
// /*AFLA*/ 		pending_list = &sma->sem_base[semnum].pending_const;
// /*AFLA*/ 
// /*AFLA*/ 	walk = pending_list->next;
// /*AFLA*/ 	while (walk != pending_list) {
// /*AFLA*/ 		int error;
// /*AFLA*/ 
// /*AFLA*/ 		q = container_of(walk, struct sem_queue, list);
// /*AFLA*/ 		walk = walk->next;
// /*AFLA*/ 
// /*AFLA*/ 		error = perform_atomic_semop(sma, q);
// /*AFLA*/ 
// /*AFLA*/ 		if (error <= 0) {
// /*AFLA*/ 			/* operation completed, remove from queue & wakeup */
// /*AFLA*/ 
// /*AFLA*/ 			unlink_queue(sma, q);
// /*AFLA*/ 
// /*AFLA*/ 			wake_up_sem_queue_prepare(pt, q, error);
// /*AFLA*/ 			if (error == 0)
// /*AFLA*/ 				semop_completed = 1;
// /*AFLA*/ 		}
// /*AFLA*/ 	}
// /*AFLA*/ 	return semop_completed;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * do_smart_wakeup_zero - wakeup all wait for zero tasks
// /*AFLA*/  * @sma: semaphore array
// /*AFLA*/  * @sops: operations that were performed
// /*AFLA*/  * @nsops: number of operations
// /*AFLA*/  * @pt: list head of the tasks that must be woken up.
// /*AFLA*/  *
// /*AFLA*/  * Checks all required queue for wait-for-zero operations, based
// /*AFLA*/  * on the actual changes that were performed on the semaphore array.
// /*AFLA*/  * The function returns 1 if at least one operation was completed successfully.
// /*AFLA*/  */
// /*AFLA*/ static int do_smart_wakeup_zero(struct sem_array *sma, struct sembuf *sops,
// /*AFLA*/ 					int nsops, struct list_head *pt)
// /*AFLA*/ {
// /*AFLA*/ 	int i;
// /*AFLA*/ 	int semop_completed = 0;
// /*AFLA*/ 	int got_zero = 0;
// /*AFLA*/ 
// /*AFLA*/ 	/* first: the per-semaphore queues, if known */
// /*AFLA*/ 	if (sops) {
// /*AFLA*/ 		for (i = 0; i < nsops; i++) {
// /*AFLA*/ 			int num = sops[i].sem_num;
// /*AFLA*/ 
// /*AFLA*/ 			if (sma->sem_base[num].semval == 0) {
// /*AFLA*/ 				got_zero = 1;
// /*AFLA*/ 				semop_completed |= wake_const_ops(sma, num, pt);
// /*AFLA*/ 			}
// /*AFLA*/ 		}
// /*AFLA*/ 	} else {
// /*AFLA*/ 		/*
// /*AFLA*/ 		 * No sops means modified semaphores not known.
// /*AFLA*/ 		 * Assume all were changed.
// /*AFLA*/ 		 */
// /*AFLA*/ 		for (i = 0; i < sma->sem_nsems; i++) {
// /*AFLA*/ 			if (sma->sem_base[i].semval == 0) {
// /*AFLA*/ 				got_zero = 1;
// /*AFLA*/ 				semop_completed |= wake_const_ops(sma, i, pt);
// /*AFLA*/ 			}
// /*AFLA*/ 		}
// /*AFLA*/ 	}
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If one of the modified semaphores got 0,
// /*AFLA*/ 	 * then check the global queue, too.
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (got_zero)
// /*AFLA*/ 		semop_completed |= wake_const_ops(sma, -1, pt);
// /*AFLA*/ 
// /*AFLA*/ 	return semop_completed;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * update_queue - look for tasks that can be completed.
// /*AFLA*/  * @sma: semaphore array.
// /*AFLA*/  * @semnum: semaphore that was modified.
// /*AFLA*/  * @pt: list head for the tasks that must be woken up.
// /*AFLA*/  *
// /*AFLA*/  * update_queue must be called after a semaphore in a semaphore array
// /*AFLA*/  * was modified. If multiple semaphores were modified, update_queue must
// /*AFLA*/  * be called with semnum = -1, as well as with the number of each modified
// /*AFLA*/  * semaphore.
// /*AFLA*/  * The tasks that must be woken up are added to @pt. The return code
// /*AFLA*/  * is stored in q->pid.
// /*AFLA*/  * The function internally checks if const operations can now succeed.
// /*AFLA*/  *
// /*AFLA*/  * The function return 1 if at least one semop was completed successfully.
// /*AFLA*/  */
// /*AFLA*/ static int update_queue(struct sem_array *sma, int semnum, struct list_head *pt)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem_queue *q;
// /*AFLA*/ 	struct list_head *walk;
// /*AFLA*/ 	struct list_head *pending_list;
// /*AFLA*/ 	int semop_completed = 0;
// /*AFLA*/ 
// /*AFLA*/ 	if (semnum == -1)
// /*AFLA*/ 		pending_list = &sma->pending_alter;
// /*AFLA*/ 	else
// /*AFLA*/ 		pending_list = &sma->sem_base[semnum].pending_alter;
// /*AFLA*/ 
// /*AFLA*/ again:
// /*AFLA*/ 	walk = pending_list->next;
// /*AFLA*/ 	while (walk != pending_list) {
// /*AFLA*/ 		int error, restart;
// /*AFLA*/ 
// /*AFLA*/ 		q = container_of(walk, struct sem_queue, list);
// /*AFLA*/ 		walk = walk->next;
// /*AFLA*/ 
// /*AFLA*/ 		/* If we are scanning the single sop, per-semaphore list of
// /*AFLA*/ 		 * one semaphore and that semaphore is 0, then it is not
// /*AFLA*/ 		 * necessary to scan further: simple increments
// /*AFLA*/ 		 * that affect only one entry succeed immediately and cannot
// /*AFLA*/ 		 * be in the  per semaphore pending queue, and decrements
// /*AFLA*/ 		 * cannot be successful if the value is already 0.
// /*AFLA*/ 		 */
// /*AFLA*/ 		if (semnum != -1 && sma->sem_base[semnum].semval == 0)
// /*AFLA*/ 			break;
// /*AFLA*/ 
// /*AFLA*/ 		error = perform_atomic_semop(sma, q);
// /*AFLA*/ 
// /*AFLA*/ 		/* Does q->sleeper still need to sleep? */
// /*AFLA*/ 		if (error > 0)
// /*AFLA*/ 			continue;
// /*AFLA*/ 
// /*AFLA*/ 		unlink_queue(sma, q);
// /*AFLA*/ 
// /*AFLA*/ 		if (error) {
// /*AFLA*/ 			restart = 0;
// /*AFLA*/ 		} else {
// /*AFLA*/ 			semop_completed = 1;
// /*AFLA*/ 			do_smart_wakeup_zero(sma, q->sops, q->nsops, pt);
// /*AFLA*/ 			restart = check_restart(sma, q);
// /*AFLA*/ 		}
// /*AFLA*/ 
// /*AFLA*/ 		wake_up_sem_queue_prepare(pt, q, error);
// /*AFLA*/ 		if (restart)
// /*AFLA*/ 			goto again;
// /*AFLA*/ 	}
// /*AFLA*/ 	return semop_completed;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * set_semotime - set sem_otime
// /*AFLA*/  * @sma: semaphore array
// /*AFLA*/  * @sops: operations that modified the array, may be NULL
// /*AFLA*/  *
// /*AFLA*/  * sem_otime is replicated to avoid cache line trashing.
// /*AFLA*/  * This function sets one instance to the current time.
// /*AFLA*/  */
// /*AFLA*/ static void set_semotime(struct sem_array *sma, struct sembuf *sops)
// /*AFLA*/ {
// /*AFLA*/ 	if (sops == NULL) {
// /*AFLA*/ 		sma->sem_base[0].sem_otime = get_seconds();
// /*AFLA*/ 	} else {
// /*AFLA*/ 		sma->sem_base[sops[0].sem_num].sem_otime =
// /*AFLA*/ 							get_seconds();
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * do_smart_update - optimized update_queue
// /*AFLA*/  * @sma: semaphore array
// /*AFLA*/  * @sops: operations that were performed
// /*AFLA*/  * @nsops: number of operations
// /*AFLA*/  * @otime: force setting otime
// /*AFLA*/  * @pt: list head of the tasks that must be woken up.
// /*AFLA*/  *
// /*AFLA*/  * do_smart_update() does the required calls to update_queue and wakeup_zero,
// /*AFLA*/  * based on the actual changes that were performed on the semaphore array.
// /*AFLA*/  * Note that the function does not do the actual wake-up: the caller is
// /*AFLA*/  * responsible for calling wake_up_sem_queue_do(@pt).
// /*AFLA*/  * It is safe to perform this call after dropping all locks.
// /*AFLA*/  */
// /*AFLA*/ static void do_smart_update(struct sem_array *sma, struct sembuf *sops, int nsops,
// /*AFLA*/ 			int otime, struct list_head *pt)
// /*AFLA*/ {
// /*AFLA*/ 	int i;
// /*AFLA*/ 
// /*AFLA*/ 	otime |= do_smart_wakeup_zero(sma, sops, nsops, pt);
// /*AFLA*/ 
// /*AFLA*/ 	if (!list_empty(&sma->pending_alter)) {
// /*AFLA*/ 		/* semaphore array uses the global queue - just process it. */
// /*AFLA*/ 		otime |= update_queue(sma, -1, pt);
// /*AFLA*/ 	} else {
// /*AFLA*/ 		if (!sops) {
// /*AFLA*/ 			/*
// /*AFLA*/ 			 * No sops, thus the modified semaphores are not
// /*AFLA*/ 			 * known. Check all.
// /*AFLA*/ 			 */
// /*AFLA*/ 			for (i = 0; i < sma->sem_nsems; i++)
// /*AFLA*/ 				otime |= update_queue(sma, i, pt);
// /*AFLA*/ 		} else {
// /*AFLA*/ 			/*
// /*AFLA*/ 			 * Check the semaphores that were increased:
// /*AFLA*/ 			 * - No complex ops, thus all sleeping ops are
// /*AFLA*/ 			 *   decrease.
// /*AFLA*/ 			 * - if we decreased the value, then any sleeping
// /*AFLA*/ 			 *   semaphore ops wont be able to run: If the
// /*AFLA*/ 			 *   previous value was too small, then the new
// /*AFLA*/ 			 *   value will be too small, too.
// /*AFLA*/ 			 */
// /*AFLA*/ 			for (i = 0; i < nsops; i++) {
// /*AFLA*/ 				if (sops[i].sem_op > 0) {
// /*AFLA*/ 					otime |= update_queue(sma,
// /*AFLA*/ 							sops[i].sem_num, pt);
// /*AFLA*/ 				}
// /*AFLA*/ 			}
// /*AFLA*/ 		}
// /*AFLA*/ 	}
// /*AFLA*/ 	if (otime)
// /*AFLA*/ 		set_semotime(sma, sops);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * check_qop: Test if a queued operation sleeps on the semaphore semnum
// /*AFLA*/  */
// /*AFLA*/ static int check_qop(struct sem_array *sma, int semnum, struct sem_queue *q,
// /*AFLA*/ 			bool count_zero)
// /*AFLA*/ {
// /*AFLA*/ 	struct sembuf *sop = q->blocking;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Linux always (since 0.99.10) reported a task as sleeping on all
// /*AFLA*/ 	 * semaphores. This violates SUS, therefore it was changed to the
// /*AFLA*/ 	 * standard compliant behavior.
// /*AFLA*/ 	 * Give the administrators a chance to notice that an application
// /*AFLA*/ 	 * might misbehave because it relies on the Linux behavior.
// /*AFLA*/ 	 */
// /*AFLA*/ 	pr_info_once("semctl(GETNCNT/GETZCNT) is since 3.16 Single Unix Specification compliant.\n"
// /*AFLA*/ 			"The task %s (%d) triggered the difference, watch for misbehavior.\n",
// /*AFLA*/ 			current->comm, task_pid_nr(current));
// /*AFLA*/ 
// /*AFLA*/ 	if (sop->sem_num != semnum)
// /*AFLA*/ 		return 0;
// /*AFLA*/ 
// /*AFLA*/ 	if (count_zero && sop->sem_op == 0)
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	if (!count_zero && sop->sem_op < 0)
// /*AFLA*/ 		return 1;
// /*AFLA*/ 
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* The following counts are associated to each semaphore:
// /*AFLA*/  *   semncnt        number of tasks waiting on semval being nonzero
// /*AFLA*/  *   semzcnt        number of tasks waiting on semval being zero
// /*AFLA*/  *
// /*AFLA*/  * Per definition, a task waits only on the semaphore of the first semop
// /*AFLA*/  * that cannot proceed, even if additional operation would block, too.
// /*AFLA*/  */
// /*AFLA*/ static int count_semcnt(struct sem_array *sma, ushort semnum,
// /*AFLA*/ 			bool count_zero)
// /*AFLA*/ {
// /*AFLA*/ 	struct list_head *l;
// /*AFLA*/ 	struct sem_queue *q;
// /*AFLA*/ 	int semcnt;
// /*AFLA*/ 
// /*AFLA*/ 	semcnt = 0;
// /*AFLA*/ 	/* First: check the simple operations. They are easy to evaluate */
// /*AFLA*/ 	if (count_zero)
// /*AFLA*/ 		l = &sma->sem_base[semnum].pending_const;
// /*AFLA*/ 	else
// /*AFLA*/ 		l = &sma->sem_base[semnum].pending_alter;
// /*AFLA*/ 
// /*AFLA*/ 	list_for_each_entry(q, l, list) {
// /*AFLA*/ 		/* all task on a per-semaphore list sleep on exactly
// /*AFLA*/ 		 * that semaphore
// /*AFLA*/ 		 */
// /*AFLA*/ 		semcnt++;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	/* Then: check the complex operations. */
// /*AFLA*/ 	list_for_each_entry(q, &sma->pending_alter, list) {
// /*AFLA*/ 		semcnt += check_qop(sma, semnum, q, count_zero);
// /*AFLA*/ 	}
// /*AFLA*/ 	if (count_zero) {
// /*AFLA*/ 		list_for_each_entry(q, &sma->pending_const, list) {
// /*AFLA*/ 			semcnt += check_qop(sma, semnum, q, count_zero);
// /*AFLA*/ 		}
// /*AFLA*/ 	}
// /*AFLA*/ 	return semcnt;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Free a semaphore set. freeary() is called with sem_ids.rwsem locked
// /*AFLA*/  * as a writer and the spinlock for this semaphore set hold. sem_ids.rwsem
// /*AFLA*/  * remains locked on exit.
// /*AFLA*/  */
// /*AFLA*/ static void freeary(struct ipc_namespace *ns, struct kern_ipc_perm *ipcp)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem_undo *un, *tu;
// /*AFLA*/ 	struct sem_queue *q, *tq;
// /*AFLA*/ 	struct sem_array *sma = container_of(ipcp, struct sem_array, sem_perm);
// /*AFLA*/ 	struct list_head tasks;
// /*AFLA*/ 	int i;
// /*AFLA*/ 
// /*AFLA*/ 	/* Free the existing undo structures for this semaphore set.  */
// /*AFLA*/ 	ipc_assert_locked_object(&sma->sem_perm);
// /*AFLA*/ 	list_for_each_entry_safe(un, tu, &sma->list_id, list_id) {
// /*AFLA*/ 		list_del(&un->list_id);
// /*AFLA*/ 		spin_lock(&un->ulp->lock);
// /*AFLA*/ 		un->semid = -1;
// /*AFLA*/ 		list_del_rcu(&un->list_proc);
// /*AFLA*/ 		spin_unlock(&un->ulp->lock);
// /*AFLA*/ 		kfree_rcu(un, rcu);
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	/* Wake up all pending processes and let them fail with EIDRM. */
// /*AFLA*/ 	INIT_LIST_HEAD(&tasks);
// /*AFLA*/ 	list_for_each_entry_safe(q, tq, &sma->pending_const, list) {
// /*AFLA*/ 		unlink_queue(sma, q);
// /*AFLA*/ 		wake_up_sem_queue_prepare(&tasks, q, -EIDRM);
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	list_for_each_entry_safe(q, tq, &sma->pending_alter, list) {
// /*AFLA*/ 		unlink_queue(sma, q);
// /*AFLA*/ 		wake_up_sem_queue_prepare(&tasks, q, -EIDRM);
// /*AFLA*/ 	}
// /*AFLA*/ 	for (i = 0; i < sma->sem_nsems; i++) {
// /*AFLA*/ 		struct sem *sem = sma->sem_base + i;
// /*AFLA*/ 		list_for_each_entry_safe(q, tq, &sem->pending_const, list) {
// /*AFLA*/ 			unlink_queue(sma, q);
// /*AFLA*/ 			wake_up_sem_queue_prepare(&tasks, q, -EIDRM);
// /*AFLA*/ 		}
// /*AFLA*/ 		list_for_each_entry_safe(q, tq, &sem->pending_alter, list) {
// /*AFLA*/ 			unlink_queue(sma, q);
// /*AFLA*/ 			wake_up_sem_queue_prepare(&tasks, q, -EIDRM);
// /*AFLA*/ 		}
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	/* Remove the semaphore set from the IDR */
// /*AFLA*/ 	sem_rmid(ns, sma);
// /*AFLA*/ 	sem_unlock(sma, -1);
// /*AFLA*/ 	rcu_read_unlock();
// /*AFLA*/ 
// /*AFLA*/ 	wake_up_sem_queue_do(&tasks);
// /*AFLA*/ 	ns->used_sems -= sma->sem_nsems;
// /*AFLA*/ 	ipc_rcu_putref(sma, sem_rcu_free);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static unsigned long copy_semid_to_user(void __user *buf, struct semid64_ds *in, int version)
// /*AFLA*/ {
// /*AFLA*/ 	switch (version) {
// /*AFLA*/ 	case IPC_64:
// /*AFLA*/ 		return copy_to_user(buf, in, sizeof(*in));
// /*AFLA*/ 	case IPC_OLD:
// /*AFLA*/ 	    {
// /*AFLA*/ 		struct semid_ds out;
// /*AFLA*/ 
// /*AFLA*/ 		memset(&out, 0, sizeof(out));
// /*AFLA*/ 
// /*AFLA*/ 		ipc64_perm_to_ipc_perm(&in->sem_perm, &out.sem_perm);
// /*AFLA*/ 
// /*AFLA*/ 		out.sem_otime	= in->sem_otime;
// /*AFLA*/ 		out.sem_ctime	= in->sem_ctime;
// /*AFLA*/ 		out.sem_nsems	= in->sem_nsems;
// /*AFLA*/ 
// /*AFLA*/ 		return copy_to_user(buf, &out, sizeof(out));
// /*AFLA*/ 	    }
// /*AFLA*/ 	default:
// /*AFLA*/ 		return -EINVAL;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static time_t get_semotime(struct sem_array *sma)
// /*AFLA*/ {
// /*AFLA*/ 	int i;
// /*AFLA*/ 	time_t res;
// /*AFLA*/ 
// /*AFLA*/ 	res = sma->sem_base[0].sem_otime;
// /*AFLA*/ 	for (i = 1; i < sma->sem_nsems; i++) {
// /*AFLA*/ 		time_t to = sma->sem_base[i].sem_otime;
// /*AFLA*/ 
// /*AFLA*/ 		if (to > res)
// /*AFLA*/ 			res = to;
// /*AFLA*/ 	}
// /*AFLA*/ 	return res;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static int semctl_nolock(struct ipc_namespace *ns, int semid,
// /*AFLA*/ 			 int cmd, int version, void __user *p)
// /*AFLA*/ {
// /*AFLA*/ 	int err;
// /*AFLA*/ 	struct sem_array *sma;
// /*AFLA*/ 
// /*AFLA*/ 	switch (cmd) {
// /*AFLA*/ 	case IPC_INFO:
// /*AFLA*/ 	case SEM_INFO:
// /*AFLA*/ 	{
// /*AFLA*/ 		struct seminfo seminfo;
// /*AFLA*/ 		int max_id;
// /*AFLA*/ 
// /*AFLA*/ 		err = security_sem_semctl(NULL, cmd);
// /*AFLA*/ 		if (err)
// /*AFLA*/ 			return err;
// /*AFLA*/ 
// /*AFLA*/ 		memset(&seminfo, 0, sizeof(seminfo));
// /*AFLA*/ 		seminfo.semmni = ns->sc_semmni;
// /*AFLA*/ 		seminfo.semmns = ns->sc_semmns;
// /*AFLA*/ 		seminfo.semmsl = ns->sc_semmsl;
// /*AFLA*/ 		seminfo.semopm = ns->sc_semopm;
// /*AFLA*/ 		seminfo.semvmx = SEMVMX;
// /*AFLA*/ 		seminfo.semmnu = SEMMNU;
// /*AFLA*/ 		seminfo.semmap = SEMMAP;
// /*AFLA*/ 		seminfo.semume = SEMUME;
// /*AFLA*/ 		down_read(&sem_ids(ns).rwsem);
// /*AFLA*/ 		if (cmd == SEM_INFO) {
// /*AFLA*/ 			seminfo.semusz = sem_ids(ns).in_use;
// /*AFLA*/ 			seminfo.semaem = ns->used_sems;
// /*AFLA*/ 		} else {
// /*AFLA*/ 			seminfo.semusz = SEMUSZ;
// /*AFLA*/ 			seminfo.semaem = SEMAEM;
// /*AFLA*/ 		}
// /*AFLA*/ 		max_id = ipc_get_maxid(&sem_ids(ns));
// /*AFLA*/ 		up_read(&sem_ids(ns).rwsem);
// /*AFLA*/ 		if (copy_to_user(p, &seminfo, sizeof(struct seminfo)))
// /*AFLA*/ 			return -EFAULT;
// /*AFLA*/ 		return (max_id < 0) ? 0 : max_id;
// /*AFLA*/ 	}
// /*AFLA*/ 	case IPC_STAT:
// /*AFLA*/ 	case SEM_STAT:
// /*AFLA*/ 	{
// /*AFLA*/ 		struct semid64_ds tbuf;
// /*AFLA*/ 		int id = 0;
// /*AFLA*/ 
// /*AFLA*/ 		memset(&tbuf, 0, sizeof(tbuf));
// /*AFLA*/ 
// /*AFLA*/ 		rcu_read_lock();
// /*AFLA*/ 		if (cmd == SEM_STAT) {
// /*AFLA*/ 			sma = sem_obtain_object(ns, semid);
// /*AFLA*/ 			if (IS_ERR(sma)) {
// /*AFLA*/ 				err = PTR_ERR(sma);
// /*AFLA*/ 				goto out_unlock;
// /*AFLA*/ 			}
// /*AFLA*/ 			id = sma->sem_perm.id;
// /*AFLA*/ 		} else {
// /*AFLA*/ 			sma = sem_obtain_object_check(ns, semid);
// /*AFLA*/ 			if (IS_ERR(sma)) {
// /*AFLA*/ 				err = PTR_ERR(sma);
// /*AFLA*/ 				goto out_unlock;
// /*AFLA*/ 			}
// /*AFLA*/ 		}
// /*AFLA*/ 
// /*AFLA*/ 		err = -EACCES;
// /*AFLA*/ 		if (ipcperms(ns, &sma->sem_perm, S_IRUGO))
// /*AFLA*/ 			goto out_unlock;
// /*AFLA*/ 
// /*AFLA*/ 		err = security_sem_semctl(sma, cmd);
// /*AFLA*/ 		if (err)
// /*AFLA*/ 			goto out_unlock;
// /*AFLA*/ 
// /*AFLA*/ 		kernel_to_ipc64_perm(&sma->sem_perm, &tbuf.sem_perm);
// /*AFLA*/ 		tbuf.sem_otime = get_semotime(sma);
// /*AFLA*/ 		tbuf.sem_ctime = sma->sem_ctime;
// /*AFLA*/ 		tbuf.sem_nsems = sma->sem_nsems;
// /*AFLA*/ 		rcu_read_unlock();
// /*AFLA*/ 		if (copy_semid_to_user(p, &tbuf, version))
// /*AFLA*/ 			return -EFAULT;
// /*AFLA*/ 		return id;
// /*AFLA*/ 	}
// /*AFLA*/ 	default:
// /*AFLA*/ 		return -EINVAL;
// /*AFLA*/ 	}
// /*AFLA*/ out_unlock:
// /*AFLA*/ 	rcu_read_unlock();
// /*AFLA*/ 	return err;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static int semctl_setval(struct ipc_namespace *ns, int semid, int semnum,
// /*AFLA*/ 		unsigned long arg)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem_undo *un;
// /*AFLA*/ 	struct sem_array *sma;
// /*AFLA*/ 	struct sem *curr;
// /*AFLA*/ 	int err;
// /*AFLA*/ 	struct list_head tasks;
// /*AFLA*/ 	int val;
// /*AFLA*/ #if defined(CONFIG_64BIT) && defined(__BIG_ENDIAN)
// /*AFLA*/ 	/* big-endian 64bit */
// /*AFLA*/ 	val = arg >> 32;
// /*AFLA*/ #else
// /*AFLA*/ 	/* 32bit or little-endian 64bit */
// /*AFLA*/ 	val = arg;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	if (val > SEMVMX || val < 0)
// /*AFLA*/ 		return -ERANGE;
// /*AFLA*/ 
// /*AFLA*/ 	INIT_LIST_HEAD(&tasks);
// /*AFLA*/ 
// /*AFLA*/ 	rcu_read_lock();
// /*AFLA*/ 	sma = sem_obtain_object_check(ns, semid);
// /*AFLA*/ 	if (IS_ERR(sma)) {
// /*AFLA*/ 		rcu_read_unlock();
// /*AFLA*/ 		return PTR_ERR(sma);
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	if (semnum < 0 || semnum >= sma->sem_nsems) {
// /*AFLA*/ 		rcu_read_unlock();
// /*AFLA*/ 		return -EINVAL;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ 	if (ipcperms(ns, &sma->sem_perm, S_IWUGO)) {
// /*AFLA*/ 		rcu_read_unlock();
// /*AFLA*/ 		return -EACCES;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	err = security_sem_semctl(sma, SETVAL);
// /*AFLA*/ 	if (err) {
// /*AFLA*/ 		rcu_read_unlock();
// /*AFLA*/ 		return -EACCES;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	sem_lock(sma, NULL, -1);
// /*AFLA*/ 
// /*AFLA*/ 	if (!ipc_valid_object(&sma->sem_perm)) {
// /*AFLA*/ 		sem_unlock(sma, -1);
// /*AFLA*/ 		rcu_read_unlock();
// /*AFLA*/ 		return -EIDRM;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	curr = &sma->sem_base[semnum];
// /*AFLA*/ 
// /*AFLA*/ 	ipc_assert_locked_object(&sma->sem_perm);
// /*AFLA*/ 	list_for_each_entry(un, &sma->list_id, list_id)
// /*AFLA*/ 		un->semadj[semnum] = 0;
// /*AFLA*/ 
// /*AFLA*/ 	curr->semval = val;
// /*AFLA*/ 	curr->sempid = task_tgid_vnr(current);
// /*AFLA*/ 	sma->sem_ctime = get_seconds();
// /*AFLA*/ 	/* maybe some queued-up processes were waiting for this */
// /*AFLA*/ 	do_smart_update(sma, NULL, 0, 0, &tasks);
// /*AFLA*/ 	sem_unlock(sma, -1);
// /*AFLA*/ 	rcu_read_unlock();
// /*AFLA*/ 	wake_up_sem_queue_do(&tasks);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static int semctl_main(struct ipc_namespace *ns, int semid, int semnum,
// /*AFLA*/ 		int cmd, void __user *p)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem_array *sma;
// /*AFLA*/ 	struct sem *curr;
// /*AFLA*/ 	int err, nsems;
// /*AFLA*/ 	ushort fast_sem_io[SEMMSL_FAST];
// /*AFLA*/ 	ushort *sem_io = fast_sem_io;
// /*AFLA*/ 	struct list_head tasks;
// /*AFLA*/ 
// /*AFLA*/ 	INIT_LIST_HEAD(&tasks);
// /*AFLA*/ 
// /*AFLA*/ 	rcu_read_lock();
// /*AFLA*/ 	sma = sem_obtain_object_check(ns, semid);
// /*AFLA*/ 	if (IS_ERR(sma)) {
// /*AFLA*/ 		rcu_read_unlock();
// /*AFLA*/ 		return PTR_ERR(sma);
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	nsems = sma->sem_nsems;
// /*AFLA*/ 
// /*AFLA*/ 	err = -EACCES;
// /*AFLA*/ 	if (ipcperms(ns, &sma->sem_perm, cmd == SETALL ? S_IWUGO : S_IRUGO))
// /*AFLA*/ 		goto out_rcu_wakeup;
// /*AFLA*/ 
// /*AFLA*/ 	err = security_sem_semctl(sma, cmd);
// /*AFLA*/ 	if (err)
// /*AFLA*/ 		goto out_rcu_wakeup;
// /*AFLA*/ 
// /*AFLA*/ 	err = -EACCES;
// /*AFLA*/ 	switch (cmd) {
// /*AFLA*/ 	case GETALL:
// /*AFLA*/ 	{
// /*AFLA*/ 		ushort __user *array = p;
// /*AFLA*/ 		int i;
// /*AFLA*/ 
// /*AFLA*/ 		sem_lock(sma, NULL, -1);
// /*AFLA*/ 		if (!ipc_valid_object(&sma->sem_perm)) {
// /*AFLA*/ 			err = -EIDRM;
// /*AFLA*/ 			goto out_unlock;
// /*AFLA*/ 		}
// /*AFLA*/ 		if (nsems > SEMMSL_FAST) {
// /*AFLA*/ 			if (!ipc_rcu_getref(sma)) {
// /*AFLA*/ 				err = -EIDRM;
// /*AFLA*/ 				goto out_unlock;
// /*AFLA*/ 			}
// /*AFLA*/ 			sem_unlock(sma, -1);
// /*AFLA*/ 			rcu_read_unlock();
// /*AFLA*/ 			sem_io = ipc_alloc(sizeof(ushort)*nsems);
// /*AFLA*/ 			if (sem_io == NULL) {
// /*AFLA*/ 				ipc_rcu_putref(sma, sem_rcu_free);
// /*AFLA*/ 				return -ENOMEM;
// /*AFLA*/ 			}
// /*AFLA*/ 
// /*AFLA*/ 			rcu_read_lock();
// /*AFLA*/ 			sem_lock_and_putref(sma);
// /*AFLA*/ 			if (!ipc_valid_object(&sma->sem_perm)) {
// /*AFLA*/ 				err = -EIDRM;
// /*AFLA*/ 				goto out_unlock;
// /*AFLA*/ 			}
// /*AFLA*/ 		}
// /*AFLA*/ 		for (i = 0; i < sma->sem_nsems; i++)
// /*AFLA*/ 			sem_io[i] = sma->sem_base[i].semval;
// /*AFLA*/ 		sem_unlock(sma, -1);
// /*AFLA*/ 		rcu_read_unlock();
// /*AFLA*/ 		err = 0;
// /*AFLA*/ 		if (copy_to_user(array, sem_io, nsems*sizeof(ushort)))
// /*AFLA*/ 			err = -EFAULT;
// /*AFLA*/ 		goto out_free;
// /*AFLA*/ 	}
// /*AFLA*/ 	case SETALL:
// /*AFLA*/ 	{
// /*AFLA*/ 		int i;
// /*AFLA*/ 		struct sem_undo *un;
// /*AFLA*/ 
// /*AFLA*/ 		if (!ipc_rcu_getref(sma)) {
// /*AFLA*/ 			err = -EIDRM;
// /*AFLA*/ 			goto out_rcu_wakeup;
// /*AFLA*/ 		}
// /*AFLA*/ 		rcu_read_unlock();
// /*AFLA*/ 
// /*AFLA*/ 		if (nsems > SEMMSL_FAST) {
// /*AFLA*/ 			sem_io = ipc_alloc(sizeof(ushort)*nsems);
// /*AFLA*/ 			if (sem_io == NULL) {
// /*AFLA*/ 				ipc_rcu_putref(sma, sem_rcu_free);
// /*AFLA*/ 				return -ENOMEM;
// /*AFLA*/ 			}
// /*AFLA*/ 		}
// /*AFLA*/ 
// /*AFLA*/ 		if (copy_from_user(sem_io, p, nsems*sizeof(ushort))) {
// /*AFLA*/ 			ipc_rcu_putref(sma, sem_rcu_free);
// /*AFLA*/ 			err = -EFAULT;
// /*AFLA*/ 			goto out_free;
// /*AFLA*/ 		}
// /*AFLA*/ 
// /*AFLA*/ 		for (i = 0; i < nsems; i++) {
// /*AFLA*/ 			if (sem_io[i] > SEMVMX) {
// /*AFLA*/ 				ipc_rcu_putref(sma, sem_rcu_free);
// /*AFLA*/ 				err = -ERANGE;
// /*AFLA*/ 				goto out_free;
// /*AFLA*/ 			}
// /*AFLA*/ 		}
// /*AFLA*/ 		rcu_read_lock();
// /*AFLA*/ 		sem_lock_and_putref(sma);
// /*AFLA*/ 		if (!ipc_valid_object(&sma->sem_perm)) {
// /*AFLA*/ 			err = -EIDRM;
// /*AFLA*/ 			goto out_unlock;
// /*AFLA*/ 		}
// /*AFLA*/ 
// /*AFLA*/ 		for (i = 0; i < nsems; i++) {
// /*AFLA*/ 			sma->sem_base[i].semval = sem_io[i];
// /*AFLA*/ 			sma->sem_base[i].sempid = task_tgid_vnr(current);
// /*AFLA*/ 		}
// /*AFLA*/ 
// /*AFLA*/ 		ipc_assert_locked_object(&sma->sem_perm);
// /*AFLA*/ 		list_for_each_entry(un, &sma->list_id, list_id) {
// /*AFLA*/ 			for (i = 0; i < nsems; i++)
// /*AFLA*/ 				un->semadj[i] = 0;
// /*AFLA*/ 		}
// /*AFLA*/ 		sma->sem_ctime = get_seconds();
// /*AFLA*/ 		/* maybe some queued-up processes were waiting for this */
// /*AFLA*/ 		do_smart_update(sma, NULL, 0, 0, &tasks);
// /*AFLA*/ 		err = 0;
// /*AFLA*/ 		goto out_unlock;
// /*AFLA*/ 	}
// /*AFLA*/ 	/* GETVAL, GETPID, GETNCTN, GETZCNT: fall-through */
// /*AFLA*/ 	}
// /*AFLA*/ 	err = -EINVAL;
// /*AFLA*/ 	if (semnum < 0 || semnum >= nsems)
// /*AFLA*/ 		goto out_rcu_wakeup;
// /*AFLA*/ 
// /*AFLA*/ 	sem_lock(sma, NULL, -1);
// /*AFLA*/ 	if (!ipc_valid_object(&sma->sem_perm)) {
// /*AFLA*/ 		err = -EIDRM;
// /*AFLA*/ 		goto out_unlock;
// /*AFLA*/ 	}
// /*AFLA*/ 	curr = &sma->sem_base[semnum];
// /*AFLA*/ 
// /*AFLA*/ 	switch (cmd) {
// /*AFLA*/ 	case GETVAL:
// /*AFLA*/ 		err = curr->semval;
// /*AFLA*/ 		goto out_unlock;
// /*AFLA*/ 	case GETPID:
// /*AFLA*/ 		err = curr->sempid;
// /*AFLA*/ 		goto out_unlock;
// /*AFLA*/ 	case GETNCNT:
// /*AFLA*/ 		err = count_semcnt(sma, semnum, 0);
// /*AFLA*/ 		goto out_unlock;
// /*AFLA*/ 	case GETZCNT:
// /*AFLA*/ 		err = count_semcnt(sma, semnum, 1);
// /*AFLA*/ 		goto out_unlock;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ out_unlock:
// /*AFLA*/ 	sem_unlock(sma, -1);
// /*AFLA*/ out_rcu_wakeup:
// /*AFLA*/ 	rcu_read_unlock();
// /*AFLA*/ 	wake_up_sem_queue_do(&tasks);
// /*AFLA*/ out_free:
// /*AFLA*/ 	if (sem_io != fast_sem_io)
// /*AFLA*/ 		ipc_free(sem_io);
// /*AFLA*/ 	return err;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long
// /*AFLA*/ copy_semid_from_user(struct semid64_ds *out, void __user *buf, int version)
// /*AFLA*/ {
// /*AFLA*/ 	switch (version) {
// /*AFLA*/ 	case IPC_64:
// /*AFLA*/ 		if (copy_from_user(out, buf, sizeof(*out)))
// /*AFLA*/ 			return -EFAULT;
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	case IPC_OLD:
// /*AFLA*/ 	    {
// /*AFLA*/ 		struct semid_ds tbuf_old;
// /*AFLA*/ 
// /*AFLA*/ 		if (copy_from_user(&tbuf_old, buf, sizeof(tbuf_old)))
// /*AFLA*/ 			return -EFAULT;
// /*AFLA*/ 
// /*AFLA*/ 		out->sem_perm.uid	= tbuf_old.sem_perm.uid;
// /*AFLA*/ 		out->sem_perm.gid	= tbuf_old.sem_perm.gid;
// /*AFLA*/ 		out->sem_perm.mode	= tbuf_old.sem_perm.mode;
// /*AFLA*/ 
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	    }
// /*AFLA*/ 	default:
// /*AFLA*/ 		return -EINVAL;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This function handles some semctl commands which require the rwsem
// /*AFLA*/  * to be held in write mode.
// /*AFLA*/  * NOTE: no locks must be held, the rwsem is taken inside this function.
// /*AFLA*/  */
// /*AFLA*/ static int semctl_down(struct ipc_namespace *ns, int semid,
// /*AFLA*/ 		       int cmd, int version, void __user *p)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem_array *sma;
// /*AFLA*/ 	int err;
// /*AFLA*/ 	struct semid64_ds semid64;
// /*AFLA*/ 	struct kern_ipc_perm *ipcp;
// /*AFLA*/ 
// /*AFLA*/ 	if (cmd == IPC_SET) {
// /*AFLA*/ 		if (copy_semid_from_user(&semid64, p, version))
// /*AFLA*/ 			return -EFAULT;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	down_write(&sem_ids(ns).rwsem);
// /*AFLA*/ 	rcu_read_lock();
// /*AFLA*/ 
// /*AFLA*/ 	ipcp = ipcctl_pre_down_nolock(ns, &sem_ids(ns), semid, cmd,
// /*AFLA*/ 				      &semid64.sem_perm, 0);
// /*AFLA*/ 	if (IS_ERR(ipcp)) {
// /*AFLA*/ 		err = PTR_ERR(ipcp);
// /*AFLA*/ 		goto out_unlock1;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	sma = container_of(ipcp, struct sem_array, sem_perm);
// /*AFLA*/ 
// /*AFLA*/ 	err = security_sem_semctl(sma, cmd);
// /*AFLA*/ 	if (err)
// /*AFLA*/ 		goto out_unlock1;
// /*AFLA*/ 
// /*AFLA*/ 	switch (cmd) {
// /*AFLA*/ 	case IPC_RMID:
// /*AFLA*/ 		sem_lock(sma, NULL, -1);
// /*AFLA*/ 		/* freeary unlocks the ipc object and rcu */
// /*AFLA*/ 		freeary(ns, ipcp);
// /*AFLA*/ 		goto out_up;
// /*AFLA*/ 	case IPC_SET:
// /*AFLA*/ 		sem_lock(sma, NULL, -1);
// /*AFLA*/ 		err = ipc_update_perm(&semid64.sem_perm, ipcp);
// /*AFLA*/ 		if (err)
// /*AFLA*/ 			goto out_unlock0;
// /*AFLA*/ 		sma->sem_ctime = get_seconds();
// /*AFLA*/ 		break;
// /*AFLA*/ 	default:
// /*AFLA*/ 		err = -EINVAL;
// /*AFLA*/ 		goto out_unlock1;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ out_unlock0:
// /*AFLA*/ 	sem_unlock(sma, -1);
// /*AFLA*/ out_unlock1:
// /*AFLA*/ 	rcu_read_unlock();
// /*AFLA*/ out_up:
// /*AFLA*/ 	up_write(&sem_ids(ns).rwsem);
// /*AFLA*/ 	return err;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ SYSCALL_DEFINE4(semctl, int, semid, int, semnum, int, cmd, unsigned long, arg)
// /*AFLA*/ {
// /*AFLA*/ 	int version;
// /*AFLA*/ 	struct ipc_namespace *ns;
// /*AFLA*/ 	void __user *p = (void __user *)arg;
// /*AFLA*/ 
// /*AFLA*/ 	if (semid < 0)
// /*AFLA*/ 		return -EINVAL;
// /*AFLA*/ 
// /*AFLA*/ 	version = ipc_parse_version(&cmd);
// /*AFLA*/ 	ns = current->nsproxy->ipc_ns;
// /*AFLA*/ 
// /*AFLA*/ 	switch (cmd) {
// /*AFLA*/ 	case IPC_INFO:
// /*AFLA*/ 	case SEM_INFO:
// /*AFLA*/ 	case IPC_STAT:
// /*AFLA*/ 	case SEM_STAT:
// /*AFLA*/ 		return semctl_nolock(ns, semid, cmd, version, p);
// /*AFLA*/ 	case GETALL:
// /*AFLA*/ 	case GETVAL:
// /*AFLA*/ 	case GETPID:
// /*AFLA*/ 	case GETNCNT:
// /*AFLA*/ 	case GETZCNT:
// /*AFLA*/ 	case SETALL:
// /*AFLA*/ 		return semctl_main(ns, semid, semnum, cmd, p);
// /*AFLA*/ 	case SETVAL:
// /*AFLA*/ 		return semctl_setval(ns, semid, semnum, arg);
// /*AFLA*/ 	case IPC_RMID:
// /*AFLA*/ 	case IPC_SET:
// /*AFLA*/ 		return semctl_down(ns, semid, cmd, version, p);
// /*AFLA*/ 	default:
// /*AFLA*/ 		return -EINVAL;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* If the task doesn't already have a undo_list, then allocate one
// /*AFLA*/  * here.  We guarantee there is only one thread using this undo list,
// /*AFLA*/  * and current is THE ONE
// /*AFLA*/  *
// /*AFLA*/  * If this allocation and assignment succeeds, but later
// /*AFLA*/  * portions of this code fail, there is no need to free the sem_undo_list.
// /*AFLA*/  * Just let it stay associated with the task, and it'll be freed later
// /*AFLA*/  * at exit time.
// /*AFLA*/  *
// /*AFLA*/  * This can block, so callers must hold no locks.
// /*AFLA*/  */
// /*AFLA*/ static inline int get_undo_list(struct sem_undo_list **undo_listp)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem_undo_list *undo_list;
// /*AFLA*/ 
// /*AFLA*/ 	undo_list = current->sysvsem.undo_list;
// /*AFLA*/ 	if (!undo_list) {
// /*AFLA*/ 		undo_list = kzalloc(sizeof(*undo_list), GFP_KERNEL);
// /*AFLA*/ 		if (undo_list == NULL)
// /*AFLA*/ 			return -ENOMEM;
// /*AFLA*/ 		spin_lock_init(&undo_list->lock);
// /*AFLA*/ 		atomic_set(&undo_list->refcnt, 1);
// /*AFLA*/ 		INIT_LIST_HEAD(&undo_list->list_proc);
// /*AFLA*/ 
// /*AFLA*/ 		current->sysvsem.undo_list = undo_list;
// /*AFLA*/ 	}
// /*AFLA*/ 	*undo_listp = undo_list;
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static struct sem_undo *__lookup_undo(struct sem_undo_list *ulp, int semid)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem_undo *un;
// /*AFLA*/ 
// /*AFLA*/ 	list_for_each_entry_rcu(un, &ulp->list_proc, list_proc) {
// /*AFLA*/ 		if (un->semid == semid)
// /*AFLA*/ 			return un;
// /*AFLA*/ 	}
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static struct sem_undo *lookup_undo(struct sem_undo_list *ulp, int semid)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem_undo *un;
// /*AFLA*/ 
// /*AFLA*/ 	assert_spin_locked(&ulp->lock);
// /*AFLA*/ 
// /*AFLA*/ 	un = __lookup_undo(ulp, semid);
// /*AFLA*/ 	if (un) {
// /*AFLA*/ 		list_del_rcu(&un->list_proc);
// /*AFLA*/ 		list_add_rcu(&un->list_proc, &ulp->list_proc);
// /*AFLA*/ 	}
// /*AFLA*/ 	return un;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * find_alloc_undo - lookup (and if not present create) undo array
// /*AFLA*/  * @ns: namespace
// /*AFLA*/  * @semid: semaphore array id
// /*AFLA*/  *
// /*AFLA*/  * The function looks up (and if not present creates) the undo structure.
// /*AFLA*/  * The size of the undo structure depends on the size of the semaphore
// /*AFLA*/  * array, thus the alloc path is not that straightforward.
// /*AFLA*/  * Lifetime-rules: sem_undo is rcu-protected, on success, the function
// /*AFLA*/  * performs a rcu_read_lock().
// /*AFLA*/  */
// /*AFLA*/ static struct sem_undo *find_alloc_undo(struct ipc_namespace *ns, int semid)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem_array *sma;
// /*AFLA*/ 	struct sem_undo_list *ulp;
// /*AFLA*/ 	struct sem_undo *un, *new;
// /*AFLA*/ 	int nsems, error;
// /*AFLA*/ 
// /*AFLA*/ 	error = get_undo_list(&ulp);
// /*AFLA*/ 	if (error)
// /*AFLA*/ 		return ERR_PTR(error);
// /*AFLA*/ 
// /*AFLA*/ 	rcu_read_lock();
// /*AFLA*/ 	spin_lock(&ulp->lock);
// /*AFLA*/ 	un = lookup_undo(ulp, semid);
// /*AFLA*/ 	spin_unlock(&ulp->lock);
// /*AFLA*/ 	if (likely(un != NULL))
// /*AFLA*/ 		goto out;
// /*AFLA*/ 
// /*AFLA*/ 	/* no undo structure around - allocate one. */
// /*AFLA*/ 	/* step 1: figure out the size of the semaphore array */
// /*AFLA*/ 	sma = sem_obtain_object_check(ns, semid);
// /*AFLA*/ 	if (IS_ERR(sma)) {
// /*AFLA*/ 		rcu_read_unlock();
// /*AFLA*/ 		return ERR_CAST(sma);
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	nsems = sma->sem_nsems;
// /*AFLA*/ 	if (!ipc_rcu_getref(sma)) {
// /*AFLA*/ 		rcu_read_unlock();
// /*AFLA*/ 		un = ERR_PTR(-EIDRM);
// /*AFLA*/ 		goto out;
// /*AFLA*/ 	}
// /*AFLA*/ 	rcu_read_unlock();
// /*AFLA*/ 
// /*AFLA*/ 	/* step 2: allocate new undo structure */
// /*AFLA*/ 	new = kzalloc(sizeof(struct sem_undo) + sizeof(short)*nsems, GFP_KERNEL);
// /*AFLA*/ 	if (!new) {
// /*AFLA*/ 		ipc_rcu_putref(sma, sem_rcu_free);
// /*AFLA*/ 		return ERR_PTR(-ENOMEM);
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	/* step 3: Acquire the lock on semaphore array */
// /*AFLA*/ 	rcu_read_lock();
// /*AFLA*/ 	sem_lock_and_putref(sma);
// /*AFLA*/ 	if (!ipc_valid_object(&sma->sem_perm)) {
// /*AFLA*/ 		sem_unlock(sma, -1);
// /*AFLA*/ 		rcu_read_unlock();
// /*AFLA*/ 		kfree(new);
// /*AFLA*/ 		un = ERR_PTR(-EIDRM);
// /*AFLA*/ 		goto out;
// /*AFLA*/ 	}
// /*AFLA*/ 	spin_lock(&ulp->lock);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * step 4: check for races: did someone else allocate the undo struct?
// /*AFLA*/ 	 */
// /*AFLA*/ 	un = lookup_undo(ulp, semid);
// /*AFLA*/ 	if (un) {
// /*AFLA*/ 		kfree(new);
// /*AFLA*/ 		goto success;
// /*AFLA*/ 	}
// /*AFLA*/ 	/* step 5: initialize & link new undo structure */
// /*AFLA*/ 	new->semadj = (short *) &new[1];
// /*AFLA*/ 	new->ulp = ulp;
// /*AFLA*/ 	new->semid = semid;
// /*AFLA*/ 	assert_spin_locked(&ulp->lock);
// /*AFLA*/ 	list_add_rcu(&new->list_proc, &ulp->list_proc);
// /*AFLA*/ 	ipc_assert_locked_object(&sma->sem_perm);
// /*AFLA*/ 	list_add(&new->list_id, &sma->list_id);
// /*AFLA*/ 	un = new;
// /*AFLA*/ 
// /*AFLA*/ success:
// /*AFLA*/ 	spin_unlock(&ulp->lock);
// /*AFLA*/ 	sem_unlock(sma, -1);
// /*AFLA*/ out:
// /*AFLA*/ 	return un;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * get_queue_result - retrieve the result code from sem_queue
// /*AFLA*/  * @q: Pointer to queue structure
// /*AFLA*/  *
// /*AFLA*/  * Retrieve the return code from the pending queue. If IN_WAKEUP is found in
// /*AFLA*/  * q->status, then we must loop until the value is replaced with the final
// /*AFLA*/  * value: This may happen if a task is woken up by an unrelated event (e.g.
// /*AFLA*/  * signal) and in parallel the task is woken up by another task because it got
// /*AFLA*/  * the requested semaphores.
// /*AFLA*/  *
// /*AFLA*/  * The function can be called with or without holding the semaphore spinlock.
// /*AFLA*/  */
// /*AFLA*/ static int get_queue_result(struct sem_queue *q)
// /*AFLA*/ {
// /*AFLA*/ 	int error;
// /*AFLA*/ 
// /*AFLA*/ 	error = q->status;
// /*AFLA*/ 	while (unlikely(error == IN_WAKEUP)) {
// /*AFLA*/ 		cpu_relax();
// /*AFLA*/ 		error = q->status;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	return error;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ SYSCALL_DEFINE4(semtimedop, int, semid, struct sembuf __user *, tsops,
// /*AFLA*/ 		unsigned, nsops, const struct timespec __user *, timeout)
// /*AFLA*/ {
// /*AFLA*/ 	int error = -EINVAL;
// /*AFLA*/ 	struct sem_array *sma;
// /*AFLA*/ 	struct sembuf fast_sops[SEMOPM_FAST];
// /*AFLA*/ 	struct sembuf *sops = fast_sops, *sop;
// /*AFLA*/ 	struct sem_undo *un;
// /*AFLA*/ 	int undos = 0, alter = 0, max, locknum;
// /*AFLA*/ 	struct sem_queue queue;
// /*AFLA*/ 	unsigned long jiffies_left = 0;
// /*AFLA*/ 	struct ipc_namespace *ns;
// /*AFLA*/ 	struct list_head tasks;
// /*AFLA*/ 
// /*AFLA*/ 	ns = current->nsproxy->ipc_ns;
// /*AFLA*/ 
// /*AFLA*/ 	if (nsops < 1 || semid < 0)
// /*AFLA*/ 		return -EINVAL;
// /*AFLA*/ 	if (nsops > ns->sc_semopm)
// /*AFLA*/ 		return -E2BIG;
// /*AFLA*/ 	if (nsops > SEMOPM_FAST) {
// /*AFLA*/ 		sops = kmalloc(sizeof(*sops)*nsops, GFP_KERNEL);
// /*AFLA*/ 		if (sops == NULL)
// /*AFLA*/ 			return -ENOMEM;
// /*AFLA*/ 	}
// /*AFLA*/ 	if (copy_from_user(sops, tsops, nsops * sizeof(*tsops))) {
// /*AFLA*/ 		error =  -EFAULT;
// /*AFLA*/ 		goto out_free;
// /*AFLA*/ 	}
// /*AFLA*/ 	if (timeout) {
// /*AFLA*/ 		struct timespec _timeout;
// /*AFLA*/ 		if (copy_from_user(&_timeout, timeout, sizeof(*timeout))) {
// /*AFLA*/ 			error = -EFAULT;
// /*AFLA*/ 			goto out_free;
// /*AFLA*/ 		}
// /*AFLA*/ 		if (_timeout.tv_sec < 0 || _timeout.tv_nsec < 0 ||
// /*AFLA*/ 			_timeout.tv_nsec >= 1000000000L) {
// /*AFLA*/ 			error = -EINVAL;
// /*AFLA*/ 			goto out_free;
// /*AFLA*/ 		}
// /*AFLA*/ 		jiffies_left = timespec_to_jiffies(&_timeout);
// /*AFLA*/ 	}
// /*AFLA*/ 	max = 0;
// /*AFLA*/ 	for (sop = sops; sop < sops + nsops; sop++) {
// /*AFLA*/ 		if (sop->sem_num >= max)
// /*AFLA*/ 			max = sop->sem_num;
// /*AFLA*/ 		if (sop->sem_flg & SEM_UNDO)
// /*AFLA*/ 			undos = 1;
// /*AFLA*/ 		if (sop->sem_op != 0)
// /*AFLA*/ 			alter = 1;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	INIT_LIST_HEAD(&tasks);
// /*AFLA*/ 
// /*AFLA*/ 	if (undos) {
// /*AFLA*/ 		/* On success, find_alloc_undo takes the rcu_read_lock */
// /*AFLA*/ 		un = find_alloc_undo(ns, semid);
// /*AFLA*/ 		if (IS_ERR(un)) {
// /*AFLA*/ 			error = PTR_ERR(un);
// /*AFLA*/ 			goto out_free;
// /*AFLA*/ 		}
// /*AFLA*/ 	} else {
// /*AFLA*/ 		un = NULL;
// /*AFLA*/ 		rcu_read_lock();
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	sma = sem_obtain_object_check(ns, semid);
// /*AFLA*/ 	if (IS_ERR(sma)) {
// /*AFLA*/ 		rcu_read_unlock();
// /*AFLA*/ 		error = PTR_ERR(sma);
// /*AFLA*/ 		goto out_free;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	error = -EFBIG;
// /*AFLA*/ 	if (max >= sma->sem_nsems)
// /*AFLA*/ 		goto out_rcu_wakeup;
// /*AFLA*/ 
// /*AFLA*/ 	error = -EACCES;
// /*AFLA*/ 	if (ipcperms(ns, &sma->sem_perm, alter ? S_IWUGO : S_IRUGO))
// /*AFLA*/ 		goto out_rcu_wakeup;
// /*AFLA*/ 
// /*AFLA*/ 	error = security_sem_semop(sma, sops, nsops, alter);
// /*AFLA*/ 	if (error)
// /*AFLA*/ 		goto out_rcu_wakeup;
// /*AFLA*/ 
// /*AFLA*/ 	error = -EIDRM;
// /*AFLA*/ 	locknum = sem_lock(sma, sops, nsops);
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * We eventually might perform the following check in a lockless
// /*AFLA*/ 	 * fashion, considering ipc_valid_object() locking constraints.
// /*AFLA*/ 	 * If nsops == 1 and there is no contention for sem_perm.lock, then
// /*AFLA*/ 	 * only a per-semaphore lock is held and it's OK to proceed with the
// /*AFLA*/ 	 * check below. More details on the fine grained locking scheme
// /*AFLA*/ 	 * entangled here and why it's RMID race safe on comments at sem_lock()
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (!ipc_valid_object(&sma->sem_perm))
// /*AFLA*/ 		goto out_unlock_free;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * semid identifiers are not unique - find_alloc_undo may have
// /*AFLA*/ 	 * allocated an undo structure, it was invalidated by an RMID
// /*AFLA*/ 	 * and now a new array with received the same id. Check and fail.
// /*AFLA*/ 	 * This case can be detected checking un->semid. The existence of
// /*AFLA*/ 	 * "un" itself is guaranteed by rcu.
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (un && un->semid == -1)
// /*AFLA*/ 		goto out_unlock_free;
// /*AFLA*/ 
// /*AFLA*/ 	queue.sops = sops;
// /*AFLA*/ 	queue.nsops = nsops;
// /*AFLA*/ 	queue.undo = un;
// /*AFLA*/ 	queue.pid = task_tgid_vnr(current);
// /*AFLA*/ 	queue.alter = alter;
// /*AFLA*/ 
// /*AFLA*/ 	error = perform_atomic_semop(sma, &queue);
// /*AFLA*/ 	if (error == 0) {
// /*AFLA*/ 		/* If the operation was successful, then do
// /*AFLA*/ 		 * the required updates.
// /*AFLA*/ 		 */
// /*AFLA*/ 		if (alter)
// /*AFLA*/ 			do_smart_update(sma, sops, nsops, 1, &tasks);
// /*AFLA*/ 		else
// /*AFLA*/ 			set_semotime(sma, sops);
// /*AFLA*/ 	}
// /*AFLA*/ 	if (error <= 0)
// /*AFLA*/ 		goto out_unlock_free;
// /*AFLA*/ 
// /*AFLA*/ 	/* We need to sleep on this operation, so we put the current
// /*AFLA*/ 	 * task into the pending queue and go to sleep.
// /*AFLA*/ 	 */
// /*AFLA*/ 
// /*AFLA*/ 	if (nsops == 1) {
// /*AFLA*/ 		struct sem *curr;
// /*AFLA*/ 		curr = &sma->sem_base[sops->sem_num];
// /*AFLA*/ 
// /*AFLA*/ 		if (alter) {
// /*AFLA*/ 			if (sma->complex_count) {
// /*AFLA*/ 				list_add_tail(&queue.list,
// /*AFLA*/ 						&sma->pending_alter);
// /*AFLA*/ 			} else {
// /*AFLA*/ 
// /*AFLA*/ 				list_add_tail(&queue.list,
// /*AFLA*/ 						&curr->pending_alter);
// /*AFLA*/ 			}
// /*AFLA*/ 		} else {
// /*AFLA*/ 			list_add_tail(&queue.list, &curr->pending_const);
// /*AFLA*/ 		}
// /*AFLA*/ 	} else {
// /*AFLA*/ 		if (!sma->complex_count)
// /*AFLA*/ 			merge_queues(sma);
// /*AFLA*/ 
// /*AFLA*/ 		if (alter)
// /*AFLA*/ 			list_add_tail(&queue.list, &sma->pending_alter);
// /*AFLA*/ 		else
// /*AFLA*/ 			list_add_tail(&queue.list, &sma->pending_const);
// /*AFLA*/ 
// /*AFLA*/ 		sma->complex_count++;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	queue.status = -EINTR;
// /*AFLA*/ 	queue.sleeper = current;
// /*AFLA*/ 
// /*AFLA*/ sleep_again:
// /*AFLA*/ 	__set_current_state(TASK_INTERRUPTIBLE);
// /*AFLA*/ 	sem_unlock(sma, locknum);
// /*AFLA*/ 	rcu_read_unlock();
// /*AFLA*/ 
// /*AFLA*/ 	if (timeout)
// /*AFLA*/ 		jiffies_left = schedule_timeout(jiffies_left);
// /*AFLA*/ 	else
// /*AFLA*/ 		schedule();
// /*AFLA*/ 
// /*AFLA*/ 	error = get_queue_result(&queue);
// /*AFLA*/ 
// /*AFLA*/ 	if (error != -EINTR) {
// /*AFLA*/ 		/* fast path: update_queue already obtained all requested
// /*AFLA*/ 		 * resources.
// /*AFLA*/ 		 * Perform a smp_mb(): User space could assume that semop()
// /*AFLA*/ 		 * is a memory barrier: Without the mb(), the cpu could
// /*AFLA*/ 		 * speculatively read in user space stale data that was
// /*AFLA*/ 		 * overwritten by the previous owner of the semaphore.
// /*AFLA*/ 		 */
// /*AFLA*/ 		smp_mb();
// /*AFLA*/ 
// /*AFLA*/ 		goto out_free;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	rcu_read_lock();
// /*AFLA*/ 	sma = sem_obtain_lock(ns, semid, sops, nsops, &locknum);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Wait until it's guaranteed that no wakeup_sem_queue_do() is ongoing.
// /*AFLA*/ 	 */
// /*AFLA*/ 	error = get_queue_result(&queue);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Array removed? If yes, leave without sem_unlock().
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (IS_ERR(sma)) {
// /*AFLA*/ 		rcu_read_unlock();
// /*AFLA*/ 		goto out_free;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If queue.status != -EINTR we are woken up by another process.
// /*AFLA*/ 	 * Leave without unlink_queue(), but with sem_unlock().
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (error != -EINTR)
// /*AFLA*/ 		goto out_unlock_free;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If an interrupt occurred we have to clean up the queue
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (timeout && jiffies_left == 0)
// /*AFLA*/ 		error = -EAGAIN;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If the wakeup was spurious, just retry
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (error == -EINTR && !signal_pending(current))
// /*AFLA*/ 		goto sleep_again;
// /*AFLA*/ 
// /*AFLA*/ 	unlink_queue(sma, &queue);
// /*AFLA*/ 
// /*AFLA*/ out_unlock_free:
// /*AFLA*/ 	sem_unlock(sma, locknum);
// /*AFLA*/ out_rcu_wakeup:
// /*AFLA*/ 	rcu_read_unlock();
// /*AFLA*/ 	wake_up_sem_queue_do(&tasks);
// /*AFLA*/ out_free:
// /*AFLA*/ 	if (sops != fast_sops)
// /*AFLA*/ 		kfree(sops);
// /*AFLA*/ 	return error;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ SYSCALL_DEFINE3(semop, int, semid, struct sembuf __user *, tsops,
// /*AFLA*/ 		unsigned, nsops)
// /*AFLA*/ {
// /*AFLA*/ 	return sys_semtimedop(semid, tsops, nsops, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* If CLONE_SYSVSEM is set, establish sharing of SEM_UNDO state between
// /*AFLA*/  * parent and child tasks.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ int copy_semundo(unsigned long clone_flags, struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem_undo_list *undo_list;
// /*AFLA*/ 	int error;
// /*AFLA*/ 
// /*AFLA*/ 	if (clone_flags & CLONE_SYSVSEM) {
// /*AFLA*/ 		error = get_undo_list(&undo_list);
// /*AFLA*/ 		if (error)
// /*AFLA*/ 			return error;
// /*AFLA*/ 		atomic_inc(&undo_list->refcnt);
// /*AFLA*/ 		tsk->sysvsem.undo_list = undo_list;
// /*AFLA*/ 	} else
// /*AFLA*/ 		tsk->sysvsem.undo_list = NULL;
// /*AFLA*/ 
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * add semadj values to semaphores, free undo structures.
// /*AFLA*/  * undo structures are not freed when semaphore arrays are destroyed
// /*AFLA*/  * so some of them may be out of date.
// /*AFLA*/  * IMPLEMENTATION NOTE: There is some confusion over whether the
// /*AFLA*/  * set of adjustments that needs to be done should be done in an atomic
// /*AFLA*/  * manner or not. That is, if we are attempting to decrement the semval
// /*AFLA*/  * should we queue up and wait until we can do so legally?
// /*AFLA*/  * The original implementation attempted to do this (queue and wait).
// /*AFLA*/  * The current implementation does not do so. The POSIX standard
// /*AFLA*/  * and SVID should be consulted to determine what behavior is mandated.
// /*AFLA*/  */
// /*AFLA*/ void exit_sem(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	struct sem_undo_list *ulp;
// /*AFLA*/ 
// /*AFLA*/ 	ulp = tsk->sysvsem.undo_list;
// /*AFLA*/ 	if (!ulp)
// /*AFLA*/ 		return;
// /*AFLA*/ 	tsk->sysvsem.undo_list = NULL;
// /*AFLA*/ 
// /*AFLA*/ 	if (!atomic_dec_and_test(&ulp->refcnt))
// /*AFLA*/ 		return;
// /*AFLA*/ 
// /*AFLA*/ 	for (;;) {
// /*AFLA*/ 		struct sem_array *sma;
// /*AFLA*/ 		struct sem_undo *un;
// /*AFLA*/ 		struct list_head tasks;
// /*AFLA*/ 		int semid, i;
// /*AFLA*/ 
// /*AFLA*/ 		cond_resched();
// /*AFLA*/ 
// /*AFLA*/ 		rcu_read_lock();
// /*AFLA*/ 		un = list_entry_rcu(ulp->list_proc.next,
// /*AFLA*/ 				    struct sem_undo, list_proc);
// /*AFLA*/ 		if (&un->list_proc == &ulp->list_proc) {
// /*AFLA*/ 			/*
// /*AFLA*/ 			 * We must wait for freeary() before freeing this ulp,
// /*AFLA*/ 			 * in case we raced with last sem_undo. There is a small
// /*AFLA*/ 			 * possibility where we exit while freeary() didn't
// /*AFLA*/ 			 * finish unlocking sem_undo_list.
// /*AFLA*/ 			 */
// /*AFLA*/ 			spin_unlock_wait(&ulp->lock);
// /*AFLA*/ 			rcu_read_unlock();
// /*AFLA*/ 			break;
// /*AFLA*/ 		}
// /*AFLA*/ 		spin_lock(&ulp->lock);
// /*AFLA*/ 		semid = un->semid;
// /*AFLA*/ 		spin_unlock(&ulp->lock);
// /*AFLA*/ 
// /*AFLA*/ 		/* exit_sem raced with IPC_RMID, nothing to do */
// /*AFLA*/ 		if (semid == -1) {
// /*AFLA*/ 			rcu_read_unlock();
// /*AFLA*/ 			continue;
// /*AFLA*/ 		}
// /*AFLA*/ 
// /*AFLA*/ 		sma = sem_obtain_object_check(tsk->nsproxy->ipc_ns, semid);
// /*AFLA*/ 		/* exit_sem raced with IPC_RMID, nothing to do */
// /*AFLA*/ 		if (IS_ERR(sma)) {
// /*AFLA*/ 			rcu_read_unlock();
// /*AFLA*/ 			continue;
// /*AFLA*/ 		}
// /*AFLA*/ 
// /*AFLA*/ 		sem_lock(sma, NULL, -1);
// /*AFLA*/ 		/* exit_sem raced with IPC_RMID, nothing to do */
// /*AFLA*/ 		if (!ipc_valid_object(&sma->sem_perm)) {
// /*AFLA*/ 			sem_unlock(sma, -1);
// /*AFLA*/ 			rcu_read_unlock();
// /*AFLA*/ 			continue;
// /*AFLA*/ 		}
// /*AFLA*/ 		un = __lookup_undo(ulp, semid);
// /*AFLA*/ 		if (un == NULL) {
// /*AFLA*/ 			/* exit_sem raced with IPC_RMID+semget() that created
// /*AFLA*/ 			 * exactly the same semid. Nothing to do.
// /*AFLA*/ 			 */
// /*AFLA*/ 			sem_unlock(sma, -1);
// /*AFLA*/ 			rcu_read_unlock();
// /*AFLA*/ 			continue;
// /*AFLA*/ 		}
// /*AFLA*/ 
// /*AFLA*/ 		/* remove un from the linked lists */
// /*AFLA*/ 		ipc_assert_locked_object(&sma->sem_perm);
// /*AFLA*/ 		list_del(&un->list_id);
// /*AFLA*/ 
// /*AFLA*/ 		/* we are the last process using this ulp, acquiring ulp->lock
// /*AFLA*/ 		 * isn't required. Besides that, we are also protected against
// /*AFLA*/ 		 * IPC_RMID as we hold sma->sem_perm lock now
// /*AFLA*/ 		 */
// /*AFLA*/ 		list_del_rcu(&un->list_proc);
// /*AFLA*/ 
// /*AFLA*/ 		/* perform adjustments registered in un */
// /*AFLA*/ 		for (i = 0; i < sma->sem_nsems; i++) {
// /*AFLA*/ 			struct sem *semaphore = &sma->sem_base[i];
// /*AFLA*/ 			if (un->semadj[i]) {
// /*AFLA*/ 				semaphore->semval += un->semadj[i];
// /*AFLA*/ 				/*
// /*AFLA*/ 				 * Range checks of the new semaphore value,
// /*AFLA*/ 				 * not defined by sus:
// /*AFLA*/ 				 * - Some unices ignore the undo entirely
// /*AFLA*/ 				 *   (e.g. HP UX 11i 11.22, Tru64 V5.1)
// /*AFLA*/ 				 * - some cap the value (e.g. FreeBSD caps
// /*AFLA*/ 				 *   at 0, but doesn't enforce SEMVMX)
// /*AFLA*/ 				 *
// /*AFLA*/ 				 * Linux caps the semaphore value, both at 0
// /*AFLA*/ 				 * and at SEMVMX.
// /*AFLA*/ 				 *
// /*AFLA*/ 				 *	Manfred <manfred@colorfullife.com>
// /*AFLA*/ 				 */
// /*AFLA*/ 				if (semaphore->semval < 0)
// /*AFLA*/ 					semaphore->semval = 0;
// /*AFLA*/ 				if (semaphore->semval > SEMVMX)
// /*AFLA*/ 					semaphore->semval = SEMVMX;
// /*AFLA*/ 				semaphore->sempid = task_tgid_vnr(current);
// /*AFLA*/ 			}
// /*AFLA*/ 		}
// /*AFLA*/ 		/* maybe some queued-up processes were waiting for this */
// /*AFLA*/ 		INIT_LIST_HEAD(&tasks);
// /*AFLA*/ 		do_smart_update(sma, NULL, 0, 1, &tasks);
// /*AFLA*/ 		sem_unlock(sma, -1);
// /*AFLA*/ 		rcu_read_unlock();
// /*AFLA*/ 		wake_up_sem_queue_do(&tasks);
// /*AFLA*/ 
// /*AFLA*/ 		kfree_rcu(un, rcu);
// /*AFLA*/ 	}
// /*AFLA*/ 	kfree(ulp);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PROC_FS
// /*AFLA*/ static int sysvipc_sem_proc_show(struct seq_file *s, void *it)
// /*AFLA*/ {
// /*AFLA*/ 	struct user_namespace *user_ns = seq_user_ns(s);
// /*AFLA*/ 	struct sem_array *sma = it;
// /*AFLA*/ 	time_t sem_otime;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The proc interface isn't aware of sem_lock(), it calls
// /*AFLA*/ 	 * ipc_lock_object() directly (in sysvipc_find_ipc).
// /*AFLA*/ 	 * In order to stay compatible with sem_lock(), we must
// /*AFLA*/ 	 * enter / leave complex_mode.
// /*AFLA*/ 	 */
// /*AFLA*/ 	complexmode_enter(sma);
// /*AFLA*/ 
// /*AFLA*/ 	sem_otime = get_semotime(sma);
// /*AFLA*/ 
// /*AFLA*/ 	seq_printf(s,
// /*AFLA*/ 		   "%10d %10d  %4o %10u %5u %5u %5u %5u %10lu %10lu\n",
// /*AFLA*/ 		   sma->sem_perm.key,
// /*AFLA*/ 		   sma->sem_perm.id,
// /*AFLA*/ 		   sma->sem_perm.mode,
// /*AFLA*/ 		   sma->sem_nsems,
// /*AFLA*/ 		   from_kuid_munged(user_ns, sma->sem_perm.uid),
// /*AFLA*/ 		   from_kgid_munged(user_ns, sma->sem_perm.gid),
// /*AFLA*/ 		   from_kuid_munged(user_ns, sma->sem_perm.cuid),
// /*AFLA*/ 		   from_kgid_munged(user_ns, sma->sem_perm.cgid),
// /*AFLA*/ 		   sem_otime,
// /*AFLA*/ 		   sma->sem_ctime);
// /*AFLA*/ 
// /*AFLA*/ 	complexmode_tryleave(sma);
// /*AFLA*/ 
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
