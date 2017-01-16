// /*AFLA*/ /*
// /*AFLA*/  * sysctl.h: General linux system control interface
// /*AFLA*/  *
// /*AFLA*/  * Begun 24 March 1995, Stephen Tweedie
// /*AFLA*/  *
// /*AFLA*/  ****************************************************************
// /*AFLA*/  ****************************************************************
// /*AFLA*/  **
// /*AFLA*/  **  WARNING:
// /*AFLA*/  **  The values in this file are exported to user space via 
// /*AFLA*/  **  the sysctl() binary interface.  Do *NOT* change the
// /*AFLA*/  **  numbering of any existing values here, and do not change
// /*AFLA*/  **  any numbers within any one set of values.  If you have to
// /*AFLA*/  **  redefine an existing interface, use a new number for it.
// /*AFLA*/  **  The kernel will then return -ENOTDIR to any application using
// /*AFLA*/  **  the old binary interface.
// /*AFLA*/  **
// /*AFLA*/  ****************************************************************
// /*AFLA*/  ****************************************************************
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _UAPI_LINUX_SYSCTL_H
// /*AFLA*/ #define _UAPI_LINUX_SYSCTL_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ 
// /*AFLA*/ #define CTL_MAXNAME 10		/* how many path components do we allow in a
// /*AFLA*/ 				   call to sysctl?   In other words, what is
// /*AFLA*/ 				   the largest acceptable value for the nlen
// /*AFLA*/ 				   member of a struct __sysctl_args to have? */
// /*AFLA*/ 
// /*AFLA*/ struct __sysctl_args {
// /*AFLA*/ 	int __user *name;
// /*AFLA*/ 	int nlen;
// /*AFLA*/ 	void __user *oldval;
// /*AFLA*/ 	size_t __user *oldlenp;
// /*AFLA*/ 	void __user *newval;
// /*AFLA*/ 	size_t newlen;
// /*AFLA*/ 	unsigned long __unused[4];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* Define sysctl names first */
// /*AFLA*/ 
// /*AFLA*/ /* Top-level names: */
// /*AFLA*/ 
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	CTL_KERN=1,		/* General kernel info and control */
// /*AFLA*/ 	CTL_VM=2,		/* VM management */
// /*AFLA*/ 	CTL_NET=3,		/* Networking */
// /*AFLA*/ 	CTL_PROC=4,		/* removal breaks strace(1) compilation */
// /*AFLA*/ 	CTL_FS=5,		/* Filesystems */
// /*AFLA*/ 	CTL_DEBUG=6,		/* Debugging */
// /*AFLA*/ 	CTL_DEV=7,		/* Devices */
// /*AFLA*/ 	CTL_BUS=8,		/* Busses */
// /*AFLA*/ 	CTL_ABI=9,		/* Binary emulation */
// /*AFLA*/ 	CTL_CPU=10,		/* CPU stuff (speed scaling, etc) */
// /*AFLA*/ 	CTL_ARLAN=254,		/* arlan wireless driver */
// /*AFLA*/ 	CTL_S390DBF=5677,	/* s390 debug */
// /*AFLA*/ 	CTL_SUNRPC=7249,	/* sunrpc debug */
// /*AFLA*/ 	CTL_PM=9899,		/* frv power management */
// /*AFLA*/ 	CTL_FRV=9898,		/* frv specific sysctls */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* CTL_BUS names: */
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	CTL_BUS_ISA=1		/* ISA */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/fs/inotify/ */
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	INOTIFY_MAX_USER_INSTANCES=1,	/* max instances per user */
// /*AFLA*/ 	INOTIFY_MAX_USER_WATCHES=2,	/* max watches per user */
// /*AFLA*/ 	INOTIFY_MAX_QUEUED_EVENTS=3	/* max queued events per instance */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* CTL_KERN names: */
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	KERN_OSTYPE=1,		/* string: system version */
// /*AFLA*/ 	KERN_OSRELEASE=2,	/* string: system release */
// /*AFLA*/ 	KERN_OSREV=3,		/* int: system revision */
// /*AFLA*/ 	KERN_VERSION=4,		/* string: compile time info */
// /*AFLA*/ 	KERN_SECUREMASK=5,	/* struct: maximum rights mask */
// /*AFLA*/ 	KERN_PROF=6,		/* table: profiling information */
// /*AFLA*/ 	KERN_NODENAME=7,	/* string: hostname */
// /*AFLA*/ 	KERN_DOMAINNAME=8,	/* string: domainname */
// /*AFLA*/ 
// /*AFLA*/ 	KERN_PANIC=15,		/* int: panic timeout */
// /*AFLA*/ 	KERN_REALROOTDEV=16,	/* real root device to mount after initrd */
// /*AFLA*/ 
// /*AFLA*/ 	KERN_SPARC_REBOOT=21,	/* reboot command on Sparc */
// /*AFLA*/ 	KERN_CTLALTDEL=22,	/* int: allow ctl-alt-del to reboot */
// /*AFLA*/ 	KERN_PRINTK=23,		/* struct: control printk logging parameters */
// /*AFLA*/ 	KERN_NAMETRANS=24,	/* Name translation */
// /*AFLA*/ 	KERN_PPC_HTABRECLAIM=25, /* turn htab reclaimation on/off on PPC */
// /*AFLA*/ 	KERN_PPC_ZEROPAGED=26,	/* turn idle page zeroing on/off on PPC */
// /*AFLA*/ 	KERN_PPC_POWERSAVE_NAP=27, /* use nap mode for power saving */
// /*AFLA*/ 	KERN_MODPROBE=28,	/* string: modprobe path */
// /*AFLA*/ 	KERN_SG_BIG_BUFF=29,	/* int: sg driver reserved buffer size */
// /*AFLA*/ 	KERN_ACCT=30,		/* BSD process accounting parameters */
// /*AFLA*/ 	KERN_PPC_L2CR=31,	/* l2cr register on PPC */
// /*AFLA*/ 
// /*AFLA*/ 	KERN_RTSIGNR=32,	/* Number of rt sigs queued */
// /*AFLA*/ 	KERN_RTSIGMAX=33,	/* Max queuable */
// /*AFLA*/ 	
// /*AFLA*/ 	KERN_SHMMAX=34,         /* long: Maximum shared memory segment */
// /*AFLA*/ 	KERN_MSGMAX=35,         /* int: Maximum size of a messege */
// /*AFLA*/ 	KERN_MSGMNB=36,         /* int: Maximum message queue size */
// /*AFLA*/ 	KERN_MSGPOOL=37,        /* int: Maximum system message pool size */
// /*AFLA*/ 	KERN_SYSRQ=38,		/* int: Sysreq enable */
// /*AFLA*/ 	KERN_MAX_THREADS=39,	/* int: Maximum nr of threads in the system */
// /*AFLA*/  	KERN_RANDOM=40,		/* Random driver */
// /*AFLA*/  	KERN_SHMALL=41,		/* int: Maximum size of shared memory */
// /*AFLA*/  	KERN_MSGMNI=42,		/* int: msg queue identifiers */
// /*AFLA*/  	KERN_SEM=43,		/* struct: sysv semaphore limits */
// /*AFLA*/  	KERN_SPARC_STOP_A=44,	/* int: Sparc Stop-A enable */
// /*AFLA*/  	KERN_SHMMNI=45,		/* int: shm array identifiers */
// /*AFLA*/ 	KERN_OVERFLOWUID=46,	/* int: overflow UID */
// /*AFLA*/ 	KERN_OVERFLOWGID=47,	/* int: overflow GID */
// /*AFLA*/ 	KERN_SHMPATH=48,	/* string: path to shm fs */
// /*AFLA*/ 	KERN_HOTPLUG=49,	/* string: path to uevent helper (deprecated) */
// /*AFLA*/ 	KERN_IEEE_EMULATION_WARNINGS=50, /* int: unimplemented ieee instructions */
// /*AFLA*/ 	KERN_S390_USER_DEBUG_LOGGING=51,  /* int: dumps of user faults */
// /*AFLA*/ 	KERN_CORE_USES_PID=52,		/* int: use core or core.%pid */
// /*AFLA*/ 	KERN_TAINTED=53,	/* int: various kernel tainted flags */
// /*AFLA*/ 	KERN_CADPID=54,		/* int: PID of the process to notify on CAD */
// /*AFLA*/ 	KERN_PIDMAX=55,		/* int: PID # limit */
// /*AFLA*/   	KERN_CORE_PATTERN=56,	/* string: pattern for core-file names */
// /*AFLA*/ 	KERN_PANIC_ON_OOPS=57,  /* int: whether we will panic on an oops */
// /*AFLA*/ 	KERN_HPPA_PWRSW=58,	/* int: hppa soft-power enable */
// /*AFLA*/ 	KERN_HPPA_UNALIGNED=59,	/* int: hppa unaligned-trap enable */
// /*AFLA*/ 	KERN_PRINTK_RATELIMIT=60, /* int: tune printk ratelimiting */
// /*AFLA*/ 	KERN_PRINTK_RATELIMIT_BURST=61,	/* int: tune printk ratelimiting */
// /*AFLA*/ 	KERN_PTY=62,		/* dir: pty driver */
// /*AFLA*/ 	KERN_NGROUPS_MAX=63,	/* int: NGROUPS_MAX */
// /*AFLA*/ 	KERN_SPARC_SCONS_PWROFF=64, /* int: serial console power-off halt */
// /*AFLA*/ 	KERN_HZ_TIMER=65,	/* int: hz timer on or off */
// /*AFLA*/ 	KERN_UNKNOWN_NMI_PANIC=66, /* int: unknown nmi panic flag */
// /*AFLA*/ 	KERN_BOOTLOADER_TYPE=67, /* int: boot loader type */
// /*AFLA*/ 	KERN_RANDOMIZE=68, /* int: randomize virtual address space */
// /*AFLA*/ 	KERN_SETUID_DUMPABLE=69, /* int: behaviour of dumps for setuid core */
// /*AFLA*/ 	KERN_SPIN_RETRY=70,	/* int: number of spinlock retries */
// /*AFLA*/ 	KERN_ACPI_VIDEO_FLAGS=71, /* int: flags for setting up video after ACPI sleep */
// /*AFLA*/ 	KERN_IA64_UNALIGNED=72, /* int: ia64 unaligned userland trap enable */
// /*AFLA*/ 	KERN_COMPAT_LOG=73,	/* int: print compat layer  messages */
// /*AFLA*/ 	KERN_MAX_LOCK_DEPTH=74, /* int: rtmutex's maximum lock depth */
// /*AFLA*/ 	KERN_NMI_WATCHDOG=75, /* int: enable/disable nmi watchdog */
// /*AFLA*/ 	KERN_PANIC_ON_NMI=76, /* int: whether we will panic on an unrecovered */
// /*AFLA*/ 	KERN_PANIC_ON_WARN=77, /* int: call panic() in WARN() functions */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* CTL_VM names: */
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	VM_UNUSED1=1,		/* was: struct: Set vm swapping control */
// /*AFLA*/ 	VM_UNUSED2=2,		/* was; int: Linear or sqrt() swapout for hogs */
// /*AFLA*/ 	VM_UNUSED3=3,		/* was: struct: Set free page thresholds */
// /*AFLA*/ 	VM_UNUSED4=4,		/* Spare */
// /*AFLA*/ 	VM_OVERCOMMIT_MEMORY=5,	/* Turn off the virtual memory safety limit */
// /*AFLA*/ 	VM_UNUSED5=6,		/* was: struct: Set buffer memory thresholds */
// /*AFLA*/ 	VM_UNUSED7=7,		/* was: struct: Set cache memory thresholds */
// /*AFLA*/ 	VM_UNUSED8=8,		/* was: struct: Control kswapd behaviour */
// /*AFLA*/ 	VM_UNUSED9=9,		/* was: struct: Set page table cache parameters */
// /*AFLA*/ 	VM_PAGE_CLUSTER=10,	/* int: set number of pages to swap together */
// /*AFLA*/ 	VM_DIRTY_BACKGROUND=11,	/* dirty_background_ratio */
// /*AFLA*/ 	VM_DIRTY_RATIO=12,	/* dirty_ratio */
// /*AFLA*/ 	VM_DIRTY_WB_CS=13,	/* dirty_writeback_centisecs */
// /*AFLA*/ 	VM_DIRTY_EXPIRE_CS=14,	/* dirty_expire_centisecs */
// /*AFLA*/ 	VM_NR_PDFLUSH_THREADS=15, /* nr_pdflush_threads */
// /*AFLA*/ 	VM_OVERCOMMIT_RATIO=16, /* percent of RAM to allow overcommit in */
// /*AFLA*/ 	VM_PAGEBUF=17,		/* struct: Control pagebuf parameters */
// /*AFLA*/ 	VM_HUGETLB_PAGES=18,	/* int: Number of available Huge Pages */
// /*AFLA*/ 	VM_SWAPPINESS=19,	/* Tendency to steal mapped memory */
// /*AFLA*/ 	VM_LOWMEM_RESERVE_RATIO=20,/* reservation ratio for lower memory zones */
// /*AFLA*/ 	VM_MIN_FREE_KBYTES=21,	/* Minimum free kilobytes to maintain */
// /*AFLA*/ 	VM_MAX_MAP_COUNT=22,	/* int: Maximum number of mmaps/address-space */
// /*AFLA*/ 	VM_LAPTOP_MODE=23,	/* vm laptop mode */
// /*AFLA*/ 	VM_BLOCK_DUMP=24,	/* block dump mode */
// /*AFLA*/ 	VM_HUGETLB_GROUP=25,	/* permitted hugetlb group */
// /*AFLA*/ 	VM_VFS_CACHE_PRESSURE=26, /* dcache/icache reclaim pressure */
// /*AFLA*/ 	VM_LEGACY_VA_LAYOUT=27, /* legacy/compatibility virtual address space layout */
// /*AFLA*/ 	VM_SWAP_TOKEN_TIMEOUT=28, /* default time for token time out */
// /*AFLA*/ 	VM_DROP_PAGECACHE=29,	/* int: nuke lots of pagecache */
// /*AFLA*/ 	VM_PERCPU_PAGELIST_FRACTION=30,/* int: fraction of pages in each percpu_pagelist */
// /*AFLA*/ 	VM_ZONE_RECLAIM_MODE=31, /* reclaim local zone memory before going off node */
// /*AFLA*/ 	VM_MIN_UNMAPPED=32,	/* Set min percent of unmapped pages */
// /*AFLA*/ 	VM_PANIC_ON_OOM=33,	/* panic at out-of-memory */
// /*AFLA*/ 	VM_VDSO_ENABLED=34,	/* map VDSO into new processes? */
// /*AFLA*/ 	VM_MIN_SLAB=35,		 /* Percent pages ignored by zone reclaim */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* CTL_NET names: */
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	NET_CORE=1,
// /*AFLA*/ 	NET_ETHER=2,
// /*AFLA*/ 	NET_802=3,
// /*AFLA*/ 	NET_UNIX=4,
// /*AFLA*/ 	NET_IPV4=5,
// /*AFLA*/ 	NET_IPX=6,
// /*AFLA*/ 	NET_ATALK=7,
// /*AFLA*/ 	NET_NETROM=8,
// /*AFLA*/ 	NET_AX25=9,
// /*AFLA*/ 	NET_BRIDGE=10,
// /*AFLA*/ 	NET_ROSE=11,
// /*AFLA*/ 	NET_IPV6=12,
// /*AFLA*/ 	NET_X25=13,
// /*AFLA*/ 	NET_TR=14,
// /*AFLA*/ 	NET_DECNET=15,
// /*AFLA*/ 	NET_ECONET=16,
// /*AFLA*/ 	NET_SCTP=17,
// /*AFLA*/ 	NET_LLC=18,
// /*AFLA*/ 	NET_NETFILTER=19,
// /*AFLA*/ 	NET_DCCP=20,
// /*AFLA*/ 	NET_IRDA=412,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/kernel/random */
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	RANDOM_POOLSIZE=1,
// /*AFLA*/ 	RANDOM_ENTROPY_COUNT=2,
// /*AFLA*/ 	RANDOM_READ_THRESH=3,
// /*AFLA*/ 	RANDOM_WRITE_THRESH=4,
// /*AFLA*/ 	RANDOM_BOOT_ID=5,
// /*AFLA*/ 	RANDOM_UUID=6
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/kernel/pty */
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	PTY_MAX=1,
// /*AFLA*/ 	PTY_NR=2
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/bus/isa */
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	BUS_ISA_MEM_BASE=1,
// /*AFLA*/ 	BUS_ISA_PORT_BASE=2,
// /*AFLA*/ 	BUS_ISA_PORT_SHIFT=3
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/core */
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	NET_CORE_WMEM_MAX=1,
// /*AFLA*/ 	NET_CORE_RMEM_MAX=2,
// /*AFLA*/ 	NET_CORE_WMEM_DEFAULT=3,
// /*AFLA*/ 	NET_CORE_RMEM_DEFAULT=4,
// /*AFLA*/ /* was	NET_CORE_DESTROY_DELAY */
// /*AFLA*/ 	NET_CORE_MAX_BACKLOG=6,
// /*AFLA*/ 	NET_CORE_FASTROUTE=7,
// /*AFLA*/ 	NET_CORE_MSG_COST=8,
// /*AFLA*/ 	NET_CORE_MSG_BURST=9,
// /*AFLA*/ 	NET_CORE_OPTMEM_MAX=10,
// /*AFLA*/ 	NET_CORE_HOT_LIST_LENGTH=11,
// /*AFLA*/ 	NET_CORE_DIVERT_VERSION=12,
// /*AFLA*/ 	NET_CORE_NO_CONG_THRESH=13,
// /*AFLA*/ 	NET_CORE_NO_CONG=14,
// /*AFLA*/ 	NET_CORE_LO_CONG=15,
// /*AFLA*/ 	NET_CORE_MOD_CONG=16,
// /*AFLA*/ 	NET_CORE_DEV_WEIGHT=17,
// /*AFLA*/ 	NET_CORE_SOMAXCONN=18,
// /*AFLA*/ 	NET_CORE_BUDGET=19,
// /*AFLA*/ 	NET_CORE_AEVENT_ETIME=20,
// /*AFLA*/ 	NET_CORE_AEVENT_RSEQTH=21,
// /*AFLA*/ 	NET_CORE_WARNINGS=22,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/ethernet */
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/802 */
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/unix */
// /*AFLA*/ 
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	NET_UNIX_DESTROY_DELAY=1,
// /*AFLA*/ 	NET_UNIX_DELETE_DELAY=2,
// /*AFLA*/ 	NET_UNIX_MAX_DGRAM_QLEN=3,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/netfilter */
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	NET_NF_CONNTRACK_MAX=1,
// /*AFLA*/ 	NET_NF_CONNTRACK_TCP_TIMEOUT_SYN_SENT=2,
// /*AFLA*/ 	NET_NF_CONNTRACK_TCP_TIMEOUT_SYN_RECV=3,
// /*AFLA*/ 	NET_NF_CONNTRACK_TCP_TIMEOUT_ESTABLISHED=4,
// /*AFLA*/ 	NET_NF_CONNTRACK_TCP_TIMEOUT_FIN_WAIT=5,
// /*AFLA*/ 	NET_NF_CONNTRACK_TCP_TIMEOUT_CLOSE_WAIT=6,
// /*AFLA*/ 	NET_NF_CONNTRACK_TCP_TIMEOUT_LAST_ACK=7,
// /*AFLA*/ 	NET_NF_CONNTRACK_TCP_TIMEOUT_TIME_WAIT=8,
// /*AFLA*/ 	NET_NF_CONNTRACK_TCP_TIMEOUT_CLOSE=9,
// /*AFLA*/ 	NET_NF_CONNTRACK_UDP_TIMEOUT=10,
// /*AFLA*/ 	NET_NF_CONNTRACK_UDP_TIMEOUT_STREAM=11,
// /*AFLA*/ 	NET_NF_CONNTRACK_ICMP_TIMEOUT=12,
// /*AFLA*/ 	NET_NF_CONNTRACK_GENERIC_TIMEOUT=13,
// /*AFLA*/ 	NET_NF_CONNTRACK_BUCKETS=14,
// /*AFLA*/ 	NET_NF_CONNTRACK_LOG_INVALID=15,
// /*AFLA*/ 	NET_NF_CONNTRACK_TCP_TIMEOUT_MAX_RETRANS=16,
// /*AFLA*/ 	NET_NF_CONNTRACK_TCP_LOOSE=17,
// /*AFLA*/ 	NET_NF_CONNTRACK_TCP_BE_LIBERAL=18,
// /*AFLA*/ 	NET_NF_CONNTRACK_TCP_MAX_RETRANS=19,
// /*AFLA*/ 	NET_NF_CONNTRACK_SCTP_TIMEOUT_CLOSED=20,
// /*AFLA*/ 	NET_NF_CONNTRACK_SCTP_TIMEOUT_COOKIE_WAIT=21,
// /*AFLA*/ 	NET_NF_CONNTRACK_SCTP_TIMEOUT_COOKIE_ECHOED=22,
// /*AFLA*/ 	NET_NF_CONNTRACK_SCTP_TIMEOUT_ESTABLISHED=23,
// /*AFLA*/ 	NET_NF_CONNTRACK_SCTP_TIMEOUT_SHUTDOWN_SENT=24,
// /*AFLA*/ 	NET_NF_CONNTRACK_SCTP_TIMEOUT_SHUTDOWN_RECD=25,
// /*AFLA*/ 	NET_NF_CONNTRACK_SCTP_TIMEOUT_SHUTDOWN_ACK_SENT=26,
// /*AFLA*/ 	NET_NF_CONNTRACK_COUNT=27,
// /*AFLA*/ 	NET_NF_CONNTRACK_ICMPV6_TIMEOUT=28,
// /*AFLA*/ 	NET_NF_CONNTRACK_FRAG6_TIMEOUT=29,
// /*AFLA*/ 	NET_NF_CONNTRACK_FRAG6_LOW_THRESH=30,
// /*AFLA*/ 	NET_NF_CONNTRACK_FRAG6_HIGH_THRESH=31,
// /*AFLA*/ 	NET_NF_CONNTRACK_CHECKSUM=32,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/ipv4 */
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	/* v2.0 compatibile variables */
// /*AFLA*/ 	NET_IPV4_FORWARD=8,
// /*AFLA*/ 	NET_IPV4_DYNADDR=9,
// /*AFLA*/ 
// /*AFLA*/ 	NET_IPV4_CONF=16,
// /*AFLA*/ 	NET_IPV4_NEIGH=17,
// /*AFLA*/ 	NET_IPV4_ROUTE=18,
// /*AFLA*/ 	NET_IPV4_FIB_HASH=19,
// /*AFLA*/ 	NET_IPV4_NETFILTER=20,
// /*AFLA*/ 
// /*AFLA*/ 	NET_IPV4_TCP_TIMESTAMPS=33,
// /*AFLA*/ 	NET_IPV4_TCP_WINDOW_SCALING=34,
// /*AFLA*/ 	NET_IPV4_TCP_SACK=35,
// /*AFLA*/ 	NET_IPV4_TCP_RETRANS_COLLAPSE=36,
// /*AFLA*/ 	NET_IPV4_DEFAULT_TTL=37,
// /*AFLA*/ 	NET_IPV4_AUTOCONFIG=38,
// /*AFLA*/ 	NET_IPV4_NO_PMTU_DISC=39,
// /*AFLA*/ 	NET_IPV4_TCP_SYN_RETRIES=40,
// /*AFLA*/ 	NET_IPV4_IPFRAG_HIGH_THRESH=41,
// /*AFLA*/ 	NET_IPV4_IPFRAG_LOW_THRESH=42,
// /*AFLA*/ 	NET_IPV4_IPFRAG_TIME=43,
// /*AFLA*/ 	NET_IPV4_TCP_MAX_KA_PROBES=44,
// /*AFLA*/ 	NET_IPV4_TCP_KEEPALIVE_TIME=45,
// /*AFLA*/ 	NET_IPV4_TCP_KEEPALIVE_PROBES=46,
// /*AFLA*/ 	NET_IPV4_TCP_RETRIES1=47,
// /*AFLA*/ 	NET_IPV4_TCP_RETRIES2=48,
// /*AFLA*/ 	NET_IPV4_TCP_FIN_TIMEOUT=49,
// /*AFLA*/ 	NET_IPV4_IP_MASQ_DEBUG=50,
// /*AFLA*/ 	NET_TCP_SYNCOOKIES=51,
// /*AFLA*/ 	NET_TCP_STDURG=52,
// /*AFLA*/ 	NET_TCP_RFC1337=53,
// /*AFLA*/ 	NET_TCP_SYN_TAILDROP=54,
// /*AFLA*/ 	NET_TCP_MAX_SYN_BACKLOG=55,
// /*AFLA*/ 	NET_IPV4_LOCAL_PORT_RANGE=56,
// /*AFLA*/ 	NET_IPV4_ICMP_ECHO_IGNORE_ALL=57,
// /*AFLA*/ 	NET_IPV4_ICMP_ECHO_IGNORE_BROADCASTS=58,
// /*AFLA*/ 	NET_IPV4_ICMP_SOURCEQUENCH_RATE=59,
// /*AFLA*/ 	NET_IPV4_ICMP_DESTUNREACH_RATE=60,
// /*AFLA*/ 	NET_IPV4_ICMP_TIMEEXCEED_RATE=61,
// /*AFLA*/ 	NET_IPV4_ICMP_PARAMPROB_RATE=62,
// /*AFLA*/ 	NET_IPV4_ICMP_ECHOREPLY_RATE=63,
// /*AFLA*/ 	NET_IPV4_ICMP_IGNORE_BOGUS_ERROR_RESPONSES=64,
// /*AFLA*/ 	NET_IPV4_IGMP_MAX_MEMBERSHIPS=65,
// /*AFLA*/ 	NET_TCP_TW_RECYCLE=66,
// /*AFLA*/ 	NET_IPV4_ALWAYS_DEFRAG=67,
// /*AFLA*/ 	NET_IPV4_TCP_KEEPALIVE_INTVL=68,
// /*AFLA*/ 	NET_IPV4_INET_PEER_THRESHOLD=69,
// /*AFLA*/ 	NET_IPV4_INET_PEER_MINTTL=70,
// /*AFLA*/ 	NET_IPV4_INET_PEER_MAXTTL=71,
// /*AFLA*/ 	NET_IPV4_INET_PEER_GC_MINTIME=72,
// /*AFLA*/ 	NET_IPV4_INET_PEER_GC_MAXTIME=73,
// /*AFLA*/ 	NET_TCP_ORPHAN_RETRIES=74,
// /*AFLA*/ 	NET_TCP_ABORT_ON_OVERFLOW=75,
// /*AFLA*/ 	NET_TCP_SYNACK_RETRIES=76,
// /*AFLA*/ 	NET_TCP_MAX_ORPHANS=77,
// /*AFLA*/ 	NET_TCP_MAX_TW_BUCKETS=78,
// /*AFLA*/ 	NET_TCP_FACK=79,
// /*AFLA*/ 	NET_TCP_REORDERING=80,
// /*AFLA*/ 	NET_TCP_ECN=81,
// /*AFLA*/ 	NET_TCP_DSACK=82,
// /*AFLA*/ 	NET_TCP_MEM=83,
// /*AFLA*/ 	NET_TCP_WMEM=84,
// /*AFLA*/ 	NET_TCP_RMEM=85,
// /*AFLA*/ 	NET_TCP_APP_WIN=86,
// /*AFLA*/ 	NET_TCP_ADV_WIN_SCALE=87,
// /*AFLA*/ 	NET_IPV4_NONLOCAL_BIND=88,
// /*AFLA*/ 	NET_IPV4_ICMP_RATELIMIT=89,
// /*AFLA*/ 	NET_IPV4_ICMP_RATEMASK=90,
// /*AFLA*/ 	NET_TCP_TW_REUSE=91,
// /*AFLA*/ 	NET_TCP_FRTO=92,
// /*AFLA*/ 	NET_TCP_LOW_LATENCY=93,
// /*AFLA*/ 	NET_IPV4_IPFRAG_SECRET_INTERVAL=94,
// /*AFLA*/ 	NET_IPV4_IGMP_MAX_MSF=96,
// /*AFLA*/ 	NET_TCP_NO_METRICS_SAVE=97,
// /*AFLA*/ 	NET_TCP_DEFAULT_WIN_SCALE=105,
// /*AFLA*/ 	NET_TCP_MODERATE_RCVBUF=106,
// /*AFLA*/ 	NET_TCP_TSO_WIN_DIVISOR=107,
// /*AFLA*/ 	NET_TCP_BIC_BETA=108,
// /*AFLA*/ 	NET_IPV4_ICMP_ERRORS_USE_INBOUND_IFADDR=109,
// /*AFLA*/ 	NET_TCP_CONG_CONTROL=110,
// /*AFLA*/ 	NET_TCP_ABC=111,
// /*AFLA*/ 	NET_IPV4_IPFRAG_MAX_DIST=112,
// /*AFLA*/  	NET_TCP_MTU_PROBING=113,
// /*AFLA*/ 	NET_TCP_BASE_MSS=114,
// /*AFLA*/ 	NET_IPV4_TCP_WORKAROUND_SIGNED_WINDOWS=115,
// /*AFLA*/ 	NET_TCP_DMA_COPYBREAK=116,
// /*AFLA*/ 	NET_TCP_SLOW_START_AFTER_IDLE=117,
// /*AFLA*/ 	NET_CIPSOV4_CACHE_ENABLE=118,
// /*AFLA*/ 	NET_CIPSOV4_CACHE_BUCKET_SIZE=119,
// /*AFLA*/ 	NET_CIPSOV4_RBM_OPTFMT=120,
// /*AFLA*/ 	NET_CIPSOV4_RBM_STRICTVALID=121,
// /*AFLA*/ 	NET_TCP_AVAIL_CONG_CONTROL=122,
// /*AFLA*/ 	NET_TCP_ALLOWED_CONG_CONTROL=123,
// /*AFLA*/ 	NET_TCP_MAX_SSTHRESH=124,
// /*AFLA*/ 	NET_TCP_FRTO_RESPONSE=125,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_IPV4_ROUTE_FLUSH=1,
// /*AFLA*/ 	NET_IPV4_ROUTE_MIN_DELAY=2, /* obsolete since 2.6.25 */
// /*AFLA*/ 	NET_IPV4_ROUTE_MAX_DELAY=3, /* obsolete since 2.6.25 */
// /*AFLA*/ 	NET_IPV4_ROUTE_GC_THRESH=4,
// /*AFLA*/ 	NET_IPV4_ROUTE_MAX_SIZE=5,
// /*AFLA*/ 	NET_IPV4_ROUTE_GC_MIN_INTERVAL=6,
// /*AFLA*/ 	NET_IPV4_ROUTE_GC_TIMEOUT=7,
// /*AFLA*/ 	NET_IPV4_ROUTE_GC_INTERVAL=8, /* obsolete since 2.6.38 */
// /*AFLA*/ 	NET_IPV4_ROUTE_REDIRECT_LOAD=9,
// /*AFLA*/ 	NET_IPV4_ROUTE_REDIRECT_NUMBER=10,
// /*AFLA*/ 	NET_IPV4_ROUTE_REDIRECT_SILENCE=11,
// /*AFLA*/ 	NET_IPV4_ROUTE_ERROR_COST=12,
// /*AFLA*/ 	NET_IPV4_ROUTE_ERROR_BURST=13,
// /*AFLA*/ 	NET_IPV4_ROUTE_GC_ELASTICITY=14,
// /*AFLA*/ 	NET_IPV4_ROUTE_MTU_EXPIRES=15,
// /*AFLA*/ 	NET_IPV4_ROUTE_MIN_PMTU=16,
// /*AFLA*/ 	NET_IPV4_ROUTE_MIN_ADVMSS=17,
// /*AFLA*/ 	NET_IPV4_ROUTE_SECRET_INTERVAL=18,
// /*AFLA*/ 	NET_IPV4_ROUTE_GC_MIN_INTERVAL_MS=19,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	NET_PROTO_CONF_ALL=-2,
// /*AFLA*/ 	NET_PROTO_CONF_DEFAULT=-3
// /*AFLA*/ 
// /*AFLA*/ 	/* And device ifindices ... */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	NET_IPV4_CONF_FORWARDING=1,
// /*AFLA*/ 	NET_IPV4_CONF_MC_FORWARDING=2,
// /*AFLA*/ 	NET_IPV4_CONF_PROXY_ARP=3,
// /*AFLA*/ 	NET_IPV4_CONF_ACCEPT_REDIRECTS=4,
// /*AFLA*/ 	NET_IPV4_CONF_SECURE_REDIRECTS=5,
// /*AFLA*/ 	NET_IPV4_CONF_SEND_REDIRECTS=6,
// /*AFLA*/ 	NET_IPV4_CONF_SHARED_MEDIA=7,
// /*AFLA*/ 	NET_IPV4_CONF_RP_FILTER=8,
// /*AFLA*/ 	NET_IPV4_CONF_ACCEPT_SOURCE_ROUTE=9,
// /*AFLA*/ 	NET_IPV4_CONF_BOOTP_RELAY=10,
// /*AFLA*/ 	NET_IPV4_CONF_LOG_MARTIANS=11,
// /*AFLA*/ 	NET_IPV4_CONF_TAG=12,
// /*AFLA*/ 	NET_IPV4_CONF_ARPFILTER=13,
// /*AFLA*/ 	NET_IPV4_CONF_MEDIUM_ID=14,
// /*AFLA*/ 	NET_IPV4_CONF_NOXFRM=15,
// /*AFLA*/ 	NET_IPV4_CONF_NOPOLICY=16,
// /*AFLA*/ 	NET_IPV4_CONF_FORCE_IGMP_VERSION=17,
// /*AFLA*/ 	NET_IPV4_CONF_ARP_ANNOUNCE=18,
// /*AFLA*/ 	NET_IPV4_CONF_ARP_IGNORE=19,
// /*AFLA*/ 	NET_IPV4_CONF_PROMOTE_SECONDARIES=20,
// /*AFLA*/ 	NET_IPV4_CONF_ARP_ACCEPT=21,
// /*AFLA*/ 	NET_IPV4_CONF_ARP_NOTIFY=22,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/ipv4/netfilter */
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_MAX=1,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_TCP_TIMEOUT_SYN_SENT=2,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_TCP_TIMEOUT_SYN_RECV=3,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_TCP_TIMEOUT_ESTABLISHED=4,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_TCP_TIMEOUT_FIN_WAIT=5,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_TCP_TIMEOUT_CLOSE_WAIT=6,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_TCP_TIMEOUT_LAST_ACK=7,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_TCP_TIMEOUT_TIME_WAIT=8,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_TCP_TIMEOUT_CLOSE=9,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_UDP_TIMEOUT=10,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_UDP_TIMEOUT_STREAM=11,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_ICMP_TIMEOUT=12,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_GENERIC_TIMEOUT=13,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_BUCKETS=14,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_LOG_INVALID=15,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_TCP_TIMEOUT_MAX_RETRANS=16,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_TCP_LOOSE=17,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_TCP_BE_LIBERAL=18,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_TCP_MAX_RETRANS=19,
// /*AFLA*/  	NET_IPV4_NF_CONNTRACK_SCTP_TIMEOUT_CLOSED=20,
// /*AFLA*/  	NET_IPV4_NF_CONNTRACK_SCTP_TIMEOUT_COOKIE_WAIT=21,
// /*AFLA*/  	NET_IPV4_NF_CONNTRACK_SCTP_TIMEOUT_COOKIE_ECHOED=22,
// /*AFLA*/  	NET_IPV4_NF_CONNTRACK_SCTP_TIMEOUT_ESTABLISHED=23,
// /*AFLA*/  	NET_IPV4_NF_CONNTRACK_SCTP_TIMEOUT_SHUTDOWN_SENT=24,
// /*AFLA*/  	NET_IPV4_NF_CONNTRACK_SCTP_TIMEOUT_SHUTDOWN_RECD=25,
// /*AFLA*/  	NET_IPV4_NF_CONNTRACK_SCTP_TIMEOUT_SHUTDOWN_ACK_SENT=26,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_COUNT=27,
// /*AFLA*/ 	NET_IPV4_NF_CONNTRACK_CHECKSUM=28,
// /*AFLA*/ };
// /*AFLA*/  
// /*AFLA*/ /* /proc/sys/net/ipv6 */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_IPV6_CONF=16,
// /*AFLA*/ 	NET_IPV6_NEIGH=17,
// /*AFLA*/ 	NET_IPV6_ROUTE=18,
// /*AFLA*/ 	NET_IPV6_ICMP=19,
// /*AFLA*/ 	NET_IPV6_BINDV6ONLY=20,
// /*AFLA*/ 	NET_IPV6_IP6FRAG_HIGH_THRESH=21,
// /*AFLA*/ 	NET_IPV6_IP6FRAG_LOW_THRESH=22,
// /*AFLA*/ 	NET_IPV6_IP6FRAG_TIME=23,
// /*AFLA*/ 	NET_IPV6_IP6FRAG_SECRET_INTERVAL=24,
// /*AFLA*/ 	NET_IPV6_MLD_MAX_MSF=25,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_IPV6_ROUTE_FLUSH=1,
// /*AFLA*/ 	NET_IPV6_ROUTE_GC_THRESH=2,
// /*AFLA*/ 	NET_IPV6_ROUTE_MAX_SIZE=3,
// /*AFLA*/ 	NET_IPV6_ROUTE_GC_MIN_INTERVAL=4,
// /*AFLA*/ 	NET_IPV6_ROUTE_GC_TIMEOUT=5,
// /*AFLA*/ 	NET_IPV6_ROUTE_GC_INTERVAL=6,
// /*AFLA*/ 	NET_IPV6_ROUTE_GC_ELASTICITY=7,
// /*AFLA*/ 	NET_IPV6_ROUTE_MTU_EXPIRES=8,
// /*AFLA*/ 	NET_IPV6_ROUTE_MIN_ADVMSS=9,
// /*AFLA*/ 	NET_IPV6_ROUTE_GC_MIN_INTERVAL_MS=10
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_IPV6_FORWARDING=1,
// /*AFLA*/ 	NET_IPV6_HOP_LIMIT=2,
// /*AFLA*/ 	NET_IPV6_MTU=3,
// /*AFLA*/ 	NET_IPV6_ACCEPT_RA=4,
// /*AFLA*/ 	NET_IPV6_ACCEPT_REDIRECTS=5,
// /*AFLA*/ 	NET_IPV6_AUTOCONF=6,
// /*AFLA*/ 	NET_IPV6_DAD_TRANSMITS=7,
// /*AFLA*/ 	NET_IPV6_RTR_SOLICITS=8,
// /*AFLA*/ 	NET_IPV6_RTR_SOLICIT_INTERVAL=9,
// /*AFLA*/ 	NET_IPV6_RTR_SOLICIT_DELAY=10,
// /*AFLA*/ 	NET_IPV6_USE_TEMPADDR=11,
// /*AFLA*/ 	NET_IPV6_TEMP_VALID_LFT=12,
// /*AFLA*/ 	NET_IPV6_TEMP_PREFERED_LFT=13,
// /*AFLA*/ 	NET_IPV6_REGEN_MAX_RETRY=14,
// /*AFLA*/ 	NET_IPV6_MAX_DESYNC_FACTOR=15,
// /*AFLA*/ 	NET_IPV6_MAX_ADDRESSES=16,
// /*AFLA*/ 	NET_IPV6_FORCE_MLD_VERSION=17,
// /*AFLA*/ 	NET_IPV6_ACCEPT_RA_DEFRTR=18,
// /*AFLA*/ 	NET_IPV6_ACCEPT_RA_PINFO=19,
// /*AFLA*/ 	NET_IPV6_ACCEPT_RA_RTR_PREF=20,
// /*AFLA*/ 	NET_IPV6_RTR_PROBE_INTERVAL=21,
// /*AFLA*/ 	NET_IPV6_ACCEPT_RA_RT_INFO_MAX_PLEN=22,
// /*AFLA*/ 	NET_IPV6_PROXY_NDP=23,
// /*AFLA*/ 	NET_IPV6_ACCEPT_SOURCE_ROUTE=25,
// /*AFLA*/ 	NET_IPV6_ACCEPT_RA_FROM_LOCAL=26,
// /*AFLA*/ 	__NET_IPV6_MAX
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/ipv6/icmp */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_IPV6_ICMP_RATELIMIT=1
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/<protocol>/neigh/<dev> */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_NEIGH_MCAST_SOLICIT=1,
// /*AFLA*/ 	NET_NEIGH_UCAST_SOLICIT=2,
// /*AFLA*/ 	NET_NEIGH_APP_SOLICIT=3,
// /*AFLA*/ 	NET_NEIGH_RETRANS_TIME=4,
// /*AFLA*/ 	NET_NEIGH_REACHABLE_TIME=5,
// /*AFLA*/ 	NET_NEIGH_DELAY_PROBE_TIME=6,
// /*AFLA*/ 	NET_NEIGH_GC_STALE_TIME=7,
// /*AFLA*/ 	NET_NEIGH_UNRES_QLEN=8,
// /*AFLA*/ 	NET_NEIGH_PROXY_QLEN=9,
// /*AFLA*/ 	NET_NEIGH_ANYCAST_DELAY=10,
// /*AFLA*/ 	NET_NEIGH_PROXY_DELAY=11,
// /*AFLA*/ 	NET_NEIGH_LOCKTIME=12,
// /*AFLA*/ 	NET_NEIGH_GC_INTERVAL=13,
// /*AFLA*/ 	NET_NEIGH_GC_THRESH1=14,
// /*AFLA*/ 	NET_NEIGH_GC_THRESH2=15,
// /*AFLA*/ 	NET_NEIGH_GC_THRESH3=16,
// /*AFLA*/ 	NET_NEIGH_RETRANS_TIME_MS=17,
// /*AFLA*/ 	NET_NEIGH_REACHABLE_TIME_MS=18,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/dccp */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_DCCP_DEFAULT=1,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/ipx */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_IPX_PPROP_BROADCASTING=1,
// /*AFLA*/ 	NET_IPX_FORWARDING=2
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/llc */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_LLC2=1,
// /*AFLA*/ 	NET_LLC_STATION=2,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/llc/llc2 */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_LLC2_TIMEOUT=1,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/llc/station */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_LLC_STATION_ACK_TIMEOUT=1,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/llc/llc2/timeout */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_LLC2_ACK_TIMEOUT=1,
// /*AFLA*/ 	NET_LLC2_P_TIMEOUT=2,
// /*AFLA*/ 	NET_LLC2_REJ_TIMEOUT=3,
// /*AFLA*/ 	NET_LLC2_BUSY_TIMEOUT=4,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/appletalk */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_ATALK_AARP_EXPIRY_TIME=1,
// /*AFLA*/ 	NET_ATALK_AARP_TICK_TIME=2,
// /*AFLA*/ 	NET_ATALK_AARP_RETRANSMIT_LIMIT=3,
// /*AFLA*/ 	NET_ATALK_AARP_RESOLVE_TIME=4
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/netrom */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_NETROM_DEFAULT_PATH_QUALITY=1,
// /*AFLA*/ 	NET_NETROM_OBSOLESCENCE_COUNT_INITIALISER=2,
// /*AFLA*/ 	NET_NETROM_NETWORK_TTL_INITIALISER=3,
// /*AFLA*/ 	NET_NETROM_TRANSPORT_TIMEOUT=4,
// /*AFLA*/ 	NET_NETROM_TRANSPORT_MAXIMUM_TRIES=5,
// /*AFLA*/ 	NET_NETROM_TRANSPORT_ACKNOWLEDGE_DELAY=6,
// /*AFLA*/ 	NET_NETROM_TRANSPORT_BUSY_DELAY=7,
// /*AFLA*/ 	NET_NETROM_TRANSPORT_REQUESTED_WINDOW_SIZE=8,
// /*AFLA*/ 	NET_NETROM_TRANSPORT_NO_ACTIVITY_TIMEOUT=9,
// /*AFLA*/ 	NET_NETROM_ROUTING_CONTROL=10,
// /*AFLA*/ 	NET_NETROM_LINK_FAILS_COUNT=11,
// /*AFLA*/ 	NET_NETROM_RESET=12
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/ax25 */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_AX25_IP_DEFAULT_MODE=1,
// /*AFLA*/ 	NET_AX25_DEFAULT_MODE=2,
// /*AFLA*/ 	NET_AX25_BACKOFF_TYPE=3,
// /*AFLA*/ 	NET_AX25_CONNECT_MODE=4,
// /*AFLA*/ 	NET_AX25_STANDARD_WINDOW=5,
// /*AFLA*/ 	NET_AX25_EXTENDED_WINDOW=6,
// /*AFLA*/ 	NET_AX25_T1_TIMEOUT=7,
// /*AFLA*/ 	NET_AX25_T2_TIMEOUT=8,
// /*AFLA*/ 	NET_AX25_T3_TIMEOUT=9,
// /*AFLA*/ 	NET_AX25_IDLE_TIMEOUT=10,
// /*AFLA*/ 	NET_AX25_N2=11,
// /*AFLA*/ 	NET_AX25_PACLEN=12,
// /*AFLA*/ 	NET_AX25_PROTOCOL=13,
// /*AFLA*/ 	NET_AX25_DAMA_SLAVE_TIMEOUT=14
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/rose */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_ROSE_RESTART_REQUEST_TIMEOUT=1,
// /*AFLA*/ 	NET_ROSE_CALL_REQUEST_TIMEOUT=2,
// /*AFLA*/ 	NET_ROSE_RESET_REQUEST_TIMEOUT=3,
// /*AFLA*/ 	NET_ROSE_CLEAR_REQUEST_TIMEOUT=4,
// /*AFLA*/ 	NET_ROSE_ACK_HOLD_BACK_TIMEOUT=5,
// /*AFLA*/ 	NET_ROSE_ROUTING_CONTROL=6,
// /*AFLA*/ 	NET_ROSE_LINK_FAIL_TIMEOUT=7,
// /*AFLA*/ 	NET_ROSE_MAX_VCS=8,
// /*AFLA*/ 	NET_ROSE_WINDOW_SIZE=9,
// /*AFLA*/ 	NET_ROSE_NO_ACTIVITY_TIMEOUT=10
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/x25 */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_X25_RESTART_REQUEST_TIMEOUT=1,
// /*AFLA*/ 	NET_X25_CALL_REQUEST_TIMEOUT=2,
// /*AFLA*/ 	NET_X25_RESET_REQUEST_TIMEOUT=3,
// /*AFLA*/ 	NET_X25_CLEAR_REQUEST_TIMEOUT=4,
// /*AFLA*/ 	NET_X25_ACK_HOLD_BACK_TIMEOUT=5,
// /*AFLA*/ 	NET_X25_FORWARD=6
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/token-ring */
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	NET_TR_RIF_TIMEOUT=1
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/decnet/ */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_DECNET_NODE_TYPE = 1,
// /*AFLA*/ 	NET_DECNET_NODE_ADDRESS = 2,
// /*AFLA*/ 	NET_DECNET_NODE_NAME = 3,
// /*AFLA*/ 	NET_DECNET_DEFAULT_DEVICE = 4,
// /*AFLA*/ 	NET_DECNET_TIME_WAIT = 5,
// /*AFLA*/ 	NET_DECNET_DN_COUNT = 6,
// /*AFLA*/ 	NET_DECNET_DI_COUNT = 7,
// /*AFLA*/ 	NET_DECNET_DR_COUNT = 8,
// /*AFLA*/ 	NET_DECNET_DST_GC_INTERVAL = 9,
// /*AFLA*/ 	NET_DECNET_CONF = 10,
// /*AFLA*/ 	NET_DECNET_NO_FC_MAX_CWND = 11,
// /*AFLA*/ 	NET_DECNET_MEM = 12,
// /*AFLA*/ 	NET_DECNET_RMEM = 13,
// /*AFLA*/ 	NET_DECNET_WMEM = 14,
// /*AFLA*/ 	NET_DECNET_DEBUG_LEVEL = 255
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/decnet/conf/<dev> */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_DECNET_CONF_LOOPBACK = -2,
// /*AFLA*/ 	NET_DECNET_CONF_DDCMP = -3,
// /*AFLA*/ 	NET_DECNET_CONF_PPP = -4,
// /*AFLA*/ 	NET_DECNET_CONF_X25 = -5,
// /*AFLA*/ 	NET_DECNET_CONF_GRE = -6,
// /*AFLA*/ 	NET_DECNET_CONF_ETHER = -7
// /*AFLA*/ 
// /*AFLA*/ 	/* ... and ifindex of devices */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/decnet/conf/<dev>/ */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_DECNET_CONF_DEV_PRIORITY = 1,
// /*AFLA*/ 	NET_DECNET_CONF_DEV_T1 = 2,
// /*AFLA*/ 	NET_DECNET_CONF_DEV_T2 = 3,
// /*AFLA*/ 	NET_DECNET_CONF_DEV_T3 = 4,
// /*AFLA*/ 	NET_DECNET_CONF_DEV_FORWARDING = 5,
// /*AFLA*/ 	NET_DECNET_CONF_DEV_BLKSIZE = 6,
// /*AFLA*/ 	NET_DECNET_CONF_DEV_STATE = 7
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/sctp */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_SCTP_RTO_INITIAL = 1,
// /*AFLA*/ 	NET_SCTP_RTO_MIN     = 2,
// /*AFLA*/ 	NET_SCTP_RTO_MAX     = 3,
// /*AFLA*/ 	NET_SCTP_RTO_ALPHA   = 4,
// /*AFLA*/ 	NET_SCTP_RTO_BETA    = 5,
// /*AFLA*/ 	NET_SCTP_VALID_COOKIE_LIFE       =  6,
// /*AFLA*/ 	NET_SCTP_ASSOCIATION_MAX_RETRANS =  7,
// /*AFLA*/ 	NET_SCTP_PATH_MAX_RETRANS        =  8,
// /*AFLA*/ 	NET_SCTP_MAX_INIT_RETRANSMITS    =  9,
// /*AFLA*/ 	NET_SCTP_HB_INTERVAL             = 10,
// /*AFLA*/ 	NET_SCTP_PRESERVE_ENABLE         = 11,
// /*AFLA*/ 	NET_SCTP_MAX_BURST               = 12,
// /*AFLA*/ 	NET_SCTP_ADDIP_ENABLE		 = 13,
// /*AFLA*/ 	NET_SCTP_PRSCTP_ENABLE		 = 14,
// /*AFLA*/ 	NET_SCTP_SNDBUF_POLICY		 = 15,
// /*AFLA*/ 	NET_SCTP_SACK_TIMEOUT		 = 16,
// /*AFLA*/ 	NET_SCTP_RCVBUF_POLICY		 = 17,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/net/bridge */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_BRIDGE_NF_CALL_ARPTABLES = 1,
// /*AFLA*/ 	NET_BRIDGE_NF_CALL_IPTABLES = 2,
// /*AFLA*/ 	NET_BRIDGE_NF_CALL_IP6TABLES = 3,
// /*AFLA*/ 	NET_BRIDGE_NF_FILTER_VLAN_TAGGED = 4,
// /*AFLA*/ 	NET_BRIDGE_NF_FILTER_PPPOE_TAGGED = 5,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* proc/sys/net/irda */
// /*AFLA*/ enum {
// /*AFLA*/ 	NET_IRDA_DISCOVERY=1,
// /*AFLA*/ 	NET_IRDA_DEVNAME=2,
// /*AFLA*/ 	NET_IRDA_DEBUG=3,
// /*AFLA*/ 	NET_IRDA_FAST_POLL=4,
// /*AFLA*/ 	NET_IRDA_DISCOVERY_SLOTS=5,
// /*AFLA*/ 	NET_IRDA_DISCOVERY_TIMEOUT=6,
// /*AFLA*/ 	NET_IRDA_SLOT_TIMEOUT=7,
// /*AFLA*/ 	NET_IRDA_MAX_BAUD_RATE=8,
// /*AFLA*/ 	NET_IRDA_MIN_TX_TURN_TIME=9,
// /*AFLA*/ 	NET_IRDA_MAX_TX_DATA_SIZE=10,
// /*AFLA*/ 	NET_IRDA_MAX_TX_WINDOW=11,
// /*AFLA*/ 	NET_IRDA_MAX_NOREPLY_TIME=12,
// /*AFLA*/ 	NET_IRDA_WARN_NOREPLY_TIME=13,
// /*AFLA*/ 	NET_IRDA_LAP_KEEPALIVE_TIME=14,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* CTL_FS names: */
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	FS_NRINODE=1,	/* int:current number of allocated inodes */
// /*AFLA*/ 	FS_STATINODE=2,
// /*AFLA*/ 	FS_MAXINODE=3,	/* int:maximum number of inodes that can be allocated */
// /*AFLA*/ 	FS_NRDQUOT=4,	/* int:current number of allocated dquots */
// /*AFLA*/ 	FS_MAXDQUOT=5,	/* int:maximum number of dquots that can be allocated */
// /*AFLA*/ 	FS_NRFILE=6,	/* int:current number of allocated filedescriptors */
// /*AFLA*/ 	FS_MAXFILE=7,	/* int:maximum number of filedescriptors that can be allocated */
// /*AFLA*/ 	FS_DENTRY=8,
// /*AFLA*/ 	FS_NRSUPER=9,	/* int:current number of allocated super_blocks */
// /*AFLA*/ 	FS_MAXSUPER=10,	/* int:maximum number of super_blocks that can be allocated */
// /*AFLA*/ 	FS_OVERFLOWUID=11,	/* int: overflow UID */
// /*AFLA*/ 	FS_OVERFLOWGID=12,	/* int: overflow GID */
// /*AFLA*/ 	FS_LEASES=13,	/* int: leases enabled */
// /*AFLA*/ 	FS_DIR_NOTIFY=14,	/* int: directory notification enabled */
// /*AFLA*/ 	FS_LEASE_TIME=15,	/* int: maximum time to wait for a lease break */
// /*AFLA*/ 	FS_DQSTATS=16,	/* disc quota usage statistics and control */
// /*AFLA*/ 	FS_XFS=17,	/* struct: control xfs parameters */
// /*AFLA*/ 	FS_AIO_NR=18,	/* current system-wide number of aio requests */
// /*AFLA*/ 	FS_AIO_MAX_NR=19,	/* system-wide maximum number of aio requests */
// /*AFLA*/ 	FS_INOTIFY=20,	/* inotify submenu */
// /*AFLA*/ 	FS_OCFS2=988,	/* ocfs2 */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/fs/quota/ */
// /*AFLA*/ enum {
// /*AFLA*/ 	FS_DQ_LOOKUPS = 1,
// /*AFLA*/ 	FS_DQ_DROPS = 2,
// /*AFLA*/ 	FS_DQ_READS = 3,
// /*AFLA*/ 	FS_DQ_WRITES = 4,
// /*AFLA*/ 	FS_DQ_CACHE_HITS = 5,
// /*AFLA*/ 	FS_DQ_ALLOCATED = 6,
// /*AFLA*/ 	FS_DQ_FREE = 7,
// /*AFLA*/ 	FS_DQ_SYNCS = 8,
// /*AFLA*/ 	FS_DQ_WARNINGS = 9,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* CTL_DEBUG names: */
// /*AFLA*/ 
// /*AFLA*/ /* CTL_DEV names: */
// /*AFLA*/ enum {
// /*AFLA*/ 	DEV_CDROM=1,
// /*AFLA*/ 	DEV_HWMON=2,
// /*AFLA*/ 	DEV_PARPORT=3,
// /*AFLA*/ 	DEV_RAID=4,
// /*AFLA*/ 	DEV_MAC_HID=5,
// /*AFLA*/ 	DEV_SCSI=6,
// /*AFLA*/ 	DEV_IPMI=7,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/dev/cdrom */
// /*AFLA*/ enum {
// /*AFLA*/ 	DEV_CDROM_INFO=1,
// /*AFLA*/ 	DEV_CDROM_AUTOCLOSE=2,
// /*AFLA*/ 	DEV_CDROM_AUTOEJECT=3,
// /*AFLA*/ 	DEV_CDROM_DEBUG=4,
// /*AFLA*/ 	DEV_CDROM_LOCK=5,
// /*AFLA*/ 	DEV_CDROM_CHECK_MEDIA=6
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/dev/parport */
// /*AFLA*/ enum {
// /*AFLA*/ 	DEV_PARPORT_DEFAULT=-3
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/dev/raid */
// /*AFLA*/ enum {
// /*AFLA*/ 	DEV_RAID_SPEED_LIMIT_MIN=1,
// /*AFLA*/ 	DEV_RAID_SPEED_LIMIT_MAX=2
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/dev/parport/default */
// /*AFLA*/ enum {
// /*AFLA*/ 	DEV_PARPORT_DEFAULT_TIMESLICE=1,
// /*AFLA*/ 	DEV_PARPORT_DEFAULT_SPINTIME=2
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/dev/parport/parport n */
// /*AFLA*/ enum {
// /*AFLA*/ 	DEV_PARPORT_SPINTIME=1,
// /*AFLA*/ 	DEV_PARPORT_BASE_ADDR=2,
// /*AFLA*/ 	DEV_PARPORT_IRQ=3,
// /*AFLA*/ 	DEV_PARPORT_DMA=4,
// /*AFLA*/ 	DEV_PARPORT_MODES=5,
// /*AFLA*/ 	DEV_PARPORT_DEVICES=6,
// /*AFLA*/ 	DEV_PARPORT_AUTOPROBE=16
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/dev/parport/parport n/devices/ */
// /*AFLA*/ enum {
// /*AFLA*/ 	DEV_PARPORT_DEVICES_ACTIVE=-3,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/dev/parport/parport n/devices/device n */
// /*AFLA*/ enum {
// /*AFLA*/ 	DEV_PARPORT_DEVICE_TIMESLICE=1,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/dev/mac_hid */
// /*AFLA*/ enum {
// /*AFLA*/ 	DEV_MAC_HID_KEYBOARD_SENDS_LINUX_KEYCODES=1,
// /*AFLA*/ 	DEV_MAC_HID_KEYBOARD_LOCK_KEYCODES=2,
// /*AFLA*/ 	DEV_MAC_HID_MOUSE_BUTTON_EMULATION=3,
// /*AFLA*/ 	DEV_MAC_HID_MOUSE_BUTTON2_KEYCODE=4,
// /*AFLA*/ 	DEV_MAC_HID_MOUSE_BUTTON3_KEYCODE=5,
// /*AFLA*/ 	DEV_MAC_HID_ADB_MOUSE_SENDS_KEYCODES=6
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/dev/scsi */
// /*AFLA*/ enum {
// /*AFLA*/ 	DEV_SCSI_LOGGING_LEVEL=1,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/dev/ipmi */
// /*AFLA*/ enum {
// /*AFLA*/ 	DEV_IPMI_POWEROFF_POWERCYCLE=1,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* /proc/sys/abi */
// /*AFLA*/ enum
// /*AFLA*/ {
// /*AFLA*/ 	ABI_DEFHANDLER_COFF=1,	/* default handler for coff binaries */
// /*AFLA*/ 	ABI_DEFHANDLER_ELF=2, 	/* default handler for ELF binaries */
// /*AFLA*/ 	ABI_DEFHANDLER_LCALL7=3,/* default handler for procs using lcall7 */
// /*AFLA*/ 	ABI_DEFHANDLER_LIBCSO=4,/* default handler for an libc.so ELF interp */
// /*AFLA*/ 	ABI_TRACE=5,		/* tracing flags */
// /*AFLA*/ 	ABI_FAKE_UTSNAME=6,	/* fake target utsname information */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_SYSCTL_H */
