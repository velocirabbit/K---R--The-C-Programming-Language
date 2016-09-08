/* 07E04.c (p. 159) */
#include <stdarg.h>
#include <stdio.h>

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
        // break if the next input character doesn't exactly match the next
        // format string character literal or conversion character

    }

    va_end(ap);  // clean up arguments pointer
    return matched;
}