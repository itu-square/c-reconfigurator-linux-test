// /*AFLA*/ #ifndef _ASM_X86_MPSPEC_DEF_H
// /*AFLA*/ #define _ASM_X86_MPSPEC_DEF_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Structure definitions for SMP machines following the
// /*AFLA*/  * Intel Multiprocessing Specification 1.1 and 1.4.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This tag identifies where the SMP configuration
// /*AFLA*/  * information is.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define SMP_MAGIC_IDENT	(('_'<<24) | ('P'<<16) | ('M'<<8) | '_')
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ # define MAX_MPC_ENTRY 1024
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Intel MP Floating Pointer Structure */
// /*AFLA*/ struct mpf_intel {
// /*AFLA*/ 	char signature[4];		/* "_MP_"			*/
// /*AFLA*/ 	unsigned int physptr;		/* Configuration table address	*/
// /*AFLA*/ 	unsigned char length;		/* Our length (paragraphs)	*/
// /*AFLA*/ 	unsigned char specification;	/* Specification version	*/
// /*AFLA*/ 	unsigned char checksum;		/* Checksum (makes sum 0)	*/
// /*AFLA*/ 	unsigned char feature1;		/* Standard or configuration ?	*/
// /*AFLA*/ 	unsigned char feature2;		/* Bit7 set for IMCR|PIC	*/
// /*AFLA*/ 	unsigned char feature3;		/* Unused (0)			*/
// /*AFLA*/ 	unsigned char feature4;		/* Unused (0)			*/
// /*AFLA*/ 	unsigned char feature5;		/* Unused (0)			*/
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define MPC_SIGNATURE "PCMP"
// /*AFLA*/ 
// /*AFLA*/ struct mpc_table {
// /*AFLA*/ 	char signature[4];
// /*AFLA*/ 	unsigned short length;		/* Size of table */
// /*AFLA*/ 	char spec;			/* 0x01 */
// /*AFLA*/ 	char checksum;
// /*AFLA*/ 	char oem[8];
// /*AFLA*/ 	char productid[12];
// /*AFLA*/ 	unsigned int oemptr;		/* 0 if not present */
// /*AFLA*/ 	unsigned short oemsize;		/* 0 if not present */
// /*AFLA*/ 	unsigned short oemcount;
// /*AFLA*/ 	unsigned int lapic;		/* APIC address */
// /*AFLA*/ 	unsigned int reserved;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Followed by entries */
// /*AFLA*/ 
// /*AFLA*/ #define	MP_PROCESSOR		0
// /*AFLA*/ #define	MP_BUS			1
// /*AFLA*/ #define	MP_IOAPIC		2
// /*AFLA*/ #define	MP_INTSRC		3
// /*AFLA*/ #define	MP_LINTSRC		4
// /*AFLA*/ /* Used by IBM NUMA-Q to describe node locality */
// /*AFLA*/ #define	MP_TRANSLATION		192
// /*AFLA*/ 
// /*AFLA*/ #define CPU_ENABLED		1	/* Processor is available */
// /*AFLA*/ #define CPU_BOOTPROCESSOR	2	/* Processor is the BP */
// /*AFLA*/ 
// /*AFLA*/ #define CPU_STEPPING_MASK	0x000F
// /*AFLA*/ #define CPU_MODEL_MASK		0x00F0
// /*AFLA*/ #define CPU_FAMILY_MASK		0x0F00
// /*AFLA*/ 
// /*AFLA*/ struct mpc_cpu {
// /*AFLA*/ 	unsigned char type;
// /*AFLA*/ 	unsigned char apicid;		/* Local APIC number */
// /*AFLA*/ 	unsigned char apicver;		/* Its versions */
// /*AFLA*/ 	unsigned char cpuflag;
// /*AFLA*/ 	unsigned int cpufeature;
// /*AFLA*/ 	unsigned int featureflag;	/* CPUID feature value */
// /*AFLA*/ 	unsigned int reserved[2];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct mpc_bus {
// /*AFLA*/ 	unsigned char type;
// /*AFLA*/ 	unsigned char busid;
// /*AFLA*/ 	unsigned char bustype[6];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* List of Bus Type string values, Intel MP Spec. */
// /*AFLA*/ #define BUSTYPE_EISA	"EISA"
// /*AFLA*/ #define BUSTYPE_ISA	"ISA"
// /*AFLA*/ #define BUSTYPE_INTERN	"INTERN"	/* Internal BUS */
// /*AFLA*/ #define BUSTYPE_MCA	"MCA"		/* Obsolete */
// /*AFLA*/ #define BUSTYPE_VL	"VL"		/* Local bus */
// /*AFLA*/ #define BUSTYPE_PCI	"PCI"
// /*AFLA*/ #define BUSTYPE_PCMCIA	"PCMCIA"
// /*AFLA*/ #define BUSTYPE_CBUS	"CBUS"
// /*AFLA*/ #define BUSTYPE_CBUSII	"CBUSII"
// /*AFLA*/ #define BUSTYPE_FUTURE	"FUTURE"
// /*AFLA*/ #define BUSTYPE_MBI	"MBI"
// /*AFLA*/ #define BUSTYPE_MBII	"MBII"
// /*AFLA*/ #define BUSTYPE_MPI	"MPI"
// /*AFLA*/ #define BUSTYPE_MPSA	"MPSA"
// /*AFLA*/ #define BUSTYPE_NUBUS	"NUBUS"
// /*AFLA*/ #define BUSTYPE_TC	"TC"
// /*AFLA*/ #define BUSTYPE_VME	"VME"
// /*AFLA*/ #define BUSTYPE_XPRESS	"XPRESS"
// /*AFLA*/ 
// /*AFLA*/ #define MPC_APIC_USABLE		0x01
// /*AFLA*/ 
// /*AFLA*/ struct mpc_ioapic {
// /*AFLA*/ 	unsigned char type;
// /*AFLA*/ 	unsigned char apicid;
// /*AFLA*/ 	unsigned char apicver;
// /*AFLA*/ 	unsigned char flags;
// /*AFLA*/ 	unsigned int apicaddr;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct mpc_intsrc {
// /*AFLA*/ 	unsigned char type;
// /*AFLA*/ 	unsigned char irqtype;
// /*AFLA*/ 	unsigned short irqflag;
// /*AFLA*/ 	unsigned char srcbus;
// /*AFLA*/ 	unsigned char srcbusirq;
// /*AFLA*/ 	unsigned char dstapic;
// /*AFLA*/ 	unsigned char dstirq;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum mp_irq_source_types {
// /*AFLA*/ 	mp_INT = 0,
// /*AFLA*/ 	mp_NMI = 1,
// /*AFLA*/ 	mp_SMI = 2,
// /*AFLA*/ 	mp_ExtINT = 3
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define MP_IRQDIR_DEFAULT	0
// /*AFLA*/ #define MP_IRQDIR_HIGH		1
// /*AFLA*/ #define MP_IRQDIR_LOW		3
// /*AFLA*/ 
// /*AFLA*/ #define MP_APIC_ALL	0xFF
// /*AFLA*/ 
// /*AFLA*/ struct mpc_lintsrc {
// /*AFLA*/ 	unsigned char type;
// /*AFLA*/ 	unsigned char irqtype;
// /*AFLA*/ 	unsigned short irqflag;
// /*AFLA*/ 	unsigned char srcbusid;
// /*AFLA*/ 	unsigned char srcbusirq;
// /*AFLA*/ 	unsigned char destapic;
// /*AFLA*/ 	unsigned char destapiclint;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define MPC_OEM_SIGNATURE "_OEM"
// /*AFLA*/ 
// /*AFLA*/ struct mpc_oemtable {
// /*AFLA*/ 	char signature[4];
// /*AFLA*/ 	unsigned short length;		/* Size of table */
// /*AFLA*/ 	char  rev;			/* 0x01 */
// /*AFLA*/ 	char  checksum;
// /*AFLA*/ 	char  mpc[8];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  *	Default configurations
// /*AFLA*/  *
// /*AFLA*/  *	1	2 CPU ISA 82489DX
// /*AFLA*/  *	2	2 CPU EISA 82489DX neither IRQ 0 timer nor IRQ 13 DMA chaining
// /*AFLA*/  *	3	2 CPU EISA 82489DX
// /*AFLA*/  *	4	2 CPU MCA 82489DX
// /*AFLA*/  *	5	2 CPU ISA+PCI
// /*AFLA*/  *	6	2 CPU EISA+PCI
// /*AFLA*/  *	7	2 CPU MCA+PCI
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ enum mp_bustype {
// /*AFLA*/ 	MP_BUS_ISA = 1,
// /*AFLA*/ 	MP_BUS_EISA,
// /*AFLA*/ 	MP_BUS_PCI,
// /*AFLA*/ };
// /*AFLA*/ #endif /* _ASM_X86_MPSPEC_DEF_H */
