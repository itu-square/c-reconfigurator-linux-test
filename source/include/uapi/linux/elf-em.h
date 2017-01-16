// /*AFLA*/ #ifndef _LINUX_ELF_EM_H
// /*AFLA*/ #define _LINUX_ELF_EM_H
// /*AFLA*/ 
// /*AFLA*/ /* These constants define the various ELF target machines */
// /*AFLA*/ #define EM_NONE		0
// /*AFLA*/ #define EM_M32		1
// /*AFLA*/ #define EM_SPARC	2
// /*AFLA*/ #define EM_386		3
// /*AFLA*/ #define EM_68K		4
// /*AFLA*/ #define EM_88K		5
// /*AFLA*/ #define EM_486		6	/* Perhaps disused */
// /*AFLA*/ #define EM_860		7
// /*AFLA*/ #define EM_MIPS		8	/* MIPS R3000 (officially, big-endian only) */
// /*AFLA*/ 				/* Next two are historical and binaries and
// /*AFLA*/ 				   modules of these types will be rejected by
// /*AFLA*/ 				   Linux.  */
// /*AFLA*/ #define EM_MIPS_RS3_LE	10	/* MIPS R3000 little-endian */
// /*AFLA*/ #define EM_MIPS_RS4_BE	10	/* MIPS R4000 big-endian */
// /*AFLA*/ 
// /*AFLA*/ #define EM_PARISC	15	/* HPPA */
// /*AFLA*/ #define EM_SPARC32PLUS	18	/* Sun's "v8plus" */
// /*AFLA*/ #define EM_PPC		20	/* PowerPC */
// /*AFLA*/ #define EM_PPC64	21	 /* PowerPC64 */
// /*AFLA*/ #define EM_SPU		23	/* Cell BE SPU */
// /*AFLA*/ #define EM_ARM		40	/* ARM 32 bit */
// /*AFLA*/ #define EM_SH		42	/* SuperH */
// /*AFLA*/ #define EM_SPARCV9	43	/* SPARC v9 64-bit */
// /*AFLA*/ #define EM_H8_300	46	/* Renesas H8/300 */
// /*AFLA*/ #define EM_IA_64	50	/* HP/Intel IA-64 */
// /*AFLA*/ #define EM_X86_64	62	/* AMD x86-64 */
// /*AFLA*/ #define EM_S390		22	/* IBM S/390 */
// /*AFLA*/ #define EM_CRIS		76	/* Axis Communications 32-bit embedded processor */
// /*AFLA*/ #define EM_M32R		88	/* Renesas M32R */
// /*AFLA*/ #define EM_MN10300	89	/* Panasonic/MEI MN10300, AM33 */
// /*AFLA*/ #define EM_OPENRISC     92     /* OpenRISC 32-bit embedded processor */
// /*AFLA*/ #define EM_BLACKFIN     106     /* ADI Blackfin Processor */
// /*AFLA*/ #define EM_ALTERA_NIOS2	113	/* Altera Nios II soft-core processor */
// /*AFLA*/ #define EM_TI_C6000	140	/* TI C6X DSPs */
// /*AFLA*/ #define EM_AARCH64	183	/* ARM 64 bit */
// /*AFLA*/ #define EM_TILEPRO	188	/* Tilera TILEPro */
// /*AFLA*/ #define EM_MICROBLAZE	189	/* Xilinx MicroBlaze */
// /*AFLA*/ #define EM_TILEGX	191	/* Tilera TILE-Gx */
// /*AFLA*/ #define EM_BPF		247	/* Linux BPF - in-kernel virtual machine */
// /*AFLA*/ #define EM_FRV		0x5441	/* Fujitsu FR-V */
// /*AFLA*/ #define EM_AVR32	0x18ad	/* Atmel AVR32 */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is an interim value that we will use until the committee comes
// /*AFLA*/  * up with a final number.
// /*AFLA*/  */
// /*AFLA*/ #define EM_ALPHA	0x9026
// /*AFLA*/ 
// /*AFLA*/ /* Bogus old m32r magic number, used by old tools. */
// /*AFLA*/ #define EM_CYGNUS_M32R	0x9041
// /*AFLA*/ /* This is the old interim value for S/390 architecture */
// /*AFLA*/ #define EM_S390_OLD	0xA390
// /*AFLA*/ /* Also Panasonic/MEI MN10300, AM33 */
// /*AFLA*/ #define EM_CYGNUS_MN10300 0xbeef
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_ELF_EM_H */
