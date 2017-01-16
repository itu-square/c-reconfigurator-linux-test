// /*AFLA*/ /*
// /*AFLA*/  * List of cgroup subsystems.
// /*AFLA*/  *
// /*AFLA*/  * DO NOT ADD ANY SUBSYSTEM WITHOUT EXPLICIT ACKS FROM CGROUP MAINTAINERS.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This file *must* be included with SUBSYS() defined.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #if IS_ENABLED(CONFIG_CPUSETS)
// /*AFLA*/ SUBSYS(cpuset)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if IS_ENABLED(CONFIG_CGROUP_SCHED)
// /*AFLA*/ SUBSYS(cpu)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if IS_ENABLED(CONFIG_CGROUP_CPUACCT)
// /*AFLA*/ SUBSYS(cpuacct)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if IS_ENABLED(CONFIG_BLK_CGROUP)
// /*AFLA*/ SUBSYS(io)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if IS_ENABLED(CONFIG_MEMCG)
// /*AFLA*/ SUBSYS(memory)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if IS_ENABLED(CONFIG_CGROUP_DEVICE)
// /*AFLA*/ SUBSYS(devices)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if IS_ENABLED(CONFIG_CGROUP_FREEZER)
// /*AFLA*/ SUBSYS(freezer)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if IS_ENABLED(CONFIG_CGROUP_NET_CLASSID)
// /*AFLA*/ SUBSYS(net_cls)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if IS_ENABLED(CONFIG_CGROUP_PERF)
// /*AFLA*/ SUBSYS(perf_event)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if IS_ENABLED(CONFIG_CGROUP_NET_PRIO)
// /*AFLA*/ SUBSYS(net_prio)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if IS_ENABLED(CONFIG_CGROUP_HUGETLB)
// /*AFLA*/ SUBSYS(hugetlb)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if IS_ENABLED(CONFIG_CGROUP_PIDS)
// /*AFLA*/ SUBSYS(pids)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The following subsystems are not supported on the default hierarchy.
// /*AFLA*/  */
// /*AFLA*/ #if IS_ENABLED(CONFIG_CGROUP_DEBUG)
// /*AFLA*/ SUBSYS(debug)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * DO NOT ADD ANY SUBSYSTEM WITHOUT EXPLICIT ACKS FROM CGROUP MAINTAINERS.
// /*AFLA*/  */
