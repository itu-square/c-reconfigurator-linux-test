// /*AFLA*/ #ifndef _ASM_X86_SPINLOCK_TYPES_H
// /*AFLA*/ #define _ASM_X86_SPINLOCK_TYPES_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PARAVIRT_SPINLOCKS
// /*AFLA*/ #define __TICKET_LOCK_INC	2
// /*AFLA*/ #define TICKET_SLOWPATH_FLAG	((__ticket_t)1)
// /*AFLA*/ #else
// /*AFLA*/ #define __TICKET_LOCK_INC	1
// /*AFLA*/ #define TICKET_SLOWPATH_FLAG	((__ticket_t)0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if (CONFIG_NR_CPUS < (256 / __TICKET_LOCK_INC))
// /*AFLA*/ typedef u8  __ticket_t;
// /*AFLA*/ typedef u16 __ticketpair_t;
// /*AFLA*/ #else
// /*AFLA*/ typedef u16 __ticket_t;
// /*AFLA*/ typedef u32 __ticketpair_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define TICKET_LOCK_INC	((__ticket_t)__TICKET_LOCK_INC)
// /*AFLA*/ 
// /*AFLA*/ #define TICKET_SHIFT	(sizeof(__ticket_t) * 8)
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/qspinlock_types.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/qrwlock_types.h>
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_SPINLOCK_TYPES_H */
