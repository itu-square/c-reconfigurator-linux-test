// /*AFLA*/ #ifndef _ASM_X86_PTRACE_ABI_H
// /*AFLA*/ #define _ASM_X86_PTRACE_ABI_H
// /*AFLA*/ 
// /*AFLA*/ #ifdef __i386__
// /*AFLA*/ 
// /*AFLA*/ #define EBX 0
// /*AFLA*/ #define ECX 1
// /*AFLA*/ #define EDX 2
// /*AFLA*/ #define ESI 3
// /*AFLA*/ #define EDI 4
// /*AFLA*/ #define EBP 5
// /*AFLA*/ #define EAX 6
// /*AFLA*/ #define DS 7
// /*AFLA*/ #define ES 8
// /*AFLA*/ #define FS 9
// /*AFLA*/ #define GS 10
// /*AFLA*/ #define ORIG_EAX 11
// /*AFLA*/ #define EIP 12
// /*AFLA*/ #define CS  13
// /*AFLA*/ #define EFL 14
// /*AFLA*/ #define UESP 15
// /*AFLA*/ #define SS   16
// /*AFLA*/ #define FRAME_SIZE 17
// /*AFLA*/ 
// /*AFLA*/ #else /* __i386__ */
// /*AFLA*/ 
// /*AFLA*/ #if defined(__ASSEMBLY__) || defined(__FRAME_OFFSETS)
// /*AFLA*/ /*
// /*AFLA*/  * C ABI says these regs are callee-preserved. They aren't saved on kernel entry
// /*AFLA*/  * unless syscall needs a complete, fully filled "struct pt_regs".
// /*AFLA*/  */
// /*AFLA*/ #define R15 0
// /*AFLA*/ #define R14 8
// /*AFLA*/ #define R13 16
// /*AFLA*/ #define R12 24
// /*AFLA*/ #define RBP 32
// /*AFLA*/ #define RBX 40
// /*AFLA*/ /* These regs are callee-clobbered. Always saved on kernel entry. */
// /*AFLA*/ #define R11 48
// /*AFLA*/ #define R10 56
// /*AFLA*/ #define R9 64
// /*AFLA*/ #define R8 72
// /*AFLA*/ #define RAX 80
// /*AFLA*/ #define RCX 88
// /*AFLA*/ #define RDX 96
// /*AFLA*/ #define RSI 104
// /*AFLA*/ #define RDI 112
// /*AFLA*/ /*
// /*AFLA*/  * On syscall entry, this is syscall#. On CPU exception, this is error code.
// /*AFLA*/  * On hw interrupt, it's IRQ number:
// /*AFLA*/  */
// /*AFLA*/ #define ORIG_RAX 120
// /*AFLA*/ /* Return frame for iretq */
// /*AFLA*/ #define RIP 128
// /*AFLA*/ #define CS 136
// /*AFLA*/ #define EFLAGS 144
// /*AFLA*/ #define RSP 152
// /*AFLA*/ #define SS 160
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ /* top of stack page */
// /*AFLA*/ #define FRAME_SIZE 168
// /*AFLA*/ 
// /*AFLA*/ #endif /* !__i386__ */
// /*AFLA*/ 
// /*AFLA*/ /* Arbitrarily choose the same ptrace numbers as used by the Sparc code. */
// /*AFLA*/ #define PTRACE_GETREGS            12
// /*AFLA*/ #define PTRACE_SETREGS            13
// /*AFLA*/ #define PTRACE_GETFPREGS          14
// /*AFLA*/ #define PTRACE_SETFPREGS          15
// /*AFLA*/ #define PTRACE_GETFPXREGS         18
// /*AFLA*/ #define PTRACE_SETFPXREGS         19
// /*AFLA*/ 
// /*AFLA*/ #define PTRACE_OLDSETOPTIONS      21
// /*AFLA*/ 
// /*AFLA*/ /* only useful for access 32bit programs / kernels */
// /*AFLA*/ #define PTRACE_GET_THREAD_AREA    25
// /*AFLA*/ #define PTRACE_SET_THREAD_AREA    26
// /*AFLA*/ 
// /*AFLA*/ #ifdef __x86_64__
// /*AFLA*/ # define PTRACE_ARCH_PRCTL	  30
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define PTRACE_SYSEMU		  31
// /*AFLA*/ #define PTRACE_SYSEMU_SINGLESTEP  32
// /*AFLA*/ 
// /*AFLA*/ #define PTRACE_SINGLEBLOCK	33	/* resume execution until next branch */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_PTRACE_ABI_H */
