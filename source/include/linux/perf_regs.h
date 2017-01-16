// /*AFLA*/ #ifndef _LINUX_PERF_REGS_H
// /*AFLA*/ #define _LINUX_PERF_REGS_H
// /*AFLA*/ 
// /*AFLA*/ struct perf_regs {
// /*AFLA*/ 	__u64		abi;
// /*AFLA*/ 	struct pt_regs	*regs;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_PERF_REGS
// /*AFLA*/ #include <asm/perf_regs.h>
// /*AFLA*/ u64 perf_reg_value(struct pt_regs *regs, int idx);
// /*AFLA*/ int perf_reg_validate(u64 mask);
// /*AFLA*/ u64 perf_reg_abi(struct task_struct *task);
// /*AFLA*/ void perf_get_regs_user(struct perf_regs *regs_user,
// /*AFLA*/ 			struct pt_regs *regs,
// /*AFLA*/ 			struct pt_regs *regs_user_copy);
// /*AFLA*/ #else
// /*AFLA*/ static inline u64 perf_reg_value(struct pt_regs *regs, int idx)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int perf_reg_validate(u64 mask)
// /*AFLA*/ {
// /*AFLA*/ 	return mask ? -ENOSYS : 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u64 perf_reg_abi(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ 	return PERF_SAMPLE_REGS_ABI_NONE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void perf_get_regs_user(struct perf_regs *regs_user,
// /*AFLA*/ 				      struct pt_regs *regs,
// /*AFLA*/ 				      struct pt_regs *regs_user_copy)
// /*AFLA*/ {
// /*AFLA*/ 	regs_user->regs = task_pt_regs(current);
// /*AFLA*/ 	regs_user->abi = perf_reg_abi(current);
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_HAVE_PERF_REGS */
// /*AFLA*/ #endif /* _LINUX_PERF_REGS_H */
