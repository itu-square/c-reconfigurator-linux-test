// /*AFLA*/ /*
// /*AFLA*/  * Linux Security plug
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2001 WireX Communications, Inc <chris@wirex.com>
// /*AFLA*/  * Copyright (C) 2001 Greg Kroah-Hartman <greg@kroah.com>
// /*AFLA*/  * Copyright (C) 2001 Networks Associates Technology, Inc <ssmalley@nai.com>
// /*AFLA*/  * Copyright (C) 2001 James Morris <jmorris@intercode.com.au>
// /*AFLA*/  * Copyright (C) 2001 Silicon Graphics, Inc. (Trust Technology Group)
// /*AFLA*/  *
// /*AFLA*/  *	This program is free software; you can redistribute it and/or modify
// /*AFLA*/  *	it under the terms of the GNU General Public License as published by
// /*AFLA*/  *	the Free Software Foundation; either version 2 of the License, or
// /*AFLA*/  *	(at your option) any later version.
// /*AFLA*/  *
// /*AFLA*/  *	Due to this file being licensed under the GPL there is controversy over
// /*AFLA*/  *	whether this permits you to write a module that #includes this file
// /*AFLA*/  *	without placing your module under the GPL.  Please consult a lawyer for
// /*AFLA*/  *	advice before doing this.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __LINUX_SECURITY_H
// /*AFLA*/ #define __LINUX_SECURITY_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/key.h>
// /*AFLA*/ #include <linux/capability.h>
// /*AFLA*/ #include <linux/fs.h>
// /*AFLA*/ #include <linux/slab.h>
// /*AFLA*/ #include <linux/err.h>
// /*AFLA*/ #include <linux/string.h>
// /*AFLA*/ #include <linux/mm.h>
// /*AFLA*/ #include <linux/fs.h>
// /*AFLA*/ 
// /*AFLA*/ struct linux_binprm;
// /*AFLA*/ struct cred;
// /*AFLA*/ struct rlimit;
// /*AFLA*/ struct siginfo;
// /*AFLA*/ struct sem_array;
// /*AFLA*/ struct sembuf;
// /*AFLA*/ struct kern_ipc_perm;
// /*AFLA*/ struct audit_context;
// /*AFLA*/ struct super_block;
// /*AFLA*/ struct inode;
// /*AFLA*/ struct dentry;
// /*AFLA*/ struct file;
// /*AFLA*/ struct vfsmount;
// /*AFLA*/ struct path;
// /*AFLA*/ struct qstr;
// /*AFLA*/ struct iattr;
// /*AFLA*/ struct fown_struct;
// /*AFLA*/ struct file_operations;
// /*AFLA*/ struct shmid_kernel;
// /*AFLA*/ struct msg_msg;
// /*AFLA*/ struct msg_queue;
// /*AFLA*/ struct xattr;
// /*AFLA*/ struct xfrm_sec_ctx;
// /*AFLA*/ struct mm_struct;
// /*AFLA*/ 
// /*AFLA*/ /* If capable should audit the security request */
// /*AFLA*/ #define SECURITY_CAP_NOAUDIT 0
// /*AFLA*/ #define SECURITY_CAP_AUDIT 1
// /*AFLA*/ 
// /*AFLA*/ /* LSM Agnostic defines for sb_set_mnt_opts */
// /*AFLA*/ #define SECURITY_LSM_NATIVE_LABELS	1
// /*AFLA*/ 
// /*AFLA*/ struct ctl_table;
// /*AFLA*/ struct audit_krule;
// /*AFLA*/ struct user_namespace;
// /*AFLA*/ struct timezone;
// /*AFLA*/ 
// /*AFLA*/ /* These functions are in security/commoncap.c */
// /*AFLA*/ extern int cap_capable(const struct cred *cred, struct user_namespace *ns,
// /*AFLA*/ 		       int cap, int audit);
// /*AFLA*/ extern int cap_settime(const struct timespec64 *ts, const struct timezone *tz);
// /*AFLA*/ extern int cap_ptrace_access_check(struct task_struct *child, unsigned int mode);
// /*AFLA*/ extern int cap_ptrace_traceme(struct task_struct *parent);
// /*AFLA*/ extern int cap_capget(struct task_struct *target, kernel_cap_t *effective, kernel_cap_t *inheritable, kernel_cap_t *permitted);
// /*AFLA*/ extern int cap_capset(struct cred *new, const struct cred *old,
// /*AFLA*/ 		      const kernel_cap_t *effective,
// /*AFLA*/ 		      const kernel_cap_t *inheritable,
// /*AFLA*/ 		      const kernel_cap_t *permitted);
// /*AFLA*/ extern int cap_bprm_set_creds(struct linux_binprm *bprm);
// /*AFLA*/ extern int cap_bprm_secureexec(struct linux_binprm *bprm);
// /*AFLA*/ extern int cap_inode_setxattr(struct dentry *dentry, const char *name,
// /*AFLA*/ 			      const void *value, size_t size, int flags);
// /*AFLA*/ extern int cap_inode_removexattr(struct dentry *dentry, const char *name);
// /*AFLA*/ extern int cap_inode_need_killpriv(struct dentry *dentry);
// /*AFLA*/ extern int cap_inode_killpriv(struct dentry *dentry);
// /*AFLA*/ extern int cap_mmap_addr(unsigned long addr);
// /*AFLA*/ extern int cap_mmap_file(struct file *file, unsigned long reqprot,
// /*AFLA*/ 			 unsigned long prot, unsigned long flags);
// /*AFLA*/ extern int cap_task_fix_setuid(struct cred *new, const struct cred *old, int flags);
// /*AFLA*/ extern int cap_task_prctl(int option, unsigned long arg2, unsigned long arg3,
// /*AFLA*/ 			  unsigned long arg4, unsigned long arg5);
// /*AFLA*/ extern int cap_task_setscheduler(struct task_struct *p);
// /*AFLA*/ extern int cap_task_setioprio(struct task_struct *p, int ioprio);
// /*AFLA*/ extern int cap_task_setnice(struct task_struct *p, int nice);
// /*AFLA*/ extern int cap_vm_enough_memory(struct mm_struct *mm, long pages);
// /*AFLA*/ 
// /*AFLA*/ struct msghdr;
// /*AFLA*/ struct sk_buff;
// /*AFLA*/ struct sock;
// /*AFLA*/ struct sockaddr;
// /*AFLA*/ struct socket;
// /*AFLA*/ struct flowi;
// /*AFLA*/ struct dst_entry;
// /*AFLA*/ struct xfrm_selector;
// /*AFLA*/ struct xfrm_policy;
// /*AFLA*/ struct xfrm_state;
// /*AFLA*/ struct xfrm_user_sec_ctx;
// /*AFLA*/ struct seq_file;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ extern unsigned long mmap_min_addr;
// /*AFLA*/ extern unsigned long dac_mmap_min_addr;
// /*AFLA*/ #else
// /*AFLA*/ #define mmap_min_addr		0UL
// /*AFLA*/ #define dac_mmap_min_addr	0UL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Values used in the task_security_ops calls
// /*AFLA*/  */
// /*AFLA*/ /* setuid or setgid, id0 == uid or gid */
// /*AFLA*/ #define LSM_SETID_ID	1
// /*AFLA*/ 
// /*AFLA*/ /* setreuid or setregid, id0 == real, id1 == eff */
// /*AFLA*/ #define LSM_SETID_RE	2
// /*AFLA*/ 
// /*AFLA*/ /* setresuid or setresgid, id0 == real, id1 == eff, uid2 == saved */
// /*AFLA*/ #define LSM_SETID_RES	4
// /*AFLA*/ 
// /*AFLA*/ /* setfsuid or setfsgid, id0 == fsuid or fsgid */
// /*AFLA*/ #define LSM_SETID_FS	8
// /*AFLA*/ 
// /*AFLA*/ /* forward declares to avoid warnings */
// /*AFLA*/ struct sched_param;
// /*AFLA*/ struct request_sock;
// /*AFLA*/ 
// /*AFLA*/ /* bprm->unsafe reasons */
// /*AFLA*/ #define LSM_UNSAFE_SHARE	1
// /*AFLA*/ #define LSM_UNSAFE_PTRACE	2
// /*AFLA*/ #define LSM_UNSAFE_PTRACE_CAP	4
// /*AFLA*/ #define LSM_UNSAFE_NO_NEW_PRIVS	8
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ extern int mmap_min_addr_handler(struct ctl_table *table, int write,
// /*AFLA*/ 				 void __user *buffer, size_t *lenp, loff_t *ppos);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* security_inode_init_security callback function to write xattrs */
// /*AFLA*/ typedef int (*initxattrs) (struct inode *inode,
// /*AFLA*/ 			   const struct xattr *xattr_array, void *fs_data);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SECURITY
// /*AFLA*/ 
// /*AFLA*/ struct security_mnt_opts {
// /*AFLA*/ 	char **mnt_opts;
// /*AFLA*/ 	int *mnt_opts_flags;
// /*AFLA*/ 	int num_mnt_opts;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline void security_init_mnt_opts(struct security_mnt_opts *opts)
// /*AFLA*/ {
// /*AFLA*/ 	opts->mnt_opts = NULL;
// /*AFLA*/ 	opts->mnt_opts_flags = NULL;
// /*AFLA*/ 	opts->num_mnt_opts = 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_free_mnt_opts(struct security_mnt_opts *opts)
// /*AFLA*/ {
// /*AFLA*/ 	int i;
// /*AFLA*/ 	if (opts->mnt_opts)
// /*AFLA*/ 		for (i = 0; i < opts->num_mnt_opts; i++)
// /*AFLA*/ 			kfree(opts->mnt_opts[i]);
// /*AFLA*/ 	kfree(opts->mnt_opts);
// /*AFLA*/ 	opts->mnt_opts = NULL;
// /*AFLA*/ 	kfree(opts->mnt_opts_flags);
// /*AFLA*/ 	opts->mnt_opts_flags = NULL;
// /*AFLA*/ 	opts->num_mnt_opts = 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* prototypes */
// /*AFLA*/ extern int security_init(void);
// /*AFLA*/ 
// /*AFLA*/ /* Security operations */
// /*AFLA*/ int security_binder_set_context_mgr(struct task_struct *mgr);
// /*AFLA*/ int security_binder_transaction(struct task_struct *from,
// /*AFLA*/ 				struct task_struct *to);
// /*AFLA*/ int security_binder_transfer_binder(struct task_struct *from,
// /*AFLA*/ 				    struct task_struct *to);
// /*AFLA*/ int security_binder_transfer_file(struct task_struct *from,
// /*AFLA*/ 				  struct task_struct *to, struct file *file);
// /*AFLA*/ int security_ptrace_access_check(struct task_struct *child, unsigned int mode);
// /*AFLA*/ int security_ptrace_traceme(struct task_struct *parent);
// /*AFLA*/ int security_capget(struct task_struct *target,
// /*AFLA*/ 		    kernel_cap_t *effective,
// /*AFLA*/ 		    kernel_cap_t *inheritable,
// /*AFLA*/ 		    kernel_cap_t *permitted);
// /*AFLA*/ int security_capset(struct cred *new, const struct cred *old,
// /*AFLA*/ 		    const kernel_cap_t *effective,
// /*AFLA*/ 		    const kernel_cap_t *inheritable,
// /*AFLA*/ 		    const kernel_cap_t *permitted);
// /*AFLA*/ int security_capable(const struct cred *cred, struct user_namespace *ns,
// /*AFLA*/ 			int cap);
// /*AFLA*/ int security_capable_noaudit(const struct cred *cred, struct user_namespace *ns,
// /*AFLA*/ 			     int cap);
// /*AFLA*/ int security_quotactl(int cmds, int type, int id, struct super_block *sb);
// /*AFLA*/ int security_quota_on(struct dentry *dentry);
// /*AFLA*/ int security_syslog(int type);
// /*AFLA*/ int security_settime64(const struct timespec64 *ts, const struct timezone *tz);
// /*AFLA*/ static inline int security_settime(const struct timespec *ts, const struct timezone *tz)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec64 ts64 = timespec_to_timespec64(*ts);
// /*AFLA*/ 
// /*AFLA*/ 	return security_settime64(&ts64, tz);
// /*AFLA*/ }
// /*AFLA*/ int security_vm_enough_memory_mm(struct mm_struct *mm, long pages);
// /*AFLA*/ int security_bprm_set_creds(struct linux_binprm *bprm);
// /*AFLA*/ int security_bprm_check(struct linux_binprm *bprm);
// /*AFLA*/ void security_bprm_committing_creds(struct linux_binprm *bprm);
// /*AFLA*/ void security_bprm_committed_creds(struct linux_binprm *bprm);
// /*AFLA*/ int security_bprm_secureexec(struct linux_binprm *bprm);
// /*AFLA*/ int security_sb_alloc(struct super_block *sb);
// /*AFLA*/ void security_sb_free(struct super_block *sb);
// /*AFLA*/ int security_sb_copy_data(char *orig, char *copy);
// /*AFLA*/ int security_sb_remount(struct super_block *sb, void *data);
// /*AFLA*/ int security_sb_kern_mount(struct super_block *sb, int flags, void *data);
// /*AFLA*/ int security_sb_show_options(struct seq_file *m, struct super_block *sb);
// /*AFLA*/ int security_sb_statfs(struct dentry *dentry);
// /*AFLA*/ int security_sb_mount(const char *dev_name, const struct path *path,
// /*AFLA*/ 		      const char *type, unsigned long flags, void *data);
// /*AFLA*/ int security_sb_umount(struct vfsmount *mnt, int flags);
// /*AFLA*/ int security_sb_pivotroot(const struct path *old_path, const struct path *new_path);
// /*AFLA*/ int security_sb_set_mnt_opts(struct super_block *sb,
// /*AFLA*/ 				struct security_mnt_opts *opts,
// /*AFLA*/ 				unsigned long kern_flags,
// /*AFLA*/ 				unsigned long *set_kern_flags);
// /*AFLA*/ int security_sb_clone_mnt_opts(const struct super_block *oldsb,
// /*AFLA*/ 				struct super_block *newsb);
// /*AFLA*/ int security_sb_parse_opts_str(char *options, struct security_mnt_opts *opts);
// /*AFLA*/ int security_dentry_init_security(struct dentry *dentry, int mode,
// /*AFLA*/ 					const struct qstr *name, void **ctx,
// /*AFLA*/ 					u32 *ctxlen);
// /*AFLA*/ int security_dentry_create_files_as(struct dentry *dentry, int mode,
// /*AFLA*/ 					struct qstr *name,
// /*AFLA*/ 					const struct cred *old,
// /*AFLA*/ 					struct cred *new);
// /*AFLA*/ 
// /*AFLA*/ int security_inode_alloc(struct inode *inode);
// /*AFLA*/ void security_inode_free(struct inode *inode);
// /*AFLA*/ int security_inode_init_security(struct inode *inode, struct inode *dir,
// /*AFLA*/ 				 const struct qstr *qstr,
// /*AFLA*/ 				 initxattrs initxattrs, void *fs_data);
// /*AFLA*/ int security_old_inode_init_security(struct inode *inode, struct inode *dir,
// /*AFLA*/ 				     const struct qstr *qstr, const char **name,
// /*AFLA*/ 				     void **value, size_t *len);
// /*AFLA*/ int security_inode_create(struct inode *dir, struct dentry *dentry, umode_t mode);
// /*AFLA*/ int security_inode_link(struct dentry *old_dentry, struct inode *dir,
// /*AFLA*/ 			 struct dentry *new_dentry);
// /*AFLA*/ int security_inode_unlink(struct inode *dir, struct dentry *dentry);
// /*AFLA*/ int security_inode_symlink(struct inode *dir, struct dentry *dentry,
// /*AFLA*/ 			   const char *old_name);
// /*AFLA*/ int security_inode_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode);
// /*AFLA*/ int security_inode_rmdir(struct inode *dir, struct dentry *dentry);
// /*AFLA*/ int security_inode_mknod(struct inode *dir, struct dentry *dentry, umode_t mode, dev_t dev);
// /*AFLA*/ int security_inode_rename(struct inode *old_dir, struct dentry *old_dentry,
// /*AFLA*/ 			  struct inode *new_dir, struct dentry *new_dentry,
// /*AFLA*/ 			  unsigned int flags);
// /*AFLA*/ int security_inode_readlink(struct dentry *dentry);
// /*AFLA*/ int security_inode_follow_link(struct dentry *dentry, struct inode *inode,
// /*AFLA*/ 			       bool rcu);
// /*AFLA*/ int security_inode_permission(struct inode *inode, int mask);
// /*AFLA*/ int security_inode_setattr(struct dentry *dentry, struct iattr *attr);
// /*AFLA*/ int security_inode_getattr(const struct path *path);
// /*AFLA*/ int security_inode_setxattr(struct dentry *dentry, const char *name,
// /*AFLA*/ 			    const void *value, size_t size, int flags);
// /*AFLA*/ void security_inode_post_setxattr(struct dentry *dentry, const char *name,
// /*AFLA*/ 				  const void *value, size_t size, int flags);
// /*AFLA*/ int security_inode_getxattr(struct dentry *dentry, const char *name);
// /*AFLA*/ int security_inode_listxattr(struct dentry *dentry);
// /*AFLA*/ int security_inode_removexattr(struct dentry *dentry, const char *name);
// /*AFLA*/ int security_inode_need_killpriv(struct dentry *dentry);
// /*AFLA*/ int security_inode_killpriv(struct dentry *dentry);
// /*AFLA*/ int security_inode_getsecurity(struct inode *inode, const char *name, void **buffer, bool alloc);
// /*AFLA*/ int security_inode_setsecurity(struct inode *inode, const char *name, const void *value, size_t size, int flags);
// /*AFLA*/ int security_inode_listsecurity(struct inode *inode, char *buffer, size_t buffer_size);
// /*AFLA*/ void security_inode_getsecid(struct inode *inode, u32 *secid);
// /*AFLA*/ int security_inode_copy_up(struct dentry *src, struct cred **new);
// /*AFLA*/ int security_inode_copy_up_xattr(const char *name);
// /*AFLA*/ int security_file_permission(struct file *file, int mask);
// /*AFLA*/ int security_file_alloc(struct file *file);
// /*AFLA*/ void security_file_free(struct file *file);
// /*AFLA*/ int security_file_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
// /*AFLA*/ int security_mmap_file(struct file *file, unsigned long prot,
// /*AFLA*/ 			unsigned long flags);
// /*AFLA*/ int security_mmap_addr(unsigned long addr);
// /*AFLA*/ int security_file_mprotect(struct vm_area_struct *vma, unsigned long reqprot,
// /*AFLA*/ 			   unsigned long prot);
// /*AFLA*/ int security_file_lock(struct file *file, unsigned int cmd);
// /*AFLA*/ int security_file_fcntl(struct file *file, unsigned int cmd, unsigned long arg);
// /*AFLA*/ void security_file_set_fowner(struct file *file);
// /*AFLA*/ int security_file_send_sigiotask(struct task_struct *tsk,
// /*AFLA*/ 				 struct fown_struct *fown, int sig);
// /*AFLA*/ int security_file_receive(struct file *file);
// /*AFLA*/ int security_file_open(struct file *file, const struct cred *cred);
// /*AFLA*/ int security_task_create(unsigned long clone_flags);
// /*AFLA*/ void security_task_free(struct task_struct *task);
// /*AFLA*/ int security_cred_alloc_blank(struct cred *cred, gfp_t gfp);
// /*AFLA*/ void security_cred_free(struct cred *cred);
// /*AFLA*/ int security_prepare_creds(struct cred *new, const struct cred *old, gfp_t gfp);
// /*AFLA*/ void security_transfer_creds(struct cred *new, const struct cred *old);
// /*AFLA*/ int security_kernel_act_as(struct cred *new, u32 secid);
// /*AFLA*/ int security_kernel_create_files_as(struct cred *new, struct inode *inode);
// /*AFLA*/ int security_kernel_module_request(char *kmod_name);
// /*AFLA*/ int security_kernel_read_file(struct file *file, enum kernel_read_file_id id);
// /*AFLA*/ int security_kernel_post_read_file(struct file *file, char *buf, loff_t size,
// /*AFLA*/ 				   enum kernel_read_file_id id);
// /*AFLA*/ int security_task_fix_setuid(struct cred *new, const struct cred *old,
// /*AFLA*/ 			     int flags);
// /*AFLA*/ int security_task_setpgid(struct task_struct *p, pid_t pgid);
// /*AFLA*/ int security_task_getpgid(struct task_struct *p);
// /*AFLA*/ int security_task_getsid(struct task_struct *p);
// /*AFLA*/ void security_task_getsecid(struct task_struct *p, u32 *secid);
// /*AFLA*/ int security_task_setnice(struct task_struct *p, int nice);
// /*AFLA*/ int security_task_setioprio(struct task_struct *p, int ioprio);
// /*AFLA*/ int security_task_getioprio(struct task_struct *p);
// /*AFLA*/ int security_task_setrlimit(struct task_struct *p, unsigned int resource,
// /*AFLA*/ 		struct rlimit *new_rlim);
// /*AFLA*/ int security_task_setscheduler(struct task_struct *p);
// /*AFLA*/ int security_task_getscheduler(struct task_struct *p);
// /*AFLA*/ int security_task_movememory(struct task_struct *p);
// /*AFLA*/ int security_task_kill(struct task_struct *p, struct siginfo *info,
// /*AFLA*/ 			int sig, u32 secid);
// /*AFLA*/ int security_task_wait(struct task_struct *p);
// /*AFLA*/ int security_task_prctl(int option, unsigned long arg2, unsigned long arg3,
// /*AFLA*/ 			unsigned long arg4, unsigned long arg5);
// /*AFLA*/ void security_task_to_inode(struct task_struct *p, struct inode *inode);
// /*AFLA*/ int security_ipc_permission(struct kern_ipc_perm *ipcp, short flag);
// /*AFLA*/ void security_ipc_getsecid(struct kern_ipc_perm *ipcp, u32 *secid);
// /*AFLA*/ int security_msg_msg_alloc(struct msg_msg *msg);
// /*AFLA*/ void security_msg_msg_free(struct msg_msg *msg);
// /*AFLA*/ int security_msg_queue_alloc(struct msg_queue *msq);
// /*AFLA*/ void security_msg_queue_free(struct msg_queue *msq);
// /*AFLA*/ int security_msg_queue_associate(struct msg_queue *msq, int msqflg);
// /*AFLA*/ int security_msg_queue_msgctl(struct msg_queue *msq, int cmd);
// /*AFLA*/ int security_msg_queue_msgsnd(struct msg_queue *msq,
// /*AFLA*/ 			      struct msg_msg *msg, int msqflg);
// /*AFLA*/ int security_msg_queue_msgrcv(struct msg_queue *msq, struct msg_msg *msg,
// /*AFLA*/ 			      struct task_struct *target, long type, int mode);
// /*AFLA*/ int security_shm_alloc(struct shmid_kernel *shp);
// /*AFLA*/ void security_shm_free(struct shmid_kernel *shp);
// /*AFLA*/ int security_shm_associate(struct shmid_kernel *shp, int shmflg);
// /*AFLA*/ int security_shm_shmctl(struct shmid_kernel *shp, int cmd);
// /*AFLA*/ int security_shm_shmat(struct shmid_kernel *shp, char __user *shmaddr, int shmflg);
// /*AFLA*/ int security_sem_alloc(struct sem_array *sma);
// /*AFLA*/ void security_sem_free(struct sem_array *sma);
// /*AFLA*/ int security_sem_associate(struct sem_array *sma, int semflg);
// /*AFLA*/ int security_sem_semctl(struct sem_array *sma, int cmd);
// /*AFLA*/ int security_sem_semop(struct sem_array *sma, struct sembuf *sops,
// /*AFLA*/ 			unsigned nsops, int alter);
// /*AFLA*/ void security_d_instantiate(struct dentry *dentry, struct inode *inode);
// /*AFLA*/ int security_getprocattr(struct task_struct *p, char *name, char **value);
// /*AFLA*/ int security_setprocattr(struct task_struct *p, char *name, void *value, size_t size);
// /*AFLA*/ int security_netlink_send(struct sock *sk, struct sk_buff *skb);
// /*AFLA*/ int security_ismaclabel(const char *name);
// /*AFLA*/ int security_secid_to_secctx(u32 secid, char **secdata, u32 *seclen);
// /*AFLA*/ int security_secctx_to_secid(const char *secdata, u32 seclen, u32 *secid);
// /*AFLA*/ void security_release_secctx(char *secdata, u32 seclen);
// /*AFLA*/ 
// /*AFLA*/ void security_inode_invalidate_secctx(struct inode *inode);
// /*AFLA*/ int security_inode_notifysecctx(struct inode *inode, void *ctx, u32 ctxlen);
// /*AFLA*/ int security_inode_setsecctx(struct dentry *dentry, void *ctx, u32 ctxlen);
// /*AFLA*/ int security_inode_getsecctx(struct inode *inode, void **ctx, u32 *ctxlen);
// /*AFLA*/ #else /* CONFIG_SECURITY */
// /*AFLA*/ struct security_mnt_opts {
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline void security_init_mnt_opts(struct security_mnt_opts *opts)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_free_mnt_opts(struct security_mnt_opts *opts)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is the default capabilities functionality.  Most of these functions
// /*AFLA*/  * are just stubbed out, but a few must call the proper capable code.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static inline int security_init(void)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_binder_set_context_mgr(struct task_struct *mgr)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_binder_transaction(struct task_struct *from,
// /*AFLA*/ 					      struct task_struct *to)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_binder_transfer_binder(struct task_struct *from,
// /*AFLA*/ 						  struct task_struct *to)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_binder_transfer_file(struct task_struct *from,
// /*AFLA*/ 						struct task_struct *to,
// /*AFLA*/ 						struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_ptrace_access_check(struct task_struct *child,
// /*AFLA*/ 					     unsigned int mode)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_ptrace_access_check(child, mode);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_ptrace_traceme(struct task_struct *parent)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_ptrace_traceme(parent);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_capget(struct task_struct *target,
// /*AFLA*/ 				   kernel_cap_t *effective,
// /*AFLA*/ 				   kernel_cap_t *inheritable,
// /*AFLA*/ 				   kernel_cap_t *permitted)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_capget(target, effective, inheritable, permitted);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_capset(struct cred *new,
// /*AFLA*/ 				   const struct cred *old,
// /*AFLA*/ 				   const kernel_cap_t *effective,
// /*AFLA*/ 				   const kernel_cap_t *inheritable,
// /*AFLA*/ 				   const kernel_cap_t *permitted)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_capset(new, old, effective, inheritable, permitted);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_capable(const struct cred *cred,
// /*AFLA*/ 				   struct user_namespace *ns, int cap)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_capable(cred, ns, cap, SECURITY_CAP_AUDIT);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_capable_noaudit(const struct cred *cred,
// /*AFLA*/ 					   struct user_namespace *ns, int cap) {
// /*AFLA*/ 	return cap_capable(cred, ns, cap, SECURITY_CAP_NOAUDIT);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_quotactl(int cmds, int type, int id,
// /*AFLA*/ 				     struct super_block *sb)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_quota_on(struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_syslog(int type)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_settime64(const struct timespec64 *ts,
// /*AFLA*/ 				     const struct timezone *tz)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_settime(ts, tz);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_settime(const struct timespec *ts,
// /*AFLA*/ 				   const struct timezone *tz)
// /*AFLA*/ {
// /*AFLA*/ 	struct timespec64 ts64 = timespec_to_timespec64(*ts);
// /*AFLA*/ 
// /*AFLA*/ 	return cap_settime(&ts64, tz);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_vm_enough_memory_mm(struct mm_struct *mm, long pages)
// /*AFLA*/ {
// /*AFLA*/ 	return __vm_enough_memory(mm, pages, cap_vm_enough_memory(mm, pages));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_bprm_set_creds(struct linux_binprm *bprm)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_bprm_set_creds(bprm);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_bprm_check(struct linux_binprm *bprm)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_bprm_committing_creds(struct linux_binprm *bprm)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_bprm_committed_creds(struct linux_binprm *bprm)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_bprm_secureexec(struct linux_binprm *bprm)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_bprm_secureexec(bprm);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sb_alloc(struct super_block *sb)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_sb_free(struct super_block *sb)
// /*AFLA*/ { }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sb_copy_data(char *orig, char *copy)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sb_remount(struct super_block *sb, void *data)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sb_kern_mount(struct super_block *sb, int flags, void *data)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sb_show_options(struct seq_file *m,
// /*AFLA*/ 					   struct super_block *sb)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sb_statfs(struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sb_mount(const char *dev_name, const struct path *path,
// /*AFLA*/ 				    const char *type, unsigned long flags,
// /*AFLA*/ 				    void *data)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sb_umount(struct vfsmount *mnt, int flags)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sb_pivotroot(const struct path *old_path,
// /*AFLA*/ 					const struct path *new_path)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sb_set_mnt_opts(struct super_block *sb,
// /*AFLA*/ 					   struct security_mnt_opts *opts,
// /*AFLA*/ 					   unsigned long kern_flags,
// /*AFLA*/ 					   unsigned long *set_kern_flags)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sb_clone_mnt_opts(const struct super_block *oldsb,
// /*AFLA*/ 					      struct super_block *newsb)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sb_parse_opts_str(char *options, struct security_mnt_opts *opts)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_alloc(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_inode_free(struct inode *inode)
// /*AFLA*/ { }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_dentry_init_security(struct dentry *dentry,
// /*AFLA*/ 						 int mode,
// /*AFLA*/ 						 const struct qstr *name,
// /*AFLA*/ 						 void **ctx,
// /*AFLA*/ 						 u32 *ctxlen)
// /*AFLA*/ {
// /*AFLA*/ 	return -EOPNOTSUPP;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_dentry_create_files_as(struct dentry *dentry,
// /*AFLA*/ 						  int mode, struct qstr *name,
// /*AFLA*/ 						  const struct cred *old,
// /*AFLA*/ 						  struct cred *new)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_init_security(struct inode *inode,
// /*AFLA*/ 						struct inode *dir,
// /*AFLA*/ 						const struct qstr *qstr,
// /*AFLA*/ 						const initxattrs xattrs,
// /*AFLA*/ 						void *fs_data)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_old_inode_init_security(struct inode *inode,
// /*AFLA*/ 						   struct inode *dir,
// /*AFLA*/ 						   const struct qstr *qstr,
// /*AFLA*/ 						   const char **name,
// /*AFLA*/ 						   void **value, size_t *len)
// /*AFLA*/ {
// /*AFLA*/ 	return -EOPNOTSUPP;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_create(struct inode *dir,
// /*AFLA*/ 					 struct dentry *dentry,
// /*AFLA*/ 					 umode_t mode)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_link(struct dentry *old_dentry,
// /*AFLA*/ 				       struct inode *dir,
// /*AFLA*/ 				       struct dentry *new_dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_unlink(struct inode *dir,
// /*AFLA*/ 					 struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_symlink(struct inode *dir,
// /*AFLA*/ 					  struct dentry *dentry,
// /*AFLA*/ 					  const char *old_name)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_mkdir(struct inode *dir,
// /*AFLA*/ 					struct dentry *dentry,
// /*AFLA*/ 					int mode)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_rmdir(struct inode *dir,
// /*AFLA*/ 					struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_mknod(struct inode *dir,
// /*AFLA*/ 					struct dentry *dentry,
// /*AFLA*/ 					int mode, dev_t dev)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_rename(struct inode *old_dir,
// /*AFLA*/ 					 struct dentry *old_dentry,
// /*AFLA*/ 					 struct inode *new_dir,
// /*AFLA*/ 					 struct dentry *new_dentry,
// /*AFLA*/ 					 unsigned int flags)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_readlink(struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_follow_link(struct dentry *dentry,
// /*AFLA*/ 					     struct inode *inode,
// /*AFLA*/ 					     bool rcu)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_permission(struct inode *inode, int mask)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_setattr(struct dentry *dentry,
// /*AFLA*/ 					  struct iattr *attr)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_getattr(const struct path *path)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_setxattr(struct dentry *dentry,
// /*AFLA*/ 		const char *name, const void *value, size_t size, int flags)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_inode_setxattr(dentry, name, value, size, flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_inode_post_setxattr(struct dentry *dentry,
// /*AFLA*/ 		const char *name, const void *value, size_t size, int flags)
// /*AFLA*/ { }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_getxattr(struct dentry *dentry,
// /*AFLA*/ 			const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_listxattr(struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_removexattr(struct dentry *dentry,
// /*AFLA*/ 			const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_inode_removexattr(dentry, name);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_need_killpriv(struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_inode_need_killpriv(dentry);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_killpriv(struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_inode_killpriv(dentry);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_getsecurity(struct inode *inode, const char *name, void **buffer, bool alloc)
// /*AFLA*/ {
// /*AFLA*/ 	return -EOPNOTSUPP;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_setsecurity(struct inode *inode, const char *name, const void *value, size_t size, int flags)
// /*AFLA*/ {
// /*AFLA*/ 	return -EOPNOTSUPP;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_listsecurity(struct inode *inode, char *buffer, size_t buffer_size)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_inode_getsecid(struct inode *inode, u32 *secid)
// /*AFLA*/ {
// /*AFLA*/ 	*secid = 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_copy_up(struct dentry *src, struct cred **new)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_copy_up_xattr(const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	return -EOPNOTSUPP;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_file_permission(struct file *file, int mask)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_file_alloc(struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_file_free(struct file *file)
// /*AFLA*/ { }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_file_ioctl(struct file *file, unsigned int cmd,
// /*AFLA*/ 				      unsigned long arg)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_mmap_file(struct file *file, unsigned long prot,
// /*AFLA*/ 				     unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_mmap_addr(unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_mmap_addr(addr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_file_mprotect(struct vm_area_struct *vma,
// /*AFLA*/ 					 unsigned long reqprot,
// /*AFLA*/ 					 unsigned long prot)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_file_lock(struct file *file, unsigned int cmd)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_file_fcntl(struct file *file, unsigned int cmd,
// /*AFLA*/ 				      unsigned long arg)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_file_set_fowner(struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_file_send_sigiotask(struct task_struct *tsk,
// /*AFLA*/ 					       struct fown_struct *fown,
// /*AFLA*/ 					       int sig)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_file_receive(struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_file_open(struct file *file,
// /*AFLA*/ 				     const struct cred *cred)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_task_create(unsigned long clone_flags)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_task_free(struct task_struct *task)
// /*AFLA*/ { }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_cred_alloc_blank(struct cred *cred, gfp_t gfp)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_cred_free(struct cred *cred)
// /*AFLA*/ { }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_prepare_creds(struct cred *new,
// /*AFLA*/ 					 const struct cred *old,
// /*AFLA*/ 					 gfp_t gfp)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_transfer_creds(struct cred *new,
// /*AFLA*/ 					   const struct cred *old)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_kernel_act_as(struct cred *cred, u32 secid)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_kernel_create_files_as(struct cred *cred,
// /*AFLA*/ 						  struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_kernel_module_request(char *kmod_name)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_kernel_read_file(struct file *file,
// /*AFLA*/ 					    enum kernel_read_file_id id)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_kernel_post_read_file(struct file *file,
// /*AFLA*/ 						 char *buf, loff_t size,
// /*AFLA*/ 						 enum kernel_read_file_id id)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_task_fix_setuid(struct cred *new,
// /*AFLA*/ 					   const struct cred *old,
// /*AFLA*/ 					   int flags)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_task_fix_setuid(new, old, flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_task_setpgid(struct task_struct *p, pid_t pgid)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_task_getpgid(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_task_getsid(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_task_getsecid(struct task_struct *p, u32 *secid)
// /*AFLA*/ {
// /*AFLA*/ 	*secid = 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_task_setnice(struct task_struct *p, int nice)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_task_setnice(p, nice);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_task_setioprio(struct task_struct *p, int ioprio)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_task_setioprio(p, ioprio);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_task_getioprio(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_task_setrlimit(struct task_struct *p,
// /*AFLA*/ 					  unsigned int resource,
// /*AFLA*/ 					  struct rlimit *new_rlim)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_task_setscheduler(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_task_setscheduler(p);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_task_getscheduler(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_task_movememory(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_task_kill(struct task_struct *p,
// /*AFLA*/ 				     struct siginfo *info, int sig,
// /*AFLA*/ 				     u32 secid)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_task_wait(struct task_struct *p)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_task_prctl(int option, unsigned long arg2,
// /*AFLA*/ 				      unsigned long arg3,
// /*AFLA*/ 				      unsigned long arg4,
// /*AFLA*/ 				      unsigned long arg5)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_task_prctl(option, arg2, arg3, arg4, arg5);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_task_to_inode(struct task_struct *p, struct inode *inode)
// /*AFLA*/ { }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_ipc_permission(struct kern_ipc_perm *ipcp,
// /*AFLA*/ 					  short flag)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_ipc_getsecid(struct kern_ipc_perm *ipcp, u32 *secid)
// /*AFLA*/ {
// /*AFLA*/ 	*secid = 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_msg_msg_alloc(struct msg_msg *msg)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_msg_msg_free(struct msg_msg *msg)
// /*AFLA*/ { }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_msg_queue_alloc(struct msg_queue *msq)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_msg_queue_free(struct msg_queue *msq)
// /*AFLA*/ { }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_msg_queue_associate(struct msg_queue *msq,
// /*AFLA*/ 					       int msqflg)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_msg_queue_msgctl(struct msg_queue *msq, int cmd)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_msg_queue_msgsnd(struct msg_queue *msq,
// /*AFLA*/ 					    struct msg_msg *msg, int msqflg)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_msg_queue_msgrcv(struct msg_queue *msq,
// /*AFLA*/ 					    struct msg_msg *msg,
// /*AFLA*/ 					    struct task_struct *target,
// /*AFLA*/ 					    long type, int mode)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_shm_alloc(struct shmid_kernel *shp)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_shm_free(struct shmid_kernel *shp)
// /*AFLA*/ { }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_shm_associate(struct shmid_kernel *shp,
// /*AFLA*/ 					 int shmflg)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_shm_shmctl(struct shmid_kernel *shp, int cmd)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_shm_shmat(struct shmid_kernel *shp,
// /*AFLA*/ 				     char __user *shmaddr, int shmflg)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sem_alloc(struct sem_array *sma)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_sem_free(struct sem_array *sma)
// /*AFLA*/ { }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sem_associate(struct sem_array *sma, int semflg)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sem_semctl(struct sem_array *sma, int cmd)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sem_semop(struct sem_array *sma,
// /*AFLA*/ 				     struct sembuf *sops, unsigned nsops,
// /*AFLA*/ 				     int alter)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_d_instantiate(struct dentry *dentry, struct inode *inode)
// /*AFLA*/ { }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_getprocattr(struct task_struct *p, char *name, char **value)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_setprocattr(struct task_struct *p, char *name, void *value, size_t size)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_netlink_send(struct sock *sk, struct sk_buff *skb)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_ismaclabel(const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_secid_to_secctx(u32 secid, char **secdata, u32 *seclen)
// /*AFLA*/ {
// /*AFLA*/ 	return -EOPNOTSUPP;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_secctx_to_secid(const char *secdata,
// /*AFLA*/ 					   u32 seclen,
// /*AFLA*/ 					   u32 *secid)
// /*AFLA*/ {
// /*AFLA*/ 	return -EOPNOTSUPP;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_release_secctx(char *secdata, u32 seclen)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_inode_invalidate_secctx(struct inode *inode)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inode_notifysecctx(struct inode *inode, void *ctx, u32 ctxlen)
// /*AFLA*/ {
// /*AFLA*/ 	return -EOPNOTSUPP;
// /*AFLA*/ }
// /*AFLA*/ static inline int security_inode_setsecctx(struct dentry *dentry, void *ctx, u32 ctxlen)
// /*AFLA*/ {
// /*AFLA*/ 	return -EOPNOTSUPP;
// /*AFLA*/ }
// /*AFLA*/ static inline int security_inode_getsecctx(struct inode *inode, void **ctx, u32 *ctxlen)
// /*AFLA*/ {
// /*AFLA*/ 	return -EOPNOTSUPP;
// /*AFLA*/ }
// /*AFLA*/ #endif	/* CONFIG_SECURITY */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SECURITY_NETWORK
// /*AFLA*/ 
// /*AFLA*/ int security_unix_stream_connect(struct sock *sock, struct sock *other, struct sock *newsk);
// /*AFLA*/ int security_unix_may_send(struct socket *sock,  struct socket *other);
// /*AFLA*/ int security_socket_create(int family, int type, int protocol, int kern);
// /*AFLA*/ int security_socket_post_create(struct socket *sock, int family,
// /*AFLA*/ 				int type, int protocol, int kern);
// /*AFLA*/ int security_socket_bind(struct socket *sock, struct sockaddr *address, int addrlen);
// /*AFLA*/ int security_socket_connect(struct socket *sock, struct sockaddr *address, int addrlen);
// /*AFLA*/ int security_socket_listen(struct socket *sock, int backlog);
// /*AFLA*/ int security_socket_accept(struct socket *sock, struct socket *newsock);
// /*AFLA*/ int security_socket_sendmsg(struct socket *sock, struct msghdr *msg, int size);
// /*AFLA*/ int security_socket_recvmsg(struct socket *sock, struct msghdr *msg,
// /*AFLA*/ 			    int size, int flags);
// /*AFLA*/ int security_socket_getsockname(struct socket *sock);
// /*AFLA*/ int security_socket_getpeername(struct socket *sock);
// /*AFLA*/ int security_socket_getsockopt(struct socket *sock, int level, int optname);
// /*AFLA*/ int security_socket_setsockopt(struct socket *sock, int level, int optname);
// /*AFLA*/ int security_socket_shutdown(struct socket *sock, int how);
// /*AFLA*/ int security_sock_rcv_skb(struct sock *sk, struct sk_buff *skb);
// /*AFLA*/ int security_socket_getpeersec_stream(struct socket *sock, char __user *optval,
// /*AFLA*/ 				      int __user *optlen, unsigned len);
// /*AFLA*/ int security_socket_getpeersec_dgram(struct socket *sock, struct sk_buff *skb, u32 *secid);
// /*AFLA*/ int security_sk_alloc(struct sock *sk, int family, gfp_t priority);
// /*AFLA*/ void security_sk_free(struct sock *sk);
// /*AFLA*/ void security_sk_clone(const struct sock *sk, struct sock *newsk);
// /*AFLA*/ void security_sk_classify_flow(struct sock *sk, struct flowi *fl);
// /*AFLA*/ void security_req_classify_flow(const struct request_sock *req, struct flowi *fl);
// /*AFLA*/ void security_sock_graft(struct sock*sk, struct socket *parent);
// /*AFLA*/ int security_inet_conn_request(struct sock *sk,
// /*AFLA*/ 			struct sk_buff *skb, struct request_sock *req);
// /*AFLA*/ void security_inet_csk_clone(struct sock *newsk,
// /*AFLA*/ 			const struct request_sock *req);
// /*AFLA*/ void security_inet_conn_established(struct sock *sk,
// /*AFLA*/ 			struct sk_buff *skb);
// /*AFLA*/ int security_secmark_relabel_packet(u32 secid);
// /*AFLA*/ void security_secmark_refcount_inc(void);
// /*AFLA*/ void security_secmark_refcount_dec(void);
// /*AFLA*/ int security_tun_dev_alloc_security(void **security);
// /*AFLA*/ void security_tun_dev_free_security(void *security);
// /*AFLA*/ int security_tun_dev_create(void);
// /*AFLA*/ int security_tun_dev_attach_queue(void *security);
// /*AFLA*/ int security_tun_dev_attach(struct sock *sk, void *security);
// /*AFLA*/ int security_tun_dev_open(void *security);
// /*AFLA*/ 
// /*AFLA*/ #else	/* CONFIG_SECURITY_NETWORK */
// /*AFLA*/ static inline int security_unix_stream_connect(struct sock *sock,
// /*AFLA*/ 					       struct sock *other,
// /*AFLA*/ 					       struct sock *newsk)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_unix_may_send(struct socket *sock,
// /*AFLA*/ 					 struct socket *other)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_socket_create(int family, int type,
// /*AFLA*/ 					 int protocol, int kern)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_socket_post_create(struct socket *sock,
// /*AFLA*/ 					      int family,
// /*AFLA*/ 					      int type,
// /*AFLA*/ 					      int protocol, int kern)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_socket_bind(struct socket *sock,
// /*AFLA*/ 				       struct sockaddr *address,
// /*AFLA*/ 				       int addrlen)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_socket_connect(struct socket *sock,
// /*AFLA*/ 					  struct sockaddr *address,
// /*AFLA*/ 					  int addrlen)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_socket_listen(struct socket *sock, int backlog)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_socket_accept(struct socket *sock,
// /*AFLA*/ 					 struct socket *newsock)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_socket_sendmsg(struct socket *sock,
// /*AFLA*/ 					  struct msghdr *msg, int size)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_socket_recvmsg(struct socket *sock,
// /*AFLA*/ 					  struct msghdr *msg, int size,
// /*AFLA*/ 					  int flags)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_socket_getsockname(struct socket *sock)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_socket_getpeername(struct socket *sock)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_socket_getsockopt(struct socket *sock,
// /*AFLA*/ 					     int level, int optname)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_socket_setsockopt(struct socket *sock,
// /*AFLA*/ 					     int level, int optname)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_socket_shutdown(struct socket *sock, int how)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int security_sock_rcv_skb(struct sock *sk,
// /*AFLA*/ 					struct sk_buff *skb)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_socket_getpeersec_stream(struct socket *sock, char __user *optval,
// /*AFLA*/ 						    int __user *optlen, unsigned len)
// /*AFLA*/ {
// /*AFLA*/ 	return -ENOPROTOOPT;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_socket_getpeersec_dgram(struct socket *sock, struct sk_buff *skb, u32 *secid)
// /*AFLA*/ {
// /*AFLA*/ 	return -ENOPROTOOPT;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_sk_alloc(struct sock *sk, int family, gfp_t priority)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_sk_free(struct sock *sk)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_sk_clone(const struct sock *sk, struct sock *newsk)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_sk_classify_flow(struct sock *sk, struct flowi *fl)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_req_classify_flow(const struct request_sock *req, struct flowi *fl)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_sock_graft(struct sock *sk, struct socket *parent)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_inet_conn_request(struct sock *sk,
// /*AFLA*/ 			struct sk_buff *skb, struct request_sock *req)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_inet_csk_clone(struct sock *newsk,
// /*AFLA*/ 			const struct request_sock *req)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_inet_conn_established(struct sock *sk,
// /*AFLA*/ 			struct sk_buff *skb)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_secmark_relabel_packet(u32 secid)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_secmark_refcount_inc(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_secmark_refcount_dec(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_tun_dev_alloc_security(void **security)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_tun_dev_free_security(void *security)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_tun_dev_create(void)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_tun_dev_attach_queue(void *security)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_tun_dev_attach(struct sock *sk, void *security)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_tun_dev_open(void *security)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif	/* CONFIG_SECURITY_NETWORK */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SECURITY_NETWORK_XFRM
// /*AFLA*/ 
// /*AFLA*/ int security_xfrm_policy_alloc(struct xfrm_sec_ctx **ctxp,
// /*AFLA*/ 			       struct xfrm_user_sec_ctx *sec_ctx, gfp_t gfp);
// /*AFLA*/ int security_xfrm_policy_clone(struct xfrm_sec_ctx *old_ctx, struct xfrm_sec_ctx **new_ctxp);
// /*AFLA*/ void security_xfrm_policy_free(struct xfrm_sec_ctx *ctx);
// /*AFLA*/ int security_xfrm_policy_delete(struct xfrm_sec_ctx *ctx);
// /*AFLA*/ int security_xfrm_state_alloc(struct xfrm_state *x, struct xfrm_user_sec_ctx *sec_ctx);
// /*AFLA*/ int security_xfrm_state_alloc_acquire(struct xfrm_state *x,
// /*AFLA*/ 				      struct xfrm_sec_ctx *polsec, u32 secid);
// /*AFLA*/ int security_xfrm_state_delete(struct xfrm_state *x);
// /*AFLA*/ void security_xfrm_state_free(struct xfrm_state *x);
// /*AFLA*/ int security_xfrm_policy_lookup(struct xfrm_sec_ctx *ctx, u32 fl_secid, u8 dir);
// /*AFLA*/ int security_xfrm_state_pol_flow_match(struct xfrm_state *x,
// /*AFLA*/ 				       struct xfrm_policy *xp,
// /*AFLA*/ 				       const struct flowi *fl);
// /*AFLA*/ int security_xfrm_decode_session(struct sk_buff *skb, u32 *secid);
// /*AFLA*/ void security_skb_classify_flow(struct sk_buff *skb, struct flowi *fl);
// /*AFLA*/ 
// /*AFLA*/ #else	/* CONFIG_SECURITY_NETWORK_XFRM */
// /*AFLA*/ 
// /*AFLA*/ static inline int security_xfrm_policy_alloc(struct xfrm_sec_ctx **ctxp,
// /*AFLA*/ 					     struct xfrm_user_sec_ctx *sec_ctx,
// /*AFLA*/ 					     gfp_t gfp)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_xfrm_policy_clone(struct xfrm_sec_ctx *old, struct xfrm_sec_ctx **new_ctxp)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_xfrm_policy_free(struct xfrm_sec_ctx *ctx)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_xfrm_policy_delete(struct xfrm_sec_ctx *ctx)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_xfrm_state_alloc(struct xfrm_state *x,
// /*AFLA*/ 					struct xfrm_user_sec_ctx *sec_ctx)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_xfrm_state_alloc_acquire(struct xfrm_state *x,
// /*AFLA*/ 					struct xfrm_sec_ctx *polsec, u32 secid)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_xfrm_state_free(struct xfrm_state *x)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_xfrm_state_delete(struct xfrm_state *x)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_xfrm_policy_lookup(struct xfrm_sec_ctx *ctx, u32 fl_secid, u8 dir)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_xfrm_state_pol_flow_match(struct xfrm_state *x,
// /*AFLA*/ 			struct xfrm_policy *xp, const struct flowi *fl)
// /*AFLA*/ {
// /*AFLA*/ 	return 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_xfrm_decode_session(struct sk_buff *skb, u32 *secid)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_skb_classify_flow(struct sk_buff *skb, struct flowi *fl)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif	/* CONFIG_SECURITY_NETWORK_XFRM */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SECURITY_PATH
// /*AFLA*/ int security_path_unlink(const struct path *dir, struct dentry *dentry);
// /*AFLA*/ int security_path_mkdir(const struct path *dir, struct dentry *dentry, umode_t mode);
// /*AFLA*/ int security_path_rmdir(const struct path *dir, struct dentry *dentry);
// /*AFLA*/ int security_path_mknod(const struct path *dir, struct dentry *dentry, umode_t mode,
// /*AFLA*/ 			unsigned int dev);
// /*AFLA*/ int security_path_truncate(const struct path *path);
// /*AFLA*/ int security_path_symlink(const struct path *dir, struct dentry *dentry,
// /*AFLA*/ 			  const char *old_name);
// /*AFLA*/ int security_path_link(struct dentry *old_dentry, const struct path *new_dir,
// /*AFLA*/ 		       struct dentry *new_dentry);
// /*AFLA*/ int security_path_rename(const struct path *old_dir, struct dentry *old_dentry,
// /*AFLA*/ 			 const struct path *new_dir, struct dentry *new_dentry,
// /*AFLA*/ 			 unsigned int flags);
// /*AFLA*/ int security_path_chmod(const struct path *path, umode_t mode);
// /*AFLA*/ int security_path_chown(const struct path *path, kuid_t uid, kgid_t gid);
// /*AFLA*/ int security_path_chroot(const struct path *path);
// /*AFLA*/ #else	/* CONFIG_SECURITY_PATH */
// /*AFLA*/ static inline int security_path_unlink(const struct path *dir, struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_path_mkdir(const struct path *dir, struct dentry *dentry,
// /*AFLA*/ 				      umode_t mode)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_path_rmdir(const struct path *dir, struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_path_mknod(const struct path *dir, struct dentry *dentry,
// /*AFLA*/ 				      umode_t mode, unsigned int dev)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_path_truncate(const struct path *path)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_path_symlink(const struct path *dir, struct dentry *dentry,
// /*AFLA*/ 					const char *old_name)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_path_link(struct dentry *old_dentry,
// /*AFLA*/ 				     const struct path *new_dir,
// /*AFLA*/ 				     struct dentry *new_dentry)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_path_rename(const struct path *old_dir,
// /*AFLA*/ 				       struct dentry *old_dentry,
// /*AFLA*/ 				       const struct path *new_dir,
// /*AFLA*/ 				       struct dentry *new_dentry,
// /*AFLA*/ 				       unsigned int flags)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_path_chmod(const struct path *path, umode_t mode)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_path_chown(const struct path *path, kuid_t uid, kgid_t gid)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_path_chroot(const struct path *path)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif	/* CONFIG_SECURITY_PATH */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KEYS
// /*AFLA*/ #ifdef CONFIG_SECURITY
// /*AFLA*/ 
// /*AFLA*/ int security_key_alloc(struct key *key, const struct cred *cred, unsigned long flags);
// /*AFLA*/ void security_key_free(struct key *key);
// /*AFLA*/ int security_key_permission(key_ref_t key_ref,
// /*AFLA*/ 			    const struct cred *cred, unsigned perm);
// /*AFLA*/ int security_key_getsecurity(struct key *key, char **_buffer);
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline int security_key_alloc(struct key *key,
// /*AFLA*/ 				     const struct cred *cred,
// /*AFLA*/ 				     unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_key_free(struct key *key)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_key_permission(key_ref_t key_ref,
// /*AFLA*/ 					  const struct cred *cred,
// /*AFLA*/ 					  unsigned perm)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_key_getsecurity(struct key *key, char **_buffer)
// /*AFLA*/ {
// /*AFLA*/ 	*_buffer = NULL;
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* CONFIG_KEYS */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_AUDIT
// /*AFLA*/ #ifdef CONFIG_SECURITY
// /*AFLA*/ int security_audit_rule_init(u32 field, u32 op, char *rulestr, void **lsmrule);
// /*AFLA*/ int security_audit_rule_known(struct audit_krule *krule);
// /*AFLA*/ int security_audit_rule_match(u32 secid, u32 field, u32 op, void *lsmrule,
// /*AFLA*/ 			      struct audit_context *actx);
// /*AFLA*/ void security_audit_rule_free(void *lsmrule);
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline int security_audit_rule_init(u32 field, u32 op, char *rulestr,
// /*AFLA*/ 					   void **lsmrule)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_audit_rule_known(struct audit_krule *krule)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int security_audit_rule_match(u32 secid, u32 field, u32 op,
// /*AFLA*/ 				   void *lsmrule, struct audit_context *actx)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void security_audit_rule_free(void *lsmrule)
// /*AFLA*/ { }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_SECURITY */
// /*AFLA*/ #endif /* CONFIG_AUDIT */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SECURITYFS
// /*AFLA*/ 
// /*AFLA*/ extern struct dentry *securityfs_create_file(const char *name, umode_t mode,
// /*AFLA*/ 					     struct dentry *parent, void *data,
// /*AFLA*/ 					     const struct file_operations *fops);
// /*AFLA*/ extern struct dentry *securityfs_create_dir(const char *name, struct dentry *parent);
// /*AFLA*/ extern void securityfs_remove(struct dentry *dentry);
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_SECURITYFS */
// /*AFLA*/ 
// /*AFLA*/ static inline struct dentry *securityfs_create_dir(const char *name,
// /*AFLA*/ 						   struct dentry *parent)
// /*AFLA*/ {
// /*AFLA*/ 	return ERR_PTR(-ENODEV);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct dentry *securityfs_create_file(const char *name,
// /*AFLA*/ 						    umode_t mode,
// /*AFLA*/ 						    struct dentry *parent,
// /*AFLA*/ 						    void *data,
// /*AFLA*/ 						    const struct file_operations *fops)
// /*AFLA*/ {
// /*AFLA*/ 	return ERR_PTR(-ENODEV);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void securityfs_remove(struct dentry *dentry)
// /*AFLA*/ {}
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SECURITY
// /*AFLA*/ 
// /*AFLA*/ static inline char *alloc_secdata(void)
// /*AFLA*/ {
// /*AFLA*/ 	return (char *)get_zeroed_page(GFP_KERNEL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void free_secdata(void *secdata)
// /*AFLA*/ {
// /*AFLA*/ 	free_page((unsigned long)secdata);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline char *alloc_secdata(void)
// /*AFLA*/ {
// /*AFLA*/         return (char *)1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void free_secdata(void *secdata)
// /*AFLA*/ { }
// /*AFLA*/ #endif /* CONFIG_SECURITY */
// /*AFLA*/ 
// /*AFLA*/ #endif /* ! __LINUX_SECURITY_H */
// /*AFLA*/ 
