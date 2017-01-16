// /*AFLA*/ /*
// /*AFLA*/  * sysfs.h - definitions for the device driver filesystem
// /*AFLA*/  *
// /*AFLA*/  * Copyright (c) 2001,2002 Patrick Mochel
// /*AFLA*/  * Copyright (c) 2004 Silicon Graphics, Inc.
// /*AFLA*/  * Copyright (c) 2007 SUSE Linux Products GmbH
// /*AFLA*/  * Copyright (c) 2007 Tejun Heo <teheo@suse.de>
// /*AFLA*/  *
// /*AFLA*/  * Please see Documentation/filesystems/sysfs.txt for more information.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _SYSFS_H_
// /*AFLA*/ #define _SYSFS_H_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/kernfs.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/lockdep.h>
// /*AFLA*/ #include <linux/kobject_ns.h>
// /*AFLA*/ #include <linux/stat.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ 
// /*AFLA*/ struct kobject;
// /*AFLA*/ struct module;
// /*AFLA*/ struct bin_attribute;
// /*AFLA*/ enum kobj_ns_type;
// /*AFLA*/ 
// /*AFLA*/ struct attribute {
// /*AFLA*/ 	const char		*name;
// /*AFLA*/ 	umode_t			mode;
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ 	bool			ignore_lockdep:1;
// /*AFLA*/ 	struct lock_class_key	*key;
// /*AFLA*/ 	struct lock_class_key	skey;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  *	sysfs_attr_init - initialize a dynamically allocated sysfs attribute
// /*AFLA*/  *	@attr: struct attribute to initialize
// /*AFLA*/  *
// /*AFLA*/  *	Initialize a dynamically allocated struct attribute so we can
// /*AFLA*/  *	make lockdep happy.  This is a new requirement for attributes
// /*AFLA*/  *	and initially this is only needed when lockdep is enabled.
// /*AFLA*/  *	Lockdep gives a nice error when your attribute is added to
// /*AFLA*/  *	sysfs if you don't have this.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ #define sysfs_attr_init(attr)				\
// /*AFLA*/ do {							\
// /*AFLA*/ 	static struct lock_class_key __key;		\
// /*AFLA*/ 							\
// /*AFLA*/ 	(attr)->key = &__key;				\
// /*AFLA*/ } while (0)
// /*AFLA*/ #else
// /*AFLA*/ #define sysfs_attr_init(attr) do {} while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct attribute_group - data structure used to declare an attribute group.
// /*AFLA*/  * @name:	Optional: Attribute group name
// /*AFLA*/  *		If specified, the attribute group will be created in
// /*AFLA*/  *		a new subdirectory with this name.
// /*AFLA*/  * @is_visible:	Optional: Function to return permissions associated with an
// /*AFLA*/  *		attribute of the group. Will be called repeatedly for each
// /*AFLA*/  *		non-binary attribute in the group. Only read/write
// /*AFLA*/  *		permissions as well as SYSFS_PREALLOC are accepted. Must
// /*AFLA*/  *		return 0 if an attribute is not visible. The returned value
// /*AFLA*/  *		will replace static permissions defined in struct attribute.
// /*AFLA*/  * @is_bin_visible:
// /*AFLA*/  *		Optional: Function to return permissions associated with a
// /*AFLA*/  *		binary attribute of the group. Will be called repeatedly
// /*AFLA*/  *		for each binary attribute in the group. Only read/write
// /*AFLA*/  *		permissions as well as SYSFS_PREALLOC are accepted. Must
// /*AFLA*/  *		return 0 if a binary attribute is not visible. The returned
// /*AFLA*/  *		value will replace static permissions defined in
// /*AFLA*/  *		struct bin_attribute.
// /*AFLA*/  * @attrs:	Pointer to NULL terminated list of attributes.
// /*AFLA*/  * @bin_attrs:	Pointer to NULL terminated list of binary attributes.
// /*AFLA*/  *		Either attrs or bin_attrs or both must be provided.
// /*AFLA*/  */
// /*AFLA*/ struct attribute_group {
// /*AFLA*/ 	const char		*name;
// /*AFLA*/ 	umode_t			(*is_visible)(struct kobject *,
// /*AFLA*/ 					      struct attribute *, int);
// /*AFLA*/ 	umode_t			(*is_bin_visible)(struct kobject *,
// /*AFLA*/ 						  struct bin_attribute *, int);
// /*AFLA*/ 	struct attribute	**attrs;
// /*AFLA*/ 	struct bin_attribute	**bin_attrs;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * Use these macros to make defining attributes easier. See include/linux/device.h
// /*AFLA*/  * for examples..
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define SYSFS_PREALLOC 010000
// /*AFLA*/ 
// /*AFLA*/ #define __ATTR(_name, _mode, _show, _store) {				\
// /*AFLA*/ 	.attr = {.name = __stringify(_name),				\
// /*AFLA*/ 		 .mode = VERIFY_OCTAL_PERMISSIONS(_mode) },		\
// /*AFLA*/ 	.show	= _show,						\
// /*AFLA*/ 	.store	= _store,						\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __ATTR_PREALLOC(_name, _mode, _show, _store) {			\
// /*AFLA*/ 	.attr = {.name = __stringify(_name),				\
// /*AFLA*/ 		 .mode = SYSFS_PREALLOC | VERIFY_OCTAL_PERMISSIONS(_mode) },\
// /*AFLA*/ 	.show	= _show,						\
// /*AFLA*/ 	.store	= _store,						\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __ATTR_RO(_name) {						\
// /*AFLA*/ 	.attr	= { .name = __stringify(_name), .mode = S_IRUGO },	\
// /*AFLA*/ 	.show	= _name##_show,						\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __ATTR_WO(_name) {						\
// /*AFLA*/ 	.attr	= { .name = __stringify(_name), .mode = S_IWUSR },	\
// /*AFLA*/ 	.store	= _name##_store,					\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __ATTR_RW(_name) __ATTR(_name, (S_IWUSR | S_IRUGO),		\
// /*AFLA*/ 			 _name##_show, _name##_store)
// /*AFLA*/ 
// /*AFLA*/ #define __ATTR_NULL { .attr = { .name = NULL } }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_LOCK_ALLOC
// /*AFLA*/ #define __ATTR_IGNORE_LOCKDEP(_name, _mode, _show, _store) {	\
// /*AFLA*/ 	.attr = {.name = __stringify(_name), .mode = _mode,	\
// /*AFLA*/ 			.ignore_lockdep = true },		\
// /*AFLA*/ 	.show		= _show,				\
// /*AFLA*/ 	.store		= _store,				\
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ #define __ATTR_IGNORE_LOCKDEP	__ATTR
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __ATTRIBUTE_GROUPS(_name)				\
// /*AFLA*/ static const struct attribute_group *_name##_groups[] = {	\
// /*AFLA*/ 	&_name##_group,						\
// /*AFLA*/ 	NULL,							\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define ATTRIBUTE_GROUPS(_name)					\
// /*AFLA*/ static const struct attribute_group _name##_group = {		\
// /*AFLA*/ 	.attrs = _name##_attrs,					\
// /*AFLA*/ };								\
// /*AFLA*/ __ATTRIBUTE_GROUPS(_name)
// /*AFLA*/ 
// /*AFLA*/ struct file;
// /*AFLA*/ struct vm_area_struct;
// /*AFLA*/ 
// /*AFLA*/ struct bin_attribute {
// /*AFLA*/ 	struct attribute	attr;
// /*AFLA*/ 	size_t			size;
// /*AFLA*/ 	void			*private;
// /*AFLA*/ 	ssize_t (*read)(struct file *, struct kobject *, struct bin_attribute *,
// /*AFLA*/ 			char *, loff_t, size_t);
// /*AFLA*/ 	ssize_t (*write)(struct file *, struct kobject *, struct bin_attribute *,
// /*AFLA*/ 			 char *, loff_t, size_t);
// /*AFLA*/ 	int (*mmap)(struct file *, struct kobject *, struct bin_attribute *attr,
// /*AFLA*/ 		    struct vm_area_struct *vma);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  *	sysfs_bin_attr_init - initialize a dynamically allocated bin_attribute
// /*AFLA*/  *	@attr: struct bin_attribute to initialize
// /*AFLA*/  *
// /*AFLA*/  *	Initialize a dynamically allocated struct bin_attribute so we
// /*AFLA*/  *	can make lockdep happy.  This is a new requirement for
// /*AFLA*/  *	attributes and initially this is only needed when lockdep is
// /*AFLA*/  *	enabled.  Lockdep gives a nice error when your attribute is
// /*AFLA*/  *	added to sysfs if you don't have this.
// /*AFLA*/  */
// /*AFLA*/ #define sysfs_bin_attr_init(bin_attr) sysfs_attr_init(&(bin_attr)->attr)
// /*AFLA*/ 
// /*AFLA*/ /* macros to create static binary attributes easier */
// /*AFLA*/ #define __BIN_ATTR(_name, _mode, _read, _write, _size) {		\
// /*AFLA*/ 	.attr = { .name = __stringify(_name), .mode = _mode },		\
// /*AFLA*/ 	.read	= _read,						\
// /*AFLA*/ 	.write	= _write,						\
// /*AFLA*/ 	.size	= _size,						\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __BIN_ATTR_RO(_name, _size) {					\
// /*AFLA*/ 	.attr	= { .name = __stringify(_name), .mode = S_IRUGO },	\
// /*AFLA*/ 	.read	= _name##_read,						\
// /*AFLA*/ 	.size	= _size,						\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define __BIN_ATTR_RW(_name, _size) __BIN_ATTR(_name,			\
// /*AFLA*/ 				   (S_IWUSR | S_IRUGO), _name##_read,	\
// /*AFLA*/ 				   _name##_write, _size)
// /*AFLA*/ 
// /*AFLA*/ #define __BIN_ATTR_NULL __ATTR_NULL
// /*AFLA*/ 
// /*AFLA*/ #define BIN_ATTR(_name, _mode, _read, _write, _size)			\
// /*AFLA*/ struct bin_attribute bin_attr_##_name = __BIN_ATTR(_name, _mode, _read,	\
// /*AFLA*/ 					_write, _size)
// /*AFLA*/ 
// /*AFLA*/ #define BIN_ATTR_RO(_name, _size)					\
// /*AFLA*/ struct bin_attribute bin_attr_##_name = __BIN_ATTR_RO(_name, _size)
// /*AFLA*/ 
// /*AFLA*/ #define BIN_ATTR_RW(_name, _size)					\
// /*AFLA*/ struct bin_attribute bin_attr_##_name = __BIN_ATTR_RW(_name, _size)
// /*AFLA*/ 
// /*AFLA*/ struct sysfs_ops {
// /*AFLA*/ 	ssize_t	(*show)(struct kobject *, struct attribute *, char *);
// /*AFLA*/ 	ssize_t	(*store)(struct kobject *, struct attribute *, const char *, size_t);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SYSFS
// /*AFLA*/ 
// /*AFLA*/ int __must_check sysfs_create_dir_ns(struct kobject *kobj, const void *ns);
// /*AFLA*/ void sysfs_remove_dir(struct kobject *kobj);
// /*AFLA*/ int __must_check sysfs_rename_dir_ns(struct kobject *kobj, const char *new_name,
// /*AFLA*/ 				     const void *new_ns);
// /*AFLA*/ int __must_check sysfs_move_dir_ns(struct kobject *kobj,
// /*AFLA*/ 				   struct kobject *new_parent_kobj,
// /*AFLA*/ 				   const void *new_ns);
// /*AFLA*/ int __must_check sysfs_create_mount_point(struct kobject *parent_kobj,
// /*AFLA*/ 					  const char *name);
// /*AFLA*/ void sysfs_remove_mount_point(struct kobject *parent_kobj,
// /*AFLA*/ 			      const char *name);
// /*AFLA*/ 
// /*AFLA*/ int __must_check sysfs_create_file_ns(struct kobject *kobj,
// /*AFLA*/ 				      const struct attribute *attr,
// /*AFLA*/ 				      const void *ns);
// /*AFLA*/ int __must_check sysfs_create_files(struct kobject *kobj,
// /*AFLA*/ 				   const struct attribute **attr);
// /*AFLA*/ int __must_check sysfs_chmod_file(struct kobject *kobj,
// /*AFLA*/ 				  const struct attribute *attr, umode_t mode);
// /*AFLA*/ void sysfs_remove_file_ns(struct kobject *kobj, const struct attribute *attr,
// /*AFLA*/ 			  const void *ns);
// /*AFLA*/ bool sysfs_remove_file_self(struct kobject *kobj, const struct attribute *attr);
// /*AFLA*/ void sysfs_remove_files(struct kobject *kobj, const struct attribute **attr);
// /*AFLA*/ 
// /*AFLA*/ int __must_check sysfs_create_bin_file(struct kobject *kobj,
// /*AFLA*/ 				       const struct bin_attribute *attr);
// /*AFLA*/ void sysfs_remove_bin_file(struct kobject *kobj,
// /*AFLA*/ 			   const struct bin_attribute *attr);
// /*AFLA*/ 
// /*AFLA*/ int __must_check sysfs_create_link(struct kobject *kobj, struct kobject *target,
// /*AFLA*/ 				   const char *name);
// /*AFLA*/ int __must_check sysfs_create_link_nowarn(struct kobject *kobj,
// /*AFLA*/ 					  struct kobject *target,
// /*AFLA*/ 					  const char *name);
// /*AFLA*/ void sysfs_remove_link(struct kobject *kobj, const char *name);
// /*AFLA*/ 
// /*AFLA*/ int sysfs_rename_link_ns(struct kobject *kobj, struct kobject *target,
// /*AFLA*/ 			 const char *old_name, const char *new_name,
// /*AFLA*/ 			 const void *new_ns);
// /*AFLA*/ 
// /*AFLA*/ void sysfs_delete_link(struct kobject *dir, struct kobject *targ,
// /*AFLA*/ 			const char *name);
// /*AFLA*/ 
// /*AFLA*/ int __must_check sysfs_create_group(struct kobject *kobj,
// /*AFLA*/ 				    const struct attribute_group *grp);
// /*AFLA*/ int __must_check sysfs_create_groups(struct kobject *kobj,
// /*AFLA*/ 				     const struct attribute_group **groups);
// /*AFLA*/ int sysfs_update_group(struct kobject *kobj,
// /*AFLA*/ 		       const struct attribute_group *grp);
// /*AFLA*/ void sysfs_remove_group(struct kobject *kobj,
// /*AFLA*/ 			const struct attribute_group *grp);
// /*AFLA*/ void sysfs_remove_groups(struct kobject *kobj,
// /*AFLA*/ 			 const struct attribute_group **groups);
// /*AFLA*/ int sysfs_add_file_to_group(struct kobject *kobj,
// /*AFLA*/ 			const struct attribute *attr, const char *group);
// /*AFLA*/ void sysfs_remove_file_from_group(struct kobject *kobj,
// /*AFLA*/ 			const struct attribute *attr, const char *group);
// /*AFLA*/ int sysfs_merge_group(struct kobject *kobj,
// /*AFLA*/ 		       const struct attribute_group *grp);
// /*AFLA*/ void sysfs_unmerge_group(struct kobject *kobj,
// /*AFLA*/ 		       const struct attribute_group *grp);
// /*AFLA*/ int sysfs_add_link_to_group(struct kobject *kobj, const char *group_name,
// /*AFLA*/ 			    struct kobject *target, const char *link_name);
// /*AFLA*/ void sysfs_remove_link_from_group(struct kobject *kobj, const char *group_name,
// /*AFLA*/ 				  const char *link_name);
// /*AFLA*/ int __compat_only_sysfs_link_entry_to_kobj(struct kobject *kobj,
// /*AFLA*/ 				      struct kobject *target_kobj,
// /*AFLA*/ 				      const char *target_name);
// /*AFLA*/ 
// /*AFLA*/ void sysfs_notify(struct kobject *kobj, const char *dir, const char *attr);
// /*AFLA*/ 
// /*AFLA*/ int __must_check sysfs_init(void);
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_enable_ns(struct kernfs_node *kn)
// /*AFLA*/ {
// /*AFLA*/ 	return kernfs_enable_ns(kn);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_SYSFS */
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_create_dir_ns(struct kobject *kobj, const void *ns)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_remove_dir(struct kobject *kobj)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_rename_dir_ns(struct kobject *kobj,
// /*AFLA*/ 				      const char *new_name, const void *new_ns)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_move_dir_ns(struct kobject *kobj,
// /*AFLA*/ 				    struct kobject *new_parent_kobj,
// /*AFLA*/ 				    const void *new_ns)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_create_mount_point(struct kobject *parent_kobj,
// /*AFLA*/ 					   const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_remove_mount_point(struct kobject *parent_kobj,
// /*AFLA*/ 					    const char *name)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_create_file_ns(struct kobject *kobj,
// /*AFLA*/ 				       const struct attribute *attr,
// /*AFLA*/ 				       const void *ns)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_create_files(struct kobject *kobj,
// /*AFLA*/ 				    const struct attribute **attr)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_chmod_file(struct kobject *kobj,
// /*AFLA*/ 				   const struct attribute *attr, umode_t mode)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_remove_file_ns(struct kobject *kobj,
// /*AFLA*/ 					const struct attribute *attr,
// /*AFLA*/ 					const void *ns)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool sysfs_remove_file_self(struct kobject *kobj,
// /*AFLA*/ 					  const struct attribute *attr)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_remove_files(struct kobject *kobj,
// /*AFLA*/ 				     const struct attribute **attr)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_create_bin_file(struct kobject *kobj,
// /*AFLA*/ 					const struct bin_attribute *attr)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_remove_bin_file(struct kobject *kobj,
// /*AFLA*/ 					 const struct bin_attribute *attr)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_create_link(struct kobject *kobj,
// /*AFLA*/ 				    struct kobject *target, const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_create_link_nowarn(struct kobject *kobj,
// /*AFLA*/ 					   struct kobject *target,
// /*AFLA*/ 					   const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_remove_link(struct kobject *kobj, const char *name)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_rename_link_ns(struct kobject *k, struct kobject *t,
// /*AFLA*/ 				       const char *old_name,
// /*AFLA*/ 				       const char *new_name, const void *ns)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_delete_link(struct kobject *k, struct kobject *t,
// /*AFLA*/ 				     const char *name)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_create_group(struct kobject *kobj,
// /*AFLA*/ 				     const struct attribute_group *grp)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_create_groups(struct kobject *kobj,
// /*AFLA*/ 				      const struct attribute_group **groups)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_update_group(struct kobject *kobj,
// /*AFLA*/ 				const struct attribute_group *grp)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_remove_group(struct kobject *kobj,
// /*AFLA*/ 				      const struct attribute_group *grp)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_remove_groups(struct kobject *kobj,
// /*AFLA*/ 				       const struct attribute_group **groups)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_add_file_to_group(struct kobject *kobj,
// /*AFLA*/ 		const struct attribute *attr, const char *group)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_remove_file_from_group(struct kobject *kobj,
// /*AFLA*/ 		const struct attribute *attr, const char *group)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_merge_group(struct kobject *kobj,
// /*AFLA*/ 		       const struct attribute_group *grp)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_unmerge_group(struct kobject *kobj,
// /*AFLA*/ 		       const struct attribute_group *grp)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_add_link_to_group(struct kobject *kobj,
// /*AFLA*/ 		const char *group_name, struct kobject *target,
// /*AFLA*/ 		const char *link_name)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_remove_link_from_group(struct kobject *kobj,
// /*AFLA*/ 		const char *group_name, const char *link_name)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __compat_only_sysfs_link_entry_to_kobj(
// /*AFLA*/ 	struct kobject *kobj,
// /*AFLA*/ 	struct kobject *target_kobj,
// /*AFLA*/ 	const char *target_name)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_notify(struct kobject *kobj, const char *dir,
// /*AFLA*/ 				const char *attr)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __must_check sysfs_init(void)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_enable_ns(struct kernfs_node *kn)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_SYSFS */
// /*AFLA*/ 
// /*AFLA*/ static inline int __must_check sysfs_create_file(struct kobject *kobj,
// /*AFLA*/ 						 const struct attribute *attr)
// /*AFLA*/ {
// /*AFLA*/ 	return sysfs_create_file_ns(kobj, attr, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_remove_file(struct kobject *kobj,
// /*AFLA*/ 				     const struct attribute *attr)
// /*AFLA*/ {
// /*AFLA*/ 	sysfs_remove_file_ns(kobj, attr, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysfs_rename_link(struct kobject *kobj, struct kobject *target,
// /*AFLA*/ 				    const char *old_name, const char *new_name)
// /*AFLA*/ {
// /*AFLA*/ 	return sysfs_rename_link_ns(kobj, target, old_name, new_name, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_notify_dirent(struct kernfs_node *kn)
// /*AFLA*/ {
// /*AFLA*/ 	kernfs_notify(kn);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kernfs_node *sysfs_get_dirent(struct kernfs_node *parent,
// /*AFLA*/ 						   const unsigned char *name)
// /*AFLA*/ {
// /*AFLA*/ 	return kernfs_find_and_get(parent, name);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kernfs_node *sysfs_get(struct kernfs_node *kn)
// /*AFLA*/ {
// /*AFLA*/ 	kernfs_get(kn);
// /*AFLA*/ 	return kn;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void sysfs_put(struct kernfs_node *kn)
// /*AFLA*/ {
// /*AFLA*/ 	kernfs_put(kn);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _SYSFS_H_ */
