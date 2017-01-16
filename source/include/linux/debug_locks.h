// /*AFLA*/ #ifndef __LINUX_DEBUG_LOCKING_H
// /*AFLA*/ #define __LINUX_DEBUG_LOCKING_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ 
// /*AFLA*/ struct task_struct;
// /*AFLA*/ 
// /*AFLA*/ extern int debug_locks;
// /*AFLA*/ extern int debug_locks_silent;
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ static inline int __debug_locks_off(void)
// /*AFLA*/ {
// /*AFLA*/ 	return xchg(&debug_locks, 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Generic 'turn off all lock debugging' function:
// /*AFLA*/  */
// /*AFLA*/ extern int debug_locks_off(void);
// /*AFLA*/ 
// /*AFLA*/ #define DEBUG_LOCKS_WARN_ON(c)						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	int __ret = 0;							\
// /*AFLA*/ 									\
// /*AFLA*/ 	if (!oops_in_progress && unlikely(c)) {				\
// /*AFLA*/ 		if (debug_locks_off() && !debug_locks_silent)		\
// /*AFLA*/ 			WARN(1, "DEBUG_LOCKS_WARN_ON(%s)", #c);		\
// /*AFLA*/ 		__ret = 1;						\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	__ret;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ # define SMP_DEBUG_LOCKS_WARN_ON(c)			DEBUG_LOCKS_WARN_ON(c)
// /*AFLA*/ #else
// /*AFLA*/ # define SMP_DEBUG_LOCKS_WARN_ON(c)			do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCKING_API_SELFTESTS
// /*AFLA*/   extern void locking_selftest(void);
// /*AFLA*/ #else
// /*AFLA*/ # define locking_selftest()	do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct task_struct;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_LOCKDEP
// /*AFLA*/ extern void debug_show_all_locks(void);
// /*AFLA*/ extern void debug_show_held_locks(struct task_struct *task);
// /*AFLA*/ extern void debug_check_no_locks_freed(const void *from, unsigned long len);
// /*AFLA*/ extern void debug_check_no_locks_held(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void debug_show_all_locks(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void debug_show_held_locks(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ debug_check_no_locks_freed(const void *from, unsigned long len)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ debug_check_no_locks_held(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
