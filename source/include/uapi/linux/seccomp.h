// /*AFLA*/ #ifndef _UAPI_LINUX_SECCOMP_H
// /*AFLA*/ #define _UAPI_LINUX_SECCOMP_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* Valid values for seccomp.mode and prctl(PR_SET_SECCOMP, <mode>) */
// /*AFLA*/ #define SECCOMP_MODE_DISABLED	0 /* seccomp is not in use. */
// /*AFLA*/ #define SECCOMP_MODE_STRICT	1 /* uses hard-coded filter. */
// /*AFLA*/ #define SECCOMP_MODE_FILTER	2 /* uses user-supplied filter. */
// /*AFLA*/ 
// /*AFLA*/ /* Valid operations for seccomp syscall. */
// /*AFLA*/ #define SECCOMP_SET_MODE_STRICT	0
// /*AFLA*/ #define SECCOMP_SET_MODE_FILTER	1
// /*AFLA*/ 
// /*AFLA*/ /* Valid flags for SECCOMP_SET_MODE_FILTER */
// /*AFLA*/ #define SECCOMP_FILTER_FLAG_TSYNC	1
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * All BPF programs must return a 32-bit value.
// /*AFLA*/  * The bottom 16-bits are for optional return data.
// /*AFLA*/  * The upper 16-bits are ordered from least permissive values to most.
// /*AFLA*/  *
// /*AFLA*/  * The ordering ensures that a min_t() over composed return values always
// /*AFLA*/  * selects the least permissive choice.
// /*AFLA*/  */
// /*AFLA*/ #define SECCOMP_RET_KILL	0x00000000U /* kill the task immediately */
// /*AFLA*/ #define SECCOMP_RET_TRAP	0x00030000U /* disallow and force a SIGSYS */
// /*AFLA*/ #define SECCOMP_RET_ERRNO	0x00050000U /* returns an errno */
// /*AFLA*/ #define SECCOMP_RET_TRACE	0x7ff00000U /* pass to a tracer or disallow */
// /*AFLA*/ #define SECCOMP_RET_ALLOW	0x7fff0000U /* allow */
// /*AFLA*/ 
// /*AFLA*/ /* Masks for the return value sections. */
// /*AFLA*/ #define SECCOMP_RET_ACTION	0x7fff0000U
// /*AFLA*/ #define SECCOMP_RET_DATA	0x0000ffffU
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct seccomp_data - the format the BPF program executes over.
// /*AFLA*/  * @nr: the system call number
// /*AFLA*/  * @arch: indicates system call convention as an AUDIT_ARCH_* value
// /*AFLA*/  *        as defined in <linux/audit.h>.
// /*AFLA*/  * @instruction_pointer: at the time of the system call.
// /*AFLA*/  * @args: up to 6 system call arguments always stored as 64-bit values
// /*AFLA*/  *        regardless of the architecture.
// /*AFLA*/  */
// /*AFLA*/ struct seccomp_data {
// /*AFLA*/ 	int nr;
// /*AFLA*/ 	__u32 arch;
// /*AFLA*/ 	__u64 instruction_pointer;
// /*AFLA*/ 	__u64 args[6];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_SECCOMP_H */
