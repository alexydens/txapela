/* Include guard */
#if !defined(_CORE_PRINTF_H)
#define _CORE_PRINTF_H

/* Includes */
#include <core/base.h>
#include <ext/stdarg.h>

/* Print formatted output using provided putc */
extern void _printf(void (*_putc)(char), const char *format, ...);

#endif /* _CORE_PRINTF_H */
