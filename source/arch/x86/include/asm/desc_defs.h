// /*AFLA*/ /* Written 2000 by Andi Kleen */
// /*AFLA*/ #ifndef _ASM_X86_DESC_DEFS_H
// /*AFLA*/ #define _ASM_X86_DESC_DEFS_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Segment descriptor structure definitions, usable from both x86_64 and i386
// /*AFLA*/  * archs.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * FIXME: Accessing the desc_struct through its fields is more elegant,
// /*AFLA*/  * and should be the one valid thing to do. However, a lot of open code
// /*AFLA*/  * still touches the a and b accessors, and doing this allow us to do it
// /*AFLA*/  * incrementally. We keep the signature as a struct, rather than an union,
// /*AFLA*/  * so we can get rid of it transparently in the future -- glommer
// /*AFLA*/  */
// /*AFLA*/ /* 8 byte segment descriptor */
// /*AFLA*/ struct desc_struct {
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct {
// /*AFLA*/ 			unsigned int a;
// /*AFLA*/ 			unsigned int b;
// /*AFLA*/ 		};
// /*AFLA*/ 		struct {
// /*AFLA*/ 			u16 limit0;
// /*AFLA*/ 			u16 base0;
// /*AFLA*/ 			unsigned base1: 8, type: 4, s: 1, dpl: 2, p: 1;
// /*AFLA*/ 			unsigned limit: 4, avl: 1, l: 1, d: 1, g: 1, base2: 8;
// /*AFLA*/ 		};
// /*AFLA*/ 	};
// /*AFLA*/ } __attribute__((packed));
// /*AFLA*/ 
// /*AFLA*/ #define GDT_ENTRY_INIT(flags, base, limit) { { { \
// /*AFLA*/ 		.a = ((limit) & 0xffff) | (((base) & 0xffff) << 16), \
// /*AFLA*/ 		.b = (((base) & 0xff0000) >> 16) | (((flags) & 0xf0ff) << 8) | \
// /*AFLA*/ 			((limit) & 0xf0000) | ((base) & 0xff000000), \
// /*AFLA*/ 	} } }
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	GATE_INTERRUPT = 0xE,
// /*AFLA*/ 	GATE_TRAP = 0xF,
// /*AFLA*/ 	GATE_CALL = 0xC,
// /*AFLA*/ 	GATE_TASK = 0x5,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* 16byte gate */
// /*AFLA*/ struct gate_struct64 {
// /*AFLA*/ 	u16 offset_low;
// /*AFLA*/ 	u16 segment;
// /*AFLA*/ 	unsigned ist : 3, zero0 : 5, type : 5, dpl : 2, p : 1;
// /*AFLA*/ 	u16 offset_middle;
// /*AFLA*/ 	u32 offset_high;
// /*AFLA*/ 	u32 zero1;
// /*AFLA*/ } __attribute__((packed));
// /*AFLA*/ 
// /*AFLA*/ #define PTR_LOW(x) ((unsigned long long)(x) & 0xFFFF)
// /*AFLA*/ #define PTR_MIDDLE(x) (((unsigned long long)(x) >> 16) & 0xFFFF)
// /*AFLA*/ #define PTR_HIGH(x) ((unsigned long long)(x) >> 32)
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	DESC_TSS = 0x9,
// /*AFLA*/ 	DESC_LDT = 0x2,
// /*AFLA*/ 	DESCTYPE_S = 0x10,	/* !system */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* LDT or TSS descriptor in the GDT. 16 bytes. */
// /*AFLA*/ struct ldttss_desc64 {
// /*AFLA*/ 	u16 limit0;
// /*AFLA*/ 	u16 base0;
// /*AFLA*/ 	unsigned base1 : 8, type : 5, dpl : 2, p : 1;
// /*AFLA*/ 	unsigned limit1 : 4, zero0 : 3, g : 1, base2 : 8;
// /*AFLA*/ 	u32 base3;
// /*AFLA*/ 	u32 zero1;
// /*AFLA*/ } __attribute__((packed));
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ typedef struct gate_struct64 gate_desc;
// /*AFLA*/ typedef struct ldttss_desc64 ldt_desc;
// /*AFLA*/ typedef struct ldttss_desc64 tss_desc;
// /*AFLA*/ #define gate_offset(g) ((g).offset_low | ((unsigned long)(g).offset_middle << 16) | ((unsigned long)(g).offset_high << 32))
// /*AFLA*/ #define gate_segment(g) ((g).segment)
// /*AFLA*/ #else
// /*AFLA*/ typedef struct desc_struct gate_desc;
// /*AFLA*/ typedef struct desc_struct ldt_desc;
// /*AFLA*/ typedef struct desc_struct tss_desc;
// /*AFLA*/ #define gate_offset(g)		(((g).b & 0xffff0000) | ((g).a & 0x0000ffff))
// /*AFLA*/ #define gate_segment(g)		((g).a >> 16)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct desc_ptr {
// /*AFLA*/ 	unsigned short size;
// /*AFLA*/ 	unsigned long address;
// /*AFLA*/ } __attribute__((packed)) ;
// /*AFLA*/ 
// /*AFLA*/ #endif /* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ /* Access rights as returned by LAR */
// /*AFLA*/ #define AR_TYPE_RODATA		(0 * (1 << 9))
// /*AFLA*/ #define AR_TYPE_RWDATA		(1 * (1 << 9))
// /*AFLA*/ #define AR_TYPE_RODATA_EXPDOWN	(2 * (1 << 9))
// /*AFLA*/ #define AR_TYPE_RWDATA_EXPDOWN	(3 * (1 << 9))
// /*AFLA*/ #define AR_TYPE_XOCODE		(4 * (1 << 9))
// /*AFLA*/ #define AR_TYPE_XRCODE		(5 * (1 << 9))
// /*AFLA*/ #define AR_TYPE_XOCODE_CONF	(6 * (1 << 9))
// /*AFLA*/ #define AR_TYPE_XRCODE_CONF	(7 * (1 << 9))
// /*AFLA*/ #define AR_TYPE_MASK		(7 * (1 << 9))
// /*AFLA*/ 
// /*AFLA*/ #define AR_DPL0			(0 * (1 << 13))
// /*AFLA*/ #define AR_DPL3			(3 * (1 << 13))
// /*AFLA*/ #define AR_DPL_MASK		(3 * (1 << 13))
// /*AFLA*/ 
// /*AFLA*/ #define AR_A			(1 << 8)   /* "Accessed" */
// /*AFLA*/ #define AR_S			(1 << 12)  /* If clear, "System" segment */
// /*AFLA*/ #define AR_P			(1 << 15)  /* "Present" */
// /*AFLA*/ #define AR_AVL			(1 << 20)  /* "AVaiLable" (no HW effect) */
// /*AFLA*/ #define AR_L			(1 << 21)  /* "Long mode" for code segments */
// /*AFLA*/ #define AR_DB			(1 << 22)  /* D/B, effect depends on type */
// /*AFLA*/ #define AR_G			(1 << 23)  /* "Granularity" (limit in pages) */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_DESC_DEFS_H */
