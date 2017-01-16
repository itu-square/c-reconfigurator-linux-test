// /*AFLA*/ /* Credentials management - see Documentation/security/credentials.txt
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2008 Red Hat, Inc. All Rights Reserved.
// /*AFLA*/  * Written by David Howells (dhowells@redhat.com)
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or
// /*AFLA*/  * modify it under the terms of the GNU General Public Licence
// /*AFLA*/  * as published by the Free Software Foundation; either version
// /*AFLA*/  * 2 of the Licence, or (at your option) any later version.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_CRED_H
// /*AFLA*/ #define _LINUX_CRED_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/capability.h>
// /*AFLA*/ #include <linux/init.h>
// /*AFLA*/ #include <linux/key.h>
// /*AFLA*/ #include <linux/selinux.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/uidgid.h>
// /*AFLA*/ 
// /*AFLA*/ struct user_struct;
// /*AFLA*/ struct cred;
// /*AFLA*/ struct inode;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * COW Supplementary groups list
// /*AFLA*/  */
// /*AFLA*/ struct group_info {
// /*AFLA*/ 	atomic_t	usage;
// /*AFLA*/ 	int		ngroups;
// /*AFLA*/ 	kgid_t		gid[0];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * get_group_info - Get a reference to a group info structure
// /*AFLA*/  * @group_info: The group info to reference
// /*AFLA*/  *
// /*AFLA*/  * This gets a reference to a set of supplementary groups.
// /*AFLA*/  *
// /*AFLA*/  * If the caller is accessing a task's credentials, they must hold the RCU read
// /*AFLA*/  * lock when reading.
// /*AFLA*/  */
// /*AFLA*/ static inline struct group_info *get_group_info(struct group_info *gi)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_inc(&gi->usage);
// /*AFLA*/ 	return gi;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * put_group_info - Release a reference to a group info structure
// /*AFLA*/  * @group_info: The group info to release
// /*AFLA*/  */
// /*AFLA*/ #define put_group_info(group_info)			\
// /*AFLA*/ do {							\
// /*AFLA*/ 	if (atomic_dec_and_test(&(group_info)->usage))	\
// /*AFLA*/ 		groups_free(group_info);		\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ extern struct group_info init_groups;
// /*AFLA*/ #ifdef CONFIG_MULTIUSER
// /*AFLA*/ extern struct group_info *groups_alloc(int);
// /*AFLA*/ extern void groups_free(struct group_info *);
// /*AFLA*/ 
// /*AFLA*/ extern int in_group_p(kgid_t);
// /*AFLA*/ extern int in_egroup_p(kgid_t);
// /*AFLA*/ #else
// /*AFLA*/ static inline void groups_free(struct group_info *group_info)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int in_group_p(kgid_t grp)
// /*AFLA*/ {
// /*AFLA*/         return 1;
// /*AFLA*/ }
// /*AFLA*/ static inline int in_egroup_p(kgid_t grp)
// /*AFLA*/ {
// /*AFLA*/         return 1;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ extern int set_current_groups(struct group_info *);
// /*AFLA*/ extern void set_groups(struct cred *, struct group_info *);
// /*AFLA*/ extern int groups_search(const struct group_info *, kgid_t);
// /*AFLA*/ extern bool may_setgroups(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The security context of a task
// /*AFLA*/  *
// /*AFLA*/  * The parts of the context break down into two categories:
// /*AFLA*/  *
// /*AFLA*/  *  (1) The objective context of a task.  These parts are used when some other
// /*AFLA*/  *	task is attempting to affect this one.
// /*AFLA*/  *
// /*AFLA*/  *  (2) The subjective context.  These details are used when the task is acting
// /*AFLA*/  *	upon another object, be that a file, a task, a key or whatever.
// /*AFLA*/  *
// /*AFLA*/  * Note that some members of this structure belong to both categories - the
// /*AFLA*/  * LSM security pointer for instance.
// /*AFLA*/  *
// /*AFLA*/  * A task has two security pointers.  task->real_cred points to the objective
// /*AFLA*/  * context that defines that task's actual details.  The objective part of this
// /*AFLA*/  * context is used whenever that task is acted upon.
// /*AFLA*/  *
// /*AFLA*/  * task->cred points to the subjective context that defines the details of how
// /*AFLA*/  * that task is going to act upon another object.  This may be overridden
// /*AFLA*/  * temporarily to point to another security context, but normally points to the
// /*AFLA*/  * same context as task->real_cred.
// /*AFLA*/  */
// /*AFLA*/ struct cred {
// /*AFLA*/ 	atomic_t	usage;
// /*AFLA*/ #ifdef CONFIG_DEBUG_CREDENTIALS
// /*AFLA*/ 	atomic_t	subscribers;	/* number of processes subscribed */
// /*AFLA*/ 	void		*put_addr;
// /*AFLA*/ 	unsigned	magic;
// /*AFLA*/ #define CRED_MAGIC	0x43736564
// /*AFLA*/ #define CRED_MAGIC_DEAD	0x44656144
// /*AFLA*/ #endif
// /*AFLA*/ 	kuid_t		uid;		/* real UID of the task */
// /*AFLA*/ 	kgid_t		gid;		/* real GID of the task */
// /*AFLA*/ 	kuid_t		suid;		/* saved UID of the task */
// /*AFLA*/ 	kgid_t		sgid;		/* saved GID of the task */
// /*AFLA*/ 	kuid_t		euid;		/* effective UID of the task */
// /*AFLA*/ 	kgid_t		egid;		/* effective GID of the task */
// /*AFLA*/ 	kuid_t		fsuid;		/* UID for VFS ops */
// /*AFLA*/ 	kgid_t		fsgid;		/* GID for VFS ops */
// /*AFLA*/ 	unsigned	securebits;	/* SUID-less security management */
// /*AFLA*/ 	kernel_cap_t	cap_inheritable; /* caps our children can inherit */
// /*AFLA*/ 	kernel_cap_t	cap_permitted;	/* caps we're permitted */
// /*AFLA*/ 	kernel_cap_t	cap_effective;	/* caps we can actually use */
// /*AFLA*/ 	kernel_cap_t	cap_bset;	/* capability bounding set */
// /*AFLA*/ 	kernel_cap_t	cap_ambient;	/* Ambient capability set */
// /*AFLA*/ #ifdef CONFIG_KEYS
// /*AFLA*/ 	unsigned char	jit_keyring;	/* default keyring to attach requested
// /*AFLA*/ 					 * keys to */
// /*AFLA*/ 	struct key __rcu *session_keyring; /* keyring inherited over fork */
// /*AFLA*/ 	struct key	*process_keyring; /* keyring private to this process */
// /*AFLA*/ 	struct key	*thread_keyring; /* keyring private to this thread */
// /*AFLA*/ 	struct key	*request_key_auth; /* assumed request_key authority */
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_SECURITY
// /*AFLA*/ 	void		*security;	/* subjective LSM security */
// /*AFLA*/ #endif
// /*AFLA*/ 	struct user_struct *user;	/* real user ID subscription */
// /*AFLA*/ 	struct user_namespace *user_ns; /* user_ns the caps and keyrings are relative to. */
// /*AFLA*/ 	struct group_info *group_info;	/* supplementary groups for euid/fsgid */
// /*AFLA*/ 	struct rcu_head	rcu;		/* RCU deletion hook */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern void __put_cred(struct cred *);
// /*AFLA*/ extern void exit_creds(struct task_struct *);
// /*AFLA*/ extern int copy_creds(struct task_struct *, unsigned long);
// /*AFLA*/ extern const struct cred *get_task_cred(struct task_struct *);
// /*AFLA*/ extern struct cred *cred_alloc_blank(void);
// /*AFLA*/ extern struct cred *prepare_creds(void);
// /*AFLA*/ extern struct cred *prepare_exec_creds(void);
// /*AFLA*/ extern int commit_creds(struct cred *);
// /*AFLA*/ extern void abort_creds(struct cred *);
// /*AFLA*/ extern const struct cred *override_creds(const struct cred *);
// /*AFLA*/ extern void revert_creds(const struct cred *);
// /*AFLA*/ extern struct cred *prepare_kernel_cred(struct task_struct *);
// /*AFLA*/ extern int change_create_files_as(struct cred *, struct inode *);
// /*AFLA*/ extern int set_security_override(struct cred *, u32);
// /*AFLA*/ extern int set_security_override_from_ctx(struct cred *, const char *);
// /*AFLA*/ extern int set_create_files_as(struct cred *, struct inode *);
// /*AFLA*/ extern void __init cred_init(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * check for validity of credentials
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_DEBUG_CREDENTIALS
// /*AFLA*/ extern void __invalid_creds(const struct cred *, const char *, unsigned);
// /*AFLA*/ extern void __validate_process_creds(struct task_struct *,
// /*AFLA*/ 				     const char *, unsigned);
// /*AFLA*/ 
// /*AFLA*/ extern bool creds_are_invalid(const struct cred *cred);
// /*AFLA*/ 
// /*AFLA*/ static inline void __validate_creds(const struct cred *cred,
// /*AFLA*/ 				    const char *file, unsigned line)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(creds_are_invalid(cred)))
// /*AFLA*/ 		__invalid_creds(cred, file, line);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define validate_creds(cred)				\
// /*AFLA*/ do {							\
// /*AFLA*/ 	__validate_creds((cred), __FILE__, __LINE__);	\
// /*AFLA*/ } while(0)
// /*AFLA*/ 
// /*AFLA*/ #define validate_process_creds()				\
// /*AFLA*/ do {								\
// /*AFLA*/ 	__validate_process_creds(current, __FILE__, __LINE__);	\
// /*AFLA*/ } while(0)
// /*AFLA*/ 
// /*AFLA*/ extern void validate_creds_for_do_exit(struct task_struct *);
// /*AFLA*/ #else
// /*AFLA*/ static inline void validate_creds(const struct cred *cred)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void validate_creds_for_do_exit(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void validate_process_creds(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline bool cap_ambient_invariant_ok(const struct cred *cred)
// /*AFLA*/ {
// /*AFLA*/ 	return cap_issubset(cred->cap_ambient,
// /*AFLA*/ 			    cap_intersect(cred->cap_permitted,
// /*AFLA*/ 					  cred->cap_inheritable));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * get_new_cred - Get a reference on a new set of credentials
// /*AFLA*/  * @cred: The new credentials to reference
// /*AFLA*/  *
// /*AFLA*/  * Get a reference on the specified set of new credentials.  The caller must
// /*AFLA*/  * release the reference.
// /*AFLA*/  */
// /*AFLA*/ static inline struct cred *get_new_cred(struct cred *cred)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_inc(&cred->usage);
// /*AFLA*/ 	return cred;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * get_cred - Get a reference on a set of credentials
// /*AFLA*/  * @cred: The credentials to reference
// /*AFLA*/  *
// /*AFLA*/  * Get a reference on the specified set of credentials.  The caller must
// /*AFLA*/  * release the reference.
// /*AFLA*/  *
// /*AFLA*/  * This is used to deal with a committed set of credentials.  Although the
// /*AFLA*/  * pointer is const, this will temporarily discard the const and increment the
// /*AFLA*/  * usage count.  The purpose of this is to attempt to catch at compile time the
// /*AFLA*/  * accidental alteration of a set of credentials that should be considered
// /*AFLA*/  * immutable.
// /*AFLA*/  */
// /*AFLA*/ static inline const struct cred *get_cred(const struct cred *cred)
// /*AFLA*/ {
// /*AFLA*/ 	struct cred *nonconst_cred = (struct cred *) cred;
// /*AFLA*/ 	validate_creds(cred);
// /*AFLA*/ 	return get_new_cred(nonconst_cred);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * put_cred - Release a reference to a set of credentials
// /*AFLA*/  * @cred: The credentials to release
// /*AFLA*/  *
// /*AFLA*/  * Release a reference to a set of credentials, deleting them when the last ref
// /*AFLA*/  * is released.
// /*AFLA*/  *
// /*AFLA*/  * This takes a const pointer to a set of credentials because the credentials
// /*AFLA*/  * on task_struct are attached by const pointers to prevent accidental
// /*AFLA*/  * alteration of otherwise immutable credential sets.
// /*AFLA*/  */
// /*AFLA*/ static inline void put_cred(const struct cred *_cred)
// /*AFLA*/ {
// /*AFLA*/ 	struct cred *cred = (struct cred *) _cred;
// /*AFLA*/ 
// /*AFLA*/ 	validate_creds(cred);
// /*AFLA*/ 	if (atomic_dec_and_test(&(cred)->usage))
// /*AFLA*/ 		__put_cred(cred);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * current_cred - Access the current task's subjective credentials
// /*AFLA*/  *
// /*AFLA*/  * Access the subjective credentials of the current task.  RCU-safe,
// /*AFLA*/  * since nobody else can modify it.
// /*AFLA*/  */
// /*AFLA*/ #define current_cred() \
// /*AFLA*/ 	rcu_dereference_protected(current->cred, 1)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * current_real_cred - Access the current task's objective credentials
// /*AFLA*/  *
// /*AFLA*/  * Access the objective credentials of the current task.  RCU-safe,
// /*AFLA*/  * since nobody else can modify it.
// /*AFLA*/  */
// /*AFLA*/ #define current_real_cred() \
// /*AFLA*/ 	rcu_dereference_protected(current->real_cred, 1)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * __task_cred - Access a task's objective credentials
// /*AFLA*/  * @task: The task to query
// /*AFLA*/  *
// /*AFLA*/  * Access the objective credentials of a task.  The caller must hold the RCU
// /*AFLA*/  * readlock.
// /*AFLA*/  *
// /*AFLA*/  * The result of this function should not be passed directly to get_cred();
// /*AFLA*/  * rather get_task_cred() should be used instead.
// /*AFLA*/  */
// /*AFLA*/ #define __task_cred(task)	\
// /*AFLA*/ 	rcu_dereference((task)->real_cred)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * get_current_cred - Get the current task's subjective credentials
// /*AFLA*/  *
// /*AFLA*/  * Get the subjective credentials of the current task, pinning them so that
// /*AFLA*/  * they can't go away.  Accessing the current task's credentials directly is
// /*AFLA*/  * not permitted.
// /*AFLA*/  */
// /*AFLA*/ #define get_current_cred()				\
// /*AFLA*/ 	(get_cred(current_cred()))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * get_current_user - Get the current task's user_struct
// /*AFLA*/  *
// /*AFLA*/  * Get the user record of the current task, pinning it so that it can't go
// /*AFLA*/  * away.
// /*AFLA*/  */
// /*AFLA*/ #define get_current_user()				\
// /*AFLA*/ ({							\
// /*AFLA*/ 	struct user_struct *__u;			\
// /*AFLA*/ 	const struct cred *__cred;			\
// /*AFLA*/ 	__cred = current_cred();			\
// /*AFLA*/ 	__u = get_uid(__cred->user);			\
// /*AFLA*/ 	__u;						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * get_current_groups - Get the current task's supplementary group list
// /*AFLA*/  *
// /*AFLA*/  * Get the supplementary group list of the current task, pinning it so that it
// /*AFLA*/  * can't go away.
// /*AFLA*/  */
// /*AFLA*/ #define get_current_groups()				\
// /*AFLA*/ ({							\
// /*AFLA*/ 	struct group_info *__groups;			\
// /*AFLA*/ 	const struct cred *__cred;			\
// /*AFLA*/ 	__cred = current_cred();			\
// /*AFLA*/ 	__groups = get_group_info(__cred->group_info);	\
// /*AFLA*/ 	__groups;					\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define task_cred_xxx(task, xxx)			\
// /*AFLA*/ ({							\
// /*AFLA*/ 	__typeof__(((struct cred *)NULL)->xxx) ___val;	\
// /*AFLA*/ 	rcu_read_lock();				\
// /*AFLA*/ 	___val = __task_cred((task))->xxx;		\
// /*AFLA*/ 	rcu_read_unlock();				\
// /*AFLA*/ 	___val;						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define task_uid(task)		(task_cred_xxx((task), uid))
// /*AFLA*/ #define task_euid(task)		(task_cred_xxx((task), euid))
// /*AFLA*/ 
// /*AFLA*/ #define current_cred_xxx(xxx)			\
// /*AFLA*/ ({						\
// /*AFLA*/ 	current_cred()->xxx;			\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define current_uid()		(current_cred_xxx(uid))
// /*AFLA*/ #define current_gid()		(current_cred_xxx(gid))
// /*AFLA*/ #define current_euid()		(current_cred_xxx(euid))
// /*AFLA*/ #define current_egid()		(current_cred_xxx(egid))
// /*AFLA*/ #define current_suid()		(current_cred_xxx(suid))
// /*AFLA*/ #define current_sgid()		(current_cred_xxx(sgid))
// /*AFLA*/ #define current_fsuid() 	(current_cred_xxx(fsuid))
// /*AFLA*/ #define current_fsgid() 	(current_cred_xxx(fsgid))
// /*AFLA*/ #define current_cap()		(current_cred_xxx(cap_effective))
// /*AFLA*/ #define current_user()		(current_cred_xxx(user))
// /*AFLA*/ #define current_security()	(current_cred_xxx(security))
// /*AFLA*/ 
// /*AFLA*/ extern struct user_namespace init_user_ns;
// /*AFLA*/ #ifdef CONFIG_USER_NS
// /*AFLA*/ #define current_user_ns()	(current_cred_xxx(user_ns))
// /*AFLA*/ #else
// /*AFLA*/ static inline struct user_namespace *current_user_ns(void)
// /*AFLA*/ {
// /*AFLA*/ 	return &init_user_ns;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define current_uid_gid(_uid, _gid)		\
// /*AFLA*/ do {						\
// /*AFLA*/ 	const struct cred *__cred;		\
// /*AFLA*/ 	__cred = current_cred();		\
// /*AFLA*/ 	*(_uid) = __cred->uid;			\
// /*AFLA*/ 	*(_gid) = __cred->gid;			\
// /*AFLA*/ } while(0)
// /*AFLA*/ 
// /*AFLA*/ #define current_euid_egid(_euid, _egid)		\
// /*AFLA*/ do {						\
// /*AFLA*/ 	const struct cred *__cred;		\
// /*AFLA*/ 	__cred = current_cred();		\
// /*AFLA*/ 	*(_euid) = __cred->euid;		\
// /*AFLA*/ 	*(_egid) = __cred->egid;		\
// /*AFLA*/ } while(0)
// /*AFLA*/ 
// /*AFLA*/ #define current_fsuid_fsgid(_fsuid, _fsgid)	\
// /*AFLA*/ do {						\
// /*AFLA*/ 	const struct cred *__cred;		\
// /*AFLA*/ 	__cred = current_cred();		\
// /*AFLA*/ 	*(_fsuid) = __cred->fsuid;		\
// /*AFLA*/ 	*(_fsgid) = __cred->fsgid;		\
// /*AFLA*/ } while(0)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_CRED_H */
