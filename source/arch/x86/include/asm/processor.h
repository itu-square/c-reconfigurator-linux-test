// /*AFLA*/ #ifndef _ASM_X86_PROCESSOR_H
// /*AFLA*/ #define _ASM_X86_PROCESSOR_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/processor-flags.h>
// /*AFLA*/ 
// /*AFLA*/ /* Forward declaration, a strange C thing */
// /*AFLA*/ struct task_struct;
// /*AFLA*/ struct mm_struct;
// /*AFLA*/ struct vm86;
// /*AFLA*/ 
// /*AFLA*/ #include <asm/math_emu.h>
// /*AFLA*/ #include <asm/segment.h>
// /*AFLA*/ #include <asm/types.h>
// /*AFLA*/ #include <uapi/asm/sigcontext.h>
// /*AFLA*/ #include <asm/current.h>
// /*AFLA*/ #include <asm/cpufeatures.h>
// /*AFLA*/ #include <asm/page.h>
// /*AFLA*/ #include <asm/pgtable_types.h>
// /*AFLA*/ #include <asm/percpu.h>
// /*AFLA*/ #include <asm/msr.h>
// /*AFLA*/ #include <asm/desc_defs.h>
// /*AFLA*/ #include <asm/nops.h>
// /*AFLA*/ #include <asm/special_insns.h>
// /*AFLA*/ #include <asm/fpu/types.h>
// /*AFLA*/ 
// /*AFLA*/ #include <linux/personality.h>
// /*AFLA*/ #include <linux/cache.h>
// /*AFLA*/ #include <linux/threads.h>
// /*AFLA*/ #include <linux/math64.h>
// /*AFLA*/ #include <linux/err.h>
// /*AFLA*/ #include <linux/irqflags.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We handle most unaligned accesses in hardware.  On the other hand
// /*AFLA*/  * unaligned DMA can be quite expensive on some Nehalem processors.
// /*AFLA*/  *
// /*AFLA*/  * Based on this we disable the IP header alignment in network drivers.
// /*AFLA*/  */
// /*AFLA*/ #define NET_IP_ALIGN	0
// /*AFLA*/ 
// /*AFLA*/ #define HBP_NUM 4
// /*AFLA*/ /*
// /*AFLA*/  * Default implementation of macro that returns current
// /*AFLA*/  * instruction pointer ("program counter").
// /*AFLA*/  */
// /*AFLA*/ static inline void *current_text_addr(void)
// /*AFLA*/ {
// /*AFLA*/ 	void *pc;
// /*AFLA*/ 
// /*AFLA*/ 	asm volatile("mov $1f, %0; 1:":"=r" (pc));
// /*AFLA*/ 
// /*AFLA*/ 	return pc;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These alignment constraints are for performance in the vSMP case,
// /*AFLA*/  * but in the task_struct case we must also meet hardware imposed
// /*AFLA*/  * alignment requirements of the FPU state:
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_X86_VSMP
// /*AFLA*/ # define ARCH_MIN_TASKALIGN		(1 << INTERNODE_CACHE_SHIFT)
// /*AFLA*/ # define ARCH_MIN_MMSTRUCT_ALIGN	(1 << INTERNODE_CACHE_SHIFT)
// /*AFLA*/ #else
// /*AFLA*/ # define ARCH_MIN_TASKALIGN		__alignof__(union fpregs_state)
// /*AFLA*/ # define ARCH_MIN_MMSTRUCT_ALIGN	0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ enum tlb_infos {
// /*AFLA*/ 	ENTRIES,
// /*AFLA*/ 	NR_INFO
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern u16 __read_mostly tlb_lli_4k[NR_INFO];
// /*AFLA*/ extern u16 __read_mostly tlb_lli_2m[NR_INFO];
// /*AFLA*/ extern u16 __read_mostly tlb_lli_4m[NR_INFO];
// /*AFLA*/ extern u16 __read_mostly tlb_lld_4k[NR_INFO];
// /*AFLA*/ extern u16 __read_mostly tlb_lld_2m[NR_INFO];
// /*AFLA*/ extern u16 __read_mostly tlb_lld_4m[NR_INFO];
// /*AFLA*/ extern u16 __read_mostly tlb_lld_1g[NR_INFO];
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  *  CPU type and hardware bug flags. Kept separately for each CPU.
// /*AFLA*/  *  Members of this structure are referenced in head.S, so think twice
// /*AFLA*/  *  before touching them. [mj]
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct cpuinfo_x86 {
// /*AFLA*/ 	__u8			x86;		/* CPU family */
// /*AFLA*/ 	__u8			x86_vendor;	/* CPU vendor */
// /*AFLA*/ 	__u8			x86_model;
// /*AFLA*/ 	__u8			x86_mask;
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 	char			wp_works_ok;	/* It doesn't on 386's */
// /*AFLA*/ 
// /*AFLA*/ 	/* Problems on some 486Dx4's and old 386's: */
// /*AFLA*/ 	char			rfu;
// /*AFLA*/ 	char			pad0;
// /*AFLA*/ 	char			pad1;
// /*AFLA*/ #else
// /*AFLA*/ 	/* Number of 4K pages in DTLB/ITLB combined(in pages): */
// /*AFLA*/ 	int			x86_tlbsize;
// /*AFLA*/ #endif
// /*AFLA*/ 	__u8			x86_virt_bits;
// /*AFLA*/ 	__u8			x86_phys_bits;
// /*AFLA*/ 	/* CPUID returned core id bits: */
// /*AFLA*/ 	__u8			x86_coreid_bits;
// /*AFLA*/ 	/* Max extended CPUID function supported: */
// /*AFLA*/ 	__u32			extended_cpuid_level;
// /*AFLA*/ 	/* Maximum supported CPUID level, -1=no CPUID: */
// /*AFLA*/ 	int			cpuid_level;
// /*AFLA*/ 	__u32			x86_capability[NCAPINTS + NBUGINTS];
// /*AFLA*/ 	char			x86_vendor_id[16];
// /*AFLA*/ 	char			x86_model_id[64];
// /*AFLA*/ 	/* in KB - valid for CPUS which support this call: */
// /*AFLA*/ 	int			x86_cache_size;
// /*AFLA*/ 	int			x86_cache_alignment;	/* In bytes */
// /*AFLA*/ 	/* Cache QoS architectural values: */
// /*AFLA*/ 	int			x86_cache_max_rmid;	/* max index */
// /*AFLA*/ 	int			x86_cache_occ_scale;	/* scale to bytes */
// /*AFLA*/ 	int			x86_power;
// /*AFLA*/ 	unsigned long		loops_per_jiffy;
// /*AFLA*/ 	/* cpuid returned max cores value: */
// /*AFLA*/ 	u16			 x86_max_cores;
// /*AFLA*/ 	u16			apicid;
// /*AFLA*/ 	u16			initial_apicid;
// /*AFLA*/ 	u16			x86_clflush_size;
// /*AFLA*/ 	/* number of cores as seen by the OS: */
// /*AFLA*/ 	u16			booted_cores;
// /*AFLA*/ 	/* Physical processor id: */
// /*AFLA*/ 	u16			phys_proc_id;
// /*AFLA*/ 	/* Logical processor id: */
// /*AFLA*/ 	u16			logical_proc_id;
// /*AFLA*/ 	/* Core id: */
// /*AFLA*/ 	u16			cpu_core_id;
// /*AFLA*/ 	/* Index into per_cpu list: */
// /*AFLA*/ 	u16			cpu_index;
// /*AFLA*/ 	u32			microcode;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define X86_VENDOR_INTEL	0
// /*AFLA*/ #define X86_VENDOR_CYRIX	1
// /*AFLA*/ #define X86_VENDOR_AMD		2
// /*AFLA*/ #define X86_VENDOR_UMC		3
// /*AFLA*/ #define X86_VENDOR_CENTAUR	5
// /*AFLA*/ #define X86_VENDOR_TRANSMETA	7
// /*AFLA*/ #define X86_VENDOR_NSC		8
// /*AFLA*/ #define X86_VENDOR_NUM		9
// /*AFLA*/ 
// /*AFLA*/ #define X86_VENDOR_UNKNOWN	0xff
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * capabilities of CPUs
// /*AFLA*/  */
// /*AFLA*/ extern struct cpuinfo_x86	boot_cpu_data;
// /*AFLA*/ extern struct cpuinfo_x86	new_cpu_data;
// /*AFLA*/ 
// /*AFLA*/ extern struct tss_struct	doublefault_tss;
// /*AFLA*/ extern __u32			cpu_caps_cleared[NCAPINTS];
// /*AFLA*/ extern __u32			cpu_caps_set[NCAPINTS];
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ DECLARE_PER_CPU_READ_MOSTLY(struct cpuinfo_x86, cpu_info);
// /*AFLA*/ #define cpu_data(cpu)		per_cpu(cpu_info, cpu)
// /*AFLA*/ #else
// /*AFLA*/ #define cpu_info		boot_cpu_data
// /*AFLA*/ #define cpu_data(cpu)		boot_cpu_data
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern const struct seq_operations cpuinfo_op;
// /*AFLA*/ 
// /*AFLA*/ #define cache_line_size()	(boot_cpu_data.x86_cache_alignment)
// /*AFLA*/ 
// /*AFLA*/ extern void cpu_detect(struct cpuinfo_x86 *c);
// /*AFLA*/ 
// /*AFLA*/ extern void early_cpu_init(void);
// /*AFLA*/ extern void identify_boot_cpu(void);
// /*AFLA*/ extern void identify_secondary_cpu(struct cpuinfo_x86 *);
// /*AFLA*/ extern void print_cpu_info(struct cpuinfo_x86 *);
// /*AFLA*/ void print_cpu_msr(struct cpuinfo_x86 *);
// /*AFLA*/ extern void init_scattered_cpuid_features(struct cpuinfo_x86 *c);
// /*AFLA*/ extern unsigned int init_intel_cacheinfo(struct cpuinfo_x86 *c);
// /*AFLA*/ extern void init_amd_cacheinfo(struct cpuinfo_x86 *c);
// /*AFLA*/ 
// /*AFLA*/ extern void detect_extended_topology(struct cpuinfo_x86 *c);
// /*AFLA*/ extern void detect_ht(struct cpuinfo_x86 *c);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ extern int have_cpuid_p(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline int have_cpuid_p(void)
// /*AFLA*/ {
// /*AFLA*/ 	return 1;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ static inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
// /*AFLA*/ 				unsigned int *ecx, unsigned int *edx)
// /*AFLA*/ {
// /*AFLA*/ 	/* ecx is often an input as well as an output. */
// /*AFLA*/ 	asm volatile("cpuid"
// /*AFLA*/ 	    : "=a" (*eax),
// /*AFLA*/ 	      "=b" (*ebx),
// /*AFLA*/ 	      "=c" (*ecx),
// /*AFLA*/ 	      "=d" (*edx)
// /*AFLA*/ 	    : "0" (*eax), "2" (*ecx)
// /*AFLA*/ 	    : "memory");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void load_cr3(pgd_t *pgdir)
// /*AFLA*/ {
// /*AFLA*/ 	write_cr3(__pa(pgdir));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ /* This is the TSS defined by the hardware. */
// /*AFLA*/ struct x86_hw_tss {
// /*AFLA*/ 	unsigned short		back_link, __blh;
// /*AFLA*/ 	unsigned long		sp0;
// /*AFLA*/ 	unsigned short		ss0, __ss0h;
// /*AFLA*/ 	unsigned long		sp1;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * We don't use ring 1, so ss1 is a convenient scratch space in
// /*AFLA*/ 	 * the same cacheline as sp0.  We use ss1 to cache the value in
// /*AFLA*/ 	 * MSR_IA32_SYSENTER_CS.  When we context switch
// /*AFLA*/ 	 * MSR_IA32_SYSENTER_CS, we first check if the new value being
// /*AFLA*/ 	 * written matches ss1, and, if it's not, then we wrmsr the new
// /*AFLA*/ 	 * value and update ss1.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * The only reason we context switch MSR_IA32_SYSENTER_CS is
// /*AFLA*/ 	 * that we set it to zero in vm86 tasks to avoid corrupting the
// /*AFLA*/ 	 * stack if we were to go through the sysenter path from vm86
// /*AFLA*/ 	 * mode.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned short		ss1;	/* MSR_IA32_SYSENTER_CS */
// /*AFLA*/ 
// /*AFLA*/ 	unsigned short		__ss1h;
// /*AFLA*/ 	unsigned long		sp2;
// /*AFLA*/ 	unsigned short		ss2, __ss2h;
// /*AFLA*/ 	unsigned long		__cr3;
// /*AFLA*/ 	unsigned long		ip;
// /*AFLA*/ 	unsigned long		flags;
// /*AFLA*/ 	unsigned long		ax;
// /*AFLA*/ 	unsigned long		cx;
// /*AFLA*/ 	unsigned long		dx;
// /*AFLA*/ 	unsigned long		bx;
// /*AFLA*/ 	unsigned long		sp;
// /*AFLA*/ 	unsigned long		bp;
// /*AFLA*/ 	unsigned long		si;
// /*AFLA*/ 	unsigned long		di;
// /*AFLA*/ 	unsigned short		es, __esh;
// /*AFLA*/ 	unsigned short		cs, __csh;
// /*AFLA*/ 	unsigned short		ss, __ssh;
// /*AFLA*/ 	unsigned short		ds, __dsh;
// /*AFLA*/ 	unsigned short		fs, __fsh;
// /*AFLA*/ 	unsigned short		gs, __gsh;
// /*AFLA*/ 	unsigned short		ldt, __ldth;
// /*AFLA*/ 	unsigned short		trace;
// /*AFLA*/ 	unsigned short		io_bitmap_base;
// /*AFLA*/ 
// /*AFLA*/ } __attribute__((packed));
// /*AFLA*/ #else
// /*AFLA*/ struct x86_hw_tss {
// /*AFLA*/ 	u32			reserved1;
// /*AFLA*/ 	u64			sp0;
// /*AFLA*/ 	u64			sp1;
// /*AFLA*/ 	u64			sp2;
// /*AFLA*/ 	u64			reserved2;
// /*AFLA*/ 	u64			ist[7];
// /*AFLA*/ 	u32			reserved3;
// /*AFLA*/ 	u32			reserved4;
// /*AFLA*/ 	u16			reserved5;
// /*AFLA*/ 	u16			io_bitmap_base;
// /*AFLA*/ 
// /*AFLA*/ } __attribute__((packed)) ____cacheline_aligned;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * IO-bitmap sizes:
// /*AFLA*/  */
// /*AFLA*/ #define IO_BITMAP_BITS			65536
// /*AFLA*/ #define IO_BITMAP_BYTES			(IO_BITMAP_BITS/8)
// /*AFLA*/ #define IO_BITMAP_LONGS			(IO_BITMAP_BYTES/sizeof(long))
// /*AFLA*/ #define IO_BITMAP_OFFSET		offsetof(struct tss_struct, io_bitmap)
// /*AFLA*/ #define INVALID_IO_BITMAP_OFFSET	0x8000
// /*AFLA*/ 
// /*AFLA*/ struct tss_struct {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The hardware state:
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct x86_hw_tss	x86_tss;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The extra 1 is there because the CPU will access an
// /*AFLA*/ 	 * additional byte beyond the end of the IO permission
// /*AFLA*/ 	 * bitmap. The extra byte must be all 1 bits, and must
// /*AFLA*/ 	 * be within the limit.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long		io_bitmap[IO_BITMAP_LONGS + 1];
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Space for the temporary SYSENTER stack.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long		SYSENTER_stack_canary;
// /*AFLA*/ 	unsigned long		SYSENTER_stack[64];
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ } ____cacheline_aligned;
// /*AFLA*/ 
// /*AFLA*/ DECLARE_PER_CPU_SHARED_ALIGNED(struct tss_struct, cpu_tss);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ DECLARE_PER_CPU(unsigned long, cpu_current_top_of_stack);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Save the original ist values for checking stack pointers during debugging
// /*AFLA*/  */
// /*AFLA*/ struct orig_ist {
// /*AFLA*/ 	unsigned long		ist[7];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ DECLARE_PER_CPU(struct orig_ist, orig_ist);
// /*AFLA*/ 
// /*AFLA*/ union irq_stack_union {
// /*AFLA*/ 	char irq_stack[IRQ_STACK_SIZE];
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * GCC hardcodes the stack canary as %gs:40.  Since the
// /*AFLA*/ 	 * irq_stack is the object at %gs:0, we reserve the bottom
// /*AFLA*/ 	 * 48 bytes of the irq stack for the canary.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct {
// /*AFLA*/ 		char gs_base[40];
// /*AFLA*/ 		unsigned long stack_canary;
// /*AFLA*/ 	};
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ DECLARE_PER_CPU_FIRST(union irq_stack_union, irq_stack_union) __visible;
// /*AFLA*/ DECLARE_INIT_PER_CPU(irq_stack_union);
// /*AFLA*/ 
// /*AFLA*/ DECLARE_PER_CPU(char *, irq_stack_ptr);
// /*AFLA*/ DECLARE_PER_CPU(unsigned int, irq_count);
// /*AFLA*/ extern asmlinkage void ignore_sysret(void);
// /*AFLA*/ #else	/* X86_64 */
// /*AFLA*/ #ifdef CONFIG_CC_STACKPROTECTOR
// /*AFLA*/ /*
// /*AFLA*/  * Make sure stack canary segment base is cached-aligned:
// /*AFLA*/  *   "For Intel Atom processors, avoid non zero segment base address
// /*AFLA*/  *    that is not aligned to cache line boundary at all cost."
// /*AFLA*/  * (Optim Ref Manual Assembly/Compiler Coding Rule 15.)
// /*AFLA*/  */
// /*AFLA*/ struct stack_canary {
// /*AFLA*/ 	char __pad[20];		/* canary at %gs:20 */
// /*AFLA*/ 	unsigned long canary;
// /*AFLA*/ };
// /*AFLA*/ DECLARE_PER_CPU_ALIGNED(struct stack_canary, stack_canary);
// /*AFLA*/ #endif
// /*AFLA*/ /*
// /*AFLA*/  * per-CPU IRQ handling stacks
// /*AFLA*/  */
// /*AFLA*/ struct irq_stack {
// /*AFLA*/ 	u32                     stack[THREAD_SIZE/sizeof(u32)];
// /*AFLA*/ } __aligned(THREAD_SIZE);
// /*AFLA*/ 
// /*AFLA*/ DECLARE_PER_CPU(struct irq_stack *, hardirq_stack);
// /*AFLA*/ DECLARE_PER_CPU(struct irq_stack *, softirq_stack);
// /*AFLA*/ #endif	/* X86_64 */
// /*AFLA*/ 
// /*AFLA*/ extern unsigned int fpu_kernel_xstate_size;
// /*AFLA*/ extern unsigned int fpu_user_xstate_size;
// /*AFLA*/ 
// /*AFLA*/ struct perf_event;
// /*AFLA*/ 
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	unsigned long		seg;
// /*AFLA*/ } mm_segment_t;
// /*AFLA*/ 
// /*AFLA*/ struct thread_struct {
// /*AFLA*/ 	/* Cached TLS descriptors: */
// /*AFLA*/ 	struct desc_struct	tls_array[GDT_ENTRY_TLS_ENTRIES];
// /*AFLA*/ 	unsigned long		sp0;
// /*AFLA*/ 	unsigned long		sp;
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 	unsigned long		sysenter_cs;
// /*AFLA*/ #else
// /*AFLA*/ 	unsigned short		es;
// /*AFLA*/ 	unsigned short		ds;
// /*AFLA*/ 	unsigned short		fsindex;
// /*AFLA*/ 	unsigned short		gsindex;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	u32			status;		/* thread synchronous flags */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 	unsigned long		fsbase;
// /*AFLA*/ 	unsigned long		gsbase;
// /*AFLA*/ #else
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * XXX: this could presumably be unsigned short.  Alternatively,
// /*AFLA*/ 	 * 32-bit kernels could be taught to use fsindex instead.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long fs;
// /*AFLA*/ 	unsigned long gs;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	/* Save middle states of ptrace breakpoints */
// /*AFLA*/ 	struct perf_event	*ptrace_bps[HBP_NUM];
// /*AFLA*/ 	/* Debug status used for traps, single steps, etc... */
// /*AFLA*/ 	unsigned long           debugreg6;
// /*AFLA*/ 	/* Keep track of the exact dr7 value set by the user */
// /*AFLA*/ 	unsigned long           ptrace_dr7;
// /*AFLA*/ 	/* Fault info: */
// /*AFLA*/ 	unsigned long		cr2;
// /*AFLA*/ 	unsigned long		trap_nr;
// /*AFLA*/ 	unsigned long		error_code;
// /*AFLA*/ #ifdef CONFIG_VM86
// /*AFLA*/ 	/* Virtual 86 mode info */
// /*AFLA*/ 	struct vm86		*vm86;
// /*AFLA*/ #endif
// /*AFLA*/ 	/* IO permissions: */
// /*AFLA*/ 	unsigned long		*io_bitmap_ptr;
// /*AFLA*/ 	unsigned long		iopl;
// /*AFLA*/ 	/* Max allowed port in the bitmap, in bytes: */
// /*AFLA*/ 	unsigned		io_bitmap_max;
// /*AFLA*/ 
// /*AFLA*/ 	mm_segment_t		addr_limit;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned int		sig_on_uaccess_err:1;
// /*AFLA*/ 	unsigned int		uaccess_err:1;	/* uaccess failed */
// /*AFLA*/ 
// /*AFLA*/ 	/* Floating point and extended processor state */
// /*AFLA*/ 	struct fpu		fpu;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * WARNING: 'fpu' is dynamically-sized.  It *MUST* be at
// /*AFLA*/ 	 * the end.
// /*AFLA*/ 	 */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Thread-synchronous status.
// /*AFLA*/  *
// /*AFLA*/  * This is different from the flags in that nobody else
// /*AFLA*/  * ever touches our thread-synchronous status, so we don't
// /*AFLA*/  * have to worry about atomic accesses.
// /*AFLA*/  */
// /*AFLA*/ #define TS_COMPAT		0x0002	/* 32bit syscall active (64BIT)*/
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Set IOPL bits in EFLAGS from given mask
// /*AFLA*/  */
// /*AFLA*/ static inline void native_set_iopl_mask(unsigned mask)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 	unsigned int reg;
// /*AFLA*/ 
// /*AFLA*/ 	asm volatile ("pushfl;"
// /*AFLA*/ 		      "popl %0;"
// /*AFLA*/ 		      "andl %1, %0;"
// /*AFLA*/ 		      "orl %2, %0;"
// /*AFLA*/ 		      "pushl %0;"
// /*AFLA*/ 		      "popfl"
// /*AFLA*/ 		      : "=&r" (reg)
// /*AFLA*/ 		      : "i" (~X86_EFLAGS_IOPL), "r" (mask));
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ native_load_sp0(struct tss_struct *tss, struct thread_struct *thread)
// /*AFLA*/ {
// /*AFLA*/ 	tss->x86_tss.sp0 = thread->sp0;
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 	/* Only happens when SEP is enabled, no need to test "SEP"arately: */
// /*AFLA*/ 	if (unlikely(tss->x86_tss.ss1 != thread->sysenter_cs)) {
// /*AFLA*/ 		tss->x86_tss.ss1 = thread->sysenter_cs;
// /*AFLA*/ 		wrmsr(MSR_IA32_SYSENTER_CS, thread->sysenter_cs, 0);
// /*AFLA*/ 	}
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_swapgs(void)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 	asm volatile("swapgs" ::: "memory");
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long current_top_of_stack(void)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 	return this_cpu_read_stable(cpu_tss.x86_tss.sp0);
// /*AFLA*/ #else
// /*AFLA*/ 	/* sp0 on x86_32 is special in and around vm86 mode. */
// /*AFLA*/ 	return this_cpu_read_stable(cpu_current_top_of_stack);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PARAVIRT
// /*AFLA*/ #include <asm/paravirt.h>
// /*AFLA*/ #else
// /*AFLA*/ #define __cpuid			native_cpuid
// /*AFLA*/ 
// /*AFLA*/ static inline void load_sp0(struct tss_struct *tss,
// /*AFLA*/ 			    struct thread_struct *thread)
// /*AFLA*/ {
// /*AFLA*/ 	native_load_sp0(tss, thread);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define set_iopl_mask native_set_iopl_mask
// /*AFLA*/ #endif /* CONFIG_PARAVIRT */
// /*AFLA*/ 
// /*AFLA*/ /* Free all resources held by a thread. */
// /*AFLA*/ extern void release_thread(struct task_struct *);
// /*AFLA*/ 
// /*AFLA*/ unsigned long get_wchan(struct task_struct *p);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Generic CPUID function
// /*AFLA*/  * clear %ecx since some cpus (Cyrix MII) do not set or clear %ecx
// /*AFLA*/  * resulting in stale register contents being returned.
// /*AFLA*/  */
// /*AFLA*/ static inline void cpuid(unsigned int op,
// /*AFLA*/ 			 unsigned int *eax, unsigned int *ebx,
// /*AFLA*/ 			 unsigned int *ecx, unsigned int *edx)
// /*AFLA*/ {
// /*AFLA*/ 	*eax = op;
// /*AFLA*/ 	*ecx = 0;
// /*AFLA*/ 	__cpuid(eax, ebx, ecx, edx);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Some CPUID calls want 'count' to be placed in ecx */
// /*AFLA*/ static inline void cpuid_count(unsigned int op, int count,
// /*AFLA*/ 			       unsigned int *eax, unsigned int *ebx,
// /*AFLA*/ 			       unsigned int *ecx, unsigned int *edx)
// /*AFLA*/ {
// /*AFLA*/ 	*eax = op;
// /*AFLA*/ 	*ecx = count;
// /*AFLA*/ 	__cpuid(eax, ebx, ecx, edx);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * CPUID functions returning a single datum
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned int cpuid_eax(unsigned int op)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned int eax, ebx, ecx, edx;
// /*AFLA*/ 
// /*AFLA*/ 	cpuid(op, &eax, &ebx, &ecx, &edx);
// /*AFLA*/ 
// /*AFLA*/ 	return eax;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int cpuid_ebx(unsigned int op)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned int eax, ebx, ecx, edx;
// /*AFLA*/ 
// /*AFLA*/ 	cpuid(op, &eax, &ebx, &ecx, &edx);
// /*AFLA*/ 
// /*AFLA*/ 	return ebx;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int cpuid_ecx(unsigned int op)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned int eax, ebx, ecx, edx;
// /*AFLA*/ 
// /*AFLA*/ 	cpuid(op, &eax, &ebx, &ecx, &edx);
// /*AFLA*/ 
// /*AFLA*/ 	return ecx;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int cpuid_edx(unsigned int op)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned int eax, ebx, ecx, edx;
// /*AFLA*/ 
// /*AFLA*/ 	cpuid(op, &eax, &ebx, &ecx, &edx);
// /*AFLA*/ 
// /*AFLA*/ 	return edx;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* REP NOP (PAUSE) is a good thing to insert into busy-wait loops. */
// /*AFLA*/ static __always_inline void rep_nop(void)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("rep; nop" ::: "memory");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void cpu_relax(void)
// /*AFLA*/ {
// /*AFLA*/ 	rep_nop();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define cpu_relax_lowlatency() cpu_relax()
// /*AFLA*/ 
// /*AFLA*/ /* Stop speculative execution and prefetching of modified code. */
// /*AFLA*/ static inline void sync_core(void)
// /*AFLA*/ {
// /*AFLA*/ 	int tmp;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_M486
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Do a CPUID if available, otherwise do a jump.  The jump
// /*AFLA*/ 	 * can conveniently enough be the jump around CPUID.
// /*AFLA*/ 	 */
// /*AFLA*/ 	asm volatile("cmpl %2,%1\n\t"
// /*AFLA*/ 		     "jl 1f\n\t"
// /*AFLA*/ 		     "cpuid\n"
// /*AFLA*/ 		     "1:"
// /*AFLA*/ 		     : "=a" (tmp)
// /*AFLA*/ 		     : "rm" (boot_cpu_data.cpuid_level), "ri" (0), "0" (1)
// /*AFLA*/ 		     : "ebx", "ecx", "edx", "memory");
// /*AFLA*/ #else
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * CPUID is a barrier to speculative execution.
// /*AFLA*/ 	 * Prefetched instructions are automatically
// /*AFLA*/ 	 * invalidated when modified.
// /*AFLA*/ 	 */
// /*AFLA*/ 	asm volatile("cpuid"
// /*AFLA*/ 		     : "=a" (tmp)
// /*AFLA*/ 		     : "0" (1)
// /*AFLA*/ 		     : "ebx", "ecx", "edx", "memory");
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void select_idle_routine(const struct cpuinfo_x86 *c);
// /*AFLA*/ extern void init_amd_e400_c1e_mask(void);
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long		boot_option_idle_override;
// /*AFLA*/ extern bool			amd_e400_c1e_detected;
// /*AFLA*/ 
// /*AFLA*/ enum idle_boot_override {IDLE_NO_OVERRIDE=0, IDLE_HALT, IDLE_NOMWAIT,
// /*AFLA*/ 			 IDLE_POLL};
// /*AFLA*/ 
// /*AFLA*/ extern void enable_sep_cpu(void);
// /*AFLA*/ extern int sysenter_setup(void);
// /*AFLA*/ 
// /*AFLA*/ extern void early_trap_init(void);
// /*AFLA*/ void early_trap_pf_init(void);
// /*AFLA*/ 
// /*AFLA*/ /* Defined in head.S */
// /*AFLA*/ extern struct desc_ptr		early_gdt_descr;
// /*AFLA*/ 
// /*AFLA*/ extern void cpu_set_gdt(int);
// /*AFLA*/ extern void switch_to_new_gdt(int);
// /*AFLA*/ extern void load_percpu_segment(int);
// /*AFLA*/ extern void cpu_init(void);
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long get_debugctlmsr(void)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long debugctlmsr = 0;
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_X86_DEBUGCTLMSR
// /*AFLA*/ 	if (boot_cpu_data.x86 < 6)
// /*AFLA*/ 		return 0;
// /*AFLA*/ #endif
// /*AFLA*/ 	rdmsrl(MSR_IA32_DEBUGCTLMSR, debugctlmsr);
// /*AFLA*/ 
// /*AFLA*/ 	return debugctlmsr;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void update_debugctlmsr(unsigned long debugctlmsr)
// /*AFLA*/ {
// /*AFLA*/ #ifndef CONFIG_X86_DEBUGCTLMSR
// /*AFLA*/ 	if (boot_cpu_data.x86 < 6)
// /*AFLA*/ 		return;
// /*AFLA*/ #endif
// /*AFLA*/ 	wrmsrl(MSR_IA32_DEBUGCTLMSR, debugctlmsr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void set_task_blockstep(struct task_struct *task, bool on);
// /*AFLA*/ 
// /*AFLA*/ /* Boot loader type from the setup header: */
// /*AFLA*/ extern int			bootloader_type;
// /*AFLA*/ extern int			bootloader_version;
// /*AFLA*/ 
// /*AFLA*/ extern char			ignore_fpu_irq;
// /*AFLA*/ 
// /*AFLA*/ #define HAVE_ARCH_PICK_MMAP_LAYOUT 1
// /*AFLA*/ #define ARCH_HAS_PREFETCHW
// /*AFLA*/ #define ARCH_HAS_SPINLOCK_PREFETCH
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ # define BASE_PREFETCH		""
// /*AFLA*/ # define ARCH_HAS_PREFETCH
// /*AFLA*/ #else
// /*AFLA*/ # define BASE_PREFETCH		"prefetcht0 %P1"
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Prefetch instructions for Pentium III (+) and AMD Athlon (+)
// /*AFLA*/  *
// /*AFLA*/  * It's not worth to care about 3dnow prefetches for the K6
// /*AFLA*/  * because they are microcoded there and very slow.
// /*AFLA*/  */
// /*AFLA*/ static inline void prefetch(const void *x)
// /*AFLA*/ {
// /*AFLA*/ 	alternative_input(BASE_PREFETCH, "prefetchnta %P1",
// /*AFLA*/ 			  X86_FEATURE_XMM,
// /*AFLA*/ 			  "m" (*(const char *)x));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * 3dnow prefetch to get an exclusive cache line.
// /*AFLA*/  * Useful for spinlocks to avoid one state transition in the
// /*AFLA*/  * cache coherency protocol:
// /*AFLA*/  */
// /*AFLA*/ static inline void prefetchw(const void *x)
// /*AFLA*/ {
// /*AFLA*/ 	alternative_input(BASE_PREFETCH, "prefetchw %P1",
// /*AFLA*/ 			  X86_FEATURE_3DNOWPREFETCH,
// /*AFLA*/ 			  "m" (*(const char *)x));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void spin_lock_prefetch(const void *x)
// /*AFLA*/ {
// /*AFLA*/ 	prefetchw(x);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define TOP_OF_INIT_STACK ((unsigned long)&init_stack + sizeof(init_stack) - \
// /*AFLA*/ 			   TOP_OF_KERNEL_STACK_PADDING)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ /*
// /*AFLA*/  * User space process size: 3GB (default).
// /*AFLA*/  */
// /*AFLA*/ #define TASK_SIZE		PAGE_OFFSET
// /*AFLA*/ #define TASK_SIZE_MAX		TASK_SIZE
// /*AFLA*/ #define STACK_TOP		TASK_SIZE
// /*AFLA*/ #define STACK_TOP_MAX		STACK_TOP
// /*AFLA*/ 
// /*AFLA*/ #define INIT_THREAD  {							  \
// /*AFLA*/ 	.sp0			= TOP_OF_INIT_STACK,			  \
// /*AFLA*/ 	.sysenter_cs		= __KERNEL_CS,				  \
// /*AFLA*/ 	.io_bitmap_ptr		= NULL,					  \
// /*AFLA*/ 	.addr_limit		= KERNEL_DS,				  \
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * TOP_OF_KERNEL_STACK_PADDING reserves 8 bytes on top of the ring0 stack.
// /*AFLA*/  * This is necessary to guarantee that the entire "struct pt_regs"
// /*AFLA*/  * is accessible even if the CPU haven't stored the SS/ESP registers
// /*AFLA*/  * on the stack (interrupt gate does not save these registers
// /*AFLA*/  * when switching to the same priv ring).
// /*AFLA*/  * Therefore beware: accessing the ss/esp fields of the
// /*AFLA*/  * "struct pt_regs" is possible, but they may contain the
// /*AFLA*/  * completely wrong values.
// /*AFLA*/  */
// /*AFLA*/ #define task_pt_regs(task) \
// /*AFLA*/ ({									\
// /*AFLA*/ 	unsigned long __ptr = (unsigned long)task_stack_page(task);	\
// /*AFLA*/ 	__ptr += THREAD_SIZE - TOP_OF_KERNEL_STACK_PADDING;		\
// /*AFLA*/ 	((struct pt_regs *)__ptr) - 1;					\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define KSTK_ESP(task)		(task_pt_regs(task)->sp)
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ /*
// /*AFLA*/  * User space process size. 47bits minus one guard page.  The guard
// /*AFLA*/  * page is necessary on Intel CPUs: if a SYSCALL instruction is at
// /*AFLA*/  * the highest possible canonical userspace address, then that
// /*AFLA*/  * syscall will enter the kernel with a non-canonical return
// /*AFLA*/  * address, and SYSRET will explode dangerously.  We avoid this
// /*AFLA*/  * particular problem by preventing anything from being mapped
// /*AFLA*/  * at the maximum canonical address.
// /*AFLA*/  */
// /*AFLA*/ #define TASK_SIZE_MAX	((1UL << 47) - PAGE_SIZE)
// /*AFLA*/ 
// /*AFLA*/ /* This decides where the kernel will search for a free chunk of vm
// /*AFLA*/  * space during mmap's.
// /*AFLA*/  */
// /*AFLA*/ #define IA32_PAGE_OFFSET	((current->personality & ADDR_LIMIT_3GB) ? \
// /*AFLA*/ 					0xc0000000 : 0xFFFFe000)
// /*AFLA*/ 
// /*AFLA*/ #define TASK_SIZE		(test_thread_flag(TIF_ADDR32) ? \
// /*AFLA*/ 					IA32_PAGE_OFFSET : TASK_SIZE_MAX)
// /*AFLA*/ #define TASK_SIZE_OF(child)	((test_tsk_thread_flag(child, TIF_ADDR32)) ? \
// /*AFLA*/ 					IA32_PAGE_OFFSET : TASK_SIZE_MAX)
// /*AFLA*/ 
// /*AFLA*/ #define STACK_TOP		TASK_SIZE
// /*AFLA*/ #define STACK_TOP_MAX		TASK_SIZE_MAX
// /*AFLA*/ 
// /*AFLA*/ #define INIT_THREAD  {						\
// /*AFLA*/ 	.sp0			= TOP_OF_INIT_STACK,		\
// /*AFLA*/ 	.addr_limit		= KERNEL_DS,			\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define task_pt_regs(tsk)	((struct pt_regs *)(tsk)->thread.sp0 - 1)
// /*AFLA*/ extern unsigned long KSTK_ESP(struct task_struct *task);
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_X86_64 */
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long thread_saved_pc(struct task_struct *tsk);
// /*AFLA*/ 
// /*AFLA*/ extern void start_thread(struct pt_regs *regs, unsigned long new_ip,
// /*AFLA*/ 					       unsigned long new_sp);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This decides where the kernel will search for a free chunk of vm
// /*AFLA*/  * space during mmap's.
// /*AFLA*/  */
// /*AFLA*/ #define TASK_UNMAPPED_BASE	(PAGE_ALIGN(TASK_SIZE / 3))
// /*AFLA*/ 
// /*AFLA*/ #define KSTK_EIP(task)		(task_pt_regs(task)->ip)
// /*AFLA*/ 
// /*AFLA*/ /* Get/set a process' ability to use the timestamp counter instruction */
// /*AFLA*/ #define GET_TSC_CTL(adr)	get_tsc_mode((adr))
// /*AFLA*/ #define SET_TSC_CTL(val)	set_tsc_mode((val))
// /*AFLA*/ 
// /*AFLA*/ extern int get_tsc_mode(unsigned long adr);
// /*AFLA*/ extern int set_tsc_mode(unsigned int val);
// /*AFLA*/ 
// /*AFLA*/ /* Register/unregister a process' MPX related resource */
// /*AFLA*/ #define MPX_ENABLE_MANAGEMENT()	mpx_enable_management()
// /*AFLA*/ #define MPX_DISABLE_MANAGEMENT()	mpx_disable_management()
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_INTEL_MPX
// /*AFLA*/ extern int mpx_enable_management(void);
// /*AFLA*/ extern int mpx_disable_management(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline int mpx_enable_management(void)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ static inline int mpx_disable_management(void)
// /*AFLA*/ {
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_X86_INTEL_MPX */
// /*AFLA*/ 
// /*AFLA*/ extern u16 amd_get_nb_id(int cpu);
// /*AFLA*/ extern u32 amd_get_nodes_per_socket(void);
// /*AFLA*/ 
// /*AFLA*/ static inline uint32_t hypervisor_cpuid_base(const char *sig, uint32_t leaves)
// /*AFLA*/ {
// /*AFLA*/ 	uint32_t base, eax, signature[3];
// /*AFLA*/ 
// /*AFLA*/ 	for (base = 0x40000000; base < 0x40010000; base += 0x100) {
// /*AFLA*/ 		cpuid(base, &eax, &signature[0], &signature[1], &signature[2]);
// /*AFLA*/ 
// /*AFLA*/ 		if (!memcmp(sig, signature, 12) &&
// /*AFLA*/ 		    (leaves == 0 || ((eax - base) >= leaves)))
// /*AFLA*/ 			return base;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long arch_align_stack(unsigned long sp);
// /*AFLA*/ extern void free_init_pages(char *what, unsigned long begin, unsigned long end);
// /*AFLA*/ 
// /*AFLA*/ void default_idle(void);
// /*AFLA*/ #ifdef	CONFIG_XEN
// /*AFLA*/ bool xen_set_default_idle(void);
// /*AFLA*/ #else
// /*AFLA*/ #define xen_set_default_idle 0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ void stop_this_cpu(void *dummy);
// /*AFLA*/ void df_debug(struct pt_regs *regs, long error_code);
// /*AFLA*/ #endif /* _ASM_X86_PROCESSOR_H */
