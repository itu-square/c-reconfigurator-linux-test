// /*AFLA*/ /*
// /*AFLA*/  * FPU data structures:
// /*AFLA*/  */
// /*AFLA*/ #ifndef _ASM_X86_FPU_H
// /*AFLA*/ #define _ASM_X86_FPU_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The legacy x87 FPU state format, as saved by FSAVE and
// /*AFLA*/  * restored by the FRSTOR instructions:
// /*AFLA*/  */
// /*AFLA*/ struct fregs_state {
// /*AFLA*/ 	u32			cwd;	/* FPU Control Word		*/
// /*AFLA*/ 	u32			swd;	/* FPU Status Word		*/
// /*AFLA*/ 	u32			twd;	/* FPU Tag Word			*/
// /*AFLA*/ 	u32			fip;	/* FPU IP Offset		*/
// /*AFLA*/ 	u32			fcs;	/* FPU IP Selector		*/
// /*AFLA*/ 	u32			foo;	/* FPU Operand Pointer Offset	*/
// /*AFLA*/ 	u32			fos;	/* FPU Operand Pointer Selector	*/
// /*AFLA*/ 
// /*AFLA*/ 	/* 8*10 bytes for each FP-reg = 80 bytes:			*/
// /*AFLA*/ 	u32			st_space[20];
// /*AFLA*/ 
// /*AFLA*/ 	/* Software status information [not touched by FSAVE]:		*/
// /*AFLA*/ 	u32			status;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The legacy fx SSE/MMX FPU state format, as saved by FXSAVE and
// /*AFLA*/  * restored by the FXRSTOR instructions. It's similar to the FSAVE
// /*AFLA*/  * format, but differs in some areas, plus has extensions at
// /*AFLA*/  * the end for the XMM registers.
// /*AFLA*/  */
// /*AFLA*/ struct fxregs_state {
// /*AFLA*/ 	u16			cwd; /* Control Word			*/
// /*AFLA*/ 	u16			swd; /* Status Word			*/
// /*AFLA*/ 	u16			twd; /* Tag Word			*/
// /*AFLA*/ 	u16			fop; /* Last Instruction Opcode		*/
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct {
// /*AFLA*/ 			u64	rip; /* Instruction Pointer		*/
// /*AFLA*/ 			u64	rdp; /* Data Pointer			*/
// /*AFLA*/ 		};
// /*AFLA*/ 		struct {
// /*AFLA*/ 			u32	fip; /* FPU IP Offset			*/
// /*AFLA*/ 			u32	fcs; /* FPU IP Selector			*/
// /*AFLA*/ 			u32	foo; /* FPU Operand Offset		*/
// /*AFLA*/ 			u32	fos; /* FPU Operand Selector		*/
// /*AFLA*/ 		};
// /*AFLA*/ 	};
// /*AFLA*/ 	u32			mxcsr;		/* MXCSR Register State */
// /*AFLA*/ 	u32			mxcsr_mask;	/* MXCSR Mask		*/
// /*AFLA*/ 
// /*AFLA*/ 	/* 8*16 bytes for each FP-reg = 128 bytes:			*/
// /*AFLA*/ 	u32			st_space[32];
// /*AFLA*/ 
// /*AFLA*/ 	/* 16*16 bytes for each XMM-reg = 256 bytes:			*/
// /*AFLA*/ 	u32			xmm_space[64];
// /*AFLA*/ 
// /*AFLA*/ 	u32			padding[12];
// /*AFLA*/ 
// /*AFLA*/ 	union {
// /*AFLA*/ 		u32		padding1[12];
// /*AFLA*/ 		u32		sw_reserved[12];
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ } __attribute__((aligned(16)));
// /*AFLA*/ 
// /*AFLA*/ /* Default value for fxregs_state.mxcsr: */
// /*AFLA*/ #define MXCSR_DEFAULT		0x1f80
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Software based FPU emulation state. This is arbitrary really,
// /*AFLA*/  * it matches the x87 format to make it easier to understand:
// /*AFLA*/  */
// /*AFLA*/ struct swregs_state {
// /*AFLA*/ 	u32			cwd;
// /*AFLA*/ 	u32			swd;
// /*AFLA*/ 	u32			twd;
// /*AFLA*/ 	u32			fip;
// /*AFLA*/ 	u32			fcs;
// /*AFLA*/ 	u32			foo;
// /*AFLA*/ 	u32			fos;
// /*AFLA*/ 	/* 8*10 bytes for each FP-reg = 80 bytes: */
// /*AFLA*/ 	u32			st_space[20];
// /*AFLA*/ 	u8			ftop;
// /*AFLA*/ 	u8			changed;
// /*AFLA*/ 	u8			lookahead;
// /*AFLA*/ 	u8			no_update;
// /*AFLA*/ 	u8			rm;
// /*AFLA*/ 	u8			alimit;
// /*AFLA*/ 	struct math_emu_info	*info;
// /*AFLA*/ 	u32			entry_eip;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * List of XSAVE features Linux knows about:
// /*AFLA*/  */
// /*AFLA*/ enum xfeature {
// /*AFLA*/ 	XFEATURE_FP,
// /*AFLA*/ 	XFEATURE_SSE,
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Values above here are "legacy states".
// /*AFLA*/ 	 * Those below are "extended states".
// /*AFLA*/ 	 */
// /*AFLA*/ 	XFEATURE_YMM,
// /*AFLA*/ 	XFEATURE_BNDREGS,
// /*AFLA*/ 	XFEATURE_BNDCSR,
// /*AFLA*/ 	XFEATURE_OPMASK,
// /*AFLA*/ 	XFEATURE_ZMM_Hi256,
// /*AFLA*/ 	XFEATURE_Hi16_ZMM,
// /*AFLA*/ 	XFEATURE_PT_UNIMPLEMENTED_SO_FAR,
// /*AFLA*/ 	XFEATURE_PKRU,
// /*AFLA*/ 
// /*AFLA*/ 	XFEATURE_MAX,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define XFEATURE_MASK_FP		(1 << XFEATURE_FP)
// /*AFLA*/ #define XFEATURE_MASK_SSE		(1 << XFEATURE_SSE)
// /*AFLA*/ #define XFEATURE_MASK_YMM		(1 << XFEATURE_YMM)
// /*AFLA*/ #define XFEATURE_MASK_BNDREGS		(1 << XFEATURE_BNDREGS)
// /*AFLA*/ #define XFEATURE_MASK_BNDCSR		(1 << XFEATURE_BNDCSR)
// /*AFLA*/ #define XFEATURE_MASK_OPMASK		(1 << XFEATURE_OPMASK)
// /*AFLA*/ #define XFEATURE_MASK_ZMM_Hi256		(1 << XFEATURE_ZMM_Hi256)
// /*AFLA*/ #define XFEATURE_MASK_Hi16_ZMM		(1 << XFEATURE_Hi16_ZMM)
// /*AFLA*/ #define XFEATURE_MASK_PT		(1 << XFEATURE_PT_UNIMPLEMENTED_SO_FAR)
// /*AFLA*/ #define XFEATURE_MASK_PKRU		(1 << XFEATURE_PKRU)
// /*AFLA*/ 
// /*AFLA*/ #define XFEATURE_MASK_FPSSE		(XFEATURE_MASK_FP | XFEATURE_MASK_SSE)
// /*AFLA*/ #define XFEATURE_MASK_AVX512		(XFEATURE_MASK_OPMASK \
// /*AFLA*/ 					 | XFEATURE_MASK_ZMM_Hi256 \
// /*AFLA*/ 					 | XFEATURE_MASK_Hi16_ZMM)
// /*AFLA*/ 
// /*AFLA*/ #define FIRST_EXTENDED_XFEATURE	XFEATURE_YMM
// /*AFLA*/ 
// /*AFLA*/ struct reg_128_bit {
// /*AFLA*/ 	u8      regbytes[128/8];
// /*AFLA*/ };
// /*AFLA*/ struct reg_256_bit {
// /*AFLA*/ 	u8	regbytes[256/8];
// /*AFLA*/ };
// /*AFLA*/ struct reg_512_bit {
// /*AFLA*/ 	u8	regbytes[512/8];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * State component 2:
// /*AFLA*/  *
// /*AFLA*/  * There are 16x 256-bit AVX registers named YMM0-YMM15.
// /*AFLA*/  * The low 128 bits are aliased to the 16 SSE registers (XMM0-XMM15)
// /*AFLA*/  * and are stored in 'struct fxregs_state::xmm_space[]' in the
// /*AFLA*/  * "legacy" area.
// /*AFLA*/  *
// /*AFLA*/  * The high 128 bits are stored here.
// /*AFLA*/  */
// /*AFLA*/ struct ymmh_struct {
// /*AFLA*/ 	struct reg_128_bit              hi_ymm[16];
// /*AFLA*/ } __packed;
// /*AFLA*/ 
// /*AFLA*/ /* Intel MPX support: */
// /*AFLA*/ 
// /*AFLA*/ struct mpx_bndreg {
// /*AFLA*/ 	u64				lower_bound;
// /*AFLA*/ 	u64				upper_bound;
// /*AFLA*/ } __packed;
// /*AFLA*/ /*
// /*AFLA*/  * State component 3 is used for the 4 128-bit bounds registers
// /*AFLA*/  */
// /*AFLA*/ struct mpx_bndreg_state {
// /*AFLA*/ 	struct mpx_bndreg		bndreg[4];
// /*AFLA*/ } __packed;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * State component 4 is used for the 64-bit user-mode MPX
// /*AFLA*/  * configuration register BNDCFGU and the 64-bit MPX status
// /*AFLA*/  * register BNDSTATUS.  We call the pair "BNDCSR".
// /*AFLA*/  */
// /*AFLA*/ struct mpx_bndcsr {
// /*AFLA*/ 	u64				bndcfgu;
// /*AFLA*/ 	u64				bndstatus;
// /*AFLA*/ } __packed;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The BNDCSR state is padded out to be 64-bytes in size.
// /*AFLA*/  */
// /*AFLA*/ struct mpx_bndcsr_state {
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct mpx_bndcsr		bndcsr;
// /*AFLA*/ 		u8				pad_to_64_bytes[64];
// /*AFLA*/ 	};
// /*AFLA*/ } __packed;
// /*AFLA*/ 
// /*AFLA*/ /* AVX-512 Components: */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * State component 5 is used for the 8 64-bit opmask registers
// /*AFLA*/  * k0-k7 (opmask state).
// /*AFLA*/  */
// /*AFLA*/ struct avx_512_opmask_state {
// /*AFLA*/ 	u64				opmask_reg[8];
// /*AFLA*/ } __packed;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * State component 6 is used for the upper 256 bits of the
// /*AFLA*/  * registers ZMM0-ZMM15. These 16 256-bit values are denoted
// /*AFLA*/  * ZMM0_H-ZMM15_H (ZMM_Hi256 state).
// /*AFLA*/  */
// /*AFLA*/ struct avx_512_zmm_uppers_state {
// /*AFLA*/ 	struct reg_256_bit		zmm_upper[16];
// /*AFLA*/ } __packed;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * State component 7 is used for the 16 512-bit registers
// /*AFLA*/  * ZMM16-ZMM31 (Hi16_ZMM state).
// /*AFLA*/  */
// /*AFLA*/ struct avx_512_hi16_state {
// /*AFLA*/ 	struct reg_512_bit		hi16_zmm[16];
// /*AFLA*/ } __packed;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * State component 9: 32-bit PKRU register.  The state is
// /*AFLA*/  * 8 bytes long but only 4 bytes is used currently.
// /*AFLA*/  */
// /*AFLA*/ struct pkru_state {
// /*AFLA*/ 	u32				pkru;
// /*AFLA*/ 	u32				pad;
// /*AFLA*/ } __packed;
// /*AFLA*/ 
// /*AFLA*/ struct xstate_header {
// /*AFLA*/ 	u64				xfeatures;
// /*AFLA*/ 	u64				xcomp_bv;
// /*AFLA*/ 	u64				reserved[6];
// /*AFLA*/ } __attribute__((packed));
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * xstate_header.xcomp_bv[63] indicates that the extended_state_area
// /*AFLA*/  * is in compacted format.
// /*AFLA*/  */
// /*AFLA*/ #define XCOMP_BV_COMPACTED_FORMAT ((u64)1 << 63)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is our most modern FPU state format, as saved by the XSAVE
// /*AFLA*/  * and restored by the XRSTOR instructions.
// /*AFLA*/  *
// /*AFLA*/  * It consists of a legacy fxregs portion, an xstate header and
// /*AFLA*/  * subsequent areas as defined by the xstate header.  Not all CPUs
// /*AFLA*/  * support all the extensions, so the size of the extended area
// /*AFLA*/  * can vary quite a bit between CPUs.
// /*AFLA*/  */
// /*AFLA*/ struct xregs_state {
// /*AFLA*/ 	struct fxregs_state		i387;
// /*AFLA*/ 	struct xstate_header		header;
// /*AFLA*/ 	u8				extended_state_area[0];
// /*AFLA*/ } __attribute__ ((packed, aligned (64)));
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is a union of all the possible FPU state formats
// /*AFLA*/  * put together, so that we can pick the right one runtime.
// /*AFLA*/  *
// /*AFLA*/  * The size of the structure is determined by the largest
// /*AFLA*/  * member - which is the xsave area.  The padding is there
// /*AFLA*/  * to ensure that statically-allocated task_structs (just
// /*AFLA*/  * the init_task today) have enough space.
// /*AFLA*/  */
// /*AFLA*/ union fpregs_state {
// /*AFLA*/ 	struct fregs_state		fsave;
// /*AFLA*/ 	struct fxregs_state		fxsave;
// /*AFLA*/ 	struct swregs_state		soft;
// /*AFLA*/ 	struct xregs_state		xsave;
// /*AFLA*/ 	u8 __padding[PAGE_SIZE];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Highest level per task FPU state data structure that
// /*AFLA*/  * contains the FPU register state plus various FPU
// /*AFLA*/  * state fields:
// /*AFLA*/  */
// /*AFLA*/ struct fpu {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * @last_cpu:
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Records the last CPU on which this context was loaded into
// /*AFLA*/ 	 * FPU registers. (In the lazy-restore case we might be
// /*AFLA*/ 	 * able to reuse FPU registers across multiple context switches
// /*AFLA*/ 	 * this way, if no intermediate task used the FPU.)
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * A value of -1 is used to indicate that the FPU state in context
// /*AFLA*/ 	 * memory is newer than the FPU state in registers, and that the
// /*AFLA*/ 	 * FPU state should be reloaded next time the task is run.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned int			last_cpu;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * @fpstate_active:
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * This flag indicates whether this context is active: if the task
// /*AFLA*/ 	 * is not running then we can restore from this context, if the task
// /*AFLA*/ 	 * is running then we should save into this context.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned char			fpstate_active;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * @fpregs_active:
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * This flag determines whether a given context is actively
// /*AFLA*/ 	 * loaded into the FPU's registers and that those registers
// /*AFLA*/ 	 * represent the task's current FPU state.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Note the interaction with fpstate_active:
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *   # task does not use the FPU:
// /*AFLA*/ 	 *   fpstate_active == 0
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *   # task uses the FPU and regs are active:
// /*AFLA*/ 	 *   fpstate_active == 1 && fpregs_active == 1
// /*AFLA*/ 	 *
// /*AFLA*/ 	 *   # the regs are inactive but still match fpstate:
// /*AFLA*/ 	 *   fpstate_active == 1 && fpregs_active == 0 && fpregs_owner == fpu
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * The third state is what we use for the lazy restore optimization
// /*AFLA*/ 	 * on lazy-switching CPUs.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned char			fpregs_active;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * @counter:
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * This counter contains the number of consecutive context switches
// /*AFLA*/ 	 * during which the FPU stays used. If this is over a threshold, the
// /*AFLA*/ 	 * lazy FPU restore logic becomes eager, to save the trap overhead.
// /*AFLA*/ 	 * This is an unsigned char so that after 256 iterations the counter
// /*AFLA*/ 	 * wraps and the context switch behavior turns lazy again; this is to
// /*AFLA*/ 	 * deal with bursty apps that only use the FPU for a short time:
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned char			counter;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * @state:
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * In-memory copy of all FPU registers that we save/restore
// /*AFLA*/ 	 * over context switches. If the task is using the FPU then
// /*AFLA*/ 	 * the registers in the FPU are more recent than this state
// /*AFLA*/ 	 * copy. If the task context-switches away then they get
// /*AFLA*/ 	 * saved here and represent the FPU state.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * After context switches there may be a (short) time period
// /*AFLA*/ 	 * during which the in-FPU hardware registers are unchanged
// /*AFLA*/ 	 * and still perfectly match this state, if the tasks
// /*AFLA*/ 	 * scheduled afterwards are not using the FPU.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * This is the 'lazy restore' window of optimization, which
// /*AFLA*/ 	 * we track though 'fpu_fpregs_owner_ctx' and 'fpu->last_cpu'.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * We detect whether a subsequent task uses the FPU via setting
// /*AFLA*/ 	 * CR0::TS to 1, which causes any FPU use to raise a #NM fault.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * During this window, if the task gets scheduled again, we
// /*AFLA*/ 	 * might be able to skip having to do a restore from this
// /*AFLA*/ 	 * memory buffer to the hardware registers - at the cost of
// /*AFLA*/ 	 * incurring the overhead of #NM fault traps.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Note that on modern CPUs that support the XSAVEOPT (or other
// /*AFLA*/ 	 * optimized XSAVE instructions), we don't use #NM traps anymore,
// /*AFLA*/ 	 * as the hardware can track whether FPU registers need saving
// /*AFLA*/ 	 * or not. On such CPUs we activate the non-lazy ('eagerfpu')
// /*AFLA*/ 	 * logic, which unconditionally saves/restores all FPU state
// /*AFLA*/ 	 * across context switches. (if FPU state exists.)
// /*AFLA*/ 	 */
// /*AFLA*/ 	union fpregs_state		state;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * WARNING: 'state' is dynamically-sized.  Do not put
// /*AFLA*/ 	 * anything after it here.
// /*AFLA*/ 	 */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_FPU_H */
