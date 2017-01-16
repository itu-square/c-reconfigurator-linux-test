// /*AFLA*/ #ifndef _LINUX_SECCOMP_H
// /*AFLA*/ #define _LINUX_SECCOMP_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/seccomp.h>
// /*AFLA*/ 
// /*AFLA*/ #define SECCOMP_FILTER_FLAG_MASK	(SECCOMP_FILTER_FLAG_TSYNC)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SECCOMP
// /*AFLA*/ 
// /*AFLA*/ #include <linux/thread_info.h>
// /*AFLA*/ #include <asm/seccomp.h>
// /*AFLA*/ 
// /*AFLA*/ struct seccomp_filter;
// /*AFLA*/ /**
// /*AFLA*/  * struct seccomp - the state of a seccomp'ed process
// /*AFLA*/  *
// /*AFLA*/  * @mode:  indicates one of the valid values above for controlled
// /*AFLA*/  *         system calls available to a process.
// /*AFLA*/  * @filter: must always point to a valid seccomp-filter or NULL as it is
// /*AFLA*/  *          accessed without locking during system call entry.
// /*AFLA*/  *
// /*AFLA*/  *          @filter must only be accessed from the context of current as there
// /*AFLA*/  *          is no read locking.
// /*AFLA*/  */
// /*AFLA*/ struct seccomp {
// /*AFLA*/ 	int mode;
// /*AFLA*/ 	struct seccomp_filter *filter;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_ARCH_SECCOMP_FILTER
// /*AFLA*/ extern int __secure_computing(const struct seccomp_data *sd);
// /*AFLA*/ static inline int secure_computing(const struct seccomp_data *sd)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(test_thread_flag(TIF_SECCOMP)))
// /*AFLA*/ 		return  __secure_computing(sd);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ extern void secure_computing_strict(int this_syscall);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern long prctl_get_seccomp(void);
// /*AFLA*/ extern long prctl_set_seccomp(unsigned long, char __user *);
// /*AFLA*/ 
// /*AFLA*/ static inline int seccomp_mode(struct seccomp *s)
// /*AFLA*/ {
// /*AFLA*/ 	return s->mode;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_SECCOMP */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ 
// /*AFLA*/ struct seccomp { };
// /*AFLA*/ struct seccomp_filter { };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_ARCH_SECCOMP_FILTER
// /*AFLA*/ static inline int secure_computing(struct seccomp_data *sd) { return 0; }
// /*AFLA*/ #else
// /*AFLA*/ static inline void secure_computing_strict(int this_syscall) { return; }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline long prctl_get_seccomp(void)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline long prctl_set_seccomp(unsigned long arg2, char __user *arg3)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int seccomp_mode(struct seccomp *s)
// /*AFLA*/ {
// /*AFLA*/ 	return SECCOMP_MODE_DISABLED;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_SECCOMP */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SECCOMP_FILTER
// /*AFLA*/ extern void put_seccomp_filter(struct task_struct *tsk);
// /*AFLA*/ extern void get_seccomp_filter(struct task_struct *tsk);
// /*AFLA*/ #else  /* CONFIG_SECCOMP_FILTER */
// /*AFLA*/ static inline void put_seccomp_filter(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ static inline void get_seccomp_filter(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_SECCOMP_FILTER */
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_SECCOMP_FILTER) && defined(CONFIG_CHECKPOINT_RESTORE)
// /*AFLA*/ extern long seccomp_get_filter(struct task_struct *task,
// /*AFLA*/ 			       unsigned long filter_off, void __user *data);
// /*AFLA*/ #else
// /*AFLA*/ static inline long seccomp_get_filter(struct task_struct *task,
// /*AFLA*/ 				      unsigned long n, void __user *data)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_SECCOMP_FILTER && CONFIG_CHECKPOINT_RESTORE */
// /*AFLA*/ #endif /* _LINUX_SECCOMP_H */
