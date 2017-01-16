// /*AFLA*/ #ifndef __LINUX_STRINGHASH_H
// /*AFLA*/ #define __LINUX_STRINGHASH_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>	/* For __pure */
// /*AFLA*/ #include <linux/types.h>	/* For u32, u64 */
// /*AFLA*/ #include <linux/hash.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Routines for hashing strings of bytes to a 32-bit hash value.
// /*AFLA*/  *
// /*AFLA*/  * These hash functions are NOT GUARANTEED STABLE between kernel
// /*AFLA*/  * versions, architectures, or even repeated boots of the same kernel.
// /*AFLA*/  * (E.g. they may depend on boot-time hardware detection or be
// /*AFLA*/  * deliberately randomized.)
// /*AFLA*/  *
// /*AFLA*/  * They are also not intended to be secure against collisions caused by
// /*AFLA*/  * malicious inputs; much slower hash functions are required for that.
// /*AFLA*/  *
// /*AFLA*/  * They are optimized for pathname components, meaning short strings.
// /*AFLA*/  * Even if a majority of files have longer names, the dynamic profile of
// /*AFLA*/  * pathname components skews short due to short directory names.
// /*AFLA*/  * (E.g. /usr/lib/libsesquipedalianism.so.3.141.)
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Version 1: one byte at a time.  Example of use:
// /*AFLA*/  *
// /*AFLA*/  * unsigned long hash = init_name_hash;
// /*AFLA*/  * while (*p)
// /*AFLA*/  *	hash = partial_name_hash(tolower(*p++), hash);
// /*AFLA*/  * hash = end_name_hash(hash);
// /*AFLA*/  *
// /*AFLA*/  * Although this is designed for bytes, fs/hfsplus/unicode.c
// /*AFLA*/  * abuses it to hash 16-bit values.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* Hash courtesy of the R5 hash in reiserfs modulo sign bits */
// /*AFLA*/ #define init_name_hash(salt)		(unsigned long)(salt)
// /*AFLA*/ 
// /*AFLA*/ /* partial hash update function. Assume roughly 4 bits per character */
// /*AFLA*/ static inline unsigned long
// /*AFLA*/ partial_name_hash(unsigned long c, unsigned long prevhash)
// /*AFLA*/ {
// /*AFLA*/ 	return (prevhash + (c << 4) + (c >> 4)) * 11;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Finally: cut down the number of bits to a int value (and try to avoid
// /*AFLA*/  * losing bits).  This also has the property (wanted by the dcache)
// /*AFLA*/  * that the msbits make a good hash table index.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long end_name_hash(unsigned long hash)
// /*AFLA*/ {
// /*AFLA*/ 	return __hash_32((unsigned int)hash);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Version 2: One word (32 or 64 bits) at a time.
// /*AFLA*/  * If CONFIG_DCACHE_WORD_ACCESS is defined (meaning <asm/word-at-a-time.h>
// /*AFLA*/  * exists, which describes major Linux platforms like x86 and ARM), then
// /*AFLA*/  * this computes a different hash function much faster.
// /*AFLA*/  *
// /*AFLA*/  * If not set, this falls back to a wrapper around the preceding.
// /*AFLA*/  */
// /*AFLA*/ extern unsigned int __pure full_name_hash(const void *salt, const char *, unsigned int);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A hash_len is a u64 with the hash of a string in the low
// /*AFLA*/  * half and the length in the high half.
// /*AFLA*/  */
// /*AFLA*/ #define hashlen_hash(hashlen) ((u32)(hashlen))
// /*AFLA*/ #define hashlen_len(hashlen)  ((u32)((hashlen) >> 32))
// /*AFLA*/ #define hashlen_create(hash, len) ((u64)(len)<<32 | (u32)(hash))
// /*AFLA*/ 
// /*AFLA*/ /* Return the "hash_len" (hash and length) of a null-terminated string */
// /*AFLA*/ extern u64 __pure hashlen_string(const void *salt, const char *name);
// /*AFLA*/ 
// /*AFLA*/ #endif	/* __LINUX_STRINGHASH_H */
