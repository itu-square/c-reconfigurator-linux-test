// /*AFLA*/ /*
// /*AFLA*/  * device.h - generic, centralized driver model
// /*AFLA*/  *
// /*AFLA*/  * Copyright (c) 2001-2003 Patrick Mochel <mochel@osdl.org>
// /*AFLA*/  * Copyright (c) 2004-2009 Greg Kroah-Hartman <gregkh@suse.de>
// /*AFLA*/  * Copyright (c) 2008-2009 Novell Inc.
// /*AFLA*/  *
// /*AFLA*/  * This file is released under the GPLv2
// /*AFLA*/  *
// /*AFLA*/  * See Documentation/driver-model/ for more information.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _DEVICE_H_
// /*AFLA*/ #define _DEVICE_H_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/ioport.h>
// /*AFLA*/ #include <linux/kobject.h>
// /*AFLA*/ #include <linux/klist.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/lockdep.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/mutex.h>
// /*AFLA*/ #include <linux/pinctrl/devinfo.h>
// /*AFLA*/ #include <linux/pm.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/ratelimit.h>
// /*AFLA*/ #include <linux/uidgid.h>
// /*AFLA*/ #include <linux/gfp.h>
// /*AFLA*/ #include <asm/device.h>
// /*AFLA*/ 
// /*AFLA*/ struct device;
// /*AFLA*/ struct device_private;
// /*AFLA*/ struct device_driver;
// /*AFLA*/ struct driver_private;
// /*AFLA*/ struct module;
// /*AFLA*/ struct class;
// /*AFLA*/ struct subsys_private;
// /*AFLA*/ struct bus_type;
// /*AFLA*/ struct device_node;
// /*AFLA*/ struct fwnode_handle;
// /*AFLA*/ struct iommu_ops;
// /*AFLA*/ struct iommu_group;
// /*AFLA*/ struct iommu_fwspec;
// /*AFLA*/ 
// /*AFLA*/ struct bus_attribute {
// /*AFLA*/ 	struct attribute	attr;
// /*AFLA*/ 	ssize_t (*show)(struct bus_type *bus, char *buf);
// /*AFLA*/ 	ssize_t (*store)(struct bus_type *bus, const char *buf, size_t count);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define BUS_ATTR(_name, _mode, _show, _store)	\
// /*AFLA*/ 	struct bus_attribute bus_attr_##_name = __ATTR(_name, _mode, _show, _store)
// /*AFLA*/ #define BUS_ATTR_RW(_name) \
// /*AFLA*/ 	struct bus_attribute bus_attr_##_name = __ATTR_RW(_name)
// /*AFLA*/ #define BUS_ATTR_RO(_name) \
// /*AFLA*/ 	struct bus_attribute bus_attr_##_name = __ATTR_RO(_name)
// /*AFLA*/ 
// /*AFLA*/ extern int __must_check bus_create_file(struct bus_type *,
// /*AFLA*/ 					struct bus_attribute *);
// /*AFLA*/ extern void bus_remove_file(struct bus_type *, struct bus_attribute *);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct bus_type - The bus type of the device
// /*AFLA*/  *
// /*AFLA*/  * @name:	The name of the bus.
// /*AFLA*/  * @dev_name:	Used for subsystems to enumerate devices like ("foo%u", dev->id).
// /*AFLA*/  * @dev_root:	Default device to use as the parent.
// /*AFLA*/  * @dev_attrs:	Default attributes of the devices on the bus.
// /*AFLA*/  * @bus_groups:	Default attributes of the bus.
// /*AFLA*/  * @dev_groups:	Default attributes of the devices on the bus.
// /*AFLA*/  * @drv_groups: Default attributes of the device drivers on the bus.
// /*AFLA*/  * @match:	Called, perhaps multiple times, whenever a new device or driver
// /*AFLA*/  *		is added for this bus. It should return a positive value if the
// /*AFLA*/  *		given device can be handled by the given driver and zero
// /*AFLA*/  *		otherwise. It may also return error code if determining that
// /*AFLA*/  *		the driver supports the device is not possible. In case of
// /*AFLA*/  *		-EPROBE_DEFER it will queue the device for deferred probing.
// /*AFLA*/  * @uevent:	Called when a device is added, removed, or a few other things
// /*AFLA*/  *		that generate uevents to add the environment variables.
// /*AFLA*/  * @probe:	Called when a new device or driver add to this bus, and callback
// /*AFLA*/  *		the specific driver's probe to initial the matched device.
// /*AFLA*/  * @remove:	Called when a device removed from this bus.
// /*AFLA*/  * @shutdown:	Called at shut-down time to quiesce the device.
// /*AFLA*/  *
// /*AFLA*/  * @online:	Called to put the device back online (after offlining it).
// /*AFLA*/  * @offline:	Called to put the device offline for hot-removal. May fail.
// /*AFLA*/  *
// /*AFLA*/  * @suspend:	Called when a device on this bus wants to go to sleep mode.
// /*AFLA*/  * @resume:	Called to bring a device on this bus out of sleep mode.
// /*AFLA*/  * @pm:		Power management operations of this bus, callback the specific
// /*AFLA*/  *		device driver's pm-ops.
// /*AFLA*/  * @iommu_ops:  IOMMU specific operations for this bus, used to attach IOMMU
// /*AFLA*/  *              driver implementations to a bus and allow the driver to do
// /*AFLA*/  *              bus-specific setup
// /*AFLA*/  * @p:		The private data of the driver core, only the driver core can
// /*AFLA*/  *		touch this.
// /*AFLA*/  * @lock_key:	Lock class key for use by the lock validator
// /*AFLA*/  *
// /*AFLA*/  * A bus is a channel between the processor and one or more devices. For the
// /*AFLA*/  * purposes of the device model, all devices are connected via a bus, even if
// /*AFLA*/  * it is an internal, virtual, "platform" bus. Buses can plug into each other.
// /*AFLA*/  * A USB controller is usually a PCI device, for example. The device model
// /*AFLA*/  * represents the actual connections between buses and the devices they control.
// /*AFLA*/  * A bus is represented by the bus_type structure. It contains the name, the
// /*AFLA*/  * default attributes, the bus' methods, PM operations, and the driver core's
// /*AFLA*/  * private data.
// /*AFLA*/  */
// /*AFLA*/ struct bus_type {
// /*AFLA*/ 	const char		*name;
// /*AFLA*/ 	const char		*dev_name;
// /*AFLA*/ 	struct device		*dev_root;
// /*AFLA*/ 	struct device_attribute	*dev_attrs;	/* use dev_groups instead */
// /*AFLA*/ 	const struct attribute_group **bus_groups;
// /*AFLA*/ 	const struct attribute_group **dev_groups;
// /*AFLA*/ 	const struct attribute_group **drv_groups;
// /*AFLA*/ 
// /*AFLA*/ 	int (*match)(struct device *dev, struct device_driver *drv);
// /*AFLA*/ 	int (*uevent)(struct device *dev, struct kobj_uevent_env *env);
// /*AFLA*/ 	int (*probe)(struct device *dev);
// /*AFLA*/ 	int (*remove)(struct device *dev);
// /*AFLA*/ 	void (*shutdown)(struct device *dev);
// /*AFLA*/ 
// /*AFLA*/ 	int (*online)(struct device *dev);
// /*AFLA*/ 	int (*offline)(struct device *dev);
// /*AFLA*/ 
// /*AFLA*/ 	int (*suspend)(struct device *dev, pm_message_t state);
// /*AFLA*/ 	int (*resume)(struct device *dev);
// /*AFLA*/ 
// /*AFLA*/ 	const struct dev_pm_ops *pm;
// /*AFLA*/ 
// /*AFLA*/ 	const struct iommu_ops *iommu_ops;
// /*AFLA*/ 
// /*AFLA*/ 	struct subsys_private *p;
// /*AFLA*/ 	struct lock_class_key lock_key;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern int __must_check bus_register(struct bus_type *bus);
// /*AFLA*/ 
// /*AFLA*/ extern void bus_unregister(struct bus_type *bus);
// /*AFLA*/ 
// /*AFLA*/ extern int __must_check bus_rescan_devices(struct bus_type *bus);
// /*AFLA*/ 
// /*AFLA*/ /* iterator helpers for buses */
// /*AFLA*/ struct subsys_dev_iter {
// /*AFLA*/ 	struct klist_iter		ki;
// /*AFLA*/ 	const struct device_type	*type;
// /*AFLA*/ };
// /*AFLA*/ void subsys_dev_iter_init(struct subsys_dev_iter *iter,
// /*AFLA*/ 			 struct bus_type *subsys,
// /*AFLA*/ 			 struct device *start,
// /*AFLA*/ 			 const struct device_type *type);
// /*AFLA*/ struct device *subsys_dev_iter_next(struct subsys_dev_iter *iter);
// /*AFLA*/ void subsys_dev_iter_exit(struct subsys_dev_iter *iter);
// /*AFLA*/ 
// /*AFLA*/ int bus_for_each_dev(struct bus_type *bus, struct device *start, void *data,
// /*AFLA*/ 		     int (*fn)(struct device *dev, void *data));
// /*AFLA*/ struct device *bus_find_device(struct bus_type *bus, struct device *start,
// /*AFLA*/ 			       void *data,
// /*AFLA*/ 			       int (*match)(struct device *dev, void *data));
// /*AFLA*/ struct device *bus_find_device_by_name(struct bus_type *bus,
// /*AFLA*/ 				       struct device *start,
// /*AFLA*/ 				       const char *name);
// /*AFLA*/ struct device *subsys_find_device_by_id(struct bus_type *bus, unsigned int id,
// /*AFLA*/ 					struct device *hint);
// /*AFLA*/ int bus_for_each_drv(struct bus_type *bus, struct device_driver *start,
// /*AFLA*/ 		     void *data, int (*fn)(struct device_driver *, void *));
// /*AFLA*/ void bus_sort_breadthfirst(struct bus_type *bus,
// /*AFLA*/ 			   int (*compare)(const struct device *a,
// /*AFLA*/ 					  const struct device *b));
// /*AFLA*/ /*
// /*AFLA*/  * Bus notifiers: Get notified of addition/removal of devices
// /*AFLA*/  * and binding/unbinding of drivers to devices.
// /*AFLA*/  * In the long run, it should be a replacement for the platform
// /*AFLA*/  * notify hooks.
// /*AFLA*/  */
// /*AFLA*/ struct notifier_block;
// /*AFLA*/ 
// /*AFLA*/ extern int bus_register_notifier(struct bus_type *bus,
// /*AFLA*/ 				 struct notifier_block *nb);
// /*AFLA*/ extern int bus_unregister_notifier(struct bus_type *bus,
// /*AFLA*/ 				   struct notifier_block *nb);
// /*AFLA*/ 
// /*AFLA*/ /* All 4 notifers below get called with the target struct device *
// /*AFLA*/  * as an argument. Note that those functions are likely to be called
// /*AFLA*/  * with the device lock held in the core, so be careful.
// /*AFLA*/  */
// /*AFLA*/ #define BUS_NOTIFY_ADD_DEVICE		0x00000001 /* device added */
// /*AFLA*/ #define BUS_NOTIFY_DEL_DEVICE		0x00000002 /* device to be removed */
// /*AFLA*/ #define BUS_NOTIFY_REMOVED_DEVICE	0x00000003 /* device removed */
// /*AFLA*/ #define BUS_NOTIFY_BIND_DRIVER		0x00000004 /* driver about to be
// /*AFLA*/ 						      bound */
// /*AFLA*/ #define BUS_NOTIFY_BOUND_DRIVER		0x00000005 /* driver bound to device */
// /*AFLA*/ #define BUS_NOTIFY_UNBIND_DRIVER	0x00000006 /* driver about to be
// /*AFLA*/ 						      unbound */
// /*AFLA*/ #define BUS_NOTIFY_UNBOUND_DRIVER	0x00000007 /* driver is unbound
// /*AFLA*/ 						      from the device */
// /*AFLA*/ #define BUS_NOTIFY_DRIVER_NOT_BOUND	0x00000008 /* driver fails to be bound */
// /*AFLA*/ 
// /*AFLA*/ extern struct kset *bus_get_kset(struct bus_type *bus);
// /*AFLA*/ extern struct klist *bus_get_device_klist(struct bus_type *bus);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * enum probe_type - device driver probe type to try
// /*AFLA*/  *	Device drivers may opt in for special handling of their
// /*AFLA*/  *	respective probe routines. This tells the core what to
// /*AFLA*/  *	expect and prefer.
// /*AFLA*/  *
// /*AFLA*/  * @PROBE_DEFAULT_STRATEGY: Used by drivers that work equally well
// /*AFLA*/  *	whether probed synchronously or asynchronously.
// /*AFLA*/  * @PROBE_PREFER_ASYNCHRONOUS: Drivers for "slow" devices which
// /*AFLA*/  *	probing order is not essential for booting the system may
// /*AFLA*/  *	opt into executing their probes asynchronously.
// /*AFLA*/  * @PROBE_FORCE_SYNCHRONOUS: Use this to annotate drivers that need
// /*AFLA*/  *	their probe routines to run synchronously with driver and
// /*AFLA*/  *	device registration (with the exception of -EPROBE_DEFER
// /*AFLA*/  *	handling - re-probing always ends up being done asynchronously).
// /*AFLA*/  *
// /*AFLA*/  * Note that the end goal is to switch the kernel to use asynchronous
// /*AFLA*/  * probing by default, so annotating drivers with
// /*AFLA*/  * %PROBE_PREFER_ASYNCHRONOUS is a temporary measure that allows us
// /*AFLA*/  * to speed up boot process while we are validating the rest of the
// /*AFLA*/  * drivers.
// /*AFLA*/  */
// /*AFLA*/ enum probe_type {
// /*AFLA*/ 	PROBE_DEFAULT_STRATEGY,
// /*AFLA*/ 	PROBE_PREFER_ASYNCHRONOUS,
// /*AFLA*/ 	PROBE_FORCE_SYNCHRONOUS,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct device_driver - The basic device driver structure
// /*AFLA*/  * @name:	Name of the device driver.
// /*AFLA*/  * @bus:	The bus which the device of this driver belongs to.
// /*AFLA*/  * @owner:	The module owner.
// /*AFLA*/  * @mod_name:	Used for built-in modules.
// /*AFLA*/  * @suppress_bind_attrs: Disables bind/unbind via sysfs.
// /*AFLA*/  * @probe_type:	Type of the probe (synchronous or asynchronous) to use.
// /*AFLA*/  * @of_match_table: The open firmware table.
// /*AFLA*/  * @acpi_match_table: The ACPI match table.
// /*AFLA*/  * @probe:	Called to query the existence of a specific device,
// /*AFLA*/  *		whether this driver can work with it, and bind the driver
// /*AFLA*/  *		to a specific device.
// /*AFLA*/  * @remove:	Called when the device is removed from the system to
// /*AFLA*/  *		unbind a device from this driver.
// /*AFLA*/  * @shutdown:	Called at shut-down time to quiesce the device.
// /*AFLA*/  * @suspend:	Called to put the device to sleep mode. Usually to a
// /*AFLA*/  *		low power state.
// /*AFLA*/  * @resume:	Called to bring a device from sleep mode.
// /*AFLA*/  * @groups:	Default attributes that get created by the driver core
// /*AFLA*/  *		automatically.
// /*AFLA*/  * @pm:		Power management operations of the device which matched
// /*AFLA*/  *		this driver.
// /*AFLA*/  * @p:		Driver core's private data, no one other than the driver
// /*AFLA*/  *		core can touch this.
// /*AFLA*/  *
// /*AFLA*/  * The device driver-model tracks all of the drivers known to the system.
// /*AFLA*/  * The main reason for this tracking is to enable the driver core to match
// /*AFLA*/  * up drivers with new devices. Once drivers are known objects within the
// /*AFLA*/  * system, however, a number of other things become possible. Device drivers
// /*AFLA*/  * can export information and configuration variables that are independent
// /*AFLA*/  * of any specific device.
// /*AFLA*/  */
// /*AFLA*/ struct device_driver {
// /*AFLA*/ 	const char		*name;
// /*AFLA*/ 	struct bus_type		*bus;
// /*AFLA*/ 
// /*AFLA*/ 	struct module		*owner;
// /*AFLA*/ 	const char		*mod_name;	/* used for built-in modules */
// /*AFLA*/ 
// /*AFLA*/ 	bool suppress_bind_attrs;	/* disables bind/unbind via sysfs */
// /*AFLA*/ 	enum probe_type probe_type;
// /*AFLA*/ 
// /*AFLA*/ 	const struct of_device_id	*of_match_table;
// /*AFLA*/ 	const struct acpi_device_id	*acpi_match_table;
// /*AFLA*/ 
// /*AFLA*/ 	int (*probe) (struct device *dev);
// /*AFLA*/ 	int (*remove) (struct device *dev);
// /*AFLA*/ 	void (*shutdown) (struct device *dev);
// /*AFLA*/ 	int (*suspend) (struct device *dev, pm_message_t state);
// /*AFLA*/ 	int (*resume) (struct device *dev);
// /*AFLA*/ 	const struct attribute_group **groups;
// /*AFLA*/ 
// /*AFLA*/ 	const struct dev_pm_ops *pm;
// /*AFLA*/ 
// /*AFLA*/ 	struct driver_private *p;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ extern int __must_check driver_register(struct device_driver *drv);
// /*AFLA*/ extern void driver_unregister(struct device_driver *drv);
// /*AFLA*/ 
// /*AFLA*/ extern struct device_driver *driver_find(const char *name,
// /*AFLA*/ 					 struct bus_type *bus);
// /*AFLA*/ extern int driver_probe_done(void);
// /*AFLA*/ extern void wait_for_device_probe(void);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* sysfs interface for exporting driver attributes */
// /*AFLA*/ 
// /*AFLA*/ struct driver_attribute {
// /*AFLA*/ 	struct attribute attr;
// /*AFLA*/ 	ssize_t (*show)(struct device_driver *driver, char *buf);
// /*AFLA*/ 	ssize_t (*store)(struct device_driver *driver, const char *buf,
// /*AFLA*/ 			 size_t count);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define DRIVER_ATTR(_name, _mode, _show, _store) \
// /*AFLA*/ 	struct driver_attribute driver_attr_##_name = __ATTR(_name, _mode, _show, _store)
// /*AFLA*/ #define DRIVER_ATTR_RW(_name) \
// /*AFLA*/ 	struct driver_attribute driver_attr_##_name = __ATTR_RW(_name)
// /*AFLA*/ #define DRIVER_ATTR_RO(_name) \
// /*AFLA*/ 	struct driver_attribute driver_attr_##_name = __ATTR_RO(_name)
// /*AFLA*/ #define DRIVER_ATTR_WO(_name) \
// /*AFLA*/ 	struct driver_attribute driver_attr_##_name = __ATTR_WO(_name)
// /*AFLA*/ 
// /*AFLA*/ extern int __must_check driver_create_file(struct device_driver *driver,
// /*AFLA*/ 					const struct driver_attribute *attr);
// /*AFLA*/ extern void driver_remove_file(struct device_driver *driver,
// /*AFLA*/ 			       const struct driver_attribute *attr);
// /*AFLA*/ 
// /*AFLA*/ extern int __must_check driver_for_each_device(struct device_driver *drv,
// /*AFLA*/ 					       struct device *start,
// /*AFLA*/ 					       void *data,
// /*AFLA*/ 					       int (*fn)(struct device *dev,
// /*AFLA*/ 							 void *));
// /*AFLA*/ struct device *driver_find_device(struct device_driver *drv,
// /*AFLA*/ 				  struct device *start, void *data,
// /*AFLA*/ 				  int (*match)(struct device *dev, void *data));
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct subsys_interface - interfaces to device functions
// /*AFLA*/  * @name:       name of the device function
// /*AFLA*/  * @subsys:     subsytem of the devices to attach to
// /*AFLA*/  * @node:       the list of functions registered at the subsystem
// /*AFLA*/  * @add_dev:    device hookup to device function handler
// /*AFLA*/  * @remove_dev: device hookup to device function handler
// /*AFLA*/  *
// /*AFLA*/  * Simple interfaces attached to a subsystem. Multiple interfaces can
// /*AFLA*/  * attach to a subsystem and its devices. Unlike drivers, they do not
// /*AFLA*/  * exclusively claim or control devices. Interfaces usually represent
// /*AFLA*/  * a specific functionality of a subsystem/class of devices.
// /*AFLA*/  */
// /*AFLA*/ struct subsys_interface {
// /*AFLA*/ 	const char *name;
// /*AFLA*/ 	struct bus_type *subsys;
// /*AFLA*/ 	struct list_head node;
// /*AFLA*/ 	int (*add_dev)(struct device *dev, struct subsys_interface *sif);
// /*AFLA*/ 	void (*remove_dev)(struct device *dev, struct subsys_interface *sif);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ int subsys_interface_register(struct subsys_interface *sif);
// /*AFLA*/ void subsys_interface_unregister(struct subsys_interface *sif);
// /*AFLA*/ 
// /*AFLA*/ int subsys_system_register(struct bus_type *subsys,
// /*AFLA*/ 			   const struct attribute_group **groups);
// /*AFLA*/ int subsys_virtual_register(struct bus_type *subsys,
// /*AFLA*/ 			    const struct attribute_group **groups);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct class - device classes
// /*AFLA*/  * @name:	Name of the class.
// /*AFLA*/  * @owner:	The module owner.
// /*AFLA*/  * @class_attrs: Default attributes of this class.
// /*AFLA*/  * @dev_groups:	Default attributes of the devices that belong to the class.
// /*AFLA*/  * @dev_kobj:	The kobject that represents this class and links it into the hierarchy.
// /*AFLA*/  * @dev_uevent:	Called when a device is added, removed from this class, or a
// /*AFLA*/  *		few other things that generate uevents to add the environment
// /*AFLA*/  *		variables.
// /*AFLA*/  * @devnode:	Callback to provide the devtmpfs.
// /*AFLA*/  * @class_release: Called to release this class.
// /*AFLA*/  * @dev_release: Called to release the device.
// /*AFLA*/  * @suspend:	Used to put the device to sleep mode, usually to a low power
// /*AFLA*/  *		state.
// /*AFLA*/  * @resume:	Used to bring the device from the sleep mode.
// /*AFLA*/  * @ns_type:	Callbacks so sysfs can detemine namespaces.
// /*AFLA*/  * @namespace:	Namespace of the device belongs to this class.
// /*AFLA*/  * @pm:		The default device power management operations of this class.
// /*AFLA*/  * @p:		The private data of the driver core, no one other than the
// /*AFLA*/  *		driver core can touch this.
// /*AFLA*/  *
// /*AFLA*/  * A class is a higher-level view of a device that abstracts out low-level
// /*AFLA*/  * implementation details. Drivers may see a SCSI disk or an ATA disk, but,
// /*AFLA*/  * at the class level, they are all simply disks. Classes allow user space
// /*AFLA*/  * to work with devices based on what they do, rather than how they are
// /*AFLA*/  * connected or how they work.
// /*AFLA*/  */
// /*AFLA*/ struct class {
// /*AFLA*/ 	const char		*name;
// /*AFLA*/ 	struct module		*owner;
// /*AFLA*/ 
// /*AFLA*/ 	struct class_attribute		*class_attrs;
// /*AFLA*/ 	const struct attribute_group	**dev_groups;
// /*AFLA*/ 	struct kobject			*dev_kobj;
// /*AFLA*/ 
// /*AFLA*/ 	int (*dev_uevent)(struct device *dev, struct kobj_uevent_env *env);
// /*AFLA*/ 	char *(*devnode)(struct device *dev, umode_t *mode);
// /*AFLA*/ 
// /*AFLA*/ 	void (*class_release)(struct class *class);
// /*AFLA*/ 	void (*dev_release)(struct device *dev);
// /*AFLA*/ 
// /*AFLA*/ 	int (*suspend)(struct device *dev, pm_message_t state);
// /*AFLA*/ 	int (*resume)(struct device *dev);
// /*AFLA*/ 
// /*AFLA*/ 	const struct kobj_ns_type_operations *ns_type;
// /*AFLA*/ 	const void *(*namespace)(struct device *dev);
// /*AFLA*/ 
// /*AFLA*/ 	const struct dev_pm_ops *pm;
// /*AFLA*/ 
// /*AFLA*/ 	struct subsys_private *p;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct class_dev_iter {
// /*AFLA*/ 	struct klist_iter		ki;
// /*AFLA*/ 	const struct device_type	*type;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct kobject *sysfs_dev_block_kobj;
// /*AFLA*/ extern struct kobject *sysfs_dev_char_kobj;
// /*AFLA*/ extern int __must_check __class_register(struct class *class,
// /*AFLA*/ 					 struct lock_class_key *key);
// /*AFLA*/ extern void class_unregister(struct class *class);
// /*AFLA*/ 
// /*AFLA*/ /* This is a #define to keep the compiler from merging different
// /*AFLA*/  * instances of the __key variable */
// /*AFLA*/ #define class_register(class)			\
// /*AFLA*/ ({						\
// /*AFLA*/ 	static struct lock_class_key __key;	\
// /*AFLA*/ 	__class_register(class, &__key);	\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ struct class_compat;
// /*AFLA*/ struct class_compat *class_compat_register(const char *name);
// /*AFLA*/ void class_compat_unregister(struct class_compat *cls);
// /*AFLA*/ int class_compat_create_link(struct class_compat *cls, struct device *dev,
// /*AFLA*/ 			     struct device *device_link);
// /*AFLA*/ void class_compat_remove_link(struct class_compat *cls, struct device *dev,
// /*AFLA*/ 			      struct device *device_link);
// /*AFLA*/ 
// /*AFLA*/ extern void class_dev_iter_init(struct class_dev_iter *iter,
// /*AFLA*/ 				struct class *class,
// /*AFLA*/ 				struct device *start,
// /*AFLA*/ 				const struct device_type *type);
// /*AFLA*/ extern struct device *class_dev_iter_next(struct class_dev_iter *iter);
// /*AFLA*/ extern void class_dev_iter_exit(struct class_dev_iter *iter);
// /*AFLA*/ 
// /*AFLA*/ extern int class_for_each_device(struct class *class, struct device *start,
// /*AFLA*/ 				 void *data,
// /*AFLA*/ 				 int (*fn)(struct device *dev, void *data));
// /*AFLA*/ extern struct device *class_find_device(struct class *class,
// /*AFLA*/ 					struct device *start, const void *data,
// /*AFLA*/ 					int (*match)(struct device *, const void *));
// /*AFLA*/ 
// /*AFLA*/ struct class_attribute {
// /*AFLA*/ 	struct attribute attr;
// /*AFLA*/ 	ssize_t (*show)(struct class *class, struct class_attribute *attr,
// /*AFLA*/ 			char *buf);
// /*AFLA*/ 	ssize_t (*store)(struct class *class, struct class_attribute *attr,
// /*AFLA*/ 			const char *buf, size_t count);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define CLASS_ATTR(_name, _mode, _show, _store) \
// /*AFLA*/ 	struct class_attribute class_attr_##_name = __ATTR(_name, _mode, _show, _store)
// /*AFLA*/ #define CLASS_ATTR_RW(_name) \
// /*AFLA*/ 	struct class_attribute class_attr_##_name = __ATTR_RW(_name)
// /*AFLA*/ #define CLASS_ATTR_RO(_name) \
// /*AFLA*/ 	struct class_attribute class_attr_##_name = __ATTR_RO(_name)
// /*AFLA*/ 
// /*AFLA*/ extern int __must_check class_create_file_ns(struct class *class,
// /*AFLA*/ 					     const struct class_attribute *attr,
// /*AFLA*/ 					     const void *ns);
// /*AFLA*/ extern void class_remove_file_ns(struct class *class,
// /*AFLA*/ 				 const struct class_attribute *attr,
// /*AFLA*/ 				 const void *ns);
// /*AFLA*/ 
// /*AFLA*/ static inline int __must_check class_create_file(struct class *class,
// /*AFLA*/ 					const struct class_attribute *attr)
// /*AFLA*/ {
// /*AFLA*/ 	return class_create_file_ns(class, attr, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void class_remove_file(struct class *class,
// /*AFLA*/ 				     const struct class_attribute *attr)
// /*AFLA*/ {
// /*AFLA*/ 	return class_remove_file_ns(class, attr, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Simple class attribute that is just a static string */
// /*AFLA*/ struct class_attribute_string {
// /*AFLA*/ 	struct class_attribute attr;
// /*AFLA*/ 	char *str;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Currently read-only only */
// /*AFLA*/ #define _CLASS_ATTR_STRING(_name, _mode, _str) \
// /*AFLA*/ 	{ __ATTR(_name, _mode, show_class_attr_string, NULL), _str }
// /*AFLA*/ #define CLASS_ATTR_STRING(_name, _mode, _str) \
// /*AFLA*/ 	struct class_attribute_string class_attr_##_name = \
// /*AFLA*/ 		_CLASS_ATTR_STRING(_name, _mode, _str)
// /*AFLA*/ 
// /*AFLA*/ extern ssize_t show_class_attr_string(struct class *class, struct class_attribute *attr,
// /*AFLA*/                         char *buf);
// /*AFLA*/ 
// /*AFLA*/ struct class_interface {
// /*AFLA*/ 	struct list_head	node;
// /*AFLA*/ 	struct class		*class;
// /*AFLA*/ 
// /*AFLA*/ 	int (*add_dev)		(struct device *, struct class_interface *);
// /*AFLA*/ 	void (*remove_dev)	(struct device *, struct class_interface *);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern int __must_check class_interface_register(struct class_interface *);
// /*AFLA*/ extern void class_interface_unregister(struct class_interface *);
// /*AFLA*/ 
// /*AFLA*/ extern struct class * __must_check __class_create(struct module *owner,
// /*AFLA*/ 						  const char *name,
// /*AFLA*/ 						  struct lock_class_key *key);
// /*AFLA*/ extern void class_destroy(struct class *cls);
// /*AFLA*/ 
// /*AFLA*/ /* This is a #define to keep the compiler from merging different
// /*AFLA*/  * instances of the __key variable */
// /*AFLA*/ #define class_create(owner, name)		\
// /*AFLA*/ ({						\
// /*AFLA*/ 	static struct lock_class_key __key;	\
// /*AFLA*/ 	__class_create(owner, name, &__key);	\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The type of device, "struct device" is embedded in. A class
// /*AFLA*/  * or bus can contain devices of different types
// /*AFLA*/  * like "partitions" and "disks", "mouse" and "event".
// /*AFLA*/  * This identifies the device type and carries type-specific
// /*AFLA*/  * information, equivalent to the kobj_type of a kobject.
// /*AFLA*/  * If "name" is specified, the uevent will contain it in
// /*AFLA*/  * the DEVTYPE variable.
// /*AFLA*/  */
// /*AFLA*/ struct device_type {
// /*AFLA*/ 	const char *name;
// /*AFLA*/ 	const struct attribute_group **groups;
// /*AFLA*/ 	int (*uevent)(struct device *dev, struct kobj_uevent_env *env);
// /*AFLA*/ 	char *(*devnode)(struct device *dev, umode_t *mode,
// /*AFLA*/ 			 kuid_t *uid, kgid_t *gid);
// /*AFLA*/ 	void (*release)(struct device *dev);
// /*AFLA*/ 
// /*AFLA*/ 	const struct dev_pm_ops *pm;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* interface for exporting device attributes */
// /*AFLA*/ struct device_attribute {
// /*AFLA*/ 	struct attribute	attr;
// /*AFLA*/ 	ssize_t (*show)(struct device *dev, struct device_attribute *attr,
// /*AFLA*/ 			char *buf);
// /*AFLA*/ 	ssize_t (*store)(struct device *dev, struct device_attribute *attr,
// /*AFLA*/ 			 const char *buf, size_t count);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct dev_ext_attribute {
// /*AFLA*/ 	struct device_attribute attr;
// /*AFLA*/ 	void *var;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ ssize_t device_show_ulong(struct device *dev, struct device_attribute *attr,
// /*AFLA*/ 			  char *buf);
// /*AFLA*/ ssize_t device_store_ulong(struct device *dev, struct device_attribute *attr,
// /*AFLA*/ 			   const char *buf, size_t count);
// /*AFLA*/ ssize_t device_show_int(struct device *dev, struct device_attribute *attr,
// /*AFLA*/ 			char *buf);
// /*AFLA*/ ssize_t device_store_int(struct device *dev, struct device_attribute *attr,
// /*AFLA*/ 			 const char *buf, size_t count);
// /*AFLA*/ ssize_t device_show_bool(struct device *dev, struct device_attribute *attr,
// /*AFLA*/ 			char *buf);
// /*AFLA*/ ssize_t device_store_bool(struct device *dev, struct device_attribute *attr,
// /*AFLA*/ 			 const char *buf, size_t count);
// /*AFLA*/ 
// /*AFLA*/ #define DEVICE_ATTR(_name, _mode, _show, _store) \
// /*AFLA*/ 	struct device_attribute dev_attr_##_name = __ATTR(_name, _mode, _show, _store)
// /*AFLA*/ #define DEVICE_ATTR_RW(_name) \
// /*AFLA*/ 	struct device_attribute dev_attr_##_name = __ATTR_RW(_name)
// /*AFLA*/ #define DEVICE_ATTR_RO(_name) \
// /*AFLA*/ 	struct device_attribute dev_attr_##_name = __ATTR_RO(_name)
// /*AFLA*/ #define DEVICE_ATTR_WO(_name) \
// /*AFLA*/ 	struct device_attribute dev_attr_##_name = __ATTR_WO(_name)
// /*AFLA*/ #define DEVICE_ULONG_ATTR(_name, _mode, _var) \
// /*AFLA*/ 	struct dev_ext_attribute dev_attr_##_name = \
// /*AFLA*/ 		{ __ATTR(_name, _mode, device_show_ulong, device_store_ulong), &(_var) }
// /*AFLA*/ #define DEVICE_INT_ATTR(_name, _mode, _var) \
// /*AFLA*/ 	struct dev_ext_attribute dev_attr_##_name = \
// /*AFLA*/ 		{ __ATTR(_name, _mode, device_show_int, device_store_int), &(_var) }
// /*AFLA*/ #define DEVICE_BOOL_ATTR(_name, _mode, _var) \
// /*AFLA*/ 	struct dev_ext_attribute dev_attr_##_name = \
// /*AFLA*/ 		{ __ATTR(_name, _mode, device_show_bool, device_store_bool), &(_var) }
// /*AFLA*/ #define DEVICE_ATTR_IGNORE_LOCKDEP(_name, _mode, _show, _store) \
// /*AFLA*/ 	struct device_attribute dev_attr_##_name =		\
// /*AFLA*/ 		__ATTR_IGNORE_LOCKDEP(_name, _mode, _show, _store)
// /*AFLA*/ 
// /*AFLA*/ extern int device_create_file(struct device *device,
// /*AFLA*/ 			      const struct device_attribute *entry);
// /*AFLA*/ extern void device_remove_file(struct device *dev,
// /*AFLA*/ 			       const struct device_attribute *attr);
// /*AFLA*/ extern bool device_remove_file_self(struct device *dev,
// /*AFLA*/ 				    const struct device_attribute *attr);
// /*AFLA*/ extern int __must_check device_create_bin_file(struct device *dev,
// /*AFLA*/ 					const struct bin_attribute *attr);
// /*AFLA*/ extern void device_remove_bin_file(struct device *dev,
// /*AFLA*/ 				   const struct bin_attribute *attr);
// /*AFLA*/ 
// /*AFLA*/ /* device resource management */
// /*AFLA*/ typedef void (*dr_release_t)(struct device *dev, void *res);
// /*AFLA*/ typedef int (*dr_match_t)(struct device *dev, void *res, void *match_data);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_DEVRES
// /*AFLA*/ extern void *__devres_alloc_node(dr_release_t release, size_t size, gfp_t gfp,
// /*AFLA*/ 				 int nid, const char *name) __malloc;
// /*AFLA*/ #define devres_alloc(release, size, gfp) \
// /*AFLA*/ 	__devres_alloc_node(release, size, gfp, NUMA_NO_NODE, #release)
// /*AFLA*/ #define devres_alloc_node(release, size, gfp, nid) \
// /*AFLA*/ 	__devres_alloc_node(release, size, gfp, nid, #release)
// /*AFLA*/ #else
// /*AFLA*/ extern void *devres_alloc_node(dr_release_t release, size_t size, gfp_t gfp,
// /*AFLA*/ 			       int nid) __malloc;
// /*AFLA*/ static inline void *devres_alloc(dr_release_t release, size_t size, gfp_t gfp)
// /*AFLA*/ {
// /*AFLA*/ 	return devres_alloc_node(release, size, gfp, NUMA_NO_NODE);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void devres_for_each_res(struct device *dev, dr_release_t release,
// /*AFLA*/ 				dr_match_t match, void *match_data,
// /*AFLA*/ 				void (*fn)(struct device *, void *, void *),
// /*AFLA*/ 				void *data);
// /*AFLA*/ extern void devres_free(void *res);
// /*AFLA*/ extern void devres_add(struct device *dev, void *res);
// /*AFLA*/ extern void *devres_find(struct device *dev, dr_release_t release,
// /*AFLA*/ 			 dr_match_t match, void *match_data);
// /*AFLA*/ extern void *devres_get(struct device *dev, void *new_res,
// /*AFLA*/ 			dr_match_t match, void *match_data);
// /*AFLA*/ extern void *devres_remove(struct device *dev, dr_release_t release,
// /*AFLA*/ 			   dr_match_t match, void *match_data);
// /*AFLA*/ extern int devres_destroy(struct device *dev, dr_release_t release,
// /*AFLA*/ 			  dr_match_t match, void *match_data);
// /*AFLA*/ extern int devres_release(struct device *dev, dr_release_t release,
// /*AFLA*/ 			  dr_match_t match, void *match_data);
// /*AFLA*/ 
// /*AFLA*/ /* devres group */
// /*AFLA*/ extern void * __must_check devres_open_group(struct device *dev, void *id,
// /*AFLA*/ 					     gfp_t gfp);
// /*AFLA*/ extern void devres_close_group(struct device *dev, void *id);
// /*AFLA*/ extern void devres_remove_group(struct device *dev, void *id);
// /*AFLA*/ extern int devres_release_group(struct device *dev, void *id);
// /*AFLA*/ 
// /*AFLA*/ /* managed devm_k.alloc/kfree for device drivers */
// /*AFLA*/ extern void *devm_kmalloc(struct device *dev, size_t size, gfp_t gfp) __malloc;
// /*AFLA*/ extern __printf(3, 0)
// /*AFLA*/ char *devm_kvasprintf(struct device *dev, gfp_t gfp, const char *fmt,
// /*AFLA*/ 		      va_list ap) __malloc;
// /*AFLA*/ extern __printf(3, 4)
// /*AFLA*/ char *devm_kasprintf(struct device *dev, gfp_t gfp, const char *fmt, ...) __malloc;
// /*AFLA*/ static inline void *devm_kzalloc(struct device *dev, size_t size, gfp_t gfp)
// /*AFLA*/ {
// /*AFLA*/ 	return devm_kmalloc(dev, size, gfp | __GFP_ZERO);
// /*AFLA*/ }
// /*AFLA*/ static inline void *devm_kmalloc_array(struct device *dev,
// /*AFLA*/ 				       size_t n, size_t size, gfp_t flags)
// /*AFLA*/ {
// /*AFLA*/ 	if (size != 0 && n > SIZE_MAX / size)
// /*AFLA*/ 		return NULL;
// /*AFLA*/ 	return devm_kmalloc(dev, n * size, flags);
// /*AFLA*/ }
// /*AFLA*/ static inline void *devm_kcalloc(struct device *dev,
// /*AFLA*/ 				 size_t n, size_t size, gfp_t flags)
// /*AFLA*/ {
// /*AFLA*/ 	return devm_kmalloc_array(dev, n, size, flags | __GFP_ZERO);
// /*AFLA*/ }
// /*AFLA*/ extern void devm_kfree(struct device *dev, void *p);
// /*AFLA*/ extern char *devm_kstrdup(struct device *dev, const char *s, gfp_t gfp) __malloc;
// /*AFLA*/ extern void *devm_kmemdup(struct device *dev, const void *src, size_t len,
// /*AFLA*/ 			  gfp_t gfp);
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long devm_get_free_pages(struct device *dev,
// /*AFLA*/ 					 gfp_t gfp_mask, unsigned int order);
// /*AFLA*/ extern void devm_free_pages(struct device *dev, unsigned long addr);
// /*AFLA*/ 
// /*AFLA*/ void __iomem *devm_ioremap_resource(struct device *dev, struct resource *res);
// /*AFLA*/ 
// /*AFLA*/ /* allows to add/remove a custom action to devres stack */
// /*AFLA*/ int devm_add_action(struct device *dev, void (*action)(void *), void *data);
// /*AFLA*/ void devm_remove_action(struct device *dev, void (*action)(void *), void *data);
// /*AFLA*/ 
// /*AFLA*/ static inline int devm_add_action_or_reset(struct device *dev,
// /*AFLA*/ 					   void (*action)(void *), void *data)
// /*AFLA*/ {
// /*AFLA*/ 	int ret;
// /*AFLA*/ 
// /*AFLA*/ 	ret = devm_add_action(dev, action, data);
// /*AFLA*/ 	if (ret)
// /*AFLA*/ 		action(data);
// /*AFLA*/ 
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct device_dma_parameters {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * a low level driver may set these to teach IOMMU code about
// /*AFLA*/ 	 * sg limitations.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned int max_segment_size;
// /*AFLA*/ 	unsigned long segment_boundary_mask;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct device - The basic device structure
// /*AFLA*/  * @parent:	The device's "parent" device, the device to which it is attached.
// /*AFLA*/  * 		In most cases, a parent device is some sort of bus or host
// /*AFLA*/  * 		controller. If parent is NULL, the device, is a top-level device,
// /*AFLA*/  * 		which is not usually what you want.
// /*AFLA*/  * @p:		Holds the private data of the driver core portions of the device.
// /*AFLA*/  * 		See the comment of the struct device_private for detail.
// /*AFLA*/  * @kobj:	A top-level, abstract class from which other classes are derived.
// /*AFLA*/  * @init_name:	Initial name of the device.
// /*AFLA*/  * @type:	The type of device.
// /*AFLA*/  * 		This identifies the device type and carries type-specific
// /*AFLA*/  * 		information.
// /*AFLA*/  * @mutex:	Mutex to synchronize calls to its driver.
// /*AFLA*/  * @bus:	Type of bus device is on.
// /*AFLA*/  * @driver:	Which driver has allocated this
// /*AFLA*/  * @platform_data: Platform data specific to the device.
// /*AFLA*/  * 		Example: For devices on custom boards, as typical of embedded
// /*AFLA*/  * 		and SOC based hardware, Linux often uses platform_data to point
// /*AFLA*/  * 		to board-specific structures describing devices and how they
// /*AFLA*/  * 		are wired.  That can include what ports are available, chip
// /*AFLA*/  * 		variants, which GPIO pins act in what additional roles, and so
// /*AFLA*/  * 		on.  This shrinks the "Board Support Packages" (BSPs) and
// /*AFLA*/  * 		minimizes board-specific #ifdefs in drivers.
// /*AFLA*/  * @driver_data: Private pointer for driver specific info.
// /*AFLA*/  * @power:	For device power management.
// /*AFLA*/  * 		See Documentation/power/devices.txt for details.
// /*AFLA*/  * @pm_domain:	Provide callbacks that are executed during system suspend,
// /*AFLA*/  * 		hibernation, system resume and during runtime PM transitions
// /*AFLA*/  * 		along with subsystem-level and driver-level callbacks.
// /*AFLA*/  * @pins:	For device pin management.
// /*AFLA*/  *		See Documentation/pinctrl.txt for details.
// /*AFLA*/  * @msi_list:	Hosts MSI descriptors
// /*AFLA*/  * @msi_domain: The generic MSI domain this device is using.
// /*AFLA*/  * @numa_node:	NUMA node this device is close to.
// /*AFLA*/  * @dma_mask:	Dma mask (if dma'ble device).
// /*AFLA*/  * @coherent_dma_mask: Like dma_mask, but for alloc_coherent mapping as not all
// /*AFLA*/  * 		hardware supports 64-bit addresses for consistent allocations
// /*AFLA*/  * 		such descriptors.
// /*AFLA*/  * @dma_pfn_offset: offset of DMA memory range relatively of RAM
// /*AFLA*/  * @dma_parms:	A low level driver may set these to teach IOMMU code about
// /*AFLA*/  * 		segment limitations.
// /*AFLA*/  * @dma_pools:	Dma pools (if dma'ble device).
// /*AFLA*/  * @dma_mem:	Internal for coherent mem override.
// /*AFLA*/  * @cma_area:	Contiguous memory area for dma allocations
// /*AFLA*/  * @archdata:	For arch-specific additions.
// /*AFLA*/  * @of_node:	Associated device tree node.
// /*AFLA*/  * @fwnode:	Associated device node supplied by platform firmware.
// /*AFLA*/  * @devt:	For creating the sysfs "dev".
// /*AFLA*/  * @id:		device instance
// /*AFLA*/  * @devres_lock: Spinlock to protect the resource of the device.
// /*AFLA*/  * @devres_head: The resources list of the device.
// /*AFLA*/  * @knode_class: The node used to add the device to the class list.
// /*AFLA*/  * @class:	The class of the device.
// /*AFLA*/  * @groups:	Optional attribute groups.
// /*AFLA*/  * @release:	Callback to free the device after all references have
// /*AFLA*/  * 		gone away. This should be set by the allocator of the
// /*AFLA*/  * 		device (i.e. the bus driver that discovered the device).
// /*AFLA*/  * @iommu_group: IOMMU group the device belongs to.
// /*AFLA*/  * @iommu_fwspec: IOMMU-specific properties supplied by firmware.
// /*AFLA*/  *
// /*AFLA*/  * @offline_disabled: If set, the device is permanently online.
// /*AFLA*/  * @offline:	Set after successful invocation of bus type's .offline().
// /*AFLA*/  *
// /*AFLA*/  * At the lowest level, every device in a Linux system is represented by an
// /*AFLA*/  * instance of struct device. The device structure contains the information
// /*AFLA*/  * that the device model core needs to model the system. Most subsystems,
// /*AFLA*/  * however, track additional information about the devices they host. As a
// /*AFLA*/  * result, it is rare for devices to be represented by bare device structures;
// /*AFLA*/  * instead, that structure, like kobject structures, is usually embedded within
// /*AFLA*/  * a higher-level representation of the device.
// /*AFLA*/  */
// /*AFLA*/ struct device {
// /*AFLA*/ 	struct device		*parent;
// /*AFLA*/ 
// /*AFLA*/ 	struct device_private	*p;
// /*AFLA*/ 
// /*AFLA*/ 	struct kobject kobj;
// /*AFLA*/ 	const char		*init_name; /* initial name of the device */
// /*AFLA*/ 	const struct device_type *type;
// /*AFLA*/ 
// /*AFLA*/ 	struct mutex		mutex;	/* mutex to synchronize calls to
// /*AFLA*/ 					 * its driver.
// /*AFLA*/ 					 */
// /*AFLA*/ 
// /*AFLA*/ 	struct bus_type	*bus;		/* type of bus device is on */
// /*AFLA*/ 	struct device_driver *driver;	/* which driver has allocated this
// /*AFLA*/ 					   device */
// /*AFLA*/ 	void		*platform_data;	/* Platform specific data, device
// /*AFLA*/ 					   core doesn't touch it */
// /*AFLA*/ 	void		*driver_data;	/* Driver data, set and get with
// /*AFLA*/ 					   dev_set/get_drvdata */
// /*AFLA*/ 	struct dev_pm_info	power;
// /*AFLA*/ 	struct dev_pm_domain	*pm_domain;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_GENERIC_MSI_IRQ_DOMAIN
// /*AFLA*/ 	struct irq_domain	*msi_domain;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_PINCTRL
// /*AFLA*/ 	struct dev_pin_info	*pins;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_GENERIC_MSI_IRQ
// /*AFLA*/ 	struct list_head	msi_list;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ 	int		numa_node;	/* NUMA node this device is close to */
// /*AFLA*/ #endif
// /*AFLA*/ 	u64		*dma_mask;	/* dma mask (if dma'able device) */
// /*AFLA*/ 	u64		coherent_dma_mask;/* Like dma_mask, but for
// /*AFLA*/ 					     alloc_coherent mappings as
// /*AFLA*/ 					     not all hardware supports
// /*AFLA*/ 					     64 bit addresses for consistent
// /*AFLA*/ 					     allocations such descriptors. */
// /*AFLA*/ 	unsigned long	dma_pfn_offset;
// /*AFLA*/ 
// /*AFLA*/ 	struct device_dma_parameters *dma_parms;
// /*AFLA*/ 
// /*AFLA*/ 	struct list_head	dma_pools;	/* dma pools (if dma'ble) */
// /*AFLA*/ 
// /*AFLA*/ 	struct dma_coherent_mem	*dma_mem; /* internal for coherent mem
// /*AFLA*/ 					     override */
// /*AFLA*/ #ifdef CONFIG_DMA_CMA
// /*AFLA*/ 	struct cma *cma_area;		/* contiguous memory area for dma
// /*AFLA*/ 					   allocations */
// /*AFLA*/ #endif
// /*AFLA*/ 	/* arch specific additions */
// /*AFLA*/ 	struct dev_archdata	archdata;
// /*AFLA*/ 
// /*AFLA*/ 	struct device_node	*of_node; /* associated device tree node */
// /*AFLA*/ 	struct fwnode_handle	*fwnode; /* firmware device node */
// /*AFLA*/ 
// /*AFLA*/ 	dev_t			devt;	/* dev_t, creates the sysfs "dev" */
// /*AFLA*/ 	u32			id;	/* device instance */
// /*AFLA*/ 
// /*AFLA*/ 	spinlock_t		devres_lock;
// /*AFLA*/ 	struct list_head	devres_head;
// /*AFLA*/ 
// /*AFLA*/ 	struct klist_node	knode_class;
// /*AFLA*/ 	struct class		*class;
// /*AFLA*/ 	const struct attribute_group **groups;	/* optional groups */
// /*AFLA*/ 
// /*AFLA*/ 	void	(*release)(struct device *dev);
// /*AFLA*/ 	struct iommu_group	*iommu_group;
// /*AFLA*/ 	struct iommu_fwspec	*iommu_fwspec;
// /*AFLA*/ 
// /*AFLA*/ 	bool			offline_disabled:1;
// /*AFLA*/ 	bool			offline:1;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline struct device *kobj_to_dev(struct kobject *kobj)
// /*AFLA*/ {
// /*AFLA*/ 	return container_of(kobj, struct device, kobj);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Get the wakeup routines, which depend on struct device */
// /*AFLA*/ #include <linux/pm_wakeup.h>
// /*AFLA*/ 
// /*AFLA*/ static inline const char *dev_name(const struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	/* Use the init name until the kobject becomes available */
// /*AFLA*/ 	if (dev->init_name)
// /*AFLA*/ 		return dev->init_name;
// /*AFLA*/ 
// /*AFLA*/ 	return kobject_name(&dev->kobj);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern __printf(2, 3)
// /*AFLA*/ int dev_set_name(struct device *dev, const char *name, ...);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ static inline int dev_to_node(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return dev->numa_node;
// /*AFLA*/ }
// /*AFLA*/ static inline void set_dev_node(struct device *dev, int node)
// /*AFLA*/ {
// /*AFLA*/ 	dev->numa_node = node;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline int dev_to_node(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return -1;
// /*AFLA*/ }
// /*AFLA*/ static inline void set_dev_node(struct device *dev, int node)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline struct irq_domain *dev_get_msi_domain(const struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_GENERIC_MSI_IRQ_DOMAIN
// /*AFLA*/ 	return dev->msi_domain;
// /*AFLA*/ #else
// /*AFLA*/ 	return NULL;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void dev_set_msi_domain(struct device *dev, struct irq_domain *d)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_GENERIC_MSI_IRQ_DOMAIN
// /*AFLA*/ 	dev->msi_domain = d;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void *dev_get_drvdata(const struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return dev->driver_data;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void dev_set_drvdata(struct device *dev, void *data)
// /*AFLA*/ {
// /*AFLA*/ 	dev->driver_data = data;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct pm_subsys_data *dev_to_psd(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return dev ? dev->power.subsys_data : NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int dev_get_uevent_suppress(const struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return dev->kobj.uevent_suppress;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void dev_set_uevent_suppress(struct device *dev, int val)
// /*AFLA*/ {
// /*AFLA*/ 	dev->kobj.uevent_suppress = val;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int device_is_registered(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return dev->kobj.state_in_sysfs;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void device_enable_async_suspend(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	if (!dev->power.is_prepared)
// /*AFLA*/ 		dev->power.async_suspend = true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void device_disable_async_suspend(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	if (!dev->power.is_prepared)
// /*AFLA*/ 		dev->power.async_suspend = false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool device_async_suspend_enabled(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return !!dev->power.async_suspend;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void dev_pm_syscore_device(struct device *dev, bool val)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_PM_SLEEP
// /*AFLA*/ 	dev->power.syscore = val;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void device_lock(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	mutex_lock(&dev->mutex);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int device_lock_interruptible(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return mutex_lock_interruptible(&dev->mutex);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int device_trylock(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return mutex_trylock(&dev->mutex);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void device_unlock(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	mutex_unlock(&dev->mutex);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void device_lock_assert(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	lockdep_assert_held(&dev->mutex);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct device_node *dev_of_node(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	if (!IS_ENABLED(CONFIG_OF))
// /*AFLA*/ 		return NULL;
// /*AFLA*/ 	return dev->of_node;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void driver_init(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * High level routines for use by the bus drivers
// /*AFLA*/  */
// /*AFLA*/ extern int __must_check device_register(struct device *dev);
// /*AFLA*/ extern void device_unregister(struct device *dev);
// /*AFLA*/ extern void device_initialize(struct device *dev);
// /*AFLA*/ extern int __must_check device_add(struct device *dev);
// /*AFLA*/ extern void device_del(struct device *dev);
// /*AFLA*/ extern int device_for_each_child(struct device *dev, void *data,
// /*AFLA*/ 		     int (*fn)(struct device *dev, void *data));
// /*AFLA*/ extern int device_for_each_child_reverse(struct device *dev, void *data,
// /*AFLA*/ 		     int (*fn)(struct device *dev, void *data));
// /*AFLA*/ extern struct device *device_find_child(struct device *dev, void *data,
// /*AFLA*/ 				int (*match)(struct device *dev, void *data));
// /*AFLA*/ extern int device_rename(struct device *dev, const char *new_name);
// /*AFLA*/ extern int device_move(struct device *dev, struct device *new_parent,
// /*AFLA*/ 		       enum dpm_order dpm_order);
// /*AFLA*/ extern const char *device_get_devnode(struct device *dev,
// /*AFLA*/ 				      umode_t *mode, kuid_t *uid, kgid_t *gid,
// /*AFLA*/ 				      const char **tmp);
// /*AFLA*/ 
// /*AFLA*/ static inline bool device_supports_offline(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return dev->bus && dev->bus->offline && dev->bus->online;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void lock_device_hotplug(void);
// /*AFLA*/ extern void unlock_device_hotplug(void);
// /*AFLA*/ extern int lock_device_hotplug_sysfs(void);
// /*AFLA*/ extern int device_offline(struct device *dev);
// /*AFLA*/ extern int device_online(struct device *dev);
// /*AFLA*/ extern void set_primary_fwnode(struct device *dev, struct fwnode_handle *fwnode);
// /*AFLA*/ extern void set_secondary_fwnode(struct device *dev, struct fwnode_handle *fwnode);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Root device objects for grouping under /sys/devices
// /*AFLA*/  */
// /*AFLA*/ extern struct device *__root_device_register(const char *name,
// /*AFLA*/ 					     struct module *owner);
// /*AFLA*/ 
// /*AFLA*/ /* This is a macro to avoid include problems with THIS_MODULE */
// /*AFLA*/ #define root_device_register(name) \
// /*AFLA*/ 	__root_device_register(name, THIS_MODULE)
// /*AFLA*/ 
// /*AFLA*/ extern void root_device_unregister(struct device *root);
// /*AFLA*/ 
// /*AFLA*/ static inline void *dev_get_platdata(const struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return dev->platform_data;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Manual binding of a device to driver. See drivers/base/bus.c
// /*AFLA*/  * for information on use.
// /*AFLA*/  */
// /*AFLA*/ extern int __must_check device_bind_driver(struct device *dev);
// /*AFLA*/ extern void device_release_driver(struct device *dev);
// /*AFLA*/ extern int  __must_check device_attach(struct device *dev);
// /*AFLA*/ extern int __must_check driver_attach(struct device_driver *drv);
// /*AFLA*/ extern void device_initial_probe(struct device *dev);
// /*AFLA*/ extern int __must_check device_reprobe(struct device *dev);
// /*AFLA*/ 
// /*AFLA*/ extern bool device_is_bound(struct device *dev);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Easy functions for dynamically creating devices on the fly
// /*AFLA*/  */
// /*AFLA*/ extern __printf(5, 0)
// /*AFLA*/ struct device *device_create_vargs(struct class *cls, struct device *parent,
// /*AFLA*/ 				   dev_t devt, void *drvdata,
// /*AFLA*/ 				   const char *fmt, va_list vargs);
// /*AFLA*/ extern __printf(5, 6)
// /*AFLA*/ struct device *device_create(struct class *cls, struct device *parent,
// /*AFLA*/ 			     dev_t devt, void *drvdata,
// /*AFLA*/ 			     const char *fmt, ...);
// /*AFLA*/ extern __printf(6, 7)
// /*AFLA*/ struct device *device_create_with_groups(struct class *cls,
// /*AFLA*/ 			     struct device *parent, dev_t devt, void *drvdata,
// /*AFLA*/ 			     const struct attribute_group **groups,
// /*AFLA*/ 			     const char *fmt, ...);
// /*AFLA*/ extern void device_destroy(struct class *cls, dev_t devt);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Platform "fixup" functions - allow the platform to have their say
// /*AFLA*/  * about devices and actions that the general device layer doesn't
// /*AFLA*/  * know about.
// /*AFLA*/  */
// /*AFLA*/ /* Notify platform of device discovery */
// /*AFLA*/ extern int (*platform_notify)(struct device *dev);
// /*AFLA*/ 
// /*AFLA*/ extern int (*platform_notify_remove)(struct device *dev);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * get_device - atomically increment the reference count for the device.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ extern struct device *get_device(struct device *dev);
// /*AFLA*/ extern void put_device(struct device *dev);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEVTMPFS
// /*AFLA*/ extern int devtmpfs_create_node(struct device *dev);
// /*AFLA*/ extern int devtmpfs_delete_node(struct device *dev);
// /*AFLA*/ extern int devtmpfs_mount(const char *mntdir);
// /*AFLA*/ #else
// /*AFLA*/ static inline int devtmpfs_create_node(struct device *dev) { return 0; }
// /*AFLA*/ static inline int devtmpfs_delete_node(struct device *dev) { return 0; }
// /*AFLA*/ static inline int devtmpfs_mount(const char *mountpoint) { return 0; }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* drivers/base/power/shutdown.c */
// /*AFLA*/ extern void device_shutdown(void);
// /*AFLA*/ 
// /*AFLA*/ /* debugging and troubleshooting/diagnostic helpers. */
// /*AFLA*/ extern const char *dev_driver_string(const struct device *dev);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PRINTK
// /*AFLA*/ 
// /*AFLA*/ extern __printf(3, 0)
// /*AFLA*/ int dev_vprintk_emit(int level, const struct device *dev,
// /*AFLA*/ 		     const char *fmt, va_list args);
// /*AFLA*/ extern __printf(3, 4)
// /*AFLA*/ int dev_printk_emit(int level, const struct device *dev, const char *fmt, ...);
// /*AFLA*/ 
// /*AFLA*/ extern __printf(3, 4)
// /*AFLA*/ void dev_printk(const char *level, const struct device *dev,
// /*AFLA*/ 		const char *fmt, ...);
// /*AFLA*/ extern __printf(2, 3)
// /*AFLA*/ void dev_emerg(const struct device *dev, const char *fmt, ...);
// /*AFLA*/ extern __printf(2, 3)
// /*AFLA*/ void dev_alert(const struct device *dev, const char *fmt, ...);
// /*AFLA*/ extern __printf(2, 3)
// /*AFLA*/ void dev_crit(const struct device *dev, const char *fmt, ...);
// /*AFLA*/ extern __printf(2, 3)
// /*AFLA*/ void dev_err(const struct device *dev, const char *fmt, ...);
// /*AFLA*/ extern __printf(2, 3)
// /*AFLA*/ void dev_warn(const struct device *dev, const char *fmt, ...);
// /*AFLA*/ extern __printf(2, 3)
// /*AFLA*/ void dev_notice(const struct device *dev, const char *fmt, ...);
// /*AFLA*/ extern __printf(2, 3)
// /*AFLA*/ void _dev_info(const struct device *dev, const char *fmt, ...);
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline __printf(3, 0)
// /*AFLA*/ int dev_vprintk_emit(int level, const struct device *dev,
// /*AFLA*/ 		     const char *fmt, va_list args)
// /*AFLA*/ { return 0; }
// /*AFLA*/ static inline __printf(3, 4)
// /*AFLA*/ int dev_printk_emit(int level, const struct device *dev, const char *fmt, ...)
// /*AFLA*/ { return 0; }
// /*AFLA*/ 
// /*AFLA*/ static inline void __dev_printk(const char *level, const struct device *dev,
// /*AFLA*/ 				struct va_format *vaf)
// /*AFLA*/ {}
// /*AFLA*/ static inline __printf(3, 4)
// /*AFLA*/ void dev_printk(const char *level, const struct device *dev,
// /*AFLA*/ 		const char *fmt, ...)
// /*AFLA*/ {}
// /*AFLA*/ 
// /*AFLA*/ static inline __printf(2, 3)
// /*AFLA*/ void dev_emerg(const struct device *dev, const char *fmt, ...)
// /*AFLA*/ {}
// /*AFLA*/ static inline __printf(2, 3)
// /*AFLA*/ void dev_crit(const struct device *dev, const char *fmt, ...)
// /*AFLA*/ {}
// /*AFLA*/ static inline __printf(2, 3)
// /*AFLA*/ void dev_alert(const struct device *dev, const char *fmt, ...)
// /*AFLA*/ {}
// /*AFLA*/ static inline __printf(2, 3)
// /*AFLA*/ void dev_err(const struct device *dev, const char *fmt, ...)
// /*AFLA*/ {}
// /*AFLA*/ static inline __printf(2, 3)
// /*AFLA*/ void dev_warn(const struct device *dev, const char *fmt, ...)
// /*AFLA*/ {}
// /*AFLA*/ static inline __printf(2, 3)
// /*AFLA*/ void dev_notice(const struct device *dev, const char *fmt, ...)
// /*AFLA*/ {}
// /*AFLA*/ static inline __printf(2, 3)
// /*AFLA*/ void _dev_info(const struct device *dev, const char *fmt, ...)
// /*AFLA*/ {}
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Stupid hackaround for existing uses of non-printk uses dev_info
// /*AFLA*/  *
// /*AFLA*/  * Note that the definition of dev_info below is actually _dev_info
// /*AFLA*/  * and a macro is used to avoid redefining dev_info
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define dev_info(dev, fmt, arg...) _dev_info(dev, fmt, ##arg)
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_DYNAMIC_DEBUG)
// /*AFLA*/ #define dev_dbg(dev, format, ...)		     \
// /*AFLA*/ do {						     \
// /*AFLA*/ 	dynamic_dev_dbg(dev, format, ##__VA_ARGS__); \
// /*AFLA*/ } while (0)
// /*AFLA*/ #elif defined(DEBUG)
// /*AFLA*/ #define dev_dbg(dev, format, arg...)		\
// /*AFLA*/ 	dev_printk(KERN_DEBUG, dev, format, ##arg)
// /*AFLA*/ #else
// /*AFLA*/ #define dev_dbg(dev, format, arg...)				\
// /*AFLA*/ ({								\
// /*AFLA*/ 	if (0)							\
// /*AFLA*/ 		dev_printk(KERN_DEBUG, dev, format, ##arg);	\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PRINTK
// /*AFLA*/ #define dev_level_once(dev_level, dev, fmt, ...)			\
// /*AFLA*/ do {									\
// /*AFLA*/ 	static bool __print_once __read_mostly;				\
// /*AFLA*/ 									\
// /*AFLA*/ 	if (!__print_once) {						\
// /*AFLA*/ 		__print_once = true;					\
// /*AFLA*/ 		dev_level(dev, fmt, ##__VA_ARGS__);			\
// /*AFLA*/ 	}								\
// /*AFLA*/ } while (0)
// /*AFLA*/ #else
// /*AFLA*/ #define dev_level_once(dev_level, dev, fmt, ...)			\
// /*AFLA*/ do {									\
// /*AFLA*/ 	if (0)								\
// /*AFLA*/ 		dev_level(dev, fmt, ##__VA_ARGS__);			\
// /*AFLA*/ } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define dev_emerg_once(dev, fmt, ...)					\
// /*AFLA*/ 	dev_level_once(dev_emerg, dev, fmt, ##__VA_ARGS__)
// /*AFLA*/ #define dev_alert_once(dev, fmt, ...)					\
// /*AFLA*/ 	dev_level_once(dev_alert, dev, fmt, ##__VA_ARGS__)
// /*AFLA*/ #define dev_crit_once(dev, fmt, ...)					\
// /*AFLA*/ 	dev_level_once(dev_crit, dev, fmt, ##__VA_ARGS__)
// /*AFLA*/ #define dev_err_once(dev, fmt, ...)					\
// /*AFLA*/ 	dev_level_once(dev_err, dev, fmt, ##__VA_ARGS__)
// /*AFLA*/ #define dev_warn_once(dev, fmt, ...)					\
// /*AFLA*/ 	dev_level_once(dev_warn, dev, fmt, ##__VA_ARGS__)
// /*AFLA*/ #define dev_notice_once(dev, fmt, ...)					\
// /*AFLA*/ 	dev_level_once(dev_notice, dev, fmt, ##__VA_ARGS__)
// /*AFLA*/ #define dev_info_once(dev, fmt, ...)					\
// /*AFLA*/ 	dev_level_once(dev_info, dev, fmt, ##__VA_ARGS__)
// /*AFLA*/ #define dev_dbg_once(dev, fmt, ...)					\
// /*AFLA*/ 	dev_level_once(dev_dbg, dev, fmt, ##__VA_ARGS__)
// /*AFLA*/ 
// /*AFLA*/ #define dev_level_ratelimited(dev_level, dev, fmt, ...)			\
// /*AFLA*/ do {									\
// /*AFLA*/ 	static DEFINE_RATELIMIT_STATE(_rs,				\
// /*AFLA*/ 				      DEFAULT_RATELIMIT_INTERVAL,	\
// /*AFLA*/ 				      DEFAULT_RATELIMIT_BURST);		\
// /*AFLA*/ 	if (__ratelimit(&_rs))						\
// /*AFLA*/ 		dev_level(dev, fmt, ##__VA_ARGS__);			\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define dev_emerg_ratelimited(dev, fmt, ...)				\
// /*AFLA*/ 	dev_level_ratelimited(dev_emerg, dev, fmt, ##__VA_ARGS__)
// /*AFLA*/ #define dev_alert_ratelimited(dev, fmt, ...)				\
// /*AFLA*/ 	dev_level_ratelimited(dev_alert, dev, fmt, ##__VA_ARGS__)
// /*AFLA*/ #define dev_crit_ratelimited(dev, fmt, ...)				\
// /*AFLA*/ 	dev_level_ratelimited(dev_crit, dev, fmt, ##__VA_ARGS__)
// /*AFLA*/ #define dev_err_ratelimited(dev, fmt, ...)				\
// /*AFLA*/ 	dev_level_ratelimited(dev_err, dev, fmt, ##__VA_ARGS__)
// /*AFLA*/ #define dev_warn_ratelimited(dev, fmt, ...)				\
// /*AFLA*/ 	dev_level_ratelimited(dev_warn, dev, fmt, ##__VA_ARGS__)
// /*AFLA*/ #define dev_notice_ratelimited(dev, fmt, ...)				\
// /*AFLA*/ 	dev_level_ratelimited(dev_notice, dev, fmt, ##__VA_ARGS__)
// /*AFLA*/ #define dev_info_ratelimited(dev, fmt, ...)				\
// /*AFLA*/ 	dev_level_ratelimited(dev_info, dev, fmt, ##__VA_ARGS__)
// /*AFLA*/ #if defined(CONFIG_DYNAMIC_DEBUG)
// /*AFLA*/ /* descriptor check is first to prevent flooding with "callbacks suppressed" */
// /*AFLA*/ #define dev_dbg_ratelimited(dev, fmt, ...)				\
// /*AFLA*/ do {									\
// /*AFLA*/ 	static DEFINE_RATELIMIT_STATE(_rs,				\
// /*AFLA*/ 				      DEFAULT_RATELIMIT_INTERVAL,	\
// /*AFLA*/ 				      DEFAULT_RATELIMIT_BURST);		\
// /*AFLA*/ 	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, fmt);			\
// /*AFLA*/ 	if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT) &&	\
// /*AFLA*/ 	    __ratelimit(&_rs))						\
// /*AFLA*/ 		__dynamic_dev_dbg(&descriptor, dev, fmt,		\
// /*AFLA*/ 				  ##__VA_ARGS__);			\
// /*AFLA*/ } while (0)
// /*AFLA*/ #elif defined(DEBUG)
// /*AFLA*/ #define dev_dbg_ratelimited(dev, fmt, ...)				\
// /*AFLA*/ do {									\
// /*AFLA*/ 	static DEFINE_RATELIMIT_STATE(_rs,				\
// /*AFLA*/ 				      DEFAULT_RATELIMIT_INTERVAL,	\
// /*AFLA*/ 				      DEFAULT_RATELIMIT_BURST);		\
// /*AFLA*/ 	if (__ratelimit(&_rs))						\
// /*AFLA*/ 		dev_printk(KERN_DEBUG, dev, fmt, ##__VA_ARGS__);	\
// /*AFLA*/ } while (0)
// /*AFLA*/ #else
// /*AFLA*/ #define dev_dbg_ratelimited(dev, fmt, ...)				\
// /*AFLA*/ do {									\
// /*AFLA*/ 	if (0)								\
// /*AFLA*/ 		dev_printk(KERN_DEBUG, dev, fmt, ##__VA_ARGS__);	\
// /*AFLA*/ } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef VERBOSE_DEBUG
// /*AFLA*/ #define dev_vdbg	dev_dbg
// /*AFLA*/ #else
// /*AFLA*/ #define dev_vdbg(dev, format, arg...)				\
// /*AFLA*/ ({								\
// /*AFLA*/ 	if (0)							\
// /*AFLA*/ 		dev_printk(KERN_DEBUG, dev, format, ##arg);	\
// /*AFLA*/ })
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * dev_WARN*() acts like dev_printk(), but with the key difference of
// /*AFLA*/  * using WARN/WARN_ONCE to include file/line information and a backtrace.
// /*AFLA*/  */
// /*AFLA*/ #define dev_WARN(dev, format, arg...) \
// /*AFLA*/ 	WARN(1, "%s %s: " format, dev_driver_string(dev), dev_name(dev), ## arg);
// /*AFLA*/ 
// /*AFLA*/ #define dev_WARN_ONCE(dev, condition, format, arg...) \
// /*AFLA*/ 	WARN_ONCE(condition, "%s %s: " format, \
// /*AFLA*/ 			dev_driver_string(dev), dev_name(dev), ## arg)
// /*AFLA*/ 
// /*AFLA*/ /* Create alias, so I can be autoloaded. */
// /*AFLA*/ #define MODULE_ALIAS_CHARDEV(major,minor) \
// /*AFLA*/ 	MODULE_ALIAS("char-major-" __stringify(major) "-" __stringify(minor))
// /*AFLA*/ #define MODULE_ALIAS_CHARDEV_MAJOR(major) \
// /*AFLA*/ 	MODULE_ALIAS("char-major-" __stringify(major) "-*")
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SYSFS_DEPRECATED
// /*AFLA*/ extern long sysfs_deprecated;
// /*AFLA*/ #else
// /*AFLA*/ #define sysfs_deprecated 0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * module_driver() - Helper macro for drivers that don't do anything
// /*AFLA*/  * special in module init/exit. This eliminates a lot of boilerplate.
// /*AFLA*/  * Each module may only use this macro once, and calling it replaces
// /*AFLA*/  * module_init() and module_exit().
// /*AFLA*/  *
// /*AFLA*/  * @__driver: driver name
// /*AFLA*/  * @__register: register function for this driver type
// /*AFLA*/  * @__unregister: unregister function for this driver type
// /*AFLA*/  * @...: Additional arguments to be passed to __register and __unregister.
// /*AFLA*/  *
// /*AFLA*/  * Use this macro to construct bus specific macros for registering
// /*AFLA*/  * drivers, and do not use it on its own.
// /*AFLA*/  */
// /*AFLA*/ #define module_driver(__driver, __register, __unregister, ...) \
// /*AFLA*/ static int __init __driver##_init(void) \
// /*AFLA*/ { \
// /*AFLA*/ 	return __register(&(__driver) , ##__VA_ARGS__); \
// /*AFLA*/ } \
// /*AFLA*/ module_init(__driver##_init); \
// /*AFLA*/ static void __exit __driver##_exit(void) \
// /*AFLA*/ { \
// /*AFLA*/ 	__unregister(&(__driver) , ##__VA_ARGS__); \
// /*AFLA*/ } \
// /*AFLA*/ module_exit(__driver##_exit);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * builtin_driver() - Helper macro for drivers that don't do anything
// /*AFLA*/  * special in init and have no exit. This eliminates some boilerplate.
// /*AFLA*/  * Each driver may only use this macro once, and calling it replaces
// /*AFLA*/  * device_initcall (or in some cases, the legacy __initcall).  This is
// /*AFLA*/  * meant to be a direct parallel of module_driver() above but without
// /*AFLA*/  * the __exit stuff that is not used for builtin cases.
// /*AFLA*/  *
// /*AFLA*/  * @__driver: driver name
// /*AFLA*/  * @__register: register function for this driver type
// /*AFLA*/  * @...: Additional arguments to be passed to __register
// /*AFLA*/  *
// /*AFLA*/  * Use this macro to construct bus specific macros for registering
// /*AFLA*/  * drivers, and do not use it on its own.
// /*AFLA*/  */
// /*AFLA*/ #define builtin_driver(__driver, __register, ...) \
// /*AFLA*/ static int __init __driver##_init(void) \
// /*AFLA*/ { \
// /*AFLA*/ 	return __register(&(__driver) , ##__VA_ARGS__); \
// /*AFLA*/ } \
// /*AFLA*/ device_initcall(__driver##_init);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _DEVICE_H_ */
