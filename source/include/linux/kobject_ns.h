// /*AFLA*/ /* Kernel object name space definitions
// /*AFLA*/  *
// /*AFLA*/  * Copyright (c) 2002-2003 Patrick Mochel
// /*AFLA*/  * Copyright (c) 2002-2003 Open Source Development Labs
// /*AFLA*/  * Copyright (c) 2006-2008 Greg Kroah-Hartman <greg@kroah.com>
// /*AFLA*/  * Copyright (c) 2006-2008 Novell Inc.
// /*AFLA*/  *
// /*AFLA*/  * Split from kobject.h by David Howells (dhowells@redhat.com)
// /*AFLA*/  *
// /*AFLA*/  * This file is released under the GPLv2.
// /*AFLA*/  *
// /*AFLA*/  * Please read Documentation/kobject.txt before using the kobject
// /*AFLA*/  * interface, ESPECIALLY the parts about reference counts and object
// /*AFLA*/  * destructors.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_KOBJECT_NS_H
// /*AFLA*/ #define _LINUX_KOBJECT_NS_H
// /*AFLA*/ 
// /*AFLA*/ struct sock;
// /*AFLA*/ struct kobject;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Namespace types which are used to tag kobjects and sysfs entries.
// /*AFLA*/  * Network namespace will likely be the first.
// /*AFLA*/  */
// /*AFLA*/ enum kobj_ns_type {
// /*AFLA*/ 	KOBJ_NS_TYPE_NONE = 0,
// /*AFLA*/ 	KOBJ_NS_TYPE_NET,
// /*AFLA*/ 	KOBJ_NS_TYPES
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Callbacks so sysfs can determine namespaces
// /*AFLA*/  *   @grab_current_ns: return a new reference to calling task's namespace
// /*AFLA*/  *   @netlink_ns: return namespace to which a sock belongs (right?)
// /*AFLA*/  *   @initial_ns: return the initial namespace (i.e. init_net_ns)
// /*AFLA*/  *   @drop_ns: drops a reference to namespace
// /*AFLA*/  */
// /*AFLA*/ struct kobj_ns_type_operations {
// /*AFLA*/ 	enum kobj_ns_type type;
// /*AFLA*/ 	bool (*current_may_mount)(void);
// /*AFLA*/ 	void *(*grab_current_ns)(void);
// /*AFLA*/ 	const void *(*netlink_ns)(struct sock *sk);
// /*AFLA*/ 	const void *(*initial_ns)(void);
// /*AFLA*/ 	void (*drop_ns)(void *);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ int kobj_ns_type_register(const struct kobj_ns_type_operations *ops);
// /*AFLA*/ int kobj_ns_type_registered(enum kobj_ns_type type);
// /*AFLA*/ const struct kobj_ns_type_operations *kobj_child_ns_ops(struct kobject *parent);
// /*AFLA*/ const struct kobj_ns_type_operations *kobj_ns_ops(struct kobject *kobj);
// /*AFLA*/ 
// /*AFLA*/ bool kobj_ns_current_may_mount(enum kobj_ns_type type);
// /*AFLA*/ void *kobj_ns_grab_current(enum kobj_ns_type type);
// /*AFLA*/ const void *kobj_ns_netlink(enum kobj_ns_type type, struct sock *sk);
// /*AFLA*/ const void *kobj_ns_initial(enum kobj_ns_type type);
// /*AFLA*/ void kobj_ns_drop(enum kobj_ns_type type, void *ns);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_KOBJECT_NS_H */
