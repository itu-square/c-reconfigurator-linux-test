// /*AFLA*/ #ifndef __IPC_NAMESPACE_H__
// /*AFLA*/ #define __IPC_NAMESPACE_H__
// /*AFLA*/ 
// /*AFLA*/ #include <linux/err.h>
// /*AFLA*/ #include <linux/idr.h>
// /*AFLA*/ #include <linux/rwsem.h>
// /*AFLA*/ #include <linux/notifier.h>
// /*AFLA*/ #include <linux/nsproxy.h>
// /*AFLA*/ #include <linux/ns_common.h>
// /*AFLA*/ 
// /*AFLA*/ struct user_namespace;
// /*AFLA*/ 
// /*AFLA*/ struct ipc_ids {
// /*AFLA*/ 	int in_use;
// /*AFLA*/ 	unsigned short seq;
// /*AFLA*/ 	struct rw_semaphore rwsem;
// /*AFLA*/ 	struct idr ipcs_idr;
// /*AFLA*/ 	int next_id;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct ipc_namespace {
// /*AFLA*/ 	atomic_t	count;
// /*AFLA*/ 	struct ipc_ids	ids[3];
// /*AFLA*/ 
// /*AFLA*/ 	int		sem_ctls[4];
// /*AFLA*/ 	int		used_sems;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned int	msg_ctlmax;
// /*AFLA*/ 	unsigned int	msg_ctlmnb;
// /*AFLA*/ 	unsigned int	msg_ctlmni;
// /*AFLA*/ 	atomic_t	msg_bytes;
// /*AFLA*/ 	atomic_t	msg_hdrs;
// /*AFLA*/ 
// /*AFLA*/ 	size_t		shm_ctlmax;
// /*AFLA*/ 	size_t		shm_ctlall;
// /*AFLA*/ 	unsigned long	shm_tot;
// /*AFLA*/ 	int		shm_ctlmni;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Defines whether IPC_RMID is forced for _all_ shm segments regardless
// /*AFLA*/ 	 * of shmctl()
// /*AFLA*/ 	 */
// /*AFLA*/ 	int		shm_rmid_forced;
// /*AFLA*/ 
// /*AFLA*/ 	struct notifier_block ipcns_nb;
// /*AFLA*/ 
// /*AFLA*/ 	/* The kern_mount of the mqueuefs sb.  We take a ref on it */
// /*AFLA*/ 	struct vfsmount	*mq_mnt;
// /*AFLA*/ 
// /*AFLA*/ 	/* # queues in this ns, protected by mq_lock */
// /*AFLA*/ 	unsigned int    mq_queues_count;
// /*AFLA*/ 
// /*AFLA*/ 	/* next fields are set through sysctl */
// /*AFLA*/ 	unsigned int    mq_queues_max;   /* initialized to DFLT_QUEUESMAX */
// /*AFLA*/ 	unsigned int    mq_msg_max;      /* initialized to DFLT_MSGMAX */
// /*AFLA*/ 	unsigned int    mq_msgsize_max;  /* initialized to DFLT_MSGSIZEMAX */
// /*AFLA*/ 	unsigned int    mq_msg_default;
// /*AFLA*/ 	unsigned int    mq_msgsize_default;
// /*AFLA*/ 
// /*AFLA*/ 	/* user_ns which owns the ipc ns */
// /*AFLA*/ 	struct user_namespace *user_ns;
// /*AFLA*/ 	struct ucounts *ucounts;
// /*AFLA*/ 
// /*AFLA*/ 	struct ns_common ns;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct ipc_namespace init_ipc_ns;
// /*AFLA*/ extern spinlock_t mq_lock;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SYSVIPC
// /*AFLA*/ extern void shm_destroy_orphaned(struct ipc_namespace *ns);
// /*AFLA*/ #else /* CONFIG_SYSVIPC */
// /*AFLA*/ static inline void shm_destroy_orphaned(struct ipc_namespace *ns) {}
// /*AFLA*/ #endif /* CONFIG_SYSVIPC */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_POSIX_MQUEUE
// /*AFLA*/ extern int mq_init_ns(struct ipc_namespace *ns);
// /*AFLA*/ /*
// /*AFLA*/  * POSIX Message Queue default values:
// /*AFLA*/  *
// /*AFLA*/  * MIN_*: Lowest value an admin can set the maximum unprivileged limit to
// /*AFLA*/  * DFLT_*MAX: Default values for the maximum unprivileged limits
// /*AFLA*/  * DFLT_{MSG,MSGSIZE}: Default values used when the user doesn't supply
// /*AFLA*/  *   an attribute to the open call and the queue must be created
// /*AFLA*/  * HARD_*: Highest value the maximums can be set to.  These are enforced
// /*AFLA*/  *   on CAP_SYS_RESOURCE apps as well making them inviolate (so make them
// /*AFLA*/  *   suitably high)
// /*AFLA*/  *
// /*AFLA*/  * POSIX Requirements:
// /*AFLA*/  *   Per app minimum openable message queues - 8.  This does not map well
// /*AFLA*/  *     to the fact that we limit the number of queues on a per namespace
// /*AFLA*/  *     basis instead of a per app basis.  So, make the default high enough
// /*AFLA*/  *     that no given app should have a hard time opening 8 queues.
// /*AFLA*/  *   Minimum maximum for HARD_MSGMAX - 32767.  I bumped this to 65536.
// /*AFLA*/  *   Minimum maximum for HARD_MSGSIZEMAX - POSIX is silent on this.  However,
// /*AFLA*/  *     we have run into a situation where running applications in the wild
// /*AFLA*/  *     require this to be at least 5MB, and preferably 10MB, so I set the
// /*AFLA*/  *     value to 16MB in hopes that this user is the worst of the bunch and
// /*AFLA*/  *     the new maximum will handle anyone else.  I may have to revisit this
// /*AFLA*/  *     in the future.
// /*AFLA*/  */
// /*AFLA*/ #define DFLT_QUEUESMAX		      256
// /*AFLA*/ #define MIN_MSGMAX			1
// /*AFLA*/ #define DFLT_MSG		       10U
// /*AFLA*/ #define DFLT_MSGMAX		       10
// /*AFLA*/ #define HARD_MSGMAX		    65536
// /*AFLA*/ #define MIN_MSGSIZEMAX		      128
// /*AFLA*/ #define DFLT_MSGSIZE		     8192U
// /*AFLA*/ #define DFLT_MSGSIZEMAX		     8192
// /*AFLA*/ #define HARD_MSGSIZEMAX	    (16*1024*1024)
// /*AFLA*/ #else
// /*AFLA*/ static inline int mq_init_ns(struct ipc_namespace *ns) { return 0; }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_IPC_NS)
// /*AFLA*/ extern struct ipc_namespace *copy_ipcs(unsigned long flags,
// /*AFLA*/ 	struct user_namespace *user_ns, struct ipc_namespace *ns);
// /*AFLA*/ 
// /*AFLA*/ static inline struct ipc_namespace *get_ipc_ns(struct ipc_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	if (ns)
// /*AFLA*/ 		atomic_inc(&ns->count);
// /*AFLA*/ 	return ns;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void put_ipc_ns(struct ipc_namespace *ns);
// /*AFLA*/ #else
// /*AFLA*/ static inline struct ipc_namespace *copy_ipcs(unsigned long flags,
// /*AFLA*/ 	struct user_namespace *user_ns, struct ipc_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	if (flags & CLONE_NEWIPC)
// /*AFLA*/ 		return ERR_PTR(-EINVAL);
// /*AFLA*/ 
// /*AFLA*/ 	return ns;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct ipc_namespace *get_ipc_ns(struct ipc_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ 	return ns;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void put_ipc_ns(struct ipc_namespace *ns)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_POSIX_MQUEUE_SYSCTL
// /*AFLA*/ 
// /*AFLA*/ struct ctl_table_header;
// /*AFLA*/ extern struct ctl_table_header *mq_register_sysctl_table(void);
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_POSIX_MQUEUE_SYSCTL */
// /*AFLA*/ 
// /*AFLA*/ static inline struct ctl_table_header *mq_register_sysctl_table(void)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_POSIX_MQUEUE_SYSCTL */
// /*AFLA*/ #endif
