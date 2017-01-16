// /*AFLA*/ #ifndef _LINUX_SEM_H
// /*AFLA*/ #define _LINUX_SEM_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ #include <linux/cache.h>
// /*AFLA*/ #include <uapi/linux/sem.h>
// /*AFLA*/ 
// /*AFLA*/ struct task_struct;
// /*AFLA*/ 
// /*AFLA*/ /* One sem_array data structure for each set of semaphores in the system. */
// /*AFLA*/ struct sem_array {
// /*AFLA*/ 	struct kern_ipc_perm	____cacheline_aligned_in_smp
// /*AFLA*/ 				sem_perm;	/* permissions .. see ipc.h */
// /*AFLA*/ 	time_t			sem_ctime;	/* last change time */
// /*AFLA*/ 	struct sem		*sem_base;	/* ptr to first semaphore in array */
// /*AFLA*/ 	struct list_head	pending_alter;	/* pending operations */
// /*AFLA*/ 						/* that alter the array */
// /*AFLA*/ 	struct list_head	pending_const;	/* pending complex operations */
// /*AFLA*/ 						/* that do not alter semvals */
// /*AFLA*/ 	struct list_head	list_id;	/* undo requests on this array */
// /*AFLA*/ 	int			sem_nsems;	/* no. of semaphores in array */
// /*AFLA*/ 	int			complex_count;	/* pending complex operations */
// /*AFLA*/ 	bool			complex_mode;	/* no parallel simple ops */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SYSVIPC
// /*AFLA*/ 
// /*AFLA*/ struct sysv_sem {
// /*AFLA*/ 	struct sem_undo_list *undo_list;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern int copy_semundo(unsigned long clone_flags, struct task_struct *tsk);
// /*AFLA*/ extern void exit_sem(struct task_struct *tsk);
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ struct sysv_sem {
// /*AFLA*/ 	/* empty */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline int copy_semundo(unsigned long clone_flags, struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void exit_sem(struct task_struct *tsk)
// /*AFLA*/ {
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_SEM_H */
