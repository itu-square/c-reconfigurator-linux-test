// /*AFLA*/ #ifndef _ASM_X86_PARAVIRT_H
// /*AFLA*/ #define _ASM_X86_PARAVIRT_H
// /*AFLA*/ /* Various instructions on x86 need to be replaced for
// /*AFLA*/  * para-virtualization: those hooks are defined here. */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PARAVIRT
// /*AFLA*/ #include <asm/pgtable_types.h>
// /*AFLA*/ #include <asm/asm.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/paravirt_types.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ #include <asm/frame.h>
// /*AFLA*/ 
// /*AFLA*/ static inline void load_sp0(struct tss_struct *tss,
// /*AFLA*/ 			     struct thread_struct *thread)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL2(pv_cpu_ops.load_sp0, tss, thread);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* The paravirtualized CPUID instruction. */
// /*AFLA*/ static inline void __cpuid(unsigned int *eax, unsigned int *ebx,
// /*AFLA*/ 			   unsigned int *ecx, unsigned int *edx)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL4(pv_cpu_ops.cpuid, eax, ebx, ecx, edx);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These special macros can be used to get or set a debugging register
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long paravirt_get_debugreg(int reg)
// /*AFLA*/ {
// /*AFLA*/ 	return PVOP_CALL1(unsigned long, pv_cpu_ops.get_debugreg, reg);
// /*AFLA*/ }
// /*AFLA*/ #define get_debugreg(var, reg) var = paravirt_get_debugreg(reg)
// /*AFLA*/ static inline void set_debugreg(unsigned long val, int reg)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL2(pv_cpu_ops.set_debugreg, reg, val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void clts(void)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL0(pv_cpu_ops.clts);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long read_cr0(void)
// /*AFLA*/ {
// /*AFLA*/ 	return PVOP_CALL0(unsigned long, pv_cpu_ops.read_cr0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_cr0(unsigned long x)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_cpu_ops.write_cr0, x);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long read_cr2(void)
// /*AFLA*/ {
// /*AFLA*/ 	return PVOP_CALL0(unsigned long, pv_mmu_ops.read_cr2);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_cr2(unsigned long x)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_mmu_ops.write_cr2, x);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long read_cr3(void)
// /*AFLA*/ {
// /*AFLA*/ 	return PVOP_CALL0(unsigned long, pv_mmu_ops.read_cr3);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_cr3(unsigned long x)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_mmu_ops.write_cr3, x);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long __read_cr4(void)
// /*AFLA*/ {
// /*AFLA*/ 	return PVOP_CALL0(unsigned long, pv_cpu_ops.read_cr4);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __write_cr4(unsigned long x)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_cpu_ops.write_cr4, x);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ static inline unsigned long read_cr8(void)
// /*AFLA*/ {
// /*AFLA*/ 	return PVOP_CALL0(unsigned long, pv_cpu_ops.read_cr8);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_cr8(unsigned long x)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_cpu_ops.write_cr8, x);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline void arch_safe_halt(void)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL0(pv_irq_ops.safe_halt);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void halt(void)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL0(pv_irq_ops.halt);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void wbinvd(void)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL0(pv_cpu_ops.wbinvd);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define get_kernel_rpl()  (pv_info.kernel_rpl)
// /*AFLA*/ 
// /*AFLA*/ static inline u64 paravirt_read_msr(unsigned msr)
// /*AFLA*/ {
// /*AFLA*/ 	return PVOP_CALL1(u64, pv_cpu_ops.read_msr, msr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void paravirt_write_msr(unsigned msr,
// /*AFLA*/ 				      unsigned low, unsigned high)
// /*AFLA*/ {
// /*AFLA*/ 	return PVOP_VCALL3(pv_cpu_ops.write_msr, msr, low, high);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u64 paravirt_read_msr_safe(unsigned msr, int *err)
// /*AFLA*/ {
// /*AFLA*/ 	return PVOP_CALL2(u64, pv_cpu_ops.read_msr_safe, msr, err);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int paravirt_write_msr_safe(unsigned msr,
// /*AFLA*/ 					  unsigned low, unsigned high)
// /*AFLA*/ {
// /*AFLA*/ 	return PVOP_CALL3(int, pv_cpu_ops.write_msr_safe, msr, low, high);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define rdmsr(msr, val1, val2)			\
// /*AFLA*/ do {						\
// /*AFLA*/ 	u64 _l = paravirt_read_msr(msr);	\
// /*AFLA*/ 	val1 = (u32)_l;				\
// /*AFLA*/ 	val2 = _l >> 32;			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define wrmsr(msr, val1, val2)			\
// /*AFLA*/ do {						\
// /*AFLA*/ 	paravirt_write_msr(msr, val1, val2);	\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define rdmsrl(msr, val)			\
// /*AFLA*/ do {						\
// /*AFLA*/ 	val = paravirt_read_msr(msr);		\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ static inline void wrmsrl(unsigned msr, u64 val)
// /*AFLA*/ {
// /*AFLA*/ 	wrmsr(msr, (u32)val, (u32)(val>>32));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define wrmsr_safe(msr, a, b)	paravirt_write_msr_safe(msr, a, b)
// /*AFLA*/ 
// /*AFLA*/ /* rdmsr with exception handling */
// /*AFLA*/ #define rdmsr_safe(msr, a, b)				\
// /*AFLA*/ ({							\
// /*AFLA*/ 	int _err;					\
// /*AFLA*/ 	u64 _l = paravirt_read_msr_safe(msr, &_err);	\
// /*AFLA*/ 	(*a) = (u32)_l;					\
// /*AFLA*/ 	(*b) = _l >> 32;				\
// /*AFLA*/ 	_err;						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ static inline int rdmsrl_safe(unsigned msr, unsigned long long *p)
// /*AFLA*/ {
// /*AFLA*/ 	int err;
// /*AFLA*/ 
// /*AFLA*/ 	*p = paravirt_read_msr_safe(msr, &err);
// /*AFLA*/ 	return err;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long long paravirt_sched_clock(void)
// /*AFLA*/ {
// /*AFLA*/ 	return PVOP_CALL0(unsigned long long, pv_time_ops.sched_clock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct static_key;
// /*AFLA*/ extern struct static_key paravirt_steal_enabled;
// /*AFLA*/ extern struct static_key paravirt_steal_rq_enabled;
// /*AFLA*/ 
// /*AFLA*/ static inline u64 paravirt_steal_clock(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return PVOP_CALL1(u64, pv_time_ops.steal_clock, cpu);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long long paravirt_read_pmc(int counter)
// /*AFLA*/ {
// /*AFLA*/ 	return PVOP_CALL1(u64, pv_cpu_ops.read_pmc, counter);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define rdpmc(counter, low, high)		\
// /*AFLA*/ do {						\
// /*AFLA*/ 	u64 _l = paravirt_read_pmc(counter);	\
// /*AFLA*/ 	low = (u32)_l;				\
// /*AFLA*/ 	high = _l >> 32;			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define rdpmcl(counter, val) ((val) = paravirt_read_pmc(counter))
// /*AFLA*/ 
// /*AFLA*/ static inline void paravirt_alloc_ldt(struct desc_struct *ldt, unsigned entries)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL2(pv_cpu_ops.alloc_ldt, ldt, entries);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void paravirt_free_ldt(struct desc_struct *ldt, unsigned entries)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL2(pv_cpu_ops.free_ldt, ldt, entries);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void load_TR_desc(void)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL0(pv_cpu_ops.load_tr_desc);
// /*AFLA*/ }
// /*AFLA*/ static inline void load_gdt(const struct desc_ptr *dtr)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_cpu_ops.load_gdt, dtr);
// /*AFLA*/ }
// /*AFLA*/ static inline void load_idt(const struct desc_ptr *dtr)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_cpu_ops.load_idt, dtr);
// /*AFLA*/ }
// /*AFLA*/ static inline void set_ldt(const void *addr, unsigned entries)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL2(pv_cpu_ops.set_ldt, addr, entries);
// /*AFLA*/ }
// /*AFLA*/ static inline void store_idt(struct desc_ptr *dtr)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_cpu_ops.store_idt, dtr);
// /*AFLA*/ }
// /*AFLA*/ static inline unsigned long paravirt_store_tr(void)
// /*AFLA*/ {
// /*AFLA*/ 	return PVOP_CALL0(unsigned long, pv_cpu_ops.store_tr);
// /*AFLA*/ }
// /*AFLA*/ #define store_tr(tr)	((tr) = paravirt_store_tr())
// /*AFLA*/ static inline void load_TLS(struct thread_struct *t, unsigned cpu)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL2(pv_cpu_ops.load_tls, t, cpu);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ static inline void load_gs_index(unsigned int gs)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_cpu_ops.load_gs_index, gs);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline void write_ldt_entry(struct desc_struct *dt, int entry,
// /*AFLA*/ 				   const void *desc)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL3(pv_cpu_ops.write_ldt_entry, dt, entry, desc);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_gdt_entry(struct desc_struct *dt, int entry,
// /*AFLA*/ 				   void *desc, int type)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL4(pv_cpu_ops.write_gdt_entry, dt, entry, desc, type);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_idt_entry(gate_desc *dt, int entry, const gate_desc *g)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL3(pv_cpu_ops.write_idt_entry, dt, entry, g);
// /*AFLA*/ }
// /*AFLA*/ static inline void set_iopl_mask(unsigned mask)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_cpu_ops.set_iopl_mask, mask);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* The paravirtualized I/O functions */
// /*AFLA*/ static inline void slow_down_io(void)
// /*AFLA*/ {
// /*AFLA*/ 	pv_cpu_ops.io_delay();
// /*AFLA*/ #ifdef REALLY_SLOW_IO
// /*AFLA*/ 	pv_cpu_ops.io_delay();
// /*AFLA*/ 	pv_cpu_ops.io_delay();
// /*AFLA*/ 	pv_cpu_ops.io_delay();
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void paravirt_activate_mm(struct mm_struct *prev,
// /*AFLA*/ 					struct mm_struct *next)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL2(pv_mmu_ops.activate_mm, prev, next);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void paravirt_arch_dup_mmap(struct mm_struct *oldmm,
// /*AFLA*/ 					  struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL2(pv_mmu_ops.dup_mmap, oldmm, mm);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void paravirt_arch_exit_mmap(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_mmu_ops.exit_mmap, mm);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __flush_tlb(void)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL0(pv_mmu_ops.flush_tlb_user);
// /*AFLA*/ }
// /*AFLA*/ static inline void __flush_tlb_global(void)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL0(pv_mmu_ops.flush_tlb_kernel);
// /*AFLA*/ }
// /*AFLA*/ static inline void __flush_tlb_single(unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_mmu_ops.flush_tlb_single, addr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void flush_tlb_others(const struct cpumask *cpumask,
// /*AFLA*/ 				    struct mm_struct *mm,
// /*AFLA*/ 				    unsigned long start,
// /*AFLA*/ 				    unsigned long end)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL4(pv_mmu_ops.flush_tlb_others, cpumask, mm, start, end);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int paravirt_pgd_alloc(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	return PVOP_CALL1(int, pv_mmu_ops.pgd_alloc, mm);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void paravirt_pgd_free(struct mm_struct *mm, pgd_t *pgd)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL2(pv_mmu_ops.pgd_free, mm, pgd);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void paravirt_alloc_pte(struct mm_struct *mm, unsigned long pfn)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL2(pv_mmu_ops.alloc_pte, mm, pfn);
// /*AFLA*/ }
// /*AFLA*/ static inline void paravirt_release_pte(unsigned long pfn)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_mmu_ops.release_pte, pfn);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void paravirt_alloc_pmd(struct mm_struct *mm, unsigned long pfn)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL2(pv_mmu_ops.alloc_pmd, mm, pfn);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void paravirt_release_pmd(unsigned long pfn)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_mmu_ops.release_pmd, pfn);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void paravirt_alloc_pud(struct mm_struct *mm, unsigned long pfn)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL2(pv_mmu_ops.alloc_pud, mm, pfn);
// /*AFLA*/ }
// /*AFLA*/ static inline void paravirt_release_pud(unsigned long pfn)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_mmu_ops.release_pud, pfn);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void pte_update(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 			      pte_t *ptep)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL3(pv_mmu_ops.pte_update, mm, addr, ptep);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pte_t __pte(pteval_t val)
// /*AFLA*/ {
// /*AFLA*/ 	pteval_t ret;
// /*AFLA*/ 
// /*AFLA*/ 	if (sizeof(pteval_t) > sizeof(long))
// /*AFLA*/ 		ret = PVOP_CALLEE2(pteval_t,
// /*AFLA*/ 				   pv_mmu_ops.make_pte,
// /*AFLA*/ 				   val, (u64)val >> 32);
// /*AFLA*/ 	else
// /*AFLA*/ 		ret = PVOP_CALLEE1(pteval_t,
// /*AFLA*/ 				   pv_mmu_ops.make_pte,
// /*AFLA*/ 				   val);
// /*AFLA*/ 
// /*AFLA*/ 	return (pte_t) { .pte = ret };
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pteval_t pte_val(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	pteval_t ret;
// /*AFLA*/ 
// /*AFLA*/ 	if (sizeof(pteval_t) > sizeof(long))
// /*AFLA*/ 		ret = PVOP_CALLEE2(pteval_t, pv_mmu_ops.pte_val,
// /*AFLA*/ 				   pte.pte, (u64)pte.pte >> 32);
// /*AFLA*/ 	else
// /*AFLA*/ 		ret = PVOP_CALLEE1(pteval_t, pv_mmu_ops.pte_val,
// /*AFLA*/ 				   pte.pte);
// /*AFLA*/ 
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pgd_t __pgd(pgdval_t val)
// /*AFLA*/ {
// /*AFLA*/ 	pgdval_t ret;
// /*AFLA*/ 
// /*AFLA*/ 	if (sizeof(pgdval_t) > sizeof(long))
// /*AFLA*/ 		ret = PVOP_CALLEE2(pgdval_t, pv_mmu_ops.make_pgd,
// /*AFLA*/ 				   val, (u64)val >> 32);
// /*AFLA*/ 	else
// /*AFLA*/ 		ret = PVOP_CALLEE1(pgdval_t, pv_mmu_ops.make_pgd,
// /*AFLA*/ 				   val);
// /*AFLA*/ 
// /*AFLA*/ 	return (pgd_t) { ret };
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pgdval_t pgd_val(pgd_t pgd)
// /*AFLA*/ {
// /*AFLA*/ 	pgdval_t ret;
// /*AFLA*/ 
// /*AFLA*/ 	if (sizeof(pgdval_t) > sizeof(long))
// /*AFLA*/ 		ret =  PVOP_CALLEE2(pgdval_t, pv_mmu_ops.pgd_val,
// /*AFLA*/ 				    pgd.pgd, (u64)pgd.pgd >> 32);
// /*AFLA*/ 	else
// /*AFLA*/ 		ret =  PVOP_CALLEE1(pgdval_t, pv_mmu_ops.pgd_val,
// /*AFLA*/ 				    pgd.pgd);
// /*AFLA*/ 
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define  __HAVE_ARCH_PTEP_MODIFY_PROT_TRANSACTION
// /*AFLA*/ static inline pte_t ptep_modify_prot_start(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 					   pte_t *ptep)
// /*AFLA*/ {
// /*AFLA*/ 	pteval_t ret;
// /*AFLA*/ 
// /*AFLA*/ 	ret = PVOP_CALL3(pteval_t, pv_mmu_ops.ptep_modify_prot_start,
// /*AFLA*/ 			 mm, addr, ptep);
// /*AFLA*/ 
// /*AFLA*/ 	return (pte_t) { .pte = ret };
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ptep_modify_prot_commit(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 					   pte_t *ptep, pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	if (sizeof(pteval_t) > sizeof(long))
// /*AFLA*/ 		/* 5 arg words */
// /*AFLA*/ 		pv_mmu_ops.ptep_modify_prot_commit(mm, addr, ptep, pte);
// /*AFLA*/ 	else
// /*AFLA*/ 		PVOP_VCALL4(pv_mmu_ops.ptep_modify_prot_commit,
// /*AFLA*/ 			    mm, addr, ptep, pte.pte);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void set_pte(pte_t *ptep, pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	if (sizeof(pteval_t) > sizeof(long))
// /*AFLA*/ 		PVOP_VCALL3(pv_mmu_ops.set_pte, ptep,
// /*AFLA*/ 			    pte.pte, (u64)pte.pte >> 32);
// /*AFLA*/ 	else
// /*AFLA*/ 		PVOP_VCALL2(pv_mmu_ops.set_pte, ptep,
// /*AFLA*/ 			    pte.pte);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void set_pte_at(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 			      pte_t *ptep, pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	if (sizeof(pteval_t) > sizeof(long))
// /*AFLA*/ 		/* 5 arg words */
// /*AFLA*/ 		pv_mmu_ops.set_pte_at(mm, addr, ptep, pte);
// /*AFLA*/ 	else
// /*AFLA*/ 		PVOP_VCALL4(pv_mmu_ops.set_pte_at, mm, addr, ptep, pte.pte);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void set_pmd_at(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 			      pmd_t *pmdp, pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	if (sizeof(pmdval_t) > sizeof(long))
// /*AFLA*/ 		/* 5 arg words */
// /*AFLA*/ 		pv_mmu_ops.set_pmd_at(mm, addr, pmdp, pmd);
// /*AFLA*/ 	else
// /*AFLA*/ 		PVOP_VCALL4(pv_mmu_ops.set_pmd_at, mm, addr, pmdp,
// /*AFLA*/ 			    native_pmd_val(pmd));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void set_pmd(pmd_t *pmdp, pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	pmdval_t val = native_pmd_val(pmd);
// /*AFLA*/ 
// /*AFLA*/ 	if (sizeof(pmdval_t) > sizeof(long))
// /*AFLA*/ 		PVOP_VCALL3(pv_mmu_ops.set_pmd, pmdp, val, (u64)val >> 32);
// /*AFLA*/ 	else
// /*AFLA*/ 		PVOP_VCALL2(pv_mmu_ops.set_pmd, pmdp, val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #if CONFIG_PGTABLE_LEVELS >= 3
// /*AFLA*/ static inline pmd_t __pmd(pmdval_t val)
// /*AFLA*/ {
// /*AFLA*/ 	pmdval_t ret;
// /*AFLA*/ 
// /*AFLA*/ 	if (sizeof(pmdval_t) > sizeof(long))
// /*AFLA*/ 		ret = PVOP_CALLEE2(pmdval_t, pv_mmu_ops.make_pmd,
// /*AFLA*/ 				   val, (u64)val >> 32);
// /*AFLA*/ 	else
// /*AFLA*/ 		ret = PVOP_CALLEE1(pmdval_t, pv_mmu_ops.make_pmd,
// /*AFLA*/ 				   val);
// /*AFLA*/ 
// /*AFLA*/ 	return (pmd_t) { ret };
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmdval_t pmd_val(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	pmdval_t ret;
// /*AFLA*/ 
// /*AFLA*/ 	if (sizeof(pmdval_t) > sizeof(long))
// /*AFLA*/ 		ret =  PVOP_CALLEE2(pmdval_t, pv_mmu_ops.pmd_val,
// /*AFLA*/ 				    pmd.pmd, (u64)pmd.pmd >> 32);
// /*AFLA*/ 	else
// /*AFLA*/ 		ret =  PVOP_CALLEE1(pmdval_t, pv_mmu_ops.pmd_val,
// /*AFLA*/ 				    pmd.pmd);
// /*AFLA*/ 
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void set_pud(pud_t *pudp, pud_t pud)
// /*AFLA*/ {
// /*AFLA*/ 	pudval_t val = native_pud_val(pud);
// /*AFLA*/ 
// /*AFLA*/ 	if (sizeof(pudval_t) > sizeof(long))
// /*AFLA*/ 		PVOP_VCALL3(pv_mmu_ops.set_pud, pudp,
// /*AFLA*/ 			    val, (u64)val >> 32);
// /*AFLA*/ 	else
// /*AFLA*/ 		PVOP_VCALL2(pv_mmu_ops.set_pud, pudp,
// /*AFLA*/ 			    val);
// /*AFLA*/ }
// /*AFLA*/ #if CONFIG_PGTABLE_LEVELS == 4
// /*AFLA*/ static inline pud_t __pud(pudval_t val)
// /*AFLA*/ {
// /*AFLA*/ 	pudval_t ret;
// /*AFLA*/ 
// /*AFLA*/ 	if (sizeof(pudval_t) > sizeof(long))
// /*AFLA*/ 		ret = PVOP_CALLEE2(pudval_t, pv_mmu_ops.make_pud,
// /*AFLA*/ 				   val, (u64)val >> 32);
// /*AFLA*/ 	else
// /*AFLA*/ 		ret = PVOP_CALLEE1(pudval_t, pv_mmu_ops.make_pud,
// /*AFLA*/ 				   val);
// /*AFLA*/ 
// /*AFLA*/ 	return (pud_t) { ret };
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pudval_t pud_val(pud_t pud)
// /*AFLA*/ {
// /*AFLA*/ 	pudval_t ret;
// /*AFLA*/ 
// /*AFLA*/ 	if (sizeof(pudval_t) > sizeof(long))
// /*AFLA*/ 		ret =  PVOP_CALLEE2(pudval_t, pv_mmu_ops.pud_val,
// /*AFLA*/ 				    pud.pud, (u64)pud.pud >> 32);
// /*AFLA*/ 	else
// /*AFLA*/ 		ret =  PVOP_CALLEE1(pudval_t, pv_mmu_ops.pud_val,
// /*AFLA*/ 				    pud.pud);
// /*AFLA*/ 
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void set_pgd(pgd_t *pgdp, pgd_t pgd)
// /*AFLA*/ {
// /*AFLA*/ 	pgdval_t val = native_pgd_val(pgd);
// /*AFLA*/ 
// /*AFLA*/ 	if (sizeof(pgdval_t) > sizeof(long))
// /*AFLA*/ 		PVOP_VCALL3(pv_mmu_ops.set_pgd, pgdp,
// /*AFLA*/ 			    val, (u64)val >> 32);
// /*AFLA*/ 	else
// /*AFLA*/ 		PVOP_VCALL2(pv_mmu_ops.set_pgd, pgdp,
// /*AFLA*/ 			    val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void pgd_clear(pgd_t *pgdp)
// /*AFLA*/ {
// /*AFLA*/ 	set_pgd(pgdp, __pgd(0));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void pud_clear(pud_t *pudp)
// /*AFLA*/ {
// /*AFLA*/ 	set_pud(pudp, __pud(0));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif	/* CONFIG_PGTABLE_LEVELS == 4 */
// /*AFLA*/ 
// /*AFLA*/ #endif	/* CONFIG_PGTABLE_LEVELS >= 3 */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_PAE
// /*AFLA*/ /* Special-case pte-setting operations for PAE, which can't update a
// /*AFLA*/    64-bit pte atomically */
// /*AFLA*/ static inline void set_pte_atomic(pte_t *ptep, pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL3(pv_mmu_ops.set_pte_atomic, ptep,
// /*AFLA*/ 		    pte.pte, pte.pte >> 32);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void pte_clear(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 			     pte_t *ptep)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL3(pv_mmu_ops.pte_clear, mm, addr, ptep);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void pmd_clear(pmd_t *pmdp)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_mmu_ops.pmd_clear, pmdp);
// /*AFLA*/ }
// /*AFLA*/ #else  /* !CONFIG_X86_PAE */
// /*AFLA*/ static inline void set_pte_atomic(pte_t *ptep, pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	set_pte(ptep, pte);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void pte_clear(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 			     pte_t *ptep)
// /*AFLA*/ {
// /*AFLA*/ 	set_pte_at(mm, addr, ptep, __pte(0));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void pmd_clear(pmd_t *pmdp)
// /*AFLA*/ {
// /*AFLA*/ 	set_pmd(pmdp, __pmd(0));
// /*AFLA*/ }
// /*AFLA*/ #endif	/* CONFIG_X86_PAE */
// /*AFLA*/ 
// /*AFLA*/ #define  __HAVE_ARCH_START_CONTEXT_SWITCH
// /*AFLA*/ static inline void arch_start_context_switch(struct task_struct *prev)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_cpu_ops.start_context_switch, prev);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void arch_end_context_switch(struct task_struct *next)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_cpu_ops.end_context_switch, next);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define  __HAVE_ARCH_ENTER_LAZY_MMU_MODE
// /*AFLA*/ static inline void arch_enter_lazy_mmu_mode(void)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL0(pv_mmu_ops.lazy_mode.enter);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void arch_leave_lazy_mmu_mode(void)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL0(pv_mmu_ops.lazy_mode.leave);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void arch_flush_lazy_mmu_mode(void)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL0(pv_mmu_ops.lazy_mode.flush);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __set_fixmap(unsigned /* enum fixed_addresses */ idx,
// /*AFLA*/ 				phys_addr_t phys, pgprot_t flags)
// /*AFLA*/ {
// /*AFLA*/ 	pv_mmu_ops.set_fixmap(idx, phys, flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_SMP) && defined(CONFIG_PARAVIRT_SPINLOCKS)
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void pv_queued_spin_lock_slowpath(struct qspinlock *lock,
// /*AFLA*/ 							u32 val)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL2(pv_lock_ops.queued_spin_lock_slowpath, lock, val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void pv_queued_spin_unlock(struct qspinlock *lock)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALLEE1(pv_lock_ops.queued_spin_unlock, lock);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void pv_wait(u8 *ptr, u8 val)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL2(pv_lock_ops.wait, ptr, val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void pv_kick(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALL1(pv_lock_ops.kick, cpu);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* SMP && PARAVIRT_SPINLOCKS */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ #define PV_SAVE_REGS "pushl %ecx; pushl %edx;"
// /*AFLA*/ #define PV_RESTORE_REGS "popl %edx; popl %ecx;"
// /*AFLA*/ 
// /*AFLA*/ /* save and restore all caller-save registers, except return value */
// /*AFLA*/ #define PV_SAVE_ALL_CALLER_REGS		"pushl %ecx;"
// /*AFLA*/ #define PV_RESTORE_ALL_CALLER_REGS	"popl  %ecx;"
// /*AFLA*/ 
// /*AFLA*/ #define PV_FLAGS_ARG "0"
// /*AFLA*/ #define PV_EXTRA_CLOBBERS
// /*AFLA*/ #define PV_VEXTRA_CLOBBERS
// /*AFLA*/ #else
// /*AFLA*/ /* save and restore all caller-save registers, except return value */
// /*AFLA*/ #define PV_SAVE_ALL_CALLER_REGS						\
// /*AFLA*/ 	"push %rcx;"							\
// /*AFLA*/ 	"push %rdx;"							\
// /*AFLA*/ 	"push %rsi;"							\
// /*AFLA*/ 	"push %rdi;"							\
// /*AFLA*/ 	"push %r8;"							\
// /*AFLA*/ 	"push %r9;"							\
// /*AFLA*/ 	"push %r10;"							\
// /*AFLA*/ 	"push %r11;"
// /*AFLA*/ #define PV_RESTORE_ALL_CALLER_REGS					\
// /*AFLA*/ 	"pop %r11;"							\
// /*AFLA*/ 	"pop %r10;"							\
// /*AFLA*/ 	"pop %r9;"							\
// /*AFLA*/ 	"pop %r8;"							\
// /*AFLA*/ 	"pop %rdi;"							\
// /*AFLA*/ 	"pop %rsi;"							\
// /*AFLA*/ 	"pop %rdx;"							\
// /*AFLA*/ 	"pop %rcx;"
// /*AFLA*/ 
// /*AFLA*/ /* We save some registers, but all of them, that's too much. We clobber all
// /*AFLA*/  * caller saved registers but the argument parameter */
// /*AFLA*/ #define PV_SAVE_REGS "pushq %%rdi;"
// /*AFLA*/ #define PV_RESTORE_REGS "popq %%rdi;"
// /*AFLA*/ #define PV_EXTRA_CLOBBERS EXTRA_CLOBBERS, "rcx" , "rdx", "rsi"
// /*AFLA*/ #define PV_VEXTRA_CLOBBERS EXTRA_CLOBBERS, "rdi", "rcx" , "rdx", "rsi"
// /*AFLA*/ #define PV_FLAGS_ARG "D"
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Generate a thunk around a function which saves all caller-save
// /*AFLA*/  * registers except for the return value.  This allows C functions to
// /*AFLA*/  * be called from assembler code where fewer than normal registers are
// /*AFLA*/  * available.  It may also help code generation around calls from C
// /*AFLA*/  * code if the common case doesn't use many registers.
// /*AFLA*/  *
// /*AFLA*/  * When a callee is wrapped in a thunk, the caller can assume that all
// /*AFLA*/  * arg regs and all scratch registers are preserved across the
// /*AFLA*/  * call. The return value in rax/eax will not be saved, even for void
// /*AFLA*/  * functions.
// /*AFLA*/  */
// /*AFLA*/ #define PV_THUNK_NAME(func) "__raw_callee_save_" #func
// /*AFLA*/ #define PV_CALLEE_SAVE_REGS_THUNK(func)					\
// /*AFLA*/ 	extern typeof(func) __raw_callee_save_##func;			\
// /*AFLA*/ 									\
// /*AFLA*/ 	asm(".pushsection .text;"					\
// /*AFLA*/ 	    ".globl " PV_THUNK_NAME(func) ";"				\
// /*AFLA*/ 	    ".type " PV_THUNK_NAME(func) ", @function;"			\
// /*AFLA*/ 	    PV_THUNK_NAME(func) ":"					\
// /*AFLA*/ 	    FRAME_BEGIN							\
// /*AFLA*/ 	    PV_SAVE_ALL_CALLER_REGS					\
// /*AFLA*/ 	    "call " #func ";"						\
// /*AFLA*/ 	    PV_RESTORE_ALL_CALLER_REGS					\
// /*AFLA*/ 	    FRAME_END							\
// /*AFLA*/ 	    "ret;"							\
// /*AFLA*/ 	    ".popsection")
// /*AFLA*/ 
// /*AFLA*/ /* Get a reference to a callee-save function */
// /*AFLA*/ #define PV_CALLEE_SAVE(func)						\
// /*AFLA*/ 	((struct paravirt_callee_save) { __raw_callee_save_##func })
// /*AFLA*/ 
// /*AFLA*/ /* Promise that "func" already uses the right calling convention */
// /*AFLA*/ #define __PV_IS_CALLEE_SAVE(func)			\
// /*AFLA*/ 	((struct paravirt_callee_save) { func })
// /*AFLA*/ 
// /*AFLA*/ static inline notrace unsigned long arch_local_save_flags(void)
// /*AFLA*/ {
// /*AFLA*/ 	return PVOP_CALLEE0(unsigned long, pv_irq_ops.save_fl);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline notrace void arch_local_irq_restore(unsigned long f)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALLEE1(pv_irq_ops.restore_fl, f);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline notrace void arch_local_irq_disable(void)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALLEE0(pv_irq_ops.irq_disable);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline notrace void arch_local_irq_enable(void)
// /*AFLA*/ {
// /*AFLA*/ 	PVOP_VCALLEE0(pv_irq_ops.irq_enable);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline notrace unsigned long arch_local_irq_save(void)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long f;
// /*AFLA*/ 
// /*AFLA*/ 	f = arch_local_save_flags();
// /*AFLA*/ 	arch_local_irq_disable();
// /*AFLA*/ 	return f;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* Make sure as little as possible of this mess escapes. */
// /*AFLA*/ #undef PARAVIRT_CALL
// /*AFLA*/ #undef __PVOP_CALL
// /*AFLA*/ #undef __PVOP_VCALL
// /*AFLA*/ #undef PVOP_VCALL0
// /*AFLA*/ #undef PVOP_CALL0
// /*AFLA*/ #undef PVOP_VCALL1
// /*AFLA*/ #undef PVOP_CALL1
// /*AFLA*/ #undef PVOP_VCALL2
// /*AFLA*/ #undef PVOP_CALL2
// /*AFLA*/ #undef PVOP_VCALL3
// /*AFLA*/ #undef PVOP_CALL3
// /*AFLA*/ #undef PVOP_VCALL4
// /*AFLA*/ #undef PVOP_CALL4
// /*AFLA*/ 
// /*AFLA*/ extern void default_banner(void);
// /*AFLA*/ 
// /*AFLA*/ #else  /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #define _PVSITE(ptype, clobbers, ops, word, algn)	\
// /*AFLA*/ 771:;						\
// /*AFLA*/ 	ops;					\
// /*AFLA*/ 772:;						\
// /*AFLA*/ 	.pushsection .parainstructions,"a";	\
// /*AFLA*/ 	 .align	algn;				\
// /*AFLA*/ 	 word 771b;				\
// /*AFLA*/ 	 .byte ptype;				\
// /*AFLA*/ 	 .byte 772b-771b;			\
// /*AFLA*/ 	 .short clobbers;			\
// /*AFLA*/ 	.popsection
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define COND_PUSH(set, mask, reg)			\
// /*AFLA*/ 	.if ((~(set)) & mask); push %reg; .endif
// /*AFLA*/ #define COND_POP(set, mask, reg)			\
// /*AFLA*/ 	.if ((~(set)) & mask); pop %reg; .endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 
// /*AFLA*/ #define PV_SAVE_REGS(set)			\
// /*AFLA*/ 	COND_PUSH(set, CLBR_RAX, rax);		\
// /*AFLA*/ 	COND_PUSH(set, CLBR_RCX, rcx);		\
// /*AFLA*/ 	COND_PUSH(set, CLBR_RDX, rdx);		\
// /*AFLA*/ 	COND_PUSH(set, CLBR_RSI, rsi);		\
// /*AFLA*/ 	COND_PUSH(set, CLBR_RDI, rdi);		\
// /*AFLA*/ 	COND_PUSH(set, CLBR_R8, r8);		\
// /*AFLA*/ 	COND_PUSH(set, CLBR_R9, r9);		\
// /*AFLA*/ 	COND_PUSH(set, CLBR_R10, r10);		\
// /*AFLA*/ 	COND_PUSH(set, CLBR_R11, r11)
// /*AFLA*/ #define PV_RESTORE_REGS(set)			\
// /*AFLA*/ 	COND_POP(set, CLBR_R11, r11);		\
// /*AFLA*/ 	COND_POP(set, CLBR_R10, r10);		\
// /*AFLA*/ 	COND_POP(set, CLBR_R9, r9);		\
// /*AFLA*/ 	COND_POP(set, CLBR_R8, r8);		\
// /*AFLA*/ 	COND_POP(set, CLBR_RDI, rdi);		\
// /*AFLA*/ 	COND_POP(set, CLBR_RSI, rsi);		\
// /*AFLA*/ 	COND_POP(set, CLBR_RDX, rdx);		\
// /*AFLA*/ 	COND_POP(set, CLBR_RCX, rcx);		\
// /*AFLA*/ 	COND_POP(set, CLBR_RAX, rax)
// /*AFLA*/ 
// /*AFLA*/ #define PARA_PATCH(struct, off)        ((PARAVIRT_PATCH_##struct + (off)) / 8)
// /*AFLA*/ #define PARA_SITE(ptype, clobbers, ops) _PVSITE(ptype, clobbers, ops, .quad, 8)
// /*AFLA*/ #define PARA_INDIRECT(addr)	*addr(%rip)
// /*AFLA*/ #else
// /*AFLA*/ #define PV_SAVE_REGS(set)			\
// /*AFLA*/ 	COND_PUSH(set, CLBR_EAX, eax);		\
// /*AFLA*/ 	COND_PUSH(set, CLBR_EDI, edi);		\
// /*AFLA*/ 	COND_PUSH(set, CLBR_ECX, ecx);		\
// /*AFLA*/ 	COND_PUSH(set, CLBR_EDX, edx)
// /*AFLA*/ #define PV_RESTORE_REGS(set)			\
// /*AFLA*/ 	COND_POP(set, CLBR_EDX, edx);		\
// /*AFLA*/ 	COND_POP(set, CLBR_ECX, ecx);		\
// /*AFLA*/ 	COND_POP(set, CLBR_EDI, edi);		\
// /*AFLA*/ 	COND_POP(set, CLBR_EAX, eax)
// /*AFLA*/ 
// /*AFLA*/ #define PARA_PATCH(struct, off)        ((PARAVIRT_PATCH_##struct + (off)) / 4)
// /*AFLA*/ #define PARA_SITE(ptype, clobbers, ops) _PVSITE(ptype, clobbers, ops, .long, 4)
// /*AFLA*/ #define PARA_INDIRECT(addr)	*%cs:addr
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define INTERRUPT_RETURN						\
// /*AFLA*/ 	PARA_SITE(PARA_PATCH(pv_cpu_ops, PV_CPU_iret), CLBR_NONE,	\
// /*AFLA*/ 		  jmp PARA_INDIRECT(pv_cpu_ops+PV_CPU_iret))
// /*AFLA*/ 
// /*AFLA*/ #define DISABLE_INTERRUPTS(clobbers)					\
// /*AFLA*/ 	PARA_SITE(PARA_PATCH(pv_irq_ops, PV_IRQ_irq_disable), clobbers, \
// /*AFLA*/ 		  PV_SAVE_REGS(clobbers | CLBR_CALLEE_SAVE);		\
// /*AFLA*/ 		  call PARA_INDIRECT(pv_irq_ops+PV_IRQ_irq_disable);	\
// /*AFLA*/ 		  PV_RESTORE_REGS(clobbers | CLBR_CALLEE_SAVE);)
// /*AFLA*/ 
// /*AFLA*/ #define ENABLE_INTERRUPTS(clobbers)					\
// /*AFLA*/ 	PARA_SITE(PARA_PATCH(pv_irq_ops, PV_IRQ_irq_enable), clobbers,	\
// /*AFLA*/ 		  PV_SAVE_REGS(clobbers | CLBR_CALLEE_SAVE);		\
// /*AFLA*/ 		  call PARA_INDIRECT(pv_irq_ops+PV_IRQ_irq_enable);	\
// /*AFLA*/ 		  PV_RESTORE_REGS(clobbers | CLBR_CALLEE_SAVE);)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ #define GET_CR0_INTO_EAX				\
// /*AFLA*/ 	push %ecx; push %edx;				\
// /*AFLA*/ 	call PARA_INDIRECT(pv_cpu_ops+PV_CPU_read_cr0);	\
// /*AFLA*/ 	pop %edx; pop %ecx
// /*AFLA*/ #else	/* !CONFIG_X86_32 */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * If swapgs is used while the userspace stack is still current,
// /*AFLA*/  * there's no way to call a pvop.  The PV replacement *must* be
// /*AFLA*/  * inlined, or the swapgs instruction must be trapped and emulated.
// /*AFLA*/  */
// /*AFLA*/ #define SWAPGS_UNSAFE_STACK						\
// /*AFLA*/ 	PARA_SITE(PARA_PATCH(pv_cpu_ops, PV_CPU_swapgs), CLBR_NONE,	\
// /*AFLA*/ 		  swapgs)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Note: swapgs is very special, and in practise is either going to be
// /*AFLA*/  * implemented with a single "swapgs" instruction or something very
// /*AFLA*/  * special.  Either way, we don't need to save any registers for
// /*AFLA*/  * it.
// /*AFLA*/  */
// /*AFLA*/ #define SWAPGS								\
// /*AFLA*/ 	PARA_SITE(PARA_PATCH(pv_cpu_ops, PV_CPU_swapgs), CLBR_NONE,	\
// /*AFLA*/ 		  call PARA_INDIRECT(pv_cpu_ops+PV_CPU_swapgs)		\
// /*AFLA*/ 		 )
// /*AFLA*/ 
// /*AFLA*/ #define GET_CR2_INTO_RAX				\
// /*AFLA*/ 	call PARA_INDIRECT(pv_mmu_ops+PV_MMU_read_cr2)
// /*AFLA*/ 
// /*AFLA*/ #define PARAVIRT_ADJUST_EXCEPTION_FRAME					\
// /*AFLA*/ 	PARA_SITE(PARA_PATCH(pv_irq_ops, PV_IRQ_adjust_exception_frame), \
// /*AFLA*/ 		  CLBR_NONE,						\
// /*AFLA*/ 		  call PARA_INDIRECT(pv_irq_ops+PV_IRQ_adjust_exception_frame))
// /*AFLA*/ 
// /*AFLA*/ #define USERGS_SYSRET64							\
// /*AFLA*/ 	PARA_SITE(PARA_PATCH(pv_cpu_ops, PV_CPU_usergs_sysret64),	\
// /*AFLA*/ 		  CLBR_NONE,						\
// /*AFLA*/ 		  jmp PARA_INDIRECT(pv_cpu_ops+PV_CPU_usergs_sysret64))
// /*AFLA*/ #endif	/* CONFIG_X86_32 */
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ #else  /* CONFIG_PARAVIRT */
// /*AFLA*/ # define default_banner x86_init_noop
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ static inline void paravirt_arch_dup_mmap(struct mm_struct *oldmm,
// /*AFLA*/ 					  struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void paravirt_arch_exit_mmap(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ #endif /* !CONFIG_PARAVIRT */
// /*AFLA*/ #endif /* _ASM_X86_PARAVIRT_H */
