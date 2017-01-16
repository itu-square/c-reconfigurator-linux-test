// /*AFLA*/ #ifndef _LINUX_RING_BUFFER_H
// /*AFLA*/ #define _LINUX_RING_BUFFER_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/kmemcheck.h>
// /*AFLA*/ #include <linux/mm.h>
// /*AFLA*/ #include <linux/seq_file.h>
// /*AFLA*/ #include <linux/poll.h>
// /*AFLA*/ 
// /*AFLA*/ struct ring_buffer;
// /*AFLA*/ struct ring_buffer_iter;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Don't refer to this struct directly, use functions below.
// /*AFLA*/  */
// /*AFLA*/ struct ring_buffer_event {
// /*AFLA*/ 	kmemcheck_bitfield_begin(bitfield);
// /*AFLA*/ 	u32		type_len:5, time_delta:27;
// /*AFLA*/ 	kmemcheck_bitfield_end(bitfield);
// /*AFLA*/ 
// /*AFLA*/ 	u32		array[];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * enum ring_buffer_type - internal ring buffer types
// /*AFLA*/  *
// /*AFLA*/  * @RINGBUF_TYPE_PADDING:	Left over page padding or discarded event
// /*AFLA*/  *				 If time_delta is 0:
// /*AFLA*/  *				  array is ignored
// /*AFLA*/  *				  size is variable depending on how much
// /*AFLA*/  *				  padding is needed
// /*AFLA*/  *				 If time_delta is non zero:
// /*AFLA*/  *				  array[0] holds the actual length
// /*AFLA*/  *				  size = 4 + length (bytes)
// /*AFLA*/  *
// /*AFLA*/  * @RINGBUF_TYPE_TIME_EXTEND:	Extend the time delta
// /*AFLA*/  *				 array[0] = time delta (28 .. 59)
// /*AFLA*/  *				 size = 8 bytes
// /*AFLA*/  *
// /*AFLA*/  * @RINGBUF_TYPE_TIME_STAMP:	Sync time stamp with external clock
// /*AFLA*/  *				 array[0]    = tv_nsec
// /*AFLA*/  *				 array[1..2] = tv_sec
// /*AFLA*/  *				 size = 16 bytes
// /*AFLA*/  *
// /*AFLA*/  * <= @RINGBUF_TYPE_DATA_TYPE_LEN_MAX:
// /*AFLA*/  *				Data record
// /*AFLA*/  *				 If type_len is zero:
// /*AFLA*/  *				  array[0] holds the actual length
// /*AFLA*/  *				  array[1..(length+3)/4] holds data
// /*AFLA*/  *				  size = 4 + length (bytes)
// /*AFLA*/  *				 else
// /*AFLA*/  *				  length = type_len << 2
// /*AFLA*/  *				  array[0..(length+3)/4-1] holds data
// /*AFLA*/  *				  size = 4 + length (bytes)
// /*AFLA*/  */
// /*AFLA*/ enum ring_buffer_type {
// /*AFLA*/ 	RINGBUF_TYPE_DATA_TYPE_LEN_MAX = 28,
// /*AFLA*/ 	RINGBUF_TYPE_PADDING,
// /*AFLA*/ 	RINGBUF_TYPE_TIME_EXTEND,
// /*AFLA*/ 	/* FIXME: RINGBUF_TYPE_TIME_STAMP not implemented */
// /*AFLA*/ 	RINGBUF_TYPE_TIME_STAMP,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ unsigned ring_buffer_event_length(struct ring_buffer_event *event);
// /*AFLA*/ void *ring_buffer_event_data(struct ring_buffer_event *event);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * ring_buffer_discard_commit will remove an event that has not
// /*AFLA*/  *   ben committed yet. If this is used, then ring_buffer_unlock_commit
// /*AFLA*/  *   must not be called on the discarded event. This function
// /*AFLA*/  *   will try to remove the event from the ring buffer completely
// /*AFLA*/  *   if another event has not been written after it.
// /*AFLA*/  *
// /*AFLA*/  * Example use:
// /*AFLA*/  *
// /*AFLA*/  *  if (some_condition)
// /*AFLA*/  *    ring_buffer_discard_commit(buffer, event);
// /*AFLA*/  *  else
// /*AFLA*/  *    ring_buffer_unlock_commit(buffer, event);
// /*AFLA*/  */
// /*AFLA*/ void ring_buffer_discard_commit(struct ring_buffer *buffer,
// /*AFLA*/ 				struct ring_buffer_event *event);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * size is in bytes for each per CPU buffer.
// /*AFLA*/  */
// /*AFLA*/ struct ring_buffer *
// /*AFLA*/ __ring_buffer_alloc(unsigned long size, unsigned flags, struct lock_class_key *key);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Because the ring buffer is generic, if other users of the ring buffer get
// /*AFLA*/  * traced by ftrace, it can produce lockdep warnings. We need to keep each
// /*AFLA*/  * ring buffer's lock class separate.
// /*AFLA*/  */
// /*AFLA*/ #define ring_buffer_alloc(size, flags)			\
// /*AFLA*/ ({							\
// /*AFLA*/ 	static struct lock_class_key __key;		\
// /*AFLA*/ 	__ring_buffer_alloc((size), (flags), &__key);	\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ int ring_buffer_wait(struct ring_buffer *buffer, int cpu, bool full);
// /*AFLA*/ int ring_buffer_poll_wait(struct ring_buffer *buffer, int cpu,
// /*AFLA*/ 			  struct file *filp, poll_table *poll_table);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define RING_BUFFER_ALL_CPUS -1
// /*AFLA*/ 
// /*AFLA*/ void ring_buffer_free(struct ring_buffer *buffer);
// /*AFLA*/ 
// /*AFLA*/ int ring_buffer_resize(struct ring_buffer *buffer, unsigned long size, int cpu);
// /*AFLA*/ 
// /*AFLA*/ void ring_buffer_change_overwrite(struct ring_buffer *buffer, int val);
// /*AFLA*/ 
// /*AFLA*/ struct ring_buffer_event *ring_buffer_lock_reserve(struct ring_buffer *buffer,
// /*AFLA*/ 						   unsigned long length);
// /*AFLA*/ int ring_buffer_unlock_commit(struct ring_buffer *buffer,
// /*AFLA*/ 			      struct ring_buffer_event *event);
// /*AFLA*/ int ring_buffer_write(struct ring_buffer *buffer,
// /*AFLA*/ 		      unsigned long length, void *data);
// /*AFLA*/ 
// /*AFLA*/ struct ring_buffer_event *
// /*AFLA*/ ring_buffer_peek(struct ring_buffer *buffer, int cpu, u64 *ts,
// /*AFLA*/ 		 unsigned long *lost_events);
// /*AFLA*/ struct ring_buffer_event *
// /*AFLA*/ ring_buffer_consume(struct ring_buffer *buffer, int cpu, u64 *ts,
// /*AFLA*/ 		    unsigned long *lost_events);
// /*AFLA*/ 
// /*AFLA*/ struct ring_buffer_iter *
// /*AFLA*/ ring_buffer_read_prepare(struct ring_buffer *buffer, int cpu);
// /*AFLA*/ void ring_buffer_read_prepare_sync(void);
// /*AFLA*/ void ring_buffer_read_start(struct ring_buffer_iter *iter);
// /*AFLA*/ void ring_buffer_read_finish(struct ring_buffer_iter *iter);
// /*AFLA*/ 
// /*AFLA*/ struct ring_buffer_event *
// /*AFLA*/ ring_buffer_iter_peek(struct ring_buffer_iter *iter, u64 *ts);
// /*AFLA*/ struct ring_buffer_event *
// /*AFLA*/ ring_buffer_read(struct ring_buffer_iter *iter, u64 *ts);
// /*AFLA*/ void ring_buffer_iter_reset(struct ring_buffer_iter *iter);
// /*AFLA*/ int ring_buffer_iter_empty(struct ring_buffer_iter *iter);
// /*AFLA*/ 
// /*AFLA*/ unsigned long ring_buffer_size(struct ring_buffer *buffer, int cpu);
// /*AFLA*/ 
// /*AFLA*/ void ring_buffer_reset_cpu(struct ring_buffer *buffer, int cpu);
// /*AFLA*/ void ring_buffer_reset(struct ring_buffer *buffer);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_RING_BUFFER_ALLOW_SWAP
// /*AFLA*/ int ring_buffer_swap_cpu(struct ring_buffer *buffer_a,
// /*AFLA*/ 			 struct ring_buffer *buffer_b, int cpu);
// /*AFLA*/ #else
// /*AFLA*/ static inline int
// /*AFLA*/ ring_buffer_swap_cpu(struct ring_buffer *buffer_a,
// /*AFLA*/ 		     struct ring_buffer *buffer_b, int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return -ENODEV;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ bool ring_buffer_empty(struct ring_buffer *buffer);
// /*AFLA*/ bool ring_buffer_empty_cpu(struct ring_buffer *buffer, int cpu);
// /*AFLA*/ 
// /*AFLA*/ void ring_buffer_record_disable(struct ring_buffer *buffer);
// /*AFLA*/ void ring_buffer_record_enable(struct ring_buffer *buffer);
// /*AFLA*/ void ring_buffer_record_off(struct ring_buffer *buffer);
// /*AFLA*/ void ring_buffer_record_on(struct ring_buffer *buffer);
// /*AFLA*/ int ring_buffer_record_is_on(struct ring_buffer *buffer);
// /*AFLA*/ void ring_buffer_record_disable_cpu(struct ring_buffer *buffer, int cpu);
// /*AFLA*/ void ring_buffer_record_enable_cpu(struct ring_buffer *buffer, int cpu);
// /*AFLA*/ 
// /*AFLA*/ u64 ring_buffer_oldest_event_ts(struct ring_buffer *buffer, int cpu);
// /*AFLA*/ unsigned long ring_buffer_bytes_cpu(struct ring_buffer *buffer, int cpu);
// /*AFLA*/ unsigned long ring_buffer_entries(struct ring_buffer *buffer);
// /*AFLA*/ unsigned long ring_buffer_overruns(struct ring_buffer *buffer);
// /*AFLA*/ unsigned long ring_buffer_entries_cpu(struct ring_buffer *buffer, int cpu);
// /*AFLA*/ unsigned long ring_buffer_overrun_cpu(struct ring_buffer *buffer, int cpu);
// /*AFLA*/ unsigned long ring_buffer_commit_overrun_cpu(struct ring_buffer *buffer, int cpu);
// /*AFLA*/ unsigned long ring_buffer_dropped_events_cpu(struct ring_buffer *buffer, int cpu);
// /*AFLA*/ unsigned long ring_buffer_read_events_cpu(struct ring_buffer *buffer, int cpu);
// /*AFLA*/ 
// /*AFLA*/ u64 ring_buffer_time_stamp(struct ring_buffer *buffer, int cpu);
// /*AFLA*/ void ring_buffer_normalize_time_stamp(struct ring_buffer *buffer,
// /*AFLA*/ 				      int cpu, u64 *ts);
// /*AFLA*/ void ring_buffer_set_clock(struct ring_buffer *buffer,
// /*AFLA*/ 			   u64 (*clock)(void));
// /*AFLA*/ 
// /*AFLA*/ size_t ring_buffer_page_len(void *page);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ void *ring_buffer_alloc_read_page(struct ring_buffer *buffer, int cpu);
// /*AFLA*/ void ring_buffer_free_read_page(struct ring_buffer *buffer, void *data);
// /*AFLA*/ int ring_buffer_read_page(struct ring_buffer *buffer, void **data_page,
// /*AFLA*/ 			  size_t len, int cpu, int full);
// /*AFLA*/ 
// /*AFLA*/ struct trace_seq;
// /*AFLA*/ 
// /*AFLA*/ int ring_buffer_print_entry_header(struct trace_seq *s);
// /*AFLA*/ int ring_buffer_print_page_header(struct trace_seq *s);
// /*AFLA*/ 
// /*AFLA*/ enum ring_buffer_flags {
// /*AFLA*/ 	RB_FL_OVERWRITE		= 1 << 0,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_RING_BUFFER_H */
