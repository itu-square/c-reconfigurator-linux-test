// /*AFLA*/ #ifndef _ASM_X86_SPINLOCK_H
// /*AFLA*/ #define _ASM_X86_SPINLOCK_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/jump_label.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <asm/page.h>
// /*AFLA*/ #include <asm/processor.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <asm/paravirt.h>
// /*AFLA*/ #include <asm/bitops.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Your basic SMP spinlocks, allowing only a single CPU anywhere
// /*AFLA*/  *
// /*AFLA*/  * Simple spin lock operations.  There are two variants, one clears IRQ's
// /*AFLA*/  * on the local processor, one does not.
// /*AFLA*/  *
// /*AFLA*/  * These are fair FIFO ticket locks, which support up to 2^16 CPUs.
// /*AFLA*/  *
// /*AFLA*/  * (the type definitions are in asm/spinlock_types.h)
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* How long a lock should spin before we consider blocking */
// /*AFLA*/ #define SPIN_THRESHOLD	(1 << 15)
// /*AFLA*/ 
// /*AFLA*/ extern struct static_key paravirt_ticketlocks_enabled;
// /*AFLA*/ static __always_inline bool static_key_false(struct static_key *key);
// /*AFLA*/ 
// /*AFLA*/ #include <asm/qspinlock.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Read-write spinlocks, allowing multiple readers
// /*AFLA*/  * but only one writer.
// /*AFLA*/  *
// /*AFLA*/  * NOTE! it is quite common to have readers in interrupts
// /*AFLA*/  * but no interrupt writers. For those circumstances we
// /*AFLA*/  * can "mix" irq-safe locks - any writer needs to get a
// /*AFLA*/  * irq-safe write-lock, but readers can get non-irqsafe
// /*AFLA*/  * read-locks.
// /*AFLA*/  *
// /*AFLA*/  * On x86, we implement read-write locks using the generic qrwlock with
// /*AFLA*/  * x86 specific optimization.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <asm/qrwlock.h>
// /*AFLA*/ 
// /*AFLA*/ #define arch_read_lock_flags(lock, flags) arch_read_lock(lock)
// /*AFLA*/ #define arch_write_lock_flags(lock, flags) arch_write_lock(lock)
// /*AFLA*/ 
// /*AFLA*/ #define arch_spin_relax(lock)	cpu_relax()
// /*AFLA*/ #define arch_read_relax(lock)	cpu_relax()
// /*AFLA*/ #define arch_write_relax(lock)	cpu_relax()
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_SPINLOCK_H */
