// /*AFLA*/ #ifndef _LINUX_FCNTL_H
// /*AFLA*/ #define _LINUX_FCNTL_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/fcntl.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifndef force_o_largefile
// /*AFLA*/ #define force_o_largefile() (BITS_PER_LONG != 32)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if BITS_PER_LONG == 32
// /*AFLA*/ #define IS_GETLK32(cmd)		((cmd) == F_GETLK)
// /*AFLA*/ #define IS_SETLK32(cmd)		((cmd) == F_SETLK)
// /*AFLA*/ #define IS_SETLKW32(cmd)	((cmd) == F_SETLKW)
// /*AFLA*/ #define IS_GETLK64(cmd)		((cmd) == F_GETLK64)
// /*AFLA*/ #define IS_SETLK64(cmd)		((cmd) == F_SETLK64)
// /*AFLA*/ #define IS_SETLKW64(cmd)	((cmd) == F_SETLKW64)
// /*AFLA*/ #else
// /*AFLA*/ #define IS_GETLK32(cmd)		(0)
// /*AFLA*/ #define IS_SETLK32(cmd)		(0)
// /*AFLA*/ #define IS_SETLKW32(cmd)	(0)
// /*AFLA*/ #define IS_GETLK64(cmd)		((cmd) == F_GETLK)
// /*AFLA*/ #define IS_SETLK64(cmd)		((cmd) == F_SETLK)
// /*AFLA*/ #define IS_SETLKW64(cmd)	((cmd) == F_SETLKW)
// /*AFLA*/ #endif /* BITS_PER_LONG == 32 */
// /*AFLA*/ 
// /*AFLA*/ #define IS_GETLK(cmd)	(IS_GETLK32(cmd)  || IS_GETLK64(cmd))
// /*AFLA*/ #define IS_SETLK(cmd)	(IS_SETLK32(cmd)  || IS_SETLK64(cmd))
// /*AFLA*/ #define IS_SETLKW(cmd)	(IS_SETLKW32(cmd) || IS_SETLKW64(cmd))
// /*AFLA*/ 
// /*AFLA*/ #endif
