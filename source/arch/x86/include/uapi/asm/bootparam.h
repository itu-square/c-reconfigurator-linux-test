// /*AFLA*/ #ifndef _ASM_X86_BOOTPARAM_H
// /*AFLA*/ #define _ASM_X86_BOOTPARAM_H
// /*AFLA*/ 
// /*AFLA*/ /* setup_data types */
// /*AFLA*/ #define SETUP_NONE			0
// /*AFLA*/ #define SETUP_E820_EXT			1
// /*AFLA*/ #define SETUP_DTB			2
// /*AFLA*/ #define SETUP_PCI			3
// /*AFLA*/ #define SETUP_EFI			4
// /*AFLA*/ 
// /*AFLA*/ /* ram_size flags */
// /*AFLA*/ #define RAMDISK_IMAGE_START_MASK	0x07FF
// /*AFLA*/ #define RAMDISK_PROMPT_FLAG		0x8000
// /*AFLA*/ #define RAMDISK_LOAD_FLAG		0x4000
// /*AFLA*/ 
// /*AFLA*/ /* loadflags */
// /*AFLA*/ #define LOADED_HIGH	(1<<0)
// /*AFLA*/ #define KASLR_FLAG	(1<<1)
// /*AFLA*/ #define QUIET_FLAG	(1<<5)
// /*AFLA*/ #define KEEP_SEGMENTS	(1<<6)
// /*AFLA*/ #define CAN_USE_HEAP	(1<<7)
// /*AFLA*/ 
// /*AFLA*/ /* xloadflags */
// /*AFLA*/ #define XLF_KERNEL_64			(1<<0)
// /*AFLA*/ #define XLF_CAN_BE_LOADED_ABOVE_4G	(1<<1)
// /*AFLA*/ #define XLF_EFI_HANDOVER_32		(1<<2)
// /*AFLA*/ #define XLF_EFI_HANDOVER_64		(1<<3)
// /*AFLA*/ #define XLF_EFI_KEXEC			(1<<4)
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/screen_info.h>
// /*AFLA*/ #include <linux/apm_bios.h>
// /*AFLA*/ #include <linux/edd.h>
// /*AFLA*/ #include <asm/e820.h>
// /*AFLA*/ #include <asm/ist.h>
// /*AFLA*/ #include <video/edid.h>
// /*AFLA*/ 
// /*AFLA*/ /* extensible setup data list node */
// /*AFLA*/ struct setup_data {
// /*AFLA*/ 	__u64 next;
// /*AFLA*/ 	__u32 type;
// /*AFLA*/ 	__u32 len;
// /*AFLA*/ 	__u8 data[0];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct setup_header {
// /*AFLA*/ 	__u8	setup_sects;
// /*AFLA*/ 	__u16	root_flags;
// /*AFLA*/ 	__u32	syssize;
// /*AFLA*/ 	__u16	ram_size;
// /*AFLA*/ 	__u16	vid_mode;
// /*AFLA*/ 	__u16	root_dev;
// /*AFLA*/ 	__u16	boot_flag;
// /*AFLA*/ 	__u16	jump;
// /*AFLA*/ 	__u32	header;
// /*AFLA*/ 	__u16	version;
// /*AFLA*/ 	__u32	realmode_swtch;
// /*AFLA*/ 	__u16	start_sys;
// /*AFLA*/ 	__u16	kernel_version;
// /*AFLA*/ 	__u8	type_of_loader;
// /*AFLA*/ 	__u8	loadflags;
// /*AFLA*/ 	__u16	setup_move_size;
// /*AFLA*/ 	__u32	code32_start;
// /*AFLA*/ 	__u32	ramdisk_image;
// /*AFLA*/ 	__u32	ramdisk_size;
// /*AFLA*/ 	__u32	bootsect_kludge;
// /*AFLA*/ 	__u16	heap_end_ptr;
// /*AFLA*/ 	__u8	ext_loader_ver;
// /*AFLA*/ 	__u8	ext_loader_type;
// /*AFLA*/ 	__u32	cmd_line_ptr;
// /*AFLA*/ 	__u32	initrd_addr_max;
// /*AFLA*/ 	__u32	kernel_alignment;
// /*AFLA*/ 	__u8	relocatable_kernel;
// /*AFLA*/ 	__u8	min_alignment;
// /*AFLA*/ 	__u16	xloadflags;
// /*AFLA*/ 	__u32	cmdline_size;
// /*AFLA*/ 	__u32	hardware_subarch;
// /*AFLA*/ 	__u64	hardware_subarch_data;
// /*AFLA*/ 	__u32	payload_offset;
// /*AFLA*/ 	__u32	payload_length;
// /*AFLA*/ 	__u64	setup_data;
// /*AFLA*/ 	__u64	pref_address;
// /*AFLA*/ 	__u32	init_size;
// /*AFLA*/ 	__u32	handover_offset;
// /*AFLA*/ } __attribute__((packed));
// /*AFLA*/ 
// /*AFLA*/ struct sys_desc_table {
// /*AFLA*/ 	__u16 length;
// /*AFLA*/ 	__u8  table[14];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Gleaned from OFW's set-parameters in cpu/x86/pc/linux.fth */
// /*AFLA*/ struct olpc_ofw_header {
// /*AFLA*/ 	__u32 ofw_magic;	/* OFW signature */
// /*AFLA*/ 	__u32 ofw_version;
// /*AFLA*/ 	__u32 cif_handler;	/* callback into OFW */
// /*AFLA*/ 	__u32 irq_desc_table;
// /*AFLA*/ } __attribute__((packed));
// /*AFLA*/ 
// /*AFLA*/ struct efi_info {
// /*AFLA*/ 	__u32 efi_loader_signature;
// /*AFLA*/ 	__u32 efi_systab;
// /*AFLA*/ 	__u32 efi_memdesc_size;
// /*AFLA*/ 	__u32 efi_memdesc_version;
// /*AFLA*/ 	__u32 efi_memmap;
// /*AFLA*/ 	__u32 efi_memmap_size;
// /*AFLA*/ 	__u32 efi_systab_hi;
// /*AFLA*/ 	__u32 efi_memmap_hi;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* The so-called "zeropage" */
// /*AFLA*/ struct boot_params {
// /*AFLA*/ 	struct screen_info screen_info;			/* 0x000 */
// /*AFLA*/ 	struct apm_bios_info apm_bios_info;		/* 0x040 */
// /*AFLA*/ 	__u8  _pad2[4];					/* 0x054 */
// /*AFLA*/ 	__u64  tboot_addr;				/* 0x058 */
// /*AFLA*/ 	struct ist_info ist_info;			/* 0x060 */
// /*AFLA*/ 	__u8  _pad3[16];				/* 0x070 */
// /*AFLA*/ 	__u8  hd0_info[16];	/* obsolete! */		/* 0x080 */
// /*AFLA*/ 	__u8  hd1_info[16];	/* obsolete! */		/* 0x090 */
// /*AFLA*/ 	struct sys_desc_table sys_desc_table; /* obsolete! */	/* 0x0a0 */
// /*AFLA*/ 	struct olpc_ofw_header olpc_ofw_header;		/* 0x0b0 */
// /*AFLA*/ 	__u32 ext_ramdisk_image;			/* 0x0c0 */
// /*AFLA*/ 	__u32 ext_ramdisk_size;				/* 0x0c4 */
// /*AFLA*/ 	__u32 ext_cmd_line_ptr;				/* 0x0c8 */
// /*AFLA*/ 	__u8  _pad4[116];				/* 0x0cc */
// /*AFLA*/ 	struct edid_info edid_info;			/* 0x140 */
// /*AFLA*/ 	struct efi_info efi_info;			/* 0x1c0 */
// /*AFLA*/ 	__u32 alt_mem_k;				/* 0x1e0 */
// /*AFLA*/ 	__u32 scratch;		/* Scratch field! */	/* 0x1e4 */
// /*AFLA*/ 	__u8  e820_entries;				/* 0x1e8 */
// /*AFLA*/ 	__u8  eddbuf_entries;				/* 0x1e9 */
// /*AFLA*/ 	__u8  edd_mbr_sig_buf_entries;			/* 0x1ea */
// /*AFLA*/ 	__u8  kbd_status;				/* 0x1eb */
// /*AFLA*/ 	__u8  _pad5[3];					/* 0x1ec */
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The sentinel is set to a nonzero value (0xff) in header.S.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * A bootloader is supposed to only take setup_header and put
// /*AFLA*/ 	 * it into a clean boot_params buffer. If it turns out that
// /*AFLA*/ 	 * it is clumsy or too generous with the buffer, it most
// /*AFLA*/ 	 * probably will pick up the sentinel variable too. The fact
// /*AFLA*/ 	 * that this variable then is still 0xff will let kernel
// /*AFLA*/ 	 * know that some variables in boot_params are invalid and
// /*AFLA*/ 	 * kernel should zero out certain portions of boot_params.
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u8  sentinel;					/* 0x1ef */
// /*AFLA*/ 	__u8  _pad6[1];					/* 0x1f0 */
// /*AFLA*/ 	struct setup_header hdr;    /* setup header */	/* 0x1f1 */
// /*AFLA*/ 	__u8  _pad7[0x290-0x1f1-sizeof(struct setup_header)];
// /*AFLA*/ 	__u32 edd_mbr_sig_buffer[EDD_MBR_SIG_MAX];	/* 0x290 */
// /*AFLA*/ 	struct e820entry e820_map[E820MAX];		/* 0x2d0 */
// /*AFLA*/ 	__u8  _pad8[48];				/* 0xcd0 */
// /*AFLA*/ 	struct edd_info eddbuf[EDDMAXNR];		/* 0xd00 */
// /*AFLA*/ 	__u8  _pad9[276];				/* 0xeec */
// /*AFLA*/ } __attribute__((packed));
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * enum x86_hardware_subarch - x86 hardware subarchitecture
// /*AFLA*/  *
// /*AFLA*/  * The x86 hardware_subarch and hardware_subarch_data were added as of the x86
// /*AFLA*/  * boot protocol 2.07 to help distinguish and support custom x86 boot
// /*AFLA*/  * sequences. This enum represents accepted values for the x86
// /*AFLA*/  * hardware_subarch.  Custom x86 boot sequences (not X86_SUBARCH_PC) do not
// /*AFLA*/  * have or simply *cannot* make use of natural stubs like BIOS or EFI, the
// /*AFLA*/  * hardware_subarch can be used on the Linux entry path to revector to a
// /*AFLA*/  * subarchitecture stub when needed. This subarchitecture stub can be used to
// /*AFLA*/  * set up Linux boot parameters or for special care to account for nonstandard
// /*AFLA*/  * handling of page tables.
// /*AFLA*/  *
// /*AFLA*/  * These enums should only ever be used by x86 code, and the code that uses
// /*AFLA*/  * it should be well contained and compartamentalized.
// /*AFLA*/  *
// /*AFLA*/  * KVM and Xen HVM do not have a subarch as these are expected to follow
// /*AFLA*/  * standard x86 boot entries. If there is a genuine need for "hypervisor" type
// /*AFLA*/  * that should be considered separately in the future. Future guest types
// /*AFLA*/  * should seriously consider working with standard x86 boot stubs such as
// /*AFLA*/  * the BIOS or EFI boot stubs.
// /*AFLA*/  *
// /*AFLA*/  * WARNING: this enum is only used for legacy hacks, for platform features that
// /*AFLA*/  *	    are not easily enumerated or discoverable. You should not ever use
// /*AFLA*/  *	    this for new features.
// /*AFLA*/  *
// /*AFLA*/  * @X86_SUBARCH_PC: Should be used if the hardware is enumerable using standard
// /*AFLA*/  *	PC mechanisms (PCI, ACPI) and doesn't need a special boot flow.
// /*AFLA*/  * @X86_SUBARCH_LGUEST: Used for x86 hypervisor demo, lguest
// /*AFLA*/  * @X86_SUBARCH_XEN: Used for Xen guest types which follow the PV boot path,
// /*AFLA*/  * 	which start at asm startup_xen() entry point and later jump to the C
// /*AFLA*/  * 	xen_start_kernel() entry point. Both domU and dom0 type of guests are
// /*AFLA*/  * 	currently supportd through this PV boot path.
// /*AFLA*/  * @X86_SUBARCH_INTEL_MID: Used for Intel MID (Mobile Internet Device) platform
// /*AFLA*/  *	systems which do not have the PCI legacy interfaces.
// /*AFLA*/  * @X86_SUBARCH_CE4100: Used for Intel CE media processor (CE4100) SoC for
// /*AFLA*/  * 	for settop boxes and media devices, the use of a subarch for CE4100
// /*AFLA*/  * 	is more of a hack...
// /*AFLA*/  */
// /*AFLA*/ enum x86_hardware_subarch {
// /*AFLA*/ 	X86_SUBARCH_PC = 0,
// /*AFLA*/ 	X86_SUBARCH_LGUEST,
// /*AFLA*/ 	X86_SUBARCH_XEN,
// /*AFLA*/ 	X86_SUBARCH_INTEL_MID,
// /*AFLA*/ 	X86_SUBARCH_CE4100,
// /*AFLA*/ 	X86_NR_SUBARCHS,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_BOOTPARAM_H */
