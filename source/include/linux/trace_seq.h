// /*AFLA*/ #ifndef _LINUX_TRACE_SEQ_H
// /*AFLA*/ #define _LINUX_TRACE_SEQ_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/seq_buf.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/page.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Trace sequences are used to allow a function to call several other functions
// /*AFLA*/  * to create a string of data to use (up to a max of PAGE_SIZE).
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct trace_seq {
// /*AFLA*/ 	unsigned char		buffer[PAGE_SIZE];
// /*AFLA*/ 	struct seq_buf		seq;
// /*AFLA*/ 	int			full;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ trace_seq_init(struct trace_seq *s)
// /*AFLA*/ {
// /*AFLA*/ 	seq_buf_init(&s->seq, s->buffer, PAGE_SIZE);
// /*AFLA*/ 	s->full = 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * trace_seq_used - amount of actual data written to buffer
// /*AFLA*/  * @s: trace sequence descriptor
// /*AFLA*/  *
// /*AFLA*/  * Returns the amount of data written to the buffer.
// /*AFLA*/  *
// /*AFLA*/  * IMPORTANT!
// /*AFLA*/  *
// /*AFLA*/  * Use this instead of @s->seq.len if you need to pass the amount
// /*AFLA*/  * of data from the buffer to another buffer (userspace, or what not).
// /*AFLA*/  * The @s->seq.len on overflow is bigger than the buffer size and
// /*AFLA*/  * using it can cause access to undefined memory.
// /*AFLA*/  */
// /*AFLA*/ static inline int trace_seq_used(struct trace_seq *s)
// /*AFLA*/ {
// /*AFLA*/ 	return seq_buf_used(&s->seq);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * trace_seq_buffer_ptr - return pointer to next location in buffer
// /*AFLA*/  * @s: trace sequence descriptor
// /*AFLA*/  *
// /*AFLA*/  * Returns the pointer to the buffer where the next write to
// /*AFLA*/  * the buffer will happen. This is useful to save the location
// /*AFLA*/  * that is about to be written to and then return the result
// /*AFLA*/  * of that write.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned char *
// /*AFLA*/ trace_seq_buffer_ptr(struct trace_seq *s)
// /*AFLA*/ {
// /*AFLA*/ 	return s->buffer + seq_buf_used(&s->seq);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * trace_seq_has_overflowed - return true if the trace_seq took too much
// /*AFLA*/  * @s: trace sequence descriptor
// /*AFLA*/  *
// /*AFLA*/  * Returns true if too much data was added to the trace_seq and it is
// /*AFLA*/  * now full and will not take anymore.
// /*AFLA*/  */
// /*AFLA*/ static inline bool trace_seq_has_overflowed(struct trace_seq *s)
// /*AFLA*/ {
// /*AFLA*/ 	return s->full || seq_buf_has_overflowed(&s->seq);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Currently only defined when tracing is enabled.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_TRACING
// /*AFLA*/ extern __printf(2, 3)
// /*AFLA*/ void trace_seq_printf(struct trace_seq *s, const char *fmt, ...);
// /*AFLA*/ extern __printf(2, 0)
// /*AFLA*/ void trace_seq_vprintf(struct trace_seq *s, const char *fmt, va_list args);
// /*AFLA*/ extern void
// /*AFLA*/ trace_seq_bprintf(struct trace_seq *s, const char *fmt, const u32 *binary);
// /*AFLA*/ extern int trace_print_seq(struct seq_file *m, struct trace_seq *s);
// /*AFLA*/ extern int trace_seq_to_user(struct trace_seq *s, char __user *ubuf,
// /*AFLA*/ 			     int cnt);
// /*AFLA*/ extern void trace_seq_puts(struct trace_seq *s, const char *str);
// /*AFLA*/ extern void trace_seq_putc(struct trace_seq *s, unsigned char c);
// /*AFLA*/ extern void trace_seq_putmem(struct trace_seq *s, const void *mem, unsigned int len);
// /*AFLA*/ extern void trace_seq_putmem_hex(struct trace_seq *s, const void *mem,
// /*AFLA*/ 				unsigned int len);
// /*AFLA*/ extern int trace_seq_path(struct trace_seq *s, const struct path *path);
// /*AFLA*/ 
// /*AFLA*/ extern void trace_seq_bitmask(struct trace_seq *s, const unsigned long *maskp,
// /*AFLA*/ 			     int nmaskbits);
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_TRACING */
// /*AFLA*/ static inline void trace_seq_printf(struct trace_seq *s, const char *fmt, ...)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void
// /*AFLA*/ trace_seq_bprintf(struct trace_seq *s, const char *fmt, const u32 *binary)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ trace_seq_bitmask(struct trace_seq *s, const unsigned long *maskp,
// /*AFLA*/ 		  int nmaskbits)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int trace_print_seq(struct seq_file *m, struct trace_seq *s)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int trace_seq_to_user(struct trace_seq *s, char __user *ubuf,
// /*AFLA*/ 				    int cnt)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline void trace_seq_puts(struct trace_seq *s, const char *str)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void trace_seq_putc(struct trace_seq *s, unsigned char c)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void
// /*AFLA*/ trace_seq_putmem(struct trace_seq *s, const void *mem, unsigned int len)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void trace_seq_putmem_hex(struct trace_seq *s, const void *mem,
// /*AFLA*/ 				       unsigned int len)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline int trace_seq_path(struct trace_seq *s, const struct path *path)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_TRACING */
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_TRACE_SEQ_H */
