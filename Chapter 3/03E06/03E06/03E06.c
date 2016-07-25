/* 03E06.c (p. 64) */
#include <stdio.h>
#include <string.h>

#define MAXSTRLEN 1000
#define EXAMPLECOUNT 8

void itoa(int n, char s[], int pad);
void reverse(char s[]);

main() {
    int ns[] = { 3, -283, 4829, -14891, 482939, -248940, 4819304, -384918 };
    char strs[EXAMPLECOUNT][MAXSTRLEN];

    for (int i = 0; i < EXAMPLECOUNT; i++) {
        itoa(ns[i], strs[i], 16);
        printf("%d to string: %s\n", ns[i], strs[i]);
    }
    printf("\n");
    system("pause");
    return 0;
}

void itoa(int n, char s[], int pad) {
    int i = 0, sign = n < 0 ? -1 : 1;
    do {
        s[i++] = sign * (n % 10) + '0';
    } while (sign * (n /= 10) > 0);
    if (sign < 0)
        s[i++] = '-';
    for (i; i < pad; i++)
        s[i] = ' ';
    s[i] = '\0';
    reverse(s);
}

void reverse(char s[]) {
    for (int i = 0; i < strlen(s) / 2; i++) {
        char c = s[i];
        s[i] = s[strlen(s) - i - 1];
        s[strlen(s) - i - 1] = c;
    }
}