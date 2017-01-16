// /*AFLA*/ #ifndef _ASM_UPROBES_H
// /*AFLA*/ #define _ASM_UPROBES_H
// /*AFLA*/ /*
// /*AFLA*/  * User-space Probes (UProbes) for x86
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify
// /*AFLA*/  * it under the terms of the GNU General Public License as published by
// /*AFLA*/  * the Free Software Foundation; either version 2 of the License, or
// /*AFLA*/  * (at your option) any later version.
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it will be useful,
// /*AFLA*/  * but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/  * GNU General Public License for more details.
// /*AFLA*/  *
// /*AFLA*/  * You should have received a copy of the GNU General Public License
// /*AFLA*/  * along with this program; if not, write to the Free Software
// /*AFLA*/  * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) IBM Corporation, 2008-2011
// /*AFLA*/  * Authors:
// /*AFLA*/  *	Srikar Dronamraju
// /*AFLA*/  *	Jim Keniston
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/notifier.h>
// /*AFLA*/ 
// /*AFLA*/ typedef u8 uprobe_opcode_t;
// /*AFLA*/ 
// /*AFLA*/ #define MAX_UINSN_BYTES			  16
// /*AFLA*/ #define UPROBE_XOL_SLOT_BYTES		 128	/* to keep it cache aligned */
// /*AFLA*/ 
// /*AFLA*/ #define UPROBE_SWBP_INSN		0xcc
// /*AFLA*/ #define UPROBE_SWBP_INSN_SIZE		   1
// /*AFLA*/ 
// /*AFLA*/ struct uprobe_xol_ops;
// /*AFLA*/ 
// /*AFLA*/ struct arch_uprobe {
// /*AFLA*/ 	union {
// /*AFLA*/ 		u8			insn[MAX_UINSN_BYTES];
// /*AFLA*/ 		u8			ixol[MAX_UINSN_BYTES];
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ 	const struct uprobe_xol_ops	*ops;
// /*AFLA*/ 
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct {
// /*AFLA*/ 			s32	offs;
// /*AFLA*/ 			u8	ilen;
// /*AFLA*/ 			u8	opc1;
// /*AFLA*/ 		}			branch;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			u8	fixups;
// /*AFLA*/ 			u8	ilen;
// /*AFLA*/ 		} 			defparam;
// /*AFLA*/ 	};
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct arch_uprobe_task {
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ 	unsigned long			saved_scratch_register;
// /*AFLA*/ #endif
// /*AFLA*/ 	unsigned int			saved_trap_nr;
// /*AFLA*/ 	unsigned int			saved_tf;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif	/* _ASM_UPROBES_H */
