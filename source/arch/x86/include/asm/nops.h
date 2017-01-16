// /*AFLA*/ #ifndef _ASM_X86_NOPS_H
// /*AFLA*/ #define _ASM_X86_NOPS_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Define nops for use with alternative() and for tracing.
// /*AFLA*/  *
// /*AFLA*/  * *_NOP5_ATOMIC must be a single instruction.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define NOP_DS_PREFIX 0x3e
// /*AFLA*/ 
// /*AFLA*/ /* generic versions from gas
// /*AFLA*/    1: nop
// /*AFLA*/    the following instructions are NOT nops in 64-bit mode,
// /*AFLA*/    for 64-bit mode use K8 or P6 nops instead
// /*AFLA*/    2: movl %esi,%esi
// /*AFLA*/    3: leal 0x00(%esi),%esi
// /*AFLA*/    4: leal 0x00(,%esi,1),%esi
// /*AFLA*/    6: leal 0x00000000(%esi),%esi
// /*AFLA*/    7: leal 0x00000000(,%esi,1),%esi
// /*AFLA*/ */
// /*AFLA*/ #define GENERIC_NOP1 0x90
// /*AFLA*/ #define GENERIC_NOP2 0x89,0xf6
// /*AFLA*/ #define GENERIC_NOP3 0x8d,0x76,0x00
// /*AFLA*/ #define GENERIC_NOP4 0x8d,0x74,0x26,0x00
// /*AFLA*/ #define GENERIC_NOP5 GENERIC_NOP1,GENERIC_NOP4
// /*AFLA*/ #define GENERIC_NOP6 0x8d,0xb6,0x00,0x00,0x00,0x00
// /*AFLA*/ #define GENERIC_NOP7 0x8d,0xb4,0x26,0x00,0x00,0x00,0x00
// /*AFLA*/ #define GENERIC_NOP8 GENERIC_NOP1,GENERIC_NOP7
// /*AFLA*/ #define GENERIC_NOP5_ATOMIC NOP_DS_PREFIX,GENERIC_NOP4
// /*AFLA*/ 
// /*AFLA*/ /* Opteron 64bit nops
// /*AFLA*/    1: nop
// /*AFLA*/    2: osp nop
// /*AFLA*/    3: osp osp nop
// /*AFLA*/    4: osp osp osp nop
// /*AFLA*/ */
// /*AFLA*/ #define K8_NOP1 GENERIC_NOP1
// /*AFLA*/ #define K8_NOP2	0x66,K8_NOP1
// /*AFLA*/ #define K8_NOP3	0x66,K8_NOP2
// /*AFLA*/ #define K8_NOP4	0x66,K8_NOP3
// /*AFLA*/ #define K8_NOP5	K8_NOP3,K8_NOP2
// /*AFLA*/ #define K8_NOP6	K8_NOP3,K8_NOP3
// /*AFLA*/ #define K8_NOP7	K8_NOP4,K8_NOP3
// /*AFLA*/ #define K8_NOP8	K8_NOP4,K8_NOP4
// /*AFLA*/ #define K8_NOP5_ATOMIC 0x66,K8_NOP4
// /*AFLA*/ 
// /*AFLA*/ /* K7 nops
// /*AFLA*/    uses eax dependencies (arbitrary choice)
// /*AFLA*/    1: nop
// /*AFLA*/    2: movl %eax,%eax
// /*AFLA*/    3: leal (,%eax,1),%eax
// /*AFLA*/    4: leal 0x00(,%eax,1),%eax
// /*AFLA*/    6: leal 0x00000000(%eax),%eax
// /*AFLA*/    7: leal 0x00000000(,%eax,1),%eax
// /*AFLA*/ */
// /*AFLA*/ #define K7_NOP1	GENERIC_NOP1
// /*AFLA*/ #define K7_NOP2	0x8b,0xc0
// /*AFLA*/ #define K7_NOP3	0x8d,0x04,0x20
// /*AFLA*/ #define K7_NOP4	0x8d,0x44,0x20,0x00
// /*AFLA*/ #define K7_NOP5	K7_NOP4,K7_NOP1
// /*AFLA*/ #define K7_NOP6	0x8d,0x80,0,0,0,0
// /*AFLA*/ #define K7_NOP7	0x8D,0x04,0x05,0,0,0,0
// /*AFLA*/ #define K7_NOP8	K7_NOP7,K7_NOP1
// /*AFLA*/ #define K7_NOP5_ATOMIC NOP_DS_PREFIX,K7_NOP4
// /*AFLA*/ 
// /*AFLA*/ /* P6 nops
// /*AFLA*/    uses eax dependencies (Intel-recommended choice)
// /*AFLA*/    1: nop
// /*AFLA*/    2: osp nop
// /*AFLA*/    3: nopl (%eax)
// /*AFLA*/    4: nopl 0x00(%eax)
// /*AFLA*/    5: nopl 0x00(%eax,%eax,1)
// /*AFLA*/    6: osp nopl 0x00(%eax,%eax,1)
// /*AFLA*/    7: nopl 0x00000000(%eax)
// /*AFLA*/    8: nopl 0x00000000(%eax,%eax,1)
// /*AFLA*/    Note: All the above are assumed to be a single instruction.
// /*AFLA*/ 	There is kernel code that depends on this.
// /*AFLA*/ */
// /*AFLA*/ #define P6_NOP1	GENERIC_NOP1
// /*AFLA*/ #define P6_NOP2	0x66,0x90
// /*AFLA*/ #define P6_NOP3	0x0f,0x1f,0x00
// /*AFLA*/ #define P6_NOP4	0x0f,0x1f,0x40,0
// /*AFLA*/ #define P6_NOP5	0x0f,0x1f,0x44,0x00,0
// /*AFLA*/ #define P6_NOP6	0x66,0x0f,0x1f,0x44,0x00,0
// /*AFLA*/ #define P6_NOP7	0x0f,0x1f,0x80,0,0,0,0
// /*AFLA*/ #define P6_NOP8	0x0f,0x1f,0x84,0x00,0,0,0,0
// /*AFLA*/ #define P6_NOP5_ATOMIC P6_NOP5
// /*AFLA*/ 
// /*AFLA*/ #ifdef __ASSEMBLY__
// /*AFLA*/ #define _ASM_MK_NOP(x) .byte x
// /*AFLA*/ #else
// /*AFLA*/ #define _ASM_MK_NOP(x) ".byte " __stringify(x) "\n"
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_MK7)
// /*AFLA*/ #define ASM_NOP1 _ASM_MK_NOP(K7_NOP1)
// /*AFLA*/ #define ASM_NOP2 _ASM_MK_NOP(K7_NOP2)
// /*AFLA*/ #define ASM_NOP3 _ASM_MK_NOP(K7_NOP3)
// /*AFLA*/ #define ASM_NOP4 _ASM_MK_NOP(K7_NOP4)
// /*AFLA*/ #define ASM_NOP5 _ASM_MK_NOP(K7_NOP5)
// /*AFLA*/ #define ASM_NOP6 _ASM_MK_NOP(K7_NOP6)
// /*AFLA*/ #define ASM_NOP7 _ASM_MK_NOP(K7_NOP7)
// /*AFLA*/ #define ASM_NOP8 _ASM_MK_NOP(K7_NOP8)
// /*AFLA*/ #define ASM_NOP5_ATOMIC _ASM_MK_NOP(K7_NOP5_ATOMIC)
// /*AFLA*/ #elif defined(CONFIG_X86_P6_NOP)
// /*AFLA*/ #define ASM_NOP1 _ASM_MK_NOP(P6_NOP1)
// /*AFLA*/ #define ASM_NOP2 _ASM_MK_NOP(P6_NOP2)
// /*AFLA*/ #define ASM_NOP3 _ASM_MK_NOP(P6_NOP3)
// /*AFLA*/ #define ASM_NOP4 _ASM_MK_NOP(P6_NOP4)
// /*AFLA*/ #define ASM_NOP5 _ASM_MK_NOP(P6_NOP5)
// /*AFLA*/ #define ASM_NOP6 _ASM_MK_NOP(P6_NOP6)
// /*AFLA*/ #define ASM_NOP7 _ASM_MK_NOP(P6_NOP7)
// /*AFLA*/ #define ASM_NOP8 _ASM_MK_NOP(P6_NOP8)
// /*AFLA*/ #define ASM_NOP5_ATOMIC _ASM_MK_NOP(P6_NOP5_ATOMIC)
// /*AFLA*/ #elif defined(CONFIG_X86_64)
// /*AFLA*/ #define ASM_NOP1 _ASM_MK_NOP(K8_NOP1)
// /*AFLA*/ #define ASM_NOP2 _ASM_MK_NOP(K8_NOP2)
// /*AFLA*/ #define ASM_NOP3 _ASM_MK_NOP(K8_NOP3)
// /*AFLA*/ #define ASM_NOP4 _ASM_MK_NOP(K8_NOP4)
// /*AFLA*/ #define ASM_NOP5 _ASM_MK_NOP(K8_NOP5)
// /*AFLA*/ #define ASM_NOP6 _ASM_MK_NOP(K8_NOP6)
// /*AFLA*/ #define ASM_NOP7 _ASM_MK_NOP(K8_NOP7)
// /*AFLA*/ #define ASM_NOP8 _ASM_MK_NOP(K8_NOP8)
// /*AFLA*/ #define ASM_NOP5_ATOMIC _ASM_MK_NOP(K8_NOP5_ATOMIC)
// /*AFLA*/ #else
// /*AFLA*/ #define ASM_NOP1 _ASM_MK_NOP(GENERIC_NOP1)
// /*AFLA*/ #define ASM_NOP2 _ASM_MK_NOP(GENERIC_NOP2)
// /*AFLA*/ #define ASM_NOP3 _ASM_MK_NOP(GENERIC_NOP3)
// /*AFLA*/ #define ASM_NOP4 _ASM_MK_NOP(GENERIC_NOP4)
// /*AFLA*/ #define ASM_NOP5 _ASM_MK_NOP(GENERIC_NOP5)
// /*AFLA*/ #define ASM_NOP6 _ASM_MK_NOP(GENERIC_NOP6)
// /*AFLA*/ #define ASM_NOP7 _ASM_MK_NOP(GENERIC_NOP7)
// /*AFLA*/ #define ASM_NOP8 _ASM_MK_NOP(GENERIC_NOP8)
// /*AFLA*/ #define ASM_NOP5_ATOMIC _ASM_MK_NOP(GENERIC_NOP5_ATOMIC)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define ASM_NOP_MAX 8
// /*AFLA*/ #define NOP_ATOMIC5 (ASM_NOP_MAX+1)	/* Entry for the 5-byte atomic NOP */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ extern const unsigned char * const *ideal_nops;
// /*AFLA*/ extern void arch_init_ideal_nops(void);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_NOPS_H */
