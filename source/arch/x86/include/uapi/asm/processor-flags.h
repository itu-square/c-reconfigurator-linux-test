// /*AFLA*/ #ifndef _UAPI_ASM_X86_PROCESSOR_FLAGS_H
// /*AFLA*/ #define _UAPI_ASM_X86_PROCESSOR_FLAGS_H
// /*AFLA*/ /* Various flags defined: can be included from assembler. */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/const.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * EFLAGS bits
// /*AFLA*/  */
// /*AFLA*/ #define X86_EFLAGS_CF_BIT	0 /* Carry Flag */
// /*AFLA*/ #define X86_EFLAGS_CF		_BITUL(X86_EFLAGS_CF_BIT)
// /*AFLA*/ #define X86_EFLAGS_FIXED_BIT	1 /* Bit 1 - always on */
// /*AFLA*/ #define X86_EFLAGS_FIXED	_BITUL(X86_EFLAGS_FIXED_BIT)
// /*AFLA*/ #define X86_EFLAGS_PF_BIT	2 /* Parity Flag */
// /*AFLA*/ #define X86_EFLAGS_PF		_BITUL(X86_EFLAGS_PF_BIT)
// /*AFLA*/ #define X86_EFLAGS_AF_BIT	4 /* Auxiliary carry Flag */
// /*AFLA*/ #define X86_EFLAGS_AF		_BITUL(X86_EFLAGS_AF_BIT)
// /*AFLA*/ #define X86_EFLAGS_ZF_BIT	6 /* Zero Flag */
// /*AFLA*/ #define X86_EFLAGS_ZF		_BITUL(X86_EFLAGS_ZF_BIT)
// /*AFLA*/ #define X86_EFLAGS_SF_BIT	7 /* Sign Flag */
// /*AFLA*/ #define X86_EFLAGS_SF		_BITUL(X86_EFLAGS_SF_BIT)
// /*AFLA*/ #define X86_EFLAGS_TF_BIT	8 /* Trap Flag */
// /*AFLA*/ #define X86_EFLAGS_TF		_BITUL(X86_EFLAGS_TF_BIT)
// /*AFLA*/ #define X86_EFLAGS_IF_BIT	9 /* Interrupt Flag */
// /*AFLA*/ #define X86_EFLAGS_IF		_BITUL(X86_EFLAGS_IF_BIT)
// /*AFLA*/ #define X86_EFLAGS_DF_BIT	10 /* Direction Flag */
// /*AFLA*/ #define X86_EFLAGS_DF		_BITUL(X86_EFLAGS_DF_BIT)
// /*AFLA*/ #define X86_EFLAGS_OF_BIT	11 /* Overflow Flag */
// /*AFLA*/ #define X86_EFLAGS_OF		_BITUL(X86_EFLAGS_OF_BIT)
// /*AFLA*/ #define X86_EFLAGS_IOPL_BIT	12 /* I/O Privilege Level (2 bits) */
// /*AFLA*/ #define X86_EFLAGS_IOPL		(_AC(3,UL) << X86_EFLAGS_IOPL_BIT)
// /*AFLA*/ #define X86_EFLAGS_NT_BIT	14 /* Nested Task */
// /*AFLA*/ #define X86_EFLAGS_NT		_BITUL(X86_EFLAGS_NT_BIT)
// /*AFLA*/ #define X86_EFLAGS_RF_BIT	16 /* Resume Flag */
// /*AFLA*/ #define X86_EFLAGS_RF		_BITUL(X86_EFLAGS_RF_BIT)
// /*AFLA*/ #define X86_EFLAGS_VM_BIT	17 /* Virtual Mode */
// /*AFLA*/ #define X86_EFLAGS_VM		_BITUL(X86_EFLAGS_VM_BIT)
// /*AFLA*/ #define X86_EFLAGS_AC_BIT	18 /* Alignment Check/Access Control */
// /*AFLA*/ #define X86_EFLAGS_AC		_BITUL(X86_EFLAGS_AC_BIT)
// /*AFLA*/ #define X86_EFLAGS_VIF_BIT	19 /* Virtual Interrupt Flag */
// /*AFLA*/ #define X86_EFLAGS_VIF		_BITUL(X86_EFLAGS_VIF_BIT)
// /*AFLA*/ #define X86_EFLAGS_VIP_BIT	20 /* Virtual Interrupt Pending */
// /*AFLA*/ #define X86_EFLAGS_VIP		_BITUL(X86_EFLAGS_VIP_BIT)
// /*AFLA*/ #define X86_EFLAGS_ID_BIT	21 /* CPUID detection */
// /*AFLA*/ #define X86_EFLAGS_ID		_BITUL(X86_EFLAGS_ID_BIT)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Basic CPU control in CR0
// /*AFLA*/  */
// /*AFLA*/ #define X86_CR0_PE_BIT		0 /* Protection Enable */
// /*AFLA*/ #define X86_CR0_PE		_BITUL(X86_CR0_PE_BIT)
// /*AFLA*/ #define X86_CR0_MP_BIT		1 /* Monitor Coprocessor */
// /*AFLA*/ #define X86_CR0_MP		_BITUL(X86_CR0_MP_BIT)
// /*AFLA*/ #define X86_CR0_EM_BIT		2 /* Emulation */
// /*AFLA*/ #define X86_CR0_EM		_BITUL(X86_CR0_EM_BIT)
// /*AFLA*/ #define X86_CR0_TS_BIT		3 /* Task Switched */
// /*AFLA*/ #define X86_CR0_TS		_BITUL(X86_CR0_TS_BIT)
// /*AFLA*/ #define X86_CR0_ET_BIT		4 /* Extension Type */
// /*AFLA*/ #define X86_CR0_ET		_BITUL(X86_CR0_ET_BIT)
// /*AFLA*/ #define X86_CR0_NE_BIT		5 /* Numeric Error */
// /*AFLA*/ #define X86_CR0_NE		_BITUL(X86_CR0_NE_BIT)
// /*AFLA*/ #define X86_CR0_WP_BIT		16 /* Write Protect */
// /*AFLA*/ #define X86_CR0_WP		_BITUL(X86_CR0_WP_BIT)
// /*AFLA*/ #define X86_CR0_AM_BIT		18 /* Alignment Mask */
// /*AFLA*/ #define X86_CR0_AM		_BITUL(X86_CR0_AM_BIT)
// /*AFLA*/ #define X86_CR0_NW_BIT		29 /* Not Write-through */
// /*AFLA*/ #define X86_CR0_NW		_BITUL(X86_CR0_NW_BIT)
// /*AFLA*/ #define X86_CR0_CD_BIT		30 /* Cache Disable */
// /*AFLA*/ #define X86_CR0_CD		_BITUL(X86_CR0_CD_BIT)
// /*AFLA*/ #define X86_CR0_PG_BIT		31 /* Paging */
// /*AFLA*/ #define X86_CR0_PG		_BITUL(X86_CR0_PG_BIT)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Paging options in CR3
// /*AFLA*/  */
// /*AFLA*/ #define X86_CR3_PWT_BIT		3 /* Page Write Through */
// /*AFLA*/ #define X86_CR3_PWT		_BITUL(X86_CR3_PWT_BIT)
// /*AFLA*/ #define X86_CR3_PCD_BIT		4 /* Page Cache Disable */
// /*AFLA*/ #define X86_CR3_PCD		_BITUL(X86_CR3_PCD_BIT)
// /*AFLA*/ #define X86_CR3_PCID_MASK	_AC(0x00000fff,UL) /* PCID Mask */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Intel CPU features in CR4
// /*AFLA*/  */
// /*AFLA*/ #define X86_CR4_VME_BIT		0 /* enable vm86 extensions */
// /*AFLA*/ #define X86_CR4_VME		_BITUL(X86_CR4_VME_BIT)
// /*AFLA*/ #define X86_CR4_PVI_BIT		1 /* virtual interrupts flag enable */
// /*AFLA*/ #define X86_CR4_PVI		_BITUL(X86_CR4_PVI_BIT)
// /*AFLA*/ #define X86_CR4_TSD_BIT		2 /* disable time stamp at ipl 3 */
// /*AFLA*/ #define X86_CR4_TSD		_BITUL(X86_CR4_TSD_BIT)
// /*AFLA*/ #define X86_CR4_DE_BIT		3 /* enable debugging extensions */
// /*AFLA*/ #define X86_CR4_DE		_BITUL(X86_CR4_DE_BIT)
// /*AFLA*/ #define X86_CR4_PSE_BIT		4 /* enable page size extensions */
// /*AFLA*/ #define X86_CR4_PSE		_BITUL(X86_CR4_PSE_BIT)
// /*AFLA*/ #define X86_CR4_PAE_BIT		5 /* enable physical address extensions */
// /*AFLA*/ #define X86_CR4_PAE		_BITUL(X86_CR4_PAE_BIT)
// /*AFLA*/ #define X86_CR4_MCE_BIT		6 /* Machine check enable */
// /*AFLA*/ #define X86_CR4_MCE		_BITUL(X86_CR4_MCE_BIT)
// /*AFLA*/ #define X86_CR4_PGE_BIT		7 /* enable global pages */
// /*AFLA*/ #define X86_CR4_PGE		_BITUL(X86_CR4_PGE_BIT)
// /*AFLA*/ #define X86_CR4_PCE_BIT		8 /* enable performance counters at ipl 3 */
// /*AFLA*/ #define X86_CR4_PCE		_BITUL(X86_CR4_PCE_BIT)
// /*AFLA*/ #define X86_CR4_OSFXSR_BIT	9 /* enable fast FPU save and restore */
// /*AFLA*/ #define X86_CR4_OSFXSR		_BITUL(X86_CR4_OSFXSR_BIT)
// /*AFLA*/ #define X86_CR4_OSXMMEXCPT_BIT	10 /* enable unmasked SSE exceptions */
// /*AFLA*/ #define X86_CR4_OSXMMEXCPT	_BITUL(X86_CR4_OSXMMEXCPT_BIT)
// /*AFLA*/ #define X86_CR4_VMXE_BIT	13 /* enable VMX virtualization */
// /*AFLA*/ #define X86_CR4_VMXE		_BITUL(X86_CR4_VMXE_BIT)
// /*AFLA*/ #define X86_CR4_SMXE_BIT	14 /* enable safer mode (TXT) */
// /*AFLA*/ #define X86_CR4_SMXE		_BITUL(X86_CR4_SMXE_BIT)
// /*AFLA*/ #define X86_CR4_FSGSBASE_BIT	16 /* enable RDWRFSGS support */
// /*AFLA*/ #define X86_CR4_FSGSBASE	_BITUL(X86_CR4_FSGSBASE_BIT)
// /*AFLA*/ #define X86_CR4_PCIDE_BIT	17 /* enable PCID support */
// /*AFLA*/ #define X86_CR4_PCIDE		_BITUL(X86_CR4_PCIDE_BIT)
// /*AFLA*/ #define X86_CR4_OSXSAVE_BIT	18 /* enable xsave and xrestore */
// /*AFLA*/ #define X86_CR4_OSXSAVE		_BITUL(X86_CR4_OSXSAVE_BIT)
// /*AFLA*/ #define X86_CR4_SMEP_BIT	20 /* enable SMEP support */
// /*AFLA*/ #define X86_CR4_SMEP		_BITUL(X86_CR4_SMEP_BIT)
// /*AFLA*/ #define X86_CR4_SMAP_BIT	21 /* enable SMAP support */
// /*AFLA*/ #define X86_CR4_SMAP		_BITUL(X86_CR4_SMAP_BIT)
// /*AFLA*/ #define X86_CR4_PKE_BIT		22 /* enable Protection Keys support */
// /*AFLA*/ #define X86_CR4_PKE		_BITUL(X86_CR4_PKE_BIT)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * x86-64 Task Priority Register, CR8
// /*AFLA*/  */
// /*AFLA*/ #define X86_CR8_TPR		_AC(0x0000000f,UL) /* task priority register */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * AMD and Transmeta use MSRs for configuration; see <asm/msr-index.h>
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  *      NSC/Cyrix CPU configuration register indexes
// /*AFLA*/  */
// /*AFLA*/ #define CX86_PCR0	0x20
// /*AFLA*/ #define CX86_GCR	0xb8
// /*AFLA*/ #define CX86_CCR0	0xc0
// /*AFLA*/ #define CX86_CCR1	0xc1
// /*AFLA*/ #define CX86_CCR2	0xc2
// /*AFLA*/ #define CX86_CCR3	0xc3
// /*AFLA*/ #define CX86_CCR4	0xe8
// /*AFLA*/ #define CX86_CCR5	0xe9
// /*AFLA*/ #define CX86_CCR6	0xea
// /*AFLA*/ #define CX86_CCR7	0xeb
// /*AFLA*/ #define CX86_PCR1	0xf0
// /*AFLA*/ #define CX86_DIR0	0xfe
// /*AFLA*/ #define CX86_DIR1	0xff
// /*AFLA*/ #define CX86_ARR_BASE	0xc4
// /*AFLA*/ #define CX86_RCR_BASE	0xdc
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_ASM_X86_PROCESSOR_FLAGS_H */
