/* Implements io/tty.h */
#include <io/tty.h>

/* Consts */
/* The maximum number of columns */
#define TTY_COLUMNS         128
/* The maximum number of rows */
#define TTY_ROWS            64
/* The background color */
#define COL_BACKGROUND      0x00000077
/* The foreground color */
#define COL_FOREGROUND      0x00FFFFFF

/* Globals */
static struct framebuffer *_fb;
static f32 scale_x = 0.0;
static f32 scale_y = 0.0;
static size_t cursor = 0;

/* Initialize basic TTY */
bool tty_init(struct framebuffer *fb) {
  /* Set framebuffer */
  _fb = fb;

  /* Set scales */
  scale_x = (f32)(fb->width) / (f32)(TTY_COLUMNS);
  scale_y = (f32)(fb->height) / (f32)(TTY_ROWS);

  /* Reset cursor */
  cursor = 0;

  return true;
}

/* Print a character to the TTY */
void tty_putc(char c) {
  /* Handle newline */
  if (c == '\n') {
    cursor += TTY_COLUMNS;
    return;
  }
  /* Handle carriage return */
  if (c == '\r') {
    cursor -= cursor % TTY_COLUMNS;
    return;
  }
  /* Handle backspace */
  if (c == '\b') {
    if (cursor > 0) cursor--;
    return;
  }

  /* If cursor is out of bounds, move screen down, wipe last line */
  if (cursor >= TTY_COLUMNS * TTY_ROWS) {
    cursor -= TTY_COLUMNS;
    memcpy(
        _fb->ptr,
        (u32*)_fb->ptr + (u32)(_fb->width * scale_y),
        (u32)((TTY_ROWS-1) * _fb->width * scale_y) * sizeof(u32)
    );
  }

  /* Find position */
  size_t char_x = cursor % TTY_COLUMNS;
  size_t char_y = cursor / TTY_COLUMNS;
  size_t pos_x = (u32)(char_x * scale_x);
  size_t pos_y = (u32)(char_y * scale_y);

  for (size_t y = 0; y < scale_y; y++) {
    for (size_t x = 0; x < scale_x; x++) {
      volatile u32 *ptr = (_fb->ptr);
      if (
          font8x8_basic[(u8)c][
            (u32)((y / scale_y) * 8)
          ] & (1 << (u32)((x / scale_x) * 8))
      ) {
        ptr[(pos_y + y) * _fb->width + (pos_x + x)] = COL_FOREGROUND;
      } else {
        ptr[(pos_y + y) * _fb->width + (pos_x + x)] = COL_BACKGROUND;
      }
    }
  }

  /* Increment cursor */
  cursor++;
}
/* Print a string to the TTY */
void tty_puts(const char *str) {
  while (*str) {
    tty_putc(*str);
    str++;
  }
}

/* Clear the TTY screen */
void tty_clear(void) {
  for (size_t i = 0; i < _fb->width * _fb->height; i++) {
    volatile u32 *ptr = (_fb->ptr);
    ptr[i] = COL_BACKGROUND;
  }
}
/* Set the cursor position of the TTY */
void tty_set_cursor(size_t x, size_t y) {
  cursor = x + y * TTY_COLUMNS;
}
