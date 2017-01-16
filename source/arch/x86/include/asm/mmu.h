// /*AFLA*/ #ifndef _ASM_X86_MMU_H
// /*AFLA*/ #define _ASM_X86_MMU_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/mutex.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The x86 doesn't have a mmu context, but
// /*AFLA*/  * we put the segment information here.
// /*AFLA*/  */
// /*AFLA*/ typedef struct {
// /*AFLA*/ #ifdef CONFIG_MODIFY_LDT_SYSCALL
// /*AFLA*/ 	struct ldt_struct *ldt;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 	/* True if mm supports a task running in 32 bit compatibility mode. */
// /*AFLA*/ 	unsigned short ia32_compat;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	struct mutex lock;
// /*AFLA*/ 	void __user *vdso;			/* vdso base address */
// /*AFLA*/ 	const struct vdso_image *vdso_image;	/* vdso image in use */
// /*AFLA*/ 
// /*AFLA*/ 	atomic_t perf_rdpmc_allowed;	/* nonzero if rdpmc is allowed */
// /*AFLA*/ #ifdef CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * One bit per protection key says whether userspace can
// /*AFLA*/ 	 * use it or not.  protected by mmap_sem.
// /*AFLA*/ 	 */
// /*AFLA*/ 	u16 pkey_allocation_map;
// /*AFLA*/ 	s16 execute_only_pkey;
// /*AFLA*/ #endif
// /*AFLA*/ } mm_context_t;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ void leave_mm(int cpu);
// /*AFLA*/ #else
// /*AFLA*/ static inline void leave_mm(int cpu)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_MMU_H */
