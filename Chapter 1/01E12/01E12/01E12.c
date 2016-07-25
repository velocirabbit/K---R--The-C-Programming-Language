/* 01E12.c (p. 21) */
#include <stdio.h>

#define IN 1
#define OUT 0

main() {
    int c, state;

    state = OUT;
    while ((c = getchar()) != '\n') {
        if (c == ' ' || c == '\n' || c == '\t') {
            state = OUT;
            putchar('\n');
        }
        else if (state == OUT) {
            state = IN;
            printf("%c", c);

        }
        else if (state == IN)
            putchar(c);
    }

    printf("\n\n");
    system("pause");
    return 0;
}