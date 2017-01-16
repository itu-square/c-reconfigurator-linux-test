// /*AFLA*/ #ifndef _ASM_X86_SEGMENT_H
// /*AFLA*/ #define _ASM_X86_SEGMENT_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/const.h>
// /*AFLA*/ #include <asm/alternative.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Constructor for a conventional segment GDT (or LDT) entry.
// /*AFLA*/  * This is a macro so it can be used in initializers.
// /*AFLA*/  */
// /*AFLA*/ #define GDT_ENTRY(flags, base, limit)			\
// /*AFLA*/ 	((((base)  & _AC(0xff000000,ULL)) << (56-24)) |	\
// /*AFLA*/ 	 (((flags) & _AC(0x0000f0ff,ULL)) << 40) |	\
// /*AFLA*/ 	 (((limit) & _AC(0x000f0000,ULL)) << (48-16)) |	\
// /*AFLA*/ 	 (((base)  & _AC(0x00ffffff,ULL)) << 16) |	\
// /*AFLA*/ 	 (((limit) & _AC(0x0000ffff,ULL))))
// /*AFLA*/ 
// /*AFLA*/ /* Simple and small GDT entries for booting only: */
// /*AFLA*/ 
// /*AFLA*/ #define GDT_ENTRY_BOOT_CS	2
// /*AFLA*/ #define GDT_ENTRY_BOOT_DS	3
// /*AFLA*/ #define GDT_ENTRY_BOOT_TSS	4
// /*AFLA*/ #define __BOOT_CS		(GDT_ENTRY_BOOT_CS*8)
// /*AFLA*/ #define __BOOT_DS		(GDT_ENTRY_BOOT_DS*8)
// /*AFLA*/ #define __BOOT_TSS		(GDT_ENTRY_BOOT_TSS*8)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Bottom two bits of selector give the ring
// /*AFLA*/  * privilege level
// /*AFLA*/  */
// /*AFLA*/ #define SEGMENT_RPL_MASK	0x3
// /*AFLA*/ 
// /*AFLA*/ /* User mode is privilege level 3: */
// /*AFLA*/ #define USER_RPL		0x3
// /*AFLA*/ 
// /*AFLA*/ /* Bit 2 is Table Indicator (TI): selects between LDT or GDT */
// /*AFLA*/ #define SEGMENT_TI_MASK		0x4
// /*AFLA*/ /* LDT segment has TI set ... */
// /*AFLA*/ #define SEGMENT_LDT		0x4
// /*AFLA*/ /* ... GDT has it cleared */
// /*AFLA*/ #define SEGMENT_GDT		0x0
// /*AFLA*/ 
// /*AFLA*/ #define GDT_ENTRY_INVALID_SEG	0
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ /*
// /*AFLA*/  * The layout of the per-CPU GDT under Linux:
// /*AFLA*/  *
// /*AFLA*/  *   0 - null								<=== cacheline #1
// /*AFLA*/  *   1 - reserved
// /*AFLA*/  *   2 - reserved
// /*AFLA*/  *   3 - reserved
// /*AFLA*/  *
// /*AFLA*/  *   4 - unused								<=== cacheline #2
// /*AFLA*/  *   5 - unused
// /*AFLA*/  *
// /*AFLA*/  *  ------- start of TLS (Thread-Local Storage) segments:
// /*AFLA*/  *
// /*AFLA*/  *   6 - TLS segment #1			[ glibc's TLS segment ]
// /*AFLA*/  *   7 - TLS segment #2			[ Wine's %fs Win32 segment ]
// /*AFLA*/  *   8 - TLS segment #3							<=== cacheline #3
// /*AFLA*/  *   9 - reserved
// /*AFLA*/  *  10 - reserved
// /*AFLA*/  *  11 - reserved
// /*AFLA*/  *
// /*AFLA*/  *  ------- start of kernel segments:
// /*AFLA*/  *
// /*AFLA*/  *  12 - kernel code segment						<=== cacheline #4
// /*AFLA*/  *  13 - kernel data segment
// /*AFLA*/  *  14 - default user CS
// /*AFLA*/  *  15 - default user DS
// /*AFLA*/  *  16 - TSS								<=== cacheline #5
// /*AFLA*/  *  17 - LDT
// /*AFLA*/  *  18 - PNPBIOS support (16->32 gate)
// /*AFLA*/  *  19 - PNPBIOS support
// /*AFLA*/  *  20 - PNPBIOS support						<=== cacheline #6
// /*AFLA*/  *  21 - PNPBIOS support
// /*AFLA*/  *  22 - PNPBIOS support
// /*AFLA*/  *  23 - APM BIOS support
// /*AFLA*/  *  24 - APM BIOS support						<=== cacheline #7
// /*AFLA*/  *  25 - APM BIOS support
// /*AFLA*/  *
// /*AFLA*/  *  26 - ESPFIX small SS
// /*AFLA*/  *  27 - per-cpu			[ offset to per-cpu data area ]
// /*AFLA*/  *  28 - stack_canary-20		[ for stack protector ]		<=== cacheline #8
// /*AFLA*/  *  29 - unused
// /*AFLA*/  *  30 - unused
// /*AFLA*/  *  31 - TSS for double fault handler
// /*AFLA*/  */
// /*AFLA*/ #define GDT_ENTRY_TLS_MIN		6
// /*AFLA*/ #define GDT_ENTRY_TLS_MAX 		(GDT_ENTRY_TLS_MIN + GDT_ENTRY_TLS_ENTRIES - 1)
// /*AFLA*/ 
// /*AFLA*/ #define GDT_ENTRY_KERNEL_CS		12
// /*AFLA*/ #define GDT_ENTRY_KERNEL_DS		13
// /*AFLA*/ #define GDT_ENTRY_DEFAULT_USER_CS	14
// /*AFLA*/ #define GDT_ENTRY_DEFAULT_USER_DS	15
// /*AFLA*/ #define GDT_ENTRY_TSS			16
// /*AFLA*/ #define GDT_ENTRY_LDT			17
// /*AFLA*/ #define GDT_ENTRY_PNPBIOS_CS32		18
// /*AFLA*/ #define GDT_ENTRY_PNPBIOS_CS16		19
// /*AFLA*/ #define GDT_ENTRY_PNPBIOS_DS		20
// /*AFLA*/ #define GDT_ENTRY_PNPBIOS_TS1		21
// /*AFLA*/ #define GDT_ENTRY_PNPBIOS_TS2		22
// /*AFLA*/ #define GDT_ENTRY_APMBIOS_BASE		23
// /*AFLA*/ 
// /*AFLA*/ #define GDT_ENTRY_ESPFIX_SS		26
// /*AFLA*/ #define GDT_ENTRY_PERCPU		27
// /*AFLA*/ #define GDT_ENTRY_STACK_CANARY		28
// /*AFLA*/ 
// /*AFLA*/ #define GDT_ENTRY_DOUBLEFAULT_TSS	31
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Number of entries in the GDT table:
// /*AFLA*/  */
// /*AFLA*/ #define GDT_ENTRIES			32
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Segment selector values corresponding to the above entries:
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define __KERNEL_CS			(GDT_ENTRY_KERNEL_CS*8)
// /*AFLA*/ #define __KERNEL_DS			(GDT_ENTRY_KERNEL_DS*8)
// /*AFLA*/ #define __USER_DS			(GDT_ENTRY_DEFAULT_USER_DS*8 + 3)
// /*AFLA*/ #define __USER_CS			(GDT_ENTRY_DEFAULT_USER_CS*8 + 3)
// /*AFLA*/ #define __ESPFIX_SS			(GDT_ENTRY_ESPFIX_SS*8)
// /*AFLA*/ 
// /*AFLA*/ /* segment for calling fn: */
// /*AFLA*/ #define PNP_CS32			(GDT_ENTRY_PNPBIOS_CS32*8)
// /*AFLA*/ /* code segment for BIOS: */
// /*AFLA*/ #define PNP_CS16			(GDT_ENTRY_PNPBIOS_CS16*8)
// /*AFLA*/ 
// /*AFLA*/ /* "Is this PNP code selector (PNP_CS32 or PNP_CS16)?" */
// /*AFLA*/ #define SEGMENT_IS_PNP_CODE(x)		(((x) & 0xf4) == PNP_CS32)
// /*AFLA*/ 
// /*AFLA*/ /* data segment for BIOS: */
// /*AFLA*/ #define PNP_DS				(GDT_ENTRY_PNPBIOS_DS*8)
// /*AFLA*/ /* transfer data segment: */
// /*AFLA*/ #define PNP_TS1				(GDT_ENTRY_PNPBIOS_TS1*8)
// /*AFLA*/ /* another data segment: */
// /*AFLA*/ #define PNP_TS2				(GDT_ENTRY_PNPBIOS_TS2*8)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ # define __KERNEL_PERCPU		(GDT_ENTRY_PERCPU*8)
// /*AFLA*/ #else
// /*AFLA*/ # define __KERNEL_PERCPU		0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_CC_STACKPROTECTOR
// /*AFLA*/ # define __KERNEL_STACK_CANARY		(GDT_ENTRY_STACK_CANARY*8)
// /*AFLA*/ #else
// /*AFLA*/ # define __KERNEL_STACK_CANARY		0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #else /* 64-bit: */
// /*AFLA*/ 
// /*AFLA*/ #include <asm/cache.h>
// /*AFLA*/ 
// /*AFLA*/ #define GDT_ENTRY_KERNEL32_CS		1
// /*AFLA*/ #define GDT_ENTRY_KERNEL_CS		2
// /*AFLA*/ #define GDT_ENTRY_KERNEL_DS		3
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We cannot use the same code segment descriptor for user and kernel mode,
// /*AFLA*/  * not even in long flat mode, because of different DPL.
// /*AFLA*/  *
// /*AFLA*/  * GDT layout to get 64-bit SYSCALL/SYSRET support right. SYSRET hardcodes
// /*AFLA*/  * selectors:
// /*AFLA*/  *
// /*AFLA*/  *   if returning to 32-bit userspace: cs = STAR.SYSRET_CS,
// /*AFLA*/  *   if returning to 64-bit userspace: cs = STAR.SYSRET_CS+16,
// /*AFLA*/  *
// /*AFLA*/  * ss = STAR.SYSRET_CS+8 (in either case)
// /*AFLA*/  *
// /*AFLA*/  * thus USER_DS should be between 32-bit and 64-bit code selectors:
// /*AFLA*/  */
// /*AFLA*/ #define GDT_ENTRY_DEFAULT_USER32_CS	4
// /*AFLA*/ #define GDT_ENTRY_DEFAULT_USER_DS	5
// /*AFLA*/ #define GDT_ENTRY_DEFAULT_USER_CS	6
// /*AFLA*/ 
// /*AFLA*/ /* Needs two entries */
// /*AFLA*/ #define GDT_ENTRY_TSS			8
// /*AFLA*/ /* Needs two entries */
// /*AFLA*/ #define GDT_ENTRY_LDT			10
// /*AFLA*/ 
// /*AFLA*/ #define GDT_ENTRY_TLS_MIN		12
// /*AFLA*/ #define GDT_ENTRY_TLS_MAX		14
// /*AFLA*/ 
// /*AFLA*/ /* Abused to load per CPU data from limit */
// /*AFLA*/ #define GDT_ENTRY_PER_CPU		15
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Number of entries in the GDT table:
// /*AFLA*/  */
// /*AFLA*/ #define GDT_ENTRIES			16
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Segment selector values corresponding to the above entries:
// /*AFLA*/  *
// /*AFLA*/  * Note, selectors also need to have a correct RPL,
// /*AFLA*/  * expressed with the +3 value for user-space selectors:
// /*AFLA*/  */
// /*AFLA*/ #define __KERNEL32_CS			(GDT_ENTRY_KERNEL32_CS*8)
// /*AFLA*/ #define __KERNEL_CS			(GDT_ENTRY_KERNEL_CS*8)
// /*AFLA*/ #define __KERNEL_DS			(GDT_ENTRY_KERNEL_DS*8)
// /*AFLA*/ #define __USER32_CS			(GDT_ENTRY_DEFAULT_USER32_CS*8 + 3)
// /*AFLA*/ #define __USER_DS			(GDT_ENTRY_DEFAULT_USER_DS*8 + 3)
// /*AFLA*/ #define __USER32_DS			__USER_DS
// /*AFLA*/ #define __USER_CS			(GDT_ENTRY_DEFAULT_USER_CS*8 + 3)
// /*AFLA*/ #define __PER_CPU_SEG			(GDT_ENTRY_PER_CPU*8 + 3)
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_PARAVIRT
// /*AFLA*/ # define get_kernel_rpl()		0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define IDT_ENTRIES			256
// /*AFLA*/ #define NUM_EXCEPTION_VECTORS		32
// /*AFLA*/ 
// /*AFLA*/ /* Bitmask of exception vectors which push an error code on the stack: */
// /*AFLA*/ #define EXCEPTION_ERRCODE_MASK		0x00027d00
// /*AFLA*/ 
// /*AFLA*/ #define GDT_SIZE			(GDT_ENTRIES*8)
// /*AFLA*/ #define GDT_ENTRY_TLS_ENTRIES		3
// /*AFLA*/ #define TLS_SIZE			(GDT_ENTRY_TLS_ENTRIES* 8)
// /*AFLA*/ 
// /*AFLA*/ #ifdef __KERNEL__
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * early_idt_handler_array is an array of entry points referenced in the
// /*AFLA*/  * early IDT.  For simplicity, it's a real array with one entry point
// /*AFLA*/  * every nine bytes.  That leaves room for an optional 'push $0' if the
// /*AFLA*/  * vector has no error code (two bytes), a 'push $vector_number' (two
// /*AFLA*/  * bytes), and a jump to the common entry code (up to five bytes).
// /*AFLA*/  */
// /*AFLA*/ #define EARLY_IDT_HANDLER_SIZE 9
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ extern const char early_idt_handler_array[NUM_EXCEPTION_VECTORS][EARLY_IDT_HANDLER_SIZE];
// /*AFLA*/ #ifdef CONFIG_TRACING
// /*AFLA*/ # define trace_early_idt_handler_array early_idt_handler_array
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Load a segment. Fall back on loading the zero segment if something goes
// /*AFLA*/  * wrong.  This variant assumes that loading zero fully clears the segment.
// /*AFLA*/  * This is always the case on Intel CPUs and, even on 64-bit AMD CPUs, any
// /*AFLA*/  * failure to fully clear the cached descriptor is only observable for
// /*AFLA*/  * FS and GS.
// /*AFLA*/  */
// /*AFLA*/ #define __loadsegment_simple(seg, value)				\
// /*AFLA*/ do {									\
// /*AFLA*/ 	unsigned short __val = (value);					\
// /*AFLA*/ 									\
// /*AFLA*/ 	asm volatile("						\n"	\
// /*AFLA*/ 		     "1:	movl %k0,%%" #seg "		\n"	\
// /*AFLA*/ 									\
// /*AFLA*/ 		     ".section .fixup,\"ax\"			\n"	\
// /*AFLA*/ 		     "2:	xorl %k0,%k0			\n"	\
// /*AFLA*/ 		     "		jmp 1b				\n"	\
// /*AFLA*/ 		     ".previous					\n"	\
// /*AFLA*/ 									\
// /*AFLA*/ 		     _ASM_EXTABLE(1b, 2b)				\
// /*AFLA*/ 									\
// /*AFLA*/ 		     : "+r" (__val) : : "memory");			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define __loadsegment_ss(value) __loadsegment_simple(ss, (value))
// /*AFLA*/ #define __loadsegment_ds(value) __loadsegment_simple(ds, (value))
// /*AFLA*/ #define __loadsegment_es(value) __loadsegment_simple(es, (value))
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * On 32-bit systems, the hidden parts of FS and GS are unobservable if
// /*AFLA*/  * the selector is NULL, so there's no funny business here.
// /*AFLA*/  */
// /*AFLA*/ #define __loadsegment_fs(value) __loadsegment_simple(fs, (value))
// /*AFLA*/ #define __loadsegment_gs(value) __loadsegment_simple(gs, (value))
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline void __loadsegment_fs(unsigned short value)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("						\n"
// /*AFLA*/ 		     "1:	movw %0, %%fs			\n"
// /*AFLA*/ 		     "2:					\n"
// /*AFLA*/ 
// /*AFLA*/ 		     _ASM_EXTABLE_HANDLE(1b, 2b, ex_handler_clear_fs)
// /*AFLA*/ 
// /*AFLA*/ 		     : : "rm" (value) : "memory");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* __loadsegment_gs is intentionally undefined.  Use load_gs_index instead. */
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define loadsegment(seg, value) __loadsegment_ ## seg (value)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Save a segment register away:
// /*AFLA*/  */
// /*AFLA*/ #define savesegment(seg, value)				\
// /*AFLA*/ 	asm("mov %%" #seg ",%0":"=r" (value) : : "memory")
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * x86-32 user GS accessors:
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ # ifdef CONFIG_X86_32_LAZY_GS
// /*AFLA*/ #  define get_user_gs(regs)		(u16)({ unsigned long v; savesegment(gs, v); v; })
// /*AFLA*/ #  define set_user_gs(regs, v)		loadsegment(gs, (unsigned long)(v))
// /*AFLA*/ #  define task_user_gs(tsk)		((tsk)->thread.gs)
// /*AFLA*/ #  define lazy_save_gs(v)		savesegment(gs, (v))
// /*AFLA*/ #  define lazy_load_gs(v)		loadsegment(gs, (v))
// /*AFLA*/ # else	/* X86_32_LAZY_GS */
// /*AFLA*/ #  define get_user_gs(regs)		(u16)((regs)->gs)
// /*AFLA*/ #  define set_user_gs(regs, v)		do { (regs)->gs = (v); } while (0)
// /*AFLA*/ #  define task_user_gs(tsk)		(task_pt_regs(tsk)->gs)
// /*AFLA*/ #  define lazy_save_gs(v)		do { } while (0)
// /*AFLA*/ #  define lazy_load_gs(v)		do { } while (0)
// /*AFLA*/ # endif	/* X86_32_LAZY_GS */
// /*AFLA*/ #endif	/* X86_32 */
// /*AFLA*/ 
// /*AFLA*/ #endif /* !__ASSEMBLY__ */
// /*AFLA*/ #endif /* __KERNEL__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_SEGMENT_H */
