// /*AFLA*/ #ifndef _ASM_X86_PERF_EVENT_H
// /*AFLA*/ #define _ASM_X86_PERF_EVENT_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Performance event hw details:
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define INTEL_PMC_MAX_GENERIC				       32
// /*AFLA*/ #define INTEL_PMC_MAX_FIXED					3
// /*AFLA*/ #define INTEL_PMC_IDX_FIXED				       32
// /*AFLA*/ 
// /*AFLA*/ #define X86_PMC_IDX_MAX					       64
// /*AFLA*/ 
// /*AFLA*/ #define MSR_ARCH_PERFMON_PERFCTR0			      0xc1
// /*AFLA*/ #define MSR_ARCH_PERFMON_PERFCTR1			      0xc2
// /*AFLA*/ 
// /*AFLA*/ #define MSR_ARCH_PERFMON_EVENTSEL0			     0x186
// /*AFLA*/ #define MSR_ARCH_PERFMON_EVENTSEL1			     0x187
// /*AFLA*/ 
// /*AFLA*/ #define ARCH_PERFMON_EVENTSEL_EVENT			0x000000FFULL
// /*AFLA*/ #define ARCH_PERFMON_EVENTSEL_UMASK			0x0000FF00ULL
// /*AFLA*/ #define ARCH_PERFMON_EVENTSEL_USR			(1ULL << 16)
// /*AFLA*/ #define ARCH_PERFMON_EVENTSEL_OS			(1ULL << 17)
// /*AFLA*/ #define ARCH_PERFMON_EVENTSEL_EDGE			(1ULL << 18)
// /*AFLA*/ #define ARCH_PERFMON_EVENTSEL_PIN_CONTROL		(1ULL << 19)
// /*AFLA*/ #define ARCH_PERFMON_EVENTSEL_INT			(1ULL << 20)
// /*AFLA*/ #define ARCH_PERFMON_EVENTSEL_ANY			(1ULL << 21)
// /*AFLA*/ #define ARCH_PERFMON_EVENTSEL_ENABLE			(1ULL << 22)
// /*AFLA*/ #define ARCH_PERFMON_EVENTSEL_INV			(1ULL << 23)
// /*AFLA*/ #define ARCH_PERFMON_EVENTSEL_CMASK			0xFF000000ULL
// /*AFLA*/ 
// /*AFLA*/ #define HSW_IN_TX					(1ULL << 32)
// /*AFLA*/ #define HSW_IN_TX_CHECKPOINTED				(1ULL << 33)
// /*AFLA*/ 
// /*AFLA*/ #define AMD64_EVENTSEL_INT_CORE_ENABLE			(1ULL << 36)
// /*AFLA*/ #define AMD64_EVENTSEL_GUESTONLY			(1ULL << 40)
// /*AFLA*/ #define AMD64_EVENTSEL_HOSTONLY				(1ULL << 41)
// /*AFLA*/ 
// /*AFLA*/ #define AMD64_EVENTSEL_INT_CORE_SEL_SHIFT		37
// /*AFLA*/ #define AMD64_EVENTSEL_INT_CORE_SEL_MASK		\
// /*AFLA*/ 	(0xFULL << AMD64_EVENTSEL_INT_CORE_SEL_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ #define AMD64_EVENTSEL_EVENT	\
// /*AFLA*/ 	(ARCH_PERFMON_EVENTSEL_EVENT | (0x0FULL << 32))
// /*AFLA*/ #define INTEL_ARCH_EVENT_MASK	\
// /*AFLA*/ 	(ARCH_PERFMON_EVENTSEL_UMASK | ARCH_PERFMON_EVENTSEL_EVENT)
// /*AFLA*/ 
// /*AFLA*/ #define X86_RAW_EVENT_MASK		\
// /*AFLA*/ 	(ARCH_PERFMON_EVENTSEL_EVENT |	\
// /*AFLA*/ 	 ARCH_PERFMON_EVENTSEL_UMASK |	\
// /*AFLA*/ 	 ARCH_PERFMON_EVENTSEL_EDGE  |	\
// /*AFLA*/ 	 ARCH_PERFMON_EVENTSEL_INV   |	\
// /*AFLA*/ 	 ARCH_PERFMON_EVENTSEL_CMASK)
// /*AFLA*/ #define X86_ALL_EVENT_FLAGS  			\
// /*AFLA*/ 	(ARCH_PERFMON_EVENTSEL_EDGE |  		\
// /*AFLA*/ 	 ARCH_PERFMON_EVENTSEL_INV | 		\
// /*AFLA*/ 	 ARCH_PERFMON_EVENTSEL_CMASK | 		\
// /*AFLA*/ 	 ARCH_PERFMON_EVENTSEL_ANY | 		\
// /*AFLA*/ 	 ARCH_PERFMON_EVENTSEL_PIN_CONTROL | 	\
// /*AFLA*/ 	 HSW_IN_TX | 				\
// /*AFLA*/ 	 HSW_IN_TX_CHECKPOINTED)
// /*AFLA*/ #define AMD64_RAW_EVENT_MASK		\
// /*AFLA*/ 	(X86_RAW_EVENT_MASK          |  \
// /*AFLA*/ 	 AMD64_EVENTSEL_EVENT)
// /*AFLA*/ #define AMD64_RAW_EVENT_MASK_NB		\
// /*AFLA*/ 	(AMD64_EVENTSEL_EVENT        |  \
// /*AFLA*/ 	 ARCH_PERFMON_EVENTSEL_UMASK)
// /*AFLA*/ #define AMD64_NUM_COUNTERS				4
// /*AFLA*/ #define AMD64_NUM_COUNTERS_CORE				6
// /*AFLA*/ #define AMD64_NUM_COUNTERS_NB				4
// /*AFLA*/ 
// /*AFLA*/ #define ARCH_PERFMON_UNHALTED_CORE_CYCLES_SEL		0x3c
// /*AFLA*/ #define ARCH_PERFMON_UNHALTED_CORE_CYCLES_UMASK		(0x00 << 8)
// /*AFLA*/ #define ARCH_PERFMON_UNHALTED_CORE_CYCLES_INDEX		0
// /*AFLA*/ #define ARCH_PERFMON_UNHALTED_CORE_CYCLES_PRESENT \
// /*AFLA*/ 		(1 << (ARCH_PERFMON_UNHALTED_CORE_CYCLES_INDEX))
// /*AFLA*/ 
// /*AFLA*/ #define ARCH_PERFMON_BRANCH_MISSES_RETIRED		6
// /*AFLA*/ #define ARCH_PERFMON_EVENTS_COUNT			7
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Intel "Architectural Performance Monitoring" CPUID
// /*AFLA*/  * detection/enumeration details:
// /*AFLA*/  */
// /*AFLA*/ union cpuid10_eax {
// /*AFLA*/ 	struct {
// /*AFLA*/ 		unsigned int version_id:8;
// /*AFLA*/ 		unsigned int num_counters:8;
// /*AFLA*/ 		unsigned int bit_width:8;
// /*AFLA*/ 		unsigned int mask_length:8;
// /*AFLA*/ 	} split;
// /*AFLA*/ 	unsigned int full;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ union cpuid10_ebx {
// /*AFLA*/ 	struct {
// /*AFLA*/ 		unsigned int no_unhalted_core_cycles:1;
// /*AFLA*/ 		unsigned int no_instructions_retired:1;
// /*AFLA*/ 		unsigned int no_unhalted_reference_cycles:1;
// /*AFLA*/ 		unsigned int no_llc_reference:1;
// /*AFLA*/ 		unsigned int no_llc_misses:1;
// /*AFLA*/ 		unsigned int no_branch_instruction_retired:1;
// /*AFLA*/ 		unsigned int no_branch_misses_retired:1;
// /*AFLA*/ 	} split;
// /*AFLA*/ 	unsigned int full;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ union cpuid10_edx {
// /*AFLA*/ 	struct {
// /*AFLA*/ 		unsigned int num_counters_fixed:5;
// /*AFLA*/ 		unsigned int bit_width_fixed:8;
// /*AFLA*/ 		unsigned int reserved:19;
// /*AFLA*/ 	} split;
// /*AFLA*/ 	unsigned int full;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct x86_pmu_capability {
// /*AFLA*/ 	int		version;
// /*AFLA*/ 	int		num_counters_gp;
// /*AFLA*/ 	int		num_counters_fixed;
// /*AFLA*/ 	int		bit_width_gp;
// /*AFLA*/ 	int		bit_width_fixed;
// /*AFLA*/ 	unsigned int	events_mask;
// /*AFLA*/ 	int		events_mask_len;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Fixed-purpose performance events:
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * All 3 fixed-mode PMCs are configured via this single MSR:
// /*AFLA*/  */
// /*AFLA*/ #define MSR_ARCH_PERFMON_FIXED_CTR_CTRL	0x38d
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The counts are available in three separate MSRs:
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* Instr_Retired.Any: */
// /*AFLA*/ #define MSR_ARCH_PERFMON_FIXED_CTR0	0x309
// /*AFLA*/ #define INTEL_PMC_IDX_FIXED_INSTRUCTIONS	(INTEL_PMC_IDX_FIXED + 0)
// /*AFLA*/ 
// /*AFLA*/ /* CPU_CLK_Unhalted.Core: */
// /*AFLA*/ #define MSR_ARCH_PERFMON_FIXED_CTR1	0x30a
// /*AFLA*/ #define INTEL_PMC_IDX_FIXED_CPU_CYCLES	(INTEL_PMC_IDX_FIXED + 1)
// /*AFLA*/ 
// /*AFLA*/ /* CPU_CLK_Unhalted.Ref: */
// /*AFLA*/ #define MSR_ARCH_PERFMON_FIXED_CTR2	0x30b
// /*AFLA*/ #define INTEL_PMC_IDX_FIXED_REF_CYCLES	(INTEL_PMC_IDX_FIXED + 2)
// /*AFLA*/ #define INTEL_PMC_MSK_FIXED_REF_CYCLES	(1ULL << INTEL_PMC_IDX_FIXED_REF_CYCLES)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We model BTS tracing as another fixed-mode PMC.
// /*AFLA*/  *
// /*AFLA*/  * We choose a value in the middle of the fixed event range, since lower
// /*AFLA*/  * values are used by actual fixed events and higher values are used
// /*AFLA*/  * to indicate other overflow conditions in the PERF_GLOBAL_STATUS msr.
// /*AFLA*/  */
// /*AFLA*/ #define INTEL_PMC_IDX_FIXED_BTS				(INTEL_PMC_IDX_FIXED + 16)
// /*AFLA*/ 
// /*AFLA*/ #define GLOBAL_STATUS_COND_CHG				BIT_ULL(63)
// /*AFLA*/ #define GLOBAL_STATUS_BUFFER_OVF			BIT_ULL(62)
// /*AFLA*/ #define GLOBAL_STATUS_UNC_OVF				BIT_ULL(61)
// /*AFLA*/ #define GLOBAL_STATUS_ASIF				BIT_ULL(60)
// /*AFLA*/ #define GLOBAL_STATUS_COUNTERS_FROZEN			BIT_ULL(59)
// /*AFLA*/ #define GLOBAL_STATUS_LBRS_FROZEN			BIT_ULL(58)
// /*AFLA*/ #define GLOBAL_STATUS_TRACE_TOPAPMI			BIT_ULL(55)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * IBS cpuid feature detection
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define IBS_CPUID_FEATURES		0x8000001b
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Same bit mask as for IBS cpuid feature flags (Fn8000_001B_EAX), but
// /*AFLA*/  * bit 0 is used to indicate the existence of IBS.
// /*AFLA*/  */
// /*AFLA*/ #define IBS_CAPS_AVAIL			(1U<<0)
// /*AFLA*/ #define IBS_CAPS_FETCHSAM		(1U<<1)
// /*AFLA*/ #define IBS_CAPS_OPSAM			(1U<<2)
// /*AFLA*/ #define IBS_CAPS_RDWROPCNT		(1U<<3)
// /*AFLA*/ #define IBS_CAPS_OPCNT			(1U<<4)
// /*AFLA*/ #define IBS_CAPS_BRNTRGT		(1U<<5)
// /*AFLA*/ #define IBS_CAPS_OPCNTEXT		(1U<<6)
// /*AFLA*/ #define IBS_CAPS_RIPINVALIDCHK		(1U<<7)
// /*AFLA*/ #define IBS_CAPS_OPBRNFUSE		(1U<<8)
// /*AFLA*/ #define IBS_CAPS_FETCHCTLEXTD		(1U<<9)
// /*AFLA*/ #define IBS_CAPS_OPDATA4		(1U<<10)
// /*AFLA*/ 
// /*AFLA*/ #define IBS_CAPS_DEFAULT		(IBS_CAPS_AVAIL		\
// /*AFLA*/ 					 | IBS_CAPS_FETCHSAM	\
// /*AFLA*/ 					 | IBS_CAPS_OPSAM)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * IBS APIC setup
// /*AFLA*/  */
// /*AFLA*/ #define IBSCTL				0x1cc
// /*AFLA*/ #define IBSCTL_LVT_OFFSET_VALID		(1ULL<<8)
// /*AFLA*/ #define IBSCTL_LVT_OFFSET_MASK		0x0F
// /*AFLA*/ 
// /*AFLA*/ /* ibs fetch bits/masks */
// /*AFLA*/ #define IBS_FETCH_RAND_EN	(1ULL<<57)
// /*AFLA*/ #define IBS_FETCH_VAL		(1ULL<<49)
// /*AFLA*/ #define IBS_FETCH_ENABLE	(1ULL<<48)
// /*AFLA*/ #define IBS_FETCH_CNT		0xFFFF0000ULL
// /*AFLA*/ #define IBS_FETCH_MAX_CNT	0x0000FFFFULL
// /*AFLA*/ 
// /*AFLA*/ /* ibs op bits/masks */
// /*AFLA*/ /* lower 4 bits of the current count are ignored: */
// /*AFLA*/ #define IBS_OP_CUR_CNT		(0xFFFF0ULL<<32)
// /*AFLA*/ #define IBS_OP_CNT_CTL		(1ULL<<19)
// /*AFLA*/ #define IBS_OP_VAL		(1ULL<<18)
// /*AFLA*/ #define IBS_OP_ENABLE		(1ULL<<17)
// /*AFLA*/ #define IBS_OP_MAX_CNT		0x0000FFFFULL
// /*AFLA*/ #define IBS_OP_MAX_CNT_EXT	0x007FFFFFULL	/* not a register bit mask */
// /*AFLA*/ #define IBS_RIP_INVALID		(1ULL<<38)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_LOCAL_APIC
// /*AFLA*/ extern u32 get_ibs_caps(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline u32 get_ibs_caps(void) { return 0; }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PERF_EVENTS
// /*AFLA*/ extern void perf_events_lapic_init(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Abuse bits {3,5} of the cpu eflags register. These flags are otherwise
// /*AFLA*/  * unused and ABI specified to be 0, so nobody should care what we do with
// /*AFLA*/  * them.
// /*AFLA*/  *
// /*AFLA*/  * EXACT - the IP points to the exact instruction that triggered the
// /*AFLA*/  *         event (HW bugs exempt).
// /*AFLA*/  * VM    - original X86_VM_MASK; see set_linear_ip().
// /*AFLA*/  */
// /*AFLA*/ #define PERF_EFLAGS_EXACT	(1UL << 3)
// /*AFLA*/ #define PERF_EFLAGS_VM		(1UL << 5)
// /*AFLA*/ 
// /*AFLA*/ struct pt_regs;
// /*AFLA*/ extern unsigned long perf_instruction_pointer(struct pt_regs *regs);
// /*AFLA*/ extern unsigned long perf_misc_flags(struct pt_regs *regs);
// /*AFLA*/ #define perf_misc_flags(regs)	perf_misc_flags(regs)
// /*AFLA*/ 
// /*AFLA*/ #include <asm/stacktrace.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We abuse bit 3 from flags to pass exact information, see perf_misc_flags
// /*AFLA*/  * and the comment with PERF_EFLAGS_EXACT.
// /*AFLA*/  */
// /*AFLA*/ #define perf_arch_fetch_caller_regs(regs, __ip)		{	\
// /*AFLA*/ 	(regs)->ip = (__ip);					\
// /*AFLA*/ 	(regs)->bp = caller_frame_pointer();			\
// /*AFLA*/ 	(regs)->cs = __KERNEL_CS;				\
// /*AFLA*/ 	regs->flags = 0;					\
// /*AFLA*/ 	asm volatile(						\
// /*AFLA*/ 		_ASM_MOV "%%"_ASM_SP ", %0\n"			\
// /*AFLA*/ 		: "=m" ((regs)->sp)				\
// /*AFLA*/ 		:: "memory"					\
// /*AFLA*/ 	);							\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct perf_guest_switch_msr {
// /*AFLA*/ 	unsigned msr;
// /*AFLA*/ 	u64 host, guest;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct perf_guest_switch_msr *perf_guest_get_msrs(int *nr);
// /*AFLA*/ extern void perf_get_x86_pmu_capability(struct x86_pmu_capability *cap);
// /*AFLA*/ extern void perf_check_microcode(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline struct perf_guest_switch_msr *perf_guest_get_msrs(int *nr)
// /*AFLA*/ {
// /*AFLA*/ 	*nr = 0;
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void perf_get_x86_pmu_capability(struct x86_pmu_capability *cap)
// /*AFLA*/ {
// /*AFLA*/ 	memset(cap, 0, sizeof(*cap));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void perf_events_lapic_init(void)	{ }
// /*AFLA*/ static inline void perf_check_microcode(void) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_CPU_SUP_INTEL
// /*AFLA*/  extern void intel_pt_handle_vmx(int on);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_PERF_EVENTS) && defined(CONFIG_CPU_SUP_AMD)
// /*AFLA*/  extern void amd_pmu_enable_virt(void);
// /*AFLA*/  extern void amd_pmu_disable_virt(void);
// /*AFLA*/ #else
// /*AFLA*/  static inline void amd_pmu_enable_virt(void) { }
// /*AFLA*/  static inline void amd_pmu_disable_virt(void) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define arch_perf_out_copy_user copy_from_user_nmi
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_PERF_EVENT_H */
