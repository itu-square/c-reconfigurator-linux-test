// /*AFLA*/ /*
// /*AFLA*/  * syscalls.h - Linux syscall interfaces (non-arch-specific)
// /*AFLA*/  *
// /*AFLA*/  * Copyright (c) 2004 Randy Dunlap
// /*AFLA*/  * Copyright (c) 2004 Open Source Development Labs
// /*AFLA*/  *
// /*AFLA*/  * This file is released under the GPLv2.
// /*AFLA*/  * See the file COPYING for more details.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_SYSCALLS_H
// /*AFLA*/ #define _LINUX_SYSCALLS_H
// /*AFLA*/ 
// /*AFLA*/ struct epoll_event;
// /*AFLA*/ struct iattr;
// /*AFLA*/ struct inode;
// /*AFLA*/ struct iocb;
// /*AFLA*/ struct io_event;
// /*AFLA*/ struct iovec;
// /*AFLA*/ struct itimerspec;
// /*AFLA*/ struct itimerval;
// /*AFLA*/ struct kexec_segment;
// /*AFLA*/ struct linux_dirent;
// /*AFLA*/ struct linux_dirent64;
// /*AFLA*/ struct list_head;
// /*AFLA*/ struct mmap_arg_struct;
// /*AFLA*/ struct msgbuf;
// /*AFLA*/ struct user_msghdr;
// /*AFLA*/ struct mmsghdr;
// /*AFLA*/ struct msqid_ds;
// /*AFLA*/ struct new_utsname;
// /*AFLA*/ struct nfsctl_arg;
// /*AFLA*/ struct __old_kernel_stat;
// /*AFLA*/ struct oldold_utsname;
// /*AFLA*/ struct old_utsname;
// /*AFLA*/ struct pollfd;
// /*AFLA*/ struct rlimit;
// /*AFLA*/ struct rlimit64;
// /*AFLA*/ struct rusage;
// /*AFLA*/ struct sched_param;
// /*AFLA*/ struct sched_attr;
// /*AFLA*/ struct sel_arg_struct;
// /*AFLA*/ struct semaphore;
// /*AFLA*/ struct sembuf;
// /*AFLA*/ struct shmid_ds;
// /*AFLA*/ struct sockaddr;
// /*AFLA*/ struct stat;
// /*AFLA*/ struct stat64;
// /*AFLA*/ struct statfs;
// /*AFLA*/ struct statfs64;
// /*AFLA*/ struct __sysctl_args;
// /*AFLA*/ struct sysinfo;
// /*AFLA*/ struct timespec;
// /*AFLA*/ struct timeval;
// /*AFLA*/ struct timex;
// /*AFLA*/ struct timezone;
// /*AFLA*/ struct tms;
// /*AFLA*/ struct utimbuf;
// /*AFLA*/ struct mq_attr;
// /*AFLA*/ struct compat_stat;
// /*AFLA*/ struct compat_timeval;
// /*AFLA*/ struct robust_list_head;
// /*AFLA*/ struct getcpu_cache;
// /*AFLA*/ struct old_linux_dirent;
// /*AFLA*/ struct perf_event_attr;
// /*AFLA*/ struct file_handle;
// /*AFLA*/ struct sigaltstack;
// /*AFLA*/ union bpf_attr;
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/aio_abi.h>
// /*AFLA*/ #include <linux/capability.h>
// /*AFLA*/ #include <linux/signal.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ #include <linux/sem.h>
// /*AFLA*/ #include <asm/siginfo.h>
// /*AFLA*/ #include <linux/unistd.h>
// /*AFLA*/ #include <linux/quota.h>
// /*AFLA*/ #include <linux/key.h>
// /*AFLA*/ #include <trace/syscall.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * __MAP - apply a macro to syscall arguments
// /*AFLA*/  * __MAP(n, m, t1, a1, t2, a2, ..., tn, an) will expand to
// /*AFLA*/  *    m(t1, a1), m(t2, a2), ..., m(tn, an)
// /*AFLA*/  * The first argument must be equal to the amount of type/name
// /*AFLA*/  * pairs given.  Note that this list of pairs (i.e. the arguments
// /*AFLA*/  * of __MAP starting at the third one) is in the same format as
// /*AFLA*/  * for SYSCALL_DEFINE<n>/COMPAT_SYSCALL_DEFINE<n>
// /*AFLA*/  */
// /*AFLA*/ #define __MAP0(m,...)
// /*AFLA*/ #define __MAP1(m,t,a) m(t,a)
// /*AFLA*/ #define __MAP2(m,t,a,...) m(t,a), __MAP1(m,__VA_ARGS__)
// /*AFLA*/ #define __MAP3(m,t,a,...) m(t,a), __MAP2(m,__VA_ARGS__)
// /*AFLA*/ #define __MAP4(m,t,a,...) m(t,a), __MAP3(m,__VA_ARGS__)
// /*AFLA*/ #define __MAP5(m,t,a,...) m(t,a), __MAP4(m,__VA_ARGS__)
// /*AFLA*/ #define __MAP6(m,t,a,...) m(t,a), __MAP5(m,__VA_ARGS__)
// /*AFLA*/ #define __MAP(n,...) __MAP##n(__VA_ARGS__)
// /*AFLA*/ 
// /*AFLA*/ #define __SC_DECL(t, a)	t a
// /*AFLA*/ #define __TYPE_IS_L(t)	(__same_type((t)0, 0L))
// /*AFLA*/ #define __TYPE_IS_UL(t)	(__same_type((t)0, 0UL))
// /*AFLA*/ #define __TYPE_IS_LL(t) (__same_type((t)0, 0LL) || __same_type((t)0, 0ULL))
// /*AFLA*/ #define __SC_LONG(t, a) __typeof(__builtin_choose_expr(__TYPE_IS_LL(t), 0LL, 0L)) a
// /*AFLA*/ #define __SC_CAST(t, a)	(t) a
// /*AFLA*/ #define __SC_ARGS(t, a)	a
// /*AFLA*/ #define __SC_TEST(t, a) (void)BUILD_BUG_ON_ZERO(!__TYPE_IS_LL(t) && sizeof(t) > sizeof(long))
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FTRACE_SYSCALLS
// /*AFLA*/ #define __SC_STR_ADECL(t, a)	#a
// /*AFLA*/ #define __SC_STR_TDECL(t, a)	#t
// /*AFLA*/ 
// /*AFLA*/ extern struct trace_event_class event_class_syscall_enter;
// /*AFLA*/ extern struct trace_event_class event_class_syscall_exit;
// /*AFLA*/ extern struct trace_event_functions enter_syscall_print_funcs;
// /*AFLA*/ extern struct trace_event_functions exit_syscall_print_funcs;
// /*AFLA*/ 
// /*AFLA*/ #define SYSCALL_TRACE_ENTER_EVENT(sname)				\
// /*AFLA*/ 	static struct syscall_metadata __syscall_meta_##sname;		\
// /*AFLA*/ 	static struct trace_event_call __used				\
// /*AFLA*/ 	  event_enter_##sname = {					\
// /*AFLA*/ 		.class			= &event_class_syscall_enter,	\
// /*AFLA*/ 		{							\
// /*AFLA*/ 			.name                   = "sys_enter"#sname,	\
// /*AFLA*/ 		},							\
// /*AFLA*/ 		.event.funcs            = &enter_syscall_print_funcs,	\
// /*AFLA*/ 		.data			= (void *)&__syscall_meta_##sname,\
// /*AFLA*/ 		.flags                  = TRACE_EVENT_FL_CAP_ANY,	\
// /*AFLA*/ 	};								\
// /*AFLA*/ 	static struct trace_event_call __used				\
// /*AFLA*/ 	  __attribute__((section("_ftrace_events")))			\
// /*AFLA*/ 	 *__event_enter_##sname = &event_enter_##sname;
// /*AFLA*/ 
// /*AFLA*/ #define SYSCALL_TRACE_EXIT_EVENT(sname)					\
// /*AFLA*/ 	static struct syscall_metadata __syscall_meta_##sname;		\
// /*AFLA*/ 	static struct trace_event_call __used				\
// /*AFLA*/ 	  event_exit_##sname = {					\
// /*AFLA*/ 		.class			= &event_class_syscall_exit,	\
// /*AFLA*/ 		{							\
// /*AFLA*/ 			.name                   = "sys_exit"#sname,	\
// /*AFLA*/ 		},							\
// /*AFLA*/ 		.event.funcs		= &exit_syscall_print_funcs,	\
// /*AFLA*/ 		.data			= (void *)&__syscall_meta_##sname,\
// /*AFLA*/ 		.flags                  = TRACE_EVENT_FL_CAP_ANY,	\
// /*AFLA*/ 	};								\
// /*AFLA*/ 	static struct trace_event_call __used				\
// /*AFLA*/ 	  __attribute__((section("_ftrace_events")))			\
// /*AFLA*/ 	*__event_exit_##sname = &event_exit_##sname;
// /*AFLA*/ 
// /*AFLA*/ #define SYSCALL_METADATA(sname, nb, ...)			\
// /*AFLA*/ 	static const char *types_##sname[] = {			\
// /*AFLA*/ 		__MAP(nb,__SC_STR_TDECL,__VA_ARGS__)		\
// /*AFLA*/ 	};							\
// /*AFLA*/ 	static const char *args_##sname[] = {			\
// /*AFLA*/ 		__MAP(nb,__SC_STR_ADECL,__VA_ARGS__)		\
// /*AFLA*/ 	};							\
// /*AFLA*/ 	SYSCALL_TRACE_ENTER_EVENT(sname);			\
// /*AFLA*/ 	SYSCALL_TRACE_EXIT_EVENT(sname);			\
// /*AFLA*/ 	static struct syscall_metadata __used			\
// /*AFLA*/ 	  __syscall_meta_##sname = {				\
// /*AFLA*/ 		.name 		= "sys"#sname,			\
// /*AFLA*/ 		.syscall_nr	= -1,	/* Filled in at boot */	\
// /*AFLA*/ 		.nb_args 	= nb,				\
// /*AFLA*/ 		.types		= nb ? types_##sname : NULL,	\
// /*AFLA*/ 		.args		= nb ? args_##sname : NULL,	\
// /*AFLA*/ 		.enter_event	= &event_enter_##sname,		\
// /*AFLA*/ 		.exit_event	= &event_exit_##sname,		\
// /*AFLA*/ 		.enter_fields	= LIST_HEAD_INIT(__syscall_meta_##sname.enter_fields), \
// /*AFLA*/ 	};							\
// /*AFLA*/ 	static struct syscall_metadata __used			\
// /*AFLA*/ 	  __attribute__((section("__syscalls_metadata")))	\
// /*AFLA*/ 	 *__p_syscall_meta_##sname = &__syscall_meta_##sname;
// /*AFLA*/ #else
// /*AFLA*/ #define SYSCALL_METADATA(sname, nb, ...)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define SYSCALL_DEFINE0(sname)					\
// /*AFLA*/ 	SYSCALL_METADATA(_##sname, 0);				\
// /*AFLA*/ 	asmlinkage long sys_##sname(void)
// /*AFLA*/ 
// /*AFLA*/ #define SYSCALL_DEFINE1(name, ...) SYSCALL_DEFINEx(1, _##name, __VA_ARGS__)
// /*AFLA*/ #define SYSCALL_DEFINE2(name, ...) SYSCALL_DEFINEx(2, _##name, __VA_ARGS__)
// /*AFLA*/ #define SYSCALL_DEFINE3(name, ...) SYSCALL_DEFINEx(3, _##name, __VA_ARGS__)
// /*AFLA*/ #define SYSCALL_DEFINE4(name, ...) SYSCALL_DEFINEx(4, _##name, __VA_ARGS__)
// /*AFLA*/ #define SYSCALL_DEFINE5(name, ...) SYSCALL_DEFINEx(5, _##name, __VA_ARGS__)
// /*AFLA*/ #define SYSCALL_DEFINE6(name, ...) SYSCALL_DEFINEx(6, _##name, __VA_ARGS__)
// /*AFLA*/ 
// /*AFLA*/ #define SYSCALL_DEFINEx(x, sname, ...)				\
// /*AFLA*/ 	SYSCALL_METADATA(sname, x, __VA_ARGS__)			\
// /*AFLA*/ 	__SYSCALL_DEFINEx(x, sname, __VA_ARGS__)
// /*AFLA*/ 
// /*AFLA*/ #define __PROTECT(...) asmlinkage_protect(__VA_ARGS__)
// /*AFLA*/ #define __SYSCALL_DEFINEx(x, name, ...)					\
// /*AFLA*/ 	asmlinkage long sys##name(__MAP(x,__SC_DECL,__VA_ARGS__))	\
// /*AFLA*/ 		__attribute__((alias(__stringify(SyS##name))));		\
// /*AFLA*/ 	static inline long SYSC##name(__MAP(x,__SC_DECL,__VA_ARGS__));	\
// /*AFLA*/ 	asmlinkage long SyS##name(__MAP(x,__SC_LONG,__VA_ARGS__));	\
// /*AFLA*/ 	asmlinkage long SyS##name(__MAP(x,__SC_LONG,__VA_ARGS__))	\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		long ret = SYSC##name(__MAP(x,__SC_CAST,__VA_ARGS__));	\
// /*AFLA*/ 		__MAP(x,__SC_TEST,__VA_ARGS__);				\
// /*AFLA*/ 		__PROTECT(x, ret,__MAP(x,__SC_ARGS,__VA_ARGS__));	\
// /*AFLA*/ 		return ret;						\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	static inline long SYSC##name(__MAP(x,__SC_DECL,__VA_ARGS__))
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys32_quotactl(unsigned int cmd, const char __user *special,
// /*AFLA*/ 			       qid_t id, void __user *addr);
// /*AFLA*/ asmlinkage long sys_time(time_t __user *tloc);
// /*AFLA*/ asmlinkage long sys_stime(time_t __user *tptr);
// /*AFLA*/ asmlinkage long sys_gettimeofday(struct timeval __user *tv,
// /*AFLA*/ 				struct timezone __user *tz);
// /*AFLA*/ asmlinkage long sys_settimeofday(struct timeval __user *tv,
// /*AFLA*/ 				struct timezone __user *tz);
// /*AFLA*/ asmlinkage long sys_adjtimex(struct timex __user *txc_p);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_times(struct tms __user *tbuf);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_gettid(void);
// /*AFLA*/ asmlinkage long sys_nanosleep(struct timespec __user *rqtp, struct timespec __user *rmtp);
// /*AFLA*/ asmlinkage long sys_alarm(unsigned int seconds);
// /*AFLA*/ asmlinkage long sys_getpid(void);
// /*AFLA*/ asmlinkage long sys_getppid(void);
// /*AFLA*/ asmlinkage long sys_getuid(void);
// /*AFLA*/ asmlinkage long sys_geteuid(void);
// /*AFLA*/ asmlinkage long sys_getgid(void);
// /*AFLA*/ asmlinkage long sys_getegid(void);
// /*AFLA*/ asmlinkage long sys_getresuid(uid_t __user *ruid, uid_t __user *euid, uid_t __user *suid);
// /*AFLA*/ asmlinkage long sys_getresgid(gid_t __user *rgid, gid_t __user *egid, gid_t __user *sgid);
// /*AFLA*/ asmlinkage long sys_getpgid(pid_t pid);
// /*AFLA*/ asmlinkage long sys_getpgrp(void);
// /*AFLA*/ asmlinkage long sys_getsid(pid_t pid);
// /*AFLA*/ asmlinkage long sys_getgroups(int gidsetsize, gid_t __user *grouplist);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_setregid(gid_t rgid, gid_t egid);
// /*AFLA*/ asmlinkage long sys_setgid(gid_t gid);
// /*AFLA*/ asmlinkage long sys_setreuid(uid_t ruid, uid_t euid);
// /*AFLA*/ asmlinkage long sys_setuid(uid_t uid);
// /*AFLA*/ asmlinkage long sys_setresuid(uid_t ruid, uid_t euid, uid_t suid);
// /*AFLA*/ asmlinkage long sys_setresgid(gid_t rgid, gid_t egid, gid_t sgid);
// /*AFLA*/ asmlinkage long sys_setfsuid(uid_t uid);
// /*AFLA*/ asmlinkage long sys_setfsgid(gid_t gid);
// /*AFLA*/ asmlinkage long sys_setpgid(pid_t pid, pid_t pgid);
// /*AFLA*/ asmlinkage long sys_setsid(void);
// /*AFLA*/ asmlinkage long sys_setgroups(int gidsetsize, gid_t __user *grouplist);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_acct(const char __user *name);
// /*AFLA*/ asmlinkage long sys_capget(cap_user_header_t header,
// /*AFLA*/ 				cap_user_data_t dataptr);
// /*AFLA*/ asmlinkage long sys_capset(cap_user_header_t header,
// /*AFLA*/ 				const cap_user_data_t data);
// /*AFLA*/ asmlinkage long sys_personality(unsigned int personality);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_sigpending(old_sigset_t __user *set);
// /*AFLA*/ asmlinkage long sys_sigprocmask(int how, old_sigset_t __user *set,
// /*AFLA*/ 				old_sigset_t __user *oset);
// /*AFLA*/ asmlinkage long sys_sigaltstack(const struct sigaltstack __user *uss,
// /*AFLA*/ 				struct sigaltstack __user *uoss);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_getitimer(int which, struct itimerval __user *value);
// /*AFLA*/ asmlinkage long sys_setitimer(int which,
// /*AFLA*/ 				struct itimerval __user *value,
// /*AFLA*/ 				struct itimerval __user *ovalue);
// /*AFLA*/ asmlinkage long sys_timer_create(clockid_t which_clock,
// /*AFLA*/ 				 struct sigevent __user *timer_event_spec,
// /*AFLA*/ 				 timer_t __user * created_timer_id);
// /*AFLA*/ asmlinkage long sys_timer_gettime(timer_t timer_id,
// /*AFLA*/ 				struct itimerspec __user *setting);
// /*AFLA*/ asmlinkage long sys_timer_getoverrun(timer_t timer_id);
// /*AFLA*/ asmlinkage long sys_timer_settime(timer_t timer_id, int flags,
// /*AFLA*/ 				const struct itimerspec __user *new_setting,
// /*AFLA*/ 				struct itimerspec __user *old_setting);
// /*AFLA*/ asmlinkage long sys_timer_delete(timer_t timer_id);
// /*AFLA*/ asmlinkage long sys_clock_settime(clockid_t which_clock,
// /*AFLA*/ 				const struct timespec __user *tp);
// /*AFLA*/ asmlinkage long sys_clock_gettime(clockid_t which_clock,
// /*AFLA*/ 				struct timespec __user *tp);
// /*AFLA*/ asmlinkage long sys_clock_adjtime(clockid_t which_clock,
// /*AFLA*/ 				struct timex __user *tx);
// /*AFLA*/ asmlinkage long sys_clock_getres(clockid_t which_clock,
// /*AFLA*/ 				struct timespec __user *tp);
// /*AFLA*/ asmlinkage long sys_clock_nanosleep(clockid_t which_clock, int flags,
// /*AFLA*/ 				const struct timespec __user *rqtp,
// /*AFLA*/ 				struct timespec __user *rmtp);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_nice(int increment);
// /*AFLA*/ asmlinkage long sys_sched_setscheduler(pid_t pid, int policy,
// /*AFLA*/ 					struct sched_param __user *param);
// /*AFLA*/ asmlinkage long sys_sched_setparam(pid_t pid,
// /*AFLA*/ 					struct sched_param __user *param);
// /*AFLA*/ asmlinkage long sys_sched_setattr(pid_t pid,
// /*AFLA*/ 					struct sched_attr __user *attr,
// /*AFLA*/ 					unsigned int flags);
// /*AFLA*/ asmlinkage long sys_sched_getscheduler(pid_t pid);
// /*AFLA*/ asmlinkage long sys_sched_getparam(pid_t pid,
// /*AFLA*/ 					struct sched_param __user *param);
// /*AFLA*/ asmlinkage long sys_sched_getattr(pid_t pid,
// /*AFLA*/ 					struct sched_attr __user *attr,
// /*AFLA*/ 					unsigned int size,
// /*AFLA*/ 					unsigned int flags);
// /*AFLA*/ asmlinkage long sys_sched_setaffinity(pid_t pid, unsigned int len,
// /*AFLA*/ 					unsigned long __user *user_mask_ptr);
// /*AFLA*/ asmlinkage long sys_sched_getaffinity(pid_t pid, unsigned int len,
// /*AFLA*/ 					unsigned long __user *user_mask_ptr);
// /*AFLA*/ asmlinkage long sys_sched_yield(void);
// /*AFLA*/ asmlinkage long sys_sched_get_priority_max(int policy);
// /*AFLA*/ asmlinkage long sys_sched_get_priority_min(int policy);
// /*AFLA*/ asmlinkage long sys_sched_rr_get_interval(pid_t pid,
// /*AFLA*/ 					struct timespec __user *interval);
// /*AFLA*/ asmlinkage long sys_setpriority(int which, int who, int niceval);
// /*AFLA*/ asmlinkage long sys_getpriority(int which, int who);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_shutdown(int, int);
// /*AFLA*/ asmlinkage long sys_reboot(int magic1, int magic2, unsigned int cmd,
// /*AFLA*/ 				void __user *arg);
// /*AFLA*/ asmlinkage long sys_restart_syscall(void);
// /*AFLA*/ asmlinkage long sys_kexec_load(unsigned long entry, unsigned long nr_segments,
// /*AFLA*/ 				struct kexec_segment __user *segments,
// /*AFLA*/ 				unsigned long flags);
// /*AFLA*/ asmlinkage long sys_kexec_file_load(int kernel_fd, int initrd_fd,
// /*AFLA*/ 				    unsigned long cmdline_len,
// /*AFLA*/ 				    const char __user *cmdline_ptr,
// /*AFLA*/ 				    unsigned long flags);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_exit(int error_code);
// /*AFLA*/ asmlinkage long sys_exit_group(int error_code);
// /*AFLA*/ asmlinkage long sys_wait4(pid_t pid, int __user *stat_addr,
// /*AFLA*/ 				int options, struct rusage __user *ru);
// /*AFLA*/ asmlinkage long sys_waitid(int which, pid_t pid,
// /*AFLA*/ 			   struct siginfo __user *infop,
// /*AFLA*/ 			   int options, struct rusage __user *ru);
// /*AFLA*/ asmlinkage long sys_waitpid(pid_t pid, int __user *stat_addr, int options);
// /*AFLA*/ asmlinkage long sys_set_tid_address(int __user *tidptr);
// /*AFLA*/ asmlinkage long sys_futex(u32 __user *uaddr, int op, u32 val,
// /*AFLA*/ 			struct timespec __user *utime, u32 __user *uaddr2,
// /*AFLA*/ 			u32 val3);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_init_module(void __user *umod, unsigned long len,
// /*AFLA*/ 				const char __user *uargs);
// /*AFLA*/ asmlinkage long sys_delete_module(const char __user *name_user,
// /*AFLA*/ 				unsigned int flags);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_OLD_SIGSUSPEND
// /*AFLA*/ asmlinkage long sys_sigsuspend(old_sigset_t mask);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_OLD_SIGSUSPEND3
// /*AFLA*/ asmlinkage long sys_sigsuspend(int unused1, int unused2, old_sigset_t mask);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_rt_sigsuspend(sigset_t __user *unewset, size_t sigsetsize);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_OLD_SIGACTION
// /*AFLA*/ asmlinkage long sys_sigaction(int, const struct old_sigaction __user *,
// /*AFLA*/ 				struct old_sigaction __user *);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_ODD_RT_SIGACTION
// /*AFLA*/ asmlinkage long sys_rt_sigaction(int,
// /*AFLA*/ 				 const struct sigaction __user *,
// /*AFLA*/ 				 struct sigaction __user *,
// /*AFLA*/ 				 size_t);
// /*AFLA*/ #endif
// /*AFLA*/ asmlinkage long sys_rt_sigprocmask(int how, sigset_t __user *set,
// /*AFLA*/ 				sigset_t __user *oset, size_t sigsetsize);
// /*AFLA*/ asmlinkage long sys_rt_sigpending(sigset_t __user *set, size_t sigsetsize);
// /*AFLA*/ asmlinkage long sys_rt_sigtimedwait(const sigset_t __user *uthese,
// /*AFLA*/ 				siginfo_t __user *uinfo,
// /*AFLA*/ 				const struct timespec __user *uts,
// /*AFLA*/ 				size_t sigsetsize);
// /*AFLA*/ asmlinkage long sys_rt_tgsigqueueinfo(pid_t tgid, pid_t  pid, int sig,
// /*AFLA*/ 		siginfo_t __user *uinfo);
// /*AFLA*/ asmlinkage long sys_kill(pid_t pid, int sig);
// /*AFLA*/ asmlinkage long sys_tgkill(pid_t tgid, pid_t pid, int sig);
// /*AFLA*/ asmlinkage long sys_tkill(pid_t pid, int sig);
// /*AFLA*/ asmlinkage long sys_rt_sigqueueinfo(pid_t pid, int sig, siginfo_t __user *uinfo);
// /*AFLA*/ asmlinkage long sys_sgetmask(void);
// /*AFLA*/ asmlinkage long sys_ssetmask(int newmask);
// /*AFLA*/ asmlinkage long sys_signal(int sig, __sighandler_t handler);
// /*AFLA*/ asmlinkage long sys_pause(void);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_sync(void);
// /*AFLA*/ asmlinkage long sys_fsync(unsigned int fd);
// /*AFLA*/ asmlinkage long sys_fdatasync(unsigned int fd);
// /*AFLA*/ asmlinkage long sys_bdflush(int func, long data);
// /*AFLA*/ asmlinkage long sys_mount(char __user *dev_name, char __user *dir_name,
// /*AFLA*/ 				char __user *type, unsigned long flags,
// /*AFLA*/ 				void __user *data);
// /*AFLA*/ asmlinkage long sys_umount(char __user *name, int flags);
// /*AFLA*/ asmlinkage long sys_oldumount(char __user *name);
// /*AFLA*/ asmlinkage long sys_truncate(const char __user *path, long length);
// /*AFLA*/ asmlinkage long sys_ftruncate(unsigned int fd, unsigned long length);
// /*AFLA*/ asmlinkage long sys_stat(const char __user *filename,
// /*AFLA*/ 			struct __old_kernel_stat __user *statbuf);
// /*AFLA*/ asmlinkage long sys_statfs(const char __user * path,
// /*AFLA*/ 				struct statfs __user *buf);
// /*AFLA*/ asmlinkage long sys_statfs64(const char __user *path, size_t sz,
// /*AFLA*/ 				struct statfs64 __user *buf);
// /*AFLA*/ asmlinkage long sys_fstatfs(unsigned int fd, struct statfs __user *buf);
// /*AFLA*/ asmlinkage long sys_fstatfs64(unsigned int fd, size_t sz,
// /*AFLA*/ 				struct statfs64 __user *buf);
// /*AFLA*/ asmlinkage long sys_lstat(const char __user *filename,
// /*AFLA*/ 			struct __old_kernel_stat __user *statbuf);
// /*AFLA*/ asmlinkage long sys_fstat(unsigned int fd,
// /*AFLA*/ 			struct __old_kernel_stat __user *statbuf);
// /*AFLA*/ asmlinkage long sys_newstat(const char __user *filename,
// /*AFLA*/ 				struct stat __user *statbuf);
// /*AFLA*/ asmlinkage long sys_newlstat(const char __user *filename,
// /*AFLA*/ 				struct stat __user *statbuf);
// /*AFLA*/ asmlinkage long sys_newfstat(unsigned int fd, struct stat __user *statbuf);
// /*AFLA*/ asmlinkage long sys_ustat(unsigned dev, struct ustat __user *ubuf);
// /*AFLA*/ #if defined(__ARCH_WANT_STAT64) || defined(__ARCH_WANT_COMPAT_STAT64)
// /*AFLA*/ asmlinkage long sys_stat64(const char __user *filename,
// /*AFLA*/ 				struct stat64 __user *statbuf);
// /*AFLA*/ asmlinkage long sys_fstat64(unsigned long fd, struct stat64 __user *statbuf);
// /*AFLA*/ asmlinkage long sys_lstat64(const char __user *filename,
// /*AFLA*/ 				struct stat64 __user *statbuf);
// /*AFLA*/ asmlinkage long sys_fstatat64(int dfd, const char __user *filename,
// /*AFLA*/ 			       struct stat64 __user *statbuf, int flag);
// /*AFLA*/ #endif
// /*AFLA*/ #if BITS_PER_LONG == 32
// /*AFLA*/ asmlinkage long sys_truncate64(const char __user *path, loff_t length);
// /*AFLA*/ asmlinkage long sys_ftruncate64(unsigned int fd, loff_t length);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_setxattr(const char __user *path, const char __user *name,
// /*AFLA*/ 			     const void __user *value, size_t size, int flags);
// /*AFLA*/ asmlinkage long sys_lsetxattr(const char __user *path, const char __user *name,
// /*AFLA*/ 			      const void __user *value, size_t size, int flags);
// /*AFLA*/ asmlinkage long sys_fsetxattr(int fd, const char __user *name,
// /*AFLA*/ 			      const void __user *value, size_t size, int flags);
// /*AFLA*/ asmlinkage long sys_getxattr(const char __user *path, const char __user *name,
// /*AFLA*/ 			     void __user *value, size_t size);
// /*AFLA*/ asmlinkage long sys_lgetxattr(const char __user *path, const char __user *name,
// /*AFLA*/ 			      void __user *value, size_t size);
// /*AFLA*/ asmlinkage long sys_fgetxattr(int fd, const char __user *name,
// /*AFLA*/ 			      void __user *value, size_t size);
// /*AFLA*/ asmlinkage long sys_listxattr(const char __user *path, char __user *list,
// /*AFLA*/ 			      size_t size);
// /*AFLA*/ asmlinkage long sys_llistxattr(const char __user *path, char __user *list,
// /*AFLA*/ 			       size_t size);
// /*AFLA*/ asmlinkage long sys_flistxattr(int fd, char __user *list, size_t size);
// /*AFLA*/ asmlinkage long sys_removexattr(const char __user *path,
// /*AFLA*/ 				const char __user *name);
// /*AFLA*/ asmlinkage long sys_lremovexattr(const char __user *path,
// /*AFLA*/ 				 const char __user *name);
// /*AFLA*/ asmlinkage long sys_fremovexattr(int fd, const char __user *name);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_brk(unsigned long brk);
// /*AFLA*/ asmlinkage long sys_mprotect(unsigned long start, size_t len,
// /*AFLA*/ 				unsigned long prot);
// /*AFLA*/ asmlinkage long sys_mremap(unsigned long addr,
// /*AFLA*/ 			   unsigned long old_len, unsigned long new_len,
// /*AFLA*/ 			   unsigned long flags, unsigned long new_addr);
// /*AFLA*/ asmlinkage long sys_remap_file_pages(unsigned long start, unsigned long size,
// /*AFLA*/ 			unsigned long prot, unsigned long pgoff,
// /*AFLA*/ 			unsigned long flags);
// /*AFLA*/ asmlinkage long sys_msync(unsigned long start, size_t len, int flags);
// /*AFLA*/ asmlinkage long sys_fadvise64(int fd, loff_t offset, size_t len, int advice);
// /*AFLA*/ asmlinkage long sys_fadvise64_64(int fd, loff_t offset, loff_t len, int advice);
// /*AFLA*/ asmlinkage long sys_munmap(unsigned long addr, size_t len);
// /*AFLA*/ asmlinkage long sys_mlock(unsigned long start, size_t len);
// /*AFLA*/ asmlinkage long sys_munlock(unsigned long start, size_t len);
// /*AFLA*/ asmlinkage long sys_mlockall(int flags);
// /*AFLA*/ asmlinkage long sys_munlockall(void);
// /*AFLA*/ asmlinkage long sys_madvise(unsigned long start, size_t len, int behavior);
// /*AFLA*/ asmlinkage long sys_mincore(unsigned long start, size_t len,
// /*AFLA*/ 				unsigned char __user * vec);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_pivot_root(const char __user *new_root,
// /*AFLA*/ 				const char __user *put_old);
// /*AFLA*/ asmlinkage long sys_chroot(const char __user *filename);
// /*AFLA*/ asmlinkage long sys_mknod(const char __user *filename, umode_t mode,
// /*AFLA*/ 				unsigned dev);
// /*AFLA*/ asmlinkage long sys_link(const char __user *oldname,
// /*AFLA*/ 				const char __user *newname);
// /*AFLA*/ asmlinkage long sys_symlink(const char __user *old, const char __user *new);
// /*AFLA*/ asmlinkage long sys_unlink(const char __user *pathname);
// /*AFLA*/ asmlinkage long sys_rename(const char __user *oldname,
// /*AFLA*/ 				const char __user *newname);
// /*AFLA*/ asmlinkage long sys_chmod(const char __user *filename, umode_t mode);
// /*AFLA*/ asmlinkage long sys_fchmod(unsigned int fd, umode_t mode);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_fcntl(unsigned int fd, unsigned int cmd, unsigned long arg);
// /*AFLA*/ #if BITS_PER_LONG == 32
// /*AFLA*/ asmlinkage long sys_fcntl64(unsigned int fd,
// /*AFLA*/ 				unsigned int cmd, unsigned long arg);
// /*AFLA*/ #endif
// /*AFLA*/ asmlinkage long sys_pipe(int __user *fildes);
// /*AFLA*/ asmlinkage long sys_pipe2(int __user *fildes, int flags);
// /*AFLA*/ asmlinkage long sys_dup(unsigned int fildes);
// /*AFLA*/ asmlinkage long sys_dup2(unsigned int oldfd, unsigned int newfd);
// /*AFLA*/ asmlinkage long sys_dup3(unsigned int oldfd, unsigned int newfd, int flags);
// /*AFLA*/ asmlinkage long sys_ioperm(unsigned long from, unsigned long num, int on);
// /*AFLA*/ asmlinkage long sys_ioctl(unsigned int fd, unsigned int cmd,
// /*AFLA*/ 				unsigned long arg);
// /*AFLA*/ asmlinkage long sys_flock(unsigned int fd, unsigned int cmd);
// /*AFLA*/ asmlinkage long sys_io_setup(unsigned nr_reqs, aio_context_t __user *ctx);
// /*AFLA*/ asmlinkage long sys_io_destroy(aio_context_t ctx);
// /*AFLA*/ asmlinkage long sys_io_getevents(aio_context_t ctx_id,
// /*AFLA*/ 				long min_nr,
// /*AFLA*/ 				long nr,
// /*AFLA*/ 				struct io_event __user *events,
// /*AFLA*/ 				struct timespec __user *timeout);
// /*AFLA*/ asmlinkage long sys_io_submit(aio_context_t, long,
// /*AFLA*/ 				struct iocb __user * __user *);
// /*AFLA*/ asmlinkage long sys_io_cancel(aio_context_t ctx_id, struct iocb __user *iocb,
// /*AFLA*/ 			      struct io_event __user *result);
// /*AFLA*/ asmlinkage long sys_sendfile(int out_fd, int in_fd,
// /*AFLA*/ 			     off_t __user *offset, size_t count);
// /*AFLA*/ asmlinkage long sys_sendfile64(int out_fd, int in_fd,
// /*AFLA*/ 			       loff_t __user *offset, size_t count);
// /*AFLA*/ asmlinkage long sys_readlink(const char __user *path,
// /*AFLA*/ 				char __user *buf, int bufsiz);
// /*AFLA*/ asmlinkage long sys_creat(const char __user *pathname, umode_t mode);
// /*AFLA*/ asmlinkage long sys_open(const char __user *filename,
// /*AFLA*/ 				int flags, umode_t mode);
// /*AFLA*/ asmlinkage long sys_close(unsigned int fd);
// /*AFLA*/ asmlinkage long sys_access(const char __user *filename, int mode);
// /*AFLA*/ asmlinkage long sys_vhangup(void);
// /*AFLA*/ asmlinkage long sys_chown(const char __user *filename,
// /*AFLA*/ 				uid_t user, gid_t group);
// /*AFLA*/ asmlinkage long sys_lchown(const char __user *filename,
// /*AFLA*/ 				uid_t user, gid_t group);
// /*AFLA*/ asmlinkage long sys_fchown(unsigned int fd, uid_t user, gid_t group);
// /*AFLA*/ #ifdef CONFIG_HAVE_UID16
// /*AFLA*/ asmlinkage long sys_chown16(const char __user *filename,
// /*AFLA*/ 				old_uid_t user, old_gid_t group);
// /*AFLA*/ asmlinkage long sys_lchown16(const char __user *filename,
// /*AFLA*/ 				old_uid_t user, old_gid_t group);
// /*AFLA*/ asmlinkage long sys_fchown16(unsigned int fd, old_uid_t user, old_gid_t group);
// /*AFLA*/ asmlinkage long sys_setregid16(old_gid_t rgid, old_gid_t egid);
// /*AFLA*/ asmlinkage long sys_setgid16(old_gid_t gid);
// /*AFLA*/ asmlinkage long sys_setreuid16(old_uid_t ruid, old_uid_t euid);
// /*AFLA*/ asmlinkage long sys_setuid16(old_uid_t uid);
// /*AFLA*/ asmlinkage long sys_setresuid16(old_uid_t ruid, old_uid_t euid, old_uid_t suid);
// /*AFLA*/ asmlinkage long sys_getresuid16(old_uid_t __user *ruid,
// /*AFLA*/ 				old_uid_t __user *euid, old_uid_t __user *suid);
// /*AFLA*/ asmlinkage long sys_setresgid16(old_gid_t rgid, old_gid_t egid, old_gid_t sgid);
// /*AFLA*/ asmlinkage long sys_getresgid16(old_gid_t __user *rgid,
// /*AFLA*/ 				old_gid_t __user *egid, old_gid_t __user *sgid);
// /*AFLA*/ asmlinkage long sys_setfsuid16(old_uid_t uid);
// /*AFLA*/ asmlinkage long sys_setfsgid16(old_gid_t gid);
// /*AFLA*/ asmlinkage long sys_getgroups16(int gidsetsize, old_gid_t __user *grouplist);
// /*AFLA*/ asmlinkage long sys_setgroups16(int gidsetsize, old_gid_t __user *grouplist);
// /*AFLA*/ asmlinkage long sys_getuid16(void);
// /*AFLA*/ asmlinkage long sys_geteuid16(void);
// /*AFLA*/ asmlinkage long sys_getgid16(void);
// /*AFLA*/ asmlinkage long sys_getegid16(void);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_utime(char __user *filename,
// /*AFLA*/ 				struct utimbuf __user *times);
// /*AFLA*/ asmlinkage long sys_utimes(char __user *filename,
// /*AFLA*/ 				struct timeval __user *utimes);
// /*AFLA*/ asmlinkage long sys_lseek(unsigned int fd, off_t offset,
// /*AFLA*/ 			  unsigned int whence);
// /*AFLA*/ asmlinkage long sys_llseek(unsigned int fd, unsigned long offset_high,
// /*AFLA*/ 			unsigned long offset_low, loff_t __user *result,
// /*AFLA*/ 			unsigned int whence);
// /*AFLA*/ asmlinkage long sys_read(unsigned int fd, char __user *buf, size_t count);
// /*AFLA*/ asmlinkage long sys_readahead(int fd, loff_t offset, size_t count);
// /*AFLA*/ asmlinkage long sys_readv(unsigned long fd,
// /*AFLA*/ 			  const struct iovec __user *vec,
// /*AFLA*/ 			  unsigned long vlen);
// /*AFLA*/ asmlinkage long sys_write(unsigned int fd, const char __user *buf,
// /*AFLA*/ 			  size_t count);
// /*AFLA*/ asmlinkage long sys_writev(unsigned long fd,
// /*AFLA*/ 			   const struct iovec __user *vec,
// /*AFLA*/ 			   unsigned long vlen);
// /*AFLA*/ asmlinkage long sys_pread64(unsigned int fd, char __user *buf,
// /*AFLA*/ 			    size_t count, loff_t pos);
// /*AFLA*/ asmlinkage long sys_pwrite64(unsigned int fd, const char __user *buf,
// /*AFLA*/ 			     size_t count, loff_t pos);
// /*AFLA*/ asmlinkage long sys_preadv(unsigned long fd, const struct iovec __user *vec,
// /*AFLA*/ 			   unsigned long vlen, unsigned long pos_l, unsigned long pos_h);
// /*AFLA*/ asmlinkage long sys_preadv2(unsigned long fd, const struct iovec __user *vec,
// /*AFLA*/ 			    unsigned long vlen, unsigned long pos_l, unsigned long pos_h,
// /*AFLA*/ 			    int flags);
// /*AFLA*/ asmlinkage long sys_pwritev(unsigned long fd, const struct iovec __user *vec,
// /*AFLA*/ 			    unsigned long vlen, unsigned long pos_l, unsigned long pos_h);
// /*AFLA*/ asmlinkage long sys_pwritev2(unsigned long fd, const struct iovec __user *vec,
// /*AFLA*/ 			    unsigned long vlen, unsigned long pos_l, unsigned long pos_h,
// /*AFLA*/ 			    int flags);
// /*AFLA*/ asmlinkage long sys_getcwd(char __user *buf, unsigned long size);
// /*AFLA*/ asmlinkage long sys_mkdir(const char __user *pathname, umode_t mode);
// /*AFLA*/ asmlinkage long sys_chdir(const char __user *filename);
// /*AFLA*/ asmlinkage long sys_fchdir(unsigned int fd);
// /*AFLA*/ asmlinkage long sys_rmdir(const char __user *pathname);
// /*AFLA*/ asmlinkage long sys_lookup_dcookie(u64 cookie64, char __user *buf, size_t len);
// /*AFLA*/ asmlinkage long sys_quotactl(unsigned int cmd, const char __user *special,
// /*AFLA*/ 				qid_t id, void __user *addr);
// /*AFLA*/ asmlinkage long sys_getdents(unsigned int fd,
// /*AFLA*/ 				struct linux_dirent __user *dirent,
// /*AFLA*/ 				unsigned int count);
// /*AFLA*/ asmlinkage long sys_getdents64(unsigned int fd,
// /*AFLA*/ 				struct linux_dirent64 __user *dirent,
// /*AFLA*/ 				unsigned int count);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_setsockopt(int fd, int level, int optname,
// /*AFLA*/ 				char __user *optval, int optlen);
// /*AFLA*/ asmlinkage long sys_getsockopt(int fd, int level, int optname,
// /*AFLA*/ 				char __user *optval, int __user *optlen);
// /*AFLA*/ asmlinkage long sys_bind(int, struct sockaddr __user *, int);
// /*AFLA*/ asmlinkage long sys_connect(int, struct sockaddr __user *, int);
// /*AFLA*/ asmlinkage long sys_accept(int, struct sockaddr __user *, int __user *);
// /*AFLA*/ asmlinkage long sys_accept4(int, struct sockaddr __user *, int __user *, int);
// /*AFLA*/ asmlinkage long sys_getsockname(int, struct sockaddr __user *, int __user *);
// /*AFLA*/ asmlinkage long sys_getpeername(int, struct sockaddr __user *, int __user *);
// /*AFLA*/ asmlinkage long sys_send(int, void __user *, size_t, unsigned);
// /*AFLA*/ asmlinkage long sys_sendto(int, void __user *, size_t, unsigned,
// /*AFLA*/ 				struct sockaddr __user *, int);
// /*AFLA*/ asmlinkage long sys_sendmsg(int fd, struct user_msghdr __user *msg, unsigned flags);
// /*AFLA*/ asmlinkage long sys_sendmmsg(int fd, struct mmsghdr __user *msg,
// /*AFLA*/ 			     unsigned int vlen, unsigned flags);
// /*AFLA*/ asmlinkage long sys_recv(int, void __user *, size_t, unsigned);
// /*AFLA*/ asmlinkage long sys_recvfrom(int, void __user *, size_t, unsigned,
// /*AFLA*/ 				struct sockaddr __user *, int __user *);
// /*AFLA*/ asmlinkage long sys_recvmsg(int fd, struct user_msghdr __user *msg, unsigned flags);
// /*AFLA*/ asmlinkage long sys_recvmmsg(int fd, struct mmsghdr __user *msg,
// /*AFLA*/ 			     unsigned int vlen, unsigned flags,
// /*AFLA*/ 			     struct timespec __user *timeout);
// /*AFLA*/ asmlinkage long sys_socket(int, int, int);
// /*AFLA*/ asmlinkage long sys_socketpair(int, int, int, int __user *);
// /*AFLA*/ asmlinkage long sys_socketcall(int call, unsigned long __user *args);
// /*AFLA*/ asmlinkage long sys_listen(int, int);
// /*AFLA*/ asmlinkage long sys_poll(struct pollfd __user *ufds, unsigned int nfds,
// /*AFLA*/ 				int timeout);
// /*AFLA*/ asmlinkage long sys_select(int n, fd_set __user *inp, fd_set __user *outp,
// /*AFLA*/ 			fd_set __user *exp, struct timeval __user *tvp);
// /*AFLA*/ asmlinkage long sys_old_select(struct sel_arg_struct __user *arg);
// /*AFLA*/ asmlinkage long sys_epoll_create(int size);
// /*AFLA*/ asmlinkage long sys_epoll_create1(int flags);
// /*AFLA*/ asmlinkage long sys_epoll_ctl(int epfd, int op, int fd,
// /*AFLA*/ 				struct epoll_event __user *event);
// /*AFLA*/ asmlinkage long sys_epoll_wait(int epfd, struct epoll_event __user *events,
// /*AFLA*/ 				int maxevents, int timeout);
// /*AFLA*/ asmlinkage long sys_epoll_pwait(int epfd, struct epoll_event __user *events,
// /*AFLA*/ 				int maxevents, int timeout,
// /*AFLA*/ 				const sigset_t __user *sigmask,
// /*AFLA*/ 				size_t sigsetsize);
// /*AFLA*/ asmlinkage long sys_gethostname(char __user *name, int len);
// /*AFLA*/ asmlinkage long sys_sethostname(char __user *name, int len);
// /*AFLA*/ asmlinkage long sys_setdomainname(char __user *name, int len);
// /*AFLA*/ asmlinkage long sys_newuname(struct new_utsname __user *name);
// /*AFLA*/ asmlinkage long sys_uname(struct old_utsname __user *);
// /*AFLA*/ asmlinkage long sys_olduname(struct oldold_utsname __user *);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_getrlimit(unsigned int resource,
// /*AFLA*/ 				struct rlimit __user *rlim);
// /*AFLA*/ #if defined(COMPAT_RLIM_OLD_INFINITY) || !(defined(CONFIG_IA64))
// /*AFLA*/ asmlinkage long sys_old_getrlimit(unsigned int resource, struct rlimit __user *rlim);
// /*AFLA*/ #endif
// /*AFLA*/ asmlinkage long sys_setrlimit(unsigned int resource,
// /*AFLA*/ 				struct rlimit __user *rlim);
// /*AFLA*/ asmlinkage long sys_prlimit64(pid_t pid, unsigned int resource,
// /*AFLA*/ 				const struct rlimit64 __user *new_rlim,
// /*AFLA*/ 				struct rlimit64 __user *old_rlim);
// /*AFLA*/ asmlinkage long sys_getrusage(int who, struct rusage __user *ru);
// /*AFLA*/ asmlinkage long sys_umask(int mask);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_msgget(key_t key, int msgflg);
// /*AFLA*/ asmlinkage long sys_msgsnd(int msqid, struct msgbuf __user *msgp,
// /*AFLA*/ 				size_t msgsz, int msgflg);
// /*AFLA*/ asmlinkage long sys_msgrcv(int msqid, struct msgbuf __user *msgp,
// /*AFLA*/ 				size_t msgsz, long msgtyp, int msgflg);
// /*AFLA*/ asmlinkage long sys_msgctl(int msqid, int cmd, struct msqid_ds __user *buf);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_semget(key_t key, int nsems, int semflg);
// /*AFLA*/ asmlinkage long sys_semop(int semid, struct sembuf __user *sops,
// /*AFLA*/ 				unsigned nsops);
// /*AFLA*/ asmlinkage long sys_semctl(int semid, int semnum, int cmd, unsigned long arg);
// /*AFLA*/ asmlinkage long sys_semtimedop(int semid, struct sembuf __user *sops,
// /*AFLA*/ 				unsigned nsops,
// /*AFLA*/ 				const struct timespec __user *timeout);
// /*AFLA*/ asmlinkage long sys_shmat(int shmid, char __user *shmaddr, int shmflg);
// /*AFLA*/ asmlinkage long sys_shmget(key_t key, size_t size, int flag);
// /*AFLA*/ asmlinkage long sys_shmdt(char __user *shmaddr);
// /*AFLA*/ asmlinkage long sys_shmctl(int shmid, int cmd, struct shmid_ds __user *buf);
// /*AFLA*/ asmlinkage long sys_ipc(unsigned int call, int first, unsigned long second,
// /*AFLA*/ 		unsigned long third, void __user *ptr, long fifth);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_mq_open(const char __user *name, int oflag, umode_t mode, struct mq_attr __user *attr);
// /*AFLA*/ asmlinkage long sys_mq_unlink(const char __user *name);
// /*AFLA*/ asmlinkage long sys_mq_timedsend(mqd_t mqdes, const char __user *msg_ptr, size_t msg_len, unsigned int msg_prio, const struct timespec __user *abs_timeout);
// /*AFLA*/ asmlinkage long sys_mq_timedreceive(mqd_t mqdes, char __user *msg_ptr, size_t msg_len, unsigned int __user *msg_prio, const struct timespec __user *abs_timeout);
// /*AFLA*/ asmlinkage long sys_mq_notify(mqd_t mqdes, const struct sigevent __user *notification);
// /*AFLA*/ asmlinkage long sys_mq_getsetattr(mqd_t mqdes, const struct mq_attr __user *mqstat, struct mq_attr __user *omqstat);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_pciconfig_iobase(long which, unsigned long bus, unsigned long devfn);
// /*AFLA*/ asmlinkage long sys_pciconfig_read(unsigned long bus, unsigned long dfn,
// /*AFLA*/ 				unsigned long off, unsigned long len,
// /*AFLA*/ 				void __user *buf);
// /*AFLA*/ asmlinkage long sys_pciconfig_write(unsigned long bus, unsigned long dfn,
// /*AFLA*/ 				unsigned long off, unsigned long len,
// /*AFLA*/ 				void __user *buf);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_prctl(int option, unsigned long arg2, unsigned long arg3,
// /*AFLA*/ 			unsigned long arg4, unsigned long arg5);
// /*AFLA*/ asmlinkage long sys_swapon(const char __user *specialfile, int swap_flags);
// /*AFLA*/ asmlinkage long sys_swapoff(const char __user *specialfile);
// /*AFLA*/ asmlinkage long sys_sysctl(struct __sysctl_args __user *args);
// /*AFLA*/ asmlinkage long sys_sysinfo(struct sysinfo __user *info);
// /*AFLA*/ asmlinkage long sys_sysfs(int option,
// /*AFLA*/ 				unsigned long arg1, unsigned long arg2);
// /*AFLA*/ asmlinkage long sys_syslog(int type, char __user *buf, int len);
// /*AFLA*/ asmlinkage long sys_uselib(const char __user *library);
// /*AFLA*/ asmlinkage long sys_ni_syscall(void);
// /*AFLA*/ asmlinkage long sys_ptrace(long request, long pid, unsigned long addr,
// /*AFLA*/ 			   unsigned long data);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_add_key(const char __user *_type,
// /*AFLA*/ 			    const char __user *_description,
// /*AFLA*/ 			    const void __user *_payload,
// /*AFLA*/ 			    size_t plen,
// /*AFLA*/ 			    key_serial_t destringid);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_request_key(const char __user *_type,
// /*AFLA*/ 				const char __user *_description,
// /*AFLA*/ 				const char __user *_callout_info,
// /*AFLA*/ 				key_serial_t destringid);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_keyctl(int cmd, unsigned long arg2, unsigned long arg3,
// /*AFLA*/ 			   unsigned long arg4, unsigned long arg5);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_ioprio_set(int which, int who, int ioprio);
// /*AFLA*/ asmlinkage long sys_ioprio_get(int which, int who);
// /*AFLA*/ asmlinkage long sys_set_mempolicy(int mode, const unsigned long __user *nmask,
// /*AFLA*/ 				unsigned long maxnode);
// /*AFLA*/ asmlinkage long sys_migrate_pages(pid_t pid, unsigned long maxnode,
// /*AFLA*/ 				const unsigned long __user *from,
// /*AFLA*/ 				const unsigned long __user *to);
// /*AFLA*/ asmlinkage long sys_move_pages(pid_t pid, unsigned long nr_pages,
// /*AFLA*/ 				const void __user * __user *pages,
// /*AFLA*/ 				const int __user *nodes,
// /*AFLA*/ 				int __user *status,
// /*AFLA*/ 				int flags);
// /*AFLA*/ asmlinkage long sys_mbind(unsigned long start, unsigned long len,
// /*AFLA*/ 				unsigned long mode,
// /*AFLA*/ 				const unsigned long __user *nmask,
// /*AFLA*/ 				unsigned long maxnode,
// /*AFLA*/ 				unsigned flags);
// /*AFLA*/ asmlinkage long sys_get_mempolicy(int __user *policy,
// /*AFLA*/ 				unsigned long __user *nmask,
// /*AFLA*/ 				unsigned long maxnode,
// /*AFLA*/ 				unsigned long addr, unsigned long flags);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_inotify_init(void);
// /*AFLA*/ asmlinkage long sys_inotify_init1(int flags);
// /*AFLA*/ asmlinkage long sys_inotify_add_watch(int fd, const char __user *path,
// /*AFLA*/ 					u32 mask);
// /*AFLA*/ asmlinkage long sys_inotify_rm_watch(int fd, __s32 wd);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_spu_run(int fd, __u32 __user *unpc,
// /*AFLA*/ 				 __u32 __user *ustatus);
// /*AFLA*/ asmlinkage long sys_spu_create(const char __user *name,
// /*AFLA*/ 		unsigned int flags, umode_t mode, int fd);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_mknodat(int dfd, const char __user * filename, umode_t mode,
// /*AFLA*/ 			    unsigned dev);
// /*AFLA*/ asmlinkage long sys_mkdirat(int dfd, const char __user * pathname, umode_t mode);
// /*AFLA*/ asmlinkage long sys_unlinkat(int dfd, const char __user * pathname, int flag);
// /*AFLA*/ asmlinkage long sys_symlinkat(const char __user * oldname,
// /*AFLA*/ 			      int newdfd, const char __user * newname);
// /*AFLA*/ asmlinkage long sys_linkat(int olddfd, const char __user *oldname,
// /*AFLA*/ 			   int newdfd, const char __user *newname, int flags);
// /*AFLA*/ asmlinkage long sys_renameat(int olddfd, const char __user * oldname,
// /*AFLA*/ 			     int newdfd, const char __user * newname);
// /*AFLA*/ asmlinkage long sys_renameat2(int olddfd, const char __user *oldname,
// /*AFLA*/ 			      int newdfd, const char __user *newname,
// /*AFLA*/ 			      unsigned int flags);
// /*AFLA*/ asmlinkage long sys_futimesat(int dfd, const char __user *filename,
// /*AFLA*/ 			      struct timeval __user *utimes);
// /*AFLA*/ asmlinkage long sys_faccessat(int dfd, const char __user *filename, int mode);
// /*AFLA*/ asmlinkage long sys_fchmodat(int dfd, const char __user * filename,
// /*AFLA*/ 			     umode_t mode);
// /*AFLA*/ asmlinkage long sys_fchownat(int dfd, const char __user *filename, uid_t user,
// /*AFLA*/ 			     gid_t group, int flag);
// /*AFLA*/ asmlinkage long sys_openat(int dfd, const char __user *filename, int flags,
// /*AFLA*/ 			   umode_t mode);
// /*AFLA*/ asmlinkage long sys_newfstatat(int dfd, const char __user *filename,
// /*AFLA*/ 			       struct stat __user *statbuf, int flag);
// /*AFLA*/ asmlinkage long sys_readlinkat(int dfd, const char __user *path, char __user *buf,
// /*AFLA*/ 			       int bufsiz);
// /*AFLA*/ asmlinkage long sys_utimensat(int dfd, const char __user *filename,
// /*AFLA*/ 				struct timespec __user *utimes, int flags);
// /*AFLA*/ asmlinkage long sys_unshare(unsigned long unshare_flags);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_splice(int fd_in, loff_t __user *off_in,
// /*AFLA*/ 			   int fd_out, loff_t __user *off_out,
// /*AFLA*/ 			   size_t len, unsigned int flags);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_vmsplice(int fd, const struct iovec __user *iov,
// /*AFLA*/ 			     unsigned long nr_segs, unsigned int flags);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_tee(int fdin, int fdout, size_t len, unsigned int flags);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_sync_file_range(int fd, loff_t offset, loff_t nbytes,
// /*AFLA*/ 					unsigned int flags);
// /*AFLA*/ asmlinkage long sys_sync_file_range2(int fd, unsigned int flags,
// /*AFLA*/ 				     loff_t offset, loff_t nbytes);
// /*AFLA*/ asmlinkage long sys_get_robust_list(int pid,
// /*AFLA*/ 				    struct robust_list_head __user * __user *head_ptr,
// /*AFLA*/ 				    size_t __user *len_ptr);
// /*AFLA*/ asmlinkage long sys_set_robust_list(struct robust_list_head __user *head,
// /*AFLA*/ 				    size_t len);
// /*AFLA*/ asmlinkage long sys_getcpu(unsigned __user *cpu, unsigned __user *node, struct getcpu_cache __user *cache);
// /*AFLA*/ asmlinkage long sys_signalfd(int ufd, sigset_t __user *user_mask, size_t sizemask);
// /*AFLA*/ asmlinkage long sys_signalfd4(int ufd, sigset_t __user *user_mask, size_t sizemask, int flags);
// /*AFLA*/ asmlinkage long sys_timerfd_create(int clockid, int flags);
// /*AFLA*/ asmlinkage long sys_timerfd_settime(int ufd, int flags,
// /*AFLA*/ 				    const struct itimerspec __user *utmr,
// /*AFLA*/ 				    struct itimerspec __user *otmr);
// /*AFLA*/ asmlinkage long sys_timerfd_gettime(int ufd, struct itimerspec __user *otmr);
// /*AFLA*/ asmlinkage long sys_eventfd(unsigned int count);
// /*AFLA*/ asmlinkage long sys_eventfd2(unsigned int count, int flags);
// /*AFLA*/ asmlinkage long sys_memfd_create(const char __user *uname_ptr, unsigned int flags);
// /*AFLA*/ asmlinkage long sys_userfaultfd(int flags);
// /*AFLA*/ asmlinkage long sys_fallocate(int fd, int mode, loff_t offset, loff_t len);
// /*AFLA*/ asmlinkage long sys_old_readdir(unsigned int, struct old_linux_dirent __user *, unsigned int);
// /*AFLA*/ asmlinkage long sys_pselect6(int, fd_set __user *, fd_set __user *,
// /*AFLA*/ 			     fd_set __user *, struct timespec __user *,
// /*AFLA*/ 			     void __user *);
// /*AFLA*/ asmlinkage long sys_ppoll(struct pollfd __user *, unsigned int,
// /*AFLA*/ 			  struct timespec __user *, const sigset_t __user *,
// /*AFLA*/ 			  size_t);
// /*AFLA*/ asmlinkage long sys_fanotify_init(unsigned int flags, unsigned int event_f_flags);
// /*AFLA*/ asmlinkage long sys_fanotify_mark(int fanotify_fd, unsigned int flags,
// /*AFLA*/ 				  u64 mask, int fd,
// /*AFLA*/ 				  const char  __user *pathname);
// /*AFLA*/ asmlinkage long sys_syncfs(int fd);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_fork(void);
// /*AFLA*/ asmlinkage long sys_vfork(void);
// /*AFLA*/ #ifdef CONFIG_CLONE_BACKWARDS
// /*AFLA*/ asmlinkage long sys_clone(unsigned long, unsigned long, int __user *, unsigned long,
// /*AFLA*/ 	       int __user *);
// /*AFLA*/ #else
// /*AFLA*/ #ifdef CONFIG_CLONE_BACKWARDS3
// /*AFLA*/ asmlinkage long sys_clone(unsigned long, unsigned long, int, int __user *,
// /*AFLA*/ 			  int __user *, unsigned long);
// /*AFLA*/ #else
// /*AFLA*/ asmlinkage long sys_clone(unsigned long, unsigned long, int __user *,
// /*AFLA*/ 	       int __user *, unsigned long);
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_execve(const char __user *filename,
// /*AFLA*/ 		const char __user *const __user *argv,
// /*AFLA*/ 		const char __user *const __user *envp);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_perf_event_open(
// /*AFLA*/ 		struct perf_event_attr __user *attr_uptr,
// /*AFLA*/ 		pid_t pid, int cpu, int group_fd, unsigned long flags);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_mmap_pgoff(unsigned long addr, unsigned long len,
// /*AFLA*/ 			unsigned long prot, unsigned long flags,
// /*AFLA*/ 			unsigned long fd, unsigned long pgoff);
// /*AFLA*/ asmlinkage long sys_old_mmap(struct mmap_arg_struct __user *arg);
// /*AFLA*/ asmlinkage long sys_name_to_handle_at(int dfd, const char __user *name,
// /*AFLA*/ 				      struct file_handle __user *handle,
// /*AFLA*/ 				      int __user *mnt_id, int flag);
// /*AFLA*/ asmlinkage long sys_open_by_handle_at(int mountdirfd,
// /*AFLA*/ 				      struct file_handle __user *handle,
// /*AFLA*/ 				      int flags);
// /*AFLA*/ asmlinkage long sys_setns(int fd, int nstype);
// /*AFLA*/ asmlinkage long sys_process_vm_readv(pid_t pid,
// /*AFLA*/ 				     const struct iovec __user *lvec,
// /*AFLA*/ 				     unsigned long liovcnt,
// /*AFLA*/ 				     const struct iovec __user *rvec,
// /*AFLA*/ 				     unsigned long riovcnt,
// /*AFLA*/ 				     unsigned long flags);
// /*AFLA*/ asmlinkage long sys_process_vm_writev(pid_t pid,
// /*AFLA*/ 				      const struct iovec __user *lvec,
// /*AFLA*/ 				      unsigned long liovcnt,
// /*AFLA*/ 				      const struct iovec __user *rvec,
// /*AFLA*/ 				      unsigned long riovcnt,
// /*AFLA*/ 				      unsigned long flags);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_kcmp(pid_t pid1, pid_t pid2, int type,
// /*AFLA*/ 			 unsigned long idx1, unsigned long idx2);
// /*AFLA*/ asmlinkage long sys_finit_module(int fd, const char __user *uargs, int flags);
// /*AFLA*/ asmlinkage long sys_seccomp(unsigned int op, unsigned int flags,
// /*AFLA*/ 			    const char __user *uargs);
// /*AFLA*/ asmlinkage long sys_getrandom(char __user *buf, size_t count,
// /*AFLA*/ 			      unsigned int flags);
// /*AFLA*/ asmlinkage long sys_bpf(int cmd, union bpf_attr *attr, unsigned int size);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_execveat(int dfd, const char __user *filename,
// /*AFLA*/ 			const char __user *const __user *argv,
// /*AFLA*/ 			const char __user *const __user *envp, int flags);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_membarrier(int cmd, int flags);
// /*AFLA*/ asmlinkage long sys_copy_file_range(int fd_in, loff_t __user *off_in,
// /*AFLA*/ 				    int fd_out, loff_t __user *off_out,
// /*AFLA*/ 				    size_t len, unsigned int flags);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_mlock2(unsigned long start, size_t len, int flags);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage long sys_pkey_mprotect(unsigned long start, size_t len,
// /*AFLA*/ 				  unsigned long prot, int pkey);
// /*AFLA*/ asmlinkage long sys_pkey_alloc(unsigned long flags, unsigned long init_val);
// /*AFLA*/ asmlinkage long sys_pkey_free(int pkey);
// /*AFLA*/ 
// /*AFLA*/ #endif
