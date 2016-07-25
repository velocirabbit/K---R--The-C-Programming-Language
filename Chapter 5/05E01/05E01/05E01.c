/* 05E01.c (p. 97) */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define BUFFERSIZE 1000

int getint(int *pn);
char getch(void);
void ungetch(char c);

char buff[BUFFERSIZE];
int pbuff = 0;

main() {
    int *ip = (int*)malloc(sizeof(int));
    int r;

    while ((r = getint(ip)) != EOF && r != 0)
        printf("%d\n\n", *ip);
    printf("\n");
    system("pause");
    return 0;
}

/**
* getint: Get the next integer from input into *pn.
*
* Returns: EOF for end of file, 0 if the next input is not a number, and a positive
*   value if the input contains a valid number.
*/
int getint(int *pn) {
    int c, sign;

    while (isspace(c = getch()));  // skip white space
    if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
        ungetch(c);  // it's not a number
        return 0;
    }
    sign = (c == '-') ? -1 : 1;
    if (c == '-' || c == '+') {
        char n = getch();
        if (isspace(n)) {
            ungetch(c);
            return 0;
        }
        else if (!isdigit(n)) {
            ungetch(n);
            ungetch(c);
            return 0;
        }
        else
            c = n;
    }
    for (*pn = 0; isdigit(c); c = getch())
        *pn = 10 * *pn + (c - '0');
    *pn *= sign;
    if (c != EOF)
        ungetch(c);
    return c;
}

/**
 * getch: Returns the next character from the input. If there are any characters in the
 *   input buffer, it will get the last character from there, first.
 *
 * Returns: the next character from the input, or EOF for end of file.
 */
char getch(void) {
    return (pbuff > 0) ? buff[--pbuff] : getchar();
}

/**
 * ungetch: Puts char c into the input buffer buff if it isn't full.
 */
void ungetch(char c) {
    if (pbuff < BUFFERSIZE)
        buff[pbuff++] = c;
    else
        printf("ERROR: Input buffer is full!\n");
}