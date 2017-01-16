// /*AFLA*/ #ifndef _ASM_X86_SPECIAL_INSNS_H
// /*AFLA*/ #define _ASM_X86_SPECIAL_INSNS_H
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifdef __KERNEL__
// /*AFLA*/ 
// /*AFLA*/ #include <asm/nops.h>
// /*AFLA*/ 
// /*AFLA*/ static inline void native_clts(void)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("clts");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Volatile isn't enough to prevent the compiler from reordering the
// /*AFLA*/  * read/write functions for the control registers and messing everything up.
// /*AFLA*/  * A memory clobber would solve the problem, but would prevent reordering of
// /*AFLA*/  * all loads stores around it, which can hurt performance. Solution is to
// /*AFLA*/  * use a variable and mimic reads and writes to it to enforce serialization
// /*AFLA*/  */
// /*AFLA*/ extern unsigned long __force_order;
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long native_read_cr0(void)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long val;
// /*AFLA*/ 	asm volatile("mov %%cr0,%0\n\t" : "=r" (val), "=m" (__force_order));
// /*AFLA*/ 	return val;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_write_cr0(unsigned long val)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("mov %0,%%cr0": : "r" (val), "m" (__force_order));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long native_read_cr2(void)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long val;
// /*AFLA*/ 	asm volatile("mov %%cr2,%0\n\t" : "=r" (val), "=m" (__force_order));
// /*AFLA*/ 	return val;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_write_cr2(unsigned long val)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("mov %0,%%cr2": : "r" (val), "m" (__force_order));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long native_read_cr3(void)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long val;
// /*AFLA*/ 	asm volatile("mov %%cr3,%0\n\t" : "=r" (val), "=m" (__force_order));
// /*AFLA*/ 	return val;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_write_cr3(unsigned long val)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("mov %0,%%cr3": : "r" (val), "m" (__force_order));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long native_read_cr4(void)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long val;
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * This could fault if CR4 does not exist.  Non-existent CR4
// /*AFLA*/ 	 * is functionally equivalent to CR4 == 0.  Keep it simple and pretend
// /*AFLA*/ 	 * that CR4 == 0 on CPUs that don't have CR4.
// /*AFLA*/ 	 */
// /*AFLA*/ 	asm volatile("1: mov %%cr4, %0\n"
// /*AFLA*/ 		     "2:\n"
// /*AFLA*/ 		     _ASM_EXTABLE(1b, 2b)
// /*AFLA*/ 		     : "=r" (val), "=m" (__force_order) : "0" (0));
// /*AFLA*/ #else
// /*AFLA*/ 	/* CR4 always exists on x86_64. */
// /*AFLA*/ 	asm volatile("mov %%cr4,%0\n\t" : "=r" (val), "=m" (__force_order));
// /*AFLA*/ #endif
// /*AFLA*/ 	return val;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_write_cr4(unsigned long val)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("mov %0,%%cr4": : "r" (val), "m" (__force_order));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ static inline unsigned long native_read_cr8(void)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long cr8;
// /*AFLA*/ 	asm volatile("movq %%cr8,%0" : "=r" (cr8));
// /*AFLA*/ 	return cr8;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void native_write_cr8(unsigned long val)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("movq %0,%%cr8" :: "r" (val) : "memory");
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS
// /*AFLA*/ static inline u32 __read_pkru(void)
// /*AFLA*/ {
// /*AFLA*/ 	u32 ecx = 0;
// /*AFLA*/ 	u32 edx, pkru;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * "rdpkru" instruction.  Places PKRU contents in to EAX,
// /*AFLA*/ 	 * clears EDX and requires that ecx=0.
// /*AFLA*/ 	 */
// /*AFLA*/ 	asm volatile(".byte 0x0f,0x01,0xee\n\t"
// /*AFLA*/ 		     : "=a" (pkru), "=d" (edx)
// /*AFLA*/ 		     : "c" (ecx));
// /*AFLA*/ 	return pkru;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __write_pkru(u32 pkru)
// /*AFLA*/ {
// /*AFLA*/ 	u32 ecx = 0, edx = 0;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * "wrpkru" instruction.  Loads contents in EAX to PKRU,
// /*AFLA*/ 	 * requires that ecx = edx = 0.
// /*AFLA*/ 	 */
// /*AFLA*/ 	asm volatile(".byte 0x0f,0x01,0xef\n\t"
// /*AFLA*/ 		     : : "a" (pkru), "c"(ecx), "d"(edx));
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline u32 __read_pkru(void)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __write_pkru(u32 pkru)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline void native_wbinvd(void)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("wbinvd": : :"memory");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern asmlinkage void native_load_gs_index(unsigned);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PARAVIRT
// /*AFLA*/ #include <asm/paravirt.h>
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long read_cr0(void)
// /*AFLA*/ {
// /*AFLA*/ 	return native_read_cr0();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_cr0(unsigned long x)
// /*AFLA*/ {
// /*AFLA*/ 	native_write_cr0(x);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long read_cr2(void)
// /*AFLA*/ {
// /*AFLA*/ 	return native_read_cr2();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_cr2(unsigned long x)
// /*AFLA*/ {
// /*AFLA*/ 	native_write_cr2(x);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long read_cr3(void)
// /*AFLA*/ {
// /*AFLA*/ 	return native_read_cr3();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_cr3(unsigned long x)
// /*AFLA*/ {
// /*AFLA*/ 	native_write_cr3(x);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long __read_cr4(void)
// /*AFLA*/ {
// /*AFLA*/ 	return native_read_cr4();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __write_cr4(unsigned long x)
// /*AFLA*/ {
// /*AFLA*/ 	native_write_cr4(x);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void wbinvd(void)
// /*AFLA*/ {
// /*AFLA*/ 	native_wbinvd();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long read_cr8(void)
// /*AFLA*/ {
// /*AFLA*/ 	return native_read_cr8();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void write_cr8(unsigned long x)
// /*AFLA*/ {
// /*AFLA*/ 	native_write_cr8(x);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void load_gs_index(unsigned selector)
// /*AFLA*/ {
// /*AFLA*/ 	native_load_gs_index(selector);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Clear the 'TS' bit */
// /*AFLA*/ static inline void clts(void)
// /*AFLA*/ {
// /*AFLA*/ 	native_clts();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif/* CONFIG_PARAVIRT */
// /*AFLA*/ 
// /*AFLA*/ #define stts() write_cr0(read_cr0() | X86_CR0_TS)
// /*AFLA*/ 
// /*AFLA*/ static inline void clflush(volatile void *__p)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("clflush %0" : "+m" (*(volatile char __force *)__p));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void clflushopt(volatile void *__p)
// /*AFLA*/ {
// /*AFLA*/ 	alternative_io(".byte " __stringify(NOP_DS_PREFIX) "; clflush %P0",
// /*AFLA*/ 		       ".byte 0x66; clflush %P0",
// /*AFLA*/ 		       X86_FEATURE_CLFLUSHOPT,
// /*AFLA*/ 		       "+m" (*(volatile char __force *)__p));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void clwb(volatile void *__p)
// /*AFLA*/ {
// /*AFLA*/ 	volatile struct { char x[64]; } *p = __p;
// /*AFLA*/ 
// /*AFLA*/ 	asm volatile(ALTERNATIVE_2(
// /*AFLA*/ 		".byte " __stringify(NOP_DS_PREFIX) "; clflush (%[pax])",
// /*AFLA*/ 		".byte 0x66; clflush (%[pax])", /* clflushopt (%%rax) */
// /*AFLA*/ 		X86_FEATURE_CLFLUSHOPT,
// /*AFLA*/ 		".byte 0x66, 0x0f, 0xae, 0x30",  /* clwb (%%rax) */
// /*AFLA*/ 		X86_FEATURE_CLWB)
// /*AFLA*/ 		: [p] "+m" (*p)
// /*AFLA*/ 		: [pax] "a" (p));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nop() asm volatile ("nop")
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* __KERNEL__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_SPECIAL_INSNS_H */
