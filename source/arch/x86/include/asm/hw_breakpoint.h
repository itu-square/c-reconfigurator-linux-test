// /*AFLA*/ #ifndef	_I386_HW_BREAKPOINT_H
// /*AFLA*/ #define	_I386_HW_BREAKPOINT_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/asm/hw_breakpoint.h>
// /*AFLA*/ 
// /*AFLA*/ #define	__ARCH_HW_BREAKPOINT_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The name should probably be something dealt in
// /*AFLA*/  * a higher level. While dealing with the user
// /*AFLA*/  * (display/resolving)
// /*AFLA*/  */
// /*AFLA*/ struct arch_hw_breakpoint {
// /*AFLA*/ 	unsigned long	address;
// /*AFLA*/ 	unsigned long	mask;
// /*AFLA*/ 	u8		len;
// /*AFLA*/ 	u8		type;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #include <linux/kdebug.h>
// /*AFLA*/ #include <linux/percpu.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ 
// /*AFLA*/ /* Available HW breakpoint length encodings */
// /*AFLA*/ #define X86_BREAKPOINT_LEN_X		0x40
// /*AFLA*/ #define X86_BREAKPOINT_LEN_1		0x40
// /*AFLA*/ #define X86_BREAKPOINT_LEN_2		0x44
// /*AFLA*/ #define X86_BREAKPOINT_LEN_4		0x4c
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ #define X86_BREAKPOINT_LEN_8		0x48
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Available HW breakpoint type encodings */
// /*AFLA*/ 
// /*AFLA*/ /* trigger on instruction execute */
// /*AFLA*/ #define X86_BREAKPOINT_EXECUTE	0x80
// /*AFLA*/ /* trigger on memory write */
// /*AFLA*/ #define X86_BREAKPOINT_WRITE	0x81
// /*AFLA*/ /* trigger on memory read or write */
// /*AFLA*/ #define X86_BREAKPOINT_RW	0x83
// /*AFLA*/ 
// /*AFLA*/ /* Total number of available HW breakpoint registers */
// /*AFLA*/ #define HBP_NUM 4
// /*AFLA*/ 
// /*AFLA*/ static inline int hw_breakpoint_slots(int type)
// /*AFLA*/ {
// /*AFLA*/ 	return HBP_NUM;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct perf_event;
// /*AFLA*/ struct pmu;
// /*AFLA*/ 
// /*AFLA*/ extern int arch_check_bp_in_kernelspace(struct perf_event *bp);
// /*AFLA*/ extern int arch_validate_hwbkpt_settings(struct perf_event *bp);
// /*AFLA*/ extern int hw_breakpoint_exceptions_notify(struct notifier_block *unused,
// /*AFLA*/ 					   unsigned long val, void *data);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ int arch_install_hw_breakpoint(struct perf_event *bp);
// /*AFLA*/ void arch_uninstall_hw_breakpoint(struct perf_event *bp);
// /*AFLA*/ void hw_breakpoint_pmu_read(struct perf_event *bp);
// /*AFLA*/ void hw_breakpoint_pmu_unthrottle(struct perf_event *bp);
// /*AFLA*/ 
// /*AFLA*/ extern void
// /*AFLA*/ arch_fill_perf_breakpoint(struct perf_event *bp);
// /*AFLA*/ 
// /*AFLA*/ unsigned long encode_dr7(int drnum, unsigned int len, unsigned int type);
// /*AFLA*/ int decode_dr7(unsigned long dr7, int bpnum, unsigned *len, unsigned *type);
// /*AFLA*/ 
// /*AFLA*/ extern int arch_bp_generic_fields(int x86_len, int x86_type,
// /*AFLA*/ 				  int *gen_len, int *gen_type);
// /*AFLA*/ 
// /*AFLA*/ extern struct pmu perf_ops_bp;
// /*AFLA*/ 
// /*AFLA*/ #endif	/* _I386_HW_BREAKPOINT_H */
