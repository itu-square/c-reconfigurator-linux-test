// /*AFLA*/ #ifndef _ASM_X86_USER32_H
// /*AFLA*/ #define _ASM_X86_USER32_H
// /*AFLA*/ 
// /*AFLA*/ /* IA32 compatible user structures for ptrace.
// /*AFLA*/  * These should be used for 32bit coredumps too. */
// /*AFLA*/ 
// /*AFLA*/ struct user_i387_ia32_struct {
// /*AFLA*/ 	u32	cwd;
// /*AFLA*/ 	u32	swd;
// /*AFLA*/ 	u32	twd;
// /*AFLA*/ 	u32	fip;
// /*AFLA*/ 	u32	fcs;
// /*AFLA*/ 	u32	foo;
// /*AFLA*/ 	u32	fos;
// /*AFLA*/ 	u32	st_space[20];   /* 8*10 bytes for each FP-reg = 80 bytes */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* FSAVE frame with extensions */
// /*AFLA*/ struct user32_fxsr_struct {
// /*AFLA*/ 	unsigned short	cwd;
// /*AFLA*/ 	unsigned short	swd;
// /*AFLA*/ 	unsigned short	twd;	/* not compatible to 64bit twd */
// /*AFLA*/ 	unsigned short	fop;
// /*AFLA*/ 	int	fip;
// /*AFLA*/ 	int	fcs;
// /*AFLA*/ 	int	foo;
// /*AFLA*/ 	int	fos;
// /*AFLA*/ 	int	mxcsr;
// /*AFLA*/ 	int	reserved;
// /*AFLA*/ 	int	st_space[32];	/* 8*16 bytes for each FP-reg = 128 bytes */
// /*AFLA*/ 	int	xmm_space[32];	/* 8*16 bytes for each XMM-reg = 128 bytes */
// /*AFLA*/ 	int	padding[56];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct user_regs_struct32 {
// /*AFLA*/ 	__u32 ebx, ecx, edx, esi, edi, ebp, eax;
// /*AFLA*/ 	unsigned short ds, __ds, es, __es;
// /*AFLA*/ 	unsigned short fs, __fs, gs, __gs;
// /*AFLA*/ 	__u32 orig_eax, eip;
// /*AFLA*/ 	unsigned short cs, __cs;
// /*AFLA*/ 	__u32 eflags, esp;
// /*AFLA*/ 	unsigned short ss, __ss;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct user32 {
// /*AFLA*/   struct user_regs_struct32 regs; /* Where the registers are actually stored */
// /*AFLA*/   int u_fpvalid;		/* True if math co-processor being used. */
// /*AFLA*/ 				/* for this mess. Not yet used. */
// /*AFLA*/   struct user_i387_ia32_struct i387;	/* Math Co-processor registers. */
// /*AFLA*/ /* The rest of this junk is to help gdb figure out what goes where */
// /*AFLA*/   __u32 u_tsize;	/* Text segment size (pages). */
// /*AFLA*/   __u32 u_dsize;	/* Data segment size (pages). */
// /*AFLA*/   __u32 u_ssize;	/* Stack segment size (pages). */
// /*AFLA*/   __u32 start_code;     /* Starting virtual address of text. */
// /*AFLA*/   __u32 start_stack;	/* Starting virtual address of stack area.
// /*AFLA*/ 				   This is actually the bottom of the stack,
// /*AFLA*/ 				   the top of the stack is always found in the
// /*AFLA*/ 				   esp register.  */
// /*AFLA*/   __u32 signal;     		/* Signal that caused the core dump. */
// /*AFLA*/   int reserved;			/* No __u32er used */
// /*AFLA*/   __u32 u_ar0;	/* Used by gdb to help find the values for */
// /*AFLA*/ 				/* the registers. */
// /*AFLA*/   __u32 u_fpstate;	/* Math Co-processor pointer. */
// /*AFLA*/   __u32 magic;		/* To uniquely identify a core file */
// /*AFLA*/   char u_comm[32];		/* User command that was responsible */
// /*AFLA*/   int u_debugreg[8];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_USER32_H */
