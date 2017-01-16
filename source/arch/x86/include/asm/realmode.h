// /*AFLA*/ #ifndef _ARCH_X86_REALMODE_H
// /*AFLA*/ #define _ARCH_X86_REALMODE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <asm/io.h>
// /*AFLA*/ 
// /*AFLA*/ /* This must match data at realmode.S */
// /*AFLA*/ struct real_mode_header {
// /*AFLA*/ 	u32	text_start;
// /*AFLA*/ 	u32	ro_end;
// /*AFLA*/ 	/* SMP trampoline */
// /*AFLA*/ 	u32	trampoline_start;
// /*AFLA*/ 	u32	trampoline_status;
// /*AFLA*/ 	u32	trampoline_header;
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 	u32	trampoline_pgd;
// /*AFLA*/ #endif
// /*AFLA*/ 	/* ACPI S3 wakeup */
// /*AFLA*/ #ifdef CONFIG_ACPI_SLEEP
// /*AFLA*/ 	u32	wakeup_start;
// /*AFLA*/ 	u32	wakeup_header;
// /*AFLA*/ #endif
// /*AFLA*/ 	/* APM/BIOS reboot */
// /*AFLA*/ 	u32	machine_real_restart_asm;
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 	u32	machine_real_restart_seg;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* This must match data at trampoline_32/64.S */
// /*AFLA*/ struct trampoline_header {
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 	u32 start;
// /*AFLA*/ 	u16 gdt_pad;
// /*AFLA*/ 	u16 gdt_limit;
// /*AFLA*/ 	u32 gdt_base;
// /*AFLA*/ #else
// /*AFLA*/ 	u64 start;
// /*AFLA*/ 	u64 efer;
// /*AFLA*/ 	u32 cr4;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct real_mode_header *real_mode_header;
// /*AFLA*/ extern unsigned char real_mode_blob_end[];
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long initial_code;
// /*AFLA*/ extern unsigned long initial_gs;
// /*AFLA*/ extern unsigned long initial_stack;
// /*AFLA*/ 
// /*AFLA*/ extern unsigned char real_mode_blob[];
// /*AFLA*/ extern unsigned char real_mode_relocs[];
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ extern unsigned char startup_32_smp[];
// /*AFLA*/ extern unsigned char boot_gdt[];
// /*AFLA*/ #else
// /*AFLA*/ extern unsigned char secondary_startup_64[];
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline size_t real_mode_size_needed(void)
// /*AFLA*/ {
// /*AFLA*/ 	if (real_mode_header)
// /*AFLA*/ 		return 0;	/* already allocated. */
// /*AFLA*/ 
// /*AFLA*/ 	return ALIGN(real_mode_blob_end - real_mode_blob, PAGE_SIZE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void set_real_mode_mem(phys_addr_t mem, size_t size);
// /*AFLA*/ void reserve_real_mode(void);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ARCH_X86_REALMODE_H */
