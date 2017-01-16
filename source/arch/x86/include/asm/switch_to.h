// /*AFLA*/ #ifndef _ASM_X86_SWITCH_TO_H
// /*AFLA*/ #define _ASM_X86_SWITCH_TO_H
// /*AFLA*/ 
// /*AFLA*/ struct task_struct; /* one of the stranger aspects of C forward declarations */
// /*AFLA*/ 
// /*AFLA*/ struct task_struct *__switch_to_asm(struct task_struct *prev,
// /*AFLA*/ 				    struct task_struct *next);
// /*AFLA*/ 
// /*AFLA*/ __visible struct task_struct *__switch_to(struct task_struct *prev,
// /*AFLA*/ 					  struct task_struct *next);
// /*AFLA*/ struct tss_struct;
// /*AFLA*/ void __switch_to_xtra(struct task_struct *prev_p, struct task_struct *next_p,
// /*AFLA*/ 		      struct tss_struct *tss);
// /*AFLA*/ 
// /*AFLA*/ /* This runs runs on the previous thread's stack. */
// /*AFLA*/ static inline void prepare_switch_to(struct task_struct *prev,
// /*AFLA*/ 				     struct task_struct *next)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_VMAP_STACK
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If we switch to a stack that has a top-level paging entry
// /*AFLA*/ 	 * that is not present in the current mm, the resulting #PF will
// /*AFLA*/ 	 * will be promoted to a double-fault and we'll panic.  Probe
// /*AFLA*/ 	 * the new stack now so that vmalloc_fault can fix up the page
// /*AFLA*/ 	 * tables if needed.  This can only happen if we use a stack
// /*AFLA*/ 	 * in vmap space.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * We assume that the stack is aligned so that it never spans
// /*AFLA*/ 	 * more than one top-level paging entry.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * To minimize cache pollution, just follow the stack pointer.
// /*AFLA*/ 	 */
// /*AFLA*/ 	READ_ONCE(*(unsigned char *)next->thread.sp);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ asmlinkage void ret_from_fork(void);
// /*AFLA*/ 
// /*AFLA*/ /* data that is pointed to by thread.sp */
// /*AFLA*/ struct inactive_task_frame {
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 	unsigned long r15;
// /*AFLA*/ 	unsigned long r14;
// /*AFLA*/ 	unsigned long r13;
// /*AFLA*/ 	unsigned long r12;
// /*AFLA*/ #else
// /*AFLA*/ 	unsigned long si;
// /*AFLA*/ 	unsigned long di;
// /*AFLA*/ #endif
// /*AFLA*/ 	unsigned long bx;
// /*AFLA*/ 	unsigned long bp;
// /*AFLA*/ 	unsigned long ret_addr;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct fork_frame {
// /*AFLA*/ 	struct inactive_task_frame frame;
// /*AFLA*/ 	struct pt_regs regs;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define switch_to(prev, next, last)					\
// /*AFLA*/ do {									\
// /*AFLA*/ 	prepare_switch_to(prev, next);					\
// /*AFLA*/ 									\
// /*AFLA*/ 	((last) = __switch_to_asm((prev), (next)));			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_SWITCH_TO_H */
