// /*AFLA*/ /*
// /*AFLA*/  * Access to user system call parameters and results
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2008-2009 Red Hat, Inc.  All rights reserved.
// /*AFLA*/  *
// /*AFLA*/  * This copyrighted material is made available to anyone wishing to use,
// /*AFLA*/  * modify, copy, or redistribute it subject to the terms and conditions
// /*AFLA*/  * of the GNU General Public License v.2.
// /*AFLA*/  *
// /*AFLA*/  * See asm-generic/syscall.h for descriptions of what we must do here.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _ASM_X86_SYSCALL_H
// /*AFLA*/ #define _ASM_X86_SYSCALL_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/audit.h>
// /*AFLA*/ #include <linux/sched.h>
// /*AFLA*/ #include <linux/err.h>
// /*AFLA*/ #include <asm/asm-offsets.h>	/* For NR_syscalls */
// /*AFLA*/ #include <asm/thread_info.h>	/* for TS_COMPAT */
// /*AFLA*/ #include <asm/unistd.h>
// /*AFLA*/ 
// /*AFLA*/ typedef asmlinkage long (*sys_call_ptr_t)(unsigned long, unsigned long,
// /*AFLA*/ 					  unsigned long, unsigned long,
// /*AFLA*/ 					  unsigned long, unsigned long);
// /*AFLA*/ extern const sys_call_ptr_t sys_call_table[];
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_X86_32)
// /*AFLA*/ #define ia32_sys_call_table sys_call_table
// /*AFLA*/ #define __NR_syscall_compat_max __NR_syscall_max
// /*AFLA*/ #define IA32_NR_syscalls NR_syscalls
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_IA32_EMULATION)
// /*AFLA*/ extern const sys_call_ptr_t ia32_sys_call_table[];
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Only the low 32 bits of orig_ax are meaningful, so we return int.
// /*AFLA*/  * This importantly ignores the high bits on 64-bit, so comparisons
// /*AFLA*/  * sign-extend the low 32 bits.
// /*AFLA*/  */
// /*AFLA*/ static inline int syscall_get_nr(struct task_struct *task, struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ 	return regs->orig_ax;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void syscall_rollback(struct task_struct *task,
// /*AFLA*/ 				    struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ 	regs->ax = regs->orig_ax;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline long syscall_get_error(struct task_struct *task,
// /*AFLA*/ 				     struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long error = regs->ax;
// /*AFLA*/ #ifdef CONFIG_IA32_EMULATION
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * TS_COMPAT is set for 32-bit syscall entries and then
// /*AFLA*/ 	 * remains set until we return to user mode.
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (task->thread.status & (TS_COMPAT|TS_I386_REGS_POKED))
// /*AFLA*/ 		/*
// /*AFLA*/ 		 * Sign-extend the value so (int)-EFOO becomes (long)-EFOO
// /*AFLA*/ 		 * and will match correctly in comparisons.
// /*AFLA*/ 		 */
// /*AFLA*/ 		error = (long) (int) error;
// /*AFLA*/ #endif
// /*AFLA*/ 	return IS_ERR_VALUE(error) ? error : 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline long syscall_get_return_value(struct task_struct *task,
// /*AFLA*/ 					    struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ 	return regs->ax;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void syscall_set_return_value(struct task_struct *task,
// /*AFLA*/ 					    struct pt_regs *regs,
// /*AFLA*/ 					    int error, long val)
// /*AFLA*/ {
// /*AFLA*/ 	regs->ax = (long) error ?: val;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 
// /*AFLA*/ static inline void syscall_get_arguments(struct task_struct *task,
// /*AFLA*/ 					 struct pt_regs *regs,
// /*AFLA*/ 					 unsigned int i, unsigned int n,
// /*AFLA*/ 					 unsigned long *args)
// /*AFLA*/ {
// /*AFLA*/ 	BUG_ON(i + n > 6);
// /*AFLA*/ 	memcpy(args, &regs->bx + i, n * sizeof(args[0]));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void syscall_set_arguments(struct task_struct *task,
// /*AFLA*/ 					 struct pt_regs *regs,
// /*AFLA*/ 					 unsigned int i, unsigned int n,
// /*AFLA*/ 					 const unsigned long *args)
// /*AFLA*/ {
// /*AFLA*/ 	BUG_ON(i + n > 6);
// /*AFLA*/ 	memcpy(&regs->bx + i, args, n * sizeof(args[0]));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int syscall_get_arch(void)
// /*AFLA*/ {
// /*AFLA*/ 	return AUDIT_ARCH_I386;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else	 /* CONFIG_X86_64 */
// /*AFLA*/ 
// /*AFLA*/ static inline void syscall_get_arguments(struct task_struct *task,
// /*AFLA*/ 					 struct pt_regs *regs,
// /*AFLA*/ 					 unsigned int i, unsigned int n,
// /*AFLA*/ 					 unsigned long *args)
// /*AFLA*/ {
// /*AFLA*/ # ifdef CONFIG_IA32_EMULATION
// /*AFLA*/ 	if (task->thread.status & TS_COMPAT)
// /*AFLA*/ 		switch (i) {
// /*AFLA*/ 		case 0:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			*args++ = regs->bx;
// /*AFLA*/ 		case 1:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			*args++ = regs->cx;
// /*AFLA*/ 		case 2:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			*args++ = regs->dx;
// /*AFLA*/ 		case 3:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			*args++ = regs->si;
// /*AFLA*/ 		case 4:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			*args++ = regs->di;
// /*AFLA*/ 		case 5:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			*args++ = regs->bp;
// /*AFLA*/ 		case 6:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 		default:
// /*AFLA*/ 			BUG();
// /*AFLA*/ 			break;
// /*AFLA*/ 		}
// /*AFLA*/ 	else
// /*AFLA*/ # endif
// /*AFLA*/ 		switch (i) {
// /*AFLA*/ 		case 0:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			*args++ = regs->di;
// /*AFLA*/ 		case 1:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			*args++ = regs->si;
// /*AFLA*/ 		case 2:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			*args++ = regs->dx;
// /*AFLA*/ 		case 3:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			*args++ = regs->r10;
// /*AFLA*/ 		case 4:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			*args++ = regs->r8;
// /*AFLA*/ 		case 5:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			*args++ = regs->r9;
// /*AFLA*/ 		case 6:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 		default:
// /*AFLA*/ 			BUG();
// /*AFLA*/ 			break;
// /*AFLA*/ 		}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void syscall_set_arguments(struct task_struct *task,
// /*AFLA*/ 					 struct pt_regs *regs,
// /*AFLA*/ 					 unsigned int i, unsigned int n,
// /*AFLA*/ 					 const unsigned long *args)
// /*AFLA*/ {
// /*AFLA*/ # ifdef CONFIG_IA32_EMULATION
// /*AFLA*/ 	if (task->thread.status & TS_COMPAT)
// /*AFLA*/ 		switch (i) {
// /*AFLA*/ 		case 0:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			regs->bx = *args++;
// /*AFLA*/ 		case 1:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			regs->cx = *args++;
// /*AFLA*/ 		case 2:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			regs->dx = *args++;
// /*AFLA*/ 		case 3:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			regs->si = *args++;
// /*AFLA*/ 		case 4:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			regs->di = *args++;
// /*AFLA*/ 		case 5:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			regs->bp = *args++;
// /*AFLA*/ 		case 6:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 		default:
// /*AFLA*/ 			BUG();
// /*AFLA*/ 			break;
// /*AFLA*/ 		}
// /*AFLA*/ 	else
// /*AFLA*/ # endif
// /*AFLA*/ 		switch (i) {
// /*AFLA*/ 		case 0:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			regs->di = *args++;
// /*AFLA*/ 		case 1:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			regs->si = *args++;
// /*AFLA*/ 		case 2:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			regs->dx = *args++;
// /*AFLA*/ 		case 3:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			regs->r10 = *args++;
// /*AFLA*/ 		case 4:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			regs->r8 = *args++;
// /*AFLA*/ 		case 5:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 			regs->r9 = *args++;
// /*AFLA*/ 		case 6:
// /*AFLA*/ 			if (!n--) break;
// /*AFLA*/ 		default:
// /*AFLA*/ 			BUG();
// /*AFLA*/ 			break;
// /*AFLA*/ 		}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int syscall_get_arch(void)
// /*AFLA*/ {
// /*AFLA*/ 	/* x32 tasks should be considered AUDIT_ARCH_X86_64. */
// /*AFLA*/ 	return in_ia32_syscall() ? AUDIT_ARCH_I386 : AUDIT_ARCH_X86_64;
// /*AFLA*/ }
// /*AFLA*/ #endif	/* CONFIG_X86_32 */
// /*AFLA*/ 
// /*AFLA*/ #endif	/* _ASM_X86_SYSCALL_H */
