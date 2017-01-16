// /*AFLA*/ #ifndef _ASM_X86_KDEBUG_H
// /*AFLA*/ #define _ASM_X86_KDEBUG_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/notifier.h>
// /*AFLA*/ 
// /*AFLA*/ struct pt_regs;
// /*AFLA*/ 
// /*AFLA*/ /* Grossly misnamed. */
// /*AFLA*/ enum die_val {
// /*AFLA*/ 	DIE_OOPS = 1,
// /*AFLA*/ 	DIE_INT3,
// /*AFLA*/ 	DIE_DEBUG,
// /*AFLA*/ 	DIE_PANIC,
// /*AFLA*/ 	DIE_NMI,
// /*AFLA*/ 	DIE_DIE,
// /*AFLA*/ 	DIE_KERNELDEBUG,
// /*AFLA*/ 	DIE_TRAP,
// /*AFLA*/ 	DIE_GPF,
// /*AFLA*/ 	DIE_CALL,
// /*AFLA*/ 	DIE_PAGE_FAULT,
// /*AFLA*/ 	DIE_NMIUNKNOWN,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern void printk_address(unsigned long address);
// /*AFLA*/ extern void die(const char *, struct pt_regs *,long);
// /*AFLA*/ extern int __must_check __die(const char *, struct pt_regs *, long);
// /*AFLA*/ extern void show_stack_regs(struct pt_regs *regs);
// /*AFLA*/ extern void __show_regs(struct pt_regs *regs, int all);
// /*AFLA*/ extern unsigned long oops_begin(void);
// /*AFLA*/ extern void oops_end(unsigned long, struct pt_regs *, int signr);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_KDEBUG_H */
