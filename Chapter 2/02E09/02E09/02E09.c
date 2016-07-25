/* 02E09.c (p. 51) */
#include <stdio.h>
#include <limits.h>

int bitcount(unsigned x);
void printBinary(unsigned x);

main() {
    unsigned x = 2893429;
    printf("%d has %d 1-bits in its binary representation.\n", x, bitcount(x));
    printf("%d in binary is ", x);
    printBinary(x);

    printf("\n");
    system("pause");
    return 0;
}

/*
 * bitcount: Counts the number of 1-bits in x.
 * In a two's complement number system (binary), the digits in a number are either 0 or 1.
 * When performing the operation: x &= (x - 1), the rigthmost 1-bit in x is deleted because
 * subtracting 1 from x makes all digits up to the second rightmost 1-bit the opposite of what
 * they originally were. Thus, taking the bitwise & operator changes the rightmost 1-bit to a 0.
 */
int bitcount(unsigned x) {
    int b;
    for (b = 0; x != 0; x &= (x - 1), b++);
    return b;
}

void printBinary(unsigned x) {
    int len = sizeof(x) * CHAR_BIT, c = 0;
    while (x != 0) {
        if (x & (1 << (len - 1)))
            printf("1");
        else
            printf("0");
        x <<= 1;
        c++;
    }
    for (c; c < len; c++)
        printf("0");
    printf("\n");
}