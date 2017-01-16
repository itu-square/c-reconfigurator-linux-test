// /*AFLA*/ #ifndef _ASM_X86_CPUFEATURE_H
// /*AFLA*/ #define _ASM_X86_CPUFEATURE_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/processor.h>
// /*AFLA*/ 
// /*AFLA*/ #if defined(__KERNEL__) && !defined(__ASSEMBLY__)
// /*AFLA*/ 
// /*AFLA*/ #include <asm/asm.h>
// /*AFLA*/ #include <linux/bitops.h>
// /*AFLA*/ 
// /*AFLA*/ enum cpuid_leafs
// /*AFLA*/ {
// /*AFLA*/ 	CPUID_1_EDX		= 0,
// /*AFLA*/ 	CPUID_8000_0001_EDX,
// /*AFLA*/ 	CPUID_8086_0001_EDX,
// /*AFLA*/ 	CPUID_LNX_1,
// /*AFLA*/ 	CPUID_1_ECX,
// /*AFLA*/ 	CPUID_C000_0001_EDX,
// /*AFLA*/ 	CPUID_8000_0001_ECX,
// /*AFLA*/ 	CPUID_LNX_2,
// /*AFLA*/ 	CPUID_LNX_3,
// /*AFLA*/ 	CPUID_7_0_EBX,
// /*AFLA*/ 	CPUID_D_1_EAX,
// /*AFLA*/ 	CPUID_F_0_EDX,
// /*AFLA*/ 	CPUID_F_1_EDX,
// /*AFLA*/ 	CPUID_8000_0008_EBX,
// /*AFLA*/ 	CPUID_6_EAX,
// /*AFLA*/ 	CPUID_8000_000A_EDX,
// /*AFLA*/ 	CPUID_7_ECX,
// /*AFLA*/ 	CPUID_8000_0007_EBX,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_FEATURE_NAMES
// /*AFLA*/ extern const char * const x86_cap_flags[NCAPINTS*32];
// /*AFLA*/ extern const char * const x86_power_flags[32];
// /*AFLA*/ #define X86_CAP_FMT "%s"
// /*AFLA*/ #define x86_cap_flag(flag) x86_cap_flags[flag]
// /*AFLA*/ #else
// /*AFLA*/ #define X86_CAP_FMT "%d:%d"
// /*AFLA*/ #define x86_cap_flag(flag) ((flag) >> 5), ((flag) & 31)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * In order to save room, we index into this array by doing
// /*AFLA*/  * X86_BUG_<name> - NCAPINTS*32.
// /*AFLA*/  */
// /*AFLA*/ extern const char * const x86_bug_flags[NBUGINTS*32];
// /*AFLA*/ 
// /*AFLA*/ #define test_cpu_cap(c, bit)						\
// /*AFLA*/ 	 test_bit(bit, (unsigned long *)((c)->x86_capability))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * There are 32 bits/features in each mask word.  The high bits
// /*AFLA*/  * (selected with (bit>>5) give us the word number and the low 5
// /*AFLA*/  * bits give us the bit/feature number inside the word.
// /*AFLA*/  * (1UL<<((bit)&31) gives us a mask for the feature_bit so we can
// /*AFLA*/  * see if it is set in the mask word.
// /*AFLA*/  */
// /*AFLA*/ #define CHECK_BIT_IN_MASK_WORD(maskname, word, bit)	\
// /*AFLA*/ 	(((bit)>>5)==(word) && (1UL<<((bit)&31) & maskname##word ))
// /*AFLA*/ 
// /*AFLA*/ #define REQUIRED_MASK_BIT_SET(feature_bit)		\
// /*AFLA*/ 	 ( CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK,  0, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK,  1, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK,  2, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK,  3, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK,  4, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK,  5, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK,  6, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK,  7, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK,  8, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK,  9, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK, 10, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK, 11, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK, 12, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK, 13, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK, 14, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK, 15, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK, 16, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(REQUIRED_MASK, 17, feature_bit) ||	\
// /*AFLA*/ 	   REQUIRED_MASK_CHECK					  ||	\
// /*AFLA*/ 	   BUILD_BUG_ON_ZERO(NCAPINTS != 18))
// /*AFLA*/ 
// /*AFLA*/ #define DISABLED_MASK_BIT_SET(feature_bit)				\
// /*AFLA*/ 	 ( CHECK_BIT_IN_MASK_WORD(DISABLED_MASK,  0, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK,  1, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK,  2, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK,  3, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK,  4, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK,  5, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK,  6, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK,  7, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK,  8, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK,  9, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK, 10, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK, 11, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK, 12, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK, 13, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK, 14, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK, 15, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK, 16, feature_bit) ||	\
// /*AFLA*/ 	   CHECK_BIT_IN_MASK_WORD(DISABLED_MASK, 17, feature_bit) ||	\
// /*AFLA*/ 	   DISABLED_MASK_CHECK					  ||	\
// /*AFLA*/ 	   BUILD_BUG_ON_ZERO(NCAPINTS != 18))
// /*AFLA*/ 
// /*AFLA*/ #define cpu_has(c, bit)							\
// /*AFLA*/ 	(__builtin_constant_p(bit) && REQUIRED_MASK_BIT_SET(bit) ? 1 :	\
// /*AFLA*/ 	 test_cpu_cap(c, bit))
// /*AFLA*/ 
// /*AFLA*/ #define this_cpu_has(bit)						\
// /*AFLA*/ 	(__builtin_constant_p(bit) && REQUIRED_MASK_BIT_SET(bit) ? 1 : 	\
// /*AFLA*/ 	 x86_this_cpu_test_bit(bit, (unsigned long *)&cpu_info.x86_capability))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This macro is for detection of features which need kernel
// /*AFLA*/  * infrastructure to be used.  It may *not* directly test the CPU
// /*AFLA*/  * itself.  Use the cpu_has() family if you want true runtime
// /*AFLA*/  * testing of CPU features, like in hypervisor code where you are
// /*AFLA*/  * supporting a possible guest feature where host support for it
// /*AFLA*/  * is not relevant.
// /*AFLA*/  */
// /*AFLA*/ #define cpu_feature_enabled(bit)	\
// /*AFLA*/ 	(__builtin_constant_p(bit) && DISABLED_MASK_BIT_SET(bit) ? 0 : static_cpu_has(bit))
// /*AFLA*/ 
// /*AFLA*/ #define boot_cpu_has(bit)	cpu_has(&boot_cpu_data, bit)
// /*AFLA*/ 
// /*AFLA*/ #define set_cpu_cap(c, bit)	set_bit(bit, (unsigned long *)((c)->x86_capability))
// /*AFLA*/ #define clear_cpu_cap(c, bit)	clear_bit(bit, (unsigned long *)((c)->x86_capability))
// /*AFLA*/ #define setup_clear_cpu_cap(bit) do { \
// /*AFLA*/ 	clear_cpu_cap(&boot_cpu_data, bit);	\
// /*AFLA*/ 	set_bit(bit, (unsigned long *)cpu_caps_cleared); \
// /*AFLA*/ } while (0)
// /*AFLA*/ #define setup_force_cpu_cap(bit) do { \
// /*AFLA*/ 	set_cpu_cap(&boot_cpu_data, bit);	\
// /*AFLA*/ 	set_bit(bit, (unsigned long *)cpu_caps_set);	\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #if defined(CC_HAVE_ASM_GOTO) && defined(CONFIG_X86_FAST_FEATURE_TESTS)
// /*AFLA*/ /*
// /*AFLA*/  * Static testing of CPU features.  Used the same as boot_cpu_has().
// /*AFLA*/  * These will statically patch the target code for additional
// /*AFLA*/  * performance.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline __pure bool _static_cpu_has(u16 bit)
// /*AFLA*/ {
// /*AFLA*/ 		asm_volatile_goto("1: jmp 6f\n"
// /*AFLA*/ 			 "2:\n"
// /*AFLA*/ 			 ".skip -(((5f-4f) - (2b-1b)) > 0) * "
// /*AFLA*/ 			         "((5f-4f) - (2b-1b)),0x90\n"
// /*AFLA*/ 			 "3:\n"
// /*AFLA*/ 			 ".section .altinstructions,\"a\"\n"
// /*AFLA*/ 			 " .long 1b - .\n"		/* src offset */
// /*AFLA*/ 			 " .long 4f - .\n"		/* repl offset */
// /*AFLA*/ 			 " .word %P1\n"			/* always replace */
// /*AFLA*/ 			 " .byte 3b - 1b\n"		/* src len */
// /*AFLA*/ 			 " .byte 5f - 4f\n"		/* repl len */
// /*AFLA*/ 			 " .byte 3b - 2b\n"		/* pad len */
// /*AFLA*/ 			 ".previous\n"
// /*AFLA*/ 			 ".section .altinstr_replacement,\"ax\"\n"
// /*AFLA*/ 			 "4: jmp %l[t_no]\n"
// /*AFLA*/ 			 "5:\n"
// /*AFLA*/ 			 ".previous\n"
// /*AFLA*/ 			 ".section .altinstructions,\"a\"\n"
// /*AFLA*/ 			 " .long 1b - .\n"		/* src offset */
// /*AFLA*/ 			 " .long 0\n"			/* no replacement */
// /*AFLA*/ 			 " .word %P0\n"			/* feature bit */
// /*AFLA*/ 			 " .byte 3b - 1b\n"		/* src len */
// /*AFLA*/ 			 " .byte 0\n"			/* repl len */
// /*AFLA*/ 			 " .byte 0\n"			/* pad len */
// /*AFLA*/ 			 ".previous\n"
// /*AFLA*/ 			 ".section .altinstr_aux,\"ax\"\n"
// /*AFLA*/ 			 "6:\n"
// /*AFLA*/ 			 " testb %[bitnum],%[cap_byte]\n"
// /*AFLA*/ 			 " jnz %l[t_yes]\n"
// /*AFLA*/ 			 " jmp %l[t_no]\n"
// /*AFLA*/ 			 ".previous\n"
// /*AFLA*/ 			 : : "i" (bit), "i" (X86_FEATURE_ALWAYS),
// /*AFLA*/ 			     [bitnum] "i" (1 << (bit & 7)),
// /*AFLA*/ 			     [cap_byte] "m" (((const char *)boot_cpu_data.x86_capability)[bit >> 3])
// /*AFLA*/ 			 : : t_yes, t_no);
// /*AFLA*/ 	t_yes:
// /*AFLA*/ 		return true;
// /*AFLA*/ 	t_no:
// /*AFLA*/ 		return false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define static_cpu_has(bit)					\
// /*AFLA*/ (								\
// /*AFLA*/ 	__builtin_constant_p(boot_cpu_has(bit)) ?		\
// /*AFLA*/ 		boot_cpu_has(bit) :				\
// /*AFLA*/ 		_static_cpu_has(bit)				\
// /*AFLA*/ )
// /*AFLA*/ #else
// /*AFLA*/ /*
// /*AFLA*/  * Fall back to dynamic for gcc versions which don't support asm goto. Should be
// /*AFLA*/  * a minority now anyway.
// /*AFLA*/  */
// /*AFLA*/ #define static_cpu_has(bit)		boot_cpu_has(bit)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define cpu_has_bug(c, bit)		cpu_has(c, (bit))
// /*AFLA*/ #define set_cpu_bug(c, bit)		set_cpu_cap(c, (bit))
// /*AFLA*/ #define clear_cpu_bug(c, bit)		clear_cpu_cap(c, (bit))
// /*AFLA*/ 
// /*AFLA*/ #define static_cpu_has_bug(bit)		static_cpu_has((bit))
// /*AFLA*/ #define boot_cpu_has_bug(bit)		cpu_has_bug(&boot_cpu_data, (bit))
// /*AFLA*/ 
// /*AFLA*/ #define MAX_CPU_FEATURES		(NCAPINTS * 32)
// /*AFLA*/ #define cpu_have_feature		boot_cpu_has
// /*AFLA*/ 
// /*AFLA*/ #define CPU_FEATURE_TYPEFMT		"x86,ven%04Xfam%04Xmod%04X"
// /*AFLA*/ #define CPU_FEATURE_TYPEVAL		boot_cpu_data.x86_vendor, boot_cpu_data.x86, \
// /*AFLA*/ 					boot_cpu_data.x86_model
// /*AFLA*/ 
// /*AFLA*/ #endif /* defined(__KERNEL__) && !defined(__ASSEMBLY__) */
// /*AFLA*/ #endif /* _ASM_X86_CPUFEATURE_H */
