// /*AFLA*/ #ifndef _ASM_X86_XEN_INTERFACE_64_H
// /*AFLA*/ #define _ASM_X86_XEN_INTERFACE_64_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * 64-bit segment selectors
// /*AFLA*/  * These flat segments are in the Xen-private section of every GDT. Since these
// /*AFLA*/  * are also present in the initial GDT, many OSes will be able to avoid
// /*AFLA*/  * installing their own GDT.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define FLAT_RING3_CS32 0xe023  /* GDT index 260 */
// /*AFLA*/ #define FLAT_RING3_CS64 0xe033  /* GDT index 261 */
// /*AFLA*/ #define FLAT_RING3_DS32 0xe02b  /* GDT index 262 */
// /*AFLA*/ #define FLAT_RING3_DS64 0x0000  /* NULL selector */
// /*AFLA*/ #define FLAT_RING3_SS32 0xe02b  /* GDT index 262 */
// /*AFLA*/ #define FLAT_RING3_SS64 0xe02b  /* GDT index 262 */
// /*AFLA*/ 
// /*AFLA*/ #define FLAT_KERNEL_DS64 FLAT_RING3_DS64
// /*AFLA*/ #define FLAT_KERNEL_DS32 FLAT_RING3_DS32
// /*AFLA*/ #define FLAT_KERNEL_DS   FLAT_KERNEL_DS64
// /*AFLA*/ #define FLAT_KERNEL_CS64 FLAT_RING3_CS64
// /*AFLA*/ #define FLAT_KERNEL_CS32 FLAT_RING3_CS32
// /*AFLA*/ #define FLAT_KERNEL_CS   FLAT_KERNEL_CS64
// /*AFLA*/ #define FLAT_KERNEL_SS64 FLAT_RING3_SS64
// /*AFLA*/ #define FLAT_KERNEL_SS32 FLAT_RING3_SS32
// /*AFLA*/ #define FLAT_KERNEL_SS   FLAT_KERNEL_SS64
// /*AFLA*/ 
// /*AFLA*/ #define FLAT_USER_DS64 FLAT_RING3_DS64
// /*AFLA*/ #define FLAT_USER_DS32 FLAT_RING3_DS32
// /*AFLA*/ #define FLAT_USER_DS   FLAT_USER_DS64
// /*AFLA*/ #define FLAT_USER_CS64 FLAT_RING3_CS64
// /*AFLA*/ #define FLAT_USER_CS32 FLAT_RING3_CS32
// /*AFLA*/ #define FLAT_USER_CS   FLAT_USER_CS64
// /*AFLA*/ #define FLAT_USER_SS64 FLAT_RING3_SS64
// /*AFLA*/ #define FLAT_USER_SS32 FLAT_RING3_SS32
// /*AFLA*/ #define FLAT_USER_SS   FLAT_USER_SS64
// /*AFLA*/ 
// /*AFLA*/ #define __HYPERVISOR_VIRT_START 0xFFFF800000000000
// /*AFLA*/ #define __HYPERVISOR_VIRT_END   0xFFFF880000000000
// /*AFLA*/ #define __MACH2PHYS_VIRT_START  0xFFFF800000000000
// /*AFLA*/ #define __MACH2PHYS_VIRT_END    0xFFFF804000000000
// /*AFLA*/ #define __MACH2PHYS_SHIFT       3
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * int HYPERVISOR_set_segment_base(unsigned int which, unsigned long base)
// /*AFLA*/  *  @which == SEGBASE_*  ;  @base == 64-bit base address
// /*AFLA*/  * Returns 0 on success.
// /*AFLA*/  */
// /*AFLA*/ #define SEGBASE_FS          0
// /*AFLA*/ #define SEGBASE_GS_USER     1
// /*AFLA*/ #define SEGBASE_GS_KERNEL   2
// /*AFLA*/ #define SEGBASE_GS_USER_SEL 3 /* Set user %gs specified in base[15:0] */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * int HYPERVISOR_iret(void)
// /*AFLA*/  * All arguments are on the kernel stack, in the following format.
// /*AFLA*/  * Never returns if successful. Current kernel context is lost.
// /*AFLA*/  * The saved CS is mapped as follows:
// /*AFLA*/  *   RING0 -> RING3 kernel mode.
// /*AFLA*/  *   RING1 -> RING3 kernel mode.
// /*AFLA*/  *   RING2 -> RING3 kernel mode.
// /*AFLA*/  *   RING3 -> RING3 user mode.
// /*AFLA*/  * However RING0 indicates that the guest kernel should return to iteself
// /*AFLA*/  * directly with
// /*AFLA*/  *      orb   $3,1*8(%rsp)
// /*AFLA*/  *      iretq
// /*AFLA*/  * If flags contains VGCF_in_syscall:
// /*AFLA*/  *   Restore RAX, RIP, RFLAGS, RSP.
// /*AFLA*/  *   Discard R11, RCX, CS, SS.
// /*AFLA*/  * Otherwise:
// /*AFLA*/  *   Restore RAX, R11, RCX, CS:RIP, RFLAGS, SS:RSP.
// /*AFLA*/  * All other registers are saved on hypercall entry and restored to user.
// /*AFLA*/  */
// /*AFLA*/ /* Guest exited in SYSCALL context? Return to guest with SYSRET? */
// /*AFLA*/ #define _VGCF_in_syscall 8
// /*AFLA*/ #define VGCF_in_syscall  (1<<_VGCF_in_syscall)
// /*AFLA*/ #define VGCF_IN_SYSCALL  VGCF_in_syscall
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ struct iret_context {
// /*AFLA*/     /* Top of stack (%rsp at point of hypercall). */
// /*AFLA*/     uint64_t rax, r11, rcx, flags, rip, cs, rflags, rsp, ss;
// /*AFLA*/     /* Bottom of iret stack frame. */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #if defined(__GNUC__) && !defined(__STRICT_ANSI__)
// /*AFLA*/ /* Anonymous union includes both 32- and 64-bit names (e.g., eax/rax). */
// /*AFLA*/ #define __DECL_REG(name) union { \
// /*AFLA*/     uint64_t r ## name, e ## name; \
// /*AFLA*/     uint32_t _e ## name; \
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ /* Non-gcc sources must always use the proper 64-bit name (e.g., rax). */
// /*AFLA*/ #define __DECL_REG(name) uint64_t r ## name
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct cpu_user_regs {
// /*AFLA*/     uint64_t r15;
// /*AFLA*/     uint64_t r14;
// /*AFLA*/     uint64_t r13;
// /*AFLA*/     uint64_t r12;
// /*AFLA*/     __DECL_REG(bp);
// /*AFLA*/     __DECL_REG(bx);
// /*AFLA*/     uint64_t r11;
// /*AFLA*/     uint64_t r10;
// /*AFLA*/     uint64_t r9;
// /*AFLA*/     uint64_t r8;
// /*AFLA*/     __DECL_REG(ax);
// /*AFLA*/     __DECL_REG(cx);
// /*AFLA*/     __DECL_REG(dx);
// /*AFLA*/     __DECL_REG(si);
// /*AFLA*/     __DECL_REG(di);
// /*AFLA*/     uint32_t error_code;    /* private */
// /*AFLA*/     uint32_t entry_vector;  /* private */
// /*AFLA*/     __DECL_REG(ip);
// /*AFLA*/     uint16_t cs, _pad0[1];
// /*AFLA*/     uint8_t  saved_upcall_mask;
// /*AFLA*/     uint8_t  _pad1[3];
// /*AFLA*/     __DECL_REG(flags);      /* rflags.IF == !saved_upcall_mask */
// /*AFLA*/     __DECL_REG(sp);
// /*AFLA*/     uint16_t ss, _pad2[3];
// /*AFLA*/     uint16_t es, _pad3[3];
// /*AFLA*/     uint16_t ds, _pad4[3];
// /*AFLA*/     uint16_t fs, _pad5[3]; /* Non-zero => takes precedence over fs_base.     */
// /*AFLA*/     uint16_t gs, _pad6[3]; /* Non-zero => takes precedence over gs_base_usr. */
// /*AFLA*/ };
// /*AFLA*/ DEFINE_GUEST_HANDLE_STRUCT(cpu_user_regs);
// /*AFLA*/ 
// /*AFLA*/ #undef __DECL_REG
// /*AFLA*/ 
// /*AFLA*/ #define xen_pfn_to_cr3(pfn) ((unsigned long)(pfn) << 12)
// /*AFLA*/ #define xen_cr3_to_pfn(cr3) ((unsigned long)(cr3) >> 12)
// /*AFLA*/ 
// /*AFLA*/ struct arch_vcpu_info {
// /*AFLA*/     unsigned long cr2;
// /*AFLA*/     unsigned long pad; /* sizeof(vcpu_info_t) == 64 */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ typedef unsigned long xen_callback_t;
// /*AFLA*/ 
// /*AFLA*/ #define XEN_CALLBACK(__cs, __rip)				\
// /*AFLA*/ 	((unsigned long)(__rip))
// /*AFLA*/ 
// /*AFLA*/ #endif /* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_XEN_INTERFACE_64_H */
