/* 02E10.c (p. 52) */
#include <stdio.h>

int lower(int c);

main() {
    char str[] = "This is a test string named TESTIE, written by Andrew Wang.";
    for (int i = 0; str[i] != '\0'; i++)
        printf("%c", lower(str[i]));
    printf("\n\n");
    system("pause");
    return 0;
}

/*
 * lower: Converts c to lower case. If it isn't an upper case letter,
 *        it's returned unchanged.
 */
int lower(int c) {
    return (c >= 'A' && c <= 'Z') ? (c + 'a' - 'A') : c;
}