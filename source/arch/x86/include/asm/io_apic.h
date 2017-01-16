// /*AFLA*/ #ifndef _ASM_X86_IO_APIC_H
// /*AFLA*/ #define _ASM_X86_IO_APIC_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <asm/mpspec.h>
// /*AFLA*/ #include <asm/apicdef.h>
// /*AFLA*/ #include <asm/irq_vectors.h>
// /*AFLA*/ #include <asm/x86_init.h>
// /*AFLA*/ /*
// /*AFLA*/  * Intel IO-APIC support for SMP and UP systems.
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 1997, 1998, 1999, 2000 Ingo Molnar
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* I/O Unit Redirection Table */
// /*AFLA*/ #define IO_APIC_REDIR_VECTOR_MASK	0x000FF
// /*AFLA*/ #define IO_APIC_REDIR_DEST_LOGICAL	0x00800
// /*AFLA*/ #define IO_APIC_REDIR_DEST_PHYSICAL	0x00000
// /*AFLA*/ #define IO_APIC_REDIR_SEND_PENDING	(1 << 12)
// /*AFLA*/ #define IO_APIC_REDIR_REMOTE_IRR	(1 << 14)
// /*AFLA*/ #define IO_APIC_REDIR_LEVEL_TRIGGER	(1 << 15)
// /*AFLA*/ #define IO_APIC_REDIR_MASKED		(1 << 16)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The structure of the IO-APIC:
// /*AFLA*/  */
// /*AFLA*/ union IO_APIC_reg_00 {
// /*AFLA*/ 	u32	raw;
// /*AFLA*/ 	struct {
// /*AFLA*/ 		u32	__reserved_2	: 14,
// /*AFLA*/ 			LTS		:  1,
// /*AFLA*/ 			delivery_type	:  1,
// /*AFLA*/ 			__reserved_1	:  8,
// /*AFLA*/ 			ID		:  8;
// /*AFLA*/ 	} __attribute__ ((packed)) bits;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ union IO_APIC_reg_01 {
// /*AFLA*/ 	u32	raw;
// /*AFLA*/ 	struct {
// /*AFLA*/ 		u32	version		:  8,
// /*AFLA*/ 			__reserved_2	:  7,
// /*AFLA*/ 			PRQ		:  1,
// /*AFLA*/ 			entries		:  8,
// /*AFLA*/ 			__reserved_1	:  8;
// /*AFLA*/ 	} __attribute__ ((packed)) bits;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ union IO_APIC_reg_02 {
// /*AFLA*/ 	u32	raw;
// /*AFLA*/ 	struct {
// /*AFLA*/ 		u32	__reserved_2	: 24,
// /*AFLA*/ 			arbitration	:  4,
// /*AFLA*/ 			__reserved_1	:  4;
// /*AFLA*/ 	} __attribute__ ((packed)) bits;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ union IO_APIC_reg_03 {
// /*AFLA*/ 	u32	raw;
// /*AFLA*/ 	struct {
// /*AFLA*/ 		u32	boot_DT		:  1,
// /*AFLA*/ 			__reserved_1	: 31;
// /*AFLA*/ 	} __attribute__ ((packed)) bits;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct IO_APIC_route_entry {
// /*AFLA*/ 	__u32	vector		:  8,
// /*AFLA*/ 		delivery_mode	:  3,	/* 000: FIXED
// /*AFLA*/ 					 * 001: lowest prio
// /*AFLA*/ 					 * 111: ExtINT
// /*AFLA*/ 					 */
// /*AFLA*/ 		dest_mode	:  1,	/* 0: physical, 1: logical */
// /*AFLA*/ 		delivery_status	:  1,
// /*AFLA*/ 		polarity	:  1,
// /*AFLA*/ 		irr		:  1,
// /*AFLA*/ 		trigger		:  1,	/* 0: edge, 1: level */
// /*AFLA*/ 		mask		:  1,	/* 0: enabled, 1: disabled */
// /*AFLA*/ 		__reserved_2	: 15;
// /*AFLA*/ 
// /*AFLA*/ 	__u32	__reserved_3	: 24,
// /*AFLA*/ 		dest		:  8;
// /*AFLA*/ } __attribute__ ((packed));
// /*AFLA*/ 
// /*AFLA*/ struct IR_IO_APIC_route_entry {
// /*AFLA*/ 	__u64	vector		: 8,
// /*AFLA*/ 		zero		: 3,
// /*AFLA*/ 		index2		: 1,
// /*AFLA*/ 		delivery_status : 1,
// /*AFLA*/ 		polarity	: 1,
// /*AFLA*/ 		irr		: 1,
// /*AFLA*/ 		trigger		: 1,
// /*AFLA*/ 		mask		: 1,
// /*AFLA*/ 		reserved	: 31,
// /*AFLA*/ 		format		: 1,
// /*AFLA*/ 		index		: 15;
// /*AFLA*/ } __attribute__ ((packed));
// /*AFLA*/ 
// /*AFLA*/ struct irq_alloc_info;
// /*AFLA*/ struct ioapic_domain_cfg;
// /*AFLA*/ 
// /*AFLA*/ #define IOAPIC_AUTO			-1
// /*AFLA*/ #define IOAPIC_EDGE			0
// /*AFLA*/ #define IOAPIC_LEVEL			1
// /*AFLA*/ 
// /*AFLA*/ #define IOAPIC_MASKED			1
// /*AFLA*/ #define IOAPIC_UNMASKED			0
// /*AFLA*/ 
// /*AFLA*/ #define IOAPIC_POL_HIGH			0
// /*AFLA*/ #define IOAPIC_POL_LOW			1
// /*AFLA*/ 
// /*AFLA*/ #define IOAPIC_DEST_MODE_PHYSICAL	0
// /*AFLA*/ #define IOAPIC_DEST_MODE_LOGICAL	1
// /*AFLA*/ 
// /*AFLA*/ #define	IOAPIC_MAP_ALLOC		0x1
// /*AFLA*/ #define	IOAPIC_MAP_CHECK		0x2
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_IO_APIC
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * # of IO-APICs and # of IRQ routing registers
// /*AFLA*/  */
// /*AFLA*/ extern int nr_ioapics;
// /*AFLA*/ 
// /*AFLA*/ extern int mpc_ioapic_id(int ioapic);
// /*AFLA*/ extern unsigned int mpc_ioapic_addr(int ioapic);
// /*AFLA*/ 
// /*AFLA*/ /* # of MP IRQ source entries */
// /*AFLA*/ extern int mp_irq_entries;
// /*AFLA*/ 
// /*AFLA*/ /* MP IRQ source entries */
// /*AFLA*/ extern struct mpc_intsrc mp_irqs[MAX_IRQ_SOURCES];
// /*AFLA*/ 
// /*AFLA*/ /* 1 if "noapic" boot option passed */
// /*AFLA*/ extern int skip_ioapic_setup;
// /*AFLA*/ 
// /*AFLA*/ /* 1 if "noapic" boot option passed */
// /*AFLA*/ extern int noioapicquirk;
// /*AFLA*/ 
// /*AFLA*/ /* -1 if "noapic" boot option passed */
// /*AFLA*/ extern int noioapicreroute;
// /*AFLA*/ 
// /*AFLA*/ extern u32 gsi_top;
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long io_apic_irqs;
// /*AFLA*/ 
// /*AFLA*/ #define IO_APIC_IRQ(x) (((x) >= NR_IRQS_LEGACY) || ((1 << (x)) & io_apic_irqs))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * If we use the IO-APIC for IRQ routing, disable automatic
// /*AFLA*/  * assignment of PCI IRQ's.
// /*AFLA*/  */
// /*AFLA*/ #define io_apic_assign_pci_irqs \
// /*AFLA*/ 	(mp_irq_entries && !skip_ioapic_setup && io_apic_irqs)
// /*AFLA*/ 
// /*AFLA*/ struct irq_cfg;
// /*AFLA*/ extern void ioapic_insert_resources(void);
// /*AFLA*/ extern int arch_early_ioapic_init(void);
// /*AFLA*/ 
// /*AFLA*/ extern int save_ioapic_entries(void);
// /*AFLA*/ extern void mask_ioapic_entries(void);
// /*AFLA*/ extern int restore_ioapic_entries(void);
// /*AFLA*/ 
// /*AFLA*/ extern void setup_ioapic_ids_from_mpc(void);
// /*AFLA*/ extern void setup_ioapic_ids_from_mpc_nocheck(void);
// /*AFLA*/ 
// /*AFLA*/ extern int mp_find_ioapic(u32 gsi);
// /*AFLA*/ extern int mp_find_ioapic_pin(int ioapic, u32 gsi);
// /*AFLA*/ extern int mp_map_gsi_to_irq(u32 gsi, unsigned int flags,
// /*AFLA*/ 			     struct irq_alloc_info *info);
// /*AFLA*/ extern void mp_unmap_irq(int irq);
// /*AFLA*/ extern int mp_register_ioapic(int id, u32 address, u32 gsi_base,
// /*AFLA*/ 			      struct ioapic_domain_cfg *cfg);
// /*AFLA*/ extern int mp_unregister_ioapic(u32 gsi_base);
// /*AFLA*/ extern int mp_ioapic_registered(u32 gsi_base);
// /*AFLA*/ 
// /*AFLA*/ extern void ioapic_set_alloc_attr(struct irq_alloc_info *info,
// /*AFLA*/ 				  int node, int trigger, int polarity);
// /*AFLA*/ 
// /*AFLA*/ extern void mp_save_irq(struct mpc_intsrc *m);
// /*AFLA*/ 
// /*AFLA*/ extern void disable_ioapic_support(void);
// /*AFLA*/ 
// /*AFLA*/ extern void __init io_apic_init_mappings(void);
// /*AFLA*/ extern unsigned int native_io_apic_read(unsigned int apic, unsigned int reg);
// /*AFLA*/ extern void native_disable_io_apic(void);
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int io_apic_read(unsigned int apic, unsigned int reg)
// /*AFLA*/ {
// /*AFLA*/ 	return x86_io_apic_ops.read(apic, reg);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void setup_IO_APIC(void);
// /*AFLA*/ extern void enable_IO_APIC(void);
// /*AFLA*/ extern void disable_IO_APIC(void);
// /*AFLA*/ extern void setup_ioapic_dest(void);
// /*AFLA*/ extern int IO_APIC_get_PCI_irq_vector(int bus, int devfn, int pin);
// /*AFLA*/ extern void print_IO_APICs(void);
// /*AFLA*/ #else  /* !CONFIG_X86_IO_APIC */
// /*AFLA*/ 
// /*AFLA*/ #define IO_APIC_IRQ(x)		0
// /*AFLA*/ #define io_apic_assign_pci_irqs 0
// /*AFLA*/ #define setup_ioapic_ids_from_mpc x86_init_noop
// /*AFLA*/ static inline void ioapic_insert_resources(void) { }
// /*AFLA*/ static inline int arch_early_ioapic_init(void) { return 0; }
// /*AFLA*/ static inline void print_IO_APICs(void) {}
// /*AFLA*/ #define gsi_top (NR_IRQS_LEGACY)
// /*AFLA*/ static inline int mp_find_ioapic(u32 gsi) { return 0; }
// /*AFLA*/ static inline int mp_map_gsi_to_irq(u32 gsi, unsigned int flags,
// /*AFLA*/ 				    struct irq_alloc_info *info)
// /*AFLA*/ {
// /*AFLA*/ 	return gsi;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void mp_unmap_irq(int irq) { }
// /*AFLA*/ 
// /*AFLA*/ static inline int save_ioapic_entries(void)
// /*AFLA*/ {
// /*AFLA*/ 	return -ENOMEM;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void mask_ioapic_entries(void) { }
// /*AFLA*/ static inline int restore_ioapic_entries(void)
// /*AFLA*/ {
// /*AFLA*/ 	return -ENOMEM;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void mp_save_irq(struct mpc_intsrc *m) { }
// /*AFLA*/ static inline void disable_ioapic_support(void) { }
// /*AFLA*/ static inline void io_apic_init_mappings(void) { }
// /*AFLA*/ #define native_io_apic_read		NULL
// /*AFLA*/ #define native_disable_io_apic		NULL
// /*AFLA*/ 
// /*AFLA*/ static inline void setup_IO_APIC(void) { }
// /*AFLA*/ static inline void enable_IO_APIC(void) { }
// /*AFLA*/ static inline void setup_ioapic_dest(void) { }
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_IO_APIC_H */
