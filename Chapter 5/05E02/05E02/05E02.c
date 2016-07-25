/* 05E02.c (p. 97) */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFERSIZE 1000

int getfloat(float *fp);
char getch(void);
void ungetch(char c);

char buff[BUFFERSIZE];
int pbuff = 0;

main() {
    int r;
    float *fp = (float*)malloc(sizeof(float));
    system("pause");
    while ((r = getfloat(fp)) != EOF && r != 0)
        printf("%f\n\n", *fp);

    printf("\n");
    system("pause");
    return 0;
}

/**
 * getfloat: Gets the next float from the input into *fp.
 *
 * Returns: EOF for end of file, 0 if the next input isn't a number, and 1 if the input
 *   ontains a valid number.
 */
int getfloat(float *fp) {
    int i = 0, sign;
    char s[BUFFERSIZE];

    while (isspace(s[i] = getch()));  // skip white spaces
    if (!isdigit(s[i]) && s[i] != EOF && s[i] != '-' && s[i] != '+') {
        ungetch(s[i]);  // not a number
        return 0;
    }
    sign = (s[i] == '-') ? -1 : 1;
    if (s[i] == '-' || s[i] == '+') {
        char t = getch();
        if (isspace(t)) {
            ungetch(s[i]);
            return 0;
        }
        else if (!isdigit(t)) {
            ungetch(t);
            ungetch(s[i]);
            return 0;
        }
        else
            s[i] = t;
    }
    for (++i; i < BUFFERSIZE && isdigit(s[i] = getch()); i++);
    if (s[i] == '.') {
        i++;
        for (i; i < BUFFERSIZE && isdigit(s[i] = getch()); i++);
    }
    if (s[i] != EOF)
        ungetch(s[i--]);
    s[++i] = 0;
    *fp = atof(s) * (float)sign;
    return 1;
}

/**
 * getch: Gets the next character from the input. If there are any characters in the
 *   input buffer, gets the last one from there instead.
 *
 * Returns: The next character from the input or input buffer buff
 */
char getch(void) {
    return (pbuff > 0) ? buff[--pbuff] : getchar();
}

/**
 * ungetch: Adds char c to the input buffer buff if it isn't full.
 */
void ungetch(char c) {
    if (pbuff < BUFFERSIZE)
        buff[pbuff++] = c;
    else
        printf("ERROR: Input buffer full!\n");
}