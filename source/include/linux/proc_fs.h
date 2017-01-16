// /*AFLA*/ /*
// /*AFLA*/  * The proc filesystem constants/structures
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LINUX_PROC_FS_H
// /*AFLA*/ #define _LINUX_PROC_FS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/fs.h>
// /*AFLA*/ 
// /*AFLA*/ struct proc_dir_entry;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PROC_FS
// /*AFLA*/ 
// /*AFLA*/ extern void proc_root_init(void);
// /*AFLA*/ extern void proc_flush_task(struct task_struct *);
// /*AFLA*/ 
// /*AFLA*/ extern struct proc_dir_entry *proc_symlink(const char *,
// /*AFLA*/ 		struct proc_dir_entry *, const char *);
// /*AFLA*/ extern struct proc_dir_entry *proc_mkdir(const char *, struct proc_dir_entry *);
// /*AFLA*/ extern struct proc_dir_entry *proc_mkdir_data(const char *, umode_t,
// /*AFLA*/ 					      struct proc_dir_entry *, void *);
// /*AFLA*/ extern struct proc_dir_entry *proc_mkdir_mode(const char *, umode_t,
// /*AFLA*/ 					      struct proc_dir_entry *);
// /*AFLA*/  
// /*AFLA*/ extern struct proc_dir_entry *proc_create_data(const char *, umode_t,
// /*AFLA*/ 					       struct proc_dir_entry *,
// /*AFLA*/ 					       const struct file_operations *,
// /*AFLA*/ 					       void *);
// /*AFLA*/ 
// /*AFLA*/ static inline struct proc_dir_entry *proc_create(
// /*AFLA*/ 	const char *name, umode_t mode, struct proc_dir_entry *parent,
// /*AFLA*/ 	const struct file_operations *proc_fops)
// /*AFLA*/ {
// /*AFLA*/ 	return proc_create_data(name, mode, parent, proc_fops, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void proc_set_size(struct proc_dir_entry *, loff_t);
// /*AFLA*/ extern void proc_set_user(struct proc_dir_entry *, kuid_t, kgid_t);
// /*AFLA*/ extern void *PDE_DATA(const struct inode *);
// /*AFLA*/ extern void *proc_get_parent_data(const struct inode *);
// /*AFLA*/ extern void proc_remove(struct proc_dir_entry *);
// /*AFLA*/ extern void remove_proc_entry(const char *, struct proc_dir_entry *);
// /*AFLA*/ extern int remove_proc_subtree(const char *, struct proc_dir_entry *);
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_PROC_FS */
// /*AFLA*/ 
// /*AFLA*/ static inline void proc_root_init(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void proc_flush_task(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct proc_dir_entry *proc_symlink(const char *name,
// /*AFLA*/ 		struct proc_dir_entry *parent,const char *dest) { return NULL;}
// /*AFLA*/ static inline struct proc_dir_entry *proc_mkdir(const char *name,
// /*AFLA*/ 	struct proc_dir_entry *parent) {return NULL;}
// /*AFLA*/ static inline struct proc_dir_entry *proc_mkdir_data(const char *name,
// /*AFLA*/ 	umode_t mode, struct proc_dir_entry *parent, void *data) { return NULL; }
// /*AFLA*/ static inline struct proc_dir_entry *proc_mkdir_mode(const char *name,
// /*AFLA*/ 	umode_t mode, struct proc_dir_entry *parent) { return NULL; }
// /*AFLA*/ #define proc_create(name, mode, parent, proc_fops) ({NULL;})
// /*AFLA*/ #define proc_create_data(name, mode, parent, proc_fops, data) ({NULL;})
// /*AFLA*/ 
// /*AFLA*/ static inline void proc_set_size(struct proc_dir_entry *de, loff_t size) {}
// /*AFLA*/ static inline void proc_set_user(struct proc_dir_entry *de, kuid_t uid, kgid_t gid) {}
// /*AFLA*/ static inline void *PDE_DATA(const struct inode *inode) {BUG(); return NULL;}
// /*AFLA*/ static inline void *proc_get_parent_data(const struct inode *inode) { BUG(); return NULL; }
// /*AFLA*/ 
// /*AFLA*/ static inline void proc_remove(struct proc_dir_entry *de) {}
// /*AFLA*/ #define remove_proc_entry(name, parent) do {} while (0)
// /*AFLA*/ static inline int remove_proc_subtree(const char *name, struct proc_dir_entry *parent) { return 0; }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_PROC_FS */
// /*AFLA*/ 
// /*AFLA*/ struct net;
// /*AFLA*/ 
// /*AFLA*/ static inline struct proc_dir_entry *proc_net_mkdir(
// /*AFLA*/ 	struct net *net, const char *name, struct proc_dir_entry *parent)
// /*AFLA*/ {
// /*AFLA*/ 	return proc_mkdir_data(name, 0, parent, net);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_PROC_FS_H */
