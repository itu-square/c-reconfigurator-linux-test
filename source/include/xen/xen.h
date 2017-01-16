// /*AFLA*/ #ifndef _XEN_XEN_H
// /*AFLA*/ #define _XEN_XEN_H
// /*AFLA*/ 
// /*AFLA*/ enum xen_domain_type {
// /*AFLA*/ 	XEN_NATIVE,		/* running on bare hardware    */
// /*AFLA*/ 	XEN_PV_DOMAIN,		/* running in a PV domain      */
// /*AFLA*/ 	XEN_HVM_DOMAIN,		/* running in a Xen hvm domain */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_XEN
// /*AFLA*/ extern enum xen_domain_type xen_domain_type;
// /*AFLA*/ #else
// /*AFLA*/ #define xen_domain_type		XEN_NATIVE
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define xen_domain()		(xen_domain_type != XEN_NATIVE)
// /*AFLA*/ #define xen_pv_domain()		(xen_domain() &&			\
// /*AFLA*/ 				 xen_domain_type == XEN_PV_DOMAIN)
// /*AFLA*/ #define xen_hvm_domain()	(xen_domain() &&			\
// /*AFLA*/ 				 xen_domain_type == XEN_HVM_DOMAIN)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_XEN_DOM0
// /*AFLA*/ #include <xen/interface/xen.h>
// /*AFLA*/ #include <asm/xen/hypervisor.h>
// /*AFLA*/ 
// /*AFLA*/ #define xen_initial_domain()	(xen_domain() && \
// /*AFLA*/ 				 xen_start_info && xen_start_info->flags & SIF_INITDOMAIN)
// /*AFLA*/ #else  /* !CONFIG_XEN_DOM0 */
// /*AFLA*/ #define xen_initial_domain()	(0)
// /*AFLA*/ #endif	/* CONFIG_XEN_DOM0 */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_XEN_PVH
// /*AFLA*/ /* This functionality exists only for x86. The XEN_PVHVM support exists
// /*AFLA*/  * only in x86 world - hence on ARM it will be always disabled.
// /*AFLA*/  * N.B. ARM guests are neither PV nor HVM nor PVHVM.
// /*AFLA*/  * It's a bit like PVH but is different also (it's further towards the H
// /*AFLA*/  * end of the spectrum than even PVH).
// /*AFLA*/  */
// /*AFLA*/ #include <xen/features.h>
// /*AFLA*/ #define xen_pvh_domain() (xen_pv_domain() && \
// /*AFLA*/ 			  xen_feature(XENFEAT_auto_translated_physmap))
// /*AFLA*/ #else
// /*AFLA*/ #define xen_pvh_domain()	(0)
// /*AFLA*/ #endif
// /*AFLA*/ #endif	/* _XEN_XEN_H */
