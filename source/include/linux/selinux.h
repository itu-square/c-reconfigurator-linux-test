// /*AFLA*/ /*
// /*AFLA*/  * SELinux services exported to the rest of the kernel.
// /*AFLA*/  *
// /*AFLA*/  * Author: James Morris <jmorris@redhat.com>
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2005 Red Hat, Inc., James Morris <jmorris@redhat.com>
// /*AFLA*/  * Copyright (C) 2006 Trusted Computer Solutions, Inc. <dgoeddel@trustedcs.com>
// /*AFLA*/  * Copyright (C) 2006 IBM Corporation, Timothy R. Chavez <tinytim@us.ibm.com>
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify
// /*AFLA*/  * it under the terms of the GNU General Public License version 2,
// /*AFLA*/  * as published by the Free Software Foundation.
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LINUX_SELINUX_H
// /*AFLA*/ #define _LINUX_SELINUX_H
// /*AFLA*/ 
// /*AFLA*/ struct selinux_audit_rule;
// /*AFLA*/ struct audit_context;
// /*AFLA*/ struct kern_ipc_perm;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SECURITY_SELINUX
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * selinux_is_enabled - is SELinux enabled?
// /*AFLA*/  */
// /*AFLA*/ bool selinux_is_enabled(void);
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline bool selinux_is_enabled(void)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ #endif	/* CONFIG_SECURITY_SELINUX */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_SELINUX_H */
