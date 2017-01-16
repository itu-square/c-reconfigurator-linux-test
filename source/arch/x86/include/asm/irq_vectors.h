// /*AFLA*/ #ifndef _ASM_X86_IRQ_VECTORS_H
// /*AFLA*/ #define _ASM_X86_IRQ_VECTORS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/threads.h>
// /*AFLA*/ /*
// /*AFLA*/  * Linux IRQ vector layout.
// /*AFLA*/  *
// /*AFLA*/  * There are 256 IDT entries (per CPU - each entry is 8 bytes) which can
// /*AFLA*/  * be defined by Linux. They are used as a jump table by the CPU when a
// /*AFLA*/  * given vector is triggered - by a CPU-external, CPU-internal or
// /*AFLA*/  * software-triggered event.
// /*AFLA*/  *
// /*AFLA*/  * Linux sets the kernel code address each entry jumps to early during
// /*AFLA*/  * bootup, and never changes them. This is the general layout of the
// /*AFLA*/  * IDT entries:
// /*AFLA*/  *
// /*AFLA*/  *  Vectors   0 ...  31 : system traps and exceptions - hardcoded events
// /*AFLA*/  *  Vectors  32 ... 127 : device interrupts
// /*AFLA*/  *  Vector  128         : legacy int80 syscall interface
// /*AFLA*/  *  Vectors 129 ... INVALIDATE_TLB_VECTOR_START-1 except 204 : device interrupts
// /*AFLA*/  *  Vectors INVALIDATE_TLB_VECTOR_START ... 255 : special interrupts
// /*AFLA*/  *
// /*AFLA*/  * 64-bit x86 has per CPU IDT tables, 32-bit has one shared IDT table.
// /*AFLA*/  *
// /*AFLA*/  * This file enumerates the exact layout of them:
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define NMI_VECTOR			0x02
// /*AFLA*/ #define MCE_VECTOR			0x12
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * IDT vectors usable for external interrupt sources start at 0x20.
// /*AFLA*/  * (0x80 is the syscall vector, 0x30-0x3f are for ISA)
// /*AFLA*/  */
// /*AFLA*/ #define FIRST_EXTERNAL_VECTOR		0x20
// /*AFLA*/ /*
// /*AFLA*/  * We start allocating at 0x21 to spread out vectors evenly between
// /*AFLA*/  * priority levels. (0x80 is the syscall vector)
// /*AFLA*/  */
// /*AFLA*/ #define VECTOR_OFFSET_START		1
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Reserve the lowest usable vector (and hence lowest priority)  0x20 for
// /*AFLA*/  * triggering cleanup after irq migration. 0x21-0x2f will still be used
// /*AFLA*/  * for device interrupts.
// /*AFLA*/  */
// /*AFLA*/ #define IRQ_MOVE_CLEANUP_VECTOR		FIRST_EXTERNAL_VECTOR
// /*AFLA*/ 
// /*AFLA*/ #define IA32_SYSCALL_VECTOR		0x80
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Vectors 0x30-0x3f are used for ISA interrupts.
// /*AFLA*/  *   round up to the next 16-vector boundary
// /*AFLA*/  */
// /*AFLA*/ #define ISA_IRQ_VECTOR(irq)		(((FIRST_EXTERNAL_VECTOR + 16) & ~15) + irq)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Special IRQ vectors used by the SMP architecture, 0xf0-0xff
// /*AFLA*/  *
// /*AFLA*/  *  some of the following vectors are 'rare', they are merged
// /*AFLA*/  *  into a single vector (CALL_FUNCTION_VECTOR) to save vector space.
// /*AFLA*/  *  TLB, reschedule and local APIC vectors are performance-critical.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define SPURIOUS_APIC_VECTOR		0xff
// /*AFLA*/ /*
// /*AFLA*/  * Sanity check
// /*AFLA*/  */
// /*AFLA*/ #if ((SPURIOUS_APIC_VECTOR & 0x0F) != 0x0F)
// /*AFLA*/ # error SPURIOUS_APIC_VECTOR definition error
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define ERROR_APIC_VECTOR		0xfe
// /*AFLA*/ #define RESCHEDULE_VECTOR		0xfd
// /*AFLA*/ #define CALL_FUNCTION_VECTOR		0xfc
// /*AFLA*/ #define CALL_FUNCTION_SINGLE_VECTOR	0xfb
// /*AFLA*/ #define THERMAL_APIC_VECTOR		0xfa
// /*AFLA*/ #define THRESHOLD_APIC_VECTOR		0xf9
// /*AFLA*/ #define REBOOT_VECTOR			0xf8
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Generic system vector for platform specific use
// /*AFLA*/  */
// /*AFLA*/ #define X86_PLATFORM_IPI_VECTOR		0xf7
// /*AFLA*/ 
// /*AFLA*/ #define POSTED_INTR_WAKEUP_VECTOR	0xf1
// /*AFLA*/ /*
// /*AFLA*/  * IRQ work vector:
// /*AFLA*/  */
// /*AFLA*/ #define IRQ_WORK_VECTOR			0xf6
// /*AFLA*/ 
// /*AFLA*/ #define UV_BAU_MESSAGE			0xf5
// /*AFLA*/ #define DEFERRED_ERROR_VECTOR		0xf4
// /*AFLA*/ 
// /*AFLA*/ /* Vector on which hypervisor callbacks will be delivered */
// /*AFLA*/ #define HYPERVISOR_CALLBACK_VECTOR	0xf3
// /*AFLA*/ 
// /*AFLA*/ /* Vector for KVM to deliver posted interrupt IPI */
// /*AFLA*/ #ifdef CONFIG_HAVE_KVM
// /*AFLA*/ #define POSTED_INTR_VECTOR		0xf2
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Local APIC timer IRQ vector is on a different priority level,
// /*AFLA*/  * to work around the 'lost local interrupt if more than 2 IRQ
// /*AFLA*/  * sources per level' errata.
// /*AFLA*/  */
// /*AFLA*/ #define LOCAL_TIMER_VECTOR		0xef
// /*AFLA*/ 
// /*AFLA*/ #define NR_VECTORS			 256
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_LOCAL_APIC
// /*AFLA*/ #define FIRST_SYSTEM_VECTOR		LOCAL_TIMER_VECTOR
// /*AFLA*/ #else
// /*AFLA*/ #define FIRST_SYSTEM_VECTOR		NR_VECTORS
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define FPU_IRQ				  13
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Size the maximum number of interrupts.
// /*AFLA*/  *
// /*AFLA*/  * If the irq_desc[] array has a sparse layout, we can size things
// /*AFLA*/  * generously - it scales up linearly with the maximum number of CPUs,
// /*AFLA*/  * and the maximum number of IO-APICs, whichever is higher.
// /*AFLA*/  *
// /*AFLA*/  * In other cases we size more conservatively, to not create too large
// /*AFLA*/  * static arrays.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define NR_IRQS_LEGACY			16
// /*AFLA*/ 
// /*AFLA*/ #define CPU_VECTOR_LIMIT		(64 * NR_CPUS)
// /*AFLA*/ #define IO_APIC_VECTOR_LIMIT		(32 * MAX_IO_APICS)
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_X86_IO_APIC) && defined(CONFIG_PCI_MSI)
// /*AFLA*/ #define NR_IRQS						\
// /*AFLA*/ 	(CPU_VECTOR_LIMIT > IO_APIC_VECTOR_LIMIT ?	\
// /*AFLA*/ 		(NR_VECTORS + CPU_VECTOR_LIMIT)  :	\
// /*AFLA*/ 		(NR_VECTORS + IO_APIC_VECTOR_LIMIT))
// /*AFLA*/ #elif defined(CONFIG_X86_IO_APIC)
// /*AFLA*/ #define	NR_IRQS				(NR_VECTORS + IO_APIC_VECTOR_LIMIT)
// /*AFLA*/ #elif defined(CONFIG_PCI_MSI)
// /*AFLA*/ #define NR_IRQS				(NR_VECTORS + CPU_VECTOR_LIMIT)
// /*AFLA*/ #else
// /*AFLA*/ #define NR_IRQS				NR_IRQS_LEGACY
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_IRQ_VECTORS_H */
