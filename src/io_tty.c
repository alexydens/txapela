/* Implements io/tty.h */
#include <io/tty.h>

/* Consts */
/* How many pixels in both directions a character takes up */
//#define pixels_per_char     16
//#define pixels_per_char     8
/* The maximum number of columns */
#define MAX_COLUMNS         96
/* The maximum number of rows */
#define MAX_ROWS            64
/* The background color */
#define COL_BACKGROUND      0x00000077
/* The foreground color */
#define COL_FOREGROUND      0x00FFFFFF

/* Globals */
static struct framebuffer *_fb;
static size_t columns = 0;
static size_t rows = 0;
static size_t cursor = 0;
static size_t pixels_per_char = 0;
static size_t scale = 0;
static char charbuf[MAX_COLUMNS * MAX_ROWS];

/* Re-render the TTY */
void _tty_refresh(void) {
  size_t i;
  for (i = 0; i < rows * columns; i++) {
    char c = charbuf[i];
    size_t x, y;
    for (y = 0; y < pixels_per_char; y++) {
      for (x = 0; x < pixels_per_char; x++) {
        volatile u32 *ptr = (volatile u32 *)_fb->ptr;
        //if (font8x8_basic[(u8)c][y/2] & (1 << (x/2))) {
        //if (font8x8_basic[(u8)c][y] & (1 << (x))) {
        if (font8x8_basic[(u8)c][y/scale] & (1 << (x/scale))) {
          ptr[
            x + (i % columns) * pixels_per_char
            + (y + (i / columns) * pixels_per_char) * _fb->width
          ] = cursor == i ? COL_BACKGROUND : COL_FOREGROUND;
        } else {
          ptr[
            x + (i % columns) * pixels_per_char
            + (y + (i / columns) * pixels_per_char) * _fb->width
          ] = cursor == i ? COL_FOREGROUND : COL_BACKGROUND;
        }
      }
    }
  }
}

/* Put a character without updating TTY */
void _tty_putc_norefresh(char c) {
  if (cursor >= columns * rows) {
    cursor = columns * (rows - 1);
    memcpy(charbuf, charbuf + columns, columns * (rows - 1));
  }
  switch (c) {
    case '\n':
      cursor += columns;
      break;
    case '\r':
      cursor -= cursor % columns;
      break;
    default:
      charbuf[cursor] = c;
      cursor++;
      break;
  }
}

/* Initialize basic TTY */
bool tty_init(struct framebuffer *fb, size_t scale_up) {
  /* Set framebuferr */
  _fb = fb;

  /* Set the pixels per character */
  pixels_per_char = scale_up * 8;
  scale = scale_up;

  /* Set dimensions of terminal */
  columns = CLAMP(_fb->width / pixels_per_char, 1, MAX_COLUMNS);
  rows = CLAMP(_fb->height / pixels_per_char, 1, MAX_ROWS);

  return true;
}

/* Change TTY scale */
void tty_scale(size_t scale_up) {
  /* Set the pixels per character */
  pixels_per_char = scale_up * 8;
  scale = scale_up;

  /* Set dimensions of terminal */
  columns = CLAMP(_fb->width / pixels_per_char, 1, MAX_COLUMNS);
  rows = CLAMP(_fb->height / pixels_per_char, 1, MAX_ROWS);

  /* Reset the TTY */
  cursor = 0;
  memset(charbuf, 0, sizeof(charbuf));

  /* Re-draw */
  _tty_refresh();
}

/* Print a character to the TTY */
void tty_tty_putc_norefresh(char c) {
  _tty_putc_norefresh(c);
  _tty_refresh();
}
/* Print a string to the TTY */
void tty_puts(const char *str) {
  while (*str) {
    _tty_putc_norefresh(*str);
    str++;
  }
  _tty_refresh();
}

/* Clear the TTY screen */
void tty_clear(void) {
  cursor = 0;
  memset(charbuf, 0, sizeof(charbuf));
  _tty_refresh();
}
/* Set the cursor position of the TTY */
void tty_set_cursor(size_t x, size_t y) {
  cursor = y * columns + x;
}
