// /*AFLA*/ #ifndef _ASM_X86_JUMP_LABEL_H
// /*AFLA*/ #define _ASM_X86_JUMP_LABEL_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef HAVE_JUMP_LABEL
// /*AFLA*/ /*
// /*AFLA*/  * For better or for worse, if jump labels (the gcc extension) are missing,
// /*AFLA*/  * then the entire static branch patching infrastructure is compiled out.
// /*AFLA*/  * If that happens, the code in here will malfunction.  Raise a compiler
// /*AFLA*/  * error instead.
// /*AFLA*/  *
// /*AFLA*/  * In theory, jump labels and the static branch patching infrastructure
// /*AFLA*/  * could be decoupled to fix this.
// /*AFLA*/  */
// /*AFLA*/ #error asm/jump_label.h included on a non-jump-label kernel
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define JUMP_LABEL_NOP_SIZE 5
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ # define STATIC_KEY_INIT_NOP P6_NOP5_ATOMIC
// /*AFLA*/ #else
// /*AFLA*/ # define STATIC_KEY_INIT_NOP GENERIC_NOP5_ATOMIC
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <asm/asm.h>
// /*AFLA*/ #include <asm/nops.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #include <linux/stringify.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ static __always_inline bool arch_static_branch(struct static_key *key, bool branch)
// /*AFLA*/ {
// /*AFLA*/ 	asm_volatile_goto("1:"
// /*AFLA*/ 		".byte " __stringify(STATIC_KEY_INIT_NOP) "\n\t"
// /*AFLA*/ 		".pushsection __jump_table,  \"aw\" \n\t"
// /*AFLA*/ 		_ASM_ALIGN "\n\t"
// /*AFLA*/ 		_ASM_PTR "1b, %l[l_yes], %c0 + %c1 \n\t"
// /*AFLA*/ 		".popsection \n\t"
// /*AFLA*/ 		: :  "i" (key), "i" (branch) : : l_yes);
// /*AFLA*/ 
// /*AFLA*/ 	return false;
// /*AFLA*/ l_yes:
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline bool arch_static_branch_jump(struct static_key *key, bool branch)
// /*AFLA*/ {
// /*AFLA*/ 	asm_volatile_goto("1:"
// /*AFLA*/ 		".byte 0xe9\n\t .long %l[l_yes] - 2f\n\t"
// /*AFLA*/ 		"2:\n\t"
// /*AFLA*/ 		".pushsection __jump_table,  \"aw\" \n\t"
// /*AFLA*/ 		_ASM_ALIGN "\n\t"
// /*AFLA*/ 		_ASM_PTR "1b, %l[l_yes], %c0 + %c1 \n\t"
// /*AFLA*/ 		".popsection \n\t"
// /*AFLA*/ 		: :  "i" (key), "i" (branch) : : l_yes);
// /*AFLA*/ 
// /*AFLA*/ 	return false;
// /*AFLA*/ l_yes:
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ typedef u64 jump_label_t;
// /*AFLA*/ #else
// /*AFLA*/ typedef u32 jump_label_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct jump_entry {
// /*AFLA*/ 	jump_label_t code;
// /*AFLA*/ 	jump_label_t target;
// /*AFLA*/ 	jump_label_t key;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #else	/* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ .macro STATIC_JUMP_IF_TRUE target, key, def
// /*AFLA*/ .Lstatic_jump_\@:
// /*AFLA*/ 	.if \def
// /*AFLA*/ 	/* Equivalent to "jmp.d32 \target" */
// /*AFLA*/ 	.byte		0xe9
// /*AFLA*/ 	.long		\target - .Lstatic_jump_after_\@
// /*AFLA*/ .Lstatic_jump_after_\@:
// /*AFLA*/ 	.else
// /*AFLA*/ 	.byte		STATIC_KEY_INIT_NOP
// /*AFLA*/ 	.endif
// /*AFLA*/ 	.pushsection __jump_table, "aw"
// /*AFLA*/ 	_ASM_ALIGN
// /*AFLA*/ 	_ASM_PTR	.Lstatic_jump_\@, \target, \key
// /*AFLA*/ 	.popsection
// /*AFLA*/ .endm
// /*AFLA*/ 
// /*AFLA*/ .macro STATIC_JUMP_IF_FALSE target, key, def
// /*AFLA*/ .Lstatic_jump_\@:
// /*AFLA*/ 	.if \def
// /*AFLA*/ 	.byte		STATIC_KEY_INIT_NOP
// /*AFLA*/ 	.else
// /*AFLA*/ 	/* Equivalent to "jmp.d32 \target" */
// /*AFLA*/ 	.byte		0xe9
// /*AFLA*/ 	.long		\target - .Lstatic_jump_after_\@
// /*AFLA*/ .Lstatic_jump_after_\@:
// /*AFLA*/ 	.endif
// /*AFLA*/ 	.pushsection __jump_table, "aw"
// /*AFLA*/ 	_ASM_ALIGN
// /*AFLA*/ 	_ASM_PTR	.Lstatic_jump_\@, \target, \key + 1
// /*AFLA*/ 	.popsection
// /*AFLA*/ .endm
// /*AFLA*/ 
// /*AFLA*/ #endif	/* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif
