// /*AFLA*/ #ifndef __CPUHOTPLUG_H
// /*AFLA*/ #define __CPUHOTPLUG_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ enum cpuhp_state {
// /*AFLA*/ 	CPUHP_OFFLINE,
// /*AFLA*/ 	CPUHP_CREATE_THREADS,
// /*AFLA*/ 	CPUHP_PERF_PREPARE,
// /*AFLA*/ 	CPUHP_PERF_X86_PREPARE,
// /*AFLA*/ 	CPUHP_PERF_X86_UNCORE_PREP,
// /*AFLA*/ 	CPUHP_PERF_X86_AMD_UNCORE_PREP,
// /*AFLA*/ 	CPUHP_PERF_X86_RAPL_PREP,
// /*AFLA*/ 	CPUHP_PERF_BFIN,
// /*AFLA*/ 	CPUHP_PERF_POWER,
// /*AFLA*/ 	CPUHP_PERF_SUPERH,
// /*AFLA*/ 	CPUHP_X86_HPET_DEAD,
// /*AFLA*/ 	CPUHP_X86_APB_DEAD,
// /*AFLA*/ 	CPUHP_VIRT_NET_DEAD,
// /*AFLA*/ 	CPUHP_SLUB_DEAD,
// /*AFLA*/ 	CPUHP_MM_WRITEBACK_DEAD,
// /*AFLA*/ 	CPUHP_SOFTIRQ_DEAD,
// /*AFLA*/ 	CPUHP_NET_MVNETA_DEAD,
// /*AFLA*/ 	CPUHP_CPUIDLE_DEAD,
// /*AFLA*/ 	CPUHP_ARM64_FPSIMD_DEAD,
// /*AFLA*/ 	CPUHP_ARM_OMAP_WAKE_DEAD,
// /*AFLA*/ 	CPUHP_IRQ_POLL_DEAD,
// /*AFLA*/ 	CPUHP_BLOCK_SOFTIRQ_DEAD,
// /*AFLA*/ 	CPUHP_VIRT_SCSI_DEAD,
// /*AFLA*/ 	CPUHP_ACPI_CPUDRV_DEAD,
// /*AFLA*/ 	CPUHP_S390_PFAULT_DEAD,
// /*AFLA*/ 	CPUHP_BLK_MQ_DEAD,
// /*AFLA*/ 	CPUHP_WORKQUEUE_PREP,
// /*AFLA*/ 	CPUHP_POWER_NUMA_PREPARE,
// /*AFLA*/ 	CPUHP_HRTIMERS_PREPARE,
// /*AFLA*/ 	CPUHP_PROFILE_PREPARE,
// /*AFLA*/ 	CPUHP_X2APIC_PREPARE,
// /*AFLA*/ 	CPUHP_SMPCFD_PREPARE,
// /*AFLA*/ 	CPUHP_RELAY_PREPARE,
// /*AFLA*/ 	CPUHP_SLAB_PREPARE,
// /*AFLA*/ 	CPUHP_MD_RAID5_PREPARE,
// /*AFLA*/ 	CPUHP_RCUTREE_PREP,
// /*AFLA*/ 	CPUHP_CPUIDLE_COUPLED_PREPARE,
// /*AFLA*/ 	CPUHP_POWERPC_PMAC_PREPARE,
// /*AFLA*/ 	CPUHP_POWERPC_MMU_CTX_PREPARE,
// /*AFLA*/ 	CPUHP_XEN_PREPARE,
// /*AFLA*/ 	CPUHP_XEN_EVTCHN_PREPARE,
// /*AFLA*/ 	CPUHP_NOTIFY_PREPARE,
// /*AFLA*/ 	CPUHP_ARM_SHMOBILE_SCU_PREPARE,
// /*AFLA*/ 	CPUHP_SH_SH3X_PREPARE,
// /*AFLA*/ 	CPUHP_BLK_MQ_PREPARE,
// /*AFLA*/ 	CPUHP_TIMERS_DEAD,
// /*AFLA*/ 	CPUHP_NOTF_ERR_INJ_PREPARE,
// /*AFLA*/ 	CPUHP_MIPS_SOC_PREPARE,
// /*AFLA*/ 	CPUHP_BRINGUP_CPU,
// /*AFLA*/ 	CPUHP_AP_IDLE_DEAD,
// /*AFLA*/ 	CPUHP_AP_OFFLINE,
// /*AFLA*/ 	CPUHP_AP_SCHED_STARTING,
// /*AFLA*/ 	CPUHP_AP_RCUTREE_DYING,
// /*AFLA*/ 	CPUHP_AP_IRQ_GIC_STARTING,
// /*AFLA*/ 	CPUHP_AP_IRQ_GICV3_STARTING,
// /*AFLA*/ 	CPUHP_AP_IRQ_HIP04_STARTING,
// /*AFLA*/ 	CPUHP_AP_IRQ_ARMADA_XP_STARTING,
// /*AFLA*/ 	CPUHP_AP_IRQ_ARMADA_CASC_STARTING,
// /*AFLA*/ 	CPUHP_AP_IRQ_BCM2836_STARTING,
// /*AFLA*/ 	CPUHP_AP_ARM_MVEBU_COHERENCY,
// /*AFLA*/ 	CPUHP_AP_PERF_X86_UNCORE_STARTING,
// /*AFLA*/ 	CPUHP_AP_PERF_X86_AMD_UNCORE_STARTING,
// /*AFLA*/ 	CPUHP_AP_PERF_X86_STARTING,
// /*AFLA*/ 	CPUHP_AP_PERF_X86_AMD_IBS_STARTING,
// /*AFLA*/ 	CPUHP_AP_PERF_X86_CQM_STARTING,
// /*AFLA*/ 	CPUHP_AP_PERF_X86_CSTATE_STARTING,
// /*AFLA*/ 	CPUHP_AP_PERF_XTENSA_STARTING,
// /*AFLA*/ 	CPUHP_AP_PERF_METAG_STARTING,
// /*AFLA*/ 	CPUHP_AP_MIPS_OP_LOONGSON3_STARTING,
// /*AFLA*/ 	CPUHP_AP_ARM_VFP_STARTING,
// /*AFLA*/ 	CPUHP_AP_ARM64_DEBUG_MONITORS_STARTING,
// /*AFLA*/ 	CPUHP_AP_PERF_ARM_HW_BREAKPOINT_STARTING,
// /*AFLA*/ 	CPUHP_AP_PERF_ARM_STARTING,
// /*AFLA*/ 	CPUHP_AP_ARM_L2X0_STARTING,
// /*AFLA*/ 	CPUHP_AP_ARM_ARCH_TIMER_STARTING,
// /*AFLA*/ 	CPUHP_AP_ARM_GLOBAL_TIMER_STARTING,
// /*AFLA*/ 	CPUHP_AP_DUMMY_TIMER_STARTING,
// /*AFLA*/ 	CPUHP_AP_JCORE_TIMER_STARTING,
// /*AFLA*/ 	CPUHP_AP_EXYNOS4_MCT_TIMER_STARTING,
// /*AFLA*/ 	CPUHP_AP_ARM_TWD_STARTING,
// /*AFLA*/ 	CPUHP_AP_METAG_TIMER_STARTING,
// /*AFLA*/ 	CPUHP_AP_QCOM_TIMER_STARTING,
// /*AFLA*/ 	CPUHP_AP_ARMADA_TIMER_STARTING,
// /*AFLA*/ 	CPUHP_AP_MARCO_TIMER_STARTING,
// /*AFLA*/ 	CPUHP_AP_MIPS_GIC_TIMER_STARTING,
// /*AFLA*/ 	CPUHP_AP_ARC_TIMER_STARTING,
// /*AFLA*/ 	CPUHP_AP_KVM_STARTING,
// /*AFLA*/ 	CPUHP_AP_KVM_ARM_VGIC_INIT_STARTING,
// /*AFLA*/ 	CPUHP_AP_KVM_ARM_VGIC_STARTING,
// /*AFLA*/ 	CPUHP_AP_KVM_ARM_TIMER_STARTING,
// /*AFLA*/ 	CPUHP_AP_ARM_XEN_STARTING,
// /*AFLA*/ 	CPUHP_AP_ARM_CORESIGHT_STARTING,
// /*AFLA*/ 	CPUHP_AP_ARM_CORESIGHT4_STARTING,
// /*AFLA*/ 	CPUHP_AP_ARM64_ISNDEP_STARTING,
// /*AFLA*/ 	CPUHP_AP_SMPCFD_DYING,
// /*AFLA*/ 	CPUHP_AP_X86_TBOOT_DYING,
// /*AFLA*/ 	CPUHP_AP_ONLINE,
// /*AFLA*/ 	CPUHP_TEARDOWN_CPU,
// /*AFLA*/ 	CPUHP_AP_ONLINE_IDLE,
// /*AFLA*/ 	CPUHP_AP_SMPBOOT_THREADS,
// /*AFLA*/ 	CPUHP_AP_X86_VDSO_VMA_ONLINE,
// /*AFLA*/ 	CPUHP_AP_PERF_ONLINE,
// /*AFLA*/ 	CPUHP_AP_PERF_X86_ONLINE,
// /*AFLA*/ 	CPUHP_AP_PERF_X86_UNCORE_ONLINE,
// /*AFLA*/ 	CPUHP_AP_PERF_X86_AMD_UNCORE_ONLINE,
// /*AFLA*/ 	CPUHP_AP_PERF_X86_AMD_POWER_ONLINE,
// /*AFLA*/ 	CPUHP_AP_PERF_X86_RAPL_ONLINE,
// /*AFLA*/ 	CPUHP_AP_PERF_X86_CQM_ONLINE,
// /*AFLA*/ 	CPUHP_AP_PERF_X86_CSTATE_ONLINE,
// /*AFLA*/ 	CPUHP_AP_PERF_S390_CF_ONLINE,
// /*AFLA*/ 	CPUHP_AP_PERF_S390_SF_ONLINE,
// /*AFLA*/ 	CPUHP_AP_PERF_ARM_CCI_ONLINE,
// /*AFLA*/ 	CPUHP_AP_PERF_ARM_CCN_ONLINE,
// /*AFLA*/ 	CPUHP_AP_PERF_ARM_L2X0_ONLINE,
// /*AFLA*/ 	CPUHP_AP_WORKQUEUE_ONLINE,
// /*AFLA*/ 	CPUHP_AP_RCUTREE_ONLINE,
// /*AFLA*/ 	CPUHP_AP_NOTIFY_ONLINE,
// /*AFLA*/ 	CPUHP_AP_ONLINE_DYN,
// /*AFLA*/ 	CPUHP_AP_ONLINE_DYN_END		= CPUHP_AP_ONLINE_DYN + 30,
// /*AFLA*/ 	CPUHP_AP_X86_HPET_ONLINE,
// /*AFLA*/ 	CPUHP_AP_X86_KVM_CLK_ONLINE,
// /*AFLA*/ 	CPUHP_AP_ACTIVE,
// /*AFLA*/ 	CPUHP_ONLINE,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ int __cpuhp_setup_state(enum cpuhp_state state,	const char *name, bool invoke,
// /*AFLA*/ 			int (*startup)(unsigned int cpu),
// /*AFLA*/ 			int (*teardown)(unsigned int cpu), bool multi_instance);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpuhp_setup_state - Setup hotplug state callbacks with calling the callbacks
// /*AFLA*/  * @state:	The state for which the calls are installed
// /*AFLA*/  * @name:	Name of the callback (will be used in debug output)
// /*AFLA*/  * @startup:	startup callback function
// /*AFLA*/  * @teardown:	teardown callback function
// /*AFLA*/  *
// /*AFLA*/  * Installs the callback functions and invokes the startup callback on
// /*AFLA*/  * the present cpus which have already reached the @state.
// /*AFLA*/  */
// /*AFLA*/ static inline int cpuhp_setup_state(enum cpuhp_state state,
// /*AFLA*/ 				    const char *name,
// /*AFLA*/ 				    int (*startup)(unsigned int cpu),
// /*AFLA*/ 				    int (*teardown)(unsigned int cpu))
// /*AFLA*/ {
// /*AFLA*/ 	return __cpuhp_setup_state(state, name, true, startup, teardown, false);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpuhp_setup_state_nocalls - Setup hotplug state callbacks without calling the
// /*AFLA*/  *			       callbacks
// /*AFLA*/  * @state:	The state for which the calls are installed
// /*AFLA*/  * @name:	Name of the callback.
// /*AFLA*/  * @startup:	startup callback function
// /*AFLA*/  * @teardown:	teardown callback function
// /*AFLA*/  *
// /*AFLA*/  * Same as @cpuhp_setup_state except that no calls are executed are invoked
// /*AFLA*/  * during installation of this callback. NOP if SMP=n or HOTPLUG_CPU=n.
// /*AFLA*/  */
// /*AFLA*/ static inline int cpuhp_setup_state_nocalls(enum cpuhp_state state,
// /*AFLA*/ 					    const char *name,
// /*AFLA*/ 					    int (*startup)(unsigned int cpu),
// /*AFLA*/ 					    int (*teardown)(unsigned int cpu))
// /*AFLA*/ {
// /*AFLA*/ 	return __cpuhp_setup_state(state, name, false, startup, teardown,
// /*AFLA*/ 				   false);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpuhp_setup_state_multi - Add callbacks for multi state
// /*AFLA*/  * @state:	The state for which the calls are installed
// /*AFLA*/  * @name:	Name of the callback.
// /*AFLA*/  * @startup:	startup callback function
// /*AFLA*/  * @teardown:	teardown callback function
// /*AFLA*/  *
// /*AFLA*/  * Sets the internal multi_instance flag and prepares a state to work as a multi
// /*AFLA*/  * instance callback. No callbacks are invoked at this point. The callbacks are
// /*AFLA*/  * invoked once an instance for this state are registered via
// /*AFLA*/  * @cpuhp_state_add_instance or @cpuhp_state_add_instance_nocalls.
// /*AFLA*/  */
// /*AFLA*/ static inline int cpuhp_setup_state_multi(enum cpuhp_state state,
// /*AFLA*/ 					  const char *name,
// /*AFLA*/ 					  int (*startup)(unsigned int cpu,
// /*AFLA*/ 							 struct hlist_node *node),
// /*AFLA*/ 					  int (*teardown)(unsigned int cpu,
// /*AFLA*/ 							  struct hlist_node *node))
// /*AFLA*/ {
// /*AFLA*/ 	return __cpuhp_setup_state(state, name, false,
// /*AFLA*/ 				   (void *) startup,
// /*AFLA*/ 				   (void *) teardown, true);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ int __cpuhp_state_add_instance(enum cpuhp_state state, struct hlist_node *node,
// /*AFLA*/ 			       bool invoke);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpuhp_state_add_instance - Add an instance for a state and invoke startup
// /*AFLA*/  *                            callback.
// /*AFLA*/  * @state:	The state for which the instance is installed
// /*AFLA*/  * @node:	The node for this individual state.
// /*AFLA*/  *
// /*AFLA*/  * Installs the instance for the @state and invokes the startup callback on
// /*AFLA*/  * the present cpus which have already reached the @state. The @state must have
// /*AFLA*/  * been earlier marked as multi-instance by @cpuhp_setup_state_multi.
// /*AFLA*/  */
// /*AFLA*/ static inline int cpuhp_state_add_instance(enum cpuhp_state state,
// /*AFLA*/ 					   struct hlist_node *node)
// /*AFLA*/ {
// /*AFLA*/ 	return __cpuhp_state_add_instance(state, node, true);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpuhp_state_add_instance_nocalls - Add an instance for a state without
// /*AFLA*/  *                                    invoking the startup callback.
// /*AFLA*/  * @state:	The state for which the instance is installed
// /*AFLA*/  * @node:	The node for this individual state.
// /*AFLA*/  *
// /*AFLA*/  * Installs the instance for the @state The @state must have been earlier
// /*AFLA*/  * marked as multi-instance by @cpuhp_setup_state_multi.
// /*AFLA*/  */
// /*AFLA*/ static inline int cpuhp_state_add_instance_nocalls(enum cpuhp_state state,
// /*AFLA*/ 						   struct hlist_node *node)
// /*AFLA*/ {
// /*AFLA*/ 	return __cpuhp_state_add_instance(state, node, false);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void __cpuhp_remove_state(enum cpuhp_state state, bool invoke);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpuhp_remove_state - Remove hotplug state callbacks and invoke the teardown
// /*AFLA*/  * @state:	The state for which the calls are removed
// /*AFLA*/  *
// /*AFLA*/  * Removes the callback functions and invokes the teardown callback on
// /*AFLA*/  * the present cpus which have already reached the @state.
// /*AFLA*/  */
// /*AFLA*/ static inline void cpuhp_remove_state(enum cpuhp_state state)
// /*AFLA*/ {
// /*AFLA*/ 	__cpuhp_remove_state(state, true);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpuhp_remove_state_nocalls - Remove hotplug state callbacks without invoking
// /*AFLA*/  *				teardown
// /*AFLA*/  * @state:	The state for which the calls are removed
// /*AFLA*/  */
// /*AFLA*/ static inline void cpuhp_remove_state_nocalls(enum cpuhp_state state)
// /*AFLA*/ {
// /*AFLA*/ 	__cpuhp_remove_state(state, false);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpuhp_remove_multi_state - Remove hotplug multi state callback
// /*AFLA*/  * @state:	The state for which the calls are removed
// /*AFLA*/  *
// /*AFLA*/  * Removes the callback functions from a multi state. This is the reverse of
// /*AFLA*/  * cpuhp_setup_state_multi(). All instances should have been removed before
// /*AFLA*/  * invoking this function.
// /*AFLA*/  */
// /*AFLA*/ static inline void cpuhp_remove_multi_state(enum cpuhp_state state)
// /*AFLA*/ {
// /*AFLA*/ 	__cpuhp_remove_state(state, false);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ int __cpuhp_state_remove_instance(enum cpuhp_state state,
// /*AFLA*/ 				  struct hlist_node *node, bool invoke);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpuhp_state_remove_instance - Remove hotplug instance from state and invoke
// /*AFLA*/  *                               the teardown callback
// /*AFLA*/  * @state:	The state from which the instance is removed
// /*AFLA*/  * @node:	The node for this individual state.
// /*AFLA*/  *
// /*AFLA*/  * Removes the instance and invokes the teardown callback on the present cpus
// /*AFLA*/  * which have already reached the @state.
// /*AFLA*/  */
// /*AFLA*/ static inline int cpuhp_state_remove_instance(enum cpuhp_state state,
// /*AFLA*/ 					      struct hlist_node *node)
// /*AFLA*/ {
// /*AFLA*/ 	return __cpuhp_state_remove_instance(state, node, true);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpuhp_state_remove_instance_nocalls - Remove hotplug instance from state
// /*AFLA*/  *					 without invoking the reatdown callback
// /*AFLA*/  * @state:	The state from which the instance is removed
// /*AFLA*/  * @node:	The node for this individual state.
// /*AFLA*/  *
// /*AFLA*/  * Removes the instance without invoking the teardown callback.
// /*AFLA*/  */
// /*AFLA*/ static inline int cpuhp_state_remove_instance_nocalls(enum cpuhp_state state,
// /*AFLA*/ 						      struct hlist_node *node)
// /*AFLA*/ {
// /*AFLA*/ 	return __cpuhp_state_remove_instance(state, node, false);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ void cpuhp_online_idle(enum cpuhp_state state);
// /*AFLA*/ #else
// /*AFLA*/ static inline void cpuhp_online_idle(enum cpuhp_state state) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
