/* Include guard */
#ifndef TX_CORE_PRINTF_H
#define TX_CORE_PRINTF_H

/* Includes */
#include <core/base.h>

/* Print formatted output using provided putc */
extern void _printf(void (*putc)(char), const char *format, ...);

#endif /* TX_CORE_PRINTF_H */
