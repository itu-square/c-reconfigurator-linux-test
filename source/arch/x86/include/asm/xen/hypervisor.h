// /*AFLA*/ /******************************************************************************
// /*AFLA*/  * hypervisor.h
// /*AFLA*/  *
// /*AFLA*/  * Linux-specific hypervisor handling.
// /*AFLA*/  *
// /*AFLA*/  * Copyright (c) 2002-2004, K A Fraser
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or
// /*AFLA*/  * modify it under the terms of the GNU General Public License version 2
// /*AFLA*/  * as published by the Free Software Foundation; or, when distributed
// /*AFLA*/  * separately from the Linux kernel or incorporated into other
// /*AFLA*/  * software packages, subject to the following license:
// /*AFLA*/  *
// /*AFLA*/  * Permission is hereby granted, free of charge, to any person obtaining a copy
// /*AFLA*/  * of this source file (the "Software"), to deal in the Software without
// /*AFLA*/  * restriction, including without limitation the rights to use, copy, modify,
// /*AFLA*/  * merge, publish, distribute, sublicense, and/or sell copies of the Software,
// /*AFLA*/  * and to permit persons to whom the Software is furnished to do so, subject to
// /*AFLA*/  * the following conditions:
// /*AFLA*/  *
// /*AFLA*/  * The above copyright notice and this permission notice shall be included in
// /*AFLA*/  * all copies or substantial portions of the Software.
// /*AFLA*/  *
// /*AFLA*/  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// /*AFLA*/  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// /*AFLA*/  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// /*AFLA*/  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// /*AFLA*/  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// /*AFLA*/  * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// /*AFLA*/  * IN THE SOFTWARE.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _ASM_X86_XEN_HYPERVISOR_H
// /*AFLA*/ #define _ASM_X86_XEN_HYPERVISOR_H
// /*AFLA*/ 
// /*AFLA*/ extern struct shared_info *HYPERVISOR_shared_info;
// /*AFLA*/ extern struct start_info *xen_start_info;
// /*AFLA*/ 
// /*AFLA*/ #include <asm/processor.h>
// /*AFLA*/ 
// /*AFLA*/ static inline uint32_t xen_cpuid_base(void)
// /*AFLA*/ {
// /*AFLA*/ 	return hypervisor_cpuid_base("XenVMMXenVMM", 2);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_XEN
// /*AFLA*/ extern bool xen_hvm_need_lapic(void);
// /*AFLA*/ 
// /*AFLA*/ static inline bool xen_x2apic_para_available(void)
// /*AFLA*/ {
// /*AFLA*/ 	return xen_hvm_need_lapic();
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline bool xen_x2apic_para_available(void)
// /*AFLA*/ {
// /*AFLA*/ 	return (xen_cpuid_base() != 0);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HOTPLUG_CPU
// /*AFLA*/ void xen_arch_register_cpu(int num);
// /*AFLA*/ void xen_arch_unregister_cpu(int num);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void xen_set_iopl_mask(unsigned mask);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_XEN_HYPERVISOR_H */
