/* 01E09.c (p. 20) */
#include <stdio.h>

main() {
    int blanks = 0;
    for (int c = getchar(); c != '\n'; c = getchar()) {
        if (c == ' ')
            ++blanks;
        else if (blanks == 0)
            putchar(c);
        else {
            putchar(' ');
            putchar(c);
            blanks = 0;
        }
    }

    printf("\n\n");
    system("pause");
    return 0;
}