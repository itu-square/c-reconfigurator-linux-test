// /*AFLA*/ /*
// /*AFLA*/  * linux/ipc/util.h
// /*AFLA*/  * Copyright (C) 1999 Christoph Rohland
// /*AFLA*/  *
// /*AFLA*/  * ipc helper functions (c) 1999 Manfred Spraul <manfred@colorfullife.com>
// /*AFLA*/  * namespaces support.      2006 OpenVZ, SWsoft Inc.
// /*AFLA*/  *                               Pavel Emelianov <xemul@openvz.org>
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _IPC_UTIL_H
// /*AFLA*/ #define _IPC_UTIL_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/unistd.h>
// /*AFLA*/ #include <linux/err.h>
// /*AFLA*/ 
// /*AFLA*/ #define SEQ_MULTIPLIER	(IPCMNI)
// /*AFLA*/ 
// /*AFLA*/ void sem_init(void);
// /*AFLA*/ void msg_init(void);
// /*AFLA*/ void shm_init(void);
// /*AFLA*/ 
// /*AFLA*/ struct ipc_namespace;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_POSIX_MQUEUE
// /*AFLA*/ extern void mq_clear_sbinfo(struct ipc_namespace *ns);
// /*AFLA*/ extern void mq_put_mnt(struct ipc_namespace *ns);
// /*AFLA*/ #else
// /*AFLA*/ static inline void mq_clear_sbinfo(struct ipc_namespace *ns) { }
// /*AFLA*/ static inline void mq_put_mnt(struct ipc_namespace *ns) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SYSVIPC
// /*AFLA*/ void sem_init_ns(struct ipc_namespace *ns);
// /*AFLA*/ void msg_init_ns(struct ipc_namespace *ns);
// /*AFLA*/ void shm_init_ns(struct ipc_namespace *ns);
// /*AFLA*/ 
// /*AFLA*/ void sem_exit_ns(struct ipc_namespace *ns);
// /*AFLA*/ void msg_exit_ns(struct ipc_namespace *ns);
// /*AFLA*/ void shm_exit_ns(struct ipc_namespace *ns);
// /*AFLA*/ #else
// /*AFLA*/ static inline void sem_init_ns(struct ipc_namespace *ns) { }
// /*AFLA*/ static inline void msg_init_ns(struct ipc_namespace *ns) { }
// /*AFLA*/ static inline void shm_init_ns(struct ipc_namespace *ns) { }
// /*AFLA*/ 
// /*AFLA*/ static inline void sem_exit_ns(struct ipc_namespace *ns) { }
// /*AFLA*/ static inline void msg_exit_ns(struct ipc_namespace *ns) { }
// /*AFLA*/ static inline void shm_exit_ns(struct ipc_namespace *ns) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct ipc_rcu {
// /*AFLA*/ 	struct rcu_head rcu;
// /*AFLA*/ 	atomic_t refcount;
// /*AFLA*/ } ____cacheline_aligned_in_smp;
// /*AFLA*/ 
// /*AFLA*/ #define ipc_rcu_to_struct(p)  ((void *)(p+1))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Structure that holds the parameters needed by the ipc operations
// /*AFLA*/  * (see after)
// /*AFLA*/  */
// /*AFLA*/ struct ipc_params {
// /*AFLA*/ 	key_t key;
// /*AFLA*/ 	int flg;
// /*AFLA*/ 	union {
// /*AFLA*/ 		size_t size;	/* for shared memories */
// /*AFLA*/ 		int nsems;	/* for semaphores */
// /*AFLA*/ 	} u;			/* holds the getnew() specific param */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Structure that holds some ipc operations. This structure is used to unify
// /*AFLA*/  * the calls to sys_msgget(), sys_semget(), sys_shmget()
// /*AFLA*/  *      . routine to call to create a new ipc object. Can be one of newque,
// /*AFLA*/  *        newary, newseg
// /*AFLA*/  *      . routine to call to check permissions for a new ipc object.
// /*AFLA*/  *        Can be one of security_msg_associate, security_sem_associate,
// /*AFLA*/  *        security_shm_associate
// /*AFLA*/  *      . routine to call for an extra check if needed
// /*AFLA*/  */
// /*AFLA*/ struct ipc_ops {
// /*AFLA*/ 	int (*getnew)(struct ipc_namespace *, struct ipc_params *);
// /*AFLA*/ 	int (*associate)(struct kern_ipc_perm *, int);
// /*AFLA*/ 	int (*more_checks)(struct kern_ipc_perm *, struct ipc_params *);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct seq_file;
// /*AFLA*/ struct ipc_ids;
// /*AFLA*/ 
// /*AFLA*/ void ipc_init_ids(struct ipc_ids *);
// /*AFLA*/ #ifdef CONFIG_PROC_FS
// /*AFLA*/ void __init ipc_init_proc_interface(const char *path, const char *header,
// /*AFLA*/ 		int ids, int (*show)(struct seq_file *, void *));
// /*AFLA*/ #else
// /*AFLA*/ #define ipc_init_proc_interface(path, header, ids, show) do {} while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define IPC_SEM_IDS	0
// /*AFLA*/ #define IPC_MSG_IDS	1
// /*AFLA*/ #define IPC_SHM_IDS	2
// /*AFLA*/ 
// /*AFLA*/ #define ipcid_to_idx(id) ((id) % SEQ_MULTIPLIER)
// /*AFLA*/ #define ipcid_to_seqx(id) ((id) / SEQ_MULTIPLIER)
// /*AFLA*/ #define IPCID_SEQ_MAX min_t(int, INT_MAX/SEQ_MULTIPLIER, USHRT_MAX)
// /*AFLA*/ 
// /*AFLA*/ /* must be called with ids->rwsem acquired for writing */
// /*AFLA*/ int ipc_addid(struct ipc_ids *, struct kern_ipc_perm *, int);
// /*AFLA*/ 
// /*AFLA*/ /* must be called with ids->rwsem acquired for reading */
// /*AFLA*/ int ipc_get_maxid(struct ipc_ids *);
// /*AFLA*/ 
// /*AFLA*/ /* must be called with both locks acquired. */
// /*AFLA*/ void ipc_rmid(struct ipc_ids *, struct kern_ipc_perm *);
// /*AFLA*/ 
// /*AFLA*/ /* must be called with ipcp locked */
// /*AFLA*/ int ipcperms(struct ipc_namespace *ns, struct kern_ipc_perm *ipcp, short flg);
// /*AFLA*/ 
// /*AFLA*/ /* for rare, potentially huge allocations.
// /*AFLA*/  * both function can sleep
// /*AFLA*/  */
// /*AFLA*/ void *ipc_alloc(int size);
// /*AFLA*/ void ipc_free(void *ptr);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * For allocation that need to be freed by RCU.
// /*AFLA*/  * Objects are reference counted, they start with reference count 1.
// /*AFLA*/  * getref increases the refcount, the putref call that reduces the recount
// /*AFLA*/  * to 0 schedules the rcu destruction. Caller must guarantee locking.
// /*AFLA*/  */
// /*AFLA*/ void *ipc_rcu_alloc(int size);
// /*AFLA*/ int ipc_rcu_getref(void *ptr);
// /*AFLA*/ void ipc_rcu_putref(void *ptr, void (*func)(struct rcu_head *head));
// /*AFLA*/ void ipc_rcu_free(struct rcu_head *head);
// /*AFLA*/ 
// /*AFLA*/ struct kern_ipc_perm *ipc_lock(struct ipc_ids *, int);
// /*AFLA*/ struct kern_ipc_perm *ipc_obtain_object_idr(struct ipc_ids *ids, int id);
// /*AFLA*/ 
// /*AFLA*/ void kernel_to_ipc64_perm(struct kern_ipc_perm *in, struct ipc64_perm *out);
// /*AFLA*/ void ipc64_perm_to_ipc_perm(struct ipc64_perm *in, struct ipc_perm *out);
// /*AFLA*/ int ipc_update_perm(struct ipc64_perm *in, struct kern_ipc_perm *out);
// /*AFLA*/ struct kern_ipc_perm *ipcctl_pre_down_nolock(struct ipc_namespace *ns,
// /*AFLA*/ 					     struct ipc_ids *ids, int id, int cmd,
// /*AFLA*/ 					     struct ipc64_perm *perm, int extra_perm);
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_ARCH_WANT_IPC_PARSE_VERSION
// /*AFLA*/ /* On IA-64, we always use the "64-bit version" of the IPC structures.  */
// /*AFLA*/ # define ipc_parse_version(cmd)	IPC_64
// /*AFLA*/ #else
// /*AFLA*/ int ipc_parse_version(int *cmd);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void free_msg(struct msg_msg *msg);
// /*AFLA*/ extern struct msg_msg *load_msg(const void __user *src, size_t len);
// /*AFLA*/ extern struct msg_msg *copy_msg(struct msg_msg *src, struct msg_msg *dst);
// /*AFLA*/ extern int store_msg(void __user *dest, struct msg_msg *msg, size_t len);
// /*AFLA*/ 
// /*AFLA*/ extern void recompute_msgmni(struct ipc_namespace *);
// /*AFLA*/ 
// /*AFLA*/ static inline int ipc_buildid(int id, int seq)
// /*AFLA*/ {
// /*AFLA*/ 	return SEQ_MULTIPLIER * seq + id;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int ipc_checkid(struct kern_ipc_perm *ipcp, int uid)
// /*AFLA*/ {
// /*AFLA*/ 	return uid / SEQ_MULTIPLIER != ipcp->seq;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ipc_lock_object(struct kern_ipc_perm *perm)
// /*AFLA*/ {
// /*AFLA*/ 	spin_lock(&perm->lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ipc_unlock_object(struct kern_ipc_perm *perm)
// /*AFLA*/ {
// /*AFLA*/ 	spin_unlock(&perm->lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ipc_assert_locked_object(struct kern_ipc_perm *perm)
// /*AFLA*/ {
// /*AFLA*/ 	assert_spin_locked(&perm->lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ipc_unlock(struct kern_ipc_perm *perm)
// /*AFLA*/ {
// /*AFLA*/ 	ipc_unlock_object(perm);
// /*AFLA*/ 	rcu_read_unlock();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * ipc_valid_object() - helper to sort out IPC_RMID races for codepaths
// /*AFLA*/  * where the respective ipc_ids.rwsem is not being held down.
// /*AFLA*/  * Checks whether the ipc object is still around or if it's gone already, as
// /*AFLA*/  * ipc_rmid() may have already freed the ID while the ipc lock was spinning.
// /*AFLA*/  * Needs to be called with kern_ipc_perm.lock held -- exception made for one
// /*AFLA*/  * checkpoint case at sys_semtimedop() as noted in code commentary.
// /*AFLA*/  */
// /*AFLA*/ static inline bool ipc_valid_object(struct kern_ipc_perm *perm)
// /*AFLA*/ {
// /*AFLA*/ 	return !perm->deleted;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct kern_ipc_perm *ipc_obtain_object_check(struct ipc_ids *ids, int id);
// /*AFLA*/ int ipcget(struct ipc_namespace *ns, struct ipc_ids *ids,
// /*AFLA*/ 			const struct ipc_ops *ops, struct ipc_params *params);
// /*AFLA*/ void free_ipcs(struct ipc_namespace *ns, struct ipc_ids *ids,
// /*AFLA*/ 		void (*free)(struct ipc_namespace *, struct kern_ipc_perm *));
// /*AFLA*/ #endif
