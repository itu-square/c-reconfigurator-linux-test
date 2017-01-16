// /*AFLA*/ #ifndef _NFS_FS_I
// /*AFLA*/ #define _NFS_FS_I
// /*AFLA*/ 
// /*AFLA*/ struct nlm_lockowner;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * NFS lock info
// /*AFLA*/  */
// /*AFLA*/ struct nfs_lock_info {
// /*AFLA*/ 	u32		state;
// /*AFLA*/ 	struct nlm_lockowner *owner;
// /*AFLA*/ 	struct list_head list;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct nfs4_lock_state;
// /*AFLA*/ struct nfs4_lock_info {
// /*AFLA*/ 	struct nfs4_lock_state *owner;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif
