#include <stdint.h>

#include "io.h"
#include "stdargs.h"
#include "uart.h"


static const char hex[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};


void put(char c)
{
    // need to send \n as \r\n for uart
    if (c == '\n') {
        uart_putc('\r');
    }
    uart_putc(c);
}


void puts(const char *str)
{
    while (*str != '\0') {
        put(*str++);
    }
}


void kprintf(const char *str, ...)
{
    // printf's manpage says it supports: %[flags][width][.precision][length]specifier
    // but we don't support any of that.
    va_list args;
    va_start(args, str);
    for ( ; *str != '\0'; str++) {
        if (*str == '%' && str[1] != '\0') {
            switch (str[1]) {
                case 's': {
                    puts(va_arg(args, const char*));
                    break;
                }
                case 'p': {
                    uint32_t ptr = va_arg(args, uint32_t);
                    char out[9];
                    for (int i = 7; i >= 0; i--) {
                        out[i] = hex[ptr & 0xF];
                        ptr >>= 4;
                    }
                    out[8] = '\0';
                    puts(out);
                    break;
                }
                case '%': {
                    put('%');
                    break;
                }
                default: {
                    break;
                }
            }
            str++;
        } else {
            put(*str);
        }
    }
    va_end(args);
}
