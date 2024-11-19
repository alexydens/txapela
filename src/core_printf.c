/* Implements core/printf.h */
#include <core/printf.h>
#include <ext/stdarg.h>

/* Consts */
/* The flags */
#define FORMAT_FLAGS_ZEROPAD      (1 << 0)
#define FORMAT_FLAGS_FORCESIGN    (1 << 1)
#define FORMAT_FLAGS_SPACESIGN    (1 << 2)
#define FORMAT_FLAGS_BASESPECIFY  (1 << 3)
/* (I don't implement the rest) */
/* The length */
#define FORMAT_LENGTH_BYTE        (1 << 0) /* 8 bits */
#define FORMAT_LENGTH_SHORT       (1 << 1) /* 16 bits */
#define FORMAT_LENGTH_INT         (1 << 2) /* 32 bits */
#define FORMAT_LENGTH_LONG        (1 << 3) /* 64 bits */
#define FORMAT_LENGTH_LONG_LONG   (1 << 4) /* Also 64 bits */
/* (I don't implement the rest) */
/* Hex digits */
const char HEXDIGITS[] = "0123456789abcdef";
const char HEXDIGITS_CAPITAL[] = "0123456789ABCDEF";

/* Print formatted output using provided putc */
void _printf(void (*_putc)(char), const char *format, ...) {
  va_list args;
  const char *ptr = format;
  va_start(args, format);

  /* The format specifier */
  struct {
    u8 flags;
    u16 width;
    u16 precision;
    u16 length;
  } state;
  /* The padding required */
  u16 padding;
  /* Iteration */
  u32 i;
  /* Number of digits */
  u16 digits;
  /* Temps */
  i32 signed_temp;
  u32 unsigned_temp;
  /* The argument itself, if storing it is needed */
  union {
    const char *str;
    u32 uint;
    i32 sint;
    f32 real;
  } arg;

  while (*ptr != '\0') {
    if (*ptr == '%') {
      ptr++;
      /* Format specifier: %[flags][width][.precision][length]specifier */
      /* Flags */
      state.flags = 0;
      while (*ptr == '0' || *ptr == '+' || *ptr == '#' || *ptr == ' ') {
        switch (*ptr) {
          case '0':
            state.flags |= FORMAT_FLAGS_ZEROPAD;
            ptr++;
            break;
          case '+':
            state.flags |= FORMAT_FLAGS_FORCESIGN;
            ptr++;
            break;
          case '#':
            state.flags |= FORMAT_FLAGS_BASESPECIFY;
            ptr++;
            break;
          case ' ':
            state.flags |= FORMAT_FLAGS_SPACESIGN;
            ptr++;
            break;
        }
      }
      /* Width */
      state.width = 0;
      if (*ptr == '*') {
        ptr++;
        state.width = va_arg(args, u32);
      } else {
        if (*ptr >= '0' && *ptr <= '9') {
          state.width = 0;
          while (*ptr >= '0' && *ptr <= '9') {
            state.width *= 10;
            state.width += *ptr - '0';
            ptr++;
          }
        }
      }
      /* Precision */
      state.precision = 0;
      if (*ptr == '.') {
        ptr++;
        if (*ptr == '*') {
          ptr++;
          state.precision = va_arg(args, u32);
        } else {
          if (*ptr >= '0' && *ptr <= '9') {
            while (*ptr >= '0' && *ptr <= '9') {
              state.precision *= 10;
              state.precision += *ptr - '0';
              ptr++;
            }
          }
        }
      }
      /* Length */
      state.length = 0;
      switch (*ptr) {
        case 'h':
          ptr++;
          if (*ptr == 'h') {
            state.length = FORMAT_LENGTH_BYTE;
            ptr++;
          }
          else state.length = FORMAT_LENGTH_SHORT;
          break;
        case 'l':
          ptr++;
          if (*ptr == 'l') {
            state.length = FORMAT_LENGTH_LONG_LONG;
            ptr++;
          }
          else state.length = FORMAT_LENGTH_LONG;
          break;
        default:
          state.length = FORMAT_LENGTH_INT;
          break;
      }
      /* Specifier */
      switch (*ptr) {
        case 'd':
        case 'i':
          ptr++;
          /* Print integer */
          i = 0;
          arg.sint = va_arg(args, i32);
          if (arg.sint < 0) {
            _putc('-');
            arg.sint = -arg.sint;
            i++;
          } else {
            if (state.flags & FORMAT_FLAGS_FORCESIGN) {
              _putc('+');
              i++;
            }
            if (state.flags & FORMAT_FLAGS_SPACESIGN) {
              _putc(' ');
              i++;
            }
          }
          digits = 0;
          signed_temp = arg.sint;
          while (signed_temp != 0) {
            signed_temp /= 10;
            digits++;
          }
          if (!arg.sint) digits=1;
          if (state.flags & FORMAT_FLAGS_ZEROPAD) {
            if (digits < state.width) {
              padding = state.width - digits - i;
              for (; padding > 0; padding--) _putc('0');
            }
          }
          for (u16 j = 0; j < digits; j++) {
            signed_temp = arg.sint;
            for (u16 k = 0; k < digits - 1 - j; k++) signed_temp /= 10;
            _putc('0' + signed_temp % 10);
            i++;
          }
          if (!(state.flags & FORMAT_FLAGS_ZEROPAD))
            for (padding = i; padding < state.width; padding++) _putc(' ');
          break;
        case 'u':
          ptr++;
          /* Print unsigned */
          i = 0;
          arg.sint = va_arg(args, i32);
          digits = 0;
          unsigned_temp = arg.sint;
          while (unsigned_temp != 0) {
            unsigned_temp /= 10;
            digits++;
          }
          if (!arg.sint) digits=1;
          if (state.flags & FORMAT_FLAGS_ZEROPAD) {
            if (digits < state.width) {
              padding = state.width - digits - i;
              for (; padding > 0; padding--) _putc('0');
            }
          }
          for (u16 j = 0; j < digits; j++) {
            unsigned_temp = arg.sint;
            for (u16 k = 0; k < digits - 1 - j; k++) unsigned_temp /= 10;
            _putc('0' + unsigned_temp % 10);
            i++;
          }
          if (!(state.flags & FORMAT_FLAGS_ZEROPAD))
            for (padding = i; padding < state.width; padding++) _putc(' ');
          break;
        case 'x':
          ptr++;
          /* Print hex */
          arg.uint = va_arg(args, u32);
          i = 0;
          if (state.flags & FORMAT_FLAGS_BASESPECIFY) {
            _putc('0');
            _putc('x');
            i += 2;
          }
          digits = 0;
          unsigned_temp = arg.uint;
          while (unsigned_temp != 0) {
            unsigned_temp /= 16;
            digits++;
          }
          if (!arg.sint) digits=1;
          if (state.flags & FORMAT_FLAGS_ZEROPAD) {
            if (digits < state.width) {
              padding = state.width - digits - i;
              for (; padding > 0; padding--) _putc('0');
            }
          }
          for (u16 j = 0; j < digits; j++) {
            unsigned_temp = arg.uint;
            for (u16 k = 0; k < digits - 1 - j; k++) unsigned_temp /= 16;
            _putc(HEXDIGITS[unsigned_temp & 0xf]);
            i++;
          }
          if (!(state.flags & FORMAT_FLAGS_ZEROPAD))
            for (padding = i; padding < state.width; padding++) _putc(' ');
          break;
        case 'X':
          ptr++;
          /* Print hex, capital */
          arg.uint = va_arg(args, u32);
          i = 0;
          if (state.flags & FORMAT_FLAGS_BASESPECIFY) {
            _putc('0');
            _putc('X');
            i += 2;
          }
          digits = 0;
          unsigned_temp = arg.uint;
          while (unsigned_temp != 0) {
            unsigned_temp /= 16;
            digits++;
          }
          if (!arg.sint) digits=1;
          if (state.flags & FORMAT_FLAGS_ZEROPAD) {
            if (digits < state.width) {
              padding = state.width - digits - i;
              for (; padding > 0; padding--) _putc('0');
            }
          }
          for (u16 j = 0; j < digits; j++) {
            unsigned_temp = arg.uint;
            for (u16 k = 0; k < digits - 1 - j; k++) unsigned_temp /= 16;
            _putc(HEXDIGITS_CAPITAL[unsigned_temp & 0x0000000f]);
            i++;
          }
          if (!(state.flags & FORMAT_FLAGS_ZEROPAD))
            for (padding = i; padding < state.width; padding++) _putc(' ');
          break;
        case 'f':
          ptr++;
          /* Print float */
          //arg.real = va_arg(args, f64);
          arg.sint = va_arg(args, i64);
          /* 
             I cannot be bothered to implement this - I don't see myself ever
           * needing to use it.
           */
          _printf(_putc, "%d", (i32)arg.sint);
          break;
        case 'c':
          ptr++;
          /* Print char */
          _putc(va_arg(args, i32));
          for (padding = 1; padding < state.width; padding++) _putc(' ');
          break;
        case 's':
          ptr++;
          /* Print string */
          arg.str = va_arg(args, const char *);
          if (state.precision)
          for (
              i = 0;
              arg.str[i] != '\0' && i < state.precision;
              i++
          ) {
            _putc(arg.str[i]);
          }
          else for (i = 0; arg.str[i] != '\0'; i++) _putc(arg.str[i]);
          for (padding = i; padding < state.width; padding++) _putc(' ');
          break;
        case '%':
          ptr++;
          _putc('%');
          break;
      }
    } else {
      _putc(*ptr);
      ptr++;
    }
  }

  va_end(args);
}
