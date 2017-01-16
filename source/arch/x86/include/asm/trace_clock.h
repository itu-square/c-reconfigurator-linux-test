// /*AFLA*/ #ifndef _ASM_X86_TRACE_CLOCK_H
// /*AFLA*/ #define _ASM_X86_TRACE_CLOCK_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_TSC
// /*AFLA*/ 
// /*AFLA*/ extern u64 notrace trace_clock_x86_tsc(void);
// /*AFLA*/ 
// /*AFLA*/ # define ARCH_TRACE_CLOCKS \
// /*AFLA*/ 	{ trace_clock_x86_tsc,	"x86-tsc",	.in_ns = 0 },
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_X86_TSC */
// /*AFLA*/ 
// /*AFLA*/ #define ARCH_TRACE_CLOCKS
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif  /* _ASM_X86_TRACE_CLOCK_H */
