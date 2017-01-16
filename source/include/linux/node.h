// /*AFLA*/ /*
// /*AFLA*/  * include/linux/node.h - generic node definition
// /*AFLA*/  *
// /*AFLA*/  * This is mainly for topological representation. We define the 
// /*AFLA*/  * basic 'struct node' here, which can be embedded in per-arch 
// /*AFLA*/  * definitions of processors.
// /*AFLA*/  *
// /*AFLA*/  * Basic handling of the devices is done in drivers/base/node.c
// /*AFLA*/  * and system devices are handled in drivers/base/sys.c. 
// /*AFLA*/  *
// /*AFLA*/  * Nodes are exported via driverfs in the class/node/devices/
// /*AFLA*/  * directory. 
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LINUX_NODE_H_
// /*AFLA*/ #define _LINUX_NODE_H_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/device.h>
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ #include <linux/workqueue.h>
// /*AFLA*/ 
// /*AFLA*/ struct node {
// /*AFLA*/ 	struct device	dev;
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_MEMORY_HOTPLUG_SPARSE) && defined(CONFIG_HUGETLBFS)
// /*AFLA*/ 	struct work_struct	node_work;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct memory_block;
// /*AFLA*/ extern struct node *node_devices[];
// /*AFLA*/ typedef  void (*node_registration_func_t)(struct node *);
// /*AFLA*/ 
// /*AFLA*/ extern void unregister_node(struct node *node);
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ extern int register_one_node(int nid);
// /*AFLA*/ extern void unregister_one_node(int nid);
// /*AFLA*/ extern int register_cpu_under_node(unsigned int cpu, unsigned int nid);
// /*AFLA*/ extern int unregister_cpu_under_node(unsigned int cpu, unsigned int nid);
// /*AFLA*/ extern int register_mem_sect_under_node(struct memory_block *mem_blk,
// /*AFLA*/ 						int nid);
// /*AFLA*/ extern int unregister_mem_sect_under_nodes(struct memory_block *mem_blk,
// /*AFLA*/ 					   unsigned long phys_index);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HUGETLBFS
// /*AFLA*/ extern void register_hugetlbfs_with_node(node_registration_func_t doregister,
// /*AFLA*/ 					 node_registration_func_t unregister);
// /*AFLA*/ #endif
// /*AFLA*/ #else
// /*AFLA*/ static inline int register_one_node(int nid)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int unregister_one_node(int nid)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int register_cpu_under_node(unsigned int cpu, unsigned int nid)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int unregister_cpu_under_node(unsigned int cpu, unsigned int nid)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int register_mem_sect_under_node(struct memory_block *mem_blk,
// /*AFLA*/ 							int nid)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int unregister_mem_sect_under_nodes(struct memory_block *mem_blk,
// /*AFLA*/ 						  unsigned long phys_index)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void register_hugetlbfs_with_node(node_registration_func_t reg,
// /*AFLA*/ 						node_registration_func_t unreg)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define to_node(device) container_of(device, struct node, dev)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_NODE_H_ */
