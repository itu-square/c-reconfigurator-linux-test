// /*AFLA*/ /******************************************************************************
// /*AFLA*/  * arch-x86_32.h
// /*AFLA*/  *
// /*AFLA*/  * Guest OS interface to x86 Xen.
// /*AFLA*/  *
// /*AFLA*/  * Permission is hereby granted, free of charge, to any person obtaining a copy
// /*AFLA*/  * of this software and associated documentation files (the "Software"), to
// /*AFLA*/  * deal in the Software without restriction, including without limitation the
// /*AFLA*/  * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// /*AFLA*/  * sell copies of the Software, and to permit persons to whom the Software is
// /*AFLA*/  * furnished to do so, subject to the following conditions:
// /*AFLA*/  *
// /*AFLA*/  * The above copyright notice and this permission notice shall be included in
// /*AFLA*/  * all copies or substantial portions of the Software.
// /*AFLA*/  *
// /*AFLA*/  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// /*AFLA*/  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// /*AFLA*/  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// /*AFLA*/  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// /*AFLA*/  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// /*AFLA*/  * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// /*AFLA*/  * DEALINGS IN THE SOFTWARE.
// /*AFLA*/  *
// /*AFLA*/  * Copyright (c) 2004-2006, K A Fraser
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _ASM_X86_XEN_INTERFACE_H
// /*AFLA*/ #define _ASM_X86_XEN_INTERFACE_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * XEN_GUEST_HANDLE represents a guest pointer, when passed as a field
// /*AFLA*/  * in a struct in memory.
// /*AFLA*/  * XEN_GUEST_HANDLE_PARAM represent a guest pointer, when passed as an
// /*AFLA*/  * hypercall argument.
// /*AFLA*/  * XEN_GUEST_HANDLE_PARAM and XEN_GUEST_HANDLE are the same on X86 but
// /*AFLA*/  * they might not be on other architectures.
// /*AFLA*/  */
// /*AFLA*/ #ifdef __XEN__
// /*AFLA*/ #define __DEFINE_GUEST_HANDLE(name, type) \
// /*AFLA*/     typedef struct { type *p; } __guest_handle_ ## name
// /*AFLA*/ #else
// /*AFLA*/ #define __DEFINE_GUEST_HANDLE(name, type) \
// /*AFLA*/     typedef type * __guest_handle_ ## name
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_GUEST_HANDLE_STRUCT(name) \
// /*AFLA*/ 	__DEFINE_GUEST_HANDLE(name, struct name)
// /*AFLA*/ #define DEFINE_GUEST_HANDLE(name) __DEFINE_GUEST_HANDLE(name, name)
// /*AFLA*/ #define GUEST_HANDLE(name)        __guest_handle_ ## name
// /*AFLA*/ 
// /*AFLA*/ #ifdef __XEN__
// /*AFLA*/ #if defined(__i386__)
// /*AFLA*/ #define set_xen_guest_handle(hnd, val)			\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		if (sizeof(hnd) == 8)			\
// /*AFLA*/ 			*(uint64_t *)&(hnd) = 0;	\
// /*AFLA*/ 		(hnd).p = val;				\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #elif defined(__x86_64__)
// /*AFLA*/ #define set_xen_guest_handle(hnd, val)	do { (hnd).p = val; } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ #else
// /*AFLA*/ #if defined(__i386__)
// /*AFLA*/ #define set_xen_guest_handle(hnd, val)			\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		if (sizeof(hnd) == 8)			\
// /*AFLA*/ 			*(uint64_t *)&(hnd) = 0;	\
// /*AFLA*/ 		(hnd) = val;				\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #elif defined(__x86_64__)
// /*AFLA*/ #define set_xen_guest_handle(hnd, val)	do { (hnd) = val; } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ /* Explicitly size integers that represent pfns in the public interface
// /*AFLA*/  * with Xen so that on ARM we can have one ABI that works for 32 and 64
// /*AFLA*/  * bit guests. */
// /*AFLA*/ typedef unsigned long xen_pfn_t;
// /*AFLA*/ #define PRI_xen_pfn "lx"
// /*AFLA*/ typedef unsigned long xen_ulong_t;
// /*AFLA*/ #define PRI_xen_ulong "lx"
// /*AFLA*/ typedef long xen_long_t;
// /*AFLA*/ #define PRI_xen_long "lx"
// /*AFLA*/ 
// /*AFLA*/ /* Guest handles for primitive C types. */
// /*AFLA*/ __DEFINE_GUEST_HANDLE(uchar, unsigned char);
// /*AFLA*/ __DEFINE_GUEST_HANDLE(uint,  unsigned int);
// /*AFLA*/ DEFINE_GUEST_HANDLE(char);
// /*AFLA*/ DEFINE_GUEST_HANDLE(int);
// /*AFLA*/ DEFINE_GUEST_HANDLE(void);
// /*AFLA*/ DEFINE_GUEST_HANDLE(uint64_t);
// /*AFLA*/ DEFINE_GUEST_HANDLE(uint32_t);
// /*AFLA*/ DEFINE_GUEST_HANDLE(xen_pfn_t);
// /*AFLA*/ DEFINE_GUEST_HANDLE(xen_ulong_t);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef HYPERVISOR_VIRT_START
// /*AFLA*/ #define HYPERVISOR_VIRT_START mk_unsigned_long(__HYPERVISOR_VIRT_START)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define MACH2PHYS_VIRT_START  mk_unsigned_long(__MACH2PHYS_VIRT_START)
// /*AFLA*/ #define MACH2PHYS_VIRT_END    mk_unsigned_long(__MACH2PHYS_VIRT_END)
// /*AFLA*/ #define MACH2PHYS_NR_ENTRIES  ((MACH2PHYS_VIRT_END-MACH2PHYS_VIRT_START)>>__MACH2PHYS_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ /* Maximum number of virtual CPUs in multi-processor guests. */
// /*AFLA*/ #define MAX_VIRT_CPUS 32
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * SEGMENT DESCRIPTOR TABLES
// /*AFLA*/  */
// /*AFLA*/ /*
// /*AFLA*/  * A number of GDT entries are reserved by Xen. These are not situated at the
// /*AFLA*/  * start of the GDT because some stupid OSes export hard-coded selector values
// /*AFLA*/  * in their ABI. These hard-coded values are always near the start of the GDT,
// /*AFLA*/  * so Xen places itself out of the way, at the far end of the GDT.
// /*AFLA*/  *
// /*AFLA*/  * NB The LDT is set using the MMUEXT_SET_LDT op of HYPERVISOR_mmuext_op
// /*AFLA*/  */
// /*AFLA*/ #define FIRST_RESERVED_GDT_PAGE  14
// /*AFLA*/ #define FIRST_RESERVED_GDT_BYTE  (FIRST_RESERVED_GDT_PAGE * 4096)
// /*AFLA*/ #define FIRST_RESERVED_GDT_ENTRY (FIRST_RESERVED_GDT_BYTE / 8)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Send an array of these to HYPERVISOR_set_trap_table().
// /*AFLA*/  * Terminate the array with a sentinel entry, with traps[].address==0.
// /*AFLA*/  * The privilege level specifies which modes may enter a trap via a software
// /*AFLA*/  * interrupt. On x86/64, since rings 1 and 2 are unavailable, we allocate
// /*AFLA*/  * privilege levels as follows:
// /*AFLA*/  *  Level == 0: No one may enter
// /*AFLA*/  *  Level == 1: Kernel may enter
// /*AFLA*/  *  Level == 2: Kernel may enter
// /*AFLA*/  *  Level == 3: Everyone may enter
// /*AFLA*/  */
// /*AFLA*/ #define TI_GET_DPL(_ti)		((_ti)->flags & 3)
// /*AFLA*/ #define TI_GET_IF(_ti)		((_ti)->flags & 4)
// /*AFLA*/ #define TI_SET_DPL(_ti, _dpl)	((_ti)->flags |= (_dpl))
// /*AFLA*/ #define TI_SET_IF(_ti, _if)	((_ti)->flags |= ((!!(_if))<<2))
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ struct trap_info {
// /*AFLA*/     uint8_t       vector;  /* exception vector                              */
// /*AFLA*/     uint8_t       flags;   /* 0-3: privilege level; 4: clear event enable?  */
// /*AFLA*/     uint16_t      cs;      /* code selector                                 */
// /*AFLA*/     unsigned long address; /* code offset                                   */
// /*AFLA*/ };
// /*AFLA*/ DEFINE_GUEST_HANDLE_STRUCT(trap_info);
// /*AFLA*/ 
// /*AFLA*/ struct arch_shared_info {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Number of valid entries in the p2m table(s) anchored at
// /*AFLA*/ 	 * pfn_to_mfn_frame_list_list and/or p2m_vaddr.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long max_pfn;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Frame containing list of mfns containing list of mfns containing p2m.
// /*AFLA*/ 	 * A value of 0 indicates it has not yet been set up, ~0 indicates it
// /*AFLA*/ 	 * has been set to invalid e.g. due to the p2m being too large for the
// /*AFLA*/ 	 * 3-level p2m tree. In this case the linear mapper p2m list anchored
// /*AFLA*/ 	 * at p2m_vaddr is to be used.
// /*AFLA*/ 	 */
// /*AFLA*/ 	xen_pfn_t pfn_to_mfn_frame_list_list;
// /*AFLA*/ 	unsigned long nmi_reason;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Following three fields are valid if p2m_cr3 contains a value
// /*AFLA*/ 	 * different from 0.
// /*AFLA*/ 	 * p2m_cr3 is the root of the address space where p2m_vaddr is valid.
// /*AFLA*/ 	 * p2m_cr3 is in the same format as a cr3 value in the vcpu register
// /*AFLA*/ 	 * state and holds the folded machine frame number (via xen_pfn_to_cr3)
// /*AFLA*/ 	 * of a L3 or L4 page table.
// /*AFLA*/ 	 * p2m_vaddr holds the virtual address of the linear p2m list. All
// /*AFLA*/ 	 * entries in the range [0...max_pfn[ are accessible via this pointer.
// /*AFLA*/ 	 * p2m_generation will be incremented by the guest before and after each
// /*AFLA*/ 	 * change of the mappings of the p2m list. p2m_generation starts at 0
// /*AFLA*/ 	 * and a value with the least significant bit set indicates that a
// /*AFLA*/ 	 * mapping update is in progress. This allows guest external software
// /*AFLA*/ 	 * (e.g. in Dom0) to verify that read mappings are consistent and
// /*AFLA*/ 	 * whether they have changed since the last check.
// /*AFLA*/ 	 * Modifying a p2m element in the linear p2m list is allowed via an
// /*AFLA*/ 	 * atomic write only.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long p2m_cr3;		/* cr3 value of the p2m address space */
// /*AFLA*/ 	unsigned long p2m_vaddr;	/* virtual address of the p2m list */
// /*AFLA*/ 	unsigned long p2m_generation;	/* generation count of p2m mapping */
// /*AFLA*/ };
// /*AFLA*/ #endif	/* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ #include <asm/xen/interface_32.h>
// /*AFLA*/ #else
// /*AFLA*/ #include <asm/xen/interface_64.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <asm/pvclock-abi.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ /*
// /*AFLA*/  * The following is all CPU context. Note that the fpu_ctxt block is filled
// /*AFLA*/  * in by FXSAVE if the CPU has feature FXSR; otherwise FSAVE is used.
// /*AFLA*/  *
// /*AFLA*/  * Also note that when calling DOMCTL_setvcpucontext and VCPU_initialise
// /*AFLA*/  * for HVM and PVH guests, not all information in this structure is updated:
// /*AFLA*/  *
// /*AFLA*/  * - For HVM guests, the structures read include: fpu_ctxt (if
// /*AFLA*/  * VGCT_I387_VALID is set), flags, user_regs, debugreg[*]
// /*AFLA*/  *
// /*AFLA*/  * - PVH guests are the same as HVM guests, but additionally use ctrlreg[3] to
// /*AFLA*/  * set cr3. All other fields not used should be set to 0.
// /*AFLA*/  */
// /*AFLA*/ struct vcpu_guest_context {
// /*AFLA*/     /* FPU registers come first so they can be aligned for FXSAVE/FXRSTOR. */
// /*AFLA*/     struct { char x[512]; } fpu_ctxt;       /* User-level FPU registers     */
// /*AFLA*/ #define VGCF_I387_VALID                (1<<0)
// /*AFLA*/ #define VGCF_IN_KERNEL                 (1<<2)
// /*AFLA*/ #define _VGCF_i387_valid               0
// /*AFLA*/ #define VGCF_i387_valid                (1<<_VGCF_i387_valid)
// /*AFLA*/ #define _VGCF_in_kernel                2
// /*AFLA*/ #define VGCF_in_kernel                 (1<<_VGCF_in_kernel)
// /*AFLA*/ #define _VGCF_failsafe_disables_events 3
// /*AFLA*/ #define VGCF_failsafe_disables_events  (1<<_VGCF_failsafe_disables_events)
// /*AFLA*/ #define _VGCF_syscall_disables_events  4
// /*AFLA*/ #define VGCF_syscall_disables_events   (1<<_VGCF_syscall_disables_events)
// /*AFLA*/ #define _VGCF_online                   5
// /*AFLA*/ #define VGCF_online                    (1<<_VGCF_online)
// /*AFLA*/     unsigned long flags;                    /* VGCF_* flags                 */
// /*AFLA*/     struct cpu_user_regs user_regs;         /* User-level CPU registers     */
// /*AFLA*/     struct trap_info trap_ctxt[256];        /* Virtual IDT                  */
// /*AFLA*/     unsigned long ldt_base, ldt_ents;       /* LDT (linear address, # ents) */
// /*AFLA*/     unsigned long gdt_frames[16], gdt_ents; /* GDT (machine frames, # ents) */
// /*AFLA*/     unsigned long kernel_ss, kernel_sp;     /* Virtual TSS (only SS1/SP1)   */
// /*AFLA*/     /* NB. User pagetable on x86/64 is placed in ctrlreg[1]. */
// /*AFLA*/     unsigned long ctrlreg[8];               /* CR0-CR7 (control registers)  */
// /*AFLA*/     unsigned long debugreg[8];              /* DB0-DB7 (debug registers)    */
// /*AFLA*/ #ifdef __i386__
// /*AFLA*/     unsigned long event_callback_cs;        /* CS:EIP of event callback     */
// /*AFLA*/     unsigned long event_callback_eip;
// /*AFLA*/     unsigned long failsafe_callback_cs;     /* CS:EIP of failsafe callback  */
// /*AFLA*/     unsigned long failsafe_callback_eip;
// /*AFLA*/ #else
// /*AFLA*/     unsigned long event_callback_eip;
// /*AFLA*/     unsigned long failsafe_callback_eip;
// /*AFLA*/     unsigned long syscall_callback_eip;
// /*AFLA*/ #endif
// /*AFLA*/     unsigned long vm_assist;                /* VMASST_TYPE_* bitmap */
// /*AFLA*/ #ifdef __x86_64__
// /*AFLA*/     /* Segment base addresses. */
// /*AFLA*/     uint64_t      fs_base;
// /*AFLA*/     uint64_t      gs_base_kernel;
// /*AFLA*/     uint64_t      gs_base_user;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ DEFINE_GUEST_HANDLE_STRUCT(vcpu_guest_context);
// /*AFLA*/ 
// /*AFLA*/ /* AMD PMU registers and structures */
// /*AFLA*/ struct xen_pmu_amd_ctxt {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Offsets to counter and control MSRs (relative to xen_pmu_arch.c.amd).
// /*AFLA*/ 	 * For PV(H) guests these fields are RO.
// /*AFLA*/ 	 */
// /*AFLA*/ 	uint32_t counters;
// /*AFLA*/ 	uint32_t ctrls;
// /*AFLA*/ 
// /*AFLA*/ 	/* Counter MSRs */
// /*AFLA*/ #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
// /*AFLA*/ 	uint64_t regs[];
// /*AFLA*/ #elif defined(__GNUC__)
// /*AFLA*/ 	uint64_t regs[0];
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Intel PMU registers and structures */
// /*AFLA*/ struct xen_pmu_cntr_pair {
// /*AFLA*/ 	uint64_t counter;
// /*AFLA*/ 	uint64_t control;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct xen_pmu_intel_ctxt {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Offsets to fixed and architectural counter MSRs (relative to
// /*AFLA*/ 	 * xen_pmu_arch.c.intel).
// /*AFLA*/ 	 * For PV(H) guests these fields are RO.
// /*AFLA*/ 	 */
// /*AFLA*/ 	uint32_t fixed_counters;
// /*AFLA*/ 	uint32_t arch_counters;
// /*AFLA*/ 
// /*AFLA*/ 	/* PMU registers */
// /*AFLA*/ 	uint64_t global_ctrl;
// /*AFLA*/ 	uint64_t global_ovf_ctrl;
// /*AFLA*/ 	uint64_t global_status;
// /*AFLA*/ 	uint64_t fixed_ctrl;
// /*AFLA*/ 	uint64_t ds_area;
// /*AFLA*/ 	uint64_t pebs_enable;
// /*AFLA*/ 	uint64_t debugctl;
// /*AFLA*/ 
// /*AFLA*/ 	/* Fixed and architectural counter MSRs */
// /*AFLA*/ #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
// /*AFLA*/ 	uint64_t regs[];
// /*AFLA*/ #elif defined(__GNUC__)
// /*AFLA*/ 	uint64_t regs[0];
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Sampled domain's registers */
// /*AFLA*/ struct xen_pmu_regs {
// /*AFLA*/ 	uint64_t ip;
// /*AFLA*/ 	uint64_t sp;
// /*AFLA*/ 	uint64_t flags;
// /*AFLA*/ 	uint16_t cs;
// /*AFLA*/ 	uint16_t ss;
// /*AFLA*/ 	uint8_t cpl;
// /*AFLA*/ 	uint8_t pad[3];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* PMU flags */
// /*AFLA*/ #define PMU_CACHED	   (1<<0) /* PMU MSRs are cached in the context */
// /*AFLA*/ #define PMU_SAMPLE_USER	   (1<<1) /* Sample is from user or kernel mode */
// /*AFLA*/ #define PMU_SAMPLE_REAL	   (1<<2) /* Sample is from realmode */
// /*AFLA*/ #define PMU_SAMPLE_PV	   (1<<3) /* Sample from a PV guest */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Architecture-specific information describing state of the processor at
// /*AFLA*/  * the time of PMU interrupt.
// /*AFLA*/  * Fields of this structure marked as RW for guest should only be written by
// /*AFLA*/  * the guest when PMU_CACHED bit in pmu_flags is set (which is done by the
// /*AFLA*/  * hypervisor during PMU interrupt). Hypervisor will read updated data in
// /*AFLA*/  * XENPMU_flush hypercall and clear PMU_CACHED bit.
// /*AFLA*/  */
// /*AFLA*/ struct xen_pmu_arch {
// /*AFLA*/ 	union {
// /*AFLA*/ 		/*
// /*AFLA*/ 		 * Processor's registers at the time of interrupt.
// /*AFLA*/ 		 * WO for hypervisor, RO for guests.
// /*AFLA*/ 		 */
// /*AFLA*/ 		struct xen_pmu_regs regs;
// /*AFLA*/ 		/*
// /*AFLA*/ 		 * Padding for adding new registers to xen_pmu_regs in
// /*AFLA*/ 		 * the future
// /*AFLA*/ 		 */
// /*AFLA*/ #define XENPMU_REGS_PAD_SZ  64
// /*AFLA*/ 		uint8_t pad[XENPMU_REGS_PAD_SZ];
// /*AFLA*/ 	} r;
// /*AFLA*/ 
// /*AFLA*/ 	/* WO for hypervisor, RO for guest */
// /*AFLA*/ 	uint64_t pmu_flags;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * APIC LVTPC register.
// /*AFLA*/ 	 * RW for both hypervisor and guest.
// /*AFLA*/ 	 * Only APIC_LVT_MASKED bit is loaded by the hypervisor into hardware
// /*AFLA*/ 	 * during XENPMU_flush or XENPMU_lvtpc_set.
// /*AFLA*/ 	 */
// /*AFLA*/ 	union {
// /*AFLA*/ 		uint32_t lapic_lvtpc;
// /*AFLA*/ 		uint64_t pad;
// /*AFLA*/ 	} l;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Vendor-specific PMU registers.
// /*AFLA*/ 	 * RW for both hypervisor and guest (see exceptions above).
// /*AFLA*/ 	 * Guest's updates to this field are verified and then loaded by the
// /*AFLA*/ 	 * hypervisor into hardware during XENPMU_flush
// /*AFLA*/ 	 */
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct xen_pmu_amd_ctxt amd;
// /*AFLA*/ 		struct xen_pmu_intel_ctxt intel;
// /*AFLA*/ 
// /*AFLA*/ 		/*
// /*AFLA*/ 		 * Padding for contexts (fixed parts only, does not include
// /*AFLA*/ 		 * MSR banks that are specified by offsets)
// /*AFLA*/ 		 */
// /*AFLA*/ #define XENPMU_CTXT_PAD_SZ  128
// /*AFLA*/ 		uint8_t pad[XENPMU_CTXT_PAD_SZ];
// /*AFLA*/ 	} c;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif	/* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Prefix forces emulation of some non-trapping instructions.
// /*AFLA*/  * Currently only CPUID.
// /*AFLA*/  */
// /*AFLA*/ #ifdef __ASSEMBLY__
// /*AFLA*/ #define XEN_EMULATE_PREFIX .byte 0x0f,0x0b,0x78,0x65,0x6e ;
// /*AFLA*/ #define XEN_CPUID          XEN_EMULATE_PREFIX cpuid
// /*AFLA*/ #else
// /*AFLA*/ #define XEN_EMULATE_PREFIX ".byte 0x0f,0x0b,0x78,0x65,0x6e ; "
// /*AFLA*/ #define XEN_CPUID          XEN_EMULATE_PREFIX "cpuid"
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_XEN_INTERFACE_H */
