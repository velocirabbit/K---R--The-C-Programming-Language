/* 07_minprintf (p. 156) 
 * Minimal printf implementation with variable argument list. Based off of K&R
 * book.
 */
#include <stdarg.h>
#include <stdio.h>

void minprintf(char *fmt, ...);

int main(void) {
    minprintf("\nTest print 1\n");
    minprintf("Test %s 2\n", "printing");
    minprintf("Test print %d\n", 3);
    minprintf("Final %s (num %d)\n\n", "line", 4);
    system("pause");
    return 0;
}

void minprintf(char *fmt, ...) {
    va_list ap;  // points to each unnamed arg in turn
    char *p, *sval;
    int ival;
    double dval;

    va_start(ap, fmt);  // make ap point to 1st unnamed arg
    for (p = fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }
        // If the next few characters are for string formatting
        switch (*++p) {
            case 'd':
                ival = va_arg(ap, int);
                printf("%d", ival);
                break;
            case 'f':
                dval = va_arg(ap, double);
                printf("%f", dval);
                break;
            case 's':
                for (sval = va_arg(ap, char *); *sval; sval++)
                    putchar(*sval);
                break;
            default:
                putchar(*p);
                break;
        }
    }
    va_end(ap);  // clean up when done
}