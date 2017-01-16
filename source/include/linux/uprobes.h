// /*AFLA*/ #ifndef _LINUX_UPROBES_H
// /*AFLA*/ #define _LINUX_UPROBES_H
// /*AFLA*/ /*
// /*AFLA*/  * User-space Probes (UProbes)
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify
// /*AFLA*/  * it under the terms of the GNU General Public License as published by
// /*AFLA*/  * the Free Software Foundation; either version 2 of the License, or
// /*AFLA*/  * (at your option) any later version.
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it will be useful,
// /*AFLA*/  * but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/  * GNU General Public License for more details.
// /*AFLA*/  *
// /*AFLA*/  * You should have received a copy of the GNU General Public License
// /*AFLA*/  * along with this program; if not, write to the Free Software
// /*AFLA*/  * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) IBM Corporation, 2008-2012
// /*AFLA*/  * Authors:
// /*AFLA*/  *	Srikar Dronamraju
// /*AFLA*/  *	Jim Keniston
// /*AFLA*/  * Copyright (C) 2011-2012 Red Hat, Inc., Peter Zijlstra
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ #include <linux/rbtree.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ struct vm_area_struct;
// /*AFLA*/ struct mm_struct;
// /*AFLA*/ struct inode;
// /*AFLA*/ struct notifier_block;
// /*AFLA*/ struct page;
// /*AFLA*/ 
// /*AFLA*/ #define UPROBE_HANDLER_REMOVE		1
// /*AFLA*/ #define UPROBE_HANDLER_MASK		1
// /*AFLA*/ 
// /*AFLA*/ #define MAX_URETPROBE_DEPTH		64
// /*AFLA*/ 
// /*AFLA*/ enum uprobe_filter_ctx {
// /*AFLA*/ 	UPROBE_FILTER_REGISTER,
// /*AFLA*/ 	UPROBE_FILTER_UNREGISTER,
// /*AFLA*/ 	UPROBE_FILTER_MMAP,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct uprobe_consumer {
// /*AFLA*/ 	int (*handler)(struct uprobe_consumer *self, struct pt_regs *regs);
// /*AFLA*/ 	int (*ret_handler)(struct uprobe_consumer *self,
// /*AFLA*/ 				unsigned long func,
// /*AFLA*/ 				struct pt_regs *regs);
// /*AFLA*/ 	bool (*filter)(struct uprobe_consumer *self,
// /*AFLA*/ 				enum uprobe_filter_ctx ctx,
// /*AFLA*/ 				struct mm_struct *mm);
// /*AFLA*/ 
// /*AFLA*/ 	struct uprobe_consumer *next;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_UPROBES
// /*AFLA*/ #include <asm/uprobes.h>
// /*AFLA*/ 
// /*AFLA*/ enum uprobe_task_state {
// /*AFLA*/ 	UTASK_RUNNING,
// /*AFLA*/ 	UTASK_SSTEP,
// /*AFLA*/ 	UTASK_SSTEP_ACK,
// /*AFLA*/ 	UTASK_SSTEP_TRAPPED,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * uprobe_task: Metadata of a task while it singlesteps.
// /*AFLA*/  */
// /*AFLA*/ struct uprobe_task {
// /*AFLA*/ 	enum uprobe_task_state		state;
// /*AFLA*/ 
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct {
// /*AFLA*/ 			struct arch_uprobe_task	autask;
// /*AFLA*/ 			unsigned long		vaddr;
// /*AFLA*/ 		};
// /*AFLA*/ 
// /*AFLA*/ 		struct {
// /*AFLA*/ 			struct callback_head	dup_xol_work;
// /*AFLA*/ 			unsigned long		dup_xol_addr;
// /*AFLA*/ 		};
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ 	struct uprobe			*active_uprobe;
// /*AFLA*/ 	unsigned long			xol_vaddr;
// /*AFLA*/ 
// /*AFLA*/ 	struct return_instance		*return_instances;
// /*AFLA*/ 	unsigned int			depth;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct return_instance {
// /*AFLA*/ 	struct uprobe		*uprobe;
// /*AFLA*/ 	unsigned long		func;
// /*AFLA*/ 	unsigned long		stack;		/* stack pointer */
// /*AFLA*/ 	unsigned long		orig_ret_vaddr; /* original return address */
// /*AFLA*/ 	bool			chained;	/* true, if instance is nested */
// /*AFLA*/ 
// /*AFLA*/ 	struct return_instance	*next;		/* keep as stack */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum rp_check {
// /*AFLA*/ 	RP_CHECK_CALL,
// /*AFLA*/ 	RP_CHECK_CHAIN_CALL,
// /*AFLA*/ 	RP_CHECK_RET,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct xol_area;
// /*AFLA*/ 
// /*AFLA*/ struct uprobes_state {
// /*AFLA*/ 	struct xol_area		*xol_area;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern int set_swbp(struct arch_uprobe *aup, struct mm_struct *mm, unsigned long vaddr);
// /*AFLA*/ extern int set_orig_insn(struct arch_uprobe *aup, struct mm_struct *mm, unsigned long vaddr);
// /*AFLA*/ extern bool is_swbp_insn(uprobe_opcode_t *insn);
// /*AFLA*/ extern bool is_trap_insn(uprobe_opcode_t *insn);
// /*AFLA*/ extern unsigned long uprobe_get_swbp_addr(struct pt_regs *regs);
// /*AFLA*/ extern unsigned long uprobe_get_trap_addr(struct pt_regs *regs);
// /*AFLA*/ extern int uprobe_write_opcode(struct mm_struct *mm, unsigned long vaddr, uprobe_opcode_t);
// /*AFLA*/ extern int uprobe_register(struct inode *inode, loff_t offset, struct uprobe_consumer *uc);
// /*AFLA*/ extern int uprobe_apply(struct inode *inode, loff_t offset, struct uprobe_consumer *uc, bool);
// /*AFLA*/ extern void uprobe_unregister(struct inode *inode, loff_t offset, struct uprobe_consumer *uc);
// /*AFLA*/ extern int uprobe_mmap(struct vm_area_struct *vma);
// /*AFLA*/ extern void uprobe_munmap(struct vm_area_struct *vma, unsigned long start, unsigned long end);
// /*AFLA*/ extern void uprobe_start_dup_mmap(void);
// /*AFLA*/ extern void uprobe_end_dup_mmap(void);
// /*AFLA*/ extern void uprobe_dup_mmap(struct mm_struct *oldmm, struct mm_struct *newmm);
// /*AFLA*/ extern void uprobe_free_utask(struct task_struct *t);
// /*AFLA*/ extern void uprobe_copy_process(struct task_struct *t, unsigned long flags);
// /*AFLA*/ extern int uprobe_post_sstep_notifier(struct pt_regs *regs);
// /*AFLA*/ extern int uprobe_pre_sstep_notifier(struct pt_regs *regs);
// /*AFLA*/ extern void uprobe_notify_resume(struct pt_regs *regs);
// /*AFLA*/ extern bool uprobe_deny_signal(void);
// /*AFLA*/ extern bool arch_uprobe_skip_sstep(struct arch_uprobe *aup, struct pt_regs *regs);
// /*AFLA*/ extern void uprobe_clear_state(struct mm_struct *mm);
// /*AFLA*/ extern int  arch_uprobe_analyze_insn(struct arch_uprobe *aup, struct mm_struct *mm, unsigned long addr);
// /*AFLA*/ extern int  arch_uprobe_pre_xol(struct arch_uprobe *aup, struct pt_regs *regs);
// /*AFLA*/ extern int  arch_uprobe_post_xol(struct arch_uprobe *aup, struct pt_regs *regs);
// /*AFLA*/ extern bool arch_uprobe_xol_was_trapped(struct task_struct *tsk);
// /*AFLA*/ extern int  arch_uprobe_exception_notify(struct notifier_block *self, unsigned long val, void *data);
// /*AFLA*/ extern void arch_uprobe_abort_xol(struct arch_uprobe *aup, struct pt_regs *regs);
// /*AFLA*/ extern unsigned long arch_uretprobe_hijack_return_addr(unsigned long trampoline_vaddr, struct pt_regs *regs);
// /*AFLA*/ extern bool arch_uretprobe_is_alive(struct return_instance *ret, enum rp_check ctx, struct pt_regs *regs);
// /*AFLA*/ extern bool arch_uprobe_ignore(struct arch_uprobe *aup, struct pt_regs *regs);
// /*AFLA*/ extern void arch_uprobe_copy_ixol(struct page *page, unsigned long vaddr,
// /*AFLA*/ 					 void *src, unsigned long len);
// /*AFLA*/ #else /* !CONFIG_UPROBES */
// /*AFLA*/ struct uprobes_state {
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define uprobe_get_trap_addr(regs)	instruction_pointer(regs)
// /*AFLA*/ 
// /*AFLA*/ static inline int
// /*AFLA*/ uprobe_register(struct inode *inode, loff_t offset, struct uprobe_consumer *uc)
// /*AFLA*/ {
// /*AFLA*/ 	return -ENOSYS;
// /*AFLA*/ }
// /*AFLA*/ static inline int
// /*AFLA*/ uprobe_apply(struct inode *inode, loff_t offset, struct uprobe_consumer *uc, bool add)
// /*AFLA*/ {
// /*AFLA*/ 	return -ENOSYS;
// /*AFLA*/ }
// /*AFLA*/ static inline void
// /*AFLA*/ uprobe_unregister(struct inode *inode, loff_t offset, struct uprobe_consumer *uc)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline int uprobe_mmap(struct vm_area_struct *vma)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void
// /*AFLA*/ uprobe_munmap(struct vm_area_struct *vma, unsigned long start, unsigned long end)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void uprobe_start_dup_mmap(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void uprobe_end_dup_mmap(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void
// /*AFLA*/ uprobe_dup_mmap(struct mm_struct *oldmm, struct mm_struct *newmm)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void uprobe_notify_resume(struct pt_regs *regs)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline bool uprobe_deny_signal(void)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ static inline void uprobe_free_utask(struct task_struct *t)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void uprobe_copy_process(struct task_struct *t, unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void uprobe_clear_state(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif /* !CONFIG_UPROBES */
// /*AFLA*/ #endif	/* _LINUX_UPROBES_H */
