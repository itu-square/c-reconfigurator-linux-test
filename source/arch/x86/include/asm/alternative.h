// /*AFLA*/ #ifndef _ASM_X86_ALTERNATIVE_H
// /*AFLA*/ #define _ASM_X86_ALTERNATIVE_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/stddef.h>
// /*AFLA*/ #include <linux/stringify.h>
// /*AFLA*/ #include <asm/asm.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Alternative inline assembly for SMP.
// /*AFLA*/  *
// /*AFLA*/  * The LOCK_PREFIX macro defined here replaces the LOCK and
// /*AFLA*/  * LOCK_PREFIX macros used everywhere in the source tree.
// /*AFLA*/  *
// /*AFLA*/  * SMP alternatives use the same data structures as the other
// /*AFLA*/  * alternatives and the X86_FEATURE_UP flag to indicate the case of a
// /*AFLA*/  * UP system running a SMP kernel.  The existing apply_alternatives()
// /*AFLA*/  * works fine for patching a SMP kernel for UP.
// /*AFLA*/  *
// /*AFLA*/  * The SMP alternative tables can be kept after boot and contain both
// /*AFLA*/  * UP and SMP versions of the instructions to allow switching back to
// /*AFLA*/  * SMP at runtime, when hotplugging in a new CPU, which is especially
// /*AFLA*/  * useful in virtualized environments.
// /*AFLA*/  *
// /*AFLA*/  * The very common lock prefix is handled as special case in a
// /*AFLA*/  * separate table which is a pure address list without replacement ptr
// /*AFLA*/  * and size information.  That keeps the table sizes small.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ #define LOCK_PREFIX_HERE \
// /*AFLA*/ 		".pushsection .smp_locks,\"a\"\n"	\
// /*AFLA*/ 		".balign 4\n"				\
// /*AFLA*/ 		".long 671f - .\n" /* offset */		\
// /*AFLA*/ 		".popsection\n"				\
// /*AFLA*/ 		"671:"
// /*AFLA*/ 
// /*AFLA*/ #define LOCK_PREFIX LOCK_PREFIX_HERE "\n\tlock; "
// /*AFLA*/ 
// /*AFLA*/ #else /* ! CONFIG_SMP */
// /*AFLA*/ #define LOCK_PREFIX_HERE ""
// /*AFLA*/ #define LOCK_PREFIX ""
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct alt_instr {
// /*AFLA*/ 	s32 instr_offset;	/* original instruction */
// /*AFLA*/ 	s32 repl_offset;	/* offset to replacement instruction */
// /*AFLA*/ 	u16 cpuid;		/* cpuid bit set for replacement */
// /*AFLA*/ 	u8  instrlen;		/* length of original instruction */
// /*AFLA*/ 	u8  replacementlen;	/* length of new instruction */
// /*AFLA*/ 	u8  padlen;		/* length of build-time padding */
// /*AFLA*/ } __packed;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Debug flag that can be tested to see whether alternative
// /*AFLA*/  * instructions were patched in already:
// /*AFLA*/  */
// /*AFLA*/ extern int alternatives_patched;
// /*AFLA*/ 
// /*AFLA*/ extern void alternative_instructions(void);
// /*AFLA*/ extern void apply_alternatives(struct alt_instr *start, struct alt_instr *end);
// /*AFLA*/ 
// /*AFLA*/ struct module;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SMP
// /*AFLA*/ extern void alternatives_smp_module_add(struct module *mod, char *name,
// /*AFLA*/ 					void *locks, void *locks_end,
// /*AFLA*/ 					void *text, void *text_end);
// /*AFLA*/ extern void alternatives_smp_module_del(struct module *mod);
// /*AFLA*/ extern void alternatives_enable_smp(void);
// /*AFLA*/ extern int alternatives_text_reserved(void *start, void *end);
// /*AFLA*/ extern bool skip_smp_alternatives;
// /*AFLA*/ #else
// /*AFLA*/ static inline void alternatives_smp_module_add(struct module *mod, char *name,
// /*AFLA*/ 					       void *locks, void *locks_end,
// /*AFLA*/ 					       void *text, void *text_end) {}
// /*AFLA*/ static inline void alternatives_smp_module_del(struct module *mod) {}
// /*AFLA*/ static inline void alternatives_enable_smp(void) {}
// /*AFLA*/ static inline int alternatives_text_reserved(void *start, void *end)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif	/* CONFIG_SMP */
// /*AFLA*/ 
// /*AFLA*/ #define b_replacement(num)	"664"#num
// /*AFLA*/ #define e_replacement(num)	"665"#num
// /*AFLA*/ 
// /*AFLA*/ #define alt_end_marker		"663"
// /*AFLA*/ #define alt_slen		"662b-661b"
// /*AFLA*/ #define alt_pad_len		alt_end_marker"b-662b"
// /*AFLA*/ #define alt_total_slen		alt_end_marker"b-661b"
// /*AFLA*/ #define alt_rlen(num)		e_replacement(num)"f-"b_replacement(num)"f"
// /*AFLA*/ 
// /*AFLA*/ #define __OLDINSTR(oldinstr, num)					\
// /*AFLA*/ 	"661:\n\t" oldinstr "\n662:\n"					\
// /*AFLA*/ 	".skip -(((" alt_rlen(num) ")-(" alt_slen ")) > 0) * "		\
// /*AFLA*/ 		"((" alt_rlen(num) ")-(" alt_slen ")),0x90\n"
// /*AFLA*/ 
// /*AFLA*/ #define OLDINSTR(oldinstr, num)						\
// /*AFLA*/ 	__OLDINSTR(oldinstr, num)					\
// /*AFLA*/ 	alt_end_marker ":\n"
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * max without conditionals. Idea adapted from:
// /*AFLA*/  * http://graphics.stanford.edu/~seander/bithacks.html#IntegerMinOrMax
// /*AFLA*/  *
// /*AFLA*/  * The additional "-" is needed because gas works with s32s.
// /*AFLA*/  */
// /*AFLA*/ #define alt_max_short(a, b)	"((" a ") ^ (((" a ") ^ (" b ")) & -(-((" a ") - (" b ")))))"
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Pad the second replacement alternative with additional NOPs if it is
// /*AFLA*/  * additionally longer than the first replacement alternative.
// /*AFLA*/  */
// /*AFLA*/ #define OLDINSTR_2(oldinstr, num1, num2) \
// /*AFLA*/ 	"661:\n\t" oldinstr "\n662:\n"								\
// /*AFLA*/ 	".skip -((" alt_max_short(alt_rlen(num1), alt_rlen(num2)) " - (" alt_slen ")) > 0) * "	\
// /*AFLA*/ 		"(" alt_max_short(alt_rlen(num1), alt_rlen(num2)) " - (" alt_slen ")), 0x90\n"	\
// /*AFLA*/ 	alt_end_marker ":\n"
// /*AFLA*/ 
// /*AFLA*/ #define ALTINSTR_ENTRY(feature, num)					      \
// /*AFLA*/ 	" .long 661b - .\n"				/* label           */ \
// /*AFLA*/ 	" .long " b_replacement(num)"f - .\n"		/* new instruction */ \
// /*AFLA*/ 	" .word " __stringify(feature) "\n"		/* feature bit     */ \
// /*AFLA*/ 	" .byte " alt_total_slen "\n"			/* source len      */ \
// /*AFLA*/ 	" .byte " alt_rlen(num) "\n"			/* replacement len */ \
// /*AFLA*/ 	" .byte " alt_pad_len "\n"			/* pad len */
// /*AFLA*/ 
// /*AFLA*/ #define ALTINSTR_REPLACEMENT(newinstr, feature, num)	/* replacement */     \
// /*AFLA*/ 	b_replacement(num)":\n\t" newinstr "\n" e_replacement(num) ":\n\t"
// /*AFLA*/ 
// /*AFLA*/ /* alternative assembly primitive: */
// /*AFLA*/ #define ALTERNATIVE(oldinstr, newinstr, feature)			\
// /*AFLA*/ 	OLDINSTR(oldinstr, 1)						\
// /*AFLA*/ 	".pushsection .altinstructions,\"a\"\n"				\
// /*AFLA*/ 	ALTINSTR_ENTRY(feature, 1)					\
// /*AFLA*/ 	".popsection\n"							\
// /*AFLA*/ 	".pushsection .altinstr_replacement, \"ax\"\n"			\
// /*AFLA*/ 	ALTINSTR_REPLACEMENT(newinstr, feature, 1)			\
// /*AFLA*/ 	".popsection"
// /*AFLA*/ 
// /*AFLA*/ #define ALTERNATIVE_2(oldinstr, newinstr1, feature1, newinstr2, feature2)\
// /*AFLA*/ 	OLDINSTR_2(oldinstr, 1, 2)					\
// /*AFLA*/ 	".pushsection .altinstructions,\"a\"\n"				\
// /*AFLA*/ 	ALTINSTR_ENTRY(feature1, 1)					\
// /*AFLA*/ 	ALTINSTR_ENTRY(feature2, 2)					\
// /*AFLA*/ 	".popsection\n"							\
// /*AFLA*/ 	".pushsection .altinstr_replacement, \"ax\"\n"			\
// /*AFLA*/ 	ALTINSTR_REPLACEMENT(newinstr1, feature1, 1)			\
// /*AFLA*/ 	ALTINSTR_REPLACEMENT(newinstr2, feature2, 2)			\
// /*AFLA*/ 	".popsection"
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Alternative instructions for different CPU types or capabilities.
// /*AFLA*/  *
// /*AFLA*/  * This allows to use optimized instructions even on generic binary
// /*AFLA*/  * kernels.
// /*AFLA*/  *
// /*AFLA*/  * length of oldinstr must be longer or equal the length of newinstr
// /*AFLA*/  * It can be padded with nops as needed.
// /*AFLA*/  *
// /*AFLA*/  * For non barrier like inlines please define new variants
// /*AFLA*/  * without volatile and memory clobber.
// /*AFLA*/  */
// /*AFLA*/ #define alternative(oldinstr, newinstr, feature)			\
// /*AFLA*/ 	asm volatile (ALTERNATIVE(oldinstr, newinstr, feature) : : : "memory")
// /*AFLA*/ 
// /*AFLA*/ #define alternative_2(oldinstr, newinstr1, feature1, newinstr2, feature2) \
// /*AFLA*/ 	asm volatile(ALTERNATIVE_2(oldinstr, newinstr1, feature1, newinstr2, feature2) ::: "memory")
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Alternative inline assembly with input.
// /*AFLA*/  *
// /*AFLA*/  * Pecularities:
// /*AFLA*/  * No memory clobber here.
// /*AFLA*/  * Argument numbers start with 1.
// /*AFLA*/  * Best is to use constraints that are fixed size (like (%1) ... "r")
// /*AFLA*/  * If you use variable sized constraints like "m" or "g" in the
// /*AFLA*/  * replacement make sure to pad to the worst case length.
// /*AFLA*/  * Leaving an unused argument 0 to keep API compatibility.
// /*AFLA*/  */
// /*AFLA*/ #define alternative_input(oldinstr, newinstr, feature, input...)	\
// /*AFLA*/ 	asm volatile (ALTERNATIVE(oldinstr, newinstr, feature)		\
// /*AFLA*/ 		: : "i" (0), ## input)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is similar to alternative_input. But it has two features and
// /*AFLA*/  * respective instructions.
// /*AFLA*/  *
// /*AFLA*/  * If CPU has feature2, newinstr2 is used.
// /*AFLA*/  * Otherwise, if CPU has feature1, newinstr1 is used.
// /*AFLA*/  * Otherwise, oldinstr is used.
// /*AFLA*/  */
// /*AFLA*/ #define alternative_input_2(oldinstr, newinstr1, feature1, newinstr2,	     \
// /*AFLA*/ 			   feature2, input...)				     \
// /*AFLA*/ 	asm volatile(ALTERNATIVE_2(oldinstr, newinstr1, feature1,	     \
// /*AFLA*/ 		newinstr2, feature2)					     \
// /*AFLA*/ 		: : "i" (0), ## input)
// /*AFLA*/ 
// /*AFLA*/ /* Like alternative_input, but with a single output argument */
// /*AFLA*/ #define alternative_io(oldinstr, newinstr, feature, output, input...)	\
// /*AFLA*/ 	asm volatile (ALTERNATIVE(oldinstr, newinstr, feature)		\
// /*AFLA*/ 		: output : "i" (0), ## input)
// /*AFLA*/ 
// /*AFLA*/ /* Like alternative_io, but for replacing a direct call with another one. */
// /*AFLA*/ #define alternative_call(oldfunc, newfunc, feature, output, input...)	\
// /*AFLA*/ 	asm volatile (ALTERNATIVE("call %P[old]", "call %P[new]", feature) \
// /*AFLA*/ 		: output : [old] "i" (oldfunc), [new] "i" (newfunc), ## input)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Like alternative_call, but there are two features and respective functions.
// /*AFLA*/  * If CPU has feature2, function2 is used.
// /*AFLA*/  * Otherwise, if CPU has feature1, function1 is used.
// /*AFLA*/  * Otherwise, old function is used.
// /*AFLA*/  */
// /*AFLA*/ #define alternative_call_2(oldfunc, newfunc1, feature1, newfunc2, feature2,   \
// /*AFLA*/ 			   output, input...)				      \
// /*AFLA*/ {									      \
// /*AFLA*/ 	register void *__sp asm(_ASM_SP);				      \
// /*AFLA*/ 	asm volatile (ALTERNATIVE_2("call %P[old]", "call %P[new1]", feature1,\
// /*AFLA*/ 		"call %P[new2]", feature2)				      \
// /*AFLA*/ 		: output, "+r" (__sp)					      \
// /*AFLA*/ 		: [old] "i" (oldfunc), [new1] "i" (newfunc1),		      \
// /*AFLA*/ 		  [new2] "i" (newfunc2), ## input);			      \
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * use this macro(s) if you need more than one output parameter
// /*AFLA*/  * in alternative_io
// /*AFLA*/  */
// /*AFLA*/ #define ASM_OUTPUT2(a...) a
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * use this macro if you need clobbers but no inputs in
// /*AFLA*/  * alternative_{input,io,call}()
// /*AFLA*/  */
// /*AFLA*/ #define ASM_NO_INPUT_CLOBBER(clbr...) "i" (0) : clbr
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_ALTERNATIVE_H */
