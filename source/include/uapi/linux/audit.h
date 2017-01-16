// /*AFLA*/ /* audit.h -- Auditing support
// /*AFLA*/  *
// /*AFLA*/  * Copyright 2003-2004 Red Hat Inc., Durham, North Carolina.
// /*AFLA*/  * All Rights Reserved.
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify
// /*AFLA*/  * it under the terms of the GNU General Public License as published by
// /*AFLA*/  * the Free Software Foundation; either version 2 of the License, or
// /*AFLA*/  * (at your option) any later version.
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it will be useful,
// /*AFLA*/  * but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/  * GNU General Public License for more details.
// /*AFLA*/  *
// /*AFLA*/  * You should have received a copy of the GNU General Public License
// /*AFLA*/  * along with this program; if not, write to the Free Software
// /*AFLA*/  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// /*AFLA*/  *
// /*AFLA*/  * Written by Rickard E. (Rik) Faith <faith@redhat.com>
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _UAPI_LINUX_AUDIT_H_
// /*AFLA*/ #define _UAPI_LINUX_AUDIT_H_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/elf-em.h>
// /*AFLA*/ 
// /*AFLA*/ /* The netlink messages for the audit system is divided into blocks:
// /*AFLA*/  * 1000 - 1099 are for commanding the audit system
// /*AFLA*/  * 1100 - 1199 user space trusted application messages
// /*AFLA*/  * 1200 - 1299 messages internal to the audit daemon
// /*AFLA*/  * 1300 - 1399 audit event messages
// /*AFLA*/  * 1400 - 1499 SE Linux use
// /*AFLA*/  * 1500 - 1599 kernel LSPP events
// /*AFLA*/  * 1600 - 1699 kernel crypto events
// /*AFLA*/  * 1700 - 1799 kernel anomaly records
// /*AFLA*/  * 1800 - 1899 kernel integrity events
// /*AFLA*/  * 1900 - 1999 future kernel use
// /*AFLA*/  * 2000 is for otherwise unclassified kernel audit messages (legacy)
// /*AFLA*/  * 2001 - 2099 unused (kernel)
// /*AFLA*/  * 2100 - 2199 user space anomaly records
// /*AFLA*/  * 2200 - 2299 user space actions taken in response to anomalies
// /*AFLA*/  * 2300 - 2399 user space generated LSPP events
// /*AFLA*/  * 2400 - 2499 user space crypto events
// /*AFLA*/  * 2500 - 2999 future user space (maybe integrity labels and related events)
// /*AFLA*/  *
// /*AFLA*/  * Messages from 1000-1199 are bi-directional. 1200-1299 & 2100 - 2999 are
// /*AFLA*/  * exclusively user space. 1300-2099 is kernel --> user space 
// /*AFLA*/  * communication.
// /*AFLA*/  */
// /*AFLA*/ #define AUDIT_GET		1000	/* Get status */
// /*AFLA*/ #define AUDIT_SET		1001	/* Set status (enable/disable/auditd) */
// /*AFLA*/ #define AUDIT_LIST		1002	/* List syscall rules -- deprecated */
// /*AFLA*/ #define AUDIT_ADD		1003	/* Add syscall rule -- deprecated */
// /*AFLA*/ #define AUDIT_DEL		1004	/* Delete syscall rule -- deprecated */
// /*AFLA*/ #define AUDIT_USER		1005	/* Message from userspace -- deprecated */
// /*AFLA*/ #define AUDIT_LOGIN		1006	/* Define the login id and information */
// /*AFLA*/ #define AUDIT_WATCH_INS		1007	/* Insert file/dir watch entry */
// /*AFLA*/ #define AUDIT_WATCH_REM		1008	/* Remove file/dir watch entry */
// /*AFLA*/ #define AUDIT_WATCH_LIST	1009	/* List all file/dir watches */
// /*AFLA*/ #define AUDIT_SIGNAL_INFO	1010	/* Get info about sender of signal to auditd */
// /*AFLA*/ #define AUDIT_ADD_RULE		1011	/* Add syscall filtering rule */
// /*AFLA*/ #define AUDIT_DEL_RULE		1012	/* Delete syscall filtering rule */
// /*AFLA*/ #define AUDIT_LIST_RULES	1013	/* List syscall filtering rules */
// /*AFLA*/ #define AUDIT_TRIM		1014	/* Trim junk from watched tree */
// /*AFLA*/ #define AUDIT_MAKE_EQUIV	1015	/* Append to watched tree */
// /*AFLA*/ #define AUDIT_TTY_GET		1016	/* Get TTY auditing status */
// /*AFLA*/ #define AUDIT_TTY_SET		1017	/* Set TTY auditing status */
// /*AFLA*/ #define AUDIT_SET_FEATURE	1018	/* Turn an audit feature on or off */
// /*AFLA*/ #define AUDIT_GET_FEATURE	1019	/* Get which features are enabled */
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_FIRST_USER_MSG	1100	/* Userspace messages mostly uninteresting to kernel */
// /*AFLA*/ #define AUDIT_USER_AVC		1107	/* We filter this differently */
// /*AFLA*/ #define AUDIT_USER_TTY		1124	/* Non-ICANON TTY input meaning */
// /*AFLA*/ #define AUDIT_LAST_USER_MSG	1199
// /*AFLA*/ #define AUDIT_FIRST_USER_MSG2	2100	/* More user space messages */
// /*AFLA*/ #define AUDIT_LAST_USER_MSG2	2999
// /*AFLA*/  
// /*AFLA*/ #define AUDIT_DAEMON_START      1200    /* Daemon startup record */
// /*AFLA*/ #define AUDIT_DAEMON_END        1201    /* Daemon normal stop record */
// /*AFLA*/ #define AUDIT_DAEMON_ABORT      1202    /* Daemon error stop record */
// /*AFLA*/ #define AUDIT_DAEMON_CONFIG     1203    /* Daemon config change */
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_SYSCALL		1300	/* Syscall event */
// /*AFLA*/ /* #define AUDIT_FS_WATCH	1301	 * Deprecated */
// /*AFLA*/ #define AUDIT_PATH		1302	/* Filename path information */
// /*AFLA*/ #define AUDIT_IPC		1303	/* IPC record */
// /*AFLA*/ #define AUDIT_SOCKETCALL	1304	/* sys_socketcall arguments */
// /*AFLA*/ #define AUDIT_CONFIG_CHANGE	1305	/* Audit system configuration change */
// /*AFLA*/ #define AUDIT_SOCKADDR		1306	/* sockaddr copied as syscall arg */
// /*AFLA*/ #define AUDIT_CWD		1307	/* Current working directory */
// /*AFLA*/ #define AUDIT_EXECVE		1309	/* execve arguments */
// /*AFLA*/ #define AUDIT_IPC_SET_PERM	1311	/* IPC new permissions record type */
// /*AFLA*/ #define AUDIT_MQ_OPEN		1312	/* POSIX MQ open record type */
// /*AFLA*/ #define AUDIT_MQ_SENDRECV	1313	/* POSIX MQ send/receive record type */
// /*AFLA*/ #define AUDIT_MQ_NOTIFY		1314	/* POSIX MQ notify record type */
// /*AFLA*/ #define AUDIT_MQ_GETSETATTR	1315	/* POSIX MQ get/set attribute record type */
// /*AFLA*/ #define AUDIT_KERNEL_OTHER	1316	/* For use by 3rd party modules */
// /*AFLA*/ #define AUDIT_FD_PAIR		1317    /* audit record for pipe/socketpair */
// /*AFLA*/ #define AUDIT_OBJ_PID		1318	/* ptrace target */
// /*AFLA*/ #define AUDIT_TTY		1319	/* Input on an administrative TTY */
// /*AFLA*/ #define AUDIT_EOE		1320	/* End of multi-record event */
// /*AFLA*/ #define AUDIT_BPRM_FCAPS	1321	/* Information about fcaps increasing perms */
// /*AFLA*/ #define AUDIT_CAPSET		1322	/* Record showing argument to sys_capset */
// /*AFLA*/ #define AUDIT_MMAP		1323	/* Record showing descriptor and flags in mmap */
// /*AFLA*/ #define AUDIT_NETFILTER_PKT	1324	/* Packets traversing netfilter chains */
// /*AFLA*/ #define AUDIT_NETFILTER_CFG	1325	/* Netfilter chain modifications */
// /*AFLA*/ #define AUDIT_SECCOMP		1326	/* Secure Computing event */
// /*AFLA*/ #define AUDIT_PROCTITLE		1327	/* Proctitle emit event */
// /*AFLA*/ #define AUDIT_FEATURE_CHANGE	1328	/* audit log listing feature changes */
// /*AFLA*/ #define AUDIT_REPLACE		1329	/* Replace auditd if this packet unanswerd */
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_AVC		1400	/* SE Linux avc denial or grant */
// /*AFLA*/ #define AUDIT_SELINUX_ERR	1401	/* Internal SE Linux Errors */
// /*AFLA*/ #define AUDIT_AVC_PATH		1402	/* dentry, vfsmount pair from avc */
// /*AFLA*/ #define AUDIT_MAC_POLICY_LOAD	1403	/* Policy file load */
// /*AFLA*/ #define AUDIT_MAC_STATUS	1404	/* Changed enforcing,permissive,off */
// /*AFLA*/ #define AUDIT_MAC_CONFIG_CHANGE	1405	/* Changes to booleans */
// /*AFLA*/ #define AUDIT_MAC_UNLBL_ALLOW	1406	/* NetLabel: allow unlabeled traffic */
// /*AFLA*/ #define AUDIT_MAC_CIPSOV4_ADD	1407	/* NetLabel: add CIPSOv4 DOI entry */
// /*AFLA*/ #define AUDIT_MAC_CIPSOV4_DEL	1408	/* NetLabel: del CIPSOv4 DOI entry */
// /*AFLA*/ #define AUDIT_MAC_MAP_ADD	1409	/* NetLabel: add LSM domain mapping */
// /*AFLA*/ #define AUDIT_MAC_MAP_DEL	1410	/* NetLabel: del LSM domain mapping */
// /*AFLA*/ #define AUDIT_MAC_IPSEC_ADDSA	1411	/* Not used */
// /*AFLA*/ #define AUDIT_MAC_IPSEC_DELSA	1412	/* Not used  */
// /*AFLA*/ #define AUDIT_MAC_IPSEC_ADDSPD	1413	/* Not used */
// /*AFLA*/ #define AUDIT_MAC_IPSEC_DELSPD	1414	/* Not used */
// /*AFLA*/ #define AUDIT_MAC_IPSEC_EVENT	1415	/* Audit an IPSec event */
// /*AFLA*/ #define AUDIT_MAC_UNLBL_STCADD	1416	/* NetLabel: add a static label */
// /*AFLA*/ #define AUDIT_MAC_UNLBL_STCDEL	1417	/* NetLabel: del a static label */
// /*AFLA*/ #define AUDIT_MAC_CALIPSO_ADD	1418	/* NetLabel: add CALIPSO DOI entry */
// /*AFLA*/ #define AUDIT_MAC_CALIPSO_DEL	1419	/* NetLabel: del CALIPSO DOI entry */
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_FIRST_KERN_ANOM_MSG   1700
// /*AFLA*/ #define AUDIT_LAST_KERN_ANOM_MSG    1799
// /*AFLA*/ #define AUDIT_ANOM_PROMISCUOUS      1700 /* Device changed promiscuous mode */
// /*AFLA*/ #define AUDIT_ANOM_ABEND            1701 /* Process ended abnormally */
// /*AFLA*/ #define AUDIT_ANOM_LINK		    1702 /* Suspicious use of file links */
// /*AFLA*/ #define AUDIT_INTEGRITY_DATA	    1800 /* Data integrity verification */
// /*AFLA*/ #define AUDIT_INTEGRITY_METADATA    1801 /* Metadata integrity verification */
// /*AFLA*/ #define AUDIT_INTEGRITY_STATUS	    1802 /* Integrity enable status */
// /*AFLA*/ #define AUDIT_INTEGRITY_HASH	    1803 /* Integrity HASH type */
// /*AFLA*/ #define AUDIT_INTEGRITY_PCR	    1804 /* PCR invalidation msgs */
// /*AFLA*/ #define AUDIT_INTEGRITY_RULE	    1805 /* policy rule */
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_KERNEL		2000	/* Asynchronous audit record. NOT A REQUEST. */
// /*AFLA*/ 
// /*AFLA*/ /* Rule flags */
// /*AFLA*/ #define AUDIT_FILTER_USER	0x00	/* Apply rule to user-generated messages */
// /*AFLA*/ #define AUDIT_FILTER_TASK	0x01	/* Apply rule at task creation (not syscall) */
// /*AFLA*/ #define AUDIT_FILTER_ENTRY	0x02	/* Apply rule at syscall entry */
// /*AFLA*/ #define AUDIT_FILTER_WATCH	0x03	/* Apply rule to file system watches */
// /*AFLA*/ #define AUDIT_FILTER_EXIT	0x04	/* Apply rule at syscall exit */
// /*AFLA*/ #define AUDIT_FILTER_TYPE	0x05	/* Apply rule at audit_log_start */
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_NR_FILTERS	6
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_FILTER_PREPEND	0x10	/* Prepend to front of list */
// /*AFLA*/ 
// /*AFLA*/ /* Rule actions */
// /*AFLA*/ #define AUDIT_NEVER    0	/* Do not build context if rule matches */
// /*AFLA*/ #define AUDIT_POSSIBLE 1	/* Build context if rule matches  */
// /*AFLA*/ #define AUDIT_ALWAYS   2	/* Generate audit record if rule matches */
// /*AFLA*/ 
// /*AFLA*/ /* Rule structure sizes -- if these change, different AUDIT_ADD and
// /*AFLA*/  * AUDIT_LIST commands must be implemented. */
// /*AFLA*/ #define AUDIT_MAX_FIELDS   64
// /*AFLA*/ #define AUDIT_MAX_KEY_LEN  256
// /*AFLA*/ #define AUDIT_BITMASK_SIZE 64
// /*AFLA*/ #define AUDIT_WORD(nr) ((__u32)((nr)/32))
// /*AFLA*/ #define AUDIT_BIT(nr)  (1 << ((nr) - AUDIT_WORD(nr)*32))
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_SYSCALL_CLASSES 16
// /*AFLA*/ #define AUDIT_CLASS_DIR_WRITE 0
// /*AFLA*/ #define AUDIT_CLASS_DIR_WRITE_32 1
// /*AFLA*/ #define AUDIT_CLASS_CHATTR 2
// /*AFLA*/ #define AUDIT_CLASS_CHATTR_32 3
// /*AFLA*/ #define AUDIT_CLASS_READ 4
// /*AFLA*/ #define AUDIT_CLASS_READ_32 5
// /*AFLA*/ #define AUDIT_CLASS_WRITE 6
// /*AFLA*/ #define AUDIT_CLASS_WRITE_32 7
// /*AFLA*/ #define AUDIT_CLASS_SIGNAL 8
// /*AFLA*/ #define AUDIT_CLASS_SIGNAL_32 9
// /*AFLA*/ 
// /*AFLA*/ /* This bitmask is used to validate user input.  It represents all bits that
// /*AFLA*/  * are currently used in an audit field constant understood by the kernel.
// /*AFLA*/  * If you are adding a new #define AUDIT_<whatever>, please ensure that
// /*AFLA*/  * AUDIT_UNUSED_BITS is updated if need be. */
// /*AFLA*/ #define AUDIT_UNUSED_BITS	0x07FFFC00
// /*AFLA*/ 
// /*AFLA*/ /* AUDIT_FIELD_COMPARE rule list */
// /*AFLA*/ #define AUDIT_COMPARE_UID_TO_OBJ_UID	1
// /*AFLA*/ #define AUDIT_COMPARE_GID_TO_OBJ_GID	2
// /*AFLA*/ #define AUDIT_COMPARE_EUID_TO_OBJ_UID	3
// /*AFLA*/ #define AUDIT_COMPARE_EGID_TO_OBJ_GID	4
// /*AFLA*/ #define AUDIT_COMPARE_AUID_TO_OBJ_UID	5
// /*AFLA*/ #define AUDIT_COMPARE_SUID_TO_OBJ_UID	6
// /*AFLA*/ #define AUDIT_COMPARE_SGID_TO_OBJ_GID	7
// /*AFLA*/ #define AUDIT_COMPARE_FSUID_TO_OBJ_UID	8
// /*AFLA*/ #define AUDIT_COMPARE_FSGID_TO_OBJ_GID	9
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_COMPARE_UID_TO_AUID	10
// /*AFLA*/ #define AUDIT_COMPARE_UID_TO_EUID	11
// /*AFLA*/ #define AUDIT_COMPARE_UID_TO_FSUID	12
// /*AFLA*/ #define AUDIT_COMPARE_UID_TO_SUID	13
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_COMPARE_AUID_TO_FSUID	14
// /*AFLA*/ #define AUDIT_COMPARE_AUID_TO_SUID	15
// /*AFLA*/ #define AUDIT_COMPARE_AUID_TO_EUID	16
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_COMPARE_EUID_TO_SUID	17
// /*AFLA*/ #define AUDIT_COMPARE_EUID_TO_FSUID	18
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_COMPARE_SUID_TO_FSUID	19
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_COMPARE_GID_TO_EGID	20
// /*AFLA*/ #define AUDIT_COMPARE_GID_TO_FSGID	21
// /*AFLA*/ #define AUDIT_COMPARE_GID_TO_SGID	22
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_COMPARE_EGID_TO_FSGID	23
// /*AFLA*/ #define AUDIT_COMPARE_EGID_TO_SGID	24
// /*AFLA*/ #define AUDIT_COMPARE_SGID_TO_FSGID	25
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_MAX_FIELD_COMPARE		AUDIT_COMPARE_SGID_TO_FSGID
// /*AFLA*/ 
// /*AFLA*/ /* Rule fields */
// /*AFLA*/ 				/* These are useful when checking the
// /*AFLA*/ 				 * task structure at task creation time
// /*AFLA*/ 				 * (AUDIT_PER_TASK).  */
// /*AFLA*/ #define AUDIT_PID	0
// /*AFLA*/ #define AUDIT_UID	1
// /*AFLA*/ #define AUDIT_EUID	2
// /*AFLA*/ #define AUDIT_SUID	3
// /*AFLA*/ #define AUDIT_FSUID	4
// /*AFLA*/ #define AUDIT_GID	5
// /*AFLA*/ #define AUDIT_EGID	6
// /*AFLA*/ #define AUDIT_SGID	7
// /*AFLA*/ #define AUDIT_FSGID	8
// /*AFLA*/ #define AUDIT_LOGINUID	9
// /*AFLA*/ #define AUDIT_PERS	10
// /*AFLA*/ #define AUDIT_ARCH	11
// /*AFLA*/ #define AUDIT_MSGTYPE	12
// /*AFLA*/ #define AUDIT_SUBJ_USER	13	/* security label user */
// /*AFLA*/ #define AUDIT_SUBJ_ROLE	14	/* security label role */
// /*AFLA*/ #define AUDIT_SUBJ_TYPE	15	/* security label type */
// /*AFLA*/ #define AUDIT_SUBJ_SEN	16	/* security label sensitivity label */
// /*AFLA*/ #define AUDIT_SUBJ_CLR	17	/* security label clearance label */
// /*AFLA*/ #define AUDIT_PPID	18
// /*AFLA*/ #define AUDIT_OBJ_USER	19
// /*AFLA*/ #define AUDIT_OBJ_ROLE	20
// /*AFLA*/ #define AUDIT_OBJ_TYPE	21
// /*AFLA*/ #define AUDIT_OBJ_LEV_LOW	22
// /*AFLA*/ #define AUDIT_OBJ_LEV_HIGH	23
// /*AFLA*/ #define AUDIT_LOGINUID_SET	24
// /*AFLA*/ 
// /*AFLA*/ 				/* These are ONLY useful when checking
// /*AFLA*/ 				 * at syscall exit time (AUDIT_AT_EXIT). */
// /*AFLA*/ #define AUDIT_DEVMAJOR	100
// /*AFLA*/ #define AUDIT_DEVMINOR	101
// /*AFLA*/ #define AUDIT_INODE	102
// /*AFLA*/ #define AUDIT_EXIT	103
// /*AFLA*/ #define AUDIT_SUCCESS   104	/* exit >= 0; value ignored */
// /*AFLA*/ #define AUDIT_WATCH	105
// /*AFLA*/ #define AUDIT_PERM	106
// /*AFLA*/ #define AUDIT_DIR	107
// /*AFLA*/ #define AUDIT_FILETYPE	108
// /*AFLA*/ #define AUDIT_OBJ_UID	109
// /*AFLA*/ #define AUDIT_OBJ_GID	110
// /*AFLA*/ #define AUDIT_FIELD_COMPARE	111
// /*AFLA*/ #define AUDIT_EXE	112
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_ARG0      200
// /*AFLA*/ #define AUDIT_ARG1      (AUDIT_ARG0+1)
// /*AFLA*/ #define AUDIT_ARG2      (AUDIT_ARG0+2)
// /*AFLA*/ #define AUDIT_ARG3      (AUDIT_ARG0+3)
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_FILTERKEY	210
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_NEGATE			0x80000000
// /*AFLA*/ 
// /*AFLA*/ /* These are the supported operators.
// /*AFLA*/  *	4  2  1  8
// /*AFLA*/  *	=  >  <  ?
// /*AFLA*/  *	----------
// /*AFLA*/  *	0  0  0	 0	00	nonsense
// /*AFLA*/  *	0  0  0	 1	08	&  bit mask
// /*AFLA*/  *	0  0  1	 0	10	<
// /*AFLA*/  *	0  1  0	 0	20	>
// /*AFLA*/  *	0  1  1	 0	30	!=
// /*AFLA*/  *	1  0  0	 0	40	=
// /*AFLA*/  *	1  0  0	 1	48	&=  bit test
// /*AFLA*/  *	1  0  1	 0	50	<=
// /*AFLA*/  *	1  1  0	 0	60	>=
// /*AFLA*/  *	1  1  1	 1	78	all operators
// /*AFLA*/  */
// /*AFLA*/ #define AUDIT_BIT_MASK			0x08000000
// /*AFLA*/ #define AUDIT_LESS_THAN			0x10000000
// /*AFLA*/ #define AUDIT_GREATER_THAN		0x20000000
// /*AFLA*/ #define AUDIT_NOT_EQUAL			0x30000000
// /*AFLA*/ #define AUDIT_EQUAL			0x40000000
// /*AFLA*/ #define AUDIT_BIT_TEST			(AUDIT_BIT_MASK|AUDIT_EQUAL)
// /*AFLA*/ #define AUDIT_LESS_THAN_OR_EQUAL	(AUDIT_LESS_THAN|AUDIT_EQUAL)
// /*AFLA*/ #define AUDIT_GREATER_THAN_OR_EQUAL	(AUDIT_GREATER_THAN|AUDIT_EQUAL)
// /*AFLA*/ #define AUDIT_OPERATORS			(AUDIT_EQUAL|AUDIT_NOT_EQUAL|AUDIT_BIT_MASK)
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	Audit_equal,
// /*AFLA*/ 	Audit_not_equal,
// /*AFLA*/ 	Audit_bitmask,
// /*AFLA*/ 	Audit_bittest,
// /*AFLA*/ 	Audit_lt,
// /*AFLA*/ 	Audit_gt,
// /*AFLA*/ 	Audit_le,
// /*AFLA*/ 	Audit_ge,
// /*AFLA*/ 	Audit_bad
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Status symbols */
// /*AFLA*/ 				/* Mask values */
// /*AFLA*/ #define AUDIT_STATUS_ENABLED		0x0001
// /*AFLA*/ #define AUDIT_STATUS_FAILURE		0x0002
// /*AFLA*/ #define AUDIT_STATUS_PID		0x0004
// /*AFLA*/ #define AUDIT_STATUS_RATE_LIMIT		0x0008
// /*AFLA*/ #define AUDIT_STATUS_BACKLOG_LIMIT	0x0010
// /*AFLA*/ #define AUDIT_STATUS_BACKLOG_WAIT_TIME	0x0020
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_FEATURE_BITMAP_BACKLOG_LIMIT	0x00000001
// /*AFLA*/ #define AUDIT_FEATURE_BITMAP_BACKLOG_WAIT_TIME	0x00000002
// /*AFLA*/ #define AUDIT_FEATURE_BITMAP_EXECUTABLE_PATH	0x00000004
// /*AFLA*/ #define AUDIT_FEATURE_BITMAP_EXCLUDE_EXTEND	0x00000008
// /*AFLA*/ #define AUDIT_FEATURE_BITMAP_ALL (AUDIT_FEATURE_BITMAP_BACKLOG_LIMIT | \
// /*AFLA*/ 				  AUDIT_FEATURE_BITMAP_BACKLOG_WAIT_TIME | \
// /*AFLA*/ 				  AUDIT_FEATURE_BITMAP_EXECUTABLE_PATH | \
// /*AFLA*/ 				  AUDIT_FEATURE_BITMAP_EXCLUDE_EXTEND)
// /*AFLA*/ 
// /*AFLA*/ /* deprecated: AUDIT_VERSION_* */
// /*AFLA*/ #define AUDIT_VERSION_LATEST 		AUDIT_FEATURE_BITMAP_ALL
// /*AFLA*/ #define AUDIT_VERSION_BACKLOG_LIMIT	AUDIT_FEATURE_BITMAP_BACKLOG_LIMIT
// /*AFLA*/ #define AUDIT_VERSION_BACKLOG_WAIT_TIME	AUDIT_FEATURE_BITMAP_BACKLOG_WAIT_TIME
// /*AFLA*/ 
// /*AFLA*/ 				/* Failure-to-log actions */
// /*AFLA*/ #define AUDIT_FAIL_SILENT	0
// /*AFLA*/ #define AUDIT_FAIL_PRINTK	1
// /*AFLA*/ #define AUDIT_FAIL_PANIC	2
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These bits disambiguate different calling conventions that share an
// /*AFLA*/  * ELF machine type, bitness, and endianness
// /*AFLA*/  */
// /*AFLA*/ #define __AUDIT_ARCH_CONVENTION_MASK 0x30000000
// /*AFLA*/ #define __AUDIT_ARCH_CONVENTION_MIPS64_N32 0x20000000
// /*AFLA*/ 
// /*AFLA*/ /* distinguish syscall tables */
// /*AFLA*/ #define __AUDIT_ARCH_64BIT 0x80000000
// /*AFLA*/ #define __AUDIT_ARCH_LE	   0x40000000
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_ARCH_AARCH64	(EM_AARCH64|__AUDIT_ARCH_64BIT|__AUDIT_ARCH_LE)
// /*AFLA*/ #define AUDIT_ARCH_ALPHA	(EM_ALPHA|__AUDIT_ARCH_64BIT|__AUDIT_ARCH_LE)
// /*AFLA*/ #define AUDIT_ARCH_ARM		(EM_ARM|__AUDIT_ARCH_LE)
// /*AFLA*/ #define AUDIT_ARCH_ARMEB	(EM_ARM)
// /*AFLA*/ #define AUDIT_ARCH_CRIS		(EM_CRIS|__AUDIT_ARCH_LE)
// /*AFLA*/ #define AUDIT_ARCH_FRV		(EM_FRV)
// /*AFLA*/ #define AUDIT_ARCH_I386		(EM_386|__AUDIT_ARCH_LE)
// /*AFLA*/ #define AUDIT_ARCH_IA64		(EM_IA_64|__AUDIT_ARCH_64BIT|__AUDIT_ARCH_LE)
// /*AFLA*/ #define AUDIT_ARCH_M32R		(EM_M32R)
// /*AFLA*/ #define AUDIT_ARCH_M68K		(EM_68K)
// /*AFLA*/ #define AUDIT_ARCH_MICROBLAZE	(EM_MICROBLAZE)
// /*AFLA*/ #define AUDIT_ARCH_MIPS		(EM_MIPS)
// /*AFLA*/ #define AUDIT_ARCH_MIPSEL	(EM_MIPS|__AUDIT_ARCH_LE)
// /*AFLA*/ #define AUDIT_ARCH_MIPS64	(EM_MIPS|__AUDIT_ARCH_64BIT)
// /*AFLA*/ #define AUDIT_ARCH_MIPS64N32	(EM_MIPS|__AUDIT_ARCH_64BIT|\
// /*AFLA*/ 				 __AUDIT_ARCH_CONVENTION_MIPS64_N32)
// /*AFLA*/ #define AUDIT_ARCH_MIPSEL64	(EM_MIPS|__AUDIT_ARCH_64BIT|__AUDIT_ARCH_LE)
// /*AFLA*/ #define AUDIT_ARCH_MIPSEL64N32	(EM_MIPS|__AUDIT_ARCH_64BIT|__AUDIT_ARCH_LE|\
// /*AFLA*/ 				 __AUDIT_ARCH_CONVENTION_MIPS64_N32)
// /*AFLA*/ #define AUDIT_ARCH_OPENRISC	(EM_OPENRISC)
// /*AFLA*/ #define AUDIT_ARCH_PARISC	(EM_PARISC)
// /*AFLA*/ #define AUDIT_ARCH_PARISC64	(EM_PARISC|__AUDIT_ARCH_64BIT)
// /*AFLA*/ #define AUDIT_ARCH_PPC		(EM_PPC)
// /*AFLA*/ /* do not define AUDIT_ARCH_PPCLE since it is not supported by audit */
// /*AFLA*/ #define AUDIT_ARCH_PPC64	(EM_PPC64|__AUDIT_ARCH_64BIT)
// /*AFLA*/ #define AUDIT_ARCH_PPC64LE	(EM_PPC64|__AUDIT_ARCH_64BIT|__AUDIT_ARCH_LE)
// /*AFLA*/ #define AUDIT_ARCH_S390		(EM_S390)
// /*AFLA*/ #define AUDIT_ARCH_S390X	(EM_S390|__AUDIT_ARCH_64BIT)
// /*AFLA*/ #define AUDIT_ARCH_SH		(EM_SH)
// /*AFLA*/ #define AUDIT_ARCH_SHEL		(EM_SH|__AUDIT_ARCH_LE)
// /*AFLA*/ #define AUDIT_ARCH_SH64		(EM_SH|__AUDIT_ARCH_64BIT)
// /*AFLA*/ #define AUDIT_ARCH_SHEL64	(EM_SH|__AUDIT_ARCH_64BIT|__AUDIT_ARCH_LE)
// /*AFLA*/ #define AUDIT_ARCH_SPARC	(EM_SPARC)
// /*AFLA*/ #define AUDIT_ARCH_SPARC64	(EM_SPARCV9|__AUDIT_ARCH_64BIT)
// /*AFLA*/ #define AUDIT_ARCH_TILEGX	(EM_TILEGX|__AUDIT_ARCH_64BIT|__AUDIT_ARCH_LE)
// /*AFLA*/ #define AUDIT_ARCH_TILEGX32	(EM_TILEGX|__AUDIT_ARCH_LE)
// /*AFLA*/ #define AUDIT_ARCH_TILEPRO	(EM_TILEPRO|__AUDIT_ARCH_LE)
// /*AFLA*/ #define AUDIT_ARCH_X86_64	(EM_X86_64|__AUDIT_ARCH_64BIT|__AUDIT_ARCH_LE)
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_PERM_EXEC		1
// /*AFLA*/ #define AUDIT_PERM_WRITE	2
// /*AFLA*/ #define AUDIT_PERM_READ		4
// /*AFLA*/ #define AUDIT_PERM_ATTR		8
// /*AFLA*/ 
// /*AFLA*/ /* MAX_AUDIT_MESSAGE_LENGTH is set in audit:lib/libaudit.h as:
// /*AFLA*/  * 8970 // PATH_MAX*2+CONTEXT_SIZE*2+11+256+1
// /*AFLA*/  * max header+body+tailer: 44 + 29 + 32 + 262 + 7 + pad
// /*AFLA*/  */
// /*AFLA*/ #define AUDIT_MESSAGE_TEXT_MAX	8560
// /*AFLA*/ 
// /*AFLA*/ /* Multicast Netlink socket groups (default up to 32) */
// /*AFLA*/ enum audit_nlgrps {
// /*AFLA*/ 	AUDIT_NLGRP_NONE,	/* Group 0 not used */
// /*AFLA*/ 	AUDIT_NLGRP_READLOG,	/* "best effort" read only socket */
// /*AFLA*/ 	__AUDIT_NLGRP_MAX
// /*AFLA*/ };
// /*AFLA*/ #define AUDIT_NLGRP_MAX                (__AUDIT_NLGRP_MAX - 1)
// /*AFLA*/ 
// /*AFLA*/ struct audit_status {
// /*AFLA*/ 	__u32		mask;		/* Bit mask for valid entries */
// /*AFLA*/ 	__u32		enabled;	/* 1 = enabled, 0 = disabled */
// /*AFLA*/ 	__u32		failure;	/* Failure-to-log action */
// /*AFLA*/ 	__u32		pid;		/* pid of auditd process */
// /*AFLA*/ 	__u32		rate_limit;	/* messages rate limit (per second) */
// /*AFLA*/ 	__u32		backlog_limit;	/* waiting messages limit */
// /*AFLA*/ 	__u32		lost;		/* messages lost */
// /*AFLA*/ 	__u32		backlog;	/* messages waiting in queue */
// /*AFLA*/ 	union {
// /*AFLA*/ 		__u32	version;	/* deprecated: audit api version num */
// /*AFLA*/ 		__u32	feature_bitmap;	/* bitmap of kernel audit features */
// /*AFLA*/ 	};
// /*AFLA*/ 	__u32		backlog_wait_time;/* message queue wait timeout */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct audit_features {
// /*AFLA*/ #define AUDIT_FEATURE_VERSION	1
// /*AFLA*/ 	__u32	vers;
// /*AFLA*/ 	__u32	mask;		/* which bits we are dealing with */
// /*AFLA*/ 	__u32	features;	/* which feature to enable/disable */
// /*AFLA*/ 	__u32	lock;		/* which features to lock */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_FEATURE_ONLY_UNSET_LOGINUID	0
// /*AFLA*/ #define AUDIT_FEATURE_LOGINUID_IMMUTABLE	1
// /*AFLA*/ #define AUDIT_LAST_FEATURE			AUDIT_FEATURE_LOGINUID_IMMUTABLE
// /*AFLA*/ 
// /*AFLA*/ #define audit_feature_valid(x)		((x) >= 0 && (x) <= AUDIT_LAST_FEATURE)
// /*AFLA*/ #define AUDIT_FEATURE_TO_MASK(x)	(1 << ((x) & 31)) /* mask for __u32 */
// /*AFLA*/ 
// /*AFLA*/ struct audit_tty_status {
// /*AFLA*/ 	__u32		enabled;	/* 1 = enabled, 0 = disabled */
// /*AFLA*/ 	__u32		log_passwd;	/* 1 = enabled, 0 = disabled */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define AUDIT_UID_UNSET (unsigned int)-1
// /*AFLA*/ 
// /*AFLA*/ /* audit_rule_data supports filter rules with both integer and string
// /*AFLA*/  * fields.  It corresponds with AUDIT_ADD_RULE, AUDIT_DEL_RULE and
// /*AFLA*/  * AUDIT_LIST_RULES requests.
// /*AFLA*/  */
// /*AFLA*/ struct audit_rule_data {
// /*AFLA*/ 	__u32		flags;	/* AUDIT_PER_{TASK,CALL}, AUDIT_PREPEND */
// /*AFLA*/ 	__u32		action;	/* AUDIT_NEVER, AUDIT_POSSIBLE, AUDIT_ALWAYS */
// /*AFLA*/ 	__u32		field_count;
// /*AFLA*/ 	__u32		mask[AUDIT_BITMASK_SIZE]; /* syscall(s) affected */
// /*AFLA*/ 	__u32		fields[AUDIT_MAX_FIELDS];
// /*AFLA*/ 	__u32		values[AUDIT_MAX_FIELDS];
// /*AFLA*/ 	__u32		fieldflags[AUDIT_MAX_FIELDS];
// /*AFLA*/ 	__u32		buflen;	/* total length of string fields */
// /*AFLA*/ 	char		buf[0];	/* string fields buffer */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_AUDIT_H_ */
