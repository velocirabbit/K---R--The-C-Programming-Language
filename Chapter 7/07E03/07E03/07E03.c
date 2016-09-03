/* 07E03.c (p. 156) */
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define INTSTRLEN 8
#define MAXSTRLEN 80

typedef enum { False = 0, True } bool;

void minprintf(char *fmt, ...);
char *formatstring(double v, char t, bool a, int minw, int pre);

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
        
        // Format specification variables
        bool leftalign = False;
        char *minwidth_s = malloc(sizeof(char[INTSTRLEN]));
        char *precision_s = malloc(sizeof(char[INTSTRLEN]));
        int minwidth = 0;
        int precision = 6;  // default 6 places after decimal are printed
        // If the next few characters are for string formatting
        if (*++p == '-') {
            leftalign = True;
            p++;
        }
        if (isdigit(*p)) {
            char *mw_p = minwidth_s;
            while (isdigit(*p))
                *mw_p++ = *p++;
            *mw_p = '\0';
            minwidth = atoi(minwidth_s);
        }
        else if (*p == '*') {
            minwidth = va_arg(ap, int);
            p++;
        }
        if (*p == '.') {
            p++;
            if (isdigit(*p)) {
                char *pre_p = precision_s;
                while (isdigit(*p))
                    *pre_p++ = *p++;
                *pre_p = '\0';
                precision = atoi(precision_s);
            }
            else if (*p == '*') {
                precision = va_arg(ap, int);
                p++;
            }
        }
        char *fstr = malloc(sizeof(char[MAXSTRLEN]));
        switch (*p) {
            case 'd': case 'i': case 'o':
            case 'x': case 'X': case 'u': case 'c':
                ival = va_arg(ap, int);
                fstr = formatstring((double)ival, *p, leftalign,
                    minwidth, precision);
                printf("%s", fstr);
                break;
            case 'f': case 'e': case 'E': case 'g': case 'G':
                dval = va_arg(ap, double);
                fstr = formatstring(dval, *p, leftalign,
                    minwidth, precision);
                printf("%s", fstr);
                break;
            case 's':
                for (sval = va_arg(ap, char *); *sval; sval++)
                    putchar(*sval);
                break;
            case '%':
                putchar(*p);
                break;
            default:
                // do nothing if any of the chars after the '%' aren't
                // one of the above
                break;
        }

        free(minwidth_s);
        free(precision_s);
    }
    va_end(ap);  // clean up when done
}

char *formatstring(double v, char t, bool a, int minw, int pre) {
    char *fstr = malloc(sizeof(char[MAXSTRLEN]));
    switch (t) {
        case 'd': case 'i': case 'o':
        case 'x': case 'X': case 'u': case 'c':

            break;
        case 'f': case 'e': case 'E': case 'g': case 'G':

            break;
        default:
            break;
    }
    return fstr;
}