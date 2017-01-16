// /*AFLA*/ #ifndef _ASM_X86_ACPI_H
// /*AFLA*/ #define _ASM_X86_ACPI_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  *  Copyright (C) 2001 Paul Diefenbaugh <paul.s.diefenbaugh@intel.com>
// /*AFLA*/  *  Copyright (C) 2001 Patrick Mochel <mochel@osdl.org>
// /*AFLA*/  *
// /*AFLA*/  * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// /*AFLA*/  *
// /*AFLA*/  *  This program is free software; you can redistribute it and/or modify
// /*AFLA*/  *  it under the terms of the GNU General Public License as published by
// /*AFLA*/  *  the Free Software Foundation; either version 2 of the License, or
// /*AFLA*/  *  (at your option) any later version.
// /*AFLA*/  *
// /*AFLA*/  *  This program is distributed in the hope that it will be useful,
// /*AFLA*/  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/  *  GNU General Public License for more details.
// /*AFLA*/  *
// /*AFLA*/  *  You should have received a copy of the GNU General Public License
// /*AFLA*/  *  along with this program; if not, write to the Free Software
// /*AFLA*/  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// /*AFLA*/  *
// /*AFLA*/  * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// /*AFLA*/  */
// /*AFLA*/ #include <acpi/pdc_intel.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/numa.h>
// /*AFLA*/ #include <asm/fixmap.h>
// /*AFLA*/ #include <asm/processor.h>
// /*AFLA*/ #include <asm/mmu.h>
// /*AFLA*/ #include <asm/mpspec.h>
// /*AFLA*/ #include <asm/realmode.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_ACPI_APEI
// /*AFLA*/ # include <asm/pgtable_types.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_ACPI
// /*AFLA*/ extern int acpi_lapic;
// /*AFLA*/ extern int acpi_ioapic;
// /*AFLA*/ extern int acpi_noirq;
// /*AFLA*/ extern int acpi_strict;
// /*AFLA*/ extern int acpi_disabled;
// /*AFLA*/ extern int acpi_pci_disabled;
// /*AFLA*/ extern int acpi_skip_timer_override;
// /*AFLA*/ extern int acpi_use_timer_override;
// /*AFLA*/ extern int acpi_fix_pin2_polarity;
// /*AFLA*/ extern int acpi_disable_cmcff;
// /*AFLA*/ 
// /*AFLA*/ extern u8 acpi_sci_flags;
// /*AFLA*/ extern int acpi_sci_override_gsi;
// /*AFLA*/ void acpi_pic_sci_set_trigger(unsigned int, u16);
// /*AFLA*/ 
// /*AFLA*/ extern int (*__acpi_register_gsi)(struct device *dev, u32 gsi,
// /*AFLA*/ 				  int trigger, int polarity);
// /*AFLA*/ extern void (*__acpi_unregister_gsi)(u32 gsi);
// /*AFLA*/ 
// /*AFLA*/ static inline void disable_acpi(void)
// /*AFLA*/ {
// /*AFLA*/ 	acpi_disabled = 1;
// /*AFLA*/ 	acpi_pci_disabled = 1;
// /*AFLA*/ 	acpi_noirq = 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int acpi_gsi_to_irq(u32 gsi, unsigned int *irq);
// /*AFLA*/ 
// /*AFLA*/ static inline void acpi_noirq_set(void) { acpi_noirq = 1; }
// /*AFLA*/ static inline void acpi_disable_pci(void)
// /*AFLA*/ {
// /*AFLA*/ 	acpi_pci_disabled = 1;
// /*AFLA*/ 	acpi_noirq_set();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Low-level suspend routine. */
// /*AFLA*/ extern int (*acpi_suspend_lowlevel)(void);
// /*AFLA*/ 
// /*AFLA*/ /* Physical address to resume after wakeup */
// /*AFLA*/ #define acpi_wakeup_address ((unsigned long)(real_mode_header->wakeup_start))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Check if the CPU can handle C2 and deeper
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned int acpi_processor_cstate_check(unsigned int max_cstate)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Early models (<=5) of AMD Opterons are not supposed to go into
// /*AFLA*/ 	 * C2 state.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Steppings 0x0A and later are good
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (boot_cpu_data.x86 == 0x0F &&
// /*AFLA*/ 	    boot_cpu_data.x86_vendor == X86_VENDOR_AMD &&
// /*AFLA*/ 	    boot_cpu_data.x86_model <= 0x05 &&
// /*AFLA*/ 	    boot_cpu_data.x86_mask < 0x0A)
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	else if (amd_e400_c1e_detected)
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	else
// /*AFLA*/ 		return max_cstate;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool arch_has_acpi_pdc(void)
// /*AFLA*/ {
// /*AFLA*/ 	struct cpuinfo_x86 *c = &cpu_data(0);
// /*AFLA*/ 	return (c->x86_vendor == X86_VENDOR_INTEL ||
// /*AFLA*/ 		c->x86_vendor == X86_VENDOR_CENTAUR);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void arch_acpi_set_pdc_bits(u32 *buf)
// /*AFLA*/ {
// /*AFLA*/ 	struct cpuinfo_x86 *c = &cpu_data(0);
// /*AFLA*/ 
// /*AFLA*/ 	buf[2] |= ACPI_PDC_C_CAPABILITY_SMP;
// /*AFLA*/ 
// /*AFLA*/ 	if (cpu_has(c, X86_FEATURE_EST))
// /*AFLA*/ 		buf[2] |= ACPI_PDC_EST_CAPABILITY_SWSMP;
// /*AFLA*/ 
// /*AFLA*/ 	if (cpu_has(c, X86_FEATURE_ACPI))
// /*AFLA*/ 		buf[2] |= ACPI_PDC_T_FFH;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If mwait/monitor is unsupported, C2/C3_FFH will be disabled
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (!cpu_has(c, X86_FEATURE_MWAIT))
// /*AFLA*/ 		buf[2] &= ~(ACPI_PDC_C_C2C3_FFH);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool acpi_has_cpu_in_madt(void)
// /*AFLA*/ {
// /*AFLA*/ 	return !!acpi_lapic;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_ACPI */
// /*AFLA*/ 
// /*AFLA*/ #define acpi_lapic 0
// /*AFLA*/ #define acpi_ioapic 0
// /*AFLA*/ #define acpi_disable_cmcff 0
// /*AFLA*/ static inline void acpi_noirq_set(void) { }
// /*AFLA*/ static inline void acpi_disable_pci(void) { }
// /*AFLA*/ static inline void disable_acpi(void) { }
// /*AFLA*/ 
// /*AFLA*/ #endif /* !CONFIG_ACPI */
// /*AFLA*/ 
// /*AFLA*/ #define ARCH_HAS_POWER_INIT	1
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_ACPI_NUMA
// /*AFLA*/ extern int x86_acpi_numa_init(void);
// /*AFLA*/ #endif /* CONFIG_ACPI_NUMA */
// /*AFLA*/ 
// /*AFLA*/ #define acpi_unlazy_tlb(x)	leave_mm(x)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_ACPI_APEI
// /*AFLA*/ static inline pgprot_t arch_apei_get_mem_attribute(phys_addr_t addr)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * We currently have no way to look up the EFI memory map
// /*AFLA*/ 	 * attributes for a region in a consistent way, because the
// /*AFLA*/ 	 * memmap is discarded after efi_free_boot_services(). So if
// /*AFLA*/ 	 * you call efi_mem_attributes() during boot and at runtime,
// /*AFLA*/ 	 * you could theoretically see different attributes.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Since we are yet to see any x86 platforms that require
// /*AFLA*/ 	 * anything other than PAGE_KERNEL (some arm64 platforms
// /*AFLA*/ 	 * require the equivalent of PAGE_KERNEL_NOCACHE), return that
// /*AFLA*/ 	 * until we know differently.
// /*AFLA*/ 	 */
// /*AFLA*/ 	 return PAGE_KERNEL;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define ACPI_TABLE_UPGRADE_MAX_PHYS (max_low_pfn_mapped << PAGE_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_ACPI_H */
