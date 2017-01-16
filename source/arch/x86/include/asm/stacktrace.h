// /*AFLA*/ /*
// /*AFLA*/  *  Copyright (C) 1991, 1992  Linus Torvalds
// /*AFLA*/  *  Copyright (C) 2000, 2001, 2002 Andi Kleen, SuSE Labs
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _ASM_X86_STACKTRACE_H
// /*AFLA*/ #define _ASM_X86_STACKTRACE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/uaccess.h>
// /*AFLA*/ #include <linux/ptrace.h>
// /*AFLA*/ #include <asm/switch_to.h>
// /*AFLA*/ 
// /*AFLA*/ enum stack_type {
// /*AFLA*/ 	STACK_TYPE_UNKNOWN,
// /*AFLA*/ 	STACK_TYPE_TASK,
// /*AFLA*/ 	STACK_TYPE_IRQ,
// /*AFLA*/ 	STACK_TYPE_SOFTIRQ,
// /*AFLA*/ 	STACK_TYPE_EXCEPTION,
// /*AFLA*/ 	STACK_TYPE_EXCEPTION_LAST = STACK_TYPE_EXCEPTION + N_EXCEPTION_STACKS-1,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct stack_info {
// /*AFLA*/ 	enum stack_type type;
// /*AFLA*/ 	unsigned long *begin, *end, *next_sp;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ bool in_task_stack(unsigned long *stack, struct task_struct *task,
// /*AFLA*/ 		   struct stack_info *info);
// /*AFLA*/ 
// /*AFLA*/ int get_stack_info(unsigned long *stack, struct task_struct *task,
// /*AFLA*/ 		   struct stack_info *info, unsigned long *visit_mask);
// /*AFLA*/ 
// /*AFLA*/ void stack_type_str(enum stack_type type, const char **begin,
// /*AFLA*/ 		    const char **end);
// /*AFLA*/ 
// /*AFLA*/ static inline bool on_stack(struct stack_info *info, void *addr, size_t len)
// /*AFLA*/ {
// /*AFLA*/ 	void *begin = info->begin;
// /*AFLA*/ 	void *end   = info->end;
// /*AFLA*/ 
// /*AFLA*/ 	return (info->type != STACK_TYPE_UNKNOWN &&
// /*AFLA*/ 		addr >= begin && addr < end &&
// /*AFLA*/ 		addr + len > begin && addr + len <= end);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int kstack_depth_to_print;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ #define STACKSLOTS_PER_LINE 8
// /*AFLA*/ #else
// /*AFLA*/ #define STACKSLOTS_PER_LINE 4
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FRAME_POINTER
// /*AFLA*/ static inline unsigned long *
// /*AFLA*/ get_frame_pointer(struct task_struct *task, struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ 	if (regs)
// /*AFLA*/ 		return (unsigned long *)regs->bp;
// /*AFLA*/ 
// /*AFLA*/ 	if (task == current)
// /*AFLA*/ 		return __builtin_frame_address(0);
// /*AFLA*/ 
// /*AFLA*/ 	return (unsigned long *)((struct inactive_task_frame *)task->thread.sp)->bp;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline unsigned long *
// /*AFLA*/ get_frame_pointer(struct task_struct *task, struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_FRAME_POINTER */
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long *
// /*AFLA*/ get_stack_pointer(struct task_struct *task, struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ 	if (regs)
// /*AFLA*/ 		return (unsigned long *)kernel_stack_pointer(regs);
// /*AFLA*/ 
// /*AFLA*/ 	if (task == current)
// /*AFLA*/ 		return __builtin_frame_address(0);
// /*AFLA*/ 
// /*AFLA*/ 	return (unsigned long *)task->thread.sp;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void show_trace_log_lvl(struct task_struct *task, struct pt_regs *regs,
// /*AFLA*/ 			unsigned long *stack, char *log_lvl);
// /*AFLA*/ 
// /*AFLA*/ void show_stack_log_lvl(struct task_struct *task, struct pt_regs *regs,
// /*AFLA*/ 			unsigned long *sp, char *log_lvl);
// /*AFLA*/ 
// /*AFLA*/ extern unsigned int code_bytes;
// /*AFLA*/ 
// /*AFLA*/ /* The form of the top of the frame on the stack */
// /*AFLA*/ struct stack_frame {
// /*AFLA*/ 	struct stack_frame *next_frame;
// /*AFLA*/ 	unsigned long return_address;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct stack_frame_ia32 {
// /*AFLA*/     u32 next_frame;
// /*AFLA*/     u32 return_address;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long caller_frame_pointer(void)
// /*AFLA*/ {
// /*AFLA*/ 	struct stack_frame *frame;
// /*AFLA*/ 
// /*AFLA*/ 	frame = __builtin_frame_address(0);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FRAME_POINTER
// /*AFLA*/ 	frame = frame->next_frame;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	return (unsigned long)frame;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_STACKTRACE_H */
