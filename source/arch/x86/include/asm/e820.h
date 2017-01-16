// /*AFLA*/ #ifndef _ASM_X86_E820_H
// /*AFLA*/ #define _ASM_X86_E820_H
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_EFI
// /*AFLA*/ #include <linux/numa.h>
// /*AFLA*/ #define E820_X_MAX (E820MAX + 3 * MAX_NUMNODES)
// /*AFLA*/ #else	/* ! CONFIG_EFI */
// /*AFLA*/ #define E820_X_MAX E820MAX
// /*AFLA*/ #endif
// /*AFLA*/ #include <uapi/asm/e820.h>
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ /* see comment in arch/x86/kernel/e820.c */
// /*AFLA*/ extern struct e820map *e820;
// /*AFLA*/ extern struct e820map *e820_saved;
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long pci_mem_start;
// /*AFLA*/ extern int e820_any_mapped(u64 start, u64 end, unsigned type);
// /*AFLA*/ extern int e820_all_mapped(u64 start, u64 end, unsigned type);
// /*AFLA*/ extern void e820_add_region(u64 start, u64 size, int type);
// /*AFLA*/ extern void e820_print_map(char *who);
// /*AFLA*/ extern int
// /*AFLA*/ sanitize_e820_map(struct e820entry *biosmap, int max_nr_map, u32 *pnr_map);
// /*AFLA*/ extern u64 e820_update_range(u64 start, u64 size, unsigned old_type,
// /*AFLA*/ 			       unsigned new_type);
// /*AFLA*/ extern u64 e820_remove_range(u64 start, u64 size, unsigned old_type,
// /*AFLA*/ 			     int checktype);
// /*AFLA*/ extern void update_e820(void);
// /*AFLA*/ extern void e820_setup_gap(void);
// /*AFLA*/ extern int e820_search_gap(unsigned long *gapstart, unsigned long *gapsize,
// /*AFLA*/ 			unsigned long start_addr, unsigned long long end_addr);
// /*AFLA*/ struct setup_data;
// /*AFLA*/ extern void parse_e820_ext(u64 phys_addr, u32 data_len);
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_X86_64) || \
// /*AFLA*/ 	(defined(CONFIG_X86_32) && defined(CONFIG_HIBERNATION))
// /*AFLA*/ extern void e820_mark_nosave_regions(unsigned long limit_pfn);
// /*AFLA*/ #else
// /*AFLA*/ static inline void e820_mark_nosave_regions(unsigned long limit_pfn)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long e820_end_of_ram_pfn(void);
// /*AFLA*/ extern unsigned long e820_end_of_low_ram_pfn(void);
// /*AFLA*/ extern u64 early_reserve_e820(u64 sizet, u64 align);
// /*AFLA*/ 
// /*AFLA*/ void memblock_x86_fill(void);
// /*AFLA*/ void memblock_find_dma_reserve(void);
// /*AFLA*/ 
// /*AFLA*/ extern void finish_e820_parsing(void);
// /*AFLA*/ extern void e820_reserve_resources(void);
// /*AFLA*/ extern void e820_reserve_resources_late(void);
// /*AFLA*/ extern void setup_memory_map(void);
// /*AFLA*/ extern char *default_machine_specific_memory_setup(void);
// /*AFLA*/ 
// /*AFLA*/ extern void e820_reallocate_tables(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Returns true iff the specified range [s,e) is completely contained inside
// /*AFLA*/  * the ISA region.
// /*AFLA*/  */
// /*AFLA*/ static inline bool is_ISA_range(u64 s, u64 e)
// /*AFLA*/ {
// /*AFLA*/ 	return s >= ISA_START_ADDRESS && e <= ISA_END_ADDRESS;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ #include <linux/ioport.h>
// /*AFLA*/ 
// /*AFLA*/ #define HIGH_MEMORY	(1024*1024)
// /*AFLA*/ #endif /* _ASM_X86_E820_H */
