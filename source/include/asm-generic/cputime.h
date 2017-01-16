// /*AFLA*/ #ifndef _ASM_GENERIC_CPUTIME_H
// /*AFLA*/ #define _ASM_GENERIC_CPUTIME_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/time.h>
// /*AFLA*/ #include <linux/jiffies.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_VIRT_CPU_ACCOUNTING
// /*AFLA*/ # include <asm-generic/cputime_jiffies.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
// /*AFLA*/ # include <asm-generic/cputime_nsecs.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
