/* 01L05.c (p. 16) */
#include <stdio.h>

main() {
    int c;

    c = getchar();
    while (c != EOF) {
        putchar(c);
        c = getchar();
    }

    printf("\n");
    system("pause");
    return 0;
}