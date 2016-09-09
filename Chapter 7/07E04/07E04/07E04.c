/* 07E04.c (p. 159) */
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

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
            bool init = True;
            while (!match) {
                char fs = *++p;
                switch (fs) {
                    case '*':  // suppression character
                        suppress = True;
                        break;
                    case 'h': case 'l': case 'L':
                        alttype = True;
                        break;
                    case 'd':
                    {
                        int v;
                        if (isdigit(c))
                            v = c - '0';
                        while (isdigit(c = getchar())) {
                            v *= 10;
                            v += c - '0';
                        }
                        if (!alttype) {
                            int *val = va_arg(ap, int*);
                            *val = v;
                        }
                        else {
                            short *val = va_arg(ap, short*);
                            *val = (short)v;
                        }
                        match = True;
                        matched++;
                        break;
                    }
                    case 'i':
                    {
                        if (init && c == '0') {
                            c = getchar();
                            if (c == 'x' || c == 'X')
                                fs = 'x';
                            else
                                fs = 'o';
                        }
                        if (!isdigit(c = getchar()))
                            break;
                    }
                    case 'o':
                    {
                        if (fs == 'o') {

                        }
                    }
                    case 'x': case 'X':
                    {
                        if (fs == 'x' || fs == 'X') {

                        }
                    }
                    default:
                    {

                    }
                }
                init = False;
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