// /*AFLA*/ #ifndef _ASM_X86_MPSPEC_H
// /*AFLA*/ #define _ASM_X86_MPSPEC_H
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #include <asm/mpspec_def.h>
// /*AFLA*/ #include <asm/x86_init.h>
// /*AFLA*/ #include <asm/apicdef.h>
// /*AFLA*/ 
// /*AFLA*/ extern int pic_mode;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Summit or generic (i.e. installer) kernels need lots of bus entries.
// /*AFLA*/  * Maximum 256 PCI busses, plus 1 ISA bus in each of 4 cabinets.
// /*AFLA*/  */
// /*AFLA*/ #if CONFIG_BASE_SMALL == 0
// /*AFLA*/ # define MAX_MP_BUSSES		260
// /*AFLA*/ #else
// /*AFLA*/ # define MAX_MP_BUSSES		32
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define MAX_IRQ_SOURCES		256
// /*AFLA*/ 
// /*AFLA*/ extern unsigned int def_to_bigsmp;
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_X86_64: */
// /*AFLA*/ 
// /*AFLA*/ #define MAX_MP_BUSSES		256
// /*AFLA*/ /* Each PCI slot may be a combo card with its own bus.  4 IRQ pins per slot. */
// /*AFLA*/ #define MAX_IRQ_SOURCES		(MAX_MP_BUSSES * 4)
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_X86_64 */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_EISA
// /*AFLA*/ extern int mp_bus_id_to_type[MAX_MP_BUSSES];
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern DECLARE_BITMAP(mp_bus_not_pci, MAX_MP_BUSSES);
// /*AFLA*/ 
// /*AFLA*/ extern unsigned int boot_cpu_physical_apicid;
// /*AFLA*/ extern u8 boot_cpu_apic_version;
// /*AFLA*/ extern unsigned long mp_lapic_addr;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_LOCAL_APIC
// /*AFLA*/ extern int smp_found_config;
// /*AFLA*/ #else
// /*AFLA*/ # define smp_found_config 0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline void get_smp_config(void)
// /*AFLA*/ {
// /*AFLA*/ 	x86_init.mpparse.get_smp_config(0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void early_get_smp_config(void)
// /*AFLA*/ {
// /*AFLA*/ 	x86_init.mpparse.get_smp_config(1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void find_smp_config(void)
// /*AFLA*/ {
// /*AFLA*/ 	x86_init.mpparse.find_smp_config();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_MPPARSE
// /*AFLA*/ extern void early_reserve_e820_mpc_new(void);
// /*AFLA*/ extern int enable_update_mptable;
// /*AFLA*/ extern int default_mpc_apic_id(struct mpc_cpu *m);
// /*AFLA*/ extern void default_smp_read_mpc_oem(struct mpc_table *mpc);
// /*AFLA*/ # ifdef CONFIG_X86_IO_APIC
// /*AFLA*/ extern void default_mpc_oem_bus_info(struct mpc_bus *m, char *str);
// /*AFLA*/ # else
// /*AFLA*/ #  define default_mpc_oem_bus_info NULL
// /*AFLA*/ # endif
// /*AFLA*/ extern void default_find_smp_config(void);
// /*AFLA*/ extern void default_get_smp_config(unsigned int early);
// /*AFLA*/ #else
// /*AFLA*/ static inline void early_reserve_e820_mpc_new(void) { }
// /*AFLA*/ #define enable_update_mptable 0
// /*AFLA*/ #define default_mpc_apic_id NULL
// /*AFLA*/ #define default_smp_read_mpc_oem NULL
// /*AFLA*/ #define default_mpc_oem_bus_info NULL
// /*AFLA*/ #define default_find_smp_config x86_init_noop
// /*AFLA*/ #define default_get_smp_config x86_init_uint_noop
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ int generic_processor_info(int apicid, int version);
// /*AFLA*/ int __generic_processor_info(int apicid, int version, bool enabled);
// /*AFLA*/ 
// /*AFLA*/ #define PHYSID_ARRAY_SIZE	BITS_TO_LONGS(MAX_LOCAL_APIC)
// /*AFLA*/ 
// /*AFLA*/ struct physid_mask {
// /*AFLA*/ 	unsigned long mask[PHYSID_ARRAY_SIZE];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ typedef struct physid_mask physid_mask_t;
// /*AFLA*/ 
// /*AFLA*/ #define physid_set(physid, map)			set_bit(physid, (map).mask)
// /*AFLA*/ #define physid_clear(physid, map)		clear_bit(physid, (map).mask)
// /*AFLA*/ #define physid_isset(physid, map)		test_bit(physid, (map).mask)
// /*AFLA*/ #define physid_test_and_set(physid, map)			\
// /*AFLA*/ 	test_and_set_bit(physid, (map).mask)
// /*AFLA*/ 
// /*AFLA*/ #define physids_and(dst, src1, src2)					\
// /*AFLA*/ 	bitmap_and((dst).mask, (src1).mask, (src2).mask, MAX_LOCAL_APIC)
// /*AFLA*/ 
// /*AFLA*/ #define physids_or(dst, src1, src2)					\
// /*AFLA*/ 	bitmap_or((dst).mask, (src1).mask, (src2).mask, MAX_LOCAL_APIC)
// /*AFLA*/ 
// /*AFLA*/ #define physids_clear(map)					\
// /*AFLA*/ 	bitmap_zero((map).mask, MAX_LOCAL_APIC)
// /*AFLA*/ 
// /*AFLA*/ #define physids_complement(dst, src)				\
// /*AFLA*/ 	bitmap_complement((dst).mask, (src).mask, MAX_LOCAL_APIC)
// /*AFLA*/ 
// /*AFLA*/ #define physids_empty(map)					\
// /*AFLA*/ 	bitmap_empty((map).mask, MAX_LOCAL_APIC)
// /*AFLA*/ 
// /*AFLA*/ #define physids_equal(map1, map2)				\
// /*AFLA*/ 	bitmap_equal((map1).mask, (map2).mask, MAX_LOCAL_APIC)
// /*AFLA*/ 
// /*AFLA*/ #define physids_weight(map)					\
// /*AFLA*/ 	bitmap_weight((map).mask, MAX_LOCAL_APIC)
// /*AFLA*/ 
// /*AFLA*/ #define physids_shift_right(d, s, n)				\
// /*AFLA*/ 	bitmap_shift_right((d).mask, (s).mask, n, MAX_LOCAL_APIC)
// /*AFLA*/ 
// /*AFLA*/ #define physids_shift_left(d, s, n)				\
// /*AFLA*/ 	bitmap_shift_left((d).mask, (s).mask, n, MAX_LOCAL_APIC)
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long physids_coerce(physid_mask_t *map)
// /*AFLA*/ {
// /*AFLA*/ 	return map->mask[0];
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void physids_promote(unsigned long physids, physid_mask_t *map)
// /*AFLA*/ {
// /*AFLA*/ 	physids_clear(*map);
// /*AFLA*/ 	map->mask[0] = physids;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void physid_set_mask_of_physid(int physid, physid_mask_t *map)
// /*AFLA*/ {
// /*AFLA*/ 	physids_clear(*map);
// /*AFLA*/ 	physid_set(physid, *map);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define PHYSID_MASK_ALL		{ {[0 ... PHYSID_ARRAY_SIZE-1] = ~0UL} }
// /*AFLA*/ #define PHYSID_MASK_NONE	{ {[0 ... PHYSID_ARRAY_SIZE-1] = 0UL} }
// /*AFLA*/ 
// /*AFLA*/ extern physid_mask_t phys_cpu_present_map;
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_MPSPEC_H */
