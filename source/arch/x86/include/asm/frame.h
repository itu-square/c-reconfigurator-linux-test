// /*AFLA*/ #ifndef _ASM_X86_FRAME_H
// /*AFLA*/ #define _ASM_X86_FRAME_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/asm.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These are stack frame creation macros.  They should be used by every
// /*AFLA*/  * callable non-leaf asm function to make kernel stack traces more reliable.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FRAME_POINTER
// /*AFLA*/ 
// /*AFLA*/ #ifdef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ .macro FRAME_BEGIN
// /*AFLA*/ 	push %_ASM_BP
// /*AFLA*/ 	_ASM_MOV %_ASM_SP, %_ASM_BP
// /*AFLA*/ .endm
// /*AFLA*/ 
// /*AFLA*/ .macro FRAME_END
// /*AFLA*/ 	pop %_ASM_BP
// /*AFLA*/ .endm
// /*AFLA*/ 
// /*AFLA*/ #else /* !__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #define FRAME_BEGIN				\
// /*AFLA*/ 	"push %" _ASM_BP "\n"			\
// /*AFLA*/ 	_ASM_MOV "%" _ASM_SP ", %" _ASM_BP "\n"
// /*AFLA*/ 
// /*AFLA*/ #define FRAME_END "pop %" _ASM_BP "\n"
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #define FRAME_OFFSET __ASM_SEL(4, 8)
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_FRAME_POINTER */
// /*AFLA*/ 
// /*AFLA*/ #define FRAME_BEGIN
// /*AFLA*/ #define FRAME_END
// /*AFLA*/ #define FRAME_OFFSET 0
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_FRAME_POINTER */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_FRAME_H */
