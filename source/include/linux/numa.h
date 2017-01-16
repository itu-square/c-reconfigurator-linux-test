// /*AFLA*/ #ifndef _LINUX_NUMA_H
// /*AFLA*/ #define _LINUX_NUMA_H
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NODES_SHIFT
// /*AFLA*/ #define NODES_SHIFT     CONFIG_NODES_SHIFT
// /*AFLA*/ #else
// /*AFLA*/ #define NODES_SHIFT     0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define MAX_NUMNODES    (1 << NODES_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ #define	NUMA_NO_NODE	(-1)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_NUMA_H */
