/* 05_getint.c (p. 97) */
#include <stdio.h>
#include <ctype.h>

#define BUFFERSIZE 1000

int getint(int *pn);
char getch(void);
void ungetch(char c);

char buff[BUFFERSIZE];
int pbuff = 0;

main() {

}

/**
 * getint: Get next integer from input into *pn.
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
    if (c == '+' || c == '-')
        c = getch();
    for (*pn = 0; isdigit(c); c = getch())
        *pn = 10 * *pn + (c - '0');
    *pn *= sign;
    if (c != EOF)
        ungetch(c);
    return c;
}

/** 
 * getch: Gets the next character from the input and returns it. If there are any
 *   characters in the input buffer buff, uses those, first.
 *
 * Returns: The next character, or EOF for end of file.
 */
char getch() {
    return (pbuff > 0) ? buff[--pbuff] : getchar();
}

/**
 * ungetch: If the buffer isn't full, puts a char into the input buffer buff.
 */
void ungetch(char c) {
    if (pbuff < BUFFERSIZE)
        buff[pbuff++] = c;
    else
        printf("ERROR: Input buffer is full!\n");
}