// /*AFLA*/ #ifndef _ASM_X86_MSR_INDEX_H
// /*AFLA*/ #define _ASM_X86_MSR_INDEX_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * CPU model specific register (MSR) numbers.
// /*AFLA*/  *
// /*AFLA*/  * Do not add new entries to this file unless the definitions are shared
// /*AFLA*/  * between multiple compilation units.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* x86-64 specific MSRs */
// /*AFLA*/ #define MSR_EFER		0xc0000080 /* extended feature register */
// /*AFLA*/ #define MSR_STAR		0xc0000081 /* legacy mode SYSCALL target */
// /*AFLA*/ #define MSR_LSTAR		0xc0000082 /* long mode SYSCALL target */
// /*AFLA*/ #define MSR_CSTAR		0xc0000083 /* compat mode SYSCALL target */
// /*AFLA*/ #define MSR_SYSCALL_MASK	0xc0000084 /* EFLAGS mask for syscall */
// /*AFLA*/ #define MSR_FS_BASE		0xc0000100 /* 64bit FS base */
// /*AFLA*/ #define MSR_GS_BASE		0xc0000101 /* 64bit GS base */
// /*AFLA*/ #define MSR_KERNEL_GS_BASE	0xc0000102 /* SwapGS GS shadow */
// /*AFLA*/ #define MSR_TSC_AUX		0xc0000103 /* Auxiliary TSC */
// /*AFLA*/ 
// /*AFLA*/ /* EFER bits: */
// /*AFLA*/ #define _EFER_SCE		0  /* SYSCALL/SYSRET */
// /*AFLA*/ #define _EFER_LME		8  /* Long mode enable */
// /*AFLA*/ #define _EFER_LMA		10 /* Long mode active (read-only) */
// /*AFLA*/ #define _EFER_NX		11 /* No execute enable */
// /*AFLA*/ #define _EFER_SVME		12 /* Enable virtualization */
// /*AFLA*/ #define _EFER_LMSLE		13 /* Long Mode Segment Limit Enable */
// /*AFLA*/ #define _EFER_FFXSR		14 /* Enable Fast FXSAVE/FXRSTOR */
// /*AFLA*/ 
// /*AFLA*/ #define EFER_SCE		(1<<_EFER_SCE)
// /*AFLA*/ #define EFER_LME		(1<<_EFER_LME)
// /*AFLA*/ #define EFER_LMA		(1<<_EFER_LMA)
// /*AFLA*/ #define EFER_NX			(1<<_EFER_NX)
// /*AFLA*/ #define EFER_SVME		(1<<_EFER_SVME)
// /*AFLA*/ #define EFER_LMSLE		(1<<_EFER_LMSLE)
// /*AFLA*/ #define EFER_FFXSR		(1<<_EFER_FFXSR)
// /*AFLA*/ 
// /*AFLA*/ /* Intel MSRs. Some also available on other CPUs */
// /*AFLA*/ #define MSR_IA32_PERFCTR0		0x000000c1
// /*AFLA*/ #define MSR_IA32_PERFCTR1		0x000000c2
// /*AFLA*/ #define MSR_FSB_FREQ			0x000000cd
// /*AFLA*/ #define MSR_PLATFORM_INFO		0x000000ce
// /*AFLA*/ 
// /*AFLA*/ #define MSR_NHM_SNB_PKG_CST_CFG_CTL	0x000000e2
// /*AFLA*/ #define NHM_C3_AUTO_DEMOTE		(1UL << 25)
// /*AFLA*/ #define NHM_C1_AUTO_DEMOTE		(1UL << 26)
// /*AFLA*/ #define ATM_LNC_C6_AUTO_DEMOTE		(1UL << 25)
// /*AFLA*/ #define SNB_C1_AUTO_UNDEMOTE		(1UL << 27)
// /*AFLA*/ #define SNB_C3_AUTO_UNDEMOTE		(1UL << 28)
// /*AFLA*/ 
// /*AFLA*/ #define MSR_MTRRcap			0x000000fe
// /*AFLA*/ #define MSR_IA32_BBL_CR_CTL		0x00000119
// /*AFLA*/ #define MSR_IA32_BBL_CR_CTL3		0x0000011e
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_SYSENTER_CS		0x00000174
// /*AFLA*/ #define MSR_IA32_SYSENTER_ESP		0x00000175
// /*AFLA*/ #define MSR_IA32_SYSENTER_EIP		0x00000176
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_MCG_CAP		0x00000179
// /*AFLA*/ #define MSR_IA32_MCG_STATUS		0x0000017a
// /*AFLA*/ #define MSR_IA32_MCG_CTL		0x0000017b
// /*AFLA*/ #define MSR_IA32_MCG_EXT_CTL		0x000004d0
// /*AFLA*/ 
// /*AFLA*/ #define MSR_OFFCORE_RSP_0		0x000001a6
// /*AFLA*/ #define MSR_OFFCORE_RSP_1		0x000001a7
// /*AFLA*/ #define MSR_TURBO_RATIO_LIMIT		0x000001ad
// /*AFLA*/ #define MSR_TURBO_RATIO_LIMIT1		0x000001ae
// /*AFLA*/ #define MSR_TURBO_RATIO_LIMIT2		0x000001af
// /*AFLA*/ 
// /*AFLA*/ #define MSR_LBR_SELECT			0x000001c8
// /*AFLA*/ #define MSR_LBR_TOS			0x000001c9
// /*AFLA*/ #define MSR_LBR_NHM_FROM		0x00000680
// /*AFLA*/ #define MSR_LBR_NHM_TO			0x000006c0
// /*AFLA*/ #define MSR_LBR_CORE_FROM		0x00000040
// /*AFLA*/ #define MSR_LBR_CORE_TO			0x00000060
// /*AFLA*/ 
// /*AFLA*/ #define MSR_LBR_INFO_0			0x00000dc0 /* ... 0xddf for _31 */
// /*AFLA*/ #define LBR_INFO_MISPRED		BIT_ULL(63)
// /*AFLA*/ #define LBR_INFO_IN_TX			BIT_ULL(62)
// /*AFLA*/ #define LBR_INFO_ABORT			BIT_ULL(61)
// /*AFLA*/ #define LBR_INFO_CYCLES			0xffff
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_PEBS_ENABLE		0x000003f1
// /*AFLA*/ #define MSR_IA32_DS_AREA		0x00000600
// /*AFLA*/ #define MSR_IA32_PERF_CAPABILITIES	0x00000345
// /*AFLA*/ #define MSR_PEBS_LD_LAT_THRESHOLD	0x000003f6
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_RTIT_CTL		0x00000570
// /*AFLA*/ #define MSR_IA32_RTIT_STATUS		0x00000571
// /*AFLA*/ #define MSR_IA32_RTIT_ADDR0_A		0x00000580
// /*AFLA*/ #define MSR_IA32_RTIT_ADDR0_B		0x00000581
// /*AFLA*/ #define MSR_IA32_RTIT_ADDR1_A		0x00000582
// /*AFLA*/ #define MSR_IA32_RTIT_ADDR1_B		0x00000583
// /*AFLA*/ #define MSR_IA32_RTIT_ADDR2_A		0x00000584
// /*AFLA*/ #define MSR_IA32_RTIT_ADDR2_B		0x00000585
// /*AFLA*/ #define MSR_IA32_RTIT_ADDR3_A		0x00000586
// /*AFLA*/ #define MSR_IA32_RTIT_ADDR3_B		0x00000587
// /*AFLA*/ #define MSR_IA32_RTIT_CR3_MATCH		0x00000572
// /*AFLA*/ #define MSR_IA32_RTIT_OUTPUT_BASE	0x00000560
// /*AFLA*/ #define MSR_IA32_RTIT_OUTPUT_MASK	0x00000561
// /*AFLA*/ 
// /*AFLA*/ #define MSR_MTRRfix64K_00000		0x00000250
// /*AFLA*/ #define MSR_MTRRfix16K_80000		0x00000258
// /*AFLA*/ #define MSR_MTRRfix16K_A0000		0x00000259
// /*AFLA*/ #define MSR_MTRRfix4K_C0000		0x00000268
// /*AFLA*/ #define MSR_MTRRfix4K_C8000		0x00000269
// /*AFLA*/ #define MSR_MTRRfix4K_D0000		0x0000026a
// /*AFLA*/ #define MSR_MTRRfix4K_D8000		0x0000026b
// /*AFLA*/ #define MSR_MTRRfix4K_E0000		0x0000026c
// /*AFLA*/ #define MSR_MTRRfix4K_E8000		0x0000026d
// /*AFLA*/ #define MSR_MTRRfix4K_F0000		0x0000026e
// /*AFLA*/ #define MSR_MTRRfix4K_F8000		0x0000026f
// /*AFLA*/ #define MSR_MTRRdefType			0x000002ff
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_CR_PAT			0x00000277
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_DEBUGCTLMSR		0x000001d9
// /*AFLA*/ #define MSR_IA32_LASTBRANCHFROMIP	0x000001db
// /*AFLA*/ #define MSR_IA32_LASTBRANCHTOIP		0x000001dc
// /*AFLA*/ #define MSR_IA32_LASTINTFROMIP		0x000001dd
// /*AFLA*/ #define MSR_IA32_LASTINTTOIP		0x000001de
// /*AFLA*/ 
// /*AFLA*/ /* DEBUGCTLMSR bits (others vary by model): */
// /*AFLA*/ #define DEBUGCTLMSR_LBR			(1UL <<  0) /* last branch recording */
// /*AFLA*/ #define DEBUGCTLMSR_BTF			(1UL <<  1) /* single-step on branches */
// /*AFLA*/ #define DEBUGCTLMSR_TR			(1UL <<  6)
// /*AFLA*/ #define DEBUGCTLMSR_BTS			(1UL <<  7)
// /*AFLA*/ #define DEBUGCTLMSR_BTINT		(1UL <<  8)
// /*AFLA*/ #define DEBUGCTLMSR_BTS_OFF_OS		(1UL <<  9)
// /*AFLA*/ #define DEBUGCTLMSR_BTS_OFF_USR		(1UL << 10)
// /*AFLA*/ #define DEBUGCTLMSR_FREEZE_LBRS_ON_PMI	(1UL << 11)
// /*AFLA*/ 
// /*AFLA*/ #define MSR_PEBS_FRONTEND		0x000003f7
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_POWER_CTL		0x000001fc
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_MC0_CTL		0x00000400
// /*AFLA*/ #define MSR_IA32_MC0_STATUS		0x00000401
// /*AFLA*/ #define MSR_IA32_MC0_ADDR		0x00000402
// /*AFLA*/ #define MSR_IA32_MC0_MISC		0x00000403
// /*AFLA*/ 
// /*AFLA*/ /* C-state Residency Counters */
// /*AFLA*/ #define MSR_PKG_C3_RESIDENCY		0x000003f8
// /*AFLA*/ #define MSR_PKG_C6_RESIDENCY		0x000003f9
// /*AFLA*/ #define MSR_PKG_C7_RESIDENCY		0x000003fa
// /*AFLA*/ #define MSR_CORE_C3_RESIDENCY		0x000003fc
// /*AFLA*/ #define MSR_CORE_C6_RESIDENCY		0x000003fd
// /*AFLA*/ #define MSR_CORE_C7_RESIDENCY		0x000003fe
// /*AFLA*/ #define MSR_KNL_CORE_C6_RESIDENCY	0x000003ff
// /*AFLA*/ #define MSR_PKG_C2_RESIDENCY		0x0000060d
// /*AFLA*/ #define MSR_PKG_C8_RESIDENCY		0x00000630
// /*AFLA*/ #define MSR_PKG_C9_RESIDENCY		0x00000631
// /*AFLA*/ #define MSR_PKG_C10_RESIDENCY		0x00000632
// /*AFLA*/ 
// /*AFLA*/ /* Interrupt Response Limit */
// /*AFLA*/ #define MSR_PKGC3_IRTL			0x0000060a
// /*AFLA*/ #define MSR_PKGC6_IRTL			0x0000060b
// /*AFLA*/ #define MSR_PKGC7_IRTL			0x0000060c
// /*AFLA*/ #define MSR_PKGC8_IRTL			0x00000633
// /*AFLA*/ #define MSR_PKGC9_IRTL			0x00000634
// /*AFLA*/ #define MSR_PKGC10_IRTL			0x00000635
// /*AFLA*/ 
// /*AFLA*/ /* Run Time Average Power Limiting (RAPL) Interface */
// /*AFLA*/ 
// /*AFLA*/ #define MSR_RAPL_POWER_UNIT		0x00000606
// /*AFLA*/ 
// /*AFLA*/ #define MSR_PKG_POWER_LIMIT		0x00000610
// /*AFLA*/ #define MSR_PKG_ENERGY_STATUS		0x00000611
// /*AFLA*/ #define MSR_PKG_PERF_STATUS		0x00000613
// /*AFLA*/ #define MSR_PKG_POWER_INFO		0x00000614
// /*AFLA*/ 
// /*AFLA*/ #define MSR_DRAM_POWER_LIMIT		0x00000618
// /*AFLA*/ #define MSR_DRAM_ENERGY_STATUS		0x00000619
// /*AFLA*/ #define MSR_DRAM_PERF_STATUS		0x0000061b
// /*AFLA*/ #define MSR_DRAM_POWER_INFO		0x0000061c
// /*AFLA*/ 
// /*AFLA*/ #define MSR_PP0_POWER_LIMIT		0x00000638
// /*AFLA*/ #define MSR_PP0_ENERGY_STATUS		0x00000639
// /*AFLA*/ #define MSR_PP0_POLICY			0x0000063a
// /*AFLA*/ #define MSR_PP0_PERF_STATUS		0x0000063b
// /*AFLA*/ 
// /*AFLA*/ #define MSR_PP1_POWER_LIMIT		0x00000640
// /*AFLA*/ #define MSR_PP1_ENERGY_STATUS		0x00000641
// /*AFLA*/ #define MSR_PP1_POLICY			0x00000642
// /*AFLA*/ 
// /*AFLA*/ /* Config TDP MSRs */
// /*AFLA*/ #define MSR_CONFIG_TDP_NOMINAL		0x00000648
// /*AFLA*/ #define MSR_CONFIG_TDP_LEVEL_1		0x00000649
// /*AFLA*/ #define MSR_CONFIG_TDP_LEVEL_2		0x0000064A
// /*AFLA*/ #define MSR_CONFIG_TDP_CONTROL		0x0000064B
// /*AFLA*/ #define MSR_TURBO_ACTIVATION_RATIO	0x0000064C
// /*AFLA*/ 
// /*AFLA*/ #define MSR_PLATFORM_ENERGY_STATUS	0x0000064D
// /*AFLA*/ 
// /*AFLA*/ #define MSR_PKG_WEIGHTED_CORE_C0_RES	0x00000658
// /*AFLA*/ #define MSR_PKG_ANY_CORE_C0_RES		0x00000659
// /*AFLA*/ #define MSR_PKG_ANY_GFXE_C0_RES		0x0000065A
// /*AFLA*/ #define MSR_PKG_BOTH_CORE_GFXE_C0_RES	0x0000065B
// /*AFLA*/ 
// /*AFLA*/ #define MSR_CORE_C1_RES			0x00000660
// /*AFLA*/ 
// /*AFLA*/ #define MSR_CC6_DEMOTION_POLICY_CONFIG	0x00000668
// /*AFLA*/ #define MSR_MC6_DEMOTION_POLICY_CONFIG	0x00000669
// /*AFLA*/ 
// /*AFLA*/ #define MSR_CORE_PERF_LIMIT_REASONS	0x00000690
// /*AFLA*/ #define MSR_GFX_PERF_LIMIT_REASONS	0x000006B0
// /*AFLA*/ #define MSR_RING_PERF_LIMIT_REASONS	0x000006B1
// /*AFLA*/ 
// /*AFLA*/ /* Hardware P state interface */
// /*AFLA*/ #define MSR_PPERF			0x0000064e
// /*AFLA*/ #define MSR_PERF_LIMIT_REASONS		0x0000064f
// /*AFLA*/ #define MSR_PM_ENABLE			0x00000770
// /*AFLA*/ #define MSR_HWP_CAPABILITIES		0x00000771
// /*AFLA*/ #define MSR_HWP_REQUEST_PKG		0x00000772
// /*AFLA*/ #define MSR_HWP_INTERRUPT		0x00000773
// /*AFLA*/ #define MSR_HWP_REQUEST 		0x00000774
// /*AFLA*/ #define MSR_HWP_STATUS			0x00000777
// /*AFLA*/ 
// /*AFLA*/ /* CPUID.6.EAX */
// /*AFLA*/ #define HWP_BASE_BIT			(1<<7)
// /*AFLA*/ #define HWP_NOTIFICATIONS_BIT		(1<<8)
// /*AFLA*/ #define HWP_ACTIVITY_WINDOW_BIT		(1<<9)
// /*AFLA*/ #define HWP_ENERGY_PERF_PREFERENCE_BIT	(1<<10)
// /*AFLA*/ #define HWP_PACKAGE_LEVEL_REQUEST_BIT	(1<<11)
// /*AFLA*/ 
// /*AFLA*/ /* IA32_HWP_CAPABILITIES */
// /*AFLA*/ #define HWP_HIGHEST_PERF(x)		(((x) >> 0) & 0xff)
// /*AFLA*/ #define HWP_GUARANTEED_PERF(x)		(((x) >> 8) & 0xff)
// /*AFLA*/ #define HWP_MOSTEFFICIENT_PERF(x)	(((x) >> 16) & 0xff)
// /*AFLA*/ #define HWP_LOWEST_PERF(x)		(((x) >> 24) & 0xff)
// /*AFLA*/ 
// /*AFLA*/ /* IA32_HWP_REQUEST */
// /*AFLA*/ #define HWP_MIN_PERF(x) 		(x & 0xff)
// /*AFLA*/ #define HWP_MAX_PERF(x) 		((x & 0xff) << 8)
// /*AFLA*/ #define HWP_DESIRED_PERF(x)		((x & 0xff) << 16)
// /*AFLA*/ #define HWP_ENERGY_PERF_PREFERENCE(x)	((x & 0xff) << 24)
// /*AFLA*/ #define HWP_ACTIVITY_WINDOW(x)		((x & 0xff3) << 32)
// /*AFLA*/ #define HWP_PACKAGE_CONTROL(x)		((x & 0x1) << 42)
// /*AFLA*/ 
// /*AFLA*/ /* IA32_HWP_STATUS */
// /*AFLA*/ #define HWP_GUARANTEED_CHANGE(x)	(x & 0x1)
// /*AFLA*/ #define HWP_EXCURSION_TO_MINIMUM(x)	(x & 0x4)
// /*AFLA*/ 
// /*AFLA*/ /* IA32_HWP_INTERRUPT */
// /*AFLA*/ #define HWP_CHANGE_TO_GUARANTEED_INT(x)	(x & 0x1)
// /*AFLA*/ #define HWP_EXCURSION_TO_MINIMUM_INT(x)	(x & 0x2)
// /*AFLA*/ 
// /*AFLA*/ #define MSR_AMD64_MC0_MASK		0xc0010044
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_MCx_CTL(x)		(MSR_IA32_MC0_CTL + 4*(x))
// /*AFLA*/ #define MSR_IA32_MCx_STATUS(x)		(MSR_IA32_MC0_STATUS + 4*(x))
// /*AFLA*/ #define MSR_IA32_MCx_ADDR(x)		(MSR_IA32_MC0_ADDR + 4*(x))
// /*AFLA*/ #define MSR_IA32_MCx_MISC(x)		(MSR_IA32_MC0_MISC + 4*(x))
// /*AFLA*/ 
// /*AFLA*/ #define MSR_AMD64_MCx_MASK(x)		(MSR_AMD64_MC0_MASK + (x))
// /*AFLA*/ 
// /*AFLA*/ /* These are consecutive and not in the normal 4er MCE bank block */
// /*AFLA*/ #define MSR_IA32_MC0_CTL2		0x00000280
// /*AFLA*/ #define MSR_IA32_MCx_CTL2(x)		(MSR_IA32_MC0_CTL2 + (x))
// /*AFLA*/ 
// /*AFLA*/ #define MSR_P6_PERFCTR0			0x000000c1
// /*AFLA*/ #define MSR_P6_PERFCTR1			0x000000c2
// /*AFLA*/ #define MSR_P6_EVNTSEL0			0x00000186
// /*AFLA*/ #define MSR_P6_EVNTSEL1			0x00000187
// /*AFLA*/ 
// /*AFLA*/ #define MSR_KNC_PERFCTR0               0x00000020
// /*AFLA*/ #define MSR_KNC_PERFCTR1               0x00000021
// /*AFLA*/ #define MSR_KNC_EVNTSEL0               0x00000028
// /*AFLA*/ #define MSR_KNC_EVNTSEL1               0x00000029
// /*AFLA*/ 
// /*AFLA*/ /* Alternative perfctr range with full access. */
// /*AFLA*/ #define MSR_IA32_PMC0			0x000004c1
// /*AFLA*/ 
// /*AFLA*/ /* AMD64 MSRs. Not complete. See the architecture manual for a more
// /*AFLA*/    complete list. */
// /*AFLA*/ 
// /*AFLA*/ #define MSR_AMD64_PATCH_LEVEL		0x0000008b
// /*AFLA*/ #define MSR_AMD64_TSC_RATIO		0xc0000104
// /*AFLA*/ #define MSR_AMD64_NB_CFG		0xc001001f
// /*AFLA*/ #define MSR_AMD64_PATCH_LOADER		0xc0010020
// /*AFLA*/ #define MSR_AMD64_OSVW_ID_LENGTH	0xc0010140
// /*AFLA*/ #define MSR_AMD64_OSVW_STATUS		0xc0010141
// /*AFLA*/ #define MSR_AMD64_LS_CFG		0xc0011020
// /*AFLA*/ #define MSR_AMD64_DC_CFG		0xc0011022
// /*AFLA*/ #define MSR_AMD64_BU_CFG2		0xc001102a
// /*AFLA*/ #define MSR_AMD64_IBSFETCHCTL		0xc0011030
// /*AFLA*/ #define MSR_AMD64_IBSFETCHLINAD		0xc0011031
// /*AFLA*/ #define MSR_AMD64_IBSFETCHPHYSAD	0xc0011032
// /*AFLA*/ #define MSR_AMD64_IBSFETCH_REG_COUNT	3
// /*AFLA*/ #define MSR_AMD64_IBSFETCH_REG_MASK	((1UL<<MSR_AMD64_IBSFETCH_REG_COUNT)-1)
// /*AFLA*/ #define MSR_AMD64_IBSOPCTL		0xc0011033
// /*AFLA*/ #define MSR_AMD64_IBSOPRIP		0xc0011034
// /*AFLA*/ #define MSR_AMD64_IBSOPDATA		0xc0011035
// /*AFLA*/ #define MSR_AMD64_IBSOPDATA2		0xc0011036
// /*AFLA*/ #define MSR_AMD64_IBSOPDATA3		0xc0011037
// /*AFLA*/ #define MSR_AMD64_IBSDCLINAD		0xc0011038
// /*AFLA*/ #define MSR_AMD64_IBSDCPHYSAD		0xc0011039
// /*AFLA*/ #define MSR_AMD64_IBSOP_REG_COUNT	7
// /*AFLA*/ #define MSR_AMD64_IBSOP_REG_MASK	((1UL<<MSR_AMD64_IBSOP_REG_COUNT)-1)
// /*AFLA*/ #define MSR_AMD64_IBSCTL		0xc001103a
// /*AFLA*/ #define MSR_AMD64_IBSBRTARGET		0xc001103b
// /*AFLA*/ #define MSR_AMD64_IBSOPDATA4		0xc001103d
// /*AFLA*/ #define MSR_AMD64_IBS_REG_COUNT_MAX	8 /* includes MSR_AMD64_IBSBRTARGET */
// /*AFLA*/ 
// /*AFLA*/ /* Fam 17h MSRs */
// /*AFLA*/ #define MSR_F17H_IRPERF			0xc00000e9
// /*AFLA*/ 
// /*AFLA*/ /* Fam 16h MSRs */
// /*AFLA*/ #define MSR_F16H_L2I_PERF_CTL		0xc0010230
// /*AFLA*/ #define MSR_F16H_L2I_PERF_CTR		0xc0010231
// /*AFLA*/ #define MSR_F16H_DR1_ADDR_MASK		0xc0011019
// /*AFLA*/ #define MSR_F16H_DR2_ADDR_MASK		0xc001101a
// /*AFLA*/ #define MSR_F16H_DR3_ADDR_MASK		0xc001101b
// /*AFLA*/ #define MSR_F16H_DR0_ADDR_MASK		0xc0011027
// /*AFLA*/ 
// /*AFLA*/ /* Fam 15h MSRs */
// /*AFLA*/ #define MSR_F15H_PERF_CTL		0xc0010200
// /*AFLA*/ #define MSR_F15H_PERF_CTR		0xc0010201
// /*AFLA*/ #define MSR_F15H_NB_PERF_CTL		0xc0010240
// /*AFLA*/ #define MSR_F15H_NB_PERF_CTR		0xc0010241
// /*AFLA*/ #define MSR_F15H_PTSC			0xc0010280
// /*AFLA*/ #define MSR_F15H_IC_CFG			0xc0011021
// /*AFLA*/ 
// /*AFLA*/ /* Fam 10h MSRs */
// /*AFLA*/ #define MSR_FAM10H_MMIO_CONF_BASE	0xc0010058
// /*AFLA*/ #define FAM10H_MMIO_CONF_ENABLE		(1<<0)
// /*AFLA*/ #define FAM10H_MMIO_CONF_BUSRANGE_MASK	0xf
// /*AFLA*/ #define FAM10H_MMIO_CONF_BUSRANGE_SHIFT 2
// /*AFLA*/ #define FAM10H_MMIO_CONF_BASE_MASK	0xfffffffULL
// /*AFLA*/ #define FAM10H_MMIO_CONF_BASE_SHIFT	20
// /*AFLA*/ #define MSR_FAM10H_NODE_ID		0xc001100c
// /*AFLA*/ 
// /*AFLA*/ /* K8 MSRs */
// /*AFLA*/ #define MSR_K8_TOP_MEM1			0xc001001a
// /*AFLA*/ #define MSR_K8_TOP_MEM2			0xc001001d
// /*AFLA*/ #define MSR_K8_SYSCFG			0xc0010010
// /*AFLA*/ #define MSR_K8_INT_PENDING_MSG		0xc0010055
// /*AFLA*/ /* C1E active bits in int pending message */
// /*AFLA*/ #define K8_INTP_C1E_ACTIVE_MASK		0x18000000
// /*AFLA*/ #define MSR_K8_TSEG_ADDR		0xc0010112
// /*AFLA*/ #define MSR_K8_TSEG_MASK		0xc0010113
// /*AFLA*/ #define K8_MTRRFIXRANGE_DRAM_ENABLE	0x00040000 /* MtrrFixDramEn bit    */
// /*AFLA*/ #define K8_MTRRFIXRANGE_DRAM_MODIFY	0x00080000 /* MtrrFixDramModEn bit */
// /*AFLA*/ #define K8_MTRR_RDMEM_WRMEM_MASK	0x18181818 /* Mask: RdMem|WrMem    */
// /*AFLA*/ 
// /*AFLA*/ /* K7 MSRs */
// /*AFLA*/ #define MSR_K7_EVNTSEL0			0xc0010000
// /*AFLA*/ #define MSR_K7_PERFCTR0			0xc0010004
// /*AFLA*/ #define MSR_K7_EVNTSEL1			0xc0010001
// /*AFLA*/ #define MSR_K7_PERFCTR1			0xc0010005
// /*AFLA*/ #define MSR_K7_EVNTSEL2			0xc0010002
// /*AFLA*/ #define MSR_K7_PERFCTR2			0xc0010006
// /*AFLA*/ #define MSR_K7_EVNTSEL3			0xc0010003
// /*AFLA*/ #define MSR_K7_PERFCTR3			0xc0010007
// /*AFLA*/ #define MSR_K7_CLK_CTL			0xc001001b
// /*AFLA*/ #define MSR_K7_HWCR			0xc0010015
// /*AFLA*/ #define MSR_K7_FID_VID_CTL		0xc0010041
// /*AFLA*/ #define MSR_K7_FID_VID_STATUS		0xc0010042
// /*AFLA*/ 
// /*AFLA*/ /* K6 MSRs */
// /*AFLA*/ #define MSR_K6_WHCR			0xc0000082
// /*AFLA*/ #define MSR_K6_UWCCR			0xc0000085
// /*AFLA*/ #define MSR_K6_EPMR			0xc0000086
// /*AFLA*/ #define MSR_K6_PSOR			0xc0000087
// /*AFLA*/ #define MSR_K6_PFIR			0xc0000088
// /*AFLA*/ 
// /*AFLA*/ /* Centaur-Hauls/IDT defined MSRs. */
// /*AFLA*/ #define MSR_IDT_FCR1			0x00000107
// /*AFLA*/ #define MSR_IDT_FCR2			0x00000108
// /*AFLA*/ #define MSR_IDT_FCR3			0x00000109
// /*AFLA*/ #define MSR_IDT_FCR4			0x0000010a
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IDT_MCR0			0x00000110
// /*AFLA*/ #define MSR_IDT_MCR1			0x00000111
// /*AFLA*/ #define MSR_IDT_MCR2			0x00000112
// /*AFLA*/ #define MSR_IDT_MCR3			0x00000113
// /*AFLA*/ #define MSR_IDT_MCR4			0x00000114
// /*AFLA*/ #define MSR_IDT_MCR5			0x00000115
// /*AFLA*/ #define MSR_IDT_MCR6			0x00000116
// /*AFLA*/ #define MSR_IDT_MCR7			0x00000117
// /*AFLA*/ #define MSR_IDT_MCR_CTRL		0x00000120
// /*AFLA*/ 
// /*AFLA*/ /* VIA Cyrix defined MSRs*/
// /*AFLA*/ #define MSR_VIA_FCR			0x00001107
// /*AFLA*/ #define MSR_VIA_LONGHAUL		0x0000110a
// /*AFLA*/ #define MSR_VIA_RNG			0x0000110b
// /*AFLA*/ #define MSR_VIA_BCR2			0x00001147
// /*AFLA*/ 
// /*AFLA*/ /* Transmeta defined MSRs */
// /*AFLA*/ #define MSR_TMTA_LONGRUN_CTRL		0x80868010
// /*AFLA*/ #define MSR_TMTA_LONGRUN_FLAGS		0x80868011
// /*AFLA*/ #define MSR_TMTA_LRTI_READOUT		0x80868018
// /*AFLA*/ #define MSR_TMTA_LRTI_VOLT_MHZ		0x8086801a
// /*AFLA*/ 
// /*AFLA*/ /* Intel defined MSRs. */
// /*AFLA*/ #define MSR_IA32_P5_MC_ADDR		0x00000000
// /*AFLA*/ #define MSR_IA32_P5_MC_TYPE		0x00000001
// /*AFLA*/ #define MSR_IA32_TSC			0x00000010
// /*AFLA*/ #define MSR_IA32_PLATFORM_ID		0x00000017
// /*AFLA*/ #define MSR_IA32_EBL_CR_POWERON		0x0000002a
// /*AFLA*/ #define MSR_EBC_FREQUENCY_ID		0x0000002c
// /*AFLA*/ #define MSR_SMI_COUNT			0x00000034
// /*AFLA*/ #define MSR_IA32_FEATURE_CONTROL        0x0000003a
// /*AFLA*/ #define MSR_IA32_TSC_ADJUST             0x0000003b
// /*AFLA*/ #define MSR_IA32_BNDCFGS		0x00000d90
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_XSS			0x00000da0
// /*AFLA*/ 
// /*AFLA*/ #define FEATURE_CONTROL_LOCKED				(1<<0)
// /*AFLA*/ #define FEATURE_CONTROL_VMXON_ENABLED_INSIDE_SMX	(1<<1)
// /*AFLA*/ #define FEATURE_CONTROL_VMXON_ENABLED_OUTSIDE_SMX	(1<<2)
// /*AFLA*/ #define FEATURE_CONTROL_LMCE				(1<<20)
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_APICBASE		0x0000001b
// /*AFLA*/ #define MSR_IA32_APICBASE_BSP		(1<<8)
// /*AFLA*/ #define MSR_IA32_APICBASE_ENABLE	(1<<11)
// /*AFLA*/ #define MSR_IA32_APICBASE_BASE		(0xfffff<<12)
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_TSCDEADLINE		0x000006e0
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_UCODE_WRITE		0x00000079
// /*AFLA*/ #define MSR_IA32_UCODE_REV		0x0000008b
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_SMM_MONITOR_CTL	0x0000009b
// /*AFLA*/ #define MSR_IA32_SMBASE			0x0000009e
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_PERF_STATUS		0x00000198
// /*AFLA*/ #define MSR_IA32_PERF_CTL		0x00000199
// /*AFLA*/ #define INTEL_PERF_CTL_MASK		0xffff
// /*AFLA*/ #define MSR_AMD_PSTATE_DEF_BASE		0xc0010064
// /*AFLA*/ #define MSR_AMD_PERF_STATUS		0xc0010063
// /*AFLA*/ #define MSR_AMD_PERF_CTL		0xc0010062
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_MPERF			0x000000e7
// /*AFLA*/ #define MSR_IA32_APERF			0x000000e8
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_THERM_CONTROL		0x0000019a
// /*AFLA*/ #define MSR_IA32_THERM_INTERRUPT	0x0000019b
// /*AFLA*/ 
// /*AFLA*/ #define THERM_INT_HIGH_ENABLE		(1 << 0)
// /*AFLA*/ #define THERM_INT_LOW_ENABLE		(1 << 1)
// /*AFLA*/ #define THERM_INT_PLN_ENABLE		(1 << 24)
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_THERM_STATUS		0x0000019c
// /*AFLA*/ 
// /*AFLA*/ #define THERM_STATUS_PROCHOT		(1 << 0)
// /*AFLA*/ #define THERM_STATUS_POWER_LIMIT	(1 << 10)
// /*AFLA*/ 
// /*AFLA*/ #define MSR_THERM2_CTL			0x0000019d
// /*AFLA*/ 
// /*AFLA*/ #define MSR_THERM2_CTL_TM_SELECT	(1ULL << 16)
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE		0x000001a0
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_TEMPERATURE_TARGET	0x000001a2
// /*AFLA*/ 
// /*AFLA*/ #define MSR_MISC_PWR_MGMT		0x000001aa
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_ENERGY_PERF_BIAS	0x000001b0
// /*AFLA*/ #define ENERGY_PERF_BIAS_PERFORMANCE	0
// /*AFLA*/ #define ENERGY_PERF_BIAS_NORMAL		6
// /*AFLA*/ #define ENERGY_PERF_BIAS_POWERSAVE	15
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_PACKAGE_THERM_STATUS		0x000001b1
// /*AFLA*/ 
// /*AFLA*/ #define PACKAGE_THERM_STATUS_PROCHOT		(1 << 0)
// /*AFLA*/ #define PACKAGE_THERM_STATUS_POWER_LIMIT	(1 << 10)
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_PACKAGE_THERM_INTERRUPT	0x000001b2
// /*AFLA*/ 
// /*AFLA*/ #define PACKAGE_THERM_INT_HIGH_ENABLE		(1 << 0)
// /*AFLA*/ #define PACKAGE_THERM_INT_LOW_ENABLE		(1 << 1)
// /*AFLA*/ #define PACKAGE_THERM_INT_PLN_ENABLE		(1 << 24)
// /*AFLA*/ 
// /*AFLA*/ /* Thermal Thresholds Support */
// /*AFLA*/ #define THERM_INT_THRESHOLD0_ENABLE    (1 << 15)
// /*AFLA*/ #define THERM_SHIFT_THRESHOLD0        8
// /*AFLA*/ #define THERM_MASK_THRESHOLD0          (0x7f << THERM_SHIFT_THRESHOLD0)
// /*AFLA*/ #define THERM_INT_THRESHOLD1_ENABLE    (1 << 23)
// /*AFLA*/ #define THERM_SHIFT_THRESHOLD1        16
// /*AFLA*/ #define THERM_MASK_THRESHOLD1          (0x7f << THERM_SHIFT_THRESHOLD1)
// /*AFLA*/ #define THERM_STATUS_THRESHOLD0        (1 << 6)
// /*AFLA*/ #define THERM_LOG_THRESHOLD0           (1 << 7)
// /*AFLA*/ #define THERM_STATUS_THRESHOLD1        (1 << 8)
// /*AFLA*/ #define THERM_LOG_THRESHOLD1           (1 << 9)
// /*AFLA*/ 
// /*AFLA*/ /* MISC_ENABLE bits: architectural */
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_FAST_STRING_BIT		0
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_FAST_STRING		(1ULL << MSR_IA32_MISC_ENABLE_FAST_STRING_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_TCC_BIT			1
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_TCC			(1ULL << MSR_IA32_MISC_ENABLE_TCC_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_EMON_BIT			7
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_EMON			(1ULL << MSR_IA32_MISC_ENABLE_EMON_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_BTS_UNAVAIL_BIT		11
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_BTS_UNAVAIL		(1ULL << MSR_IA32_MISC_ENABLE_BTS_UNAVAIL_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_PEBS_UNAVAIL_BIT		12
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_PEBS_UNAVAIL		(1ULL << MSR_IA32_MISC_ENABLE_PEBS_UNAVAIL_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_ENHANCED_SPEEDSTEP_BIT	16
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_ENHANCED_SPEEDSTEP		(1ULL << MSR_IA32_MISC_ENABLE_ENHANCED_SPEEDSTEP_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_MWAIT_BIT			18
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_MWAIT			(1ULL << MSR_IA32_MISC_ENABLE_MWAIT_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_LIMIT_CPUID_BIT		22
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_LIMIT_CPUID		(1ULL << MSR_IA32_MISC_ENABLE_LIMIT_CPUID_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_XTPR_DISABLE_BIT		23
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_XTPR_DISABLE		(1ULL << MSR_IA32_MISC_ENABLE_XTPR_DISABLE_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_XD_DISABLE_BIT		34
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_XD_DISABLE			(1ULL << MSR_IA32_MISC_ENABLE_XD_DISABLE_BIT)
// /*AFLA*/ 
// /*AFLA*/ /* MISC_ENABLE bits: model-specific, meaning may vary from core to core */
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_X87_COMPAT_BIT		2
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_X87_COMPAT			(1ULL << MSR_IA32_MISC_ENABLE_X87_COMPAT_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_TM1_BIT			3
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_TM1			(1ULL << MSR_IA32_MISC_ENABLE_TM1_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_SPLIT_LOCK_DISABLE_BIT	4
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_SPLIT_LOCK_DISABLE		(1ULL << MSR_IA32_MISC_ENABLE_SPLIT_LOCK_DISABLE_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_L3CACHE_DISABLE_BIT	6
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_L3CACHE_DISABLE		(1ULL << MSR_IA32_MISC_ENABLE_L3CACHE_DISABLE_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_SUPPRESS_LOCK_BIT		8
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_SUPPRESS_LOCK		(1ULL << MSR_IA32_MISC_ENABLE_SUPPRESS_LOCK_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_PREFETCH_DISABLE_BIT	9
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_PREFETCH_DISABLE		(1ULL << MSR_IA32_MISC_ENABLE_PREFETCH_DISABLE_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_FERR_BIT			10
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_FERR			(1ULL << MSR_IA32_MISC_ENABLE_FERR_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_FERR_MULTIPLEX_BIT		10
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_FERR_MULTIPLEX		(1ULL << MSR_IA32_MISC_ENABLE_FERR_MULTIPLEX_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_TM2_BIT			13
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_TM2			(1ULL << MSR_IA32_MISC_ENABLE_TM2_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_ADJ_PREF_DISABLE_BIT	19
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_ADJ_PREF_DISABLE		(1ULL << MSR_IA32_MISC_ENABLE_ADJ_PREF_DISABLE_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_SPEEDSTEP_LOCK_BIT		20
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_SPEEDSTEP_LOCK		(1ULL << MSR_IA32_MISC_ENABLE_SPEEDSTEP_LOCK_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_L1D_CONTEXT_BIT		24
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_L1D_CONTEXT		(1ULL << MSR_IA32_MISC_ENABLE_L1D_CONTEXT_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_DCU_PREF_DISABLE_BIT	37
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_DCU_PREF_DISABLE		(1ULL << MSR_IA32_MISC_ENABLE_DCU_PREF_DISABLE_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_TURBO_DISABLE_BIT		38
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_TURBO_DISABLE		(1ULL << MSR_IA32_MISC_ENABLE_TURBO_DISABLE_BIT)
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_IP_PREF_DISABLE_BIT	39
// /*AFLA*/ #define MSR_IA32_MISC_ENABLE_IP_PREF_DISABLE		(1ULL << MSR_IA32_MISC_ENABLE_IP_PREF_DISABLE_BIT)
// /*AFLA*/ 
// /*AFLA*/ #define MSR_IA32_TSC_DEADLINE		0x000006E0
// /*AFLA*/ 
// /*AFLA*/ /* P4/Xeon+ specific */
// /*AFLA*/ #define MSR_IA32_MCG_EAX		0x00000180
// /*AFLA*/ #define MSR_IA32_MCG_EBX		0x00000181
// /*AFLA*/ #define MSR_IA32_MCG_ECX		0x00000182
// /*AFLA*/ #define MSR_IA32_MCG_EDX		0x00000183
// /*AFLA*/ #define MSR_IA32_MCG_ESI		0x00000184
// /*AFLA*/ #define MSR_IA32_MCG_EDI		0x00000185
// /*AFLA*/ #define MSR_IA32_MCG_EBP		0x00000186
// /*AFLA*/ #define MSR_IA32_MCG_ESP		0x00000187
// /*AFLA*/ #define MSR_IA32_MCG_EFLAGS		0x00000188
// /*AFLA*/ #define MSR_IA32_MCG_EIP		0x00000189
// /*AFLA*/ #define MSR_IA32_MCG_RESERVED		0x0000018a
// /*AFLA*/ 
// /*AFLA*/ /* Pentium IV performance counter MSRs */
// /*AFLA*/ #define MSR_P4_BPU_PERFCTR0		0x00000300
// /*AFLA*/ #define MSR_P4_BPU_PERFCTR1		0x00000301
// /*AFLA*/ #define MSR_P4_BPU_PERFCTR2		0x00000302
// /*AFLA*/ #define MSR_P4_BPU_PERFCTR3		0x00000303
// /*AFLA*/ #define MSR_P4_MS_PERFCTR0		0x00000304
// /*AFLA*/ #define MSR_P4_MS_PERFCTR1		0x00000305
// /*AFLA*/ #define MSR_P4_MS_PERFCTR2		0x00000306
// /*AFLA*/ #define MSR_P4_MS_PERFCTR3		0x00000307
// /*AFLA*/ #define MSR_P4_FLAME_PERFCTR0		0x00000308
// /*AFLA*/ #define MSR_P4_FLAME_PERFCTR1		0x00000309
// /*AFLA*/ #define MSR_P4_FLAME_PERFCTR2		0x0000030a
// /*AFLA*/ #define MSR_P4_FLAME_PERFCTR3		0x0000030b
// /*AFLA*/ #define MSR_P4_IQ_PERFCTR0		0x0000030c
// /*AFLA*/ #define MSR_P4_IQ_PERFCTR1		0x0000030d
// /*AFLA*/ #define MSR_P4_IQ_PERFCTR2		0x0000030e
// /*AFLA*/ #define MSR_P4_IQ_PERFCTR3		0x0000030f
// /*AFLA*/ #define MSR_P4_IQ_PERFCTR4		0x00000310
// /*AFLA*/ #define MSR_P4_IQ_PERFCTR5		0x00000311
// /*AFLA*/ #define MSR_P4_BPU_CCCR0		0x00000360
// /*AFLA*/ #define MSR_P4_BPU_CCCR1		0x00000361
// /*AFLA*/ #define MSR_P4_BPU_CCCR2		0x00000362
// /*AFLA*/ #define MSR_P4_BPU_CCCR3		0x00000363
// /*AFLA*/ #define MSR_P4_MS_CCCR0			0x00000364
// /*AFLA*/ #define MSR_P4_MS_CCCR1			0x00000365
// /*AFLA*/ #define MSR_P4_MS_CCCR2			0x00000366
// /*AFLA*/ #define MSR_P4_MS_CCCR3			0x00000367
// /*AFLA*/ #define MSR_P4_FLAME_CCCR0		0x00000368
// /*AFLA*/ #define MSR_P4_FLAME_CCCR1		0x00000369
// /*AFLA*/ #define MSR_P4_FLAME_CCCR2		0x0000036a
// /*AFLA*/ #define MSR_P4_FLAME_CCCR3		0x0000036b
// /*AFLA*/ #define MSR_P4_IQ_CCCR0			0x0000036c
// /*AFLA*/ #define MSR_P4_IQ_CCCR1			0x0000036d
// /*AFLA*/ #define MSR_P4_IQ_CCCR2			0x0000036e
// /*AFLA*/ #define MSR_P4_IQ_CCCR3			0x0000036f
// /*AFLA*/ #define MSR_P4_IQ_CCCR4			0x00000370
// /*AFLA*/ #define MSR_P4_IQ_CCCR5			0x00000371
// /*AFLA*/ #define MSR_P4_ALF_ESCR0		0x000003ca
// /*AFLA*/ #define MSR_P4_ALF_ESCR1		0x000003cb
// /*AFLA*/ #define MSR_P4_BPU_ESCR0		0x000003b2
// /*AFLA*/ #define MSR_P4_BPU_ESCR1		0x000003b3
// /*AFLA*/ #define MSR_P4_BSU_ESCR0		0x000003a0
// /*AFLA*/ #define MSR_P4_BSU_ESCR1		0x000003a1
// /*AFLA*/ #define MSR_P4_CRU_ESCR0		0x000003b8
// /*AFLA*/ #define MSR_P4_CRU_ESCR1		0x000003b9
// /*AFLA*/ #define MSR_P4_CRU_ESCR2		0x000003cc
// /*AFLA*/ #define MSR_P4_CRU_ESCR3		0x000003cd
// /*AFLA*/ #define MSR_P4_CRU_ESCR4		0x000003e0
// /*AFLA*/ #define MSR_P4_CRU_ESCR5		0x000003e1
// /*AFLA*/ #define MSR_P4_DAC_ESCR0		0x000003a8
// /*AFLA*/ #define MSR_P4_DAC_ESCR1		0x000003a9
// /*AFLA*/ #define MSR_P4_FIRM_ESCR0		0x000003a4
// /*AFLA*/ #define MSR_P4_FIRM_ESCR1		0x000003a5
// /*AFLA*/ #define MSR_P4_FLAME_ESCR0		0x000003a6
// /*AFLA*/ #define MSR_P4_FLAME_ESCR1		0x000003a7
// /*AFLA*/ #define MSR_P4_FSB_ESCR0		0x000003a2
// /*AFLA*/ #define MSR_P4_FSB_ESCR1		0x000003a3
// /*AFLA*/ #define MSR_P4_IQ_ESCR0			0x000003ba
// /*AFLA*/ #define MSR_P4_IQ_ESCR1			0x000003bb
// /*AFLA*/ #define MSR_P4_IS_ESCR0			0x000003b4
// /*AFLA*/ #define MSR_P4_IS_ESCR1			0x000003b5
// /*AFLA*/ #define MSR_P4_ITLB_ESCR0		0x000003b6
// /*AFLA*/ #define MSR_P4_ITLB_ESCR1		0x000003b7
// /*AFLA*/ #define MSR_P4_IX_ESCR0			0x000003c8
// /*AFLA*/ #define MSR_P4_IX_ESCR1			0x000003c9
// /*AFLA*/ #define MSR_P4_MOB_ESCR0		0x000003aa
// /*AFLA*/ #define MSR_P4_MOB_ESCR1		0x000003ab
// /*AFLA*/ #define MSR_P4_MS_ESCR0			0x000003c0
// /*AFLA*/ #define MSR_P4_MS_ESCR1			0x000003c1
// /*AFLA*/ #define MSR_P4_PMH_ESCR0		0x000003ac
// /*AFLA*/ #define MSR_P4_PMH_ESCR1		0x000003ad
// /*AFLA*/ #define MSR_P4_RAT_ESCR0		0x000003bc
// /*AFLA*/ #define MSR_P4_RAT_ESCR1		0x000003bd
// /*AFLA*/ #define MSR_P4_SAAT_ESCR0		0x000003ae
// /*AFLA*/ #define MSR_P4_SAAT_ESCR1		0x000003af
// /*AFLA*/ #define MSR_P4_SSU_ESCR0		0x000003be
// /*AFLA*/ #define MSR_P4_SSU_ESCR1		0x000003bf /* guess: not in manual */
// /*AFLA*/ 
// /*AFLA*/ #define MSR_P4_TBPU_ESCR0		0x000003c2
// /*AFLA*/ #define MSR_P4_TBPU_ESCR1		0x000003c3
// /*AFLA*/ #define MSR_P4_TC_ESCR0			0x000003c4
// /*AFLA*/ #define MSR_P4_TC_ESCR1			0x000003c5
// /*AFLA*/ #define MSR_P4_U2L_ESCR0		0x000003b0
// /*AFLA*/ #define MSR_P4_U2L_ESCR1		0x000003b1
// /*AFLA*/ 
// /*AFLA*/ #define MSR_P4_PEBS_MATRIX_VERT		0x000003f2
// /*AFLA*/ 
// /*AFLA*/ /* Intel Core-based CPU performance counters */
// /*AFLA*/ #define MSR_CORE_PERF_FIXED_CTR0	0x00000309
// /*AFLA*/ #define MSR_CORE_PERF_FIXED_CTR1	0x0000030a
// /*AFLA*/ #define MSR_CORE_PERF_FIXED_CTR2	0x0000030b
// /*AFLA*/ #define MSR_CORE_PERF_FIXED_CTR_CTRL	0x0000038d
// /*AFLA*/ #define MSR_CORE_PERF_GLOBAL_STATUS	0x0000038e
// /*AFLA*/ #define MSR_CORE_PERF_GLOBAL_CTRL	0x0000038f
// /*AFLA*/ #define MSR_CORE_PERF_GLOBAL_OVF_CTRL	0x00000390
// /*AFLA*/ 
// /*AFLA*/ /* Geode defined MSRs */
// /*AFLA*/ #define MSR_GEODE_BUSCONT_CONF0		0x00001900
// /*AFLA*/ 
// /*AFLA*/ /* Intel VT MSRs */
// /*AFLA*/ #define MSR_IA32_VMX_BASIC              0x00000480
// /*AFLA*/ #define MSR_IA32_VMX_PINBASED_CTLS      0x00000481
// /*AFLA*/ #define MSR_IA32_VMX_PROCBASED_CTLS     0x00000482
// /*AFLA*/ #define MSR_IA32_VMX_EXIT_CTLS          0x00000483
// /*AFLA*/ #define MSR_IA32_VMX_ENTRY_CTLS         0x00000484
// /*AFLA*/ #define MSR_IA32_VMX_MISC               0x00000485
// /*AFLA*/ #define MSR_IA32_VMX_CR0_FIXED0         0x00000486
// /*AFLA*/ #define MSR_IA32_VMX_CR0_FIXED1         0x00000487
// /*AFLA*/ #define MSR_IA32_VMX_CR4_FIXED0         0x00000488
// /*AFLA*/ #define MSR_IA32_VMX_CR4_FIXED1         0x00000489
// /*AFLA*/ #define MSR_IA32_VMX_VMCS_ENUM          0x0000048a
// /*AFLA*/ #define MSR_IA32_VMX_PROCBASED_CTLS2    0x0000048b
// /*AFLA*/ #define MSR_IA32_VMX_EPT_VPID_CAP       0x0000048c
// /*AFLA*/ #define MSR_IA32_VMX_TRUE_PINBASED_CTLS  0x0000048d
// /*AFLA*/ #define MSR_IA32_VMX_TRUE_PROCBASED_CTLS 0x0000048e
// /*AFLA*/ #define MSR_IA32_VMX_TRUE_EXIT_CTLS      0x0000048f
// /*AFLA*/ #define MSR_IA32_VMX_TRUE_ENTRY_CTLS     0x00000490
// /*AFLA*/ #define MSR_IA32_VMX_VMFUNC             0x00000491
// /*AFLA*/ 
// /*AFLA*/ /* VMX_BASIC bits and bitmasks */
// /*AFLA*/ #define VMX_BASIC_VMCS_SIZE_SHIFT	32
// /*AFLA*/ #define VMX_BASIC_TRUE_CTLS		(1ULL << 55)
// /*AFLA*/ #define VMX_BASIC_64		0x0001000000000000LLU
// /*AFLA*/ #define VMX_BASIC_MEM_TYPE_SHIFT	50
// /*AFLA*/ #define VMX_BASIC_MEM_TYPE_MASK	0x003c000000000000LLU
// /*AFLA*/ #define VMX_BASIC_MEM_TYPE_WB	6LLU
// /*AFLA*/ #define VMX_BASIC_INOUT		0x0040000000000000LLU
// /*AFLA*/ 
// /*AFLA*/ /* MSR_IA32_VMX_MISC bits */
// /*AFLA*/ #define MSR_IA32_VMX_MISC_VMWRITE_SHADOW_RO_FIELDS (1ULL << 29)
// /*AFLA*/ #define MSR_IA32_VMX_MISC_PREEMPTION_TIMER_SCALE   0x1F
// /*AFLA*/ /* AMD-V MSRs */
// /*AFLA*/ 
// /*AFLA*/ #define MSR_VM_CR                       0xc0010114
// /*AFLA*/ #define MSR_VM_IGNNE                    0xc0010115
// /*AFLA*/ #define MSR_VM_HSAVE_PA                 0xc0010117
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_MSR_INDEX_H */
