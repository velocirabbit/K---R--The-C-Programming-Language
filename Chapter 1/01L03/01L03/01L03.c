/* 01L03.c: Print Fahrenheit-Celsius Table with a For Statement (p. 13) */
#include <stdio.h>

main() {
    int fahr;

    for (fahr = 0; fahr <= 300; fahr = fahr + 20)
        printf("%3d %6.1f\n", fahr, (5.0 / 9.0) * (fahr - 32));

    printf("\n");
    system("pause");
    return 0;
}