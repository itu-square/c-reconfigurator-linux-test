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
// /*AFLA*/ #ifndef _LINUX_EDD_H
// /*AFLA*/ #define _LINUX_EDD_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/edd.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ extern struct edd edd;
// /*AFLA*/ #endif				/*!__ASSEMBLY__ */
// /*AFLA*/ #endif				/* _LINUX_EDD_H */
