/* Implements core/base.h */
#include <core/base.h>

/* Memcpy */
void *memcpy(void *dst, const void *src, u32 size) {
  if (!dst || !src) return NULL;
  if (!size) return dst;
  for (u32 i = 0; i < size; i++) ((u8 *)dst)[i] = ((u8 *)src)[i];
  return dst;
}
/* Memset */
void *memset(void *dst, u8 val, u32 size) {
  if (!dst) return NULL;
  if (!size) return dst;
  for (u32 i = 0; i < size; i++) ((u8 *)dst)[i] = val;
  return dst;
}
/* Memmove */
void *memmove(void *dst, const void *src, u32 size) {
  if (!dst || !src) return NULL;
  if (!size) return dst;
  for (u32 i = 0; i < size; i++) ((u8 *)dst)[i] = ((u8 *)src)[i];
  return dst;
}
/* Memcmp */
int memcmp(const void *a, const void *b, u32 size) {
  if (!a || !b) return 0;
  if (!size) return 0;
  for (u32 i = 0; i < size; i++) if (((u8 *)a)[i] != ((u8 *)b)[i]) return 1;
  return 0;
}
/* Strlen */
size_t strlen(const char *str) {
  size_t len = 0;
  if (!str) return 0;
  while (*str++) len++;
  return len;
}
