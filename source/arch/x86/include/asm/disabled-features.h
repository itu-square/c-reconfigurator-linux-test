// /*AFLA*/ #ifndef _ASM_X86_DISABLED_FEATURES_H
// /*AFLA*/ #define _ASM_X86_DISABLED_FEATURES_H
// /*AFLA*/ 
// /*AFLA*/ /* These features, although they might be available in a CPU
// /*AFLA*/  * will not be used because the compile options to support
// /*AFLA*/  * them are not present.
// /*AFLA*/  *
// /*AFLA*/  * This code allows them to be checked and disabled at
// /*AFLA*/  * compile time without an explicit #ifdef.  Use
// /*AFLA*/  * cpu_feature_enabled().
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_INTEL_MPX
// /*AFLA*/ # define DISABLE_MPX	0
// /*AFLA*/ #else
// /*AFLA*/ # define DISABLE_MPX	(1<<(X86_FEATURE_MPX & 31))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ # define DISABLE_VME		(1<<(X86_FEATURE_VME & 31))
// /*AFLA*/ # define DISABLE_K6_MTRR	(1<<(X86_FEATURE_K6_MTRR & 31))
// /*AFLA*/ # define DISABLE_CYRIX_ARR	(1<<(X86_FEATURE_CYRIX_ARR & 31))
// /*AFLA*/ # define DISABLE_CENTAUR_MCR	(1<<(X86_FEATURE_CENTAUR_MCR & 31))
// /*AFLA*/ #else
// /*AFLA*/ # define DISABLE_VME		0
// /*AFLA*/ # define DISABLE_K6_MTRR	0
// /*AFLA*/ # define DISABLE_CYRIX_ARR	0
// /*AFLA*/ # define DISABLE_CENTAUR_MCR	0
// /*AFLA*/ #endif /* CONFIG_X86_64 */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS
// /*AFLA*/ # define DISABLE_PKU		0
// /*AFLA*/ # define DISABLE_OSPKE		0
// /*AFLA*/ #else
// /*AFLA*/ # define DISABLE_PKU		(1<<(X86_FEATURE_PKU & 31))
// /*AFLA*/ # define DISABLE_OSPKE		(1<<(X86_FEATURE_OSPKE & 31))
// /*AFLA*/ #endif /* CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Make sure to add features to the correct mask
// /*AFLA*/  */
// /*AFLA*/ #define DISABLED_MASK0	(DISABLE_VME)
// /*AFLA*/ #define DISABLED_MASK1	0
// /*AFLA*/ #define DISABLED_MASK2	0
// /*AFLA*/ #define DISABLED_MASK3	(DISABLE_CYRIX_ARR|DISABLE_CENTAUR_MCR|DISABLE_K6_MTRR)
// /*AFLA*/ #define DISABLED_MASK4	0
// /*AFLA*/ #define DISABLED_MASK5	0
// /*AFLA*/ #define DISABLED_MASK6	0
// /*AFLA*/ #define DISABLED_MASK7	0
// /*AFLA*/ #define DISABLED_MASK8	0
// /*AFLA*/ #define DISABLED_MASK9	(DISABLE_MPX)
// /*AFLA*/ #define DISABLED_MASK10	0
// /*AFLA*/ #define DISABLED_MASK11	0
// /*AFLA*/ #define DISABLED_MASK12	0
// /*AFLA*/ #define DISABLED_MASK13	0
// /*AFLA*/ #define DISABLED_MASK14	0
// /*AFLA*/ #define DISABLED_MASK15	0
// /*AFLA*/ #define DISABLED_MASK16	(DISABLE_PKU|DISABLE_OSPKE)
// /*AFLA*/ #define DISABLED_MASK17	0
// /*AFLA*/ #define DISABLED_MASK_CHECK BUILD_BUG_ON_ZERO(NCAPINTS != 18)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_DISABLED_FEATURES_H */
