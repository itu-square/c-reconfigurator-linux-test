// /*AFLA*/ #ifndef _LINUX_RESOURCE_H
// /*AFLA*/ #define _LINUX_RESOURCE_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/resource.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ struct task_struct;
// /*AFLA*/ 
// /*AFLA*/ int getrusage(struct task_struct *p, int who, struct rusage __user *ru);
// /*AFLA*/ int do_prlimit(struct task_struct *tsk, unsigned int resource,
// /*AFLA*/ 		struct rlimit *new_rlim, struct rlimit *old_rlim);
// /*AFLA*/ 
// /*AFLA*/ #endif
