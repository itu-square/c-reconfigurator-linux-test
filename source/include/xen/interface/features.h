// /*AFLA*/ /******************************************************************************
// /*AFLA*/  * features.h
// /*AFLA*/  *
// /*AFLA*/  * Feature flags, reported by XENVER_get_features.
// /*AFLA*/  *
// /*AFLA*/  * Copyright (c) 2006, Keir Fraser <keir@xensource.com>
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __XEN_PUBLIC_FEATURES_H__
// /*AFLA*/ #define __XEN_PUBLIC_FEATURES_H__
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * If set, the guest does not need to write-protect its pagetables, and can
// /*AFLA*/  * update them via direct writes.
// /*AFLA*/  */
// /*AFLA*/ #define XENFEAT_writable_page_tables       0
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * If set, the guest does not need to write-protect its segment descriptor
// /*AFLA*/  * tables, and can update them via direct writes.
// /*AFLA*/  */
// /*AFLA*/ #define XENFEAT_writable_descriptor_tables 1
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * If set, translation between the guest's 'pseudo-physical' address space
// /*AFLA*/  * and the host's machine address space are handled by the hypervisor. In this
// /*AFLA*/  * mode the guest does not need to perform phys-to/from-machine translations
// /*AFLA*/  * when performing page table operations.
// /*AFLA*/  */
// /*AFLA*/ #define XENFEAT_auto_translated_physmap    2
// /*AFLA*/ 
// /*AFLA*/ /* If set, the guest is running in supervisor mode (e.g., x86 ring 0). */
// /*AFLA*/ #define XENFEAT_supervisor_mode_kernel     3
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * If set, the guest does not need to allocate x86 PAE page directories
// /*AFLA*/  * below 4GB. This flag is usually implied by auto_translated_physmap.
// /*AFLA*/  */
// /*AFLA*/ #define XENFEAT_pae_pgdir_above_4gb        4
// /*AFLA*/ 
// /*AFLA*/ /* x86: Does this Xen host support the MMU_PT_UPDATE_PRESERVE_AD hypercall? */
// /*AFLA*/ #define XENFEAT_mmu_pt_update_preserve_ad  5
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * If set, GNTTABOP_map_grant_ref honors flags to be placed into guest kernel
// /*AFLA*/  * available pte bits.
// /*AFLA*/  */
// /*AFLA*/ #define XENFEAT_gnttab_map_avail_bits      7
// /*AFLA*/ 
// /*AFLA*/ /* x86: Does this Xen host support the HVM callback vector type? */
// /*AFLA*/ #define XENFEAT_hvm_callback_vector        8
// /*AFLA*/ 
// /*AFLA*/ /* x86: pvclock algorithm is safe to use on HVM */
// /*AFLA*/ #define XENFEAT_hvm_safe_pvclock           9
// /*AFLA*/ 
// /*AFLA*/ /* x86: pirq can be used by HVM guests */
// /*AFLA*/ #define XENFEAT_hvm_pirqs           10
// /*AFLA*/ 
// /*AFLA*/ /* operation as Dom0 is supported */
// /*AFLA*/ #define XENFEAT_dom0                      11
// /*AFLA*/ 
// /*AFLA*/ #define XENFEAT_NR_SUBMAPS 1
// /*AFLA*/ 
// /*AFLA*/ #endif /* __XEN_PUBLIC_FEATURES_H__ */
