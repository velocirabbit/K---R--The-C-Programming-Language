/* 01E14.c (p. 24) */
#include <stdio.h>

#define NUMDIGITS 10
#define NUMLETTERS 26

main() {
    int c;
    int digits[NUMDIGITS], letters[NUMLETTERS];

    for (int i = 0; i < NUMDIGITS; i++)
        digits[i] = 0;
    for (int i = 0; i < NUMLETTERS; i++)
        letters[i] = 0;

    while ((c = getchar()) != '\n') {
        if (c >= '0' && c <= '9')
            digits[c - '0']++;
        else if (c >= 'a' && c <= 'z')
            letters[c - 'a']++;
        else if (c >= 'A' && c <= 'Z')
            letters[c - 'A']++;
    }

    printf("\n\n");
    for (int i = 0; i < NUMDIGITS; i++) {
        printf("%c\t| ", '0' + i);
        for (int j = 0; j < digits[i]; j++)
            printf("x");
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < NUMLETTERS; i++) {
        printf("%c\t| ", 'A' + i);
        for (int j = 0; j < letters[i]; j++)
            printf("x");
        printf("\n");
    }

    printf("\n");
    system("pause");
    return 0;
}