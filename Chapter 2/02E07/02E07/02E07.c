/* 02E07.c (p. 49) */
#include <stdio.h>
#include <limits.h>

unsigned invert(unsigned x, int p, int n);
void printBinary(unsigned x);

main() {
    unsigned char uc = 'a';
    unsigned short us = 2830;
    unsigned int ui = 58204;
    unsigned long ul = 920573;

    // unsigned char
    printf("uc = %c, invert(uc, 5, 3) = %c\n", uc, invert(uc, 5, 3));
    printf("\tuc = ");
    printBinary(uc);
    printf("\tinvert(uc, 5, 3) = ");
    printBinary(invert(uc, 5, 3));
    printf("\n");

    // unsigned short
    printf("us = %d, invert(us, 8, 4) = %d\n", us, invert(us, 8, 4));
    printf("\tus = ");
    printBinary(us);
    printf("\tinvert(us, 8, 4) = ");
    printBinary(invert(us, 8, 4));
    printf("\n");

    //unsigned int
    printf("ui = %d, invert(ui, 13, 8) = %d\n", ui, invert(ui, 13, 8));
    printf("\tui = ");
    printBinary(ui);
    printf("\tinvert(ui, 13, 8) = ");
    printBinary(invert(ui, 13, 8));
    printf("\n");

    //unsigned long
    printf("ul = %d, invert(ul, 20, 10) = %d\n", ul, invert(ul, 20, 10));
    printf("\tul = ");
    printBinary(ul);
    printf("\tinvert(ul, 20, 10) = ");
    printBinary(invert(ul, 20, 10));
    printf("\n");

    printf("\n");
    system("pause");
    return 0;
}

/*
 * invert: Returns x with the n bits that begin at position p inverted (i.e., 1 changed
 *         into 0 and vice versa), leaving the others unchanged.
 */
unsigned invert(unsigned x, int p, int n) {
    return (x & ~(~(~0 << n) << (p - n))) | (~x & (~(~0 << n) << (p - n)));
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