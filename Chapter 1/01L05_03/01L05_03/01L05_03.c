/* 01L05_03.c (p. 18) */
#include <stdio.h>

main() {
    double nc;
    for (nc = 0; getchar() != EOF; ++nc);  // empty body - all work done in the loop definition
    printf("%.0f\n", nc);

    printf("\n");
    system("pause");
    return 0;
}