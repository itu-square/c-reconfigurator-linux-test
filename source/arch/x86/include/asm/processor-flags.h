// /*AFLA*/ #ifndef _ASM_X86_PROCESSOR_FLAGS_H
// /*AFLA*/ #define _ASM_X86_PROCESSOR_FLAGS_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/asm/processor-flags.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_VM86
// /*AFLA*/ #define X86_VM_MASK	X86_EFLAGS_VM
// /*AFLA*/ #else
// /*AFLA*/ #define X86_VM_MASK	0 /* No VM86 support */
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* _ASM_X86_PROCESSOR_FLAGS_H */
