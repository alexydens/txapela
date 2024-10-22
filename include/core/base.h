/* Include guard */
#ifndef TX_CORE_BASE_H
#define TX_CORE_BASE_H

/* --- BASIC TYPES --- */
/* UNSIGNED INTEGERS */
/* An 8-bit unsigned integer */
typedef unsigned char       u8;
/* A 16-bit unsigned integer */
typedef unsigned short      u16;
/* A 32-bit unsigned integer */
typedef unsigned int        u32;
/* A 64-bit unsigned integer */
typedef unsigned long       u64;
/* SIGNED INTEGERS */
/* A 8-bit signed integer */
typedef signed char         i8;
/* A 16-bit signed integer */
typedef signed short        i16;
/* A 32-bit signed integer */
typedef signed int          i32;
/* A 64-bit signed integer */
typedef signed long         i64;
/* FLOATING POINT */
/* A 32-bit floating point number */
typedef float               f32;
/* A 64-bit floating point number */
typedef double              f64;
/* BOOLEAN */
/* A boolean value */
typedef u8                  bool;
enum { false, true };
/* STATIC ASSERT ALL THE TYPE SIZES */
_Static_assert(sizeof(u8) == 1, "u8 should be 1 byte");
_Static_assert(sizeof(u16) == 2, "u16 should be 2 bytes");
_Static_assert(sizeof(u32) == 4, "u32 should be 4 bytes");
_Static_assert(sizeof(u64) == 8, "u64 should be 8 bytes");
_Static_assert(sizeof(i8) == 1, "i8 should be 1 byte");
_Static_assert(sizeof(i16) == 2, "i16 should be 2 bytes");
_Static_assert(sizeof(i32) == 4, "i32 should be 4 bytes");
_Static_assert(sizeof(i64) == 8, "i64 should be 8 bytes");
_Static_assert(sizeof(f32) == 4, "f32 should be 4 bytes");
_Static_assert(sizeof(f64) == 8, "f64 should be 8 bytes");
_Static_assert(sizeof(bool) == 1, "bool should be 1 byte");

/* --- TYPE ALIASES AND STRUCTS --- */
/* Size type */
typedef u64 size_t;

/* --- ATTRIBUTES --- */
/* Packed structs are always stored how they are written */
#define __packed      __attribute__((packed))
/* Aligned variables are aligned on the boundary specified */
#define __aligned(x)  __attribute__((aligned(x)))
/* Putting something into a particular linker section */
#define __section(x)  __attribute__((used, section(x)))

/* --- MACROS --- */
/* Return the number of elements in an array */
#define ARRLEN(x)   (sizeof(x) / sizeof(x[0]))
/* Swap two variables */
#define SWAP(x, y)  do { __typeof(x) tmp = x; x = y; y = tmp; } while (0)
/* Get maximum value */
#define MAX(x, y)   ((x) > (y) ? (x) : (y))
/* Get minimum value */
#define MIN(x, y)   ((x) < (y) ? (x) : (y))
/* Get the absolute value of a number */
#define ABS(x)      ((x) < 0 ? -(x) : (x))
/* Clamp a value between two numbers */
#define CLAMP(x, min, max)  ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

/* --- CONSTANTS --- */
/* MATHEMATICS */
/* Pi (half the way around a circle in radians) */
#define PI                3.14159265358979323846
/* Euler's number */
#define E                 2.71828182845904523536
/* FLOATING POINT VALUES */
/* Infinity */
#define INFINITY          (1.0 / 0.0)
/* -Infinity */
#define NEG_INFINITY      (-1.0 / 0.0)
/* NaN */
#define NAN               (0.0 / 0.0)
/* SIZE PREFIXES */
/* One Kibibyte */
#define KiB                1024
/* One Mebibyte */
#define MiB                (1024 * 1024)
/* One Gibibyte */
#define GiB                (1024 * 1024 * 1024)
/* One Tebibyte */
#define TiB                (1024 * 1024 * 1024 * 1024)
/* MINIMUM/MAXIMUM VALUES FOR EACH TYPE */
/* Max value of u8 */
#define MAX_U8             0xFF
/* Max value of u16 */
#define MAX_U16            0xFFFF
/* Max value of u32 */
#define MAX_U32            0xFFFFFFFF
/* Max value of u64 */
#define MAX_U64            0xFFFFFFFFFFFFFFFF
/* Min value of i8 */
#define MIN_I8             -128
/* Max value of i8 */
#define MAX_I8             127
/* Min value of i16 */
#define MIN_I16            -32768
/* Max value of i16 */
#define MAX_I16            32767
/* Min value of i32 */
#define MIN_I32            -2147483648
/* Max value of i32 */
#define MAX_I32            2147483647
/* Min value of i64 */
#define MIN_I64            -9223372036854775808
/* Max value of i64 */
#define MAX_I64            9223372036854775807
/* MISC */
/* A null pointer */
#define NULL               ((void*)0)

/* FUNCTIONS REQUIRED BY COMPILER */
/* Memset */
static inline void *memset(void *dst, u8 val, size_t size) {
  size_t i;
  if (!dst) return NULL;
  if (!size) return dst;
  for (i = 0; i < size; i++) ((u8 *)dst)[i] = val;
  return dst;
}
/* Memcpy */
static inline void *memcpy(void *dst, const void *src, size_t size) {
  size_t i;
  if (!dst || !src) return NULL;
  if (!size) return dst;
  for (i = 0; i < size; i++) ((u8 *)dst)[i] = ((u8 *)src)[i];
  return dst;
}
/* Strlen */
static inline u32 strlen(const char *str) {
  size_t len = 0;
  if (!str) return 0;
  while (str[len]) len++;
  return len;
}
/* Memmove */
static inline void *memmove(void *dest, const void *src, size_t n) {
  u8 *pdest = (u8 *)dest;
  const u8 *psrc = (const u8 *)src;
  size_t i;

  if (src > dest) {
    for (i = 0; i < n; i++) {
      pdest[i] = psrc[i];
    }
  } else {
    for (i = n; i > 0; i--) {
      pdest[i - 1] = psrc[i - 1];
    }
  }

  return dest;
}
/* Memcmp */
static inline int memcmp(const void *s1, const void *s2, size_t n) {
  const u8 *p1 = (const u8 *)s1;
  const u8 *p2 = (const u8 *)s2;
  size_t i;

  for (i = 0; i < n; i++) {
    if (p1[i] != p2[i]) {
      return p1[i] - p2[i];
    }
  }

  return 0;
}

#endif /* TX_CORE_BASE_H */
