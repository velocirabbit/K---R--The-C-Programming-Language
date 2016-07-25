/* 02E06.c (p. 49) */
#include <stdio.h>
#include <limits.h>

unsigned setbits(unsigned x, int p, int n, unsigned y);
void printBinary(unsigned x);

main() {
    unsigned char uc1 = 'a', uc2 = 'z';
    unsigned short us1 = 289, us2 = 892;
    unsigned int ui1 = 1938, ui2 = 8928;
    unsigned long ul1 = 82925, ul2 = 82472;

    // printing unsigned chars
    printf("uc1 = %c, uc2 = %c, setbits(uc1, 4, 2, uc2) = %c\n", uc1, uc2, setbits(uc1, 4, 2, uc2));
    printf("\tuc1 = ");
    printBinary(uc1);
    printf("\tuc2 = ");
    printBinary(uc2);
    printf("\tsetbits(uc1, 4, 2, uc2) = ");
    printBinary(setbits(uc1, 4, 2, uc2));
    printf("\n");

    // printing unsigned shorts
    printf("us1 = %d, us2 = %d, setbits(us1, 8, 4, us2) = %d\n", us1, us2, setbits(us1, 8, 4, us2));
    printf("\tus1 = ");
    printBinary(us1);
    printf("\tus2 = ");
    printBinary(us2);
    printf("\tsetbits(us1, 8, 4, us2) = ");
    printBinary(setbits(us1, 8, 4, us2));
    printf("\n");

    //printing unsigned ints
    printf("ui1 = %d, ui2 = %d, setbits(ui1, 9, 7, ui2) = %d\n", ui1, ui2, setbits(ui1, 9, 7, ui2));
    printf("\tui1 = ");
    printBinary(ui1);
    printf("\tui2 = ");
    printBinary(ui2);
    printf("\tsetbits(ui1, 9, 7, ui2) = ");
    printBinary(setbits(ui1, 9, 7, ui2));
    printf("\n");

    //printing unsigned longs
    printf("ul1 = %d, ul2 = %d, setbits(ul1, 14, 8, ul2) = %d\n", ul1, ul2, setbits(ul1, 14, 8, ul2));
    printf("\tul1 = ");
    printBinary(ul1);
    printf("\tul2 = ");
    printBinary(ul2);
    printf("\tsetbits(ul1, 14, 8, ul2) = ");
    printBinary(setbits(ul1, 14, 8, ul2));
    printf("\n");

    printf("\n");
    system("pause");
    return 0;
}

/*
 * setbits: Returns x with the n bits that begin at position p set to the
 * rightmost n bits of y, leaving the other bits unchanged
 */
unsigned setbits(unsigned x, int p, int n, unsigned y) {
    x = x & ~(~(~0 << n) << (p - n));
    y = y & (~(~0 << n) << (p - n));
    return x | y;
}

void printBinary(unsigned x) {
    int len = sizeof(x) * CHAR_BIT, c = 0;
    while (x != 0) {
        if (x & (1 << (len - 1)))
            printf("1");
        else
            printf("0");
        x = x << 1;
        c++;
    }
    for (c; c <= len; c++)
        printf("0");
    printf("\n");
}