// /*AFLA*/ #ifndef _ASM_X86_MSR_H
// /*AFLA*/ #define _ASM_X86_MSR_H
// /*AFLA*/ 
// /*AFLA*/ #include "msr-index.h"
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #include <asm/asm.h>
// /*AFLA*/ #include <asm/errno.h>
// /*AFLA*/ #include <asm/cpumask.h>
// /*AFLA*/ #include <uapi/asm/msr.h>
// /*AFLA*/ 
// /*AFLA*/ struct msr {
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct {
// /*AFLA*/ 			u32 l;
// /*AFLA*/ 			u32 h;
// /*AFLA*/ 		};
// /*AFLA*/ 		u64 q;
// /*AFLA*/ 	};
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct msr_info {
// /*AFLA*/ 	u32 msr_no;
// /*AFLA*/ 	struct msr reg;
// /*AFLA*/ 	struct msr *msrs;
// /*AFLA*/ 	int err;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct msr_regs_info {
// /*AFLA*/ 	u32 *regs;
// /*AFLA*/ 	int err;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct saved_msr {
// /*AFLA*/ 	bool valid;
// /*AFLA*/ 	struct msr_info info;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct saved_msrs {
// /*AFLA*/ 	unsigned int num;
// /*AFLA*/ 	struct saved_msr *array;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * both i386 and x86_64 returns 64-bit value in edx:eax, but gcc's "A"
// /*AFLA*/  * constraint has different meanings. For i386, "A" means exactly
// /*AFLA*/  * edx:eax, while for x86_64 it doesn't mean rdx:rax or edx:eax. Instead,
// /*AFLA*/  * it means rax *or* rdx.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ /* Using 64-bit values saves one instruction clearing the high half of low */
// /*AFLA*/ #define DECLARE_ARGS(val, low, high)	unsigned long low, high
// /*AFLA*/ #define EAX_EDX_VAL(val, low, high)	((low) | (high) << 32)
// /*AFLA*/ #define EAX_EDX_RET(val, low, high)	"=a" (low), "=d" (high)
// /*AFLA*/ #else
// /*AFLA*/ #define DECLARE_ARGS(val, low, high)	unsigned long long val
// /*AFLA*/ #define EAX_EDX_VAL(val, low, high)	(val)
// /*AFLA*/ #define EAX_EDX_RET(val, low, high)	"=A" (val)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TRACEPOINTS
// /*AFLA*/ /*
// /*AFLA*/  * Be very careful with includes. This header is prone to include loops.
// /*AFLA*/  */
// /*AFLA*/ #include <asm/atomic.h>
// /*AFLA*/ #include <linux/tracepoint-defs.h>
// /*AFLA*/ 
// /*AFLA*/ extern struct tracepoint __tracepoint_read_msr;
// /*AFLA*/ extern struct tracepoint __tracepoint_write_msr;
// /*AFLA*/ extern struct tracepoint __tracepoint_rdpmc;
// /*AFLA*/ #define msr_tracepoint_active(t) static_key_false(&(t).key)
// /*AFLA*/ extern void do_trace_write_msr(unsigned msr, u64 val, int failed);
// /*AFLA*/ extern void do_trace_read_msr(unsigned msr, u64 val, int failed);
// /*AFLA*/ extern void do_trace_rdpmc(unsigned msr, u64 val, int failed);
// /*AFLA*/ #else
// /*AFLA*/ #define msr_tracepoint_active(t) false
// /*AFLA*/ static inline void do_trace_write_msr(unsigned msr, u64 val, int failed) {}
// /*AFLA*/ static inline void do_trace_read_msr(unsigned msr, u64 val, int failed) {}
// /*AFLA*/ static inline void do_trace_rdpmc(unsigned msr, u64 val, int failed) {}
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long long native_read_msr(unsigned int msr)
// /*AFLA*/ {
// /*AFLA*/ 	DECLARE_ARGS(val, low, high);
// /*AFLA*/ 
// /*AFLA*/ 	asm volatile("1: rdmsr\n"
// /*AFLA*/ 		     "2:\n"
// /*AFLA*/ 		     _ASM_EXTABLE_HANDLE(1b, 2b, ex_handler_rdmsr_unsafe)
// /*AFLA*/ 		     : EAX_EDX_RET(val, low, high) : "c" (msr));
// /*AFLA*/ 	if (msr_tracepoint_active(__tracepoint_read_msr))
// /*AFLA*/ 		do_trace_read_msr(msr, EAX_EDX_VAL(val, low, high), 0);
// /*AFLA*/ 	return EAX_EDX_VAL(val, low, high);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long long native_read_msr_safe(unsigned int msr,
// /*AFLA*/ 						      int *err)
// /*AFLA*/ {
// /*AFLA*/ 	DECLARE_ARGS(val, low, high);
// /*AFLA*/ 
// /*AFLA*/ 	asm volatile("2: rdmsr ; xor %[err],%[err]\n"
// /*AFLA*/ 		     "1:\n\t"
// /*AFLA*/ 		     ".section .fixup,\"ax\"\n\t"
// /*AFLA*/ 		     "3: mov %[fault],%[err]\n\t"
// /*AFLA*/ 		     "xorl %%eax, %%eax\n\t"
// /*AFLA*/ 		     "xorl %%edx, %%edx\n\t"
// /*AFLA*/ 		     "jmp 1b\n\t"
// /*AFLA*/ 		     ".previous\n\t"
// /*AFLA*/ 		     _ASM_EXTABLE(2b, 3b)
// /*AFLA*/ 		     : [err] "=r" (*err), EAX_EDX_RET(val, low, high)
// /*AFLA*/ 		     : "c" (msr), [fault] "i" (-EIO));
// /*AFLA*/ 	if (msr_tracepoint_active(__tracepoint_read_msr))
// /*AFLA*/ 		do_trace_read_msr(msr, EAX_EDX_VAL(val, low, high), *err);
// /*AFLA*/ 	return EAX_EDX_VAL(val, low, high);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Can be uninlined because referenced by paravirt */
// /*AFLA*/ notrace static inline void native_write_msr(unsigned int msr,
// /*AFLA*/ 					    unsigned low, unsigned high)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("1: wrmsr\n"
// /*AFLA*/ 		     "2:\n"
// /*AFLA*/ 		     _ASM_EXTABLE_HANDLE(1b, 2b, ex_handler_wrmsr_unsafe)
// /*AFLA*/ 		     : : "c" (msr), "a"(low), "d" (high) : "memory");
// /*AFLA*/ 	if (msr_tracepoint_active(__tracepoint_write_msr))
// /*AFLA*/ 		do_trace_write_msr(msr, ((u64)high << 32 | low), 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Can be uninlined because referenced by paravirt */
// /*AFLA*/ notrace static inline int native_write_msr_safe(unsigned int msr,
// /*AFLA*/ 					unsigned low, unsigned high)
// /*AFLA*/ {
// /*AFLA*/ 	int err;
// /*AFLA*/ 	asm volatile("2: wrmsr ; xor %[err],%[err]\n"
// /*AFLA*/ 		     "1:\n\t"
// /*AFLA*/ 		     ".section .fixup,\"ax\"\n\t"
// /*AFLA*/ 		     "3:  mov %[fault],%[err] ; jmp 1b\n\t"
// /*AFLA*/ 		     ".previous\n\t"
// /*AFLA*/ 		     _ASM_EXTABLE(2b, 3b)
// /*AFLA*/ 		     : [err] "=a" (err)
// /*AFLA*/ 		     : "c" (msr), "0" (low), "d" (high),
// /*AFLA*/ 		       [fault] "i" (-EIO)
// /*AFLA*/ 		     : "memory");
// /*AFLA*/ 	if (msr_tracepoint_active(__tracepoint_write_msr))
// /*AFLA*/ 		do_trace_write_msr(msr, ((u64)high << 32 | low), err);
// /*AFLA*/ 	return err;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int rdmsr_safe_regs(u32 regs[8]);
// /*AFLA*/ extern int wrmsr_safe_regs(u32 regs[8]);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rdtsc() - returns the current TSC without ordering constraints
// /*AFLA*/  *
// /*AFLA*/  * rdtsc() returns the result of RDTSC as a 64-bit integer.  The
// /*AFLA*/  * only ordering constraint it supplies is the ordering implied by
// /*AFLA*/  * "asm volatile": it will put the RDTSC in the place you expect.  The
// /*AFLA*/  * CPU can and will speculatively execute that RDTSC, though, so the
// /*AFLA*/  * results can be non-monotonic if compared on different CPUs.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline unsigned long long rdtsc(void)
// /*AFLA*/ {
// /*AFLA*/ 	DECLARE_ARGS(val, low, high);
// /*AFLA*/ 
// /*AFLA*/ 	asm volatile("rdtsc" : EAX_EDX_RET(val, low, high));
// /*AFLA*/ 
// /*AFLA*/ 	return EAX_EDX_VAL(val, low, high);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rdtsc_ordered() - read the current TSC in program order
// /*AFLA*/  *
// /*AFLA*/  * rdtsc_ordered() returns the result of RDTSC as a 64-bit integer.
// /*AFLA*/  * It is ordered like a load to a global in-memory counter.  It should
// /*AFLA*/  * be impossible to observe non-monotonic rdtsc_unordered() behavior
// /*AFLA*/  * across multiple CPUs as long as the TSC is synced.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline unsigned long long rdtsc_ordered(void)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The RDTSC instruction is not ordered relative to memory
// /*AFLA*/ 	 * access.  The Intel SDM and the AMD APM are both vague on this
// /*AFLA*/ 	 * point, but empirically an RDTSC instruction can be
// /*AFLA*/ 	 * speculatively executed before prior loads.  An RDTSC
// /*AFLA*/ 	 * immediately after an appropriate barrier appears to be
// /*AFLA*/ 	 * ordered as a normal load, that is, it provides the same
// /*AFLA*/ 	 * ordering guarantees as reading from a global memory location
// /*AFLA*/ 	 * that some other imaginary CPU is updating continuously with a
// /*AFLA*/ 	 * time stamp.
// /*AFLA*/ 	 */
// /*AFLA*/ 	alternative_2("", "mfence", X86_FEATURE_MFENCE_RDTSC,
// /*AFLA*/ 			  "lfence", X86_FEATURE_LFENCE_RDTSC);
// /*AFLA*/ 	return rdtsc();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Deprecated, keep it for a cycle for easier merging: */
// /*AFLA*/ #define rdtscll(now)	do { (now) = rdtsc_ordered(); } while (0)
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long long native_read_pmc(int counter)
// /*AFLA*/ {
// /*AFLA*/ 	DECLARE_ARGS(val, low, high);
// /*AFLA*/ 
// /*AFLA*/ 	asm volatile("rdpmc" : EAX_EDX_RET(val, low, high) : "c" (counter));
// /*AFLA*/ 	if (msr_tracepoint_active(__tracepoint_rdpmc))
// /*AFLA*/ 		do_trace_rdpmc(counter, EAX_EDX_VAL(val, low, high), 0);
// /*AFLA*/ 	return EAX_EDX_VAL(val, low, high);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PARAVIRT
// /*AFLA*/ #include <asm/paravirt.h>
// /*AFLA*/ #else
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ /*
// /*AFLA*/  * Access to machine-specific registers (available on 586 and better only)
// /*AFLA*/  * Note: the rd* operations modify the parameters directly (without using
// /*AFLA*/  * pointer indirection), this allows gcc to optimize better
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define rdmsr(msr, low, high)					\
// /*AFLA*/ do {								\
// /*AFLA*/ 	u64 __val = native_read_msr((msr));			\
// /*AFLA*/ 	(void)((low) = (u32)__val);				\
// /*AFLA*/ 	(void)((high) = (u32)(__val >> 32));			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ static inline void wrmsr(unsigned msr, unsigned low, unsigned high)
// /*AFLA*/ {
// /*AFLA*/ 	native_write_msr(msr, low, high);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define rdmsrl(msr, val)			\
// /*AFLA*/ 	((val) = native_read_msr((msr)))
// /*AFLA*/ 
// /*AFLA*/ static inline void wrmsrl(unsigned msr, u64 val)
// /*AFLA*/ {
// /*AFLA*/ 	native_write_msr(msr, (u32)(val & 0xffffffffULL), (u32)(val >> 32));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* wrmsr with exception handling */
// /*AFLA*/ static inline int wrmsr_safe(unsigned msr, unsigned low, unsigned high)
// /*AFLA*/ {
// /*AFLA*/ 	return native_write_msr_safe(msr, low, high);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* rdmsr with exception handling */
// /*AFLA*/ #define rdmsr_safe(msr, low, high)				\
// /*AFLA*/ ({								\
// /*AFLA*/ 	int __err;						\
// /*AFLA*/ 	u64 __val = native_read_msr_safe((msr), &__err);	\
// /*AFLA*/ 	(*low) = (u32)__val;					\
// /*AFLA*/ 	(*high) = (u32)(__val >> 32);				\
// /*AFLA*/ 	__err;							\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ static inline int rdmsrl_safe(unsigned msr, unsigned long long *p)
// /*AFLA*/ {
// /*AFLA*/ 	int err;
// /*AFLA*/ 
// /*AFLA*/ 	*p = native_read_msr_safe(msr, &err);
// /*AFLA*/ 	return err;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define rdpmc(counter, low, high)			\
// /*AFLA*/ do {							\
// /*AFLA*/ 	u64 _l = native_read_pmc((counter));		\
// /*AFLA*/ 	(low)  = (u32)_l;				\
// /*AFLA*/ 	(high) = (u32)(_l >> 32);			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define rdpmcl(counter, val) ((val) = native_read_pmc(counter))
// /*AFLA*/ 
// /*AFLA*/ #endif	/* !CONFIG_PARAVIRT */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * 64-bit version of wrmsr_safe():
// /*AFLA*/  */
// /*AFLA*/ static inline int wrmsrl_safe(u32 msr, u64 val)
// /*AFLA*/ {
// /*AFLA*/ 	return wrmsr_safe(msr, (u32)val,  (u32)(val >> 32));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define write_tsc(low, high) wrmsr(MSR_IA32_TSC, (low), (high))
// /*AFLA*/ 
// /*AFLA*/ #define write_rdtscp_aux(val) wrmsr(MSR_TSC_AUX, (val), 0)
// /*AFLA*/ 
// /*AFLA*/ struct msr *msrs_alloc(void);
// /*AFLA*/ void msrs_free(struct msr *msrs);
// /*AFLA*/ int msr_set_bit(u32 msr, u8 bit);
// /*AFLA*/ int msr_clear_bit(u32 msr, u8 bit);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ int rdmsr_on_cpu(unsigned int cpu, u32 msr_no, u32 *l, u32 *h);
// /*AFLA*/ int wrmsr_on_cpu(unsigned int cpu, u32 msr_no, u32 l, u32 h);
// /*AFLA*/ int rdmsrl_on_cpu(unsigned int cpu, u32 msr_no, u64 *q);
// /*AFLA*/ int wrmsrl_on_cpu(unsigned int cpu, u32 msr_no, u64 q);
// /*AFLA*/ void rdmsr_on_cpus(const struct cpumask *mask, u32 msr_no, struct msr *msrs);
// /*AFLA*/ void wrmsr_on_cpus(const struct cpumask *mask, u32 msr_no, struct msr *msrs);
// /*AFLA*/ int rdmsr_safe_on_cpu(unsigned int cpu, u32 msr_no, u32 *l, u32 *h);
// /*AFLA*/ int wrmsr_safe_on_cpu(unsigned int cpu, u32 msr_no, u32 l, u32 h);
// /*AFLA*/ int rdmsrl_safe_on_cpu(unsigned int cpu, u32 msr_no, u64 *q);
// /*AFLA*/ int wrmsrl_safe_on_cpu(unsigned int cpu, u32 msr_no, u64 q);
// /*AFLA*/ int rdmsr_safe_regs_on_cpu(unsigned int cpu, u32 regs[8]);
// /*AFLA*/ int wrmsr_safe_regs_on_cpu(unsigned int cpu, u32 regs[8]);
// /*AFLA*/ #else  /*  CONFIG_SMP  */
// /*AFLA*/ static inline int rdmsr_on_cpu(unsigned int cpu, u32 msr_no, u32 *l, u32 *h)
// /*AFLA*/ {
// /*AFLA*/ 	rdmsr(msr_no, *l, *h);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int wrmsr_on_cpu(unsigned int cpu, u32 msr_no, u32 l, u32 h)
// /*AFLA*/ {
// /*AFLA*/ 	wrmsr(msr_no, l, h);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int rdmsrl_on_cpu(unsigned int cpu, u32 msr_no, u64 *q)
// /*AFLA*/ {
// /*AFLA*/ 	rdmsrl(msr_no, *q);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int wrmsrl_on_cpu(unsigned int cpu, u32 msr_no, u64 q)
// /*AFLA*/ {
// /*AFLA*/ 	wrmsrl(msr_no, q);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void rdmsr_on_cpus(const struct cpumask *m, u32 msr_no,
// /*AFLA*/ 				struct msr *msrs)
// /*AFLA*/ {
// /*AFLA*/        rdmsr_on_cpu(0, msr_no, &(msrs[0].l), &(msrs[0].h));
// /*AFLA*/ }
// /*AFLA*/ static inline void wrmsr_on_cpus(const struct cpumask *m, u32 msr_no,
// /*AFLA*/ 				struct msr *msrs)
// /*AFLA*/ {
// /*AFLA*/        wrmsr_on_cpu(0, msr_no, msrs[0].l, msrs[0].h);
// /*AFLA*/ }
// /*AFLA*/ static inline int rdmsr_safe_on_cpu(unsigned int cpu, u32 msr_no,
// /*AFLA*/ 				    u32 *l, u32 *h)
// /*AFLA*/ {
// /*AFLA*/ 	return rdmsr_safe(msr_no, l, h);
// /*AFLA*/ }
// /*AFLA*/ static inline int wrmsr_safe_on_cpu(unsigned int cpu, u32 msr_no, u32 l, u32 h)
// /*AFLA*/ {
// /*AFLA*/ 	return wrmsr_safe(msr_no, l, h);
// /*AFLA*/ }
// /*AFLA*/ static inline int rdmsrl_safe_on_cpu(unsigned int cpu, u32 msr_no, u64 *q)
// /*AFLA*/ {
// /*AFLA*/ 	return rdmsrl_safe(msr_no, q);
// /*AFLA*/ }
// /*AFLA*/ static inline int wrmsrl_safe_on_cpu(unsigned int cpu, u32 msr_no, u64 q)
// /*AFLA*/ {
// /*AFLA*/ 	return wrmsrl_safe(msr_no, q);
// /*AFLA*/ }
// /*AFLA*/ static inline int rdmsr_safe_regs_on_cpu(unsigned int cpu, u32 regs[8])
// /*AFLA*/ {
// /*AFLA*/ 	return rdmsr_safe_regs(regs);
// /*AFLA*/ }
// /*AFLA*/ static inline int wrmsr_safe_regs_on_cpu(unsigned int cpu, u32 regs[8])
// /*AFLA*/ {
// /*AFLA*/ 	return wrmsr_safe_regs(regs);
// /*AFLA*/ }
// /*AFLA*/ #endif  /* CONFIG_SMP */
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ #endif /* _ASM_X86_MSR_H */
