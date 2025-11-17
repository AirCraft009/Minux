//
// Created by cocon on 16.11.2025.
//

#include <stdarg.h>
#include "SPut.c"
#include "../../util/string.c"
#include "../../util/rounding.c"

/**
 *
 * @param format specify the format % is the sign after which a format specifier can be set
 * @param ... other arguments that are interpreted by the format specifiers
 * this method doesn't write to a secondary buffer,
 * but writes directly into Vram.
 * it doesn't check if  the ammount of specifiers add up.
 * Or checks security in general
 */
void printf(const char *format, ...) {
    // to be extended in the future
    // can also be optimized
    va_list ap;
    va_start(ap, format);
    int printingColor = White;

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 's': {
                    const char *s = va_arg(ap, const char *);
                    putString(printingColor, s);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(ap, int);
                    putChar(printingColor, c);
                    break;
                }
                case 'd': {
                    int d = va_arg(ap, int);
                    if (d < 0) {
                        putChar(printingColor, '-');
                        d *= -1;
                    }
                    uint8_t digits = digits32(d);
                    char number[digits+1];
                    for (int i = 1; i <= digits; i ++) {
                        number[digits - i] = '0' + d % 10;
                        d /= 10;
                    }
                    number[digits] = '\0';
                    putString(printingColor, number);
                    break;
                }
                case 'b': {
                    const int b = va_arg(ap, int);
                    if (b == 0) {
                        putString(printingColor, "false");
                    }else {
                        putString(printingColor, "true");
                    }
                    break;
                }
                case 'f': {
                    // color (f because of [german] Farbe)
                    const int f = va_arg(ap, int);
                    if (f < 0 || f > 16) {
                        return;
                    }
                    printingColor = f;
                    break;
                }
                case '%':
                    putChar(White, '%');
                    break;
                default:
                    // unsupported specifier handling
                    break;
            }
        } else {
            putChar(printingColor, *format);
        }
        format++;
    }

    va_end(ap);
}

