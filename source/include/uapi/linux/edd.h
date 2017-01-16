// /*AFLA*/ /*
// /*AFLA*/  * linux/include/linux/edd.h
// /*AFLA*/  *  Copyright (C) 2002, 2003, 2004 Dell Inc.
// /*AFLA*/  *  by Matt Domsch <Matt_Domsch@dell.com>
// /*AFLA*/  *
// /*AFLA*/  * structures and definitions for the int 13h, ax={41,48}h
// /*AFLA*/  * BIOS Enhanced Disk Drive Services
// /*AFLA*/  * This is based on the T13 group document D1572 Revision 0 (August 14 2002)
// /*AFLA*/  * available at http://www.t13.org/docs2002/d1572r0.pdf.  It is
// /*AFLA*/  * very similar to D1484 Revision 3 http://www.t13.org/docs2002/d1484r3.pdf
// /*AFLA*/  *
// /*AFLA*/  * In a nutshell, arch/{i386,x86_64}/boot/setup.S populates a scratch
// /*AFLA*/  * table in the boot_params that contains a list of BIOS-enumerated
// /*AFLA*/  * boot devices.
// /*AFLA*/  * In arch/{i386,x86_64}/kernel/setup.c, this information is
// /*AFLA*/  * transferred into the edd structure, and in drivers/firmware/edd.c, that
// /*AFLA*/  * information is used to identify BIOS boot disk.  The code in setup.S
// /*AFLA*/  * is very sensitive to the size of these structures.
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify
// /*AFLA*/  * it under the terms of the GNU General Public License v2.0 as published by
// /*AFLA*/  * the Free Software Foundation
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it will be useful,
// /*AFLA*/  * but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/  * GNU General Public License for more details.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ #ifndef _UAPI_LINUX_EDD_H
// /*AFLA*/ #define _UAPI_LINUX_EDD_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ #define EDDNR 0x1e9		/* addr of number of edd_info structs at EDDBUF
// /*AFLA*/ 				   in boot_params - treat this as 1 byte  */
// /*AFLA*/ #define EDDBUF	0xd00		/* addr of edd_info structs in boot_params */
// /*AFLA*/ #define EDDMAXNR 6		/* number of edd_info structs starting at EDDBUF  */
// /*AFLA*/ #define EDDEXTSIZE 8		/* change these if you muck with the structures */
// /*AFLA*/ #define EDDPARMSIZE 74
// /*AFLA*/ #define CHECKEXTENSIONSPRESENT 0x41
// /*AFLA*/ #define GETDEVICEPARAMETERS 0x48
// /*AFLA*/ #define LEGACYGETDEVICEPARAMETERS 0x08
// /*AFLA*/ #define EDDMAGIC1 0x55AA
// /*AFLA*/ #define EDDMAGIC2 0xAA55
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define READ_SECTORS 0x02         /* int13 AH=0x02 is READ_SECTORS command */
// /*AFLA*/ #define EDD_MBR_SIG_OFFSET 0x1B8  /* offset of signature in the MBR */
// /*AFLA*/ #define EDD_MBR_SIG_BUF    0x290  /* addr in boot params */
// /*AFLA*/ #define EDD_MBR_SIG_MAX 16        /* max number of signatures to store */
// /*AFLA*/ #define EDD_MBR_SIG_NR_BUF 0x1ea  /* addr of number of MBR signtaures at EDD_MBR_SIG_BUF
// /*AFLA*/ 				     in boot_params - treat this as 1 byte  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #define EDD_EXT_FIXED_DISK_ACCESS           (1 << 0)
// /*AFLA*/ #define EDD_EXT_DEVICE_LOCKING_AND_EJECTING (1 << 1)
// /*AFLA*/ #define EDD_EXT_ENHANCED_DISK_DRIVE_SUPPORT (1 << 2)
// /*AFLA*/ #define EDD_EXT_64BIT_EXTENSIONS            (1 << 3)
// /*AFLA*/ 
// /*AFLA*/ #define EDD_INFO_DMA_BOUNDARY_ERROR_TRANSPARENT (1 << 0)
// /*AFLA*/ #define EDD_INFO_GEOMETRY_VALID                (1 << 1)
// /*AFLA*/ #define EDD_INFO_REMOVABLE                     (1 << 2)
// /*AFLA*/ #define EDD_INFO_WRITE_VERIFY                  (1 << 3)
// /*AFLA*/ #define EDD_INFO_MEDIA_CHANGE_NOTIFICATION     (1 << 4)
// /*AFLA*/ #define EDD_INFO_LOCKABLE                      (1 << 5)
// /*AFLA*/ #define EDD_INFO_NO_MEDIA_PRESENT              (1 << 6)
// /*AFLA*/ #define EDD_INFO_USE_INT13_FN50                (1 << 7)
// /*AFLA*/ 
// /*AFLA*/ struct edd_device_params {
// /*AFLA*/ 	__u16 length;
// /*AFLA*/ 	__u16 info_flags;
// /*AFLA*/ 	__u32 num_default_cylinders;
// /*AFLA*/ 	__u32 num_default_heads;
// /*AFLA*/ 	__u32 sectors_per_track;
// /*AFLA*/ 	__u64 number_of_sectors;
// /*AFLA*/ 	__u16 bytes_per_sector;
// /*AFLA*/ 	__u32 dpte_ptr;		/* 0xFFFFFFFF for our purposes */
// /*AFLA*/ 	__u16 key;		/* = 0xBEDD */
// /*AFLA*/ 	__u8 device_path_info_length;	/* = 44 */
// /*AFLA*/ 	__u8 reserved2;
// /*AFLA*/ 	__u16 reserved3;
// /*AFLA*/ 	__u8 host_bus_type[4];
// /*AFLA*/ 	__u8 interface_type[8];
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u16 base_address;
// /*AFLA*/ 			__u16 reserved1;
// /*AFLA*/ 			__u32 reserved2;
// /*AFLA*/ 		} __attribute__ ((packed)) isa;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u8 bus;
// /*AFLA*/ 			__u8 slot;
// /*AFLA*/ 			__u8 function;
// /*AFLA*/ 			__u8 channel;
// /*AFLA*/ 			__u32 reserved;
// /*AFLA*/ 		} __attribute__ ((packed)) pci;
// /*AFLA*/ 		/* pcix is same as pci */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u64 reserved;
// /*AFLA*/ 		} __attribute__ ((packed)) ibnd;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u64 reserved;
// /*AFLA*/ 		} __attribute__ ((packed)) xprs;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u64 reserved;
// /*AFLA*/ 		} __attribute__ ((packed)) htpt;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u64 reserved;
// /*AFLA*/ 		} __attribute__ ((packed)) unknown;
// /*AFLA*/ 	} interface_path;
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u8 device;
// /*AFLA*/ 			__u8 reserved1;
// /*AFLA*/ 			__u16 reserved2;
// /*AFLA*/ 			__u32 reserved3;
// /*AFLA*/ 			__u64 reserved4;
// /*AFLA*/ 		} __attribute__ ((packed)) ata;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u8 device;
// /*AFLA*/ 			__u8 lun;
// /*AFLA*/ 			__u8 reserved1;
// /*AFLA*/ 			__u8 reserved2;
// /*AFLA*/ 			__u32 reserved3;
// /*AFLA*/ 			__u64 reserved4;
// /*AFLA*/ 		} __attribute__ ((packed)) atapi;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u16 id;
// /*AFLA*/ 			__u64 lun;
// /*AFLA*/ 			__u16 reserved1;
// /*AFLA*/ 			__u32 reserved2;
// /*AFLA*/ 		} __attribute__ ((packed)) scsi;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u64 serial_number;
// /*AFLA*/ 			__u64 reserved;
// /*AFLA*/ 		} __attribute__ ((packed)) usb;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u64 eui;
// /*AFLA*/ 			__u64 reserved;
// /*AFLA*/ 		} __attribute__ ((packed)) i1394;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u64 wwid;
// /*AFLA*/ 			__u64 lun;
// /*AFLA*/ 		} __attribute__ ((packed)) fibre;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u64 identity_tag;
// /*AFLA*/ 			__u64 reserved;
// /*AFLA*/ 		} __attribute__ ((packed)) i2o;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u32 array_number;
// /*AFLA*/ 			__u32 reserved1;
// /*AFLA*/ 			__u64 reserved2;
// /*AFLA*/ 		} __attribute__ ((packed)) raid;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u8 device;
// /*AFLA*/ 			__u8 reserved1;
// /*AFLA*/ 			__u16 reserved2;
// /*AFLA*/ 			__u32 reserved3;
// /*AFLA*/ 			__u64 reserved4;
// /*AFLA*/ 		} __attribute__ ((packed)) sata;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			__u64 reserved1;
// /*AFLA*/ 			__u64 reserved2;
// /*AFLA*/ 		} __attribute__ ((packed)) unknown;
// /*AFLA*/ 	} device_path;
// /*AFLA*/ 	__u8 reserved4;
// /*AFLA*/ 	__u8 checksum;
// /*AFLA*/ } __attribute__ ((packed));
// /*AFLA*/ 
// /*AFLA*/ struct edd_info {
// /*AFLA*/ 	__u8 device;
// /*AFLA*/ 	__u8 version;
// /*AFLA*/ 	__u16 interface_support;
// /*AFLA*/ 	__u16 legacy_max_cylinder;
// /*AFLA*/ 	__u8 legacy_max_head;
// /*AFLA*/ 	__u8 legacy_sectors_per_track;
// /*AFLA*/ 	struct edd_device_params params;
// /*AFLA*/ } __attribute__ ((packed));
// /*AFLA*/ 
// /*AFLA*/ struct edd {
// /*AFLA*/ 	unsigned int mbr_signature[EDD_MBR_SIG_MAX];
// /*AFLA*/ 	struct edd_info edd_info[EDDMAXNR];
// /*AFLA*/ 	unsigned char mbr_signature_nr;
// /*AFLA*/ 	unsigned char edd_info_nr;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif				/*!__ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_EDD_H */
