// /*AFLA*/ #ifndef _UAPI_SCREEN_INFO_H
// /*AFLA*/ #define _UAPI_SCREEN_INFO_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These are set up by the setup-routine at boot-time:
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct screen_info {
// /*AFLA*/ 	__u8  orig_x;		/* 0x00 */
// /*AFLA*/ 	__u8  orig_y;		/* 0x01 */
// /*AFLA*/ 	__u16 ext_mem_k;	/* 0x02 */
// /*AFLA*/ 	__u16 orig_video_page;	/* 0x04 */
// /*AFLA*/ 	__u8  orig_video_mode;	/* 0x06 */
// /*AFLA*/ 	__u8  orig_video_cols;	/* 0x07 */
// /*AFLA*/ 	__u8  flags;		/* 0x08 */
// /*AFLA*/ 	__u8  unused2;		/* 0x09 */
// /*AFLA*/ 	__u16 orig_video_ega_bx;/* 0x0a */
// /*AFLA*/ 	__u16 unused3;		/* 0x0c */
// /*AFLA*/ 	__u8  orig_video_lines;	/* 0x0e */
// /*AFLA*/ 	__u8  orig_video_isVGA;	/* 0x0f */
// /*AFLA*/ 	__u16 orig_video_points;/* 0x10 */
// /*AFLA*/ 
// /*AFLA*/ 	/* VESA graphic mode -- linear frame buffer */
// /*AFLA*/ 	__u16 lfb_width;	/* 0x12 */
// /*AFLA*/ 	__u16 lfb_height;	/* 0x14 */
// /*AFLA*/ 	__u16 lfb_depth;	/* 0x16 */
// /*AFLA*/ 	__u32 lfb_base;		/* 0x18 */
// /*AFLA*/ 	__u32 lfb_size;		/* 0x1c */
// /*AFLA*/ 	__u16 cl_magic, cl_offset; /* 0x20 */
// /*AFLA*/ 	__u16 lfb_linelength;	/* 0x24 */
// /*AFLA*/ 	__u8  red_size;		/* 0x26 */
// /*AFLA*/ 	__u8  red_pos;		/* 0x27 */
// /*AFLA*/ 	__u8  green_size;	/* 0x28 */
// /*AFLA*/ 	__u8  green_pos;	/* 0x29 */
// /*AFLA*/ 	__u8  blue_size;	/* 0x2a */
// /*AFLA*/ 	__u8  blue_pos;		/* 0x2b */
// /*AFLA*/ 	__u8  rsvd_size;	/* 0x2c */
// /*AFLA*/ 	__u8  rsvd_pos;		/* 0x2d */
// /*AFLA*/ 	__u16 vesapm_seg;	/* 0x2e */
// /*AFLA*/ 	__u16 vesapm_off;	/* 0x30 */
// /*AFLA*/ 	__u16 pages;		/* 0x32 */
// /*AFLA*/ 	__u16 vesa_attributes;	/* 0x34 */
// /*AFLA*/ 	__u32 capabilities;     /* 0x36 */
// /*AFLA*/ 	__u32 ext_lfb_base;	/* 0x3a */
// /*AFLA*/ 	__u8  _reserved[2];	/* 0x3e */
// /*AFLA*/ } __attribute__((packed));
// /*AFLA*/ 
// /*AFLA*/ #define VIDEO_TYPE_MDA		0x10	/* Monochrome Text Display	*/
// /*AFLA*/ #define VIDEO_TYPE_CGA		0x11	/* CGA Display 			*/
// /*AFLA*/ #define VIDEO_TYPE_EGAM		0x20	/* EGA/VGA in Monochrome Mode	*/
// /*AFLA*/ #define VIDEO_TYPE_EGAC		0x21	/* EGA in Color Mode		*/
// /*AFLA*/ #define VIDEO_TYPE_VGAC		0x22	/* VGA+ in Color Mode		*/
// /*AFLA*/ #define VIDEO_TYPE_VLFB		0x23	/* VESA VGA in graphic mode	*/
// /*AFLA*/ 
// /*AFLA*/ #define VIDEO_TYPE_PICA_S3	0x30	/* ACER PICA-61 local S3 video	*/
// /*AFLA*/ #define VIDEO_TYPE_MIPS_G364	0x31    /* MIPS Magnum 4000 G364 video  */
// /*AFLA*/ #define VIDEO_TYPE_SGI          0x33    /* Various SGI graphics hardware */
// /*AFLA*/ 
// /*AFLA*/ #define VIDEO_TYPE_TGAC		0x40	/* DEC TGA */
// /*AFLA*/ 
// /*AFLA*/ #define VIDEO_TYPE_SUN          0x50    /* Sun frame buffer. */
// /*AFLA*/ #define VIDEO_TYPE_SUNPCI       0x51    /* Sun PCI based frame buffer. */
// /*AFLA*/ 
// /*AFLA*/ #define VIDEO_TYPE_PMAC		0x60	/* PowerMacintosh frame buffer. */
// /*AFLA*/ 
// /*AFLA*/ #define VIDEO_TYPE_EFI		0x70	/* EFI graphic mode		*/
// /*AFLA*/ 
// /*AFLA*/ #define VIDEO_FLAGS_NOCURSOR	(1 << 0) /* The video mode has no cursor set */
// /*AFLA*/ 
// /*AFLA*/ #define VIDEO_CAPABILITY_SKIP_QUIRKS	(1 << 0)
// /*AFLA*/ #define VIDEO_CAPABILITY_64BIT_BASE	(1 << 1)	/* Frame buffer base is 64-bit */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_SCREEN_INFO_H */
