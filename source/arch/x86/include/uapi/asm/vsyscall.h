// /*AFLA*/ #ifndef _UAPI_ASM_X86_VSYSCALL_H
// /*AFLA*/ #define _UAPI_ASM_X86_VSYSCALL_H
// /*AFLA*/ 
// /*AFLA*/ enum vsyscall_num {
// /*AFLA*/ 	__NR_vgettimeofday,
// /*AFLA*/ 	__NR_vtime,
// /*AFLA*/ 	__NR_vgetcpu,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define VSYSCALL_ADDR (-10UL << 20)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_ASM_X86_VSYSCALL_H */
