// /*AFLA*/ /*
// /*AFLA*/  * Macros for manipulating and testing flags related to a
// /*AFLA*/  * pageblock_nr_pages number of pages.
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify
// /*AFLA*/  * it under the terms of the GNU General Public License as published by
// /*AFLA*/  * the Free Software Foundation version 2 of the License
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
// /*AFLA*/  * Copyright (C) IBM Corporation, 2006
// /*AFLA*/  *
// /*AFLA*/  * Original author, Mel Gorman
// /*AFLA*/  * Major cleanups and reduction of bit operations, Andy Whitcroft
// /*AFLA*/  */
// /*AFLA*/ #ifndef PAGEBLOCK_FLAGS_H
// /*AFLA*/ #define PAGEBLOCK_FLAGS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /* Bit indices that affect a whole block of pages */
// /*AFLA*/ enum pageblock_bits {
// /*AFLA*/ 	PB_migrate,
// /*AFLA*/ 	PB_migrate_end = PB_migrate + 3 - 1,
// /*AFLA*/ 			/* 3 bits required for migrate types */
// /*AFLA*/ 	PB_migrate_skip,/* If set the block is skipped by compaction */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Assume the bits will always align on a word. If this assumption
// /*AFLA*/ 	 * changes then get/set pageblock needs updating.
// /*AFLA*/ 	 */
// /*AFLA*/ 	NR_PAGEBLOCK_BITS
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HUGETLB_PAGE
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HUGETLB_PAGE_SIZE_VARIABLE
// /*AFLA*/ 
// /*AFLA*/ /* Huge page sizes are variable */
// /*AFLA*/ extern unsigned int pageblock_order;
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_HUGETLB_PAGE_SIZE_VARIABLE */
// /*AFLA*/ 
// /*AFLA*/ /* Huge pages are a constant size */
// /*AFLA*/ #define pageblock_order		HUGETLB_PAGE_ORDER
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_HUGETLB_PAGE_SIZE_VARIABLE */
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_HUGETLB_PAGE */
// /*AFLA*/ 
// /*AFLA*/ /* If huge pages are not used, group by MAX_ORDER_NR_PAGES */
// /*AFLA*/ #define pageblock_order		(MAX_ORDER-1)
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_HUGETLB_PAGE */
// /*AFLA*/ 
// /*AFLA*/ #define pageblock_nr_pages	(1UL << pageblock_order)
// /*AFLA*/ 
// /*AFLA*/ /* Forward declaration */
// /*AFLA*/ struct page;
// /*AFLA*/ 
// /*AFLA*/ unsigned long get_pfnblock_flags_mask(struct page *page,
// /*AFLA*/ 				unsigned long pfn,
// /*AFLA*/ 				unsigned long end_bitidx,
// /*AFLA*/ 				unsigned long mask);
// /*AFLA*/ 
// /*AFLA*/ void set_pfnblock_flags_mask(struct page *page,
// /*AFLA*/ 				unsigned long flags,
// /*AFLA*/ 				unsigned long pfn,
// /*AFLA*/ 				unsigned long end_bitidx,
// /*AFLA*/ 				unsigned long mask);
// /*AFLA*/ 
// /*AFLA*/ /* Declarations for getting and setting flags. See mm/page_alloc.c */
// /*AFLA*/ #define get_pageblock_flags_group(page, start_bitidx, end_bitidx) \
// /*AFLA*/ 	get_pfnblock_flags_mask(page, page_to_pfn(page),		\
// /*AFLA*/ 			end_bitidx,					\
// /*AFLA*/ 			(1 << (end_bitidx - start_bitidx + 1)) - 1)
// /*AFLA*/ #define set_pageblock_flags_group(page, flags, start_bitidx, end_bitidx) \
// /*AFLA*/ 	set_pfnblock_flags_mask(page, flags, page_to_pfn(page),		\
// /*AFLA*/ 			end_bitidx,					\
// /*AFLA*/ 			(1 << (end_bitidx - start_bitidx + 1)) - 1)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_COMPACTION
// /*AFLA*/ #define get_pageblock_skip(page) \
// /*AFLA*/ 			get_pageblock_flags_group(page, PB_migrate_skip,     \
// /*AFLA*/ 							PB_migrate_skip)
// /*AFLA*/ #define clear_pageblock_skip(page) \
// /*AFLA*/ 			set_pageblock_flags_group(page, 0, PB_migrate_skip,  \
// /*AFLA*/ 							PB_migrate_skip)
// /*AFLA*/ #define set_pageblock_skip(page) \
// /*AFLA*/ 			set_pageblock_flags_group(page, 1, PB_migrate_skip,  \
// /*AFLA*/ 							PB_migrate_skip)
// /*AFLA*/ #endif /* CONFIG_COMPACTION */
// /*AFLA*/ 
// /*AFLA*/ #endif	/* PAGEBLOCK_FLAGS_H */
