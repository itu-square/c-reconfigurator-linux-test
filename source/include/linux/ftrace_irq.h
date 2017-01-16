// /*AFLA*/ #ifndef _LINUX_FTRACE_IRQ_H
// /*AFLA*/ #define _LINUX_FTRACE_IRQ_H
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_FTRACE_NMI_ENTER
// /*AFLA*/ extern void arch_ftrace_nmi_enter(void);
// /*AFLA*/ extern void arch_ftrace_nmi_exit(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void arch_ftrace_nmi_enter(void) { }
// /*AFLA*/ static inline void arch_ftrace_nmi_exit(void) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HWLAT_TRACER
// /*AFLA*/ extern bool trace_hwlat_callback_enabled;
// /*AFLA*/ extern void trace_hwlat_callback(bool enter);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline void ftrace_nmi_enter(void)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_HWLAT_TRACER
// /*AFLA*/ 	if (trace_hwlat_callback_enabled)
// /*AFLA*/ 		trace_hwlat_callback(true);
// /*AFLA*/ #endif
// /*AFLA*/ 	arch_ftrace_nmi_enter();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ftrace_nmi_exit(void)
// /*AFLA*/ {
// /*AFLA*/ 	arch_ftrace_nmi_exit();
// /*AFLA*/ #ifdef CONFIG_HWLAT_TRACER
// /*AFLA*/ 	if (trace_hwlat_callback_enabled)
// /*AFLA*/ 		trace_hwlat_callback(false);
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_FTRACE_IRQ_H */
