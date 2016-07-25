/* 01L02_01.c: Print Fahrenheit-Celsius (floating-point version) (p. 12) */
#include <stdio.h>

main() {
    float fahr, celsius;
    int lower, upper, step;

    lower = 0;  // lower limit of temperature table
    upper = 300;  // upper limit
    step = 20;  // step size

    fahr = lower;
    while (fahr <= upper) {  // comparison operators convert ints to float if either are a float
        celsius = (5.0 / 9.0) * (fahr - 32.0);  // a decimal point in a number says to use floating-point
        printf("%3.0f %6.1f\n", fahr, celsius);  // floating-point string formatting
        fahr = fahr + step;
    }

    printf("\n");
    system("pause");
    return 0;
}