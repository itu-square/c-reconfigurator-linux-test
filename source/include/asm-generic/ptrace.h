// /*AFLA*/ /*
// /*AFLA*/  * Common low level (register) ptrace helpers
// /*AFLA*/  *
// /*AFLA*/  * Copyright 2004-2011 Analog Devices Inc.
// /*AFLA*/  *
// /*AFLA*/  * Licensed under the GPL-2 or later.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASM_GENERIC_PTRACE_H__
// /*AFLA*/ #define __ASM_GENERIC_PTRACE_H__
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ /* Helpers for working with the instruction pointer */
// /*AFLA*/ #ifndef GET_IP
// /*AFLA*/ #define GET_IP(regs) ((regs)->pc)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef SET_IP
// /*AFLA*/ #define SET_IP(regs, val) (GET_IP(regs) = (val))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long instruction_pointer(struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ 	return GET_IP(regs);
// /*AFLA*/ }
// /*AFLA*/ static inline void instruction_pointer_set(struct pt_regs *regs,
// /*AFLA*/                                            unsigned long val)
// /*AFLA*/ {
// /*AFLA*/ 	SET_IP(regs, val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifndef profile_pc
// /*AFLA*/ #define profile_pc(regs) instruction_pointer(regs)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Helpers for working with the user stack pointer */
// /*AFLA*/ #ifndef GET_USP
// /*AFLA*/ #define GET_USP(regs) ((regs)->usp)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef SET_USP
// /*AFLA*/ #define SET_USP(regs, val) (GET_USP(regs) = (val))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long user_stack_pointer(struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ 	return GET_USP(regs);
// /*AFLA*/ }
// /*AFLA*/ static inline void user_stack_pointer_set(struct pt_regs *regs,
// /*AFLA*/                                           unsigned long val)
// /*AFLA*/ {
// /*AFLA*/ 	SET_USP(regs, val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Helpers for working with the frame pointer */
// /*AFLA*/ #ifndef GET_FP
// /*AFLA*/ #define GET_FP(regs) ((regs)->fp)
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef SET_FP
// /*AFLA*/ #define SET_FP(regs, val) (GET_FP(regs) = (val))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long frame_pointer(struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ 	return GET_FP(regs);
// /*AFLA*/ }
// /*AFLA*/ static inline void frame_pointer_set(struct pt_regs *regs,
// /*AFLA*/                                      unsigned long val)
// /*AFLA*/ {
// /*AFLA*/ 	SET_FP(regs, val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif
