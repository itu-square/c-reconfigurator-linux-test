// /*AFLA*/ /*
// /*AFLA*/  * Written by: Matthew Dobson, IBM Corporation
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2002, IBM Corp.
// /*AFLA*/  *
// /*AFLA*/  * All rights reserved.
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify
// /*AFLA*/  * it under the terms of the GNU General Public License as published by
// /*AFLA*/  * the Free Software Foundation; either version 2 of the License, or
// /*AFLA*/  * (at your option) any later version.
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it will be useful, but
// /*AFLA*/  * WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
// /*AFLA*/  * NON INFRINGEMENT.  See the GNU General Public License for more
// /*AFLA*/  * details.
// /*AFLA*/  *
// /*AFLA*/  * You should have received a copy of the GNU General Public License
// /*AFLA*/  * along with this program; if not, write to the Free Software
// /*AFLA*/  * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
// /*AFLA*/  *
// /*AFLA*/  * Send feedback to <colpatch@us.ibm.com>
// /*AFLA*/  */
// /*AFLA*/ #ifndef _ASM_X86_TOPOLOGY_H
// /*AFLA*/ #define _ASM_X86_TOPOLOGY_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * to preserve the visibility of NUMA_NO_NODE definition,
// /*AFLA*/  * moved to there from here.  May be used independent of
// /*AFLA*/  * CONFIG_NUMA.
// /*AFLA*/  */
// /*AFLA*/ #include <linux/numa.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/mpspec.h>
// /*AFLA*/ #include <asm/percpu.h>
// /*AFLA*/ 
// /*AFLA*/ /* Mappings between logical cpu number and node number */
// /*AFLA*/ DECLARE_EARLY_PER_CPU(int, x86_cpu_to_node_map);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_PER_CPU_MAPS
// /*AFLA*/ /*
// /*AFLA*/  * override generic percpu implementation of cpu_to_node
// /*AFLA*/  */
// /*AFLA*/ extern int __cpu_to_node(int cpu);
// /*AFLA*/ #define cpu_to_node __cpu_to_node
// /*AFLA*/ 
// /*AFLA*/ extern int early_cpu_to_node(int cpu);
// /*AFLA*/ 
// /*AFLA*/ #else	/* !CONFIG_DEBUG_PER_CPU_MAPS */
// /*AFLA*/ 
// /*AFLA*/ /* Same function but used if called before per_cpu areas are setup */
// /*AFLA*/ static inline int early_cpu_to_node(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return early_per_cpu(x86_cpu_to_node_map, cpu);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* !CONFIG_DEBUG_PER_CPU_MAPS */
// /*AFLA*/ 
// /*AFLA*/ /* Mappings between node number and cpus on that node. */
// /*AFLA*/ extern cpumask_var_t node_to_cpumask_map[MAX_NUMNODES];
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_PER_CPU_MAPS
// /*AFLA*/ extern const struct cpumask *cpumask_of_node(int node);
// /*AFLA*/ #else
// /*AFLA*/ /* Returns a pointer to the cpumask of CPUs on Node 'node'. */
// /*AFLA*/ static inline const struct cpumask *cpumask_of_node(int node)
// /*AFLA*/ {
// /*AFLA*/ 	return node_to_cpumask_map[node];
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void setup_node_to_cpumask_map(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Returns the number of the node containing Node 'node'. This
// /*AFLA*/  * architecture is flat, so it is a pretty simple function!
// /*AFLA*/  */
// /*AFLA*/ #define parent_node(node) (node)
// /*AFLA*/ 
// /*AFLA*/ #define pcibus_to_node(bus) __pcibus_to_node(bus)
// /*AFLA*/ 
// /*AFLA*/ extern int __node_distance(int, int);
// /*AFLA*/ #define node_distance(a, b) __node_distance(a, b)
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_NUMA */
// /*AFLA*/ 
// /*AFLA*/ static inline int numa_node_id(void)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ /*
// /*AFLA*/  * indicate override:
// /*AFLA*/  */
// /*AFLA*/ #define numa_node_id numa_node_id
// /*AFLA*/ 
// /*AFLA*/ static inline int early_cpu_to_node(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void setup_node_to_cpumask_map(void) { }
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/topology.h>
// /*AFLA*/ 
// /*AFLA*/ extern const struct cpumask *cpu_coregroup_mask(int cpu);
// /*AFLA*/ 
// /*AFLA*/ #define topology_logical_package_id(cpu)	(cpu_data(cpu).logical_proc_id)
// /*AFLA*/ #define topology_physical_package_id(cpu)	(cpu_data(cpu).phys_proc_id)
// /*AFLA*/ #define topology_core_id(cpu)			(cpu_data(cpu).cpu_core_id)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ #define topology_core_cpumask(cpu)		(per_cpu(cpu_core_map, cpu))
// /*AFLA*/ #define topology_sibling_cpumask(cpu)		(per_cpu(cpu_sibling_map, cpu))
// /*AFLA*/ 
// /*AFLA*/ extern unsigned int __max_logical_packages;
// /*AFLA*/ #define topology_max_packages()			(__max_logical_packages)
// /*AFLA*/ 
// /*AFLA*/ extern int __max_smt_threads;
// /*AFLA*/ 
// /*AFLA*/ static inline int topology_max_smt_threads(void)
// /*AFLA*/ {
// /*AFLA*/ 	return __max_smt_threads;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ int topology_update_package_map(unsigned int apicid, unsigned int cpu);
// /*AFLA*/ extern int topology_phys_to_logical_pkg(unsigned int pkg);
// /*AFLA*/ #else
// /*AFLA*/ #define topology_max_packages()			(1)
// /*AFLA*/ static inline int
// /*AFLA*/ topology_update_package_map(unsigned int apicid, unsigned int cpu) { return 0; }
// /*AFLA*/ static inline int topology_phys_to_logical_pkg(unsigned int pkg) { return 0; }
// /*AFLA*/ static inline int topology_max_smt_threads(void) { return 1; }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline void arch_fix_phys_package_id(int num, u32 slot)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct pci_bus;
// /*AFLA*/ int x86_pci_root_bus_node(int bus);
// /*AFLA*/ void x86_pci_root_bus_resources(int bus, struct list_head *resources);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_TOPOLOGY_H */
