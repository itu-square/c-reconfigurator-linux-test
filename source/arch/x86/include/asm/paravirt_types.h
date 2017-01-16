// /*AFLA*/ #ifndef _ASM_X86_PARAVIRT_TYPES_H
// /*AFLA*/ #define _ASM_X86_PARAVIRT_TYPES_H
// /*AFLA*/ 
// /*AFLA*/ /* Bitmask of what can be clobbered: usually at least eax. */
// /*AFLA*/ #define CLBR_NONE 0
// /*AFLA*/ #define CLBR_EAX  (1 << 0)
// /*AFLA*/ #define CLBR_ECX  (1 << 1)
// /*AFLA*/ #define CLBR_EDX  (1 << 2)
// /*AFLA*/ #define CLBR_EDI  (1 << 3)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ /* CLBR_ANY should match all regs platform has. For i386, that's just it */
// /*AFLA*/ #define CLBR_ANY  ((1 << 4) - 1)
// /*AFLA*/ 
// /*AFLA*/ #define CLBR_ARG_REGS	(CLBR_EAX | CLBR_EDX | CLBR_ECX)
// /*AFLA*/ #define CLBR_RET_REG	(CLBR_EAX | CLBR_EDX)
// /*AFLA*/ #define CLBR_SCRATCH	(0)
// /*AFLA*/ #else
// /*AFLA*/ #define CLBR_RAX  CLBR_EAX
// /*AFLA*/ #define CLBR_RCX  CLBR_ECX
// /*AFLA*/ #define CLBR_RDX  CLBR_EDX
// /*AFLA*/ #define CLBR_RDI  CLBR_EDI
// /*AFLA*/ #define CLBR_RSI  (1 << 4)
// /*AFLA*/ #define CLBR_R8   (1 << 5)
// /*AFLA*/ #define CLBR_R9   (1 << 6)
// /*AFLA*/ #define CLBR_R10  (1 << 7)
// /*AFLA*/ #define CLBR_R11  (1 << 8)
// /*AFLA*/ 
// /*AFLA*/ #define CLBR_ANY  ((1 << 9) - 1)
// /*AFLA*/ 
// /*AFLA*/ #define CLBR_ARG_REGS	(CLBR_RDI | CLBR_RSI | CLBR_RDX | \
// /*AFLA*/ 			 CLBR_RCX | CLBR_R8 | CLBR_R9)
// /*AFLA*/ #define CLBR_RET_REG	(CLBR_RAX)
// /*AFLA*/ #define CLBR_SCRATCH	(CLBR_R10 | CLBR_R11)
// /*AFLA*/ 
// /*AFLA*/ #endif /* X86_64 */
// /*AFLA*/ 
// /*AFLA*/ #define CLBR_CALLEE_SAVE ((CLBR_ARG_REGS | CLBR_SCRATCH) & ~CLBR_RET_REG)
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #include <asm/desc_defs.h>
// /*AFLA*/ #include <asm/kmap_types.h>
// /*AFLA*/ #include <asm/pgtable_types.h>
// /*AFLA*/ 
// /*AFLA*/ struct page;
// /*AFLA*/ struct thread_struct;
// /*AFLA*/ struct desc_ptr;
// /*AFLA*/ struct tss_struct;
// /*AFLA*/ struct mm_struct;
// /*AFLA*/ struct desc_struct;
// /*AFLA*/ struct task_struct;
// /*AFLA*/ struct cpumask;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Wrapper type for pointers to code which uses the non-standard
// /*AFLA*/  * calling convention.  See PV_CALL_SAVE_REGS_THUNK below.
// /*AFLA*/  */
// /*AFLA*/ struct paravirt_callee_save {
// /*AFLA*/ 	void *func;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* general info */
// /*AFLA*/ struct pv_info {
// /*AFLA*/ 	unsigned int kernel_rpl;
// /*AFLA*/ 	int shared_kernel_pmd;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 	u16 extra_user_64bit_cs;  /* __USER_CS if none */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	const char *name;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct pv_init_ops {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Patch may replace one of the defined code sequences with
// /*AFLA*/ 	 * arbitrary code, subject to the same register constraints.
// /*AFLA*/ 	 * This generally means the code is not free to clobber any
// /*AFLA*/ 	 * registers other than EAX.  The patch function should return
// /*AFLA*/ 	 * the number of bytes of code generated, as we nop pad the
// /*AFLA*/ 	 * rest in generic code.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned (*patch)(u8 type, u16 clobber, void *insnbuf,
// /*AFLA*/ 			  unsigned long addr, unsigned len);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ struct pv_lazy_ops {
// /*AFLA*/ 	/* Set deferred update mode, used for batching operations. */
// /*AFLA*/ 	void (*enter)(void);
// /*AFLA*/ 	void (*leave)(void);
// /*AFLA*/ 	void (*flush)(void);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct pv_time_ops {
// /*AFLA*/ 	unsigned long long (*sched_clock)(void);
// /*AFLA*/ 	unsigned long long (*steal_clock)(int cpu);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct pv_cpu_ops {
// /*AFLA*/ 	/* hooks for various privileged instructions */
// /*AFLA*/ 	unsigned long (*get_debugreg)(int regno);
// /*AFLA*/ 	void (*set_debugreg)(int regno, unsigned long value);
// /*AFLA*/ 
// /*AFLA*/ 	void (*clts)(void);
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long (*read_cr0)(void);
// /*AFLA*/ 	void (*write_cr0)(unsigned long);
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long (*read_cr4)(void);
// /*AFLA*/ 	void (*write_cr4)(unsigned long);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 	unsigned long (*read_cr8)(void);
// /*AFLA*/ 	void (*write_cr8)(unsigned long);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	/* Segment descriptor handling */
// /*AFLA*/ 	void (*load_tr_desc)(void);
// /*AFLA*/ 	void (*load_gdt)(const struct desc_ptr *);
// /*AFLA*/ 	void (*load_idt)(const struct desc_ptr *);
// /*AFLA*/ 	/* store_gdt has been removed. */
// /*AFLA*/ 	void (*store_idt)(struct desc_ptr *);
// /*AFLA*/ 	void (*set_ldt)(const void *desc, unsigned entries);
// /*AFLA*/ 	unsigned long (*store_tr)(void);
// /*AFLA*/ 	void (*load_tls)(struct thread_struct *t, unsigned int cpu);
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 	void (*load_gs_index)(unsigned int idx);
// /*AFLA*/ #endif
// /*AFLA*/ 	void (*write_ldt_entry)(struct desc_struct *ldt, int entrynum,
// /*AFLA*/ 				const void *desc);
// /*AFLA*/ 	void (*write_gdt_entry)(struct desc_struct *,
// /*AFLA*/ 				int entrynum, const void *desc, int size);
// /*AFLA*/ 	void (*write_idt_entry)(gate_desc *,
// /*AFLA*/ 				int entrynum, const gate_desc *gate);
// /*AFLA*/ 	void (*alloc_ldt)(struct desc_struct *ldt, unsigned entries);
// /*AFLA*/ 	void (*free_ldt)(struct desc_struct *ldt, unsigned entries);
// /*AFLA*/ 
// /*AFLA*/ 	void (*load_sp0)(struct tss_struct *tss, struct thread_struct *t);
// /*AFLA*/ 
// /*AFLA*/ 	void (*set_iopl_mask)(unsigned mask);
// /*AFLA*/ 
// /*AFLA*/ 	void (*wbinvd)(void);
// /*AFLA*/ 	void (*io_delay)(void);
// /*AFLA*/ 
// /*AFLA*/ 	/* cpuid emulation, mostly so that caps bits can be disabled */
// /*AFLA*/ 	void (*cpuid)(unsigned int *eax, unsigned int *ebx,
// /*AFLA*/ 		      unsigned int *ecx, unsigned int *edx);
// /*AFLA*/ 
// /*AFLA*/ 	/* Unsafe MSR operations.  These will warn or panic on failure. */
// /*AFLA*/ 	u64 (*read_msr)(unsigned int msr);
// /*AFLA*/ 	void (*write_msr)(unsigned int msr, unsigned low, unsigned high);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Safe MSR operations.
// /*AFLA*/ 	 * read sets err to 0 or -EIO.  write returns 0 or -EIO.
// /*AFLA*/ 	 */
// /*AFLA*/ 	u64 (*read_msr_safe)(unsigned int msr, int *err);
// /*AFLA*/ 	int (*write_msr_safe)(unsigned int msr, unsigned low, unsigned high);
// /*AFLA*/ 
// /*AFLA*/ 	u64 (*read_pmc)(int counter);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Switch to usermode gs and return to 64-bit usermode using
// /*AFLA*/ 	 * sysret.  Only used in 64-bit kernels to return to 64-bit
// /*AFLA*/ 	 * processes.  Usermode register state, including %rsp, must
// /*AFLA*/ 	 * already be restored.
// /*AFLA*/ 	 */
// /*AFLA*/ 	void (*usergs_sysret64)(void);
// /*AFLA*/ 
// /*AFLA*/ 	/* Normal iret.  Jump to this with the standard iret stack
// /*AFLA*/ 	   frame set up. */
// /*AFLA*/ 	void (*iret)(void);
// /*AFLA*/ 
// /*AFLA*/ 	void (*swapgs)(void);
// /*AFLA*/ 
// /*AFLA*/ 	void (*start_context_switch)(struct task_struct *prev);
// /*AFLA*/ 	void (*end_context_switch)(struct task_struct *next);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct pv_irq_ops {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Get/set interrupt state.  save_fl and restore_fl are only
// /*AFLA*/ 	 * expected to use X86_EFLAGS_IF; all other bits
// /*AFLA*/ 	 * returned from save_fl are undefined, and may be ignored by
// /*AFLA*/ 	 * restore_fl.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * NOTE: These functions callers expect the callee to preserve
// /*AFLA*/ 	 * more registers than the standard C calling convention.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct paravirt_callee_save save_fl;
// /*AFLA*/ 	struct paravirt_callee_save restore_fl;
// /*AFLA*/ 	struct paravirt_callee_save irq_disable;
// /*AFLA*/ 	struct paravirt_callee_save irq_enable;
// /*AFLA*/ 
// /*AFLA*/ 	void (*safe_halt)(void);
// /*AFLA*/ 	void (*halt)(void);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 	void (*adjust_exception_frame)(void);
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct pv_mmu_ops {
// /*AFLA*/ 	unsigned long (*read_cr2)(void);
// /*AFLA*/ 	void (*write_cr2)(unsigned long);
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long (*read_cr3)(void);
// /*AFLA*/ 	void (*write_cr3)(unsigned long);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Hooks for intercepting the creation/use/destruction of an
// /*AFLA*/ 	 * mm_struct.
// /*AFLA*/ 	 */
// /*AFLA*/ 	void (*activate_mm)(struct mm_struct *prev,
// /*AFLA*/ 			    struct mm_struct *next);
// /*AFLA*/ 	void (*dup_mmap)(struct mm_struct *oldmm,
// /*AFLA*/ 			 struct mm_struct *mm);
// /*AFLA*/ 	void (*exit_mmap)(struct mm_struct *mm);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ 	/* TLB operations */
// /*AFLA*/ 	void (*flush_tlb_user)(void);
// /*AFLA*/ 	void (*flush_tlb_kernel)(void);
// /*AFLA*/ 	void (*flush_tlb_single)(unsigned long addr);
// /*AFLA*/ 	void (*flush_tlb_others)(const struct cpumask *cpus,
// /*AFLA*/ 				 struct mm_struct *mm,
// /*AFLA*/ 				 unsigned long start,
// /*AFLA*/ 				 unsigned long end);
// /*AFLA*/ 
// /*AFLA*/ 	/* Hooks for allocating and freeing a pagetable top-level */
// /*AFLA*/ 	int  (*pgd_alloc)(struct mm_struct *mm);
// /*AFLA*/ 	void (*pgd_free)(struct mm_struct *mm, pgd_t *pgd);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Hooks for allocating/releasing pagetable pages when they're
// /*AFLA*/ 	 * attached to a pagetable
// /*AFLA*/ 	 */
// /*AFLA*/ 	void (*alloc_pte)(struct mm_struct *mm, unsigned long pfn);
// /*AFLA*/ 	void (*alloc_pmd)(struct mm_struct *mm, unsigned long pfn);
// /*AFLA*/ 	void (*alloc_pud)(struct mm_struct *mm, unsigned long pfn);
// /*AFLA*/ 	void (*release_pte)(unsigned long pfn);
// /*AFLA*/ 	void (*release_pmd)(unsigned long pfn);
// /*AFLA*/ 	void (*release_pud)(unsigned long pfn);
// /*AFLA*/ 
// /*AFLA*/ 	/* Pagetable manipulation functions */
// /*AFLA*/ 	void (*set_pte)(pte_t *ptep, pte_t pteval);
// /*AFLA*/ 	void (*set_pte_at)(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 			   pte_t *ptep, pte_t pteval);
// /*AFLA*/ 	void (*set_pmd)(pmd_t *pmdp, pmd_t pmdval);
// /*AFLA*/ 	void (*set_pmd_at)(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 			   pmd_t *pmdp, pmd_t pmdval);
// /*AFLA*/ 	void (*pte_update)(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 			   pte_t *ptep);
// /*AFLA*/ 
// /*AFLA*/ 	pte_t (*ptep_modify_prot_start)(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 					pte_t *ptep);
// /*AFLA*/ 	void (*ptep_modify_prot_commit)(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 					pte_t *ptep, pte_t pte);
// /*AFLA*/ 
// /*AFLA*/ 	struct paravirt_callee_save pte_val;
// /*AFLA*/ 	struct paravirt_callee_save make_pte;
// /*AFLA*/ 
// /*AFLA*/ 	struct paravirt_callee_save pgd_val;
// /*AFLA*/ 	struct paravirt_callee_save make_pgd;
// /*AFLA*/ 
// /*AFLA*/ #if CONFIG_PGTABLE_LEVELS >= 3
// /*AFLA*/ #ifdef CONFIG_X86_PAE
// /*AFLA*/ 	void (*set_pte_atomic)(pte_t *ptep, pte_t pteval);
// /*AFLA*/ 	void (*pte_clear)(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 			  pte_t *ptep);
// /*AFLA*/ 	void (*pmd_clear)(pmd_t *pmdp);
// /*AFLA*/ 
// /*AFLA*/ #endif	/* CONFIG_X86_PAE */
// /*AFLA*/ 
// /*AFLA*/ 	void (*set_pud)(pud_t *pudp, pud_t pudval);
// /*AFLA*/ 
// /*AFLA*/ 	struct paravirt_callee_save pmd_val;
// /*AFLA*/ 	struct paravirt_callee_save make_pmd;
// /*AFLA*/ 
// /*AFLA*/ #if CONFIG_PGTABLE_LEVELS == 4
// /*AFLA*/ 	struct paravirt_callee_save pud_val;
// /*AFLA*/ 	struct paravirt_callee_save make_pud;
// /*AFLA*/ 
// /*AFLA*/ 	void (*set_pgd)(pgd_t *pudp, pgd_t pgdval);
// /*AFLA*/ #endif	/* CONFIG_PGTABLE_LEVELS == 4 */
// /*AFLA*/ #endif	/* CONFIG_PGTABLE_LEVELS >= 3 */
// /*AFLA*/ 
// /*AFLA*/ 	struct pv_lazy_ops lazy_mode;
// /*AFLA*/ 
// /*AFLA*/ 	/* dom0 ops */
// /*AFLA*/ 
// /*AFLA*/ 	/* Sometimes the physical address is a pfn, and sometimes its
// /*AFLA*/ 	   an mfn.  We can tell which is which from the index. */
// /*AFLA*/ 	void (*set_fixmap)(unsigned /* enum fixed_addresses */ idx,
// /*AFLA*/ 			   phys_addr_t phys, pgprot_t flags);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct arch_spinlock;
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ #include <asm/spinlock_types.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct qspinlock;
// /*AFLA*/ 
// /*AFLA*/ struct pv_lock_ops {
// /*AFLA*/ 	void (*queued_spin_lock_slowpath)(struct qspinlock *lock, u32 val);
// /*AFLA*/ 	struct paravirt_callee_save queued_spin_unlock;
// /*AFLA*/ 
// /*AFLA*/ 	void (*wait)(u8 *ptr, u8 val);
// /*AFLA*/ 	void (*kick)(int cpu);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* This contains all the paravirt structures: we get a convenient
// /*AFLA*/  * number for each function using the offset which we use to indicate
// /*AFLA*/  * what to patch. */
// /*AFLA*/ struct paravirt_patch_template {
// /*AFLA*/ 	struct pv_init_ops pv_init_ops;
// /*AFLA*/ 	struct pv_time_ops pv_time_ops;
// /*AFLA*/ 	struct pv_cpu_ops pv_cpu_ops;
// /*AFLA*/ 	struct pv_irq_ops pv_irq_ops;
// /*AFLA*/ 	struct pv_mmu_ops pv_mmu_ops;
// /*AFLA*/ 	struct pv_lock_ops pv_lock_ops;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct pv_info pv_info;
// /*AFLA*/ extern struct pv_init_ops pv_init_ops;
// /*AFLA*/ extern struct pv_time_ops pv_time_ops;
// /*AFLA*/ extern struct pv_cpu_ops pv_cpu_ops;
// /*AFLA*/ extern struct pv_irq_ops pv_irq_ops;
// /*AFLA*/ extern struct pv_mmu_ops pv_mmu_ops;
// /*AFLA*/ extern struct pv_lock_ops pv_lock_ops;
// /*AFLA*/ 
// /*AFLA*/ #define PARAVIRT_PATCH(x)					\
// /*AFLA*/ 	(offsetof(struct paravirt_patch_template, x) / sizeof(void *))
// /*AFLA*/ 
// /*AFLA*/ #define paravirt_type(op)				\
// /*AFLA*/ 	[paravirt_typenum] "i" (PARAVIRT_PATCH(op)),	\
// /*AFLA*/ 	[paravirt_opptr] "i" (&(op))
// /*AFLA*/ #define paravirt_clobber(clobber)		\
// /*AFLA*/ 	[paravirt_clobber] "i" (clobber)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Generate some code, and mark it as patchable by the
// /*AFLA*/  * apply_paravirt() alternate instruction patcher.
// /*AFLA*/  */
// /*AFLA*/ #define _paravirt_alt(insn_string, type, clobber)	\
// /*AFLA*/ 	"771:\n\t" insn_string "\n" "772:\n"		\
// /*AFLA*/ 	".pushsection .parainstructions,\"a\"\n"	\
// /*AFLA*/ 	_ASM_ALIGN "\n"					\
// /*AFLA*/ 	_ASM_PTR " 771b\n"				\
// /*AFLA*/ 	"  .byte " type "\n"				\
// /*AFLA*/ 	"  .byte 772b-771b\n"				\
// /*AFLA*/ 	"  .short " clobber "\n"			\
// /*AFLA*/ 	".popsection\n"
// /*AFLA*/ 
// /*AFLA*/ /* Generate patchable code, with the default asm parameters. */
// /*AFLA*/ #define paravirt_alt(insn_string)					\
// /*AFLA*/ 	_paravirt_alt(insn_string, "%c[paravirt_typenum]", "%c[paravirt_clobber]")
// /*AFLA*/ 
// /*AFLA*/ /* Simple instruction patching code. */
// /*AFLA*/ #define NATIVE_LABEL(a,x,b) "\n\t.globl " a #x "_" #b "\n" a #x "_" #b ":\n\t"
// /*AFLA*/ 
// /*AFLA*/ #define DEF_NATIVE(ops, name, code)					\
// /*AFLA*/ 	__visible extern const char start_##ops##_##name[], end_##ops##_##name[];	\
// /*AFLA*/ 	asm(NATIVE_LABEL("start_", ops, name) code NATIVE_LABEL("end_", ops, name))
// /*AFLA*/ 
// /*AFLA*/ unsigned paravirt_patch_ident_32(void *insnbuf, unsigned len);
// /*AFLA*/ unsigned paravirt_patch_ident_64(void *insnbuf, unsigned len);
// /*AFLA*/ unsigned paravirt_patch_call(void *insnbuf,
// /*AFLA*/ 			     const void *target, u16 tgt_clobbers,
// /*AFLA*/ 			     unsigned long addr, u16 site_clobbers,
// /*AFLA*/ 			     unsigned len);
// /*AFLA*/ unsigned paravirt_patch_jmp(void *insnbuf, const void *target,
// /*AFLA*/ 			    unsigned long addr, unsigned len);
// /*AFLA*/ unsigned paravirt_patch_default(u8 type, u16 clobbers, void *insnbuf,
// /*AFLA*/ 				unsigned long addr, unsigned len);
// /*AFLA*/ 
// /*AFLA*/ unsigned paravirt_patch_insns(void *insnbuf, unsigned len,
// /*AFLA*/ 			      const char *start, const char *end);
// /*AFLA*/ 
// /*AFLA*/ unsigned native_patch(u8 type, u16 clobbers, void *ibuf,
// /*AFLA*/ 		      unsigned long addr, unsigned len);
// /*AFLA*/ 
// /*AFLA*/ int paravirt_disable_iospace(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This generates an indirect call based on the operation type number.
// /*AFLA*/  * The type number, computed in PARAVIRT_PATCH, is derived from the
// /*AFLA*/  * offset into the paravirt_patch_template structure, and can therefore be
// /*AFLA*/  * freely converted back into a structure offset.
// /*AFLA*/  */
// /*AFLA*/ #define PARAVIRT_CALL	"call *%c[paravirt_opptr];"
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These macros are intended to wrap calls through one of the paravirt
// /*AFLA*/  * ops structs, so that they can be later identified and patched at
// /*AFLA*/  * runtime.
// /*AFLA*/  *
// /*AFLA*/  * Normally, a call to a pv_op function is a simple indirect call:
// /*AFLA*/  * (pv_op_struct.operations)(args...).
// /*AFLA*/  *
// /*AFLA*/  * Unfortunately, this is a relatively slow operation for modern CPUs,
// /*AFLA*/  * because it cannot necessarily determine what the destination
// /*AFLA*/  * address is.  In this case, the address is a runtime constant, so at
// /*AFLA*/  * the very least we can patch the call to e a simple direct call, or
// /*AFLA*/  * ideally, patch an inline implementation into the callsite.  (Direct
// /*AFLA*/  * calls are essentially free, because the call and return addresses
// /*AFLA*/  * are completely predictable.)
// /*AFLA*/  *
// /*AFLA*/  * For i386, these macros rely on the standard gcc "regparm(3)" calling
// /*AFLA*/  * convention, in which the first three arguments are placed in %eax,
// /*AFLA*/  * %edx, %ecx (in that order), and the remaining arguments are placed
// /*AFLA*/  * on the stack.  All caller-save registers (eax,edx,ecx) are expected
// /*AFLA*/  * to be modified (either clobbered or used for return values).
// /*AFLA*/  * X86_64, on the other hand, already specifies a register-based calling
// /*AFLA*/  * conventions, returning at %rax, with parameteres going on %rdi, %rsi,
// /*AFLA*/  * %rdx, and %rcx. Note that for this reason, x86_64 does not need any
// /*AFLA*/  * special handling for dealing with 4 arguments, unlike i386.
// /*AFLA*/  * However, x86_64 also have to clobber all caller saved registers, which
// /*AFLA*/  * unfortunately, are quite a bit (r8 - r11)
// /*AFLA*/  *
// /*AFLA*/  * The call instruction itself is marked by placing its start address
// /*AFLA*/  * and size into the .parainstructions section, so that
// /*AFLA*/  * apply_paravirt() in arch/i386/kernel/alternative.c can do the
// /*AFLA*/  * appropriate patching under the control of the backend pv_init_ops
// /*AFLA*/  * implementation.
// /*AFLA*/  *
// /*AFLA*/  * Unfortunately there's no way to get gcc to generate the args setup
// /*AFLA*/  * for the call, and then allow the call itself to be generated by an
// /*AFLA*/  * inline asm.  Because of this, we must do the complete arg setup and
// /*AFLA*/  * return value handling from within these macros.  This is fairly
// /*AFLA*/  * cumbersome.
// /*AFLA*/  *
// /*AFLA*/  * There are 5 sets of PVOP_* macros for dealing with 0-4 arguments.
// /*AFLA*/  * It could be extended to more arguments, but there would be little
// /*AFLA*/  * to be gained from that.  For each number of arguments, there are
// /*AFLA*/  * the two VCALL and CALL variants for void and non-void functions.
// /*AFLA*/  *
// /*AFLA*/  * When there is a return value, the invoker of the macro must specify
// /*AFLA*/  * the return type.  The macro then uses sizeof() on that type to
// /*AFLA*/  * determine whether its a 32 or 64 bit value, and places the return
// /*AFLA*/  * in the right register(s) (just %eax for 32-bit, and %edx:%eax for
// /*AFLA*/  * 64-bit). For x86_64 machines, it just returns at %rax regardless of
// /*AFLA*/  * the return value size.
// /*AFLA*/  *
// /*AFLA*/  * 64-bit arguments are passed as a pair of adjacent 32-bit arguments
// /*AFLA*/  * i386 also passes 64-bit arguments as a pair of adjacent 32-bit arguments
// /*AFLA*/  * in low,high order
// /*AFLA*/  *
// /*AFLA*/  * Small structures are passed and returned in registers.  The macro
// /*AFLA*/  * calling convention can't directly deal with this, so the wrapper
// /*AFLA*/  * functions must do this.
// /*AFLA*/  *
// /*AFLA*/  * These PVOP_* macros are only defined within this header.  This
// /*AFLA*/  * means that all uses must be wrapped in inline functions.  This also
// /*AFLA*/  * makes sure the incoming and outgoing types are always correct.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ #define PVOP_VCALL_ARGS							\
// /*AFLA*/ 	unsigned long __eax = __eax, __edx = __edx, __ecx = __ecx;	\
// /*AFLA*/ 	register void *__sp asm("esp")
// /*AFLA*/ #define PVOP_CALL_ARGS			PVOP_VCALL_ARGS
// /*AFLA*/ 
// /*AFLA*/ #define PVOP_CALL_ARG1(x)		"a" ((unsigned long)(x))
// /*AFLA*/ #define PVOP_CALL_ARG2(x)		"d" ((unsigned long)(x))
// /*AFLA*/ #define PVOP_CALL_ARG3(x)		"c" ((unsigned long)(x))
// /*AFLA*/ 
// /*AFLA*/ #define PVOP_VCALL_CLOBBERS		"=a" (__eax), "=d" (__edx),	\
// /*AFLA*/ 					"=c" (__ecx)
// /*AFLA*/ #define PVOP_CALL_CLOBBERS		PVOP_VCALL_CLOBBERS
// /*AFLA*/ 
// /*AFLA*/ #define PVOP_VCALLEE_CLOBBERS		"=a" (__eax), "=d" (__edx)
// /*AFLA*/ #define PVOP_CALLEE_CLOBBERS		PVOP_VCALLEE_CLOBBERS
// /*AFLA*/ 
// /*AFLA*/ #define EXTRA_CLOBBERS
// /*AFLA*/ #define VEXTRA_CLOBBERS
// /*AFLA*/ #else  /* CONFIG_X86_64 */
// /*AFLA*/ /* [re]ax isn't an arg, but the return val */
// /*AFLA*/ #define PVOP_VCALL_ARGS						\
// /*AFLA*/ 	unsigned long __edi = __edi, __esi = __esi,		\
// /*AFLA*/ 		__edx = __edx, __ecx = __ecx, __eax = __eax;	\
// /*AFLA*/ 	register void *__sp asm("rsp")
// /*AFLA*/ #define PVOP_CALL_ARGS		PVOP_VCALL_ARGS
// /*AFLA*/ 
// /*AFLA*/ #define PVOP_CALL_ARG1(x)		"D" ((unsigned long)(x))
// /*AFLA*/ #define PVOP_CALL_ARG2(x)		"S" ((unsigned long)(x))
// /*AFLA*/ #define PVOP_CALL_ARG3(x)		"d" ((unsigned long)(x))
// /*AFLA*/ #define PVOP_CALL_ARG4(x)		"c" ((unsigned long)(x))
// /*AFLA*/ 
// /*AFLA*/ #define PVOP_VCALL_CLOBBERS	"=D" (__edi),				\
// /*AFLA*/ 				"=S" (__esi), "=d" (__edx),		\
// /*AFLA*/ 				"=c" (__ecx)
// /*AFLA*/ #define PVOP_CALL_CLOBBERS	PVOP_VCALL_CLOBBERS, "=a" (__eax)
// /*AFLA*/ 
// /*AFLA*/ /* void functions are still allowed [re]ax for scratch */
// /*AFLA*/ #define PVOP_VCALLEE_CLOBBERS	"=a" (__eax)
// /*AFLA*/ #define PVOP_CALLEE_CLOBBERS	PVOP_VCALLEE_CLOBBERS
// /*AFLA*/ 
// /*AFLA*/ #define EXTRA_CLOBBERS	 , "r8", "r9", "r10", "r11"
// /*AFLA*/ #define VEXTRA_CLOBBERS	 , "rax", "r8", "r9", "r10", "r11"
// /*AFLA*/ #endif	/* CONFIG_X86_32 */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PARAVIRT_DEBUG
// /*AFLA*/ #define PVOP_TEST_NULL(op)	BUG_ON(op == NULL)
// /*AFLA*/ #else
// /*AFLA*/ #define PVOP_TEST_NULL(op)	((void)op)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define ____PVOP_CALL(rettype, op, clbr, call_clbr, extra_clbr,		\
// /*AFLA*/ 		      pre, post, ...)					\
// /*AFLA*/ 	({								\
// /*AFLA*/ 		rettype __ret;						\
// /*AFLA*/ 		PVOP_CALL_ARGS;						\
// /*AFLA*/ 		PVOP_TEST_NULL(op);					\
// /*AFLA*/ 		/* This is 32-bit specific, but is okay in 64-bit */	\
// /*AFLA*/ 		/* since this condition will never hold */		\
// /*AFLA*/ 		if (sizeof(rettype) > sizeof(unsigned long)) {		\
// /*AFLA*/ 			asm volatile(pre				\
// /*AFLA*/ 				     paravirt_alt(PARAVIRT_CALL)	\
// /*AFLA*/ 				     post				\
// /*AFLA*/ 				     : call_clbr, "+r" (__sp)		\
// /*AFLA*/ 				     : paravirt_type(op),		\
// /*AFLA*/ 				       paravirt_clobber(clbr),		\
// /*AFLA*/ 				       ##__VA_ARGS__			\
// /*AFLA*/ 				     : "memory", "cc" extra_clbr);	\
// /*AFLA*/ 			__ret = (rettype)((((u64)__edx) << 32) | __eax); \
// /*AFLA*/ 		} else {						\
// /*AFLA*/ 			asm volatile(pre				\
// /*AFLA*/ 				     paravirt_alt(PARAVIRT_CALL)	\
// /*AFLA*/ 				     post				\
// /*AFLA*/ 				     : call_clbr, "+r" (__sp)		\
// /*AFLA*/ 				     : paravirt_type(op),		\
// /*AFLA*/ 				       paravirt_clobber(clbr),		\
// /*AFLA*/ 				       ##__VA_ARGS__			\
// /*AFLA*/ 				     : "memory", "cc" extra_clbr);	\
// /*AFLA*/ 			__ret = (rettype)__eax;				\
// /*AFLA*/ 		}							\
// /*AFLA*/ 		__ret;							\
// /*AFLA*/ 	})
// /*AFLA*/ 
// /*AFLA*/ #define __PVOP_CALL(rettype, op, pre, post, ...)			\
// /*AFLA*/ 	____PVOP_CALL(rettype, op, CLBR_ANY, PVOP_CALL_CLOBBERS,	\
// /*AFLA*/ 		      EXTRA_CLOBBERS, pre, post, ##__VA_ARGS__)
// /*AFLA*/ 
// /*AFLA*/ #define __PVOP_CALLEESAVE(rettype, op, pre, post, ...)			\
// /*AFLA*/ 	____PVOP_CALL(rettype, op.func, CLBR_RET_REG,			\
// /*AFLA*/ 		      PVOP_CALLEE_CLOBBERS, ,				\
// /*AFLA*/ 		      pre, post, ##__VA_ARGS__)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define ____PVOP_VCALL(op, clbr, call_clbr, extra_clbr, pre, post, ...)	\
// /*AFLA*/ 	({								\
// /*AFLA*/ 		PVOP_VCALL_ARGS;					\
// /*AFLA*/ 		PVOP_TEST_NULL(op);					\
// /*AFLA*/ 		asm volatile(pre					\
// /*AFLA*/ 			     paravirt_alt(PARAVIRT_CALL)		\
// /*AFLA*/ 			     post					\
// /*AFLA*/ 			     : call_clbr, "+r" (__sp)			\
// /*AFLA*/ 			     : paravirt_type(op),			\
// /*AFLA*/ 			       paravirt_clobber(clbr),			\
// /*AFLA*/ 			       ##__VA_ARGS__				\
// /*AFLA*/ 			     : "memory", "cc" extra_clbr);		\
// /*AFLA*/ 	})
// /*AFLA*/ 
// /*AFLA*/ #define __PVOP_VCALL(op, pre, post, ...)				\
// /*AFLA*/ 	____PVOP_VCALL(op, CLBR_ANY, PVOP_VCALL_CLOBBERS,		\
// /*AFLA*/ 		       VEXTRA_CLOBBERS,					\
// /*AFLA*/ 		       pre, post, ##__VA_ARGS__)
// /*AFLA*/ 
// /*AFLA*/ #define __PVOP_VCALLEESAVE(op, pre, post, ...)				\
// /*AFLA*/ 	____PVOP_VCALL(op.func, CLBR_RET_REG,				\
// /*AFLA*/ 		      PVOP_VCALLEE_CLOBBERS, ,				\
// /*AFLA*/ 		      pre, post, ##__VA_ARGS__)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define PVOP_CALL0(rettype, op)						\
// /*AFLA*/ 	__PVOP_CALL(rettype, op, "", "")
// /*AFLA*/ #define PVOP_VCALL0(op)							\
// /*AFLA*/ 	__PVOP_VCALL(op, "", "")
// /*AFLA*/ 
// /*AFLA*/ #define PVOP_CALLEE0(rettype, op)					\
// /*AFLA*/ 	__PVOP_CALLEESAVE(rettype, op, "", "")
// /*AFLA*/ #define PVOP_VCALLEE0(op)						\
// /*AFLA*/ 	__PVOP_VCALLEESAVE(op, "", "")
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define PVOP_CALL1(rettype, op, arg1)					\
// /*AFLA*/ 	__PVOP_CALL(rettype, op, "", "", PVOP_CALL_ARG1(arg1))
// /*AFLA*/ #define PVOP_VCALL1(op, arg1)						\
// /*AFLA*/ 	__PVOP_VCALL(op, "", "", PVOP_CALL_ARG1(arg1))
// /*AFLA*/ 
// /*AFLA*/ #define PVOP_CALLEE1(rettype, op, arg1)					\
// /*AFLA*/ 	__PVOP_CALLEESAVE(rettype, op, "", "", PVOP_CALL_ARG1(arg1))
// /*AFLA*/ #define PVOP_VCALLEE1(op, arg1)						\
// /*AFLA*/ 	__PVOP_VCALLEESAVE(op, "", "", PVOP_CALL_ARG1(arg1))
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define PVOP_CALL2(rettype, op, arg1, arg2)				\
// /*AFLA*/ 	__PVOP_CALL(rettype, op, "", "", PVOP_CALL_ARG1(arg1),		\
// /*AFLA*/ 		    PVOP_CALL_ARG2(arg2))
// /*AFLA*/ #define PVOP_VCALL2(op, arg1, arg2)					\
// /*AFLA*/ 	__PVOP_VCALL(op, "", "", PVOP_CALL_ARG1(arg1),			\
// /*AFLA*/ 		     PVOP_CALL_ARG2(arg2))
// /*AFLA*/ 
// /*AFLA*/ #define PVOP_CALLEE2(rettype, op, arg1, arg2)				\
// /*AFLA*/ 	__PVOP_CALLEESAVE(rettype, op, "", "", PVOP_CALL_ARG1(arg1),	\
// /*AFLA*/ 			  PVOP_CALL_ARG2(arg2))
// /*AFLA*/ #define PVOP_VCALLEE2(op, arg1, arg2)					\
// /*AFLA*/ 	__PVOP_VCALLEESAVE(op, "", "", PVOP_CALL_ARG1(arg1),		\
// /*AFLA*/ 			   PVOP_CALL_ARG2(arg2))
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define PVOP_CALL3(rettype, op, arg1, arg2, arg3)			\
// /*AFLA*/ 	__PVOP_CALL(rettype, op, "", "", PVOP_CALL_ARG1(arg1),		\
// /*AFLA*/ 		    PVOP_CALL_ARG2(arg2), PVOP_CALL_ARG3(arg3))
// /*AFLA*/ #define PVOP_VCALL3(op, arg1, arg2, arg3)				\
// /*AFLA*/ 	__PVOP_VCALL(op, "", "", PVOP_CALL_ARG1(arg1),			\
// /*AFLA*/ 		     PVOP_CALL_ARG2(arg2), PVOP_CALL_ARG3(arg3))
// /*AFLA*/ 
// /*AFLA*/ /* This is the only difference in x86_64. We can make it much simpler */
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ #define PVOP_CALL4(rettype, op, arg1, arg2, arg3, arg4)			\
// /*AFLA*/ 	__PVOP_CALL(rettype, op,					\
// /*AFLA*/ 		    "push %[_arg4];", "lea 4(%%esp),%%esp;",		\
// /*AFLA*/ 		    PVOP_CALL_ARG1(arg1), PVOP_CALL_ARG2(arg2),		\
// /*AFLA*/ 		    PVOP_CALL_ARG3(arg3), [_arg4] "mr" ((u32)(arg4)))
// /*AFLA*/ #define PVOP_VCALL4(op, arg1, arg2, arg3, arg4)				\
// /*AFLA*/ 	__PVOP_VCALL(op,						\
// /*AFLA*/ 		    "push %[_arg4];", "lea 4(%%esp),%%esp;",		\
// /*AFLA*/ 		    "0" ((u32)(arg1)), "1" ((u32)(arg2)),		\
// /*AFLA*/ 		    "2" ((u32)(arg3)), [_arg4] "mr" ((u32)(arg4)))
// /*AFLA*/ #else
// /*AFLA*/ #define PVOP_CALL4(rettype, op, arg1, arg2, arg3, arg4)			\
// /*AFLA*/ 	__PVOP_CALL(rettype, op, "", "",				\
// /*AFLA*/ 		    PVOP_CALL_ARG1(arg1), PVOP_CALL_ARG2(arg2),		\
// /*AFLA*/ 		    PVOP_CALL_ARG3(arg3), PVOP_CALL_ARG4(arg4))
// /*AFLA*/ #define PVOP_VCALL4(op, arg1, arg2, arg3, arg4)				\
// /*AFLA*/ 	__PVOP_VCALL(op, "", "",					\
// /*AFLA*/ 		     PVOP_CALL_ARG1(arg1), PVOP_CALL_ARG2(arg2),	\
// /*AFLA*/ 		     PVOP_CALL_ARG3(arg3), PVOP_CALL_ARG4(arg4))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Lazy mode for batching updates / context switch */
// /*AFLA*/ enum paravirt_lazy_mode {
// /*AFLA*/ 	PARAVIRT_LAZY_NONE,
// /*AFLA*/ 	PARAVIRT_LAZY_MMU,
// /*AFLA*/ 	PARAVIRT_LAZY_CPU,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum paravirt_lazy_mode paravirt_get_lazy_mode(void);
// /*AFLA*/ void paravirt_start_context_switch(struct task_struct *prev);
// /*AFLA*/ void paravirt_end_context_switch(struct task_struct *next);
// /*AFLA*/ 
// /*AFLA*/ void paravirt_enter_lazy_mmu(void);
// /*AFLA*/ void paravirt_leave_lazy_mmu(void);
// /*AFLA*/ void paravirt_flush_lazy_mmu(void);
// /*AFLA*/ 
// /*AFLA*/ void _paravirt_nop(void);
// /*AFLA*/ u32 _paravirt_ident_32(u32);
// /*AFLA*/ u64 _paravirt_ident_64(u64);
// /*AFLA*/ 
// /*AFLA*/ #define paravirt_nop	((void *)_paravirt_nop)
// /*AFLA*/ 
// /*AFLA*/ /* These all sit in the .parainstructions section to tell us what to patch. */
// /*AFLA*/ struct paravirt_patch_site {
// /*AFLA*/ 	u8 *instr; 		/* original instructions */
// /*AFLA*/ 	u8 instrtype;		/* type of this instruction */
// /*AFLA*/ 	u8 len;			/* length of original instruction */
// /*AFLA*/ 	u16 clobbers;		/* what registers you may clobber */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct paravirt_patch_site __parainstructions[],
// /*AFLA*/ 	__parainstructions_end[];
// /*AFLA*/ 
// /*AFLA*/ #endif	/* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif	/* _ASM_X86_PARAVIRT_TYPES_H */
