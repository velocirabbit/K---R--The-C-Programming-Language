/* 07E04.c (p. 159) */
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* Function macros */
#define ISHEX(c)    (isdigit(c) || (tolower(c) >= 'a' && tolower(c) <= 'f'))
#define HEXVAL(c)   (isdigit(c) ? c - '0' : 10 + tolower(c) - 'a')

#define CONVCHAR    '%'  // indicates a conversion character sequence to follow
/* Basic scanf conversion characters */
#define DECIMAL     'd'  // deciaml integer
#define INTEGER     'i'  // integer
#define OCTAL       'o'  // octal integer
#define UNSIGNED    'u'  // unsigned decimal integer
#define HEXADECIMAL 'x'  // hexadecimal integer
#define CHARACTER   'c'  // characters
#define CHARSTRING  's'  // character string
#define EXPONENT    'e'  // floating-point number with exponent
#define FLOAT       'f'  // floating-point number (expanded)
#define EXPORFLOAT  'g'  // floating-point number (chooses 'e' or 'f')
#define PERCENTCHAR '%'  // literal '%'

typedef enum { False = 0, True } bool;

int minscanf(char *format, ...);

int main(void) {
    return 0;
}

int minscanf(char *format, ...) {
    // configure arguments pointer and point to first variable argument
    va_list ap;
    va_start(ap, format);

    int matched = 0;
    char *p;
    for (p = format; *p; p++) {
        // read characters from standard input
        int c = getchar();
        if (c == EOF)
            return c;
        else if (*p == '%') {
            // Look at the next character and evaluate correct formatting. If
            // the next char isn't a known conversion character, break
            bool match = False;
            bool suppress = False;
            bool alttype = False;
            int maxwidth;
            while (!match) {
                char fs = *++p;
                switch (fs) {
                    case '*':  // suppression character
                        suppress = True;
                        break;
                    case 'h': case 'l': case 'L':
                        alttype = True;
                        break;
                    case 'd': case 'i': case 'o': case 'x': case 'X': case 'u':
                    {
                        int v = 0;
                        bool neg = False;
                        bool octal = (fs == 'o') ? True : False;
                        bool hexa = (tolower(fs) == 'x') ? True : False;
                        if (fs != 'u' && c == '-') {
                            neg = True;
                            c = getchar();
                        }
                        if (fs == 'i' && !octal && !hexa) {
                            if (c == '0') {
                                c = getchar();
                                if (tolower(c) == 'x')
                                    hexa = True;
                                else if (isdigit(c)) {
                                    octal = True;
                                    v = c - '0';
                                }
                            }
                            else if (isdigit(c))
                                v = c - '0';
                            else
                                break;
                        }
                        else if (isdigit(c))
                            v = c - '0';
                        else
                            break;
                        if (hexa) {
                            c = getchar();
                            while (ISHEX(c)) {
                                v *= 16;
                                v += HEXVAL(c);
                            }
                        }
                        else
                            while (isdigit(c = getchar())) {
                                v *= octal ? 8 : 10;
                                v += c - '0';
                            }
                        if (!alttype) {
                            int *val = va_arg(ap, int*);
                            *val = (neg ? -1 : 1) * v;
                            *val = (fs == 'u') ? (unsigned)*val : *val;
                        }
                        else {
                            short *val = va_arg(ap, short*);
                            *val = (short)(neg ? -1 : 1) * (short)v;
                            *val = (fs == 'u') ? (unsigned)*val : *val;
                        }
                        match = True;
                        break;
                    }
                    case 'c':
                    {
                        char *val = va_arg(ap, char*);
                        *val = getchar();
                        match = True;
                        break;
                    }
                    case 's':
                    {
                        char *val = va_arg(ap, char*);
                        if (!isblank(c))
                            *val++ = c;
                        while (!isblank(c = getchar()))
                            *val++ = c;
                        *val = '\0';
                        match = True;
                        break;
                    }
                    default:
                    {
                        // Get max field width
                        if (isdigit(fs)) {
                            maxwidth = fs - '0';
                            while (isdigit((fs = *++p))) {
                                maxwidth *= 10;
                                maxwidth += fs - '0';
                            }
                            p--;
                        }
                    }
                }
                if (match)
                    matched++;
            }
        }
        else if (isspace(*p) || *p == c)
            // If the next char isn't a part of a format specifier, then check
            // to see if it matches the next character in the format string.
            // If it matches exactly, move on to the next one. If not, break
            continue;
        else
            break;
    }

    va_end(ap);  // clean up arguments pointer
    return matched;
}