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
// /*AFLA*/ #ifndef _LINUX_APM_H
// /*AFLA*/ #define _LINUX_APM_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/apm_bios.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define APM_CS		(GDT_ENTRY_APMBIOS_BASE * 8)
// /*AFLA*/ #define APM_CS_16	(APM_CS + 8)
// /*AFLA*/ #define APM_DS		(APM_CS_16 + 8)
// /*AFLA*/ 
// /*AFLA*/ /* Results of APM Installation Check */
// /*AFLA*/ #define APM_16_BIT_SUPPORT	0x0001
// /*AFLA*/ #define APM_32_BIT_SUPPORT	0x0002
// /*AFLA*/ #define APM_IDLE_SLOWS_CLOCK	0x0004
// /*AFLA*/ #define APM_BIOS_DISABLED      	0x0008
// /*AFLA*/ #define APM_BIOS_DISENGAGED     0x0010
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Data for APM that is persistent across module unload/load
// /*AFLA*/  */
// /*AFLA*/ struct apm_info {
// /*AFLA*/ 	struct apm_bios_info	bios;
// /*AFLA*/ 	unsigned short		connection_version;
// /*AFLA*/ 	int			get_power_status_broken;
// /*AFLA*/ 	int			get_power_status_swabinminutes;
// /*AFLA*/ 	int			allow_ints;
// /*AFLA*/ 	int			forbid_idle;
// /*AFLA*/ 	int			realmode_power_off;
// /*AFLA*/ 	int			disabled;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The APM function codes
// /*AFLA*/  */
// /*AFLA*/ #define	APM_FUNC_INST_CHECK	0x5300
// /*AFLA*/ #define	APM_FUNC_REAL_CONN	0x5301
// /*AFLA*/ #define	APM_FUNC_16BIT_CONN	0x5302
// /*AFLA*/ #define	APM_FUNC_32BIT_CONN	0x5303
// /*AFLA*/ #define	APM_FUNC_DISCONN	0x5304
// /*AFLA*/ #define	APM_FUNC_IDLE		0x5305
// /*AFLA*/ #define	APM_FUNC_BUSY		0x5306
// /*AFLA*/ #define	APM_FUNC_SET_STATE	0x5307
// /*AFLA*/ #define	APM_FUNC_ENABLE_PM	0x5308
// /*AFLA*/ #define	APM_FUNC_RESTORE_BIOS	0x5309
// /*AFLA*/ #define	APM_FUNC_GET_STATUS	0x530a
// /*AFLA*/ #define	APM_FUNC_GET_EVENT	0x530b
// /*AFLA*/ #define	APM_FUNC_GET_STATE	0x530c
// /*AFLA*/ #define	APM_FUNC_ENABLE_DEV_PM	0x530d
// /*AFLA*/ #define	APM_FUNC_VERSION	0x530e
// /*AFLA*/ #define	APM_FUNC_ENGAGE_PM	0x530f
// /*AFLA*/ #define	APM_FUNC_GET_CAP	0x5310
// /*AFLA*/ #define	APM_FUNC_RESUME_TIMER	0x5311
// /*AFLA*/ #define	APM_FUNC_RESUME_ON_RING	0x5312
// /*AFLA*/ #define	APM_FUNC_TIMER		0x5313
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Function code for APM_FUNC_RESUME_TIMER
// /*AFLA*/  */
// /*AFLA*/ #define	APM_FUNC_DISABLE_TIMER	0
// /*AFLA*/ #define	APM_FUNC_GET_TIMER	1
// /*AFLA*/ #define	APM_FUNC_SET_TIMER	2
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Function code for APM_FUNC_RESUME_ON_RING
// /*AFLA*/  */
// /*AFLA*/ #define	APM_FUNC_DISABLE_RING	0
// /*AFLA*/ #define	APM_FUNC_ENABLE_RING	1
// /*AFLA*/ #define	APM_FUNC_GET_RING	2
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Function code for APM_FUNC_TIMER_STATUS
// /*AFLA*/  */
// /*AFLA*/ #define	APM_FUNC_TIMER_DISABLE	0
// /*AFLA*/ #define	APM_FUNC_TIMER_ENABLE	1
// /*AFLA*/ #define	APM_FUNC_TIMER_GET	2
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * in arch/i386/kernel/setup.c
// /*AFLA*/  */
// /*AFLA*/ extern struct apm_info	apm_info;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is the "All Devices" ID communicated to the BIOS
// /*AFLA*/  */
// /*AFLA*/ #define APM_DEVICE_BALL		((apm_info.connection_version > 0x0100) ? \
// /*AFLA*/ 				 APM_DEVICE_ALL : APM_DEVICE_OLD_ALL)
// /*AFLA*/ #endif	/* LINUX_APM_H */
