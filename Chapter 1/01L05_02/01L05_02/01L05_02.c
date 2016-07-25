/* 01L05_02.c (p. 18) */
#include <stdio.h>

main() {
    long nc;

    nc = 0;
    while (getchar() != EOF)
        ++nc;
    printf("%ld\n", nc);

    printf("\n");
    system("pause");
    return 0;
}