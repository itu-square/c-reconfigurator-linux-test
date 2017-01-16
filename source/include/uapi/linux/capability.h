// /*AFLA*/ /*
// /*AFLA*/  * This is <linux/capability.h>
// /*AFLA*/  *
// /*AFLA*/  * Andrew G. Morgan <morgan@kernel.org>
// /*AFLA*/  * Alexander Kjeldaas <astor@guardian.no>
// /*AFLA*/  * with help from Aleph1, Roland Buresund and Andrew Main.
// /*AFLA*/  *
// /*AFLA*/  * See here for the libcap library ("POSIX draft" compliance):
// /*AFLA*/  *
// /*AFLA*/  * ftp://www.kernel.org/pub/linux/libs/security/linux-privs/kernel-2.6/
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _UAPI_LINUX_CAPABILITY_H
// /*AFLA*/ #define _UAPI_LINUX_CAPABILITY_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /* User-level do most of the mapping between kernel and user
// /*AFLA*/    capabilities based on the version tag given by the kernel. The
// /*AFLA*/    kernel might be somewhat backwards compatible, but don't bet on
// /*AFLA*/    it. */
// /*AFLA*/ 
// /*AFLA*/ /* Note, cap_t, is defined by POSIX (draft) to be an "opaque" pointer to
// /*AFLA*/    a set of three capability sets.  The transposition of 3*the
// /*AFLA*/    following structure to such a composite is better handled in a user
// /*AFLA*/    library since the draft standard requires the use of malloc/free
// /*AFLA*/    etc.. */
// /*AFLA*/ 
// /*AFLA*/ #define _LINUX_CAPABILITY_VERSION_1  0x19980330
// /*AFLA*/ #define _LINUX_CAPABILITY_U32S_1     1
// /*AFLA*/ 
// /*AFLA*/ #define _LINUX_CAPABILITY_VERSION_2  0x20071026  /* deprecated - use v3 */
// /*AFLA*/ #define _LINUX_CAPABILITY_U32S_2     2
// /*AFLA*/ 
// /*AFLA*/ #define _LINUX_CAPABILITY_VERSION_3  0x20080522
// /*AFLA*/ #define _LINUX_CAPABILITY_U32S_3     2
// /*AFLA*/ 
// /*AFLA*/ typedef struct __user_cap_header_struct {
// /*AFLA*/ 	__u32 version;
// /*AFLA*/ 	int pid;
// /*AFLA*/ } __user *cap_user_header_t;
// /*AFLA*/ 
// /*AFLA*/ typedef struct __user_cap_data_struct {
// /*AFLA*/         __u32 effective;
// /*AFLA*/         __u32 permitted;
// /*AFLA*/         __u32 inheritable;
// /*AFLA*/ } __user *cap_user_data_t;
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define VFS_CAP_REVISION_MASK	0xFF000000
// /*AFLA*/ #define VFS_CAP_REVISION_SHIFT	24
// /*AFLA*/ #define VFS_CAP_FLAGS_MASK	~VFS_CAP_REVISION_MASK
// /*AFLA*/ #define VFS_CAP_FLAGS_EFFECTIVE	0x000001
// /*AFLA*/ 
// /*AFLA*/ #define VFS_CAP_REVISION_1	0x01000000
// /*AFLA*/ #define VFS_CAP_U32_1           1
// /*AFLA*/ #define XATTR_CAPS_SZ_1         (sizeof(__le32)*(1 + 2*VFS_CAP_U32_1))
// /*AFLA*/ 
// /*AFLA*/ #define VFS_CAP_REVISION_2	0x02000000
// /*AFLA*/ #define VFS_CAP_U32_2           2
// /*AFLA*/ #define XATTR_CAPS_SZ_2         (sizeof(__le32)*(1 + 2*VFS_CAP_U32_2))
// /*AFLA*/ 
// /*AFLA*/ #define XATTR_CAPS_SZ           XATTR_CAPS_SZ_2
// /*AFLA*/ #define VFS_CAP_U32             VFS_CAP_U32_2
// /*AFLA*/ #define VFS_CAP_REVISION	VFS_CAP_REVISION_2
// /*AFLA*/ 
// /*AFLA*/ struct vfs_cap_data {
// /*AFLA*/ 	__le32 magic_etc;            /* Little endian */
// /*AFLA*/ 	struct {
// /*AFLA*/ 		__le32 permitted;    /* Little endian */
// /*AFLA*/ 		__le32 inheritable;  /* Little endian */
// /*AFLA*/ 	} data[VFS_CAP_U32];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifndef __KERNEL__
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Backwardly compatible definition for source code - trapped in a
// /*AFLA*/  * 32-bit world. If you find you need this, please consider using
// /*AFLA*/  * libcap to untrap yourself...
// /*AFLA*/  */
// /*AFLA*/ #define _LINUX_CAPABILITY_VERSION  _LINUX_CAPABILITY_VERSION_1
// /*AFLA*/ #define _LINUX_CAPABILITY_U32S     _LINUX_CAPABILITY_U32S_1
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  ** POSIX-draft defined capabilities.
// /*AFLA*/  **/
// /*AFLA*/ 
// /*AFLA*/ /* In a system with the [_POSIX_CHOWN_RESTRICTED] option defined, this
// /*AFLA*/    overrides the restriction of changing file ownership and group
// /*AFLA*/    ownership. */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_CHOWN            0
// /*AFLA*/ 
// /*AFLA*/ /* Override all DAC access, including ACL execute access if
// /*AFLA*/    [_POSIX_ACL] is defined. Excluding DAC access covered by
// /*AFLA*/    CAP_LINUX_IMMUTABLE. */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_DAC_OVERRIDE     1
// /*AFLA*/ 
// /*AFLA*/ /* Overrides all DAC restrictions regarding read and search on files
// /*AFLA*/    and directories, including ACL restrictions if [_POSIX_ACL] is
// /*AFLA*/    defined. Excluding DAC access covered by CAP_LINUX_IMMUTABLE. */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_DAC_READ_SEARCH  2
// /*AFLA*/ 
// /*AFLA*/ /* Overrides all restrictions about allowed operations on files, where
// /*AFLA*/    file owner ID must be equal to the user ID, except where CAP_FSETID
// /*AFLA*/    is applicable. It doesn't override MAC and DAC restrictions. */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_FOWNER           3
// /*AFLA*/ 
// /*AFLA*/ /* Overrides the following restrictions that the effective user ID
// /*AFLA*/    shall match the file owner ID when setting the S_ISUID and S_ISGID
// /*AFLA*/    bits on that file; that the effective group ID (or one of the
// /*AFLA*/    supplementary group IDs) shall match the file owner ID when setting
// /*AFLA*/    the S_ISGID bit on that file; that the S_ISUID and S_ISGID bits are
// /*AFLA*/    cleared on successful return from chown(2) (not implemented). */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_FSETID           4
// /*AFLA*/ 
// /*AFLA*/ /* Overrides the restriction that the real or effective user ID of a
// /*AFLA*/    process sending a signal must match the real or effective user ID
// /*AFLA*/    of the process receiving the signal. */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_KILL             5
// /*AFLA*/ 
// /*AFLA*/ /* Allows setgid(2) manipulation */
// /*AFLA*/ /* Allows setgroups(2) */
// /*AFLA*/ /* Allows forged gids on socket credentials passing. */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_SETGID           6
// /*AFLA*/ 
// /*AFLA*/ /* Allows set*uid(2) manipulation (including fsuid). */
// /*AFLA*/ /* Allows forged pids on socket credentials passing. */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_SETUID           7
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  ** Linux-specific capabilities
// /*AFLA*/  **/
// /*AFLA*/ 
// /*AFLA*/ /* Without VFS support for capabilities:
// /*AFLA*/  *   Transfer any capability in your permitted set to any pid,
// /*AFLA*/  *   remove any capability in your permitted set from any pid
// /*AFLA*/  * With VFS support for capabilities (neither of above, but)
// /*AFLA*/  *   Add any capability from current's capability bounding set
// /*AFLA*/  *       to the current process' inheritable set
// /*AFLA*/  *   Allow taking bits out of capability bounding set
// /*AFLA*/  *   Allow modification of the securebits for a process
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_SETPCAP          8
// /*AFLA*/ 
// /*AFLA*/ /* Allow modification of S_IMMUTABLE and S_APPEND file attributes */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_LINUX_IMMUTABLE  9
// /*AFLA*/ 
// /*AFLA*/ /* Allows binding to TCP/UDP sockets below 1024 */
// /*AFLA*/ /* Allows binding to ATM VCIs below 32 */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_NET_BIND_SERVICE 10
// /*AFLA*/ 
// /*AFLA*/ /* Allow broadcasting, listen to multicast */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_NET_BROADCAST    11
// /*AFLA*/ 
// /*AFLA*/ /* Allow interface configuration */
// /*AFLA*/ /* Allow administration of IP firewall, masquerading and accounting */
// /*AFLA*/ /* Allow setting debug option on sockets */
// /*AFLA*/ /* Allow modification of routing tables */
// /*AFLA*/ /* Allow setting arbitrary process / process group ownership on
// /*AFLA*/    sockets */
// /*AFLA*/ /* Allow binding to any address for transparent proxying (also via NET_RAW) */
// /*AFLA*/ /* Allow setting TOS (type of service) */
// /*AFLA*/ /* Allow setting promiscuous mode */
// /*AFLA*/ /* Allow clearing driver statistics */
// /*AFLA*/ /* Allow multicasting */
// /*AFLA*/ /* Allow read/write of device-specific registers */
// /*AFLA*/ /* Allow activation of ATM control sockets */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_NET_ADMIN        12
// /*AFLA*/ 
// /*AFLA*/ /* Allow use of RAW sockets */
// /*AFLA*/ /* Allow use of PACKET sockets */
// /*AFLA*/ /* Allow binding to any address for transparent proxying (also via NET_ADMIN) */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_NET_RAW          13
// /*AFLA*/ 
// /*AFLA*/ /* Allow locking of shared memory segments */
// /*AFLA*/ /* Allow mlock and mlockall (which doesn't really have anything to do
// /*AFLA*/    with IPC) */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_IPC_LOCK         14
// /*AFLA*/ 
// /*AFLA*/ /* Override IPC ownership checks */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_IPC_OWNER        15
// /*AFLA*/ 
// /*AFLA*/ /* Insert and remove kernel modules - modify kernel without limit */
// /*AFLA*/ #define CAP_SYS_MODULE       16
// /*AFLA*/ 
// /*AFLA*/ /* Allow ioperm/iopl access */
// /*AFLA*/ /* Allow sending USB messages to any device via /proc/bus/usb */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_SYS_RAWIO        17
// /*AFLA*/ 
// /*AFLA*/ /* Allow use of chroot() */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_SYS_CHROOT       18
// /*AFLA*/ 
// /*AFLA*/ /* Allow ptrace() of any process */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_SYS_PTRACE       19
// /*AFLA*/ 
// /*AFLA*/ /* Allow configuration of process accounting */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_SYS_PACCT        20
// /*AFLA*/ 
// /*AFLA*/ /* Allow configuration of the secure attention key */
// /*AFLA*/ /* Allow administration of the random device */
// /*AFLA*/ /* Allow examination and configuration of disk quotas */
// /*AFLA*/ /* Allow setting the domainname */
// /*AFLA*/ /* Allow setting the hostname */
// /*AFLA*/ /* Allow calling bdflush() */
// /*AFLA*/ /* Allow mount() and umount(), setting up new smb connection */
// /*AFLA*/ /* Allow some autofs root ioctls */
// /*AFLA*/ /* Allow nfsservctl */
// /*AFLA*/ /* Allow VM86_REQUEST_IRQ */
// /*AFLA*/ /* Allow to read/write pci config on alpha */
// /*AFLA*/ /* Allow irix_prctl on mips (setstacksize) */
// /*AFLA*/ /* Allow flushing all cache on m68k (sys_cacheflush) */
// /*AFLA*/ /* Allow removing semaphores */
// /*AFLA*/ /* Used instead of CAP_CHOWN to "chown" IPC message queues, semaphores
// /*AFLA*/    and shared memory */
// /*AFLA*/ /* Allow locking/unlocking of shared memory segment */
// /*AFLA*/ /* Allow turning swap on/off */
// /*AFLA*/ /* Allow forged pids on socket credentials passing */
// /*AFLA*/ /* Allow setting readahead and flushing buffers on block devices */
// /*AFLA*/ /* Allow setting geometry in floppy driver */
// /*AFLA*/ /* Allow turning DMA on/off in xd driver */
// /*AFLA*/ /* Allow administration of md devices (mostly the above, but some
// /*AFLA*/    extra ioctls) */
// /*AFLA*/ /* Allow tuning the ide driver */
// /*AFLA*/ /* Allow access to the nvram device */
// /*AFLA*/ /* Allow administration of apm_bios, serial and bttv (TV) device */
// /*AFLA*/ /* Allow manufacturer commands in isdn CAPI support driver */
// /*AFLA*/ /* Allow reading non-standardized portions of pci configuration space */
// /*AFLA*/ /* Allow DDI debug ioctl on sbpcd driver */
// /*AFLA*/ /* Allow setting up serial ports */
// /*AFLA*/ /* Allow sending raw qic-117 commands */
// /*AFLA*/ /* Allow enabling/disabling tagged queuing on SCSI controllers and sending
// /*AFLA*/    arbitrary SCSI commands */
// /*AFLA*/ /* Allow setting encryption key on loopback filesystem */
// /*AFLA*/ /* Allow setting zone reclaim policy */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_SYS_ADMIN        21
// /*AFLA*/ 
// /*AFLA*/ /* Allow use of reboot() */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_SYS_BOOT         22
// /*AFLA*/ 
// /*AFLA*/ /* Allow raising priority and setting priority on other (different
// /*AFLA*/    UID) processes */
// /*AFLA*/ /* Allow use of FIFO and round-robin (realtime) scheduling on own
// /*AFLA*/    processes and setting the scheduling algorithm used by another
// /*AFLA*/    process. */
// /*AFLA*/ /* Allow setting cpu affinity on other processes */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_SYS_NICE         23
// /*AFLA*/ 
// /*AFLA*/ /* Override resource limits. Set resource limits. */
// /*AFLA*/ /* Override quota limits. */
// /*AFLA*/ /* Override reserved space on ext2 filesystem */
// /*AFLA*/ /* Modify data journaling mode on ext3 filesystem (uses journaling
// /*AFLA*/    resources) */
// /*AFLA*/ /* NOTE: ext2 honors fsuid when checking for resource overrides, so
// /*AFLA*/    you can override using fsuid too */
// /*AFLA*/ /* Override size restrictions on IPC message queues */
// /*AFLA*/ /* Allow more than 64hz interrupts from the real-time clock */
// /*AFLA*/ /* Override max number of consoles on console allocation */
// /*AFLA*/ /* Override max number of keymaps */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_SYS_RESOURCE     24
// /*AFLA*/ 
// /*AFLA*/ /* Allow manipulation of system clock */
// /*AFLA*/ /* Allow irix_stime on mips */
// /*AFLA*/ /* Allow setting the real-time clock */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_SYS_TIME         25
// /*AFLA*/ 
// /*AFLA*/ /* Allow configuration of tty devices */
// /*AFLA*/ /* Allow vhangup() of tty */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_SYS_TTY_CONFIG   26
// /*AFLA*/ 
// /*AFLA*/ /* Allow the privileged aspects of mknod() */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_MKNOD            27
// /*AFLA*/ 
// /*AFLA*/ /* Allow taking of leases on files */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_LEASE            28
// /*AFLA*/ 
// /*AFLA*/ /* Allow writing the audit log via unicast netlink socket */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_AUDIT_WRITE      29
// /*AFLA*/ 
// /*AFLA*/ /* Allow configuration of audit via unicast netlink socket */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_AUDIT_CONTROL    30
// /*AFLA*/ 
// /*AFLA*/ #define CAP_SETFCAP	     31
// /*AFLA*/ 
// /*AFLA*/ /* Override MAC access.
// /*AFLA*/    The base kernel enforces no MAC policy.
// /*AFLA*/    An LSM may enforce a MAC policy, and if it does and it chooses
// /*AFLA*/    to implement capability based overrides of that policy, this is
// /*AFLA*/    the capability it should use to do so. */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_MAC_OVERRIDE     32
// /*AFLA*/ 
// /*AFLA*/ /* Allow MAC configuration or state changes.
// /*AFLA*/    The base kernel requires no MAC configuration.
// /*AFLA*/    An LSM may enforce a MAC policy, and if it does and it chooses
// /*AFLA*/    to implement capability based checks on modifications to that
// /*AFLA*/    policy or the data required to maintain it, this is the
// /*AFLA*/    capability it should use to do so. */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_MAC_ADMIN        33
// /*AFLA*/ 
// /*AFLA*/ /* Allow configuring the kernel's syslog (printk behaviour) */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_SYSLOG           34
// /*AFLA*/ 
// /*AFLA*/ /* Allow triggering something that will wake the system */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_WAKE_ALARM            35
// /*AFLA*/ 
// /*AFLA*/ /* Allow preventing system suspends */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_BLOCK_SUSPEND    36
// /*AFLA*/ 
// /*AFLA*/ /* Allow reading the audit log via multicast netlink socket */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_AUDIT_READ		37
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define CAP_LAST_CAP         CAP_AUDIT_READ
// /*AFLA*/ 
// /*AFLA*/ #define cap_valid(x) ((x) >= 0 && (x) <= CAP_LAST_CAP)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Bit location of each capability (used by user-space library and kernel)
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define CAP_TO_INDEX(x)     ((x) >> 5)        /* 1 << 5 == bits in __u32 */
// /*AFLA*/ #define CAP_TO_MASK(x)      (1 << ((x) & 31)) /* mask for indexed __u32 */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_CAPABILITY_H */
