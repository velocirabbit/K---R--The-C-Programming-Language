/* 04E12.c (p. 88) */
#include <stdio.h>

#define MAXSTRLEN 1000

int itoa(char s[], int n, int p);

main() {
    char s[MAXSTRLEN];
    int arr[] = { 392, -4829, 34, -1093, 23894, -1039, 32, -23092, 763489, -48 };
    system("pause");
    for (int i = 0; i < 10; i++) {
        itoa(s, arr[i], 0);
        printf("%s\n", s);
    }

    printf("\n");
    system("pause");
    return 0;
}

/*
 * itoa: Recursive version of itoa as an exercise. Stores the string in s[]
 */
int itoa(char s[], int n, int p) {
    int sign = (n < 0) ? -1 : 1;
    if (n < 0)
        s[p++] = '-';

    if (n / 10)
        p = itoa(s, sign * n / 10, p);
    if (p < MAXSTRLEN - 1)
        s[p + 1] = 0;
    s[p] = ((sign * n) % 10) + '0';
    return p + 1;
}