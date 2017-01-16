// /*AFLA*/ #ifndef _ASM_X86_EXTABLE_H
// /*AFLA*/ #define _ASM_X86_EXTABLE_H
// /*AFLA*/ /*
// /*AFLA*/  * The exception table consists of triples of addresses relative to the
// /*AFLA*/  * exception table entry itself. The first address is of an instruction
// /*AFLA*/  * that is allowed to fault, the second is the target at which the program
// /*AFLA*/  * should continue. The third is a handler function to deal with the fault
// /*AFLA*/  * caused by the instruction in the first field.
// /*AFLA*/  *
// /*AFLA*/  * All the routines below use bits of fixup code that are out of line
// /*AFLA*/  * with the main instruction path.  This means when everything is well,
// /*AFLA*/  * we don't even have to jump over them.  Further, they do not intrude
// /*AFLA*/  * on our cache or tlb entries.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct exception_table_entry {
// /*AFLA*/ 	int insn, fixup, handler;
// /*AFLA*/ };
// /*AFLA*/ struct pt_regs;
// /*AFLA*/ 
// /*AFLA*/ #define ARCH_HAS_RELATIVE_EXTABLE
// /*AFLA*/ 
// /*AFLA*/ #define swap_ex_entry_fixup(a, b, tmp, delta)			\
// /*AFLA*/ 	do {							\
// /*AFLA*/ 		(a)->fixup = (b)->fixup + (delta);		\
// /*AFLA*/ 		(b)->fixup = (tmp).fixup - (delta);		\
// /*AFLA*/ 		(a)->handler = (b)->handler + (delta);		\
// /*AFLA*/ 		(b)->handler = (tmp).handler - (delta);		\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ extern int fixup_exception(struct pt_regs *regs, int trapnr);
// /*AFLA*/ extern bool ex_has_fault_handler(unsigned long ip);
// /*AFLA*/ extern void early_fixup_exception(struct pt_regs *regs, int trapnr);
// /*AFLA*/ 
// /*AFLA*/ #endif
