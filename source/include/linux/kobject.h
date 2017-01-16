// /*AFLA*/ /*
// /*AFLA*/  * kobject.h - generic kernel object infrastructure.
// /*AFLA*/  *
// /*AFLA*/  * Copyright (c) 2002-2003 Patrick Mochel
// /*AFLA*/  * Copyright (c) 2002-2003 Open Source Development Labs
// /*AFLA*/  * Copyright (c) 2006-2008 Greg Kroah-Hartman <greg@kroah.com>
// /*AFLA*/  * Copyright (c) 2006-2008 Novell Inc.
// /*AFLA*/  *
// /*AFLA*/  * This file is released under the GPLv2.
// /*AFLA*/  *
// /*AFLA*/  * Please read Documentation/kobject.txt before using the kobject
// /*AFLA*/  * interface, ESPECIALLY the parts about reference counts and object
// /*AFLA*/  * destructors.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _KOBJECT_H_
// /*AFLA*/ #define _KOBJECT_H_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/sysfs.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/kref.h>
// /*AFLA*/ #include <linux/kobject_ns.h>
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/wait.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/workqueue.h>
// /*AFLA*/ 
// /*AFLA*/ #define UEVENT_HELPER_PATH_LEN		256
// /*AFLA*/ #define UEVENT_NUM_ENVP			32	/* number of env pointers */
// /*AFLA*/ #define UEVENT_BUFFER_SIZE		2048	/* buffer for the variables */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_UEVENT_HELPER
// /*AFLA*/ /* path to the userspace helper executed on an event */
// /*AFLA*/ extern char uevent_helper[];
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* counter to tag the uevent, read only except for the kobject core */
// /*AFLA*/ extern u64 uevent_seqnum;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The actions here must match the index to the string array
// /*AFLA*/  * in lib/kobject_uevent.c
// /*AFLA*/  *
// /*AFLA*/  * Do not add new actions here without checking with the driver-core
// /*AFLA*/  * maintainers. Action strings are not meant to express subsystem
// /*AFLA*/  * or device specific properties. In most cases you want to send a
// /*AFLA*/  * kobject_uevent_env(kobj, KOBJ_CHANGE, env) with additional event
// /*AFLA*/  * specific variables added to the event environment.
// /*AFLA*/  */
// /*AFLA*/ enum kobject_action {
// /*AFLA*/ 	KOBJ_ADD,
// /*AFLA*/ 	KOBJ_REMOVE,
// /*AFLA*/ 	KOBJ_CHANGE,
// /*AFLA*/ 	KOBJ_MOVE,
// /*AFLA*/ 	KOBJ_ONLINE,
// /*AFLA*/ 	KOBJ_OFFLINE,
// /*AFLA*/ 	KOBJ_MAX
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct kobject {
// /*AFLA*/ 	const char		*name;
// /*AFLA*/ 	struct list_head	entry;
// /*AFLA*/ 	struct kobject		*parent;
// /*AFLA*/ 	struct kset		*kset;
// /*AFLA*/ 	struct kobj_type	*ktype;
// /*AFLA*/ 	struct kernfs_node	*sd; /* sysfs directory entry */
// /*AFLA*/ 	struct kref		kref;
// /*AFLA*/ #ifdef CONFIG_DEBUG_KOBJECT_RELEASE
// /*AFLA*/ 	struct delayed_work	release;
// /*AFLA*/ #endif
// /*AFLA*/ 	unsigned int state_initialized:1;
// /*AFLA*/ 	unsigned int state_in_sysfs:1;
// /*AFLA*/ 	unsigned int state_add_uevent_sent:1;
// /*AFLA*/ 	unsigned int state_remove_uevent_sent:1;
// /*AFLA*/ 	unsigned int uevent_suppress:1;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern __printf(2, 3)
// /*AFLA*/ int kobject_set_name(struct kobject *kobj, const char *name, ...);
// /*AFLA*/ extern __printf(2, 0)
// /*AFLA*/ int kobject_set_name_vargs(struct kobject *kobj, const char *fmt,
// /*AFLA*/ 			   va_list vargs);
// /*AFLA*/ 
// /*AFLA*/ static inline const char *kobject_name(const struct kobject *kobj)
// /*AFLA*/ {
// /*AFLA*/ 	return kobj->name;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void kobject_init(struct kobject *kobj, struct kobj_type *ktype);
// /*AFLA*/ extern __printf(3, 4) __must_check
// /*AFLA*/ int kobject_add(struct kobject *kobj, struct kobject *parent,
// /*AFLA*/ 		const char *fmt, ...);
// /*AFLA*/ extern __printf(4, 5) __must_check
// /*AFLA*/ int kobject_init_and_add(struct kobject *kobj,
// /*AFLA*/ 			 struct kobj_type *ktype, struct kobject *parent,
// /*AFLA*/ 			 const char *fmt, ...);
// /*AFLA*/ 
// /*AFLA*/ extern void kobject_del(struct kobject *kobj);
// /*AFLA*/ 
// /*AFLA*/ extern struct kobject * __must_check kobject_create(void);
// /*AFLA*/ extern struct kobject * __must_check kobject_create_and_add(const char *name,
// /*AFLA*/ 						struct kobject *parent);
// /*AFLA*/ 
// /*AFLA*/ extern int __must_check kobject_rename(struct kobject *, const char *new_name);
// /*AFLA*/ extern int __must_check kobject_move(struct kobject *, struct kobject *);
// /*AFLA*/ 
// /*AFLA*/ extern struct kobject *kobject_get(struct kobject *kobj);
// /*AFLA*/ extern void kobject_put(struct kobject *kobj);
// /*AFLA*/ 
// /*AFLA*/ extern const void *kobject_namespace(struct kobject *kobj);
// /*AFLA*/ extern char *kobject_get_path(struct kobject *kobj, gfp_t flag);
// /*AFLA*/ 
// /*AFLA*/ struct kobj_type {
// /*AFLA*/ 	void (*release)(struct kobject *kobj);
// /*AFLA*/ 	const struct sysfs_ops *sysfs_ops;
// /*AFLA*/ 	struct attribute **default_attrs;
// /*AFLA*/ 	const struct kobj_ns_type_operations *(*child_ns_type)(struct kobject *kobj);
// /*AFLA*/ 	const void *(*namespace)(struct kobject *kobj);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct kobj_uevent_env {
// /*AFLA*/ 	char *argv[3];
// /*AFLA*/ 	char *envp[UEVENT_NUM_ENVP];
// /*AFLA*/ 	int envp_idx;
// /*AFLA*/ 	char buf[UEVENT_BUFFER_SIZE];
// /*AFLA*/ 	int buflen;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct kset_uevent_ops {
// /*AFLA*/ 	int (* const filter)(struct kset *kset, struct kobject *kobj);
// /*AFLA*/ 	const char *(* const name)(struct kset *kset, struct kobject *kobj);
// /*AFLA*/ 	int (* const uevent)(struct kset *kset, struct kobject *kobj,
// /*AFLA*/ 		      struct kobj_uevent_env *env);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct kobj_attribute {
// /*AFLA*/ 	struct attribute attr;
// /*AFLA*/ 	ssize_t (*show)(struct kobject *kobj, struct kobj_attribute *attr,
// /*AFLA*/ 			char *buf);
// /*AFLA*/ 	ssize_t (*store)(struct kobject *kobj, struct kobj_attribute *attr,
// /*AFLA*/ 			 const char *buf, size_t count);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern const struct sysfs_ops kobj_sysfs_ops;
// /*AFLA*/ 
// /*AFLA*/ struct sock;
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct kset - a set of kobjects of a specific type, belonging to a specific subsystem.
// /*AFLA*/  *
// /*AFLA*/  * A kset defines a group of kobjects.  They can be individually
// /*AFLA*/  * different "types" but overall these kobjects all want to be grouped
// /*AFLA*/  * together and operated on in the same manner.  ksets are used to
// /*AFLA*/  * define the attribute callbacks and other common events that happen to
// /*AFLA*/  * a kobject.
// /*AFLA*/  *
// /*AFLA*/  * @list: the list of all kobjects for this kset
// /*AFLA*/  * @list_lock: a lock for iterating over the kobjects
// /*AFLA*/  * @kobj: the embedded kobject for this kset (recursion, isn't it fun...)
// /*AFLA*/  * @uevent_ops: the set of uevent operations for this kset.  These are
// /*AFLA*/  * called whenever a kobject has something happen to it so that the kset
// /*AFLA*/  * can add new environment variables, or filter out the uevents if so
// /*AFLA*/  * desired.
// /*AFLA*/  */
// /*AFLA*/ struct kset {
// /*AFLA*/ 	struct list_head list;
// /*AFLA*/ 	spinlock_t list_lock;
// /*AFLA*/ 	struct kobject kobj;
// /*AFLA*/ 	const struct kset_uevent_ops *uevent_ops;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern void kset_init(struct kset *kset);
// /*AFLA*/ extern int __must_check kset_register(struct kset *kset);
// /*AFLA*/ extern void kset_unregister(struct kset *kset);
// /*AFLA*/ extern struct kset * __must_check kset_create_and_add(const char *name,
// /*AFLA*/ 						const struct kset_uevent_ops *u,
// /*AFLA*/ 						struct kobject *parent_kobj);
// /*AFLA*/ 
// /*AFLA*/ static inline struct kset *to_kset(struct kobject *kobj)
// /*AFLA*/ {
// /*AFLA*/ 	return kobj ? container_of(kobj, struct kset, kobj) : NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kset *kset_get(struct kset *k)
// /*AFLA*/ {
// /*AFLA*/ 	return k ? to_kset(kobject_get(&k->kobj)) : NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void kset_put(struct kset *k)
// /*AFLA*/ {
// /*AFLA*/ 	kobject_put(&k->kobj);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct kobj_type *get_ktype(struct kobject *kobj)
// /*AFLA*/ {
// /*AFLA*/ 	return kobj->ktype;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct kobject *kset_find_obj(struct kset *, const char *);
// /*AFLA*/ 
// /*AFLA*/ /* The global /sys/kernel/ kobject for people to chain off of */
// /*AFLA*/ extern struct kobject *kernel_kobj;
// /*AFLA*/ /* The global /sys/kernel/mm/ kobject for people to chain off of */
// /*AFLA*/ extern struct kobject *mm_kobj;
// /*AFLA*/ /* The global /sys/hypervisor/ kobject for people to chain off of */
// /*AFLA*/ extern struct kobject *hypervisor_kobj;
// /*AFLA*/ /* The global /sys/power/ kobject for people to chain off of */
// /*AFLA*/ extern struct kobject *power_kobj;
// /*AFLA*/ /* The global /sys/firmware/ kobject for people to chain off of */
// /*AFLA*/ extern struct kobject *firmware_kobj;
// /*AFLA*/ 
// /*AFLA*/ int kobject_uevent(struct kobject *kobj, enum kobject_action action);
// /*AFLA*/ int kobject_uevent_env(struct kobject *kobj, enum kobject_action action,
// /*AFLA*/ 			char *envp[]);
// /*AFLA*/ 
// /*AFLA*/ __printf(2, 3)
// /*AFLA*/ int add_uevent_var(struct kobj_uevent_env *env, const char *format, ...);
// /*AFLA*/ 
// /*AFLA*/ int kobject_action_type(const char *buf, size_t count,
// /*AFLA*/ 			enum kobject_action *type);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _KOBJECT_H_ */
