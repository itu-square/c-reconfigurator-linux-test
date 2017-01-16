// /*AFLA*/ /* audit.h -- Auditing support
// /*AFLA*/  *
// /*AFLA*/  * Copyright 2003-2004 Red Hat Inc., Durham, North Carolina.
// /*AFLA*/  * All Rights Reserved.
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify
// /*AFLA*/  * it under the terms of the GNU General Public License as published by
// /*AFLA*/  * the Free Software Foundation; either version 2 of the License, or
// /*AFLA*/  * (at your option) any later version.
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it will be useful,
// /*AFLA*/  * but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/  * GNU General Public License for more details.
// /*AFLA*/  *
// /*AFLA*/  * You should have received a copy of the GNU General Public License
// /*AFLA*/  * along with this program; if not, write to the Free Software
// /*AFLA*/  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// /*AFLA*/  *
// /*AFLA*/  * Written by Rickard E. (Rik) Faith <faith@redhat.com>
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LINUX_AUDIT_H_
// /*AFLA*/ #define _LINUX_AUDIT_H_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/sched.h>
// /*AFLA*/ #include <linux/ptrace.h>
// /*AFLA*/ #include <uapi/linux/audit.h>
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_INO_UNSET ((unsigned long)-1)
// /*AFLA*/ #define AUDIT_DEV_UNSET ((dev_t)-1)
// /*AFLA*/ 
// /*AFLA*/ struct audit_sig_info {
// /*AFLA*/ 	uid_t		uid;
// /*AFLA*/ 	pid_t		pid;
// /*AFLA*/ 	char		ctx[0];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct audit_buffer;
// /*AFLA*/ struct audit_context;
// /*AFLA*/ struct inode;
// /*AFLA*/ struct netlink_skb_parms;
// /*AFLA*/ struct path;
// /*AFLA*/ struct linux_binprm;
// /*AFLA*/ struct mq_attr;
// /*AFLA*/ struct mqstat;
// /*AFLA*/ struct audit_watch;
// /*AFLA*/ struct audit_tree;
// /*AFLA*/ struct sk_buff;
// /*AFLA*/ 
// /*AFLA*/ struct audit_krule {
// /*AFLA*/ 	u32			pflags;
// /*AFLA*/ 	u32			flags;
// /*AFLA*/ 	u32			listnr;
// /*AFLA*/ 	u32			action;
// /*AFLA*/ 	u32			mask[AUDIT_BITMASK_SIZE];
// /*AFLA*/ 	u32			buflen; /* for data alloc on list rules */
// /*AFLA*/ 	u32			field_count;
// /*AFLA*/ 	char			*filterkey; /* ties events to rules */
// /*AFLA*/ 	struct audit_field	*fields;
// /*AFLA*/ 	struct audit_field	*arch_f; /* quick access to arch field */
// /*AFLA*/ 	struct audit_field	*inode_f; /* quick access to an inode field */
// /*AFLA*/ 	struct audit_watch	*watch;	/* associated watch */
// /*AFLA*/ 	struct audit_tree	*tree;	/* associated watched tree */
// /*AFLA*/ 	struct audit_fsnotify_mark	*exe;
// /*AFLA*/ 	struct list_head	rlist;	/* entry in audit_{watch,tree}.rules list */
// /*AFLA*/ 	struct list_head	list;	/* for AUDIT_LIST* purposes only */
// /*AFLA*/ 	u64			prio;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Flag to indicate legacy AUDIT_LOGINUID unset usage */
// /*AFLA*/ #define AUDIT_LOGINUID_LEGACY		0x1
// /*AFLA*/ 
// /*AFLA*/ struct audit_field {
// /*AFLA*/ 	u32				type;
// /*AFLA*/ 	union {
// /*AFLA*/ 		u32			val;
// /*AFLA*/ 		kuid_t			uid;
// /*AFLA*/ 		kgid_t			gid;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			char		*lsm_str;
// /*AFLA*/ 			void		*lsm_rule;
// /*AFLA*/ 		};
// /*AFLA*/ 	};
// /*AFLA*/ 	u32				op;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern int is_audit_feature_set(int which);
// /*AFLA*/ 
// /*AFLA*/ extern int __init audit_register_class(int class, unsigned *list);
// /*AFLA*/ extern int audit_classify_syscall(int abi, unsigned syscall);
// /*AFLA*/ extern int audit_classify_arch(int arch);
// /*AFLA*/ /* only for compat system calls */
// /*AFLA*/ extern unsigned compat_write_class[];
// /*AFLA*/ extern unsigned compat_read_class[];
// /*AFLA*/ extern unsigned compat_dir_class[];
// /*AFLA*/ extern unsigned compat_chattr_class[];
// /*AFLA*/ extern unsigned compat_signal_class[];
// /*AFLA*/ 
// /*AFLA*/ extern int audit_classify_compat_syscall(int abi, unsigned syscall);
// /*AFLA*/ 
// /*AFLA*/ /* audit_names->type values */
// /*AFLA*/ #define	AUDIT_TYPE_UNKNOWN	0	/* we don't know yet */
// /*AFLA*/ #define	AUDIT_TYPE_NORMAL	1	/* a "normal" audit record */
// /*AFLA*/ #define	AUDIT_TYPE_PARENT	2	/* a parent audit record */
// /*AFLA*/ #define	AUDIT_TYPE_CHILD_DELETE 3	/* a child being deleted */
// /*AFLA*/ #define	AUDIT_TYPE_CHILD_CREATE 4	/* a child being created */
// /*AFLA*/ 
// /*AFLA*/ /* maximized args number that audit_socketcall can process */
// /*AFLA*/ #define AUDITSC_ARGS		6
// /*AFLA*/ 
// /*AFLA*/ /* bit values for ->signal->audit_tty */
// /*AFLA*/ #define AUDIT_TTY_ENABLE	BIT(0)
// /*AFLA*/ #define AUDIT_TTY_LOG_PASSWD	BIT(1)
// /*AFLA*/ 
// /*AFLA*/ struct filename;
// /*AFLA*/ 
// /*AFLA*/ extern void audit_log_session_info(struct audit_buffer *ab);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_AUDIT
// /*AFLA*/ /* These are defined in audit.c */
// /*AFLA*/ 				/* Public API */
// /*AFLA*/ extern __printf(4, 5)
// /*AFLA*/ void audit_log(struct audit_context *ctx, gfp_t gfp_mask, int type,
// /*AFLA*/ 	       const char *fmt, ...);
// /*AFLA*/ 
// /*AFLA*/ extern struct audit_buffer *audit_log_start(struct audit_context *ctx, gfp_t gfp_mask, int type);
// /*AFLA*/ extern __printf(2, 3)
// /*AFLA*/ void audit_log_format(struct audit_buffer *ab, const char *fmt, ...);
// /*AFLA*/ extern void		    audit_log_end(struct audit_buffer *ab);
// /*AFLA*/ extern bool		    audit_string_contains_control(const char *string,
// /*AFLA*/ 							  size_t len);
// /*AFLA*/ extern void		    audit_log_n_hex(struct audit_buffer *ab,
// /*AFLA*/ 					  const unsigned char *buf,
// /*AFLA*/ 					  size_t len);
// /*AFLA*/ extern void		    audit_log_n_string(struct audit_buffer *ab,
// /*AFLA*/ 					       const char *buf,
// /*AFLA*/ 					       size_t n);
// /*AFLA*/ extern void		    audit_log_n_untrustedstring(struct audit_buffer *ab,
// /*AFLA*/ 							const char *string,
// /*AFLA*/ 							size_t n);
// /*AFLA*/ extern void		    audit_log_untrustedstring(struct audit_buffer *ab,
// /*AFLA*/ 						      const char *string);
// /*AFLA*/ extern void		    audit_log_d_path(struct audit_buffer *ab,
// /*AFLA*/ 					     const char *prefix,
// /*AFLA*/ 					     const struct path *path);
// /*AFLA*/ extern void		    audit_log_key(struct audit_buffer *ab,
// /*AFLA*/ 					  char *key);
// /*AFLA*/ extern void		    audit_log_link_denied(const char *operation,
// /*AFLA*/ 						  struct path *link);
// /*AFLA*/ extern void		    audit_log_lost(const char *message);
// /*AFLA*/ #ifdef CONFIG_SECURITY
// /*AFLA*/ extern void 		    audit_log_secctx(struct audit_buffer *ab, u32 secid);
// /*AFLA*/ #else
// /*AFLA*/ static inline void	    audit_log_secctx(struct audit_buffer *ab, u32 secid)
// /*AFLA*/ { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern int audit_log_task_context(struct audit_buffer *ab);
// /*AFLA*/ extern void audit_log_task_info(struct audit_buffer *ab,
// /*AFLA*/ 				struct task_struct *tsk);
// /*AFLA*/ 
// /*AFLA*/ extern int		    audit_update_lsm_rules(void);
// /*AFLA*/ 
// /*AFLA*/ 				/* Private API (for audit.c only) */
// /*AFLA*/ extern int audit_rule_change(int type, __u32 portid, int seq,
// /*AFLA*/ 				void *data, size_t datasz);
// /*AFLA*/ extern int audit_list_rules_send(struct sk_buff *request_skb, int seq);
// /*AFLA*/ 
// /*AFLA*/ extern u32 audit_enabled;
// /*AFLA*/ #else /* CONFIG_AUDIT */
// /*AFLA*/ static inline __printf(4, 5)
// /*AFLA*/ void audit_log(struct audit_context *ctx, gfp_t gfp_mask, int type,
// /*AFLA*/ 	       const char *fmt, ...)
// /*AFLA*/ { }
// /*AFLA*/ static inline struct audit_buffer *audit_log_start(struct audit_context *ctx,
// /*AFLA*/ 						   gfp_t gfp_mask, int type)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ static inline __printf(2, 3)
// /*AFLA*/ void audit_log_format(struct audit_buffer *ab, const char *fmt, ...)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_log_end(struct audit_buffer *ab)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_log_n_hex(struct audit_buffer *ab,
// /*AFLA*/ 				   const unsigned char *buf, size_t len)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_log_n_string(struct audit_buffer *ab,
// /*AFLA*/ 				      const char *buf, size_t n)
// /*AFLA*/ { }
// /*AFLA*/ static inline void  audit_log_n_untrustedstring(struct audit_buffer *ab,
// /*AFLA*/ 						const char *string, size_t n)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_log_untrustedstring(struct audit_buffer *ab,
// /*AFLA*/ 					     const char *string)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_log_d_path(struct audit_buffer *ab,
// /*AFLA*/ 				    const char *prefix,
// /*AFLA*/ 				    const struct path *path)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_log_key(struct audit_buffer *ab, char *key)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_log_link_denied(const char *string,
// /*AFLA*/ 					 const struct path *link)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_log_secctx(struct audit_buffer *ab, u32 secid)
// /*AFLA*/ { }
// /*AFLA*/ static inline int audit_log_task_context(struct audit_buffer *ab)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_log_task_info(struct audit_buffer *ab,
// /*AFLA*/ 				       struct task_struct *tsk)
// /*AFLA*/ { }
// /*AFLA*/ #define audit_enabled 0
// /*AFLA*/ #endif /* CONFIG_AUDIT */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_AUDIT_COMPAT_GENERIC
// /*AFLA*/ #define audit_is_compat(arch)  (!((arch) & __AUDIT_ARCH_64BIT))
// /*AFLA*/ #else
// /*AFLA*/ #define audit_is_compat(arch)  false
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_AUDITSYSCALL
// /*AFLA*/ #include <asm/syscall.h> /* for syscall_get_arch() */
// /*AFLA*/ 
// /*AFLA*/ /* These are defined in auditsc.c */
// /*AFLA*/ 				/* Public API */
// /*AFLA*/ extern int  audit_alloc(struct task_struct *task);
// /*AFLA*/ extern void __audit_free(struct task_struct *task);
// /*AFLA*/ extern void __audit_syscall_entry(int major, unsigned long a0, unsigned long a1,
// /*AFLA*/ 				  unsigned long a2, unsigned long a3);
// /*AFLA*/ extern void __audit_syscall_exit(int ret_success, long ret_value);
// /*AFLA*/ extern struct filename *__audit_reusename(const __user char *uptr);
// /*AFLA*/ extern void __audit_getname(struct filename *name);
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_INODE_PARENT	1	/* dentry represents the parent */
// /*AFLA*/ #define AUDIT_INODE_HIDDEN	2	/* audit record should be hidden */
// /*AFLA*/ extern void __audit_inode(struct filename *name, const struct dentry *dentry,
// /*AFLA*/ 				unsigned int flags);
// /*AFLA*/ extern void __audit_file(const struct file *);
// /*AFLA*/ extern void __audit_inode_child(struct inode *parent,
// /*AFLA*/ 				const struct dentry *dentry,
// /*AFLA*/ 				const unsigned char type);
// /*AFLA*/ extern void __audit_seccomp(unsigned long syscall, long signr, int code);
// /*AFLA*/ extern void __audit_ptrace(struct task_struct *t);
// /*AFLA*/ 
// /*AFLA*/ static inline bool audit_dummy_context(void)
// /*AFLA*/ {
// /*AFLA*/ 	void *p = current->audit_context;
// /*AFLA*/ 	return !p || *(int *)p;
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_free(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(task->audit_context))
// /*AFLA*/ 		__audit_free(task);
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_syscall_entry(int major, unsigned long a0,
// /*AFLA*/ 				       unsigned long a1, unsigned long a2,
// /*AFLA*/ 				       unsigned long a3)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(current->audit_context))
// /*AFLA*/ 		__audit_syscall_entry(major, a0, a1, a2, a3);
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_syscall_exit(void *pt_regs)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(current->audit_context)) {
// /*AFLA*/ 		int success = is_syscall_success(pt_regs);
// /*AFLA*/ 		long return_code = regs_return_value(pt_regs);
// /*AFLA*/ 
// /*AFLA*/ 		__audit_syscall_exit(success, return_code);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ static inline struct filename *audit_reusename(const __user char *name)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		return __audit_reusename(name);
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_getname(struct filename *name)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		__audit_getname(name);
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_inode(struct filename *name,
// /*AFLA*/ 				const struct dentry *dentry,
// /*AFLA*/ 				unsigned int parent) {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context())) {
// /*AFLA*/ 		unsigned int flags = 0;
// /*AFLA*/ 		if (parent)
// /*AFLA*/ 			flags |= AUDIT_INODE_PARENT;
// /*AFLA*/ 		__audit_inode(name, dentry, flags);
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_file(struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		__audit_file(file);
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_inode_parent_hidden(struct filename *name,
// /*AFLA*/ 						const struct dentry *dentry)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		__audit_inode(name, dentry,
// /*AFLA*/ 				AUDIT_INODE_PARENT | AUDIT_INODE_HIDDEN);
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_inode_child(struct inode *parent,
// /*AFLA*/ 				     const struct dentry *dentry,
// /*AFLA*/ 				     const unsigned char type) {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		__audit_inode_child(parent, dentry, type);
// /*AFLA*/ }
// /*AFLA*/ void audit_core_dumps(long signr);
// /*AFLA*/ 
// /*AFLA*/ static inline void audit_seccomp(unsigned long syscall, long signr, int code)
// /*AFLA*/ {
// /*AFLA*/ 	if (!audit_enabled)
// /*AFLA*/ 		return;
// /*AFLA*/ 
// /*AFLA*/ 	/* Force a record to be reported if a signal was delivered. */
// /*AFLA*/ 	if (signr || unlikely(!audit_dummy_context()))
// /*AFLA*/ 		__audit_seccomp(syscall, signr, code);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void audit_ptrace(struct task_struct *t)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		__audit_ptrace(t);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 				/* Private API (for audit.c only) */
// /*AFLA*/ extern unsigned int audit_serial(void);
// /*AFLA*/ extern int auditsc_get_stamp(struct audit_context *ctx,
// /*AFLA*/ 			      struct timespec *t, unsigned int *serial);
// /*AFLA*/ extern int audit_set_loginuid(kuid_t loginuid);
// /*AFLA*/ 
// /*AFLA*/ static inline kuid_t audit_get_loginuid(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return tsk->loginuid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int audit_get_sessionid(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return tsk->sessionid;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void __audit_ipc_obj(struct kern_ipc_perm *ipcp);
// /*AFLA*/ extern void __audit_ipc_set_perm(unsigned long qbytes, uid_t uid, gid_t gid, umode_t mode);
// /*AFLA*/ extern void __audit_bprm(struct linux_binprm *bprm);
// /*AFLA*/ extern int __audit_socketcall(int nargs, unsigned long *args);
// /*AFLA*/ extern int __audit_sockaddr(int len, void *addr);
// /*AFLA*/ extern void __audit_fd_pair(int fd1, int fd2);
// /*AFLA*/ extern void __audit_mq_open(int oflag, umode_t mode, struct mq_attr *attr);
// /*AFLA*/ extern void __audit_mq_sendrecv(mqd_t mqdes, size_t msg_len, unsigned int msg_prio, const struct timespec *abs_timeout);
// /*AFLA*/ extern void __audit_mq_notify(mqd_t mqdes, const struct sigevent *notification);
// /*AFLA*/ extern void __audit_mq_getsetattr(mqd_t mqdes, struct mq_attr *mqstat);
// /*AFLA*/ extern int __audit_log_bprm_fcaps(struct linux_binprm *bprm,
// /*AFLA*/ 				  const struct cred *new,
// /*AFLA*/ 				  const struct cred *old);
// /*AFLA*/ extern void __audit_log_capset(const struct cred *new, const struct cred *old);
// /*AFLA*/ extern void __audit_mmap_fd(int fd, int flags);
// /*AFLA*/ 
// /*AFLA*/ static inline void audit_ipc_obj(struct kern_ipc_perm *ipcp)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		__audit_ipc_obj(ipcp);
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_fd_pair(int fd1, int fd2)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		__audit_fd_pair(fd1, fd2);
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_ipc_set_perm(unsigned long qbytes, uid_t uid, gid_t gid, umode_t mode)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		__audit_ipc_set_perm(qbytes, uid, gid, mode);
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_bprm(struct linux_binprm *bprm)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		__audit_bprm(bprm);
// /*AFLA*/ }
// /*AFLA*/ static inline int audit_socketcall(int nargs, unsigned long *args)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		return __audit_socketcall(nargs, args);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int audit_sockaddr(int len, void *addr)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		return __audit_sockaddr(len, addr);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_mq_open(int oflag, umode_t mode, struct mq_attr *attr)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		__audit_mq_open(oflag, mode, attr);
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_mq_sendrecv(mqd_t mqdes, size_t msg_len, unsigned int msg_prio, const struct timespec *abs_timeout)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		__audit_mq_sendrecv(mqdes, msg_len, msg_prio, abs_timeout);
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_mq_notify(mqd_t mqdes, const struct sigevent *notification)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		__audit_mq_notify(mqdes, notification);
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_mq_getsetattr(mqd_t mqdes, struct mq_attr *mqstat)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		__audit_mq_getsetattr(mqdes, mqstat);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int audit_log_bprm_fcaps(struct linux_binprm *bprm,
// /*AFLA*/ 				       const struct cred *new,
// /*AFLA*/ 				       const struct cred *old)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		return __audit_log_bprm_fcaps(bprm, new, old);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void audit_log_capset(const struct cred *new,
// /*AFLA*/ 				   const struct cred *old)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		__audit_log_capset(new, old);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void audit_mmap_fd(int fd, int flags)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(!audit_dummy_context()))
// /*AFLA*/ 		__audit_mmap_fd(fd, flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int audit_n_rules;
// /*AFLA*/ extern int audit_signals;
// /*AFLA*/ #else /* CONFIG_AUDITSYSCALL */
// /*AFLA*/ static inline int audit_alloc(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_free(struct task_struct *task)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_syscall_entry(int major, unsigned long a0,
// /*AFLA*/ 				       unsigned long a1, unsigned long a2,
// /*AFLA*/ 				       unsigned long a3)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_syscall_exit(void *pt_regs)
// /*AFLA*/ { }
// /*AFLA*/ static inline bool audit_dummy_context(void)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ static inline struct filename *audit_reusename(const __user char *name)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_getname(struct filename *name)
// /*AFLA*/ { }
// /*AFLA*/ static inline void __audit_inode(struct filename *name,
// /*AFLA*/ 					const struct dentry *dentry,
// /*AFLA*/ 					unsigned int flags)
// /*AFLA*/ { }
// /*AFLA*/ static inline void __audit_inode_child(struct inode *parent,
// /*AFLA*/ 					const struct dentry *dentry,
// /*AFLA*/ 					const unsigned char type)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_inode(struct filename *name,
// /*AFLA*/ 				const struct dentry *dentry,
// /*AFLA*/ 				unsigned int parent)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_file(struct file *file)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_inode_parent_hidden(struct filename *name,
// /*AFLA*/ 				const struct dentry *dentry)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_inode_child(struct inode *parent,
// /*AFLA*/ 				     const struct dentry *dentry,
// /*AFLA*/ 				     const unsigned char type)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_core_dumps(long signr)
// /*AFLA*/ { }
// /*AFLA*/ static inline void __audit_seccomp(unsigned long syscall, long signr, int code)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_seccomp(unsigned long syscall, long signr, int code)
// /*AFLA*/ { }
// /*AFLA*/ static inline int auditsc_get_stamp(struct audit_context *ctx,
// /*AFLA*/ 			      struct timespec *t, unsigned int *serial)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline kuid_t audit_get_loginuid(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return INVALID_UID;
// /*AFLA*/ }
// /*AFLA*/ static inline unsigned int audit_get_sessionid(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return -1;
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_ipc_obj(struct kern_ipc_perm *ipcp)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_ipc_set_perm(unsigned long qbytes, uid_t uid,
// /*AFLA*/ 					gid_t gid, umode_t mode)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_bprm(struct linux_binprm *bprm)
// /*AFLA*/ { }
// /*AFLA*/ static inline int audit_socketcall(int nargs, unsigned long *args)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_fd_pair(int fd1, int fd2)
// /*AFLA*/ { }
// /*AFLA*/ static inline int audit_sockaddr(int len, void *addr)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_mq_open(int oflag, umode_t mode, struct mq_attr *attr)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_mq_sendrecv(mqd_t mqdes, size_t msg_len,
// /*AFLA*/ 				     unsigned int msg_prio,
// /*AFLA*/ 				     const struct timespec *abs_timeout)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_mq_notify(mqd_t mqdes,
// /*AFLA*/ 				   const struct sigevent *notification)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_mq_getsetattr(mqd_t mqdes, struct mq_attr *mqstat)
// /*AFLA*/ { }
// /*AFLA*/ static inline int audit_log_bprm_fcaps(struct linux_binprm *bprm,
// /*AFLA*/ 				       const struct cred *new,
// /*AFLA*/ 				       const struct cred *old)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void audit_log_capset(const struct cred *new,
// /*AFLA*/ 				    const struct cred *old)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_mmap_fd(int fd, int flags)
// /*AFLA*/ { }
// /*AFLA*/ static inline void audit_ptrace(struct task_struct *t)
// /*AFLA*/ { }
// /*AFLA*/ #define audit_n_rules 0
// /*AFLA*/ #define audit_signals 0
// /*AFLA*/ #endif /* CONFIG_AUDITSYSCALL */
// /*AFLA*/ 
// /*AFLA*/ static inline bool audit_loginuid_set(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return uid_valid(audit_get_loginuid(tsk));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void audit_log_string(struct audit_buffer *ab, const char *buf)
// /*AFLA*/ {
// /*AFLA*/ 	audit_log_n_string(ab, buf, strlen(buf));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
