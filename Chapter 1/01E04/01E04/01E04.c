/* 01E04.c (p. 13) */
#include <stdio.h>

main() {
    float celsius, fahr;
    int lower, upper, step;

    lower = 0;
    upper = 100;
    step = 10;

    celsius = lower;
    printf("Celsius\tFahrenheit\n");
    while (celsius <= upper) {
        fahr = (celsius * 9.0 / 5.0) + 32.0;
        printf("  %3.0f  \t  %6.1f\n", celsius, fahr);
        celsius = celsius + step;
    }

    printf("\n");
    system("pause");
    return 0;
}