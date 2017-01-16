// /*AFLA*/ /* K8 NUMA support */
// /*AFLA*/ /* Copyright 2002,2003 by Andi Kleen, SuSE Labs */
// /*AFLA*/ /* 2.5 Version loosely based on the NUMAQ Code by Pat Gaughen. */
// /*AFLA*/ #ifndef _ASM_X86_MMZONE_64_H
// /*AFLA*/ #define _ASM_X86_MMZONE_64_H
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ 
// /*AFLA*/ #include <linux/mmdebug.h>
// /*AFLA*/ #include <asm/smp.h>
// /*AFLA*/ 
// /*AFLA*/ extern struct pglist_data *node_data[];
// /*AFLA*/ 
// /*AFLA*/ #define NODE_DATA(nid)		(node_data[nid])
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* _ASM_X86_MMZONE_64_H */
