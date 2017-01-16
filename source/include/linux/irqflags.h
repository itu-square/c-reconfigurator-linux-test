// /*AFLA*/ /*
// /*AFLA*/  * include/linux/irqflags.h
// /*AFLA*/  *
// /*AFLA*/  * IRQ flags tracing: follow the state of the hardirq and softirq flags and
// /*AFLA*/  * provide callbacks for transitions between ON and OFF states.
// /*AFLA*/  *
// /*AFLA*/  * This file gets included from lowlevel asm headers too, to provide
// /*AFLA*/  * wrapped versions of the local_irq_*() APIs, based on the
// /*AFLA*/  * raw_local_irq_*() macros from the lowlevel headers.
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LINUX_TRACE_IRQFLAGS_H
// /*AFLA*/ #define _LINUX_TRACE_IRQFLAGS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/typecheck.h>
// /*AFLA*/ #include <asm/irqflags.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TRACE_IRQFLAGS
// /*AFLA*/   extern void trace_softirqs_on(unsigned long ip);
// /*AFLA*/   extern void trace_softirqs_off(unsigned long ip);
// /*AFLA*/   extern void trace_hardirqs_on(void);
// /*AFLA*/   extern void trace_hardirqs_off(void);
// /*AFLA*/ # define trace_hardirq_context(p)	((p)->hardirq_context)
// /*AFLA*/ # define trace_softirq_context(p)	((p)->softirq_context)
// /*AFLA*/ # define trace_hardirqs_enabled(p)	((p)->hardirqs_enabled)
// /*AFLA*/ # define trace_softirqs_enabled(p)	((p)->softirqs_enabled)
// /*AFLA*/ # define trace_hardirq_enter()	do { current->hardirq_context++; } while (0)
// /*AFLA*/ # define trace_hardirq_exit()	do { current->hardirq_context--; } while (0)
// /*AFLA*/ # define lockdep_softirq_enter()	do { current->softirq_context++; } while (0)
// /*AFLA*/ # define lockdep_softirq_exit()	do { current->softirq_context--; } while (0)
// /*AFLA*/ # define INIT_TRACE_IRQFLAGS	.softirqs_enabled = 1,
// /*AFLA*/ #else
// /*AFLA*/ # define trace_hardirqs_on()		do { } while (0)
// /*AFLA*/ # define trace_hardirqs_off()		do { } while (0)
// /*AFLA*/ # define trace_softirqs_on(ip)		do { } while (0)
// /*AFLA*/ # define trace_softirqs_off(ip)		do { } while (0)
// /*AFLA*/ # define trace_hardirq_context(p)	0
// /*AFLA*/ # define trace_softirq_context(p)	0
// /*AFLA*/ # define trace_hardirqs_enabled(p)	0
// /*AFLA*/ # define trace_softirqs_enabled(p)	0
// /*AFLA*/ # define trace_hardirq_enter()		do { } while (0)
// /*AFLA*/ # define trace_hardirq_exit()		do { } while (0)
// /*AFLA*/ # define lockdep_softirq_enter()	do { } while (0)
// /*AFLA*/ # define lockdep_softirq_exit()		do { } while (0)
// /*AFLA*/ # define INIT_TRACE_IRQFLAGS
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_IRQSOFF_TRACER) || \
// /*AFLA*/ 	defined(CONFIG_PREEMPT_TRACER)
// /*AFLA*/  extern void stop_critical_timings(void);
// /*AFLA*/  extern void start_critical_timings(void);
// /*AFLA*/ #else
// /*AFLA*/ # define stop_critical_timings() do { } while (0)
// /*AFLA*/ # define start_critical_timings() do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Wrap the arch provided IRQ routines to provide appropriate checks.
// /*AFLA*/  */
// /*AFLA*/ #define raw_local_irq_disable()		arch_local_irq_disable()
// /*AFLA*/ #define raw_local_irq_enable()		arch_local_irq_enable()
// /*AFLA*/ #define raw_local_irq_save(flags)			\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		typecheck(unsigned long, flags);	\
// /*AFLA*/ 		flags = arch_local_irq_save();		\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #define raw_local_irq_restore(flags)			\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		typecheck(unsigned long, flags);	\
// /*AFLA*/ 		arch_local_irq_restore(flags);		\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #define raw_local_save_flags(flags)			\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		typecheck(unsigned long, flags);	\
// /*AFLA*/ 		flags = arch_local_save_flags();	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #define raw_irqs_disabled_flags(flags)			\
// /*AFLA*/ 	({						\
// /*AFLA*/ 		typecheck(unsigned long, flags);	\
// /*AFLA*/ 		arch_irqs_disabled_flags(flags);	\
// /*AFLA*/ 	})
// /*AFLA*/ #define raw_irqs_disabled()		(arch_irqs_disabled())
// /*AFLA*/ #define raw_safe_halt()			arch_safe_halt()
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The local_irq_*() APIs are equal to the raw_local_irq*()
// /*AFLA*/  * if !TRACE_IRQFLAGS.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_TRACE_IRQFLAGS
// /*AFLA*/ #define local_irq_enable() \
// /*AFLA*/ 	do { trace_hardirqs_on(); raw_local_irq_enable(); } while (0)
// /*AFLA*/ #define local_irq_disable() \
// /*AFLA*/ 	do { raw_local_irq_disable(); trace_hardirqs_off(); } while (0)
// /*AFLA*/ #define local_irq_save(flags)				\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		raw_local_irq_save(flags);		\
// /*AFLA*/ 		trace_hardirqs_off();			\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define local_irq_restore(flags)			\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		if (raw_irqs_disabled_flags(flags)) {	\
// /*AFLA*/ 			raw_local_irq_restore(flags);	\
// /*AFLA*/ 			trace_hardirqs_off();		\
// /*AFLA*/ 		} else {				\
// /*AFLA*/ 			trace_hardirqs_on();		\
// /*AFLA*/ 			raw_local_irq_restore(flags);	\
// /*AFLA*/ 		}					\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #define safe_halt()				\
// /*AFLA*/ 	do {					\
// /*AFLA*/ 		trace_hardirqs_on();		\
// /*AFLA*/ 		raw_safe_halt();		\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_TRACE_IRQFLAGS */
// /*AFLA*/ 
// /*AFLA*/ #define local_irq_enable()	do { raw_local_irq_enable(); } while (0)
// /*AFLA*/ #define local_irq_disable()	do { raw_local_irq_disable(); } while (0)
// /*AFLA*/ #define local_irq_save(flags)					\
// /*AFLA*/ 	do {							\
// /*AFLA*/ 		raw_local_irq_save(flags);			\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #define local_irq_restore(flags) do { raw_local_irq_restore(flags); } while (0)
// /*AFLA*/ #define safe_halt()		do { raw_safe_halt(); } while (0)
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_TRACE_IRQFLAGS */
// /*AFLA*/ 
// /*AFLA*/ #define local_save_flags(flags)	raw_local_save_flags(flags)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Some architectures don't define arch_irqs_disabled(), so even if either
// /*AFLA*/  * definition would be fine we need to use different ones for the time being
// /*AFLA*/  * to avoid build issues.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_TRACE_IRQFLAGS_SUPPORT
// /*AFLA*/ #define irqs_disabled()					\
// /*AFLA*/ 	({						\
// /*AFLA*/ 		unsigned long _flags;			\
// /*AFLA*/ 		raw_local_save_flags(_flags);		\
// /*AFLA*/ 		raw_irqs_disabled_flags(_flags);	\
// /*AFLA*/ 	})
// /*AFLA*/ #else /* !CONFIG_TRACE_IRQFLAGS_SUPPORT */
// /*AFLA*/ #define irqs_disabled()	raw_irqs_disabled()
// /*AFLA*/ #endif /* CONFIG_TRACE_IRQFLAGS_SUPPORT */
// /*AFLA*/ 
// /*AFLA*/ #define irqs_disabled_flags(flags) raw_irqs_disabled_flags(flags)
// /*AFLA*/ 
// /*AFLA*/ #endif
