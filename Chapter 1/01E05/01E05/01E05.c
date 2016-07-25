/* 01E05.c (p. 14) */
#include <stdio.h>

main() {
    for (int fahr = 300; fahr >= 0; fahr = fahr - 20)
        printf("%3d %6.1f\n", fahr, (5.0 / 9.0) * (fahr - 32.0));

    printf("\n");
    system("pause");
    return 0;
}