// /*AFLA*/ #ifndef _LINUX_SEQ_BUF_H
// /*AFLA*/ #define _LINUX_SEQ_BUF_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/fs.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Trace sequences are used to allow a function to call several other functions
// /*AFLA*/  * to create a string of data to use.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * seq_buf - seq buffer structure
// /*AFLA*/  * @buffer:	pointer to the buffer
// /*AFLA*/  * @size:	size of the buffer
// /*AFLA*/  * @len:	the amount of data inside the buffer
// /*AFLA*/  * @readpos:	The next position to read in the buffer.
// /*AFLA*/  */
// /*AFLA*/ struct seq_buf {
// /*AFLA*/ 	char			*buffer;
// /*AFLA*/ 	size_t			size;
// /*AFLA*/ 	size_t			len;
// /*AFLA*/ 	loff_t			readpos;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline void seq_buf_clear(struct seq_buf *s)
// /*AFLA*/ {
// /*AFLA*/ 	s->len = 0;
// /*AFLA*/ 	s->readpos = 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ seq_buf_init(struct seq_buf *s, unsigned char *buf, unsigned int size)
// /*AFLA*/ {
// /*AFLA*/ 	s->buffer = buf;
// /*AFLA*/ 	s->size = size;
// /*AFLA*/ 	seq_buf_clear(s);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * seq_buf have a buffer that might overflow. When this happens
// /*AFLA*/  * the len and size are set to be equal.
// /*AFLA*/  */
// /*AFLA*/ static inline bool
// /*AFLA*/ seq_buf_has_overflowed(struct seq_buf *s)
// /*AFLA*/ {
// /*AFLA*/ 	return s->len > s->size;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ seq_buf_set_overflow(struct seq_buf *s)
// /*AFLA*/ {
// /*AFLA*/ 	s->len = s->size + 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * How much buffer is left on the seq_buf?
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned int
// /*AFLA*/ seq_buf_buffer_left(struct seq_buf *s)
// /*AFLA*/ {
// /*AFLA*/ 	if (seq_buf_has_overflowed(s))
// /*AFLA*/ 		return 0;
// /*AFLA*/ 
// /*AFLA*/ 	return s->size - s->len;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* How much buffer was written? */
// /*AFLA*/ static inline unsigned int seq_buf_used(struct seq_buf *s)
// /*AFLA*/ {
// /*AFLA*/ 	return min(s->len, s->size);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * seq_buf_get_buf - get buffer to write arbitrary data to
// /*AFLA*/  * @s: the seq_buf handle
// /*AFLA*/  * @bufp: the beginning of the buffer is stored here
// /*AFLA*/  *
// /*AFLA*/  * Return the number of bytes available in the buffer, or zero if
// /*AFLA*/  * there's no space.
// /*AFLA*/  */
// /*AFLA*/ static inline size_t seq_buf_get_buf(struct seq_buf *s, char **bufp)
// /*AFLA*/ {
// /*AFLA*/ 	WARN_ON(s->len > s->size + 1);
// /*AFLA*/ 
// /*AFLA*/ 	if (s->len < s->size) {
// /*AFLA*/ 		*bufp = s->buffer + s->len;
// /*AFLA*/ 		return s->size - s->len;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	*bufp = NULL;
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * seq_buf_commit - commit data to the buffer
// /*AFLA*/  * @s: the seq_buf handle
// /*AFLA*/  * @num: the number of bytes to commit
// /*AFLA*/  *
// /*AFLA*/  * Commit @num bytes of data written to a buffer previously acquired
// /*AFLA*/  * by seq_buf_get.  To signal an error condition, or that the data
// /*AFLA*/  * didn't fit in the available space, pass a negative @num value.
// /*AFLA*/  */
// /*AFLA*/ static inline void seq_buf_commit(struct seq_buf *s, int num)
// /*AFLA*/ {
// /*AFLA*/ 	if (num < 0) {
// /*AFLA*/ 		seq_buf_set_overflow(s);
// /*AFLA*/ 	} else {
// /*AFLA*/ 		/* num must be negative on overflow */
// /*AFLA*/ 		BUG_ON(s->len + num > s->size);
// /*AFLA*/ 		s->len += num;
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern __printf(2, 3)
// /*AFLA*/ int seq_buf_printf(struct seq_buf *s, const char *fmt, ...);
// /*AFLA*/ extern __printf(2, 0)
// /*AFLA*/ int seq_buf_vprintf(struct seq_buf *s, const char *fmt, va_list args);
// /*AFLA*/ extern int seq_buf_print_seq(struct seq_file *m, struct seq_buf *s);
// /*AFLA*/ extern int seq_buf_to_user(struct seq_buf *s, char __user *ubuf,
// /*AFLA*/ 			   int cnt);
// /*AFLA*/ extern int seq_buf_puts(struct seq_buf *s, const char *str);
// /*AFLA*/ extern int seq_buf_putc(struct seq_buf *s, unsigned char c);
// /*AFLA*/ extern int seq_buf_putmem(struct seq_buf *s, const void *mem, unsigned int len);
// /*AFLA*/ extern int seq_buf_putmem_hex(struct seq_buf *s, const void *mem,
// /*AFLA*/ 			      unsigned int len);
// /*AFLA*/ extern int seq_buf_path(struct seq_buf *s, const struct path *path, const char *esc);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_BINARY_PRINTF
// /*AFLA*/ extern int
// /*AFLA*/ seq_buf_bprintf(struct seq_buf *s, const char *fmt, const u32 *binary);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_SEQ_BUF_H */
