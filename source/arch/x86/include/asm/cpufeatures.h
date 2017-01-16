// /*AFLA*/ #ifndef _ASM_X86_CPUFEATURES_H
// /*AFLA*/ #define _ASM_X86_CPUFEATURES_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef _ASM_X86_REQUIRED_FEATURES_H
// /*AFLA*/ #include <asm/required-features.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef _ASM_X86_DISABLED_FEATURES_H
// /*AFLA*/ #include <asm/disabled-features.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Defines x86 CPU feature bits
// /*AFLA*/  */
// /*AFLA*/ #define NCAPINTS	18	/* N 32-bit words worth of info */
// /*AFLA*/ #define NBUGINTS	1	/* N 32-bit bug flags */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Note: If the comment begins with a quoted string, that string is used
// /*AFLA*/  * in /proc/cpuinfo instead of the macro name.  If the string is "",
// /*AFLA*/  * this feature bit is not displayed in /proc/cpuinfo at all.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* Intel-defined CPU features, CPUID level 0x00000001 (edx), word 0 */
// /*AFLA*/ #define X86_FEATURE_FPU		( 0*32+ 0) /* Onboard FPU */
// /*AFLA*/ #define X86_FEATURE_VME		( 0*32+ 1) /* Virtual Mode Extensions */
// /*AFLA*/ #define X86_FEATURE_DE		( 0*32+ 2) /* Debugging Extensions */
// /*AFLA*/ #define X86_FEATURE_PSE		( 0*32+ 3) /* Page Size Extensions */
// /*AFLA*/ #define X86_FEATURE_TSC		( 0*32+ 4) /* Time Stamp Counter */
// /*AFLA*/ #define X86_FEATURE_MSR		( 0*32+ 5) /* Model-Specific Registers */
// /*AFLA*/ #define X86_FEATURE_PAE		( 0*32+ 6) /* Physical Address Extensions */
// /*AFLA*/ #define X86_FEATURE_MCE		( 0*32+ 7) /* Machine Check Exception */
// /*AFLA*/ #define X86_FEATURE_CX8		( 0*32+ 8) /* CMPXCHG8 instruction */
// /*AFLA*/ #define X86_FEATURE_APIC	( 0*32+ 9) /* Onboard APIC */
// /*AFLA*/ #define X86_FEATURE_SEP		( 0*32+11) /* SYSENTER/SYSEXIT */
// /*AFLA*/ #define X86_FEATURE_MTRR	( 0*32+12) /* Memory Type Range Registers */
// /*AFLA*/ #define X86_FEATURE_PGE		( 0*32+13) /* Page Global Enable */
// /*AFLA*/ #define X86_FEATURE_MCA		( 0*32+14) /* Machine Check Architecture */
// /*AFLA*/ #define X86_FEATURE_CMOV	( 0*32+15) /* CMOV instructions */
// /*AFLA*/ 					  /* (plus FCMOVcc, FCOMI with FPU) */
// /*AFLA*/ #define X86_FEATURE_PAT		( 0*32+16) /* Page Attribute Table */
// /*AFLA*/ #define X86_FEATURE_PSE36	( 0*32+17) /* 36-bit PSEs */
// /*AFLA*/ #define X86_FEATURE_PN		( 0*32+18) /* Processor serial number */
// /*AFLA*/ #define X86_FEATURE_CLFLUSH	( 0*32+19) /* CLFLUSH instruction */
// /*AFLA*/ #define X86_FEATURE_DS		( 0*32+21) /* "dts" Debug Store */
// /*AFLA*/ #define X86_FEATURE_ACPI	( 0*32+22) /* ACPI via MSR */
// /*AFLA*/ #define X86_FEATURE_MMX		( 0*32+23) /* Multimedia Extensions */
// /*AFLA*/ #define X86_FEATURE_FXSR	( 0*32+24) /* FXSAVE/FXRSTOR, CR4.OSFXSR */
// /*AFLA*/ #define X86_FEATURE_XMM		( 0*32+25) /* "sse" */
// /*AFLA*/ #define X86_FEATURE_XMM2	( 0*32+26) /* "sse2" */
// /*AFLA*/ #define X86_FEATURE_SELFSNOOP	( 0*32+27) /* "ss" CPU self snoop */
// /*AFLA*/ #define X86_FEATURE_HT		( 0*32+28) /* Hyper-Threading */
// /*AFLA*/ #define X86_FEATURE_ACC		( 0*32+29) /* "tm" Automatic clock control */
// /*AFLA*/ #define X86_FEATURE_IA64	( 0*32+30) /* IA-64 processor */
// /*AFLA*/ #define X86_FEATURE_PBE		( 0*32+31) /* Pending Break Enable */
// /*AFLA*/ 
// /*AFLA*/ /* AMD-defined CPU features, CPUID level 0x80000001, word 1 */
// /*AFLA*/ /* Don't duplicate feature flags which are redundant with Intel! */
// /*AFLA*/ #define X86_FEATURE_SYSCALL	( 1*32+11) /* SYSCALL/SYSRET */
// /*AFLA*/ #define X86_FEATURE_MP		( 1*32+19) /* MP Capable. */
// /*AFLA*/ #define X86_FEATURE_NX		( 1*32+20) /* Execute Disable */
// /*AFLA*/ #define X86_FEATURE_MMXEXT	( 1*32+22) /* AMD MMX extensions */
// /*AFLA*/ #define X86_FEATURE_FXSR_OPT	( 1*32+25) /* FXSAVE/FXRSTOR optimizations */
// /*AFLA*/ #define X86_FEATURE_GBPAGES	( 1*32+26) /* "pdpe1gb" GB pages */
// /*AFLA*/ #define X86_FEATURE_RDTSCP	( 1*32+27) /* RDTSCP */
// /*AFLA*/ #define X86_FEATURE_LM		( 1*32+29) /* Long Mode (x86-64) */
// /*AFLA*/ #define X86_FEATURE_3DNOWEXT	( 1*32+30) /* AMD 3DNow! extensions */
// /*AFLA*/ #define X86_FEATURE_3DNOW	( 1*32+31) /* 3DNow! */
// /*AFLA*/ 
// /*AFLA*/ /* Transmeta-defined CPU features, CPUID level 0x80860001, word 2 */
// /*AFLA*/ #define X86_FEATURE_RECOVERY	( 2*32+ 0) /* CPU in recovery mode */
// /*AFLA*/ #define X86_FEATURE_LONGRUN	( 2*32+ 1) /* Longrun power control */
// /*AFLA*/ #define X86_FEATURE_LRTI	( 2*32+ 3) /* LongRun table interface */
// /*AFLA*/ 
// /*AFLA*/ /* Other features, Linux-defined mapping, word 3 */
// /*AFLA*/ /* This range is used for feature bits which conflict or are synthesized */
// /*AFLA*/ #define X86_FEATURE_CXMMX	( 3*32+ 0) /* Cyrix MMX extensions */
// /*AFLA*/ #define X86_FEATURE_K6_MTRR	( 3*32+ 1) /* AMD K6 nonstandard MTRRs */
// /*AFLA*/ #define X86_FEATURE_CYRIX_ARR	( 3*32+ 2) /* Cyrix ARRs (= MTRRs) */
// /*AFLA*/ #define X86_FEATURE_CENTAUR_MCR	( 3*32+ 3) /* Centaur MCRs (= MTRRs) */
// /*AFLA*/ /* cpu types for specific tunings: */
// /*AFLA*/ #define X86_FEATURE_K8		( 3*32+ 4) /* "" Opteron, Athlon64 */
// /*AFLA*/ #define X86_FEATURE_K7		( 3*32+ 5) /* "" Athlon */
// /*AFLA*/ #define X86_FEATURE_P3		( 3*32+ 6) /* "" P3 */
// /*AFLA*/ #define X86_FEATURE_P4		( 3*32+ 7) /* "" P4 */
// /*AFLA*/ #define X86_FEATURE_CONSTANT_TSC ( 3*32+ 8) /* TSC ticks at a constant rate */
// /*AFLA*/ #define X86_FEATURE_UP		( 3*32+ 9) /* smp kernel running on up */
// /*AFLA*/ #define X86_FEATURE_ART		( 3*32+10) /* Platform has always running timer (ART) */
// /*AFLA*/ #define X86_FEATURE_ARCH_PERFMON ( 3*32+11) /* Intel Architectural PerfMon */
// /*AFLA*/ #define X86_FEATURE_PEBS	( 3*32+12) /* Precise-Event Based Sampling */
// /*AFLA*/ #define X86_FEATURE_BTS		( 3*32+13) /* Branch Trace Store */
// /*AFLA*/ #define X86_FEATURE_SYSCALL32	( 3*32+14) /* "" syscall in ia32 userspace */
// /*AFLA*/ #define X86_FEATURE_SYSENTER32	( 3*32+15) /* "" sysenter in ia32 userspace */
// /*AFLA*/ #define X86_FEATURE_REP_GOOD	( 3*32+16) /* rep microcode works well */
// /*AFLA*/ #define X86_FEATURE_MFENCE_RDTSC ( 3*32+17) /* "" Mfence synchronizes RDTSC */
// /*AFLA*/ #define X86_FEATURE_LFENCE_RDTSC ( 3*32+18) /* "" Lfence synchronizes RDTSC */
// /*AFLA*/ #define X86_FEATURE_ACC_POWER	( 3*32+19) /* AMD Accumulated Power Mechanism */
// /*AFLA*/ #define X86_FEATURE_NOPL	( 3*32+20) /* The NOPL (0F 1F) instructions */
// /*AFLA*/ #define X86_FEATURE_ALWAYS	( 3*32+21) /* "" Always-present feature */
// /*AFLA*/ #define X86_FEATURE_XTOPOLOGY	( 3*32+22) /* cpu topology enum extensions */
// /*AFLA*/ #define X86_FEATURE_TSC_RELIABLE ( 3*32+23) /* TSC is known to be reliable */
// /*AFLA*/ #define X86_FEATURE_NONSTOP_TSC	( 3*32+24) /* TSC does not stop in C states */
// /*AFLA*/ /* free, was #define X86_FEATURE_CLFLUSH_MONITOR ( 3*32+25) * "" clflush reqd with monitor */
// /*AFLA*/ #define X86_FEATURE_EXTD_APICID	( 3*32+26) /* has extended APICID (8 bits) */
// /*AFLA*/ #define X86_FEATURE_AMD_DCM     ( 3*32+27) /* multi-node processor */
// /*AFLA*/ #define X86_FEATURE_APERFMPERF	( 3*32+28) /* APERFMPERF */
// /*AFLA*/ #define X86_FEATURE_EAGER_FPU	( 3*32+29) /* "eagerfpu" Non lazy FPU restore */
// /*AFLA*/ #define X86_FEATURE_NONSTOP_TSC_S3 ( 3*32+30) /* TSC doesn't stop in S3 state */
// /*AFLA*/ 
// /*AFLA*/ /* Intel-defined CPU features, CPUID level 0x00000001 (ecx), word 4 */
// /*AFLA*/ #define X86_FEATURE_XMM3	( 4*32+ 0) /* "pni" SSE-3 */
// /*AFLA*/ #define X86_FEATURE_PCLMULQDQ	( 4*32+ 1) /* PCLMULQDQ instruction */
// /*AFLA*/ #define X86_FEATURE_DTES64	( 4*32+ 2) /* 64-bit Debug Store */
// /*AFLA*/ #define X86_FEATURE_MWAIT	( 4*32+ 3) /* "monitor" Monitor/Mwait support */
// /*AFLA*/ #define X86_FEATURE_DSCPL	( 4*32+ 4) /* "ds_cpl" CPL Qual. Debug Store */
// /*AFLA*/ #define X86_FEATURE_VMX		( 4*32+ 5) /* Hardware virtualization */
// /*AFLA*/ #define X86_FEATURE_SMX		( 4*32+ 6) /* Safer mode */
// /*AFLA*/ #define X86_FEATURE_EST		( 4*32+ 7) /* Enhanced SpeedStep */
// /*AFLA*/ #define X86_FEATURE_TM2		( 4*32+ 8) /* Thermal Monitor 2 */
// /*AFLA*/ #define X86_FEATURE_SSSE3	( 4*32+ 9) /* Supplemental SSE-3 */
// /*AFLA*/ #define X86_FEATURE_CID		( 4*32+10) /* Context ID */
// /*AFLA*/ #define X86_FEATURE_SDBG	( 4*32+11) /* Silicon Debug */
// /*AFLA*/ #define X86_FEATURE_FMA		( 4*32+12) /* Fused multiply-add */
// /*AFLA*/ #define X86_FEATURE_CX16	( 4*32+13) /* CMPXCHG16B */
// /*AFLA*/ #define X86_FEATURE_XTPR	( 4*32+14) /* Send Task Priority Messages */
// /*AFLA*/ #define X86_FEATURE_PDCM	( 4*32+15) /* Performance Capabilities */
// /*AFLA*/ #define X86_FEATURE_PCID	( 4*32+17) /* Process Context Identifiers */
// /*AFLA*/ #define X86_FEATURE_DCA		( 4*32+18) /* Direct Cache Access */
// /*AFLA*/ #define X86_FEATURE_XMM4_1	( 4*32+19) /* "sse4_1" SSE-4.1 */
// /*AFLA*/ #define X86_FEATURE_XMM4_2	( 4*32+20) /* "sse4_2" SSE-4.2 */
// /*AFLA*/ #define X86_FEATURE_X2APIC	( 4*32+21) /* x2APIC */
// /*AFLA*/ #define X86_FEATURE_MOVBE	( 4*32+22) /* MOVBE instruction */
// /*AFLA*/ #define X86_FEATURE_POPCNT      ( 4*32+23) /* POPCNT instruction */
// /*AFLA*/ #define X86_FEATURE_TSC_DEADLINE_TIMER	( 4*32+24) /* Tsc deadline timer */
// /*AFLA*/ #define X86_FEATURE_AES		( 4*32+25) /* AES instructions */
// /*AFLA*/ #define X86_FEATURE_XSAVE	( 4*32+26) /* XSAVE/XRSTOR/XSETBV/XGETBV */
// /*AFLA*/ #define X86_FEATURE_OSXSAVE	( 4*32+27) /* "" XSAVE enabled in the OS */
// /*AFLA*/ #define X86_FEATURE_AVX		( 4*32+28) /* Advanced Vector Extensions */
// /*AFLA*/ #define X86_FEATURE_F16C	( 4*32+29) /* 16-bit fp conversions */
// /*AFLA*/ #define X86_FEATURE_RDRAND	( 4*32+30) /* The RDRAND instruction */
// /*AFLA*/ #define X86_FEATURE_HYPERVISOR	( 4*32+31) /* Running on a hypervisor */
// /*AFLA*/ 
// /*AFLA*/ /* VIA/Cyrix/Centaur-defined CPU features, CPUID level 0xC0000001, word 5 */
// /*AFLA*/ #define X86_FEATURE_XSTORE	( 5*32+ 2) /* "rng" RNG present (xstore) */
// /*AFLA*/ #define X86_FEATURE_XSTORE_EN	( 5*32+ 3) /* "rng_en" RNG enabled */
// /*AFLA*/ #define X86_FEATURE_XCRYPT	( 5*32+ 6) /* "ace" on-CPU crypto (xcrypt) */
// /*AFLA*/ #define X86_FEATURE_XCRYPT_EN	( 5*32+ 7) /* "ace_en" on-CPU crypto enabled */
// /*AFLA*/ #define X86_FEATURE_ACE2	( 5*32+ 8) /* Advanced Cryptography Engine v2 */
// /*AFLA*/ #define X86_FEATURE_ACE2_EN	( 5*32+ 9) /* ACE v2 enabled */
// /*AFLA*/ #define X86_FEATURE_PHE		( 5*32+10) /* PadLock Hash Engine */
// /*AFLA*/ #define X86_FEATURE_PHE_EN	( 5*32+11) /* PHE enabled */
// /*AFLA*/ #define X86_FEATURE_PMM		( 5*32+12) /* PadLock Montgomery Multiplier */
// /*AFLA*/ #define X86_FEATURE_PMM_EN	( 5*32+13) /* PMM enabled */
// /*AFLA*/ 
// /*AFLA*/ /* More extended AMD flags: CPUID level 0x80000001, ecx, word 6 */
// /*AFLA*/ #define X86_FEATURE_LAHF_LM	( 6*32+ 0) /* LAHF/SAHF in long mode */
// /*AFLA*/ #define X86_FEATURE_CMP_LEGACY	( 6*32+ 1) /* If yes HyperThreading not valid */
// /*AFLA*/ #define X86_FEATURE_SVM		( 6*32+ 2) /* Secure virtual machine */
// /*AFLA*/ #define X86_FEATURE_EXTAPIC	( 6*32+ 3) /* Extended APIC space */
// /*AFLA*/ #define X86_FEATURE_CR8_LEGACY	( 6*32+ 4) /* CR8 in 32-bit mode */
// /*AFLA*/ #define X86_FEATURE_ABM		( 6*32+ 5) /* Advanced bit manipulation */
// /*AFLA*/ #define X86_FEATURE_SSE4A	( 6*32+ 6) /* SSE-4A */
// /*AFLA*/ #define X86_FEATURE_MISALIGNSSE ( 6*32+ 7) /* Misaligned SSE mode */
// /*AFLA*/ #define X86_FEATURE_3DNOWPREFETCH ( 6*32+ 8) /* 3DNow prefetch instructions */
// /*AFLA*/ #define X86_FEATURE_OSVW	( 6*32+ 9) /* OS Visible Workaround */
// /*AFLA*/ #define X86_FEATURE_IBS		( 6*32+10) /* Instruction Based Sampling */
// /*AFLA*/ #define X86_FEATURE_XOP		( 6*32+11) /* extended AVX instructions */
// /*AFLA*/ #define X86_FEATURE_SKINIT	( 6*32+12) /* SKINIT/STGI instructions */
// /*AFLA*/ #define X86_FEATURE_WDT		( 6*32+13) /* Watchdog timer */
// /*AFLA*/ #define X86_FEATURE_LWP		( 6*32+15) /* Light Weight Profiling */
// /*AFLA*/ #define X86_FEATURE_FMA4	( 6*32+16) /* 4 operands MAC instructions */
// /*AFLA*/ #define X86_FEATURE_TCE		( 6*32+17) /* translation cache extension */
// /*AFLA*/ #define X86_FEATURE_NODEID_MSR	( 6*32+19) /* NodeId MSR */
// /*AFLA*/ #define X86_FEATURE_TBM		( 6*32+21) /* trailing bit manipulations */
// /*AFLA*/ #define X86_FEATURE_TOPOEXT	( 6*32+22) /* topology extensions CPUID leafs */
// /*AFLA*/ #define X86_FEATURE_PERFCTR_CORE ( 6*32+23) /* core performance counter extensions */
// /*AFLA*/ #define X86_FEATURE_PERFCTR_NB  ( 6*32+24) /* NB performance counter extensions */
// /*AFLA*/ #define X86_FEATURE_BPEXT	(6*32+26) /* data breakpoint extension */
// /*AFLA*/ #define X86_FEATURE_PTSC	( 6*32+27) /* performance time-stamp counter */
// /*AFLA*/ #define X86_FEATURE_PERFCTR_L2	( 6*32+28) /* L2 performance counter extensions */
// /*AFLA*/ #define X86_FEATURE_MWAITX	( 6*32+29) /* MWAIT extension (MONITORX/MWAITX) */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Auxiliary flags: Linux defined - For features scattered in various
// /*AFLA*/  * CPUID levels like 0x6, 0xA etc, word 7.
// /*AFLA*/  *
// /*AFLA*/  * Reuse free bits when adding new feature flags!
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define X86_FEATURE_CPB		( 7*32+ 2) /* AMD Core Performance Boost */
// /*AFLA*/ #define X86_FEATURE_EPB		( 7*32+ 3) /* IA32_ENERGY_PERF_BIAS support */
// /*AFLA*/ 
// /*AFLA*/ #define X86_FEATURE_HW_PSTATE	( 7*32+ 8) /* AMD HW-PState */
// /*AFLA*/ #define X86_FEATURE_PROC_FEEDBACK ( 7*32+ 9) /* AMD ProcFeedbackInterface */
// /*AFLA*/ 
// /*AFLA*/ #define X86_FEATURE_INTEL_PT	( 7*32+15) /* Intel Processor Trace */
// /*AFLA*/ #define X86_FEATURE_AVX512_4VNNIW (7*32+16) /* AVX-512 Neural Network Instructions */
// /*AFLA*/ #define X86_FEATURE_AVX512_4FMAPS (7*32+17) /* AVX-512 Multiply Accumulation Single precision */
// /*AFLA*/ 
// /*AFLA*/ /* Virtualization flags: Linux defined, word 8 */
// /*AFLA*/ #define X86_FEATURE_TPR_SHADOW  ( 8*32+ 0) /* Intel TPR Shadow */
// /*AFLA*/ #define X86_FEATURE_VNMI        ( 8*32+ 1) /* Intel Virtual NMI */
// /*AFLA*/ #define X86_FEATURE_FLEXPRIORITY ( 8*32+ 2) /* Intel FlexPriority */
// /*AFLA*/ #define X86_FEATURE_EPT         ( 8*32+ 3) /* Intel Extended Page Table */
// /*AFLA*/ #define X86_FEATURE_VPID        ( 8*32+ 4) /* Intel Virtual Processor ID */
// /*AFLA*/ 
// /*AFLA*/ #define X86_FEATURE_VMMCALL     ( 8*32+15) /* Prefer vmmcall to vmcall */
// /*AFLA*/ #define X86_FEATURE_XENPV       ( 8*32+16) /* "" Xen paravirtual guest */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* Intel-defined CPU features, CPUID level 0x00000007:0 (ebx), word 9 */
// /*AFLA*/ #define X86_FEATURE_FSGSBASE	( 9*32+ 0) /* {RD/WR}{FS/GS}BASE instructions*/
// /*AFLA*/ #define X86_FEATURE_TSC_ADJUST	( 9*32+ 1) /* TSC adjustment MSR 0x3b */
// /*AFLA*/ #define X86_FEATURE_BMI1	( 9*32+ 3) /* 1st group bit manipulation extensions */
// /*AFLA*/ #define X86_FEATURE_HLE		( 9*32+ 4) /* Hardware Lock Elision */
// /*AFLA*/ #define X86_FEATURE_AVX2	( 9*32+ 5) /* AVX2 instructions */
// /*AFLA*/ #define X86_FEATURE_SMEP	( 9*32+ 7) /* Supervisor Mode Execution Protection */
// /*AFLA*/ #define X86_FEATURE_BMI2	( 9*32+ 8) /* 2nd group bit manipulation extensions */
// /*AFLA*/ #define X86_FEATURE_ERMS	( 9*32+ 9) /* Enhanced REP MOVSB/STOSB */
// /*AFLA*/ #define X86_FEATURE_INVPCID	( 9*32+10) /* Invalidate Processor Context ID */
// /*AFLA*/ #define X86_FEATURE_RTM		( 9*32+11) /* Restricted Transactional Memory */
// /*AFLA*/ #define X86_FEATURE_CQM		( 9*32+12) /* Cache QoS Monitoring */
// /*AFLA*/ #define X86_FEATURE_MPX		( 9*32+14) /* Memory Protection Extension */
// /*AFLA*/ #define X86_FEATURE_AVX512F	( 9*32+16) /* AVX-512 Foundation */
// /*AFLA*/ #define X86_FEATURE_AVX512DQ	( 9*32+17) /* AVX-512 DQ (Double/Quad granular) Instructions */
// /*AFLA*/ #define X86_FEATURE_RDSEED	( 9*32+18) /* The RDSEED instruction */
// /*AFLA*/ #define X86_FEATURE_ADX		( 9*32+19) /* The ADCX and ADOX instructions */
// /*AFLA*/ #define X86_FEATURE_SMAP	( 9*32+20) /* Supervisor Mode Access Prevention */
// /*AFLA*/ #define X86_FEATURE_CLFLUSHOPT	( 9*32+23) /* CLFLUSHOPT instruction */
// /*AFLA*/ #define X86_FEATURE_CLWB	( 9*32+24) /* CLWB instruction */
// /*AFLA*/ #define X86_FEATURE_AVX512PF	( 9*32+26) /* AVX-512 Prefetch */
// /*AFLA*/ #define X86_FEATURE_AVX512ER	( 9*32+27) /* AVX-512 Exponential and Reciprocal */
// /*AFLA*/ #define X86_FEATURE_AVX512CD	( 9*32+28) /* AVX-512 Conflict Detection */
// /*AFLA*/ #define X86_FEATURE_SHA_NI	( 9*32+29) /* SHA1/SHA256 Instruction Extensions */
// /*AFLA*/ #define X86_FEATURE_AVX512BW	( 9*32+30) /* AVX-512 BW (Byte/Word granular) Instructions */
// /*AFLA*/ #define X86_FEATURE_AVX512VL	( 9*32+31) /* AVX-512 VL (128/256 Vector Length) Extensions */
// /*AFLA*/ 
// /*AFLA*/ /* Extended state features, CPUID level 0x0000000d:1 (eax), word 10 */
// /*AFLA*/ #define X86_FEATURE_XSAVEOPT	(10*32+ 0) /* XSAVEOPT */
// /*AFLA*/ #define X86_FEATURE_XSAVEC	(10*32+ 1) /* XSAVEC */
// /*AFLA*/ #define X86_FEATURE_XGETBV1	(10*32+ 2) /* XGETBV with ECX = 1 */
// /*AFLA*/ #define X86_FEATURE_XSAVES	(10*32+ 3) /* XSAVES/XRSTORS */
// /*AFLA*/ 
// /*AFLA*/ /* Intel-defined CPU QoS Sub-leaf, CPUID level 0x0000000F:0 (edx), word 11 */
// /*AFLA*/ #define X86_FEATURE_CQM_LLC	(11*32+ 1) /* LLC QoS if 1 */
// /*AFLA*/ 
// /*AFLA*/ /* Intel-defined CPU QoS Sub-leaf, CPUID level 0x0000000F:1 (edx), word 12 */
// /*AFLA*/ #define X86_FEATURE_CQM_OCCUP_LLC (12*32+ 0) /* LLC occupancy monitoring if 1 */
// /*AFLA*/ #define X86_FEATURE_CQM_MBM_TOTAL (12*32+ 1) /* LLC Total MBM monitoring */
// /*AFLA*/ #define X86_FEATURE_CQM_MBM_LOCAL (12*32+ 2) /* LLC Local MBM monitoring */
// /*AFLA*/ 
// /*AFLA*/ /* AMD-defined CPU features, CPUID level 0x80000008 (ebx), word 13 */
// /*AFLA*/ #define X86_FEATURE_CLZERO	(13*32+0) /* CLZERO instruction */
// /*AFLA*/ #define X86_FEATURE_IRPERF	(13*32+1) /* Instructions Retired Count */
// /*AFLA*/ 
// /*AFLA*/ /* Thermal and Power Management Leaf, CPUID level 0x00000006 (eax), word 14 */
// /*AFLA*/ #define X86_FEATURE_DTHERM	(14*32+ 0) /* Digital Thermal Sensor */
// /*AFLA*/ #define X86_FEATURE_IDA		(14*32+ 1) /* Intel Dynamic Acceleration */
// /*AFLA*/ #define X86_FEATURE_ARAT	(14*32+ 2) /* Always Running APIC Timer */
// /*AFLA*/ #define X86_FEATURE_PLN		(14*32+ 4) /* Intel Power Limit Notification */
// /*AFLA*/ #define X86_FEATURE_PTS		(14*32+ 6) /* Intel Package Thermal Status */
// /*AFLA*/ #define X86_FEATURE_HWP		(14*32+ 7) /* Intel Hardware P-states */
// /*AFLA*/ #define X86_FEATURE_HWP_NOTIFY	(14*32+ 8) /* HWP Notification */
// /*AFLA*/ #define X86_FEATURE_HWP_ACT_WINDOW (14*32+ 9) /* HWP Activity Window */
// /*AFLA*/ #define X86_FEATURE_HWP_EPP	(14*32+10) /* HWP Energy Perf. Preference */
// /*AFLA*/ #define X86_FEATURE_HWP_PKG_REQ (14*32+11) /* HWP Package Level Request */
// /*AFLA*/ 
// /*AFLA*/ /* AMD SVM Feature Identification, CPUID level 0x8000000a (edx), word 15 */
// /*AFLA*/ #define X86_FEATURE_NPT		(15*32+ 0) /* Nested Page Table support */
// /*AFLA*/ #define X86_FEATURE_LBRV	(15*32+ 1) /* LBR Virtualization support */
// /*AFLA*/ #define X86_FEATURE_SVML	(15*32+ 2) /* "svm_lock" SVM locking MSR */
// /*AFLA*/ #define X86_FEATURE_NRIPS	(15*32+ 3) /* "nrip_save" SVM next_rip save */
// /*AFLA*/ #define X86_FEATURE_TSCRATEMSR  (15*32+ 4) /* "tsc_scale" TSC scaling support */
// /*AFLA*/ #define X86_FEATURE_VMCBCLEAN   (15*32+ 5) /* "vmcb_clean" VMCB clean bits support */
// /*AFLA*/ #define X86_FEATURE_FLUSHBYASID (15*32+ 6) /* flush-by-ASID support */
// /*AFLA*/ #define X86_FEATURE_DECODEASSISTS (15*32+ 7) /* Decode Assists support */
// /*AFLA*/ #define X86_FEATURE_PAUSEFILTER (15*32+10) /* filtered pause intercept */
// /*AFLA*/ #define X86_FEATURE_PFTHRESHOLD (15*32+12) /* pause filter threshold */
// /*AFLA*/ #define X86_FEATURE_AVIC	(15*32+13) /* Virtual Interrupt Controller */
// /*AFLA*/ 
// /*AFLA*/ /* Intel-defined CPU features, CPUID level 0x00000007:0 (ecx), word 16 */
// /*AFLA*/ #define X86_FEATURE_PKU		(16*32+ 3) /* Protection Keys for Userspace */
// /*AFLA*/ #define X86_FEATURE_OSPKE	(16*32+ 4) /* OS Protection Keys Enable */
// /*AFLA*/ 
// /*AFLA*/ /* AMD-defined CPU features, CPUID level 0x80000007 (ebx), word 17 */
// /*AFLA*/ #define X86_FEATURE_OVERFLOW_RECOV (17*32+0) /* MCA overflow recovery support */
// /*AFLA*/ #define X86_FEATURE_SUCCOR	(17*32+1) /* Uncorrectable error containment and recovery */
// /*AFLA*/ #define X86_FEATURE_SMCA	(17*32+3) /* Scalable MCA */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * BUG word(s)
// /*AFLA*/  */
// /*AFLA*/ #define X86_BUG(x)		(NCAPINTS*32 + (x))
// /*AFLA*/ 
// /*AFLA*/ #define X86_BUG_F00F		X86_BUG(0) /* Intel F00F */
// /*AFLA*/ #define X86_BUG_FDIV		X86_BUG(1) /* FPU FDIV */
// /*AFLA*/ #define X86_BUG_COMA		X86_BUG(2) /* Cyrix 6x86 coma */
// /*AFLA*/ #define X86_BUG_AMD_TLB_MMATCH	X86_BUG(3) /* "tlb_mmatch" AMD Erratum 383 */
// /*AFLA*/ #define X86_BUG_AMD_APIC_C1E	X86_BUG(4) /* "apic_c1e" AMD Erratum 400 */
// /*AFLA*/ #define X86_BUG_11AP		X86_BUG(5) /* Bad local APIC aka 11AP */
// /*AFLA*/ #define X86_BUG_FXSAVE_LEAK	X86_BUG(6) /* FXSAVE leaks FOP/FIP/FOP */
// /*AFLA*/ #define X86_BUG_CLFLUSH_MONITOR	X86_BUG(7) /* AAI65, CLFLUSH required before MONITOR */
// /*AFLA*/ #define X86_BUG_SYSRET_SS_ATTRS	X86_BUG(8) /* SYSRET doesn't fix up SS attrs */
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ /*
// /*AFLA*/  * 64-bit kernels don't use X86_BUG_ESPFIX.  Make the define conditional
// /*AFLA*/  * to avoid confusion.
// /*AFLA*/  */
// /*AFLA*/ #define X86_BUG_ESPFIX		X86_BUG(9) /* "" IRET to 16-bit SS corrupts ESP/RSP high bits */
// /*AFLA*/ #endif
// /*AFLA*/ #define X86_BUG_NULL_SEG	X86_BUG(10) /* Nulling a selector preserves the base */
// /*AFLA*/ #define X86_BUG_SWAPGS_FENCE	X86_BUG(11) /* SWAPGS without input dep on GS */
// /*AFLA*/ #define X86_BUG_MONITOR		X86_BUG(12) /* IPI required to wake up remote CPU */
// /*AFLA*/ #endif /* _ASM_X86_CPUFEATURES_H */
