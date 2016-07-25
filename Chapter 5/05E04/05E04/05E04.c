/* 05E04.c (p. 107) */
#include <stdio.h>
#include <string.h>

typedef enum { False = 0, True } bool;

bool strend(char s[], char t[]);

main() {
    char s1[] = "This is the first test string.";
    char t1[] = "test string.";
    char s2[] = "Oogly boogly boo!";
    char t2[] = "blah blah";
    printf("First comparison: ");
    if (strend(s1, t1))
        printf("True");
    else
        printf("False");
    printf("\nSecond comparison: ");
    if (strend(s2, t2))
        printf("True");
    else
        printf("False");
    printf("\n\n");
    system("pause");
    return 0;
}

/**
 * strend: Returns True if the string t occurs at the end of the string s, and False
 *   otherwise.
 */
bool strend(char s[], char t[]) {
    int tLen = strlen(t);
    int sLen = strlen(s);
    int i;
    if (tLen > sLen)
        return False;
    for (i = 0; i < tLen && s[sLen - tLen + i] == t[i]; i++);
    if (s[sLen - tLen + i] == 0 && t[i] == 0)
        return True;
    return False;
}