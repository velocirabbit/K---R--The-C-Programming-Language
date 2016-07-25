/* 04E14.c (p. 91) */
#include <stdio.h>
#define swap(t, x, y) {t temp = x; x = y; y = temp;}

main() {
    int var1 = 93, var2 = 75;
    printf("Before swap:\n");
    printf("\tvar1 = %d, var2 = %d\n\n", var1, var2);
    swap(int, var1, var2);
    printf("After swap:\n");
    printf("\tvar1 = %d, var2 = %d\n\n", var1, var2);

    printf("\n");
    system("pause");
    return 0;
}