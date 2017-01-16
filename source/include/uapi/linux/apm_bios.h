// /*AFLA*/ /*
// /*AFLA*/  * Include file for the interface to an APM BIOS
// /*AFLA*/  * Copyright 1994-2001 Stephen Rothwell (sfr@canb.auug.org.au)
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify it
// /*AFLA*/  * under the terms of the GNU General Public License as published by the
// /*AFLA*/  * Free Software Foundation; either version 2, or (at your option) any
// /*AFLA*/  * later version.
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it will be useful, but
// /*AFLA*/  * WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// /*AFLA*/  * General Public License for more details.
// /*AFLA*/  */
// /*AFLA*/ #ifndef _UAPI_LINUX_APM_H
// /*AFLA*/ #define _UAPI_LINUX_APM_H
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ typedef unsigned short	apm_event_t;
// /*AFLA*/ typedef unsigned short	apm_eventinfo_t;
// /*AFLA*/ 
// /*AFLA*/ struct apm_bios_info {
// /*AFLA*/ 	__u16	version;
// /*AFLA*/ 	__u16	cseg;
// /*AFLA*/ 	__u32	offset;
// /*AFLA*/ 	__u16	cseg_16;
// /*AFLA*/ 	__u16	dseg;
// /*AFLA*/ 	__u16	flags;
// /*AFLA*/ 	__u16	cseg_len;
// /*AFLA*/ 	__u16	cseg_16_len;
// /*AFLA*/ 	__u16	dseg_len;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Power states
// /*AFLA*/  */
// /*AFLA*/ #define APM_STATE_READY		0x0000
// /*AFLA*/ #define APM_STATE_STANDBY	0x0001
// /*AFLA*/ #define APM_STATE_SUSPEND	0x0002
// /*AFLA*/ #define APM_STATE_OFF		0x0003
// /*AFLA*/ #define APM_STATE_BUSY		0x0004
// /*AFLA*/ #define APM_STATE_REJECT	0x0005
// /*AFLA*/ #define APM_STATE_OEM_SYS	0x0020
// /*AFLA*/ #define APM_STATE_OEM_DEV	0x0040
// /*AFLA*/ 
// /*AFLA*/ #define APM_STATE_DISABLE	0x0000
// /*AFLA*/ #define APM_STATE_ENABLE	0x0001
// /*AFLA*/ 
// /*AFLA*/ #define APM_STATE_DISENGAGE	0x0000
// /*AFLA*/ #define APM_STATE_ENGAGE	0x0001
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Events (results of Get PM Event)
// /*AFLA*/  */
// /*AFLA*/ #define APM_SYS_STANDBY		0x0001
// /*AFLA*/ #define APM_SYS_SUSPEND		0x0002
// /*AFLA*/ #define APM_NORMAL_RESUME	0x0003
// /*AFLA*/ #define APM_CRITICAL_RESUME	0x0004
// /*AFLA*/ #define APM_LOW_BATTERY		0x0005
// /*AFLA*/ #define APM_POWER_STATUS_CHANGE	0x0006
// /*AFLA*/ #define APM_UPDATE_TIME		0x0007
// /*AFLA*/ #define APM_CRITICAL_SUSPEND	0x0008
// /*AFLA*/ #define APM_USER_STANDBY	0x0009
// /*AFLA*/ #define APM_USER_SUSPEND	0x000a
// /*AFLA*/ #define APM_STANDBY_RESUME	0x000b
// /*AFLA*/ #define APM_CAPABILITY_CHANGE	0x000c
// /*AFLA*/ #define APM_USER_HIBERNATION	0x000d
// /*AFLA*/ #define APM_HIBERNATION_RESUME	0x000e
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Error codes
// /*AFLA*/  */
// /*AFLA*/ #define APM_SUCCESS		0x00
// /*AFLA*/ #define APM_DISABLED		0x01
// /*AFLA*/ #define APM_CONNECTED		0x02
// /*AFLA*/ #define APM_NOT_CONNECTED	0x03
// /*AFLA*/ #define APM_16_CONNECTED	0x05
// /*AFLA*/ #define APM_16_UNSUPPORTED	0x06
// /*AFLA*/ #define APM_32_CONNECTED	0x07
// /*AFLA*/ #define APM_32_UNSUPPORTED	0x08
// /*AFLA*/ #define APM_BAD_DEVICE		0x09
// /*AFLA*/ #define APM_BAD_PARAM		0x0a
// /*AFLA*/ #define APM_NOT_ENGAGED		0x0b
// /*AFLA*/ #define APM_BAD_FUNCTION	0x0c
// /*AFLA*/ #define APM_RESUME_DISABLED	0x0d
// /*AFLA*/ #define APM_NO_ERROR		0x53
// /*AFLA*/ #define APM_BAD_STATE		0x60
// /*AFLA*/ #define APM_NO_EVENTS		0x80
// /*AFLA*/ #define APM_NOT_PRESENT		0x86
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * APM Device IDs
// /*AFLA*/  */
// /*AFLA*/ #define APM_DEVICE_BIOS		0x0000
// /*AFLA*/ #define APM_DEVICE_ALL		0x0001
// /*AFLA*/ #define APM_DEVICE_DISPLAY	0x0100
// /*AFLA*/ #define APM_DEVICE_STORAGE	0x0200
// /*AFLA*/ #define APM_DEVICE_PARALLEL	0x0300
// /*AFLA*/ #define APM_DEVICE_SERIAL	0x0400
// /*AFLA*/ #define APM_DEVICE_NETWORK	0x0500
// /*AFLA*/ #define APM_DEVICE_PCMCIA	0x0600
// /*AFLA*/ #define APM_DEVICE_BATTERY	0x8000
// /*AFLA*/ #define APM_DEVICE_OEM		0xe000
// /*AFLA*/ #define APM_DEVICE_OLD_ALL	0xffff
// /*AFLA*/ #define APM_DEVICE_CLASS	0x00ff
// /*AFLA*/ #define APM_DEVICE_MASK		0xff00
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Battery status
// /*AFLA*/  */
// /*AFLA*/ #define APM_MAX_BATTERIES	2
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * APM defined capability bit flags
// /*AFLA*/  */
// /*AFLA*/ #define APM_CAP_GLOBAL_STANDBY		0x0001
// /*AFLA*/ #define APM_CAP_GLOBAL_SUSPEND		0x0002
// /*AFLA*/ #define APM_CAP_RESUME_STANDBY_TIMER	0x0004 /* Timer resume from standby */
// /*AFLA*/ #define APM_CAP_RESUME_SUSPEND_TIMER	0x0008 /* Timer resume from suspend */
// /*AFLA*/ #define APM_CAP_RESUME_STANDBY_RING	0x0010 /* Resume on Ring fr standby */
// /*AFLA*/ #define APM_CAP_RESUME_SUSPEND_RING	0x0020 /* Resume on Ring fr suspend */
// /*AFLA*/ #define APM_CAP_RESUME_STANDBY_PCMCIA	0x0040 /* Resume on PCMCIA Ring	*/
// /*AFLA*/ #define APM_CAP_RESUME_SUSPEND_PCMCIA	0x0080 /* Resume on PCMCIA Ring	*/
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * ioctl operations
// /*AFLA*/  */
// /*AFLA*/ #include <linux/ioctl.h>
// /*AFLA*/ 
// /*AFLA*/ #define APM_IOC_STANDBY		_IO('A', 1)
// /*AFLA*/ #define APM_IOC_SUSPEND		_IO('A', 2)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_APM_H */
