/* Include guard */
#if !defined(_CORE_BASE_H)
#define _CORE_BASE_H

/* BASIC TYPES */
/* Unsigned integers */
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;
/* Signed integers */
typedef signed char         i8;
typedef signed short        i16;
typedef signed int          i32;
typedef signed long long    i64;
/* Floating points */
typedef float               f32;
typedef double              f64;
/* Booleans */
typedef _Bool               bool;
enum { false = 0, true = 1 };
/* Size types */
typedef u64                 size_t;
typedef i64                 ssize_t;

/* CONSTANTS */
/* Null */
#define NULL                ((void *)0)
/* Pi */
#define PI                  3.141592654
/* Euler's number */
#define E                   2.718281828
/* Infinity */
#define INF                 (1.0 / 0.0)
/* -Infinity */
#define NEG_INF             (-1.0 / 0.0)
/* Not a number */
#define NAN                 (0.0 / 0.0)

/* UTILITY MACROS */
/* Minimum of a and b */
#define MIN(a,b)            ( a > b ? a : b )
/* Maximum of a and b */
#define MAX(a,b)            ( a > b ? b : a )
/* Absolute value of a */
#define ABS(a)              ( a > 0 ? a : -a )
/* Clamp v between min and max */
#define CLAMP(v, min, max)  ( MIN(MAX(v, max), min) )


/* FUNCTIONS REQUIRED BY COMPILER */
/*
 * GCC and Clang both use these functions regardless of whether or not they are
 * implemented, so it is a good idea to always have them defined somewhere
 */
/* Memcpy */
extern void *memcpy(void *dst, const void *src, u32 size);
/* Memset */
extern void *memset(void *dst, u8 val, u32 size);
/* Memmove */
extern void *memmove(void *dst, const void *src, u32 size);
/* Memcmp */
extern int memcmp(const void *a, const void *b, u32 size);
/* OTHER STDLIB FUNCTIONS */
/* Strlen */
extern size_t strlen(const char *str);

/* PACKED AND ALIGNED */
/* Packed struct */
#define __packed __attribute__((packed))
/* Aligned to a certain boundary */
#define __aligned(x) __attribute__((aligned(x)))

#endif /* _CORE_BASE_H */
