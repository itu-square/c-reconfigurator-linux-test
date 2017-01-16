// /*AFLA*/ #ifndef _UAPI_ASM_X86_SIGCONTEXT_H
// /*AFLA*/ #define _UAPI_ASM_X86_SIGCONTEXT_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Linux signal context definitions. The sigcontext includes a complex
// /*AFLA*/  * hierarchy of CPU and FPU state, available to user-space (on the stack) when
// /*AFLA*/  * a signal handler is executed.
// /*AFLA*/  *
// /*AFLA*/  * As over the years this ABI grew from its very simple roots towards
// /*AFLA*/  * supporting more and more CPU state organically, some of the details (which
// /*AFLA*/  * were rather clever hacks back in the days) became a bit quirky by today.
// /*AFLA*/  *
// /*AFLA*/  * The current ABI includes flexible provisions for future extensions, so we
// /*AFLA*/  * won't have to grow new quirks for quite some time. Promise!
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ #define FP_XSTATE_MAGIC1		0x46505853U
// /*AFLA*/ #define FP_XSTATE_MAGIC2		0x46505845U
// /*AFLA*/ #define FP_XSTATE_MAGIC2_SIZE		sizeof(FP_XSTATE_MAGIC2)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Bytes 464..511 in the current 512-byte layout of the FXSAVE/FXRSTOR frame
// /*AFLA*/  * are reserved for SW usage. On CPUs supporting XSAVE/XRSTOR, these bytes are
// /*AFLA*/  * used to extend the fpstate pointer in the sigcontext, which now includes the
// /*AFLA*/  * extended state information along with fpstate information.
// /*AFLA*/  *
// /*AFLA*/  * If sw_reserved.magic1 == FP_XSTATE_MAGIC1 then there's a
// /*AFLA*/  * sw_reserved.extended_size bytes large extended context area present. (The
// /*AFLA*/  * last 32-bit word of this extended area (at the
// /*AFLA*/  * fpstate+extended_size-FP_XSTATE_MAGIC2_SIZE address) is set to
// /*AFLA*/  * FP_XSTATE_MAGIC2 so that you can sanity check your size calculations.)
// /*AFLA*/  *
// /*AFLA*/  * This extended area typically grows with newer CPUs that have larger and
// /*AFLA*/  * larger XSAVE areas.
// /*AFLA*/  */
// /*AFLA*/ struct _fpx_sw_bytes {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If set to FP_XSTATE_MAGIC1 then this is an xstate context.
// /*AFLA*/ 	 * 0 if a legacy frame.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u32				magic1;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Total size of the fpstate area:
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *  - if magic1 == 0 then it's sizeof(struct _fpstate)
// /*AFLA*/ 	 *  - if magic1 == FP_XSTATE_MAGIC1 then it's sizeof(struct _xstate)
// /*AFLA*/ 	 *    plus extensions (if any)
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u32				extended_size;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Feature bit mask (including FP/SSE/extended state) that is present
// /*AFLA*/ 	 * in the memory layout:
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u64				xfeatures;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Actual XSAVE state size, based on the xfeatures saved in the layout.
// /*AFLA*/ 	 * 'extended_size' is greater than 'xstate_size':
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u32				xstate_size;
// /*AFLA*/ 
// /*AFLA*/ 	/* For future use: */
// /*AFLA*/ 	__u32				padding[7];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * As documented in the iBCS2 standard:
// /*AFLA*/  *
// /*AFLA*/  * The first part of "struct _fpstate" is just the normal i387 hardware setup,
// /*AFLA*/  * the extra "status" word is used to save the coprocessor status word before
// /*AFLA*/  * entering the handler.
// /*AFLA*/  *
// /*AFLA*/  * The FPU state data structure has had to grow to accommodate the extended FPU
// /*AFLA*/  * state required by the Streaming SIMD Extensions.  There is no documented
// /*AFLA*/  * standard to accomplish this at the moment.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* 10-byte legacy floating point register: */
// /*AFLA*/ struct _fpreg {
// /*AFLA*/ 	__u16				significand[4];
// /*AFLA*/ 	__u16				exponent;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* 16-byte floating point register: */
// /*AFLA*/ struct _fpxreg {
// /*AFLA*/ 	__u16				significand[4];
// /*AFLA*/ 	__u16				exponent;
// /*AFLA*/ 	__u16				padding[3];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* 16-byte XMM register: */
// /*AFLA*/ struct _xmmreg {
// /*AFLA*/ 	__u32				element[4];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define X86_FXSR_MAGIC			0x0000
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The 32-bit FPU frame:
// /*AFLA*/  */
// /*AFLA*/ struct _fpstate_32 {
// /*AFLA*/ 	/* Legacy FPU environment: */
// /*AFLA*/ 	__u32				cw;
// /*AFLA*/ 	__u32				sw;
// /*AFLA*/ 	__u32				tag;
// /*AFLA*/ 	__u32				ipoff;
// /*AFLA*/ 	__u32				cssel;
// /*AFLA*/ 	__u32				dataoff;
// /*AFLA*/ 	__u32				datasel;
// /*AFLA*/ 	struct _fpreg			_st[8];
// /*AFLA*/ 	__u16				status;
// /*AFLA*/ 	__u16				magic;		/* 0xffff: regular FPU data only */
// /*AFLA*/ 							/* 0x0000: FXSR FPU data */
// /*AFLA*/ 
// /*AFLA*/ 	/* FXSR FPU environment */
// /*AFLA*/ 	__u32				_fxsr_env[6];	/* FXSR FPU env is ignored */
// /*AFLA*/ 	__u32				mxcsr;
// /*AFLA*/ 	__u32				reserved;
// /*AFLA*/ 	struct _fpxreg			_fxsr_st[8];	/* FXSR FPU reg data is ignored */
// /*AFLA*/ 	struct _xmmreg			_xmm[8];	/* First 8 XMM registers */
// /*AFLA*/ 	union {
// /*AFLA*/ 		__u32			padding1[44];	/* Second 8 XMM registers plus padding */
// /*AFLA*/ 		__u32			padding[44];	/* Alias name for old user-space */
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ 	union {
// /*AFLA*/ 		__u32			padding2[12];
// /*AFLA*/ 		struct _fpx_sw_bytes	sw_reserved;	/* Potential extended state is encoded here */
// /*AFLA*/ 	};
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The 64-bit FPU frame. (FXSAVE format and later)
// /*AFLA*/  *
// /*AFLA*/  * Note1: If sw_reserved.magic1 == FP_XSTATE_MAGIC1 then the structure is
// /*AFLA*/  *        larger: 'struct _xstate'. Note that 'struct _xstate' embedds
// /*AFLA*/  *        'struct _fpstate' so that you can always assume the _fpstate portion
// /*AFLA*/  *        exists so that you can check the magic value.
// /*AFLA*/  *
// /*AFLA*/  * Note2: Reserved fields may someday contain valuable data. Always
// /*AFLA*/  *	  save/restore them when you change signal frames.
// /*AFLA*/  */
// /*AFLA*/ struct _fpstate_64 {
// /*AFLA*/ 	__u16				cwd;
// /*AFLA*/ 	__u16				swd;
// /*AFLA*/ 	/* Note this is not the same as the 32-bit/x87/FSAVE twd: */
// /*AFLA*/ 	__u16				twd;
// /*AFLA*/ 	__u16				fop;
// /*AFLA*/ 	__u64				rip;
// /*AFLA*/ 	__u64				rdp;
// /*AFLA*/ 	__u32				mxcsr;
// /*AFLA*/ 	__u32				mxcsr_mask;
// /*AFLA*/ 	__u32				st_space[32];	/*  8x  FP registers, 16 bytes each */
// /*AFLA*/ 	__u32				xmm_space[64];	/* 16x XMM registers, 16 bytes each */
// /*AFLA*/ 	__u32				reserved2[12];
// /*AFLA*/ 	union {
// /*AFLA*/ 		__u32			reserved3[12];
// /*AFLA*/ 		struct _fpx_sw_bytes	sw_reserved;	/* Potential extended state is encoded here */
// /*AFLA*/ 	};
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef __i386__
// /*AFLA*/ # define _fpstate _fpstate_32
// /*AFLA*/ #else
// /*AFLA*/ # define _fpstate _fpstate_64
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct _header {
// /*AFLA*/ 	__u64				xfeatures;
// /*AFLA*/ 	__u64				reserved1[2];
// /*AFLA*/ 	__u64				reserved2[5];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct _ymmh_state {
// /*AFLA*/ 	/* 16x YMM registers, 16 bytes each: */
// /*AFLA*/ 	__u32				ymmh_space[64];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Extended state pointed to by sigcontext::fpstate.
// /*AFLA*/  *
// /*AFLA*/  * In addition to the fpstate, information encoded in _xstate::xstate_hdr
// /*AFLA*/  * indicates the presence of other extended state information supported
// /*AFLA*/  * by the CPU and kernel:
// /*AFLA*/  */
// /*AFLA*/ struct _xstate {
// /*AFLA*/ 	struct _fpstate			fpstate;
// /*AFLA*/ 	struct _header			xstate_hdr;
// /*AFLA*/ 	struct _ymmh_state		ymmh;
// /*AFLA*/ 	/* New processor state extensions go here: */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The 32-bit signal frame:
// /*AFLA*/  */
// /*AFLA*/ struct sigcontext_32 {
// /*AFLA*/ 	__u16				gs, __gsh;
// /*AFLA*/ 	__u16				fs, __fsh;
// /*AFLA*/ 	__u16				es, __esh;
// /*AFLA*/ 	__u16				ds, __dsh;
// /*AFLA*/ 	__u32				di;
// /*AFLA*/ 	__u32				si;
// /*AFLA*/ 	__u32				bp;
// /*AFLA*/ 	__u32				sp;
// /*AFLA*/ 	__u32				bx;
// /*AFLA*/ 	__u32				dx;
// /*AFLA*/ 	__u32				cx;
// /*AFLA*/ 	__u32				ax;
// /*AFLA*/ 	__u32				trapno;
// /*AFLA*/ 	__u32				err;
// /*AFLA*/ 	__u32				ip;
// /*AFLA*/ 	__u16				cs, __csh;
// /*AFLA*/ 	__u32				flags;
// /*AFLA*/ 	__u32				sp_at_signal;
// /*AFLA*/ 	__u16				ss, __ssh;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * fpstate is really (struct _fpstate *) or (struct _xstate *)
// /*AFLA*/ 	 * depending on the FP_XSTATE_MAGIC1 encoded in the SW reserved
// /*AFLA*/ 	 * bytes of (struct _fpstate) and FP_XSTATE_MAGIC2 present at the end
// /*AFLA*/ 	 * of extended memory layout. See comments at the definition of
// /*AFLA*/ 	 * (struct _fpx_sw_bytes)
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u32				fpstate; /* Zero when no FPU/extended context */
// /*AFLA*/ 	__u32				oldmask;
// /*AFLA*/ 	__u32				cr2;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The 64-bit signal frame:
// /*AFLA*/  */
// /*AFLA*/ struct sigcontext_64 {
// /*AFLA*/ 	__u64				r8;
// /*AFLA*/ 	__u64				r9;
// /*AFLA*/ 	__u64				r10;
// /*AFLA*/ 	__u64				r11;
// /*AFLA*/ 	__u64				r12;
// /*AFLA*/ 	__u64				r13;
// /*AFLA*/ 	__u64				r14;
// /*AFLA*/ 	__u64				r15;
// /*AFLA*/ 	__u64				di;
// /*AFLA*/ 	__u64				si;
// /*AFLA*/ 	__u64				bp;
// /*AFLA*/ 	__u64				bx;
// /*AFLA*/ 	__u64				dx;
// /*AFLA*/ 	__u64				ax;
// /*AFLA*/ 	__u64				cx;
// /*AFLA*/ 	__u64				sp;
// /*AFLA*/ 	__u64				ip;
// /*AFLA*/ 	__u64				flags;
// /*AFLA*/ 	__u16				cs;
// /*AFLA*/ 	__u16				gs;
// /*AFLA*/ 	__u16				fs;
// /*AFLA*/ 	__u16				ss;
// /*AFLA*/ 	__u64				err;
// /*AFLA*/ 	__u64				trapno;
// /*AFLA*/ 	__u64				oldmask;
// /*AFLA*/ 	__u64				cr2;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * fpstate is really (struct _fpstate *) or (struct _xstate *)
// /*AFLA*/ 	 * depending on the FP_XSTATE_MAGIC1 encoded in the SW reserved
// /*AFLA*/ 	 * bytes of (struct _fpstate) and FP_XSTATE_MAGIC2 present at the end
// /*AFLA*/ 	 * of extended memory layout. See comments at the definition of
// /*AFLA*/ 	 * (struct _fpx_sw_bytes)
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u64				fpstate; /* Zero when no FPU/extended context */
// /*AFLA*/ 	__u64				reserved1[8];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Create the real 'struct sigcontext' type:
// /*AFLA*/  */
// /*AFLA*/ #ifdef __KERNEL__
// /*AFLA*/ # ifdef __i386__
// /*AFLA*/ #  define sigcontext sigcontext_32
// /*AFLA*/ # else
// /*AFLA*/ #  define sigcontext sigcontext_64
// /*AFLA*/ # endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The old user-space sigcontext definition, just in case user-space still
// /*AFLA*/  * relies on it. The kernel definition (in asm/sigcontext.h) has unified
// /*AFLA*/  * field names but otherwise the same layout.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __KERNEL__
// /*AFLA*/ 
// /*AFLA*/ #define _fpstate_ia32			_fpstate_32
// /*AFLA*/ #define sigcontext_ia32			sigcontext_32
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ # ifdef __i386__
// /*AFLA*/ struct sigcontext {
// /*AFLA*/ 	__u16				gs, __gsh;
// /*AFLA*/ 	__u16				fs, __fsh;
// /*AFLA*/ 	__u16				es, __esh;
// /*AFLA*/ 	__u16				ds, __dsh;
// /*AFLA*/ 	__u32				edi;
// /*AFLA*/ 	__u32				esi;
// /*AFLA*/ 	__u32				ebp;
// /*AFLA*/ 	__u32				esp;
// /*AFLA*/ 	__u32				ebx;
// /*AFLA*/ 	__u32				edx;
// /*AFLA*/ 	__u32				ecx;
// /*AFLA*/ 	__u32				eax;
// /*AFLA*/ 	__u32				trapno;
// /*AFLA*/ 	__u32				err;
// /*AFLA*/ 	__u32				eip;
// /*AFLA*/ 	__u16				cs, __csh;
// /*AFLA*/ 	__u32				eflags;
// /*AFLA*/ 	__u32				esp_at_signal;
// /*AFLA*/ 	__u16				ss, __ssh;
// /*AFLA*/ 	struct _fpstate __user		*fpstate;
// /*AFLA*/ 	__u32				oldmask;
// /*AFLA*/ 	__u32				cr2;
// /*AFLA*/ };
// /*AFLA*/ # else /* __x86_64__: */
// /*AFLA*/ struct sigcontext {
// /*AFLA*/ 	__u64				r8;
// /*AFLA*/ 	__u64				r9;
// /*AFLA*/ 	__u64				r10;
// /*AFLA*/ 	__u64				r11;
// /*AFLA*/ 	__u64				r12;
// /*AFLA*/ 	__u64				r13;
// /*AFLA*/ 	__u64				r14;
// /*AFLA*/ 	__u64				r15;
// /*AFLA*/ 	__u64				rdi;
// /*AFLA*/ 	__u64				rsi;
// /*AFLA*/ 	__u64				rbp;
// /*AFLA*/ 	__u64				rbx;
// /*AFLA*/ 	__u64				rdx;
// /*AFLA*/ 	__u64				rax;
// /*AFLA*/ 	__u64				rcx;
// /*AFLA*/ 	__u64				rsp;
// /*AFLA*/ 	__u64				rip;
// /*AFLA*/ 	__u64				eflags;		/* RFLAGS */
// /*AFLA*/ 	__u16				cs;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Prior to 2.5.64 ("[PATCH] x86-64 updates for 2.5.64-bk3"),
// /*AFLA*/ 	 * Linux saved and restored fs and gs in these slots.  This
// /*AFLA*/ 	 * was counterproductive, as fsbase and gsbase were never
// /*AFLA*/ 	 * saved, so arch_prctl was presumably unreliable.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * These slots should never be reused without extreme caution:
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *  - Some DOSEMU versions stash fs and gs in these slots manually,
// /*AFLA*/ 	 *    thus overwriting anything the kernel expects to be preserved
// /*AFLA*/ 	 *    in these slots.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *  - If these slots are ever needed for any other purpose,
// /*AFLA*/ 	 *    there is some risk that very old 64-bit binaries could get
// /*AFLA*/ 	 *    confused.  I doubt that many such binaries still work,
// /*AFLA*/ 	 *    though, since the same patch in 2.5.64 also removed the
// /*AFLA*/ 	 *    64-bit set_thread_area syscall, so it appears that there
// /*AFLA*/ 	 *    is no TLS API beyond modify_ldt that works in both pre-
// /*AFLA*/ 	 *    and post-2.5.64 kernels.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * If the kernel ever adds explicit fs, gs, fsbase, and gsbase
// /*AFLA*/ 	 * save/restore, it will most likely need to be opt-in and use
// /*AFLA*/ 	 * different context slots.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u16				gs;
// /*AFLA*/ 	__u16				fs;
// /*AFLA*/ 	union {
// /*AFLA*/ 		__u16			ss;	/* If UC_SIGCONTEXT_SS */
// /*AFLA*/ 		__u16			__pad0;	/* Alias name for old (!UC_SIGCONTEXT_SS) user-space */
// /*AFLA*/ 	};
// /*AFLA*/ 	__u64				err;
// /*AFLA*/ 	__u64				trapno;
// /*AFLA*/ 	__u64				oldmask;
// /*AFLA*/ 	__u64				cr2;
// /*AFLA*/ 	struct _fpstate __user		*fpstate;	/* Zero when no FPU context */
// /*AFLA*/ #  ifdef __ILP32__
// /*AFLA*/ 	__u32				__fpstate_pad;
// /*AFLA*/ #  endif
// /*AFLA*/ 	__u64				reserved1[8];
// /*AFLA*/ };
// /*AFLA*/ # endif /* __x86_64__ */
// /*AFLA*/ #endif /* !__KERNEL__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_ASM_X86_SIGCONTEXT_H */
