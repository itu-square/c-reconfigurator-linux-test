// /*AFLA*/ /******************************************************************************
// /*AFLA*/  * xen.h
// /*AFLA*/  *
// /*AFLA*/  * Guest OS interface to Xen.
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
// /*AFLA*/  * Copyright (c) 2004, K A Fraser
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __XEN_PUBLIC_XEN_H__
// /*AFLA*/ #define __XEN_PUBLIC_XEN_H__
// /*AFLA*/ 
// /*AFLA*/ #include <asm/xen/interface.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * XEN "SYSTEM CALLS" (a.k.a. HYPERCALLS).
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * x86_32: EAX = vector; EBX, ECX, EDX, ESI, EDI = args 1, 2, 3, 4, 5.
// /*AFLA*/  *         EAX = return value
// /*AFLA*/  *         (argument registers may be clobbered on return)
// /*AFLA*/  * x86_64: RAX = vector; RDI, RSI, RDX, R10, R8, R9 = args 1, 2, 3, 4, 5, 6.
// /*AFLA*/  *         RAX = return value
// /*AFLA*/  *         (argument registers not clobbered on return; RCX, R11 are)
// /*AFLA*/  */
// /*AFLA*/ #define __HYPERVISOR_set_trap_table        0
// /*AFLA*/ #define __HYPERVISOR_mmu_update            1
// /*AFLA*/ #define __HYPERVISOR_set_gdt               2
// /*AFLA*/ #define __HYPERVISOR_stack_switch          3
// /*AFLA*/ #define __HYPERVISOR_set_callbacks         4
// /*AFLA*/ #define __HYPERVISOR_fpu_taskswitch        5
// /*AFLA*/ #define __HYPERVISOR_sched_op_compat       6
// /*AFLA*/ #define __HYPERVISOR_platform_op           7
// /*AFLA*/ #define __HYPERVISOR_set_debugreg          8
// /*AFLA*/ #define __HYPERVISOR_get_debugreg          9
// /*AFLA*/ #define __HYPERVISOR_update_descriptor    10
// /*AFLA*/ #define __HYPERVISOR_memory_op            12
// /*AFLA*/ #define __HYPERVISOR_multicall            13
// /*AFLA*/ #define __HYPERVISOR_update_va_mapping    14
// /*AFLA*/ #define __HYPERVISOR_set_timer_op         15
// /*AFLA*/ #define __HYPERVISOR_event_channel_op_compat 16
// /*AFLA*/ #define __HYPERVISOR_xen_version          17
// /*AFLA*/ #define __HYPERVISOR_console_io           18
// /*AFLA*/ #define __HYPERVISOR_physdev_op_compat    19
// /*AFLA*/ #define __HYPERVISOR_grant_table_op       20
// /*AFLA*/ #define __HYPERVISOR_vm_assist            21
// /*AFLA*/ #define __HYPERVISOR_update_va_mapping_otherdomain 22
// /*AFLA*/ #define __HYPERVISOR_iret                 23 /* x86 only */
// /*AFLA*/ #define __HYPERVISOR_vcpu_op              24
// /*AFLA*/ #define __HYPERVISOR_set_segment_base     25 /* x86/64 only */
// /*AFLA*/ #define __HYPERVISOR_mmuext_op            26
// /*AFLA*/ #define __HYPERVISOR_xsm_op               27
// /*AFLA*/ #define __HYPERVISOR_nmi_op               28
// /*AFLA*/ #define __HYPERVISOR_sched_op             29
// /*AFLA*/ #define __HYPERVISOR_callback_op          30
// /*AFLA*/ #define __HYPERVISOR_xenoprof_op          31
// /*AFLA*/ #define __HYPERVISOR_event_channel_op     32
// /*AFLA*/ #define __HYPERVISOR_physdev_op           33
// /*AFLA*/ #define __HYPERVISOR_hvm_op               34
// /*AFLA*/ #define __HYPERVISOR_sysctl               35
// /*AFLA*/ #define __HYPERVISOR_domctl               36
// /*AFLA*/ #define __HYPERVISOR_kexec_op             37
// /*AFLA*/ #define __HYPERVISOR_tmem_op              38
// /*AFLA*/ #define __HYPERVISOR_xc_reserved_op       39 /* reserved for XenClient */
// /*AFLA*/ #define __HYPERVISOR_xenpmu_op            40
// /*AFLA*/ 
// /*AFLA*/ /* Architecture-specific hypercall definitions. */
// /*AFLA*/ #define __HYPERVISOR_arch_0               48
// /*AFLA*/ #define __HYPERVISOR_arch_1               49
// /*AFLA*/ #define __HYPERVISOR_arch_2               50
// /*AFLA*/ #define __HYPERVISOR_arch_3               51
// /*AFLA*/ #define __HYPERVISOR_arch_4               52
// /*AFLA*/ #define __HYPERVISOR_arch_5               53
// /*AFLA*/ #define __HYPERVISOR_arch_6               54
// /*AFLA*/ #define __HYPERVISOR_arch_7               55
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * VIRTUAL INTERRUPTS
// /*AFLA*/  *
// /*AFLA*/  * Virtual interrupts that a guest OS may receive from Xen.
// /*AFLA*/  * In the side comments, 'V.' denotes a per-VCPU VIRQ while 'G.' denotes a
// /*AFLA*/  * global VIRQ. The former can be bound once per VCPU and cannot be re-bound.
// /*AFLA*/  * The latter can be allocated only once per guest: they must initially be
// /*AFLA*/  * allocated to VCPU0 but can subsequently be re-bound.
// /*AFLA*/  */
// /*AFLA*/ #define VIRQ_TIMER      0  /* V. Timebase update, and/or requested timeout.  */
// /*AFLA*/ #define VIRQ_DEBUG      1  /* V. Request guest to dump debug info.           */
// /*AFLA*/ #define VIRQ_CONSOLE    2  /* G. (DOM0) Bytes received on emergency console. */
// /*AFLA*/ #define VIRQ_DOM_EXC    3  /* G. (DOM0) Exceptional event for some domain.   */
// /*AFLA*/ #define VIRQ_TBUF       4  /* G. (DOM0) Trace buffer has records available.  */
// /*AFLA*/ #define VIRQ_DEBUGGER   6  /* G. (DOM0) A domain has paused for debugging.   */
// /*AFLA*/ #define VIRQ_XENOPROF   7  /* V. XenOprofile interrupt: new sample available */
// /*AFLA*/ #define VIRQ_CON_RING   8  /* G. (DOM0) Bytes received on console            */
// /*AFLA*/ #define VIRQ_PCPU_STATE 9  /* G. (DOM0) PCPU state changed                   */
// /*AFLA*/ #define VIRQ_MEM_EVENT  10 /* G. (DOM0) A memory event has occured           */
// /*AFLA*/ #define VIRQ_XC_RESERVED 11 /* G. Reserved for XenClient                     */
// /*AFLA*/ #define VIRQ_ENOMEM     12 /* G. (DOM0) Low on heap memory       */
// /*AFLA*/ #define VIRQ_XENPMU     13  /* PMC interrupt                                 */
// /*AFLA*/ 
// /*AFLA*/ /* Architecture-specific VIRQ definitions. */
// /*AFLA*/ #define VIRQ_ARCH_0    16
// /*AFLA*/ #define VIRQ_ARCH_1    17
// /*AFLA*/ #define VIRQ_ARCH_2    18
// /*AFLA*/ #define VIRQ_ARCH_3    19
// /*AFLA*/ #define VIRQ_ARCH_4    20
// /*AFLA*/ #define VIRQ_ARCH_5    21
// /*AFLA*/ #define VIRQ_ARCH_6    22
// /*AFLA*/ #define VIRQ_ARCH_7    23
// /*AFLA*/ 
// /*AFLA*/ #define NR_VIRQS       24
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * enum neg_errnoval HYPERVISOR_mmu_update(const struct mmu_update reqs[],
// /*AFLA*/  *                                         unsigned count, unsigned *done_out,
// /*AFLA*/  *                                         unsigned foreigndom)
// /*AFLA*/  * @reqs is an array of mmu_update_t structures ((ptr, val) pairs).
// /*AFLA*/  * @count is the length of the above array.
// /*AFLA*/  * @pdone is an output parameter indicating number of completed operations
// /*AFLA*/  * @foreigndom[15:0]: FD, the expected owner of data pages referenced in this
// /*AFLA*/  *                    hypercall invocation. Can be DOMID_SELF.
// /*AFLA*/  * @foreigndom[31:16]: PFD, the expected owner of pagetable pages referenced
// /*AFLA*/  *                     in this hypercall invocation. The value of this field
// /*AFLA*/  *                     (x) encodes the PFD as follows:
// /*AFLA*/  *                     x == 0 => PFD == DOMID_SELF
// /*AFLA*/  *                     x != 0 => PFD == x - 1
// /*AFLA*/  *
// /*AFLA*/  * Sub-commands: ptr[1:0] specifies the appropriate MMU_* command.
// /*AFLA*/  * -------------
// /*AFLA*/  * ptr[1:0] == MMU_NORMAL_PT_UPDATE:
// /*AFLA*/  * Updates an entry in a page table belonging to PFD. If updating an L1 table,
// /*AFLA*/  * and the new table entry is valid/present, the mapped frame must belong to
// /*AFLA*/  * FD. If attempting to map an I/O page then the caller assumes the privilege
// /*AFLA*/  * of the FD.
// /*AFLA*/  * FD == DOMID_IO: Permit /only/ I/O mappings, at the priv level of the caller.
// /*AFLA*/  * FD == DOMID_XEN: Map restricted areas of Xen's heap space.
// /*AFLA*/  * ptr[:2]  -- Machine address of the page-table entry to modify.
// /*AFLA*/  * val      -- Value to write.
// /*AFLA*/  *
// /*AFLA*/  * There also certain implicit requirements when using this hypercall. The
// /*AFLA*/  * pages that make up a pagetable must be mapped read-only in the guest.
// /*AFLA*/  * This prevents uncontrolled guest updates to the pagetable. Xen strictly
// /*AFLA*/  * enforces this, and will disallow any pagetable update which will end up
// /*AFLA*/  * mapping pagetable page RW, and will disallow using any writable page as a
// /*AFLA*/  * pagetable. In practice it means that when constructing a page table for a
// /*AFLA*/  * process, thread, etc, we MUST be very dilligient in following these rules:
// /*AFLA*/  *  1). Start with top-level page (PGD or in Xen language: L4). Fill out
// /*AFLA*/  *      the entries.
// /*AFLA*/  *  2). Keep on going, filling out the upper (PUD or L3), and middle (PMD
// /*AFLA*/  *      or L2).
// /*AFLA*/  *  3). Start filling out the PTE table (L1) with the PTE entries. Once
// /*AFLA*/  *      done, make sure to set each of those entries to RO (so writeable bit
// /*AFLA*/  *      is unset). Once that has been completed, set the PMD (L2) for this
// /*AFLA*/  *      PTE table as RO.
// /*AFLA*/  *  4). When completed with all of the PMD (L2) entries, and all of them have
// /*AFLA*/  *      been set to RO, make sure to set RO the PUD (L3). Do the same
// /*AFLA*/  *      operation on PGD (L4) pagetable entries that have a PUD (L3) entry.
// /*AFLA*/  *  5). Now before you can use those pages (so setting the cr3), you MUST also
// /*AFLA*/  *      pin them so that the hypervisor can verify the entries. This is done
// /*AFLA*/  *      via the HYPERVISOR_mmuext_op(MMUEXT_PIN_L4_TABLE, guest physical frame
// /*AFLA*/  *      number of the PGD (L4)). And this point the HYPERVISOR_mmuext_op(
// /*AFLA*/  *      MMUEXT_NEW_BASEPTR, guest physical frame number of the PGD (L4)) can be
// /*AFLA*/  *      issued.
// /*AFLA*/  * For 32-bit guests, the L4 is not used (as there is less pagetables), so
// /*AFLA*/  * instead use L3.
// /*AFLA*/  * At this point the pagetables can be modified using the MMU_NORMAL_PT_UPDATE
// /*AFLA*/  * hypercall. Also if so desired the OS can also try to write to the PTE
// /*AFLA*/  * and be trapped by the hypervisor (as the PTE entry is RO).
// /*AFLA*/  *
// /*AFLA*/  * To deallocate the pages, the operations are the reverse of the steps
// /*AFLA*/  * mentioned above. The argument is MMUEXT_UNPIN_TABLE for all levels and the
// /*AFLA*/  * pagetable MUST not be in use (meaning that the cr3 is not set to it).
// /*AFLA*/  *
// /*AFLA*/  * ptr[1:0] == MMU_MACHPHYS_UPDATE:
// /*AFLA*/  * Updates an entry in the machine->pseudo-physical mapping table.
// /*AFLA*/  * ptr[:2]  -- Machine address within the frame whose mapping to modify.
// /*AFLA*/  *             The frame must belong to the FD, if one is specified.
// /*AFLA*/  * val      -- Value to write into the mapping entry.
// /*AFLA*/  *
// /*AFLA*/  * ptr[1:0] == MMU_PT_UPDATE_PRESERVE_AD:
// /*AFLA*/  * As MMU_NORMAL_PT_UPDATE above, but A/D bits currently in the PTE are ORed
// /*AFLA*/  * with those in @val.
// /*AFLA*/  *
// /*AFLA*/  * @val is usually the machine frame number along with some attributes.
// /*AFLA*/  * The attributes by default follow the architecture defined bits. Meaning that
// /*AFLA*/  * if this is a X86_64 machine and four page table layout is used, the layout
// /*AFLA*/  * of val is:
// /*AFLA*/  *  - 63 if set means No execute (NX)
// /*AFLA*/  *  - 46-13 the machine frame number
// /*AFLA*/  *  - 12 available for guest
// /*AFLA*/  *  - 11 available for guest
// /*AFLA*/  *  - 10 available for guest
// /*AFLA*/  *  - 9 available for guest
// /*AFLA*/  *  - 8 global
// /*AFLA*/  *  - 7 PAT (PSE is disabled, must use hypercall to make 4MB or 2MB pages)
// /*AFLA*/  *  - 6 dirty
// /*AFLA*/  *  - 5 accessed
// /*AFLA*/  *  - 4 page cached disabled
// /*AFLA*/  *  - 3 page write through
// /*AFLA*/  *  - 2 userspace accessible
// /*AFLA*/  *  - 1 writeable
// /*AFLA*/  *  - 0 present
// /*AFLA*/  *
// /*AFLA*/  *  The one bits that does not fit with the default layout is the PAGE_PSE
// /*AFLA*/  *  also called PAGE_PAT). The MMUEXT_[UN]MARK_SUPER arguments to the
// /*AFLA*/  *  HYPERVISOR_mmuext_op serve as mechanism to set a pagetable to be 4MB
// /*AFLA*/  *  (or 2MB) instead of using the PAGE_PSE bit.
// /*AFLA*/  *
// /*AFLA*/  *  The reason that the PAGE_PSE (bit 7) is not being utilized is due to Xen
// /*AFLA*/  *  using it as the Page Attribute Table (PAT) bit - for details on it please
// /*AFLA*/  *  refer to Intel SDM 10.12. The PAT allows to set the caching attributes of
// /*AFLA*/  *  pages instead of using MTRRs.
// /*AFLA*/  *
// /*AFLA*/  *  The PAT MSR is as follows (it is a 64-bit value, each entry is 8 bits):
// /*AFLA*/  *                    PAT4                 PAT0
// /*AFLA*/  *  +-----+-----+----+----+----+-----+----+----+
// /*AFLA*/  *  | UC  | UC- | WC | WB | UC | UC- | WC | WB |  <= Linux
// /*AFLA*/  *  +-----+-----+----+----+----+-----+----+----+
// /*AFLA*/  *  | UC  | UC- | WT | WB | UC | UC- | WT | WB |  <= BIOS (default when machine boots)
// /*AFLA*/  *  +-----+-----+----+----+----+-----+----+----+
// /*AFLA*/  *  | rsv | rsv | WP | WC | UC | UC- | WT | WB |  <= Xen
// /*AFLA*/  *  +-----+-----+----+----+----+-----+----+----+
// /*AFLA*/  *
// /*AFLA*/  *  The lookup of this index table translates to looking up
// /*AFLA*/  *  Bit 7, Bit 4, and Bit 3 of val entry:
// /*AFLA*/  *
// /*AFLA*/  *  PAT/PSE (bit 7) ... PCD (bit 4) .. PWT (bit 3).
// /*AFLA*/  *
// /*AFLA*/  *  If all bits are off, then we are using PAT0. If bit 3 turned on,
// /*AFLA*/  *  then we are using PAT1, if bit 3 and bit 4, then PAT2..
// /*AFLA*/  *
// /*AFLA*/  *  As you can see, the Linux PAT1 translates to PAT4 under Xen. Which means
// /*AFLA*/  *  that if a guest that follows Linux's PAT setup and would like to set Write
// /*AFLA*/  *  Combined on pages it MUST use PAT4 entry. Meaning that Bit 7 (PAGE_PAT) is
// /*AFLA*/  *  set. For example, under Linux it only uses PAT0, PAT1, and PAT2 for the
// /*AFLA*/  *  caching as:
// /*AFLA*/  *
// /*AFLA*/  *   WB = none (so PAT0)
// /*AFLA*/  *   WC = PWT (bit 3 on)
// /*AFLA*/  *   UC = PWT | PCD (bit 3 and 4 are on).
// /*AFLA*/  *
// /*AFLA*/  * To make it work with Xen, it needs to translate the WC bit as so:
// /*AFLA*/  *
// /*AFLA*/  *  PWT (so bit 3 on) --> PAT (so bit 7 is on) and clear bit 3
// /*AFLA*/  *
// /*AFLA*/  * And to translate back it would:
// /*AFLA*/  *
// /*AFLA*/  * PAT (bit 7 on) --> PWT (bit 3 on) and clear bit 7.
// /*AFLA*/  */
// /*AFLA*/ #define MMU_NORMAL_PT_UPDATE      0 /* checked '*ptr = val'. ptr is MA.       */
// /*AFLA*/ #define MMU_MACHPHYS_UPDATE       1 /* ptr = MA of frame to modify entry for  */
// /*AFLA*/ #define MMU_PT_UPDATE_PRESERVE_AD 2 /* atomically: *ptr = val | (*ptr&(A|D)) */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * MMU EXTENDED OPERATIONS
// /*AFLA*/  *
// /*AFLA*/  * enum neg_errnoval HYPERVISOR_mmuext_op(mmuext_op_t uops[],
// /*AFLA*/  *                                        unsigned int count,
// /*AFLA*/  *                                        unsigned int *pdone,
// /*AFLA*/  *                                        unsigned int foreigndom)
// /*AFLA*/  */
// /*AFLA*/ /* HYPERVISOR_mmuext_op() accepts a list of mmuext_op structures.
// /*AFLA*/  * A foreigndom (FD) can be specified (or DOMID_SELF for none).
// /*AFLA*/  * Where the FD has some effect, it is described below.
// /*AFLA*/  *
// /*AFLA*/  * cmd: MMUEXT_(UN)PIN_*_TABLE
// /*AFLA*/  * mfn: Machine frame number to be (un)pinned as a p.t. page.
// /*AFLA*/  *      The frame must belong to the FD, if one is specified.
// /*AFLA*/  *
// /*AFLA*/  * cmd: MMUEXT_NEW_BASEPTR
// /*AFLA*/  * mfn: Machine frame number of new page-table base to install in MMU.
// /*AFLA*/  *
// /*AFLA*/  * cmd: MMUEXT_NEW_USER_BASEPTR [x86/64 only]
// /*AFLA*/  * mfn: Machine frame number of new page-table base to install in MMU
// /*AFLA*/  *      when in user space.
// /*AFLA*/  *
// /*AFLA*/  * cmd: MMUEXT_TLB_FLUSH_LOCAL
// /*AFLA*/  * No additional arguments. Flushes local TLB.
// /*AFLA*/  *
// /*AFLA*/  * cmd: MMUEXT_INVLPG_LOCAL
// /*AFLA*/  * linear_addr: Linear address to be flushed from the local TLB.
// /*AFLA*/  *
// /*AFLA*/  * cmd: MMUEXT_TLB_FLUSH_MULTI
// /*AFLA*/  * vcpumask: Pointer to bitmap of VCPUs to be flushed.
// /*AFLA*/  *
// /*AFLA*/  * cmd: MMUEXT_INVLPG_MULTI
// /*AFLA*/  * linear_addr: Linear address to be flushed.
// /*AFLA*/  * vcpumask: Pointer to bitmap of VCPUs to be flushed.
// /*AFLA*/  *
// /*AFLA*/  * cmd: MMUEXT_TLB_FLUSH_ALL
// /*AFLA*/  * No additional arguments. Flushes all VCPUs' TLBs.
// /*AFLA*/  *
// /*AFLA*/  * cmd: MMUEXT_INVLPG_ALL
// /*AFLA*/  * linear_addr: Linear address to be flushed from all VCPUs' TLBs.
// /*AFLA*/  *
// /*AFLA*/  * cmd: MMUEXT_FLUSH_CACHE
// /*AFLA*/  * No additional arguments. Writes back and flushes cache contents.
// /*AFLA*/  *
// /*AFLA*/  * cmd: MMUEXT_FLUSH_CACHE_GLOBAL
// /*AFLA*/  * No additional arguments. Writes back and flushes cache contents
// /*AFLA*/  * on all CPUs in the system.
// /*AFLA*/  *
// /*AFLA*/  * cmd: MMUEXT_SET_LDT
// /*AFLA*/  * linear_addr: Linear address of LDT base (NB. must be page-aligned).
// /*AFLA*/  * nr_ents: Number of entries in LDT.
// /*AFLA*/  *
// /*AFLA*/  * cmd: MMUEXT_CLEAR_PAGE
// /*AFLA*/  * mfn: Machine frame number to be cleared.
// /*AFLA*/  *
// /*AFLA*/  * cmd: MMUEXT_COPY_PAGE
// /*AFLA*/  * mfn: Machine frame number of the destination page.
// /*AFLA*/  * src_mfn: Machine frame number of the source page.
// /*AFLA*/  *
// /*AFLA*/  * cmd: MMUEXT_[UN]MARK_SUPER
// /*AFLA*/  * mfn: Machine frame number of head of superpage to be [un]marked.
// /*AFLA*/  */
// /*AFLA*/ #define MMUEXT_PIN_L1_TABLE      0
// /*AFLA*/ #define MMUEXT_PIN_L2_TABLE      1
// /*AFLA*/ #define MMUEXT_PIN_L3_TABLE      2
// /*AFLA*/ #define MMUEXT_PIN_L4_TABLE      3
// /*AFLA*/ #define MMUEXT_UNPIN_TABLE       4
// /*AFLA*/ #define MMUEXT_NEW_BASEPTR       5
// /*AFLA*/ #define MMUEXT_TLB_FLUSH_LOCAL   6
// /*AFLA*/ #define MMUEXT_INVLPG_LOCAL      7
// /*AFLA*/ #define MMUEXT_TLB_FLUSH_MULTI   8
// /*AFLA*/ #define MMUEXT_INVLPG_MULTI      9
// /*AFLA*/ #define MMUEXT_TLB_FLUSH_ALL    10
// /*AFLA*/ #define MMUEXT_INVLPG_ALL       11
// /*AFLA*/ #define MMUEXT_FLUSH_CACHE      12
// /*AFLA*/ #define MMUEXT_SET_LDT          13
// /*AFLA*/ #define MMUEXT_NEW_USER_BASEPTR 15
// /*AFLA*/ #define MMUEXT_CLEAR_PAGE       16
// /*AFLA*/ #define MMUEXT_COPY_PAGE        17
// /*AFLA*/ #define MMUEXT_FLUSH_CACHE_GLOBAL 18
// /*AFLA*/ #define MMUEXT_MARK_SUPER       19
// /*AFLA*/ #define MMUEXT_UNMARK_SUPER     20
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ struct mmuext_op {
// /*AFLA*/ 	unsigned int cmd;
// /*AFLA*/ 	union {
// /*AFLA*/ 		/* [UN]PIN_TABLE, NEW_BASEPTR, NEW_USER_BASEPTR
// /*AFLA*/ 		 * CLEAR_PAGE, COPY_PAGE, [UN]MARK_SUPER */
// /*AFLA*/ 		xen_pfn_t mfn;
// /*AFLA*/ 		/* INVLPG_LOCAL, INVLPG_ALL, SET_LDT */
// /*AFLA*/ 		unsigned long linear_addr;
// /*AFLA*/ 	} arg1;
// /*AFLA*/ 	union {
// /*AFLA*/ 		/* SET_LDT */
// /*AFLA*/ 		unsigned int nr_ents;
// /*AFLA*/ 		/* TLB_FLUSH_MULTI, INVLPG_MULTI */
// /*AFLA*/ 		void *vcpumask;
// /*AFLA*/ 		/* COPY_PAGE */
// /*AFLA*/ 		xen_pfn_t src_mfn;
// /*AFLA*/ 	} arg2;
// /*AFLA*/ };
// /*AFLA*/ DEFINE_GUEST_HANDLE_STRUCT(mmuext_op);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* These are passed as 'flags' to update_va_mapping. They can be ORed. */
// /*AFLA*/ /* When specifying UVMF_MULTI, also OR in a pointer to a CPU bitmap.   */
// /*AFLA*/ /* UVMF_LOCAL is merely UVMF_MULTI with a NULL bitmap pointer.         */
// /*AFLA*/ #define UVMF_NONE               (0UL<<0) /* No flushing at all.   */
// /*AFLA*/ #define UVMF_TLB_FLUSH          (1UL<<0) /* Flush entire TLB(s).  */
// /*AFLA*/ #define UVMF_INVLPG             (2UL<<0) /* Flush only one entry. */
// /*AFLA*/ #define UVMF_FLUSHTYPE_MASK     (3UL<<0)
// /*AFLA*/ #define UVMF_MULTI              (0UL<<2) /* Flush subset of TLBs. */
// /*AFLA*/ #define UVMF_LOCAL              (0UL<<2) /* Flush local TLB.      */
// /*AFLA*/ #define UVMF_ALL                (1UL<<2) /* Flush all TLBs.       */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Commands to HYPERVISOR_console_io().
// /*AFLA*/  */
// /*AFLA*/ #define CONSOLEIO_write         0
// /*AFLA*/ #define CONSOLEIO_read          1
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Commands to HYPERVISOR_vm_assist().
// /*AFLA*/  */
// /*AFLA*/ #define VMASST_CMD_enable                0
// /*AFLA*/ #define VMASST_CMD_disable               1
// /*AFLA*/ 
// /*AFLA*/ /* x86/32 guests: simulate full 4GB segment limits. */
// /*AFLA*/ #define VMASST_TYPE_4gb_segments         0
// /*AFLA*/ 
// /*AFLA*/ /* x86/32 guests: trap (vector 15) whenever above vmassist is used. */
// /*AFLA*/ #define VMASST_TYPE_4gb_segments_notify  1
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * x86 guests: support writes to bottom-level PTEs.
// /*AFLA*/  * NB1. Page-directory entries cannot be written.
// /*AFLA*/  * NB2. Guest must continue to remove all writable mappings of PTEs.
// /*AFLA*/  */
// /*AFLA*/ #define VMASST_TYPE_writable_pagetables  2
// /*AFLA*/ 
// /*AFLA*/ /* x86/PAE guests: support PDPTs above 4GB. */
// /*AFLA*/ #define VMASST_TYPE_pae_extended_cr3     3
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * x86 guests: Sane behaviour for virtual iopl
// /*AFLA*/  *  - virtual iopl updated from do_iret() hypercalls.
// /*AFLA*/  *  - virtual iopl reported in bounce frames.
// /*AFLA*/  *  - guest kernels assumed to be level 0 for the purpose of iopl checks.
// /*AFLA*/  */
// /*AFLA*/ #define VMASST_TYPE_architectural_iopl   4
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * All guests: activate update indicator in vcpu_runstate_info
// /*AFLA*/  * Enable setting the XEN_RUNSTATE_UPDATE flag in guest memory mapped
// /*AFLA*/  * vcpu_runstate_info during updates of the runstate information.
// /*AFLA*/  */
// /*AFLA*/ #define VMASST_TYPE_runstate_update_flag 5
// /*AFLA*/ 
// /*AFLA*/ #define MAX_VMASST_TYPE 5
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ typedef uint16_t domid_t;
// /*AFLA*/ 
// /*AFLA*/ /* Domain ids >= DOMID_FIRST_RESERVED cannot be used for ordinary domains. */
// /*AFLA*/ #define DOMID_FIRST_RESERVED (0x7FF0U)
// /*AFLA*/ 
// /*AFLA*/ /* DOMID_SELF is used in certain contexts to refer to oneself. */
// /*AFLA*/ #define DOMID_SELF (0x7FF0U)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * DOMID_IO is used to restrict page-table updates to mapping I/O memory.
// /*AFLA*/  * Although no Foreign Domain need be specified to map I/O pages, DOMID_IO
// /*AFLA*/  * is useful to ensure that no mappings to the OS's own heap are accidentally
// /*AFLA*/  * installed. (e.g., in Linux this could cause havoc as reference counts
// /*AFLA*/  * aren't adjusted on the I/O-mapping code path).
// /*AFLA*/  * This only makes sense in MMUEXT_SET_FOREIGNDOM, but in that context can
// /*AFLA*/  * be specified by any calling domain.
// /*AFLA*/  */
// /*AFLA*/ #define DOMID_IO   (0x7FF1U)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * DOMID_XEN is used to allow privileged domains to map restricted parts of
// /*AFLA*/  * Xen's heap space (e.g., the machine_to_phys table).
// /*AFLA*/  * This only makes sense in MMUEXT_SET_FOREIGNDOM, and is only permitted if
// /*AFLA*/  * the caller is privileged.
// /*AFLA*/  */
// /*AFLA*/ #define DOMID_XEN  (0x7FF2U)
// /*AFLA*/ 
// /*AFLA*/ /* DOMID_COW is used as the owner of sharable pages */
// /*AFLA*/ #define DOMID_COW  (0x7FF3U)
// /*AFLA*/ 
// /*AFLA*/ /* DOMID_INVALID is used to identify pages with unknown owner. */
// /*AFLA*/ #define DOMID_INVALID (0x7FF4U)
// /*AFLA*/ 
// /*AFLA*/ /* Idle domain. */
// /*AFLA*/ #define DOMID_IDLE (0x7FFFU)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Send an array of these to HYPERVISOR_mmu_update().
// /*AFLA*/  * NB. The fields are natural pointer/address size for this architecture.
// /*AFLA*/  */
// /*AFLA*/ struct mmu_update {
// /*AFLA*/     uint64_t ptr;       /* Machine address of PTE. */
// /*AFLA*/     uint64_t val;       /* New contents of PTE.    */
// /*AFLA*/ };
// /*AFLA*/ DEFINE_GUEST_HANDLE_STRUCT(mmu_update);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Send an array of these to HYPERVISOR_multicall().
// /*AFLA*/  * NB. The fields are logically the natural register size for this
// /*AFLA*/  * architecture. In cases where xen_ulong_t is larger than this then
// /*AFLA*/  * any unused bits in the upper portion must be zero.
// /*AFLA*/  */
// /*AFLA*/ struct multicall_entry {
// /*AFLA*/     xen_ulong_t op;
// /*AFLA*/     xen_long_t result;
// /*AFLA*/     xen_ulong_t args[6];
// /*AFLA*/ };
// /*AFLA*/ DEFINE_GUEST_HANDLE_STRUCT(multicall_entry);
// /*AFLA*/ 
// /*AFLA*/ struct vcpu_time_info {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Updates to the following values are preceded and followed
// /*AFLA*/ 	 * by an increment of 'version'. The guest can therefore
// /*AFLA*/ 	 * detect updates by looking for changes to 'version'. If the
// /*AFLA*/ 	 * least-significant bit of the version number is set then an
// /*AFLA*/ 	 * update is in progress and the guest must wait to read a
// /*AFLA*/ 	 * consistent set of values.  The correct way to interact with
// /*AFLA*/ 	 * the version number is similar to Linux's seqlock: see the
// /*AFLA*/ 	 * implementations of read_seqbegin/read_seqretry.
// /*AFLA*/ 	 */
// /*AFLA*/ 	uint32_t version;
// /*AFLA*/ 	uint32_t pad0;
// /*AFLA*/ 	uint64_t tsc_timestamp;   /* TSC at last update of time vals.  */
// /*AFLA*/ 	uint64_t system_time;     /* Time, in nanosecs, since boot.    */
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Current system time:
// /*AFLA*/ 	 *   system_time + ((tsc - tsc_timestamp) << tsc_shift) * tsc_to_system_mul
// /*AFLA*/ 	 * CPU frequency (Hz):
// /*AFLA*/ 	 *   ((10^9 << 32) / tsc_to_system_mul) >> tsc_shift
// /*AFLA*/ 	 */
// /*AFLA*/ 	uint32_t tsc_to_system_mul;
// /*AFLA*/ 	int8_t   tsc_shift;
// /*AFLA*/ 	int8_t   pad1[3];
// /*AFLA*/ }; /* 32 bytes */
// /*AFLA*/ 
// /*AFLA*/ struct vcpu_info {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * 'evtchn_upcall_pending' is written non-zero by Xen to indicate
// /*AFLA*/ 	 * a pending notification for a particular VCPU. It is then cleared
// /*AFLA*/ 	 * by the guest OS /before/ checking for pending work, thus avoiding
// /*AFLA*/ 	 * a set-and-check race. Note that the mask is only accessed by Xen
// /*AFLA*/ 	 * on the CPU that is currently hosting the VCPU. This means that the
// /*AFLA*/ 	 * pending and mask flags can be updated by the guest without special
// /*AFLA*/ 	 * synchronisation (i.e., no need for the x86 LOCK prefix).
// /*AFLA*/ 	 * This may seem suboptimal because if the pending flag is set by
// /*AFLA*/ 	 * a different CPU then an IPI may be scheduled even when the mask
// /*AFLA*/ 	 * is set. However, note:
// /*AFLA*/ 	 *  1. The task of 'interrupt holdoff' is covered by the per-event-
// /*AFLA*/ 	 *     channel mask bits. A 'noisy' event that is continually being
// /*AFLA*/ 	 *     triggered can be masked at source at this very precise
// /*AFLA*/ 	 *     granularity.
// /*AFLA*/ 	 *  2. The main purpose of the per-VCPU mask is therefore to restrict
// /*AFLA*/ 	 *     reentrant execution: whether for concurrency control, or to
// /*AFLA*/ 	 *     prevent unbounded stack usage. Whatever the purpose, we expect
// /*AFLA*/ 	 *     that the mask will be asserted only for short periods at a time,
// /*AFLA*/ 	 *     and so the likelihood of a 'spurious' IPI is suitably small.
// /*AFLA*/ 	 * The mask is read before making an event upcall to the guest: a
// /*AFLA*/ 	 * non-zero mask therefore guarantees that the VCPU will not receive
// /*AFLA*/ 	 * an upcall activation. The mask is cleared when the VCPU requests
// /*AFLA*/ 	 * to block: this avoids wakeup-waiting races.
// /*AFLA*/ 	 */
// /*AFLA*/ 	uint8_t evtchn_upcall_pending;
// /*AFLA*/ 	uint8_t evtchn_upcall_mask;
// /*AFLA*/ 	xen_ulong_t evtchn_pending_sel;
// /*AFLA*/ 	struct arch_vcpu_info arch;
// /*AFLA*/ 	struct pvclock_vcpu_time_info time;
// /*AFLA*/ }; /* 64 bytes (x86) */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Xen/kernel shared data -- pointer provided in start_info.
// /*AFLA*/  * NB. We expect that this struct is smaller than a page.
// /*AFLA*/  */
// /*AFLA*/ struct shared_info {
// /*AFLA*/ 	struct vcpu_info vcpu_info[MAX_VIRT_CPUS];
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * A domain can create "event channels" on which it can send and receive
// /*AFLA*/ 	 * asynchronous event notifications. There are three classes of event that
// /*AFLA*/ 	 * are delivered by this mechanism:
// /*AFLA*/ 	 *  1. Bi-directional inter- and intra-domain connections. Domains must
// /*AFLA*/ 	 *     arrange out-of-band to set up a connection (usually by allocating
// /*AFLA*/ 	 *     an unbound 'listener' port and avertising that via a storage service
// /*AFLA*/ 	 *     such as xenstore).
// /*AFLA*/ 	 *  2. Physical interrupts. A domain with suitable hardware-access
// /*AFLA*/ 	 *     privileges can bind an event-channel port to a physical interrupt
// /*AFLA*/ 	 *     source.
// /*AFLA*/ 	 *  3. Virtual interrupts ('events'). A domain can bind an event-channel
// /*AFLA*/ 	 *     port to a virtual interrupt source, such as the virtual-timer
// /*AFLA*/ 	 *     device or the emergency console.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Event channels are addressed by a "port index". Each channel is
// /*AFLA*/ 	 * associated with two bits of information:
// /*AFLA*/ 	 *  1. PENDING -- notifies the domain that there is a pending notification
// /*AFLA*/ 	 *     to be processed. This bit is cleared by the guest.
// /*AFLA*/ 	 *  2. MASK -- if this bit is clear then a 0->1 transition of PENDING
// /*AFLA*/ 	 *     will cause an asynchronous upcall to be scheduled. This bit is only
// /*AFLA*/ 	 *     updated by the guest. It is read-only within Xen. If a channel
// /*AFLA*/ 	 *     becomes pending while the channel is masked then the 'edge' is lost
// /*AFLA*/ 	 *     (i.e., when the channel is unmasked, the guest must manually handle
// /*AFLA*/ 	 *     pending notifications as no upcall will be scheduled by Xen).
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * To expedite scanning of pending notifications, any 0->1 pending
// /*AFLA*/ 	 * transition on an unmasked channel causes a corresponding bit in a
// /*AFLA*/ 	 * per-vcpu selector word to be set. Each bit in the selector covers a
// /*AFLA*/ 	 * 'C long' in the PENDING bitfield array.
// /*AFLA*/ 	 */
// /*AFLA*/ 	xen_ulong_t evtchn_pending[sizeof(xen_ulong_t) * 8];
// /*AFLA*/ 	xen_ulong_t evtchn_mask[sizeof(xen_ulong_t) * 8];
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Wallclock time: updated only by control software. Guests should base
// /*AFLA*/ 	 * their gettimeofday() syscall on this wallclock-base value.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct pvclock_wall_clock wc;
// /*AFLA*/ 
// /*AFLA*/ 	struct arch_shared_info arch;
// /*AFLA*/ 
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Start-of-day memory layout
// /*AFLA*/  *
// /*AFLA*/  *  1. The domain is started within contiguous virtual-memory region.
// /*AFLA*/  *  2. The contiguous region begins and ends on an aligned 4MB boundary.
// /*AFLA*/  *  3. This the order of bootstrap elements in the initial virtual region:
// /*AFLA*/  *      a. relocated kernel image
// /*AFLA*/  *      b. initial ram disk              [mod_start, mod_len]
// /*AFLA*/  *         (may be omitted)
// /*AFLA*/  *      c. list of allocated page frames [mfn_list, nr_pages]
// /*AFLA*/  *         (unless relocated due to XEN_ELFNOTE_INIT_P2M)
// /*AFLA*/  *      d. start_info_t structure        [register ESI (x86)]
// /*AFLA*/  *         in case of dom0 this page contains the console info, too
// /*AFLA*/  *      e. unless dom0: xenstore ring page
// /*AFLA*/  *      f. unless dom0: console ring page
// /*AFLA*/  *      g. bootstrap page tables         [pt_base, CR3 (x86)]
// /*AFLA*/  *      h. bootstrap stack               [register ESP (x86)]
// /*AFLA*/  *  4. Bootstrap elements are packed together, but each is 4kB-aligned.
// /*AFLA*/  *  5. The list of page frames forms a contiguous 'pseudo-physical' memory
// /*AFLA*/  *     layout for the domain. In particular, the bootstrap virtual-memory
// /*AFLA*/  *     region is a 1:1 mapping to the first section of the pseudo-physical map.
// /*AFLA*/  *  6. All bootstrap elements are mapped read-writable for the guest OS. The
// /*AFLA*/  *     only exception is the bootstrap page table, which is mapped read-only.
// /*AFLA*/  *  7. There is guaranteed to be at least 512kB padding after the final
// /*AFLA*/  *     bootstrap element. If necessary, the bootstrap virtual region is
// /*AFLA*/  *     extended by an extra 4MB to ensure this.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define MAX_GUEST_CMDLINE 1024
// /*AFLA*/ struct start_info {
// /*AFLA*/ 	/* THE FOLLOWING ARE FILLED IN BOTH ON INITIAL BOOT AND ON RESUME.    */
// /*AFLA*/ 	char magic[32];             /* "xen-<version>-<platform>".            */
// /*AFLA*/ 	unsigned long nr_pages;     /* Total pages allocated to this domain.  */
// /*AFLA*/ 	unsigned long shared_info;  /* MACHINE address of shared info struct. */
// /*AFLA*/ 	uint32_t flags;             /* SIF_xxx flags.                         */
// /*AFLA*/ 	xen_pfn_t store_mfn;        /* MACHINE page number of shared page.    */
// /*AFLA*/ 	uint32_t store_evtchn;      /* Event channel for store communication. */
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct {
// /*AFLA*/ 			xen_pfn_t mfn;      /* MACHINE page number of console page.   */
// /*AFLA*/ 			uint32_t  evtchn;   /* Event channel for console page.        */
// /*AFLA*/ 		} domU;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			uint32_t info_off;  /* Offset of console_info struct.         */
// /*AFLA*/ 			uint32_t info_size; /* Size of console_info struct from start.*/
// /*AFLA*/ 		} dom0;
// /*AFLA*/ 	} console;
// /*AFLA*/ 	/* THE FOLLOWING ARE ONLY FILLED IN ON INITIAL BOOT (NOT RESUME).     */
// /*AFLA*/ 	unsigned long pt_base;      /* VIRTUAL address of page directory.     */
// /*AFLA*/ 	unsigned long nr_pt_frames; /* Number of bootstrap p.t. frames.       */
// /*AFLA*/ 	unsigned long mfn_list;     /* VIRTUAL address of page-frame list.    */
// /*AFLA*/ 	unsigned long mod_start;    /* VIRTUAL address of pre-loaded module.  */
// /*AFLA*/ 	unsigned long mod_len;      /* Size (bytes) of pre-loaded module.     */
// /*AFLA*/ 	int8_t cmd_line[MAX_GUEST_CMDLINE];
// /*AFLA*/ 	/* The pfn range here covers both page table and p->m table frames.   */
// /*AFLA*/ 	unsigned long first_p2m_pfn;/* 1st pfn forming initial P->M table.    */
// /*AFLA*/ 	unsigned long nr_p2m_frames;/* # of pfns forming initial P->M table.  */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* These flags are passed in the 'flags' field of start_info_t. */
// /*AFLA*/ #define SIF_PRIVILEGED      (1<<0)  /* Is the domain privileged? */
// /*AFLA*/ #define SIF_INITDOMAIN      (1<<1)  /* Is this the initial control domain? */
// /*AFLA*/ #define SIF_MULTIBOOT_MOD   (1<<2)  /* Is mod_start a multiboot module? */
// /*AFLA*/ #define SIF_MOD_START_PFN   (1<<3)  /* Is mod_start a PFN? */
// /*AFLA*/ #define SIF_VIRT_P2M_4TOOLS (1<<4)  /* Do Xen tools understand a virt. mapped */
// /*AFLA*/ 				    /* P->M making the 3 level tree obsolete? */
// /*AFLA*/ #define SIF_PM_MASK       (0xFF<<8) /* reserve 1 byte for xen-pm options */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A multiboot module is a package containing modules very similar to a
// /*AFLA*/  * multiboot module array. The only differences are:
// /*AFLA*/  * - the array of module descriptors is by convention simply at the beginning
// /*AFLA*/  *   of the multiboot module,
// /*AFLA*/  * - addresses in the module descriptors are based on the beginning of the
// /*AFLA*/  *   multiboot module,
// /*AFLA*/  * - the number of modules is determined by a termination descriptor that has
// /*AFLA*/  *   mod_start == 0.
// /*AFLA*/  *
// /*AFLA*/  * This permits to both build it statically and reference it in a configuration
// /*AFLA*/  * file, and let the PV guest easily rebase the addresses to virtual addresses
// /*AFLA*/  * and at the same time count the number of modules.
// /*AFLA*/  */
// /*AFLA*/ struct xen_multiboot_mod_list {
// /*AFLA*/ 	/* Address of first byte of the module */
// /*AFLA*/ 	uint32_t mod_start;
// /*AFLA*/ 	/* Address of last byte of the module (inclusive) */
// /*AFLA*/ 	uint32_t mod_end;
// /*AFLA*/ 	/* Address of zero-terminated command line */
// /*AFLA*/ 	uint32_t cmdline;
// /*AFLA*/ 	/* Unused, must be zero */
// /*AFLA*/ 	uint32_t pad;
// /*AFLA*/ };
// /*AFLA*/ /*
// /*AFLA*/  * The console structure in start_info.console.dom0
// /*AFLA*/  *
// /*AFLA*/  * This structure includes a variety of information required to
// /*AFLA*/  * have a working VGA/VESA console.
// /*AFLA*/  */
// /*AFLA*/ struct dom0_vga_console_info {
// /*AFLA*/ 	uint8_t video_type;
// /*AFLA*/ #define XEN_VGATYPE_TEXT_MODE_3 0x03
// /*AFLA*/ #define XEN_VGATYPE_VESA_LFB    0x23
// /*AFLA*/ #define XEN_VGATYPE_EFI_LFB     0x70
// /*AFLA*/ 
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct {
// /*AFLA*/ 			/* Font height, in pixels. */
// /*AFLA*/ 			uint16_t font_height;
// /*AFLA*/ 			/* Cursor location (column, row). */
// /*AFLA*/ 			uint16_t cursor_x, cursor_y;
// /*AFLA*/ 			/* Number of rows and columns (dimensions in characters). */
// /*AFLA*/ 			uint16_t rows, columns;
// /*AFLA*/ 		} text_mode_3;
// /*AFLA*/ 
// /*AFLA*/ 		struct {
// /*AFLA*/ 			/* Width and height, in pixels. */
// /*AFLA*/ 			uint16_t width, height;
// /*AFLA*/ 			/* Bytes per scan line. */
// /*AFLA*/ 			uint16_t bytes_per_line;
// /*AFLA*/ 			/* Bits per pixel. */
// /*AFLA*/ 			uint16_t bits_per_pixel;
// /*AFLA*/ 			/* LFB physical address, and size (in units of 64kB). */
// /*AFLA*/ 			uint32_t lfb_base;
// /*AFLA*/ 			uint32_t lfb_size;
// /*AFLA*/ 			/* RGB mask offsets and sizes, as defined by VBE 1.2+ */
// /*AFLA*/ 			uint8_t  red_pos, red_size;
// /*AFLA*/ 			uint8_t  green_pos, green_size;
// /*AFLA*/ 			uint8_t  blue_pos, blue_size;
// /*AFLA*/ 			uint8_t  rsvd_pos, rsvd_size;
// /*AFLA*/ 
// /*AFLA*/ 			/* VESA capabilities (offset 0xa, VESA command 0x4f00). */
// /*AFLA*/ 			uint32_t gbl_caps;
// /*AFLA*/ 			/* Mode attributes (offset 0x0, VESA command 0x4f01). */
// /*AFLA*/ 			uint16_t mode_attrs;
// /*AFLA*/ 		} vesa_lfb;
// /*AFLA*/ 	} u;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ typedef uint64_t cpumap_t;
// /*AFLA*/ 
// /*AFLA*/ typedef uint8_t xen_domain_handle_t[16];
// /*AFLA*/ 
// /*AFLA*/ /* Turn a plain number into a C unsigned long constant. */
// /*AFLA*/ #define __mk_unsigned_long(x) x ## UL
// /*AFLA*/ #define mk_unsigned_long(x) __mk_unsigned_long(x)
// /*AFLA*/ 
// /*AFLA*/ #define TMEM_SPEC_VERSION 1
// /*AFLA*/ 
// /*AFLA*/ struct tmem_op {
// /*AFLA*/ 	uint32_t cmd;
// /*AFLA*/ 	int32_t pool_id;
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct {  /* for cmd == TMEM_NEW_POOL */
// /*AFLA*/ 			uint64_t uuid[2];
// /*AFLA*/ 			uint32_t flags;
// /*AFLA*/ 		} new;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			uint64_t oid[3];
// /*AFLA*/ 			uint32_t index;
// /*AFLA*/ 			uint32_t tmem_offset;
// /*AFLA*/ 			uint32_t pfn_offset;
// /*AFLA*/ 			uint32_t len;
// /*AFLA*/ 			GUEST_HANDLE(void) gmfn; /* guest machine page frame */
// /*AFLA*/ 		} gen;
// /*AFLA*/ 	} u;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ DEFINE_GUEST_HANDLE(u64);
// /*AFLA*/ 
// /*AFLA*/ #else /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ /* In assembly code we cannot use C numeric constant suffixes. */
// /*AFLA*/ #define mk_unsigned_long(x) x
// /*AFLA*/ 
// /*AFLA*/ #endif /* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* __XEN_PUBLIC_XEN_H__ */
