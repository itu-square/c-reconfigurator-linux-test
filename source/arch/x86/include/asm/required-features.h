// /*AFLA*/ #ifndef _ASM_X86_REQUIRED_FEATURES_H
// /*AFLA*/ #define _ASM_X86_REQUIRED_FEATURES_H
// /*AFLA*/ 
// /*AFLA*/ /* Define minimum CPUID feature set for kernel These bits are checked
// /*AFLA*/    really early to actually display a visible error message before the
// /*AFLA*/    kernel dies.  Make sure to assign features to the proper mask!
// /*AFLA*/ 
// /*AFLA*/    Some requirements that are not in CPUID yet are also in the
// /*AFLA*/    CONFIG_X86_MINIMUM_CPU_FAMILY which is checked too.
// /*AFLA*/ 
// /*AFLA*/    The real information is in arch/x86/Kconfig.cpu, this just converts
// /*AFLA*/    the CONFIGs into a bitmask */
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_MATH_EMULATION
// /*AFLA*/ # define NEED_FPU	(1<<(X86_FEATURE_FPU & 31))
// /*AFLA*/ #else
// /*AFLA*/ # define NEED_FPU	0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_X86_PAE) || defined(CONFIG_X86_64)
// /*AFLA*/ # define NEED_PAE	(1<<(X86_FEATURE_PAE & 31))
// /*AFLA*/ #else
// /*AFLA*/ # define NEED_PAE	0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_CMPXCHG64
// /*AFLA*/ # define NEED_CX8	(1<<(X86_FEATURE_CX8 & 31))
// /*AFLA*/ #else
// /*AFLA*/ # define NEED_CX8	0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_X86_CMOV) || defined(CONFIG_X86_64)
// /*AFLA*/ # define NEED_CMOV	(1<<(X86_FEATURE_CMOV & 31))
// /*AFLA*/ #else
// /*AFLA*/ # define NEED_CMOV	0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_USE_3DNOW
// /*AFLA*/ # define NEED_3DNOW	(1<<(X86_FEATURE_3DNOW & 31))
// /*AFLA*/ #else
// /*AFLA*/ # define NEED_3DNOW	0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_X86_P6_NOP) || defined(CONFIG_X86_64)
// /*AFLA*/ # define NEED_NOPL	(1<<(X86_FEATURE_NOPL & 31))
// /*AFLA*/ #else
// /*AFLA*/ # define NEED_NOPL	0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MATOM
// /*AFLA*/ # define NEED_MOVBE	(1<<(X86_FEATURE_MOVBE & 31))
// /*AFLA*/ #else
// /*AFLA*/ # define NEED_MOVBE	0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ #ifdef CONFIG_PARAVIRT
// /*AFLA*/ /* Paravirtualized systems may not have PSE or PGE available */
// /*AFLA*/ #define NEED_PSE	0
// /*AFLA*/ #define NEED_PGE	0
// /*AFLA*/ #else
// /*AFLA*/ #define NEED_PSE	(1<<(X86_FEATURE_PSE) & 31)
// /*AFLA*/ #define NEED_PGE	(1<<(X86_FEATURE_PGE) & 31)
// /*AFLA*/ #endif
// /*AFLA*/ #define NEED_MSR	(1<<(X86_FEATURE_MSR & 31))
// /*AFLA*/ #define NEED_FXSR	(1<<(X86_FEATURE_FXSR & 31))
// /*AFLA*/ #define NEED_XMM	(1<<(X86_FEATURE_XMM & 31))
// /*AFLA*/ #define NEED_XMM2	(1<<(X86_FEATURE_XMM2 & 31))
// /*AFLA*/ #define NEED_LM		(1<<(X86_FEATURE_LM & 31))
// /*AFLA*/ #else
// /*AFLA*/ #define NEED_PSE	0
// /*AFLA*/ #define NEED_MSR	0
// /*AFLA*/ #define NEED_PGE	0
// /*AFLA*/ #define NEED_FXSR	0
// /*AFLA*/ #define NEED_XMM	0
// /*AFLA*/ #define NEED_XMM2	0
// /*AFLA*/ #define NEED_LM		0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define REQUIRED_MASK0	(NEED_FPU|NEED_PSE|NEED_MSR|NEED_PAE|\
// /*AFLA*/ 			 NEED_CX8|NEED_PGE|NEED_FXSR|NEED_CMOV|\
// /*AFLA*/ 			 NEED_XMM|NEED_XMM2)
// /*AFLA*/ #define SSE_MASK	(NEED_XMM|NEED_XMM2)
// /*AFLA*/ 
// /*AFLA*/ #define REQUIRED_MASK1	(NEED_LM|NEED_3DNOW)
// /*AFLA*/ 
// /*AFLA*/ #define REQUIRED_MASK2	0
// /*AFLA*/ #define REQUIRED_MASK3	(NEED_NOPL)
// /*AFLA*/ #define REQUIRED_MASK4	(NEED_MOVBE)
// /*AFLA*/ #define REQUIRED_MASK5	0
// /*AFLA*/ #define REQUIRED_MASK6	0
// /*AFLA*/ #define REQUIRED_MASK7	0
// /*AFLA*/ #define REQUIRED_MASK8	0
// /*AFLA*/ #define REQUIRED_MASK9	0
// /*AFLA*/ #define REQUIRED_MASK10	0
// /*AFLA*/ #define REQUIRED_MASK11	0
// /*AFLA*/ #define REQUIRED_MASK12	0
// /*AFLA*/ #define REQUIRED_MASK13	0
// /*AFLA*/ #define REQUIRED_MASK14	0
// /*AFLA*/ #define REQUIRED_MASK15	0
// /*AFLA*/ #define REQUIRED_MASK16	0
// /*AFLA*/ #define REQUIRED_MASK17	0
// /*AFLA*/ #define REQUIRED_MASK_CHECK BUILD_BUG_ON_ZERO(NCAPINTS != 18)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_REQUIRED_FEATURES_H */
