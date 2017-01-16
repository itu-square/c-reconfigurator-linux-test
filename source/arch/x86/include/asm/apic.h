// /*AFLA*/ #ifndef _ASM_X86_APIC_H
// /*AFLA*/ #define _ASM_X86_APIC_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ #include <linux/pm.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/alternative.h>
// /*AFLA*/ #include <asm/cpufeature.h>
// /*AFLA*/ #include <asm/apicdef.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <asm/fixmap.h>
// /*AFLA*/ #include <asm/mpspec.h>
// /*AFLA*/ #include <asm/msr.h>
// /*AFLA*/ #include <asm/idle.h>
// /*AFLA*/ 
// /*AFLA*/ #define ARCH_APICTIMER_STOPS_ON_C3	1
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Debugging macros
// /*AFLA*/  */
// /*AFLA*/ #define APIC_QUIET   0
// /*AFLA*/ #define APIC_VERBOSE 1
// /*AFLA*/ #define APIC_DEBUG   2
// /*AFLA*/ 
// /*AFLA*/ /* Macros for apic_extnmi which controls external NMI masking */
// /*AFLA*/ #define APIC_EXTNMI_BSP		0 /* Default */
// /*AFLA*/ #define APIC_EXTNMI_ALL		1
// /*AFLA*/ #define APIC_EXTNMI_NONE	2
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Define the default level of output to be very little
// /*AFLA*/  * This can be turned up by using apic=verbose for more
// /*AFLA*/  * information and apic=debug for _lots_ of information.
// /*AFLA*/  * apic_verbosity is defined in apic.c
// /*AFLA*/  */
// /*AFLA*/ #define apic_printk(v, s, a...) do {       \
// /*AFLA*/ 		if ((v) <= apic_verbosity) \
// /*AFLA*/ 			printk(s, ##a);    \
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_X86_LOCAL_APIC) && defined(CONFIG_X86_32)
// /*AFLA*/ extern void generic_apic_probe(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void generic_apic_probe(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_LOCAL_APIC
// /*AFLA*/ 
// /*AFLA*/ extern unsigned int apic_verbosity;
// /*AFLA*/ extern int local_apic_timer_c2_ok;
// /*AFLA*/ 
// /*AFLA*/ extern int disable_apic;
// /*AFLA*/ extern unsigned int lapic_timer_frequency;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ extern void __inquire_remote_apic(int apicid);
// /*AFLA*/ #else /* CONFIG_SMP */
// /*AFLA*/ static inline void __inquire_remote_apic(int apicid)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ static inline void default_inquire_remote_apic(int apicid)
// /*AFLA*/ {
// /*AFLA*/ 	if (apic_verbosity >= APIC_DEBUG)
// /*AFLA*/ 		__inquire_remote_apic(apicid);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * With 82489DX we can't rely on apic feature bit
// /*AFLA*/  * retrieved via cpuid but still have to deal with
// /*AFLA*/  * such an apic chip so we assume that SMP configuration
// /*AFLA*/  * is found from MP table (64bit case uses ACPI mostly
// /*AFLA*/  * which set smp presence flag as well so we are safe
// /*AFLA*/  * to use this helper too).
// /*AFLA*/  */
// /*AFLA*/ static inline bool apic_from_smp_config(void)
// /*AFLA*/ {
// /*AFLA*/ 	return smp_found_config && !disable_apic;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Basic functions accessing APICs.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_PARAVIRT
// /*AFLA*/ #include <asm/paravirt.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern int setup_profiling_timer(unsigned int);
// /*AFLA*/ 
// /*AFLA*/ static inline void native_apic_mem_write(u32 reg, u32 v)
// /*AFLA*/ {
// /*AFLA*/ 	volatile u32 *addr = (volatile u32 *)(APIC_BASE + reg);
// /*AFLA*/ 
// /*AFLA*/ 	alternative_io("movl %0, %P1", "xchgl %0, %P1", X86_BUG_11AP,
// /*AFLA*/ 		       ASM_OUTPUT2("=r" (v), "=m" (*addr)),
// /*AFLA*/ 		       ASM_OUTPUT2("0" (v), "m" (*addr)));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u32 native_apic_mem_read(u32 reg)
// /*AFLA*/ {
// /*AFLA*/ 	return *((volatile u32 *)(APIC_BASE + reg));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void native_apic_wait_icr_idle(void);
// /*AFLA*/ extern u32 native_safe_apic_wait_icr_idle(void);
// /*AFLA*/ extern void native_apic_icr_write(u32 low, u32 id);
// /*AFLA*/ extern u64 native_apic_icr_read(void);
// /*AFLA*/ 
// /*AFLA*/ static inline bool apic_is_x2apic_enabled(void)
// /*AFLA*/ {
// /*AFLA*/ 	u64 msr;
// /*AFLA*/ 
// /*AFLA*/ 	if (rdmsrl_safe(MSR_IA32_APICBASE, &msr))
// /*AFLA*/ 		return false;
// /*AFLA*/ 	return msr & X2APIC_ENABLE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void enable_IR_x2apic(void);
// /*AFLA*/ 
// /*AFLA*/ extern int get_physical_broadcast(void);
// /*AFLA*/ 
// /*AFLA*/ extern int lapic_get_maxlvt(void);
// /*AFLA*/ extern void clear_local_APIC(void);
// /*AFLA*/ extern void disconnect_bsp_APIC(int virt_wire_setup);
// /*AFLA*/ extern void disable_local_APIC(void);
// /*AFLA*/ extern void lapic_shutdown(void);
// /*AFLA*/ extern void sync_Arb_IDs(void);
// /*AFLA*/ extern void init_bsp_APIC(void);
// /*AFLA*/ extern void setup_local_APIC(void);
// /*AFLA*/ extern void init_apic_mappings(void);
// /*AFLA*/ void register_lapic_address(unsigned long address);
// /*AFLA*/ extern void setup_boot_APIC_clock(void);
// /*AFLA*/ extern void setup_secondary_APIC_clock(void);
// /*AFLA*/ extern void lapic_update_tsc_freq(void);
// /*AFLA*/ extern int APIC_init_uniprocessor(void);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ static inline int apic_force_enable(unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	return -1;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ extern int apic_force_enable(unsigned long addr);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern int apic_bsp_setup(bool upmode);
// /*AFLA*/ extern void apic_ap_setup(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * On 32bit this is mach-xxx local
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ extern int apic_is_clustered_box(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline int apic_is_clustered_box(void)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern int setup_APIC_eilvt(u8 lvt_off, u8 vector, u8 msg_type, u8 mask);
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_X86_LOCAL_APIC */
// /*AFLA*/ static inline void lapic_shutdown(void) { }
// /*AFLA*/ #define local_apic_timer_c2_ok		1
// /*AFLA*/ static inline void init_apic_mappings(void) { }
// /*AFLA*/ static inline void disable_local_APIC(void) { }
// /*AFLA*/ # define setup_boot_APIC_clock x86_init_noop
// /*AFLA*/ # define setup_secondary_APIC_clock x86_init_noop
// /*AFLA*/ static inline void lapic_update_tsc_freq(void) { }
// /*AFLA*/ #endif /* !CONFIG_X86_LOCAL_APIC */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_X2APIC
// /*AFLA*/ /*
// /*AFLA*/  * Make previous memory operations globally visible before
// /*AFLA*/  * sending the IPI through x2apic wrmsr. We need a serializing instruction or
// /*AFLA*/  * mfence for this.
// /*AFLA*/  */
// /*AFLA*/ static inline void x2apic_wrmsr_fence(void)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("mfence" : : : "memory");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_apic_msr_write(u32 reg, u32 v)
// /*AFLA*/ {
// /*AFLA*/ 	if (reg == APIC_DFR || reg == APIC_ID || reg == APIC_LDR ||
// /*AFLA*/ 	    reg == APIC_LVR)
// /*AFLA*/ 		return;
// /*AFLA*/ 
// /*AFLA*/ 	wrmsr(APIC_BASE_MSR + (reg >> 4), v, 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_apic_msr_eoi_write(u32 reg, u32 v)
// /*AFLA*/ {
// /*AFLA*/ 	wrmsr(APIC_BASE_MSR + (APIC_EOI >> 4), APIC_EOI_ACK, 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u32 native_apic_msr_read(u32 reg)
// /*AFLA*/ {
// /*AFLA*/ 	u64 msr;
// /*AFLA*/ 
// /*AFLA*/ 	if (reg == APIC_DFR)
// /*AFLA*/ 		return -1;
// /*AFLA*/ 
// /*AFLA*/ 	rdmsrl(APIC_BASE_MSR + (reg >> 4), msr);
// /*AFLA*/ 	return (u32)msr;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_x2apic_wait_icr_idle(void)
// /*AFLA*/ {
// /*AFLA*/ 	/* no need to wait for icr idle in x2apic */
// /*AFLA*/ 	return;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u32 native_safe_x2apic_wait_icr_idle(void)
// /*AFLA*/ {
// /*AFLA*/ 	/* no need to wait for icr idle in x2apic */
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_x2apic_icr_write(u32 low, u32 id)
// /*AFLA*/ {
// /*AFLA*/ 	wrmsrl(APIC_BASE_MSR + (APIC_ICR >> 4), ((__u64) id) << 32 | low);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u64 native_x2apic_icr_read(void)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long val;
// /*AFLA*/ 
// /*AFLA*/ 	rdmsrl(APIC_BASE_MSR + (APIC_ICR >> 4), val);
// /*AFLA*/ 	return val;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int x2apic_mode;
// /*AFLA*/ extern int x2apic_phys;
// /*AFLA*/ extern void __init check_x2apic(void);
// /*AFLA*/ extern void x2apic_setup(void);
// /*AFLA*/ static inline int x2apic_enabled(void)
// /*AFLA*/ {
// /*AFLA*/ 	return boot_cpu_has(X86_FEATURE_X2APIC) && apic_is_x2apic_enabled();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define x2apic_supported()	(boot_cpu_has(X86_FEATURE_X2APIC))
// /*AFLA*/ #else /* !CONFIG_X86_X2APIC */
// /*AFLA*/ static inline void check_x2apic(void) { }
// /*AFLA*/ static inline void x2apic_setup(void) { }
// /*AFLA*/ static inline int x2apic_enabled(void) { return 0; }
// /*AFLA*/ 
// /*AFLA*/ #define x2apic_mode		(0)
// /*AFLA*/ #define	x2apic_supported()	(0)
// /*AFLA*/ #endif /* !CONFIG_X86_X2APIC */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ #define	SET_APIC_ID(x)		(apic->set_apic_id(x))
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Copyright 2004 James Cleverdon, IBM.
// /*AFLA*/  * Subject to the GNU Public License, v.2
// /*AFLA*/  *
// /*AFLA*/  * Generic APIC sub-arch data struct.
// /*AFLA*/  *
// /*AFLA*/  * Hacked for x86-64 by James Cleverdon from i386 architecture code by
// /*AFLA*/  * Martin Bligh, Andi Kleen, James Bottomley, John Stultz, and
// /*AFLA*/  * James Cleverdon.
// /*AFLA*/  */
// /*AFLA*/ struct apic {
// /*AFLA*/ 	char *name;
// /*AFLA*/ 
// /*AFLA*/ 	int (*probe)(void);
// /*AFLA*/ 	int (*acpi_madt_oem_check)(char *oem_id, char *oem_table_id);
// /*AFLA*/ 	int (*apic_id_valid)(int apicid);
// /*AFLA*/ 	int (*apic_id_registered)(void);
// /*AFLA*/ 
// /*AFLA*/ 	u32 irq_delivery_mode;
// /*AFLA*/ 	u32 irq_dest_mode;
// /*AFLA*/ 
// /*AFLA*/ 	const struct cpumask *(*target_cpus)(void);
// /*AFLA*/ 
// /*AFLA*/ 	int disable_esr;
// /*AFLA*/ 
// /*AFLA*/ 	int dest_logical;
// /*AFLA*/ 	unsigned long (*check_apicid_used)(physid_mask_t *map, int apicid);
// /*AFLA*/ 
// /*AFLA*/ 	void (*vector_allocation_domain)(int cpu, struct cpumask *retmask,
// /*AFLA*/ 					 const struct cpumask *mask);
// /*AFLA*/ 	void (*init_apic_ldr)(void);
// /*AFLA*/ 
// /*AFLA*/ 	void (*ioapic_phys_id_map)(physid_mask_t *phys_map, physid_mask_t *retmap);
// /*AFLA*/ 
// /*AFLA*/ 	void (*setup_apic_routing)(void);
// /*AFLA*/ 	int (*cpu_present_to_apicid)(int mps_cpu);
// /*AFLA*/ 	void (*apicid_to_cpu_present)(int phys_apicid, physid_mask_t *retmap);
// /*AFLA*/ 	int (*check_phys_apicid_present)(int phys_apicid);
// /*AFLA*/ 	int (*phys_pkg_id)(int cpuid_apic, int index_msb);
// /*AFLA*/ 
// /*AFLA*/ 	unsigned int (*get_apic_id)(unsigned long x);
// /*AFLA*/ 	unsigned long (*set_apic_id)(unsigned int id);
// /*AFLA*/ 
// /*AFLA*/ 	int (*cpu_mask_to_apicid_and)(const struct cpumask *cpumask,
// /*AFLA*/ 				      const struct cpumask *andmask,
// /*AFLA*/ 				      unsigned int *apicid);
// /*AFLA*/ 
// /*AFLA*/ 	/* ipi */
// /*AFLA*/ 	void (*send_IPI)(int cpu, int vector);
// /*AFLA*/ 	void (*send_IPI_mask)(const struct cpumask *mask, int vector);
// /*AFLA*/ 	void (*send_IPI_mask_allbutself)(const struct cpumask *mask,
// /*AFLA*/ 					 int vector);
// /*AFLA*/ 	void (*send_IPI_allbutself)(int vector);
// /*AFLA*/ 	void (*send_IPI_all)(int vector);
// /*AFLA*/ 	void (*send_IPI_self)(int vector);
// /*AFLA*/ 
// /*AFLA*/ 	/* wakeup_secondary_cpu */
// /*AFLA*/ 	int (*wakeup_secondary_cpu)(int apicid, unsigned long start_eip);
// /*AFLA*/ 
// /*AFLA*/ 	void (*inquire_remote_apic)(int apicid);
// /*AFLA*/ 
// /*AFLA*/ 	/* apic ops */
// /*AFLA*/ 	u32 (*read)(u32 reg);
// /*AFLA*/ 	void (*write)(u32 reg, u32 v);
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * ->eoi_write() has the same signature as ->write().
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Drivers can support both ->eoi_write() and ->write() by passing the same
// /*AFLA*/ 	 * callback value. Kernel can override ->eoi_write() and fall back
// /*AFLA*/ 	 * on write for EOI.
// /*AFLA*/ 	 */
// /*AFLA*/ 	void (*eoi_write)(u32 reg, u32 v);
// /*AFLA*/ 	u64 (*icr_read)(void);
// /*AFLA*/ 	void (*icr_write)(u32 low, u32 high);
// /*AFLA*/ 	void (*wait_icr_idle)(void);
// /*AFLA*/ 	u32 (*safe_wait_icr_idle)(void);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Called very early during boot from get_smp_config().  It should
// /*AFLA*/ 	 * return the logical apicid.  x86_[bios]_cpu_to_apicid is
// /*AFLA*/ 	 * initialized before this function is called.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * If logical apicid can't be determined that early, the function
// /*AFLA*/ 	 * may return BAD_APICID.  Logical apicid will be configured after
// /*AFLA*/ 	 * init_apic_ldr() while bringing up CPUs.  Note that NUMA affinity
// /*AFLA*/ 	 * won't be applied properly during early boot in this case.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int (*x86_32_early_logical_apicid)(int cpu);
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Pointer to the local APIC driver in use on this system (there's
// /*AFLA*/  * always just one such driver in use - the kernel decides via an
// /*AFLA*/  * early probing process which one it picks - and then sticks to it):
// /*AFLA*/  */
// /*AFLA*/ extern struct apic *apic;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * APIC drivers are probed based on how they are listed in the .apicdrivers
// /*AFLA*/  * section. So the order is important and enforced by the ordering
// /*AFLA*/  * of different apic driver files in the Makefile.
// /*AFLA*/  *
// /*AFLA*/  * For the files having two apic drivers, we use apic_drivers()
// /*AFLA*/  * to enforce the order with in them.
// /*AFLA*/  */
// /*AFLA*/ #define apic_driver(sym)					\
// /*AFLA*/ 	static const struct apic *__apicdrivers_##sym __used		\
// /*AFLA*/ 	__aligned(sizeof(struct apic *))			\
// /*AFLA*/ 	__section(.apicdrivers) = { &sym }
// /*AFLA*/ 
// /*AFLA*/ #define apic_drivers(sym1, sym2)					\
// /*AFLA*/ 	static struct apic *__apicdrivers_##sym1##sym2[2] __used	\
// /*AFLA*/ 	__aligned(sizeof(struct apic *))				\
// /*AFLA*/ 	__section(.apicdrivers) = { &sym1, &sym2 }
// /*AFLA*/ 
// /*AFLA*/ extern struct apic *__apicdrivers[], *__apicdrivers_end[];
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * APIC functionality to boot other CPUs - only used on SMP:
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ extern int wakeup_secondary_cpu_via_nmi(int apicid, unsigned long start_eip);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_LOCAL_APIC
// /*AFLA*/ 
// /*AFLA*/ static inline u32 apic_read(u32 reg)
// /*AFLA*/ {
// /*AFLA*/ 	return apic->read(reg);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void apic_write(u32 reg, u32 val)
// /*AFLA*/ {
// /*AFLA*/ 	apic->write(reg, val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void apic_eoi(void)
// /*AFLA*/ {
// /*AFLA*/ 	apic->eoi_write(APIC_EOI, APIC_EOI_ACK);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u64 apic_icr_read(void)
// /*AFLA*/ {
// /*AFLA*/ 	return apic->icr_read();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void apic_icr_write(u32 low, u32 high)
// /*AFLA*/ {
// /*AFLA*/ 	apic->icr_write(low, high);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void apic_wait_icr_idle(void)
// /*AFLA*/ {
// /*AFLA*/ 	apic->wait_icr_idle();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u32 safe_apic_wait_icr_idle(void)
// /*AFLA*/ {
// /*AFLA*/ 	return apic->safe_wait_icr_idle();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void __init apic_set_eoi_write(void (*eoi_write)(u32 reg, u32 v));
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_X86_LOCAL_APIC */
// /*AFLA*/ 
// /*AFLA*/ static inline u32 apic_read(u32 reg) { return 0; }
// /*AFLA*/ static inline void apic_write(u32 reg, u32 val) { }
// /*AFLA*/ static inline void apic_eoi(void) { }
// /*AFLA*/ static inline u64 apic_icr_read(void) { return 0; }
// /*AFLA*/ static inline void apic_icr_write(u32 low, u32 high) { }
// /*AFLA*/ static inline void apic_wait_icr_idle(void) { }
// /*AFLA*/ static inline u32 safe_apic_wait_icr_idle(void) { return 0; }
// /*AFLA*/ static inline void apic_set_eoi_write(void (*eoi_write)(u32 reg, u32 v)) {}
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_X86_LOCAL_APIC */
// /*AFLA*/ 
// /*AFLA*/ static inline void ack_APIC_irq(void)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * ack_APIC_irq() actually gets compiled as a single instruction
// /*AFLA*/ 	 * ... yummie.
// /*AFLA*/ 	 */
// /*AFLA*/ 	apic_eoi();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned default_get_apic_id(unsigned long x)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned int ver = GET_APIC_VERSION(apic_read(APIC_LVR));
// /*AFLA*/ 
// /*AFLA*/ 	if (APIC_XAPIC(ver) || boot_cpu_has(X86_FEATURE_EXTD_APICID))
// /*AFLA*/ 		return (x >> 24) & 0xFF;
// /*AFLA*/ 	else
// /*AFLA*/ 		return (x >> 24) & 0x0F;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Warm reset vector position:
// /*AFLA*/  */
// /*AFLA*/ #define TRAMPOLINE_PHYS_LOW		0x467
// /*AFLA*/ #define TRAMPOLINE_PHYS_HIGH		0x469
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ extern void apic_send_IPI_self(int vector);
// /*AFLA*/ 
// /*AFLA*/ DECLARE_PER_CPU(int, x2apic_extra_bits);
// /*AFLA*/ 
// /*AFLA*/ extern int default_cpu_present_to_apicid(int mps_cpu);
// /*AFLA*/ extern int default_check_phys_apicid_present(int phys_apicid);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void generic_bigsmp_probe(void);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_LOCAL_APIC
// /*AFLA*/ 
// /*AFLA*/ #include <asm/smp.h>
// /*AFLA*/ 
// /*AFLA*/ #define APIC_DFR_VALUE	(APIC_DFR_FLAT)
// /*AFLA*/ 
// /*AFLA*/ static inline const struct cpumask *default_target_cpus(void)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ 	return cpu_online_mask;
// /*AFLA*/ #else
// /*AFLA*/ 	return cpumask_of(0);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline const struct cpumask *online_target_cpus(void)
// /*AFLA*/ {
// /*AFLA*/ 	return cpu_online_mask;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ DECLARE_EARLY_PER_CPU_READ_MOSTLY(u16, x86_bios_cpu_apicid);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int read_apic_id(void)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned int reg;
// /*AFLA*/ 
// /*AFLA*/ 	reg = apic_read(APIC_ID);
// /*AFLA*/ 
// /*AFLA*/ 	return apic->get_apic_id(reg);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int default_apic_id_valid(int apicid)
// /*AFLA*/ {
// /*AFLA*/ 	return (apicid < 255);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int default_acpi_madt_oem_check(char *, char *);
// /*AFLA*/ 
// /*AFLA*/ extern void default_setup_apic_routing(void);
// /*AFLA*/ 
// /*AFLA*/ extern struct apic apic_noop;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 
// /*AFLA*/ static inline int noop_x86_32_early_logical_apicid(int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return BAD_APICID;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Set up the logical destination ID.
// /*AFLA*/  *
// /*AFLA*/  * Intel recommends to set DFR, LDR and TPR before enabling
// /*AFLA*/  * an APIC.  See e.g. "AP-388 82489DX User's Manual" (Intel
// /*AFLA*/  * document number 292116).  So here it goes...
// /*AFLA*/  */
// /*AFLA*/ extern void default_init_apic_ldr(void);
// /*AFLA*/ 
// /*AFLA*/ static inline int default_apic_id_registered(void)
// /*AFLA*/ {
// /*AFLA*/ 	return physid_isset(read_apic_id(), phys_cpu_present_map);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int default_phys_pkg_id(int cpuid_apic, int index_msb)
// /*AFLA*/ {
// /*AFLA*/ 	return cpuid_apic >> index_msb;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline int
// /*AFLA*/ flat_cpu_mask_to_apicid_and(const struct cpumask *cpumask,
// /*AFLA*/ 			    const struct cpumask *andmask,
// /*AFLA*/ 			    unsigned int *apicid)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long cpu_mask = cpumask_bits(cpumask)[0] &
// /*AFLA*/ 				 cpumask_bits(andmask)[0] &
// /*AFLA*/ 				 cpumask_bits(cpu_online_mask)[0] &
// /*AFLA*/ 				 APIC_ALL_CPUS;
// /*AFLA*/ 
// /*AFLA*/ 	if (likely(cpu_mask)) {
// /*AFLA*/ 		*apicid = (unsigned int)cpu_mask;
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	} else {
// /*AFLA*/ 		return -EINVAL;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int
// /*AFLA*/ default_cpu_mask_to_apicid_and(const struct cpumask *cpumask,
// /*AFLA*/ 			       const struct cpumask *andmask,
// /*AFLA*/ 			       unsigned int *apicid);
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ flat_vector_allocation_domain(int cpu, struct cpumask *retmask,
// /*AFLA*/ 			      const struct cpumask *mask)
// /*AFLA*/ {
// /*AFLA*/ 	/* Careful. Some cpus do not strictly honor the set of cpus
// /*AFLA*/ 	 * specified in the interrupt destination when using lowest
// /*AFLA*/ 	 * priority interrupt delivery mode.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * In particular there was a hyperthreading cpu observed to
// /*AFLA*/ 	 * deliver interrupts to the wrong hyperthread when only one
// /*AFLA*/ 	 * hyperthread was specified in the interrupt desitination.
// /*AFLA*/ 	 */
// /*AFLA*/ 	cpumask_clear(retmask);
// /*AFLA*/ 	cpumask_bits(retmask)[0] = APIC_ALL_CPUS;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ default_vector_allocation_domain(int cpu, struct cpumask *retmask,
// /*AFLA*/ 				 const struct cpumask *mask)
// /*AFLA*/ {
// /*AFLA*/ 	cpumask_copy(retmask, cpumask_of(cpu));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long default_check_apicid_used(physid_mask_t *map, int apicid)
// /*AFLA*/ {
// /*AFLA*/ 	return physid_isset(apicid, *map);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void default_ioapic_phys_id_map(physid_mask_t *phys_map, physid_mask_t *retmap)
// /*AFLA*/ {
// /*AFLA*/ 	*retmap = *phys_map;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __default_cpu_present_to_apicid(int mps_cpu)
// /*AFLA*/ {
// /*AFLA*/ 	if (mps_cpu < nr_cpu_ids && cpu_present(mps_cpu))
// /*AFLA*/ 		return (int)per_cpu(x86_bios_cpu_apicid, mps_cpu);
// /*AFLA*/ 	else
// /*AFLA*/ 		return BAD_APICID;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int
// /*AFLA*/ __default_check_phys_apicid_present(int phys_apicid)
// /*AFLA*/ {
// /*AFLA*/ 	return physid_isset(phys_apicid, phys_cpu_present_map);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ static inline int default_cpu_present_to_apicid(int mps_cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return __default_cpu_present_to_apicid(mps_cpu);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int
// /*AFLA*/ default_check_phys_apicid_present(int phys_apicid)
// /*AFLA*/ {
// /*AFLA*/ 	return __default_check_phys_apicid_present(phys_apicid);
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ extern int default_cpu_present_to_apicid(int mps_cpu);
// /*AFLA*/ extern int default_check_phys_apicid_present(int phys_apicid);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_X86_LOCAL_APIC */
// /*AFLA*/ extern void irq_enter(void);
// /*AFLA*/ extern void irq_exit(void);
// /*AFLA*/ 
// /*AFLA*/ static inline void entering_irq(void)
// /*AFLA*/ {
// /*AFLA*/ 	irq_enter();
// /*AFLA*/ 	exit_idle();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void entering_ack_irq(void)
// /*AFLA*/ {
// /*AFLA*/ 	entering_irq();
// /*AFLA*/ 	ack_APIC_irq();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ipi_entering_ack_irq(void)
// /*AFLA*/ {
// /*AFLA*/ 	irq_enter();
// /*AFLA*/ 	ack_APIC_irq();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void exiting_irq(void)
// /*AFLA*/ {
// /*AFLA*/ 	irq_exit();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void exiting_ack_irq(void)
// /*AFLA*/ {
// /*AFLA*/ 	ack_APIC_irq();
// /*AFLA*/ 	irq_exit();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void ioapic_zap_locks(void);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_APIC_H */
