/* 03E05.c (p. 64) */
#include <stdio.h>
#include <string.h>
#define EXPARRSIZE 8
#define MAXSTRLEN 1000

enum { A = 10, B, C, D, E, F, G, H, I, J = 19,
       K, L, M, N, O, P, Q, R, S, T = 29,
       U, V, W, X, Y, Z = 35};

int pow(int n, int pow);
void itob(int n, char s[], int b);

main() {
    int ns[] = { 249, -2390, 1374, 5928, -41049, 23, 538, -4 };
    char strs[EXPARRSIZE][MAXSTRLEN];

    for (int b = 2; b < 20; b++) {
        for (int s = 0; s < EXPARRSIZE; s++) {
            itob(ns[s], strs[s], b);
            printf("%d in base %d = %s\n", ns[s], b, strs[s]);
        }
        printf("\n");
    }

    printf("\n");
    system("pause");
    return 0;
}

/*
 * pow: Returns int n raised to the pow power.
 */
int pow(int n, int pow) {
    int p = 1;
    for (int i = 0; i < pow; i++)
        p *= n;
    return p;
}

/* 
 * itob: Converts base-10 integer n into a base b character representation in the string s. If
 *       anything goes wrong, nothing will be stored in s. 
 */
void itob(int n, char s[], int b) {
    if (b > 36)
        return;
    // i -> counts position in s; c -> keeps track of value in b;
    int i = 0, c = 0, p, sign;
    if (n < 0) {
        sign = -1;
        s[i++] = '-';
    }
    else
        sign = 1;

    // p is the number of positions value n will have in base b
    for (p = 1; pow(b, p) <= sign * n; p++);

    for (int j = p - 1; j >= 0; j--) {
        int m;
        for (m = 1; m * pow(b, j) + c <= sign * n; m++);
        m--;
        if (m < 10)
            s[i++] = m + '0';
        else
            s[i++] = m - 10 + 'A';
        c += m * pow(b, j);
    }

    s[i] = '\0';
}