// /*AFLA*/ #ifndef _LINUX_KCOV_H
// /*AFLA*/ #define _LINUX_KCOV_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/kcov.h>
// /*AFLA*/ 
// /*AFLA*/ struct task_struct;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KCOV
// /*AFLA*/ 
// /*AFLA*/ void kcov_task_init(struct task_struct *t);
// /*AFLA*/ void kcov_task_exit(struct task_struct *t);
// /*AFLA*/ 
// /*AFLA*/ enum kcov_mode {
// /*AFLA*/ 	/* Coverage collection is not enabled yet. */
// /*AFLA*/ 	KCOV_MODE_DISABLED = 0,
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Tracing coverage collection mode.
// /*AFLA*/ 	 * Covered PCs are collected in a per-task buffer.
// /*AFLA*/ 	 */
// /*AFLA*/ 	KCOV_MODE_TRACE = 1,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline void kcov_task_init(struct task_struct *t) {}
// /*AFLA*/ static inline void kcov_task_exit(struct task_struct *t) {}
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_KCOV */
// /*AFLA*/ #endif /* _LINUX_KCOV_H */
