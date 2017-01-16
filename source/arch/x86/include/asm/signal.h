// /*AFLA*/ #ifndef _ASM_X86_SIGNAL_H
// /*AFLA*/ #define _ASM_X86_SIGNAL_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ 
// /*AFLA*/ /* Most things should be clean enough to redefine this at will, if care
// /*AFLA*/    is taken to make libc match.  */
// /*AFLA*/ 
// /*AFLA*/ #define _NSIG		64
// /*AFLA*/ 
// /*AFLA*/ #ifdef __i386__
// /*AFLA*/ # define _NSIG_BPW	32
// /*AFLA*/ #else
// /*AFLA*/ # define _NSIG_BPW	64
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define _NSIG_WORDS	(_NSIG / _NSIG_BPW)
// /*AFLA*/ 
// /*AFLA*/ typedef unsigned long old_sigset_t;		/* at least 32 bits */
// /*AFLA*/ 
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	unsigned long sig[_NSIG_WORDS];
// /*AFLA*/ } sigset_t;
// /*AFLA*/ 
// /*AFLA*/ /* non-uapi in-kernel SA_FLAGS for those indicates ABI for a signal frame */
// /*AFLA*/ #define SA_IA32_ABI	0x02000000u
// /*AFLA*/ #define SA_X32_ABI	0x01000000u
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_COMPAT
// /*AFLA*/ typedef sigset_t compat_sigset_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ #include <uapi/asm/signal.h>
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ extern void do_signal(struct pt_regs *regs);
// /*AFLA*/ 
// /*AFLA*/ #define __ARCH_HAS_SA_RESTORER
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/asm/sigcontext.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef __i386__
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_SIG_BITOPS
// /*AFLA*/ 
// /*AFLA*/ #define sigaddset(set,sig)		    \
// /*AFLA*/ 	(__builtin_constant_p(sig)	    \
// /*AFLA*/ 	 ? __const_sigaddset((set), (sig))  \
// /*AFLA*/ 	 : __gen_sigaddset((set), (sig)))
// /*AFLA*/ 
// /*AFLA*/ static inline void __gen_sigaddset(sigset_t *set, int _sig)
// /*AFLA*/ {
// /*AFLA*/ 	asm("btsl %1,%0" : "+m"(*set) : "Ir"(_sig - 1) : "cc");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __const_sigaddset(sigset_t *set, int _sig)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long sig = _sig - 1;
// /*AFLA*/ 	set->sig[sig / _NSIG_BPW] |= 1 << (sig % _NSIG_BPW);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define sigdelset(set, sig)		    \
// /*AFLA*/ 	(__builtin_constant_p(sig)	    \
// /*AFLA*/ 	 ? __const_sigdelset((set), (sig))  \
// /*AFLA*/ 	 : __gen_sigdelset((set), (sig)))
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ static inline void __gen_sigdelset(sigset_t *set, int _sig)
// /*AFLA*/ {
// /*AFLA*/ 	asm("btrl %1,%0" : "+m"(*set) : "Ir"(_sig - 1) : "cc");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __const_sigdelset(sigset_t *set, int _sig)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long sig = _sig - 1;
// /*AFLA*/ 	set->sig[sig / _NSIG_BPW] &= ~(1 << (sig % _NSIG_BPW));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __const_sigismember(sigset_t *set, int _sig)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long sig = _sig - 1;
// /*AFLA*/ 	return 1 & (set->sig[sig / _NSIG_BPW] >> (sig % _NSIG_BPW));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __gen_sigismember(sigset_t *set, int _sig)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned char ret;
// /*AFLA*/ 	asm("btl %2,%1\n\tsetc %0"
// /*AFLA*/ 	    : "=qm"(ret) : "m"(*set), "Ir"(_sig-1) : "cc");
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define sigismember(set, sig)			\
// /*AFLA*/ 	(__builtin_constant_p(sig)		\
// /*AFLA*/ 	 ? __const_sigismember((set), (sig))	\
// /*AFLA*/ 	 : __gen_sigismember((set), (sig)))
// /*AFLA*/ 
// /*AFLA*/ struct pt_regs;
// /*AFLA*/ 
// /*AFLA*/ #else /* __i386__ */
// /*AFLA*/ 
// /*AFLA*/ #undef __HAVE_ARCH_SIG_BITOPS
// /*AFLA*/ 
// /*AFLA*/ #endif /* !__i386__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ #endif /* _ASM_X86_SIGNAL_H */
