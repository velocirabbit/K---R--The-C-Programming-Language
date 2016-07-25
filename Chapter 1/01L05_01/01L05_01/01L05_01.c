/* 01L05_01.c (p. 17) */
#include <stdio.h>

main() {
    int c;

    while ((c = getchar()) != EOF)
        putchar(c);

    printf("\n");
    system("pause");
    return 0;
}