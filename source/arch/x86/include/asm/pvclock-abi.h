// /*AFLA*/ #ifndef _ASM_X86_PVCLOCK_ABI_H
// /*AFLA*/ #define _ASM_X86_PVCLOCK_ABI_H
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These structs MUST NOT be changed.
// /*AFLA*/  * They are the ABI between hypervisor and guest OS.
// /*AFLA*/  * Both Xen and KVM are using this.
// /*AFLA*/  *
// /*AFLA*/  * pvclock_vcpu_time_info holds the system time and the tsc timestamp
// /*AFLA*/  * of the last update. So the guest can use the tsc delta to get a
// /*AFLA*/  * more precise system time.  There is one per virtual cpu.
// /*AFLA*/  *
// /*AFLA*/  * pvclock_wall_clock references the point in time when the system
// /*AFLA*/  * time was zero (usually boot time), thus the guest calculates the
// /*AFLA*/  * current wall clock by adding the system time.
// /*AFLA*/  *
// /*AFLA*/  * Protocol for the "version" fields is: hypervisor raises it (making
// /*AFLA*/  * it uneven) before it starts updating the fields and raises it again
// /*AFLA*/  * (making it even) when it is done.  Thus the guest can make sure the
// /*AFLA*/  * time values it got are consistent by checking the version before
// /*AFLA*/  * and after reading them.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct pvclock_vcpu_time_info {
// /*AFLA*/ 	u32   version;
// /*AFLA*/ 	u32   pad0;
// /*AFLA*/ 	u64   tsc_timestamp;
// /*AFLA*/ 	u64   system_time;
// /*AFLA*/ 	u32   tsc_to_system_mul;
// /*AFLA*/ 	s8    tsc_shift;
// /*AFLA*/ 	u8    flags;
// /*AFLA*/ 	u8    pad[2];
// /*AFLA*/ } __attribute__((__packed__)); /* 32 bytes */
// /*AFLA*/ 
// /*AFLA*/ struct pvclock_wall_clock {
// /*AFLA*/ 	u32   version;
// /*AFLA*/ 	u32   sec;
// /*AFLA*/ 	u32   nsec;
// /*AFLA*/ } __attribute__((__packed__));
// /*AFLA*/ 
// /*AFLA*/ #define PVCLOCK_TSC_STABLE_BIT	(1 << 0)
// /*AFLA*/ #define PVCLOCK_GUEST_STOPPED	(1 << 1)
// /*AFLA*/ /* PVCLOCK_COUNTS_FROM_ZERO broke ABI and can't be used anymore. */
// /*AFLA*/ #define PVCLOCK_COUNTS_FROM_ZERO (1 << 2)
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ #endif /* _ASM_X86_PVCLOCK_ABI_H */
