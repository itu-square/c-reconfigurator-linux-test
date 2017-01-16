// /*AFLA*/ #ifndef _ASM_X86_NUMA_H
// /*AFLA*/ #define _ASM_X86_NUMA_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/nodemask.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/topology.h>
// /*AFLA*/ #include <asm/apicdef.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ 
// /*AFLA*/ #define NR_NODE_MEMBLKS		(MAX_NUMNODES*2)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Too small node sizes may confuse the VM badly. Usually they
// /*AFLA*/  * result from BIOS bugs. So dont recognize nodes as standalone
// /*AFLA*/  * NUMA entities that have less than this amount of RAM listed:
// /*AFLA*/  */
// /*AFLA*/ #define NODE_MIN_SIZE (4*1024*1024)
// /*AFLA*/ 
// /*AFLA*/ extern int numa_off;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * __apicid_to_node[] stores the raw mapping between physical apicid and
// /*AFLA*/  * node and is used to initialize cpu_to_node mapping.
// /*AFLA*/  *
// /*AFLA*/  * The mapping may be overridden by apic->numa_cpu_node() on 32bit and thus
// /*AFLA*/  * should be accessed by the accessors - set_apicid_to_node() and
// /*AFLA*/  * numa_cpu_node().
// /*AFLA*/  */
// /*AFLA*/ extern s16 __apicid_to_node[MAX_LOCAL_APIC];
// /*AFLA*/ extern nodemask_t numa_nodes_parsed __initdata;
// /*AFLA*/ 
// /*AFLA*/ extern int __init numa_add_memblk(int nodeid, u64 start, u64 end);
// /*AFLA*/ extern void __init numa_set_distance(int from, int to, int distance);
// /*AFLA*/ 
// /*AFLA*/ static inline void set_apicid_to_node(int apicid, s16 node)
// /*AFLA*/ {
// /*AFLA*/ 	__apicid_to_node[apicid] = node;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int numa_cpu_node(int cpu);
// /*AFLA*/ 
// /*AFLA*/ #else	/* CONFIG_NUMA */
// /*AFLA*/ static inline void set_apicid_to_node(int apicid, s16 node)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int numa_cpu_node(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return NUMA_NO_NODE;
// /*AFLA*/ }
// /*AFLA*/ #endif	/* CONFIG_NUMA */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ # include <asm/numa_32.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ extern void numa_set_node(int cpu, int node);
// /*AFLA*/ extern void numa_clear_node(int cpu);
// /*AFLA*/ extern void __init init_cpu_to_node(void);
// /*AFLA*/ extern void numa_add_cpu(int cpu);
// /*AFLA*/ extern void numa_remove_cpu(int cpu);
// /*AFLA*/ #else	/* CONFIG_NUMA */
// /*AFLA*/ static inline void numa_set_node(int cpu, int node)	{ }
// /*AFLA*/ static inline void numa_clear_node(int cpu)		{ }
// /*AFLA*/ static inline void init_cpu_to_node(void)		{ }
// /*AFLA*/ static inline void numa_add_cpu(int cpu)		{ }
// /*AFLA*/ static inline void numa_remove_cpu(int cpu)		{ }
// /*AFLA*/ #endif	/* CONFIG_NUMA */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_PER_CPU_MAPS
// /*AFLA*/ void debug_cpumask_set_cpu(int cpu, int node, bool enable);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA_EMU
// /*AFLA*/ #define FAKE_NODE_MIN_SIZE	((u64)32 << 20)
// /*AFLA*/ #define FAKE_NODE_MIN_HASH_MASK	(~(FAKE_NODE_MIN_SIZE - 1UL))
// /*AFLA*/ void numa_emu_cmdline(char *);
// /*AFLA*/ #endif /* CONFIG_NUMA_EMU */
// /*AFLA*/ 
// /*AFLA*/ #endif	/* _ASM_X86_NUMA_H */
