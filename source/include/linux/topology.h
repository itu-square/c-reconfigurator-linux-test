// /*AFLA*/ /*
// /*AFLA*/  * include/linux/topology.h
// /*AFLA*/  *
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
// /*AFLA*/ #ifndef _LINUX_TOPOLOGY_H
// /*AFLA*/ #define _LINUX_TOPOLOGY_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ #include <linux/bitops.h>
// /*AFLA*/ #include <linux/mmzone.h>
// /*AFLA*/ #include <linux/smp.h>
// /*AFLA*/ #include <linux/percpu.h>
// /*AFLA*/ #include <asm/topology.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef nr_cpus_node
// /*AFLA*/ #define nr_cpus_node(node) cpumask_weight(cpumask_of_node(node))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define for_each_node_with_cpus(node)			\
// /*AFLA*/ 	for_each_online_node(node)			\
// /*AFLA*/ 		if (nr_cpus_node(node))
// /*AFLA*/ 
// /*AFLA*/ int arch_update_cpu_topology(void);
// /*AFLA*/ 
// /*AFLA*/ /* Conform to ACPI 2.0 SLIT distance definitions */
// /*AFLA*/ #define LOCAL_DISTANCE		10
// /*AFLA*/ #define REMOTE_DISTANCE		20
// /*AFLA*/ #ifndef node_distance
// /*AFLA*/ #define node_distance(from,to)	((from) == (to) ? LOCAL_DISTANCE : REMOTE_DISTANCE)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef RECLAIM_DISTANCE
// /*AFLA*/ /*
// /*AFLA*/  * If the distance between nodes in a system is larger than RECLAIM_DISTANCE
// /*AFLA*/  * (in whatever arch specific measurement units returned by node_distance())
// /*AFLA*/  * and node_reclaim_mode is enabled then the VM will only call node_reclaim()
// /*AFLA*/  * on nodes within this distance.
// /*AFLA*/  */
// /*AFLA*/ #define RECLAIM_DISTANCE 30
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef PENALTY_FOR_NODE_WITH_CPUS
// /*AFLA*/ #define PENALTY_FOR_NODE_WITH_CPUS	(1)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_USE_PERCPU_NUMA_NODE_ID
// /*AFLA*/ DECLARE_PER_CPU(int, numa_node);
// /*AFLA*/ 
// /*AFLA*/ #ifndef numa_node_id
// /*AFLA*/ /* Returns the number of the current Node. */
// /*AFLA*/ static inline int numa_node_id(void)
// /*AFLA*/ {
// /*AFLA*/ 	return raw_cpu_read(numa_node);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef cpu_to_node
// /*AFLA*/ static inline int cpu_to_node(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return per_cpu(numa_node, cpu);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef set_numa_node
// /*AFLA*/ static inline void set_numa_node(int node)
// /*AFLA*/ {
// /*AFLA*/ 	this_cpu_write(numa_node, node);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef set_cpu_numa_node
// /*AFLA*/ static inline void set_cpu_numa_node(int cpu, int node)
// /*AFLA*/ {
// /*AFLA*/ 	per_cpu(numa_node, cpu) = node;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #else	/* !CONFIG_USE_PERCPU_NUMA_NODE_ID */
// /*AFLA*/ 
// /*AFLA*/ /* Returns the number of the current Node. */
// /*AFLA*/ #ifndef numa_node_id
// /*AFLA*/ static inline int numa_node_id(void)
// /*AFLA*/ {
// /*AFLA*/ 	return cpu_to_node(raw_smp_processor_id());
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif	/* [!]CONFIG_USE_PERCPU_NUMA_NODE_ID */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_MEMORYLESS_NODES
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * N.B., Do NOT reference the '_numa_mem_' per cpu variable directly.
// /*AFLA*/  * It will not be defined when CONFIG_HAVE_MEMORYLESS_NODES is not defined.
// /*AFLA*/  * Use the accessor functions set_numa_mem(), numa_mem_id() and cpu_to_mem().
// /*AFLA*/  */
// /*AFLA*/ DECLARE_PER_CPU(int, _numa_mem_);
// /*AFLA*/ extern int _node_numa_mem_[MAX_NUMNODES];
// /*AFLA*/ 
// /*AFLA*/ #ifndef set_numa_mem
// /*AFLA*/ static inline void set_numa_mem(int node)
// /*AFLA*/ {
// /*AFLA*/ 	this_cpu_write(_numa_mem_, node);
// /*AFLA*/ 	_node_numa_mem_[numa_node_id()] = node;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef node_to_mem_node
// /*AFLA*/ static inline int node_to_mem_node(int node)
// /*AFLA*/ {
// /*AFLA*/ 	return _node_numa_mem_[node];
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef numa_mem_id
// /*AFLA*/ /* Returns the number of the nearest Node with memory */
// /*AFLA*/ static inline int numa_mem_id(void)
// /*AFLA*/ {
// /*AFLA*/ 	return raw_cpu_read(_numa_mem_);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef cpu_to_mem
// /*AFLA*/ static inline int cpu_to_mem(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return per_cpu(_numa_mem_, cpu);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef set_cpu_numa_mem
// /*AFLA*/ static inline void set_cpu_numa_mem(int cpu, int node)
// /*AFLA*/ {
// /*AFLA*/ 	per_cpu(_numa_mem_, cpu) = node;
// /*AFLA*/ 	_node_numa_mem_[cpu_to_node(cpu)] = node;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #else	/* !CONFIG_HAVE_MEMORYLESS_NODES */
// /*AFLA*/ 
// /*AFLA*/ #ifndef numa_mem_id
// /*AFLA*/ /* Returns the number of the nearest Node with memory */
// /*AFLA*/ static inline int numa_mem_id(void)
// /*AFLA*/ {
// /*AFLA*/ 	return numa_node_id();
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef node_to_mem_node
// /*AFLA*/ static inline int node_to_mem_node(int node)
// /*AFLA*/ {
// /*AFLA*/ 	return node;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef cpu_to_mem
// /*AFLA*/ static inline int cpu_to_mem(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return cpu_to_node(cpu);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif	/* [!]CONFIG_HAVE_MEMORYLESS_NODES */
// /*AFLA*/ 
// /*AFLA*/ #ifndef topology_physical_package_id
// /*AFLA*/ #define topology_physical_package_id(cpu)	((void)(cpu), -1)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef topology_core_id
// /*AFLA*/ #define topology_core_id(cpu)			((void)(cpu), 0)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef topology_sibling_cpumask
// /*AFLA*/ #define topology_sibling_cpumask(cpu)		cpumask_of(cpu)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef topology_core_cpumask
// /*AFLA*/ #define topology_core_cpumask(cpu)		cpumask_of(cpu)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SCHED_SMT
// /*AFLA*/ static inline const struct cpumask *cpu_smt_mask(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return topology_sibling_cpumask(cpu);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline const struct cpumask *cpu_cpu_mask(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return cpumask_of_node(cpu_to_node(cpu));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_TOPOLOGY_H */
