/* 01L07.c (p. 24) */
#include <stdio.h>

int power(int m, int n);

main() {
    int i;

    for (i = 0; i < 10; ++i)
        printf("%d %d %d\n", i, power(2, i), power(-3, i));

    printf("\n");
    system("pause");
    return 0;
}

/* power: raise base to n-th power: n >= 0 */
int power(int base, int n) {
    int i, p;

    p = 1;
    for (i = 1; i <= n; ++i)
        p = p * base;
    return p;
}