/* 07E03.c (p. 156) */
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


#define INTSTRLEN 8
#define MAXSTRLEN 80

typedef enum { False = 0, True } bool;

void minprintf(char *fmt, ...);
void formatstring(double v, char t, bool a, int minw, int pre);
int numdigits(double v, int base, int pre);
void xputchar(char c, int n);

int main(void) {
    int i = 1;
    minprintf("\nTest print %-5i for good luck!\n", i++);
    minprintf("Test %s %o to start this off\n", "printing", 12 + i++);
    minprintf("Test print %15.10d for good measure\n", i++);
    minprintf("A test print (#%-4.4f) with floats\n", i++);
    minprintf("Final %s (num %x)\n", "line", i++);
    minprintf("JK, here is #%10d, involving a negative\n", -1 * i++);
    minprintf("And here is a float: %4.3f that's also negative!\n", -93.242 + i++);
    minprintf("Here's another.... %-15.12f. TADA!\n", -5220.520 + i++);
    minprintf("What if I threw an %8.8E in?!\n", -42.2 + i++);
    minprintf("Or this weird G %-12.12g thing?!\n", 117.120 + i++);
    minprintf("What about a float <1 such as %4.8f? Weird.\n", 0.3429);
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
        switch (*p) {
            case 'd': case 'i': case 'o':
            case 'x': case 'X': case 'u': case 'c':
                ival = va_arg(ap, int);
                formatstring((double)ival, *p, leftalign,
                    minwidth, precision);
                break;
            case 'f': case 'e': case 'E': case 'g': case 'G':
                dval = va_arg(ap, double);
                formatstring(dval, *p, leftalign,
                    minwidth, precision);
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

void formatstring(double v, char t, bool a, int minw, int pre) {
    int numd;
    int inumd;
    char s[MAXSTRLEN];
    switch (t) {
        case 'd': case 'i': case 'u':
            numd = numdigits(v, 10, pre);
            if (!a && (minw > 0 && numd < minw))
                xputchar(' ', minw - numd);
            _itoa((int)v, s, 10);
            for (int i = 0; s[i] != '\0'; i++)
                putchar(s[i]);
            if (a && numd < minw)
                xputchar(' ', minw - numd);
            break;
        case 'o':
            numd = numdigits(v, 8, pre);
            if (!a && (minw > 0 && numd < minw))
                xputchar(' ', minw - numd);
            _itoa((int)v, s, 8);
            for (int i = 0; s[i] != '\0'; i++)
                putchar(s[i]);
            if (a && numd < minw)
                xputchar(' ', minw - numd);
            break;
        case 'x': case 'X':
            numd = numdigits(v, 16, pre);
            if (a && numd < minw)
                xputchar(' ', minw - numd);
            _itoa((int)v, s, 16);
            for (int i = 0; s[i] != '\0'; i++)
                putchar(s[i]);
            if (!a && numd < minw)
                xputchar(' ', minw - numd);
            break;
        case 'c':
            putchar((char)v);
            break;
        case 'g': case 'G':
            numd = numdigits(v, 10, pre);
            inumd = numdigits((int)v, 10, pre);
            if (numd - inumd > 4 || inumd >= pre)
                t = (t == 'g') ? 'e' : 'E';
            else
                t = 'f';
            // continue on to either of the next two...
        case 'f':
        {
            if (t == 'f') {  // run this if it matches 'f'
                numd = numdigits(v, 10, pre);
                inumd = numdigits((int)v, 10, pre);
                if (!a && numd < minw)
                    xputchar(' ', minw - numd);
                _itoa((int)v, s, 10);
                for (int i = 0; s[i] != '\0'; i++)
                    putchar(s[i]);
                putchar('.');
                v -= (int)v;
                v = (v < 0) ? -v : v;
                for (int i = 0; i < pre && v != 0.0; i++) {
                    _itoa((int)(v *= 10), s, 10);
                    putchar(s[0]);
                    v -= (int)v;
                }
                if (a && numd < minw)
                    xputchar(' ', minw - numd);
            }
        }
        case 'e': case 'E':
        {
            if (t == 'e' || t == 'E') {
                numd = numdigits(v, 10, pre);
                inumd = numdigits((int)v, 10, pre);
                int exp = 0;
                if (inumd > 1) {
                    exp = inumd - 1;
                    if (v < 0) exp--;
                    for (int i = 0; i < exp; i++, v /= 10.0);
                }
                else if (inumd == 0)
                    for (v; v < 0.0; exp++, v *= 10.0);
                _itoa((int)v, s, 10);
                putchar(s[0]);
                if (v < 0) putchar(s[1]);
                putchar('.');
                v -= (int)v;
                v = (v < 0) ? -v : v;
                for (int i = 0; i < pre && v != 0.0; i++) {
                    _itoa((int)(v *= 10), s, 10);
                    putchar(s[0]);
                    v -= (int)v;
                }
                putchar((t == 'e') ? 'e' : 'E');
                _itoa(exp, s, 10);
                for (int i = 0; s[i] != '\0'; i++)
                    putchar(s[i]);
                break;
            }
        }
        default:
            break;
    }
}

int numdigits(double v, int base, int pre) {
    int n = 0;
    if (v < 0) {
        n++;
        v = -v;
    }
    int i = (int)v;
    for (n; i > 0; i /= base, n++);  // int truncation
    if (v - (int)v != 0) {
        n++;  // for the '.'
        v -= (int)v;  // just the decimal portion
        int d = 0;
        for (v *= base; v > 0.0 && d < pre; v = (v - (int)v) * base, d++, n++);
    }
    return n;
}

void xputchar(char c, int n) {
    for (int i = 0; i < n; i++)
        putchar(c);
}