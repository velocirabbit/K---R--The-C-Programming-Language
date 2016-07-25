/* 01L05_04.c (p. 19) */
#include <stdio.h>

main() {
    int c, nl;

    nl = 0;
    while ((c = getchar()) != EOF)
        if (c == '\n')
            ++nl;
    printf("%d\n", nl);

    printf("\n");
    system("pause");
    return 0;
}