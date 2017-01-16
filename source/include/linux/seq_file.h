// /*AFLA*/ #ifndef _LINUX_SEQ_FILE_H
// /*AFLA*/ #define _LINUX_SEQ_FILE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/string.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ #include <linux/mutex.h>
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ #include <linux/nodemask.h>
// /*AFLA*/ #include <linux/fs.h>
// /*AFLA*/ #include <linux/cred.h>
// /*AFLA*/ 
// /*AFLA*/ struct seq_operations;
// /*AFLA*/ 
// /*AFLA*/ struct seq_file {
// /*AFLA*/ 	char *buf;
// /*AFLA*/ 	size_t size;
// /*AFLA*/ 	size_t from;
// /*AFLA*/ 	size_t count;
// /*AFLA*/ 	size_t pad_until;
// /*AFLA*/ 	loff_t index;
// /*AFLA*/ 	loff_t read_pos;
// /*AFLA*/ 	u64 version;
// /*AFLA*/ 	struct mutex lock;
// /*AFLA*/ 	const struct seq_operations *op;
// /*AFLA*/ 	int poll_event;
// /*AFLA*/ 	const struct file *file;
// /*AFLA*/ 	void *private;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct seq_operations {
// /*AFLA*/ 	void * (*start) (struct seq_file *m, loff_t *pos);
// /*AFLA*/ 	void (*stop) (struct seq_file *m, void *v);
// /*AFLA*/ 	void * (*next) (struct seq_file *m, void *v, loff_t *pos);
// /*AFLA*/ 	int (*show) (struct seq_file *m, void *v);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define SEQ_SKIP 1
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * seq_has_overflowed - check if the buffer has overflowed
// /*AFLA*/  * @m: the seq_file handle
// /*AFLA*/  *
// /*AFLA*/  * seq_files have a buffer which may overflow. When this happens a larger
// /*AFLA*/  * buffer is reallocated and all the data will be printed again.
// /*AFLA*/  * The overflow state is true when m->count == m->size.
// /*AFLA*/  *
// /*AFLA*/  * Returns true if the buffer received more than it can hold.
// /*AFLA*/  */
// /*AFLA*/ static inline bool seq_has_overflowed(struct seq_file *m)
// /*AFLA*/ {
// /*AFLA*/ 	return m->count == m->size;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * seq_get_buf - get buffer to write arbitrary data to
// /*AFLA*/  * @m: the seq_file handle
// /*AFLA*/  * @bufp: the beginning of the buffer is stored here
// /*AFLA*/  *
// /*AFLA*/  * Return the number of bytes available in the buffer, or zero if
// /*AFLA*/  * there's no space.
// /*AFLA*/  */
// /*AFLA*/ static inline size_t seq_get_buf(struct seq_file *m, char **bufp)
// /*AFLA*/ {
// /*AFLA*/ 	BUG_ON(m->count > m->size);
// /*AFLA*/ 	if (m->count < m->size)
// /*AFLA*/ 		*bufp = m->buf + m->count;
// /*AFLA*/ 	else
// /*AFLA*/ 		*bufp = NULL;
// /*AFLA*/ 
// /*AFLA*/ 	return m->size - m->count;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * seq_commit - commit data to the buffer
// /*AFLA*/  * @m: the seq_file handle
// /*AFLA*/  * @num: the number of bytes to commit
// /*AFLA*/  *
// /*AFLA*/  * Commit @num bytes of data written to a buffer previously acquired
// /*AFLA*/  * by seq_buf_get.  To signal an error condition, or that the data
// /*AFLA*/  * didn't fit in the available space, pass a negative @num value.
// /*AFLA*/  */
// /*AFLA*/ static inline void seq_commit(struct seq_file *m, int num)
// /*AFLA*/ {
// /*AFLA*/ 	if (num < 0) {
// /*AFLA*/ 		m->count = m->size;
// /*AFLA*/ 	} else {
// /*AFLA*/ 		BUG_ON(m->count + num > m->size);
// /*AFLA*/ 		m->count += num;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * seq_setwidth - set padding width
// /*AFLA*/  * @m: the seq_file handle
// /*AFLA*/  * @size: the max number of bytes to pad.
// /*AFLA*/  *
// /*AFLA*/  * Call seq_setwidth() for setting max width, then call seq_printf() etc. and
// /*AFLA*/  * finally call seq_pad() to pad the remaining bytes.
// /*AFLA*/  */
// /*AFLA*/ static inline void seq_setwidth(struct seq_file *m, size_t size)
// /*AFLA*/ {
// /*AFLA*/ 	m->pad_until = m->count + size;
// /*AFLA*/ }
// /*AFLA*/ void seq_pad(struct seq_file *m, char c);
// /*AFLA*/ 
// /*AFLA*/ char *mangle_path(char *s, const char *p, const char *esc);
// /*AFLA*/ int seq_open(struct file *, const struct seq_operations *);
// /*AFLA*/ ssize_t seq_read(struct file *, char __user *, size_t, loff_t *);
// /*AFLA*/ loff_t seq_lseek(struct file *, loff_t, int);
// /*AFLA*/ int seq_release(struct inode *, struct file *);
// /*AFLA*/ int seq_write(struct seq_file *seq, const void *data, size_t len);
// /*AFLA*/ 
// /*AFLA*/ __printf(2, 0)
// /*AFLA*/ void seq_vprintf(struct seq_file *m, const char *fmt, va_list args);
// /*AFLA*/ __printf(2, 3)
// /*AFLA*/ void seq_printf(struct seq_file *m, const char *fmt, ...);
// /*AFLA*/ void seq_putc(struct seq_file *m, char c);
// /*AFLA*/ void seq_puts(struct seq_file *m, const char *s);
// /*AFLA*/ void seq_put_decimal_ull(struct seq_file *m, const char *delimiter,
// /*AFLA*/ 			 unsigned long long num);
// /*AFLA*/ void seq_put_decimal_ll(struct seq_file *m, const char *delimiter, long long num);
// /*AFLA*/ void seq_escape(struct seq_file *m, const char *s, const char *esc);
// /*AFLA*/ 
// /*AFLA*/ void seq_hex_dump(struct seq_file *m, const char *prefix_str, int prefix_type,
// /*AFLA*/ 		  int rowsize, int groupsize, const void *buf, size_t len,
// /*AFLA*/ 		  bool ascii);
// /*AFLA*/ 
// /*AFLA*/ int seq_path(struct seq_file *, const struct path *, const char *);
// /*AFLA*/ int seq_file_path(struct seq_file *, struct file *, const char *);
// /*AFLA*/ int seq_dentry(struct seq_file *, struct dentry *, const char *);
// /*AFLA*/ int seq_path_root(struct seq_file *m, const struct path *path,
// /*AFLA*/ 		  const struct path *root, const char *esc);
// /*AFLA*/ 
// /*AFLA*/ int single_open(struct file *, int (*)(struct seq_file *, void *), void *);
// /*AFLA*/ int single_open_size(struct file *, int (*)(struct seq_file *, void *), void *, size_t);
// /*AFLA*/ int single_release(struct inode *, struct file *);
// /*AFLA*/ void *__seq_open_private(struct file *, const struct seq_operations *, int);
// /*AFLA*/ int seq_open_private(struct file *, const struct seq_operations *, int);
// /*AFLA*/ int seq_release_private(struct inode *, struct file *);
// /*AFLA*/ 
// /*AFLA*/ static inline struct user_namespace *seq_user_ns(struct seq_file *seq)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_USER_NS
// /*AFLA*/ 	return seq->file->f_cred->user_ns;
// /*AFLA*/ #else
// /*AFLA*/ 	extern struct user_namespace init_user_ns;
// /*AFLA*/ 	return &init_user_ns;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * seq_show_options - display mount options with appropriate escapes.
// /*AFLA*/  * @m: the seq_file handle
// /*AFLA*/  * @name: the mount option name
// /*AFLA*/  * @value: the mount option name's value, can be NULL
// /*AFLA*/  */
// /*AFLA*/ static inline void seq_show_option(struct seq_file *m, const char *name,
// /*AFLA*/ 				   const char *value)
// /*AFLA*/ {
// /*AFLA*/ 	seq_putc(m, ',');
// /*AFLA*/ 	seq_escape(m, name, ",= \t\n\\");
// /*AFLA*/ 	if (value) {
// /*AFLA*/ 		seq_putc(m, '=');
// /*AFLA*/ 		seq_escape(m, value, ", \t\n\\");
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * seq_show_option_n - display mount options with appropriate escapes
// /*AFLA*/  *		       where @value must be a specific length.
// /*AFLA*/  * @m: the seq_file handle
// /*AFLA*/  * @name: the mount option name
// /*AFLA*/  * @value: the mount option name's value, cannot be NULL
// /*AFLA*/  * @length: the length of @value to display
// /*AFLA*/  *
// /*AFLA*/  * This is a macro since this uses "length" to define the size of the
// /*AFLA*/  * stack buffer.
// /*AFLA*/  */
// /*AFLA*/ #define seq_show_option_n(m, name, value, length) {	\
// /*AFLA*/ 	char val_buf[length + 1];			\
// /*AFLA*/ 	strncpy(val_buf, value, length);		\
// /*AFLA*/ 	val_buf[length] = '\0';				\
// /*AFLA*/ 	seq_show_option(m, name, val_buf);		\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define SEQ_START_TOKEN ((void *)1)
// /*AFLA*/ /*
// /*AFLA*/  * Helpers for iteration over list_head-s in seq_files
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ extern struct list_head *seq_list_start(struct list_head *head,
// /*AFLA*/ 		loff_t pos);
// /*AFLA*/ extern struct list_head *seq_list_start_head(struct list_head *head,
// /*AFLA*/ 		loff_t pos);
// /*AFLA*/ extern struct list_head *seq_list_next(void *v, struct list_head *head,
// /*AFLA*/ 		loff_t *ppos);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Helpers for iteration over hlist_head-s in seq_files
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ extern struct hlist_node *seq_hlist_start(struct hlist_head *head,
// /*AFLA*/ 					  loff_t pos);
// /*AFLA*/ extern struct hlist_node *seq_hlist_start_head(struct hlist_head *head,
// /*AFLA*/ 					       loff_t pos);
// /*AFLA*/ extern struct hlist_node *seq_hlist_next(void *v, struct hlist_head *head,
// /*AFLA*/ 					 loff_t *ppos);
// /*AFLA*/ 
// /*AFLA*/ extern struct hlist_node *seq_hlist_start_rcu(struct hlist_head *head,
// /*AFLA*/ 					      loff_t pos);
// /*AFLA*/ extern struct hlist_node *seq_hlist_start_head_rcu(struct hlist_head *head,
// /*AFLA*/ 						   loff_t pos);
// /*AFLA*/ extern struct hlist_node *seq_hlist_next_rcu(void *v,
// /*AFLA*/ 						   struct hlist_head *head,
// /*AFLA*/ 						   loff_t *ppos);
// /*AFLA*/ 
// /*AFLA*/ /* Helpers for iterating over per-cpu hlist_head-s in seq_files */
// /*AFLA*/ extern struct hlist_node *seq_hlist_start_percpu(struct hlist_head __percpu *head, int *cpu, loff_t pos);
// /*AFLA*/ 
// /*AFLA*/ extern struct hlist_node *seq_hlist_next_percpu(void *v, struct hlist_head __percpu *head, int *cpu, loff_t *pos);
// /*AFLA*/ 
// /*AFLA*/ #endif
