// /*AFLA*/ #ifndef _LINUX_RANGE_H
// /*AFLA*/ #define _LINUX_RANGE_H
// /*AFLA*/ 
// /*AFLA*/ struct range {
// /*AFLA*/ 	u64   start;
// /*AFLA*/ 	u64   end;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ int add_range(struct range *range, int az, int nr_range,
// /*AFLA*/ 		u64 start, u64 end);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ int add_range_with_merge(struct range *range, int az, int nr_range,
// /*AFLA*/ 				u64 start, u64 end);
// /*AFLA*/ 
// /*AFLA*/ void subtract_range(struct range *range, int az, u64 start, u64 end);
// /*AFLA*/ 
// /*AFLA*/ int clean_sort_range(struct range *range, int az);
// /*AFLA*/ 
// /*AFLA*/ void sort_range(struct range *range, int nr_range);
// /*AFLA*/ 
// /*AFLA*/ #define MAX_RESOURCE ((resource_size_t)~0)
// /*AFLA*/ static inline resource_size_t cap_resource(u64 val)
// /*AFLA*/ {
// /*AFLA*/ 	if (val > MAX_RESOURCE)
// /*AFLA*/ 		return MAX_RESOURCE;
// /*AFLA*/ 
// /*AFLA*/ 	return val;
// /*AFLA*/ }
// /*AFLA*/ #endif
