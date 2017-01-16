// /*AFLA*/ #ifndef _LINUX_POISON_H
// /*AFLA*/ #define _LINUX_POISON_H
// /*AFLA*/ 
// /*AFLA*/ /********** include/linux/list.h **********/
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Architectures might want to move the poison pointer offset
// /*AFLA*/  * into some well-recognized area such as 0xdead000000000000,
// /*AFLA*/  * that is also not mappable by user-space exploits:
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_ILLEGAL_POINTER_VALUE
// /*AFLA*/ # define POISON_POINTER_DELTA _AC(CONFIG_ILLEGAL_POINTER_VALUE, UL)
// /*AFLA*/ #else
// /*AFLA*/ # define POISON_POINTER_DELTA 0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These are non-NULL pointers that will result in page faults
// /*AFLA*/  * under normal circumstances, used to verify that nobody uses
// /*AFLA*/  * non-initialized list entries.
// /*AFLA*/  */
// /*AFLA*/ #define LIST_POISON1  ((void *) 0x100 + POISON_POINTER_DELTA)
// /*AFLA*/ #define LIST_POISON2  ((void *) 0x200 + POISON_POINTER_DELTA)
// /*AFLA*/ 
// /*AFLA*/ /********** include/linux/timer.h **********/
// /*AFLA*/ /*
// /*AFLA*/  * Magic number "tsta" to indicate a static timer initializer
// /*AFLA*/  * for the object debugging code.
// /*AFLA*/  */
// /*AFLA*/ #define TIMER_ENTRY_STATIC	((void *) 0x300 + POISON_POINTER_DELTA)
// /*AFLA*/ 
// /*AFLA*/ /********** mm/debug-pagealloc.c **********/
// /*AFLA*/ #ifdef CONFIG_PAGE_POISONING_ZERO
// /*AFLA*/ #define PAGE_POISON 0x00
// /*AFLA*/ #else
// /*AFLA*/ #define PAGE_POISON 0xaa
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /********** mm/page_alloc.c ************/
// /*AFLA*/ 
// /*AFLA*/ #define TAIL_MAPPING	((void *) 0x400 + POISON_POINTER_DELTA)
// /*AFLA*/ 
// /*AFLA*/ /********** mm/slab.c **********/
// /*AFLA*/ /*
// /*AFLA*/  * Magic nums for obj red zoning.
// /*AFLA*/  * Placed in the first word before and the first word after an obj.
// /*AFLA*/  */
// /*AFLA*/ #define	RED_INACTIVE	0x09F911029D74E35BULL	/* when obj is inactive */
// /*AFLA*/ #define	RED_ACTIVE	0xD84156C5635688C0ULL	/* when obj is active */
// /*AFLA*/ 
// /*AFLA*/ #define SLUB_RED_INACTIVE	0xbb
// /*AFLA*/ #define SLUB_RED_ACTIVE		0xcc
// /*AFLA*/ 
// /*AFLA*/ /* ...and for poisoning */
// /*AFLA*/ #define	POISON_INUSE	0x5a	/* for use-uninitialised poisoning */
// /*AFLA*/ #define POISON_FREE	0x6b	/* for use-after-free poisoning */
// /*AFLA*/ #define	POISON_END	0xa5	/* end-byte of poisoning */
// /*AFLA*/ 
// /*AFLA*/ /********** arch/$ARCH/mm/init.c **********/
// /*AFLA*/ #define POISON_FREE_INITMEM	0xcc
// /*AFLA*/ 
// /*AFLA*/ /********** arch/ia64/hp/common/sba_iommu.c **********/
// /*AFLA*/ /*
// /*AFLA*/  * arch/ia64/hp/common/sba_iommu.c uses a 16-byte poison string with a
// /*AFLA*/  * value of "SBAIOMMU POISON\0" for spill-over poisoning.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /********** fs/jbd/journal.c **********/
// /*AFLA*/ #define JBD_POISON_FREE		0x5b
// /*AFLA*/ #define JBD2_POISON_FREE	0x5c
// /*AFLA*/ 
// /*AFLA*/ /********** drivers/base/dmapool.c **********/
// /*AFLA*/ #define	POOL_POISON_FREED	0xa7	/* !inuse */
// /*AFLA*/ #define	POOL_POISON_ALLOCATED	0xa9	/* !initted */
// /*AFLA*/ 
// /*AFLA*/ /********** drivers/atm/ **********/
// /*AFLA*/ #define ATM_POISON_FREE		0x12
// /*AFLA*/ #define ATM_POISON		0xdeadbeef
// /*AFLA*/ 
// /*AFLA*/ /********** kernel/mutexes **********/
// /*AFLA*/ #define MUTEX_DEBUG_INIT	0x11
// /*AFLA*/ #define MUTEX_DEBUG_FREE	0x22
// /*AFLA*/ 
// /*AFLA*/ /********** lib/flex_array.c **********/
// /*AFLA*/ #define FLEX_ARRAY_FREE	0x6c	/* for use-after-free poisoning */
// /*AFLA*/ 
// /*AFLA*/ /********** security/ **********/
// /*AFLA*/ #define KEY_DESTROY		0xbd
// /*AFLA*/ 
// /*AFLA*/ #endif
