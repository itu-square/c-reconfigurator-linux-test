// /*AFLA*/ #ifndef _ASM_X86_MATH_EMU_H
// /*AFLA*/ #define _ASM_X86_MATH_EMU_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/ptrace.h>
// /*AFLA*/ 
// /*AFLA*/ /* This structure matches the layout of the data saved to the stack
// /*AFLA*/    following a device-not-present interrupt, part of it saved
// /*AFLA*/    automatically by the 80386/80486.
// /*AFLA*/    */
// /*AFLA*/ struct math_emu_info {
// /*AFLA*/ 	long ___orig_eip;
// /*AFLA*/ 	struct pt_regs *regs;
// /*AFLA*/ };
// /*AFLA*/ #endif /* _ASM_X86_MATH_EMU_H */
