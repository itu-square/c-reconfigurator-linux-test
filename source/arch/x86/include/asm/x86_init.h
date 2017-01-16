// /*AFLA*/ #ifndef _ASM_X86_PLATFORM_H
// /*AFLA*/ #define _ASM_X86_PLATFORM_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/bootparam.h>
// /*AFLA*/ 
// /*AFLA*/ struct mpc_bus;
// /*AFLA*/ struct mpc_cpu;
// /*AFLA*/ struct mpc_table;
// /*AFLA*/ struct cpuinfo_x86;
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct x86_init_mpparse - platform specific mpparse ops
// /*AFLA*/  * @mpc_record:			platform specific mpc record accounting
// /*AFLA*/  * @setup_ioapic_ids:		platform specific ioapic id override
// /*AFLA*/  * @mpc_apic_id:		platform specific mpc apic id assignment
// /*AFLA*/  * @smp_read_mpc_oem:		platform specific oem mpc table setup
// /*AFLA*/  * @mpc_oem_pci_bus:		platform specific pci bus setup (default NULL)
// /*AFLA*/  * @mpc_oem_bus_info:		platform specific mpc bus info
// /*AFLA*/  * @find_smp_config:		find the smp configuration
// /*AFLA*/  * @get_smp_config:		get the smp configuration
// /*AFLA*/  */
// /*AFLA*/ struct x86_init_mpparse {
// /*AFLA*/ 	void (*mpc_record)(unsigned int mode);
// /*AFLA*/ 	void (*setup_ioapic_ids)(void);
// /*AFLA*/ 	int (*mpc_apic_id)(struct mpc_cpu *m);
// /*AFLA*/ 	void (*smp_read_mpc_oem)(struct mpc_table *mpc);
// /*AFLA*/ 	void (*mpc_oem_pci_bus)(struct mpc_bus *m);
// /*AFLA*/ 	void (*mpc_oem_bus_info)(struct mpc_bus *m, char *name);
// /*AFLA*/ 	void (*find_smp_config)(void);
// /*AFLA*/ 	void (*get_smp_config)(unsigned int early);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct x86_init_resources - platform specific resource related ops
// /*AFLA*/  * @probe_roms:			probe BIOS roms
// /*AFLA*/  * @reserve_resources:		reserve the standard resources for the
// /*AFLA*/  *				platform
// /*AFLA*/  * @memory_setup:		platform specific memory setup
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ struct x86_init_resources {
// /*AFLA*/ 	void (*probe_roms)(void);
// /*AFLA*/ 	void (*reserve_resources)(void);
// /*AFLA*/ 	char *(*memory_setup)(void);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct x86_init_irqs - platform specific interrupt setup
// /*AFLA*/  * @pre_vector_init:		init code to run before interrupt vectors
// /*AFLA*/  *				are set up.
// /*AFLA*/  * @intr_init:			interrupt init code
// /*AFLA*/  * @trap_init:			platform specific trap setup
// /*AFLA*/  */
// /*AFLA*/ struct x86_init_irqs {
// /*AFLA*/ 	void (*pre_vector_init)(void);
// /*AFLA*/ 	void (*intr_init)(void);
// /*AFLA*/ 	void (*trap_init)(void);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct x86_init_oem - oem platform specific customizing functions
// /*AFLA*/  * @arch_setup:			platform specific architecure setup
// /*AFLA*/  * @banner:			print a platform specific banner
// /*AFLA*/  */
// /*AFLA*/ struct x86_init_oem {
// /*AFLA*/ 	void (*arch_setup)(void);
// /*AFLA*/ 	void (*banner)(void);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct x86_init_paging - platform specific paging functions
// /*AFLA*/  * @pagetable_init:	platform specific paging initialization call to setup
// /*AFLA*/  *			the kernel pagetables and prepare accessors functions.
// /*AFLA*/  *			Callback must call paging_init(). Called once after the
// /*AFLA*/  *			direct mapping for phys memory is available.
// /*AFLA*/  */
// /*AFLA*/ struct x86_init_paging {
// /*AFLA*/ 	void (*pagetable_init)(void);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct x86_init_timers - platform specific timer setup
// /*AFLA*/  * @setup_perpcu_clockev:	set up the per cpu clock event device for the
// /*AFLA*/  *				boot cpu
// /*AFLA*/  * @timer_init:			initialize the platform timer (default PIT/HPET)
// /*AFLA*/  * @wallclock_init:		init the wallclock device
// /*AFLA*/  */
// /*AFLA*/ struct x86_init_timers {
// /*AFLA*/ 	void (*setup_percpu_clockev)(void);
// /*AFLA*/ 	void (*timer_init)(void);
// /*AFLA*/ 	void (*wallclock_init)(void);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct x86_init_iommu - platform specific iommu setup
// /*AFLA*/  * @iommu_init:			platform specific iommu setup
// /*AFLA*/  */
// /*AFLA*/ struct x86_init_iommu {
// /*AFLA*/ 	int (*iommu_init)(void);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct x86_init_pci - platform specific pci init functions
// /*AFLA*/  * @arch_init:			platform specific pci arch init call
// /*AFLA*/  * @init:			platform specific pci subsystem init
// /*AFLA*/  * @init_irq:			platform specific pci irq init
// /*AFLA*/  * @fixup_irqs:			platform specific pci irq fixup
// /*AFLA*/  */
// /*AFLA*/ struct x86_init_pci {
// /*AFLA*/ 	int (*arch_init)(void);
// /*AFLA*/ 	int (*init)(void);
// /*AFLA*/ 	void (*init_irq)(void);
// /*AFLA*/ 	void (*fixup_irqs)(void);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct x86_init_ops - functions for platform specific setup
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ struct x86_init_ops {
// /*AFLA*/ 	struct x86_init_resources	resources;
// /*AFLA*/ 	struct x86_init_mpparse		mpparse;
// /*AFLA*/ 	struct x86_init_irqs		irqs;
// /*AFLA*/ 	struct x86_init_oem		oem;
// /*AFLA*/ 	struct x86_init_paging		paging;
// /*AFLA*/ 	struct x86_init_timers		timers;
// /*AFLA*/ 	struct x86_init_iommu		iommu;
// /*AFLA*/ 	struct x86_init_pci		pci;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct x86_cpuinit_ops - platform specific cpu hotplug setups
// /*AFLA*/  * @setup_percpu_clockev:	set up the per cpu clock event device
// /*AFLA*/  * @early_percpu_clock_init:	early init of the per cpu clock event device
// /*AFLA*/  */
// /*AFLA*/ struct x86_cpuinit_ops {
// /*AFLA*/ 	void (*setup_percpu_clockev)(void);
// /*AFLA*/ 	void (*early_percpu_clock_init)(void);
// /*AFLA*/ 	void (*fixup_cpu_id)(struct cpuinfo_x86 *c, int node);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct timespec;
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct x86_legacy_devices - legacy x86 devices
// /*AFLA*/  *
// /*AFLA*/  * @pnpbios: this platform can have a PNPBIOS. If this is disabled the platform
// /*AFLA*/  * 	is known to never have a PNPBIOS.
// /*AFLA*/  *
// /*AFLA*/  * These are devices known to require LPC or ISA bus. The definition of legacy
// /*AFLA*/  * devices adheres to the ACPI 5.2.9.3 IA-PC Boot Architecture flag
// /*AFLA*/  * ACPI_FADT_LEGACY_DEVICES. These devices consist of user visible devices on
// /*AFLA*/  * the LPC or ISA bus. User visible devices are devices that have end-user
// /*AFLA*/  * accessible connectors (for example, LPT parallel port). Legacy devices on
// /*AFLA*/  * the LPC bus consist for example of serial and parallel ports, PS/2 keyboard
// /*AFLA*/  * / mouse, and the floppy disk controller. A system that lacks all known
// /*AFLA*/  * legacy devices can assume all devices can be detected exclusively via
// /*AFLA*/  * standard device enumeration mechanisms including the ACPI namespace.
// /*AFLA*/  *
// /*AFLA*/  * A system which has does not have ACPI_FADT_LEGACY_DEVICES enabled must not
// /*AFLA*/  * have any of the legacy devices enumerated below present.
// /*AFLA*/  */
// /*AFLA*/ struct x86_legacy_devices {
// /*AFLA*/ 	int pnpbios;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct x86_legacy_features - legacy x86 features
// /*AFLA*/  *
// /*AFLA*/  * @rtc: this device has a CMOS real-time clock present
// /*AFLA*/  * @reserve_bios_regions: boot code will search for the EBDA address and the
// /*AFLA*/  * 	start of the 640k - 1M BIOS region.  If false, the platform must
// /*AFLA*/  * 	ensure that its memory map correctly reserves sub-1MB regions as needed.
// /*AFLA*/  * @devices: legacy x86 devices, refer to struct x86_legacy_devices
// /*AFLA*/  * 	documentation for further details.
// /*AFLA*/  */
// /*AFLA*/ struct x86_legacy_features {
// /*AFLA*/ 	int rtc;
// /*AFLA*/ 	int reserve_bios_regions;
// /*AFLA*/ 	struct x86_legacy_devices devices;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct x86_platform_ops - platform specific runtime functions
// /*AFLA*/  * @calibrate_cpu:		calibrate CPU
// /*AFLA*/  * @calibrate_tsc:		calibrate TSC, if different from CPU
// /*AFLA*/  * @get_wallclock:		get time from HW clock like RTC etc.
// /*AFLA*/  * @set_wallclock:		set time back to HW clock
// /*AFLA*/  * @is_untracked_pat_range	exclude from PAT logic
// /*AFLA*/  * @nmi_init			enable NMI on cpus
// /*AFLA*/  * @i8042_detect		pre-detect if i8042 controller exists
// /*AFLA*/  * @save_sched_clock_state:	save state for sched_clock() on suspend
// /*AFLA*/  * @restore_sched_clock_state:	restore state for sched_clock() on resume
// /*AFLA*/  * @apic_post_init:		adjust apic if neeeded
// /*AFLA*/  * @legacy:			legacy features
// /*AFLA*/  * @set_legacy_features:	override legacy features. Use of this callback
// /*AFLA*/  * 				is highly discouraged. You should only need
// /*AFLA*/  * 				this if your hardware platform requires further
// /*AFLA*/  * 				custom fine tuning far beyong what may be
// /*AFLA*/  * 				possible in x86_early_init_platform_quirks() by
// /*AFLA*/  * 				only using the current x86_hardware_subarch
// /*AFLA*/  * 				semantics.
// /*AFLA*/  */
// /*AFLA*/ struct x86_platform_ops {
// /*AFLA*/ 	unsigned long (*calibrate_cpu)(void);
// /*AFLA*/ 	unsigned long (*calibrate_tsc)(void);
// /*AFLA*/ 	void (*get_wallclock)(struct timespec *ts);
// /*AFLA*/ 	int (*set_wallclock)(const struct timespec *ts);
// /*AFLA*/ 	void (*iommu_shutdown)(void);
// /*AFLA*/ 	bool (*is_untracked_pat_range)(u64 start, u64 end);
// /*AFLA*/ 	void (*nmi_init)(void);
// /*AFLA*/ 	unsigned char (*get_nmi_reason)(void);
// /*AFLA*/ 	int (*i8042_detect)(void);
// /*AFLA*/ 	void (*save_sched_clock_state)(void);
// /*AFLA*/ 	void (*restore_sched_clock_state)(void);
// /*AFLA*/ 	void (*apic_post_init)(void);
// /*AFLA*/ 	struct x86_legacy_features legacy;
// /*AFLA*/ 	void (*set_legacy_features)(void);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct pci_dev;
// /*AFLA*/ 
// /*AFLA*/ struct x86_msi_ops {
// /*AFLA*/ 	int (*setup_msi_irqs)(struct pci_dev *dev, int nvec, int type);
// /*AFLA*/ 	void (*teardown_msi_irq)(unsigned int irq);
// /*AFLA*/ 	void (*teardown_msi_irqs)(struct pci_dev *dev);
// /*AFLA*/ 	void (*restore_msi_irqs)(struct pci_dev *dev);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct x86_io_apic_ops {
// /*AFLA*/ 	unsigned int	(*read)   (unsigned int apic, unsigned int reg);
// /*AFLA*/ 	void		(*disable)(void);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct x86_init_ops x86_init;
// /*AFLA*/ extern struct x86_cpuinit_ops x86_cpuinit;
// /*AFLA*/ extern struct x86_platform_ops x86_platform;
// /*AFLA*/ extern struct x86_msi_ops x86_msi;
// /*AFLA*/ extern struct x86_io_apic_ops x86_io_apic_ops;
// /*AFLA*/ 
// /*AFLA*/ extern void x86_early_init_platform_quirks(void);
// /*AFLA*/ extern void x86_init_noop(void);
// /*AFLA*/ extern void x86_init_uint_noop(unsigned int unused);
// /*AFLA*/ 
// /*AFLA*/ #endif
