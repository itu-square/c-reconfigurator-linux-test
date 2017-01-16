// /*AFLA*/ /*
// /*AFLA*/  * linux/include/asm-generic/topology.h
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
// /*AFLA*/ #ifndef _ASM_GENERIC_TOPOLOGY_H
// /*AFLA*/ #define _ASM_GENERIC_TOPOLOGY_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef	CONFIG_NUMA
// /*AFLA*/ 
// /*AFLA*/ /* Other architectures wishing to use this simple topology API should fill
// /*AFLA*/    in the below functions as appropriate in their own <asm/topology.h> file. */
// /*AFLA*/ #ifndef cpu_to_node
// /*AFLA*/ #define cpu_to_node(cpu)	((void)(cpu),0)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef set_numa_node
// /*AFLA*/ #define set_numa_node(node)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef set_cpu_numa_node
// /*AFLA*/ #define set_cpu_numa_node(cpu, node)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef cpu_to_mem
// /*AFLA*/ #define cpu_to_mem(cpu)		((void)(cpu),0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef parent_node
// /*AFLA*/ #define parent_node(node)	((void)(node),0)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef cpumask_of_node
// /*AFLA*/ #define cpumask_of_node(node)	((void)node, cpu_online_mask)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef pcibus_to_node
// /*AFLA*/ #define pcibus_to_node(bus)	((void)(bus), -1)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef cpumask_of_pcibus
// /*AFLA*/ #define cpumask_of_pcibus(bus)	(pcibus_to_node(bus) == -1 ?		\
// /*AFLA*/ 				 cpu_all_mask :				\
// /*AFLA*/ 				 cpumask_of_node(pcibus_to_node(bus)))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif	/* CONFIG_NUMA */
// /*AFLA*/ 
// /*AFLA*/ #if !defined(CONFIG_NUMA) || !defined(CONFIG_HAVE_MEMORYLESS_NODES)
// /*AFLA*/ 
// /*AFLA*/ #ifndef set_numa_mem
// /*AFLA*/ #define set_numa_mem(node)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef set_cpu_numa_mem
// /*AFLA*/ #define set_cpu_numa_mem(cpu, node)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif	/* !CONFIG_NUMA || !CONFIG_HAVE_MEMORYLESS_NODES */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_GENERIC_TOPOLOGY_H */
