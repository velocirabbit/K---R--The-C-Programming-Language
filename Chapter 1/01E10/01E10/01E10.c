/* 01E10.c (p. 20) */
#include <stdio.h>

main() {
    for (int c = getchar(); c != EOF; c = getchar()) {
        if (c == '\t')
            printf("\\t");
        else if (c == '\b')
            printf("\\b");
        else if (c == '\\')
            printf("\\\\");
        else
            putchar(c);
    }

    printf("\n\n");
    system("pause");
    return 0;
}