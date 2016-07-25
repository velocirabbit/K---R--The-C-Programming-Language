/* 02E08.c (p. 49) */
#include <stdio.h>
#include <limits.h>

int rightrot(int x, int n);
void printBinary(int x);

main() {
    int x = 13825734;
    int xInv = rightrot(x, 17);

    printf("x = %d, xInv = %d\n", x, xInv);
    printf("\tx = \t");
    printBinary(x);
    printf("\txInv = \t");
    printBinary(xInv);

    printf("\n");
    system("pause");
    return 0;
}

/*
 * rightrot: Returns the value of integer x rotate to the right by n bit positions.
 */
int rightrot(int x, int n) {
    int right = x & ~(~0 << n), len = sizeof(x) * CHAR_BIT;
    x = (x >> n) & ~(~0 << (len - n));
    return x | (right << (len - n));
}

void printBinary(int x) {
    int len = sizeof(x) * CHAR_BIT, c = 0;
    while (x != 0) {
        if (x & (1 << (len - 1)))
            printf("1");
        else
            printf("0");
        x = x << 1;
        c++;
    }
    for (c; c < len; c++)
        printf("0");
    printf("\n");
}