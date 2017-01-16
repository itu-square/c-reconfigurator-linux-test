// /*AFLA*/ #ifndef _ASM_X86_APICDEF_H
// /*AFLA*/ #define _ASM_X86_APICDEF_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Constants for various Intel APICs. (local APIC, IOAPIC, etc.)
// /*AFLA*/  *
// /*AFLA*/  * Alan Cox <Alan.Cox@linux.org>, 1995.
// /*AFLA*/  * Ingo Molnar <mingo@redhat.com>, 1999, 2000
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define IO_APIC_DEFAULT_PHYS_BASE	0xfec00000
// /*AFLA*/ #define	APIC_DEFAULT_PHYS_BASE		0xfee00000
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is the IO-APIC register space as specified
// /*AFLA*/  * by Intel docs:
// /*AFLA*/  */
// /*AFLA*/ #define IO_APIC_SLOT_SIZE		1024
// /*AFLA*/ 
// /*AFLA*/ #define	APIC_ID		0x20
// /*AFLA*/ 
// /*AFLA*/ #define	APIC_LVR	0x30
// /*AFLA*/ #define		APIC_LVR_MASK		0xFF00FF
// /*AFLA*/ #define		APIC_LVR_DIRECTED_EOI	(1 << 24)
// /*AFLA*/ #define		GET_APIC_VERSION(x)	((x) & 0xFFu)
// /*AFLA*/ #define		GET_APIC_MAXLVT(x)	(((x) >> 16) & 0xFFu)
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ #  define	APIC_INTEGRATED(x)	((x) & 0xF0u)
// /*AFLA*/ #else
// /*AFLA*/ #  define	APIC_INTEGRATED(x)	(1)
// /*AFLA*/ #endif
// /*AFLA*/ #define		APIC_XAPIC(x)		((x) >= 0x14)
// /*AFLA*/ #define		APIC_EXT_SPACE(x)	((x) & 0x80000000)
// /*AFLA*/ #define	APIC_TASKPRI	0x80
// /*AFLA*/ #define		APIC_TPRI_MASK		0xFFu
// /*AFLA*/ #define	APIC_ARBPRI	0x90
// /*AFLA*/ #define		APIC_ARBPRI_MASK	0xFFu
// /*AFLA*/ #define	APIC_PROCPRI	0xA0
// /*AFLA*/ #define	APIC_EOI	0xB0
// /*AFLA*/ #define		APIC_EOI_ACK		0x0 /* Docs say 0 for future compat. */
// /*AFLA*/ #define	APIC_RRR	0xC0
// /*AFLA*/ #define	APIC_LDR	0xD0
// /*AFLA*/ #define		APIC_LDR_MASK		(0xFFu << 24)
// /*AFLA*/ #define		GET_APIC_LOGICAL_ID(x)	(((x) >> 24) & 0xFFu)
// /*AFLA*/ #define		SET_APIC_LOGICAL_ID(x)	(((x) << 24))
// /*AFLA*/ #define		APIC_ALL_CPUS		0xFFu
// /*AFLA*/ #define	APIC_DFR	0xE0
// /*AFLA*/ #define		APIC_DFR_CLUSTER		0x0FFFFFFFul
// /*AFLA*/ #define		APIC_DFR_FLAT			0xFFFFFFFFul
// /*AFLA*/ #define	APIC_SPIV	0xF0
// /*AFLA*/ #define		APIC_SPIV_DIRECTED_EOI		(1 << 12)
// /*AFLA*/ #define		APIC_SPIV_FOCUS_DISABLED	(1 << 9)
// /*AFLA*/ #define		APIC_SPIV_APIC_ENABLED		(1 << 8)
// /*AFLA*/ #define	APIC_ISR	0x100
// /*AFLA*/ #define	APIC_ISR_NR     0x8     /* Number of 32 bit ISR registers. */
// /*AFLA*/ #define	APIC_TMR	0x180
// /*AFLA*/ #define	APIC_IRR	0x200
// /*AFLA*/ #define	APIC_ESR	0x280
// /*AFLA*/ #define		APIC_ESR_SEND_CS	0x00001
// /*AFLA*/ #define		APIC_ESR_RECV_CS	0x00002
// /*AFLA*/ #define		APIC_ESR_SEND_ACC	0x00004
// /*AFLA*/ #define		APIC_ESR_RECV_ACC	0x00008
// /*AFLA*/ #define		APIC_ESR_SENDILL	0x00020
// /*AFLA*/ #define		APIC_ESR_RECVILL	0x00040
// /*AFLA*/ #define		APIC_ESR_ILLREGA	0x00080
// /*AFLA*/ #define 	APIC_LVTCMCI	0x2f0
// /*AFLA*/ #define	APIC_ICR	0x300
// /*AFLA*/ #define		APIC_DEST_SELF		0x40000
// /*AFLA*/ #define		APIC_DEST_ALLINC	0x80000
// /*AFLA*/ #define		APIC_DEST_ALLBUT	0xC0000
// /*AFLA*/ #define		APIC_ICR_RR_MASK	0x30000
// /*AFLA*/ #define		APIC_ICR_RR_INVALID	0x00000
// /*AFLA*/ #define		APIC_ICR_RR_INPROG	0x10000
// /*AFLA*/ #define		APIC_ICR_RR_VALID	0x20000
// /*AFLA*/ #define		APIC_INT_LEVELTRIG	0x08000
// /*AFLA*/ #define		APIC_INT_ASSERT		0x04000
// /*AFLA*/ #define		APIC_ICR_BUSY		0x01000
// /*AFLA*/ #define		APIC_DEST_LOGICAL	0x00800
// /*AFLA*/ #define		APIC_DEST_PHYSICAL	0x00000
// /*AFLA*/ #define		APIC_DM_FIXED		0x00000
// /*AFLA*/ #define		APIC_DM_FIXED_MASK	0x00700
// /*AFLA*/ #define		APIC_DM_LOWEST		0x00100
// /*AFLA*/ #define		APIC_DM_SMI		0x00200
// /*AFLA*/ #define		APIC_DM_REMRD		0x00300
// /*AFLA*/ #define		APIC_DM_NMI		0x00400
// /*AFLA*/ #define		APIC_DM_INIT		0x00500
// /*AFLA*/ #define		APIC_DM_STARTUP		0x00600
// /*AFLA*/ #define		APIC_DM_EXTINT		0x00700
// /*AFLA*/ #define		APIC_VECTOR_MASK	0x000FF
// /*AFLA*/ #define	APIC_ICR2	0x310
// /*AFLA*/ #define		GET_APIC_DEST_FIELD(x)	(((x) >> 24) & 0xFF)
// /*AFLA*/ #define		SET_APIC_DEST_FIELD(x)	((x) << 24)
// /*AFLA*/ #define	APIC_LVTT	0x320
// /*AFLA*/ #define	APIC_LVTTHMR	0x330
// /*AFLA*/ #define	APIC_LVTPC	0x340
// /*AFLA*/ #define	APIC_LVT0	0x350
// /*AFLA*/ #define		APIC_LVT_TIMER_BASE_MASK	(0x3 << 18)
// /*AFLA*/ #define		GET_APIC_TIMER_BASE(x)		(((x) >> 18) & 0x3)
// /*AFLA*/ #define		SET_APIC_TIMER_BASE(x)		(((x) << 18))
// /*AFLA*/ #define		APIC_TIMER_BASE_CLKIN		0x0
// /*AFLA*/ #define		APIC_TIMER_BASE_TMBASE		0x1
// /*AFLA*/ #define		APIC_TIMER_BASE_DIV		0x2
// /*AFLA*/ #define		APIC_LVT_TIMER_ONESHOT		(0 << 17)
// /*AFLA*/ #define		APIC_LVT_TIMER_PERIODIC		(1 << 17)
// /*AFLA*/ #define		APIC_LVT_TIMER_TSCDEADLINE	(2 << 17)
// /*AFLA*/ #define		APIC_LVT_MASKED			(1 << 16)
// /*AFLA*/ #define		APIC_LVT_LEVEL_TRIGGER		(1 << 15)
// /*AFLA*/ #define		APIC_LVT_REMOTE_IRR		(1 << 14)
// /*AFLA*/ #define		APIC_INPUT_POLARITY		(1 << 13)
// /*AFLA*/ #define		APIC_SEND_PENDING		(1 << 12)
// /*AFLA*/ #define		APIC_MODE_MASK			0x700
// /*AFLA*/ #define		GET_APIC_DELIVERY_MODE(x)	(((x) >> 8) & 0x7)
// /*AFLA*/ #define		SET_APIC_DELIVERY_MODE(x, y)	(((x) & ~0x700) | ((y) << 8))
// /*AFLA*/ #define			APIC_MODE_FIXED		0x0
// /*AFLA*/ #define			APIC_MODE_NMI		0x4
// /*AFLA*/ #define			APIC_MODE_EXTINT	0x7
// /*AFLA*/ #define	APIC_LVT1	0x360
// /*AFLA*/ #define	APIC_LVTERR	0x370
// /*AFLA*/ #define	APIC_TMICT	0x380
// /*AFLA*/ #define	APIC_TMCCT	0x390
// /*AFLA*/ #define	APIC_TDCR	0x3E0
// /*AFLA*/ #define APIC_SELF_IPI	0x3F0
// /*AFLA*/ #define		APIC_TDR_DIV_TMBASE	(1 << 2)
// /*AFLA*/ #define		APIC_TDR_DIV_1		0xB
// /*AFLA*/ #define		APIC_TDR_DIV_2		0x0
// /*AFLA*/ #define		APIC_TDR_DIV_4		0x1
// /*AFLA*/ #define		APIC_TDR_DIV_8		0x2
// /*AFLA*/ #define		APIC_TDR_DIV_16		0x3
// /*AFLA*/ #define		APIC_TDR_DIV_32		0x8
// /*AFLA*/ #define		APIC_TDR_DIV_64		0x9
// /*AFLA*/ #define		APIC_TDR_DIV_128	0xA
// /*AFLA*/ #define	APIC_EFEAT	0x400
// /*AFLA*/ #define	APIC_ECTRL	0x410
// /*AFLA*/ #define APIC_EILVTn(n)	(0x500 + 0x10 * n)
// /*AFLA*/ #define		APIC_EILVT_NR_AMD_K8	1	/* # of extended interrupts */
// /*AFLA*/ #define		APIC_EILVT_NR_AMD_10H	4
// /*AFLA*/ #define		APIC_EILVT_NR_MAX	APIC_EILVT_NR_AMD_10H
// /*AFLA*/ #define		APIC_EILVT_LVTOFF(x)	(((x) >> 4) & 0xF)
// /*AFLA*/ #define		APIC_EILVT_MSG_FIX	0x0
// /*AFLA*/ #define		APIC_EILVT_MSG_SMI	0x2
// /*AFLA*/ #define		APIC_EILVT_MSG_NMI	0x4
// /*AFLA*/ #define		APIC_EILVT_MSG_EXT	0x7
// /*AFLA*/ #define		APIC_EILVT_MASKED	(1 << 16)
// /*AFLA*/ 
// /*AFLA*/ #define APIC_BASE (fix_to_virt(FIX_APIC_BASE))
// /*AFLA*/ #define APIC_BASE_MSR	0x800
// /*AFLA*/ #define XAPIC_ENABLE	(1UL << 11)
// /*AFLA*/ #define X2APIC_ENABLE	(1UL << 10)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ # define MAX_IO_APICS 64
// /*AFLA*/ # define MAX_LOCAL_APIC 256
// /*AFLA*/ #else
// /*AFLA*/ # define MAX_IO_APICS 128
// /*AFLA*/ # define MAX_LOCAL_APIC 32768
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * All x86-64 systems are xAPIC compatible.
// /*AFLA*/  * In the following, "apicid" is a physical APIC ID.
// /*AFLA*/  */
// /*AFLA*/ #define XAPIC_DEST_CPUS_SHIFT	4
// /*AFLA*/ #define XAPIC_DEST_CPUS_MASK	((1u << XAPIC_DEST_CPUS_SHIFT) - 1)
// /*AFLA*/ #define XAPIC_DEST_CLUSTER_MASK	(XAPIC_DEST_CPUS_MASK << XAPIC_DEST_CPUS_SHIFT)
// /*AFLA*/ #define APIC_CLUSTER(apicid)	((apicid) & XAPIC_DEST_CLUSTER_MASK)
// /*AFLA*/ #define APIC_CLUSTERID(apicid)	(APIC_CLUSTER(apicid) >> XAPIC_DEST_CPUS_SHIFT)
// /*AFLA*/ #define APIC_CPUID(apicid)	((apicid) & XAPIC_DEST_CPUS_MASK)
// /*AFLA*/ #define NUM_APIC_CLUSTERS	((BAD_APICID + 1) >> XAPIC_DEST_CPUS_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * the local APIC register structure, memory mapped. Not terribly well
// /*AFLA*/  * tested, but we might eventually use this one in the future - the
// /*AFLA*/  * problem why we cannot use it right now is the P5 APIC, it has an
// /*AFLA*/  * errata which cannot take 8-bit reads and writes, only 32-bit ones ...
// /*AFLA*/  */
// /*AFLA*/ #define u32 unsigned int
// /*AFLA*/ 
// /*AFLA*/ struct local_apic {
// /*AFLA*/ 
// /*AFLA*/ /*000*/	struct { u32 __reserved[4]; } __reserved_01;
// /*AFLA*/ 
// /*AFLA*/ /*010*/	struct { u32 __reserved[4]; } __reserved_02;
// /*AFLA*/ 
// /*AFLA*/ /*020*/	struct { /* APIC ID Register */
// /*AFLA*/ 		u32   __reserved_1	: 24,
// /*AFLA*/ 			phys_apic_id	:  4,
// /*AFLA*/ 			__reserved_2	:  4;
// /*AFLA*/ 		u32 __reserved[3];
// /*AFLA*/ 	} id;
// /*AFLA*/ 
// /*AFLA*/ /*030*/	const
// /*AFLA*/ 	struct { /* APIC Version Register */
// /*AFLA*/ 		u32   version		:  8,
// /*AFLA*/ 			__reserved_1	:  8,
// /*AFLA*/ 			max_lvt		:  8,
// /*AFLA*/ 			__reserved_2	:  8;
// /*AFLA*/ 		u32 __reserved[3];
// /*AFLA*/ 	} version;
// /*AFLA*/ 
// /*AFLA*/ /*040*/	struct { u32 __reserved[4]; } __reserved_03;
// /*AFLA*/ 
// /*AFLA*/ /*050*/	struct { u32 __reserved[4]; } __reserved_04;
// /*AFLA*/ 
// /*AFLA*/ /*060*/	struct { u32 __reserved[4]; } __reserved_05;
// /*AFLA*/ 
// /*AFLA*/ /*070*/	struct { u32 __reserved[4]; } __reserved_06;
// /*AFLA*/ 
// /*AFLA*/ /*080*/	struct { /* Task Priority Register */
// /*AFLA*/ 		u32   priority	:  8,
// /*AFLA*/ 			__reserved_1	: 24;
// /*AFLA*/ 		u32 __reserved_2[3];
// /*AFLA*/ 	} tpr;
// /*AFLA*/ 
// /*AFLA*/ /*090*/	const
// /*AFLA*/ 	struct { /* Arbitration Priority Register */
// /*AFLA*/ 		u32   priority	:  8,
// /*AFLA*/ 			__reserved_1	: 24;
// /*AFLA*/ 		u32 __reserved_2[3];
// /*AFLA*/ 	} apr;
// /*AFLA*/ 
// /*AFLA*/ /*0A0*/	const
// /*AFLA*/ 	struct { /* Processor Priority Register */
// /*AFLA*/ 		u32   priority	:  8,
// /*AFLA*/ 			__reserved_1	: 24;
// /*AFLA*/ 		u32 __reserved_2[3];
// /*AFLA*/ 	} ppr;
// /*AFLA*/ 
// /*AFLA*/ /*0B0*/	struct { /* End Of Interrupt Register */
// /*AFLA*/ 		u32   eoi;
// /*AFLA*/ 		u32 __reserved[3];
// /*AFLA*/ 	} eoi;
// /*AFLA*/ 
// /*AFLA*/ /*0C0*/	struct { u32 __reserved[4]; } __reserved_07;
// /*AFLA*/ 
// /*AFLA*/ /*0D0*/	struct { /* Logical Destination Register */
// /*AFLA*/ 		u32   __reserved_1	: 24,
// /*AFLA*/ 			logical_dest	:  8;
// /*AFLA*/ 		u32 __reserved_2[3];
// /*AFLA*/ 	} ldr;
// /*AFLA*/ 
// /*AFLA*/ /*0E0*/	struct { /* Destination Format Register */
// /*AFLA*/ 		u32   __reserved_1	: 28,
// /*AFLA*/ 			model		:  4;
// /*AFLA*/ 		u32 __reserved_2[3];
// /*AFLA*/ 	} dfr;
// /*AFLA*/ 
// /*AFLA*/ /*0F0*/	struct { /* Spurious Interrupt Vector Register */
// /*AFLA*/ 		u32	spurious_vector	:  8,
// /*AFLA*/ 			apic_enabled	:  1,
// /*AFLA*/ 			focus_cpu	:  1,
// /*AFLA*/ 			__reserved_2	: 22;
// /*AFLA*/ 		u32 __reserved_3[3];
// /*AFLA*/ 	} svr;
// /*AFLA*/ 
// /*AFLA*/ /*100*/	struct { /* In Service Register */
// /*AFLA*/ /*170*/		u32 bitfield;
// /*AFLA*/ 		u32 __reserved[3];
// /*AFLA*/ 	} isr [8];
// /*AFLA*/ 
// /*AFLA*/ /*180*/	struct { /* Trigger Mode Register */
// /*AFLA*/ /*1F0*/		u32 bitfield;
// /*AFLA*/ 		u32 __reserved[3];
// /*AFLA*/ 	} tmr [8];
// /*AFLA*/ 
// /*AFLA*/ /*200*/	struct { /* Interrupt Request Register */
// /*AFLA*/ /*270*/		u32 bitfield;
// /*AFLA*/ 		u32 __reserved[3];
// /*AFLA*/ 	} irr [8];
// /*AFLA*/ 
// /*AFLA*/ /*280*/	union { /* Error Status Register */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			u32   send_cs_error			:  1,
// /*AFLA*/ 				receive_cs_error		:  1,
// /*AFLA*/ 				send_accept_error		:  1,
// /*AFLA*/ 				receive_accept_error		:  1,
// /*AFLA*/ 				__reserved_1			:  1,
// /*AFLA*/ 				send_illegal_vector		:  1,
// /*AFLA*/ 				receive_illegal_vector		:  1,
// /*AFLA*/ 				illegal_register_address	:  1,
// /*AFLA*/ 				__reserved_2			: 24;
// /*AFLA*/ 			u32 __reserved_3[3];
// /*AFLA*/ 		} error_bits;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			u32 errors;
// /*AFLA*/ 			u32 __reserved_3[3];
// /*AFLA*/ 		} all_errors;
// /*AFLA*/ 	} esr;
// /*AFLA*/ 
// /*AFLA*/ /*290*/	struct { u32 __reserved[4]; } __reserved_08;
// /*AFLA*/ 
// /*AFLA*/ /*2A0*/	struct { u32 __reserved[4]; } __reserved_09;
// /*AFLA*/ 
// /*AFLA*/ /*2B0*/	struct { u32 __reserved[4]; } __reserved_10;
// /*AFLA*/ 
// /*AFLA*/ /*2C0*/	struct { u32 __reserved[4]; } __reserved_11;
// /*AFLA*/ 
// /*AFLA*/ /*2D0*/	struct { u32 __reserved[4]; } __reserved_12;
// /*AFLA*/ 
// /*AFLA*/ /*2E0*/	struct { u32 __reserved[4]; } __reserved_13;
// /*AFLA*/ 
// /*AFLA*/ /*2F0*/	struct { u32 __reserved[4]; } __reserved_14;
// /*AFLA*/ 
// /*AFLA*/ /*300*/	struct { /* Interrupt Command Register 1 */
// /*AFLA*/ 		u32   vector			:  8,
// /*AFLA*/ 			delivery_mode		:  3,
// /*AFLA*/ 			destination_mode	:  1,
// /*AFLA*/ 			delivery_status		:  1,
// /*AFLA*/ 			__reserved_1		:  1,
// /*AFLA*/ 			level			:  1,
// /*AFLA*/ 			trigger			:  1,
// /*AFLA*/ 			__reserved_2		:  2,
// /*AFLA*/ 			shorthand		:  2,
// /*AFLA*/ 			__reserved_3		:  12;
// /*AFLA*/ 		u32 __reserved_4[3];
// /*AFLA*/ 	} icr1;
// /*AFLA*/ 
// /*AFLA*/ /*310*/	struct { /* Interrupt Command Register 2 */
// /*AFLA*/ 		union {
// /*AFLA*/ 			u32   __reserved_1	: 24,
// /*AFLA*/ 				phys_dest	:  4,
// /*AFLA*/ 				__reserved_2	:  4;
// /*AFLA*/ 			u32   __reserved_3	: 24,
// /*AFLA*/ 				logical_dest	:  8;
// /*AFLA*/ 		} dest;
// /*AFLA*/ 		u32 __reserved_4[3];
// /*AFLA*/ 	} icr2;
// /*AFLA*/ 
// /*AFLA*/ /*320*/	struct { /* LVT - Timer */
// /*AFLA*/ 		u32   vector		:  8,
// /*AFLA*/ 			__reserved_1	:  4,
// /*AFLA*/ 			delivery_status	:  1,
// /*AFLA*/ 			__reserved_2	:  3,
// /*AFLA*/ 			mask		:  1,
// /*AFLA*/ 			timer_mode	:  1,
// /*AFLA*/ 			__reserved_3	: 14;
// /*AFLA*/ 		u32 __reserved_4[3];
// /*AFLA*/ 	} lvt_timer;
// /*AFLA*/ 
// /*AFLA*/ /*330*/	struct { /* LVT - Thermal Sensor */
// /*AFLA*/ 		u32  vector		:  8,
// /*AFLA*/ 			delivery_mode	:  3,
// /*AFLA*/ 			__reserved_1	:  1,
// /*AFLA*/ 			delivery_status	:  1,
// /*AFLA*/ 			__reserved_2	:  3,
// /*AFLA*/ 			mask		:  1,
// /*AFLA*/ 			__reserved_3	: 15;
// /*AFLA*/ 		u32 __reserved_4[3];
// /*AFLA*/ 	} lvt_thermal;
// /*AFLA*/ 
// /*AFLA*/ /*340*/	struct { /* LVT - Performance Counter */
// /*AFLA*/ 		u32   vector		:  8,
// /*AFLA*/ 			delivery_mode	:  3,
// /*AFLA*/ 			__reserved_1	:  1,
// /*AFLA*/ 			delivery_status	:  1,
// /*AFLA*/ 			__reserved_2	:  3,
// /*AFLA*/ 			mask		:  1,
// /*AFLA*/ 			__reserved_3	: 15;
// /*AFLA*/ 		u32 __reserved_4[3];
// /*AFLA*/ 	} lvt_pc;
// /*AFLA*/ 
// /*AFLA*/ /*350*/	struct { /* LVT - LINT0 */
// /*AFLA*/ 		u32   vector		:  8,
// /*AFLA*/ 			delivery_mode	:  3,
// /*AFLA*/ 			__reserved_1	:  1,
// /*AFLA*/ 			delivery_status	:  1,
// /*AFLA*/ 			polarity	:  1,
// /*AFLA*/ 			remote_irr	:  1,
// /*AFLA*/ 			trigger		:  1,
// /*AFLA*/ 			mask		:  1,
// /*AFLA*/ 			__reserved_2	: 15;
// /*AFLA*/ 		u32 __reserved_3[3];
// /*AFLA*/ 	} lvt_lint0;
// /*AFLA*/ 
// /*AFLA*/ /*360*/	struct { /* LVT - LINT1 */
// /*AFLA*/ 		u32   vector		:  8,
// /*AFLA*/ 			delivery_mode	:  3,
// /*AFLA*/ 			__reserved_1	:  1,
// /*AFLA*/ 			delivery_status	:  1,
// /*AFLA*/ 			polarity	:  1,
// /*AFLA*/ 			remote_irr	:  1,
// /*AFLA*/ 			trigger		:  1,
// /*AFLA*/ 			mask		:  1,
// /*AFLA*/ 			__reserved_2	: 15;
// /*AFLA*/ 		u32 __reserved_3[3];
// /*AFLA*/ 	} lvt_lint1;
// /*AFLA*/ 
// /*AFLA*/ /*370*/	struct { /* LVT - Error */
// /*AFLA*/ 		u32   vector		:  8,
// /*AFLA*/ 			__reserved_1	:  4,
// /*AFLA*/ 			delivery_status	:  1,
// /*AFLA*/ 			__reserved_2	:  3,
// /*AFLA*/ 			mask		:  1,
// /*AFLA*/ 			__reserved_3	: 15;
// /*AFLA*/ 		u32 __reserved_4[3];
// /*AFLA*/ 	} lvt_error;
// /*AFLA*/ 
// /*AFLA*/ /*380*/	struct { /* Timer Initial Count Register */
// /*AFLA*/ 		u32   initial_count;
// /*AFLA*/ 		u32 __reserved_2[3];
// /*AFLA*/ 	} timer_icr;
// /*AFLA*/ 
// /*AFLA*/ /*390*/	const
// /*AFLA*/ 	struct { /* Timer Current Count Register */
// /*AFLA*/ 		u32   curr_count;
// /*AFLA*/ 		u32 __reserved_2[3];
// /*AFLA*/ 	} timer_ccr;
// /*AFLA*/ 
// /*AFLA*/ /*3A0*/	struct { u32 __reserved[4]; } __reserved_16;
// /*AFLA*/ 
// /*AFLA*/ /*3B0*/	struct { u32 __reserved[4]; } __reserved_17;
// /*AFLA*/ 
// /*AFLA*/ /*3C0*/	struct { u32 __reserved[4]; } __reserved_18;
// /*AFLA*/ 
// /*AFLA*/ /*3D0*/	struct { u32 __reserved[4]; } __reserved_19;
// /*AFLA*/ 
// /*AFLA*/ /*3E0*/	struct { /* Timer Divide Configuration Register */
// /*AFLA*/ 		u32   divisor		:  4,
// /*AFLA*/ 			__reserved_1	: 28;
// /*AFLA*/ 		u32 __reserved_2[3];
// /*AFLA*/ 	} timer_dcr;
// /*AFLA*/ 
// /*AFLA*/ /*3F0*/	struct { u32 __reserved[4]; } __reserved_20;
// /*AFLA*/ 
// /*AFLA*/ } __attribute__ ((packed));
// /*AFLA*/ 
// /*AFLA*/ #undef u32
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/  #define BAD_APICID 0xFFu
// /*AFLA*/ #else
// /*AFLA*/  #define BAD_APICID 0xFFFFu
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ enum ioapic_irq_destination_types {
// /*AFLA*/ 	dest_Fixed		= 0,
// /*AFLA*/ 	dest_LowestPrio		= 1,
// /*AFLA*/ 	dest_SMI		= 2,
// /*AFLA*/ 	dest__reserved_1	= 3,
// /*AFLA*/ 	dest_NMI		= 4,
// /*AFLA*/ 	dest_INIT		= 5,
// /*AFLA*/ 	dest__reserved_2	= 6,
// /*AFLA*/ 	dest_ExtINT		= 7
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_APICDEF_H */
