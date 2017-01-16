// /*AFLA*/ /* Authentication token and access key management
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2004, 2007 Red Hat, Inc. All Rights Reserved.
// /*AFLA*/  * Written by David Howells (dhowells@redhat.com)
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or
// /*AFLA*/  * modify it under the terms of the GNU General Public License
// /*AFLA*/  * as published by the Free Software Foundation; either version
// /*AFLA*/  * 2 of the License, or (at your option) any later version.
// /*AFLA*/  *
// /*AFLA*/  *
// /*AFLA*/  * See Documentation/security/keys.txt for information on keys/keyrings.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_KEY_H
// /*AFLA*/ #define _LINUX_KEY_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/rbtree.h>
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ #include <linux/sysctl.h>
// /*AFLA*/ #include <linux/rwsem.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/assoc_array.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef __KERNEL__
// /*AFLA*/ #include <linux/uidgid.h>
// /*AFLA*/ 
// /*AFLA*/ /* key handle serial number */
// /*AFLA*/ typedef int32_t key_serial_t;
// /*AFLA*/ 
// /*AFLA*/ /* key handle permissions mask */
// /*AFLA*/ typedef uint32_t key_perm_t;
// /*AFLA*/ 
// /*AFLA*/ struct key;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KEYS
// /*AFLA*/ 
// /*AFLA*/ #undef KEY_DEBUGGING
// /*AFLA*/ 
// /*AFLA*/ #define KEY_POS_VIEW	0x01000000	/* possessor can view a key's attributes */
// /*AFLA*/ #define KEY_POS_READ	0x02000000	/* possessor can read key payload / view keyring */
// /*AFLA*/ #define KEY_POS_WRITE	0x04000000	/* possessor can update key payload / add link to keyring */
// /*AFLA*/ #define KEY_POS_SEARCH	0x08000000	/* possessor can find a key in search / search a keyring */
// /*AFLA*/ #define KEY_POS_LINK	0x10000000	/* possessor can create a link to a key/keyring */
// /*AFLA*/ #define KEY_POS_SETATTR	0x20000000	/* possessor can set key attributes */
// /*AFLA*/ #define KEY_POS_ALL	0x3f000000
// /*AFLA*/ 
// /*AFLA*/ #define KEY_USR_VIEW	0x00010000	/* user permissions... */
// /*AFLA*/ #define KEY_USR_READ	0x00020000
// /*AFLA*/ #define KEY_USR_WRITE	0x00040000
// /*AFLA*/ #define KEY_USR_SEARCH	0x00080000
// /*AFLA*/ #define KEY_USR_LINK	0x00100000
// /*AFLA*/ #define KEY_USR_SETATTR	0x00200000
// /*AFLA*/ #define KEY_USR_ALL	0x003f0000
// /*AFLA*/ 
// /*AFLA*/ #define KEY_GRP_VIEW	0x00000100	/* group permissions... */
// /*AFLA*/ #define KEY_GRP_READ	0x00000200
// /*AFLA*/ #define KEY_GRP_WRITE	0x00000400
// /*AFLA*/ #define KEY_GRP_SEARCH	0x00000800
// /*AFLA*/ #define KEY_GRP_LINK	0x00001000
// /*AFLA*/ #define KEY_GRP_SETATTR	0x00002000
// /*AFLA*/ #define KEY_GRP_ALL	0x00003f00
// /*AFLA*/ 
// /*AFLA*/ #define KEY_OTH_VIEW	0x00000001	/* third party permissions... */
// /*AFLA*/ #define KEY_OTH_READ	0x00000002
// /*AFLA*/ #define KEY_OTH_WRITE	0x00000004
// /*AFLA*/ #define KEY_OTH_SEARCH	0x00000008
// /*AFLA*/ #define KEY_OTH_LINK	0x00000010
// /*AFLA*/ #define KEY_OTH_SETATTR	0x00000020
// /*AFLA*/ #define KEY_OTH_ALL	0x0000003f
// /*AFLA*/ 
// /*AFLA*/ #define KEY_PERM_UNDEF	0xffffffff
// /*AFLA*/ 
// /*AFLA*/ struct seq_file;
// /*AFLA*/ struct user_struct;
// /*AFLA*/ struct signal_struct;
// /*AFLA*/ struct cred;
// /*AFLA*/ 
// /*AFLA*/ struct key_type;
// /*AFLA*/ struct key_owner;
// /*AFLA*/ struct keyring_list;
// /*AFLA*/ struct keyring_name;
// /*AFLA*/ 
// /*AFLA*/ struct keyring_index_key {
// /*AFLA*/ 	struct key_type		*type;
// /*AFLA*/ 	const char		*description;
// /*AFLA*/ 	size_t			desc_len;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ union key_payload {
// /*AFLA*/ 	void __rcu		*rcu_data0;
// /*AFLA*/ 	void			*data[4];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*****************************************************************************/
// /*AFLA*/ /*
// /*AFLA*/  * key reference with possession attribute handling
// /*AFLA*/  *
// /*AFLA*/  * NOTE! key_ref_t is a typedef'd pointer to a type that is not actually
// /*AFLA*/  * defined. This is because we abuse the bottom bit of the reference to carry a
// /*AFLA*/  * flag to indicate whether the calling process possesses that key in one of
// /*AFLA*/  * its keyrings.
// /*AFLA*/  *
// /*AFLA*/  * the key_ref_t has been made a separate type so that the compiler can reject
// /*AFLA*/  * attempts to dereference it without proper conversion.
// /*AFLA*/  *
// /*AFLA*/  * the three functions are used to assemble and disassemble references
// /*AFLA*/  */
// /*AFLA*/ typedef struct __key_reference_with_attributes *key_ref_t;
// /*AFLA*/ 
// /*AFLA*/ static inline key_ref_t make_key_ref(const struct key *key,
// /*AFLA*/ 				     bool possession)
// /*AFLA*/ {
// /*AFLA*/ 	return (key_ref_t) ((unsigned long) key | possession);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct key *key_ref_to_ptr(const key_ref_t key_ref)
// /*AFLA*/ {
// /*AFLA*/ 	return (struct key *) ((unsigned long) key_ref & ~1UL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool is_key_possessed(const key_ref_t key_ref)
// /*AFLA*/ {
// /*AFLA*/ 	return (unsigned long) key_ref & 1UL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*****************************************************************************/
// /*AFLA*/ /*
// /*AFLA*/  * authentication token / access credential / keyring
// /*AFLA*/  * - types of key include:
// /*AFLA*/  *   - keyrings
// /*AFLA*/  *   - disk encryption IDs
// /*AFLA*/  *   - Kerberos TGTs and tickets
// /*AFLA*/  */
// /*AFLA*/ struct key {
// /*AFLA*/ 	atomic_t		usage;		/* number of references */
// /*AFLA*/ 	key_serial_t		serial;		/* key serial number */
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct list_head graveyard_link;
// /*AFLA*/ 		struct rb_node	serial_node;
// /*AFLA*/ 	};
// /*AFLA*/ 	struct rw_semaphore	sem;		/* change vs change sem */
// /*AFLA*/ 	struct key_user		*user;		/* owner of this key */
// /*AFLA*/ 	void			*security;	/* security data for this key */
// /*AFLA*/ 	union {
// /*AFLA*/ 		time_t		expiry;		/* time at which key expires (or 0) */
// /*AFLA*/ 		time_t		revoked_at;	/* time at which key was revoked */
// /*AFLA*/ 	};
// /*AFLA*/ 	time_t			last_used_at;	/* last time used for LRU keyring discard */
// /*AFLA*/ 	kuid_t			uid;
// /*AFLA*/ 	kgid_t			gid;
// /*AFLA*/ 	key_perm_t		perm;		/* access permissions */
// /*AFLA*/ 	unsigned short		quotalen;	/* length added to quota */
// /*AFLA*/ 	unsigned short		datalen;	/* payload data length
// /*AFLA*/ 						 * - may not match RCU dereferenced payload
// /*AFLA*/ 						 * - payload should contain own length
// /*AFLA*/ 						 */
// /*AFLA*/ 
// /*AFLA*/ #ifdef KEY_DEBUGGING
// /*AFLA*/ 	unsigned		magic;
// /*AFLA*/ #define KEY_DEBUG_MAGIC		0x18273645u
// /*AFLA*/ #define KEY_DEBUG_MAGIC_X	0xf8e9dacbu
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long		flags;		/* status flags (change with bitops) */
// /*AFLA*/ #define KEY_FLAG_INSTANTIATED	0	/* set if key has been instantiated */
// /*AFLA*/ #define KEY_FLAG_DEAD		1	/* set if key type has been deleted */
// /*AFLA*/ #define KEY_FLAG_REVOKED	2	/* set if key had been revoked */
// /*AFLA*/ #define KEY_FLAG_IN_QUOTA	3	/* set if key consumes quota */
// /*AFLA*/ #define KEY_FLAG_USER_CONSTRUCT	4	/* set if key is being constructed in userspace */
// /*AFLA*/ #define KEY_FLAG_NEGATIVE	5	/* set if key is negative */
// /*AFLA*/ #define KEY_FLAG_ROOT_CAN_CLEAR	6	/* set if key can be cleared by root without permission */
// /*AFLA*/ #define KEY_FLAG_INVALIDATED	7	/* set if key has been invalidated */
// /*AFLA*/ #define KEY_FLAG_BUILTIN	8	/* set if key is built in to the kernel */
// /*AFLA*/ #define KEY_FLAG_ROOT_CAN_INVAL	9	/* set if key can be invalidated by root without permission */
// /*AFLA*/ #define KEY_FLAG_KEEP		10	/* set if key should not be removed */
// /*AFLA*/ 
// /*AFLA*/ 	/* the key type and key description string
// /*AFLA*/ 	 * - the desc is used to match a key against search criteria
// /*AFLA*/ 	 * - it should be a printable string
// /*AFLA*/ 	 * - eg: for krb5 AFS, this might be "afs@REDHAT.COM"
// /*AFLA*/ 	 */
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct keyring_index_key index_key;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			struct key_type	*type;		/* type of key */
// /*AFLA*/ 			char		*description;
// /*AFLA*/ 		};
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ 	/* key data
// /*AFLA*/ 	 * - this is used to hold the data actually used in cryptography or
// /*AFLA*/ 	 *   whatever
// /*AFLA*/ 	 */
// /*AFLA*/ 	union {
// /*AFLA*/ 		union key_payload payload;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			/* Keyring bits */
// /*AFLA*/ 			struct list_head name_link;
// /*AFLA*/ 			struct assoc_array keys;
// /*AFLA*/ 		};
// /*AFLA*/ 		int reject_error;
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ 	/* This is set on a keyring to restrict the addition of a link to a key
// /*AFLA*/ 	 * to it.  If this method isn't provided then it is assumed that the
// /*AFLA*/ 	 * keyring is open to any addition.  It is ignored for non-keyring
// /*AFLA*/ 	 * keys.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * This is intended for use with rings of trusted keys whereby addition
// /*AFLA*/ 	 * to the keyring needs to be controlled.  KEY_ALLOC_BYPASS_RESTRICTION
// /*AFLA*/ 	 * overrides this, allowing the kernel to add extra keys without
// /*AFLA*/ 	 * restriction.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int (*restrict_link)(struct key *keyring,
// /*AFLA*/ 			     const struct key_type *type,
// /*AFLA*/ 			     const union key_payload *payload);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern struct key *key_alloc(struct key_type *type,
// /*AFLA*/ 			     const char *desc,
// /*AFLA*/ 			     kuid_t uid, kgid_t gid,
// /*AFLA*/ 			     const struct cred *cred,
// /*AFLA*/ 			     key_perm_t perm,
// /*AFLA*/ 			     unsigned long flags,
// /*AFLA*/ 			     int (*restrict_link)(struct key *,
// /*AFLA*/ 						  const struct key_type *,
// /*AFLA*/ 						  const union key_payload *));
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define KEY_ALLOC_IN_QUOTA		0x0000	/* add to quota, reject if would overrun */
// /*AFLA*/ #define KEY_ALLOC_QUOTA_OVERRUN		0x0001	/* add to quota, permit even if overrun */
// /*AFLA*/ #define KEY_ALLOC_NOT_IN_QUOTA		0x0002	/* not in quota */
// /*AFLA*/ #define KEY_ALLOC_BUILT_IN		0x0004	/* Key is built into kernel */
// /*AFLA*/ #define KEY_ALLOC_BYPASS_RESTRICTION	0x0008	/* Override the check on restricted keyrings */
// /*AFLA*/ 
// /*AFLA*/ extern void key_revoke(struct key *key);
// /*AFLA*/ extern void key_invalidate(struct key *key);
// /*AFLA*/ extern void key_put(struct key *key);
// /*AFLA*/ 
// /*AFLA*/ static inline struct key *__key_get(struct key *key)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_inc(&key->usage);
// /*AFLA*/ 	return key;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct key *key_get(struct key *key)
// /*AFLA*/ {
// /*AFLA*/ 	return key ? __key_get(key) : key;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void key_ref_put(key_ref_t key_ref)
// /*AFLA*/ {
// /*AFLA*/ 	key_put(key_ref_to_ptr(key_ref));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct key *request_key(struct key_type *type,
// /*AFLA*/ 			       const char *description,
// /*AFLA*/ 			       const char *callout_info);
// /*AFLA*/ 
// /*AFLA*/ extern struct key *request_key_with_auxdata(struct key_type *type,
// /*AFLA*/ 					    const char *description,
// /*AFLA*/ 					    const void *callout_info,
// /*AFLA*/ 					    size_t callout_len,
// /*AFLA*/ 					    void *aux);
// /*AFLA*/ 
// /*AFLA*/ extern struct key *request_key_async(struct key_type *type,
// /*AFLA*/ 				     const char *description,
// /*AFLA*/ 				     const void *callout_info,
// /*AFLA*/ 				     size_t callout_len);
// /*AFLA*/ 
// /*AFLA*/ extern struct key *request_key_async_with_auxdata(struct key_type *type,
// /*AFLA*/ 						  const char *description,
// /*AFLA*/ 						  const void *callout_info,
// /*AFLA*/ 						  size_t callout_len,
// /*AFLA*/ 						  void *aux);
// /*AFLA*/ 
// /*AFLA*/ extern int wait_for_key_construction(struct key *key, bool intr);
// /*AFLA*/ 
// /*AFLA*/ extern int key_validate(const struct key *key);
// /*AFLA*/ 
// /*AFLA*/ extern key_ref_t key_create_or_update(key_ref_t keyring,
// /*AFLA*/ 				      const char *type,
// /*AFLA*/ 				      const char *description,
// /*AFLA*/ 				      const void *payload,
// /*AFLA*/ 				      size_t plen,
// /*AFLA*/ 				      key_perm_t perm,
// /*AFLA*/ 				      unsigned long flags);
// /*AFLA*/ 
// /*AFLA*/ extern int key_update(key_ref_t key,
// /*AFLA*/ 		      const void *payload,
// /*AFLA*/ 		      size_t plen);
// /*AFLA*/ 
// /*AFLA*/ extern int key_link(struct key *keyring,
// /*AFLA*/ 		    struct key *key);
// /*AFLA*/ 
// /*AFLA*/ extern int key_unlink(struct key *keyring,
// /*AFLA*/ 		      struct key *key);
// /*AFLA*/ 
// /*AFLA*/ extern struct key *keyring_alloc(const char *description, kuid_t uid, kgid_t gid,
// /*AFLA*/ 				 const struct cred *cred,
// /*AFLA*/ 				 key_perm_t perm,
// /*AFLA*/ 				 unsigned long flags,
// /*AFLA*/ 				 int (*restrict_link)(struct key *,
// /*AFLA*/ 						      const struct key_type *,
// /*AFLA*/ 						      const union key_payload *),
// /*AFLA*/ 				 struct key *dest);
// /*AFLA*/ 
// /*AFLA*/ extern int restrict_link_reject(struct key *keyring,
// /*AFLA*/ 				const struct key_type *type,
// /*AFLA*/ 				const union key_payload *payload);
// /*AFLA*/ 
// /*AFLA*/ extern int keyring_clear(struct key *keyring);
// /*AFLA*/ 
// /*AFLA*/ extern key_ref_t keyring_search(key_ref_t keyring,
// /*AFLA*/ 				struct key_type *type,
// /*AFLA*/ 				const char *description);
// /*AFLA*/ 
// /*AFLA*/ extern int keyring_add_key(struct key *keyring,
// /*AFLA*/ 			   struct key *key);
// /*AFLA*/ 
// /*AFLA*/ extern struct key *key_lookup(key_serial_t id);
// /*AFLA*/ 
// /*AFLA*/ static inline key_serial_t key_serial(const struct key *key)
// /*AFLA*/ {
// /*AFLA*/ 	return key ? key->serial : 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void key_set_timeout(struct key *, unsigned);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The permissions required on a key that we're looking up.
// /*AFLA*/  */
// /*AFLA*/ #define	KEY_NEED_VIEW	0x01	/* Require permission to view attributes */
// /*AFLA*/ #define	KEY_NEED_READ	0x02	/* Require permission to read content */
// /*AFLA*/ #define	KEY_NEED_WRITE	0x04	/* Require permission to update / modify */
// /*AFLA*/ #define	KEY_NEED_SEARCH	0x08	/* Require permission to search (keyring) or find (key) */
// /*AFLA*/ #define	KEY_NEED_LINK	0x10	/* Require permission to link */
// /*AFLA*/ #define	KEY_NEED_SETATTR 0x20	/* Require permission to change attributes */
// /*AFLA*/ #define	KEY_NEED_ALL	0x3f	/* All the above permissions */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * key_is_instantiated - Determine if a key has been positively instantiated
// /*AFLA*/  * @key: The key to check.
// /*AFLA*/  *
// /*AFLA*/  * Return true if the specified key has been positively instantiated, false
// /*AFLA*/  * otherwise.
// /*AFLA*/  */
// /*AFLA*/ static inline bool key_is_instantiated(const struct key *key)
// /*AFLA*/ {
// /*AFLA*/ 	return test_bit(KEY_FLAG_INSTANTIATED, &key->flags) &&
// /*AFLA*/ 		!test_bit(KEY_FLAG_NEGATIVE, &key->flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define rcu_dereference_key(KEY)					\
// /*AFLA*/ 	(rcu_dereference_protected((KEY)->payload.rcu_data0,		\
// /*AFLA*/ 				   rwsem_is_locked(&((struct key *)(KEY))->sem)))
// /*AFLA*/ 
// /*AFLA*/ #define rcu_assign_keypointer(KEY, PAYLOAD)				\
// /*AFLA*/ do {									\
// /*AFLA*/ 	rcu_assign_pointer((KEY)->payload.rcu_data0, (PAYLOAD));	\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SYSCTL
// /*AFLA*/ extern struct ctl_table key_sysctls[];
// /*AFLA*/ #endif
// /*AFLA*/ /*
// /*AFLA*/  * the userspace interface
// /*AFLA*/  */
// /*AFLA*/ extern int install_thread_keyring_to_cred(struct cred *cred);
// /*AFLA*/ extern void key_fsuid_changed(struct task_struct *tsk);
// /*AFLA*/ extern void key_fsgid_changed(struct task_struct *tsk);
// /*AFLA*/ extern void key_init(void);
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_KEYS */
// /*AFLA*/ 
// /*AFLA*/ #define key_validate(k)			0
// /*AFLA*/ #define key_serial(k)			0
// /*AFLA*/ #define key_get(k) 			({ NULL; })
// /*AFLA*/ #define key_revoke(k)			do { } while(0)
// /*AFLA*/ #define key_invalidate(k)		do { } while(0)
// /*AFLA*/ #define key_put(k)			do { } while(0)
// /*AFLA*/ #define key_ref_put(k)			do { } while(0)
// /*AFLA*/ #define make_key_ref(k, p)		NULL
// /*AFLA*/ #define key_ref_to_ptr(k)		NULL
// /*AFLA*/ #define is_key_possessed(k)		0
// /*AFLA*/ #define key_fsuid_changed(t)		do { } while(0)
// /*AFLA*/ #define key_fsgid_changed(t)		do { } while(0)
// /*AFLA*/ #define key_init()			do { } while(0)
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_KEYS */
// /*AFLA*/ #endif /* __KERNEL__ */
// /*AFLA*/ #endif /* _LINUX_KEY_H */
