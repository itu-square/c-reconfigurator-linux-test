// /*AFLA*/ /*
// /*AFLA*/  * Include file for the interface to IST BIOS
// /*AFLA*/  * Copyright 2002 Andy Grover <andrew.grover@intel.com>
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
// /*AFLA*/ #ifndef _UAPI_ASM_X86_IST_H
// /*AFLA*/ #define _UAPI_ASM_X86_IST_H
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ struct ist_info {
// /*AFLA*/ 	__u32 signature;
// /*AFLA*/ 	__u32 command;
// /*AFLA*/ 	__u32 event;
// /*AFLA*/ 	__u32 perf_level;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_ASM_X86_IST_H */
