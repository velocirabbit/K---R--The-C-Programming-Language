/* 01E06.c (p. 17) */
#include <stdio.h>

main() {
    int c;

    while (c = getchar() != EOF) {
        printf("%d\n", c);
    }

    printf("\n");
    system("pause");
    return 0;
}