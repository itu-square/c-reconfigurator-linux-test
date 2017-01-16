// /*AFLA*/ /* thread_info.h: common low-level thread information accessors
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2002  David Howells (dhowells@redhat.com)
// /*AFLA*/  * - Incorporating suggestions made by Linus Torvalds
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_THREAD_INFO_H
// /*AFLA*/ #define _LINUX_THREAD_INFO_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ 
// /*AFLA*/ struct timespec;
// /*AFLA*/ struct compat_timespec;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_THREAD_INFO_IN_TASK
// /*AFLA*/ #define current_thread_info() ((struct thread_info *)current)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * System call restart block.
// /*AFLA*/  */
// /*AFLA*/ struct restart_block {
// /*AFLA*/ 	long (*fn)(struct restart_block *);
// /*AFLA*/ 	union {
// /*AFLA*/ 		/* For futex_wait and futex_wait_requeue_pi */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			u32 __user *uaddr;
// /*AFLA*/ 			u32 val;
// /*AFLA*/ 			u32 flags;
// /*AFLA*/ 			u32 bitset;
// /*AFLA*/ 			u64 time;
// /*AFLA*/ 			u32 __user *uaddr2;
// /*AFLA*/ 		} futex;
// /*AFLA*/ 		/* For nanosleep */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			clockid_t clockid;
// /*AFLA*/ 			struct timespec __user *rmtp;
// /*AFLA*/ #ifdef CONFIG_COMPAT
// /*AFLA*/ 			struct compat_timespec __user *compat_rmtp;
// /*AFLA*/ #endif
// /*AFLA*/ 			u64 expires;
// /*AFLA*/ 		} nanosleep;
// /*AFLA*/ 		/* For poll */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			struct pollfd __user *ufds;
// /*AFLA*/ 			int nfds;
// /*AFLA*/ 			int has_timeout;
// /*AFLA*/ 			unsigned long tv_sec;
// /*AFLA*/ 			unsigned long tv_nsec;
// /*AFLA*/ 		} poll;
// /*AFLA*/ 	};
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern long do_no_restart_syscall(struct restart_block *parm);
// /*AFLA*/ 
// /*AFLA*/ #include <linux/bitops.h>
// /*AFLA*/ #include <asm/thread_info.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef __KERNEL__
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_STACK_USAGE
// /*AFLA*/ # define THREADINFO_GFP		(GFP_KERNEL_ACCOUNT | __GFP_NOTRACK | \
// /*AFLA*/ 				 __GFP_ZERO)
// /*AFLA*/ #else
// /*AFLA*/ # define THREADINFO_GFP		(GFP_KERNEL_ACCOUNT | __GFP_NOTRACK)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * flag set/clear/test wrappers
// /*AFLA*/  * - pass TIF_xxxx constants to these functions
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static inline void set_ti_thread_flag(struct thread_info *ti, int flag)
// /*AFLA*/ {
// /*AFLA*/ 	set_bit(flag, (unsigned long *)&ti->flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void clear_ti_thread_flag(struct thread_info *ti, int flag)
// /*AFLA*/ {
// /*AFLA*/ 	clear_bit(flag, (unsigned long *)&ti->flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int test_and_set_ti_thread_flag(struct thread_info *ti, int flag)
// /*AFLA*/ {
// /*AFLA*/ 	return test_and_set_bit(flag, (unsigned long *)&ti->flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int test_and_clear_ti_thread_flag(struct thread_info *ti, int flag)
// /*AFLA*/ {
// /*AFLA*/ 	return test_and_clear_bit(flag, (unsigned long *)&ti->flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int test_ti_thread_flag(struct thread_info *ti, int flag)
// /*AFLA*/ {
// /*AFLA*/ 	return test_bit(flag, (unsigned long *)&ti->flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define set_thread_flag(flag) \
// /*AFLA*/ 	set_ti_thread_flag(current_thread_info(), flag)
// /*AFLA*/ #define clear_thread_flag(flag) \
// /*AFLA*/ 	clear_ti_thread_flag(current_thread_info(), flag)
// /*AFLA*/ #define test_and_set_thread_flag(flag) \
// /*AFLA*/ 	test_and_set_ti_thread_flag(current_thread_info(), flag)
// /*AFLA*/ #define test_and_clear_thread_flag(flag) \
// /*AFLA*/ 	test_and_clear_ti_thread_flag(current_thread_info(), flag)
// /*AFLA*/ #define test_thread_flag(flag) \
// /*AFLA*/ 	test_ti_thread_flag(current_thread_info(), flag)
// /*AFLA*/ 
// /*AFLA*/ #define tif_need_resched() test_thread_flag(TIF_NEED_RESCHED)
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_HAVE_ARCH_WITHIN_STACK_FRAMES
// /*AFLA*/ static inline int arch_within_stack_frames(const void * const stack,
// /*AFLA*/ 					   const void * const stackend,
// /*AFLA*/ 					   const void *obj, unsigned long len)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HARDENED_USERCOPY
// /*AFLA*/ extern void __check_object_size(const void *ptr, unsigned long n,
// /*AFLA*/ 					bool to_user);
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void check_object_size(const void *ptr, unsigned long n,
// /*AFLA*/ 					      bool to_user)
// /*AFLA*/ {
// /*AFLA*/ 	if (!__builtin_constant_p(n))
// /*AFLA*/ 		__check_object_size(ptr, n, to_user);
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline void check_object_size(const void *ptr, unsigned long n,
// /*AFLA*/ 				     bool to_user)
// /*AFLA*/ { }
// /*AFLA*/ #endif /* CONFIG_HARDENED_USERCOPY */
// /*AFLA*/ 
// /*AFLA*/ #endif	/* __KERNEL__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_THREAD_INFO_H */
