/* 01E15.c (p. 27) */
#include <stdio.h>

float tempF2C(int fahr);

main() {
    int fahr, celsius;
    int lower, upper, step;

    lower = 0;
    upper = 300;
    step = 20;

    for (fahr = lower; fahr <= upper; fahr = fahr + step) {
        printf("%d\t%6.1f\n", fahr, tempF2C(fahr));
    }

    printf("\n");
    system("pause");
    return 0;
}

float tempF2C(int fahr) {
    return (5.0 / 9.0) * (fahr - 32.0);
}