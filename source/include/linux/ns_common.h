// /*AFLA*/ #ifndef _LINUX_NS_COMMON_H
// /*AFLA*/ #define _LINUX_NS_COMMON_H
// /*AFLA*/ 
// /*AFLA*/ struct proc_ns_operations;
// /*AFLA*/ 
// /*AFLA*/ struct ns_common {
// /*AFLA*/ 	atomic_long_t stashed;
// /*AFLA*/ 	const struct proc_ns_operations *ops;
// /*AFLA*/ 	unsigned int inum;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif
