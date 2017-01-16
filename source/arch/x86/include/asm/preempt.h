// /*AFLA*/ #ifndef __ASM_PREEMPT_H
// /*AFLA*/ #define __ASM_PREEMPT_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/rmwcc.h>
// /*AFLA*/ #include <asm/percpu.h>
// /*AFLA*/ #include <linux/thread_info.h>
// /*AFLA*/ 
// /*AFLA*/ DECLARE_PER_CPU(int, __preempt_count);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We use the PREEMPT_NEED_RESCHED bit as an inverted NEED_RESCHED such
// /*AFLA*/  * that a decrement hitting 0 means we can and should reschedule.
// /*AFLA*/  */
// /*AFLA*/ #define PREEMPT_ENABLED	(0 + PREEMPT_NEED_RESCHED)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We mask the PREEMPT_NEED_RESCHED bit so as not to confuse all current users
// /*AFLA*/  * that think a non-zero value indicates we cannot preempt.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline int preempt_count(void)
// /*AFLA*/ {
// /*AFLA*/ 	return raw_cpu_read_4(__preempt_count) & ~PREEMPT_NEED_RESCHED;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void preempt_count_set(int pc)
// /*AFLA*/ {
// /*AFLA*/ 	raw_cpu_write_4(__preempt_count, pc);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * must be macros to avoid header recursion hell
// /*AFLA*/  */
// /*AFLA*/ #define init_task_preempt_count(p) do { } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define init_idle_preempt_count(p, cpu) do { \
// /*AFLA*/ 	per_cpu(__preempt_count, (cpu)) = PREEMPT_ENABLED; \
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We fold the NEED_RESCHED bit into the preempt count such that
// /*AFLA*/  * preempt_enable() can decrement and test for needing to reschedule with a
// /*AFLA*/  * single instruction.
// /*AFLA*/  *
// /*AFLA*/  * We invert the actual bit, so that when the decrement hits 0 we know we both
// /*AFLA*/  * need to resched (the bit is cleared) and can resched (no preempt count).
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void set_preempt_need_resched(void)
// /*AFLA*/ {
// /*AFLA*/ 	raw_cpu_and_4(__preempt_count, ~PREEMPT_NEED_RESCHED);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void clear_preempt_need_resched(void)
// /*AFLA*/ {
// /*AFLA*/ 	raw_cpu_or_4(__preempt_count, PREEMPT_NEED_RESCHED);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline bool test_preempt_need_resched(void)
// /*AFLA*/ {
// /*AFLA*/ 	return !(raw_cpu_read_4(__preempt_count) & PREEMPT_NEED_RESCHED);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The various preempt_count add/sub methods
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void __preempt_count_add(int val)
// /*AFLA*/ {
// /*AFLA*/ 	raw_cpu_add_4(__preempt_count, val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void __preempt_count_sub(int val)
// /*AFLA*/ {
// /*AFLA*/ 	raw_cpu_add_4(__preempt_count, -val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Because we keep PREEMPT_NEED_RESCHED set when we do _not_ need to reschedule
// /*AFLA*/  * a decrement which hits zero means we have no preempt_count and should
// /*AFLA*/  * reschedule.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline bool __preempt_count_dec_and_test(void)
// /*AFLA*/ {
// /*AFLA*/ 	GEN_UNARY_RMWcc("decl", __preempt_count, __percpu_arg(0), e);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Returns true when we need to resched and can (barring IRQ state).
// /*AFLA*/  */
// /*AFLA*/ static __always_inline bool should_resched(int preempt_offset)
// /*AFLA*/ {
// /*AFLA*/ 	return unlikely(raw_cpu_read_4(__preempt_count) == preempt_offset);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PREEMPT
// /*AFLA*/   extern asmlinkage void ___preempt_schedule(void);
// /*AFLA*/ # define __preempt_schedule()					\
// /*AFLA*/ ({								\
// /*AFLA*/ 	register void *__sp asm(_ASM_SP);			\
// /*AFLA*/ 	asm volatile ("call ___preempt_schedule" : "+r"(__sp));	\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/   extern asmlinkage void preempt_schedule(void);
// /*AFLA*/   extern asmlinkage void ___preempt_schedule_notrace(void);
// /*AFLA*/ # define __preempt_schedule_notrace()					\
// /*AFLA*/ ({									\
// /*AFLA*/ 	register void *__sp asm(_ASM_SP);				\
// /*AFLA*/ 	asm volatile ("call ___preempt_schedule_notrace" : "+r"(__sp));	\
// /*AFLA*/ })
// /*AFLA*/   extern asmlinkage void preempt_schedule_notrace(void);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASM_PREEMPT_H */
